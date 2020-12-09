#ifndef __DELAY_H
#define __DELAY_H

#include <stm32f4xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include "LED.h"


#define ENABLE_ISR 1
#define DISABLE_ISR 0

//These values have been checked with an oscillascope to produce accurate interupts
//Clock frequency was set to 180MHz
#define PSC_1s			9227
#define PSC_100ms 	9212
#define PSC_1ms 		8385
#define PSC_1us 		46
#define PSC_Var_Delay		92 			//create timer updates at 1MHz


#define ARR_1s 			10001
#define ARR_100ms 	1001
#define ARR_1ms 		11
#define ARR_1us 		2
#define ARR_Var_Delay		0xFFFF	//allows overflow

//These values have been check with an oscillascope to produce an accurate clock tick
#define	PSC_1ms_Tick


void buzzer_PWM(unsigned int period_us);

void Init_Timer2(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

void Init_Timer3(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

void Init_Timer4_WhiteLight(unsigned int duration);

void Init_Timer5(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);





//polling delays
unsigned int TIM2_Elapsed_ms(unsigned int startTime);
void Wait3_us(int delay_us);
void Wait3_ms(int delay_ms);
void Wait3_s(int delay_s);
void Wait4_us(int delay_us);
void Wait4_ms(int delay_ms);
_Bool wait_1ms_ButtonCheck(void);
#endif
