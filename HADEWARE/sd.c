#include "sd.h"

/************************************************************/
//SD卡读写专用buf，当读写函数的数据缓存区不是4字节对齐的时候，需要用到该数组，确保数据缓存区地址是4字节对齐的
__align(4) u8 SDIO_DATA_BUFFER[512];

/****************************title****************************
函数名称：SD_GetCardInfo
函数功能：得到卡信息
入口参数: ardinfo:卡信息存储区
出口参数:错误状态
说明：
****************************************************************/
u8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo)
{
    u8 sta;
    sta=HAL_SD_Get_CardInfo(&hsd,cardinfo);
    return sta;
}

#if (SD_DMA_MODE==1)        //DMA模式

//通过DMA读取SD卡一个扇区
//buf:读数据缓存区
//sector:扇区地址
//blocksize:扇区大小(一般都是512字节)
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0;
    
    err=HAL_SD_ReadBlocks_DMA(&hsd,buf,sector,blocksize,cnt);//通过DMA读取SD卡一个扇区
    if(err==0)//读取成功
    {
        //等待读取完成
        err=HAL_SD_CheckReadOperation(&hsd,(uint32_t)SD_TIMEOUT);
    }
    
    return err;
}

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//blocksize:扇区大小(一般都是512字节)
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0; 
    
    err=HAL_SD_WriteBlocks_DMA(&hsd,buf,sector,blocksize,cnt);//通过DMA写SD卡一个扇区
    if(err==0)//写成功
    {
        //等待读取完成/
       err=HAL_SD_CheckWriteOperation(&hsd,(uint32_t)SD_TIMEOUT);
    }
    
    return err;
}

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
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

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
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
            sta=SD_WriteBlocks_DMA((uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//单个sector的写操作
            buf+=512;
        }
    }else
    {
        sta=SD_WriteBlocks_DMA((uint32_t*)buf,lsector,512,cnt);//多个sector的写操作
    }
    return sta;
} 

//SDMMC1中断服务函数
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&hsd);
}

#else                                   //轮训模式

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt)
{
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    lsector<<=9;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            sta=HAL_SD_ReadBlocks(&hsd,(uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//单个sector的读操作
            memcpy(buf,SDIO_DATA_BUFFER,512);
            buf+=512;
        }
    }else
    {
        sta=HAL_SD_ReadBlocks(&hsd,(uint32_t*)buf,lsector,512,cnt);//单个sector的读操作
    }
    INTX_ENABLE();//开启总中断
    return sta;
}  

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt)
{   
    u8 sta=SD_OK;
    long long lsector=sector;
    u8 n;
    lsector<<=9;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
    if((u32)buf%4!=0)
    {
        for(n=0;n<cnt;n++)
        {
            memcpy(SDIO_DATA_BUFFER,buf,512);
            sta=HAL_SD_WriteBlocks(&hsd,(uint32_t*)SDIO_DATA_BUFFER,lsector+512*n,512,1);//单个sector的写操作
            buf+=512;
        }
    }else
    {
        sta=HAL_SD_WriteBlocks(&hsd,(uint32_t*)buf,lsector,512,cnt);//多个sector的写操作
    }
    INTX_ENABLE();//开启总中断
    return sta;
} 
#endif


