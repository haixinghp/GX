/***************************************************************************************
**�ļ���:     DRV_Motor.c
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
#include "DRV_Motor.h"
#if  DRV_MOTOR_MODULE_EN
#warning DRV_MOTORģ��ʹ��!
/*******************************�궨��***************************/  

/*************************.C�����ṹ����***********************/  

/*************************˽��ȫ�ֱ���*************************/  

/*************************����ȫ�ֱ���*************************/  


/*************************�ֲ�����*****************************/  


/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       DRV_Motor_Stop
**��������:     �������ֹͣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Motor_Init(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
}
/***************************************************************************************
**������:       DRV_Motor_Stop
**��������:     �������ֹͣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Motor_Stop(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(0);
    MT_STOP_PROCESS();
}

/***************************************************************************************
**������:       DRV_Motor_Open
**��������:     ���������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Motor_Open(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(1);
    MT_OPEN_PROCESS();
}
/***************************************************************************************
**������:       DRV_Motor_Close
**��������:     ���������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Motor_Close(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(1);
    MT_CLOSE_PROCESS();
}

/***************************************************************************************
**������:       DRV_Motor_LockHandle
**��������:     ���ּ��
**�������:     --
**�������:     
INT8S
0 ״̬0
1 ״̬1
**��ע:         
****************************************************************************************/
INT8S DRV_Motor_LockHandle(void)
{
  //ע���ʼ��IO
    if (1 == M_BAC_IN_READ())
    {
      return 1;
    }
    else
    {
      return 0;
    }
}
/***************************************************************************************
**������:       DRV_Motor_Enable
**��������:     ���ʹ��
**�������:     
1 ���ʹ��
0 ���ʧ��
**�������:     --
**��ע:         
****************************************************************************************/
INT8S DRV_Motor_Enable(INT8U enb)
{
	M_M_EN_OUT_PP();
	if(enb)
	{
		M_M_EN_FUN_ON;
	}
	else
	{
		M_M_EN_FUN_OFF;
	}
	return 0;
}
#endif  /*DRV_MOTOR_MODULE_EN*/
/************************************Code End******************************************/