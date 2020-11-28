#ifndef __DELAY_H
#define __DELAY_H

#include <stm32f4xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include "Delay.h"


#define ENABLE_ISR 1
#define DISABLE_ISR 0

//These values have been checked with an oscillascope
//Clock frequency was set to 180MHz
#define PSC_1s			9227
#define PSC_100ms 	9212
#define PSC_1ms 		8385
#define PSC_1us 		46
#define PSC_Var_Delay		92 			//create timer updates at 1MHz


#define ARR_1s 			10000
#define ARR_100ms 	1000
#define ARR_1ms 		10
#define ARR_1us 		1
#define ARR_Var_Delay		0xFFFF	//allows overflow

void Init_PWM_Timer1(unsigned int PSC_val, unsigned int ARR_val);

void Init_Timer2(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

void Init_Timer3(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

void Init_Timer4(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

void Init_Timer5(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);





//polling delays
void Wait2_us(int delay_us);
void Wait2_ms(int delay_ms);
void Wait2_s(int delay_s);
_Bool wait_1ms_ButtonCheck(void);
#endif
