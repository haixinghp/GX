/***************************************************************************************
**�ļ���:     DRV_KeySacn.h
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
#ifndef __DRV_KEY_SACN_H__
#define __DRV_KEY_SACN_H__
#include "MyConfig.h"
#define DRV_KEY_SACN_MODULE_EN (1) //ģ��ʹ��
#if DRV_KEY_SACN_MODULE_EN
/*************************ר�ú궨��*************************/

#define DRV_KEY_SACN_GET_1 (1)
#define DRV_KEY_SACN_GET_2 (2)
#define DRV_KEY_SACN_GET_3 (3)
#define DRV_KEY_SACN_GET_4 (4)
#define DRV_KEY_SACN_GET_5 (5)
#define DRV_KEY_SACN_GET_6 (6)
#define DRV_KEY_SACN_GET_7 (7)
#define DRV_KEY_SACN_GET_8 (8)
#define DRV_KEY_SACN_GET_9 (9)
#define DRV_KEY_SACN_GET_0 (0)
#define DRV_KEY_SACN_GET_MENU (0x0a)
#define DRV_KEY_SACN_GET_ENTER (0x0b)
#define DRV_KEY_SACN_GET_CLOSE (0x0C)
#define DRV_KEY_SACN_GET_BELL (0x0D)
#define DRV_KEY_SACN_GET_NOKEY (0xFF)
/**********ԭʼ����*********/
#define DRV_KEY_SACN_ORIGINAL_0 (0)
#define DRV_KEY_SACN_ORIGINAL_1 (1)
#define DRV_KEY_SACN_ORIGINAL_2 (2)
#define DRV_KEY_SACN_ORIGINAL_3 (3)
#define DRV_KEY_SACN_ORIGINAL_4 (4)
#define DRV_KEY_SACN_ORIGINAL_5 (5)
#define DRV_KEY_SACN_ORIGINAL_6 (6)
#define DRV_KEY_SACN_ORIGINAL_7 (7)
#define DRV_KEY_SACN_ORIGINAL_8 (8)
#define DRV_KEY_SACN_ORIGINAL_9 (9)
#define DRV_KEY_SACN_ORIGINAL_10 (10)
#define DRV_KEY_SACN_ORIGINAL_11 (11)
#define DRV_KEY_SACN_ORIGINAL_12 (12)
#define DRV_KEY_SACN_ORIGINAL_13 (13)
#define DRV_KEY_SACN_ORIGINAL_14 (14)
#define DRV_KEY_SACN_ORIGINAL_15 (15)
/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_KEY_SACN_GLOBALS
#define DRV_KEY_SACN_EXT
#else
#define DRV_KEY_SACN_EXT extern
#endif /*DRV_KEY_SACN_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
    DRV_KEY_SACN_EXT void DRV_KeySacn_Init(void);
    DRV_KEY_SACN_EXT uint8_t DRV_KeySacn_GetValue(void);
    DRV_KEY_SACN_EXT void DRV_KeySacn_EnterSleepConfig(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_KEY_SACN_MODULE_EN*/

#endif /*DRV_KEY_SACN_MODULE_EN*/
#endif /*__DRV_KEY_SACN_H__*/
/************************************Code End******************************************/
