#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"
#define OFF 0
#define ON 1

#define LD1 0
#define LD2 7
#define LD3 14


#define USER_BUTTON 13

#define TRAF_RED1 2
#define TRAF_YEL1 3
#define TRAF_GEN1 6
#define TRAF_RED2 7
#define TRAF_YEL2 8
#define TRAF_GEN2 9
#define GREEN_MAN 10

#define Dpad_A		0
#define Dpad_B		1
#define Dpad_C		2
#define Dpad_D		3

#define LDR 0

void Init_Dpad(void);
void Init_LEDs(void);
void Init_Traffic_LEDs(void);
void Init_BlueButton(void);


void Toggle_B(int PIN);
void Toggle_C(int PIN);

void Set_B(int B_PIN, _Bool state);
void Set_C(int C_PIN, _Bool state);
_Bool checkButton(char port, unsigned short pin);
void lightController(_Bool R1, _Bool Y1, _Bool G1, _Bool GM, _Bool R2, _Bool Y2, _Bool G2);
#endif
