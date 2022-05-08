// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : edma_reg.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef EDMA_REG_H_
#define EDMA_REG_H_

#include "type.h"
#include "memmap.h"
typedef struct
{
	__IO unsigned int EDMACCR;//0x00@0x40
	__IO unsigned int EDMACCSR;//0x04@0x44
	__IO unsigned int EDMACRBAR;//0x08@0x48
	__IO unsigned int EDMACWBAR;//0x0c@0x4c
	__IO unsigned int EDMACMINSUMR;//0x10@0x50
	__IO unsigned int EDMACMINCNTR;//0x14@0x54
	__IO unsigned int EDMACMAJSUMR;//0x18@0x58
	__IO unsigned int EDMACMAJCNTR;//0x1c@0x5c
	__IO unsigned int EDMACSPAR;   //0x20@0x60

}EDMA_TypeDef;
#define PN0		0x00000000	//SPI1
#define PN1		0x00000100	//SPI2
#define PN2		0x00000200	//USI1
#define PN3		0x00000300	//USI2
#define PN4		0x00000400	//USI3
#define PN5		0x00000500	//AES
#define PN6		0x00000600  //DES
#define PN7		0x00000700  //SCB2
#define PN8		0x00000800  //SCI1
#define PN9		0x00000900  //SMS4
#define PN10  0x00000a00  //SHA
#define PN11	0x00000b00	//SSF33
#define PN12	0x00000c00	//RSA
#define PN13	0x00000d00	//SCI2
#define PN15	0x00000f00	//ZUC
#define START0	0x00010004
#define START1	0x00010020
#define TTYPE0	0x00000000	//sram to sram
#define TTYPE1	0x00000020	//peripheral to sram
#define TTYPE2	0x00000040	//sram to peripheral
#define TTYPE3	0x00000060	//sram to peripheral then peripheral to sram

#define EW_EN  0x00010000
#define	CHANNEL_VALID			0x00000002
#define PRELOAD_6		(6<<2)
#define EDMA1_CH0_DONE  0x02
//edmac
#define EDMACCR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR)
#define EDMACCSR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x04)
#define EDMACRBAR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x08)
#define EDMACWBAR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x0c)
#define EDMACMINSUMR0	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x10)
#define EDMACMINCNTR0	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x14)
#define EDMACMAJSUMR0	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x18)
#define EDMACMAJCNTR0	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x1c)
#define EDMACSPAR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x20)
#define EDMACRBARSTEP0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x24)
#define EDMACWBARSTEP0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x28)
#define EDMACLASTMINSUMR0		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x2c)

#define EDMACCR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x40)
#define EDMACCSR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x44)
#define EDMACRBAR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x48)
#define EDMACWBAR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x4c)
#define EDMACMINSUMR1	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x50)
#define EDMACMINCNTR1	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x54)
#define EDMACMAJSUMR1	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x58)
#define EDMACMAJCNTR1	*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x5c)
#define EDMACSPAR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x60)
#define EDMACLASTMINSUMR1		*(volatile unsigned long *)(EDMAC0_BASE_ADDR + 0x6c)
//EDMAC Control State Register (EDMACCSR)
#define EDMAC_MINORDONE_MASK			(0x00000002)	//MINOR DONE
#define EDMAC_EDMAEN_MASK				(0x00010000)	//EDMAEN
#define EDMAC_MAJORDONE_MASK			(0x00000008)	//MAJOR DONE


#define EDMAC_CHVALID_MASK				(0x00000002)
#define EDMAC_MINDONEIE_MASK			(0x00000001)
#define EDMAC_SPI_HW_MASK				(0x40000000)


#endif /* EDMA_REG_H_ */
