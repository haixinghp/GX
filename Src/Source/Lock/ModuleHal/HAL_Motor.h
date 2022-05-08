/***************************************************************************************
**文件名:     HAL_Motor.h
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
#ifndef __HAL_MOTOR_H__
#define __HAL_MOTOR_H__
#include "MyConfig.h"
#define HAL_MOTOR_MODULE_EN (1) //模块使能
#if HAL_MOTOR_MODULE_EN
/*************************专用宏定义*************************/
typedef enum
{
    eMotorRunStatusNone,       //无动作
    eMotorRunStatusOpen,       //开门
    eMotorRunStatusOpenWait,   //开门中
    eMotorRunStatusOpen1,      //再次开门
    eMotorRunStatusOpen1Wait,  //再次开门中
    eMotorRunStatusOpenOk,     //开门成功
    eMotorRunStatusOpenOkWait, //开门成功等待
    eMotorRunStatusClose,      //关门
    eMotorRunStatusCloseWait,  //关门中
    eMotorRunStatusClose1,     //再次关门
    eMotorRunStatusClose1Wait, //再次关门中
    eMotorRunStatusCloseOK,    //关门完成
} TYPEe_MotorRunStatus;
/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_MOTOR_GLOBALS
#define HAL_MOTOR_EXT
#else
#define HAL_MOTOR_EXT extern
#endif /*HAL_MOTOR_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    HAL_MOTOR_EXT void HAL_Motor_Init(void);
    HAL_MOTOR_EXT void HAL_Motor_OpenClose(void);
    HAL_MOTOR_EXT void HAL_Motor_Open(void);
    HAL_MOTOR_EXT void HAL_Motor_Close(void);
    HAL_MOTOR_EXT INT8S HAL_Motor_Deal(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_MOTOR_MODULE_EN*/

#endif /*HAL_MOTOR_MODULE_EN*/
#endif /*__HAL_MOTOR_H__*/
/************************************Code End******************************************/
