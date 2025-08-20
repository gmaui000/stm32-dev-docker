#include "main.h"
#include "uart.h"
#include "gpio.h"


// Define GPIO pin for LED
#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC

// Task function to blink the LED
void vBlinkTask(void *pvParameters)
{
  // Initialize GPIO pin for LED
  // HAL_GPIO_Init(LED_PORT, &(GPIO_InitTypeDef){LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL});
  uint8_t* debug_messagePtr;
  for (;;)
  {
    // Toggle the LED state
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

    sprintf((char *)pcDebugBuffer, "RTOS Task Toggle\r\n");
    Debug_Mutex();

    // Delay for 500ms (500 ticks when using the tick rate of 1kHz)
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

int main()
{
  HAL_Init();
  SystemClock_Config();

  blink_gpio_init();
  USART1_UART_Init();

  uint8_t rxbuff[20] = {0};

  char txData[50] = {0};
  sprintf(txData, "Clock: %d", (int)SystemCoreClock);
  uint16_t size = strlen(txData);
  HAL_UART_Transmit_IT(&huart1, txData, size);

  //HAL_UART_Receive_IT(&huart1, rxbuff, 1);

  // Create the blink task
  xDebugQueue = xQueueCreate( 10, 4);
  xDebugQueueMutex = xSemaphoreCreateMutex();
  xUART1Mutex = xSemaphoreCreateMutex();

   /* Check the queues and mutexes were created successfully. */
    if( (xDebugQueue != NULL) & (xDebugQueueMutex != NULL) &  (xUART1Mutex != NULL) )
    {
      if(xTaskCreate(vBlinkTask, "BlinkTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
      {
        //Error
        Error_Handler();
      }
      if(xTaskCreate(prvDebug_Task, "DebugTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
      {
        //Error
        Error_Handler();
      }
    }
    else
    {
      sprintf((char *)txData, "Tasks creation Failed\r\n");
      HAL_UART_Transmit_IT(&huart1, txData, strlen(txData));
    }
  

  // Start the scheduler
  vTaskStartScheduler();

  while (1)
  {

    // blink();
  }

  return 0;
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}