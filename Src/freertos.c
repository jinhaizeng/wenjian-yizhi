/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "uart.h"
#include "usart.h"
#include "lcd.h"
#include "sdio.h"
#include "key.h"
#include "sram.h"
#include "sdio.h"
/* USER CODE END Includes */

/************************************************************/
u32 testsram[250000] __attribute__((at(0X68000000)));//测试用数组

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId Task_LEDHandle;

/* USER CODE BEGIN Variables */
osThreadId Task_usartHandle;
osThreadId Task_WIFIHandle;
osThreadId Task_SRAMHandle;
//osThreadId Task_TESTHandle;
/* USER CODE END Variables */
//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}
/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void Func_LED(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void Func_usart(void const * argument);
void Func_WIFI(void const * argument);
void Func_SRAM(void const * argument);
//void Func_TEST(void const * argument);
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_LED */
  osThreadDef(Task_LED, Func_LED, osPriorityNormal, 0, 128);
  Task_LEDHandle = osThreadCreate(osThread(Task_LED), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(Task_usart, Func_usart, osPriorityNormal, 0, 128);
  Task_usartHandle = osThreadCreate(osThread(Task_usart), NULL);
  
  osThreadDef(Task_WIFI, Func_WIFI, osPriorityNormal, 0, 128);
  Task_WIFIHandle = osThreadCreate(osThread(Task_WIFI), NULL);
  
  osThreadDef(Task_SRAM, Func_SRAM, osPriorityNormal, 0, 128);
  Task_SRAMHandle = osThreadCreate(osThread(Task_SRAM), NULL);
  
//  osThreadDef(Task_TEST, Func_TEST, osPriorityNormal, 0, 128);
//  Task_TESTHandle = osThreadCreate(osThread(Task_TEST), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Func_LED function */
void Func_LED(void const * argument)
{
  /* USER CODE BEGIN Func_LED */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
    osDelay(500);
    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
    osDelay(500);
    
    osDelay(1);
  }
  /* USER CODE END Func_LED */
}

/* USER CODE BEGIN Application */
void Func_usart(void const * argument)
{
  /* USER CODE BEGIN Func_LED */
  
	u8 len;
  POINT_COLOR=RED;//设置字体为红色 
  
	LCD_ShowString(30,50,200,16,16,"Aopllo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/10/15");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 0");
  /* Infinite loop */
  for(;;)
  {
    if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
    osDelay(1);
    
  }
  
  /* USER CODE END Func_LED */
}
 

void Func_WIFI(void const * argument)
{
  /* USER CODE BEGIN Func_LED */
	u8 length;
  /* Infinite loop */
  for(;;)
  {
//    if(!MX_SDIO_SD_Init())
//    {
//      //show_sdcard_info();	//打印SD卡相关信息
//      POINT_COLOR=BLUE;	//设置字体为蓝色 
//      //检测SD卡成功 											    
//      LCD_ShowString(30,150,200,16,16,"SD Card OK    ");
//      LCD_ShowString(30,170,200,16,16,"SD Card Size:     MB");
//      LCD_ShowNum(30+13*8,170,SDCardInfo.CardCapacity>>20,5,16);//显示SD卡容量	
//      
//    }
    if(USART3_RX_STA&0x8000)
		{					   
			length=USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			HAL_UART_Transmit(&huart3,(uint8_t*)USART3_RX_BUF,length,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n\r\n");//插入换行
			USART3_RX_STA=0;
		}
    //HAL_UART_Transmit(&huart3,(uint8_t*)m ,3,1000);
    osDelay(1);
    
  }
}
void Func_SRAM(void const * argument)
{
  u32 ts=0;
  u8 key;
  for(ts=0;ts<2500;ts++)
  {
     testsram[ts]=ts;//预存测试数据     
  } 
  
  for(;;)
  {
    
    key=KEY_Scan(0);//不支持连按	
    
    if(key==KEY0_PRES)fsmc_sram_test(60,170);//测试SRAM容量
		else if(key==KEY1_PRES)//打印预存测试数据
		{
			for(ts=0;ts<250000;ts++)LCD_ShowxNum(60,190,testsram[ts],6,16,0);//显示测试数据	 
		}
    
    osDelay(10);   
   
  }
}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
