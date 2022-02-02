#include "libraries/init.h"
#include <string.h>

#define N_XM 29
#define N_YM 17
#define N_SIZE 4 //size of snake segments

#define N_SCORE_X  91
#define N_SCORE_Y  33

#define N_PLAYFIELDX  ((128-N_XM*N_SIZE)/2)
#define N_PLAYFIELDY  54

#define N_STARTLEN  5
#define N_SPEED     220

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
 */

typedef struct n_coord {
    uint8_t x;
    uint8_t y;
} n_coord;

uint8_t n_dir, n_ndir, n_field[N_YM][N_XM], n_gameover;
uint16_t n_score;
n_coord n_head, n_tail, n_food;

#include "assets/snake_assets.h"

static void n_keys();
static void n_update();
static void n_updateCoords(n_coord *c, uint8_t dir);
static void n_drawSegment(n_coord c, uint8_t texture);
static void n_placefood();
static void n_drawScore();


void Snake(void){
    srand(millis()<<3);
    Graphics_setBackgroundColor(&ctx,N_BACKGROUND);
    Graphics_Rectangle rect = {N_PLAYFIELDX-2,     N_PLAYFIELDY-2,
                               128-N_PLAYFIELDX+2, N_PLAYFIELDY + N_YM*N_SIZE+2};

    for (;;){
        n_score=0;
        n_gameover=0;
        n_dir=2;
        n_ndir=2;
        n_update();

        Graphics_clearDisplay(&ctx);
        Graphics_drawRectangle(&ctx, &rect); //draw border
        Graphics_drawImage(&ctx, &snake_background, 0, 2); //draw background image
        n_drawScore();

        n_head.x=N_STARTLEN;  n_head.y=N_YM/2; //reset snake position
        n_tail.x=0;           n_tail.y=N_YM/2;

        memset(n_field, 0, N_YM*N_XM);      //clear board
        memset(&(n_field[n_tail.y][n_tail.x]), 2, N_STARTLEN); //fill with snake
        int8_t i;
        for (i=0; i<N_STARTLEN; i++)
            n_update();
        n_placefood();

        uint32_t time = millis();
        while(!n_gameover){
            n_keys();
            uint32_t t = millis();
            if (t>time+N_SPEED){
                n_dir=n_ndir;
                n_update();
                time = t;
            }
        }
        wait (1000);
    }
}

static void n_drawSegment(n_coord c, uint8_t texture){
    if (texture > 15){ //clear if not valid
        Graphics_Rectangle rect = { N_PLAYFIELDX+c.x*N_SIZE, N_PLAYFIELDY+c.y*N_SIZE, N_SIZE+N_PLAYFIELDX+(c.x*N_SIZE), N_SIZE+N_PLAYFIELDY+(c.y*N_SIZE) };
        Graphics_setForegroundColor(&ctx,N_BACKGROUND);
        Graphics_fillRectangle(&ctx, &rect); //clear the square
        Graphics_setForegroundColor(&ctx,N_FOREGROUND);
        return;
    }
    snake_assets.pPixel=pixel_snake_assets[texture]; //select
    Graphics_drawImage(&ctx, &snake_assets, N_PLAYFIELDX+c.x*N_SIZE, N_PLAYFIELDY+c.y*N_SIZE);
    return;
}

static void n_drawScore(){
    int8_t buffer[10];
    s_sprintf(buffer, "%5d",n_score);
    Graphics_drawString(&ctx, buffer, 10, N_SCORE_X, N_SCORE_Y, true);
}

static void n_update(){

    uint8_t *taildir = &n_field[n_tail.y][n_tail.x]; //store old tail
    if ( *taildir & N_ISFAT ){
        *taildir &= ~N_ISFAT;                            //clear isfat and only grow on next cycle (snake get longer)
    } else {
        n_drawSegment(n_tail, 255);            //clear current tail on display
        n_updateCoords(&n_tail, *taildir);               //move tail
        *taildir=0;                                      //clear old tail
        n_drawSegment(n_tail, 9+(n_field[n_tail.y][n_tail.x]&0b111)); //draw new tail
    }



    static uint8_t prev_dir = 2;
    uint8_t eating = n_field[n_head.y][n_head.x];        //store before overwirting head
    n_field[n_head.y][n_head.x] = n_dir;            //set current direction

    if (eating != 0){
        if (eating==255){
            n_field[n_head.y][n_head.x] |= N_ISFAT; //set fat propriety to tell snake to get longer
            n_score++;
            n_drawScore();
        } else {
            n_gameover = 1;
            return;
        }
    }

    uint8_t texture;
    if (n_field[n_head.y][n_head.x]&N_ISFAT){
        texture = 14; //fat texture
    } else {
        switch (n_dir){               //selects the right corner or straight texture
        case 1:
            if(prev_dir == 4){
                texture = 5;
                break;
            }
            if(prev_dir == 2){
                texture = 4;
                break;
            }
            texture = 1;
            break;
        case 2:
            if(prev_dir == 1){
                texture = 2;
                break;
            }
            if(prev_dir == 3){
                texture = 5;
                break;
            }
            texture = 0;
            break;
        case 3:
            if(prev_dir == 4){
                texture = 2;
                break;
            }
            if(prev_dir == 2){
                texture = 3;
                break;
            }
            texture = 1;
            break;
        case 4:
            if(prev_dir == 1){
                texture = 3;
                break;
            }
            if(prev_dir == 3){
                texture = 4;
                break;
            }
            texture = 0;
            break;
        }
    }
    n_drawSegment(n_head, texture);       //draw body where head used to be

    n_updateCoords(&n_head, n_dir);
    n_drawSegment(n_head, 5+n_dir); //draw head texture

    prev_dir=n_dir; //previous direction
    if(eating==255) n_placefood();
}

static void n_placefood(){
    do{
        n_food.x=rand()%N_XM;
        n_food.y=rand()%N_YM;
    } while (n_field[n_food.y][n_food.x]!=0);
    n_field[n_food.y][n_food.x] = 255;

    n_drawSegment(n_food, 15);
}

static void n_keys(){
    uint16_t bts = getButtons();
    if (bts & JOYSTICK_UP && n_dir != 3){
        n_ndir = 1;
        return;
    }
    if (bts & JOYSTICK_RIGHT && n_dir != 4){
        n_ndir = 2;
        return;
    }
    if (bts & JOYSTICK_DOWN && n_dir != 1){
        n_ndir = 3;
        return;
    }
    if (bts & JOYSTICK_LEFT && n_dir != 2 ){
        n_ndir = 4;
        return;
    }
    /*if (bts & BUTTON_A){
        n_ndir = n_dir+1;
        if (n_ndir>4) n_ndir=1;
        return;
    }
    if (bts & BUTTON_B){
       n_ndir = (n_dir-1);
       if (n_ndir==255) n_ndir=4;
       return;
   }*/
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
