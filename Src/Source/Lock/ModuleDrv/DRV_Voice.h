/***************************************************************************************
**�ļ���:     DRV_Voice.h
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
#ifndef __DRV_VOICE_H__
#define __DRV_VOICE_H__
#include "MyConfig.h"
#include "i2c_drv.h"
#define DRV_VOICE_MODULE_EN (1) //ģ��ʹ��
#if DRV_VOICE_MODULE_EN
/*************************ר�ú궨��*************************/
/***************************************************************************/ /**
* @brief
*   VOICE�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
	

/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_VOICE_GLOBALS
#define DRV_VOICE_EXT
#else
#define DRV_VOICE_EXT extern
#endif /*DRV_VOICE_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
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
