#ifndef __APDU_COMMANDS_H_
#define	__APDU_COMMANDS_H_

#include "type.h"


#define CLA				apdu_cmdbuf[0]
#define INS				apdu_cmdbuf[1]
//#define P1				//apdu_cmdbuf[2]
//#define P2				//apdu_cmdbuf[3]
//#define P1P2			//(P1*256|P2)
#define P3				//apdu_cmdbuf[4]

#define __UART_D
//#define __UART_PRI


//class
#define	CLA_STANDARD					0x00
#define	CLA_USER80						0x80
#define	CLA_USER84						0x84

//ins
//Test
#define INS_WEITE_SN				0xEA//恢复ROM
#define INS_REBOOT					0xE5//恢复ROM
#define INS_EARSE_ID2_FLAG			0xE8//擦除ID2_KEY写入标志
#define INS_READ_VERSION			0xE2//获取软件和硬件版本号
#define INS_GET_RESPONSE			0xC0//GET RESPONSE

//Standard
#define INS_GET_CHALLENGE			0x84//GET CHALLENGE
#define INS_GET_RESPONSE			0xC0//GET RESPONSE
//ID2 standard instruct
#define	INS_GET_ID2					0xF8//获取ID2,ID值
#define	INS_GET_VENDOR_INFO			0xFC//获取厂商信息
#define	INS_COMPUTE_DIGEST			0xF0//计算摘要值（SHA1/SHA256/SHA512/SM3）
#define	INS_GENERATE_KEY_PAIR		0xF2//生成密钥对
#define	INS_ASYMMETRIC_ENCRYPT		0xF4//非对称算法
#define	INS_SYMMETRIC_ENCRYPT		0xF6//对称算法（3DES/AES/SM）
#define	INS_GET_SN					0x11//获取ID2,ID值
//ID2 CLA_USER84
#define	INS_SECURITY_STORE_ID2		0xE2//安全通道下存储读写

//ID2 sym op instruct
#define	DES_CBC_NOPADDING			00
#define	DES_ECB_NOPADDING			01
#define	AES_CBC_NOPADDING			02
#define	AES_ECB_NOPADDING			03
#define	DES_CBC_ISO9797_M1			04
#define	DES_CBC_ISO9797_M2			05
#define	AES_CBC_ISO9797_M1			06
#define	AES_CBC_ISO9797_M2			07

extern U32 smchallenge[8];
extern U32 *sm_challenge;
extern volatile U32 sm_challenge_len;

extern U8 pbuffer1[];

extern unsigned char apdu_cmdbuf[256];
extern void Recover_ROM();
extern void WriteChipSN();
extern void Earse_ID2_FLAG();
extern void InsReadVersion(void);
void InsGetResponse();
void apdu_get_random(void);
void apdu_get_ID(uint8_t len);
void apdu_get_vendor_info();
void apdu_get_sn();
void apdu_get_data();
U16 apdu_hash();
U16 apdu_sym_op();
void apdu_scr_trans();
void read_cid(UINT8 *cid);
void ReadChipSN(U32* chip_sn, U8 pagenum);

#define	P1_BIT0_ENCODE		0x00//0-Encode, 1-DECODE
#define	P1_BIT0_DECODE		0x01//0-Encode, 1-DECODE
#define	P1_BIT1_ECB			0x00//0-ECB, 1-CBC
#define	P1_BIT1_CFB			0x01// 0-CFB
#define	P1_BIT1_CBC			0x02// 2-CBC
#define	P1_BIT1_OFB			0x03// 0-OFB

#endif /* __APDU_COMMANDS_H_ */

