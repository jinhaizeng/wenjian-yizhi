#ifndef _LCD_H
#define _LCD_H
#include "sys.h"
#include "stm32f4xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//LCD��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define LCD_LED(n)              (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))   //LCD����PD13              //LCD����PD13

//LCD LTDC��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			//ˮƽͬ�����
	u16 vsw;			//��ֱͬ�����
	u16 hbp;			//ˮƽ����
	u16 vbp;			//��ֱ����
	u16 hfp;			//ˮƽǰ��
	u16 vfp;			//��ֱǰ�� 
	u8 activelayer;		//��ǰ����:0/1	
	u8 dir;				//0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 

extern _ltdc_dev lcdltdc;		            //����LCD LTDC����


#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  

#endif 
