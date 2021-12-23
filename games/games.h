#ifndef _GAMES_H_
#define _GAMES_H_

void Tetris ();
void Stack ();
void Snake ();

#define NUMGAMES 5
struct games {
    int8_t* name;             //video game name
    int8_t* desc;            //short description
    void (*start)(void);    //function pointer to the start function

} games[] = {//14 char per line       |            |            | avoid having words split by this bar
     {"Tetris",         "The soviet   mind game                 ", &Tetris},
     {"Stack",          "Stack blocks higher by    timing inputs", &Stack},
     {"Snake",          "The classic  nokia game   with reptiles", &Snake},
     {"Minesweeper",    "find mines   with your    logic        ", NULL},
     {"TESTENTRY",      "123456789012345678901234567890123456789",NULL}
};

#endif
