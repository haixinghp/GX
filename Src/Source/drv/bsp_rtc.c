

#include "bsp_rtc.h"
#include "systick_drv.h"
#include "sys.h"
#include "rtc_drv.h"
#include "cpm_reg.h"
#include "debug.h"
#include "delay.h"

static unsigned int _SysTimeTask = 0; /*系统时间任务*/

static unsigned char Common_Leap[2][12] = {
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, //闰年
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  //平年
};

#define xMINUTE (60)         //1分的秒数
#define xHOUR (60 * xMINUTE) //1小时的秒数
#define xDAY (24 * xHOUR)    //1天的秒数
#define xYEAR (365 * xDAY)   //1年的秒数

tm time = {10, 10, 50, 50};

/****************************************************************************************
* 函 数： RTC_Ini
* 功 能： rtc初始化
****************************************************************************************/
void RTC_Ini(void)
{

  RTC_Init(EXTERNAL_CLK_SEL);
}

/****************************************************************************************
* 函 数： RTC_SetDate2Seconds
* 功 能： 根据当前时间设置转换为秒数
* 描 述：
* 输 入：
* 输 出：
* 注 意：
****************************************************************************************/
unsigned int RTC_SetDate2Seconds(_DateTime nTime)
{
  static unsigned int month[12] = {
      /*01月*/ xDAY * (0),
      /*02月*/ xDAY * (31),
      /*03月*/ xDAY * (31 + 28),
      /*04月*/ xDAY * (31 + 28 + 31),
      /*05月*/ xDAY * (31 + 28 + 31 + 30),
      /*06月*/ xDAY * (31 + 28 + 31 + 30 + 31),
      /*07月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30),
      /*08月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31),
      /*09月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
      /*10月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
      /*11月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
      /*12月*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)};
  unsigned int seconds = 0;
  unsigned short year = 0;

  year = nTime.year + 2000 - 1970;                  //不考虑2100年千年虫问题
  seconds = xYEAR * year + xDAY * ((year + 1) / 4); //前几年过去的秒数
  seconds += month[nTime.month - 1];                //加上今年本月过去的秒数
  if ((nTime.month > 2) && (((year + 2) % 4) == 0)) //2008年为闰年
    seconds += xDAY;                                //闰年加1天秒数
  seconds += xDAY * (nTime.day - 1);                //加上本天过去的秒数
  seconds += xHOUR * nTime.hour;                    //加上本小时过去的秒数
  seconds += xMINUTE * nTime.minute;                //加上本分钟过去的秒数
  seconds += nTime.second;                          //加上当前秒数<br>　seconds -= 8 * xHOUR;
  return seconds;
}
/****************************************************************************************
* 函 数： RTC_Date2Seconds
* 功 能： 根据当前时间，转换为秒数
* 描 述：
* 输 入：
* 输 出：
* 注 意：
****************************************************************************************/
unsigned int RTC_Date2Seconds(void)
{
  unsigned int seconds = 0;
  unsigned short year = 0;

  RTC_Get();

  seconds += xDAY * (time.day);     //加上本天过去的秒数
  seconds += xHOUR * time.hour;     //加上本小时过去的秒数
  seconds += xMINUTE * time.minute; //加上本分钟过去的秒数
  seconds += time.second;           //加上当前秒数<br>　seconds -= 8 * xHOUR;
  seconds -= 8 * 3600;
  return seconds;
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
_DateTime RTC_Seconds2Date(unsigned long seconds)
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
_DateTime RTC_GuoXin2Date(void)
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
/****************************************************************************************
* 函 数： RTC_getHour
* 功 能： 计算小时
* 描 述：
* 输 入：
* 输 出：
* 注 意：
****************************************************************************************/
/*
*动态密码小时计算
*/

int RTC_getHour(unsigned short CNT, unsigned char year_res)
{

  int result;
  unsigned char temp_time[6];
  unsigned short hour;
  unsigned short year = 0, month = 0;
  unsigned short day = 0;
  unsigned char i;
  _DateTime Dtime;
  Dtime = RTC_GuoXin2Date(); //获取时间

  temp_time[0] = Dtime.year;
  temp_time[1] = Dtime.month;
  temp_time[2] = Dtime.day;

  temp_time[3] = Dtime.hour;

  year = temp_time[0] + 2000;
  if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
  {
    i = Leap_Year;
  }
  else
  {
    i = Common_year;
  }
  for (month = 0; month < (temp_time[1] - 1); month++)
  {
    day = day + Common_Leap[i][month];
  }

  hour = (day + temp_time[2] - 1) * 24 + temp_time[3];

  if (i == Leap_Year)
  {
    if (year_res)
    {
      if ((CNT + 8784) > hour)
      {
        result = 1;
      }
      else
      {
        result = -1;
      }
    }
    else
    {
      if (CNT > hour)
      {
        result = 1;
      }
      else
      {
        result = -1;
      }
    }
  }
  else if (i == Common_year)
  {
    if (year_res)
    {
      if ((CNT + 8760) > hour)
      {
        result = 1;
      }
      else
      {
        result = -1;
      }
    }
    else
    {
      if (CNT > hour)
      {
        result = 1;
      }
      else
      {
        result = -1;
      }
    }
  }
  return result;
}

/****************************************************************************************
* 函 数： RTC_Ini
* 功 能： rtc初始化
****************************************************************************************/
void RTC_Set(unsigned long seconds)
{
  RTC_Seconds2Date(seconds);
  RTC_SetTime(time);
  //	printf("set time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
}

/****************************************************************************************
* 函 数： RTC_Ini
* 功 能： rtc初始化
****************************************************************************************/
void RTC_Get(void)
{
  RTC_GetTime(&time);
  //	printf("get time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
}
