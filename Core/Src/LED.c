#include "LED.h"
#include "stm32f4xx.h"	// STM32F4 library

#define LED_PIN (1u<<13)
#define GPIOGEN (1u<<6)

void LED_init(){
	RCC->AHB1ENR |= GPIOGEN;

	GPIOG->MODER &= ~(3U<<(13*2));
	GPIOG->MODER |= (1U<<(13*2));
}

void LED_ON(){
	GPIOG->ODR |= LED_PIN;
}

void LED_OFF(){
	GPIOG->ODR &= ~LED_PIN;
}

void LED_toggle(int delay){
	GPIOG->ODR ^= LED_PIN;
	for(volatile int i=0;i<=delay;i++);
}
