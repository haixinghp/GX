/***************************************************************************************
**�ļ���:     DRV_EEPROM.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
****************************************************************************************/
#include "DRV_EEPROM.h"
#if DRV_EEPROM_MODULE_EN
#warning DRV_EEPROMģ��ʹ��!
/*******************************�궨��***************************/

/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_EEPROM_ReadWrite
**��������:     EEPROM ��д
**�������:     
INT8U flag, д����߶�ȡ
INT32U addr, ������ַ
INT8U *data, ����ָ��
INT16U length ���������ݳ���
**�������:     
INT8S
0 �ɹ�
-1 ͨѶʧ��
**��ע:         �Զ���ҳд��
****************************************************************************************/
INT8S DRV_EEPROM_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length)
{
	uint8 rw_len, page_offset;
	M_EEPROM_WP_OUT_PP(); //д����
	if (flag == 0)
	{

		while (length > 0)
		{
#ifdef DRV_EEPROM_WRITE_PROTECT
			M_EEPROM_WP_FUN_OFF; //�ر�EEPROMд����
#endif
			page_offset = DRV_EEPROM_PAGE_SIZE - (addr % DRV_EEPROM_PAGE_SIZE); //????�̡�?�㨰3���ꨮ����??����?D?D���??��3��?��?��
			rw_len = length > page_offset ? page_offset : length;				//?D??�̡�?�㨰3���ꨮ����?��?��3��?D?D�䨨?��y?Y3��?��?��
			//--------��?D?D�䨨?2������------------------------------
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
			length = length - rw_len; //?????1��D��???��DD�䨨?��?��y?Y3��?��?��
			nrf_delay_ms(6);
			if (length > 0)
			{
				addr = addr + rw_len;
			}
		}
#ifdef DRV_EEPROM_WRITE_PROTECT
		M_EEPROM_WP_FUN_ON; //��EEPROMд����
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
// 	M_EEPROM_WP_OUT_PP(); //д����
// 						  //	nrf_delay_ms(10);	  //ͣ��
// 	if (NULL == data)
// 	{
// 		return -2; //��������
// 	}
// 	if (flag == DRV_EEPROM_WRITE)
// 	{
// 		M_EEPROM_WP_FUN_OFF; //�ر�EEPROMд����
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
// 		M_EEPROM_WP_FUN_ON; //��EEPROMд����
// 		return 0;			//�����ɹ�
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