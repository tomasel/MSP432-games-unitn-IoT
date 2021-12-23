#include "libraries/init.h"

void Stack(void){
    Graphics_clearDisplay(&ctx);
    Graphics_drawStringCentered(&ctx,"STACK",-1, 64, 64, true);
    while (1);
}
