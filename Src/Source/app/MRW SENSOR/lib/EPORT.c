/*
 * EPORT.c
 *
 *  Created on: 2016Äê9ÔÂ22ÈÕ
 *      
 */
#include "EPORT.h"

void EPORT_PinControlConfig(EPORT_t *eport, U8 pin, EPORT_ControlConfig_t *config)
{
	U8 shift;

	pin &= 0x7;
	shift = pin << 1;

	switch(config->TriggerMode){
	case EPORT_HIGH_LEVEL_TRIGGER:
		eport->EPPAR = (eport->EPPAR & (~(3 << shift)));
		eport->EPLPR |= (1 << pin);
		break;
	case EPORT_LOW_LEVEL_TRIGGER:
		eport->EPPAR = (eport->EPPAR & (~(3 << shift)));
		eport->EPLPR &= ~(1 << pin);
		break;
	case EPORT_RISING_EDGE_TRIGGER:
		eport->EPPAR = (eport->EPPAR & (~(3 << shift))) | (1 << shift);
		break;
	case EPORT_FALLING_EDGE_TRIGGER:
		eport->EPPAR = (eport->EPPAR & (~(3 << shift))) | (2 << shift);
		break;
	case EPORT_BOTH_EDGE_TRIGGER:
		eport->EPPAR = (eport->EPPAR & (~(3 << shift))) | (3 << shift);
		break;
	}

	if (config->DirectionOutput){
		eport->EPDDR |= (1 << pin);
	}else{
		eport->EPDDR &= ~(1 << pin);
	}

	if (config->EnablePullup){
		eport->EPPUER |= (1 << pin);
	}else{
		eport->EPPUER &= ~(1 << pin);
	}

	if (config->OpenDrain){
		eport->EPODER |= (1 << pin);
	}else{
		eport->EPODER &= ~(1 << pin);
	}
}

void EPORT_EnableInterrupt(EPORT_t *eport, U8 pin)
{
	eport->EPIER |= (1 << (pin & 0x7));
}

void EPORT_DisableInterrupt(EPORT_t *eport, U8 pin)
{
	eport->EPIER &= ~(1 << (pin & 0x7));
}

void EPORT_ClearInterrupt(EPORT_t *eport, U8 pin)
{
	eport->EPFR |= (1 << (pin & 0x7));
}

U8 EPORT_IsInterrupt(EPORT_t *eport, U8 pin)
{
	return ((eport->EPFR >> (pin & 0x7)) & 1);
}

U8 EPORT_ReadPort(EPORT_t *eport)
{
	return eport->EPPDR;
}

void EPORT_WritePort(EPORT_t *eport, U8 port_level)
{
	eport->EPDR = port_level;
}

U8 EPORT_ReadPin(EPORT_t *eport, U8 pin)
{
	return ((eport->EPPDR >> (pin & 0x7)) & 1);
}

void EPORT_WritePin(EPORT_t *eport, U8 pin, U8 level)
{
	if (level){
		eport->EPDR |= (1 << (pin & 0x7));
	}else{
		eport->EPDR &= ~(1 << (pin & 0x7));
	}
}
