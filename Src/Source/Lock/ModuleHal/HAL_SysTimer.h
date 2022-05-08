/***************************************************************************************
**�ļ���:     HAL_SysTimer.h
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
#ifndef   __HAL_SYS_TIMER_H__
#define   __HAL_SYS_TIMER_H__
#include "MyConfig.h"
#define HAL_SYS_TIMER_MODULE_EN (1) //ģ��ʹ��
#if HAL_SYS_TIMER_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  
typedef  void (*halSysTimerHandler_t)(void);		//�ص�����
typedef struct
{
    volatile INT32U tick;                            //ϵͳ�δ������	
} TYPEs_sysTimerCtrl;


#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_SYS_TIMER_GLOBALS
#define HAL_SYS_TIMER_EXT
#else
#define HAL_SYS_TIMER_EXT extern
#endif     /*HAL_SYS_TIMER_GLOBALS*/

/*************************��������*************************/  
HAL_SYS_TIMER_EXT  TYPEs_sysTimerCtrl sysTimerCtrl; 
/*************************��������*************************/
HAL_SYS_TIMER_EXT void HAL_SysTimerInit(void);
HAL_SYS_TIMER_EXT INT8S HAL_SysTimeDeal(void);
HAL_SYS_TIMER_EXT void HAL_SysTimerClose(void);
HAL_SYS_TIMER_EXT void HAL_SysTimer_ClearCnt(void);
HAL_SYS_TIMER_EXT INT8S HAL_SysTimerHandlerReg(halSysTimerHandler_t pfun);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_SYS_TIMER_MODULE_EN*/

#endif    /*HAL_SYS_TIMER_MODULE_EN*/
#endif     /*__HAL_SYS_TIMER_H__*/
/************************************Code End******************************************/

