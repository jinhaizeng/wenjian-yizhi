#include "sram.h"

#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
/****************************title****************************
函数名称：
函数功能：在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
入口参数: pBuffer:字节指针
          WriteAddr:要写入的地址
          要写入的字节数
出口参数:
说明：
****************************************************************/
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)  
	{										    
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}

/****************************title****************************
函数名称：
函数功能：在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
入口参数: Buffer:字节指针
          ReadAddr:要读出的起始地址
          n:要写入的字节数
出口参数:
说明：
****************************************************************/
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
}
//外部内存测试(最大支持1M字节内存测试)	    
void fsmc_sram_test(u16 x,u16 y)
{  
	u32 i=0;  	  
	u8 temp=0;	   
	u8 sval=0;	//在地址0读到的数据	  				   
  	LCD_ShowString(x,y,239,y+16,16,"Ex Memory Test:   0KB"); 
	//每隔4K字节,写入一个数据,总共写入256个数据,刚好是1M字节
	for(i=0;i<1024*1024;i+=4096)
	{
		FSMC_SRAM_WriteBuffer(&temp,i,1);
		temp++;
	}
	//依次读出之前写入的数据,进行校验		  
 	for(i=0;i<1024*1024;i+=4096) 
	{
  		FSMC_SRAM_ReadBuffer(&temp,i,1);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//后面读出的数据一定要比第一次读到的数据大.
      printf("%d",(u16)(temp-sval+1)*4);
		LCD_ShowxNum(x+15*8,y,(u16)(temp-sval+1)*4,4,16,0);//显示内存容量  
 	}					 
}	
