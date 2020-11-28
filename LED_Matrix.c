#include "LED_Matrix.h"

void init_matrix(){
	//look at code on page 248 of STM32 programming book
	
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;	//enable SPI3 Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;// enable GPIOB clock
	
	//set pins as digital output
	GPIOB->MODER &= ~((3u<<(2*IOE))|(3u<<(2*LE)));
	GPIOB->MODER |=	((1u<<(2*IOE))|(1u<<(2*LE)));
	
	GPIOC->MODER &= ~((3u<<(2*SCK))|(3u<<(2*MOSI)));
	GPIOC->MODER |= ((2u<<(2*SCK))|(2u<<(2*MOSI)));
	
	
	GPIOB->AFR[1] &= ~(0x000F0000); //clear alt function for pin 12
	GPIOB->AFR[1] |= (0x00060000);	//set alt function to SPI3 on pin 12
	
	GPIOC->AFR[1]	&= ~(0x000F0F00);	//clear alt function for pins 10 and 12
	GPIOC->AFR[1]	|= (0x00060600);	//set alt function to SPI3 on pins 10 and 12
	
	SPI3->CR1 =		0x334;	//SET bits 3 and 2 to master
	SPI3->CR2 =		0;
	SPI3->CR1 |=	0x40;		//SPI3 enable
	
}