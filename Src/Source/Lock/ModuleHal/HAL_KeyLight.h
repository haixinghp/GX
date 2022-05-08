/***************************************************************************************
**文件名:     HAL_KeyLight.h
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
#ifndef __HAL_KEY_LIGHT_H__
#define __HAL_KEY_LIGHT_H__
#include "MyConfig.h"
#include "DRV_MBI5024.h"
#define HAL_KEY_LIGHT_MODULE_EN (1) //模块使能
#if HAL_KEY_LIGHT_MODULE_EN
/*************************专用宏定义*************************/
#define HAL_KEY_LIGHT_ONCE_0 (DRV_MBI5024_LED_0)
#define HAL_KEY_LIGHT_ONCE_1 (DRV_MBI5024_LED_1)
#define HAL_KEY_LIGHT_ONCE_2 (DRV_MBI5024_LED_2)
#define HAL_KEY_LIGHT_ONCE_3 (DRV_MBI5024_LED_3)
#define HAL_KEY_LIGHT_ONCE_4 (DRV_MBI5024_LED_4)
#define HAL_KEY_LIGHT_ONCE_5 (DRV_MBI5024_LED_5)
#define HAL_KEY_LIGHT_ONCE_6 (DRV_MBI5024_LED_6)
#define HAL_KEY_LIGHT_ONCE_7 (DRV_MBI5024_LED_7)
#define HAL_KEY_LIGHT_ONCE_8 (DRV_MBI5024_LED_8)
#define HAL_KEY_LIGHT_ONCE_9 (DRV_MBI5024_LED_9)
#define HAL_KEY_LIGHT_ONCE_E (DRV_MBI5024_LED_E)
#define HAL_KEY_LIGHT_ONCE_M (DRV_MBI5024_LED_M)

#define HAL_KEY_BACK_LIGHT_ALL_ON (HAL_KEY_LIGHT_ONCE_1 | \
                                   HAL_KEY_LIGHT_ONCE_2 | \
                                   HAL_KEY_LIGHT_ONCE_3 | \
                                   HAL_KEY_LIGHT_ONCE_4 | \
                                   HAL_KEY_LIGHT_ONCE_5 | \
                                   HAL_KEY_LIGHT_ONCE_6 | \
                                   HAL_KEY_LIGHT_ONCE_7 | \
                                   HAL_KEY_LIGHT_ONCE_8 | \
                                   HAL_KEY_LIGHT_ONCE_9 | \
                                   HAL_KEY_LIGHT_ONCE_0 | \
                                   HAL_KEY_LIGHT_ONCE_M | \
                                   HAL_KEY_LIGHT_ONCE_E)
#define HAL_KEY_BACK_LIGHT_FACTORY (HAL_KEY_LIGHT_ONCE_E | \
                                     HAL_KEY_LIGHT_ONCE_M)
#define HAL_BAT_GREEN (0x2000)
#define HAL_BAT_RED (0x1000)
#define HAL_SYS_GREEN (0x8000)
#define HAL_SYS_RED (0x4000)
// #define HAL_BAT_RED (0x2000)
/*************************变量结构声明*************************/
typedef enum
{
    eColorBlue,  //蓝色
    eColorRed,   //红色
    eColorGreen, //绿色
    eColorClose, //关闭
} TYPEe_Color;

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_KEY_LIGHT_GLOBALS
#define HAL_KEY_LIGHT_EXT
#else
#define HAL_KEY_LIGHT_EXT extern
#endif /*HAL_KEY_LIGHT_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLightInit(void);
    HAL_KEY_LIGHT_EXT void HAL_KeyLightDeal(void);
    HAL_KEY_LIGHT_EXT void HAL_KeyLightPressKey(INT8U key,INT16U dataTmp);
    HAL_KEY_LIGHT_EXT void HAL_KeyLightOnce(INT16U dat);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLight_ChkBusy(void);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLightDataSet(INT16U const *dat, INT16U len, INT16U flickerTime, INT16U repeatCnt);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLight_batterySet(TYPEe_Color battery);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLight_sysStatusSet(TYPEe_Color sysStatus);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLightStatusRun(INT8U cmd);
    HAL_KEY_LIGHT_EXT INT8S HAL_KeyLightUniteData(INT8U *dat, INT8U len);
    HAL_KEY_LIGHT_EXT void HAL_KeyLight_EnterSleepConfig(void);
	HAL_KEY_LIGHT_EXT INT8S HAL_KeyLight_batterySetDir(TYPEe_Color battery);
	HAL_KEY_LIGHT_EXT void HAL_KeyLight_DirDispaly(uint16_t data);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_KEY_LIGHT_MODULE_EN*/

#endif /*HAL_KEY_LIGHT_MODULE_EN*/
#endif /*__HAL_KEY_LIGHT_H__*/
/************************************Code End******************************************/
