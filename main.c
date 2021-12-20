#include "libraries/sound.h"
#include "libraries/util.h"
#include "libraries/graphic.h"
#include "libraries/input.h"
#include "games/games.h"

#include <stdlib.h>

#define CHAR_HEIGHT 7 //px
#define GAMES_LIST_LINES 6 //character lines
#define GAMES_LIST_SPACING 4 //px
#define GAMES_LIST_WIDTH 77  //px
#define GAMES_LIST_BACKGROUND               GRAPHICS_COLOR_LIGHT_GRAY //color
#define GAMES_LIST_SELECTED_BACKGROUND      0x202020 //color
#define FONT_COLOR                          0x404040
#define FONT_COLOR_SELECTED                 GRAPHICS_COLOR_WHITE

#define DESC_CHAR_LINE 14

uint8_t selection=1;

void drawGameList ();

void main(void) {
    _hwInit();
    _graphicsInit();
    _inputInit();
    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    drawGameList();




    while(1);
}

void drawGameList (){
   uint8_t i;
   Graphics_Rectangle rect = {0,0, GAMES_LIST_WIDTH, GAMES_LIST_LINES*(CHAR_HEIGHT+GAMES_LIST_SPACING)+2*GAMES_LIST_SPACING};
   Graphics_setForegroundColor(&ctx, GAMES_LIST_BACKGROUND);
   Graphics_fillRectangle(&ctx, &rect);
   Graphics_setForegroundColor(&ctx, FONT_COLOR);
   for (i=0; i<GAMES_LIST_LINES; i++){
       Graphics_drawString(&ctx, games[i].name, -1, 7, i*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING, false);
   }

   rect.yMin = selection*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING-1;
   rect.yMax = rect.yMin + CHAR_HEIGHT+1;

   Graphics_setForegroundColor(&ctx, GAMES_LIST_SELECTED_BACKGROUND);
   Graphics_fillRectangle(&ctx, &rect);

   Graphics_setForegroundColor(&ctx, FONT_COLOR_SELECTED);
   Graphics_drawString(&ctx, games[selection].name, -1, 7, selection*(CHAR_HEIGHT+GAMES_LIST_SPACING)+GAMES_LIST_SPACING, false);

   Graphics_setForegroundColor(&ctx, 0);
   Graphics_drawString(&ctx, games[selection].desc, DESC_CHAR_LINE, 3, 93, true);
   Graphics_drawString(&ctx, games[selection].desc+DESC_CHAR_LINE, DESC_CHAR_LINE, 3, 93+CHAR_HEIGHT+GAMES_LIST_SPACING, true);

}
