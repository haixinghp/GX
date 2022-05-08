#include "Timer.h"
#include "type.h"
#include "ccm4202s.h"
#include "sys.h"

extern UINT32 g_ips_clk;
volatile U32 tick_count = 0;

static volatile U32 start_int = 0;
static volatile U32 start_frac = 0;



void timerInit(void)
{
	PIT_ControlConfig_t config;
	U16 pmr = (g_ips_clk/ 1000/4);

	config.PreScaler = 2;
	config.Stop_In_Doze = 0;
	config.Stop_In_Debug = 0;
	config.Overwrite = 1;
	config.Reload = 1;

	PIT_ControlConfig(&PIT1, &config);
	PIT_ControlConfig(&PIT2, &config);

	PIT_WritePMR(&PIT1, pmr);
	PIT_EnableInterrupt(&PIT1);
	PIT_Enable(&PIT1);

	PIT_DisableInterrupt(&PIT2);
	
	NVIC_Init(3, 3, PIT1_IRQn, 2);
	NVIC_Init(3, 3, PIT1_IRQn, 2);

	__asm("CPSID I");
	tick_count = 0;
	start_int = 0;
	start_frac = 0;
	__asm("CPSIE I");
}


void timerISR(void)
{
	if (PIT_IsInterrupt(&PIT1)){
		PIT_ClearInterrupt(&PIT1);
		tick_count++;
	}else if (PIT_IsInterrupt(&PIT2)){
		PIT_ClearInterrupt(&PIT2);
	}
}


void timerRestart(void)
{
	start_frac = PIT_GetCNT(&PIT1);
	start_int = tick_count;
}

U32  timerElapsedUs(void)
{
	U32 result = 0x00;
	U32 end_frac = PIT_GetCNT(&PIT1);
	U32 end_int = tick_count;
	S32 tmp     = 0;
	S32 diff_frac =  (S32)start_frac - (S32)end_frac;

	tmp = (S32)(g_ips_clk / (1000 * 1000));
	tmp = (S32)(diff_frac * (S32)4 / tmp );
	
	result = (((end_int - start_int) * 1000) + tmp);
	/*if (diff_frac<0){
		printf("end_int:%d\r\n",end_int);
		printf("diff_frac:%d\r\n",diff_frac);
		printf("end_frac:%d\r\n",end_frac);
		printf("start_frac:%d\r\n",start_frac);
		printf("start_int:%d\r\n",start_int);
		printf("result:%d\r\n",result);
		printf("g_ips_clk:%d\r\n",g_ips_clk);
	}*/
	return result;
}


void timerDelayUs(U32 us)
{
	U32 pmr = us * (SYS_CLOCK / (1000 * 1000)) / 4;

	PIT_SetPRE(&PIT2, 1);
	PIT_WritePMR(&PIT2, pmr);
	PIT_Enable(&PIT2);
	while(!PIT_IsInterrupt(&PIT2));
	PIT_ClearInterrupt(&PIT2);
	PIT_Disable(&PIT2);
}


void timerDelayMs(U32 ms)
{
	U32 pmr = ms * (g_ips_clk / 1000) / 4096;

	PIT_SetPRE(&PIT2, 12);
	PIT_WritePMR(&PIT2, pmr);
	PIT_Enable(&PIT2);
	while(!PIT_IsInterrupt(&PIT2));
	PIT_ClearInterrupt(&PIT2);
	PIT_Disable(&PIT2);
}


U32 systemTickMs(void)
{
	return tick_count;
}

