/***************************************************************************************
**�ļ���:     DRV_EEPROM.h
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
#ifndef __DRV_EEPROM_H__
#define __DRV_EEPROM_H__
#include "MyConfig.h"
#define DRV_EEPROM_MODULE_EN (1) //ģ��ʹ��
#if DRV_EEPROM_MODULE_EN
/*************************ר�ú궨��*************************/
#define DRV_EEPROM_PAGE_SIZE (128)   //ҳ��С
#define DRV_EEPROM_READ (1)          //��
#define DRV_EEPROM_WRITE (0)         //д
#define DRV_EEPROM_WRITE_ADDR (0XA6) //д��ַ
#define DRV_EEPROM_READ_ADDR (0XA7)  //д��ַ
#define DRV_EEPROM_WRITE_PROTECT (1) //д����
/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_EEPROM_GLOBALS
#define DRV_EEPROM_EXT
#else
#define DRV_EEPROM_EXT extern
#endif /*DRV_EEPROM_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
    DRV_EEPROM_EXT INT8S DRV_EEPROM_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_EEPROM_MODULE_EN*/

#endif /*DRV_EEPROM_MODULE_EN*/
#endif /*__DRV_EEPROM_H__*/
/************************************Code End******************************************/
