/***************************************************************************************
**�ļ���:     HAL_SysTimer.c
**�汾:       V1.00
**˵��:       Ӳ�������
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
****************************************************************************************/
#include "HAL_SysTimer.h"
#include "DRV_SysTimer.h"
#if  HAL_SYS_TIMER_MODULE_EN
#warning HAL_SYS_TIMERģ��ʹ��!
/*******************************�궨��***************************/  

/*************************.C�����ṹ����***********************/  

/*************************˽��ȫ�ֱ���*************************/  

/*************************����ȫ�ֱ���*************************/  
TYPEs_sysTimerCtrl sysTimerCtrl=
{
    0,      //UINT32 tick;                            //ϵͳ�δ������	
};

/*************************�ֲ�����*****************************/  
/***************************************************************************************
**������:       HAL_SysTimeRun
**��������:     ϵͳ�δ�ʱ���ۻ�,��Ҫ�ŵ��ж�������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void HAL_SysTimeRun(void)
{
    sysTimerCtrl.tick++;
}

/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       HAL_SysTimerInit
**��������:     ����ϵͳʱ�ӳ�ʼ����������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_SysTimerInit(void)
{
    DRV_SysTimerInit();
//    DRV_SysTimerHandlerReg(HAL_SysTimeRun);     //ע�ᶨʱ���жϷ������
//	sysTimerCtrl.tick = 0; //���ϵͳ��ʱ��
}

/***************************************************************************************
**������:       HAL_SysTimerHandlerReg
**��������:     �ص�����ע��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
INT8S HAL_SysTimerHandlerReg(halSysTimerHandler_t pfun)
{
   return  DRV_SysTimerHandlerReg(pfun);
}
/***************************************************************************************
**������:       HAL_SysTimerClose
**��������:     �ر�ϵͳʱ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_SysTimerClose(void)
{
	DRV_SysTimerClose();
}
/***************************************************************************************
**������:       HAL_SysTimeDeal
**��������:     �δ�ʱ�Ӵ���
**�������:     --
**�������:     0,�δ�ʱ�䵽;-1,�δ�ʱ��δ����
**��ע:         
****************************************************************************************/
INT8S HAL_SysTimeDeal(void)
{
    if (0 != sysTimerCtrl.tick)
    {
        sysTimerCtrl.tick --;
        return 0;
    }
    else
    {
        return -1;
    }
}
/***************************************************************************************
**������:       HAL_SysTimer_ClearCnt
**��������:     ���������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_SysTimer_ClearCnt(void)
{
    sysTimerCtrl.tick = 0;
}    
#endif  /*HAL_SYS_TIMER_MODULE_EN*/
/************************************Code End******************************************/