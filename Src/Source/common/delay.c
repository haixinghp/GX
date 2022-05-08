
#include "sys.h"

#include "cpm_drv.h"
#include "delay.h"

#if 0
/*******************************************************************************
* Function Name  : DelayMS
* Description    : Delay n MS
* Input          : delaymS£ºDelay Parame
* Output         : None
* Return         : None
*******************************************************************************/
void DelayMS(vu32 delaymS)
{
//	UINT32 counter=0;
	UINT32 k = 0;
	vu32 counter_ms = g_sys_clk/6000;
	
	while(delaymS --)
	{
		while(k < counter_ms)k ++;
		k = 0;
	}
}
/*******************************************************************************
* Function Name  : DelayMS
* Description    : Delay n MS
* Input          : delaymS£ºDelay Parame
* Output         : None
* Return         : None
*******************************************************************************/
void DelayUS(vu32 delayUS)
{
//	UINT32 counter=0;
	UINT32 k = 0;
	vu32 counter = g_sys_clk/6000000;
	
	while(delayUS --)
	{
		while(k < counter)k ++;
		k = 0;
	}
}
#endif
#if 1
/*******************************************************************************
* Function Name  : DelayMS
* Description    : Delay n MS
* Input          : delaymS¡êoDelay Parame
* Output         : None
* Return         : None
*******************************************************************************/
void DelayMS(vu32 delaymS)
{
// UINT32 counter=0;
 UINT32 k = 0;
 vu32 counter_ms = g_sys_clk/12000*2;
 
 while(delaymS --)
 {
  while(k < counter_ms)k ++;
  k = 0;
 }
}

void DelayUS(UINT32 delayuS)
{
  UINT32 k = 0;
 vu32 counter_us = (g_sys_clk/12000000*2)-2;
 
 while(delayuS --)
 {
  while(k < counter_us)k ++;
  k = 0;
 }
}
#endif
/*******************************************************************************
* Function Name  : delay
* Description    : Delay n nop
* Input          : time: n npo
* Output         : None
* Return         : None
*******************************************************************************/
void delay(vu32 time)
{
	while(time--);
}

