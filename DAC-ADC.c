#include "DAC-ADC.h"


void init_DAC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;			//DAC port clock enable
	DAC_port->MODER|=(3u<<(2*DAC_pin));			//DAC output pin set as anaglogue
	
	RCC->APB1ENR|=RCC_APB1ENR_DACEN;				//DAC clock enable
	DAC->CR|=DAC_CR_EN2;										//DAC 2 enabled
}

void output_dac(unsigned short data)
{
	DAC->DHR12R2=(data & 0x0FFF);			//write data 12 bits to DAC 2 output register
}
//void waveSelector(unsigned short currentSample){
//	extern unsigned short frequency, waveType;
//	unsigned short timePeriod, timeRemainder;
//	timePeriod	= SAMPLE_RATE/frequency;
//	//timeRemainder = SAMPLE_RATE%frequency;
//	
//	
//	switch(waveType){
//		case 0:
//			break;
//		
//		case 1:
//			sawWave(currentSample, timePeriod);
//			break;
//	
//		case 2:
//			triangleWave(currentSample, timePeriod);
//			break;
//		
//		case 3:
//			sineWave(currentSample, timePeriod);
//			break;
//	}
//}

void sawWave(unsigned short sampleNo, unsigned short period){

	unsigned short Vout = sampleNo*(0xFFF/period);
	output_dac(Vout);
}

void triangleWave(unsigned short sampleNo, unsigned short period){
	unsigned short Vout = 2*(sampleNo*(0xFFF/period)) & 0xFFF;
	
	if (Vout > 0x7FF){			//check which part of the wave we're in
		Vout = (0x7FF-(Vout - 0x7FF));
	}
	Vout *=2;
	output_dac(Vout);
}

void sineWave(unsigned short sampleNo, unsigned short period){
	unsigned short dacVal = 0;
	
	dacVal = 2047*(sin(6.28319*(period)*sampleNo)+1);
	
	output_dac(dacVal);
}

void init_ADC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;	//GPIOC clock enable
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;	//GPIOA clock enable
	ADC_LDR_input_port->MODER|=(3u<<(2*ADC_LDR_input_pin));	//ADC LDR input pin is analogue mode
	ADC_Pot_input_port->MODER|=(3u<<(2*ADC_Pot_input_pin));	//ADC Pot input pin is analogue mode
	
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;		//ADC clock enable
	ADC1->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC1->SQR3|=ADC_Pot_Channel;				//set channel
	ADC1->CR2|=ADC_CR2_ADON;						//enable ADC
	
	
}


unsigned short read_adc(void)
{
	ADC1->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC1->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC1->DR;									//return converted value
}

unsigned short get_ADC_Voltage(){
	unsigned short level = read_adc();
	unsigned short voltage = 0;
	voltage = ((mVref * level)/ADC_res)/100;
	return voltage;
}
