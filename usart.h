#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>

#define USART_MODULE	USART3
#define USART_PORT		GPIOD
#define USART_TX_pin	8
#define USART_RX_pin	9
#define BAUDRATE			9600

int fputc(int ch, FILE *f);
int fgetc(FILE *f);


void usart_print(char string[]);
void send_usart(unsigned char d);
char receive_usart(void);
void init_USART(void);

#endif
