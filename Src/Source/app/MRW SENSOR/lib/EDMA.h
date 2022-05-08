/*
 * EDMA.h
 *
 *  Created on: 2016Äê9ÔÂ19ÈÕ
 *      
 */

#ifndef EDMA_H_
#define EDMA_H_
#include "CCM_Types.h"

typedef struct{
	U32 CCR0;            //EDMAC Control Register of CHannel 0
	U32 CSR0;            //EDMAC Control State Register
	U32 RBAR0;          //EDMAC Read Buff Address Register of Channel 0
	U32 WBAR0;          //EDMAC Write Buffer Address Register of Channel 0
	U32 MINISUMR0;		//EDMAC MINOR SUM Counter Register of Channel 0
	U32 MINICNTR0;		//EDMAC MINOR Counter Register of Channel 0
	U32 MAJSUMR0;		//EDMAC MAJOR SUM Counter Register of Channel 0
	U32 MAJCNTR0;		//EDMAC MAJOR Counter Register of Channel 0
	U16 SPAR0;          //EDMAC Special Peripheral Address Register of Channel 0
	U16 U16RESV1;
	U32 U32RESV1;
	U32 U32RESV2;
	U32 U32RESV3;
	U32 U32RESV4;
	U32 U32RESV5;
	U32 U32RESV6;
	U32 U32RESV7;
	U32 CCR1;            //EDMAC Control Register of CHannel 1
	U32 CSR1;            //EDMAC Control State Register
	U32 RBAR1;          //EDMAC Read Buff Address Register of Channel 1
	U32 WBAR1;          //EDMAC Write Buffer Address Register of Channel 1
	U32 MINISUMR1;		//EDMAC MINOR SUM Counter Register of Channel 1
	U32 MINICNTR1;		//EDMAC MINOR Counter Register of Channel 1
	U32 MAJSUMR1;		//EDMAC MAJOR SUM Counter Register of Channel 1
	U32 MAJCNTR1;		//EDMAC MAJOR Counter Register of Channel 1
	U16 SPAR1;          //EDMAC Special Peripheral Address Register of Channel 1

}EDMAC_t;

typedef enum{
	EDMAC_CHANNEL0 = 0,
	EDMAC_CHANNEL1 = 1
}EDMAC_CH_t;

#define EDMAC_TTYPE_SRAM_TO_SRAM         0
#define EDMAC_TTYPE_PERIPHERAL_TO_SRAM   1
#define EDMAC_TTYPE_SRAM_TO_PERIPHERAL   2
#define EDMAC_TTYPE_SRAM_PERI_TO_SRAM    3

#define EDMAC_PN_AES    5
#define EDMAC_PN_DES    6
#define EDMAC_PN_SMS4   9
#define EDMAC_PN_SHA    10
#define EDMAC_PN_RSA    12

#define EDMAC_PN_SPI1   0
#define EDMAC_PN_SPI2   1
#define EDMAC_PN_USI1   2
#define EDMAC_PN_USI2   3
#define EDMAC_PN_SCI1   8
#define EDMAC_PN_SCI3   11
#define EDMAC_PN_SCI2   13
#define EDMAC_PN_SPI3   14

#define EDMAC_INTERRUPT_START   0x80
#define EDMAC_INTERRUPT_DONE    0x01

typedef struct{
	U16 Transfer_Num;
	U8 Compare_Enable;
	U8 Compare_Skip;
	U8 Preload_Disable;
	U8 Transfer_Type;
	U8 Peripheral_Num;
	U8 Out_Buff_Inc_Disable;
	U8 In_Buff_Inc_Disable;
	U8 *Out_Buff;
	U8 *In_Buff;
}EDMAC_ControlConfig_t;

void EDMAC_ControlConfig(EDMAC_t *dma, EDMAC_CH_t ch, EDMAC_ControlConfig_t *config);
void EDMAC_EnableInterrupt(EDMAC_t *dma, EDMAC_CH_t ch, U8 intp);
void EDMAC_DisableInterrupt(EDMAC_t *dma, EDMAC_CH_t ch, U8 intp);
void EDMAC_Enable(EDMAC_t *dma, EDMAC_CH_t ch);
void EDMAC_Disable(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_ISFail(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_IsStart(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_IsDone(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_IsBusy(EDMAC_t *dma, EDMAC_CH_t ch);
void EDMAC_ClearFail(EDMAC_t *dma, EDMAC_CH_t ch);
void EDMAC_ClearStart(EDMAC_t *dma, EDMAC_CH_t ch);
void EDMAC_ClearDone(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_LastStartChannel(EDMAC_t *dma, EDMAC_CH_t ch);
U8 EDMAC_LastDoneChannel(EDMAC_t *dma, EDMAC_CH_t ch);
U16 EDMAC_GetCount(EDMAC_t *dma);


#endif /* EDMA_H_ */
