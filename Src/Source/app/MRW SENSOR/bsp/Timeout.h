/*
 * Timeout.h
 *
 *  Created on: 2016��9��14��
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
