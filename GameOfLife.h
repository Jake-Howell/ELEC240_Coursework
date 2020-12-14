#ifndef _GOL
#define _GOL
#include 	<stm32f4xx.h>
#include "LED_Matrix.h"
#include 	"LED.h"
#include "SWITCH.h"
#include "lcd.h"
#include "SevenSeg_Display.h"
#include "buzzer.h"
#include "usart.h"
#include "DAC-ADC.h"

#define SONG1_LENGTH 			56
#define END_SONG_LENGTH		20
#define BLUE_PRESS_LENGTH 8
#define DPAD_PRESS_LENGTH	6


void startGameOfLife();
_Bool runGameOfLife(_Bool frame[2][8][16], _Bool buffNum);
_Bool rules(_Bool frame[2][8][16], _Bool buffNum, int cellX, int cellY);
void init_GameOfLife(_Bool frame[2][8][16], unsigned int difficulty);
void resetFrame(_Bool frame[2][8][16]);
_Bool killRandomCell(_Bool frame[2][8][16]);
#endif