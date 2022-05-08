/***************************************************************************************
**文件名:     DRV_RTC.h
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
#ifndef   __DRV_RTC_H__
#define   __DRV_RTC_H__
#include "MyConfig.h"
#define DRV_RTC_MODULE_EN (1) //模块使能
#if DRV_RTC_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  
typedef struct t_DateTime
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} _DateTime;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_RTC_GLOBALS
#define DRV_RTC_EXT
#else
#define DRV_RTC_EXT extern
#endif     /*DRV_RTC_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
DRV_RTC_EXT void DRV_RTC_Init(void);
DRV_RTC_EXT void DRV_RTC_SetByTick(uint32_t tick);
DRV_RTC_EXT _DateTime DRV_RTC_TimeGet(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_RTC_MODULE_EN*/

#endif    /*DRV_RTC_MODULE_EN*/
#endif     /*__DRV_RTC_H__*/
/************************************Code End******************************************/

