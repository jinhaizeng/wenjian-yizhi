#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f4xx_hal.h"
#include "math.h"
#include "lcd.h"

extern UART_HandleTypeDef huart2;           //UART1�������ƵĶ���
void Error_Handler(void);
/*********************************************************************************************
* @brief  �������Ƶ��ܿ���
* @param
*
* @param
* @retval
*/
#define key_flag 1          //����ɨ��ĺ�����1Ϊ����0Ϊ��
#endif
