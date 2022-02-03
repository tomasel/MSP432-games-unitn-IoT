/*
 * e_2048.h
 *
 *  Created on: 3 feb 2022
 *      Author: eliat
 */

#include <ti/grlib/grlib.h>
#include <stdint.h>

static const uint8_t logo_st[] = {
  0b00000000,0b00000000,0b00000000,0b00000011,
  0b00000000,0b00000000,0b00000000,0b00000001,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000111,0b10011110,0b00011001,0b11100000,
  0b00000000,0b10010010,0b00111001,0b00100000,
  0b00000000,0b10010010,0b00101001,0b11100000,
  0b00000001,0b00010010,0b01001001,0b11100000,
  0b00000010,0b00010010,0b11111111,0b00100000,
  0b00000111,0b10011110,0b00001001,0b11100000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,
  0b10000000,0b00000000,0b00000000,0b00000001,
  0b11000000,0b00000000,0b00000000,0b00000011
};

static const uint32_t palette_2048[] = {0xecc400,0xffffff};

Graphics_Image logo_2048 = {
    GRAPHICS_IMAGE_FMT_1BPP_UNCOMP,
    32,
    32,
    2,
    palette_2048,
    logo_st
};
