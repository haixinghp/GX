/***************************************************************************************
**�ļ���:     HAL_Motor.c
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
#include "HAL_Motor.h"
#include "DRV_Motor.h"
#if HAL_MOTOR_MODULE_EN
#warning HAL_MOTORģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_MOTOR_OPEN_TIME (200)           //����ʱ��
#define HAL_MOTOR_OPEN_WAIT_TIME (10)       //���Ź���ͣ��ʱ��
#define HAL_MOTOR_OPEN_OK_TIME (6000)       //������ɵȴ�ʱ��
#define HAL_MOTOR_CLOSE_TIME (200)          //����ʱ��
#define HAL_MOTOR_CLOSE_WAIT_TIME (10)      //����ʱ��
#define HAL_MOTOR_LOCK_HANDLE_CHK_TIME (10) //����״̬������
/*************************.C�����ṹ����***********************/
typedef struct
{
    INT32U timeLimit;            //����ʱ��
    INT32U timeCnt;              //ʱ�������
    INT32U lockHandleCnt;        //���ֲ���������
    INT8U passageLockFlag;       //ͨ������
    TYPEe_MotorRunStatus status; //����״̬
} TYPEs_MotorRun;
/*************************˽��ȫ�ֱ���*************************/
static TYPEs_MotorRun MotorRun; //δ��ʼ��!
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_Motor_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Motor_Init(void)
{
    MotorRun.timeCnt = 0;
    MotorRun.lockHandleCnt = 0;
    MotorRun.status = eMotorRunStatusNone;
    DRV_Motor_Init();
}
/***************************************************************************************
**������:       HAL_Motor_OpenClose
**��������:     ����������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Motor_OpenClose(void)
{
    MotorRun.status = eMotorRunStatusOpen; //��������״̬��
    MotorRun.passageLockFlag = 0;
}
/***************************************************************************************
**������:       HAL_Motor_Open
**��������:     �������Զ�����
**�������:     --
**�������:     --
**��ע:         Ԥ����ͨ����
****************************************************************************************/
void HAL_Motor_Open(void)
{
    MotorRun.status = eMotorRunStatusOpen; //��������״̬��
    MotorRun.passageLockFlag = 1;
}
/***************************************************************************************
**������:       HAL_Motor_Close
**��������:     ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Motor_Close(void)
{
    MotorRun.status = eMotorRunStatusClose; //��������״̬��
    MotorRun.passageLockFlag = 0;
}
/***************************************************************************************
**������:       HAL_Motor_Deal
**��������:     ������д���
**�������:     --
**�������:     
INT8S
0 ������
1; //���ųɹ�
2; //���ź���ųɹ�
3; //ͨ�������ųɹ�
**��ע:         
����1ms����
****************************************************************************************/
INT8S HAL_Motor_Deal(void)
{
    INT8S reTmp = 0;
    if (MotorRun.timeCnt > 0) //��������1
    {
        MotorRun.timeCnt--;
    }
    switch (MotorRun.status)
    {
    case eMotorRunStatusNone: //�޶���
        break;
    case eMotorRunStatusOpen: //����
        DRV_Motor_Open();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_TIME;
        MotorRun.status = eMotorRunStatusOpenWait;
        break;
    case eMotorRunStatusOpenWait: //������
        if (MotorRun.timeCnt > 0) //����ʱ��δ��
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_WAIT_TIME;
        MotorRun.status = eMotorRunStatusOpen1;
        break;
    case eMotorRunStatusOpen1: //�ٴο���
        DRV_Motor_Open();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_TIME;
        MotorRun.status = eMotorRunStatusOpen1Wait;
        break;
    case eMotorRunStatusOpen1Wait: //�ٴο�����
        if (MotorRun.timeCnt > 0)  //����ʱ��δ��
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusOpenOk;
        break;
    case eMotorRunStatusOpenOk: //���ųɹ�
        MotorRun.timeCnt = HAL_MOTOR_OPEN_OK_TIME;
        if (1 == MotorRun.passageLockFlag)
        {
            MotorRun.status = eMotorRunStatusNone;
            reTmp = 3; //ͨ�������ųɹ�
        }
        else
        {
            MotorRun.status = eMotorRunStatusOpenOkWait;
            reTmp = 1; //���ųɹ�
        }
        break;
    case eMotorRunStatusOpenOkWait:      //���ųɹ��ȴ�
        if (1 == DRV_Motor_LockHandle()) //���������״̬
        {
            MotorRun.lockHandleCnt++;
            if (MotorRun.lockHandleCnt >= HAL_MOTOR_LOCK_HANDLE_CHK_TIME)
            {
                MotorRun.timeCnt = 0; //���ڵȴ���ֱ����Ϊ�ͻ��Ѿ�����
            }
        }
        else
        {
            MotorRun.lockHandleCnt = 0; //���ּ�����������
        }
        if (MotorRun.timeCnt > 0) //����ʱ��δ��
        {
            break;
        }
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusClose;
        break;
    case eMotorRunStatusClose: //����
        DRV_Motor_Close();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_TIME;
        MotorRun.status = eMotorRunStatusCloseWait;
        break;
    case eMotorRunStatusCloseWait: //������
        if (MotorRun.timeCnt > 0)  //����ʱ��δ��
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_WAIT_TIME;
        MotorRun.status = eMotorRunStatusClose1;
        break;
    case eMotorRunStatusClose1: //�ٴι���
        DRV_Motor_Close();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_TIME;
        MotorRun.status = eMotorRunStatusClose1Wait;
        break;
    case eMotorRunStatusClose1Wait: //�ٴι�����
        if (MotorRun.timeCnt > 0)   //����ʱ��δ��
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusCloseOK;
        break;
    case eMotorRunStatusCloseOK:               //�������
        reTmp = 2;                             //���ź���ųɹ�
        MotorRun.status = eMotorRunStatusNone; //�������
        break;
    default:
        break;
    }
    return reTmp;
}
#endif /*HAL_MOTOR_MODULE_EN*/
       /************************************Code End******************************************/
