/***************************************************************************************
**文件名:     DRV_Motor.c
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
#include "DRV_Motor.h"
#if  DRV_MOTOR_MODULE_EN
#warning DRV_MOTOR模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  


/*************************局部函数*****************************/  


/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       DRV_Motor_Stop
**功能描述:     电机驱动停止
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Motor_Init(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
}
/***************************************************************************************
**函数名:       DRV_Motor_Stop
**功能描述:     电机驱动停止
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Motor_Stop(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(0);
    MT_STOP_PROCESS();
}

/***************************************************************************************
**函数名:       DRV_Motor_Open
**功能描述:     电机驱动开
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Motor_Open(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(1);
    MT_OPEN_PROCESS();
}
/***************************************************************************************
**函数名:       DRV_Motor_Close
**功能描述:     电机驱动关
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Motor_Close(void)
{
    M_M1_OUT_PP();
  	M_M2_OUT_PP();
	DRV_Motor_Enable(1);
    MT_CLOSE_PROCESS();
}

/***************************************************************************************
**函数名:       DRV_Motor_LockHandle
**功能描述:     把手检测
**输入参数:     --
**输出参数:     
INT8S
0 状态0
1 状态1
**备注:         
****************************************************************************************/
INT8S DRV_Motor_LockHandle(void)
{
  //注意初始化IO
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
**函数名:       DRV_Motor_Enable
**功能描述:     电机使能
**输入参数:     
1 电机使能
0 电机失能
**输出参数:     --
**备注:         
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