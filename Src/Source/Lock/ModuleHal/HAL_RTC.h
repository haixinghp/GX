/***************************************************************************************
**�ļ���:     HAL_RTC.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
stm = localtime((const time_t *)&tick);

****************************************************************************************/
#ifndef   __HAL_RTC_H__
#define   __HAL_RTC_H__
#include "MyConfig.h"
#include <time.h>
#define HAL_RTC_MODULE_EN (1) //ģ��ʹ��
#if HAL_RTC_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  
//typedef struct tm TYPEs_Stm;//��׼ʱ��,�����ڱ�׼���ļ�<time.h>��
//typedef struct
//{
//	struct tm stm; //��׼ʱ��,�����ڱ�׼���ļ�<time.h>��
//}TYPEs_RtcTime;
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_RTC_GLOBALS
#define HAL_RTC_EXT
#else
#define HAL_RTC_EXT extern
#endif     /*HAL_RTC_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
HAL_RTC_EXT void HAL_RTC_Init(void);
HAL_RTC_EXT void HAL_RTC_SetByTick(uint32_t tick);
HAL_RTC_EXT uint8_t HAL_RTC_TimeGetStm(struct tm *stmTmp);
HAL_RTC_EXT uint32_t HAL_RTC_Stm2Tick(struct tm tim);
HAL_RTC_EXT struct tm * HAL_RTC_Tick2Stm(uint32_t tick);
HAL_RTC_EXT uint32_t HAL_RTC_GetTick(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_RTC_MODULE_EN*/

#endif    /*HAL_RTC_MODULE_EN*/
#endif     /*__HAL_RTC_H__*/
/************************************Code End******************************************/

