#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f4xx_hal.h"
#include "math.h"
#include "lcd.h"

extern UART_HandleTypeDef huart2;           //UART1串口名称的定义
void Error_Handler(void);
/*********************************************************************************************
* @brief  几个控制的总开关
* @param
*
* @param
* @retval
*/
#define key_flag 1          //按键扫描的函数，1为开，0为关
#endif
