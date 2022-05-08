/***************************************************************************************
**�ļ���:     DRV_RTC.c
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
#include "DRV_RTC.h"

#include "rtc_drv.h"
#include "cpm_reg.h"
#include "debug.h"
#include "pci_drv.h"
//#include "bsp_rtc.h"

#if DRV_RTC_MODULE_EN
#warning DRV_RTCģ��ʹ��!
/*******************************�궨��***************************/

/*************************.C�����ṹ����***********************/
//typedef struct t_DateTime
//{
//  int year;
//  int month;
//  int day;
//  int hour;
//  int minute;
//  int second;
//} _DateTime;
/*************************˽��ȫ�ֱ���*************************/
static GXtm time = {10, 10, 50, 50};
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/
/****************************************************************************************
* �� ���� RTC_Ini
* �� �ܣ� rtc��ʼ��
****************************************************************************************/
static void RTC_Get(void)
{
  RTC_GetTime(&time);
  //	printf("get time : Day:%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
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

static _DateTime RTC_Seconds2Date(unsigned long seconds)
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
static _DateTime RTC_GuoXin2Date(void)
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
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       --
**��������:     --
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_RTC_Init(void)
{
  RTC_Init(EXTERNAL_CLK_SEL);
}
/***************************************************************************************
**������:       DRV_RTC_SetByTick
**��������:     ͨ��ʱ�����ʼ��ʱ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_RTC_SetByTick(uint32_t tick)
{
  RTC_Seconds2Date(tick);
  RTC_SetTime(time);
}
/***************************************************************************************
**������:       DRV_RTC_SetByTick
**��������:     ͨ��ʱ�����ʼ��ʱ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
_DateTime DRV_RTC_TimeGet(void)
{
  return RTC_GuoXin2Date();
}
#endif /*DRV_RTC_MODULE_EN*/
       /************************************Code End******************************************/