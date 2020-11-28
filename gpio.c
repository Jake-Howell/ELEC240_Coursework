#include "gpio.h"
#include "GlobalDef.h"
/*void Init_LED(void)
{
	//ENABLE PORT(S)
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;		//GPIO B clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	GPIOB->MODER&=~(3u<<(2*0));						//clear pin functions on GPIOB
	GPIOB->MODER|=(1u<<(2*0));						//set new pin functions on GPIOB
}	*/
	
void Init_LEDs(void){
	//ENABLE PORT(S)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	//ONLY GPIO B clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	//enable LD1 LD2 LD3 as outputs(PB0, PB7, PB14)
	GPIOB->MODER |= (1<<(2*LD1))|(1<<(2*LD2))|(1<<(2*LD3));										
	GPIOB->MODER &= ~((0x2<<(0x2*LD1))|(0x2<<(0x2*LD2))|(0x2<<(0x2*LD3)));
}	

void Init_Traffic_LEDs(void){
	//ENABLE PORT(S)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	//ONLY GPIO C clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
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

void Init_BlueButton(void){
	//PORT C13
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER &= ~((1<<(2*USER_BUTTON))|(2<<(2*USER_BUTTON))); // set Blue button as input
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

void lightController(_Bool R1, _Bool Y1, _Bool G1,_Bool GM, _Bool R2, _Bool Y2, _Bool G2){
	
	GPIOC->BSRR = 0x004C0380; //004C to reset TRAF1 bits, 0380 to set all TRAF2 bits as they are inverted
	GPIOF->BSRR = 0x00000400;	//0400 to set GREEN_MAN as it is inverted (because of open drain)
	int RYG1 = (R1<<TRAF_RED1)|(Y1<<TRAF_YEL1)|(G1<<TRAF_GEN1);
	int RYG2 = (R2<<(TRAF_RED2))|(Y2<<(TRAF_YEL2))|(G2<<(TRAF_GEN2));
	int tot = RYG1+(RYG2<<16);
	GPIOC->BSRR = tot;
	GPIOF->BSRR = (GM<<(GREEN_MAN + 16)); // turn inverted LED on
}
