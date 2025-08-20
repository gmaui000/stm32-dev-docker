/**
 * @file uart.c
 * @author rohirto
 * @brief UART Module
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "main.h"
#include "uart.h"


UART_HandleTypeDef huart1;

uint8_t RX1_Char = 0x00;

//FreeRTOS Stuff
//Global variables
uint8_t pcDebugBuffer[50];
QueueHandle_t xDebugQueue;                //Queue to Debug the bsl comm and application layer
SemaphoreHandle_t xUART1Mutex;            // Mutex used to safeguard UART 1 Tx for debugging purpose
//TaskHandle_t xDebug_Handle = NULL;               //task handle of prvDebug_Task
QueueHandle_t xDebugQueue;                //Queue to Debug the bsl comm and application layer
SemaphoreHandle_t xDebugQueueMutex;        // Mutex for safeguarding writing of debug Queue



/**
* @brief Debug_Print 
* @param pointer to message string
* @retval None
* Function to print debug messages sent from different tasks
* 
*/
void Debug_Print (unsigned char *pcMessage)
{    

    
  //bsl_UART_Transmit(&huart1, (uint8_t*) pcMessage, strlen((const char *)pcMessage));
  HAL_UART_Transmit_IT(&huart1, (uint8_t*) pcMessage, strlen((const char *)pcMessage));
   
}

void prvDebug_Task (void* pvParameters)
{
    //BSLDebug_Struct xTemp;      //Temporary variable to store queue data
    unsigned char* pcMessage;

    for(;;)
    {
        if(xQueueReceive(xDebugQueue, &pcMessage, 0) == pdPASS)
        {
            /* Look up the event code */
            Debug_Print( pcMessage );
            
            /* Lower its priority*/
           // vTaskPrioritySet(NULL, 1);
        }
        else
        {
            /* Lower its priority*/
            //vTaskPrioritySet(NULL, 1);
        }
        
    }
}


void USART1_UART_Init(void)
{
    
    

    // GPIO_InitStruct.Pin = GPIO_PIN_9;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // GPIO_InitStruct.Pin = GPIO_PIN_10;
    // GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    
    
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (huart->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}

//---------[ UART Data Reception Completion CallBackFunc. ]---------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(&huart1, &RX1_Char, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    // Data transmission completed
    
  }
}