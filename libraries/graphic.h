#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>
#include "util.h"

#define SCREEN_MAX_X 128
#define SCREEN_MAX_Y 128

Graphics_Context ctx;

void _graphicsInit();

void Graphics_drawArc(const Graphics_Context *context, uint8_t x, uint8_t y, uint8_t radius, uint8_t start, uint8_t end);
void Graphics_fillTriangle(const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void Graphics_drawTriangle(const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

#endif 
