#ifndef _INPUT_H_
#define _INPUT_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define JOYSTICK_CENTER 8000
#define JOYSTICK_DEVIATION

uint16_t joystick_x;
uint16_t joystick_y;

uint16_t getButtons();

#define BUTTON_1       0b1
#define BUTTON_2       0b10
#define BUTTON_A       0b100
#define BUTTON_B       0b1000
#define BUTTON_J       0b10000
#define JOYSTICK_UP    0b100000
#define JOYSTICK_DOWN  0b1000000
#define JOYSTICK_LEFT  0b10000000
#define JOYSTICK_RIGHT 0b100000000

void _inputInit();

#endif 
