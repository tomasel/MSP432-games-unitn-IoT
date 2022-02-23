#include "libraries/init.h"
#include "assets/minesweeper_assets.h"
#include <math.h>
#define M_XM 16
#define M_YM 14
#define M_YSTART 16 
#define M_WIDTH 8

#define M_BACKG 0xffffff
#define M_GRIDC 0x000000
#define M_SELECT_COLOR 0xff0000
#define M_NBOMB 40
#define M_NBOMBSTR "40"

/*7-6-5-4-3-2-1-0
  0bUFB-3210
bits 0-1-2-3 	Number for clear cells
bit  4: 		Is bomb
bit  5: 		flagged
bit  6: 		uncovered
*/
#define M_NUMBER 0b00001111
#define M_ISBOMB 0b00100000
#define M_FLAGGD 0b01000000
#define M_UNCOVE 0b10000000

uint8_t m_board[M_YM][M_XM], m_state;
/*state:
 * 0: regular gameplay
 * 1: game has been reset but board is not cleared
 * 2: game lost
 * 3: game won
 */

uint8_t m_px=M_XM/2-1, m_py=M_YM/2-1;
uint8_t m_nflags, m_ncleared;

#define M_NCHECKS 8

struct Coords{
int8_t x;
int8_t y;
} m_offsets[M_NCHECKS]={{-1, -1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};

void m_flag  ();
void m_clear (uint8_t x, uint8_t y);
void m_drawBoard();
void m_drawCell (uint8_t x, uint8_t y);
void m_reset();

void m_playerMove(uint8_t dir);

void Minesweeper (){
    static bool held=0;
    for (;;){
        m_state=1;       //set gamestate
        srand(millis());  //shuffle RNG
        m_nflags=M_NBOMB; //reset number of flags
        m_ncleared=0;     //reset number of cleared cells
        m_drawBoard();    //redraw empty board
        m_playerMove(255);//draw cursor

        while (m_state<=1){ //while in playable state
               uint16_t i = getButtons();
               if(i&JOYSTICK_UP)
                   m_playerMove(3);
               if(i&JOYSTICK_DOWN)
                  m_playerMove(2);
               if(i&JOYSTICK_LEFT)
                  m_playerMove(1);
               if(i&JOYSTICK_RIGHT)
                  m_playerMove(0);

               uint32_t magn = sqrt(joystick_y*joystick_y+(joystick_x*joystick_x));

               if (i&(JOYSTICK_RIGHT|JOYSTICK_LEFT|JOYSTICK_UP|JOYSTICK_DOWN))
                   wait(150);

               if(i&(BUTTON_A|BUTTON_J)){
                   if (!held){
                       m_flag();
                       held=1;
                   }
               } else held=0;


               if( i&BUTTON_B ){
                   if (m_state==1)
                       m_reset();
                   m_clear(m_px,m_py);
               }

           }
        wait(5000);

    }

}

void m_playerMove(uint8_t dir){
    Graphics_Rectangle rect;
    rect.xMin=m_px*M_WIDTH-1;
    rect.xMax=rect.xMin+M_WIDTH;
    rect.yMin=m_py*M_WIDTH+M_YSTART;
    rect.yMax=rect.yMin+M_WIDTH;
    Graphics_drawRectangle(&ctx, &rect);
    switch (dir){
    case 0:
        if (m_px<M_XM-1)
            m_px++;
        break;
    case 1:
        if(m_px>0)
            m_px--;
        break;
    case 2:
        if(m_py<M_YM-1)
            m_py++;
        break;
    case 3:
        if(m_py>0)
            m_py--;
        break;
    }
    rect.xMin=m_px*M_WIDTH-1;
    rect.xMax=rect.xMin+M_WIDTH;
    rect.yMin=m_py*M_WIDTH+M_YSTART;
    rect.yMax=rect.yMin+M_WIDTH;
    Graphics_setForegroundColor (&ctx, M_SELECT_COLOR);
    Graphics_drawRectangle(&ctx, &rect);
    Graphics_setForegroundColor (&ctx, M_GRIDC);
}
void m_drawBoard(){
    Graphics_setBackgroundColor(&ctx, M_BACKG);
    Graphics_setForegroundColor(&ctx, M_GRIDC);
    Graphics_clearDisplay(&ctx);

    uint8_t i,j;
    for (i=0; i<M_YM; i++) //draw horizontal grid lines
        Graphics_drawLineH(&ctx, 0, 127, i*M_WIDTH+M_YSTART);
    for (i=1; i<=M_XM; i++)//draw vertical lines
        Graphics_drawLineV(&ctx, i*M_WIDTH-1, M_YSTART, 127);

    m_icon_bmp.pPixel = m_gfx_pixels[0]; //select blank cell
    for (i=0; i<M_YM; i++)
		for (j=0; j<M_XM; j++)
            Graphics_drawImage(&ctx, &m_icon_bmp, j*M_WIDTH, i*M_WIDTH+M_YSTART+1);

    m_icon_bmp.pPixel = m_gfx_pixels[1]; 
    Graphics_drawImage(&ctx, &m_icon_bmp, 3,5); //draw flag for flag counter
    Graphics_drawString(&ctx, M_NBOMBSTR, 2, 13, 5, false);

    m_icon_bmp.pPixel = m_gfx_pixels[2]; 
    Graphics_drawImage(&ctx, &m_icon_bmp, 32,5); //draw bomb
    Graphics_drawString(&ctx, M_NBOMBSTR, 2, 13+29, 5, false);


}

void m_drawCell (uint8_t x, uint8_t y){
    uint8_t cell = m_board[y][x];

    m_icon_bmp.pPixel = m_gfx_pixels[0]; //select blank

    if (cell&M_FLAGGD)
        m_icon_bmp.pPixel = m_gfx_pixels[1]; //draw flag for flag counter
    if (m_state == 1 ){
        m_icon_bmp.pPixel = m_gfx_pixels[0];//draw empty cell,
        //cells will be generated only after clearing the frist
        goto end; //skips drawing numbers
    }

    if (cell&M_UNCOVE){
        m_number_bmp.pPixel = m_num_pixels[cell&M_NUMBER]; //select texture
        m_num_palette[1] = n_num_colors[cell&M_NUMBER];   //select palette
        Graphics_drawImage(&ctx, &m_number_bmp, x*M_WIDTH, y*M_WIDTH+M_YSTART+2); //draw number
        return;
    }

    
    end:
    Graphics_drawImage(&ctx, &m_icon_bmp, x*M_WIDTH, y*M_WIDTH+M_YSTART+1); //draw uncovered cell
}


void m_reset(){
	uint8_t i;
	uint8_t x, y;
    memset(m_board, 0, M_XM*M_YM); //clear board

	for (i=0; i<M_NBOMB; i++){
		do {
			x=rand()%M_XM; y=rand()%M_YM;
		} while (m_board[y][x]!=0 || (x<=m_px+1 &&  x>=m_px-1 && y<=m_py+1 && y>=m_py-1));  //make sure to place bombs in new places and not next to the cursor
		m_board[y][x]=M_ISBOMB;
	}
	m_state=0;
}

void m_flag (){
	uint8_t *t=&m_board[m_py][m_px];
	if (*t&M_UNCOVE||(m_state!=0)) //can't flag uncovered tiles or if game is not started
		return;
		
	if (*t&M_FLAGGD) 
		m_nflags++;
	else {
		if (m_nflags==0)
		    return; //limit on number of flags
		else
		    m_nflags--;
	}
	*t^=M_FLAGGD;				//flag cell
	m_drawCell(m_px, m_py);     //draw

	int8_t str[3];
	s_sprintf(str, "%d", m_nflags);
	Graphics_drawString(&ctx, str, 2, 13, 5, true);
}

void m_clear(uint8_t x, uint8_t y){
	if ( !(x<M_XM && y<M_YM) || (m_board[y][x] & M_UNCOVE) || (m_board[y][x] & M_FLAGGD)){  //can't clear if out of bounds, already uncovered, or flagged
		return;
	}
	if (m_board[y][x]&M_ISBOMB){
		m_state=3; //gameover
		m_icon_bmp.pPixel = m_gfx_pixels[2]; //select BOMB graphics
		uint8_t i, j;
        for (i=0; i<M_YM; i++)              //display all bombs
            for (j=0; j<M_XM; j++)
                if (m_board[i][j]&M_ISBOMB)
                    Graphics_drawImage(&ctx, &m_icon_bmp, j*M_WIDTH, i*M_WIDTH+M_YSTART+1);
		return;		
	}
	uint8_t t=0, i;
	for (i=0;i<M_NCHECKS; i++){
		int8_t ox = m_offsets[i].x+x;
		int8_t oy = m_offsets[i].y+y;
		if((ox>=0 && ox<M_XM && oy>=0 && oy<M_YM) && m_board[oy][ox] & M_ISBOMB)
			t++;
	}
	m_board[y][x]=t|M_UNCOVE;//set M_uncovered
	m_drawCell(x, y);
	if (t==0){	//recursive call
		for (i=0; i<M_NCHECKS; i++){
			m_clear(x+m_offsets[i].x,y+m_offsets[i].y);	
		}
	}
	m_ncleared++;
	if (m_ncleared==M_XM*M_YM-M_NBOMB)
		m_state=2;   //game won!
}

