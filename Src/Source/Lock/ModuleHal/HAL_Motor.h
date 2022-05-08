/***************************************************************************************
**�ļ���:     HAL_Motor.h
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
#ifndef __HAL_MOTOR_H__
#define __HAL_MOTOR_H__
#include "MyConfig.h"
#define HAL_MOTOR_MODULE_EN (1) //ģ��ʹ��
#if HAL_MOTOR_MODULE_EN
/*************************ר�ú궨��*************************/
typedef enum
{
    eMotorRunStatusNone,       //�޶���
    eMotorRunStatusOpen,       //����
    eMotorRunStatusOpenWait,   //������
    eMotorRunStatusOpen1,      //�ٴο���
    eMotorRunStatusOpen1Wait,  //�ٴο�����
    eMotorRunStatusOpenOk,     //���ųɹ�
    eMotorRunStatusOpenOkWait, //���ųɹ��ȴ�
    eMotorRunStatusClose,      //����
    eMotorRunStatusCloseWait,  //������
    eMotorRunStatusClose1,     //�ٴι���
    eMotorRunStatusClose1Wait, //�ٴι�����
    eMotorRunStatusCloseOK,    //�������
} TYPEe_MotorRunStatus;
/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_MOTOR_GLOBALS
#define HAL_MOTOR_EXT
#else
#define HAL_MOTOR_EXT extern
#endif /*HAL_MOTOR_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
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
