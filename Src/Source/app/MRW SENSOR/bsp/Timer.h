/*
 * Timer.h
 *
 *  Created on: 2016Äê9ÔÂ14ÈÕ
 *      Author: Jason
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "CCM_MEM.h"

extern void timerInit(void);
extern void timerRestart(void);
extern U32  timerElapsedUs(void);
extern void timerDelayUs(U32 us);
extern void timerDelayMs(U32 ms);
extern U32 systemTickMs(void);

#endif /* TIMEOUT_H_ */
