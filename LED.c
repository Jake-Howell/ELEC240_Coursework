#include "LED.h"

void Init_LEDs(void){
	//ENABLE PORT(S)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	//ONLY GPIO B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	//ONLY GPIO C clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;	//ONLY GPIO F clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	//enable LD1 LD2 LD3 as outputs(PB0, PB7, PB14)
	GPIOB->MODER |= (1<<(2*LD1))|(1<<(2*LD2))|(1<<(2*LD3));										
	GPIOB->MODER &= ~((0x2<<(0x2*LD1))|(0x2<<(0x2*LD2))|(0x2<<(0x2*LD3)));


	//CONFIGURE PORT PIN FUNCTIONS
	//enable TRAFF LEDs as outputs (PC2, PC3, PC6, PC7, PC8, PC9)
	//TRAFF_1
	GPIOC->MODER |= (1<<(2*TRAF_RED1))|(1<<(2*TRAF_YEL1))|(1<<(2*TRAF_GEN1));
	GPIOC->MODER &= ~((2<<(2*TRAF_RED1))|(2<<(2*TRAF_YEL1))|(2<<(2*TRAF_GEN1)));
	
	//TRAFF_2
	GPIOC->MODER |= (1<<(2*TRAF_RED2))|(1<<(2*TRAF_YEL2))|(1<<(2*TRAF_GEN2));	
	GPIOC->MODER &= ~((2<<(2*TRAF_RED2))|(2<<(2*TRAF_YEL2))|(2<<(2*TRAF_GEN2)));
	
	//GREEN_MAN
	GPIOF->MODER |= (1<<(2*GREEN_MAN));
	GPIOF->MODER &= ~(2<<(2*GREEN_MAN));
	
	//this register only uses 16 bits (1 bit per pin)
	//set to PUSH/PULL (reset state)
	GPIOC->OTYPER &= ~((1<<(TRAF_RED1))|(1<<(TRAF_YEL1))|(1<<(TRAF_GEN1)));
	//SET to open drain(set state)
	GPIOC->OTYPER |= ((1<<(TRAF_RED2))|(1<<(TRAF_YEL2))|(1<<(TRAF_GEN2)));
	GPIOF->OTYPER |= (1<<GREEN_MAN);
	//set Initial values to turn off LEDs
	GPIOC->BSRR = 0x004C0380;
	GPIOF->ODR |= (1<<GREEN_MAN);
}


void Toggle_B(int B_PIN){
	GPIOB->ODR^=(1u<<B_PIN);								//XOR GPIOB output data register to invert the selected pin
}

void Toggle_C(int C_PIN){
	GPIOC->ODR^=(1u<<C_PIN);
}

void Set_B(int B_PIN, _Bool state){
	if (state == 1){
		GPIOB->ODR |= (1u<<B_PIN);
	}
	else{
		GPIOB->ODR &= ~(1u<<B_PIN);
	}
}

void Set_C(int C_PIN, _Bool state){
	if (state == 1){
		GPIOC->ODR |= (1u<<C_PIN);
	}
	else{
		GPIOC->ODR &= ~(1u<<C_PIN);
	}
}

void durationIndicatorLight(unsigned int duration){
	TIM4->ARR = 1000/duration;
}

void readySteadyGo(){
	lightController(0,0,0,0,1,0,0);
	play_note(NOTE_C5);
	Wait3_ms(100);
	play_note(SILENT);
	Wait3_ms(350);
	lightController(0,0,0,0,1,1,0);
	play_note(NOTE_C5);
	Wait3_ms(100);
	play_note(SILENT);
	Wait3_ms(350);
	lightController(0,0,0,0,0,0,1);
	play_note(NOTE_A5);
	Wait3_ms(500);
	play_note(SILENT);
}


void morse_dot(){
	Set_B(LD2, 1);	//turn on blue LED
	Wait3_ms(250);	//hold for 250ms to represent a dot
	Set_B(LD2, 0);	//turn blue LED off
	Wait3_ms(250);	//hold 250ms to represent char space
}
void morse_dash(){
	Set_B(LD2, 1);	//turn on blue LED
	Wait3_ms(500);	//hold for half a second to represent a dash
	Set_B(LD2, 0);	//turn off blue LED
	Wait3_ms(250);	//hold 250ms to represent char space
}

void convertToMorse(unsigned int digit){
	
	switch (digit){
		case 0:	//---- = 0 in morse code
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dash();
			break;
		
		case 1:	//	.---- 	= 1 in morse code
			morse_dot();
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dash();
			break;
		case 2:	//	..--- 	= 2 in morse code
			morse_dot();
			morse_dot();
			morse_dash();
			morse_dash();
			morse_dash();
			break;
		case 3: //	...-- 	= 3 in morse code
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dash();
			morse_dash();
			break;
		case 4: //	....- 	= 4 in morse code
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dash();
			break;
		case 5: //	..... 	= 5 in morse code 
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dot();
			break;
		case 6: //	-.... 	= 6 in morse code
			morse_dash();
			morse_dot();
			morse_dot();
			morse_dot();
			morse_dot();
			break;
		case 7: //	--... 	= 7 in morse code
			morse_dash();
			morse_dash();
			morse_dot();
			morse_dot();
			morse_dot();
			break;
		case 8: //	---.. 	= 8 in morse code
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dot();
			morse_dot();
			break;
		case 9: //	----. 	= 9 in morse code
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dash();
			morse_dot();
			break;
	
	}

}

void morseCodeScore(unsigned int score){
	unsigned int hundreds, tens, units;

	if (score >=100){	//dont convert hundreds if there arn't any
		hundreds 	= score/100;			//calculate hundreds digit
		convertToMorse(hundreds);
	}
	if (score >=10){	//dont convert tens if there arnt any
		tens = (score%100)/10;			//calculate tens digit
		convertToMorse(tens);
	}
	units = ((score%100)%10);			//calculate units digit
	convertToMorse(units); //converts units regardless of what the score is
	
	
	
	
	
}


void lightController(_Bool R1, _Bool Y1, _Bool G1,_Bool GM, _Bool R2, _Bool Y2, _Bool G2){
	
	GPIOC->BSRR = 0x004C0380; //004C to reset TRAF1 bits, 0380 to set all TRAF2 bits as they are inverted
	GPIOF->BSRR = 0x00000400;	//0400 to set GREEN_MAN as it is inverted (because of open drain)
	int RYG1 = (R1<<TRAF_RED1)|(Y1<<TRAF_YEL1)|(G1<<TRAF_GEN1);
	int RYG2 = (R2<<(TRAF_RED2))|(Y2<<(TRAF_YEL2))|(G2<<(TRAF_GEN2));
	int tot = RYG1+(RYG2<<16);
	GPIOC->BSRR = tot;
	GPIOF->BSRR = (GM<<(GREEN_MAN + 16)); // turn inverted LED on
}
