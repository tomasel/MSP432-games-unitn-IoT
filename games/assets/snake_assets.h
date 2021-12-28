/* Copyright (c) 2021, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <ti/grlib/grlib.h>
#include <stdint.h>

static const uint8_t pixel_snake_assets[][4] = {
{0b00000000,
 0b11010000,
 0b10110000,
 0b00000000}, //horizontal 0

{0b01100000,
 0b01000000,
 0b00100000,
 0b01100000}, //vertical  1
///////////////////////////////
{0b00000000,
 0b00110000,
 0b01010000,
 0b01100000}, //corner 1  2

{0b00000000,
 0b11000000,
 0b10100000,
 0b01100000}, //corner 2  3

{0b01100000,
 0b10100000,
 0b11000000,
 0b00000000}, //corner 3  4

{0b01100000,
 0b01010000,
 0b00110000,
 0b00000000}, //corner 4  5
//////////////////////////////
{0b01100000,
 0b01100000,
 0b01010000,
 0b01100000}, //head 1    6

{0b01000000,
 0b10110000,
 0b11110000,
 0b00000000}, //head 2    7

{0b01100000,
 0b10100000,
 0b01100000,
 0b01100000}, //head 3   8

{0b00100000,
 0b11010000,
 0b11110000,
 0b00000000}, //head 4   9

 //////////////////////////////
{0b01100000,
 0b01100000,
 0b01000000,
 0b01000000}, //tail 1   10

{0b00000000,
 0b00110000,
 0b11110000,
 0b00000000}, //tail 2    11

{0b01000000,
 0b01000000,
 0b01100000,
 0b01100000}, //tail 3   12

{0b00000000,
 0b11000000,
 0b11110000,
 0b00000000}, //tail 4   13
 //////////////////////////////
{0b01100000,
 0b11010000,
 0b10110000,
 0b01100000}, //fat horizontal/vertical 14
 /////////////////////////////
{0b00000000,
 0b00100000,
 0b01010000,
 0b00100000}, //foood
};

static const uint32_t palette_snake_assets1BPP_UNCOMP[]= { N_BACKGROUND, N_FOREGROUND };

Graphics_Image  snake_assets = {
	IMAGE_FMT_1BPP_UNCOMP,
	4,
	4,
	2,
	palette_snake_assets1BPP_UNCOMP,
	pixel_snake_assets[2],
};

