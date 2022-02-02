#include "util.h"
#include <stdio.h>
#include <stdlib.h>

const Timer_A_ContinuousModeConfig continuousModeConfig = {
        TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,     // ACLK/64 = 1 kHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
        TIMER_A_DO_CLEAR                    // Clear Counter
};

void _hwInit(){
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System*/
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* initializes ACLK to 64 kHz*/
    CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);

    //setup clock for millis and sleep function
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);
    /* Enabling interrupts and going to sleep */
    Interrupt_enableInterrupt(INT_TA2_N);
    /* Starting the Timer_A2 in continuous mode */
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

    _soundInit();
}

//takes an integer, a string and a minimum length and appends the integer to the string
//return value is the number of characters written
uint8_t s_appendInteger (int32_t integer, int8_t *string, int8_t length){

    uint8_t l=0;
    if(integer < 0) {
        *string++='-'; //add minus sign if negative and invert sign
        integer=-integer;
        l++; //increase character count
    }

    char b[10];
    uint32_t digit = integer;
    uint8_t i = 0;
    while( digit > 0 ){         //compute the digits and store them in a temp string
        b[i++] = digit % 10;
        digit /= 10;
    }

    if (integer == 0) {     //if the number is zero add a zero
        b[i++] = 0;
    }

    for (length -= i; length>0; length--){ //print enough spaces to fill
        *string++=' ';
        l++;
    }

    for ( ; i>0; i-- ) {
        *string++ = '0' + b[i-1]; //reverse string and append to main string
        l++;
    }

    return l;
}

//simpler sprintf implementation, in order to save memory (this is like 1/10th the memory of real sprintf())
void s_sprintf(int8_t *str, const char *fs, ... ){
    va_list valist;
    va_start(valist, fs);
    char *s;
    while(*fs){
        if(*fs != '%'){
            *str=*fs;   //copy string normally
        } else {
            switch(*++fs) {
            case '%':
                *str='%';
                break;
            case 'c':
                *str=va_arg(valist, int32_t);   //add the character
                break;
            case 's':
                s = va_arg(valist, char*);
                while(*s){  //until we find NULL on the current string
                    *str++=*s++;    //copy each character on the result
                }
                break;
            case 'd':
                str += s_appendInteger(va_arg(valist, int), str, 0)-1; //subtract 1 because 1 will be added back
                break;
            default:
                if ( *fs>'0' && *fs<='9' && fs[1] == 'd') { //for constant width integer, like for example: %6d
                    str += s_appendInteger(va_arg(valist, int), str, *fs-'0')-1;
                    fs++; //increment format string because it's 2 characters long
                }
            }
        }
        str++;  //increment string to next character
        fs++;   //increment format string
    }
    *str=NULL; //null terminate string
    va_end(valist);
}


uint16_t millisHI = 0;                      //high 16 bits of millisecond counter
uint32_t millis(){                          //concurrency problem and freezes every once in a while
    uint16_t _millisHI, _millisLO;
    do {
        _millisHI = millisHI;
        _millisLO = Timer_A_getCounterValue(TIMER_A2_BASE);
    } while (_millisHI != millisHI);             //if timer overflow is triggered after the values are stored, reupdate everything
                                                 //this should prevent cases where the counter is behind by 2^16 
    return _millisHI<<16|_millisLO;                  //add the high bits to make a full 32 bit
}

void TA2_N_IRQHandler(void){
    Timer_A_clearInterruptFlag(TIMER_A2_BASE);
    millisHI++;
}

void wait(uint32_t ms){
    uint32_t start = millis();
    while(start + ms > millis());
}
