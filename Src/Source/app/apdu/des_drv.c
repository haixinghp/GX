// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : des_drv.c
// Version      : V0.1
// 只基于本工程
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "type.h"
#include "memmap.h"
#include "des_drv.h"
#include "des_reg.h"
#include "edma_drv.h"

//volatile UINT8 dpa_des_t = 0; //disable level

void deskey(unsigned int k_mode, unsigned int s_mode, unsigned int *key, unsigned int *iv,unsigned char ende)
{
	unsigned int des_antihack=0;
	DAEKIN1RH = key[0];
	DAEKIN1RL = key[1];
	if((k_mode == DES_KS_TRI128)||(k_mode == DES_KS_TRI192))
	{
		DAEKIN2RH = key[2];
		DAEKIN2RL = key[3];
		if(k_mode == DES_KS_TRI192)
		{
			DAEKIN3RH = key[4];
			DAEKIN3RL = key[5];
		}
	}
	if(s_mode == DES_OP_CBC)
	{
		DAEIVINRH = iv[0];
		DAEIVINRL = iv[1];
	}
	DAECSR = k_mode|s_mode|ende|DES_ENABLE|des_antihack;
//	DAECSR = k_mode|s_mode|ende|DES_ENABLE|DES_VDES_EN|DES_DISTURB;
//	DESSEEDH = Algo_GetRandomWord();
//	DESSEEDL = Algo_GetRandomWord();
}

unsigned char des_op(unsigned int k_mode, unsigned int s_mode, unsigned int des_antihack,unsigned char ende, unsigned int *key, unsigned int *iv,unsigned int *pin, unsigned int *pout, unsigned int datalen)
{

	deskey(k_mode, s_mode, key,iv, ende);

	EDMAC_2ALG(PN6, (U32)pin, (U32)pout, datalen);

	return 0;

}

