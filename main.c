#include "libraries/sound.h"
#include "libraries/util.h"
#include "libraries/graphic.h"
#include "libraries/input.h"

#include <string.h>
#include <stdlib.h>

void main(void)
{
    _hwInit();
    _graphicsInit();
    _inputInit();
    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();



    while (1){
        Graphics_fillTriangle (&ctx, rand()%128,rand()%128, rand()%128,rand()%128,  rand()%128,rand()%128);
        Graphics_setForegroundColor(&ctx, rand());
        while (!(getButtons()&BUTTON_A));
    }



    while(1);
}
