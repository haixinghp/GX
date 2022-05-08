/***************************************************************************************
**�ļ���:     DRV_I2C.h
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
#ifndef   __DRV_I2C_H__
#define   __DRV_I2C_H__
#include "MyConfig.h"
#define DRV_I2C_MODULE_EN (1) //ģ��ʹ��
#if DRV_I2C_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_I2C_GLOBALS
#define DRV_I2C_EXT
#else
#define DRV_I2C_EXT extern
#endif     /*DRV_I2C_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_I2C_EXT INT8U DRV_I2C_WriteNByte(INT8U i2cAddr,INT8U addr, INT8U *data,INT32U len);
DRV_I2C_EXT INT8U DRV_I2C_ReadNByte(INT8U i2cAddr,INT8U addr, INT8U *data,INT32U len);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_I2C_MODULE_EN*/
#endif    /*DRV_I2C_MODULE_EN*/
#endif     /*__DRV_I2C_H__*/
/************************************Code End******************************************/

