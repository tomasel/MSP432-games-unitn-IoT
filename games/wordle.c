#include "libraries/init.h"
//#include "assets/wordle.h"

void Wordle (void) {
    Graphics_clearDisplay(&ctx);
    Graphics_drawStringCentered(&ctx,"Wordle",-1, 64, 64, true);
    while (1);
}
