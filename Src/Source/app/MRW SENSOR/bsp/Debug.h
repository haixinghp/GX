/*
 * Debug.h
 *
 *  Created on: 2016��9��14��
 *      
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include "CCM_MEM.h"
#include "uart.h"



extern  volatile USARTypDef	Debug_Tag ;


void debugInit(U32 baudrate);
void debugPrint(const char *fmt, ...);
U8   debugReadByte(S32 *ch);


#endif /* DEBUG_H_ */
