#include "SWITCH.h"


void Init_Dpad(void){
	__disable_irq();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;						//enable clock for port G
	
	DPAD_PORT->MODER &= ~((3<<(2*Dpad_A)) | (3<<(2*Dpad_B)) | (3<<(2*Dpad_C))| (3<<(2*Dpad_D)));		//set pins as inputs
	DPAD_PORT->PUPDR &= ~((3u<<(2*Dpad_C)) | (3u<<(2*Dpad_D)));																			//clear PUPD settings
	DPAD_PORT->PUPDR |= (2u<<(2*Dpad_C)) | (2u<<(2*Dpad_D));						//set pull down resistor to ground floating values
	
	SYSCFG->EXTICR[0] &= ~(0xFFFF);	//clear selection
	SYSCFG->EXTICR[0] |= (0x6666);	//select port C
	
	
	EXTI->IMR 	|= 0x000F;						//
	EXTI->RTSR 	|= 0x000F;						//set trigger to rising edge
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	//NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	__enable_irq();
}


void Init_BlueButton(void){
	//PORT C13
	__disable_irq();
	RCC->APB2ENR |= 0x4000;	//ENABLE SYSCFG clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	//enable port C clock
	
	USER_BUTTON_PORT->MODER &= ~((3u<<(2*USER_BUTTON))); // set Blue button as input
	
	SYSCFG->EXTICR[3] &= ~0x00F0;
	SYSCFG->EXTICR[3] |= 0x0020;
	
	EXTI->IMR  |= 0x2000;
	EXTI->RTSR |= 0x2000;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();
	
}

void EXTI0_IRQHandler(void){
	
	EXTI->PR = 0x0001;//something;
}
void EXTI1_IRQHandler(void){
	
	EXTI->PR = 0x0002;//something;
}
void EXTI2_IRQHandler(void){
	
	EXTI->PR = 0x0004;//something;
}
void EXTI3_IRQHandler(void){
	
	EXTI->PR = 0x0008;//something;
}


//blue button interupt handeler
void EXTI15_10_IRQHandler(void){
	extern struct _SWITCH_DATA switchData;
	
	Wait3_us(1000);
	unsigned int startTime = TIM2->CNT;
	if(checkButton('C',USER_BUTTON) != 0){ //checkButton if button is still pressed after debounce
		switchData.Blue = 1;		//set Blue switch flag in switch data
	}
	
	EXTI->PR = 0x2000; 		//CLEAR_BIT interrupt flag
	return;
}


_Bool checkButton(char port, unsigned short pin){
	unsigned short state;
	
	switch(port){
		case 'A':																			//select port A
			state = ((GPIOA->IDR & (1u<<(pin)))>>pin);	//check state of pin on port A
			break;
		case 'B':																			//select port B
			state = ((GPIOB->IDR & (1u<<(pin)))>>pin);	//check state of pin on port B
			break;
		case 'C':																			//select port C
			state = ((GPIOC->IDR & (1u<<(pin)))>>pin);	//check state of pin on port C
			break;
		case 'D':																			//select port D
			state = ((GPIOD->IDR & (1u<<(pin)))>>pin);	//check state of pin on port D
			break;
		case 'E':																			//select port E
			state = ((GPIOE->IDR & (1u<<(pin)))>>pin);	//check state of pin on port E
			break;
		case 'F':																			//select port F
			state = ((GPIOF->IDR & (1u<<(pin)))>>pin);	//check state of pin on port F
			break;
		case 'G':																			//select port G
			state = ((GPIOG->IDR & (1u<<(pin)))>>pin);	//check state of pin on port G
			break;
	}
	
	return state;																		//return state of selected pin
}





