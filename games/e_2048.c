#include "libraries/init.h"

// grid height and width
#define E_W 4
#define E_H 4

// colors (every number >2048 is black, because fuck that)
// TODO: do we want colors?
#define E_BACK 0x44475A
#define E_TEXT 0xFFFFFF
#define E_0    0x000000
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

typedef struct e_point {
    int x, y;
} e_point;

static void e_newnumber();  // generates a new element
static void e_move(int direction);   // also does all the sums

void e_2048 () {
   
}