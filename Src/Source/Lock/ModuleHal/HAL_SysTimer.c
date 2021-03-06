/***************************************************************************************
**文件名:     HAL_SysTimer.c
**版本:       V1.00
**说明:       硬件抽象层
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
****************************************************************************************/
#include "HAL_SysTimer.h"
#include "DRV_SysTimer.h"
#if  HAL_SYS_TIMER_MODULE_EN
#warning HAL_SYS_TIMER模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  
TYPEs_sysTimerCtrl sysTimerCtrl=
{
    0,      //UINT32 tick;                            //系统滴答计数器	
};

/*************************局部函数*****************************/  
/***************************************************************************************
**函数名:       HAL_SysTimeRun
**功能描述:     系统滴答时钟累积,需要放到中断中运行
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void HAL_SysTimeRun(void)
{
    sysTimerCtrl.tick++;
}

/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       HAL_SysTimerInit
**功能描述:     调用系统时钟初始化驱动函数
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_SysTimerInit(void)
{
    DRV_SysTimerInit();
//    DRV_SysTimerHandlerReg(HAL_SysTimeRun);     //注册定时器中断服务程序
//	sysTimerCtrl.tick = 0; //清空系统定时器
}

/***************************************************************************************
**函数名:       HAL_SysTimerHandlerReg
**功能描述:     回调函数注册
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
INT8S HAL_SysTimerHandlerReg(halSysTimerHandler_t pfun)
{
   return  DRV_SysTimerHandlerReg(pfun);
}
/***************************************************************************************
**函数名:       HAL_SysTimerClose
**功能描述:     关闭系统时钟
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_SysTimerClose(void)
{
	DRV_SysTimerClose();
}
/***************************************************************************************
**函数名:       HAL_SysTimeDeal
**功能描述:     滴答时钟处理
**输入参数:     --
**输出参数:     0,滴答时间到;-1,滴答时间未到。
**备注:         
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
**函数名:       HAL_SysTimer_ClearCnt
**功能描述:     清除计数器
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_SysTimer_ClearCnt(void)
{
    sysTimerCtrl.tick = 0;
}    
#endif  /*HAL_SYS_TIMER_MODULE_EN*/
/************************************Code End******************************************/
