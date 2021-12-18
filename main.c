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

    char string[32];
    s_sprintf(string,"%d",-1024);
    Graphics_drawString(&ctx, string, -1, 10, 10, 1);

    while(1);
}
