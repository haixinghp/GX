/***************************************************************************************
**文件名:     HAL_Power.h
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
#ifndef __HAL_POWER_H__
#define __HAL_POWER_H__
#include "MyConfig.h"
#include "DRV_Power.h"
#define HAL_POWER_MODULE_EN (1) //模块使能
#if HAL_POWER_MODULE_EN
/*************************专用宏定义*************************/

/*************************变量结构声明*************************/
typedef enum
{
    eWakeUpSourceNull = 0,      //没有唤醒源
	eWakeUpSourcePOR=1,           //上电复位
    eWakeUpSourceIr = 2,        //红外
    eWakeUpSourceFinger = 3,    //指纹
    eWakeUpSourceKey = 4,       //触摸按键
    eWakeUpSourceSlip = 5,      //滑盖
    eWakeUpSourceAlarm = 6,     //报警
    eWakeUpSourceBac = 7,       //把手
    eWakeUpSourceBle = 10,      //蓝牙
    eWakeUpSourceAutoLock = 11, //
    eWakeUpSourceDEF = 0X11,    //一键布防
    eWakeUpSourceSpeed = 0x22,  //加速度
    eWakeUpSourceWifi = 0x40,   //
    eWakeUpSourceClose = 0x80,  //一键关闭电子功能
    eWakeUpSourceClear = 0xaa,  //清空
    eWakeUpSourceNFC,           //NFC
    eWakeUpSourceFace,           //人脸
    eWakeUpSourceBack,          //后板
	eWakeUpSourceCatEye,     //猫眼	
	eWakeUpSourceTime,     //定时器	
    eWakeUpSourceAutoHandle,       //全自动把手
    eWakeUpSourceActiveDefense, //主动防御
} TYPEe_WakeUpSource;           //系统唤醒源


#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_POWER_GLOBALS
#define HAL_POWER_EXT
#else
#define HAL_POWER_EXT extern
#endif /*HAL_POWER_GLOBALS*/

    /*************************公共变量*************************/
    HAL_POWER_EXT TYPEe_WakeUpSource WakeUpSource;
    /*************************公共函数*************************/
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
