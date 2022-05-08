/***************************************************************************************
**文件名:     DRV_RTC.c
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
#include "DRV_RTC.h"

#include "rtc_drv.h"
#include "cpm_reg.h"
#include "debug.h"
#include "pci_drv.h"
//#include "bsp_rtc.h"

#if DRV_RTC_MODULE_EN
#warning DRV_RTC模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/
//typedef struct t_DateTime
//{
//  int year;
//  int month;
//  int day;
//  int hour;
//  int minute;
//  int second;
//} _DateTime;
/*************************私有全局变量*************************/
static GXtm time = {10, 10, 50, 50};
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/****************************************************************************************
* 函 数： RTC_Ini
* 功 能： rtc初始化
****************************************************************************************/
static void RTC_Get(void)
{
  RTC_GetTime(&time);
  //	printf("get time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
}
/****************************************************************************************
* 函 数： RTC_Seconds2Date
* 功 能： 根据当前秒数，转换为时间
* 描 述：
* 输 入：
* 输 出：
* 注 意：
****************************************************************************************/
//UNIX转为UTC 已进行时区转换 北京时间UTC+8
_DateTime RTC_GB_Seconds2Date(unsigned long seconds)
{
  static unsigned int month[12] = {
      /*01月*/ 31,
      /*02月*/ 28,
      /*03月*/ 31,
      /*04月*/ 30,
      /*05月*/ 31,
      /*06月*/ 30,
      /*07月*/ 31,
      /*08月*/ 31,
      /*09月*/ 30,
      /*10月*/ 31,
      /*11月*/ 30,
      /*12月*/ 31};
  unsigned int days;
  unsigned short leap_y_count;
  _DateTime time;

  time.second = seconds % 60; //获得秒
  seconds /= 60;
  time.minute = seconds % 60; //获得分
  //seconds          += 8 * 60 ;        //时区矫正 转为UTC+8 bylzs
  seconds /= 60;
  time.hour = seconds % 24;           //获得时
  days = seconds / 24;                //获得总天数
  leap_y_count = (days + 365) / 1461; //过去了多少个闰年(4年一闰)
  if (((days + 366) % 1461) == 0)
  {                                  //闰年的最后1天
    time.year = 1970 + (days / 366); //获得年
    time.month = 12;                 //调整月
    time.day = 31;
  }
  days -= leap_y_count;
  time.year = 1970 + (days / 365); //获得年
  days %= 365;                     //今年的第几天
  days = 01 + days;                //1日开始
  if ((time.year % 4) == 0)
  {
    if (days > 60)
      --days; //闰年调整
    else
    {
      if (days == 60)
      {
        time.month = 2;
        time.day = 29;
      }
    }
  }
  for (time.month = 0; month[time.month] < days; time.month++)
  {
    days -= month[time.month];
  }
  ++time.month;    //调整月
  time.day = days; //获得日
  time.year -= 2000;
  return time;
}

static _DateTime RTC_Seconds2Date(unsigned long seconds)
{

  unsigned int days;
  unsigned short leap_y_count;

  time.second = seconds % 60; //获得秒
  seconds /= 60;
  time.minute = seconds % 60; //获得分
                              //    seconds          += 8 * 60 ;        //时区矫正 转为UTC+8 bylzs
  seconds /= 60;
  time.hour = seconds % 24; //获得时
  time.day = seconds / 24;  //获得总天数
}
//国芯格式转为UTC 已进行时区转换 北京时间UTC+8
static _DateTime RTC_GuoXin2Date(void)
{
  static unsigned int month[12] = {
      /*01月*/ 31,
      /*02月*/ 28,
      /*03月*/ 31,
      /*04月*/ 30,
      /*05月*/ 31,
      /*06月*/ 30,
      /*07月*/ 31,
      /*08月*/ 31,
      /*09月*/ 30,
      /*10月*/ 31,
      /*11月*/ 30,
      /*12月*/ 31};
  unsigned int days;
  unsigned short leap_y_count;
  _DateTime Dtime;
  RTC_Get();
  days = time.day;                    //获得天
  leap_y_count = (days + 365) / 1461; //过去了多少个闰年(4年一闰)
  if (((days + 366) % 1461) == 0)
  {                                   //闰年的最后1天
    Dtime.year = 1970 + (days / 366); //获得年
    Dtime.month = 12;                 //调整月
    Dtime.day = 31;
  }
  days -= leap_y_count;
  Dtime.year = 1970 + (days / 365); //获得年
  days %= 365;                      //今年的第几天
  days = 01 + days;                 //1日开始
  if ((Dtime.year % 4) == 0)
  {
    if (days > 60)
      --days; //闰年调整
    else
    {
      if (days == 60)
      {
        Dtime.month = 2;
        Dtime.day = 29;
      }
    }
  }
  for (Dtime.month = 0; month[Dtime.month] < days; Dtime.month++)
  {
    days -= month[Dtime.month];
  }
  ++Dtime.month;    //调整月
  Dtime.day = days; //获得日
  Dtime.hour = time.hour;
  Dtime.minute = time.minute;
  Dtime.second = time.second;
  return Dtime;
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_RTC_Init(void)
{
  RTC_Init(EXTERNAL_CLK_SEL);
}
/***************************************************************************************
**函数名:       DRV_RTC_SetByTick
**功能描述:     通过时间戳初始化时间
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_RTC_SetByTick(uint32_t tick)
{
  RTC_Seconds2Date(tick);
  RTC_SetTime(time);
}
/***************************************************************************************
**函数名:       DRV_RTC_SetByTick
**功能描述:     通过时间戳初始化时间
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
_DateTime DRV_RTC_TimeGet(void)
{
  return RTC_GuoXin2Date();
}
#endif /*DRV_RTC_MODULE_EN*/
       /************************************Code End******************************************/