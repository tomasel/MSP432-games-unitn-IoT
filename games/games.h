#ifndef _GAMES_H_
#define _GAMES_H_

void Tetris ();
void Stack ();
void Snake ();
void Minesweeper();
void Wordle ();
void e_2048 ();

#define NUMGAMES 6
struct games {
    int8_t* name;             //video game name
    int8_t* desc;            //short description
    void (*start)(void);    //function pointer to the start function

} games[] = {//14 char per line       |            |            | avoid having words split by this bar
     {"Tetris",         "The soviet   mind game                 ", &Tetris},
     {"Stack",          "Stack blocks higher by    timing inputs", &Stack},
     {"Snake",          "The classic  nokia game   with reptiles", &Snake},
     {"Minesweeper",    "Find mines   with your    logic        ", &Minesweeper},
     {"2048",           "Reach 2048                             ", &e_2048},
     {"Wordle",         "Guess the    word                      ", &Wordle}
};

#endif
