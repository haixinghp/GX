/***************************************************************************************
**�ļ���:     Function.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:
*�汾:V1.00
*�޸���:hhx
*ʱ��:
*�޸�˵����
����������ļ�
**��ע:
****************************************************************************************/
#ifndef   __FUNCTION_H__
#define   __FUNCTION_H__
#include "MyConfig.h"
#include "menu.h"
#define FUNCTION_MODULE_EN (1) //ģ��ʹ��
#if FUNCTION_MODULE_EN
/*************************ר�ú궨��*************************/

/*************************�����ṹ����*************************/

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef FUNCTION_GLOBALS
#define FUNCTION_EXT
#else
#define FUNCTION_EXT extern
#endif     /*FUNCTION_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
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

