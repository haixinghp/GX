/***************************************************************************************
**文件名:     Function.h
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
#ifndef   __FUNCTION_H__
#define   __FUNCTION_H__
#include "MyConfig.h"
#include "menu.h"
#define FUNCTION_MODULE_EN (1) //模块使能
#if FUNCTION_MODULE_EN
/*************************专用宏定义*************************/

/*************************变量结构声明*************************/

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef FUNCTION_GLOBALS
#define FUNCTION_EXT
#else
#define FUNCTION_EXT extern
#endif     /*FUNCTION_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    FUNCTION_EXT int8_t Function_RTCTimerGet(void);
    FUNCTION_EXT int8_t Function_KeyRandomGet(void);
    FUNCTION_EXT int8_t Function_KeyRandomLight(void);
    FUNCTION_EXT int8_t Function_AppRegChk(void);
    FUNCTION_EXT int8_t Function_SysRamInit(void);
    FUNCTION_EXT int8_t Function_SysRamBackup(void);
    FUNCTION_EXT int8_t Function_SysStorageInit(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*FUNCTION_MODULE_EN*/

#endif    /*FUNCTION_MODULE_EN*/
#endif     /*__FUNCTION_H__*/
/************************************Code End******************************************/

