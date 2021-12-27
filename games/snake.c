#include "libraries/init.h"
#include <string.h>
/*
 * head
 *  up
 *  l
 *  r
 *  dn
 *  eating_up
 *  eating_l
 *  eating_r
 *  eating_dn
 *
 * tail
 *  up
 *  l
 *  r
 *  dn
 *
 * body
 *  up
 *  l
 *  r
 *  dn
 *
 * body_fat
 *  up
 *  l
 *  r
 *  dn
 *
 *corner_fat
 *  L
 *  J
 *  [
 *  ]
 *
 * corner
 *  L
 *  J
 *  [
 *  ]
 *
 *  //total: 28 textures
 */

#define N_XM 28
#define N_YM 17
#define N_SIZE 4 //size of snake segments

#define N_PLAYFIELDX  ((128-N_XM*N_SIZE)/2)
#define N_PLAYFIELDY  (128-(N_YM*N_SIZE)-15)

#define N_STARTLEN 5

#define N_FOREGROUND 0x000000
#define N_BACKGROUND 0xB2BD08


#define N_ISFAT 0b1000

/* snake direction:
 * 1 up
 * 2 right
 * 3 down
 * 4 left
 *
 * 3 lsb: direction
 * bit 3: isFat
 * bit 4: isCorner
 * 0b000 0 0 000
 */

typedef struct n_coord {
    uint8_t x;
    uint8_t y;
} n_coord;

uint8_t n_dir, n_field[N_YM][N_XM], n_gameover;
uint16_t n_score;
n_coord n_head, n_tail, n_food;


static void n_keys();
static void n_update();
static void n_updateCoords(n_coord *c, uint8_t dir);
static void n_redraw();
static void n_drawSegment(uint8_t x, uint8_t y, uint8_t type);
static void n_placefood();

void Snake(void){
    srand(millis()<<3);
    Graphics_setBackgroundColor(&ctx,N_BACKGROUND);
    Graphics_clearDisplay(&ctx);
    Graphics_Rectangle rect = {N_PLAYFIELDX-2,     N_PLAYFIELDY-2,
                               128-N_PLAYFIELDX+2, N_PLAYFIELDY + N_YM*N_SIZE+2};
    Graphics_drawRectangle(&ctx, &rect);
    for (;;){
        n_score=0;
        n_gameover=0;
        n_dir=2;

        n_head.x=N_XM/2;     n_head.y=N_YM/2; //reset snake position
        n_tail.x=n_head.x-N_STARTLEN;  n_tail.y=N_YM/2;

        memset(n_field, 0, N_YM*N_XM);      //clear board
        memset(&(n_field[n_tail.y][n_tail.x]), 2, N_STARTLEN); //fill with snake

        n_redraw();
        n_placefood();

        while(!n_gameover){
            n_keys();
            n_update();
            wait(400);
        }
    }
}

static void n_drawSegment(uint8_t x, uint8_t y, uint8_t type){
    Graphics_Rectangle rect = { N_PLAYFIELDX+x*N_SIZE, N_PLAYFIELDY+y*N_SIZE, N_SIZE+N_PLAYFIELDX+(x*N_SIZE), N_SIZE+N_PLAYFIELDY+(y*N_SIZE) };
    switch (type&0b111){
    case 0:
        Graphics_setForegroundColor(&ctx,N_BACKGROUND);
        Graphics_fillRectangle(&ctx, &rect); //clear the square
        Graphics_setForegroundColor(&ctx,N_FOREGROUND);
        break;
    case 1: case 3:
        rect.xMin++; rect.xMax--;          //make thinner
        Graphics_fillRectangle(&ctx, &rect);
        break;
    case 2: case 4:
        rect.yMin++; rect.yMax--;          //make thinner
        Graphics_fillRectangle(&ctx, &rect);
        break;
    }
}

static void n_keys(){
    uint16_t bts = getButtons();
    if (bts & JOYSTICK_UP && n_dir != 3){
        n_dir = 1;
        return;
    }
    if (bts & JOYSTICK_RIGHT && n_dir != 4){
        n_dir = 2;
        return;
    }
    if (bts & JOYSTICK_DOWN && n_dir != 1){
        n_dir = 3;
        return;
    }
    if (bts & JOYSTICK_LEFT && n_dir != 2 ){
        n_dir = 4;
        return;
    }
}


static void n_updateCoords(n_coord *c, uint8_t dir){
    switch (dir & 0b111){
    case 1:
        c->y--;
        break;
    case 2:
        c->x++;
        break;
    case 3:
        c->y++;
        break;
    case 4:
        c->x--;
        break;
    }

    if (c->x==255)
        c->x=N_XM-1;
    if (c->x==N_XM)
        c->x=0;

    if (c->y==255)
        c->y=N_YM-1;
    if (c->y==N_YM)
        c->y=0;
}

static void n_update(){

    uint8_t *taildir = &n_field[n_tail.y][n_tail.x]; //store old tail
    if ( *taildir & N_ISFAT ){
        *taildir &= ~N_ISFAT;                            //clear isfat and only gorw on next cycle (snake get longer)
    } else {
        n_drawSegment(n_tail.x, n_tail.y, 0);            //clear current tail on display
        n_updateCoords(&n_tail, *taildir);               //move tail
        *taildir=0;                                     //clear old tail
    }



    uint8_t t = n_field[n_head.y][n_head.x];        //check before overwirting head
    n_field[n_head.y][n_head.x] = n_dir;            //set current direction

    if (t != 0){
        if (t==255){
            n_field[n_head.y][n_head.x] |= N_ISFAT; //set fat propriety to tell snake to get longer
            n_score++;
            n_placefood(); //i know this can bug plz fix later
        } else {
            n_gameover = 1;
            return;
        }
    }


    n_updateCoords(&n_head, n_dir);
    n_drawSegment(n_head.x, n_head.y, n_dir);

}

static void n_redraw(){


}

static void n_placefood(){
    do{
        n_food.x=rand()%N_XM;
        n_food.y=rand()%N_YM;
    } while (n_field[n_food.y][n_food.x]!=0);
    n_field[n_food.y][n_food.x] = 255;

    Graphics_Rectangle rect = {N_PLAYFIELDX+N_SIZE*n_food.x, N_PLAYFIELDY+n_food.y*N_SIZE,
                               N_SIZE+N_PLAYFIELDX+(n_food.x*N_SIZE), N_SIZE+N_PLAYFIELDY+(n_food.y*N_SIZE)};
    Graphics_drawRectangle(&ctx, &rect);
}
