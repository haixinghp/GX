/***************************************************************************************
**文件名:     DRV_Adc.c
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

#include "type.h"
#include "debug.h"
//#include "adc_drv.h"
#include "adc_demo.h"
#include <string.h>
#include "delay.h"

#include "DRV_Adc.h"
#if DRV_ADC_MODULE_EN
#warning DRV_ADC模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Adc_Init(void)
{
	M_BAT_MS_OUT_PP();
	M_BAT_MS_OUT_0();
}
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Adc_MsSw(uint8_t sw)
{
	if (0 == sw)
    {
       M_BAT_MS_OUT_0();
    }
    else
	{
		M_BAT_MS_OUT_1();
	}
}
/***************************************************************************************
**函数名:       DRV_Adc_Conversion
**功能描述:     ADC转换
uint8_t chx
转换的通道
**输入参数:     
uint32_t
直接获取电压值
**输出参数:     --
**备注:         已经多次转换并滤波,并把数据扩大相应倍数
****************************************************************************************/
uint32_t DRV_Adc_Conversion(uint8_t chx)
{
    return (3.3*(ADC_GetConversionValue(chx)*1.0)/4095)*100;
    
}
/***************************************************************************************
**函数名:       DRV_Adc_EnterSleepConfig
**功能描述:     进入休眠
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Adc_EnterSleepConfig(void)
{
	M_BAT_MS_OUT_PP();
	M_BAT_MS_OUT_0();
}
#endif /*DRV_ADC_MODULE_EN*/
/************************************Code End******************************************/
