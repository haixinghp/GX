/***************************************************************************************
**�ļ���:     DRV_Adc.h
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
#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__
#include "MyConfig.h"
#include "adc_drv.h"
#define DRV_ADC_MODULE_EN (1) //ģ��ʹ��
#if DRV_ADC_MODULE_EN
/*************************ר�ú궨��*************************/


#define DRV_ADCCH_0 (ADCCH_0)
#define DRV_ADCCH_1 (ADCCH_1)
#define DRV_ADCCH_2 (ADCCH_2)
#define DRV_ADCCH_9 (ADCCH_9)
#define DRV_ADCCH_IN_0 (ADCCH_IN_0)
#define DRV_ADCCH_IN_1 (ADCCH_IN_1)
#define DRV_ADCCH_IN_2 (ADCCH_IN_2)
#define DRV_ADCCH_IN_3 (ADCCH_IN_3)
#define DRV_ADCCH_IN_4 (ADCCH_IN_4)
#define DRV_ADCCH_IN_5 (ADCCH_IN_5)
#define DRV_ADCCH_IN_6 (ADCCH_IN_6)
#define DRV_ADCCH_IN_7 (ADCCH_IN_7)
#define DRV_ADC_DIS (ADC_DIS)
#define DRV_ADC_EN (ADC_EN)
/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_ADC_GLOBALS
#define DRV_ADC_EXT
#else
#define DRV_ADC_EXT extern
#endif /*DRV_ADC_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
	DRV_ADC_EXT void DRV_Adc_Init(void);
	DRV_ADC_EXT void DRV_Adc_MsSw(uint8_t sw);
	DRV_ADC_EXT uint32_t DRV_Adc_Conversion(uint8_t chx);
	DRV_ADC_EXT void DRV_Adc_EnterSleepConfig(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_ADC_MODULE_EN*/

#endif /*DRV_ADC_MODULE_EN*/
#endif /*__DRV_ADC_H__*/
/************************************Code End******************************************/
