// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : sha_reg.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "memmap.h"
#include "type.h"

#ifndef __SHA_REG_H__
#define __SHA_REG_H__

//sha
#define SHACSR     			(*(volatile UINT32 *)(SHA_BASE_ADDR))
#define SHACSRH16			(*(volatile UINT16 *)(SHA_BASE_ADDR+0x2))
#define SHACSRL16			(*(volatile UINT16 *)(SHA_BASE_ADDR))
#define SHACSRL8 			(*(volatile UINT8  *)(SHA_BASE_ADDR +0x1))
#define SHACSRT8   		    (*(volatile UINT8  *)(SHA_BASE_ADDR +0x2))
#define SHACSRH8    		(*(volatile UINT8  *)(SHA_BASE_ADDR +0x1))
#define SHAWDR     			(*(volatile UINT32 *)(SHA_BASE_ADDR +0x4))
#define SHAWDR_8    		(*(volatile UINT8  *)(SHA_BASE_ADDR +0x4))
#define SHAWDR_16    		(*(volatile UINT16 *)(SHA_BASE_ADDR +0x4))
#define SHARDR     			(*(volatile UINT32 *)(SHA_BASE_ADDR +0x08))
#define SHAWLENTH   		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x1c))
#define SHAWSR      		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x0c))
#define SHAWLENTH3   		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x10))
#define SHAWLENTH2   		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x14))
#define SHAWLENTH1   		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x18))
#define SHAWLENTH0   		(*(volatile UINT32 *)(SHA_BASE_ADDR +0x1c))

#define SHA_BUSY   			0x00010000
#define SHA_FINAL  			0x01
#define SHADONE				0x00020000
#define MODE_SM3			0x00000000
#define MODE_SHA0			0x00000010
#define MODE_SHA1			0x00000020
#define MODE_SHA224			0x00000030
#define MODE_SHA256			0x00000040
#define MODE_SHA384			0x00000050
#define MODE_SHA512			0x00000060
#define MODE_111			0x00000070
#define FIRSTB				0x00000008
#define SHA_IE				0x00000004
#define RESET				0x00000002
#define SHA_START			0x00000001
#define SHA_RST     		0x00000002

#define SHA_FINAL_OP  (SHACSRH8 = 0x1)

#endif /*__SHA_REG_H__*/
