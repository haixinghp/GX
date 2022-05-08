/***************************************************************************************
**�ļ���:     HAL_KeySacn.h
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
#ifndef   __HAL_KEY_SCAN_H__
#define   __HAL_KEY_SCAN_H__
#include "MyConfig.h"
#define HAL_KEY_SCAN_MODULE_EN (1) //ģ��ʹ��
#if HAL_KEY_SCAN_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_KEY_SCAN_GLOBALS
#define HAL_KEY_SCAN_EXT
#else
#define HAL_KEY_SCAN_EXT extern
#endif     /*HAL_KEY_SCAN_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
HAL_KEY_SCAN_EXT void HAL_KeySacnInit(void);
HAL_KEY_SCAN_EXT void HAL_KeySacnIsrFlagSet(void);
HAL_KEY_SCAN_EXT void HAL_KeySacnIsrFlagClear(void);
HAL_KEY_SCAN_EXT INT8U HAL_KeySacnGet(void);
HAL_KEY_SCAN_EXT void HAL_KeySacn_EnterSleepConfig(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_KEY_SCAN_MODULE_EN*/

#endif    /*HAL_KEY_SCAN_MODULE_EN*/
#endif     /*__HAL_KEY_SCAN_H__*/
/************************************Code End******************************************/

