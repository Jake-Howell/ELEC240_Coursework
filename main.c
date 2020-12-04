#include 	<stm32f4xx.h>
#include 	"Delay.h"
#include 	"lcd.h"
#include 	"gpio.h"
#include	"usart.h"
#include	"SevenSeg_Display.h"
#include	"buzzer.h"
#include	"LED_Matrix.h"
#include 	"GlobalDef.h"
#include 	"PLL_Config.c"

#include 	"GameOfLife.h"

FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{x=x;}

enum games{GOL = 0, SNAKE, FLAPPY_BIRD};

unsigned int gameMenu();

int main(){
	PLL_Config();
	SystemCoreClockUpdate();
	Init_LEDs();
	
	Init_Timer2(45000, 0xFFFFFFFF, DISABLE_ISR);
	Init_Timer3(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ISR);
	init_LCD();
	init_USART();
	Init_Dpad();
	Init_BlueButton();
	init_SevenSeg();
	Set_B(LD1,1);
	
	
	while(1){
		switch(gameMenu()){
			case GOL:
				startGameOfLife();
			
				break;
			case SNAKE:
				break;
			case FLAPPY_BIRD:
				break;
		}
	}
//	for (int i = 0; i<100; i++){
//		set_SevenSeg(i);
//		Wait3_ms(50);
//	}
//	updateLCD("testing 123",0);
//	lcdLocate(1,0);
//	
//	for (int i=0; i<16; i++){putLCD(0x04);}
//	lcdLocate(1,0);
//	for (int i=0; i<16; i++){
//		if (i>0){
//			lcdLocate(1,(i-1));
//			putLCD(' ');
//		}
//		lcdLocate(1,i);
//		putLCD(0x02);
//		Wait3_ms(250);
//		lcdLocate(1,i);
//		putLCD(0x03);
//		Wait3_ms(250);
//	}
//	
//	printf("testing 132");
	while(1){__NOP();}

}

unsigned int gameMenu(){
	enum debounceState {NOT_PRESSED = 0, PRESSED, DEBOUNCED};	//create enum states for switch case
	unsigned int buttonState = NOT_PRESSED;	//set initial value to button state
	unsigned int startTime = TIM3->CNT, timeElapsed; 	//timer variables
	_Bool up, down, blueButton;												//set names for buttons
	unsigned short buttonBus, lastBus;
	unsigned short gameNum = 0;
	
	char gameList[3][16] = {
		"Game Of Life    ",
		"Snake           ",
		"Flappy Bird     ",
		};
	
	updateLCD("Please Scroll", 0);
	updateLCD("with A and C", 1);
	
		
	while(1){
		//check values of buttons and merg them into buttonBus
		up 					= checkButton('G', Dpad_A);
		down 				= checkButton('G', Dpad_C);
		blueButton 	= checkButton('C', USER_BUTTON);
		buttonBus = (blueButton<<0)| (up<<1)|(down<<2);
		
		timeElapsed = (TIM3->CNT - startTime)&0xFFFF;	//work out time elapsed since start time was last set
		switch(buttonState){
			
			case NOT_PRESSED:
				if (buttonBus != 0){
					lastBus = buttonBus;
					buttonState = PRESSED;
					startTime = TIM3->CNT;
				}else{buttonState = NOT_PRESSED;}
				break;
			
			case PRESSED:
				if ((buttonBus == 0) && timeElapsed < 10000){ //if button is released beofre 10 ms
					buttonState = NOT_PRESSED;									//not a real press so go back to start
				}else if (buttonBus == 0){										//if button is released after 10 ms
					buttonState = DEBOUNCED;										//button is debounced, go to next state
				}
				break;
			
			case DEBOUNCED:
				if((lastBus & 1) == 1){ // check if blueButton was pressed
					return gameNum;				//select current game
					
				}else if ((lastBus & (1<<1)) > 0){ // check if up was pressed
					gameNum++; //move up in game list
					
				}else if ((lastBus & (1<<2)) > 0){ //check if down was pressed
					gameNum--; //move down in game list
				}
				lastBus = 0; //reset last bus
				//clamping for gameNum to make sure it doesn't index outside games list
				if (gameNum > 2){
					gameNum = 0;
				}
				//update the LCD to show game's name
				updateLCD(gameList[gameNum],1);
				buttonState = NOT_PRESSED;
				
				break;
		}	
	}
		
		
	
	return gameNum;
}
