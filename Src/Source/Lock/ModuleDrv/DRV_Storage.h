/***************************************************************************************
**文件名:     DRV_Storage.h
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
#ifndef   __DRV_STORAGE_H__
#define   __DRV_STORAGE_H__
#include "MyConfig.h"
#define DRV_STORAGE_MODULE_EN (1) //模块使能
#if DRV_STORAGE_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_STORAGE_GLOBALS
#define DRV_STORAGE_EXT
#else
#define DRV_STORAGE_EXT extern
#endif     /*DRV_STORAGE_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
DRV_STORAGE_EXT void DRV_Storage_Init(void);
DRV_STORAGE_EXT INT8S DRV_Storage_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length);
DRV_STORAGE_EXT void DRV_Storage_EnterSleepConfig(void);
DRV_STORAGE_EXT INT8S DRV_Storage_EraseSector(uint32_t Addr);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_STORAGE_MODULE_EN*/

#endif    /*DRV_STORAGE_MODULE_EN*/
#endif     /*__DRV_STORAGE_H__*/
/************************************Code End******************************************/

