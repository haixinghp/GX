// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : sha_drv.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
#ifndef __SHA_H__
#define __SHA_H__

#include "sha_reg.h"

/*******Hash�㷨���ͱ��********/
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

/*******Hash�㷨���ݽṹ********/
typedef	struct Hash_tx
{
	unsigned int  hash_len[4];	   //���δ������ݳ��ȣ�
	unsigned int  DataBuf[16];     //����������
	unsigned char HASHMode;	       //Hash�㷨���ͱ��
	unsigned char fb_flag[1];      //��һ�����ݱ�־,1 - ��һ�����ݴ�����;0 - δ���

}Hash_tx;

typedef	struct Hash_Cx
{
	unsigned int  wRsdLenInBuf;	  //���μ���ʣ�����ݳ��ȣ�
	unsigned int  hash_len[4];	  	  //�Ѽ��������ܳ��ȣ�
	unsigned int  wDataBuf[16];   //��������ʣ�����ݻ�����
	unsigned int  wMidRes[8]; 	  //���������м���������
	unsigned char bHASHMode;	  //Hash�㷨���ͱ��
	unsigned int  block_length;	  	  //�㷨block���ȣ�
}Hash_Cx;

/*************************************************************
 * phash_tx     ��ϣ�㷨���ݽṹ
 * HashModeNum  ��ϣ�㷨���
 * ***********************************************************/
void SHA_Init(Hash_tx *phash_tx, unsigned char HashModeNum);

/*************************************************************
 * phash_tx       ��ϣ�㷨���ݽṹ
 * pMessageBlock  ����������
 * DataLen        ���������ݳ���
 * ***********************************************************/
void SHA_Update(Hash_tx *phash_tx, unsigned char *pMessageBlock, unsigned int DataLen);

/*************************************************************
 * phash_tx       ��ϣ�㷨���ݽṹ
 * pRes           ��ϣ������
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
