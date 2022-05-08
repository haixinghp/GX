/***************************************************************************************
**文件名:     DRV_OnChipFlash.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
****************************************************************************************/
#include "DRV_OnChipFlash.h"
#include "libEFlash.h"
#include "eflash_drv.h"
#if  DRV_ONCHIP_FLASH_MODULE_EN
#warning DRV_ONCHIP_FLASH模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  

/*************************局部函数*****************************/  
/*******************************************************************************
* Function Name  : EFLASH_WordsRead
* Description    :  从EFLASH目标地址读取长度为len的word数据
* Input          : - addr: EFLASH destination address
*                  - len:  需要读取word数据的长度
*
* Output         : - des:  读取的word数据缓冲
* Return         : None
******************************************************************************/
static void EFLASH_Words_Read(UINT32 addr, UINT32 *des, UINT32 len)
{
	UINT32 i;

	for(i = 0; i < len; i ++)
	{
		des[i] = (*(volatile UINT32 *)(addr+i*4));
	}
}
/***************************************************************************************
**函数名:       DRV_OnChipFlash_Unlock
**功能描述:     EFLASH解锁
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_OnChipFlash_Unlock(void)
{
	EFLASH_SetWritePermission();
}
/***************************************************************************************
**函数名:       DRV_OnChipFlash_Lock
**功能描述:     EFLASH锁定
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_OnChipFlash_Lock(void)
{
	EFLASH_ClrWritePermission();
}
/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       DRV_OnChipFlash_Init
**功能描述:     片内FLASH初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_OnChipFlash_Init(void)
{
    UINT32 eflash_clk;
	eflash_clk = g_sys_clk;
	EFLASH_Init(eflash_clk/1000);
}
/***************************************************************************************
**函数名:       DRV_OnChipFlash_PageErase
**功能描述:     EFLASH页擦除
**输入参数:     
addr: 要擦除的EFLASH页地址
**输出参数:     
TRUE：成功   
FALSE：失败
**备注:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_PageErase(UINT32 addr)
{
	 UINT8 reTmp;
	DRV_OnChipFlash_Unlock();
	 reTmp = EFLASH_PageErase(addr);
	DRV_OnChipFlash_Lock();
	return reTmp;
}

/***************************************************************************************
**函数名:       DRV_OnChipFlash_EraseBytes
**功能描述:     flash扇区擦除
**输入参数:     
addr  所擦除扇区内的起始地址
size  所擦除扇区内的数据大小
**输出参数:     
TRUE：成功   
FALSE：失败
**备注:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_EraseBytes(UINT32 addr,UINT32 size)
{
	uint32_t	tp4, sp4, i;
	tp4 = size / FLASH_SECTOR_SIZE;
	  sp4 = size % FLASH_SECTOR_SIZE;
		if( sp4 > 0 )
		{
			tp4++;
		}
	    for(i=0; i<tp4; i++)
		{			
			 if(FALSE ==  DRV_OnChipFlash_PageErase( addr ))
			 {
				 return FALSE;
			 }
			 else
			 {
				 addr += FLASH_SECTOR_SIZE; //继续操作下一个扇区
			 }
		} 
		return 	TRUE;	
}

/***************************************************************************************
**函数名:       DRV_OnChipFlash_WordsProg
**功能描述:     word编程
**输入参数:     
addr: 要编程的EFLASH地址
des:  编程的word数据指针
len:  要编程的word数据长度
**输出参数:     
TRUE：编程成功   
FALSE：编程失败
**备注:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_WirteNWords(UINT32 addr, UINT32 *des, UINT32 len)
{
	return EFLASH_WordsProg(addr,des,len);
}
/***************************************************************************************
**函数名:       DRV_OnChipFlash_WordsProg
**功能描述:     word编程
**输入参数:     
addr: FLASH地址
des:  word数据指针
len:  word数据长度
**输出参数:     
TRUE：编程成功   
FALSE：编程失败
**备注:         
****************************************************************************************/
void DRV_OnChipFlash_ReadNWords(UINT32 addr, UINT32 *des, UINT32 len)
{
	EFLASH_Words_Read(addr,des,len);
}
/***************************************************************************************
**函数名:       DRV_OnChipFlash_Write
**功能描述:     EFLASH任意地址写数据
**输入参数:     
addr: FLASH地址
des:  word数据指针
len:  word数据长度
**输出参数:     
TRUE：编程成功   
FALSE：编程失败
**备注:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_Write(UINT32 flash_addr, const UINT8* ram_addr, UINT32 len)
{
	UINT8 reTmp;
	DRV_OnChipFlash_Unlock();
	reTmp  = EFLASH_Write(flash_addr,ram_addr,len);
	DRV_OnChipFlash_Lock();
	return reTmp;
}


/***************************************************************************************
**函数名:       DRV_OnChipFlash_CheckSum
**功能描述:     校验
**输入参数:     
Addr 地址
ByteLength 字节长度
**输出参数:     --
**备注:         
****************************************************************************************/
UINT32 DRV_OnChipFlash_CheckSum(UINT32 Addr, UINT32 ByteLength)
{
    UINT32 smu =0;
	  UINT32 i;
    
    for (i=0; i<ByteLength; i++)
    {
       smu += *((unsigned char *)(Addr+i));
    }
		return ( smu % ( 2^32 ) );
}
/*******************************************************************************
* Function Name  : DRV_OnChipFlash_ReadBytes
* Description    :  从EFLASH目标地址读取长度为len的Bytes数据
* Input          : - addr: EFLASH destination address
*                  - len:  需要读取word数据的长度
*
* Output         : - des:  读取的word数据缓冲
* Return         : None
******************************************************************************/
/***************************************************************************************
**函数名:       DRV_OnChipFlash_ReadBytes
**功能描述:     从EFLASH目标地址读取长度为len的Bytes数据
**输入参数:     
Addr 地址
des 数据缓冲
len 字节长度
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_OnChipFlash_ReadBytes(UINT32 addr, UINT8 *des, UINT32 len)
{
	UINT32 i;

	for(i = 0; i < len; i ++)
	{
		des[i] = (*(volatile UINT8 *)(addr+i));
	}
}

#endif  /*DRV_ONCHIP_FLASH_MODULE_EN*/
/************************************Code End******************************************/
