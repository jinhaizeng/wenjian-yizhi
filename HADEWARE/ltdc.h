#ifndef _LCD_H
#define _LCD_H
#include "sys.h"
#include "stm32f4xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//LCD驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define LCD_LED(n)              (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))   //LCD背光PD13              //LCD背光PD13

//LCD LTDC重要参数集
typedef struct  
{							 
	u32 pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;			//水平同步宽度
	u16 vsw;			//垂直同步宽度
	u16 hbp;			//水平后廊
	u16 vbp;			//垂直后廊
	u16 hfp;			//水平前廊
	u16 vfp;			//垂直前廊 
	u8 activelayer;		//当前层编号:0/1	
	u8 dir;				//0,竖屏;1,横屏;
	u16 width;			//LCD宽度
	u16 height;			//LCD高度
	u32 pixsize;		//每个像素所占字节数
}_ltdc_dev; 

extern _ltdc_dev lcdltdc;		            //管理LCD LTDC参数


#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//用户修改配置部分:

//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000  

#endif 
