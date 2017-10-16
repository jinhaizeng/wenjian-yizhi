#include "SD_TEST.h"


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
//测试SD卡的读取
//从secaddr地址开始,读取seccnt个扇区的数据
//secaddr:扇区地址
//seccnt:扇区数
void sd_test_read(u32 secaddr,u32 seccnt)
{
	u32 i; u8 *buf; u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//申请内存
	sta=SD_ReadDisk(buf,secaddr,seccnt);//读取secaddr扇区开始的内容
	if(sta==0)						
	{	 
		printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)printf("%x ",buf[i]);//打印secaddr开始的扇区数据    	   
		printf("\r\nDATA ENDED\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);	//释放内存	   
}
//测试SD卡的写入(慎用,最好写全是0XFF的扇区,否则可能损坏SD卡.)
//从secaddr地址开始,写入seccnt个扇区的数据
//secaddr:扇区地址
//seccnt:扇区数
void sd_test_write(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf; u8 sta=0; 
	buf=mymalloc(SRAMEX,seccnt*512);		//从SDRAM申请内存
	for(i=0;i<seccnt*512;i++) buf[i]=i*3; 		//初始化写入的数据,是3的倍数.
	sta=SD_WriteDisk(buf,secaddr,seccnt);		//从secaddr扇区开始写入seccnt个扇区内容
	if(sta==0) printf("Write over!\r\n");		  
else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);					//释放内存	   
}
