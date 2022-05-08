// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : pit32_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "debug.h"
#include "pit32_drv.h"
//#include "test_demo.h"

#ifdef BCTC_DEMO
#include "iccemv.h"
#endif

#if PIT32_ADDR == PIT1_BASE_ADDR
	#define PIT_IRQn   PIT1_IRQn
#else
	#define PIT_IRQn   PIT2_IRQn
#endif
extern UINT32 sleep_cnt;
typedef volatile unsigned short  VU16;
typedef volatile unsigned int    VU32;
typedef struct{
	VU16 PCSR;          //PIT Control and Status Register
	VU16 RSVD;
	VU32 PMR;           //PIT Modulus Register
	VU32 PCNTR;         //PIT Count Register
	VU32 Reserved_06;
}PIT_t;
#define PIT11   (*(PIT_t   *)(0x40007000))
extern volatile U32 tick_count;

/*******************************************************************************
* Function Name  : PIT32_ISR
* Description    : PIT32�жϴ���
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
static void PIT32_ISR(PIT32_TypeDef *PITx)
{
	UINT32 tmp1 = PITx->PCSR;

	if (tmp1 & PCSR_PIF)//pit
	{
		PITx->PCSR |= PIT_PIF;
/*************************************************************/
		//����Ϊ7816��ģʽ����Ҫ��ʱ���жϴ������
#ifdef USI_PIT
		if((k_IcMax_TotalTimeCount>0)&&(k_total_timeover==0))
		{
			  k_Ic_TotalTimeCount++;
			 if(k_Ic_TotalTimeCount>k_IcMax_TotalTimeCount)
			 {
				k_total_timeover=1;
			 }
			 if(k_total_timeover==1)
			 {
			   k_Ic_TotalTimeCount=0;
			   PIT32_Stop();
			 }
			 else
			 {

			 }
		 }	
#else 
		
#endif
	/***************************************************************/
	}
}

void PIT1_IRQHandler(void)
{
  tick_count++;
	PIT32_ISR(PIT1);
}

void PIT2_IRQHandler(void)
{
	PIT32_ISR(PIT2);
}

void PIT_en(PIT32_TypeDef *PITx)
{
	PITx->PCSR |= PCSR_EN;
}
void PIT_dis(PIT32_TypeDef *PITx)
{
	PITx->PCSR &=~PCSR_EN;
}
/*******************************************************************************
* Function Name  : PIT32_Init
* Description    : PIT��ʼ��
* Input          : - Prescaler��PITʱ�ӷ�Ƶֵ
*                  - PMRReloadCounterVal:PIT����������ʼֵ
*
* Output         : None
* Return         : None
******************************************************************************/
void PIT32_Init(PIT32_TypeDef *PITx,PIT32_CLKDIV Prescaler, UINT32 PMRReloadCounterVal,BOOL pitINT)
{
	//ModuleClk_On(MODULE_CLK_PIT1_IPG_CLK);
		if(pitINT == TRUE)
	{
		if(PITx==PIT1)
			NVIC_Init(3, 3, PIT1_IRQn, 2);
		else if(PITx==PIT2)
			NVIC_Init(3, 3, PIT2_IRQn, 2);
	}
	PITx->PCSR &= (~PCSR_EN);
	if(pitINT == TRUE)
	{
		PITx->PCSR = (Prescaler<<8)|PCSR_OVW|PCSR_PIE|PCSR_RLD|PCSR_PDBG; //�ж�ģʽ
	}
	else
	{
		PITx->PCSR = (Prescaler<<8)|PCSR_OVW|PCSR_RLD;    //��ͨģʽ
	}
  PITx->PMR = PMRReloadCounterVal;
	PITx->PCSR |= PCSR_EN;
}

/*******************************************************************************
* Function Name  : PIT32_Start
* Description    : PIT����
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void PIT32_Start(PIT32_TypeDef *PITx)
{
	PITx->PCSR |= PCSR_EN;
}

/*******************************************************************************
* Function Name  : PIT32_Stop
* Description    : PIT�ر�
* Input          : -None
*
* Output         : None
* Return         : None
******************************************************************************/
void PIT32_Stop(PIT32_TypeDef *PITx)
{
	PITx->PCSR &= ~PCSR_EN;
}


/*******************************************************************************
* Function Name  : PIT32_Cmd
* Description    : PITʹ�ܿ���
* Input          : - NewState��new state of the specified PIT.
*                              This parameter can be: ENABLE or DISABLE.
*
* Output         : None
* Return         : None
******************************************************************************/
void PIT32_Cmd(PIT32_TypeDef *PITx,FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		/* Enable the PIT */
		PITx->PCSR |= PCSR_EN;
	}
	else
	{
		/* Disable the PIT */
		PITx->PCSR &= ~PCSR_EN;
	}
}
/*******************************************************************************
* Function Name  : PIT32_CmdPIT_ITConfig
* Description    : PIT�ж�ʹ�ܿ���
* Input          : - NewState��new state of the specified PIT interrupts.
*                              This parameter can be: ENABLE or DISABLE.
*
* Output         : None
* Return         : None
******************************************************************************/
void PIT32_ITConfig(PIT32_TypeDef *PITx,FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		/* Enable the Interrupt source		s */
		PITx->PCSR |= PCSR_PIE;
	}
	else
	{
		/* Disable the Interrupt sources */
		PITx->PCSR &= (UINT16)~PCSR_PIE;
	}
}

/*******************************************************************************
* Function Name  : PIT32_ReadCNTR
* Description    : ��ȡPIT����ֵ
* Input          : None
*
* Output         : ����ֵ
* Return         : ����ֵ
******************************************************************************/
UINT32 PIT32_ReadCNTR(PIT32_TypeDef *PITx)
{
	return PITx->PCNTR;
}

/*******************************************************************************
* Function Name  : Time_Init
* Description    : PIT��ʼ��
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void Time_Init(void)
{
	//ʹ��PIT32_0��Ϊdelay��׼,1msһ���ж�
	U32 delay_data;
	delay_data=(g_ips_clk)/1000000*1000*2;
	PIT32_Init(PIT1,PIT32_CLK_DIV_1,delay_data,TRUE);
//	PIT32_Cmd(ENABLE);
//	PIT32_ITConfig(ENABLE);
//	PIT32_Start();	
}
