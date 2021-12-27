#ifndef _UTIL_H_
#define _UTIL_H_

#include "init.h"

//uint32_t rand();

void _hwInit();


void schedule(uint32_t ms, void (*func)(void));
void s_sprintf(int8_t *str, const char *format, ... );
void wait(uint32_t ms);
uint32_t millis();
#endif 
