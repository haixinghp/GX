/***************************************************************************************
**文件名:     AppUser.h
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
#ifndef   __APP_USER_H__
#define   __APP_USER_H__
#include "MyConfig.h"
#define APP_USER_MODULE_EN (1) //模块使能
#if APP_USER_MODULE_EN
/*************************专用宏定义*************************/

/*************************变量结构声明*************************/

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef APP_USER_GLOBALS
#define APP_USER_EXT
#else
#define APP_USER_EXT extern
#endif     /*APP_USER_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    APP_USER_EXT void AppUser_Service(void);
    APP_USER_EXT void AppUser_Init(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*APP_USER_MODULE_EN*/

#endif    /*APP_USER_MODULE_EN*/
#endif     /*__APP_USER_H__*/
/************************************Code End******************************************/

