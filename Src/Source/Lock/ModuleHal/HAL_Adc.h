/***************************************************************************************
**�ļ���:     HAL_Adc.h
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
#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__
#include "MyConfig.h"
#define HAL_ADC_MODULE_EN (1) //ģ��ʹ��
#if HAL_ADC_MODULE_EN
/*************************ר�ú궨��*************************/

/*************************�����ṹ����*************************/
typedef enum
{
    eAdcPort1 = 0x00, //ͨ��1
    eAdcPort2 = 0x01, //ͨ��2
} TYPEe_AdcPort;
typedef enum
{
    eAdcSwClose = 0x00, 
    eAdcSwOpen = 0x01, 
} TYPEe_AdcSw;
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_ADC_GLOBALS
#define HAL_ADC_EXT
#else
#define HAL_ADC_EXT extern
#endif /*HAL_ADC_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
    HAL_ADC_EXT void HAL_Adc_Init(void);
    HAL_ADC_EXT void HAL_Adc_Conversion(uint16_t *upBat,uint16_t *downBat);
    HAL_ADC_EXT void HAL_Adc_MsSw(TYPEe_AdcSw sw);
    HAL_ADC_EXT void HAL_Adc_EnterSleepConfig(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_ADC_MODULE_EN*/

#endif /*HAL_ADC_MODULE_EN*/
#endif /*__HAL_ADC_H__*/
/************************************Code End******************************************/
