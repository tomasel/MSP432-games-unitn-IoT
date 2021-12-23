#include "libraries/init.h"

void Snake(void){
    Graphics_clearDisplay(&ctx);
    Graphics_drawStringCentered(&ctx,"SNAKE",-1, 64, 64, true);
    while (1);
}
