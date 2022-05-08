/***************************************************************************************
**�ļ���:     DRV_RTC.h
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
#ifndef   __DRV_RTC_H__
#define   __DRV_RTC_H__
#include "MyConfig.h"
#define DRV_RTC_MODULE_EN (1) //ģ��ʹ��
#if DRV_RTC_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  
typedef struct t_DateTime
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} _DateTime;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_RTC_GLOBALS
#define DRV_RTC_EXT
#else
#define DRV_RTC_EXT extern
#endif     /*DRV_RTC_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_RTC_EXT void DRV_RTC_Init(void);
DRV_RTC_EXT void DRV_RTC_SetByTick(uint32_t tick);
DRV_RTC_EXT _DateTime DRV_RTC_TimeGet(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_RTC_MODULE_EN*/

#endif    /*DRV_RTC_MODULE_EN*/
#endif     /*__DRV_RTC_H__*/
/************************************Code End******************************************/

