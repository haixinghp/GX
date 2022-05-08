/*
 * Timeout.h
 *
 *  Created on: 2016Äê9ÔÂ14ÈÕ
 *      
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_
#include "CCM_MEM.h"

void timeoutInit(void);
void timeoutStart(void);
void timeoutStop(void);
U32  timeoutElapsed(void);

#endif /* TIMEOUT_H_ */
