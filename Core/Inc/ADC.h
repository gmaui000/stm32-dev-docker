#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"	// STM32F4 library
#include <stdint.h>

void pa1_adc_init();
uint32_t adc_read(void);
void start_adc_conversion(void);

#endif /* ADC_H_ */
