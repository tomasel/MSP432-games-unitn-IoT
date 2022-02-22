#include "graphic.h"
#include "stdio.h"

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

/*
The following functions for drawing triangles were never used in the final, but they do work
so i decided to keep them in, also here's a steam train:

                  @~~~@~@~~@~~~@~
     ~~@~@~~~@@@@~~@~@~@@@~@~@
   ~@@~~@@@~@~@~@@~@~        
   @~-@@~@~          _______
   \   /             | /-\ |
    | |              |\\_/ |
  /-| |-----/^\------|--|--|    |----------/+|
 |================== |     |    |            |
 |                   | 232 |    |  MSPGames  |
  \---==/--\__/--\___|_____|_D-C|____________|
 /|--- |====XX====|  /-\ /-\      /-\   /-\ 
/_;     \__/  \__/   \-/ \-/      \-/   \-/
*/

void Graphics_drawTriangle(const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    Graphics_drawLine(context, x0, y0, x1, y1);
    Graphics_drawLine(context, x1, y1, x2, y2);
    Graphics_drawLine(context, x0, y0, x2, y2);
}


void Graphics_fillTriangle(const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){ 
    uint8_t temp;

    if(y1 < y0){        //sort by y
        temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    if(y2 < y0){
        temp = x0;
        x0 = x2;
        x2 = temp;
        temp = y0;
        y0 = y2;
        y2 = temp;
    }
    if(y2 < y1){
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    //calculate line between top and bottom point
    float mainSlope   = (float)(x2-x0)/(y2-y0);  //slope between top and bottom most points
    float topSlope    = (float)(x1-x0)/(y1-y0);
    float bottomSlope = (float)(x2-x1)/(y2-y1);

    uint8_t i;
    uint8_t x;
    for (i=y0; i<y2; i++){ //for every y-level
        if (i<y1){
            x = topSlope*(i-y0)+x0;          //if we are above the middle point use the first
        } else {
            x = bottomSlope*(i-y1)+x1;
        }
        Graphics_drawLineH(context, mainSlope*(i-y0)+x0, x , i);
    }


}
