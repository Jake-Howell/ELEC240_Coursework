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

struct _SONG_DATA playSong(unsigned int song[], struct _SONG_DATA songData, _Bool loop){
	unsigned int timeElapsed = TIM2_Elapsed_ms(songData.noteStartTime_ms);//keep track of time
	unsigned int noteDuration = song[(2*songData.noteNum)+1];							//index the node duration
	
	
	if ((timeElapsed > noteDuration)){													//check if next note needs to be played
		if(((2*songData.noteNum) < (songData.song_length - 2))){	//check that song has not finished
			songData.noteNum++;																			//select the next note in song
		}else if(loop == 1){songData.noteNum = 0;}								//replay song if loop is true
		else{}																										//continue playing last note (should be silent) if loop is false
		unsigned int notePeriod = song[(2*songData.noteNum)];			//index note period
		play_note(notePeriod);																		//playSong next note
		songData.noteStartTime_ms = TIM2->CNT;										//update note start time when new note is played
	}

	return songData;																						//return updated song data
}
