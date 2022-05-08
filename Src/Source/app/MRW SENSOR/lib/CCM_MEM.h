/*
 *
 *
 *  Created on: 2016年9月14日
 *      
 */

#ifndef CCM_MEM_H_
#define CCM_MEM_H_
#include "CCM_Types.h"
//#include "cpm_drv.h"
#include "CPM.h"
#include "SCI.h"
#include "SPI.h"
#include "EDMA.h"
#include "PIT.h"
#include "eflash_api.h"
#include "EPORT.h"

//#define SYS_CLOCK      (96 * 1000 * 1000)
#define SYS_CLOCK      (80 * 1000 * 1000)

#define CPM    (*(CPM_t   *)(0x40004000))

#define SCI1   (*(SCI_t   *)(0x40013000))
#define SCI2   (*(SCI_t   *)(0x40014000))

#define SPI1   (*(SPI_t   *)(0x40010000))
#define SPI2   (*(SPI_t   *)(0x40011000))
#define SPI3   (*(SPI_t   *)(0x40012000))

#define EDMA   (*(EDMAC_t *)(0x4000a000))

#define PIT1   (*(PIT_t   *)(0x40007000))
#define PIT2   (*(PIT_t   *)(0x40008000))

#define EFLASH (*(EFLASH_t*)(0x40003800))

#define EPORT  (*(EPORT_t *)(0x40019000))

#define CACHE_ENABLE		(1)//use cache

#define SPI_EDMA_ENABLE		(1)//spi use edma

#define CCM    (*(CCM_t *))(0x40001000)

//#define EA_IER  (*((VU32 *)(0xE0000010)))                //总中断控制器

#endif /* CCM3403_H_ */
