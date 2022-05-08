/***************************************************************************************
**�ļ���:     DRV_Alarm.h
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
#ifndef __DRV_ALARM_H__
#define __DRV_ALARM_H__
#include "MyConfig.h"
#define DRV_ALARM_MODULE_EN (1) //ģ��ʹ��
#if DRV_ALARM_MODULE_EN
/*************************ר�ú궨��*************************/


/*************************�����ṹ����*************************/
typedef void (*AlarmIntHandler_t)(void);
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_ALARM_GLOBALS
#define DRV_ALARM_EXT
#else
#define DRV_ALARM_EXT extern
#endif /*DRV_ALARM_GLOBALS*/

  /*************************��������*************************/
  DRV_ALARM_EXT AlarmIntHandler_t AlarmIntHandler;
  /*************************��������*************************/
  DRV_ALARM_EXT void DRV_Alarm_SetWakeUp(void);
  DRV_ALARM_EXT void DRV_Alarm_SetInterrpt(void);
  DRV_ALARM_EXT void DRV_Alarm_InterrptService(EPORT_PINx pin);
  DRV_ALARM_EXT int8_t DRV_Alarm_GetStatus(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_ALARM_MODULE_EN*/

#endif /*DRV_ALARM_MODULE_EN*/
#endif /*__DRV_ALARM_H__*/
/************************************Code End******************************************/
