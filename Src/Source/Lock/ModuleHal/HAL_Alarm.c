/***************************************************************************************
**文件名:     HAL_Alarm.c
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
#include "HAL_Alarm.h"
#if  HAL_ALARM_MODULE_EN
#warning HAL_ALARM模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  


/*************************局部函数*****************************/  


/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
/*
void HAL_Alarm_Init(void)
{
    
}
*/

/***************************************************************************************
**函数名:       HAL_Alarm_SetWakeUp
**功能描述:     唤醒后配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Alarm_SetWakeUp(void)
{
	DRV_Alarm_SetWakeUp();
}
/***************************************************************************************
**函数名:       HAL_Alarm_SetInterrpt
**功能描述:     io配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Alarm_SetInterrpt(void)
{
	DRV_Alarm_SetInterrpt();
}
/***************************************************************************************
**函数名:       HAL_Alarm_SetInterrpt
**功能描述:     io配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Alarm_InterruptReg(AlarmIntHandler_t fun)
{
	if(NULL == fun)
	{
		return -1; //参数错误
	}
	AlarmIntHandler = fun; //传递函数指针
}

/***************************************************************************************
**函数名:       HAL_Alarm_GetStatus
**功能描述:     报警获取状态
**输入参数:     --
**输出参数:     
int8_t
0、低电平
1、高电平
-1 滤波中
**备注:         
****************************************************************************************/
int8_t HAL_Alarm_GetStatus()
{
	return DRV_Alarm_GetStatus();
}
#endif  /*HAL_ALARM_MODULE_EN*/
/************************************Code End******************************************/