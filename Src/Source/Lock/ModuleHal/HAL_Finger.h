/***************************************************************************************
**文件名:     HAL_Finger.h
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
#ifndef __HAL_FINGER_H__
#define __HAL_FINGER_H__
#include "MyConfig.h"
#define HAL_FINGER_MODULE_EN (1) //模块使能
#if HAL_FINGER_MODULE_EN
/*************************专用宏定义*************************/

/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_FINGER_GLOBALS
#define HAL_FINGER_EXT
#else
#define HAL_FINGER_EXT extern
#endif /*HAL_FINGER_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    HAL_FINGER_EXT int8_t HAL_Finger_Init(void);
    HAL_FINGER_EXT int8_t HAL_Finger_Reg(uint16_t model);
    HAL_FINGER_EXT int8_t HAL_Finger_VerifyStart(void);
    HAL_FINGER_EXT int8_t HAL_Finger_Verify(uint16_t *id);
    HAL_FINGER_EXT int8_t HAL_Finger_RegStart(void);
    HAL_FINGER_EXT int8_t HAL_Finger_RegReStart(void);
    HAL_FINGER_EXT void HAL_Finger_SaveBufSet(uint8_t dat);
    HAL_FINGER_EXT uint8_t HAL_Finger_SaveBufGet(void);
    HAL_FINGER_EXT int8_t HAL_Finger_DelStart(void);
    HAL_FINGER_EXT int8_t HAL_Finger_Del(uint16_t model, uint16_t num);
    HAL_FINGER_EXT int8_t HAL_Finger_EmptyStart(void);
    HAL_FINGER_EXT int8_t HAL_Finger_Empty(void);
	HAL_FINGER_EXT void HAL_Finger_EnterSleepConfig(void);
	HAL_FINGER_EXT int8_t HAL_Finger_HavePress(void);
    HAL_FINGER_EXT int8_t HAL_Finger_ChkFingerLeave(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_FINGER_MODULE_EN*/

#endif /*HAL_FINGER_MODULE_EN*/
#endif /*__HAL_FINGER_H__*/
/************************************Code End******************************************/
