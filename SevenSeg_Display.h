#ifndef _SEVEN_SEG
#define _SEVEN_SEG
#include <stm32f4xx.h>
#include "Delay.h"
#include "DAC-ADC.h"
//select port E
#define SEVENSEG_PORT	GPIOE

#define D0	2	//SEG F
#define D1	3	//SEG E
#define D2	4	//SEG G
#define D3	5 //SEG D
#define D4	6	//SEG A
#define	D5	7	//SEG B
#define	D6	8	//SEG C
#define D7	9	//Decimal Point

#define LE1	10
#define	LE2	11

#define set_seg1()	SEVENSEG_PORT->BSRR = (1u<<LE1)
#define clr_seg1()	SEVENSEG_PORT->BSRR = (1u<<(LE1+16))
#define set_seg2()	SEVENSEG_PORT->BSRR = (1u<<LE2)
#define clr_seg2()	SEVENSEG_PORT->BSRR = (1u<<(LE2+16))
#define blank_seg() SEVENSEG_PORT->BSRR = 0x8FFC0000;

#define set0()	SEVENSEG_PORT->ODR |= (1u<<D0)|(1u<<D1)|(1u<<D3)|(1u<<D4)|(1u<<D5)|(1u<<D6)
#define set1()	SEVENSEG_PORT->ODR |= (1u<<D0)|(1u<<D1)

void init_SevenSeg();
unsigned int set_difficulty();
void set_SevenSeg(int num);
int numberGen(int n);
void updateTens(int tens);
void updateOnes(int ones);




#endif
