/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-08-25     Gao-Zh    first version
 */

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "stm32f10x_it.h"
#include "main.h"

#define  LED_RED_GPIO      GPIOC
#define  LED_RED_PIN       GPIO_Pin_5   

#define  LED_BLUE_GPIO     GPIOB
#define  LED_BLUE_PIN      GPIO_Pin_2

//c 5 
void LED_BLUE_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_RED_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(LED_RED_GPIO, LED_RED_PIN);
}

//b 2
void LED_RED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_BLUE_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(LED_BLUE_GPIO, LED_BLUE_PIN);
}

void vTaskBlueLED(void *pvParameters)
{
    (void) pvParameters;

    while(1)
    {
        // 切换 LED 状态
        GPIO_WriteBit(LED_BLUE_GPIO, LED_BLUE_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_BLUE_GPIO, LED_BLUE_PIN)));

        // 延时 1 秒
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskRedLED(void *pvParameters)
{
    (void) pvParameters;

    while(1)
    {
        // 切换 LED 状态
				GPIO_WriteBit(LED_RED_GPIO, LED_RED_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_RED_GPIO, LED_RED_PIN)));
				vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{
    // 配置系统时钟
    board_init();

    // 初始化 LED
    LED_BLUE_Init();
		LED_RED_Init();

    // 创建 LED 闪烁任务
    xTaskCreate(vTaskBlueLED, "LED BLUE Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(vTaskRedLED, "LED RED Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // 启动调度器
    vTaskStartScheduler();

    // 如果启动调度器失败，将会执行到这里
    while(1);
}

