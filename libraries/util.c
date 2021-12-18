#include "util.h"
#include <stdio.h>
#include <stdlib.h>
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



    _soundInit();
}

//takes an integer, a string and a minimum length and appends the integer to the string
//return value is the number of characters written
uint8_t s_appendInteger (int32_t integer, char* string, int8_t length){

    uint8_t l=0;
    if(integer < 0) {
        *string++='-';
        integer=-integer;
        l++;
    }

    char b[10];
    uint32_t digit = integer;
    uint8_t i = 0;
    while( digit > 0 ){         //compute the digits and store them in a temp string
        b[i++] = digit % 10;
        digit /= 10;
    }

    if (integer == 0) {
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

//simple sprintf implementation to save memory
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
                while(*s){
                    *str++=*s++;    //copy each character one after the other
                }
                break;
            case 'd':
                str += s_appendInteger(va_arg(valist, int), str, 0)-1; //subtract 1 because 1 will be added back
                break;
            default:
                if ( *fs>'0' && *fs<='9' && fs[1] == 'd') { //for constant width example: %6d
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
