#include "Flash.h"
#include "eflash_api.h"
//#include "Cache.h"
#include <string.h>
//#include <system.h>
#include "CCM_Types.h"
#include "Timer.h"
#include "sensor.h"
#include "SPI.h"
#include "ssi.h"
#include "dma.h"
#include "sys.h"

extern UINT32 g_sys_clk;


#define EFLASH_CLOCK  (g_sys_clk)

void SPI_Flash_Init(void);
U8 SPIx_ReadWriteByte(U8 TxData);
void EDMAC_SPI_SEND(void * sendbuf, unsigned int len);
void EDMAC_SPI_RECV(void * recvbuf, unsigned int len);
U8 SPI_Flash_ReadSR(void);
void SPI_FLASH_Write_Enable(void);
void SPI_FLASH_Write_Disable(void);
void SPI_Flash_Write_Page(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite);
void SPI_Flash_Write_Multipages(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite);
void SPI_Flash_Erase_Sector(U32 Dst_Addr);
void SPI_Flash_Wait_Busy(void);


void flashInit(void)
{
	
	eflash_init(EFLASH_CLOCK);
	
	SPI_Flash_Init();
}

U8 EFlashBuffer[512]__attribute__ ((aligned (4))); ;
U8 eflashWrite(U32 writeAddr,U16 numByteToWrite,U8* pBuffer)
{
	U8 ret_value;
	U32 secpos;
	U16 secoff,secremain;
	U8 * buffer=EFlashBuffer;
 	secpos=writeAddr/512;
	secoff=writeAddr%512;
	secremain=512-secoff;

 	if(numByteToWrite<=secremain)
	{
		secremain=numByteToWrite;
	}
	while(1)
	{
		memcpy(buffer,(U8 *)(secpos*512),512);

		ret_value = eflash_page_erase_withkey(secpos*512, EFLASH_OPERATION_KEY);
		if(ret_value)
			return ret_value;

		 memcpy(&buffer[secoff],pBuffer,secremain);

		 ret_value=eflash_bulk_program_withkey(secpos*512,128,(U32*)buffer,EFLASH_OPERATION_KEY);
		 if(ret_value)
			return ret_value;

		if(numByteToWrite==secremain) break;
		else
		{
			secpos++;
			secoff=0;
		    pBuffer+=secremain;
		    numByteToWrite-=secremain;
			if(numByteToWrite>512) secremain=512;
			else secremain=numByteToWrite;
		}
	}
    return 0;
}

U8 SPIx_ReadWriteByte(U8 TxData)
{
	U8  rxdata;
	EDMAC_ControlConfig_t config;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI3;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = &TxData;
	config.In_Buff = &rxdata;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
	config.Transfer_Num = 1;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL1, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL1);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL1));
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
	return rxdata;
}

void EDMAC_SPI_SEND(void * sendbuf, unsigned int len)
{
	EDMAC_ControlConfig_t config;
	U8 tmpU8;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI3;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 1;
	config.Out_Buff = sendbuf;
	config.In_Buff = &tmpU8;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
	config.Transfer_Num = len;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL1, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL1);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL1));
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
}

void EDMAC_SPI_RECV(void * recvbuf, unsigned int len)
{
	EDMAC_ControlConfig_t config;
	U8 tmpU8;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI3;
	config.Out_Buff_Inc_Disable = 1;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = &tmpU8;
	config.In_Buff = recvbuf;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
	config.Transfer_Num = len;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL1, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL1);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL1));
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL1);
}

void SPI_Flash_Init(void)
{
	SSI_Config_t ssi_config = {0, };

	ssi_config.CPOL = 0;
	ssi_config.CPHA = 0;
	ssi_config.Frame_Bits = 7;			//8-bit data lens
	ssi_config.Wait_Cycle = 0x10;		//wait cycle between cmd & data
	ssi_config.BaudRate_Pre = 8;
	ssi_config.Ins_Lens = 0;
	ssi_config.TX_Data_Level = 4;
	
	SSI_Init(SSI1, ssi_config);
#ifdef SSI_DMA_EN
	DMA_REG_Init(DMA2_BASE_ADDR);
	NVIC_Init(3, 3, DMA2_IRQn, 2);
#endif
}

void SPI_Flash_Quad_Mode_EN()
{
	volatile unsigned int temp;
		
	temp=SSI_EFlash_Get_Status2(SSI1);
	if(!(temp & 0x02))
	{
		SPI_FLASH_Write_Enable();
		SSI_EFlash_Prog_Status2(SSI1, 0x02);					//enable spi flash to quad mode.
		SPI_FLASH_Write_Disable();
	}
}


U8 SPI_Flash_ReadSR(void)
{
	return SSI_EFlash_Get_Status1(SSI1);
}

void SPI_FLASH_Write_Enable(void)
{
	SSI_EFlash_Write_Enable(SSI1);
}

void SPI_FLASH_Write_Disable(void)
{
	SSI_EFlash_Write_Disable(SSI1);
}

void SPI_Flash_Write_Page(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite)
{
	U8 status;
	
#ifndef SSI_DMA_EN
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_Program(SSI1, WriteAddr, pBuffer, NumByteToWrite);					//NORMAL SPI
//	SSI_EFlash_QUAD_Program(SSI1, WriteAddr, pBuffer, NumByteToWrite);		//QUAL SPI
	SSI_EFlash_Write_Disable(SSI1);
#else
	/* NORMAL SPI {*/
	SSI_EFlash_Write_Enable(SSI1);
	SSI_STD_DMA_Send(SSI1, 0, pBuffer, WriteAddr, NumByteToWrite, 0);
	SSI_Check_Idle(SSI1);
	//SSI_EFlash_Write_Disable(SSI1);
	/* }NORMAL SPI. */
	
	
	/* { QUAD SPI DMA. */
//	SSI_EFlash_Write_Enable(SSI1);
//	SSI_QUAD_Init(SSI1, 0, NumByteToWrite - 1, 8); //set to quad
//	SSI_QUAD_DMA_Send(SSI1, 0, pBuffer, WriteAddr, NumByteToWrite, 0);
//	SSI_Check_Idle(SSI1);
//	SSI_Standard_Init(SSI1);
//	SSI_EFlash_Write_Disable(SSI1);
	/* }. */

	do
	 {
		 status = SSI_EFlash_Get_Status1(SSI1);
	 }while(status & 0x01);
	 SSI_EFlash_Write_Disable(SSI1);
#endif
}

//write Multipages
void SPI_Flash_Write_Multipages(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite)
{
	U16 pageremain;

	pageremain=256-WriteAddr%256;
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;
	while(1)
	{
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;
			if(NumByteToWrite>256)pageremain=256;
			else pageremain=NumByteToWrite;
		}
	};
}

void SPI_Flash_Erase_Sector(U32 Dst_Addr)
{
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_Sector_Erase(SSI1, Dst_Addr*4096);
	SSI_EFlash_Write_Disable(SSI1);
}

void SPI_Flash_Wait_Busy(void)
{
	while ((SPI_Flash_ReadSR()&0x01)==0x01);
}

void SPI_Flash_Write_Sectors(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite)
{
	U32 secpos = WriteAddr/4096, secnum = (NumByteToWrite + 4095) / 4096;

	WriteAddr = secpos * 4096;

	while (secnum--) {
		SPI_Flash_Erase_Sector(secpos++);

		if (NumByteToWrite > 4096) {
			SPI_Flash_Write_Multipages(pBuffer, WriteAddr, 4096);
			NumByteToWrite -= 4096;
		} else {
			SPI_Flash_Write_Multipages(pBuffer, WriteAddr, NumByteToWrite);
		}

		pBuffer += 4096;
		WriteAddr += 4096;
	}
}

void SPI_Flash_Read(U8* pBuffer,U32 ReadAddr,U16 NumByteToRead)
{
	U8 status;
	U32 loop;
	U32 mod;
	U32 i;
	U32 read_len;
	
#ifndef SSI_DMA_EN
	//SSI_EFlash_QUAD_Read(SSI1, ReadAddr, pBuffer, NumByteToRead);			//QUAD SPI
	//SSI_EFlash_Read(SSI1, ReadAddr, pBuffer, NumByteToRead);					//NORMAL SPI
	SSI_EFlash_DUAL_Read(SSI1, ReadAddr, pBuffer, NumByteToRead);			//DUAL SPI
#else
//	SSI_STD_DMA_Read(SSI1, 0, pBuffer, ReadAddr, NumByteToRead, 0);		//NORMAL SPI
	
	/* { QUAL SPI DMA. */
	loop = NumByteToRead / 26624;		//26K per packet
	mod = NumByteToRead % 26624;		//26 * 1024 = 26624
	if(mod != 0)
		loop++;

	for(i = 0; i < loop; i++)
	{
		if(i == (loop -1))
			read_len = NumByteToRead;
		else
			read_len = 26624;

		SSI_DUAL_Init(SSI1, read_len , 8); //set to quad
		SSI_DUAL_DMA_Read(SSI1, 0, pBuffer + i * 26624, ReadAddr + i * 26624, read_len, 0);
		SSI_Standard_Init(SSI1);
		
		NumByteToRead -= 26624;
	}
	/* }. */
	/* { QUAL SPI DMA. */
//	SSI_QUAD_Init(SSI1, 1, NumByteToRead - 1, 8); //set to quad
//	SSI_QUAD_DMA_Read(SSI1, 0, pBuffer, ReadAddr, NumByteToRead, 1);
//	SSI_Standard_Init(SSI1);
	/* }. */
	
	do
	 {
		 status = SSI_EFlash_Get_Status1(SSI1);
		 //printf("status: 0x%02x\r\n", status);
	 }while(status & 0x01);
#endif	
	
}

U16 SPI_Flash_ReadID(void)
{
	return Read_ID_Test(SSI1);
}
