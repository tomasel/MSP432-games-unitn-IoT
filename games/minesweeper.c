#include "libraries/init.h"
#include "assets/minesweeper_assets.h"
#include <string.h>
#define M_XM 16
#define M_YM 14
#define M_YSTART 16 
#define M_WIDTH 8

#define M_BACKG 0xffffff
#define M_GRIDC 0x000000

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

uint8_t m_board[M_YM][M_XM], m_gameover;
uint8_t m_px=M_XM/2-1, m_py=M_YM/2-1;
uint16_t m_nflags, m_ncleared;

#define M_NCHECKS 8

struct Coords{
int8_t x;
int8_t y;
} m_offsets[M_NCHECKS]={{-1, -1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};

void m_flag  (uint8_t x, uint8_t y);
void m_clear (uint8_t x, uint8_t y);
void m_drawBoard();
void m_drawCell (uint8_t x, uint8_t y);
void m_drawCursor();
void m_reset();

void Minesweeper (){
    m_drawBoard();
}


void m_drawBoard(){
    Graphics_setBackgroundColor(&ctx, M_BACKG);
    Graphics_setForegroundColor(&ctx, M_GRIDC);
    Graphics_clearDisplay(&ctx);

    uint8_t i,j;
    for (i=0; i<M_YM; i++) //draw horizontal grid lines
        Graphics_drawLineH(&ctx, 0, 127, i*M_WIDTH+M_YSTART);
    for (i=1; i<=M_YM; i++)//draw vertical lines
        Graphics_drawLineV(&ctx, i*M_WIDTH, M_YSTART, 127);

    m_icon_bmp.pPixel = m_gfx_pixels[0]; //select blank cell
    for (i=0; i<M_YM; i++)
		for (j=0; j<M_XM; j++)
            Graphics_drawImage(&ctx, &m_icon_bmp, j*M_WIDTH, i*M_WIDTH+M_YSTART);

    m_icon_bmp.pPixel = m_gfx_pixels[1]; 
    Graphics_drawImage(&ctx, &m_icon_bmp, 3,3); //draw flag for flag counter
    Graphics_drawString(&ctx, M_NBOMBSTR, 2, 13, 2, false);

    m_icon_bmp.pPixel = m_gfx_pixels[2]; 
    Graphics_drawImage(&ctx, &m_icon_bmp, 32,3); //draw bomb
    Graphics_drawString(&ctx, M_NBOMBSTR, 2, 13+29, 2, false);


}

void m_drawCursor(){

}

void m_drawCell (uint8_t x, uint8_t y){
    uint8_t cell = m_board[y][x];
    
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
	m_gameover=0;
	m_nflags=0;
	m_ncleared=0;
}

void m_flag (uint8_t x, uint8_t y){
	uint8_t *t=&m_board[y][x];
	if (*t&M_UNCOVE||m_gameover) //can't flag uncovered tiles
		return;
		
	if (*t&M_FLAGGD) 
		m_nflags--;
	else {
		if (m_nflags>=M_NBOMB) return;//limit on number of flags
		else m_nflags++;
	}
	*t^=M_FLAGGD;				//flag cell
	//draw();
}

void m_clear(uint8_t x, uint8_t y){
	if ( !(x<M_XM && y<M_YM) || (m_board[y][x] & M_UNCOVE) || (m_board[y][x] & M_FLAGGD)){  //can't clear if out of bounds, already uncovered, or flagged
		return;
	}
	if (m_board[y][x]&M_ISBOMB){
		m_gameover=1;
		//draw();
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
	if (t==0){	//recursive call
		for (i=0;i<M_NCHECKS; i++){
			m_clear(x+m_offsets[i].x,y+m_offsets[i].y);	
		}
	}
	m_ncleared++;
	if (m_ncleared==M_XM*M_YM-M_NBOMB)
		m_gameover=3;
}

