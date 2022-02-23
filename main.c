#include "libraries/sound.h"
#include "libraries/util.h"
#include "libraries/graphic.h"
#include "libraries/input.h"
#include "games/games.h"

#include <stdlib.h>

#define CHAR_HEIGHT 7 //px
#define GAMES_LIST_LINES NUMGAMES //character lines
#define GAMES_LIST_SPACING 4 //px
#define GAMES_LIST_WIDTH 77  //px
#define GAMES_LIST_BACKGROUND               GRAPHICS_COLOR_LIGHT_GRAY //color
#define GAMES_LIST_SELECTED_BACKGROUND      0x202020 //color
#define FONT_COLOR                          0x404040
#define FONT_COLOR_SELECTED                 GRAPHICS_COLOR_WHITE

#define DESC_CHAR_LINE 20

int8_t selection=0;

void drawGameList ();

void main(void) {
    _hwInit();
    _graphicsInit();
    _inputInit();
    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    drawGameList();
    Graphics_drawImage(&ctx, &Pressbutton4BPP_UNCOMP,1, 52);
    bool held = 0;
    while(1){
        uint16_t buttons = getButtons();

        if (buttons&(JOYSTICK_DOWN|JOYSTICK_UP)){
            if(!held){
                if (buttons&JOYSTICK_DOWN && selection<NUMGAMES-1) {
                    selection ++;
                } else if (buttons&JOYSTICK_UP && selection>0){
                    selection --;
                }
                drawGameList();
                held=true;
            }
        } else {
            held=false;
        }
        if (buttons&(BUTTON_A|BUTTON_B)){
            (*games[selection].start)();
        }
    }
}

void drawGameList (){
   uint8_t i;
   Graphics_Rectangle rect = {0,0, GAMES_LIST_WIDTH, GAMES_LIST_LINES*(CHAR_HEIGHT+GAMES_LIST_SPACING)+2*GAMES_LIST_SPACING};
   Graphics_setForegroundColor(&ctx, GAMES_LIST_BACKGROUND);
   Graphics_fillRectangle(&ctx, &rect);
   Graphics_setForegroundColor(&ctx, FONT_COLOR);

   for (i=0; i<NUMGAMES; i++){
       Graphics_drawString(&ctx, games[i].name, -1, 7, i*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING, false);
   }

   rect.yMin = selection*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING-1;
   rect.yMax = rect.yMin + CHAR_HEIGHT+1;

   Graphics_setForegroundColor(&ctx, GAMES_LIST_SELECTED_BACKGROUND);
   Graphics_fillRectangle(&ctx, &rect);


   Graphics_setForegroundColor(&ctx, FONT_COLOR_SELECTED);
   Graphics_drawString(&ctx, games[selection].name, -1, 7, selection*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING, false);


   //draw text on the bottom
   Graphics_setForegroundColor(&ctx, 0);
   Graphics_drawString(&ctx, games[selection].desc, DESC_CHAR_LINE, 3, 95, true);
   Graphics_drawString(&ctx, games[selection].desc+DESC_CHAR_LINE, DESC_CHAR_LINE, 3, 95+CHAR_HEIGHT+GAMES_LIST_SPACING, true);


   //draw side image
   Graphics_drawImage(&ctx, games[selection].img, GAMES_LIST_WIDTH+1, 0);

}
