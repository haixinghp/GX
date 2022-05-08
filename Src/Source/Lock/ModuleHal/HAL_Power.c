/***************************************************************************************
**文件名:     HAL_Power.c
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
#include "HAL_Power.h"
#include "DRV_Power.h"
#if HAL_POWER_MODULE_EN
#warning HAL_POWER模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
TYPEe_WakeUpSource WakeUpSource = eWakeUpSourceNull;
/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_Power_TouchExitInPinHandlerReg
**功能描述:     注册触摸中断回调句柄
**输入参数:     
FunInPinHanler_t fun
要注册的函数
**输出参数:     
int8
0 成功
-1 失败
**备注:         
****************************************************************************************/
int8_t HAL_Power_TouchExitInPinHandlerReg(FunInPinHandler_t fun)
{
	//	return DRV_Power_TouchExitInPinHandlerReg(fun);
	return 0;
}
/***************************************************************************************
**函数名:       HAL_Power_GpioInterrputInit
**功能描述:     初始化系统中断
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_GpioInterrputInit(void)
{
	//	DRV_Power_GpioInterrputInit();
}

/***************************************************************************************
**函数名:       HAL_Power_EnterSleep
**功能描述:     进入系统休眠
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_EnterSleep(void)
{
	DRV_Power_EnterSleep();
}

/***************************************************************************************
**函数名:       HAL_Power_ClosePeripheral
**功能描述:     关闭外设
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_ClosePeripheral(void)
{
	//	DRV_Power_ClosePeripheral();
	//	DRV_Power_GpioSetSleep();
}
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
/*
void HAL_Power_Init(void)
{
    
}
*/

/***************************************************************************************
**函数名:       HAL_Power_ReuseInterruptConfig
**功能描述:     复用中断配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_ReuseInterruptConfig(void)
{
	DRV_Power_ReuseInterruptConfig();
}
/***************************************************************************************
**函数名:       HAL_Power_ReuseInterruptWakeupConfig
**功能描述:     复用中断配置唤醒
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_ReuseInterruptWakeupConfig(void)
{
	DRV_Power_ReuseInterruptWakeupConfig();
}
/***************************************************************************************
**函数名:       HAL_Power_ReservedSleepConfig
**功能描述:     未使用到的IO初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_ReservedSleepConfig(void)
{
	DRV_Power_ReservedSleepConfig();
}
/***************************************************************************************
**函数名:       DRV_Power_CPUInit
**功能描述:     CPU初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_CPUInit(void)
{
	DRV_Power_CPUInit();
}

/***************************************************************************************
**函数名:       HAL_Power_SysWakeupChk
**功能描述:     唤醒源查询 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Power_SysWakeupChk(void)
{
	switch (DRV_Power_SysWakeupChk())
	{
	case eSysWakeupNull:
		WakeUpSource = eWakeUpSourceNull;
		break;
	case eSysWakeupFinger:
		WakeUpSource = eWakeUpSourceFinger;
		break;
	case eSysWakeupNFC:
		WakeUpSource = eWakeUpSourceNFC;
		break;
	case eSysWakeupIr:
		WakeUpSource = eWakeUpSourceIr;
		break;
	case eSysWakeupBack:
		WakeUpSource = eWakeUpSourceBack;
		break;
	case eSysWakeupPOR:
		WakeUpSource = eWakeUpSourcePOR;
		break;
	case eSysWakeupTSI:
		WakeUpSource = eWakeUpSourceKey;
		break;
	case eSysWakeupAlarm:
		WakeUpSource = eWakeUpSourceAlarm;
		break;
	case eSysWakeupCatEye:
		WakeUpSource = eWakeUpSourceCatEye;
		break;
	case eSysWakeupTime:
		WakeUpSource = eWakeUpSourceTime;
		break;
    case eSysWakeupActiveDefense:
		WakeUpSource = eWakeUpSourceActiveDefense;
		break;
	default:
		WakeUpSource = eWakeUpSourceNull;
		break;
	}
	return 0;
}
/***************************************************************************************
**函数名:       HAL_Power_RAMToHold
**功能描述:     把数据存入RAM保持区 
**输入参数:     
int8_t *dat,  数据
uint32_t len  数据长度
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Power_RAMToHold(uint8_t *dat,uint32_t len)
{
	return DRV_Power_RAMToHold(dat,len);
}
/***************************************************************************************
**函数名:       HAL_Power_HoldToRAM
**功能描述:     RAM保持区读取数据 
**输入参数:     
int8_t *dat,  数据
uint32_t len  数据长度
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Power_HoldToRAM(uint8_t *dat,uint32_t len)
{
	return DRV_Power_HoldToRAM(dat,len);
}

/***************************************************************************************
**函数名:       HAL_Power_ReservedWakeupConfig
**功能描述:     未使用到的IO初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_ReservedWakeupConfig(void)
{
	DRV_Power_ReservedWakeupConfig();
}
/***************************************************************************************
**函数名:       HAL_Power_WakeupIoConfig
**功能描述:     唤醒后IO配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_WakeupIoConfig(void)
{
	DRV_Power_WakeupIoConfig();
}
/***************************************************************************************
**函数名:       HAL_Power_CPURst
**功能描述:     CPU重启
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_CPURst(void)
{
	DRV_Power_CPURst();
}

/***************************************************************************************
**函数名:       HAL_Power_TrngInit
**功能描述:     随机数发生器初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Power_TrngInit(void)
{
	DRV_Power_TrngInit();
}
/***************************************************************************************
**函数名:       HAL_Power_GetRandomWord
**功能描述:     随机数发生器
**输入参数:     --
**输出参数:     
uint32_t 随机数
**备注:         
****************************************************************************************/
uint32_t HAL_Power_GetRandomWord(void)
{
	return DRV_Power_GetRandomWord();
}
/***************************************************************************************
**函数名:       HAL_BuFang_Time_Init
**功能描述:     布防报警初始化，当你检测到门外有人，需要启动布防告警时候，调用此接口
**输入参数:     uint8_t time 报警时间
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_BuFang_Time_Init(uint8_t time,uint32_t *systickCounter)
{
	#if 1
    DRV_BuFang_Time_Init(time,systickCounter);
	#endif
}

/***************************************************************************************
**函数名:       HAL_BuFang_Time_Detet
**功能描述:     工作模式下布防检测结果
**输入参数:     uint8_t time 报警时间
**输出参数:     --
**备注:         
BUFANG_START表示开始检测，
BUFANG_DETET表示到人，
BUFANG_NO_DETET没检测到人
****************************************************************************************/
uint8_t HAL_BuFang_Time_Detet(uint8_t time,uint32_t *systickCounter)
{
	#if 1
    return DRV_BuFang_Time_Detet( time,systickCounter);
	#else
	return 3;
	#endif 
}    
/***************************************************************************************
**函数名:       DRV_BuFang_Time_Sleep
**功能描述:     进入低功耗时候判断是否需要布防检测
**输入参数:     uint8_t time 报警时间
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_BuFang_Time_Sleep(uint8_t time,uint32_t *systickCounter)
{
	#if 1
    DRV_BuFang_Time_Sleep(time,systickCounter);
	#endif
}
/***************************************************************************************
**函数名:       HAL_ReTry_Time_Set
**功能描述:     进入低功耗时候判断是否需要布防检测
**输入参数:     uint8_t time 报警时间
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_ReTry_Time_Set(uint8_t time)
{
    DRV_ReTry_Time_Set(time);
}
/***************************************************************************************
**函数名:       HAL_Bufang_Clear
**功能描述:     获取是否清除布防的标志
**输入参数:     --
**输出参数:     
uint8_t
0 可以清除
1 不可清除
**备注:         
****************************************************************************************/
uint8_t HAL_Bufang_Clear(void)
{
	#if 0
    return DRV_Bufang_Clear();
	#else
	return 0;
	#endif
}

/***************************************************************************************
**函数名:       HAL_InitWDT
**功能描述:     初始化看门狗
**输入参数:     --
**输出参数:     
uint32_t
看门狗时间
**备注:         
****************************************************************************************/
void HAL_InitWDT(uint16_t t)
{
	DRV_InitWDT(t);
}	
/***************************************************************************************
**函数名:       HAL_WDTFeedDog
**功能描述:     喂狗
**输入参数:     --
**输出参数:     
uint32_t
看门狗时间
**备注:         
****************************************************************************************/
void HAL_WDTFeedDog(void)
{
	DRV_WDTFeedDog();
}	
#endif /*HAL_POWER_MODULE_EN*/
	   /************************************Code End******************************************/