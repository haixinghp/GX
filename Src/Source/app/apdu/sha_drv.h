// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : sha_drv.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
#ifndef __SHA_H__
#define __SHA_H__

#include "sha_reg.h"

/*******Hash算法类型编号********/
#define	HASH_SM3				0
#define	HASH_SHA0				1
#define	HASH_SHA1				2
#define	HASH_SHA224				3
#define HASH_SHA256				4
#define HASH_SHA384				5
#define HASH_SHA512				6
#define	SHA_MODE_MASK		((7)<<4)

static const UINT32 sha_mid_res_len[][3] =
{
	/* mode			mid		res */
	{MODE_SM3,		8,		8},
	{MODE_SHA0,		5,		5},
	{MODE_SHA1,		5,		5},
	{MODE_SHA224,	8,		7},
	{MODE_SHA256,	8,		8},
};

#define SHA_MID_LEN(mode)	sha_mid_res_len[mode][1]
#define SHA_RES_LEN(mode)	sha_mid_res_len[mode][2]
#define SHA_GET_MODE		(SHACSR & SHA_MODE_MASK)

/*******Hash算法数据结构********/
typedef	struct Hash_tx
{
	unsigned int  hash_len[4];	   //本次传送数据长度；
	unsigned int  DataBuf[16];     //本次运算结果
	unsigned char HASHMode;	       //Hash算法类型编号
	unsigned char fb_flag[1];      //第一包数据标志,1 - 第一包数据传输完;0 - 未完成

}Hash_tx;

typedef	struct Hash_Cx
{
	unsigned int  wRsdLenInBuf;	  //本次计算剩余数据长度；
	unsigned int  hash_len[4];	  	  //已计算数据总长度；
	unsigned int  wDataBuf[16];   //本次运算剩余数据缓冲区
	unsigned int  wMidRes[8]; 	  //本次运算中间结果缓冲区
	unsigned char bHASHMode;	  //Hash算法类型编号
	unsigned int  block_length;	  	  //算法block长度；
}Hash_Cx;

/*************************************************************
 * phash_tx     哈希算法数据结构
 * HashModeNum  哈希算法编号
 * ***********************************************************/
void SHA_Init(Hash_tx *phash_tx, unsigned char HashModeNum);

/*************************************************************
 * phash_tx       哈希算法数据结构
 * pMessageBlock  待处理数据
 * DataLen        待处理数据长度
 * ***********************************************************/
void SHA_Update(Hash_tx *phash_tx, unsigned char *pMessageBlock, unsigned int DataLen);

/*************************************************************
 * phash_tx       哈希算法数据结构
 * pRes           哈希运算结果
 * ***********************************************************/
void SHA_Final(Hash_tx *phash_tx, unsigned int *pRes);


void SHA_Init_Algo(Hash_Cx *phash_Cx,unsigned char HashModeNum);
void SHA_Update_Algo( Hash_Cx *phash_Cx,unsigned char *pMessageBlock, unsigned int  DataLen);
void SHA_Final_Algo( Hash_Cx *phash_Cx, unsigned char *pMessageBlock, unsigned int  DataLen, unsigned int *pRes);

#define SHA0_R_Len				5
#define SHA1_R_Len  			5
#define SHA224_R_Len 			7
#define SHA256_R_Len 			8
#define SM3_R_Len				8
#define SHA384_R_Len 			12
#define SHA512_R_Len 			16

#endif /* __SHA_H__ */
