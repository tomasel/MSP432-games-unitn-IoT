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

    npd nts[] = {{A4, 240},{0, 2000},{A5, 240}

    };
    playSong(3,nts);

    while(1);
}
