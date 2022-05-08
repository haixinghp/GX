#include	<stdio.h>
#include	<string.h>
#include	"type.h"
#include	"sm.h"
#include  "aes_drv.h"
#include  "des_drv.h"
#include  "des_reg.h"

#include	"apdu_commands.h"
#include "apdu_drv.h"
U32 smchallenge[8];
U32 *sm_challenge = (U32 *)smchallenge;
volatile U32 sm_challenge_len = 0;
//extern unsigned char  * pout_sci;
//const U8 a5_sci_chip_sn1[16] = "76543210";            //for test,need todo

//unsigned int rootkey[6] = {0,0,0,0,0,0};//出厂密钥，用于导入MF下的主控密钥

U8 rootkeylen = SM_KEYLEN_DES;
U8 rootkeytype = SM_KEYTYPE_CTRL;
U8 rootkeyalgo = SM_ALGO_3DES;

//U32 card_key[2]={0x12345678,0x88888888};
//U32 card_iv[2]= {0xabcd1234,0xcdef1234};

// 用户使用随机数max 0x200
//U32 usertrng[0x80];
//U32 *p_user_trng = (U32 *)usertrng;
//volatile U32 user_trng_len = 0;

volatile U8 sm_currentdf_block = 0xff;//当前df文件的锁定状态，0xff-未锁定，0-永久锁定，1-临时锁定
#if 1
/*U16 sm_calcmac(U8 *pcmd, U8 *pdata, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf, U8 mac[4])
 ********************************************************************
 *功能：计算mac
 *参数：
 *返回值:
 *
 *********************************************************************
 */
U16 sm_calcmac(U8 *pdata, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf, U8 mac[4])
{
	U8 *ptmp = pdata;
//	U32 len = datalen + APDU_CMD_LEN;
	U32 len = datalen;
	U32 buf[SM_DATALEN_DES/4];
	U32 encode[2];
	U8 lenmode;

	//lsun: 补齐原则，在长度上补齐为8byte，尾部以0x80+0x00(0-7个)结尾
	memset(&pdata[datalen], 0x80, 1);
	memset(&pdata[datalen+1], 0, SM_DATALEN_DES);
	len = len - len%SM_DATALEN_DES + SM_DATALEN_DES;

	memcpy((U8*)buf, ptmp, SM_DATALEN_DES);
	ptmp += SM_DATALEN_DES;

	//sm_challenge_len = 8;
	if(sm_challenge_len != 8){
		return SW69_REFDATA_UNUSABLE;
	}

/*--------------------------------------------------------------------------------*/
	if(pkey->algo == SM_ALGO_DES){//des
		lenmode = DES_KS_64;//cbc模式第一次加密
		des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,sm_challenge,buf,(U32*)encode, 8);
	}
	else if(pkey->algo == SM_ALGO_3DES){
		if(len != SM_DATALEN_DES){//不止一组数据，则第一组数据进行des cbs模式加密
			lenmode = DES_KS_64;//cbc模式第一次加密
			des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,sm_challenge,buf,(U32*)encode, 8);
		}
		else{//只有一组数据，直接进行3des cbc模式加密
			lenmode = DES_KS_TRI192;//cbc模式第一次加密
			des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,sm_challenge,buf,(U32*)encode, 8);
		}
	}
	else{
		return SW68_UNSUPPORT_SM;//KEY的长度不正确，
	}
	len -= SM_DATALEN_DES;

	while(len>0){
		memcpy((U8*)buf, ptmp, SM_DATALEN_DES);
		ptmp +=SM_DATALEN_DES;
		if((len == SM_DATALEN_DES)&&(pkey->algo == SM_ALGO_3DES)){//3des的最后一组数据用3des运算
			lenmode = DES_KS_TRI192;
			des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,(U32*)encode,buf,  (U32*)encode, 8);

		}
		else{
			lenmode = DES_KS_64;
			des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,(U32*)encode,buf,  (U32*)encode, 8);

		}

		len -= SM_DATALEN_DES;
	}
/*--------------------------------------------------------------------------------*/

	memset(sm_challenge, 0xff, sm_challenge_len);
	sm_challenge_len = 0;
	memcpy(mac, (U8*)encode, SM_MAC_LEN);

	return SW_SUCCESS;
}

/*U16 sm_decode_data(U32 *pdata, U32 *pdecode, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf)
 ********************************************************************
 *功能：数据加密，支持算法des、3des
 *参数：
 *返回值:
 *
 *********************************************************************
 */
U16 ID2_key_encode(U32 *pdata, U32 *pdecode, U16 datalen, U32 *pkeybuf,U32 *iv)
{
	U8 lenmode;

	lenmode = DES_KS_TRI192;

	if((datalen)%SM_DATALEN_DES !=0){
			return SW69_SMDATA_INCORRECT;
	}

	des_op(lenmode,DES_OP_CBC,0,DES_ENCRYPT,pkeybuf,iv,pdata, pdecode ,datalen);

	return SW_SUCCESS;
}

/*U16 sm_decode_data(U32 *pdata, U32 *pdecode, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf)
 ********************************************************************
 *功能：数据解密，支持算法des、3des
 *参数：
 *返回值:
 *
 *********************************************************************
 */
U16 ID2_key_decode(U32 *pdata, U32 *pdecode, U16 datalen, U32 *pkeybuf,U32 *iv)
{
	U8 lenmode;

	lenmode = DES_KS_TRI192;

	if((datalen)%SM_DATALEN_DES !=0){
			return SW69_SMDATA_INCORRECT;
	}

	des_op(lenmode,DES_OP_CBC,0,DES_DECRYPT,pkeybuf,iv,pdata, pdecode ,datalen);

	return SW_SUCCESS;
}

/*U16 sm_decode_data(U32 *pdata, U32 *pdecode, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf)
 ********************************************************************
 *功能：数据解密，支持算法des、3des
 *参数：
 *返回值:
 *
 *********************************************************************
 */
U16 sm_decode_data(U32 *pdata, U32 *pdecode, U16 datalen, KEY_STRUCT *pkey, U32 *pkeybuf,U32 *iv)
{
	U8 lenmode;

	if(pkey->algo != SM_ALGO_3DES){//密钥算法不匹配
		return SW6F_KEY_ALGO;
	}

	lenmode = DES_KS_TRI192;

	if((datalen)%SM_DATALEN_DES !=0){
			return SW69_SMDATA_INCORRECT;
	}

	des_op(lenmode,DES_OP_CBC,0,DES_DECRYPT,pkeybuf,iv,pdata, pdecode ,datalen);
	//des_op(lenmode,DES_OP_ECB,0,DES_DECRYPT,pkeybuf,0,pdata, pdecode ,datalen);

//	memset((U8*)sm_challenge, 0xff, sm_challenge_len);
//	sm_challenge_len = 0;
	return SW_SUCCESS;
}

/********************************************************************
*功能：ID2安全通道读写
*参数：
*返回值:
*
*********************************************************************
*/
U16 sm_manage(U8 *pdatain, U8 **pdataout, U32 *datalen)
{
	U8 *ptmp = pdatain,*ptmp1 = pdatain;
	U8 tmp[256]={0};
	U16 ret;
	U16 psec, lsec, len;
	U32 addr;
	KEY_STRUCT key;
	U32 sm_keybuf[8];
	U8 sm_ivbuf[8];
	U8 *pkeybuf = (U8 *)sm_keybuf;
	U8 *pivbuf = (U8 *)sm_ivbuf;
	U8 keytype;
	U8 mac[4], mac1[4];
	U8 buf[5];
	U8 i;
	U8 smchallenge1[8];
	//U8 *pout_sci = (U8*)SCI_OUTDATA_ADDR;
	U8 chip0_SN[8];

	U8 *pbuf = pdatain+0x400;

	len =  *datalen;

//	smchallenge[0]=0x41a4ee37;	//todo for test calc mac
//	smchallenge[1]=0x2ff14270;

	//做线路保护处理，即计算mac
	//step 1
	//len -= SM_MAC_LEN;//传输数据长度包含数据长度和mac长度两个部分
	memcpy(mac1, &ptmp[len], SM_MAC_LEN);      //保存下发的mac为mac1

	memcpy(tmp,ptmp,len);
	//memmove((U8 *)(ptmp+5),(U8*)(ptmp), len);   //数据前加上命令字段
	memcpy(ptmp,apdu_cmdbuf, 4);
	ptmp[4] = len;
	memcpy(ptmp+5,tmp,len);

	//memcpy((U8 *)(ptmp),(U8*)(pout_sci), len+5);

	//SN(0-7)+RDN(8B)+SN(8-15)
	read_cid(chip0_SN);   

	memcpy((U8 *)(pkeybuf),&chip0_SN, 8);
	memcpy((U8 *)(pkeybuf+8),sm_challenge, 8);
	memset((U8 *)(pkeybuf+16),0xff, 8);
//	memcpy(pkeybuf, rootkey, rootkeylen);			//lsun:set key

	smchallenge1[0] = sm_challenge[0];
	smchallenge1[1] = sm_challenge[0]>>8;
	smchallenge1[2] = sm_challenge[0]>>16;
	smchallenge1[3] = sm_challenge[0]>>24;
	smchallenge1[4] = sm_challenge[1];
	smchallenge1[5] = sm_challenge[1]>>8;
	smchallenge1[6] = sm_challenge[1]>>16;
	smchallenge1[7] = sm_challenge[1]>>24;

	for(i=0;i<8;i++)					//加解密初始化向量 IV
	{
		sm_ivbuf[i] = smchallenge1[i]^pkeybuf[i];
	}

	key.size = rootkeylen;
	key.type = rootkeytype;
	key.algo = rootkeyalgo;

	ret = sm_calcmac(ptmp, len+5, &key, (U32 *)pkeybuf, mac);
	if(ret != SW_SUCCESS){
		return ret;
	}
	
	if(memcmp(mac1, mac, SM_MAC_LEN)){  
		return SW69_SMDATA_INCORRECT;
	}

	//step2
	//做加密保护处理，即解密
	//memmove((U8*)(ptmp),(U8 *)(ptmp+5), len);   //znie:重新组包，数据前移除命令字段
	memcpy(tmp,ptmp+5,len);
	memcpy(ptmp,tmp,len);

	ret = sm_decode_data((U32 *)ptmp1, (U32*)pbuf, len, &key, (U32 *)pkeybuf,(U32 *)pivbuf);
	if(ret != SW_SUCCESS){
			return ret;
	}
	*datalen = pbuf[0];
	if(*datalen > len-4){//解密后的数据长度非法，超过了传输的数据长度
		return 	SW69_SMDATA_INCORRECT;
	}
	*pdataout = &pbuf[0];

	return SW_SUCCESS;
}
#endif
