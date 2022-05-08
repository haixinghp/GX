/*
 * cache.c
 *
 *  Created on: 2017Äê2ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "Cache.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 check_target_cache_attr
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Description:
//    check if target address is cached.
//
// Input:
//    the queried start address.
//
// Output:
//    target address cache attribute.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cache_com check_target_cache_attr(UINT32 addr)
{
	cache_com retVal=cacheOff;
//	int region, section, regionShift, found;
//	UINT32 highRegVal, lowRegVal, baseAddr;

	if( (DPCCCR & ENCACHE)== 0x00)
	{
		retVal=cacheOff;
	}
	else
	{
			if(IPCCRGS_H & 0x000000AA)
			{
				retVal = cacheThrough;
			}
	}

	return retVal;
}

void invalid_page_DCache(UINT32 addr)
{
	DPCRINVPAGEADDR = addr;
	DPCRINVPAGESIZE = 0x200 | 0x1;
	
	while((DPCRINVPAGESIZE & 0x1));		//cleard if invalidate completely.
}

void disableCache(void)
{
	//sync DCache
	DPCCCR = GO | PUSHW1 | PUSHW0;
	while(((DPCCCR) & 0x80000000) == 0x80000000);
	
	//sync ICache
	IPCCCR = GO | PUSHW1 | PUSHW0;
	while(((IPCCCR) & 0x80000000) == 0x80000000);
}

