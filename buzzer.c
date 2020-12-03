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
}

struct _SOUND_DATA playSong(unsigned int song[], struct _SOUND_DATA data){
	unsigned int timeElapsed = TIM2_Elapsed_ms(data.noteStartTime_ms);//keep track of time
	unsigned int noteDuration = song[(2*data.noteNum)+1];							//index the node duration
	
	
	if ((timeElapsed > noteDuration)){//check if next note needs to be played
		if(((2*data.noteNum) < (data.song_length - 2))){			//check that song has not finished
			data.noteNum++;																			//select the next note in song
			data.noteStartTime_ms = TIM2->CNT;									//update note start time when new note is played
			unsigned int notePeriod = song[(2*data.noteNum)];			//index note period
			play_note(notePeriod);																//playSong next note
		}else if(data.loopState == LOOP_TRUE){								//replay song if loop is true
			data.noteNum = 0;
		}
		//continue playing last note (should be silent) if loop is false
	}

	return data;																						//return updated song data
}
