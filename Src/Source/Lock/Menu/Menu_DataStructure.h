/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**
**备注:      
****************************************************************************************/
#ifndef   __MENU_DATASTRUCTURE_H__
#define   __MENU_DATASTRUCTURE_H__
#include "config.h"
#define MENU_DATASTRUCTURE_MODULE_EN (1) //模块使能
#if MENU_DATASTRUCTURE_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef MENU_DATASTRUCTURE_GLOBALS
#define MENU_DATASTRUCTURE_EXT
#else
#define MENU_DATASTRUCTURE_EXT extern
#endif     /*MENU_DATASTRUCTURE_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/

#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*MENU_DATASTRUCTURE_MODULE_EN*/

#endif    /*MENU_DATASTRUCTURE_MODULE_EN*/
#endif     /*__MENU_DATASTRUCTURE_H__*/
/************************************Code End******************************************/

//.c文件模版
#include "Menu_DataStructure.h"
#if  MENU_DATASTRUCTURE_MODULE_EN
#warning MENU_DATASTRUCTURE模块使能!
/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  


/*************************局部函数*****************************/  


/*************************全局函数****************************/  

#endif  /*MENU_DATASTRUCTURE_MODULE_EN*/
/************************************Code End******************************************/