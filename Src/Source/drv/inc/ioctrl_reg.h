
#ifndef __IOCTRL_REG_H__
#define __IOCTRL_REG_H__
#include "type.h"

typedef struct
{
	__IO UINT32 SPI_CONTROL_REG;	 					//0x00
	__IO UINT32 USI_CONTROL_REG;   					//0x04
	__IO UINT32 I2C_CONTROL_REG;   					//0x08
	__IO UINT32 SCI_CONTROL_REG;   					//0x0c
	__IO UINT32 GINTL_CONTROL_REG;  				//0x10
	__IO UINT32 GINTH_CONTROL_REG;  				//0x14
	__IO UINT32 RESERVED1;   								//0x18
	__IO UINT32 SWAP_CONTROL_REG;   				//0x1C
	__IO UINT32 SPIM1_CONTROL_REG;  				//0x20
	__IO UINT32 SPIM2_CONTROL_REG;  				//0x24
	__IO UINT32	RESERVED2;									//0x28
	__IO UINT32 GINT_31_30_CONTROL_REG;			//0x2c
	__IO UINT32	RESERVED3;									//0x30
	__IO UINT32	RESERVED4;									//0x34
	__IO UINT32	GINT_39_32_CONTROL_REG;			//0x38
	__IO UINT32	GINT_29_22_CONTROL_REG;			//0x3c
	__IO UINT32	RESERVED5;									//0x40
	__IO UINT32 CLKOUT_RSTOUT_CONTROL_REG;	//0x44
	__IO UINT32	RESERVED6;									//0x48
	__IO UINT32	RESERVED7;									//0x4c
	__IO UINT32	RESERVED8;									//0x50
}IOCTRL_TypeDef;


#endif


