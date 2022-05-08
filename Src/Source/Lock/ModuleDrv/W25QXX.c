/***************************************************************************************
**文件名:     W25QXX.c
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
#include "W25QXX.h"
#include "spi_drv.h"
#include "spi_reg.h"

#if  W25QXX_MODULE_EN
#warning W25QXX模块使能!
/*******************************宏定义***************************/  


#define    SPIFLASH_CMD_LENGTH        0x03
#define    SPIFLASH_WRITE_BUSYBIT     0X01
#define    SPIFlash_ReadData_CMD      0x03
#define    SPIFlash_WriteEnable_CMD   0x06
#define    SPIFlash_WriteDisable_CMD  0x04
#define    SPIFlash_PageProgram_CMD   0x02
#define    SPIFlash_WriteSR_CMD       0x01
#define    SPIFlash_ReadSR_CMD        0x05
#define    SPIFlash_SecErase_CMD      0x20
#define    SPIFlash_BlockErase_CMD    0xD8
#define    SPIFlash_PAGEBYTE_LENGTH   256
#define    SPIFlash_SECBYTE_LENGTH   (1024*4)
#define    FLASH_BLOCK_NUMBLE         7
#define    FLASH_PAGE_NUMBLE          8
/*************************.C变量结构声明***********************/  
//typedef enum
//{
//    RET_SUCCESS = 0,
//    RET_FlashRead_ERROR,
//    RET_WriteCmd_ERROR,
//    RET_WaitBusy_ERROR
//}SpiRetValueType;
/*************************私有全局变量*************************/  
static uint8_t RW_Buf[RW_BUF_SIZE];
/*************************公共全局变量*************************/  


/*************************局部函数*****************************/  

/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       W25QXX_WriteCMD
**功能描述:     写入命令
**输入参数:     *CMD：指向待写入的命令
**输出参数:     0,成功
**备注:         
****************************************************************************************/
uint8_t W25QXX_WriteCMD(uint8_t *CMD)
{
	RW_Buf[0]=*CMD;
	RW_Buf[1]=*(CMD+1);
	RW_Buf[2]=*(CMD+2);
	DRV_SPI_WriteReadNByte(RW_Buf,3,NULL,0);
	return 0;
}
/***************************************************************************************
**函数名:       W25QXX_WriteEnable
**功能描述:     写使能
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void W25QXX_WriteEnable(void)
{
	uint8_t cmdTmp = SPIFlash_WriteEnable_CMD;
	DRV_SPI_WriteReadNByte(&cmdTmp,1,NULL,0);
}
/***************************************************************************************
**函数名:       W25QXX_EraseSector
**功能描述:     擦除扇区，W25Q128FVSIG最小的擦除单位是扇区
**输入参数:     
Block_Num：块号
Sector_Number：扇区号
**输出参数:     --
**备注:         
****************************************************************************************/
void W25QXX_EraseSector(uint8_t Block_Num,uint8_t Sector_Number)
{
	RW_Buf[0] = SPIFlash_SecErase_CMD;
	RW_Buf[1] = Block_Num;
	RW_Buf[2] = Sector_Number<<4;
	RW_Buf[3] = 0x00;														
	W25QXX_WriteEnable();
	DRV_SPI_WriteReadNByte(RW_Buf,4,NULL,0);														
}
/***************************************************************************************
**函数名:       W25QXX_WritePage
**功能描述:     向指定的地址写入数据
**输入参数:     
pBuffer:指向待写入的数据
WriteAddr:写入的起始地址
WriteBytesNum:写入的字节数
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t W25QXX_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
{
	RW_Buf[0] = SPIFlash_PageProgram_CMD;
	RW_Buf[1] = (uint8_t)((WriteAddr&0x00ff0000)>>16);
	RW_Buf[2] = (uint8_t)((WriteAddr&0x0000ff00)>>8);
	RW_Buf[3] = (uint8_t)WriteAddr;
	if(NULL == pBuffer)		//传入指针错误
	{
		return -1;
	}
	if((WriteBytesNum+4)>RW_BUF_SIZE)		//传入的长度溢出
	{
		return -2;
	}
	memcpy(&RW_Buf[4],pBuffer,WriteBytesNum);
	W25QXX_WriteEnable();
	DRV_SPI_WriteReadNByte(RW_Buf,WriteBytesNum+4,NULL,0);
//	nrf_delay_ms(100);	
}
/***************************************************************************************
**函数名:       W25QXX_Read
**功能描述:     从指定的地址读出指定长度的数据
**输入参数:     
pBuffer：指向存放读出数据的首地址       
ReadAddr：待读出数据的起始地址
ReadBytesNum：读出的字节数
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t W25QXX_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
	RW_Buf[0] = SPIFlash_ReadData_CMD;
	RW_Buf[1] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
	RW_Buf[2] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
	RW_Buf[3] = (uint8_t)ReadAddr;
	if(NULL == pBuffer)		//传入指针错误
	{
		return -1;
	}
	if((ReadBytesNum+4)>RW_BUF_SIZE)		//传入的长度溢出
	{
		return -2;
	}
	DRV_SPI_WriteReadNByte(RW_Buf,4,&RW_Buf[4],ReadBytesNum);
	memcpy(pBuffer,&RW_Buf[4],ReadBytesNum);
	return 0;
}
/***************************************************************************************
**函数名:       W25QXX_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void W25QXX_Init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
	SPI_StructInit(SPI_Mode_Master,&SPI_InitStruct);
	SPI_Init(SPI4,&SPI_InitStruct);
}

#endif  /*W25QXX_MODULE_EN*/
/************************************Code End******************************************/