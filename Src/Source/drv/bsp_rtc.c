

#include "bsp_rtc.h"
#include "systick_drv.h"
#include "sys.h"
#include "rtc_drv.h"
#include "cpm_reg.h"
#include "debug.h"
#include "delay.h"

static unsigned int _SysTimeTask = 0; /*ϵͳʱ������*/

static unsigned char Common_Leap[2][12] = {
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, //����
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  //ƽ��
};

#define xMINUTE (60)         //1�ֵ�����
#define xHOUR (60 * xMINUTE) //1Сʱ������
#define xDAY (24 * xHOUR)    //1�������
#define xYEAR (365 * xDAY)   //1�������

tm time = {10, 10, 50, 50};

/****************************************************************************************
* �� ���� RTC_Ini
* �� �ܣ� rtc��ʼ��
****************************************************************************************/
void RTC_Ini(void)
{

  RTC_Init(EXTERNAL_CLK_SEL);
}

/****************************************************************************************
* �� ���� RTC_SetDate2Seconds
* �� �ܣ� ���ݵ�ǰʱ������ת��Ϊ����
* �� ����
* �� �룺
* �� ����
* ע �⣺
****************************************************************************************/
unsigned int RTC_SetDate2Seconds(_DateTime nTime)
{
  static unsigned int month[12] = {
      /*01��*/ xDAY * (0),
      /*02��*/ xDAY * (31),
      /*03��*/ xDAY * (31 + 28),
      /*04��*/ xDAY * (31 + 28 + 31),
      /*05��*/ xDAY * (31 + 28 + 31 + 30),
      /*06��*/ xDAY * (31 + 28 + 31 + 30 + 31),
      /*07��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30),
      /*08��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31),
      /*09��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
      /*10��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
      /*11��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
      /*12��*/ xDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)};
  unsigned int seconds = 0;
  unsigned short year = 0;

  year = nTime.year + 2000 - 1970;                  //������2100��ǧ�������
  seconds = xYEAR * year + xDAY * ((year + 1) / 4); //ǰ�����ȥ������
  seconds += month[nTime.month - 1];                //���Ͻ��걾�¹�ȥ������
  if ((nTime.month > 2) && (((year + 2) % 4) == 0)) //2008��Ϊ����
    seconds += xDAY;                                //�����1������
  seconds += xDAY * (nTime.day - 1);                //���ϱ����ȥ������
  seconds += xHOUR * nTime.hour;                    //���ϱ�Сʱ��ȥ������
  seconds += xMINUTE * nTime.minute;                //���ϱ����ӹ�ȥ������
  seconds += nTime.second;                          //���ϵ�ǰ����<br>��seconds -= 8 * xHOUR;
  return seconds;
}
/****************************************************************************************
* �� ���� RTC_Date2Seconds
* �� �ܣ� ���ݵ�ǰʱ�䣬ת��Ϊ����
* �� ����
* �� �룺
* �� ����
* ע �⣺
****************************************************************************************/
unsigned int RTC_Date2Seconds(void)
{
  unsigned int seconds = 0;
  unsigned short year = 0;

  RTC_Get();

  seconds += xDAY * (time.day);     //���ϱ����ȥ������
  seconds += xHOUR * time.hour;     //���ϱ�Сʱ��ȥ������
  seconds += xMINUTE * time.minute; //���ϱ����ӹ�ȥ������
  seconds += time.second;           //���ϵ�ǰ����<br>��seconds -= 8 * xHOUR;
  seconds -= 8 * 3600;
  return seconds;
}
/****************************************************************************************
* �� ���� RTC_Seconds2Date
* �� �ܣ� ���ݵ�ǰ������ת��Ϊʱ��
* �� ����
* �� �룺
* �� ����
* ע �⣺
****************************************************************************************/
//UNIXתΪUTC �ѽ���ʱ��ת�� ����ʱ��UTC+8
_DateTime RTC_Seconds2Date(unsigned long seconds)
{

  unsigned int days;
  unsigned short leap_y_count;

  time.second = seconds % 60; //�����
  seconds /= 60;
  time.minute = seconds % 60; //��÷�
                              //    seconds          += 8 * 60 ;        //ʱ������ תΪUTC+8 bylzs
  seconds /= 60;
  time.hour = seconds % 24; //���ʱ
  time.day = seconds / 24;  //���������
}

//��о��ʽתΪUTC �ѽ���ʱ��ת�� ����ʱ��UTC+8
_DateTime RTC_GuoXin2Date(void)
{

  static unsigned int month[12] = {
      /*01��*/ 31,
      /*02��*/ 28,
      /*03��*/ 31,
      /*04��*/ 30,
      /*05��*/ 31,
      /*06��*/ 30,
      /*07��*/ 31,
      /*08��*/ 31,
      /*09��*/ 30,
      /*10��*/ 31,
      /*11��*/ 30,
      /*12��*/ 31};
  unsigned int days;
  unsigned short leap_y_count;
  _DateTime Dtime;
  RTC_Get();
  days = time.day;                    //�����
  leap_y_count = (days + 365) / 1461; //��ȥ�˶��ٸ�����(4��һ��)
  if (((days + 366) % 1461) == 0)
  {                                   //��������1��
    Dtime.year = 1970 + (days / 366); //�����
    Dtime.month = 12;                 //������
    Dtime.day = 31;
  }
  days -= leap_y_count;
  Dtime.year = 1970 + (days / 365); //�����
  days %= 365;                      //����ĵڼ���
  days = 01 + days;                 //1�տ�ʼ
  if ((Dtime.year % 4) == 0)
  {
    if (days > 60)
      --days; //�������
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
  ++Dtime.month;    //������
  Dtime.day = days; //�����
  Dtime.hour = time.hour;
  Dtime.minute = time.minute;
  Dtime.second = time.second;
  return Dtime;
}
/****************************************************************************************
* �� ���� RTC_Seconds2Date
* �� �ܣ� ���ݵ�ǰ������ת��Ϊʱ��
* �� ����
* �� �룺
* �� ����
* ע �⣺
****************************************************************************************/
//UNIXתΪUTC �ѽ���ʱ��ת�� ����ʱ��UTC+8
_DateTime RTC_GB_Seconds2Date(unsigned long seconds)
{
  static unsigned int month[12] = {
      /*01��*/ 31,
      /*02��*/ 28,
      /*03��*/ 31,
      /*04��*/ 30,
      /*05��*/ 31,
      /*06��*/ 30,
      /*07��*/ 31,
      /*08��*/ 31,
      /*09��*/ 30,
      /*10��*/ 31,
      /*11��*/ 30,
      /*12��*/ 31};
  unsigned int days;
  unsigned short leap_y_count;
  _DateTime time;

  time.second = seconds % 60; //�����
  seconds /= 60;
  time.minute = seconds % 60; //��÷�
  //seconds          += 8 * 60 ;        //ʱ������ תΪUTC+8 bylzs
  seconds /= 60;
  time.hour = seconds % 24;           //���ʱ
  days = seconds / 24;                //���������
  leap_y_count = (days + 365) / 1461; //��ȥ�˶��ٸ�����(4��һ��)
  if (((days + 366) % 1461) == 0)
  {                                  //��������1��
    time.year = 1970 + (days / 366); //�����
    time.month = 12;                 //������
    time.day = 31;
  }
  days -= leap_y_count;
  time.year = 1970 + (days / 365); //�����
  days %= 365;                     //����ĵڼ���
  days = 01 + days;                //1�տ�ʼ
  if ((time.year % 4) == 0)
  {
    if (days > 60)
      --days; //�������
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
  ++time.month;    //������
  time.day = days; //�����
  time.year -= 2000;
  return time;
}
/****************************************************************************************
* �� ���� RTC_getHour
* �� �ܣ� ����Сʱ
* �� ����
* �� �룺
* �� ����
* ע �⣺
****************************************************************************************/
/*
*��̬����Сʱ����
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
  Dtime = RTC_GuoXin2Date(); //��ȡʱ��

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
* �� ���� RTC_Ini
* �� �ܣ� rtc��ʼ��
****************************************************************************************/
void RTC_Set(unsigned long seconds)
{
  RTC_Seconds2Date(seconds);
  RTC_SetTime(time);
  //	printf("set time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
}

/****************************************************************************************
* �� ���� RTC_Ini
* �� �ܣ� rtc��ʼ��
****************************************************************************************/
void RTC_Get(void)
{
  RTC_GetTime(&time);
  //	printf("get time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
}
