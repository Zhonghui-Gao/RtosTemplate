/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     LCKFB-LP    first version
 */
//#include "board.h"

//static uint32_t systick_us = 0;
////static uint32_t systick_ms = 0;

//void board_init(void)
//{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��
//	systick_us=SystemCoreClock/8000000;	      				//Ϊϵͳʱ�ӵ�1/8
////	systick_ms=(uint32_t)systick_us*1000;					//ÿ����SysTick��VAL������Ŀ  	
//}

//void delay_us(uint32_t us)
//{
//	uint32_t systickCount;	
//	uint32_t count = us * systick_us;
//	
//	if(us == 1)
//	{
//		SysTick->LOAD=count - 8;
//	}
//	else
//	{
//		SysTick->LOAD=count -10; 	//ʱ�����
//	}	
//	
//	SysTick->VAL=0x00;        	//��ռ�����	
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����	  
//	
//	do
//	{
//		systickCount=SysTick->CTRL;
//	}while((systickCount&0x01)&&!(systickCount&(1<<16))); //�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����		
//	
//}

//void delay_ms(uint32_t ms){ delay_us(ms * 1000); }

//void delay_1us(uint32_t us){ delay_us(us); }

//void delay_1ms(uint32_t ms){ delay_us(ms * 1000); }
#include "board.h"


void SystemClock_Config(void)
{
    // ����ϵͳʱ��Ϊ 72 MHz
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == SUCCESS)
    {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
        
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while (RCC_GetSYSCLKSource() != 0x08) {}
    }
    SystemCoreClockUpdate();  // ����ϵͳʱ��
}

void board_init(void)
{
    SystemClock_Config();  // ����ϵͳʱ��
}

