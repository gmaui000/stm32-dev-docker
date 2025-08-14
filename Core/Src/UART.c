#include "UART.h"
#define GPIOAEN   		(1U<<0) // Enable clock for GPIOA
#define USART1EN  		(1U<<4)	// USART1  @ GPIOA : PA09 & PA10

#define SYS_FREQ		16000000
#define APB2_CLK		SYS_FREQ

#define CR1_TE			(1U<<3)
#define CR1_RE 			(1U<<2)
#define CR1_UE			(1U<<13)

#define SR_TXE			(1U<<7)
#define SR_RXNE			(1U<<5)

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate);
void usart1_write(int ch);
int _write(int file, char *ptr, int len);
// int __io_putchar();

/* __io_putchar is a printf() lower level
 * function call that is specific for stm32cubeIDE
 * */

//int __io_putchar(int ch){ // All correct arguments are passed by printf
//	usart1_write(ch);
//	return ch;
//}

/* _write is a weak function called when using printf()
 * It is called before __io_putchar but is used in more general newlib hence prefered
 * */


int _write(int file, char *ptr, int len) { // All correct arguments are passed by printf
    for (int i = 0; i < len; i++) {
        usart1_write(ptr[i]);
    }
    return len;
}

void usart1_tx_init(void){
	/************Configure USART GPIO pins***********/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA09 to alternate function mode*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set PA09 & PA10 to alternate function type to UART_TX (AF7)*/
	/* Notes :
	 * 	- Pin number corresponds to x in AFRHx
	 * 	- All pin 1 are in AFRL1
	 * 	- All pin 9 are in AFRH9
	 */

	/* Note : There is no AFRL/AFRH in ST header, instead
	 * AFR[0] = AFRL
	 * AFR[1] = AFRH
	 */

	GPIOA->AFR[1] &= ~(0xFU<<4);
	GPIOA->AFR[1] |=  (0x7U<<4);

	/************Configure USART Module***********/
	/*Enable clock access to USART1*/
	RCC->APB2ENR |= USART1EN;

	/*Configure USART Baud rate*/
	uart_set_baudrate(USART1,APB2_CLK,UART_BAUDRATE);

	/*Configure transfer direction*/
	/* For transmission */
	USART1->CR1 = CR1_TE;

	/*Enable USART module*/
	USART1->CR1 |= CR1_UE;
}

void usart1_rxtx_init(void){
	/************Configure USART GPIO pins***********/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA09 & PA10 to alternate function mode*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set PA10 to alternate function mode*/
	GPIOA->MODER &= ~(1U<<20);
	GPIOA->MODER |= (1U<<21);

	/*Set PA09 & PA10 to alternate function type to UART_TX (AF7)*/
	/* Notes :
	 * 	- Pin number corresponds to x in AFRHx
	 * 	- All pin 1 are in AFRL1
	 * 	- All pin 9 are in AFRH9
	 */

	/* Note : There is not AFRL/AFRH in ST header, instead
	 * AFR[0] = AFRL
	 * AFR[1] = AFRH
	 */

	GPIOA->AFR[1] &= ~(0xFU<<4);
	GPIOA->AFR[1] |=  (0x7U<<4);

	GPIOA->AFR[1] &= ~(0xFU<<8);
	GPIOA->AFR[1] |=  (0x7U<<8);

	/************Configure USART Module***********/
	/*Enable clock access to USART1*/
	RCC->APB2ENR |= USART1EN;

	/*Configure USART Baud rate*/
	uart_set_baudrate(USART1,APB2_CLK,UART_BAUDRATE);

	/*Configure transfer direction*/
	/* For transmission & receiver*/
	USART1->CR1 = (CR1_TE | CR1_RE);

	/*Enable USART module*/
	USART1->CR1 |= CR1_UE;
}

char usart1_read(void){
	/*Make sure receive data register is not empty*/
	while(!(USART1->SR & SR_RXNE)){}

	/*Read transmitted data register*/
	return USART1->DR;
}

void usart1_write(int ch){
	/* Make sure transmit data register is empty*/
	while(!(USART1->SR & SR_TXE)){}

	/* Write to transmit data register*/
	USART1->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate)
{
    double mantissa;
    double fraction;
    uint16_t USARTDIV_MANT;
    uint16_t USARTDIV_FRAC;

    /* Check page: 981 ,section 30.3.4 in reference manual of STM32F429ZI
     * Using a value of OVER8 = 0 the formula posed in Equation #1 reduces to:
     * USARTDIV = SYS_FREQ / 16 * baud
     * Hence,
     * USARTDIV = 16000000 / 16 * 115200 (SYS_FREQ = 16Mhz & baud rate = 115200)
     * USARTDIV = 8.6806
     *
     * Check page: 1013, section:30.6.3 in reference manual of STM32F429ZI
     * This makes the mantissa portion of USARTDIV = 8
     * This makes the fractional portion of USARTDIV .6806
     *
     * Turning .6806 into a 16 bit number: .6806 * 16 = 10.8896 which
     * rounded to the next whole number is = 11
     *
     * The mantissa binary value (8) is: 0000 0000 1000 (bits 11:0 of the USART_BRR)
     * The fractional binary value (11) is: 1011 (bits 3:0 of the USART_BRR)
     * Bottom Line: We need to set the USART_BRR to:
     * 0b 0000 0000 0000 0000 0000 0000 1000 1011 or 0x8b
     * Doing this pro-gramatically as follows:
     */

    /* Step #1: Calculate the mantissa */
    mantissa = (double)periph_clk / (double)(baudrate * 16);

    /* Step #2: Calculate the fraction */
    fraction = ((mantissa - ((long)mantissa))) * 16;

    /* Step #3: Round the fraction double up if fractional portion of fraction is > .50 */
    if (fraction - ((long)fraction) >= .5)
        fraction++;

    /* Step #4: store the mantissa into a uint16_t value (USARTDIV_MANT) */
    USARTDIV_MANT = mantissa;

    /* Step #5: store the fraction into a unit16_t value (USARTDIV_FRAC) */
    USARTDIV_FRAC = fraction;

    /* Step #6: set the 1st 4 bits [3:0] of the USART_BRR register to USARTDIV's fraction */
    USARTx->BRR = (USARTDIV_FRAC << 0);

    /* Step #7: set the next 12 bits [11:0] of the USART_BRR to USARTDIV's mantissa */
    USARTx->BRR |= (USARTDIV_MANT << 4);
}
