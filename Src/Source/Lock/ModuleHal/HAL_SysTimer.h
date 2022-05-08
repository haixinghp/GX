/***************************************************************************************
**文件名:     HAL_SysTimer.h
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
#ifndef   __HAL_SYS_TIMER_H__
#define   __HAL_SYS_TIMER_H__
#include "MyConfig.h"
#define HAL_SYS_TIMER_MODULE_EN (1) //模块使能
#if HAL_SYS_TIMER_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  
typedef  void (*halSysTimerHandler_t)(void);		//回调函数
typedef struct
{
    volatile INT32U tick;                            //系统滴答计数器	
} TYPEs_sysTimerCtrl;


#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_SYS_TIMER_GLOBALS
#define HAL_SYS_TIMER_EXT
#else
#define HAL_SYS_TIMER_EXT extern
#endif     /*HAL_SYS_TIMER_GLOBALS*/

/*************************公共变量*************************/  
HAL_SYS_TIMER_EXT  TYPEs_sysTimerCtrl sysTimerCtrl; 
/*************************公共函数*************************/
HAL_SYS_TIMER_EXT void HAL_SysTimerInit(void);
HAL_SYS_TIMER_EXT INT8S HAL_SysTimeDeal(void);
HAL_SYS_TIMER_EXT void HAL_SysTimerClose(void);
HAL_SYS_TIMER_EXT void HAL_SysTimer_ClearCnt(void);
HAL_SYS_TIMER_EXT INT8S HAL_SysTimerHandlerReg(halSysTimerHandler_t pfun);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_SYS_TIMER_MODULE_EN*/

#endif    /*HAL_SYS_TIMER_MODULE_EN*/
#endif     /*__HAL_SYS_TIMER_H__*/
/************************************Code End******************************************/

