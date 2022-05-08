/*
 * nfc_demo.c
 *
 *  Created on: 2018年8月23日
 *      Author: CCORE YY
 */
#include "nfc_demo.h"
#include "spi_drv.h"
#include "pcd_interface.h"
#include "emv_layer4.h"
//#include "ept_drv.h"
#include "string.h"
#include "debug.h"
#include "delay.h"

void nfc_delayms_test()
{
	SPI_ConfigGpio(SPI2,SPI_MISO,GPIO_OUTPUT);
	SPI_ConfigGpio(SPI2,SPI_MOSI,GPIO_OUTPUT);
	SPI_ConfigGpio(SPI2,SPI_SCK, GPIO_OUTPUT);
	SPI_ConfigGpio(SPI2,SPI_SS,  GPIO_OUTPUT);
	while(1)
	{
		SPI_WriteGpioData(SPI2,SPI_MISO,Bit_SET);
		SPI_WriteGpioData(SPI2,SPI_MOSI,Bit_SET);
		SPI_WriteGpioData(SPI2,SPI_SCK, Bit_SET);
		SPI_WriteGpioData(SPI2,SPI_SS,  Bit_SET);
		DelayMS(100);
		SPI_WriteGpioData(SPI2,SPI_MISO,Bit_RESET);
		SPI_WriteGpioData(SPI2,SPI_MOSI,Bit_RESET);
		SPI_WriteGpioData(SPI2,SPI_SCK, Bit_RESET);
		SPI_WriteGpioData(SPI2,SPI_SS,  Bit_RESET);
		DelayMS(10);
	}
}
void NFC_Demo(void)
{
	char len;
	char i;
	UINT8 cmd_selectDDF1[] = "\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31";
	UINT8 cmd_select_purse[] = "\x00\xA4\x04\x00\x08\xA0\x00\x00\x03\x33\x01\x01\x01";
	UINT8 cmd_read_balance[] = "\x80\xCA\x9F\x79\x09";
	UINT8 buf[0x80];
	size_t res_len;
	
	SPI_InitTypeDef SPI_InitStruct;
	//nfc init
	SPI_StructInit_NFC(SPI_Mode_Master,&SPI_InitStruct);
	SPI_Init(SPI1, &SPI_InitStruct);
	//配置nfc模块的ss引脚
	SPI_ConfigGpio(SPI2,SPI_MISO,GPIO_OUTPUT);
	SPI_WriteGpioData(SPI2,SPI_MISO,Bit_SET);
	//配置nfc模块的rst引脚
	SPI_ConfigGpio(SPI2,SPI_MOSI,GPIO_OUTPUT);

	printf("NFC DEMO\r\n");
//	fm17550_test_spi_comm_simple();//测试4202与17550的通信

	//硬件复位读卡器
	PcdHWReset();
	PcdSoftReset();


//	active card
	while(1)
	{
		while(1)
		{
			if(!GuiActive())
			{
				printf("Find Card\r\n");
				break;
			}
		}

		while(1)
		{
			//选择金融应用
			printf("选择金融应用:\r\n");
			printf("-> ");
			len = sizeof(cmd_selectDDF1)-1;
			for(i = 0;i < len;i++)
			{
				printf(" %02x",cmd_selectDDF1[i]);
			}
			printf("\r\n");
			memcpy(buf,cmd_selectDDF1,len);
			if(0 == emvTransceiveApdu(cmd_selectDDF1,len,buf,128,&res_len))
			{
				printf("<- ");
				for(i = 0;i <res_len;i++)
					printf(" %02x",buf[i]);
				printf("\r\n");
			}
			else
			{
				printf("NFC ERROR ! \r\n");
				break;
			}
			//选择钱包
			printf("选择钱包:\r\n");
			printf("-> ");
			len = sizeof(cmd_select_purse)-1;
			for(i = 0;i < len;i++)
			{
				printf(" %02x",cmd_select_purse[i]);
			}
			printf("\r\n");
			memcpy(buf,cmd_select_purse,len);
			if(0 == emvTransceiveApdu(cmd_select_purse,len,buf,128,&res_len))
			{
				printf("<- ");
				for(i = 0;i <res_len;i++)
					printf(" %02x",buf[i]);
				printf("\r\n");
			}
			else
			{
				printf("NFC ERROR ! \r\n");
				break;
			}
			//读取余额
			printf("读取余额:\r\n");
			printf("-> ");
			len = sizeof(cmd_read_balance)-1;
			for(i = 0;i < len;i++)
			{
				printf(" %02x",cmd_read_balance[i]);
			}
			printf("\r\n");
			memcpy(buf,cmd_read_balance,len);
			if(0 == emvTransceiveApdu(cmd_read_balance,len,buf,128,&res_len))
			{
				printf("<- ");
				for(i = 0;i <res_len;i++)
					printf(" %02x",buf[i]);
				printf("\r\n");
			}
			else
			{
				printf("NFC ERROR ! \r\n");
				break;
			}
			DelayMS(1000);
		}
	}
}
