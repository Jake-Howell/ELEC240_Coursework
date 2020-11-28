#include "buzzer.h"

void init_buzzer(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	BUZ_PORT->AFR[1] 	|=	(3u<<(4*(BUZ-8)));	//BUZ pin for tim 8
	BUZ_PORT->MODER 	&=	~(3u<<(2*BUZ));	//reset pin mode
	BUZ_PORT->MODER 	|=	(2u<<(2*BUZ));	//set pin as alternate function
	
	Init_PWM_Timer1(450, 1000);
}
