#include "libraries/init.h"

/////////////////////////////////////CONSTANTS////////////////////////////////
#define T_XM 10
#define T_YM 20
/////////////////////////////////////SETTINGS////////////////////////////////
//comment out to disable
#define T_WALLKICK        //wall kicks instead of failing to rotate a piece, it moves it
//#define T_FALLPREVIEW     //shows a preview of where the piece will fall

////////////////////////////////////GRAPHICS//////////////////////////////////
#define T_BACKGROUND GRAPHICS_COLOR_BLACK
#define T_CLEARCOLOR GRAPHICS_COLOR_WHITE
#define T_FONTCOLOR  GRAPHICS_COLOR_WHITE
#define T_BORDERCOLOR GRAPHICS_COLOR_WHITE
#define T__COLORCONVERSION ((c&0xff)/2)&0xff|((c&0xff00)/2)&0xff00|((c&0xff0000)/2)&0xff0000


#define T_BLOCKW 6
#define T_SCRXOFFSET 3
#define T_SCRYOFFSET 3
#define T_NEWBLOCKX 12
#define T_NEWBLOCKY 1

//color palette // I O T S Z L J
uint32_t t_colors[7]      = {0x01f1f2, 0xffd500, 0xa001f1, 0x02f102, 0xf00001, 0xef8201, 0x0100f1};


const uint8_t t_leveldelay[20] = {32, 32, 29, 25, 22, 18, 15, 11, 7, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1};
#define T_ACTIONDELAY 256       //delay after a hard drop or after clearing a line
#define T_LEVELUP t_lvl* t_lvl * 3  //number of lines you need to clear to level up (can be any formula)

#define T_SHIFTDELAY 140  //autoshift delay (lower for earlier repeat)
#define T_SHIFTSPEED 31   //autoshift speed (lower for faster movement)

#define T_OFFX -1
#define T_OFFY -1


/////////////////////////////////////SETTINGS////////////////////////////////


//7 tetrominos, 4 rotations
static const uint16_t             // 0b 00011011 01010101  0b 00-01-10-11 (x offset)   01-01-01-01 (y offset)
    t_tetrominos[7][4] = {                   //                           0  1  2  3               1  1  1  1     {(0,1),(1,1),(2,1),(3,1)}
        {0b0001101101010101, 0b1010101000011011, 0b0001101110101010, 0b0101010100011011},   //I
        {0b0110011001011010, 0b0110011001011010, 0b0110011001011010, 0b0110011001011010},   //O
        {0b0001011010100110, 0b0101100101101011, 0b0001011010101110, 0b0101000101101011},   //T
        {0b0001011010100101, 0b0101101001101011, 0b0001011011111010, 0b0000010101101011},   //S
        {0b0001011001011010, 0b1010010101101011, 0b0001011010101111, 0b0000010111101001},   //Z
        {0b1000011001101010, 0b0101011001101111, 0b0001100010101011, 0b0001010101011011},   //L
        {0b0000011001101010, 0b0101011001101101, 0b0001101010101011, 0b0001010111011011}};  //J

#ifdef T_WALLKICK
static const struct Coords {
    int8_t x, y;
} SRS[2][4][2][4] = {  //srs +[line][4 rotations][2 directions][4 checks]
    {{{{.x = -1, .y = 0}, {.x = -1, .y = +1}, {.x = 0, .y = -2}, {.x = -1, .y = -2}}, {{.x = +1, .y = 0}, {.x = +1, .y = +1}, {.x = 0, .y = -2}, {.x = +1, .y = -2}}},
     {{{.x = +1, .y = 0}, {.x = +1, .y = -1}, {.x = 0, .y = +2}, {.x = +1, .y = +2}}, {{.x = +1, .y = 0}, {.x = +1, .y = -1}, {.x = 0, .y = +2}, {.x = +1, .y = +2}}},
     {{{.x = +1, .y = 0}, {.x = +1, .y = +1}, {.x = 0, .y = -2}, {.x = +1, .y = -2}}, {{.x = -1, .y = 0}, {.x = -1, .y = +1}, {.x = 0, .y = -2}, {.x = -1, .y = -2}}},
     {{{.x = -1, .y = 0}, {.x = -1, .y = -1}, {.x = 0, .y = +2}, {.x = -1, .y = +2}}, {{.x = -1, .y = 0}, {.x = -1, .y = -1}, {.x = 0, .y = +2}, {.x = -1, .y = +2}}}},
    {//I teromino
     {{{.x = -2, .y = 0}, {.x = +1, .y = 0}, {.x = +1, .y = +2}, {.x = -2, .y = -1}}, {{.x = +2, .y = 0}, {.x = -1, .y = 0}, {.x = -1, .y = +2}, {.x = +2, .y = -1}}},
     {{{.x = -1, .y = 0}, {.x = +2, .y = 0}, {.x = -1, .y = +2}, {.x = +2, .y = -1}}, {{.x = +2, .y = 0}, {.x = -1, .y = 0}, {.x = +2, .y = +1}, {.x = -1, .y = -2}}},
     {{{.x = +2, .y = 0}, {.x = -1, .y = 0}, {.x = +2, .y = +1}, {.x = -1, .y = -1}}, {{.x = -2, .y = 0}, {.x = +1, .y = 0}, {.x = -2, .y = +1}, {.x = +1, .y = -1}}},
     {{{.x = -2, .y = 0}, {.x = +1, .y = 0}, {.x = -2, .y = +1}, {.x = +1, .y = -2}}, {{.x = +1, .y = 0}, {.x = -2, .y = 0}, {.x = +1, .y = +2}, {.x = -2, .y = -1}}}}};
#endif

uint8_t grid[T_YM][T_XM];
uint8_t t_lvl;
int8_t t_rot, t_piece, t_currX, t_currY, t_newpiece, t_newscore = 0;

uint8_t t_flags;
#define GAMEOVER 0b00000001
#define REPROT   0b00000100
#define REPSIDE  0b00001000
#define REPDROP  0b00010000
uint8_t t_bag[7], bgi;
#define T_TXTMAX 10 
int8_t t_buffer[T_TXTMAX];
uint32_t t_score, t_totlines;
uint32_t t_fallCounter = 0;  //used for gravity

bool t_move(int8_t dx,int8_t dy, int8_t dr);
void t_drawPiece(uint8_t pid, uint8_t rot, int8_t x, int8_t y, uint32_t color);
void t_drawScore();
void t_draw();
void t_clearRows();
void t_randompiece();
void t_keys();
void t_place();

#define hnget(y, b) ((y >> (b << 1)) & 0b11)


void Tetris (void) {
    uint8_t i;
       for (;;) {
           bgi = 0;
           t_totlines = 0;
           t_flags = 0;
           t_score = 0;
           t_lvl = 1;
           for (i=0; i<T_XM*T_YM; i++){
               grid[0][i]=0;
           }

           t_randompiece();
           t_randompiece();
           t_randompiece();  //clear current and next, then create new bag
           t_draw();

           while (!(t_flags & GAMEOVER)) {  //gameloop
                   t_keys();
                   if (millis() - t_fallCounter > t_leveldelay[t_lvl] * 20) {
                       t_fallCounter = millis();
                       t_drawPiece(t_piece, t_rot, t_currX, t_currY, T_BACKGROUND);
                       if (!t_move(0, 1, 0))
                           t_place();
                       t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
                   }
           }
           //pause();
       }
}

bool t_move(int8_t dx, int8_t dy, int8_t dr){
    int8_t i, x, y, nx, ny, nr;
    nx = t_currX + dx;
    ny = t_currY + dy;
    nr = (t_rot + dr < 0) ? 3 : (t_rot + dr) % 4;
    uint16_t p = t_tetrominos[t_piece][nr];
    for (i = 0; i < 4; i++) {
        x = hnget(p, i + 4) + nx;
        y = hnget(p, i) + ny;
        if (x < 0 || x >= T_XM || y >= T_YM || y < 0 || grid[y][x] != 0)
            return 0;
    }
    t_currX = nx;
    t_currY = ny;
    t_rot = nr;
    return 1;
}

void t_clearRows(void) {
    int8_t full, cont = 0;
    uint16_t i;
    for (i = 0; i < T_YM; i++) {  //check each row
        full = 1;
        uint16_t j;
        for (j = 0; j < T_XM && full; j++) {  //check each space in the row
            if (grid[i][j] == 0)
                full = 0;
        }
        if (full) {

            Graphics_Rectangle rect = {.xMin=T_SCRXOFFSET, .xMax= T_SCRXOFFSET+T_BLOCKW*T_XM, .yMin=i*T_BLOCKW+T_SCRYOFFSET, .yMax=(i+1)*T_BLOCKW+T_SCRYOFFSET};
            Graphics_setForegroundColor(&ctx, T_BACKGROUND);
            Graphics_fillRectangle(&ctx, &rect);
            rect.yMin+=T_BLOCKW/3;
            rect.yMax-=T_BLOCKW/3;
            Graphics_setForegroundColor(&ctx, T_CLEARCOLOR);
            Graphics_fillRectangle(&ctx, &rect);

            t_totlines++;
            cont++;
            uint16_t j;
            for (j=i; j>0; j--){       //copy each line down
                memcpy(grid[j],grid[j-1],T_XM);
            }
            for (j=0; j<T_XM;j++) grid[0][j]=0; //clear first row
        }
    }

    if (cont) {
        wait(T_ACTIONDELAY);
        t_score += (cont * cont * 100) * t_lvl;
        t_draw();
        if (t_totlines >= T_LEVELUP && t_lvl < 19) t_lvl++;
    }
}


void t_place() {
    uint8_t i;
    uint16_t p = t_tetrominos[t_piece][t_rot];
    for (i = 0; i < 4; i++)
        grid[hnget(p, i) + t_currY][hnget(p, i + 4) + t_currX] = t_piece+1;
    t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
    t_randompiece();
    //draw();
    t_clearRows();
    if (!t_move(0, 0, 0)) {  //check for gameover
        t_flags |= GAMEOVER;
        wait(T_ACTIONDELAY);
    }
}

bool t_rotate(int8_t _dir) {
    if (t_move(0, 0, _dir))
        return 1;
#ifdef T_WALLKICK
    int8_t i;
    struct Coords t;
    for (i = 0; i < 4; i++) {
        t = SRS[!t_piece][t_rot][!(_dir + 1)][i];
        if (t_move(t.x, t.y, _dir))
            return 1;
    }
#endif
    return 0;
}

void t_keys(void){
    uint16_t bts = getButtons();
    if (bts&BUTTON_1) {
        t_flags |= GAMEOVER;
        return;
    }
    /*if ((GetKeyState('P') | GetKeyState(VK_ESCAPE)) & 0x8000)
        pause();*/

    int8_t dir = 0;

    //rotation
    if (bts&BUTTON_A) dir = 1;
    if (bts&BUTTON_B) dir = -1;
    if (dir) {
        if (!(t_flags & REPROT)){
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, T_BACKGROUND);
            t_rotate(dir);
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
        }

        t_flags |= REPROT;
    } else
        t_flags &= ~REPROT;

    //sideways movement
    dir = 0;
    if      (bts&JOYSTICK_RIGHT) dir = 1;
    else if (bts&JOYSTICK_LEFT) dir = -1;
    if (dir) {
        static uint32_t presstime = 0;
        if (!(t_flags & REPSIDE) || millis() - presstime > T_SHIFTDELAY) {  //delayed autoshift delay
            static uint32_t t = 0;
            if (millis() - t > T_SHIFTSPEED) {  //autoshift
                t_drawPiece(t_piece, t_rot, t_currX, t_currY, T_BACKGROUND);
                t_move(dir, 0, 0);
                t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
                t=millis();
            }
        }
        if (!(t_flags & REPSIDE)) {
            t_flags |= REPSIDE;
            presstime = millis();
        }
    } else
        t_flags &= ~REPSIDE;

    //drop
    if (bts&JOYSTICK_DOWN) {
        static uint32_t t = 0;
        if (millis() - t > T_SHIFTSPEED) {
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, T_BACKGROUND);
            if (t_move(0, 1, 0)) {
                t_score += t_lvl;
                t_fallCounter = millis();
                t_drawScore();
            }
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
            t = millis();
        }
    }
    if (bts&BUTTON_J) {

        if (!(t_flags & REPDROP)) {
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, T_BACKGROUND);
            while (t_move(0, 1, 0)) {
                t_score += 2 * t_lvl;
            }
            t_place();
            wait(T_ACTIONDELAY);
            t_flags |= REPDROP;
            t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
            t_drawScore();
        }


    } else
        t_flags &= ~REPDROP;
}

void t_drawPiece(uint8_t pid, uint8_t rot, int8_t x, int8_t y, uint32_t color){
    Graphics_Rectangle rect;
    uint16_t p= t_tetrominos[pid][rot];
    uint8_t i;

    Graphics_setForegroundColor(&ctx, color);
    for (i = 0; i < 4; i++){
        rect.yMin=(hnget(p, i) + y)*T_BLOCKW+T_SCRYOFFSET;
        rect.yMax=rect.yMin+T_BLOCKW-1;
        rect.xMin= (hnget(p, i + 4) + x)*T_BLOCKW+T_SCRXOFFSET;
        rect.xMax=rect.xMin+T_BLOCKW-1;
        Graphics_fillRectangle(&ctx, &rect);
    }
    #ifdef GRIDCOLOR
        if (color==BACKGROUND)        //border
            return;
        Graphics_setForegroundColor(&g_sContext, GRIDCOLOR);
        for (i = 0; i < 4; i++){
            rect.yMin=(hnget(p, i) + y)*BLOCKW+SCRYOFFSET;
            rect.yMax=rect.yMin+BLOCKW;
            rect.xMin= (hnget(p, i + 4) + x)*BLOCKW+SCRXOFFSET;
            rect.xMax=rect.xMin+BLOCKW-1;
            Graphics_drawRectangle(&g_sContext, &rect);
         }
    #endif
}

void t_drawScore(){
    s_sprintf(t_buffer,"%7d",t_score);
    Graphics_setForegroundColor(&ctx, T_FONTCOLOR);
    Graphics_drawString(&ctx, t_buffer,T_TXTMAX, T_XM*(T_BLOCKW+1),T_BLOCKW*9+2, true);
}

void t_draw() {
    Graphics_setBackgroundColor(&ctx, T_BACKGROUND);
    Graphics_clearDisplay(&ctx);

    Graphics_Rectangle rect = {.xMin=T_SCRXOFFSET-1, .xMax= T_SCRXOFFSET+T_BLOCKW*T_XM, .yMin=T_SCRYOFFSET-1, .yMax=T_SCRYOFFSET+T_BLOCKW*T_YM};
    Graphics_setForegroundColor(&ctx, T_BORDERCOLOR);
    Graphics_drawRectangle(&ctx, &rect);

    uint8_t i, j, y;
    rect.xMin=T_SCRXOFFSET;
    rect.xMax=T_SCRXOFFSET+T_BLOCKW-1;
    rect.yMin=T_SCRYOFFSET;
    rect.yMax=T_SCRYOFFSET+T_BLOCKW-1;
    for (i = 0; i < T_YM; i++) {
        for (j = 0; j < T_XM; j++) {
            y = grid[i][j];
            if (y){
                Graphics_setForegroundColor(&ctx, t_colors[(y-1)]);
                Graphics_fillRectangle(&ctx, &rect);
                #ifdef GRIDCOLOR
                    Graphics_setForegroundColor(&g_sContext, GRIDCOLOR);
                    Graphics_drawRectangle(&g_sContext, &rect);
                #endif
            }
            rect.xMax+=T_BLOCKW;            rect.xMin+=T_BLOCKW;
        }
        rect.xMax=T_SCRXOFFSET+T_BLOCKW-1;    rect.yMax+=T_BLOCKW;
        rect.xMin=T_SCRXOFFSET;           rect.yMin+=T_BLOCKW;
    }

#ifdef FALLPREVIEW
        int t2=t_currY;
        while(t_move(0, 1, 0));
        uint32_t c=t_colors[t_piece];
        t_drawPiece(t_piece, t_rot, t_currX, t_currY, T__COLORCONVERSION);
        t_currY=t2;
#endif

    //draw current piece

    t_drawPiece(t_piece, t_rot, t_currX, t_currY, t_colors[t_piece]);
    t_drawPiece(t_newpiece, 0, T_NEWBLOCKX, T_NEWBLOCKY, t_colors[t_newpiece]);


    Graphics_setForegroundColor(&ctx, T_FONTCOLOR);
    Graphics_drawString(&ctx, "Next:",5,T_XM*(T_BLOCKW+1), 4, false);

    Graphics_drawString(&ctx, "Lines:", 6, T_XM*(T_BLOCKW+1),T_BLOCKW*5, false);
    s_sprintf(t_buffer,"%5d",t_totlines);
    Graphics_drawString(&ctx, t_buffer,T_TXTMAX, T_XM*(T_BLOCKW+1),T_BLOCKW*6+2, false);

    Graphics_drawString(&ctx, "Score:", 6, T_XM*(T_BLOCKW+1),T_BLOCKW*8, false);
    t_drawScore();

    s_sprintf(t_buffer,"Lvl:%2d",t_lvl);
    Graphics_drawString(&ctx, t_buffer,T_TXTMAX, T_XM*(T_BLOCKW+1),T_BLOCKW*11, false);
}

void t_randompiece() {
    t_drawPiece(t_newpiece, 0, T_NEWBLOCKX, T_NEWBLOCKY, T_BACKGROUND);
    t_piece = t_newpiece;
    t_rot = 0;  //load the new piece
    t_currX = T_XM / 2 + T_OFFX;
    t_currY = T_OFFY;

    if (bgi == 0) {  //Generate bag for next batch
        uint8_t i;
        for (bgi = 0; bgi < 7; bgi++) {
        randnew:
            t_bag[bgi] = rand() % 7;
            for (i = 0; i < bgi; i++)
                if (t_bag[i] == t_bag[bgi]) goto randnew;  //if piece is already in bag, try again
        }
    }
    t_newpiece = t_bag[--bgi];

#ifdef FALLPREVIEW
    int t2=t_currY;
    while(t_move(0, 1, 0));
    uint32_t c=t_colors[t_piece];
    t_drawPiece(t_piece, t_rot, t_currX, t_currY, T__COLORCONVERSION);
    t_currY=t2;
#endif

    t_drawPiece(t_newpiece, 0, T_NEWBLOCKX, T_NEWBLOCKY, t_colors[t_newpiece]);
}
