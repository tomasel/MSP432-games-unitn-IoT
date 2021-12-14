#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


uint32_t ms;

void graphicsInit(void);

void sleep(uint32_t ms);
void schedule(uint32_t ms, void (*func)(void));


void screenClear(COLOR background);
void drawString(const restrict char* str, uint8_t len, uint8_t x, uint8_t y, COLOR foreground,COLOR background);
void drawStringCentered(const restrict char* str, uint8_t len, uint8_t x, uint8_t y, COLOR foreground,COLOR background);
void drawBitamp(uint8_t x, uint8_t y, uint8_t w, uint8_t h, void* image);
void drawPixel(uint8_t x, uint8_t y, COLOR foreground);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t w1, COLOR foreground);
void drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t w1, COLOR foreground);
void fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t w1, COLOR foreground);
void drawCircle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t w1, COLOR foreground);
void fillCircle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t w1, COLOR foreground);
void clear(void);

#endif 
