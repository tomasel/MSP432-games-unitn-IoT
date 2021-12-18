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



void Graphics_drawArc(const Graphics_Context *context, uint8_t x, uint8_t y, uint8_t radius, uint8_t start, uint8_t end){

 }


fillBottomFlatTriangle( const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    float invslope1 = (x1 - x0) / (y1 - y0);
    float invslope2 = (x2 - x0) / (y2 - y0);

    float curx1 = x0;
    float curx2 = x0;

    int scanlineY;
    for (scanlineY = y0; scanlineY <= y1; scanlineY++){
        Graphics_drawLineH(context, (int)curx1, (int)curx2, scanlineY);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

fillTopFlatTriangle( const Graphics_Context *context, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    float invslope1 = (x2 - x0) / (y2 - y0);
    float invslope2 = (x2 - x1) / (y2 - y1);

    float curx1 = x2;
    float curx2 = x2;

    int scanlineY;
    for (scanlineY = y2; scanlineY > y0; scanlineY--){
        Graphics_drawLineH(context, (int)curx1, (int)curx2, scanlineY);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

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

    if (y1 == y2) {
        fillBottomFlatTriangle(context, x0, y0, x1, y1, x2, y2);
        return;
      }
      /* check for trivial case of top-flat triangle */
      if (y0 == y1){
        fillTopFlatTriangle(context, x0, y0, x1, y1, x2, y2);
        return;
      }
      else  {
        /* general case - split the triangle in a topflat and bottom-flat one */
          uint8_t x4;
          x4 = (int)(x0 + ((float)(y1 - y0) / (float)(y2 - y0)) * (x2 - x1));
        fillBottomFlatTriangle(context, x0, y0, x1, y1, x4, y1);
        fillTopFlatTriangle(context, x1, y1, x4, y1, x2, y2);
      }
}

//test which is the fastest draw pixel
