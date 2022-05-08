/***************************************************************************************
**文件名:     Function.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:
*版本:V1.00
*修改者:hhx
*时间:
*修改说明：
创建了这个文件
**备注:
****************************************************************************************/
#include "Function.h"
#include "menu.h"
#include "HAL_KeyLight.h"
#include "HAL_RTC.h"
#include "HAL_Storage.h"
#include "HAL_Power.h"
#if FUNCTION_MODULE_EN
#warning FUNCTION模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Function_KeyRandomCreate
**功能描述:     获取随机数
**输入参数:
TYPEe_WakeUpSource wakeUp, 唤醒源
int8_t *random, 得到的随机数
uint8_t randomTime
**输出参数:     --
**备注:
****************************************************************************************/
int8_t Function_KeyRandomCreate(TYPEe_WakeUpSource wakeUp, uint8_t *random, uint8_t randomTime)
{
    uint8_t temp_key[10] = {2, 2, 3, 4, 5, 6, 7, 8, 7, 8};
//    uint8_t tmp=0;
    *random = temp_key[randomTime % 10]; //取秒的余数
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

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Function_RTCTimerGet
**功能描述:     获取RTC
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
int8_t Function_RTCTimerGet(void)
{
    HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
    return 0;
}
/***************************************************************************************
**函数名:       Function_KeyRandomGet
**功能描述:     获取随机数
**输入参数:     --
**输出参数:     --
**备注:
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
**函数名:       Function_KeyRandomLight
**功能描述:     随机数灯光显示
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
int8_t Function_KeyRandomLight(void)
{
    HAL_KeyLightUniteData(st_InputInfo.keyRandom,2);//更新灯光
    return 0;
}
#if 0
/***************************************************************************************
**函数名:       Function_AppRegChk
**功能描述:     检查系统是否注册
**输入参数:     --
**输出参数:     --
**备注:
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
**函数名:       Function_SysRamInit
**功能描述:     设置系统ram
**输入参数:     --
**输出参数:     --
**备注:
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
    st_InputInfo.faceRun  = eFaceRunEnable; //使能人脸
    st_InputInfo.faceVerifyCnt = 0; //人脸验证次数清0
    st_InputInfo.faceVerifySuccessFlag = 0; //未验证通过
    st_InputInfo.motorRuningFlag = 0; //清除开门中标志
    st_InputInfo.backbatGetFlag = 0;    //未获取到后板电源
	st_InputInfo.lowPowerCannotWork = 0;
    DEBUG_LOG_OUT("fst_InputInfo.errorCnt.Face:%d\n",st_InputInfo.errorCnt.Face);
    return 0;
}
/***************************************************************************************
**函数名:       Function_SysRamInit
**功能描述:     设置系统ram
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
int8_t Function_SysRamBackup(void)
{
    st_InputInfo.insideOpenLockFlag = 0; //清除开门标志
    HAL_Power_RAMToHold((uint8_t *)(&st_InputInfo),sizeof(TYPEs_InputInfo));
    return 0;
}


#endif /*FUNCTION_MODULE_EN*/
/************************************Code End******************************************/