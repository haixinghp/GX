/***************************************************************************************
**�ļ���:     APP_Storage.h
**�汾:       V1.00
**˵��:       ���ļ���ϵͳ�Ĵ洢�ṹ
**�޸ļ�¼:
*�汾:V1.00
*�޸���:hhx
*ʱ��:
*�޸�˵����
����������ļ�
**��ע:
****************************************************************************************/
#ifndef __APP_STORAGE_H__
#define __APP_STORAGE_H__
#include "MyConfig.h"
#include "HAL_Storage.h"
#define APP_STORAGE_MODULE_EN (1) //ģ��ʹ��
#if APP_STORAGE_MODULE_EN
/*************************ר�ú궨��*************************/
#define APP_STORAGE_FINGER_GENERAL_START (MEM_FINGER_GENERAL_START_ID)
#define APP_STORAGE_FINGER_GUEST_ALL_LOCAL (MEM_FINGER_GUEST_ALL_LOCAL)
/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef APP_STORAGE_GLOBALS
#define APP_STORAGE_EXT
#else
#define APP_STORAGE_EXT extern
#endif /*APP_STORAGE_GLOBALS*/

    /*************************��������*************************/
    APP_STORAGE_EXT TYPEs_StorageSave storageSave;
    APP_STORAGE_EXT TYPEs_SystemDat systemDat;
    /*************************��������*************************/
    APP_STORAGE_EXT INT8S APP_Storage_ChkFingerNum(void);
    APP_STORAGE_EXT INT8S APP_Storage_ReadOneFingerReg(void);
    APP_STORAGE_EXT INT8S APP_Storage_SaveOneFingerReg(void);
    APP_STORAGE_EXT INT8S APP_Storage_DelOneFingerReg(void);
    APP_STORAGE_EXT INT8S APP_Storage_ResetFingerStruct(void);
    APP_STORAGE_EXT INT8S APP_Storage_lockRegStatusGet(void);
    APP_STORAGE_EXT INT8S APP_Storage_lockRegStatusSet(INT8U status);
    APP_STORAGE_EXT INT8S APP_Storage_AdminFingerNumChk(void);
    APP_STORAGE_EXT INT8S APP_Storage_GeneralFingerNumChk(void);
    APP_STORAGE_EXT INT8S APP_Storage_SaveLockRegStatus(void);
    APP_STORAGE_EXT INT8S APP_Storage_SaveAdminFingerNum(void);
    APP_STORAGE_EXT INT8S APP_Storage_SaveGeneralFingerNum(void);
    APP_STORAGE_EXT INT16U APP_Storage_SearchEmptyFingerLocalAdmin(void);
    APP_STORAGE_EXT INT16U APP_Storage_SearchEmptyFingerLocalGeneral(void);
    APP_STORAGE_EXT INT8S APP_Storage_SaveOneFingerRegLocal(void);
    APP_STORAGE_EXT INT8S APP_Storage_DelAllFingerLocalGeneral(void);
    APP_STORAGE_EXT INT8S APP_Storage_SystemDataInit(void);
    APP_STORAGE_EXT INT8S APP_Storage_SystemDataRead(void);
    APP_STORAGE_EXT INT8S APP_Storage_DelAllFingerLocal(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*APP_STORAGE_MODULE_EN*/

#endif /*APP_STORAGE_MODULE_EN*/
#endif /*__APP_STORAGE_H__*/
/************************************Code End******************************************/
