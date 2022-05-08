// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : ssi_reg.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef __SSI_REG_H__
#define __SSI_REG_H__

#include "type.h"

typedef struct
{
__IO unsigned int SSI_CTRLR0;    	//0x00
__IO unsigned int SSI_CTRLR1;    	//0x04
__IO unsigned int SSI_SSIENR;   	//0x08
__IO unsigned int SSI_MWCR;      	//0x0c
__IO unsigned int SSI_SER;       	//0x10
__IO unsigned int SSI_BAUDR;     	//0x14
__IO unsigned int SSI_TXFTLR;    	//0x18
__IO unsigned int SSI_RXFTLR;    	//0x1c
__IO unsigned int SSI_TXFLR;     	//0x20
__IO unsigned int SSI_RXFLR;     	//0x24
__IO unsigned int SSI_SR;        	//0x28
__IO unsigned int SSI_IMR;        	//0x2c
__IO unsigned int SSI_ISR;        	//0x30
__IO unsigned int SSI_RISR;       	//0x34
__IO unsigned int SSI_TXOICR;          	//0x38
__IO unsigned int SSI_RXOICR;          	//0x3c
__IO unsigned int SSI_RXUICR;          	//0x40
__IO unsigned int SSI_MSTICR;          	//0x44
__IO unsigned int SSI_ICR;             	//0x48
__IO unsigned int SSI_DMACR;           	//0x4c
__IO unsigned int SSI_DMATDLR;         	//0x50
__IO unsigned int SSI_DMARDLR;         	//0x54
__IO unsigned int SSI_IDR;             	//0x58
__IO unsigned int SSI_SSIC_VERSION_ID; 	//0x5c
__IO unsigned int SSI_DR;              	//0x60
__IO unsigned int RESERVERED[35];	
__IO unsigned int SSI_RX_SAMPLE_DELAY; 	//0xf0
__IO unsigned int SSI_SPI_CTRLR0;      	//0xf4
__IO unsigned int SSI_DDR_DRIVE_EDGE;  	//0xf8
__IO unsigned int SSI_XIP_MODE_BITS;   	//0xfc 
__IO unsigned int XIP_INCR_INST;        //0x100
__IO unsigned int XIP_WRAP_INST;        //0x104
__IO unsigned int XIP_CTRL;             //0x108
__IO unsigned int XIP_SER;              //0x10C
__IO unsigned int XRXOICR;              //0x110	
__IO unsigned int XIP_CNT_TIME_OUT;     //0x114	

}SSI_TypeDef;

typedef struct{
    U8 CPOL;
    U8 CPHA;
    U16 BaudRate_Pre;
    U8 Frame_Bits;
		U8 Wait_Cycle;
		U8 Ins_Lens;
		U8 TX_Data_Level;
}SSI_Config_t;

#define SSI1  (SSI_TypeDef *)(0x60000000)
#define SSI2  (SSI_TypeDef *)(0x4004a000)

#define SR_TXE				0x20
#define SR_RFF        0x10
#define SR_RFNE       0x08
#define SR_TFE				0x04
#define SR_TFNF       0x02
#define SR_BUSY       0x01

#define DMACR_TDMAE   0x02
#define DMACR_RDMAE   0x01

//#define SSI_DMA_INT
//#define SSI_DMA_EN

//w25q128 CMD
#define DUMMY_BYTE				0xa5
#define READ_ID_CMD				0x90  
#define WRITE_EN_CMD			0x06
#define WRITE_DIS_CMD			0x04
#define SECT_ERASE_CMD		0x20
#define GET_SAT1_CMD			0x05
#define READ_CMD					0x03
#define PAGE_PROG_CMD			0x02
#define GET_SAT2_CMD			0x35
#define PROG_STA2_CMD			0x31
#define QUAD_PROG_CMD			0x32
#define QUAD_READ_CMD			0x6b
#define DUAL_READ_CMD			0x3b
#define QPI_ENTER_CMD			0x38
#define SET_READ_PARA_CMD	0xc0

extern volatile int dma_int;

extern void SSI_DMA_ISR(void);
extern void SSI_Init(SSI_TypeDef *SSIx, SSI_Config_t ssi_config);
//extern void SSI_EFlash_Set_Read_Para(SSI_TypeDef *SSIx);
extern void SSI_Check_Idle(SSI_TypeDef *SSIx);
extern void SSI_Standard_Init(SSI_TypeDef *SSIx);

extern unsigned short Read_ID_Test(SSI_TypeDef *SSIx);
extern void SSI_EFlash_Read(SSI_TypeDef *SSIx, unsigned int addr, unsigned char *buf, int num);
//extern unsigned char SSI_EFlash_Get_Status1(SSI_TypeDef *SSIx);
extern void SSI_EFlash_Write_Enable(SSI_TypeDef *SSIx);
extern void SSI_EFlash_Write_Disable(SSI_TypeDef *SSIx);
extern void SSI_EFlash_Sector_Erase(SSI_TypeDef *SSIx, unsigned int addr);
extern void SSI_EFlash_Program(SSI_TypeDef *SSIx, unsigned int addr, unsigned char *buf, int num);
extern unsigned char SSI_EFlash_Get_Status2(SSI_TypeDef *SSIx);
extern void SSI_EFlash_Prog_Status2(SSI_TypeDef *SSIx, unsigned char val);
extern void SSI_Std_Dma_Trig(SSI_TypeDef *SSIx, unsigned cmd, unsigned int addr, int dmaConf);

//SPI Flash Dual Mode Operation.
extern void SSI_DUAL_Init(SSI_TypeDef *SSIx, int num, int waitCycles);
extern void SSI_DUAL_DMA_Read(SSI_TypeDef *SSIx,int dma_ch, UINT8* pread, UINT32 addr, UINT32 length, BOOL binten);
extern void SSI_EFlash_DUAL_Read(SSI_TypeDef *SSIx, unsigned int addr, unsigned char *buf, int num);
extern void SSI_QUAD_DMA_Send(SSI_TypeDef *SSIx,int dma_ch, UINT8* psend, UINT32 addr, UINT32 length, BOOL binten);

//SPI Flash Quad Mode Operation.
extern void SSI_QUAD_Init(SSI_TypeDef *SSIx, int read, int num, int waitCycles);
extern void SSI_QUAD_DMA_Read(SSI_TypeDef *SSIx,int dma_ch, UINT8* pread, UINT32 addr, UINT32 length, BOOL binten);
extern void SSI_EFlash_QUAD_Program(SSI_TypeDef *SSIx, unsigned int addr, unsigned char *buf, int num);
extern void SSI_EFlash_QUAD_Read(SSI_TypeDef *SSIx, unsigned int addr, unsigned char *buf, int num);
extern void SSI_STD_DMA_Send(SSI_TypeDef *SSIx,int dma_ch, UINT8* psend, UINT32 addr, UINT32 length, BOOL binten);

/*******************************************************************************
* Function Name  : void SSI_STD_DMA_Send
* Description    : spi dma传送函数
* Input          : - SPIx: SPI 基地址
*                - dma_ch: DMA channel
*				          - psend: 发送数据地址
*                  _ addr: SPI flash地址
*				         - length： 传输数据长度
*				         - binten: 是否开启中断模式
*
* Output         : None
* Return         : None
******************************************************************************/
extern void SSI_STD_DMA_Send(SSI_TypeDef *SSIx,int dma_ch, UINT8* psend, UINT32 addr, UINT32 length, BOOL binten);
/*******************************************************************************
* Function Name  : SSI_Std_DMA_Read
* Description    : spi dma接收函数
* Input          : - SPIx: SPI 基地址
*                - dma_ch: DMA channel
*				          - pread: 发送数据地址
*                  - addr: spi flash地址
*				        - length ： 传输数据长度
*				        - binten : 是否开启中断模式
*
* Output         : None
* Return         : None
******************************************************************************/
extern void SSI_STD_DMA_Read(SSI_TypeDef *SSIx,int dma_ch, UINT8* pread, UINT32 addr, UINT32 length, BOOL binten);

#endif /* __SSI_REG_H__ */
