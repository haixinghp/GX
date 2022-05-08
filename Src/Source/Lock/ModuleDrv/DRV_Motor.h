/***************************************************************************************
**�ļ���:     DRV_Motor.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
****************************************************************************************/
#ifndef   __DRV_MOTOR_H__
#define   __DRV_MOTOR_H__
#include "MyConfig.h"
#define DRV_MOTOR_MODULE_EN (1) //ģ��ʹ��
#if DRV_MOTOR_MODULE_EN
/*************************ר�ú궨��*************************/  
#define M_M_EN_GPIO_PIN           			  ( 24 ) 		//оƬ���ʱ��
#define M_M_EN_OUT_PP()             nrf_gpio_cfg_output(M_M_EN_GPIO_PIN) ;M_M_EN_FUN_OFF
#define M_M_EN_FUN_ON               nrf_gpio_pin_set(M_M_EN_GPIO_PIN)						
#define M_M_EN_FUN_OFF              nrf_gpio_pin_clear(M_M_EN_GPIO_PIN)		
/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_MOTOR_GLOBALS
#define DRV_MOTOR_EXT
#else
#define DRV_MOTOR_EXT extern
#endif     /*DRV_MOTOR_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
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

