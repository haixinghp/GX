/***************************************************************************************
**文件名:     DRV_Alarm.h
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
#ifndef __DRV_ALARM_H__
#define __DRV_ALARM_H__
#include "MyConfig.h"
#define DRV_ALARM_MODULE_EN (1) //模块使能
#if DRV_ALARM_MODULE_EN
/*************************专用宏定义*************************/


/*************************变量结构声明*************************/
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

  /*************************公共变量*************************/
  DRV_ALARM_EXT AlarmIntHandler_t AlarmIntHandler;
  /*************************公共函数*************************/
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
