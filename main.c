#include "libraries/sound.h"
#include "libraries/util.h"
#include "libraries/graphic.h"
#include "libraries/input.h"

#include <string.h>

void main(void)
{
    _hwInit();
    _graphicsInit();
    _inputInit();
    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();


    int8_t string[32];
    while (1){
        uint16_t buttons = getButtons();
        s_sprintf(string,"x%6dy%6d",joystick_x,joystick_y);
        Graphics_drawString(&ctx, string, -1, 10, 10, 1);
        Graphics_drawPixel (&ctx, joystick_x/128, 128-joystick_y/128);
        s_sprintf(string,"%c%c%c%c%c",buttons&BUTTON_1?'1':' ',buttons&BUTTON_2?'2':' ',buttons&BUTTON_A?'A':' ',buttons&BUTTON_B?'B':' ',buttons&BUTTON_J?'J':' ');
        Graphics_drawString(&ctx, string, -1, 10, 20, 1);
    }

    Graphics_fillTriangle(&ctx, 10, 10, 40, 64, 100, 80);
    Graphics_setForegroundColor(&ctx, 0);
    Graphics_drawTriangle(&ctx, 10, 10, 40, 64, 100, 80);
    while(1);
}
