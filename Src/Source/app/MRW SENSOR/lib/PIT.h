/*
 * PIT.h
 *
 *  Created on: 2016Äê9ÔÂ20ÈÕ
 *      
 */

#ifndef PIT_H_
#define PIT_H_
#include "CCM_Types.h"

typedef struct{
	VU16 PCSR;          //PIT Control and Status Register
	VU16 RSVD;
	VU32 PMR;           //PIT Modulus Register
	VU32 PCNTR;         //PIT Count Register
	VU32 Reserved_06;
}PIT_t;

typedef struct{
	U8 PreScaler;
	U8 Stop_In_Doze;
	U8 Stop_In_Debug;
	U8 Overwrite;
	U8 Reload;
}PIT_ControlConfig_t;


void PIT_ControlConfig(PIT_t *pit, PIT_ControlConfig_t *config);
void PIT_SetPRE(PIT_t *pit, U8 pre);
void PIT_EnableInterrupt(PIT_t *pit);
void PIT_DisableInterrupt(PIT_t *pit);
void PIT_ClearInterrupt(PIT_t *pit);
U8 PIT_IsInterrupt(PIT_t *pit);
void PIT_Enable(PIT_t *pit);
void PIT_Disable(PIT_t *pit);
void PIT_WritePMR(PIT_t *pit, U16 pmr);
U16 PIT_GetCNT(PIT_t *pit);

#endif /* PIT_H_ */
