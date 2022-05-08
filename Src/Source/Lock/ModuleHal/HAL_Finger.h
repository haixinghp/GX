/***************************************************************************************
**�ļ���:     HAL_Finger.h
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
#ifndef __HAL_FINGER_H__
#define __HAL_FINGER_H__
#include "MyConfig.h"
#define HAL_FINGER_MODULE_EN (1) //ģ��ʹ��
#if HAL_FINGER_MODULE_EN
/*************************ר�ú궨��*************************/

/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_FINGER_GLOBALS
#define HAL_FINGER_EXT
#else
#define HAL_FINGER_EXT extern
#endif /*HAL_FINGER_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
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
