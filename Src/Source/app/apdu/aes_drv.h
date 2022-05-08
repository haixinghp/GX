
#ifndef __AES_DRV_H__
#define __AES_DRV_H__

#include "type.h"

#define P1_BIT0_ENCODE	0x00//0-Encode, 1-DECODE
#define P1_BIT0_DECODE	0x01//0-Encode, 1-DECODE
#define	P1_BIT1_ECB			0x00//0-ECB, 1-CBC
#define	P1_BIT1_CFB			0x01// 0-CFB
#define	P1_BIT1_CBC			0x02// 2-CBC
#define	P1_BIT1_OFB			0x03// 0-OFB

#define	DAES_P1_1SINGLE			0x00//0-DES/AES, 2-2DES/2AES, 3-3des/3AES
#define	DAES_P1_2DOUBLE			0x08//0-DES, 2-2DES, 3-3des
#define	DAES_P1_3TREBLE			0x0c//0-DES, 2-2DES, 3-3des

//extern void AES_EnDecrypt(UINT8 EncDe_Mode, UINT8 CTR_ECB_MODE,UINT32 *Key, UINT32 *cntIV,UINT32 key_mode, UINT32 *srcbuffer, UINT32 *Output, UINT32 datalen);
void Aesop(U16 len, U8 ende, U32 *pin, U32* pout);
void aes_reset(void);
void input_AesKey(U8 mode, U32* pkeybuf);
void input_AesIV(U8 mode,U32 *piv);

#endif /* __AES_DRV_H__ */
