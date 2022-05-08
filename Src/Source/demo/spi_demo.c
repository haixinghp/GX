// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : spi_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <string.h>
#include "memmap.h"
#include "spi_demo.h"
#include "spi_drv.h"
#include "spi_reg.h"
#include "dmac_drv.h"
#include "debug.h"
#include "delay.h"
#include "ccm_drv.h"
#include "ccm_reg.h"

#define SPI_MASTER_MODE  1

void Demo_SPI_GpioConfig(SPI_TypeDef *SPIx)
{
	SPI_ConfigGpio(SPIx, SPI_MISO, GPIO_OUTPUT);
	SPI_ConfigGpio(SPIx, SPI_MOSI, GPIO_OUTPUT);
	SPI_ConfigGpio(SPIx, SPI_SCK, GPIO_INPUT);
	SPI_ConfigGpio(SPIx, SPI_SS, GPIO_INPUT);
	
	while(1)
	{
		SPI_WriteGpioData(SPIx, SPI_MISO, SPI_ReadGpioData(SPIx, SPI_SCK));
		SPI_WriteGpioData(SPIx, SPI_MOSI, SPI_ReadGpioData(SPIx, SPI_SS));
		DelayMS(500);
	}

}

void Demo_SPI_CPUNormalTransfer(SPI_TypeDef *SPIx)
{
	UINT32 len,i;
	UINT16 ret;
	UINT8 send[256] = {0};
	UINT8 receive[256];
	SPI_InitTypeDef SPI_InitStruct;

	printf("SPI CPU normal test.\r\n");

#if SPI_MASTER_MODE 
    
	SPI_StructInit(SPI_Mode_Master,&SPI_InitStruct);
	SPI_Init(SPIx,&SPI_InitStruct);

	len = 0x100;
	memset(receive,0x00,len);
	for(i = 0;i<len;i++)
	{
		send[i] = i;
	}
    while(1)
    {
        DelayMS(500);
        SPI_CS_L(SPIx);
        ret = SPI_MasterSendData(SPIx,send,len);
        SPI_CS_H(SPIx);
        if(STATUS_OK == ret)
        {
            printf("SPI master send success.\r\n");
            DelayMS(500);
            SPI_CS_L(SPIx);
            ret = SPI_MasterReceiveData(SPIx,receive,len);
            SPI_CS_H(SPIx);
            if(STATUS_OK == ret)
            {
                printf("SPI master receive success.\r\n");
                printf("receive = ");
                for(i = 0;i <len;i++)
                {
                    printf("%02x",receive[i]);
                }
                printf("\r\n");
            }
            else
            {
                printf("SPI master receive failed,ret = %04x.\r\n",ret);
            }
        }
        else
        {
            printf("SPI master send failed,ret = %04x.\r\n",ret);
        }
    }

#else //slave
	SPI_StructInit(SPI_Mode_Slave,&SPI_InitStruct);
	SPI_Init(SPIx,&SPI_InitStruct);

	len = 0x100;
	for(i = 0;i<len;i++)
	{
		send[i] = i;
	}
	while(1)
	{
		memset(receive,0,len);
		ret = SPI_SlaveReceiveData(SPIx,receive,len);
		if(STATUS_OK == ret)
		{
			printf("SPI slave receive success.\r\n");
            printf("receive = ");
            for(i = 0;i <len;i++)
            {
                printf("%02x",receive[i]);
            }
            printf("\r\n");
			ret = SPI_SlaveSendData(SPIx,send,len);
			if(STATUS_OK == ret)
			{
				printf("SPI slave send success.\r\n");
			}
			else
			{
				printf("SPI slave send failed,ret = %04x.\r\n",ret);
			}
			
		}
		else
		{
			printf("SPI slave receive failed,ret = %04x.\r\n",ret);
		}
		
	}

#endif
}

void Demo_SPI_DMATransfer(SPI_TypeDef *SPIx)
{
	UINT32 len,i;
	UINT16 ret;
	UINT8 send[256] = {0};
	UINT8 receive[256];
	UINT32 dma_base_addr;
	SPI_InitTypeDef SPI_InitStruct;
	

	printf("SPI DMA test.\r\n");

	if((SPIx == SPI1) || (SPIx == SPI2))
	{
		dma_base_addr = DMA1_BASE_ADDR;
	}
	else if(SPIx == SPI3)
	{
		dma_base_addr = DMA2_BASE_ADDR;
	}
	else
	{
		printf("SPI type id error.\r\n");
		while(1);
	}
	
	DMA_Init(dma_base_addr);

    len = 0x100;
	for(i = 0;i<len;i++)
	{
		send[i] = i;
	}
#if SPI_MASTER_MODE   

    printf("SPI DMA master test.\r\n");
    SPI_StructInit(SPI_Mode_Master,&SPI_InitStruct);
    SPI_Init(SPIx,&SPI_InitStruct);
    
    while(1)
    {
        DelayMS(500);
        memset(receive,0x00,len);
        SPI_CS_L(SPIx);
        SPI_EnableDMA(SPIx,TRUE);
        ret = dma_spitran(SPIx,send,receive,len,FALSE);
        SPI_EnableDMA(SPIx,FALSE);
        SPI_CS_H(SPIx);
        if(STATUS_OK == ret)
        {
            printf("SPI DMA transfer success.\r\n");
        }
        else
        {
            printf("SPI DMA transfer failed,ret = %04x.\r\n",ret);
        }
        
        printf("receive = ");
        for(i = 0;i <len;i++)
        {
            printf("%02x",receive[i]);
        }
        printf("\r\n");
    }
#else //slave mode

    printf("SPI DMA slave test.\r\n");
    SPI_StructInit(SPI_Mode_Slave,&SPI_InitStruct);
    SPI_Init(SPIx,&SPI_InitStruct);
    while(1)
    {
        memset(receive,0x00,len);
        SPI_EnableDMA(SPIx,TRUE);
        ret = dma_spitran(SPIx,send,receive,len,FALSE);
        SPI_EnableDMA(SPIx,FALSE);
        if(STATUS_OK == ret)
        {
            printf("SPI DMA transfer success.\r\n");
            printf("receive = ");
            for(i = 0;i <len;i++)
            {
                printf("%02x",receive[i]);
            }
            printf("\r\n");
        }
        else
        {
            printf("SPI DMA transfer failed,ret = %04x.\r\n",ret);
        }
    }
#endif    
}

void Demo_SPI_EDMATransfer(SPI_TypeDef *SPIx)
{
	UINT32 len,i;
	UINT16 ret;
	UINT8 send[256] = {0};
	UINT8 receive[256];
	SPI_InitTypeDef SPI_InitStruct;

#if SPI_MASTER_MODE
    
	SPI_StructInit(SPI_Mode_Master,&SPI_InitStruct);
	SPI_Init(SPIx,&SPI_InitStruct);
    len = 0x100;
	memset(receive,0x00,len);
	for(i = 0;i<len;i++)
	{
		send[i] = i;
	}
    while(1)
    {
        
#if 1    //全双工
        DelayMS(500);
        SPI_CS_L(SPIx);
        ret = EDMA_SPITransfer(SPIx, (UINT32)send,(UINT32)receive, len);
        SPI_CS_H(SPIx);
        if(STATUS_OK == ret)
        {
            printf("SPI EDMA transfer success.\r\n");
            printf("receive = ");
            for(i = 0;i <len;i++)
            {
                printf("%02x",receive[i]);
            }
            printf("\r\n");
        }
        else
        {
            printf("SPI EDMA transfer failed,ret = %04x.\r\n",ret);
        }
#else   //半双工
       DelayMS(500);   
       SPI_CS_L(SPIx);
       ret = SPI_MasterSendDataByEDMA(SPIx,(UINT32)send,len);
       SPI_CS_H(SPIx);
       if(STATUS_OK == ret)
       {
           printf("SPI EDMA master send success.\r\n");
           DelayMS(500);
           memset(receive,0x00,len);
           SPI_CS_L(SPIx);
           ret = SPI_MasterRecvDataByEDMA(SPIx,(UINT32)receive,len);
           SPI_CS_H(SPIx);
           if(STATUS_OK == ret)
           {
               printf("SPI EDMA master receive success.\r\n");
               printf("receive = ");
               for(i = 0;i <len;i++)
               {
                   printf("%02x",receive[i]);
               }
               printf("\r\n");
           }
           else
           {
               printf("SPI EDMA master receive failed,ret = %04x.\r\n",ret);
           }
       }
       else
       {
           printf("SPI EDMA master send failed,ret = %04x.\r\n",ret);
       }
#endif
    }
	
	
	
#else //slave
	SPI_StructInit(SPI_Mode_Slave,&SPI_InitStruct);
	SPI_Init(SPIx,&SPI_InitStruct);
    
	len = 0x100;
    for(i = 0;i<len;i++)
	{
		send[i] = i;
	}
	while(1)
	{
#if 1  //全双工
        memset(receive,0x00,len);
        ret = EDMA_SPITransfer(SPIx, (UINT32)send,(UINT32)receive, len);
        if(STATUS_OK == ret)
        {
            printf("SPI EDMA transfer success.\r\n");
            printf("receive = ");
            for(i = 0;i <len;i++)
            {
                printf("%02x",receive[i]);
            }
            printf("\r\n");
        }
        else
        {
            printf("SPI EDMA transfer failed,ret = %04x.\r\n",ret);
        }
#else //半双工
    
        memset(receive,0x00,len);
        SPIx->SPICR1 |= SPI_ENABLE;
        ret = SPI_SlaveRecvDataByEDMA(SPIx,(UINT32)receive,len);
        SPIx->SPICR1 &= ~SPI_ENABLE;
        if(STATUS_OK == ret)
        {
            printf("SPI EDMA slave receive success.\r\n");
            printf("receive = ");
            for(i = 0;i <len;i++)
            {
                printf("%02x",receive[i]);
            }
            printf("\r\n");
            SPIx->SPICR1 |= SPI_ENABLE;
            ret = SPI_SlaveSendDataByEDMA(SPIx,(UINT32)send,len);
            SPIx->SPICR1 &= ~SPI_ENABLE;
            if(STATUS_OK == ret)
            {
                printf("SPI EDMA slave send success.\r\n");
            }
            else
            {
                printf("SPI EDMA slave send failed,ret = %04x.\r\n",ret);
            }
        }
        else
        {
            printf("SPI EDMA slave receive failed,ret = %04x.\r\n",ret);
        }
#endif
	}
#endif
}

void SPI_Demo(void)
{
	SPI_TypeDef *spi_type_id;
	
	spi_type_id = SPI1;

	//SPI CPU 查询方式，半双工
	Demo_SPI_CPUNormalTransfer(spi_type_id);

	//SPI DMA方式，全双工模式,栈及全局变量放在0x20000000之后，即IRAM：Start0x20000000,size0x38000
//	Demo_SPI_DMATransfer(spi_type_id);

	//SPI EDMA方式,全双工和半双工，栈及全局变量放在0x20000000之后，即IRAM：Start0x20000000,size0x38000
//	Demo_SPI_EDMATransfer(spi_type_id);

	//SPI pin脚（SS、CLK、MOSI、MISO） 配置成GPIO功能，不支持中断
//	Demo_SPI_GpioConfig(spi_type_id);

}
