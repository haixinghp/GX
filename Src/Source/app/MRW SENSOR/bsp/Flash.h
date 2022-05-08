#ifndef FLASH_H_
#define FLASH_H_
#include "CCM_Types.h"
#include "EPORT.h"

#define SPI_FLASH_PERIPHERALS  &SPI3
#define SPI_FLASH_CS_PIN 0x04
#define SPI_FLASH_CS_0		(SPI_WritePin(&SPI3, PIN_SS, 0))
#define SPI_FLASH_CS_1		(SPI_WritePin(&SPI3, PIN_SS, 1))
#define W25X16_ID		    (0XC213)
#define GD25Q16C_ID 		(0XC814)
#define FLASH_ID		     GD25Q16C_ID
#define GD25Q_WriteEnable		0x06
#define GD25Q_WriteDisable		0x04
#define GD25Q_ReadStatusReg		0x05
#define GD25Q_WriteStatusReg    0x01
#define GD25Q_ReadData			0x03
#define GD25Q_FastReadData		0x0B
#define GD25Q_PageProgram		0x02
#define GD25Q_BlockErase		0xD8
#define GD25Q_SectorErase		0x20
#define GD25Q_ChipErase			0xC7
#define GD25Q_PowerDown			0xB9
#define GD25Q_ReleasePowerDown	0xAB
#define GD25Q_DeviceID			0xAB
#define GD25Q_ManufactDeviceID	0x90
#define GD25Q_JedecDeviceID		0x9F


extern void flashInit(void);
extern U8 eflashWrite(U32 writeAddr,U16 numByteToWrite,U8* pBuffer);

extern U16  SPI_Flash_ReadID(void);
extern void SPI_Flash_Read(U8* pBuffer,U32 ReadAddr,U16 NumByteToRead);
extern void SPI_Flash_Write_Sectors(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite);

//for test only.
extern void SPI_Flash_Erase_Sector(U32 Dst_Addr);
extern void SPI_Flash_Write_Page(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite);


#endif /* FLASH_H_ */
