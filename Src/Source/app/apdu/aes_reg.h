// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : aes_reg.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "memmap.h"

//aes
#define AES_DIOR              (*(volatile unsigned long *)(AES_BASE_ADDR))
#define AES_KEY0              (*(volatile unsigned long *)(AES_BASE_ADDR+0x10))
#define AES_KEY1              (*(volatile unsigned long *)(AES_BASE_ADDR+0x14))
#define AES_KEY2              (*(volatile unsigned long *)(AES_BASE_ADDR+0x18))
#define AES_KEY3              (*(volatile unsigned long *)(AES_BASE_ADDR+0x1c))
#define AES_KEY4              (*(volatile unsigned long *)(AES_BASE_ADDR+0x20))
#define AES_KEY5              (*(volatile unsigned long *)(AES_BASE_ADDR+0x24))
#define AES_KEY6              (*(volatile unsigned long *)(AES_BASE_ADDR+0x28))
#define AES_KEY7              (*(volatile unsigned long *)(AES_BASE_ADDR+0x2c))

#define AES_PKEY							((volatile unsigned long *)(AES_BASE_ADDR+0x10))//ADDR

#define AES_CSR               (*(volatile unsigned long *)(AES_BASE_ADDR+0x30))

#define AES_CNT0              (*(volatile unsigned long *)(AES_BASE_ADDR+0x34))
#define AES_CNT1              (*(volatile unsigned long *)(AES_BASE_ADDR+0x38))
#define AES_CNT2              (*(volatile unsigned long *)(AES_BASE_ADDR+0x3c))
#define AES_CNT3              (*(volatile unsigned long *)(AES_BASE_ADDR+0x40))
	
#define AES_PCNTR             ((volatile unsigned long *)(AES_BASE_ADDR+0x34))

#define AES_IV0              (*(volatile unsigned long *)(AES_BASE_ADDR+0x44))
#define AES_IV1              (*(volatile unsigned long *)(AES_BASE_ADDR+0x48))
#define AES_IV2              (*(volatile unsigned long *)(AES_BASE_ADDR+0x4c))
#define AES_IV3              (*(volatile unsigned long *)(AES_BASE_ADDR+0x50))

#define AES_DPA_EN			  0x8000
#define AES_NEW_KEY			  0x100
#define AES_KEY_128			  0x00
#define AES_KEY_192			  0x02
#define AES_KEY_256			  0x04
#define AES_KEY_MOD				0x06

#define AESIE	              0x80	// 1-interrupt enable 0-disable
#define AESRST	              0x40	// 1-reset
#define AESMSEL               0x20  // 1-CTR mode,0-ECB mode
#define AES_DONE		      0x10	// 1-done
#define AES_BUSY		      0x08	// 1-busy
#define AES_CMODE_DEC         0x01
#define AES_CMODE_ENC	      0x00
#define AES_ECB_MODE		  0x00000000
#define AES_CBC_MODE		  0x00000200
#define AES_CFB1_MODE		  0x00000400
#define AES_CFB128_MODE		  0x05000400
#define AES_OFB1_MODE		  0x00000600
#define AES_OFB128_MODE		  0x05000600
#define AES_CTR_MODE		  0x00000800

#define AES_CMODE	0x01
