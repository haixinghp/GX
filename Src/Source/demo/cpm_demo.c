// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : cpm_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "cpm_drv.h"
#include "debug.h"
#include "cpm_demo.h"
#include "eport_drv.h"
#include "tc_drv.h"
#include "wdt_drv.h"
#include "delay.h"

/*
 * 注意，测试前要打开eport 0，1，2的中断功能，再额外打开一个中断，用来唤醒芯片的normal sleep（必须配置成为电平触发）
 * */
 

void CPM_Demo(void)
{
	DelayMS(2000);	//调试时建议加上延时，预防上电进入低功耗无法仿真
	
	EPORT_Init(EPORT_PIN5,LOW_LEVEL_INT);

	EPORT_ConfigGpio(EPORT_PIN1, GPIO_INPUT);
	EPORT_ConfigGpio(EPORT_PIN2, GPIO_INPUT);
	EPORT_ConfigGpio(EPORT_PIN3, GPIO_INPUT);
	EPORT_ConfigGpio(EPORT_PIN4, GPIO_INPUT);
	
	while(1)
	{
		//检测USB是否有插入
		if (get_usb_det_sta() == TRUE)
		{
			printf("USB DET STA IS H\r\n");
		}

		//eport 1 - normal sleep
		if(EPORT_ReadGpioData(EPORT_PIN1) == 0)
		{
			DelayMS(50);
			if(EPORT_ReadGpioData(EPORT_PIN1) == 0)
			{

				printf("Enter sleep...\r\n");
				CPM_Sleep();
				printf("wake up from sleep.\r\n");
			}
		}

		//eport 2 - poweroff 1
		if(EPORT_ReadGpioData(EPORT_PIN2) == 0)
		{
			DelayMS(50);
			if(EPORT_ReadGpioData(EPORT_PIN2) == 0)
			{

				printf("enter power off 1...\r\n");
				CPM_PowerOff_1();		//POFF1，将栈和上电需要初始化的全局变量放到DRAM中，即0x1FFF8000~0x1FFFFFFF
				WDT_Close(); //关闭CLK
				TC_Close(); //关闭TC
				UART_Debug_Init(SCI1,g_ips_clk,115200);         //初始化串口
				printf("wake up from sleep.\r\n");
			}
		}
		
		//eport 3 - poweroff 1.5
		if(EPORT_ReadGpioData(EPORT_PIN3) == 0)
		{
			DelayMS(50);
			if(EPORT_ReadGpioData(EPORT_PIN3) == 0)
			{

				printf("enter power off 1.5...\r\n");
				CPM_PowerOff_1p5();
			}
		}

		//eport 4 - poweroff 2
		if(EPORT_ReadGpioData(EPORT_PIN4) == 0)
		{
			DelayMS(50);
			if(EPORT_ReadGpioData(EPORT_PIN4) == 0)
			{
				printf("\n\tenter power off 2...\n");
				CPM_PowerOff_2();
			}
		}
	}
}
