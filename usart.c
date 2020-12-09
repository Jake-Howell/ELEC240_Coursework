#include "usart.h"

void usart_print(char string[]){
	int length = 0;
	for (length = 0; string[length]!='\0'; length++);	//finding length of char array
	for (int i = 0; i < length; i++){									//printing each char in array
		send_usart(string[i]);
	}
}

void send_usart(unsigned char d)
{
	while(!(USART_MODULE->SR & USART_SR_TC));
	USART_MODULE->DR=d;												//write byte to usart data register
}
char receive_usart(void){
	while(!(USART_MODULE->SR & 0x0020)){ //check if Receive Not Empty flag is set
		__NOP();
	}
	return USART_MODULE->DR;						 //Return contense of status register
}
void init_USART(unsigned int baudRate)
{
	__disable_irq();
	unsigned char i1,i2;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;		//usart port clock enable
	
	USART_PORT->MODER&=~(										//clear pin function bits
		(3u<<(2*USART_TX_pin))
		|(3u<<(2*USART_RX_pin))
			);
	USART_PORT->MODER|=(										//reset pin function bits (alternate function)
		(2u<<(2*USART_TX_pin))
		|(2u<<(2*USART_RX_pin))
			);
	
	i1=USART_TX_pin/8;											// used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	i2=USART_RX_pin>>3;											// same as above for RX_pin (uses shift to divide)

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(USART_TX_pin-(i1*8))));    //clear pin function
	USART_PORT->AFR[i1]|=(0x07<<(4*(USART_TX_pin-(i1*8))));			// set usart as alternate function for TX_pin
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(USART_RX_pin-(i2*8))));		// clear pin function
	USART_PORT->AFR[i2]|=(0x07<<(4*(USART_RX_pin-(i2*8))));			//set usart as alternate function for RX_pin
	
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;			//usart clock enable
	USART_MODULE->CR1|=(										//USART CONFIG
			USART_CR1_TE												//transmit enable
			|USART_CR1_RE												//receive enable
			|USART_CR1_UE												//usart main enable bit
			|0x0020															//Enable interrupt on RXNE (Receive Not Empty Flag)
				);
	
	unsigned int temp  = ((((SystemCoreClock)/4)<<5)/(16*baudRate));
	unsigned int result = (temp>>1) + (temp & 0x1);
	
	USART_MODULE->BRR= result;		//set baud rate
	//USART_MODULE->BRR=0x124F;																								//this is not an accurate way to set the baudrate
																									//and only works for 16Mhz system clock.
	NVIC_EnableIRQ(USART3_IRQn);										//enable USART3 Interrupt																								
	__enable_irq();
}

int fputc(int ch, FILE *f){			//
	send_usart((uint8_t) ch);			//send individual char to usart Data register
	return ch;								
}

int fgetc(FILE *f){
	while(!(USART_MODULE->SR & 0x0020)){__NOP();} //Check Receive Not Empty bit
	uint8_t ch = USART_MODULE->DR;								//load value of Data Register
	return ch;																		//return char to rest of code
}

void USART3_IRQHandler(void){
		USART_MODULE->SR &= ~(0x0020);	//clear the interupt flag
		char c = USART_MODULE->DR;
		usart_print("Char Interupt Recived: ");
		send_usart(c);
		usart_print("\n\r");

}