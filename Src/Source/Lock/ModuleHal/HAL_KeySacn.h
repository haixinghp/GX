/***************************************************************************************
**文件名:     HAL_KeySacn.h
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
#ifndef   __HAL_KEY_SCAN_H__
#define   __HAL_KEY_SCAN_H__
#include "MyConfig.h"
#define HAL_KEY_SCAN_MODULE_EN (1) //模块使能
#if HAL_KEY_SCAN_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_KEY_SCAN_GLOBALS
#define HAL_KEY_SCAN_EXT
#else
#define HAL_KEY_SCAN_EXT extern
#endif     /*HAL_KEY_SCAN_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
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

