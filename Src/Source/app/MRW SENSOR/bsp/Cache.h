/*
 * cache.h
 *
 *  Created on: 2017Äê2ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef CACHE_H_
#define CACHE_H_

#include "CCM_MEM.h"
#include "type.h"

#define BIT(x)  ((U32)1 << (x))

#define CACHE_BASE_ADDR         (0x40051000)
#define CACHE2_BASE_ADDR        (0x40055000)

#define ICACHE		    ((CACHE_TypeDef *)(CACHE_BASE_ADDR))
#define DCACHE		    ((CACHE_TypeDef *)(CACHE2_BASE_ADDR))

typedef enum{
	cacheOff = 0,
	cacheThrough,
	cacheBack,
}cache_com;


typedef struct
{
	__IO UINT32 CACHE_CCR;//0x00
	__IO UINT32 CACHE_CLCR;//0x04
	__IO UINT32 CACHE_CSAR;//0x08
	__IO UINT32 CACHE_CCVR;//0x0c
	__IO UINT32 RESERVED1[4];//0x10~0x1C
	__IO UINT32 CACHE_ACRG;//0x20

}CACHE_TypeDef;

#define IPCCCR  *(volatile UINT32*)(CACHE_BASE_ADDR +0x0)
#define IPCCLCR *(volatile UINT32*)(CACHE_BASE_ADDR +0x4)
#define IPCCSAR *(volatile UINT32*)(CACHE_BASE_ADDR +0x8)
#define IPCCCVR *(volatile UINT32*)(CACHE_BASE_ADDR +0xc)
#define IPCCSRR *(volatile UINT32*)(CACHE_BASE_ADDR +0x10)

#define IPCCRGS *(volatile UINT32*)(CACHE_BASE_ADDR +0x20)
#define IPCCRGS_H *(volatile UINT32*)(CACHE_BASE_ADDR +0x24)
	
#define IR2HIGHADDR		*(volatile UINT32 *)(CACHE_BASE_ADDR + 0x80)

#define IPCRINVPAGEADDR	*(volatile UINT32 *)(CACHE_BASE_ADDR + 0x180)
#define IPCRINVPAGESIZE	*(volatile UINT32 *)(CACHE_BASE_ADDR + 0x184)

#define IPSCCR  *(volatile UINT32*)(CACHE_BASE_ADDR +0x800)
#define IPSCLCR *(volatile UINT32*)(CACHE_BASE_ADDR +0x804)
#define IPSCSAR *(volatile UINT32*)(CACHE_BASE_ADDR +0x808)
#define IPSCCVR *(volatile UINT32*)(CACHE_BASE_ADDR +0x80c)
#define IPSCSRR *(volatile UINT32*)(CACHE_BASE_ADDR +0x810)

#define IPSCRGS *(volatile UINT32*)(CACHE_BASE_ADDR +0x820)
	
#define DPCCCR  *(volatile UINT32*)(CACHE2_BASE_ADDR +0x0)
#define DPCCLCR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x4)
#define DPCCSAR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x8)
#define DPCCCVR *(volatile UINT32*)(CACHE2_BASE_ADDR +0xc)
#define DPCCSRR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x10)

#define DPCCRGS *(volatile UINT32*)(CACHE2_BASE_ADDR +0x20)
#define DPCCRGS_H *(volatile UINT32*)(CACHE2_BASE_ADDR +0x24)
	
#define DR2HIGHADDR		*(volatile UINT32 *)(CACHE2_BASE_ADDR + 0x80)

#define DPCRINVPAGEADDR	*(volatile UINT32 *)(CACHE2_BASE_ADDR + 0x180)
#define DPCRINVPAGESIZE	*(volatile UINT32 *)(CACHE2_BASE_ADDR + 0x184)

#define DPSCCR  *(volatile UINT32*)(CACHE2_BASE_ADDR +0x800)
#define DPSCLCR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x804)
#define DPSCSAR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x808)
#define DPSCCVR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x80c)
#define DPSCSRR *(volatile UINT32*)(CACHE2_BASE_ADDR +0x810)

#define DPSCRGS *(volatile UINT32*)(CACHE2_BASE_ADDR +0x820)

#define PAGE_CACHE_CLEAN_GO		0x00000001

#define INVW1   (((UINT32)1)<<26)
#define INVW0   (((UINT32)1)<<24)
#define PUSHW0  (((UINT32)1)<<25)
#define PUSHW1  (((UINT32)1)<<27)
#define GO      (((UINT32)1)<<31)
#define ENWRBUF (((UINT32)1)<<1)
#define ENCACHE (((UINT32)1)<<0)

#define LGO     (0x01)

#define CACHE_LINE_SIZE  0x10
#define CACHE_LINE_MASK  0x0f

#define R0_WT_WB				(((UINT32)1)<<0)
#define R0_ENCACHE				(((UINT32)1)<<1)
#define R2_WT_WB				(((UINT32)1)<<4)
#define R2_ENCACHE				(((UINT32)1)<<5)
#define RAM0_WT_WB				(((UINT32)1)<<12)
#define RAM0_ENCACHE			(((UINT32)1)<<13)

#define WRITE_BACK				(0xFF)
#define WRITE_THROUGH			(0xaa)

#define EFLASH_WRITE_BACK			(0xff)
#define EFLASH_WRITE_THROUGH	(0xaa)

#define BOOT_CACHEOFF			0xff00ffff
#define ROM_CACHEOFF			0xfffcffff
//#define SPIM1_CACHEOFF		0xffffff3f
#define SPIM1_CACHEOFF		0xffff00ff
#define EFLASH_CACHEOFF		0xffffff00

#define BOOT_CACHE_SHIFT	(16)
#define ROM_CACHE_SHIFT		(16)
#define SPIM1_CACHE_SHIFT	(8)

#define CACHE_R2_TARGET_BASE_ADDR      (0x08000000)
#define CACHE_R6_TARGET_BASE_ADDR      (0x10000000)
#define CACHE_R7_TARGET_BASE_ADDR      (0x00000000)

void invalid_page_DCache(UINT32 addr);
void DCACHE_Init(cache_com spim1, cache_com eflash, cache_com rom, cache_com boot);
void ICACHE_Init(cache_com spim1, cache_com eflash, cache_com rom, cache_com boot);
cache_com check_target_cache_attr(UINT32 addr);
void disableCache(void);

#endif /* CACHE_H_ */
