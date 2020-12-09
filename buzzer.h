#ifndef _BUZZER
#define _BUZZER


#include <stm32f4xx.h>
#include "Delay.h"
#define BUZ_PORT	GPIOB	//set buzzer port
#define BUZ		13				//set pin used for buzzer

#define LOOP_TRUE 	1		//create constant to set music to loop
#define LOOP_FALSE	0		//create constant to play music once


//note time periods in us
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

struct _SOUND_DATA{								//create struct to store song meta data
	unsigned int noteStartTime_ms;	//store the start time of each new note
	unsigned int noteNum;						//store which note in the song is currently being played
	unsigned int song_length;				//store the length of the song array
	_Bool loopState;								//store weather the song should loop or not
};

void init_buzzer();								//buzzer initiliser 
void play_note(int period);				//change note being played
struct _SOUND_DATA playSong(unsigned int song[],struct _SOUND_DATA data);	//control music and update SOUND_DATA struct

#endif