/***************************************************************************************
**�ļ���:     HAL_BreathingLight.h
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
#ifndef   __HAL_BREATHING_LIGHT_H__
#define   __HAL_BREATHING_LIGHT_H__
#include "MyConfig.h"
#define HAL_BREATHING_LIGHT_MODULE_EN (1) //ģ��ʹ��
#if HAL_BREATHING_LIGHT_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_BREATHING_LIGHT_GLOBALS
#define HAL_BREATHING_LIGHT_EXT
#else
#define HAL_BREATHING_LIGHT_EXT extern
#endif     /*HAL_BREATHING_LIGHT_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
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


