#ifndef _DAC_ADC_H
#define _DAC_ADC_H
#include <stm32f4xx.h>
#include "usart.h"

#define ADC_LDR_input_port		GPIOC	//define port for LDR
#define ADC_Pot_input_port		GPIOA	//define port for potentiometer

#define ADC_LDR_input_pin			0			//define LDR pin number
#define ADC_Pot_input_pin			0			//define potentiometer pin number

#define ADC_LDR_Channel				10		//define LDR chanel
#define ADC_Pot_Channel				0			//define potentiometer chanel

#define mVref 3300									//define referance voltage (in mV)
#define ADC_res 4096								//define resoloution of ADC (12-bit)
#define Vstep (mVref/ADC_res)				//define voltage step for given resoloution and Vref




#include "math.h"													
#define DAC_port	GPIOA							//define
#define DAC_pin		5
#define SAMPLE_RATE 44100

void init_DAC(void);
void output_dac(unsigned short data);

void waveSelector(unsigned short currentSample);

void sawWave(unsigned short sampleNo,unsigned short period);
void triangleWave(unsigned short sampleNo,unsigned short period);
void sineWave(unsigned short sampleNo,unsigned short period);

void init_ADC(void);								//initalise ADC
unsigned short read_adc(void);
unsigned short get_ADC_Voltage(void);

#endif
