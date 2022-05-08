// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : aes_drv.c
// Version      : V0.1
// 只基于本工程
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "type.h"
#include "memmap.h"
#include "aes_reg.h"
#include "aes_drv.h"
#include "edma_drv.h"


void Aes_Crypt(U16 len, U32* pdata, U32* pout)
{
	U16 i, j;

	len += len & 0x3;
	for(i=0; i<len/4; i+=4)
	{
		for(j=0; j<4; j++){
			AES_DIOR = pdata[j+i];
		}

		while((AES_CSR & AES_DONE) != AES_DONE);// wait aes done

		for(j=0; j<4; j++){
			pout[j+i] = AES_DIOR;
		}

		AES_CSR |= AES_DONE; // clear done
	}

}

void Aesop(U16 len, U8 ende, U32 *pin, U32* pout)
{
    UINT32 i;
	if((ende & P1_BIT0_DECODE) == P1_BIT0_DECODE){
		AES_CSR |= AES_CMODE;
	}
	else
	{
		AES_CSR &= ~AES_CMODE;
	}
	
	//Aes_Crypt(len, pin, pout);
	EDMAC_2ALG(PN5, (U32)pin, (U32)pout, len);

}

void aes_reset(void)
{
	AES_CSR |= 0x40;
}

void input_AesKey(U8 mode, U32* pkeybuf)
{
	U32 i;

	AES_CSR &= ~AES_KEY_MOD;

	for(i=0; i<4; i++){
		AES_PKEY[i] = pkeybuf[i];
	}

	if((mode&0xf0) != 0)
	{
		mode = 0;
	}
	if((mode&DAES_P1_2DOUBLE) == DAES_P1_2DOUBLE){
		AES_PKEY[4] = pkeybuf[4];
		AES_PKEY[5] = pkeybuf[5];
		AES_CSR |= AES_KEY_192;
	}
	if((mode&DAES_P1_3TREBLE) == DAES_P1_3TREBLE){
		AES_PKEY[6] = pkeybuf[6];
		AES_PKEY[7] = pkeybuf[7];
		AES_CSR &= ~AES_KEY_MOD;
		AES_CSR |= AES_KEY_256;
	}

	AES_CSR |= AES_NEW_KEY;
}

void input_AesIV(U8 mode,U32 *piv)
{
	U32 i;

	if((mode&P1_BIT1_CBC) == P1_BIT1_CBC)
	{
		AES_CSR |= AESMSEL;		//CBC
		for(i=0; i<4; i++){
			AES_PCNTR[i] = piv[i];
		}
	}
}