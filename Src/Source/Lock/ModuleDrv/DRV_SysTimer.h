/***************************************************************************************
**�ļ���:     DRV_SysTimer.h
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
#ifndef   __DRV_SYS_TIMER_H__
#define   __DRV_SYS_TIMER_H__
#include "MyConfig.h"
#define DRV_SYS_TIMER_MODULE_EN (1) //ģ��ʹ��
#if DRV_SYS_TIMER_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  
typedef  void (*sysTimerHandler_t)(void);		//�ص�����
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_SYS_TIMER_GLOBALS
#define DRV_SYS_TIMER_EXT
#else
#define DRV_SYS_TIMER_EXT extern
#endif     /*DRV_SYS_TIMER_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_SYS_TIMER_EXT INT8S DRV_SysTimerHandlerReg(sysTimerHandler_t pfun);
DRV_SYS_TIMER_EXT void DRV_SysTimerInit(void);
DRV_SYS_TIMER_EXT void DRV_SysTimerClose(void);
DRV_SYS_TIMER_EXT void DRV_SysTimerHandler(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_SYS_TIMER_MODULE_EN*/

#endif    /*DRV_SYS_TIMER_MODULE_EN*/
#endif     /*__DRV_SYS_TIMER_H__*/
/************************************Code End******************************************/

