/***************************************************************************************
**文件名:     DRV_Motor.h
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
#ifndef   __DRV_MOTOR_H__
#define   __DRV_MOTOR_H__
#include "MyConfig.h"
#define DRV_MOTOR_MODULE_EN (1) //模块使能
#if DRV_MOTOR_MODULE_EN
/*************************专用宏定义*************************/  
#define M_M_EN_GPIO_PIN           			  ( 24 ) 		//芯片检测时钟
#define M_M_EN_OUT_PP()             nrf_gpio_cfg_output(M_M_EN_GPIO_PIN) ;M_M_EN_FUN_OFF
#define M_M_EN_FUN_ON               nrf_gpio_pin_set(M_M_EN_GPIO_PIN)						
#define M_M_EN_FUN_OFF              nrf_gpio_pin_clear(M_M_EN_GPIO_PIN)		
/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_MOTOR_GLOBALS
#define DRV_MOTOR_EXT
#else
#define DRV_MOTOR_EXT extern
#endif     /*DRV_MOTOR_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
DRV_MOTOR_EXT void DRV_Motor_Init(void);
DRV_MOTOR_EXT void DRV_Motor_Stop(void);
DRV_MOTOR_EXT void DRV_Motor_Open(void);
DRV_MOTOR_EXT void DRV_Motor_Close(void);
DRV_MOTOR_EXT INT8S DRV_Motor_LockHandle(void);
DRV_MOTOR_EXT INT8S DRV_Motor_Enable(INT8U enb);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_MOTOR_MODULE_EN*/

#endif    /*DRV_MOTOR_MODULE_EN*/
#endif     /*__DRV_MOTOR_H__*/
/************************************Code End******************************************/

