#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"
#include "Delay.h"
#include "buzzer.h"
#define OFF 0			//create off constant to make code more readable
#define ON 1			//create on constant to make code more readable

#define LD1 0
#define LD2 7
#define LD3 14


//assign LED pin numbers
#define TRAF_RED1 2		
#define TRAF_YEL1 3
#define TRAF_GEN1 6
#define TRAF_RED2 7
#define TRAF_YEL2 8
#define TRAF_GEN2 9
#define GREEN_MAN 10


#define LDR 0

void Init_LEDs(void);
void Init_Traffic_LEDs(void);


void Toggle_B(int PIN);
void Toggle_C(int PIN);

void Set_B(int B_PIN, _Bool state);
void Set_C(int C_PIN, _Bool state);
void readySteadyGo();
void morseCodeScore(unsigned int score);
void lightController(_Bool R1, _Bool Y1, _Bool G1, _Bool GM, _Bool R2, _Bool Y2, _Bool G2);

void morse_dot();
void morse_dash();

#endif
