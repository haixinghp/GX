/***************************************************************************************
**文件名:     APP_Power.h
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
#ifndef __APP_POWER_H__
#define __APP_POWER_H__
#include "MyConfig.h"
#define APP_POWER_MODULE_EN (1) //模块使能
#if APP_POWER_MODULE_EN
/*************************专用宏定义*************************/
#define DC_SMS_MODE_MAX_V (2121)            //SMS模块电压极限
#define DC_STATUS_LCD (0X0001)              //LCD电源
#define DC_STATUS_DSP (0x0002)              //DSP电源
#define DC_STATUS_GSM (0x0004)              //GSM电源
#define DC_STATUS_KEYBLACKLIGHT (0X0008)    //键盘背光
#define DC_STATUS_MOTOR (0X0010)            //电机电源
#define DC_STATUS_UNTRUCH_SLEEP (0X1000)    //假休眠
#define DC_WIFI_V_LIMT (690)                //wifi推送电压
#define DC_STATUS_LOW (690)                 //低电压
#define DC_STATUS_LOW_SYSTEM_NOT_WORK (650) //低电压，无法工作
/************************
 * 系统唤醒源
 * *********************/
#define SYS_WAKEUP_SOURE_NULL (0)     //没有唤醒源
#define SYS_WAKEUP_SOURE_CLEAR (0xAA) // 清空
#define SYS_WAKEUP_SOURE_BAC (7)      // 把手
#define SYS_WAKEUP_SOURE_TOUCH (3)    //指纹
#define SYS_WAKEUP_SOURE_ALARM (6)    //防翘
#define SYS_WAKEUP_SOURE_IR (2)       //红外
#define SYS_WAKEUP_SOURE_KEY_IRQ (4)  //触摸板
#define SYS_WAKEUP_SOURE_SLIP (5)     //滑盖
#define SYS_WAKEUP_SOURE_BLE (10)     //
#define SYS_WAKEUP_SOURE_AUTOLOCK (11)
#define SYS_WAKEUP_SOURE_WIFI (0X40)  //红外
#define SYS_WAKEUP_SOURE_CLOSE (0X80) //一键关闭电子功能
#define SYS_WAKEUP_SOURE_DEF (0X11)   //一键布防
#define SYS_WAKEUP_SOURE_SPEED (0X22) // 加速度

#define WAKE_SRC_TYPE_DISABLE (0)
#define WAKE_SRC_TYPE_ENABLE (1)
#define WAKE_SRC_TYPE_ING (2)

/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef APP_POWER_GLOBALS
#define APP_POWER_EXT
#else
#define APP_POWER_EXT extern
#endif /*APP_POWER_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    APP_POWER_EXT void APP_Power_Init(void);
    APP_POWER_EXT void APP_Power_WakeupInit(void);
    APP_POWER_EXT void APP_Power_EnterSleepConfig(void);
    APP_POWER_EXT void APP_Power_EnterSleep(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*APP_POWER_MODULE_EN*/

#endif /*APP_POWER_MODULE_EN*/
#endif /*__APP_POWER_H__*/
/************************************Code End******************************************/
