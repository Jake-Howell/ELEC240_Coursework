#include 	<stm32f4xx.h>
#include 	"Delay.h"
#include 	"lcd.h"
#include 	"LED.h"
#include	"usart.h"
#include	"SevenSeg_Display.h"
#include	"buzzer.h"
#include	"LED_Matrix.h"
#include 	"DAC-ADC.h"
#include 	"PLL_Config.c"

#include 	"GameOfLife.h"

FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{x=x;}

struct _SWITCH_DATA switchData;
struct _ADC_DATA adcData;


char user_name[6];

unsigned int PWM_Timing_WhiteLight	= 100;	//allow program to update TMark for PWM of white LED

enum games{GOL = 0, SNAKE, FLAPPY_BIRD};



void loadingBar();
unsigned int gameMenu();

int main(){
	PLL_Config();
	SystemCoreClockUpdate();
	
	__disable_irq();
	
	Init_LEDs();
	//timer 2 used for sound and ticks every 0.5 ms
	Init_Timer2(45000, 0xFFFFFFFF, DISABLE_ISR);
	//timer 3 is used for creating delays and ticks every us
	Init_Timer3(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ISR);
	//Timer 4 used to toggle green LED 
	Init_Timer4_GreenFlash(PSC_100ms, (2*ARR_100ms), ENABLE_ISR); //5Hz interupt for onboard Green LED
	Init_Timer5_WhitePWM(920, 1000,ENABLE_ISR);
	Init_Timer7_ADC(PSC_1us, 21, ENABLE_ISR);											//interupt every 21 us for 48Ksps
	
	//buzzer uses Timer 1 for PWM to create sound
	init_buzzer();
	init_LCD();									//initalise LCD in 4bit mode
	init_USART(115200);					//initalise USART with 115200 baud rate
	Init_Dpad();								//initalise DPad buttons
	Init_BlueButton();					//initalise the User button
	init_SevenSeg();						//initalise the 7 seg display
	init_ADC();									//initalise the ADC
	init_DAC();
		
	__enable_irq();							//after all initalisation, turn on interupts
	
	//set initial values to switch data
	switchData.A = 0;
	switchData.B = 0;
	switchData.C = 0;
	switchData.D = 0;
	switchData.Blue = 0;
	switchData.BlueLongPress = 0;
	
	updateLCD("test",0);
	//loadingBar();
	move_usart_cursor(0,3);
	clear_usart(CURSOR_TO_BEGINING,SCREEN);
	format_usart_text("Hello World\n\r", RED_TEXT, BLUE_BKG);
	

	startGameOfLife();
	while(1){
		
		

		switch(gameMenu()){
			case GOL:
				readySteadyGo();
				startGameOfLife();
			
				break;
			case SNAKE:
				break;
			case FLAPPY_BIRD:
				break;
		}
	}

	while(1){__NOP();}
}

void loadingBar(){
	
	//fill line with custom chars
	char printedLine[16] = {0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4};
	
	LCD_CLR();									//clear LCD
	updateLCD("Loading...",0);	//print Loading to first line of LCD

	
	updateLCD(printedLine,1);		//print first frame of animation
	Wait3_ms(250);							//wait 250ms for next frame
	
	for (int i=1; i<16; i++){		//itterate until packman eats all the food
		
		printedLine[i-1] = ' ';		//clear the last character
		
		printedLine[i] = 0x3;			//display open packman in position i
		updateLCD(printedLine,1);	//printedLine frame
		Wait3_ms(250);						//wait 250ms for next frame		
		
		printedLine[i] = 0x2;			//replace packman open with packman closed
		updateLCD(printedLine,1);	//printedLine next frame
		Wait3_ms(250);						//wait 250ms for next frame		
	}
	LCD_CLR();
}

unsigned int gameMenu(){
	enum debounceState {NOT_PRESSED = 0, PRESSED, DEBOUNCED};	//create enum states for switch case
	unsigned int buttonState = NOT_PRESSED;	//set initial value to button state
	unsigned int startTime = TIM3->CNT, timeElapsed; 	//timer variables
	_Bool up, down, blueButton;												//set names for buttons
	unsigned short buttonBus, lastBus;
	short gameNum = 0;
	
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
				}else if(gameNum < 0){
					gameNum = 2;
				}
				//update the LCD to show game's name
				updateLCD(gameList[gameNum],1);
				buttonState = NOT_PRESSED;
				
				break;
		}	
	}
		
		
	
	return gameNum;
}
