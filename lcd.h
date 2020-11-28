#ifndef _LCD_H_
#define _LCD_H_


//LCD meta data
#define LCD_WIDTH		16 								//LCD 16 char wide
#define LCD_LINE1		0x80							//LCD DDRAM address for line 1 
#define LCD_LINE2		0xc0							//LCD DDRAM address for line 2
#define LCD_D0_pin	0									//micro controller pin connected to LCD D0 pin
#define LCD_D4_pin	4
#define BUSY_FLAG		(LCD_D0_pin + 7)	//micro controller pin connected to LCD busy flag
#define LCD_PORT	GPIOD								//micro controller port LCD is connected to
#define LCD_RS_pin	11								//micro controller pin connected to LCD Register Select pin
#define LCD_RW_pin	12								//micro controller pin connected to LCD Read/Write pin
#define LCD_E_pin		13								//micro controller pin connected to LCD Enable pin
#define LCD_4_BIT		0x20							//put LCD in 4 bit mode
#define LCD_BACKLIGHT 14

//LCD comands

#define LCD_CLR()		cmdLCD(0x01)		//clear whole display


//LCD Register Presets
#define set_LCD_RS()	LCD_PORT->BSRR=(1u<<LCD_RS_pin)					//Reg Select = 1
#define clr_LCD_RS()	LCD_PORT->BSRR=(1u<<(LCD_RS_pin+16))		//Reg Select = 0
#define set_LCD_RW()	LCD_PORT->BSRR=(1u<<LCD_RW_pin)					//Read/Write = 1
#define clr_LCD_RW()	LCD_PORT->BSRR=(1u<<(LCD_RW_pin+16))		//Read/Write = 0
#define set_LCD_E()		LCD_PORT->BSRR=(1u<<LCD_E_pin)					//Enable 		 = 1
#define clr_LCD_E()		LCD_PORT->BSRR=(1u<<(LCD_E_pin+16))			//Enable 		 = 0
#define set_BackLight()	LCD_PORT->BSRR= (1u<<LCD_BACKLIGHT)
#define clr_BackLight() LCD_PORT->BSRR= (1u<<LCD_BACKLIGHT + 16)



#define set_LCD_bus_input()		LCD_PORT->MODER&=~(0xFFFF<<(2*LCD_D0_pin))	//change LCD bus to 4 bit input mode
#define set_LCD_bus_output()	LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin))		//change LCD bus to 4 bit output mode

#include <stm32f4xx.h>

//void lcd_delayus(unsigned int us);
void WaitLcdBusy(void);
void set_LCD_data(unsigned char data);
void LCD_strobe(void);
void cmdLCD(unsigned char cmd);
void lcdLocate(int row, int col);
void putLCD(unsigned char put);
void initLCD(void);
void updateLCD(char line[LCD_WIDTH],int LineNo);
void loadCustomChars();
#endif
