/*
 * ClockAndPower.c
 *
 *  Created on: 2016年9月14日
 *      
 */
#include "ClockAndPower.h"
#include "Cache.h"
#include "clk_switch.h"
//#include "cpm_drv.h"
//#include "cpm_drv.h"
#include "CPM.h"
#include "sys.h"
#include "cache.h"

void CPM_SysClkSelect(U32 sys_sel, U32 div);
void CPM_Vref_Trim(unsigned int trim_value);
static void cpm_set_sysclk_div(U32 div);
UINT32 IPS_Clk_Op(UINT32 div_op, UINT32 op_data);
UINT32 Get_Sys_Clock(void);

void clockInit(void)
{
//trim 160M的时钟
#if SYS_CLK_TRIM_108M
	CPM_OSC_Switch(OSC_108M_HZ);		
#endif
#if SYS_CLK_TRIM_120M
	CPM_OSC_Switch(OSC_120M_HZ);
#elif SYS_CLK_TRIM_150M
	CPM_OSC_Switch(OSC_150M_HZ);
#elif SYS_CLK_TRIM_160M
	CPM_OSC_Switch(OSC_160M_HZ); //系统时钟160M，需2分频
#endif
	
	//配置系统时钟
	CPM_SysClkSelect(SYSCLK_OSC160M, 1);
	
	//配置IPS时钟的分频系数不可以为0
	IPS_Clk_Op(UPDATA_DIVIDER, 1); 
	
	// get system clock
	g_sys_clk = Get_Sys_Clock();	
	
	//get ips clock
	g_ips_clk = g_sys_clk / (IPS_Clk_Op(GET_NOW_DIVIDER, 0) + 1);	

	CPM.SYSCGTCR = DMAC1_GTE
									| DMAC2_GTE
									| CRC0_GTE
									| CRC1_GTE
									| AHB2_MUX_GTE
									| SRAMD_GTE
									| SRAM0_GTE
									| SRAM1_GTE
									| SRAM2_GTE
									| SRAM3_GTE
									| SSI4_GTE
									| SSI5_GTE
									| ROM_GTE
//									| M2S_BUS_M_GTE
									;
//
	CPM.MULTICGTCR = EFM_BUS_CGT
//										| MCC_CGT
//										| MCCADR_CGT
										| ADC_CGT
										| MESH_CGT
										| TC_CGT
										| CLKOUT_CGT
										| KEY_CTRL_CGT
										| EFM_CGT
										| CPM_CGT
										| EPORT_CGT
										| EPORT1_CGT
//										| EPORT2_CGT
//										| EPORT3_CGT
//										| EPORT4_CGT
//										| TRACE_CGT
										;

	CPM.ARITHCGTCR=0;
	CPM.IPSCGTCR= IO_CTRL_GTE
								| WDT_GTE
								| RTC_GTE
								| PIT1_GTE
								| PIT2_GTE
								| USI1_GTE
								| EDMAC1_GTE
								| SPI1_GTE
								| SPI2_GTE
								| SPI3_GTE
								| SCI1_GTE
								| SCI2_GTE
								| USI2_GTE
								| I2C1_GTE
								| PWM0_GTE
								| I2C2_GTE
								| I2C3_GTE
								| SCI3_GTE
								| QADC_GTE
								| DAC_GTE
//								| MCC_GTE
								| TSI_GTE
								| LD_GTE
								| TRNG_GTE
								| PGD_GTE
								| SEC_DET_APB
//								| PCI_GTE
//								| PMURTC_GTE
								| AHB2IPS_GTE
								| RIMS_APB;
}


void powerEnterSleep(void)
{
	//还未实现
}

static void delay(volatile U32 val)
{
	while(val--);
}


void clockSet40M(void)
{

}


void jmp_to_rom(void)
{
	__asm("CPSID I");//disable exception & interrupt
//	cpm_sysclk_select(SYSCLK_OSC400M);//select 400MHz OSC
	 clockSet40M();//sysclk == 40M
#if ( CACHE_ENABLE	==	(1) )
	disableCache();
#endif
	__asm("b 0x100");//jmp to rom
}


/******************************************************************************
* Function Name  : cpm_set_sysclk_div
* Description    : 设置OSC120M分频作为系统时钟
* Input          :  div： the divide value of OSC120M_CLK
*
* Output         : None
* Return         : None
******************************************************************************/
static void cpm_set_sysclk_div(U32 div)
{
	CPM.SCDIVR &= ~(SYS_DIV_MASK);
	CPM.SCDIVR |= (div << SYS_DIV_SHIFT);
	
	CPM.CDIVUPDR |= SYSDIV_UPD;
}






