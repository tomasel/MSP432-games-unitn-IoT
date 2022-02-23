#include "libraries/init.h"
#include "assets/e_2048.h"
#include <string.h>

// table
#define E_screenCln 28
#define E_screenRow 23
#define E_screenOffx 17
#define E_screenOffy 36
#define E_screenEnd SCREEN_MAX_X
Graphics_Rectangle e_table = {E_screenOffx,E_screenOffy,E_screenEnd,E_screenEnd};
uint8_t e_grid[4][4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // contiene i valori che il giocatore può vedere
uint32_t e_score;
// colors
uint32_t e_colors[] = {
                     E_BACK,
                     E_2,
                     E_4,
                     E_8,
                     E_16,
                     E_32,
                     E_64,
                     E_128,
                     E_256,
                     E_512,
                     E_1024,
                     E_2048,
                     ClrBlack};
#define E_TEXTCLR 0xFFFFFF

int8_t* e_numbers_str[] = {"","2","4","8","16","32","64","128","256","512","1024","2048","4096","8192","16K","32K","64K","128K"};

// functions
void e_cls (void);
void e_drawLogo (void);
void e_drawTable (void);
void e_drawScore (void);
void e_random2 (void);
void e_endgame (void);
bool e_valid (uint8_t dir);
void e_move (uint8_t dir);
void e_drawNum (uint8_t x, uint8_t y);
void e_rotateBoard ();

// flags
short e_occu = 0;     // currently occupied square

// main
void e_2048 () {
    srand(millis());

    while (1) {
        memset(e_grid,0,16); //clear table
        e_cls();
        e_drawLogo();
        e_drawTable();

        e_random2();
        e_random2();
        e_random2();
               e_random2(); e_random2();
               e_random2();
        do {
            uint16_t butts = getButtons();
            if (butts & JOYSTICK_LEFT) {
                e_move(3);
                wait(200);
            }
            if (butts & JOYSTICK_UP) {
                e_rotateBoard();
                e_move(3);
                e_rotateBoard();
                e_rotateBoard();
                e_rotateBoard();
            }

        } while (1);
    }
}


// ===
void e_cls(void) {
    Graphics_clearDisplay(&ctx);
}

void e_drawLogo(void) {
    Graphics_drawImage(&ctx, &logo_2048, 0, 0);
}

void e_drawTable(void) {
    Graphics_setForegroundColor(&ctx,E_BACK);
    Graphics_fillRectangle(&ctx,&e_table);
    Graphics_setForegroundColor(&ctx,0);
    Graphics_drawRectangle(&ctx,&e_table);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + 2 * E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + 3 * E_screenRow);
    Graphics_drawLineV(&ctx,E_screenOffx + E_screenCln,E_screenOffy,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOffx + 2 * E_screenCln,E_screenOffy,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOffx + 3 * E_screenCln,E_screenOffy,E_screenEnd);
}

void e_random2 (void) {
    uint8_t x, y;
    do {
        x = rand() % 4;
        y = rand() % 4;
    }while (e_grid[y][x] != 0);
    e_grid[y][x] = 10;
    e_drawNum(x,y);
}

void e_endgame (void) {
    // screen of death
}

/*
 * 0 ^
 * 1 >
 * 2 v
 * 3 <
 */
void e_move (uint8_t dir) {
    uint8_t i, j;
    int8_t k;
    bool success = 0;
    for (i = 0; i < 4; i++) {
            for (j = 1; j < 4; j++) {
                uint8_t tmp = e_grid[i][j];
                if (tmp == 0) continue; //skip if we are at 0;
                for (k=j-1; k>=0; k--){
                    if ( e_grid[i][k] == tmp ){
                        tmp++;
                        e_grid[i][k]=0; //clear cell we merged with
                        e_drawNum(k, i);
                        e_score+=1<<tmp; //add to score 2^tmp since tmp is log2(num)
                        continue;
                    }
                    if (e_grid[i][k] != 0){
                        break;
                    }
                    success = 1;
                }
                k++;
                e_grid[i][j] = 0;
                e_drawNum(j, i);

                e_grid[i][k] = tmp;
                e_drawNum(k, i);
            }
        }

    if (success){
        e_random2();
        int8_t str[20];
        s_sprintf(&str, "Score:%9d",e_score);
        Graphics_setForegroundColor(&ctx, 0);
        Graphics_drawString(&ctx, str, 20, 34, 5, true);
    }


}
void e_rotateBoard() {
    uint8_t i,j;
    uint8_t tmp;
    for (i=0; i<2; i++) {
        for (j=i; j<3-i-1; j++) {
            tmp = e_grid[i][j];
            e_grid[i][j] = e_grid[j][4-i-1];
            e_grid[j][3-i-1] = e_grid[4-i-1][4-j-1];
            e_grid[4-i-1][4-j-1] = e_grid[4-j-1][i];
            e_grid[4-j-1][i] = tmp;
        }
    }
}

void e_drawNum (uint8_t x, uint8_t y) {
    Graphics_Rectangle rect = {.xMin = x * E_screenCln + E_screenOffx + 1,
                               .yMin = y * E_screenRow + E_screenOffy + 1,
                               .xMax = (x + 1) * E_screenCln + E_screenOffx - 1,
                               .yMax = (y + 1) * E_screenRow + E_screenOffy - 1};
    uint8_t val = e_grid[y][x];
    Graphics_setForegroundColor(&ctx,e_colors[val]);
    Graphics_fillRectangle(&ctx,&rect);
    Graphics_setForegroundColor(&ctx,E_TEXTCLR);
    Graphics_drawStringCentered(&ctx,e_numbers_str[val],4,rect.xMin + E_screenCln / 2,rect.yMin + E_screenRow / 2,false);
}

