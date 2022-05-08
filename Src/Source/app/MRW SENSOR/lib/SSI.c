// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : ssi_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "delay.h"
#include "debug.h"
#include "common.h"
#include "dma.h"
#include "delay.h"
#include "SSI.h"
//#include "syscfg.h"

extern DMA_CONTROL_REG *m_dma_control;
extern DMA_CHANNEL_REG *m_dma_channel[DMAC_CHNUM];



//启动SSI模块
void SSI_Enable(SSI_TypeDef *SSIx)
{
    SSIx->SSI_SSIENR = 0x01;
}

//停止SSI模块
void SSI_Disable(SSI_TypeDef *SSIx)
{
	while(SSIx->SSI_SR&SR_BUSY){;}
    SSIx->SSI_SSIENR = 0x00;
}

void SSI_Check_Idle(SSI_TypeDef *SSIx)
{
	while((SSIx->SSI_SR & SR_TFE)!=SR_TFE){;}
	while(SSIx->SSI_SR&SR_BUSY){;}
}
	
void SSI_Init(SSI_TypeDef *SSIx, SSI_Config_t ssi_config)
{
	 while(SSIx->SSI_SR&SR_BUSY){;}
	
   SSI_Disable(SSIx);
		 
		 
	 SSIx->SSI_CTRLR1 = 0x00;
	 SSIx->SSI_CTRLR0 =  ssi_config.Frame_Bits 
												| (ssi_config.CPHA << 8) 
												| (ssi_config.CPOL << 9);
	 SSIx->SSI_BAUDR = ssi_config.BaudRate_Pre;
	 SSIx->SSI_TXFTLR = 0x00;
	 SSIx->SSI_RXFTLR = 0x00;
	 SSIx->SSI_SPI_CTRLR0 = ssi_config.Wait_Cycle << 11
													| ssi_config.Ins_Lens << 8;
	 SSIx->SSI_IMR = 0x00;
	 SSIx->SSI_DMATDLR = ssi_config.TX_Data_Level;
	 SSIx->SSI_DMACR = 0x00;	 

	 SSI_Enable(SSIx);
}

 





/*******************************************************************************
* Function Name  : SSI_EFlash_Write_Enable
* Description    : SSI Flash写使能 
* Input          : - SSIx: SSI 基地址
* Output         : None
* Return         : None
******************************************************************************/
void SSI_EFlash_Write_Disable(SSI_TypeDef *SSIx)
{
	volatile unsigned char status;
	volatile unsigned char temp;
	
	if((SSI_EFlash_Get_Status1(SSIx)&0x02) == 0x02)
	{
		SSIx->SSI_DR=WRITE_DIS_CMD;
	
		__asm("nop");
		__asm("nop");
		__asm("nop");
		
		while((SSIx->SSI_SR&SR_TFE)==0x00){;}
			
		while(SSIx->SSI_SR&SR_BUSY){;}
		
		while(SSIx->SSI_SR&SR_RFNE)
		{
				temp=SSIx->SSI_DR;//清空fifo
		}	
		
		do
		{
			status=SSI_EFlash_Get_Status1(SSIx);		 
		}while(status&0x01);
	}
}	

















