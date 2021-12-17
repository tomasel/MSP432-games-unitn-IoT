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

uint8_t s_intos (int32_t integer, char* string){

    if(integer == 0) {
        *string='0';
        return 1;
    }
    uint8_t l=0;
    if(integer < 0) {
        *string++='-';
        integer=-integer;
        l++;
    }
    char b[10];
    int digit = integer;
    uint8_t i = 0;
    while( digit > 0 ){         //store the digits reversed in a temp string
        b[i++] = digit % 10;
        digit /= 10;
    }

    while(i) {
        *string++ = '0' + b[i---1]; //reverse string
        l++;
    }
    return l-1;
}

void s_sprintf(char *str, const char *fs, ... ){
    va_list valist;
    va_start(valist, fs);
    char *s;
    while(*fs){
        if(*fs != '%'){
            *str=*fs;   //copy string normally
            str++;
            fs++;
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
                str+=s_intos(va_arg(valist, int),str);
                break;
            }
            str++;
            ++fs;
        }
    }
    *str=NULL; //null terminate string
    va_end(valist);
}
