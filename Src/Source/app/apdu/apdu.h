/*
 * Copyright (C) 2010 C*Core Co., Ltd. All rights reserved.
 */
#ifndef __APDU_H_
#define	__APDU_H_

#include "type.h"

//class
#define	CLA_STANDARD					0x00
#define	CLA_STANDARD_CUSTOMSM			0x04
#define	CLA_USER80			0x80
#define	CLA_USER81			0x81        //use crc

// APDU status words
#define SW_SUCCESS							(UINT16)0x9000//成功
#define SW_NULL								(UINT8)0x60
//#define SW_MEMORY_VERIFY_ERROR				(UINT16)0x6581
#define SW_FLASH_ERASE_ERROR				(UINT16)0x6581//存储器故障
#define SW_FLASH_PROGRAM_ERROR				(UINT16)0x6582
#define SW_WRONG_LENGTH						(UINT16)0x6700//LC长度错误
#define SW_WRONG_DATA						(UINT16)0x6A80//不正确的数据域
#define SW_WRONG_P1P2						(UINT16)0x6A86//P1P2不正确
#define SW_INS_NOT_SUPPORTED				(UINT16)0x6D00//INS不支持
#define SW_UNKNOWN							(UINT16)0x6F00

#define SW_EXTAUTH_FAIL						(UINT16)0x6907//会话验证失败
#define SW_SECURITY_STATUS_NOT_SATISFIED	(UINT16)0x6982
#define SW_CONDITIONS_NOT_SATISFIED			(UINT16)0x6985//使用条件不满足
#define SW_CALC_MAC_ERR						(UINT16)0x6988//计算错误/MAC错误
#define SW_CERT_STATUS_ERROR				(UINT16)0x698A//使用的证书状态不被满足
#define SW_FUNC_NOT_SUPPORTED				(UINT16)0x6A81
//#define SW_WRONG_P1P2						(UINT16)0x6B00
#define SW_CLA_NOT_SUPPORTED				(UINT16)0x6E00//CLA不支持
#define SW_WARNING_61						(UINT16)0x6100
#define SW_WARNING_63						(UINT16)0x6300
#define SW_AUTH_FAIL						(UINT16)0x63CF//认证失败
#define SW_WARNING_6C						(UINT16)0x6C00
#define SW_ENCODE_ERROR						(UINT16)0x9082//加密错误
#define SW_DECODE_ERROR						(UINT16)0x9084//解密错误
#define SW9086_ERR_VALIDATE					(UINT16)0x9086//验签错误
#define SW9088_GENERATE_KEY_ERR				(UINT16)0x9088//生成密钥对错误
#define SW908A_SIGN_ERR						(UINT16)0x908a//签名错误

#define SW94_MAC_INCORRECT					(UINT16)0x9406
extern unsigned char g_PCBval;
extern unsigned char g_CIDval;

extern unsigned char adpu_status;
extern unsigned char apdu_buf[];
extern unsigned char apdu_cmdbuf[];
extern unsigned char *pout_usi;
extern volatile unsigned char apdu_cnt;
extern unsigned short apdu_datalen;
extern unsigned int apdu_addr;
extern unsigned short response_len;
extern unsigned short resultcode;

#define CLA				apdu_cmdbuf[0]
#define INS				apdu_cmdbuf[1]
#define P1				apdu_cmdbuf[2]
#define P2				apdu_cmdbuf[3]
#define P3				apdu_cmdbuf[4]
/*****apdu define*********/
#define APDU_SW_IS_OK(sw)		(sw == APDU_SW_OK)
#define APDU_SW_IS_GET_RESPONSE(sw)		\
	((sw & APDU_SW1_MASK) == APDU_SW_GET_RESPONSE)
#define APDU_SW_IS_ERR_LENGTH(sw)	(sw == APDU_SW_ERR_LENGTH)

#define	APDU_CMD_LEN		4

#define	APDU_STATE_IDLE		0
#define	APDU_STATE_PPS		1
#define	APDU_STATE_CMD		2
#define	APDU_STATE_DATA		3
#define	APDU_STATE_PPS_OK	4
#define	APDU_STATE_CMD_OK	5
#define	APDU_STATE_DATA_OK	6
#define	APDU_STATE_ATR		7
//#define	ATR_LENGTH		0x11

typedef struct __cos_core_info
{
	UINT32		EndianFlag;   //   decide big endian or small
	UINT32		Version;
	UINT32 	SupportAlgos[12];
	UINT32 	ExternalAlgo;
	UINT32		InternalAlgo;

	UINT32		ProtectAlgo;
	UINT32		PinHashAlgo;
	UINT32		WriteIsfKeyAlgo;
	UINT32		TotalCapacity;

	UINT32		RemainCapacity;
	UINT32		MaxTransLen;
	UINT32		MaxSm2TransLen;
	//UINT8		Cos_Version[32];
}COS_CORE_INFO;

typedef struct cos_dev_info
{
	COS_CORE_INFO cos_core_dinf;
	UINT32		DevType;
	UINT32		DevFlags;
	UINT8		HardwareVer[16];
	UINT8		FirmwareVer[32];
	UINT8		SoftwareVer[48];
	UINT32		Chip_SN_Len;
	UINT8		Chip_SN[16];
	UINT32		Vendor_SN_Len;
	UINT8		Vendor_SN[32];
}COS_DEV_INFO;

typedef union device_info
{
	COS_DEV_INFO dev_inf;
	UINT8 buf[sizeof(COS_DEV_INFO)];
}DEVICE_INFO;

typedef struct USI_run_state{
	volatile U16 apdu_datalen;
	volatile U16 apdu_wto_cnt;
	volatile U16 resultcode;
	/*get response*/
	volatile U16 response_len;
	volatile U16 response_ofst;
	/*write block data*/
	volatile U16 block_data_len;
	volatile U16 block_data_num;

	volatile U8 time_out;
	volatile U8 isr_status;
}USI_RUN_STATE;

extern USI_RUN_STATE usi_run_state;
extern unsigned short C0_data_len;

#endif /* __APDU_H */

