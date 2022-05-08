/***************************************************************************************
**�ļ���:     APP_Power.h
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
#ifndef __APP_POWER_H__
#define __APP_POWER_H__
#include "MyConfig.h"
#define APP_POWER_MODULE_EN (1) //ģ��ʹ��
#if APP_POWER_MODULE_EN
/*************************ר�ú궨��*************************/
#define DC_SMS_MODE_MAX_V (2121)            //SMSģ���ѹ����
#define DC_STATUS_LCD (0X0001)              //LCD��Դ
#define DC_STATUS_DSP (0x0002)              //DSP��Դ
#define DC_STATUS_GSM (0x0004)              //GSM��Դ
#define DC_STATUS_KEYBLACKLIGHT (0X0008)    //���̱���
#define DC_STATUS_MOTOR (0X0010)            //�����Դ
#define DC_STATUS_UNTRUCH_SLEEP (0X1000)    //������
#define DC_WIFI_V_LIMT (690)                //wifi���͵�ѹ
#define DC_STATUS_LOW (690)                 //�͵�ѹ
#define DC_STATUS_LOW_SYSTEM_NOT_WORK (650) //�͵�ѹ���޷�����
/************************
 * ϵͳ����Դ
 * *********************/
#define SYS_WAKEUP_SOURE_NULL (0)     //û�л���Դ
#define SYS_WAKEUP_SOURE_CLEAR (0xAA) // ���
#define SYS_WAKEUP_SOURE_BAC (7)      // ����
#define SYS_WAKEUP_SOURE_TOUCH (3)    //ָ��
#define SYS_WAKEUP_SOURE_ALARM (6)    //����
#define SYS_WAKEUP_SOURE_IR (2)       //����
#define SYS_WAKEUP_SOURE_KEY_IRQ (4)  //������
#define SYS_WAKEUP_SOURE_SLIP (5)     //����
#define SYS_WAKEUP_SOURE_BLE (10)     //
#define SYS_WAKEUP_SOURE_AUTOLOCK (11)
#define SYS_WAKEUP_SOURE_WIFI (0X40)  //����
#define SYS_WAKEUP_SOURE_CLOSE (0X80) //һ���رյ��ӹ���
#define SYS_WAKEUP_SOURE_DEF (0X11)   //һ������
#define SYS_WAKEUP_SOURE_SPEED (0X22) // ���ٶ�

#define WAKE_SRC_TYPE_DISABLE (0)
#define WAKE_SRC_TYPE_ENABLE (1)
#define WAKE_SRC_TYPE_ING (2)

/*************************�����ṹ����*************************/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef APP_POWER_GLOBALS
#define APP_POWER_EXT
#else
#define APP_POWER_EXT extern
#endif /*APP_POWER_GLOBALS*/

    /*************************��������*************************/

    /*************************��������*************************/
    APP_POWER_EXT void APP_Power_Init(void);
    APP_POWER_EXT void APP_Power_WakeupInit(void);
    APP_POWER_EXT void APP_Power_EnterSleepConfig(void);
    APP_POWER_EXT void APP_Power_EnterSleep(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*APP_POWER_MODULE_EN*/

#endif /*APP_POWER_MODULE_EN*/
#endif /*__APP_POWER_H__*/
/************************************Code End******************************************/
