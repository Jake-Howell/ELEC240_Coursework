#ifndef _GOL
#define _GOL
#include 	<stm32f4xx.h>
#include "LED_Matrix.h"
#include "gpio.h"
#include "lcd.h"
#include "SevenSeg_Display.h"
#include "buzzer.h"

#define SONG1_LENGTH 			56
#define END_SONG_LENGTH		20

void startGameOfLife();
_Bool runGameOfLife(_Bool frame[2][8][16], _Bool buffNum);
_Bool rules(_Bool frame[2][8][16], _Bool buffNum, int cellX, int cellY);
_Bool init_GameOfLife(_Bool frame[2][8][16]);
#endif