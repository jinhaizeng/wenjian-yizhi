#include "SD_TEST.h"


//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}
//����SD���Ķ�ȡ
//��secaddr��ַ��ʼ,��ȡseccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_read(u32 secaddr,u32 seccnt)
{
	u32 i; u8 *buf; u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//�����ڴ�
	sta=SD_ReadDisk(buf,secaddr,seccnt);//��ȡsecaddr������ʼ������
	if(sta==0)						
	{	 
		printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)printf("%x ",buf[i]);//��ӡsecaddr��ʼ����������    	   
		printf("\r\nDATA ENDED\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);	//�ͷ��ڴ�	   
}
//����SD����д��(����,���дȫ��0XFF������,���������SD��.)
//��secaddr��ַ��ʼ,д��seccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_write(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf; u8 sta=0; 
	buf=mymalloc(SRAMEX,seccnt*512);		//��SDRAM�����ڴ�
	for(i=0;i<seccnt*512;i++) buf[i]=i*3; 		//��ʼ��д�������,��3�ı���.
	sta=SD_WriteDisk(buf,secaddr,seccnt);		//��secaddr������ʼд��seccnt����������
	if(sta==0) printf("Write over!\r\n");		  
else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);					//�ͷ��ڴ�	   
}
