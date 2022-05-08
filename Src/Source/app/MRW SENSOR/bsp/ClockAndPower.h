/*
 * ClockAndPower.h
 *
 *  Created on: 2016Äê9ÔÂ14ÈÕ
 *      
 */

#ifndef CLOCKANDPOWER_H_
#define CLOCKANDPOWER_H_
#include "CCM_MEM.h"

void clockInit(void);
void powerEnterSleep(void);
void clockSet40M(void);
void jmp_to_rom(void);


#endif /* CLOCKANDPOWER_H_ */
