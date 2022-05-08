/***************************************************************************************
**�ļ���:     HAL_Power.h
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
#ifndef __HAL_POWER_H__
#define __HAL_POWER_H__
#include "MyConfig.h"
#include "DRV_Power.h"
#define HAL_POWER_MODULE_EN (1) //ģ��ʹ��
#if HAL_POWER_MODULE_EN
/*************************ר�ú궨��*************************/

/*************************�����ṹ����*************************/
typedef enum
{
    eWakeUpSourceNull = 0,      //û�л���Դ
	eWakeUpSourcePOR=1,           //�ϵ縴λ
    eWakeUpSourceIr = 2,        //����
    eWakeUpSourceFinger = 3,    //ָ��
    eWakeUpSourceKey = 4,       //��������
    eWakeUpSourceSlip = 5,      //����
    eWakeUpSourceAlarm = 6,     //����
    eWakeUpSourceBac = 7,       //����
    eWakeUpSourceBle = 10,      //����
    eWakeUpSourceAutoLock = 11, //
    eWakeUpSourceDEF = 0X11,    //һ������
    eWakeUpSourceSpeed = 0x22,  //���ٶ�
    eWakeUpSourceWifi = 0x40,   //
    eWakeUpSourceClose = 0x80,  //һ���رյ��ӹ���
    eWakeUpSourceClear = 0xaa,  //���
    eWakeUpSourceNFC,           //NFC
    eWakeUpSourceFace,           //����
    eWakeUpSourceBack,          //���
	eWakeUpSourceCatEye,     //è��	
	eWakeUpSourceTime,     //��ʱ��	
    eWakeUpSourceAutoHandle,       //ȫ�Զ�����
    eWakeUpSourceActiveDefense, //��������
} TYPEe_WakeUpSource;           //ϵͳ����Դ


#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_POWER_GLOBALS
#define HAL_POWER_EXT
#else
#define HAL_POWER_EXT extern
#endif /*HAL_POWER_GLOBALS*/

    /*************************��������*************************/
    HAL_POWER_EXT TYPEe_WakeUpSource WakeUpSource;
    /*************************��������*************************/
    HAL_POWER_EXT int8_t HAL_Power_TouchExitInPinHandlerReg(FunInPinHandler_t fun);
    HAL_POWER_EXT void HAL_Power_GpioInterrputInit(void);
    HAL_POWER_EXT void HAL_Power_EnterSleep(void);
    HAL_POWER_EXT void HAL_Power_ClosePeripheral(void);
    HAL_POWER_EXT void HAL_Power_ReuseInterruptConfig(void);
	HAL_POWER_EXT void HAL_Power_ReuseInterruptWakeupConfig(void);
    HAL_POWER_EXT void HAL_Power_ReservedSleepConfig(void);
    HAL_POWER_EXT void HAL_Power_CPUInit(void);
    HAL_POWER_EXT int8_t HAL_Power_SysWakeupChk(void);
    HAL_POWER_EXT int8_t HAL_Power_RAMToHold(uint8_t *dat,uint32_t len);
    HAL_POWER_EXT int8_t HAL_Power_HoldToRAM(uint8_t *dat,uint32_t len);
	HAL_POWER_EXT void HAL_Power_ReservedWakeupConfig(void);
	HAL_POWER_EXT void HAL_Power_WakeupIoConfig(void);
	HAL_POWER_EXT void HAL_Power_CPURst(void);
	HAL_POWER_EXT void HAL_Power_TrngInit(void);
	HAL_POWER_EXT uint32_t HAL_Power_GetRandomWord(void);
    
    HAL_POWER_EXT void HAL_BuFang_Time_Init(uint8_t time,uint32_t *systickCounter);
    HAL_POWER_EXT uint8_t HAL_BuFang_Time_Detet(uint8_t time,uint32_t *systickCounter);
    HAL_POWER_EXT void HAL_BuFang_Time_Sleep(uint8_t time,uint32_t *systickCounter);
    HAL_POWER_EXT void HAL_ReTry_Time_Set(uint8_t time);
    HAL_POWER_EXT uint8_t HAL_Bufang_Clear(void);
	HAL_POWER_EXT void HAL_InitWDT(uint16_t t);
	HAL_POWER_EXT void HAL_WDTFeedDog(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_POWER_MODULE_EN*/

#endif /*HAL_POWER_MODULE_EN*/
#endif /*__HAL_POWER_H__*/
/************************************Code End******************************************/
