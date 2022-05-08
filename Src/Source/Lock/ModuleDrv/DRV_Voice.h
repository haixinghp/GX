/***************************************************************************************
**文件名:     DRV_Voice.h
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
#ifndef __DRV_VOICE_H__
#define __DRV_VOICE_H__
#include "MyConfig.h"
#include "i2c_drv.h"
#define DRV_VOICE_MODULE_EN (1) //模块使能
#if DRV_VOICE_MODULE_EN
/*************************专用宏定义*************************/
/***************************************************************************/ /**
* @brief
*   VOICE驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_VOICE_PIN
#define M_VOICE_PIN (I2C_SDA)
#define M_VOICE_PORT (I2C3)
#define M_VOICE_OUT_PP() I2C_ConfigGpio(M_VOICE_PORT, M_VOICE_PIN, GPIO_OUTPUT)
#define M_VOICE_IN_NOPULL()                                   \
    do                                                        \
    {                                                         \
        I2C_ConfigGpio(M_VOICE_PORT, M_VOICE_PIN, GPIO_INPUT) \
    } while (0)
#define M_VOICE_IN_IPU()                                      \
    do                                                        \
    {                                                         \
        I2C_ConfigGpio(M_VOICE_PORT, M_VOICE_PIN, GPIO_INPUT) \
    } while (0)
#define M_VOICE_IN_IPD()                                      \
    do                                                        \
    {                                                         \
        I2C_ConfigGpio(M_VOICE_PORT, M_VOICE_PIN, GPIO_INPUT) \
    } while (0)
#define M_VOICE_OUT_0() I2C_WriteGpioData(M_VOICE_PORT, M_VOICE_PIN, Bit_RESET)
#define M_VOICE_OUT_1() I2C_WriteGpioData(M_VOICE_PORT, M_VOICE_PIN, Bit_SET)
#define M_VOICE_OUT_TOGGLE()
#define M_VOICE_READIN() I2C_ReadGpioData(M_VOICE_PORT, M_VOICE_PIN)
#define M_VOICE_DETECT_ENABLE()
#define M_VOICE_LPM()
#define M_VOICE_EXIT_HITOLO_ENABLE()
#define M_VOICE_EXIT_LOTOHI_ENABLE()
#define M_VOICE_EXIT_HITOLO_DISABLE()
#define M_VOICE_EXIT_LOTOHI_DISABLE()
#endif
	

/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_VOICE_GLOBALS
#define DRV_VOICE_EXT
#else
#define DRV_VOICE_EXT extern
#endif /*DRV_VOICE_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    DRV_VOICE_EXT void DRV_Voice_Init(void);
    DRV_VOICE_EXT void DRV_Voice_Play(uint8_t dat);
    DRV_VOICE_EXT void DRV_Voice_Bell(uint8_t dat);
	DRV_VOICE_EXT void DRV_Voice_EnterSleepConfig(void);
	DRV_VOICE_EXT void DRV_Voice_BellEnterSleepConfig(void);
	DRV_VOICE_EXT void DRV_Voice_VolumeSet(uint8_t dat);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_VOICE_MODULE_EN*/

#endif /*DRV_VOICE_MODULE_EN*/
#endif /*__DRV_VOICE_H__*/
/************************************Code End******************************************/
