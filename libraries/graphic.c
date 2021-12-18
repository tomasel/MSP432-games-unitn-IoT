#include "graphic.h"
#include "stdio.h"

#define GFX_BUFFER_SIZE 32
static int8_t txt_buffer[GFX_BUFFER_SIZE];


void _graphicsInit(){
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&ctx, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&ctx, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&ctx, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&ctx, &g_sFontFixed6x8);
    Graphics_clearDisplay(&ctx);
}



void Graphics_drawArc(const Graphics_Context *context, uint8_t x, uint8_t y, uint8_t radius, uint8_t start, uint8_t end){

 }


void Graphics_drawTriangle(const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

}

//test which is the fastest draw pixel
