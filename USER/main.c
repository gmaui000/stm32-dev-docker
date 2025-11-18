#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

extern vu16 var_Exp;
int main(void)
{
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    uart_init(9600);
    TIM2_PWM_Init(999,7199);
    TIM3_PWM_Init(9999,7199);

    while(1)
    {
        // LED = 0;
        // delay_ms(500);
        // LED = 1;
        // delay_ms(500);
        
        // // 添加串口测试输出
        // printf("Test message from main loop\r\n");
    }
}
