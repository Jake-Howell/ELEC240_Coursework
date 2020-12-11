#include <stm32f4xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include "Delay.h"
/*void Toggle_BPin(int pin)
{
	GPIOB->ODR ^= (0x1<<pin);							//Toggle_BPin Any GPIOB pin using XOR
}*/

void buzzer_PWM(unsigned int period_us){
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//ENABLE tim1 to run on APB2 (Advanced Perphieral Bus 2's clock tick) - 90MHz in this case
	
	TIM1->PSC = 45 - 1;									//setting pre-scaler for 1us ticks (APB2 clock devider) 
	TIM1->ARR = period_us - 1;					//counter reload value (Auto Reload Register - TIM1 ARR is only 16 bit)	
	TIM1->CNT = 0;											//set initial value to counter
	TIM1->CCMR1 = 0x0068;								//PWM mode wit preload enabled
	TIM1->CCER 	=	0x0004;								//ENABLE PWM on ch1N
	TIM1->CCR1 	=	(period_us/2);				//pulse width of 50%
	TIM1->BDTR	=	0x8000;								//ENABLE output
	TIM1->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void Init_Timer2(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//ENABLE tim2 to run on APB1 (Advanced Perphieral Bus 1's clock tick) - 90MHz in this case
	TIM2->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM2->PSC = PSC_val -1;							//setting pre-scaler (APB1 clock devider) 
	TIM2->ARR = ARR_val -1;							//counter reload value (Auto Reload Register - TIM2 ARR is 32 bit)	
	TIM2->CNT = 0xFFFFFFFE;							//set initial value to counter
	if (ISR_Enable == 1){
		NVIC->ISER[0]|=(1u<<28);					//timer 2 global interrupt enabled
	}
	TIM2->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void Init_Timer3(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//ENABLE tim3 to run on APB1 (Advanced Perphieral Bus 1's clock tick) - 90MHz in this case
	TIM3->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM3->PSC = PSC_val - 1;						//setting pre-scaler (APB1 clock devider) 
	TIM3->ARR = ARR_val - 1;						//counter reload value (Auto Reload Register - TIM3 ARR is only 16 bit)	
	TIM3->CNT = 0;											//set initial value to counter
	if (ISR_Enable == 1){
		NVIC->ISER[0]|=(1u<<29);					//timer 3 global interrupt enabled
	}
	TIM3->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void Init_Timer4_GreenFlash(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	//ENABLE tim4 to run on APB1 (Advanced Perphieral Bus 1's clock tick) - 90MHz in this case
	TIM4->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM4->PSC = PSC_val - 1;						//setting pre-scaler (APB1 clock devider) 
	TIM4->ARR = ARR_val - 1;						//counter reload value (Auto Reload Register - TIM4 ARR is only 16 bit)	
	TIM4->CNT = 0;											//set initial value to counter
	if (ISR_Enable == 1){
		NVIC->ISER[0]|=(1u<<30);					//timer 4 global interrupt enabled
	}
	TIM4->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void Init_Timer5(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable){	//32 bit timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;	//ENABLE tim5 to run on APB1 (Advanced Perphieral Bus 1's clock tick) - 90MHz in this case
	TIM5->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM5->PSC = PSC_val - 1;							//setting pre-scaler (APB1 clock devider) 
	TIM5->ARR = ARR_val - 1;								//counter reload value (Auto Reload Register - TIM3 ARR is only 16 bit)	
	TIM5->CNT = 0;											//set initial value to counter
	
	if (ISR_Enable == 1){
		NVIC_EnableIRQ(TIM5_IRQn);						//timer 5 global interrupt enabled
	}
	TIM5->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void TIM2_IRQHandler(void){							//TIMER 2 INTERRUPT SERVICE ROUTINE
	TIM2->SR &= ~TIM_SR_UIF;							//clear interrupt flag in status register
}

void TIM3_IRQHandler(void){
	TIM3->SR &= ~TIM_SR_UIF;							//clear interrupt flag in status register
}

void TIM4_IRQHandler(void){
	TIM4->SR &= ~TIM_SR_UIF;							//clear interrupt flag in status register
	GPIOB->ODR ^= (1u<<LD1);				//toggle white light
}

void TIM5_IRQHandler(void){
	TIM5->SR &= ~TIM_SR_UIF;
	
}


unsigned int TIM2_Elapsed_ms(unsigned int startTime){
	
	unsigned int currentTime = TIM2->CNT;
	unsigned int timeElapsed_ms = (((currentTime - startTime) & 0xFFFFFFFF)/2);
	return timeElapsed_ms;
}

unsigned int TIM3_Elapsed_us(unsigned int startTime){
	
	unsigned int currentTime = TIM3->CNT;
	unsigned int timeElapsed_us = (((currentTime - startTime) & 0xFFFF));
	return timeElapsed_us;
}


void Wait3_us(int delay_us){
	
	int start = TIM3->CNT;								//mark the stating point
	int current_CNT = start;
	
	while(((current_CNT - start) & (0xFFFF)) < delay_us){
		current_CNT = TIM3->CNT;
		
	}//calculate diffrence in time and compare to delay_cycles
	
}

void Wait3_ms(int delay_ms){
	int count = delay_ms;
	while(count > 0){
		Wait3_us(1000);
		count --;
	}
}

void Wait4_us(int delay_us){
	
	int start = TIM4->CNT;								//mark the stating point
	int current_CNT = start;
	
	while(((current_CNT - start) & (0xFFFF)) < delay_us){
		current_CNT = TIM4->CNT;
		
	}//calculate diffrence in time and compare to delay_cycles
}

void Wait4_ms(int delay_ms){
	int count = delay_ms;
	while(count > 0){
		Wait4_us(1000);
		count --;
	}
}


void Wait3_s(int delay_s){
	int i;
		for(i = 0; i < delay_s; i++){
			Wait3_ms(1000);							//loops a 1 second delay
		}
}
