#include "SevenSeg_Display.h"
#include "Delay.h"

void init_SevenSeg(){
	
	//enable GPIO E clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	
	//Clear pin modes before setting them
	SEVENSEG_PORT->MODER &= 
	~((3u<<(2*D0))
	|(3u<<(2*D1))
	|(3u<<(2*D2))
	|(3u<<(2*D3))
	|(3u<<(2*D4))
	|(3u<<(2*D5))
	|(3u<<(2*D6))
	|(3u<<(2*D7))
	|(3u<<(2*LE1))
	|(3u<<(2*LE2))
	|(3u<<(2*IOE)));
	
	//setting GPIOE pins as outputs
	SEVENSEG_PORT->MODER |= 
	((1u<<(2*D0))
	|(1u<<(2*D1))
	|(1u<<(2*D2))
	|(1u<<(2*D3))
	|(1u<<(2*D4))
	|(1u<<(2*D5))
	|(1u<<(2*D6))
	|(1u<<(2*D7))
	|(1u<<(2*LE1))
	|(1u<<(2*LE2))
	|(1u<<(2*IOE)));
}

void set_SevenSeg(int num){
		updateTens(num/10);		//only write tens digit
		updateOnes(num%10);		//only write ones digit
}

void updateTens(int tens){
		blank_seg();
		SEVENSEG_PORT->BSRR = numberGen(tens); //display
		clr_seg2();		//deselect ones digit
		set_seg1();		//select tens digit
}
void updateOnes(int ones){
		blank_seg();														//clear previous number on bus
		SEVENSEG_PORT->BSRR = numberGen(ones); 	//display new number on bus
		set_seg2();		//deselect ones digit
		clr_seg1();		//select tens digit
}

int numberGen(int n){
	int output = 0;
	//selecting which pins to turn on for specified number
	switch(n){
		case 0:
			output |= (1u<<D0)|(1u<<D1)|(1u<<D3)|(1u<<D4)|(1u<<D5)|(1u<<D6);
			break;
		case 1:
			output |= (1u<<D0)|(1u<<D1);
			break;
		case 2:
			output |= (1u<<D1)|(1u<<D2)|(1u<<D3)|(1u<<D4)|(1u<<D5);
			break;
		case 3:
			output |= (1u<<D2)|(1u<<D3)|(1u<<D4)|(1u<<D5)|(1u<<D6);
			break;
		case 4:
			output |= (1u<<D0)|(1u<<D2)|(1u<<D5)|(1u<<D6);
			break;
		case 5:
			output |= (1u<<D0)|(1u<<D2)|(1u<<D3)|(1u<<D4)|(1u<<D6);
			break;
		case 6:
			output |= (1u<<D0)|(1u<<D1)|(1u<<D2)|(1u<<D3)|(1u<<D4)|(1u<<D6);
			break;
		case 7:
			output |= (1u<<D4)|(1u<<D5)|(1u<<D6);
			break;
		case 8:
			output |= (1u<<D0)|(1u<<D1)|(1u<<D2)|(1u<<D3)|(1u<<D4)|(1u<<D5)|(1u<<D6);
			break;
		case 9:
			output |= (1u<<D0)|(1u<<D2)|(1u<<D4)|(1u<<D5)|(1u<<D6);
			break;
		default:
			//set output to make seven seg blank if number isn't 0-9
			output = 0x8FFC0000;		
	}
	//return number to be set on bus
	return output;
}
