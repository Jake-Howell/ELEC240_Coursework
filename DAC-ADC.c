#include "DAC-ADC.h"


void init_DAC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;			//DAC port clock enable
	DAC_port->MODER|=(3u<<(2*DAC_pin));			//DAC output pin set as anaglogue
	
	RCC->APB1ENR	|=RCC_APB1ENR_DACEN;			//DAC clock enable
	DAC->CR				|=DAC_CR_EN2;							//DAC 2 enabled
}

void init_ADC(void)
{
	RCC->AHB1ENR	|=RCC_AHB1ENR_GPIOCEN;	//GPIOC clock enable
	RCC->AHB1ENR	|=RCC_AHB1ENR_GPIOAEN;	//GPIOA clock enable
	
	ADC_LDR_input_port->MODER	|=(3u<<(2*ADC_LDR_input_pin));	//ADC LDR input pin is analogue mode
	ADC_Pot_input_port->MODER	|=(3u<<(2*ADC_Pot_input_pin));	//ADC Pot input pin is analogue mode
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;			//ADC clock enable
	ADC1->SQR1	&= ~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3	&= ~ADC_SQR3_SQ1;					//clear channel select bits
	ADC1->SQR3	|=  ADC_Pot_Channel;				//set channel
	ADC1->CR2		|=  ADC_CR2_ADON;						//enable ADC
}

unsigned short read_adc(unsigned int INPUT)
{
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;			//ADC clock enable
	ADC1->SQR1	&= ~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3	&= ~ADC_SQR3_SQ1;					//clear channel select bits
	switch(INPUT){
		case 0:
			ADC1->SQR3 |= ADC_LDR_Channel;
			break;
		
		case 1:
			ADC1->SQR3 |= ADC_Pot_Channel;
			break;
	}
	ADC1->CR2		|=  ADC_CR2_ADON;									//enable ADC
	ADC1->CR2|=ADC_CR2_SWSTART;										//start ADC conversion
	while((ADC1->SR & ADC_SR_EOC)==0){__NOP();}		//wait for ADC conversion complete
	return ADC1->DR;															//return converted value
}

unsigned short get_ADC_Voltage(unsigned int input){
	extern struct _ADC_DATA adcData;
	unsigned short level;
	
	if (input == LDR){
		level = adcData.ldr;
	}else {
		level = adcData.pot;
	}
	
	unsigned short voltage = 0;
	voltage = ((mVref * level)/ADC_res)/10;
	return voltage;
}

unsigned short randomNum(){ //generate a random number from 0 to 128
	unsigned int num;
	extern struct _ADC_DATA adcData;
	num = adcData.ldr * adcData.pot;		//multiply noisy signals together to generate random number
	
	return num & 0x7F; //truncate to 7 bits
}

void TIM7_IRQHandler(void){				//timer interupt for updating ADC values
	
	extern struct _ADC_DATA adcData;

	adcData.ldr = read_adc(LDR);		//read LDR value
	adcData.pot = read_adc(POT);		//read POT value
	
	adcData.sampleNo++;
	if (adcData.sampleNo > 48000){	//clamping for sample no
		adcData.sampleNo = 0;
	}
	//sineWave(adcData.sampleNo, 2400); //generate a 20Hz sine wave
	
	TIM7->SR &= ~TIM_SR_UIF;				//clear interrupt flag in status register
}

void output_dac(unsigned short data)
{
	DAC->DHR12R2=(data & 0x0FFF);					//write data 12 bits to DAC 2 output register
}

void sineWave(unsigned short sampleNo, unsigned short period){
	unsigned short dacVal = 0;
	
	dacVal = 2047*(sin(6.28319*(period)*sampleNo)+1);
	
	output_dac(dacVal);
}

