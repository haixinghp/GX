/***************************************************************************************
**文件名:     DRV_EEPROM.h
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
#ifndef __DRV_EEPROM_H__
#define __DRV_EEPROM_H__
#include "MyConfig.h"
#define DRV_EEPROM_MODULE_EN (1) //模块使能
#if DRV_EEPROM_MODULE_EN
/*************************专用宏定义*************************/
#define DRV_EEPROM_PAGE_SIZE (128)   //页大小
#define DRV_EEPROM_READ (1)          //读
#define DRV_EEPROM_WRITE (0)         //写
#define DRV_EEPROM_WRITE_ADDR (0XA6) //写地址
#define DRV_EEPROM_READ_ADDR (0XA7)  //写地址
#define DRV_EEPROM_WRITE_PROTECT (1) //写保护
/*************************变量结构声明*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_EEPROM_GLOBALS
#define DRV_EEPROM_EXT
#else
#define DRV_EEPROM_EXT extern
#endif /*DRV_EEPROM_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    DRV_EEPROM_EXT INT8S DRV_EEPROM_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_EEPROM_MODULE_EN*/

#endif /*DRV_EEPROM_MODULE_EN*/
#endif /*__DRV_EEPROM_H__*/
/************************************Code End******************************************/
