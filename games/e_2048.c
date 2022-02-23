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
void e_drawTable (void);
void e_random2 (void);
bool e_endgame (void);
bool e_moveLEFT ();
void e_redrawTable ();
void e_drawNum (uint8_t x, uint8_t y);
void e_rotateBoard ();


// main
void e_2048 () {
    srand(millis());
    bool draw;
    while (1) {

        memset(e_grid,0,16); //clear table
        e_score=0;

        e_random2();
        e_random2();

        e_drawTable();
        do {
            draw=false;
            uint16_t butts = getButtons();
            if (butts & JOYSTICK_LEFT) {
                draw=e_moveLEFT();
            }
            if (butts & JOYSTICK_UP) {
                e_rotateBoard();
                draw=e_moveLEFT();
                e_rotateBoard();
                e_rotateBoard();
                e_rotateBoard();
            }
            if (butts & JOYSTICK_RIGHT) {
                e_rotateBoard();
                e_rotateBoard();
                draw=e_moveLEFT();
                e_rotateBoard();
                e_rotateBoard();
            }
            if (butts & JOYSTICK_DOWN) {
                e_rotateBoard();
                e_rotateBoard();
                e_rotateBoard();
                draw=e_moveLEFT();
                e_rotateBoard();
            }
            if (draw){
                e_redrawTable();
                if (e_endgame())
                    break; //gameover
            }

            wait(100);
        } while (1);
        wait (10000);
    }
}



void e_drawTable(void) {


    Graphics_clearDisplay(&ctx);
    Graphics_drawImage(&ctx, &logo_2048, 0, 0);

    Graphics_setForegroundColor(&ctx,0);
    Graphics_drawRectangle(&ctx,&e_table);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + 2 * E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOffx,E_screenEnd,E_screenOffy + 3 * E_screenRow);
    Graphics_drawLineV(&ctx,E_screenOffx + E_screenCln,E_screenOffy,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOffx + 2 * E_screenCln,E_screenOffy,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOffx + 3 * E_screenCln,E_screenOffy,E_screenEnd);
    e_redrawTable();
    int8_t str[20];
    s_sprintf(&str, "Score:%9d", e_score);
    Graphics_drawString(&ctx, str, 20, 34, 5, true);
}

void e_random2 (void) {
    uint8_t x, y;
    do {
        x = rand() % 4;
        y = rand() % 4;
    }while (e_grid[y][x] != 0);
    e_grid[y][x] = 1;
}

bool e_endgame (void) {
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (e_grid[i][j]==0)    //if there's empty cells
                return false;
        }
        for (j = 0; j < 3; j++) {
            if ( e_grid[i][j] == e_grid[i][j+1]||
                 e_grid[j][i] == e_grid[j+1][i] );
                return false;       // if a merge is possible

        }
    }
    return true;    //else it's gameover
}

void e_redrawTable (){
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            e_drawNum(j, i);
        }
    }
}

bool e_moveLEFT () {
    uint8_t i, j;
    int8_t k;
    bool success = 0;
    for (i = 0; i < 4; i++) {
            for (j = 1; j < 4; j++) {
                uint8_t tmp = e_grid[i][j];
                if (tmp == 0) continue; //skip if we are at 0;
                for (k=j-1; k>=0; k--){
                    if ( e_grid[i][k] == tmp ){
                        success = 1;
                        tmp++; //increase the value of the cell
                        e_grid[i][k]=0; //clear cell we merged with
                        e_score+=1<<tmp; //add to score 2^tmp since tmp is log2(num)
                        continue;
                    }
                    if (e_grid[i][k] != 0){ //stop once we find a non-zero cell that doesn't cause a merge
                        break;
                    }
                    success = 1;
                }
                k++;
                e_grid[i][j] = 0;   //clear starting cell
                e_grid[i][k] = tmp; //populate new position
            }
        }

    if (success){
        e_random2();
        int8_t str[20];
        s_sprintf(&str, "Score:%9d", e_score);
        Graphics_setForegroundColor(&ctx, 0);
        Graphics_drawString(&ctx, str, 20, 34, 5, true);
    }
    return success;
}
void e_rotateBoard() {
    uint8_t i,j,n=4;
    uint8_t tmp;
    for (i=0; i<n/2; i++) {
        for (j=i; j<n-i-1; j++) {
            tmp = e_grid[i][j];
            e_grid[i][j] = e_grid[j][n-i-1];
            e_grid[j][n-i-1] = e_grid[n-i-1][n-j-1];
            e_grid[n-i-1][n-j-1] = e_grid[n-j-1][i];
            e_grid[n-j-1][i] = tmp;
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

    if (val!=0){
        Graphics_setForegroundColor(&ctx,E_TEXTCLR);
           Graphics_drawStringCentered(&ctx,e_numbers_str[val],4,rect.xMin + E_screenCln / 2,rect.yMin + E_screenRow / 2,false);
    }
   }

