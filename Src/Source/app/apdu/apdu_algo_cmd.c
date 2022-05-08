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
/*����: apdu_sci_hash_func(void)
 *���ܣ�
 *��Σ�
 *���أ�
 *��ע��
 */
/*********************************************************/
Hash_Cx ccore_hash_Ct;
/*******************************************************/
//AES����
UINT8 aes_encrypt_cbc1(UINT8 *IV, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 i, j;
	UINT32 crypto[4];		//CBCģʽ��ʱ������
	UINT32 data_in[4];		//CBCģʽ��ʱ������
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


//AES����
UINT8 aes_decrypt_cbc1(UINT8 *IV, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 i, j;
	UINT32 crypto[4]={0};		//CBCģʽ��ʱ������
	UINT32 data_in[4]={0};		//CBCģʽ��ʱ������
	UINT32 g_algorithm_buf2[4]={0};

	memcpy((UINT8*)crypto, IV, 16);

	for(i=0;i<length/4;i+=4)
	{
		//ECBģʽ���ܣ�����ģʽ��Դ���ݡ�Ŀ�����ݡ����ȡ�key
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
/*����:apdu_sci_hash_key(void)
 *���ܣ��û��Գ��㷨�� ö����Կ
 *��Σ�void
 *���أ�void
 *��ע��
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

//����ID2 KEY
U16 decrypt_ID2_Key(U32 enkey_addr,U32 *DE_key)
{
	U16 ret;
    //	UINT32 read_buf[6];
	//U32 chip0_SN[2];
	U8 SN[8];
	U32 temp_key[12];
  
	U32 De_storage_key[6];  //���ڼ��ܴ洢�����

     
	memcpy((U8*)DE_key, (U8*)(enkey_addr), 24);   //����ID2 key������
//	memcpy((U8*)temp_key, (U8*)(enkey_addr+0x03), 24);   //����ID2 key������
////step1 ��ȡ������������Կ
//    
//	memcpy((U8*)De_storage_key, (U8*)(EFLASH_ID2_RANDOM_ADDR), 24);   //���ù̶���ַ��Կ
////step2 ��ȡSN,��ΪIVֵ

//	read_cid(SN);
////step3 ���ܵõ�ID2 Key
//	ret = ID2_key_decode((U32 *)temp_key, (U32*)DE_key, 24, (U32 *)De_storage_key,(U32 *)SN);
	return ret;
}


//�Գ��㷨
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
			if(endemode == 0x51||endemode == 0x52||endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
			}
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case AES_CBC_NOPADDING:
			ivlen = 16;				//AES
			maclen = 16;
			if(endemode == 0x51||endemode == 0x52||endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
			}
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
			}
			else
			{
				return SW_FUNC_NOT_SUPPORTED;
			}
			break;
		case DES_ECB_NOPADDING:     //ECB
		case AES_ECB_NOPADDING:
			ivlen = 0;
			oplen = len;   //�ӽ��ܳ���
			break;
		case DES_CBC_ISO9797_M1:
			ivlen = 8;				//DES
			maclen = 8;
			if(endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
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
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				paddinglen = 8 - (oplen%0x08);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
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
			if(endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
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
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				paddinglen = 8 - (oplen%0x08);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
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
			if(endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
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
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				paddinglen = 16 - (oplen%0x10);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
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
			if(endemode == 0x53)//�ӽ���,����MAC
			{
				oplen = len-ivlen;   //�ӽ��ܳ���
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
			else if(endemode == 0x54)   //��֤MAC
			{
				oplen = len-ivlen-maclen;   //�ӽ��ܳ���
				paddinglen = 16 - (oplen%0x10);
				memcpy((U8*)MAC_data,pdata_after_sm+(ivlen+oplen)/4,maclen);//lsun:��MAC
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
	memcpy((U8*)IV_data,(U8*)pdataposition,ivlen);//lsun:�˴�piv=0x800005������word����
//	if(endemode == 0x54) 	//��֤MAC,ȡMACֵ
//	{
//		memcpy((U8*)MAC_data,pdataposition+(ivlen+oplen)/4,maclen);//lsun:��MAC
//	}

	pindata = pdataposition+ivlen/4;// offset 16+ivlen;
	memmove(pcmd,pindata,len);//�����������ƶ�Word�����ַ
	pindata = (U32*)pcmd;

	poutdata = pout;

	//���������ݴ������ӽ���
//////////////////////////////////////////////////////////////////////////////
	switch (symmode)
	{
		case DES_ECB_NOPADDING:          //DES EBC
			//lenmode = DES_KS_64;
			lenmode = DES_KS_TRI192;
			//DES ���������0x8��������
			if(0 != (oplen%0x08))
				return SW_WRONG_LENGTH;

/*****************************����DES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_DES_KEY_ADDR,DE_key);
/***************************************************************************/
//			des_op(lenmode,DES_OP_CBC,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)temp_key , (U32*)pout_sci ,pindata,poutdata,oplen);
			des_op(lenmode,DES_OP_ECB,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
			*poutlen = oplen;      //��������ȥ��IV����
			break;

		case AES_ECB_NOPADDING:          //AES EBC

/*****************************����AES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_AES_KEY_ADDR,DE_key);
            for(i=0;i<4;i++)
   printf("De_storage_key[%d]=0x%04x\r\n",i,DE_key[i]);
/***************************************************************************/
			//AES ���������0x10��������
			if(0 != (oplen%0x10))
				return SW_WRONG_LENGTH;

			lenmode = DAES_P1_1SINGLE;

			aes_reset();
			input_AesKey(lenmode, (U32*)DE_key);

			Aesop(oplen, (endemode==0x51)? P1_BIT0_ENCODE : P1_BIT0_DECODE, pindata, poutdata);
			*poutlen = oplen;      //��������ȥ��IV����
			break;

		case DES_CBC_NOPADDING:          //DES CBC
		case DES_CBC_ISO9797_M1:
		case DES_CBC_ISO9797_M2:
			//lenmode = DES_KS_64;
			lenmode = DES_KS_TRI192;
/*****************************����DES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_DES_KEY_ADDR,DE_key);
/***************************************************************************/
			if(endemode == 0x51||endemode == 0x52)		//�ӽ���
			{
				des_op(lenmode,DES_OP_CBC,0,(endemode==0x51)? DES_ENCRYPT : DES_DECRYPT,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
				*poutlen = oplen;      //��������ȥ��IV����
			}
			else if(endemode == 0x53||endemode == 0x54)   //������֤MAC
			{
				des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT ,(U32*)DE_key , (U32*)IV_data ,pindata,poutdata,oplen);
				//���һ�����ֵΪMAC
				memcpy((U8*)poutdata,(U8*)poutdata+oplen-maclen,maclen);//lsun:��MAC
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
/*****************************����AES_Key***************************************/
			decrypt_ID2_Key(EFLASH_ID2_AES_KEY_ADDR,DE_key);
/***************************************************************************/
			//AES ���������0x10��������
			if(0 != (oplen%0x10))
				return SW_WRONG_LENGTH;

			lenmode = DAES_P1_1SINGLE;

			aes_reset();
			input_AesKey(lenmode, (U32*)DE_key);

			if(endemode==0x51)
			{
				//pout_sci=IV Ϊ��4�ֽڶ���
				aes_encrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				*poutlen = oplen;		//��������ȥ��IV����
			}
			else if(endemode==0x52)
			{
				aes_decrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				*poutlen = oplen;		//��������ȥ��IV����
			}
			else if(endemode == 0x53||endemode == 0x54)   //������֤MAC
			{
				aes_encrypt_cbc1((U8*)IV_data, (U8*)pindata, (U8*)poutdata, oplen);
				//���һ�����ֵΪMAC
				memcpy((U8*)poutdata,(U8*)poutdata+oplen-maclen,maclen);//lsun:��MAC
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
/*����:apdu_sci_sym_op(void)
 *���ܣ��û��Գ��㷨����
 *��Σ�void
 *���أ�void
 *��ע��
 */
/*********************************************************/
U16 apdu_sym_op(uint8_t len,uint16_t *rlen)
{
	//U16 len;
	U16 status;

	//ժҪֵ���㳤�� = ���һ��datalen + ���*256 -1
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

