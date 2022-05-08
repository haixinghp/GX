#ifndef _DES_H_
#define _DES_H_
#include "type.h"


extern void DES_EnDecrypt(UINT8 EncDe_Mode, UINT8 ECB_CBC_Mode, UINT32 *Key, UINT32 KeyLen, UINT32 *ivbuffer, UINT32 *srcbuffer, UINT32 *Output,UINT32 datalen, UINT8 check_parity);

extern unsigned char des_op(unsigned int k_mode, unsigned int s_mode, unsigned int des_antihack,unsigned char ende, unsigned int *key, unsigned int *iv,unsigned int *pin, unsigned int *pout, unsigned int datalen);

#endif /* _DES_H_ */
