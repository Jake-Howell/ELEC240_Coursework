#include "LED_Matrix.h"

void init_matrix(){
	//look at code on page 248 of STM32 programming book
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;	//enable SPI3 Clock
	
	//set pins to alternate function
	GPIOB->MODER &= ~((3u<<(2*IOE))|(3u<<(2*LE)));
	GPIOB->MODER |=	((1u<<(2*IOE))|(1u<<(2*LE)));
	
	GPIOC->MODER &= ~((3u<<(2*SCK))|(3u<<(2*MOSI)));
	GPIOC->MODER |= ((2u<<(2*SCK))|(2u<<(2*MOSI)));
	
	GPIOC->AFR[1]	&= ~(0x000F0F00);	//clear alt function for pins 10 and 12
	GPIOC->AFR[1]	|= (0x00060600);	//set alt function to SPI3 on pins 10 and 12
	
	SPI3->CR1 = 0x334;	//set bits 3 and 2 to master	bits 5,4,3 is baud rate ctr 1MHz
	SPI3->CR2 =	0;
	SPI3->CR1|=	0x40;		//SPI3 Enable
	
	//GPIOB->BSRR = (1u<<(IOE+16));
}

//set data on bus
void matrix_write(unsigned short rowData, char row){
	rowData = rowData & 0xFFFF;	//truncate to 16 bits
	
	char rightByte 	= 	((rowData>>8) & 0xFF);
	char leftByte		= 	(rowData & 0xFF);
	
	while(!(SPI3->SR & 0x2)){__NOP();}
	GPIOB->BSRR = 0x40;									//Setting the latch (pin 6)
	SPI3->DR = rightByte;								//write data to SPI data register
	while(!(SPI3->SR & 0x2)){__NOP();}
	SPI3->DR = leftByte;								//write data to SPI data register
	while(!(SPI3->SR & 0x2)){__NOP();}
	SPI3->DR = (row & 0x7);							//write data to SPI data register
	
	while(SPI3->SR & 0x80){__NOP();}
	GPIOB->BSRR = 0x00400000;						//Resetting the latch (pin 6)			
	//SPI3->DR =	0;
}

//compile row data into short
unsigned short setRow(unsigned short rowNum, _Bool data[8][16]){
	unsigned short rowData = 0;
	for (int i = 0; i<16; i++){
		rowData|= ((data[7-rowNum][i])<<i);
	}
	return rowData;
}

//keep displaying current frame
void matrix_display(_Bool frame[8][16]){
	for (int rowNum=0; rowNum<8; rowNum++){
		matrix_write(setRow(rowNum,frame),rowNum);
	}
}

void updateFrame(_Bool lastFrame[8][16],unsigned short dx, unsigned short dy){
	_Bool nextFrame[8][16];
	
	for(int y=0; y<8; y++){
		for(int x=0; x<16; x++){
			nextFrame[x][y]=lastFrame[((y+dy)&0x7)][((x+dx)&0xFF)];	
		}
	}
	for(int y=0; y<8; y++){
		for(int x=0; x<16; x++){
			lastFrame[x][y]=nextFrame[x][y];	
		}
	}
}