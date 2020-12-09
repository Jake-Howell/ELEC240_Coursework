#ifndef _SWITCH_H_
#define _SWITCH_H_
#include <stm32f4xx.h>
#include "LED.h"
#include "Delay.h"

#define USER_BUTTON 				13
#define USER_BUTTON_PORT		GPIOC

#define Dpad_A		0
#define Dpad_B		1
#define Dpad_C		2
#define Dpad_D		3

#define DPAD_PORT	GPIOG


struct _SWITCH_DATA{								//create struct to store song meta data
	_Bool A;
	_Bool B;
	_Bool C;
	_Bool D;
	_Bool Blue;
	_Bool BlueLongPress;
};


void Init_Dpad(void);
void Init_BlueButton(void);
_Bool checkButton(char port, unsigned short pin);


#endif