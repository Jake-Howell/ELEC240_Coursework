#include "buzzer.h"

void init_buzzer(){
	buzzer_PWM(0);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	BUZ_PORT->AFR[1] 	|=	(1u<<(4*(BUZ-8)));	//BUZ pin for tim 8
	BUZ_PORT->MODER 	&=	~(3u<<(2*BUZ));			//reset pin mode
	BUZ_PORT->MODER 	|=	(2u<<(2*BUZ));			//set pin as alternate function
	
	play_note(NOTE_A4);
	Wait3_ms(250);
	play_note(NOTE_E5);
	Wait3_ms(250);
	play_note(SILENT);
}

void play_note(int period){
	TIM1->ARR 	= period;			//update period
	TIM1->CCR1 	= (period/2);	//update duty cycle
	
	//set interupt to turn off note
}



unsigned int nextNote(unsigned int song[], unsigned int noteNum){
	unsigned short noteSelect = song[(noteNum*2)];	//index note period
	unsigned short time = song[((noteNum*2)+1)];		//index note time
	
	play_note(noteSelect);													//update buzzer to play next note
	return time;																		//return length of time note needs to be played
}