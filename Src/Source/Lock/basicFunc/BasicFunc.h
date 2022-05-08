/***************************************************************************************
**文件名:     BasicFunc.h
**版本:       V1.00
**说明:       基础数据功能模块，全局可用
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件 
**备注:      
****************************************************************************************/
#ifndef   __BASIC_FUNC_H__
#define   __BASIC_FUNC_H__
#include "MyConfig.h"
#define BASIC_FUNC_MODULE_EN (1) //模块使能
#if BASIC_FUNC_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef BASIC_FUNC_GLOBALS
#define BASIC_FUNC_EXT
#else
#define BASIC_FUNC_EXT extern
#endif     /*BASIC_FUNC_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
BASIC_FUNC_EXT uint16_t BasicFunc_GetSumChk(uint8_t *dat, uint16_t len);

#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*BASIC_FUNC_MODULE_EN*/
#endif    /*BASIC_FUNC_MODULE_EN*/
#endif     /*__BASIC_FUNC_H__*/
/************************************Code End******************************************/

