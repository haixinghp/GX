/***************************************************************************************
**�ļ���:     HAL_Alarm.h
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
#ifndef   __HAL_ALARM_H__
#define   __HAL_ALARM_H__
#include "MyConfig.h"
#include "DRV_Alarm.h"
#define HAL_ALARM_MODULE_EN (1) //ģ��ʹ��
#if HAL_ALARM_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_ALARM_GLOBALS
#define HAL_ALARM_EXT
#else
#define HAL_ALARM_EXT extern
#endif     /*HAL_ALARM_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
HAL_ALARM_EXT void HAL_Alarm_SetWakeUp(void);
HAL_ALARM_EXT void HAL_Alarm_SetInterrpt(void);
HAL_ALARM_EXT int8_t HAL_Alarm_InterruptReg(AlarmIntHandler_t fun);
HAL_ALARM_EXT int8_t HAL_Alarm_GetStatus(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_ALARM_MODULE_EN*/

#endif    /*HAL_ALARM_MODULE_EN*/
#endif     /*__HAL_ALARM_H__*/
/************************************Code End******************************************/


