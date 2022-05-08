/***************************************************************************************
**�ļ���:     Function.c
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
#include "Function.h"
#include "menu.h"
#include "HAL_KeyLight.h"
#include "HAL_RTC.h"
#include "HAL_Storage.h"
#include "HAL_Power.h"
#if FUNCTION_MODULE_EN
#warning FUNCTIONģ��ʹ��!
/*******************************�궨��***************************/

/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       Function_KeyRandomCreate
**��������:     ��ȡ�����
**�������:
TYPEe_WakeUpSource wakeUp, ����Դ
int8_t *random, �õ��������
uint8_t randomTime
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_KeyRandomCreate(TYPEe_WakeUpSource wakeUp, uint8_t *random, uint8_t randomTime)
{
    uint8_t temp_key[10] = {2, 2, 3, 4, 5, 6, 7, 8, 7, 8};
//    uint8_t tmp=0;
    *random = temp_key[randomTime % 10]; //ȡ�������
    // tmp = *random;
    if (eWakeUpSourceNull == wakeUp)
    {
        *(random + 1) = 0;
    }
    else
    {
        if ((*random + (uint8_t)(wakeUp)) <= 9)
        {
            *(random + 1) = (*random) + (uint8_t)(wakeUp);
        }
        else if ((uint8_t)(wakeUp) < 9)
        {
            *random = *random % (9 - (uint8_t)(wakeUp));
            if(0 == *random)
            {
                *random = 1;
            }
            *(random + 1) = *random + (uint8_t)(wakeUp);
        }
        else
        {
            *(random + 1) = 0;
        }
    }
    return 0;
}

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       Function_RTCTimerGet
**��������:     ��ȡRTC
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_RTCTimerGet(void)
{
    HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
    return 0;
}
/***************************************************************************************
**������:       Function_KeyRandomGet
**��������:     ��ȡ�����
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_KeyRandomGet(void)
{

    Function_KeyRandomCreate(WakeUpSource,
                             st_InputInfo.keyRandom,
                             st_InputInfo.RTCTimer.tm_sec);
//	st_InputInfo.keyRandomLockFlag = eKeyRandomLockFlagEnable;
    return 0;
}
/***************************************************************************************
**������:       Function_KeyRandomLight
**��������:     ������ƹ���ʾ
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_KeyRandomLight(void)
{
    HAL_KeyLightUniteData(st_InputInfo.keyRandom,2);//���µƹ�
    return 0;
}
#if 0
/***************************************************************************************
**������:       Function_AppRegChk
**��������:     ���ϵͳ�Ƿ�ע��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_AppRegChk(void)
{
    if(storageSave.sysDat.allPhone == storageSave.sysDat.leavePhone)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
#endif
/***************************************************************************************
**������:       Function_SysRamInit
**��������:     ����ϵͳram
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_SysRamInit(void)
{
    if (eWakeUpSourcePOR ==  WakeUpSource)
    {
        memset((uint8_t *)(&st_InputInfo),0,sizeof(TYPEs_InputInfo));
        HAL_Power_RAMToHold((uint8_t *)(&st_InputInfo),sizeof(TYPEs_InputInfo));
    }
    else
    {
        DRV_Power_HoldToRAM((uint8_t *)(&st_InputInfo),sizeof(TYPEs_InputInfo));
    }
    st_InputInfo.faceRun  = eFaceRunEnable; //ʹ������
    st_InputInfo.faceVerifyCnt = 0; //������֤������0
    st_InputInfo.faceVerifySuccessFlag = 0; //δ��֤ͨ��
    st_InputInfo.motorRuningFlag = 0; //��������б�־
    st_InputInfo.backbatGetFlag = 0;    //δ��ȡ������Դ
	st_InputInfo.lowPowerCannotWork = 0;
    DEBUG_LOG_OUT("fst_InputInfo.errorCnt.Face:%d\n",st_InputInfo.errorCnt.Face);
    return 0;
}
/***************************************************************************************
**������:       Function_SysRamInit
**��������:     ����ϵͳram
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t Function_SysRamBackup(void)
{
    st_InputInfo.insideOpenLockFlag = 0; //������ű�־
    HAL_Power_RAMToHold((uint8_t *)(&st_InputInfo),sizeof(TYPEs_InputInfo));
    return 0;
}


#endif /*FUNCTION_MODULE_EN*/
/************************************Code End******************************************/