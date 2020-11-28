#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"


void Init_LEDs(void);
void Init_Traffic_LEDs(void);
void Init_BlueButton(void);


void Toggle_B(int PIN);
void Toggle_C(int PIN);

void Set_B(int B_PIN, _Bool state);
void Set_C(int C_PIN, _Bool state);

void lightController(_Bool R1, _Bool Y1, _Bool G1, _Bool GM, _Bool R2, _Bool Y2, _Bool G2);
#endif
