/***************************************************************************************
**�ļ���:     W25QXX.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
****************************************************************************************/
#include "W25QXX.h"
#include "spi_drv.h"
#include "spi_reg.h"

#if  W25QXX_MODULE_EN
#warning W25QXXģ��ʹ��!
/*******************************�궨��***************************/  


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
/*************************.C�����ṹ����***********************/  
//typedef enum
//{
//    RET_SUCCESS = 0,
//    RET_FlashRead_ERROR,
//    RET_WriteCmd_ERROR,
//    RET_WaitBusy_ERROR
//}SpiRetValueType;
/*************************˽��ȫ�ֱ���*************************/  
static uint8_t RW_Buf[RW_BUF_SIZE];
/*************************����ȫ�ֱ���*************************/  


/*************************�ֲ�����*****************************/  

/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       W25QXX_WriteCMD
**��������:     д������
**�������:     *CMD��ָ���д�������
**�������:     0,�ɹ�
**��ע:         
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
**������:       W25QXX_WriteEnable
**��������:     дʹ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void W25QXX_WriteEnable(void)
{
	uint8_t cmdTmp = SPIFlash_WriteEnable_CMD;
	DRV_SPI_WriteReadNByte(&cmdTmp,1,NULL,0);
}
/***************************************************************************************
**������:       W25QXX_EraseSector
**��������:     ����������W25Q128FVSIG��С�Ĳ�����λ������
**�������:     
Block_Num�����
Sector_Number��������
**�������:     --
**��ע:         
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
**������:       W25QXX_WritePage
**��������:     ��ָ���ĵ�ַд������
**�������:     
pBuffer:ָ���д�������
WriteAddr:д�����ʼ��ַ
WriteBytesNum:д����ֽ���
**�������:     --
**��ע:         
****************************************************************************************/
int8_t W25QXX_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
{
	RW_Buf[0] = SPIFlash_PageProgram_CMD;
	RW_Buf[1] = (uint8_t)((WriteAddr&0x00ff0000)>>16);
	RW_Buf[2] = (uint8_t)((WriteAddr&0x0000ff00)>>8);
	RW_Buf[3] = (uint8_t)WriteAddr;
	if(NULL == pBuffer)		//����ָ�����
	{
		return -1;
	}
	if((WriteBytesNum+4)>RW_BUF_SIZE)		//����ĳ������
	{
		return -2;
	}
	memcpy(&RW_Buf[4],pBuffer,WriteBytesNum);
	W25QXX_WriteEnable();
	DRV_SPI_WriteReadNByte(RW_Buf,WriteBytesNum+4,NULL,0);
//	nrf_delay_ms(100);	
}
/***************************************************************************************
**������:       W25QXX_Read
**��������:     ��ָ���ĵ�ַ����ָ�����ȵ�����
**�������:     
pBuffer��ָ���Ŷ������ݵ��׵�ַ       
ReadAddr�����������ݵ���ʼ��ַ
ReadBytesNum���������ֽ���
**�������:     --
**��ע:         
****************************************************************************************/
int8_t W25QXX_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
	RW_Buf[0] = SPIFlash_ReadData_CMD;
	RW_Buf[1] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
	RW_Buf[2] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
	RW_Buf[3] = (uint8_t)ReadAddr;
	if(NULL == pBuffer)		//����ָ�����
	{
		return -1;
	}
	if((ReadBytesNum+4)>RW_BUF_SIZE)		//����ĳ������
	{
		return -2;
	}
	DRV_SPI_WriteReadNByte(RW_Buf,4,&RW_Buf[4],ReadBytesNum);
	memcpy(pBuffer,&RW_Buf[4],ReadBytesNum);
	return 0;
}
/***************************************************************************************
**������:       W25QXX_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void W25QXX_Init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
	SPI_StructInit(SPI_Mode_Master,&SPI_InitStruct);
	SPI_Init(SPI4,&SPI_InitStruct);
}

#endif  /*W25QXX_MODULE_EN*/
/************************************Code End******************************************/