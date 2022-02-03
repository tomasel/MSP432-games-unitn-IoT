#include "libraries/init.h"
#include "assets/e_2048.h"
#include "assets/minesweeper_assets.h"

// colors
#define E_BACK 0xBBADA0
#define E_4    0x4495D4
#define E_8    0x2F6895
#define E_16   0xF5BD70
#define E_32   0xF2A032
#define E_64   0xCD8829
#define E_128  0xE37051
#define E_256  0xDE5833
#define E_512  0xBD4A2B
#define E_1024 0x5454DA
#define E_2048 0xDA54D8

// table
#define E_Tbl_origin 32
#define E_Tbl_end    SCREEN_MAX_X-1
#define E_Tbl_hgt    96
#define E_Tbl_row    E_Tbl_hgt/4
Graphics_Rectangle table = {E_Tbl_origin,E_Tbl_origin,E_Tbl_end,E_Tbl_end};

// functions
void e_cls (void);
void e_drawLogo (void);
void e_drawTable (void);
void e_drawScore (void);

// main
void e_2048 () {
    //Graphics_setBackgroundColor(&ctx,E_BACK);
    e_cls();
    //e_drawLogo();
    //e_drawTable();
    //Graphics_drawImage(&ctx, &m_number_bmp, 8, 8);
    Graphics_drawImage(&ctx, &m_icon_bmp, 0, 0);
    while (1) {

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
    Graphics_drawRectangle(&ctx,&table);
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - 3 * E_Tbl_row);
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - 2 * E_Tbl_row);
    Graphics_drawLineH(&ctx,E_Tbl_origin,E_Tbl_end,E_Tbl_end - E_Tbl_row);
    Graphics_drawLineV(&ctx,E_Tbl_end - E_Tbl_row,E_Tbl_origin,E_Tbl_end);
    Graphics_drawLineV(&ctx,E_Tbl_end - 2 * E_Tbl_row,E_Tbl_origin,E_Tbl_end);
    Graphics_drawLineV(&ctx,E_Tbl_end - 3 * E_Tbl_row,E_Tbl_origin,E_Tbl_end);
}
