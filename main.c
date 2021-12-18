#include "libraries/sound.h"
#include "libraries/util.h"
#include "libraries/graphic.h"

#include <string.h>

void main(void)
{
    _hwInit();
    _graphicsInit();

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    npd nts[] = {{A4, 240},{0, 20000},{A5, 240}

    };
    //playSong(3,nts);

    //int8_t string[32];
    //s_sprintf(string,"test%dtest",1234);
    //Graphics_drawString(&ctx, string, -1, 10, 10, 1);
    Graphics_fillTriangle(&ctx, 10, 10, 40, 64, 100, 80);
    Graphics_setForegroundColor(&ctx, 0);
    Graphics_drawTriangle(&ctx, 10, 10, 40, 64, 100, 80);
    while(1);
}
