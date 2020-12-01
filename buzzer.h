#include <stm32f4xx.h>
#include "Delay.h"
#define BUZ_PORT	GPIOB
#define BUZ		13

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



void init_buzzer();
void play_note(int period);
unsigned int nextNote(unsigned int song[], unsigned int noteNum);