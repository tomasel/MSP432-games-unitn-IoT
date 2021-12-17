#ifndef _INPUT_H_
#define _INPUT_H_

uint8_t buttonstatus;
int16_t joistick_x;
int16_t joistick_y;

#define __BUTTON_1 0b1
#define __BUTTON_2 0b10
#define __BUTTON_A 0b100
#define __BUTTON_B 0b1000
#define __BUTTON_J 0b10000

#define BUTTON_1_PRESSED  buttonstatus&__BUTTON_1
#define BUTTON_2_PRESSED  buttonstatus&__BUTTON_2
#define BUTTON_A_PRESSED  buttonstatus&__BUTTON_A
#define BUTTON_B_PRESSED  buttonstatus&__BUTTON_B
#define BUTTON_J_PRESSED  buttonstatus&__BUTTON_J

#endif 
