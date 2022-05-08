#include "type.h"
#include "apdu_drv.h"
#include "id2.h"
#include "apdu_commands.h"
#include "des_reg.h"
#include "uart_drv.h"
#include "aes_reg.h"
#include "sha_drv.h"
#include "apdu.h"
#include "aes_drv.h"
#include "sys.h"
extern UINT8 apdu_buf[256];
/*********************************************************/
/*函数: apdu_sci_hash_func(void)
 *功能：
 *入参：
 *返回：
 *备注：
 */
/*********************************************************/
Hash_Cx ccore_hash_Ct;
/*******************************************************/
//AES加密
UINT8 aes_encrypt_cbc1(UINT8 *IV, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 i, j;
	UINT32 crypto[4];		//CBC模式临时缓冲区
	UINT32 data_in[4];		//CBC模式临时缓冲区
	UINT32 g_algorithm_buf1[4];

//	memcpy((UINT8 *)aesKey, key, 32);
//	memcpy((UINT8*)g_algorithm_buf1, plain, length);
	memcpy((UINT8*)crypto, IV, 16);

	for(i=0;i<length/4;i+=4)
	{
		memcpy((UINT8*)g_algorithm_buf1, plain+i*4, 16);

		for(j=0;j<4;j++)
		{
			data_in[j] = g_algorithm_buf1[j] ^ crypto[j];
		}
		Aesop(16,P1_BIT0_ENCODE, data_in,crypto);
//		Aesop(16,P1_BIT0_DECODE,cipher+i*4,data_in);
		memcpy(cipher+i*4, (UINT8*)crypto, 16);
	}
	return 0;
}


//AES解密
UINT8 aes_decrypt_cbc1(UINT8 *IV, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 i, j;
	UINT32 crypto[4]={0};		//CBC模式临时缓冲区
	UINT32 data_in[4]={0};		//CBC模式临时缓冲区
	UINT32 g_algorithm_buf2[4]={0};

	memcpy((UINT8*)crypto, IV, 16);

	for(i=0;i<length/4;i+=4)
	{
		//ECB模式加密：加密模式、源数据、目标数据、长度、key
		Aesop(16,P1_BIT0_DECODE,((U32 *)cipher)+i,data_in);
		for(j=0;j<4;j++)
		{
			g_algorithm_buf2[j] = data_in[j] ^ crypto[j];
			memcpy(plain+i*4, (UINT8*)g_algorithm_buf2, 16);
		}
		memcpy((UINT8*)crypto, cipher+i*4, 16);
	}
	return 0;
}

U16 ID2_hash_op(U8 *pcmd, U32 *pin, U32 datalen, U32 *pout, U32* poutlen,uint8_t p2,uint8_t p3) {
	U8 cla, ALG_mode, i;
	U8 type;
	U32 block, left;
	U32 len,mlen,idlen;

	type = pcmd[0];

	len = datalen;

	switch(type){
		case ID2_HASH_SHA1:
			ALG_mode = HASH_SHA1;
			break;
		case ID2_HASH_SHA224:
			ALG_mode = HASH_SHA224;
			break;
		case ID2_HASH_SHA256:
			ALG_mode = HASH_SHA256;
			break;
		case ID2_HASH_SHA384:
			ALG_mode = HASH_SHA384;
			break;
		case ID2_HASH_SHA512:
			ALG_mode = HASH_SHA512;
			break;
		case ID2_HASH_SM3:
			ALG_mode = HASH_SM3;
			break;
		default:
			break;
	}

	if (ALG_mode != HASH_SM3 && ALG_mode != HASH_SHA0 && ALG_mode != HASH_SHA1 && \
		ALG_mode != HASH_SHA224 && ALG_mode != HASH_SHA256&& ALG_mode != HASH_SHA384\
		&& ALG_mode != HASH_SHA512) {
		return SW_WRONG_P1P2 ;
	}

	if((len >  0xff-5) || (len == 0)){
		return SW_WRONG_LENGTH ;
	}
	
	if(p2==0){
		SHA_Init_Algo(&ccore_hash_Ct, ALG_mode);
	}
	
	if(p3==1){
		SHA_Final_Algo(&ccore_hash_Ct, (U8*)pin, len, pout);
		len = SHA_RES_LEN(ALG_mode);
		*poutlen = (U16)(len*4);
		return SW_SUCCESS;
	}else{
		SHA_Update_Algo(&ccore_hash_Ct, (U8*)pin, len);
		*poutlen = 0;
		return SW_SUCCESS;
	}
}

/*********************************************************/
/*函数:apdu_sci_hash_key(void)
 *功能：用户对称算法的 枚举密钥
 *入参：void
 *返回：void
 *备注：
 */
/*********************************************************/
U16 apdu_hash( uint8_t *rbuf,uint8_t len, uint16_t *rlen,uint8_t p2,uint8_t p3)
{
//	U32 len;
	U16 status;
	
	//len = P3;
	
	status = ID2_hash_op(apdu_buf,(U32 *)(apdu_buf+1),len,(U32*)rbuf,&rlen,p2,p3);
	if(status!=SW_SUCCESS)
		return status;

//#ifdef UART
//	//Transmitdata(pbuffer1, len);
//	UART_Send(SCI,pbuffer1,len);
//	resultCode = SW_SUCCESS;
//#else
//	response_len = len;
//	if(response_len == 0)
//	{
//		resultCode =SW_SUCCESS;
//	}
//	else
//	{
//		resultCode = SW61_GET_RESPONSE | response_len;
//	}
//#endif
}

//解密ID2 KEY
U16 decrypt_ID2_Key(U32 enkey_addr,U32 *DE_key)
{
	U16 ret;
    //	UINT32 read_buf[6];
	//U32 chip0_SN[2];
	U8 SN[8];
	U32 temp_key[12];
  
	U32 De_storage_key[6];  //用于加密存储随机数

     
	memcpy((U8*)DE_key, (U8*)(enkey_addr), 24);   //调用ID2 key的密文
//	memcpy((U8*)temp_key, (U8*)(enkey_addr+0x03), 24);   //调用ID2 key的密文
////step1 获取保存的随机数密钥
//    
//	memcpy((U8*)De_storage_key, (U8*)(EFLASH_ID2_RANDOM_ADDR), 24);   //调用固定地址密钥
////step2 获取SN,做为IV值

//	read_cid(SN);
////step3 解密得到ID2 Key
//	ret = ID2_key_decode((U32 *)temp_key, (U32*)DE_key, 24, (U32 *)De_storage_key,(U32 *)SN);
	return ret;
}


//对称算法
U16 ID2_sym_op(U8 *pcmd, U32 *pdata, U32 datalen, U32 *pout, U16* poutlen)
{
	U8 p1, p2,i,paddinglen;
	U16 ret,fileid,psec, rnum,status;
	U8 lenmode, symmode,endemode, keyid,sksel, key_mode;
	U32 addr;
	USER_SYMKEY_STRUCT user_sym_key;

	U32 DE_key[12];
	U32 IV_data[4],MAC_data[4];
	U32 *pindata;
	U32 *poutdata;
	U32 *pdataposition;
	U32 oplen,ivlen,templen,maclen;
	U32 *piv;
	U32 *pdata_after_sm = (U32*)pdata;
	U32 len = datalen;
	U32 first_sec_data_len = 0;

	*poutlen = 0;

	endemode = pcmd[0];
	symmode = pcmd[1];
	key_mode = pcmd[2];
	len = pcmd[4]|pcmd[3]<<8;

	switch (symmode)
	{
		case DES_CBC_NOPADDING:
			ivlen = 8;				//DES
			maclen = 8;
			if(endemode == 0x51||endemode == 0x52||endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case AES_CBC_NOPADDING:
			ivlen = 16;				//AES
			maclen = 16;
			if(endemode == 0x51||endemode == 0x52||endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case DES_ECB_NOPADDING:     //ECB
		case AES_ECB_NOPADDING:
			ivlen = 0;
			oplen = len;   //加解密长度
			break;
		case DES_CBC_ISO9797_M1:
			ivlen = 8;				//DES
			maclen = 8;
			if(endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
				paddinglen = 8 - (oplen%0x08);
				if(0 == paddinglen)
				{
					oplen = oplen + 8;
					memset((U8*)pdata+len,0x00,8);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len,0x00,paddinglen);
				}
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				paddinglen = 8 - (oplen%0x08);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
				if(0 == paddinglen)
				{
					oplen = oplen + 8;
					memset((U8*)pdata+len-maclen,0x00,8);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len-maclen,0x00,paddinglen);
				}
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case DES_CBC_ISO9797_M2:
			ivlen = 8;				//DES
			maclen = 8;
			if(endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
				paddinglen = 8 - (oplen%0x08);
				if(0 == paddinglen)
				{
					oplen = oplen + 8;
					memset((U8*)pdata+len,0x80,1);
					memset((U8*)pdata+len+1,0x00,7);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len,0x80,1);
					memset((U8*)pdata+len+1,0x00,paddinglen-1);
				}
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				paddinglen = 8 - (oplen%0x08);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
				if(0 == paddinglen)
				{
					oplen = oplen + 8;
					memset((U8*)pdata+len-maclen,0x80,1);
					memset((U8*)pdata+len+1-maclen,0x00,7);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len-maclen,0x80,1);
					memset((U8*)pdata+len+1-maclen,0x00,paddinglen-1);
				}

			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case AES_CBC_ISO9797_M1:
			ivlen = 16;				//DES
			maclen = 16;
			if(endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
				paddinglen = 16 - (oplen%0x10);
				if(0 == paddinglen)
				{
					oplen = oplen + 16;
					memset((U8*)pdata+len,0x00,16);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len,0x00,paddinglen);
				}
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				paddinglen = 16 - (oplen%0x10);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
				if(0 == paddinglen)
				{
					oplen = oplen + 16;
					memset((U8*)pdata+len-maclen,0x00,16);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len-maclen,0x00,paddinglen);
				}
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case AES_CBC_ISO9797_M2:
			ivlen = 16;				//DES
			maclen = 16;
			if(endemode == 0x53)//加解密,计算MAC
			{
				oplen = len-ivlen;   //加解密长度
				paddinglen = 16 - (oplen%0x10);
				if(0 == paddinglen)
				{
					oplen = oplen + 16;
					memset((U8*)pdata+len,0x80,1);
					memset((U8*)pdata+len+1,0x00,15);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len,0x80,1);
					memset((U8*)pdata+len+1,0x00,paddinglen-1);
				}
			}
			else if(endemode == 0x54)   //验证MAC
			{
				oplen = len-ivlen-maclen;   //加解密长度
				paddinglen = 16 - (oplen%0x10);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:搬MAC
				if(0 == paddinglen)
				{
					oplen = oplen + 16;
					memset((U8*)pdata+len-maclen,0x80,1);
					memset((U8*)pdata+len+1-maclen,0x00,15);
				}
				else
				{
					oplen = oplen + paddinglen;
					memset((U8*)pdata+len-maclen,0x80,1);
					memset((U8*)pdata+len+1-maclen,0x00,paddinglen-1);
				}

			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;

		default:
			return SW_FUNC_NOT_SUPPORTED;
			break;
	}

	pdataposition = pdata_after_sm;
	piv = pdataposition;		//IV
	memcpy((U8*)IV_data,(U8*)pdataposition,ivlen);//lsun:此处piv=0x800005，不是word对齐
//	if(endemode == 0x54) 	//验证MAC,取MAC值
//	{
//		memcpy((U8*)MAC_data,pdataposition+(ivlen+oplen)/4,maclen);//lsun:搬MAC
//	}

	pindata = pdataposition+ivlen/4;// offset 16+ivlen;
	memmove(pcmd,pindata,len);//将解密数据移动Word对齐地址
	pindata = (U32*)pcmd;

	poutdata = pout;

	//下面做数据处理，做加解密
//////////////////////////////////////////////////////////////////////////////
	switch (symmode)
	{
		case DES_ECB_NOPADDING:          //DES EBC
			//lenmode = DES_KS_64;
			lenmode = DES_KS_TRI192;
			//DES 运算必须是0x8的整数倍
			if(0 != (oplen%0x08))
				return SW_WRONG_LENGTH;

/*****************************解密DES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_DES_KEY_ADDR,DE_key);
/***************************************************************************/
//			des_op(lenmode,DES_OP_CBC,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)temp_key , (U32*)pout_sci ,pindata,poutdata,oplen);
			des_op(lenmode,DES_OP_ECB,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
			*poutlen = oplen;      //返回数据去除IV长度
			break;

		case AES_ECB_NOPADDING:          //AES EBC

/*****************************解密AES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_AES_KEY_ADDR,DE_key);
            for(i=0;i<4;i++)
   printf("De_storage_key[%d]=0x%04x\r\n",i,DE_key[i]);
/***************************************************************************/
			//AES 运算必须是0x10的整数倍
			if(0 != (oplen%0x10))
				return SW_WRONG_LENGTH;

			lenmode = DAES_P1_1SINGLE;

			aes_reset();
			input_AesKey(lenmode, (U32*)DE_key);

			Aesop(oplen, (endemode==0x51)? P1_BIT0_ENCODE : P1_BIT0_DECODE, pindata, poutdata);
			*poutlen = oplen;      //返回数据去除IV长度
			break;

		case DES_CBC_NOPADDING:          //DES CBC
		case DES_CBC_ISO9797_M1:
		case DES_CBC_ISO9797_M2:
			//lenmode = DES_KS_64;
			lenmode = DES_KS_TRI192;
/*****************************解密DES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_DES_KEY_ADDR,DE_key);
/***************************************************************************/
			if(endemode == 0x51||endemode == 0x52)		//加解密
			{
				des_op(lenmode,DES_OP_CBC,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
				*poutlen = oplen;      //返回数据去除IV长度
			}
			else if(endemode == 0x53||endemode == 0x54)   //计算验证MAC
			{
				des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT ,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
				//最后一组加密值为MAC
				memcpy((U8*)poutdata,(U8*)poutdata+oplen-maclen,maclen);//lsun:搬MAC
				if(endemode == 0x53)
				{
					*poutlen = maclen;
				}
				else if(endemode == 0x54)
				{
					status = memcmp((U8*)MAC_data, (U8*)poutdata, maclen);
					if(status)
					{
						return SW94_MAC_INCORRECT;
					}
					else
					{
						*poutlen = 0;
						return SW_SUCCESS;
					}
				}
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case AES_CBC_NOPADDING:          //AES CBC
		case AES_CBC_ISO9797_M1:
		case AES_CBC_ISO9797_M2:
/*****************************解密AES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_AES_KEY_ADDR,DE_key);
/***************************************************************************/
			//AES 运算必须是0x10的整数倍
			if(0 != (oplen%0x10))
				return SW_WRONG_LENGTH;

			lenmode = DAES_P1_1SINGLE;

			aes_reset();
			input_AesKey(lenmode, (U32*)DE_key);

			if(endemode==0x51)
			{
				//pout_sci=IV 为了4字节对齐
				aes_encrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				*poutlen = oplen;		//返回数据去除IV长度
			}
			else if(endemode==0x52)
			{
				aes_decrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				*poutlen = oplen;		//返回数据去除IV长度
			}
			else if(endemode == 0x53||endemode == 0x54)   //计算验证MAC
			{
				aes_encrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				//最后一组加密值为MAC
				memcpy((U8*)poutdata,(U8*)poutdata+oplen-maclen,maclen);//lsun:搬MAC
				if(endemode == 0x53)
				{
					*poutlen = maclen;
				}
				else if(endemode == 0x54)
				{
					status = memcmp((U8*)MAC_data, (U8*)poutdata, maclen);
					if(status)
					{
						return SW94_MAC_INCORRECT;
					}
					else
					{
						*poutlen = 0;
						return SW_SUCCESS;
					}
				}
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;

		default:
			*poutlen = 0;
			return SW_FUNC_NOT_SUPPORTED;
	}

	return SW_SUCCESS;
}

/*********************************************************/
/*函数:apdu_sci_sym_op(void)
 *功能：用户对称算法计算
 *入参：void
 *返回：void
 *备注：
 */
/*********************************************************/
U16 apdu_sym_op(uint8_t len,uint16_t *rlen)
{
	//U16 len;
	U16 status;

	//摘要值计算长度 = 最后一包datalen + 块号*256 -1
	//len = P3-5;
	
	status = ID2_sym_op(apdu_buf,(U32 *)(apdu_buf+5),len,(U32*)pbuffer1,&rlen);
	if(status!=SW_SUCCESS)
		return status;

//#ifdef UART
//	//Transmitdata(pbuffer1, len);
//	UART_Send(SCI,pbuffer1,len);
//	resultCode = SW_SUCCESS;
//#else
//	response_len = len;
//	if(response_len == 0)
//	{
//		resultCode =SW_SUCCESS;
//	}
//	else
//	{
//		resultCode = SW61_GET_RESPONSE | response_len;
//	}
//#endif
}

