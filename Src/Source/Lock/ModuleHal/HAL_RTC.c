/***************************************************************************************
**文件名:     HAL_RTC.c
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
#include "HAL_RTC.h"
#include "DRV_RTC.h"
#if  HAL_RTC_MODULE_EN
#warning HAL_RTC模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/  


/*************************局部函数*****************************/  


/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       HAL_RTC_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_RTC_Init(void)
{
    DRV_RTC_Init();
}
/***************************************************************************************
**函数名:       HAL_RTC_SetByTick
**功能描述:     通过时间戳初始化时间
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_RTC_SetByTick(uint32_t tick)
{
	tick = tick+28800;    //加8个小时.中国处于东8区。
	DRV_RTC_SetByTick(tick);
}
/***************************************************************************************
**函数名:       HAL_RTC_TimeGetStm
**功能描述:     获取时间
**输入参数:     --
**输出参数:     
TYPEs_Stm
标准时间格式
**备注:         
****************************************************************************************/
uint8_t HAL_RTC_TimeGetStm(struct tm *stmTmp)
{
	_DateTime tmTmp;
//	struct tm stmTmp;
	tmTmp =DRV_RTC_TimeGet();
	stmTmp->tm_sec = tmTmp.second;
	stmTmp->tm_min = tmTmp.minute;
	stmTmp->tm_hour = tmTmp.hour;
	stmTmp->tm_mday = tmTmp.day;
	stmTmp->tm_mon = tmTmp.month;
	stmTmp->tm_year = tmTmp.year;
	DEBUG_LOG_OUT("stmTmp->tm_year:%d\n",stmTmp->tm_year);
	DEBUG_LOG_OUT("stmTmp->tm_mon:%d\n",stmTmp->tm_mon);
	DEBUG_LOG_OUT("stmTmp->tm_mday:%d\n",stmTmp->tm_mday);
	DEBUG_LOG_OUT("stmTmp->tm_hour%d\n",stmTmp->tm_hour);
	DEBUG_LOG_OUT("stmTmp->tm_min%d\n",stmTmp->tm_min);
	DEBUG_LOG_OUT("stmTmp->tm_sec:%d\n",stmTmp->tm_sec);
	return 0;
	
}
/***************************************************************************************
**函数名:       HAL_RTC_GetTick
**功能描述:     获取时间戳
**输入参数:     --
**输出参数:     
uint32_t
时间戳
**备注:         
****************************************************************************************/
uint32_t HAL_RTC_GetTick(void)
{
	struct tm tmTmp;
	HAL_RTC_TimeGetStm(&tmTmp);
	return HAL_RTC_Stm2Tick(tmTmp);
}
/***************************************************************************************
**函数名:       HAL_RTC_Stm2Tick
**功能描述:     时间转换为时间戳
**输入参数:     --
**输出参数:     
TYPEs_Stm
标准时间格式
**备注:         
注意，读取的时间不能小于28800
****************************************************************************************/
uint32_t HAL_RTC_Stm2Tick(struct tm tim)
{ 
    uint32_t tickTmp =0; 
	tim.tm_year -= 1900;  /* 自 1900 起的年数                */
	tim.tm_mon -= 1; /* 月份，范围从 0 到 11                */
    tickTmp = mktime(&tim);
    if(tickTmp < 28800)
    {
        return 0;
    }
	return tickTmp-28800;
}

struct tm * HAL_RTC_Tick2Stm(uint32_t tick)
{
	struct tm *stm;
	tick = tick+28800;    //加8个小时.中国处于东8区。
	stm = localtime((const time_t *)&tick);
	return stm;
}
#endif  /*HAL_RTC_MODULE_EN*/
/************************************Code End******************************************/