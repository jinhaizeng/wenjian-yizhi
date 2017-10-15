#ifndef __SRAM_H
#define __SRAM_H

/****************************Í·ÎÄ¼þ********************************/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "lcd.h"
/************************************************************/
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n);
void fsmc_sram_test(u16 x,u16 y);


#endif
