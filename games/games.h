#ifndef _GAMES_H_
#define _GAMES_H_

void Tetris ();
void Snake ();

#define NUMGAMES 4
struct games {
    int8_t* name;             //video game name
    int8_t* desc;            //short description
    void (*start)(void);    //function pointer to the start function

} games[] = {//14 char per line       | <-- don't have words split by this bar
     {"Tetris",         "The soviet    mind game   ", NULL},
     {"Stack",          "Build a tower high        ", NULL},
     {"Snake",          "The classic   nokia game  ", NULL},
     {"Minesweeper",    "Mind the bomb squares!    ", NULL}
};

#endif
