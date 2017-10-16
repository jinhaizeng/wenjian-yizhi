#ifndef __SD_TEST_H
#define __SD_TEST_H
/***************************Í·ÎÄ¼þ*********************************/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "lcd.h"
#include "sd.h"
#include "malloc.h"
/************************************************************/

void show_sdcard_info(void);
void sd_test_read(u32 secaddr,u32 seccnt);
void sd_test_write(u32 secaddr,u32 seccnt);
#endif

