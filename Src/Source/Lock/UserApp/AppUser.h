/***************************************************************************************
**�ļ���:     AppUser.h
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
#ifndef   __APP_USER_H__
#define   __APP_USER_H__
#include "MyConfig.h"
#define APP_USER_MODULE_EN (1) //ģ��ʹ��
#if APP_USER_MODULE_EN
/*************************ר�ú궨��*************************/

/*************************�����ṹ����*************************/

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef APP_USER_GLOBALS
#define APP_USER_EXT
#else
#define APP_USER_EXT extern
#endif     /*APP_USER_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
    APP_USER_EXT void AppUser_Service(void);
    APP_USER_EXT void AppUser_Init(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*APP_USER_MODULE_EN*/

#endif    /*APP_USER_MODULE_EN*/
#endif     /*__APP_USER_H__*/
/************************************Code End******************************************/

