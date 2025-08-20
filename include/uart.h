/**
 * @file uart.h
 * @author rohirto
 * @brief UART Header
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __UART_H
#define __UART_H

#include "main.h"


extern UART_HandleTypeDef huart1;
extern QueueHandle_t xDebugQueue;                //Queue to Debug the bsl comm and application layer
//extern TaskHandle_t xDebug_Handle;
extern SemaphoreHandle_t xUART1Mutex;
extern uint8_t pcDebugBuffer[50];
extern SemaphoreHandle_t xDebugQueueMutex;        // Mutex for safeguarding writing of debug Queue

//Macros
//Macros
/* This Macro takes the debug queue mutex, writes to debug queue and then releases the mutex */
#define Debug_Mutex()               /* Store the address of string buffer (to be printed) in pointer variable */\
                                    debug_messagePtr = pcDebugBuffer;\
                                    /* Write to Queue*/\
                                    xQueueSendToBack(xDebugQueue, &debug_messagePtr, 0 );\
                                    /* Give back the Mutex */
                                

void prvDebug_Task (void* pvParameters);
void Debug_Print (unsigned char *pcMessage);
void USART1_UART_Init(void);
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);



#endif