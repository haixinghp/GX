/*
 * PIT.c
 *
 *  Created on: 2016Äê9ÔÂ20ÈÕ
 *      
 */
#include "PIT.h"

void PIT_ControlConfig(PIT_t *pit, PIT_ControlConfig_t *config)
{
	pit->PCSR = (((U16)config->PreScaler & 0xF) << 8)  |
			    ((config->Stop_In_Doze) ? 0x0040 : 0x0000) |
			    ((config->Stop_In_Debug) ? 0x0020 : 0x0000) |
			    ((config->Overwrite) ? 0x0010 : 0x0000) |
			    ((config->Reload) ? 0x0002 : 0x0000) |
			    (pit->PCSR & 0x000D);
}

void PIT_SetPRE(PIT_t *pit, U8 pre)
{
	pit->PCSR = (pit->PCSR & 0xF0FF) | ((U16)pre << 8);
}

void PIT_EnableInterrupt(PIT_t *pit)
{
	pit->PCSR |= 0x0008;
}

void PIT_DisableInterrupt(PIT_t *pit)
{
	pit->PCSR &= ~0x0008;
}

void PIT_ClearInterrupt(PIT_t *pit)
{
	pit->PCSR |= 0x0004;
}

U8 PIT_IsInterrupt(PIT_t *pit)
{
	return ((pit->PCSR >> 2) & 1);
}

void PIT_Enable(PIT_t *pit)
{
	pit->PCSR |= 0x0001;
}
void PIT_Disable(PIT_t *pit)
{
	pit->PCSR &= ~0x0001;
}

void PIT_WritePMR(PIT_t *pit, U16 pmr)
{
	pit->PMR = pmr;
}

U16 PIT_GetCNT(PIT_t *pit)
{
	return pit->PCNTR;
}
