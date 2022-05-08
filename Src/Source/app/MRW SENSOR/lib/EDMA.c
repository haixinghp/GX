/*
 * EDMA.c
 *
 *  Created on: 2016Äê9ÔÂ19ÈÕ
 *      
 */
#include "EDMA.h"

void EDMAC_ControlConfig(EDMAC_t *dma, EDMAC_CH_t ch, EDMAC_ControlConfig_t *config)
{
	switch(ch){
	case EDMAC_CHANNEL0:
		dma->MINISUMR0 = config->Transfer_Num;
		dma->CCR0 =((config->Compare_Enable)  ? 0x8000 : 0x0000) |
		           ((config->Compare_Skip)    ? 0x4000 : 0x0000) |
		           ((config->Preload_Disable) ? 0x2000 : 0x0000) |
		           ((config->Peripheral_Num & 0xF) << 8)         |
		           ((config->Transfer_Type & 0x3) << 5)          |
		           0x00000002                                    |
		           (dma->CCR0 & 0x00000081);
		dma->RBAR0 = ((config->Out_Buff_Inc_Disable) ? 0x80000000 : 0) |
				     ((U32)config->Out_Buff & 0x0003FFFF);
		dma->WBAR0 = ((config->In_Buff_Inc_Disable)  ? 0x80000000 : 0) |
				     ((U32)config->In_Buff  & 0x0003FFFF);
		dma->CSR0 |= 0x06;
		break;
	case EDMAC_CHANNEL1:
		dma->MINISUMR1 = config->Transfer_Num;
		dma->CCR1 =((config->Compare_Enable)  ? 0x8000 : 0x0000) |
				   ((config->Compare_Skip)    ? 0x4000 : 0x0000) |
				   ((config->Preload_Disable) ? 0x2000 : 0x0000) |
				   ((config->Peripheral_Num & 0xF) << 8)         |
				   ((config->Transfer_Type & 0x3) << 5)          |
				   0x00000002                                    |
				   (dma->CCR1 & 0x00000081);
		dma->RBAR1 = ((config->Out_Buff_Inc_Disable) ? 0x80000000 : 0) |
					 ((U32)config->Out_Buff & 0x0003FFFF);
		dma->WBAR1 = ((config->In_Buff_Inc_Disable)  ? 0x80000000 : 0) |
					 ((U32)config->In_Buff  & 0x0003FFFF);
		dma->CSR1 |= 0x30;
		break;
	}
}

void EDMAC_EnableInterrupt(EDMAC_t *dma, EDMAC_CH_t ch, U8 intp)
{
	switch(ch){
	case EDMAC_CHANNEL0:
		dma->CCR0 |= intp;
		break;
	case EDMAC_CHANNEL1:
		dma->CCR1 |= intp;
		break;
	}
}

void EDMAC_DisableInterrupt(EDMAC_t *dma, EDMAC_CH_t ch, U8 intp)
{
	switch(ch){
	case EDMAC_CHANNEL0:
		dma->CCR0 &= ~intp;
		break;
	case EDMAC_CHANNEL1:
		dma->CCR1 &= ~intp;
		break;
	}
}

void EDMAC_Enable(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		dma->CSR0  |= 0x00010000;
		break;
	case EDMAC_CHANNEL1:
		dma->CSR1  |= 0x00010000;
		break;
	}
}
void EDMAC_Disable(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		dma->CSR0  &= ~0x00010000;
		break;
	case EDMAC_CHANNEL1:
		dma->CSR1  &= ~0x00010000;
		break;
	}
}

U8 EDMAC_ISFail(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 15) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 15) & 1);
	default:
		return 0;
	}
}

U8 EDMAC_IsStart(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 2) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 2) & 1);
	default:
		return 0;
	}
}

U8 EDMAC_IsDone(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 1) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 1) & 1);
	default:
		return 0;
	}
}

U8 EDMAC_IsBusy(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 0) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 0) & 1);
	default:
		return 0;
	}
}

void EDMAC_ClearFail(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		dma->CSR0 |= (1 << 15);
		break;
	case EDMAC_CHANNEL1:
		dma->CSR1 |= (1 << 15);
		break;
	}
}

void EDMAC_ClearStart(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		dma->CSR0 |= (1 << 2);
		break;
	case EDMAC_CHANNEL1:
		dma->CSR1 |= (1 << 2);
		break;
	}
}
void EDMAC_ClearDone(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		dma->CSR0 |= (1 << 1);
		break;
	case EDMAC_CHANNEL1:
		dma->CSR1 |= (1 << 1);
		break;
	}}

U8 EDMAC_LastStartChannel(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 7) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 7) & 1);
	default:
		return 0;
	}
}

U8 EDMAC_LastDoneChannel(EDMAC_t *dma, EDMAC_CH_t ch)
{
	switch(ch)
	{
	case EDMAC_CHANNEL0:
		return ((dma->CSR0 >> 6) & 1);
	case EDMAC_CHANNEL1:
		return ((dma->CSR1 >> 6) & 1);
	default:
		return 0;
	}
}

U16 EDMAC_GetCount(EDMAC_t *dma)
{
	return (dma->MINICNTR0 & 0xFFFFFFFFUL);
}

