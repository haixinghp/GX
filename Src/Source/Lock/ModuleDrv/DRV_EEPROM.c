/***************************************************************************************
**文件名:     DRV_EEPROM.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
****************************************************************************************/
#include "DRV_EEPROM.h"
#if DRV_EEPROM_MODULE_EN
#warning DRV_EEPROM模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_EEPROM_ReadWrite
**功能描述:     EEPROM 读写
**输入参数:     
INT8U flag, 写入或者读取
INT32U addr, 操作地址
INT8U *data, 操作指针
INT16U length 操作的数据长度
**输出参数:     
INT8S
0 成功
-1 通讯失败
**备注:         自动分页写入
****************************************************************************************/
INT8S DRV_EEPROM_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length)
{
	uint8 rw_len, page_offset;
	M_EEPROM_WP_OUT_PP(); //写保护
	if (flag == 0)
	{

		while (length > 0)
		{
#ifdef DRV_EEPROM_WRITE_PROTECT
			M_EEPROM_WP_FUN_OFF; //关闭EEPROM写保护
#endif
			page_offset = DRV_EEPROM_PAGE_SIZE - (addr % DRV_EEPROM_PAGE_SIZE); //????μ±?°ò3ê￡óà￡??éá?D?D′μ??·3¤?è?￡
			rw_len = length > page_offset ? page_offset : length;				//?D??μ±?°ò3ê￡óà￡?±?ò3á?D?D′è?êy?Y3¤?è?￡
			//--------á?D?D′è?2ù×÷------------------------------
			if (!I2C_Start_I2c())
			{
				return -1;
			}
			I2C_SendByte(DRV_EEPROM_WRITE_ADDR);
			if (!I2C_WaitAck_I2c())
			{
				I2C_Stop_I2c();
				return -1;
			}
			I2C_SendByte((addr >> 8) & 0xff);
			I2C_WaitAck_I2c();
			I2C_SendByte((addr >> 0) & 0xff);
			I2C_WaitAck_I2c();
			for (INT16U i = 0; i < rw_len; i++)
			{
				I2C_SendByte(*data);
				data++;
				I2C_WaitAck_I2c();
			}
			I2C_Stop_I2c();
			//==================================================
			length = length - rw_len; //?????1óD￡???óDD′è?μ?êy?Y3¤?è?￡
			nrf_delay_ms(6);
			if (length > 0)
			{
				addr = addr + rw_len;
			}
		}
#ifdef DRV_EEPROM_WRITE_PROTECT
		M_EEPROM_WP_FUN_ON; //打开EEPROM写保护
#endif
		return 0;
	}
	else
	{

		if (!I2C_Start_I2c())
		{
			return -1;
		}
		I2C_SendByte(DRV_EEPROM_WRITE_ADDR);
		if (!I2C_WaitAck_I2c())
		{
			I2C_Stop_I2c();
			return -1;
		}
		I2C_SendByte((addr >> 8) & 0xff);
		I2C_WaitAck_I2c();
		I2C_SendByte((addr >> 0) & 0xff);
		I2C_WaitAck_I2c();
		I2C_Start_I2c();
		I2C_SendByte(DRV_EEPROM_WRITE_ADDR | 1);
		I2C_WaitAck_I2c();
		for (INT16U i = 0; i < length; i++, data++)
		{
			*data = I2C_RcvByte();
			if (i < (length - 1))
			{
				I2C_Ack_I2c();
			}
		}
		I2C_NoAck_I2c();
		I2C_Stop_I2c();
		return 0;
	}
}
// INT8S DRV_EEPROM_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length)
// {
// 	M_EEPROM_WP_OUT_PP(); //写保护
// 						  //	nrf_delay_ms(10);	  //停顿
// 	if (NULL == data)
// 	{
// 		return -2; //参数错误
// 	}
// 	if (flag == DRV_EEPROM_WRITE)
// 	{
// 		M_EEPROM_WP_FUN_OFF; //关闭EEPROM写保护
// 		if (!I2C_Start_I2c())
// 		{
// 			return -1;
// 		}
// 		I2C_SendByte(DRV_EEPROM_WRITE_ADDR);
// 		if (!I2C_WaitAck_I2c())
// 		{
// 			I2C_Stop_I2c();
// 			return -1;
// 		}
// 		I2C_SendByte((addr >> 8) & 0xff);
// 		I2C_WaitAck_I2c();
// 		I2C_SendByte((addr >> 0) & 0xff);
// 		I2C_WaitAck_I2c();
// 		while (length--)
// 		{
// 			I2C_SendByte(*data);
// 			data++;
// 			I2C_WaitAck_I2c();
// 		}
// 		I2C_Stop_I2c();
// 		M_EEPROM_WP_FUN_ON; //打开EEPROM写保护
// 		return 0;			//操作成功
// 	}
// 	else
// 	{
// 		if (!I2C_Start_I2c())
// 		{
// 			return -1;
// 		}
// 		I2C_SendByte(DRV_EEPROM_WRITE_ADDR);
// 		if (!I2C_WaitAck_I2c())
// 		{
// 			I2C_Stop_I2c();
// 			return -1;
// 		}
// 		I2C_SendByte((addr >> 8) & 0xff);
// 		I2C_WaitAck_I2c();
// 		I2C_SendByte((addr >> 0) & 0xff);
// 		I2C_WaitAck_I2c();
// 		I2C_Start_I2c();
// 		I2C_SendByte(DRV_EEPROM_READ_ADDR);
// 		I2C_WaitAck_I2c();
// 		for (INT16U i = 0; i < length; i++, data++)
// 		{
// 			*data = I2C_RcvByte();
// 			if (i < (length - 1))
// 			{
// 				I2C_Ack_I2c();
// 			}
// 		}
// 		I2C_NoAck_I2c();
// 		I2C_Stop_I2c();
// 		return 0;
// 	}
// }

#endif /*DRV_EEPROM_MODULE_EN*/
	   /************************************Code End******************************************/