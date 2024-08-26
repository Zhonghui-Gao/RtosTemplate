/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     LCKFB-LP    first version
 */
 
#include "bsp_uart.h" 
#include "stdio.h"

uint8_t  u1_recv_buff[USART1_RECEIVE_LENGTH]; // ���ջ�����
uint16_t u1_recv_length;                      // �������ݳ���
uint8_t  u1_recv_flag;                        // ������ɱ�־λ

void uart1_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_9;//TX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_10;//RX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//�������ô��ڵĽṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//��������1��ʱ��

	USART_DeInit(USART1);//�����˼�ǽ���˴��ڵ���������

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//���ò�����
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//�ֽڳ���Ϊ8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//û��У��λ
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//����������Ϊ�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //���ṩ���� 
	USART_Init(USART1,&USART_InitStructure);//����ز�����ʼ��������1
	
	USART_ClearFlag(USART1,USART_FLAG_RXNE);//��ʼ����ʱ���������λ
	USART_ClearFlag(USART1,USART_FLAG_IDLE);//��ʼ����ʱ���������λ

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��ʼ���ý����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//��ʼ���ÿ����ж�

	USART_Cmd(USART1,ENABLE);//��������1
	
	NVIC_InitTypeDef NVIC_InitStructure;//�жϿ��ƽṹ���������

	NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;//�ж�ͨ��ָ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//ȷ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�����ô��ж�ͨ��
		
}

#if !defined(__MICROLIB)
//��ʹ��΢��Ļ�����Ҫ�������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
//�����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
	while( RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE) ){}
	
    return ch;
}

/************************************************
�������� �� uart1_receive_clear
��    �� �� 
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LC
*************************************************/
void uart1_receive_clear(void)
{
	u1_recv_length = 0;
	u1_recv_flag = 0;
}

/************************************************
�������� �� uart1_get_data
��    �� �� �ж������Ƿ��յ����յ��򷵻������׵�ַ
��    �� �� ��
�� �� ֵ �� ���ջ��������׵�ַ
��    �� �� LC
*************************************************/
uint8_t *uart1_get_data(void)
{
	if( u1_recv_flag == 1 )
	{
		uart1_receive_clear();
		return u1_recv_buff;
	}	
	
	return NULL;
}


/******** ����1 �жϷ����� ***********/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//�ж��ǲ�������н����жϷ���
	{
		u1_recv_buff[u1_recv_length++] = USART_ReceiveData(USART1); // �ѽ��յ������ݷŵ���������
		
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); //�Ѿ�����������־λ 
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)//�ж��ǲ�������п����жϷ���
	{
		volatile uint32_t temp;
		temp = USART1->SR; // ��ȡ״̬�Ĵ��������IDLE��־
		temp = USART1->DR; // ��ȡ���ݼĴ��������IDLE��־
		
		u1_recv_buff[u1_recv_length] = '\0';
		u1_recv_flag = 1;
		
		// ���������������û���κ��ã�����
		// USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}


