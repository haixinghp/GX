/***************************************************************************************
**文件名:     HAL_Alarm.h
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
#ifndef   __HAL_ALARM_H__
#define   __HAL_ALARM_H__
#include "MyConfig.h"
#include "DRV_Alarm.h"
#define HAL_ALARM_MODULE_EN (1) //模块使能
#if HAL_ALARM_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_ALARM_GLOBALS
#define HAL_ALARM_EXT
#else
#define HAL_ALARM_EXT extern
#endif     /*HAL_ALARM_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
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


