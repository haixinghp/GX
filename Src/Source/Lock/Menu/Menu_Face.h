/***************************************************************************************
**文件名:     Menu_Face.h
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
#ifndef   __Menu_FACE_H__
#define   __Menu_FACE_H__
#include "MyConfig.h"
#define Menu_FACE_MODULE_EN (1) //模块使能
#if Menu_FACE_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef Menu_FACE_GLOBALS
#define Menu_FACE_EXT
#else
#define Menu_FACE_EXT extern
#endif     /*Menu_FACE_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/

#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*Menu_FACE_MODULE_EN*/

#endif    /*Menu_FACE_MODULE_EN*/
#endif     /*__Menu_FACE_H__*/
/************************************Code End******************************************/

