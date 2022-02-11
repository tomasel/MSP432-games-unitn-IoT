#include "libraries/init.h"
#include "assets/e_2048.h"
#include <string.h>

// table
#define E_screenCln 24
#define E_screenRow 24
#define E_screenOff 32
#define E_screenEnd SCREEN_MAX_X
Graphics_Rectangle e_table = {E_screenOff,E_screenOff,E_screenEnd,E_screenEnd};
int arr_table[4][4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // contiene i valori che il giocatore può vedere

// colors
uint32_t colors[] = {E_2,
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

char* e_numbers_str[] = {"2","4","8","16","32","64","128","256","512","1024","2048","4096","8192","16K","32K","64K","128K"};

// functions
void e_cls (void);
void e_drawLogo (void);
void e_drawTable (void);
void e_drawScore (void);
void e_random2 (void);
void e_endgame (void);

// flags
short e_occu = 0;     // currently occupied square

// main
void e_2048 () {
    srand(millis());
    e_cls();
    e_drawLogo();
    e_drawTable();
    while (1) {
        // genera nuovo 2
        e_random2();
        // aspettare input

        // aggiornare stato
        if (e_occu == 16) {
            e_endgame();
        } else {
            // ...
        }
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
    Graphics_drawRectangle(&ctx,&e_table);
    /*
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - 3 * E_Tbl_row);
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - 2 * E_Tbl_row);
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - E_Tbl_row);
    Graphics_drawLineV(&ctx,E_Tbl_end - E_Tbl_row,E_Tbl_origin,E_Tbl_end);
    Graphics_drawLineV(&ctx,E_Tbl_end - 2 * E_Tbl_row,E_Tbl_origin,E_Tbl_end);
    Graphics_drawLineV(&ctx,E_Tbl_end - 3 * E_Tbl_row,E_Tbl_origin,E_Tbl_end);
    */
    Graphics_drawLineH(&ctx,E_screenOff,E_screenEnd,E_screenOff + E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOff,E_screenEnd,E_screenOff + 2 * E_screenRow);
    Graphics_drawLineH(&ctx,E_screenOff,E_screenEnd,E_screenOff + 3 * E_screenRow);
    Graphics_drawLineV(&ctx,E_screenOff + E_screenCln,E_screenOff,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOff + 2 * E_screenCln,E_screenOff,E_screenEnd);
    Graphics_drawLineV(&ctx,E_screenOff + 3 * E_screenCln,E_screenOff,E_screenEnd);
}

void e_random2 (void) {
    int x, y;
    if (e_occu < 16) {
        while (1) {
            x = rand() % 4;
            y = rand() % 4;
            if (arr_table[x][y] == 0) {
                Graphics_drawImage(&ctx,
                                   &e_2_img,
                                   x * E_screenCln + E_screenOff + 1,
                                   y * E_screenRow + E_screenOff + 1);
                arr_table[x][y] = 2;
                ++e_occu;
                return;
            }
        }
    }
}

void e_endgame (void) {
    // screen of death
}
