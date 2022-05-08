/***************************************************************************************
**文件名:     DRV_MBI5024.h
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
#ifndef __DRV_MBI5024_H__
#define __DRV_MBI5024_H__
#include "MyConfig.h"
#define DRV_MBI5024_MODULE_EN (1) //模块使能
#if DRV_MBI5024_MODULE_EN
/*************************专用宏定义*************************/

#define DRV_MBI5024_LED_0 0x0040
#define DRV_MBI5024_LED_1 0x0100
#define DRV_MBI5024_LED_2 0x0010
#define DRV_MBI5024_LED_3 0x0001
#define DRV_MBI5024_LED_4 0x0800
#define DRV_MBI5024_LED_5 0x0020
#define DRV_MBI5024_LED_6 0x0002
#define DRV_MBI5024_LED_7 0x0400
#define DRV_MBI5024_LED_8 0x0080
#define DRV_MBI5024_LED_9 0x0004
#define DRV_MBI5024_LED_E 0x0008
#define DRV_MBI5024_LED_M 0x0200
#define DRV_MBI5024_LED_R 0x1000
#define DRV_MBI5024_LED_G 0x2000
#define DRV_MBI5024_LED_VBR 0x4000
#define DRV_MBI5024_LED_VBG 0x8000
/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_MBI5024_GLOBALS
#define DRV_MBI5024_EXT
#else
#define DRV_MBI5024_EXT extern
#endif /*DRV_MBI5024_GLOBALS*/

    /*************************公共变量*************************/
    
    /*************************公共函数*************************/
	DRV_MBI5024_EXT void DRV_MBI5024_Init(void);
    DRV_MBI5024_EXT void DRV_MBI5024_Out(u16 dat, u8 enable);
    DRV_MBI5024_EXT void DRV_MBI5024_LEDClose(void);
    DRV_MBI5024_EXT void DRV_MBI5024_LEDOpen(void);
    DRV_MBI5024_EXT void DRV_MBI5024_EnterSleepConfig(void);
    DRV_MBI5024_EXT uint8_t DRV_MBI5024_SysStatusSet(uint8_t dat);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_MBI5024_MODULE_EN*/

#endif /*DRV_MBI5024_MODULE_EN*/
#endif /*__DRV_MBI5024_H__*/
/************************************Code End******************************************/
