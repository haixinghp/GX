/***************************************************************************************
**文件名:     DRV_I2C.h
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
#ifndef   __DRV_I2C_H__
#define   __DRV_I2C_H__
#include "MyConfig.h"
#define DRV_I2C_MODULE_EN (1) //模块使能
#if DRV_I2C_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_I2C_GLOBALS
#define DRV_I2C_EXT
#else
#define DRV_I2C_EXT extern
#endif     /*DRV_I2C_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
DRV_I2C_EXT INT8U DRV_I2C_WriteNByte(INT8U i2cAddr,INT8U addr, INT8U *data,INT32U len);
DRV_I2C_EXT INT8U DRV_I2C_ReadNByte(INT8U i2cAddr,INT8U addr, INT8U *data,INT32U len);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_I2C_MODULE_EN*/
#endif    /*DRV_I2C_MODULE_EN*/
#endif     /*__DRV_I2C_H__*/
/************************************Code End******************************************/

