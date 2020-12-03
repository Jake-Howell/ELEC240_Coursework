#include "GameOfLife.h"


void startGameOfLife(){
	Init_Dpad();
	init_buzzer();
	init_matrix();
	init_SevenSeg();
	init_LCD();
	Init_LEDs();
	//128 bit frame with blank 128 bit buffer
	_Bool frame[2][8][16] =
	{
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}; 
	
	unsigned int song1[SONG1_LENGTH] = {
		SILENT,		1,
		NOTE_D5, 	200,
		SILENT, 	100,
		NOTE_D5, 	200,
		SILENT,		100,
		NOTE_D5,	100,
		SILENT,		100,
		NOTE_DS5, 200,
		SILENT,		100,
		NOTE_DS5, 200,
		SILENT,		100,
		NOTE_DS5, 100,
		SILENT, 	100,
		NOTE_D5, 	200,
		SILENT, 	100,
		NOTE_D5, 	200,
		SILENT,		100,
		NOTE_D5,	100,
		SILENT,		100,
		
		NOTE_A5,	100,
		NOTE_GS5,	100,
		NOTE_G5,	100,
		NOTE_FS5,	100,
		
		NOTE_A5,	100,
		NOTE_GS5,	100,
		NOTE_G5,	100,
		NOTE_FS5,	100,
		SILENT,		1
	};
	unsigned int endSong[END_SONG_LENGTH] = {
		SILENT,		1,
		NOTE_D5, 	300,
		NOTE_CS5, 300,
		NOTE_C5,	300,
		NOTE_B4,	100,
		NOTE_C5,	100,
		NOTE_B4,	100,
		NOTE_AS4,	100,
		NOTE_B4,	400,
		SILENT,		1,
	};
	
	
	updateLCD("The Game of Life",0);
	Wait3_s(1);
	updateLCD("Move with D Pad",0);
	updateLCD("select with blue",1);
	Wait3_s(1);
	updateLCD("Long press blue",0);
	updateLCD("to start",1);
	
	init_GameOfLife(frame);	//let player make their move
	
	
	//after player presses blue button for 2 seconds
	updateLCD("How long will",0);
	updateLCD("you survive?",1);
	_Bool continueGame = 1;
	unsigned short bufferNum = 0;
	unsigned short score = 0;
	
	//set sound variables to play song
	struct _SONG_DATA songData;
	songData.noteStartTime_ms = TIM2->CNT;
	songData.song_length = SONG1_LENGTH;
	songData.noteNum = 0;
	
	while(continueGame){
		
		for(int i=0; i<500; i++){
			matrix_display(frame,bufferNum);								//draw the nextNote frame
			songData = playSong(song1, songData, LOOP_TRUE);	//play music
		}
		//translateFrame(frame,bufferNum,0,1);
		continueGame = runGameOfLife(frame,bufferNum);
		
		//printf("Score:  %d",score);
		set_SevenSeg(score++);
		bufferNum = !bufferNum;
	}
	
	//reset sound variables to play new song
	songData.noteStartTime_ms = TIM2->CNT;
	songData.song_length = END_SONG_LENGTH;
	songData.noteNum = 0;
	
	songData = playSong(endSong, songData, LOOP_FALSE);
	
	updateLCD("      GAME      ",0);
	updateLCD("      OVER      ",1);
	
	
	//play end game tune
	while(1){	//keep looping until the end of the song
		matrix_display(frame,bufferNum);
		songData = playSong(endSong, songData, LOOP_FALSE);
	}
}

_Bool runGameOfLife(_Bool frame[2][8][16], _Bool buffNum){
	_Bool continueGame = 0;
	for(int y=0; y<8; y++){																	//itterate through rows
		for(int x=0; x<16; x++){															//itterate through cols
			frame[!buffNum][y][x] = rules(frame, buffNum,x,y); 	//apply rules for given cell
			if(frame[!buffNum][y][x] == frame[buffNum][y][x]){	//check for differances in the frame
				continueGame = continueGame;											//if no diffrence, leave check next cell
			}else{
				continueGame = 1;																	//if theres a diffrance the game will continue
			}
		}
	}
	return continueGame;
}

//_Bool endGame(_Bool frame[2][8][16], _Bool buffNum){
//	
//}


_Bool rules(_Bool frame[2][8][16], _Bool buffNum, int cellX, int cellY){
	
	//get the current cell state
	_Bool cellState = frame[buffNum][cellY][cellX];
	
	//check all neighbouring cells states
	unsigned short neighbourCount=
		frame[buffNum][((cellY-1)&0x7)][((cellX-1)&0xF)] +
		frame[buffNum][((cellY-1)&0x7)][((cellX+0)&0xF)] +
		frame[buffNum][((cellY-1)&0x7)][((cellX+1)&0xF)] +
		frame[buffNum][((cellY+0)&0x7)][((cellX+1)&0xF)] +
		frame[buffNum][((cellY+1)&0x7)][((cellX+1)&0xF)] +
		frame[buffNum][((cellY+1)&0x7)][((cellX+0)&0xF)] +
		frame[buffNum][((cellY+1)&0x7)][((cellX-1)&0xF)] +
		frame[buffNum][((cellY+0)&0x7)][((cellX-1)&0xF)];
	
	//Rule 1: if a living cell has fewer than two neighbours it dies
	if (neighbourCount < 2 && cellState == 1){	
		cellState =0;
	}
	//Rule 2: if a living cell has 2 or 3 neighbours it survives to the next round
	else if((neighbourCount == 2 || neighbourCount == 3) && cellState == 1){
		cellState = cellState;
	}
	//Rule 3: if a cell has more than 3 neighbours, it dies 
	else if(neighbourCount > 3 && cellState == 1){
		cellState = 0;
	}
	//Rule 4:
	else if(neighbourCount == 3 && cellState == 0){
		cellState = 1;
	}
	
	return cellState;
}

_Bool init_GameOfLife(_Bool frame[2][8][16]){
	
	
	unsigned short cursorX = 0, cursorY = 0, buttonBus = 0, lastBus = 0;
	enum STATES {NOT_PRESSED = 0, PRESSED, DEBOUNCED};
	int currentState = NOT_PRESSED;
	int nextState = NOT_PRESSED;
	
	_Bool userButtonVal = 0, up = 0, down = 0, left = 0, right = 0, startGameOfLife = 0;
	
	//debounce variables
	int startTime=0,timeElapsed = 0, longPress_CNT_ms = 0;
	
	set_SevenSeg(0);
	while(startGameOfLife == 0){
		matrix_display(frame,0);													//draw next frame
		
		
		userButtonVal = checkButton('C',USER_BUTTON);
		up 		= checkButton('G', Dpad_D);
		down 	= checkButton('G', Dpad_B);
		left 	= checkButton('G', Dpad_A);
		right	= checkButton('G', Dpad_C);
		
		//update button bus
		buttonBus = userButtonVal | (up<<1u) | (down<<2u) | (left<<3u) | (right<<4u);
		timeElapsed = (TIM3->CNT - startTime)&0xFFFF;
		switch(currentState){
			
			
			case NOT_PRESSED:
				if (buttonBus>0){
					lastBus = buttonBus;
					startTime = TIM3->CNT;
					nextState = PRESSED;
				}else{nextState = NOT_PRESSED;}
				break;
				
				
			case PRESSED:
				//if button released before debounce time, ignore input
				if ((buttonBus==0) && (timeElapsed < 20)){
					nextState = NOT_PRESSED;
				}else if (buttonBus == 0){
					startTime = 0;				//reset time
					timeElapsed = 0;			//reset time
					longPress_CNT_ms = 0;	//reset long press counter if button was released before 2 seconds
					nextState = DEBOUNCED;
				}
				else if ((userButtonVal == 1) && (timeElapsed > 0x3E8)){	//count 1ms
					longPress_CNT_ms++;
					if (longPress_CNT_ms > 2000){	//check if button was pressed for longer than 2 seconds
						startGameOfLife = 1; //exit while loop if long pressed
					}
					startTime = TIM3->CNT;	//reset start to count to next milla second
				}
				break;
				
				
			case DEBOUNCED:
				if((lastBus & 1)==1){
					frame[0][cursorY][cursorX] = !frame[0][cursorY][cursorX];
				}else{
					cursorX = (cursorX + (-1*(lastBus & (1u<<3u))>>3u) + (1*(lastBus&(1u<<4u))>>4u)) & 0xF;
					cursorY = (cursorY + (-1*(lastBus & (1u<<1u))>>1u) + (1*(lastBus&(1u<<2u))>>2u)) & 0x7;
				}
				nextState = NOT_PRESSED;
				break;
		}
		currentState = nextState;
		
	}
	return frame;
}
