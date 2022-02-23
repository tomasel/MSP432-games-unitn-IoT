#ifndef _GAMES_H_
#define _GAMES_H_

#include "assets/background.h"

void Tetris ();
void Snake ();
void Minesweeper();
void e_2048 ();

#define NUMGAMES 4
struct games {
    int8_t* name;             //video game name
    int8_t* desc;            //short description
    const Graphics_Image* img;
    void (*start)(void);    //function pointer to the start function

} games[] = {//14 char per line              |                    avoid having words split by this bar
     {"2048",           "Reach 2048 by combi-ning squares        ", &e_20484BPP_COMP_RLE4, &e_2048},
     {"Tetris",         "The soviet mind game                    ", &tetris4BPP_COMP_RLE4,    &Tetris},
     {"Snake",          "The classic Nokia   game with reptiles  ", &nokia4BPP_COMP_RLE4, &Snake},
     {"Minesweeper",    "Find mines with yourlogic, the XP game  ", &minesweeper4BPP_COMP_RLE4, &Minesweeper},
};

#endif
