/*	Find where USB B connection is connected with board via UART/USART
 *	Board : STM32F429ZI
 * 	This USB communicates via Virtual COMM Port through USART1
 * 	USART1 address : 0x4001 1000 - 0x4001 13FF
 * 	TX_Port: Port A
 * 	TX_Pin: 9
 *
 *	RX_Port: Port A
 *	RX_Pin: 10
 *
 *	Address Bus : APB2 90 MHZ @ USART1
 *
 * 	USART1 uses GPIOA : PA09(TX) PA10(RX)
 * 	So,
 * 		- need to map with alternate function mapping in Data sheet
 * 		- find type of alternate function (AF7)
 */
#include <stdio.h>
#include "ADC.h"
#include "UART.h"
#include "systick.h"
#include "LED.h"
#include "timer.h"

uint32_t sensor_value;

int main(void) {
    LED_init();                      // Ensure your LED is initialized
    tim2_output_pa5_compare();      // Starts TIM2 in output compare mode

    while (1) {
        if (TIM2->SR & TIM_SR_CC1IF) {
            TIM2->SR &= ~TIM_SR_CC1IF;  // Clear compare match flag
            LED_toggle(0);               // Blink LED every 1s
        }
    }
}
