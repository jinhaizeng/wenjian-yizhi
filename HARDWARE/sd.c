#include "sd.h"

/************************************************************/
//SD����дר��buf������д���������ݻ���������4�ֽڶ����ʱ����Ҫ�õ������飬ȷ�����ݻ�������ַ��4�ֽڶ����
__align(4) u8 SDIO_DATA_BUFFER[512];

/****************************title****************************
�������ƣ�SD_GetCardInfo
�������ܣ��õ�����Ϣ
��ڲ���: ardinfo:����Ϣ�洢��
���ڲ���:����״̬
˵����
****************************************************************/
u8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo)
{
    u8 sta;
    sta=HAL_SD_Get_CardInfo(&hsd,cardinfo);
    return sta;
}

#if (SD_DMA_MODE==1)        //DMAģʽ

//ͨ��DMA��ȡSD��һ������
//buf:�����ݻ�����
//sector:������ַ
//blocksize:������С(һ�㶼��512�ֽ�)
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;
u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0;
    
    err=HAL_SD_ReadBlocks_DMA(&hsd,buf,sector,blocksize,cnt);//ͨ��DMA��ȡSD��һ������
    if(err==0)//��ȡ�ɹ�
    {
        //�ȴ���ȡ���
        err=HAL_SD_CheckReadOperation(&hsd,(uint32_t)SD_TIMEOUT);
    }
    
    return err;
}

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//blocksize:������С(һ�㶼��512�ֽ�)
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;	
u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0; 
    
    err=HAL_SD_WriteBlocks_DMA(&hsd,buf,sector,blocksize,cnt);//ͨ��DMAдSD��һ������
    if(err==0)//д�ɹ�
    {
        //�ȴ���ȡ���/
       err=HAL_SD_CheckWriteOperation(&hsd,(uint32_t)SD_TIMEOUT);
    }
    
    return err;
}

//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;
u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt)
{
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    if(SDCardInfo.CardType!=STD_CAPACITY_SD_CARD_V1_1)lsector<<=9;
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            sta=SD_ReadBlocks_DMA((uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);
            memcpy(buf,SDIO_DATA_BUFFER,512);
            buf+=512;
        }
    }else
    {
        sta=SD_ReadBlocks_DMA((uint32_t*)buf,lsector, 512,cnt);
    }
    return sta;
}  

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt)
{   
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    if(SDCardInfo.CardType!=STD_CAPACITY_SD_CARD_V1_1)lsector<<=9;
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            memcpy(SDIO_DATA_BUFFER,buf,512);
            sta=SD_WriteBlocks_DMA((uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//����sector��д����
            buf+=512;
        }
    }else
    {
        sta=SD_WriteBlocks_DMA((uint32_t*)buf,lsector,512,cnt);//���sector��д����
    }
    return sta;
} 

//SDMMC1�жϷ�����
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&hsd);
}

#else                                   //��ѵģʽ

//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;
u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt)
{
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    lsector<<=9;
    INTX_DISABLE();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            sta=HAL_SD_ReadBlocks(&hsd,(uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//����sector�Ķ�����
            memcpy(buf,SDIO_DATA_BUFFER,512);
            buf+=512;
        }
    }else
    {
        sta=HAL_SD_ReadBlocks(&hsd,(uint32_t*)buf,lsector,512,cnt);//����sector�Ķ�����
    }
    INTX_ENABLE();//�������ж�
    return sta;
}  

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt)
{   
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    lsector<<=9;
    INTX_DISABLE();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            memcpy(SDIO_DATA_BUFFER,buf,512);
            sta=HAL_SD_WriteBlocks(&hsd,(uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//����sector��д����
            buf+=512;
        }
    }else
    {
        sta=HAL_SD_WriteBlocks(&hsd,(uint32_t*)buf,lsector,512,cnt);//���sector��д����
    }
    INTX_ENABLE();//�������ж�
    return sta;
} 
#endif


