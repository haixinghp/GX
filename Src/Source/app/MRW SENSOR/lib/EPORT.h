/*
 * EPORT.h
 *
 *  Created on: 2016Äê9ÔÂ22ÈÕ
 *      
 */

#ifndef EPORT_H_
#define EPORT_H_
#include "CCM_Types.h"

typedef struct{
	VU16 EPPAR;         //EPORT Pin Assignment Register
	VU8  EPIER;         //EPORT Interrupt Enable Register
	VU8  EPDDR;         //EPORT Data Direction Register
	VU8  EPPDR;         //EPORT Pin Data Register
	VU8  EPDR;          //EPORT Data Register
	VU8  EPPUER;        //EPORT Pin Pull-up enable Register
	VU8  EPFR;          //EPORT Flag Register
	VU8  EPODER;        //EPORT Open Drain Register
	VU8  EPLPR;         //EPORT Level Polarity Register
}EPORT_t;

typedef enum{
	EPORT_HIGH_LEVEL_TRIGGER,
	EPORT_LOW_LEVEL_TRIGGER,
	EPORT_RISING_EDGE_TRIGGER,
	EPORT_FALLING_EDGE_TRIGGER,
	EPORT_BOTH_EDGE_TRIGGER
}EPORT_TriggerMode_t;

typedef struct{
	EPORT_TriggerMode_t TriggerMode;
	U8 DirectionOutput;
	U8 EnablePullup;
	U8 OpenDrain;
}EPORT_ControlConfig_t;

void EPORT_PinControlConfig(EPORT_t *eport, U8 pin, EPORT_ControlConfig_t *config);
void EPORT_EnableInterrupt(EPORT_t *eport, U8 pin);
void EPORT_DisableInterrupt(EPORT_t *eport, U8 pin);
void EPORT_ClearInterrupt(EPORT_t *eport, U8 pin);
U8 EPORT_IsInterrupt(EPORT_t *eport, U8 pin);
U8 EPORT_ReadPort(EPORT_t *eport);
void EPORT_WritePort(EPORT_t *eport, U8 port_level);
U8 EPORT_ReadPin(EPORT_t *eport, U8 pin);
void EPORT_WritePin(EPORT_t *eport, U8 pin, U8 level);

#endif /* EPORT_H_ */
