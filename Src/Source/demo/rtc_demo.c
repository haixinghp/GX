// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : rtc_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "rtc_drv.h"
#include "common.h"
//#include "iccemv.h"
#include "debug.h"
#include "delay.h"
#include "cpm_reg.h"
#include "bsp_rtc.h"
void RTC_Demo(void)
{
	
//	 _DateTime Dtime;
//	
//	RTC_Ini();
//	Dtime.year = 20;
//	Dtime.month = 10;
//	Dtime.day = 27;
//	Dtime.hour = 10;
//	Dtime.minute =29;
//	Dtime.second = 14;
//	RTC_Set( RTC_SetDate2Seconds(Dtime));
//	DelayMS(1000);//设置完时间，不能马上读取，需要延时一段时间
//	while(1)
//		{
//			Dtime = RTC_GuoXin2Date(); //获取时间
//			printf("year=%d\r\n",Dtime.year);
//			printf("month=%d\r\n",Dtime.month);
//			printf("day=%d\r\n",Dtime.day);
//			printf("hour=%d\r\n",Dtime.hour);
//			printf("minute=%d\r\n",Dtime.minute);
//			printf("second=%d\r\n",Dtime.second);	
//			DelayMS(3000);		
//		}
	
	GXtm time = {10, 10, 50, 50};
	
	RTC_Init(EXTERNAL_CLK_SEL);
//	RTC_Init(INTERNAL_CLK_SEL);
	
	DelayMS(1000);

	RTC_SetTime(time);
	time.second = 20;
	RTC_SetAlarm(time, SECOND_ALARM);
	DelayMS(1000);//设置完时间，不能马上读取，需要延时一段时间
	while(1)
	{
		printf("g_rtc_int_sta = 0x%08x\n", g_rtc_int_sta);
		if (g_rtc_int_sta&Ala_intf)
		{
			g_rtc_int_sta &= ~Ala_intf;
			printf("alarm interrupt\n");
		}
		RTC_GetTime(&time);
		printf("time : D-%d [%d:%d:%d]\n", time.day, time.hour, time.minute, time.second);
		DelayMS(1000);
	}
}
void RTC_TIME_TEST()
{
// _DateTime Dtime;
//	
//	RTC_Ini();
//	Dtime.year = 20;
//	Dtime.month = 10;
//	Dtime.day = 27;
//	Dtime.hour = 10;
//	Dtime.minute =29;
//	Dtime.second = 14;
//	RTC_Set( RTC_SetDate2Seconds(Dtime));
//	DelayMS(1000);//设置完时间，不能马上读取，需要延时一段时间
//	while(1)
//		{
//			Dtime = RTC_GuoXin2Date(); //获取时间
//			printf("year=%d\r\n",Dtime.year);
//			printf("month=%d\r\n",Dtime.month);
//			printf("day=%d\r\n",Dtime.day);
//			printf("hour=%d\r\n",Dtime.hour);
//			printf("minute=%d\r\n",Dtime.minute);
//			printf("second=%d\r\n",Dtime.second);	
//			DelayMS(3000);		
//		}
}
