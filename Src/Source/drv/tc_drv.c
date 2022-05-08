// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : tc_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "debug.h"
#include "tc_drv.h"
#include "cpm_drv.h"

volatile uint8_t g_tc_int_flag = 0;
/*******************************************************************************
* Function Name  : TC_ISR
* Description    : TC �жϴ�����
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void TC_ISR(void)
{
	TC->TCCR |= TC_IF;
	while (TC->TCCR & TC_IS)
		g_tc_int_flag = 1;
}

void TC_IRQHandler(void)
{
	TC_ISR();
}

/*******************************************************************************
* Function Name  : TC_Init
* Description    : TC��ʼ��
* Input          : - tc_wdp: TCԤ��Ƶֵ
*                  - Counterval��time��ʼֵ
*
* Output         : None
* Return         : None
******************************************************************************/
void TC_Init(TC_TIMER_PRESCALER tc_wdp, UINT16 Counterval)
{
	TC->TCMR = Counterval;
	TC->TCCR = (tc_wdp << 4) | TC_IE;
	TC->TCCR |= TC_CU;
}

/*******************************************************************************
* Function Name  : TC_Close
* Description    : �ر�TC
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void TC_Close(void)
{
	TC->TCCR &= ~TC_RN;
	TC->TCCR &= ~TC_IE;
}

/*******************************************************************************
* Function Name  : TC_FeedWatchDog
* Description    : TCι�������TC watchdog����ֹtc time out����ϵͳ�ж�
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void TC_FeedWatchDog(void)
{
	TC->TCSR = 0x5555;
	TC->TCSR = 0xAAAA;
}

/*******************************************************************************
* Function Name  : TC_GetCount
* Description    : ��ȡTC counterֵ
* Input          : None
*
* Output         : TC counter
* Return         : None
******************************************************************************/
UINT32 TC_GetCount(void)
{
	return (TC->TCCNTR);
}

/*******************************************************************************
* Function Name  : TC_ResetInit
* Description    : TC reset���ܳ�ʼ��
* Input          : -wdp��TC��Ƶϵ��
*                  -tcmr��TC������ʼֵ
*                  -reset_en���Ƿ����TC reset
* Output         : None
* Return         : None
******************************************************************************/
void TC_ResetInit(TC_TIMER_PRESCALER wdp, UINT16 tcmr, BOOL reset_en)
{
	UINT16 temp;

	if (FALSE == reset_en) //close reset
	{
		TC->TCCR &= ~TC_RN;
		TC->TCMR = tcmr;
		temp = TC->TCCR;
		temp &= (~0x7f);
		NVIC_Init(3, 3, TC_IRQn, 2);
		TC->TCCR = temp | TC_IE | (wdp << TC_WDP_SHIFT);
	}
	else
	{
		TC->TCMR = tcmr;
		temp = TC->TCCR;
		temp &= (~0x7f);
		TC->TCCR = temp | TC_RN | (wdp << TC_WDP_SHIFT);
	}
	TC->TCCR |= TC_CU; // update WDP & WMR
}

/*******************************************************************************
* Function Name  : TC_IntInit
* Description    : TC �жϹ��ܳ�ʼ��
* Input          : -wdp��TC��Ƶϵ��
*                  -tcmr��TC������ʼֵ
*                  -int_en���Ƿ����TC int
* Output         : None
* Return         : None
******************************************************************************/
void TC_IntInit(TC_TIMER_PRESCALER wdp, UINT16 tcmr, BOOL int_en)
{
	UINT16 temp;

	TC->TCMR = tcmr;
	temp = TC->TCCR;
	temp &= (~0x7f);

	if (TRUE == int_en)
	{
		NVIC_Init(3, 3, TC_IRQn, 2);
		TC->TCCR = temp | TC_IE | (wdp << TC_WDP_SHIFT);
	}
	else
		TC->TCCR = temp | (wdp << TC_WDP_SHIFT);

	TC->TCCR |= TC_CU;
}

/**
 * @brief TC select RTC32K source.
 * 1. TC_selectClockSource(1); //ѡ���ⲿ32K������ΪTCʱ��Դ.
 * @param clk_src 
 */
void TC_selectClockSource(unsigned int clk_src)
{
	clk_src = !!clk_src;
	CPM->CPM_CSWCFGR = (CPM->CPM_CSWCFGR & ~OSCL_SEL) | (clk_src << 6);
}
