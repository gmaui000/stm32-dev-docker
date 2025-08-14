/*	ADC implementation
 *	Board : STM32F429ZI
 *	Address Bus : APB2 90 MHZ @ USART1
 *
 *	ADC implementation and for this we check pin definition in data sheet
 *	ADC123_IN1 : PA1
 */
#include "ADC.h"
#define ADC1EN 				(1U<<8)
#define GPIOAEN 			(1U<<0) // Enable clock for GPIOA from AHB1 bus
#define ADC_CH1				(1U<<0)
#define ADC_SEQ_LEN_MASK  	(0xF << 20)
#define ADC_SEQ_LEN_1    	(0U  << 20)
#define CR2_ADON			(1u<<0)
#define CR2_CONT			(1u<<1)
#define CR2_SWSTART			(1u<<30)
#define SR_EOC				(1u<<1)

void pa1_adc_init(){
	/***Configure the ADC GPIOA pin***/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the pin mode to analog*/
	GPIOA->MODER &= ~(0x3 << 2); // clear PA1 bits
	GPIOA->MODER |=  (0x3 << 2); // set analog mode

	/***Configure the ADC module***/
	/*Enable clock access to ADC123_IN1*/
	RCC->APB2ENR |= ADC1EN;

	/***Configure the ADC parameters***/
	/*Conversion sequence start*/
//	ADC1->SQR3 &= ~(1U<<0); // clear bits sequence register 3
//	ADC1->SQR3 |= (1U<<0); // set channel

	ADC1->SQR3 &= ~(0x1F); // clear bits sequence register 3
	ADC1->SQR3 |= (1U); // set channel

	/*Conversion sequence length*/
	ADC1->SQR1 &= ~ADC_SEQ_LEN_MASK;
	ADC1->SQR1 |=  ADC_SEQ_LEN_1;

	/*Enable ADC module*/
	ADC1->CR2 |= CR2_ADON;
}

void start_adc_conversion(void){

	/*Enable continuous conversion*/
	ADC1->CR2 |= CR2_CONT;

	/*Start ADC conversion*/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){
	/*Wait for conversion to complete*/
	while(!(ADC1->SR & SR_EOC)){} // while loop to wait for conversion

	/*Read converted result*/
	return ADC1->DR;
}
