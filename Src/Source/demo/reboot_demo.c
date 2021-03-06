// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : reboot_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "eflash_drv.h"
#include "reboot_demo.h"
#include "i2c_drv.h"
#include "i2c_reg.h"
#include "type.h"
#include "debug.h"
#include "cpm_drv.h"
#include "reset_drv.h"

void Reboot_Demo(void)
{
	UINT32 eflash_clk;
	
	if(0 == g_sys_clk)
	{
		eflash_clk = Get_Sys_Clock();
	}
	else
	{
		eflash_clk = g_sys_clk;
	}
	
	
	EFLASH_Init(eflash_clk/1000);
	
	EFLASH_SetWritePermission();
	
	EFlash_Recovery_to_Boot();
    
    //恢复boot后，不断电直接重启
    Set_POR_Reset();
	 
	while(1);
}

void I2C_Config_Gpio_Reboot(I2C_PIN I2C_PINx, UINT8 GPIO_Dir)
{
	I2C_TypeDef *i2c = (I2C_TypeDef *)I2C1_BASE_ADDR;
	//i2c = (I2C_TypeDef *)I2C1_BASE_ADDR;
	
	*(volatile UINT32*)(0x40023074) &= ~0x00000040;		//SDA和TSI CH6复用，禁止TSI CH6

	i2c->PCR |= 0xc0;//config gpio
	if (GPIO_Dir == GPIO_OUTPUT)
	{
		i2c->DDR |= (1<<I2C_PINx);//output
	}
	else if (GPIO_Dir == GPIO_INPUT)
	{
		i2c->DDR &= (~(1<<I2C_PINx));//input
	}
}

UINT8 I2C_Read_Gpio_Data_Reboot(I2C_PIN I2C_PINx)
{
	UINT8 bitstatus = 0x00;

	I2C_TypeDef *i2c = (I2C_TypeDef *)I2C1_BASE_ADDR;

	bitstatus = i2c->PDR;
	if (bitstatus &(Bit_SET<<I2C_PINx))
		bitstatus = Bit_SET;
	else
		bitstatus = Bit_RESET;
	
	i2c->PCR &= (~0xc0);	//配置成i2c功能

	return bitstatus;
}

