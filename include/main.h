/**
 * @file main.h
 * @author rohirto
 * @brief Main Header File
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"


void Error_Handler(void);
void SystemClock_Config(void);

#endif