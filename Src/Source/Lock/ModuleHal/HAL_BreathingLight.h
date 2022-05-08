/***************************************************************************************
**文件名:     HAL_BreathingLight.h
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
#ifndef   __HAL_BREATHING_LIGHT_H__
#define   __HAL_BREATHING_LIGHT_H__
#include "MyConfig.h"
#define HAL_BREATHING_LIGHT_MODULE_EN (1) //模块使能
#if HAL_BREATHING_LIGHT_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_BREATHING_LIGHT_GLOBALS
#define HAL_BREATHING_LIGHT_EXT
#else
#define HAL_BREATHING_LIGHT_EXT extern
#endif     /*HAL_BREATHING_LIGHT_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
HAL_BREATHING_LIGHT_EXT void HAL_BreathingLightRed(void);
HAL_BREATHING_LIGHT_EXT void HAL_BreathingLightBlue(void);
HAL_BREATHING_LIGHT_EXT void HAL_BreathingLightGreen(void);
HAL_BREATHING_LIGHT_EXT void HAL_BreathingLightDeal(void);
HAL_BREATHING_LIGHT_EXT void HAL_BreathingLightOff(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_BREATHING_LIGHT_MODULE_EN*/

#endif    /*HAL_BREATHING_LIGHT_MODULE_EN*/
#endif     /*__HAL_BREATHING_LIGHT_H__*/
/************************************Code End******************************************/


