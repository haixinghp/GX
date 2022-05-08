/***************************************************************************************
**�ļ���:     DRV_Storage.h
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
#ifndef   __DRV_STORAGE_H__
#define   __DRV_STORAGE_H__
#include "MyConfig.h"
#define DRV_STORAGE_MODULE_EN (1) //ģ��ʹ��
#if DRV_STORAGE_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_STORAGE_GLOBALS
#define DRV_STORAGE_EXT
#else
#define DRV_STORAGE_EXT extern
#endif     /*DRV_STORAGE_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_STORAGE_EXT void DRV_Storage_Init(void);
DRV_STORAGE_EXT INT8S DRV_Storage_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length);
DRV_STORAGE_EXT void DRV_Storage_EnterSleepConfig(void);
DRV_STORAGE_EXT INT8S DRV_Storage_EraseSector(uint32_t Addr);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_STORAGE_MODULE_EN*/

#endif    /*DRV_STORAGE_MODULE_EN*/
#endif     /*__DRV_STORAGE_H__*/
/************************************Code End******************************************/

