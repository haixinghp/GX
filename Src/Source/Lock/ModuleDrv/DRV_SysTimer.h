/***************************************************************************************
**文件名:     DRV_SysTimer.h
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
#ifndef   __DRV_SYS_TIMER_H__
#define   __DRV_SYS_TIMER_H__
#include "MyConfig.h"
#define DRV_SYS_TIMER_MODULE_EN (1) //模块使能
#if DRV_SYS_TIMER_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  
typedef  void (*sysTimerHandler_t)(void);		//回调函数
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_SYS_TIMER_GLOBALS
#define DRV_SYS_TIMER_EXT
#else
#define DRV_SYS_TIMER_EXT extern
#endif     /*DRV_SYS_TIMER_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
DRV_SYS_TIMER_EXT INT8S DRV_SysTimerHandlerReg(sysTimerHandler_t pfun);
DRV_SYS_TIMER_EXT void DRV_SysTimerInit(void);
DRV_SYS_TIMER_EXT void DRV_SysTimerClose(void);
DRV_SYS_TIMER_EXT void DRV_SysTimerHandler(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_SYS_TIMER_MODULE_EN*/

#endif    /*DRV_SYS_TIMER_MODULE_EN*/
#endif     /*__DRV_SYS_TIMER_H__*/
/************************************Code End******************************************/

