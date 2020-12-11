#include "GameOfLife.h"


void startGameOfLife(){
	Init_Dpad();
	//init_buzzer();
	init_matrix();
	init_SevenSeg();
	init_LCD();
	Init_LEDs();
	char userName[6]; 
	
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
	
	
	//printf("%c",0x1B);
	updateLCD("The Game of Life",0);
	updateLCD("Enter Name on PC",1);
	
	
	printf("Please enter your name (6 chars max) and hit enter:\r\n");
	usart_get_string(userName,6);
	printf("Your Username is:\t%s",userName);
	
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
	struct _SOUND_DATA songData;
	songData.noteStartTime_ms = TIM2->CNT;
	songData.noteNum = 0;
	songData.song_length = SONG1_LENGTH;
	songData.loopState = LOOP_TRUE;
	
	
	while(continueGame){
		
		for(int i=0; i<500; i++){
			matrix_display(frame,bufferNum);			//draw the next frame
			songData = playSong(song1, songData);	//play music
		}

		continueGame = runGameOfLife(frame,bufferNum);
		score++;																			//for each new frame add 1 to score
		//printf("Score:  %d",score);
		updateLCDscore(userName, score);
		bufferNum = !bufferNum;						//flip the buffer to draw the next frame
	}
	
	//reset sound variables to play new song
	songData.noteStartTime_ms = TIM2->CNT;
	songData.noteNum = 0;
	songData.song_length = END_SONG_LENGTH;
	songData.loopState = LOOP_FALSE;
	
	songData = playSong(endSong, songData);
	
	updateLCD("   GAME OVER!   ",0);		//print GAME over to LCD line 0
	updateLCDscore(userName, score);			//print name and score at end of game
	
	
	unsigned int startTime = TIM2->CNT, timeElapsed = 0; 
	//play end game tune
	while(timeElapsed < 3000){	//keep looping until the end of the song
		matrix_display(frame,bufferNum);
		songData = playSong(endSong, songData);
		timeElapsed = TIM2_Elapsed_ms(startTime);
	}
	resetFrame(frame);									//reset frame to all 0's 
	matrix_display(frame, bufferNum);		//display blank frame after game finishes
	morseCodeScore(score);							//flash score in morse code with blue on board led
	
	return;	//return to game menu
}

_Bool runGameOfLife(_Bool frame[2][8][16], _Bool buffNum){//itterate through each cell and apply rules
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


void resetFrame(_Bool frame[2][8][16]){ //used to sett every element in the frame array to 0
	
	for (int B = 0; B<2; B++){				//itterate through buffers
		for(int y = 0; y < 8; y++){			//itterate through y positions
			for (int x = 0; x<16; x++){		//itterate through x positions
				frame[B][y][x] = 0;					//set each element within array to 0
			}
		}
	}
}

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

void init_GameOfLife(_Bool frame[2][8][16]){
	//reset sound variables to play new song
	struct _SOUND_DATA bluePressData;		//set initial conditions for bluePressData to run sound effects
	bluePressData.noteStartTime_ms = TIM2->CNT;						//define note start time
	bluePressData.noteNum = ((BLUE_PRESS_LENGTH - 2)/2);	//start at end of sound so it doesn't start playing
	bluePressData.song_length = BLUE_PRESS_LENGTH;				//define how long the sound effect array is
	bluePressData.loopState = LOOP_FALSE;									//make the souund effect only play once
	
	struct _SOUND_DATA dPadPressData;		//set initial conditions for dPadPressData to run sound effects
	dPadPressData.noteStartTime_ms = TIM2->CNT;						//define note start time
	dPadPressData.noteNum = ((DPAD_PRESS_LENGTH - 2)/2);	//start at end of sound so it doesn't start playing
	dPadPressData.song_length = DPAD_PRESS_LENGTH;				//define how long the sound effect array is
	dPadPressData.loopState = LOOP_FALSE;									//make the souund effect only play once
	
	//sound effects
	unsigned int bluePress[BLUE_PRESS_LENGTH] = {	//sound effect for user button press
		SILENT, 	1,			//play nothing for 1ms
		NOTE_A4, 	50,			//play note A4 for 50ms
		NOTE_E5, 	50,			//play note E5 for 50ms
		SILENT, 	1				//play nothing for 1ms
	};
	unsigned int dpadPress[DPAD_PRESS_LENGTH] = {	//sound effect for dpad button press
		SILENT, 	1,			//play nothing for 1ms
		NOTE_B4, 	50,			//play note B5 for 50ms
		SILENT, 	1				//play nothing for 1ms
	};
	
	
	unsigned short cursorX = 0, cursorY = 0, buttonBus = 0, lastBus = 0;	//initalise shorts
	enum STATES {NOT_PRESSED = 0, PRESSED, DEBOUNCED};										//create enums for switch
	int currentState = NOT_PRESSED;																				//set initial button state to NOT_PRESSED
	int nextState = NOT_PRESSED;																					//set next state to NOT_PRESSED
	_Bool currentCellState;																								//create bool to store the state of the selected cell
	_Bool userButtonVal = 0, up = 0, down = 0, left = 0, right = 0, startGameOfLife = 0;	//initalise bools with 0
	
	//debounce variables
	unsigned int startTime=0,timeElapsed = 0, longPress_CNT_ms = 0;				
	
	set_SevenSeg(0);
	while(startGameOfLife == 0){
		matrix_display(frame,0);														//draw next frame
		bluePressData = playSong(bluePress, bluePressData);	//run blue button sound effects
		dPadPressData = playSong(dpadPress, dPadPressData);	//run dpad sound effects
		
		userButtonVal = checkButton('C', USER_BUTTON);			//get state of user button
		up 		= checkButton('G', Dpad_D);										//get state of Dpad D
		down 	= checkButton('G', Dpad_B);										//get state of Dpad B
		left 	= checkButton('G', Dpad_A);										//get state of Dpad A
		right	= checkButton('G', Dpad_C);										//get state of Dpad C
		
		//combine all states into a bus to make debouncing easier
		buttonBus = userButtonVal | (up<<1u) | (down<<2u) | (left<<3u) | (right<<4u);
		
		
		timeElapsed = (TIM3->CNT - startTime)&0xFFFF;				//update time elapsed for debounce
		switch(currentState){
			
			
			case NOT_PRESSED:
				if (buttonBus>0){									//checkButton if any button is pressed
					lastBus = buttonBus;						//store the value of all buttons for inspection later
					startTime = TIM3->CNT;					//set start time
					nextState = PRESSED;						//switch to next state
				}else{nextState = NOT_PRESSED;}		//stay in current stare
				break;
				
				
			case PRESSED:
				if ((buttonBus==0) && (timeElapsed < 20)){ //if button released before debounce time, ignore input
					nextState = NOT_PRESSED;
				}
				else if (buttonBus == 0){ 	//if button is released after 20ms 
					startTime = 0;						//reset time
					timeElapsed = 0;					//reset time
					longPress_CNT_ms = 0;			//reset long press counter if button was released before 2 seconds
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
				if((lastBus & 1) == 1){	//check if blue button was pressed
					currentCellState = !currentCellState;						//get initial cell state
					bluePressData.noteNum = 0; 											//reset note to start sound effect
					
				}else{
					dPadPressData.noteNum = 0; //reset note to start sound effect
					frame[0][cursorY][cursorX] = currentCellState; //set cell state before cursor moves
					
					//move cursor position on LED matrix
					cursorX = (cursorX + (-1*(lastBus & (1u<<3u))>>3u) + (1*(lastBus&(1u<<4u))>>4u)) & 0xF; //move left of right
					cursorY = (cursorY + (-1*(lastBus & (1u<<1u))>>1u) + (1*(lastBus&(1u<<2u))>>2u)) & 0x7; //move up or down
					
					currentCellState = frame[0][cursorY][cursorX];	//once moved, store initial cell state for cursor position
					frame[0][cursorY][cursorX] = 1;									//keep light on in cursor location to show cursor
				}
				nextState = NOT_PRESSED;													//set next state to NOT_PRESSED once debounce is complete
				break;
		}
		currentState = nextState;															//update current state
		
	}
}
