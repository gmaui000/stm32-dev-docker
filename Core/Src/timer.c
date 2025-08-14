/* Implementing timer function
 *
 * Datasheet Block diagram : TIM2 APB1 bus @ 45 MHz
 * Default Crystal MHz = 16MHz
 * */

#include "timer.h"
#define TIM2EN	 	(1U<<0)
#define SYSCLK		16000000
#define TIM2_CR_CEN	(1U<<0)
#define OC_TOGGLE 	((1U << 4) | (1U << 5))
#define CCER_CC1E	(1U << 0)

/*Time out ever 1 second*/
void tim2_1hz_init(void){
	/*Enable clock access*/
	RCC->APB1ENR |= TIM2EN;

	/* While setting value do n-1 since counting starts from 0*/
	/*Set Pre-scaler value*/
	TIM2->PSC = 1600 - 1; // 16000000/1600 = 10000

	/*Set Auto reload value*/
	TIM2->ARR =  10000 -1; // 10000/10000 = 1

	/*CLear Timer*/
	TIM2->CNT = 0;

	/*Enable Timer*/
	TIM2->CR1 |= TIM2_CR_CEN;
}

/*Time out ever 1 second*/
void tim2_output_pa5_compare(void){
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U << 0);

	/*set PA5 to alternate function mode*/
	GPIOA->MODER &= ~(2U << (5 * 2));
	GPIOA->MODER |= (1U << 11); 

	/*Set PA5 to AF01 (TIM2_CH1)*/
	GPIOA->AFR[0] &= ~(0xF << (10 * 2));
	GPIOA->AFR[0] |= (1U << (10 * 2));

	/*Enable clock access*/
	RCC->APB1ENR |= TIM2EN;

	/* While setting value do n-1 since counting starts from 0*/
	/*Set Pre-scaler value*/
	TIM2->PSC = 1600 - 1; // 16000000/1600 = 10000

	/*Set Auto reload value*/
	TIM2->ARR =  10000 -1; // 10000/10000 = 1

	/*Set output compare toggle mode*/
	TIM2->CCMR1 = (1U << 4) | (1U << 5); 

	/*Enable tim2 ch1 in compare mode*/
	TIM2->CCER |= CCER_CC1E; // Enable channel 1

	/*CLear Timer*/
	TIM2->CNT = 0;

	/*Enable Timer*/
	TIM2->CR1 |= TIM2_CR_CEN;
}
