#ifndef _BUZZER
#define _BUZZER


#include <stm32f4xx.h>
#include "Delay.h"
#define BUZ_PORT	GPIOB
#define BUZ		13

#define LOOP_TRUE 	1
#define LOOP_FALSE	0

//notes
#define SILENT		0
#define NOTE_A4 	2273
#define NOTE_AS4 	2145
#define NOTE_B4		2025
#define NOTE_C5		1911
#define NOTE_CS5 	1804
#define NOTE_D5		1703
#define NOTE_DS5	1607
#define NOTE_E5		1517
#define NOTE_F5		1432
#define NOTE_FS5	1351
#define NOTE_G5		1276
#define NOTE_GS5	1204
#define NOTE_A5		1137

struct _SONG_DATA{
	unsigned int noteStartTime_ms;
	unsigned int noteNum;
	unsigned int song_length;
};

void init_buzzer();
void play_note(int period);
struct _SONG_DATA playSong(unsigned int song[], struct _SONG_DATA s, _Bool loop);

#endif