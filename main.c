#include 	<stm32f4xx.h>
#include 	"Delay.h"
#include 	"lcd.h"
#include 	"gpio.h"
#include	"usart.h"
#include	"SevenSeg_Display.h"
#include	"buzzer.h"
#include	"LED_Matrix.h"
#include 	"GlobalDef.h"
#include 	"PLL_Config.c"

#include 	"GameOfLife.h"

FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{x=x;}

int main(){
	PLL_Config();
	SystemCoreClockUpdate();
	Init_LEDs();

	Init_Timer2(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ISR);
	init_LCD();
	init_USART();
	init_SevenSeg();
	init_buzzer();
	Set_B(LD1,1);
//	Set_B(LD2,1);
//	Set_B(LD3,1);
	
	
//	lcdLocate(0,0);
//	putLCD('A');
//	//Wait2_ms(100);
//	putLCD('B');
//	Wait2_ms(100);
//	putLCD('C');
//	Wait2_ms(100);
//	putLCD('D');
//	//Wait2_ms(200);
	
	startGameOfLife();

//	for (int i = 0; i<100; i++){
//		set_SevenSeg(i);
//		Wait2_ms(50);
//	}
//	updateLCD("testing 123",0);
//	lcdLocate(1,0);
//	
//	for (int i=0; i<16; i++){putLCD(0x04);}
//	lcdLocate(1,0);
//	for (int i=0; i<16; i++){
//		if (i>0){
//			lcdLocate(1,(i-1));
//			putLCD(' ');
//		}
//		lcdLocate(1,i);
//		putLCD(0x02);
//		Wait2_ms(250);
//		lcdLocate(1,i);
//		putLCD(0x03);
//		Wait2_ms(250);
//	}
//	
//	printf("testing 132");
	while(1){__NOP();}

}
