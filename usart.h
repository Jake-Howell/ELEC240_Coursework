#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>


#define USART_MODULE	USART3	//define USART module
#define USART_PORT		GPIOD		//define which port USART will use
#define USART_TX_pin	8				//define transmission pin
#define USART_RX_pin	9				//define recieve pin


#define LINE 		 0
#define SCREEN 	 1

enum colours{
	RESET_COLOUR,
	BLACK_TEXT = 30,
	RED_TEXT,
	GREEN_TEXT,
	YELLOW_TEXT,
	BLUE_TEXT,
	MAGENTA_TEXT,
	CYAN_TEXT,
	WHITE_TEXT,
	
	BLACK_BKG = 40,
	RED_BKG,
	GREEN_BKG,
	YELLOW_BKG,
	BLUE_BKG,
	MAGENTA_BKG,
	CYAN_BKG,
	WHITE_BKG
};

enum clearScreenCodes{
	CURSOR_TO_END = 0,
	CURSOR_TO_BEGINING,
	CLEAR_ALL	
};


int fputc(int ch, FILE *f);		
int fgetc(FILE *f);


void usart_print(char string[]);
void send_usart(unsigned char d);
char receive_usart(void);
void usart_get_string(char string[], unsigned int sizeOfString);
void init_USART(unsigned int baudRate);

void format_usart_text(char string[], int txt_colour, int bkg_colour);
void move_usart_cursor(int x, int y);
void clear_usart(int clear, _Bool clearType);

#endif
