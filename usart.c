#include "usart.h"


void move_usart_cursor(int x, int y){	//controle cursor location relative to cursors last position
	
	if (y>0){									//if y is posative
		printf("\x1b[%dA", y);	//use move up code and move by y
	}	
	else if(y<0){							//if y is negative
		y = y -(2*y); 					//make y posative
		printf("\x1b[%dB", y);	//use move down code and move by y
	}
	if (x>0){									//if x is posative 
		printf("\x1b[%dC", x);	//use move right code and move by x
	}
	else if(x<0){							//if x is negative
		x = x - (2*x);					//make x posative
		printf("\x1b[%dD", x);	//use move left code and move by x
	}
}

void format_usart_text(char string[], int txt_colour, int bkg_colour){	//print string to console with text colour and background colour
	
	//set text colour, then set background colour, print string, then unset background colour and text colour
	printf("\x1b[%dm\x1b[%dm%s\x1b[%dm\x1b[%dm", txt_colour, bkg_colour, string, RESET_COLOUR, BLACK_BKG);
	
}

void clear_usart(int clear, _Bool clearType){	//clear selected parts of screen and lines
	if (clearType == LINE){
		printf("\x1b[%dK",clear);									//if clear line is set, use whatever code to act on only the current line
	}else if (clearType == SCREEN){
		printf("\x1b[%dJ",clear);									//if clear screen is set, use whatever code to act on the whole screen
	}
}



void usart_print(char string[]){										//take a string and print each character
	int length = 0;																		
	for (length = 0; string[length]!='\0'; length++);	//finding length of char array
	for (int i = 0; i < length; i++){									//itterate through given string
		send_usart(string[i]);													//print each char in array
	}
}

void send_usart(unsigned char d)
{
	while(!(USART_MODULE->SR & USART_SR_TC));	//wait until data register is not in use
	USART_MODULE->DR=d;												//write byte to usart data register
}
char receive_usart(void){
	while(!(USART_MODULE->SR & 0x0020)){ 			//check if Receive Not Empty flag is set
		__NOP();
	}
	return USART_MODULE->DR;						 			//Return contense of status register
}

void usart_get_string(char string[], unsigned int sizeOfString){
	printf("%c[32m",0x1B);
	char c;
	for (int i = 0; i < sizeOfString; i++){	//stop before string is out of index
		
		c = receive_usart();						//get next character
		if (c == '\r'){break;}					//exit for loop when user hits enter
		if ((c > 0x1F)&&(c < 0x7F)){		//check that character is within ASCII limits (from 'Space' to '~')
			string[i] = c;								//add character to string if it's valid
		}else if(c == 0x7F){
			string[(i-1)] = 0;						//if back space detected, delet last char in string
			i = i-2;											//move back to deleted char position (accounting for addition in next loop)
		}
		else{i --;}											//if back space detected, move back in loop
		
		send_usart(c);									//echo character back to screen so user can see what they're typing
	}
	printf("%c[0m",0x1B);
	printf("\r\n");										//move cursor to next line
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
	//NVIC_EnableIRQ(USART3_IRQn);										//enable USART3 Interrupt																								
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

//void USART3_IRQHandler(void){
//		USART_MODULE->SR &= ~(0x0020);	//clear the interupt flag
//		char c = USART_MODULE->DR;
//		usart_print("Char Interupt Recived: ");
//		send_usart(c);
//		usart_print("\n\r");
//}