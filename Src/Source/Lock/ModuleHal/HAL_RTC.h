/***************************************************************************************
**文件名:     HAL_RTC.h
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件 
**备注:      
stm = localtime((const time_t *)&tick);

****************************************************************************************/
#ifndef   __HAL_RTC_H__
#define   __HAL_RTC_H__
#include "MyConfig.h"
#include <time.h>
#define HAL_RTC_MODULE_EN (1) //模块使能
#if HAL_RTC_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  
//typedef struct tm TYPEs_Stm;//标准时间,定义在标准库文件<time.h>中
//typedef struct
//{
//	struct tm stm; //标准时间,定义在标准库文件<time.h>中
//}TYPEs_RtcTime;
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_RTC_GLOBALS
#define HAL_RTC_EXT
#else
#define HAL_RTC_EXT extern
#endif     /*HAL_RTC_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
HAL_RTC_EXT void HAL_RTC_Init(void);
HAL_RTC_EXT void HAL_RTC_SetByTick(uint32_t tick);
HAL_RTC_EXT uint8_t HAL_RTC_TimeGetStm(struct tm *stmTmp);
HAL_RTC_EXT uint32_t HAL_RTC_Stm2Tick(struct tm tim);
HAL_RTC_EXT struct tm * HAL_RTC_Tick2Stm(uint32_t tick);
HAL_RTC_EXT uint32_t HAL_RTC_GetTick(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_RTC_MODULE_EN*/

#endif    /*HAL_RTC_MODULE_EN*/
#endif     /*__HAL_RTC_H__*/
/************************************Code End******************************************/

