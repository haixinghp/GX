/***************************************************************************************
**文件名:     DRV_Power.h
**版本:       V1.00
**说明:       io配置，硬件共有的资源都在这里。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件 
**备注:      
****************************************************************************************/
#ifndef __DRV_POWER_H__
#define __DRV_POWER_H__
#include "MyConfig.h"
#define DRV_POWER_MODULE_EN (1) //模块使能
#if DRV_POWER_MODULE_EN
/*************************专用宏定义*************************/
#define DRV_POWER_FILTER_TIME (5)
#define DRV_POWER_FILTER_OVET_TIME (100)

/*************************变量结构声明*************************/
typedef void (*FunInPinHandler_t)(void); //中断回调句柄
typedef void (*ReuseIntHandler_t)(void);
typedef enum
{
  eReuseIntNull = 0, //没有唤醒源
  eReuseIntFinger,   //指纹
  eReuseIntNFC,      //NFC
  eReuseIntIr,     	//红外
  eReuseIntBack,     //后板
  eReuseIntCatEye,     //猫眼	
  eReuseIntMAX,      //申请空间
} TYPEe_ReuseInt;    //复用唤醒源
typedef enum
{
  eSysWakeupNull = 0, //没有唤醒源
  eSysWakeupFinger,   //指纹
  eSysWakeupNFC,      //NFC
  eSysWakeupIr,       //红外
  eSysWakeupBack,     //后板
  eSysWakeupCatEye,     //猫眼	
  eSysWakeupPOR,      //上电复位
  eSysWakeupTSI,      //触摸按键
  eSysWakeupAlarm,    //报警
  eSysWakeupTime,    //定时器唤醒
  eSysWakeupActiveDefense, //主动防御  
  eSysWakeupMAX,      //申请空间
} TYPEe_SysWakeup;    //复用唤醒源

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_POWER_GLOBALS
#define DRV_POWER_EXT
#else
#define DRV_POWER_EXT extern
#endif /*DRV_POWER_GLOBALS*/

  /*************************公共变量*************************/

  /*************************公共函数*************************/
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
