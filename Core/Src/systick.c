#include <stdio.h>
#include <stdlib.h>
#include "systick.h"

#define SYSTICK_LOAD_VALUE		(16000)
#define SYSCLK					(16000000)
#define SYST_CTRL_ENABLE		(1U<<0)
#define SYST_CTRL_CLKSOURCE		(1U<<2)
#define SYST_CTRL_COUNT			(1U<<16)

void systickDelayMs(int delay){
	/****Configure SysTick****/
	/*Reload with number of clocks per milliseconds*/
	SysTick->LOAD = SYSTICK_LOAD_VALUE;

	/*Clear SysTick current value*/
	SysTick->VAL = 0;

	/*Enable SysTick and select internal clock source*/
	SysTick->CTRL = SYST_CTRL_ENABLE | SYST_CTRL_CLKSOURCE;

	for(int i=0;i<delay;i++){
		/*Wait until COUNTFLAG is set*/
		while((SysTick->CTRL & SYST_CTRL_COUNT) == 0){}
	}
	SysTick->CTRL = 0;
}


