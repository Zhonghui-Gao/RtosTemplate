/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     LCKFB-LP    first version
 */
 
 #ifndef __BSP_UART_H__
 #define __BSP_UART_H__
 
 #include "stm32f10x.h"
 
 
 /* 串口缓冲区的数据长度 */
#define USART1_RECEIVE_LENGTH  1024

extern uint8_t  u1_recv_buff[USART1_RECEIVE_LENGTH]; // 接收缓冲区
extern uint16_t u1_recv_length;                     // 接收数据长度
extern uint8_t  u1_recv_flag;                       // 接收完成标志位
 
 
 //外部可调用函数的声明
void uart1_init(uint32_t __Baud);
void USART1_IRQHandler(void);
void uart1_receive_clear(void);
uint8_t *uart1_get_data(void);
 
 
 
#endif
