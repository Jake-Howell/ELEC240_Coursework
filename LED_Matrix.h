#ifndef _LED_MATRIX
#define _LED_MATRIX
#include <stm32f4xx.h>
#include "Delay.h"

//GPIOB
#define IOE		12
#define LE		6

//GPIOC
#define SCK		10
#define	MOSI	12

void init_matrix();
void matrix_write(unsigned short rowData, char row);
void matrix_display(_Bool frame[2][8][16], _Bool buffNum);
void translateFrame(_Bool frame[2][8][16], _Bool buffNum,unsigned short dx, unsigned short dy);


#endif