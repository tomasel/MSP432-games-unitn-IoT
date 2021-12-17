#ifndef _SOUND_H_
#define _SOUND_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "util.h"
#include "graphic.h"
#define CLOCKSPEED 48000000 //clockspeed in hertz



void _soundInit();

void stopSound();
void playNoise();


void playNote(uint8_t noteNum); //accepts midi note numbers from 24 to 107

typedef struct npd {uint8_t note; uint16_t duration;} npd;
void playSong( uint32_t numNotes, npd* notes);

#define STOP_NOTE 0

#define	B7	107
#define	Bb7	106
#define	A7	105
#define	Ab7	104
#define	G7	103
#define	Gb7	102
#define	F7	101
#define	E7	100
#define	Eb7	99
#define	D7	98
#define	Db7	97
#define	C7	96
#define	B6	95
#define	Bb6	94
#define	A6	93
#define	Ab6	92
#define	G6	91
#define	Gb6	90
#define	F6	89
#define	E6	88
#define	Eb6	87
#define	D6	86
#define	Db6	85
#define	C6	84
#define	B5	83
#define	Bb5	82
#define	A5	81
#define	Ab5	80
#define	G5	79
#define	Gb5	78
#define	F5	77
#define	E5	76
#define	Eb5	75
#define	D5	74
#define	Db5	73
#define	C5	72
#define	B4	71
#define	Bb4	70
#define	A4	69
#define	Ab4	68
#define	G4	67
#define	Gb4	66
#define	F4	65
#define	E4	64
#define	Eb4	63
#define	D4	62
#define	Db4	61
#define	C4	60
#define MIDDLE_C	C4
#define	B3	59
#define	Bb3	58
#define	A3	57
#define	Ab3	56
#define	G3	55
#define	Gb3	54
#define	F3	53
#define	E3	52
#define	Eb3	51
#define	D3	50
#define	Db3	49
#define	C3	48
#define	B2	47
#define	Bb2	46
#define	A2	45
#define	Ab2	44
#define	G2	43
#define	Gb2	42
#define	F2	41
#define	E2	40
#define	Eb2	39
#define	D2	38
#define	Db2	37
#define	C2	36
#define	B1	35
#define	Bb1	34
#define	A1	33
#define	Ab1	32
#define	G1	31
#define	Gb1	30
#define	F1	29
#define	E1	28
#define	Eb1	27
#define	D1	26
#define	Db1	25
#define	C1	24

#define As7 106
#define Gs7 104
#define Fs7 102
#define Ds7 99
#define Cs7 97
#define As6 94
#define Gs6 92
#define Fs6 90
#define Ds6 87
#define Cs6 85
#define As5 82
#define Gs5 80
#define Fs5 78
#define Ds5 75
#define Cs5 73
#define As4 70
#define Gs4 68
#define Fs4 66
#define Ds4 63
#define Cs4 61
#define As3 58
#define Gs3 56
#define Fs3 54
#define Ds3 51
#define Cs3 49
#define As2 46
#define Gs2 44
#define Fs2 42
#define Ds2 39
#define Cs2 37
#define As1 34
#define Gs1 32
#define Fs1 30
#define Ds1 27
#define Cs1 25


//italian notes
#define DO1   C1
#define DOd1  Cs1
#define REb1  Db1
#define RE1   D1
#define REd1  Ds1
#define MIb1  Eb1
#define MI1   E1
#define FA1   F1
#define FAd1  Fs1
#define SOLb1 Gb1
#define SOL1  G1
#define SOLd1 Gs1
#define LAb1  Ab1
#define LA1   A1
#define LAd1  As1
#define SIb1  Bb1
#define SI1   B1

#define DO2   C2
#define DOd2  Cs2
#define REb2  Db2
#define RE2   D2
#define REd2  Ds2
#define MIb2  Eb2
#define MI2   E2
#define FA2   F2
#define FAd2  Fs2
#define SOLb2 Gb2
#define SOL2  G2
#define SOLd2 Gs2
#define LAb2  Ab2
#define LA2   A2
#define LAd2  As2
#define SIb2  Bb2
#define SI2   B2

#define DO3   C3
#define DOd3  Cs3
#define REb3  Db3
#define RE3   D3
#define REd3  Ds3
#define MIb3  Eb3
#define MI3   E3
#define FA3   F3
#define FAd3  Fs3
#define SOLb3 Gb3
#define SOL3  G3
#define SOLd3 Gs3
#define LAb3  Ab3
#define LA3   A3
#define LAd3  As3
#define SIb3  Bb3
#define SI3   B3

#define DO4   C4
#define DOd4  Cs4
#define REb4  Db4
#define RE4   D4
#define REd4  Ds4
#define MIb4  Eb4
#define MI4   E4
#define FA4   F4
#define FAd4  Fs4
#define SOLb4 Gb4
#define SOL4  G4
#define SOLd4 Gs4
#define LAb4  Ab4
#define LA4   A4
#define LAd4  As4
#define SIb4  Bb4
#define SI4   B4

#define DO5   C5
#define DOd5  Cs5
#define REb5  Db5
#define RE5   D5
#define REd5  Ds5
#define MIb5  Eb5
#define MI5   E5
#define FA5   F5
#define FAd5  Fs5
#define SOLb5 Gb5
#define SOL5  G5
#define SOLd5 Gs5
#define LAb5  Ab5
#define LA5   A5
#define LAd5  As5
#define SIb5  Bb5
#define SI5   B5

#define DO6   C6
#define DOd6  Cs6
#define REb6  Db6
#define RE6   D6
#define REd6  Ds6
#define MIb6  Eb6
#define MI6   E6
#define FA6   F6
#define FAd6  Fs6
#define SOLb6 Gb6
#define SOL6  G6
#define SOLd6 Gs6
#define LAb6  Ab6
#define LA6   A6
#define LAd6  As6
#define SIb6  Bb6
#define SI6   B6

#define DO7   C7
#define DOd7  Cs7
#define REb7  Db7
#define RE7   D7
#define REd7  Ds7
#define MIb7  Eb7
#define MI7   E7
#define FA7   F7
#define FAd7  Fs7
#define SOLb7 Gb7
#define SOL7  G7
#define SOLd7 Gs7
#define LAb7  Ab7
#define LA7   A7
#define LAd7  As7
#define SIb7  Bb7
#define SI7   B7


#endif
