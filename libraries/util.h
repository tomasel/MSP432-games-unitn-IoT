#ifndef _UTIL_H_
#define _UTIL_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>

//uint32_t rand();

void _hwInit();


void schedule(uint32_t ms, void (*func)(void));
void s_sprintf(int8_t *str, const char *format, ... );
void wait(uint32_t ms);
uint32_t millis();
#endif 
