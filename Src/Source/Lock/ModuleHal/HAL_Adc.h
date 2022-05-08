/***************************************************************************************
**文件名:     HAL_Adc.h
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
#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__
#include "MyConfig.h"
#define HAL_ADC_MODULE_EN (1) //模块使能
#if HAL_ADC_MODULE_EN
/*************************专用宏定义*************************/

/*************************变量结构声明*************************/
typedef enum
{
    eAdcPort1 = 0x00, //通道1
    eAdcPort2 = 0x01, //通道2
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

    /*************************公共变量*************************/

    /*************************公共函数*************************/
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
