#ifndef __SD_H
#define __SD_H

/*****************************头文件*******************************/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "sdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "string.h"

/***************************声明*********************************/
#define SD_TIMEOUT      ((uint32_t)100000000)       //超时时间
#define SD_DMA_MODE     1	                        //1：DMA模式，0：查询模式  

/******************************函数声明******************************/
u8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);
u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt);
u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt);





#endif


