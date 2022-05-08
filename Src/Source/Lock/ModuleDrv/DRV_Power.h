/***************************************************************************************
**�ļ���:     DRV_Power.h
**�汾:       V1.00
**˵��:       io���ã�Ӳ�����е���Դ�������
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
****************************************************************************************/
#ifndef __DRV_POWER_H__
#define __DRV_POWER_H__
#include "MyConfig.h"
#define DRV_POWER_MODULE_EN (1) //ģ��ʹ��
#if DRV_POWER_MODULE_EN
/*************************ר�ú궨��*************************/
#define DRV_POWER_FILTER_TIME (5)
#define DRV_POWER_FILTER_OVET_TIME (100)

/*************************�����ṹ����*************************/
typedef void (*FunInPinHandler_t)(void); //�жϻص����
typedef void (*ReuseIntHandler_t)(void);
typedef enum
{
  eReuseIntNull = 0, //û�л���Դ
  eReuseIntFinger,   //ָ��
  eReuseIntNFC,      //NFC
  eReuseIntIr,     	//����
  eReuseIntBack,     //���
  eReuseIntCatEye,     //è��	
  eReuseIntMAX,      //����ռ�
} TYPEe_ReuseInt;    //���û���Դ
typedef enum
{
  eSysWakeupNull = 0, //û�л���Դ
  eSysWakeupFinger,   //ָ��
  eSysWakeupNFC,      //NFC
  eSysWakeupIr,       //����
  eSysWakeupBack,     //���
  eSysWakeupCatEye,     //è��	
  eSysWakeupPOR,      //�ϵ縴λ
  eSysWakeupTSI,      //��������
  eSysWakeupAlarm,    //����
  eSysWakeupTime,    //��ʱ������
  eSysWakeupActiveDefense, //��������  
  eSysWakeupMAX,      //����ռ�
} TYPEe_SysWakeup;    //���û���Դ

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_POWER_GLOBALS
#define DRV_POWER_EXT
#else
#define DRV_POWER_EXT extern
#endif /*DRV_POWER_GLOBALS*/

  /*************************��������*************************/

  /*************************��������*************************/
  DRV_POWER_EXT int8_t DRV_Power_TouchExitInPinHandlerReg(FunInPinHandler_t fun);
  DRV_POWER_EXT void DRV_Power_GpioInterrputInit(void);
  DRV_POWER_EXT void DRV_Power_EnterSleep(void);
  DRV_POWER_EXT void DRV_Power_GpioSetSleep(void);
  DRV_POWER_EXT void DRV_Power_ClosePeripheral(void);
  DRV_POWER_EXT void DRV_Power_ReuseInterrptService(EPORT_PINx pin);
  DRV_POWER_EXT void DRV_Power_ReuseInterruptConfig(void);
  DRV_POWER_EXT void DRV_Power_ReuseInterruptWakeupConfig(void);
  DRV_POWER_EXT void DRV_Power_EnterSleep(void);
  DRV_POWER_EXT void DRV_Power_ReservedSleepConfig(void);
  DRV_POWER_EXT void DRV_Power_CPUInit(void);
  DRV_POWER_EXT TYPEe_SysWakeup DRV_Power_SysWakeupChk(void);
  DRV_POWER_EXT int8_t DRV_Power_RAMToHold(uint8_t *dat,uint32_t len);
  DRV_POWER_EXT int8_t DRV_Power_HoldToRAM(uint8_t *dat,uint32_t len);
  DRV_POWER_EXT void DRV_Power_ReservedWakeupConfig(void);
  DRV_POWER_EXT void DRV_Power_WakeupIoConfig(void);
  DRV_POWER_EXT void DRV_Power_CPURst(void);
  DRV_POWER_EXT void DRV_Power_TrngInit(void);
  DRV_POWER_EXT uint32_t DRV_Power_GetRandomWord(void);
  DRV_POWER_EXT void DRV_BuFang_Time_Init(uint8_t time, uint32_t *systickCounter);
  DRV_POWER_EXT uint8_t DRV_BuFang_Time_Detet(uint8_t time,uint32_t *systickCounter);
  DRV_POWER_EXT void DRV_BuFang_Time_Sleep(uint8_t time,uint32_t *systickCounter);
  DRV_POWER_EXT void DRV_ReTry_Time_Set(uint8_t time);
  DRV_POWER_EXT uint8_t DRV_Bufang_Clear(void);
  DRV_POWER_EXT void DRV_InitWDT(uint16_t t);
  DRV_POWER_EXT void DRV_WDTFeedDog(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_POWER_MODULE_EN*/

#endif /*DRV_POWER_MODULE_EN*/
#endif /*__DRV_POWER_H__*/
/************************************Code End******************************************/
