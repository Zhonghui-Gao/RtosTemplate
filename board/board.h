/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     LCKFB-LP    first version
 */
 
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f10x.h"

void board_init(void);

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

void delay_1us(uint32_t us);
void delay_1ms(uint32_t ms);





#endif
