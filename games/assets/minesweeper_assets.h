#include <ti/grlib/grlib.h>
#include <stdint.h>

static const uint8_t m_num_pixels[][6] = {
    {0,0,0,0,0,0}, //0
    {0b00010000,
     0b00110000,
     0b01010000,
     0b00010000,
     0b00010000,
     0b01111100}, //1
    {0b00111000,
     0b01000100,
     0b00001000,
     0b00010000,
     0b00100000,
     0b01111100}, //2
    {0b00111000,
     0b00000100,
     0b00011000,
     0b00000100,
     0b00000100,
     0b00111000}, //3
    {0b00000100,
     0b00001100,
     0b00010100,
     0b00100100,
     0b01111100,
     0b00000100}, //4
    {0b01111100,
     0b01000000,
     0b01111000,
     0b00000100,
     0b01111000}, //5
    {0b00111000,
     0b01000000,
     0b01111000,
     0b01000100,
     0b01000100,
     0b00111000}, //6
    {0b01111100,
     0b00000100,
     0b00001000,
     0b00001000,
     0b00010000,
     0b00010000}, //7
    {0b00111100,
     0b01000100,
     0b00111000,
     0b01000100,
     0b01000100,
     0b01111000} //8
};

static const uint8_t m_gfx_pixels[][14] = {
    {0b00000000,0b00000000,
     0b00010101,0b01011000,
     0b00010101,0b01011000,
     0b00010101,0b01011000,
     0b00010101,0b01011000,
     0b00010101,0b01011000,
     0b00101010,0b10101000}, //blank

    {0b00000000,0b00000000,
     0b00010111,0b11111000,
     0b00010111,0b11111000,
     0b00010110,0b01011000,
     0b00010000,0b00011000,
     0b00000000,0b00001000,
     0b00101010,0b10101000} //flag
};

const uint32_t n_num_colors[] = {0x000000, 0x3366ff, 0x008000, 0xff0000, 0x333399, 0x800000, 0x008080, 0x000000, 0xff00ff};

uint32_t                m_num_pallette [] = {0xc0c0c0, 0x3366ff};
static const uint32_t   m_4_pallette   [] = {0xc0c0c0, 0xffffff, 0x808080, 0xff00000}; 

const Graphics_Image  m_number_bmp = {
    IMAGE_FMT_1BPP_UNCOMP,
    7,
    6,
    2,
    m_num_pallette,
    m_num_pixels[1],
};




const Graphics_Image  m_icon_bmp = {
    IMAGE_FMT_2BPP_UNCOMP,
    7,
    7,
    4,
    m_4_pallette,
    m_gfx_pixels[1],
};
