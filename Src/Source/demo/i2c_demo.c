// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : adc_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "i2c_demo.h"
#include "i2c_drv.h"
#include "type.h"
#include "debug.h"
#include "delay.h"
#include "common.h"

#define I2C_MODE_TEST    1

//#define I2C_MASTER_DEMO

void I2C_GPIO_Test(void)
{
#if 0
	I2C_ConfigGpio(I2C_SDA, GPIO_OUTPUT);
	I2C_ConfigGpio(I2C_SCL, GPIO_OUTPUT);
	
	while(1)
	{
		I2C_WriteGpioData(I2C_SDA, Bit_SET);
		I2C_WriteGpioData(I2C_SCL, Bit_SET);	
		DelayMS(500);
		I2C_WriteGpioData(I2C_SDA, Bit_RESET);
		I2C_WriteGpioData(I2C_SCL, Bit_RESET);
		DelayMS(500);
	}
	
#else
//	I2C_ConfigGpio(I2C_SDA, GPIO_INPUT);
//	I2C_ConfigGpio(I2C_SCL, GPIO_INPUT);
//	
//	while(1)
//	{
//		printf("I2C_SDA: %02x\r\n",I2C_ReadGpioData(I2C_SDA));
//		printf("I2C_SCL: %02x\r\n",I2C_ReadGpioData(I2C_SCL));
//		printf("***************************8\r\n");
//		DelayMS(1000);
//	}
#endif
}

void I2C_Demo(void)
{
#if I2C_MODE_TEST
	
#ifdef I2C_MASTER_DEMO

	UINT8 ret_val = 0;
	UINT8 i2c_write_buf[256];
	UINT8 i2c_read_buf[256];
	UINT32 i;

	for(i=0;i<0xc0;i++)
	{
		i2c_write_buf[i] = (i+1);
		i2c_read_buf[i] = 0;
	}

	ret_val = i2c_master_init(0x0);

	if(ret_val == FALSE)
	{
		printf("The input arguments have errors\n");
	}
	I2c_writedata(0x230, i2c_write_buf, 0xc0);

	delay(1000);
	I2c_readdata(0x230, i2c_read_buf, 0xc0);

	for(i=0;i<0xc0;i++)
	{
		if(i2c_read_buf[i] != i2c_write_buf[i])
		{
			printf("The i2c driver fails\n");
			return;
		}
	}

	printf("The i2c driver succeeds\n");

#else
	UINT8 ret_val = 0;
	UINT8 i2c_receive_buf[256];

	UINT32 i;

	for(i=0;i<0xc0;i++)
	{
		i2c_receive_buf[i] = 0;
	}

	ret_val = i2c_slave_init(0x230);

	if(ret_val == FALSE)
	{
		printf("The input arguments have errors\n");
	}

	while(1)
	{
		i2c_slave_trans(i2c_receive_buf, 0xc0, i2c_receive_buf, 0xc0);
		
		for(i = 0;i<0xc0;i++)
		{
			printf("%02x",i2c_receive_buf[i]);
		}
		printf("\r\n");
	}

#endif
	
#else		//I2C GPIO test
	I2C_GPIO_Test();
#endif
}


