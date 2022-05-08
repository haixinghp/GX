// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : sha_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include "type.h"
#include "edma_reg.h"
#include "sha_reg.h"
#include "sha_drv.h"
#include "cpm_drv.h"
#include "cpm_reg.h"

#define	__Word_Reverse_(x)		(  ((x&0xff)<<24) | (((x>>8)&0xff)<<16) | (((x>>16)&0xff)<<8) | ((x>>24)&0xff)   )

const int SHA1_IV[] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210, 0xf0e1d2c3};

//{0x67452301,0xefcdab89,0x98badcfe,0x10325476,
//					   0xc3d2e1f0};

const int SHA224_IV[] = { 0xd89e05c1, 0x07d57c36, 0x17dd7030, 0x39590ef7,

		0x310bc0ff, 0x11155868, 0xa78ff964, 0xa44ffabe};



const int SHA256_IV[] ={ 0x67e6096a, 0x85ae67bb, 0x72f36e3c, 0x3af54fa5,
						0x7f520e51, 0x8c68059b, 0xabd9831f, 0x19cde05b
};

const int SHA384_IV[] = {0xcbbb9d5d,0xc1059ed8,0x629a292a,0x367cd507,
						 0x9159015a,0x3070dd17,0x152fecd8,0xf70e5939,
						 0x67332667,0xffc00b31,0x8eb44a87,0x68581511,
						 0xdb0c2e0d,0x64f98fa7,0x47b5481d,0xbefa4fa4};

const int SHA512_IV[] = {0x6a09e667,0xf3bcc908,0xbb67ae85,0x84caa73b,
						 0x3c6ef372,0xfe94f82b,0xa54ff53a,0x5f1d36f1,
						 0x510e527f,0xade682d1,0x9b05688c,0x2b3e6c1f,
						 0x1f83d9ab,0xfb41bd6b,0x5be0cd19,0x137e2179};

const int SM3_IV[] = {
		0x6f168073, 0xb9b21449, 0xd7422417, 0x00068ada,
		0xbc306fa9, 0xaa383116, 0x4dee8de3, 0x4e0efbb0

};

void SHA_Init(Hash_tx *phash_tx, unsigned char HashModeNum)
{
	unsigned int i;

	if(HashModeNum > 6)
	{
		return;
	}

	SHACSR = RESET;
	SHACSR = 0;

	phash_tx->fb_flag[0]  = 0;
	phash_tx->HASHMode = HashModeNum;

	for(i = 0 ; i < 4 ; i++)
	{
		phash_tx->hash_len[i] = 0;
	}
	for(i = 0 ; i < 16 ; i++)
	{
		phash_tx->DataBuf[i] = 0;
	}
}

void SHA_Update(Hash_tx *phash_tx, unsigned char *pMessageBlock, unsigned int DataLen)
{
	int i;
	unsigned char HashModeNum;
	unsigned char seed_len;
	unsigned int reg;

    if(phash_tx->HASHMode == 0)
    {
    	HashModeNum = MODE_SM3;  //sm3
    	seed_len = SM3_R_Len;
    }
    else if(phash_tx->HASHMode == 1)
    {
    	HashModeNum = MODE_SHA0;  //SHA-0
    	seed_len = SHA0_R_Len;
    }
    else if(phash_tx->HASHMode == 2)
    {
    	HashModeNum = MODE_SHA1;  //SHA_1
    	seed_len = SHA1_R_Len;
    }
    else if(phash_tx->HASHMode == 3)
    {
    	HashModeNum = MODE_SHA224; //SHA-224
    	seed_len = SHA224_R_Len;
    }
    else if(phash_tx->HASHMode == 4)
    {
    	HashModeNum = MODE_SHA256; //SHA-256
    	seed_len = SHA256_R_Len;
    }
    else if(phash_tx->HASHMode == 5)
    {
    	HashModeNum = MODE_SHA384; //SHA-384
    	seed_len = SHA384_R_Len;
    }
    else if(phash_tx->HASHMode == 6)
    {
    	HashModeNum = MODE_SHA512; //SHA-512
    	seed_len = SHA512_R_Len;
    }

    if(phash_tx->fb_flag[0] == 0)
    {
    	phash_tx->fb_flag[0] = 1;
    	SHACSR = (HashModeNum)|FIRSTB|SHA_START;
    }
    else
    {
    	SHACSR = (HashModeNum)|SHA_START;
    	SHAWLENTH3 = phash_tx->hash_len[3];
    	SHAWLENTH2 = phash_tx->hash_len[2];
    	SHAWLENTH1 = phash_tx->hash_len[1];
    	SHAWLENTH0 = phash_tx->hash_len[0];

    	for(i = 0 ; i < seed_len ; i++)
    	{
    		SHAWSR = phash_tx->DataBuf[i];
    	}
    }

	//reg = Read_PSR();
	//Disable_Interrupts;		
	
	EDMACRBAR0 = (unsigned int)pMessageBlock ;
	EDMACMINSUMR0 = DataLen;
	EDMACCR0 = (TTYPE2|PN10|0x00000002);
	EDMACCSR0 = 0x00010000;
	while((EDMACCSR0&0x02)==0);         //channel0 done
	EDMACCSR0 |= 0x02;                  //clear done

	EDMACCSR0 = 0x00000000;
	EDMACCR0 = 0;

	//Write_PSR(reg);

	//read result
    while(SHACSR & SHA_BUSY);

    for(i = 0 ; i < seed_len ; i++)
    {
    	phash_tx->DataBuf[i]=SHARDR;
    	//phash_tx->DataBuf[i]=(phash_tx->DataBuf[i]);
    }

    //read length
    phash_tx->hash_len[3] = SHAWLENTH3;
    phash_tx->hash_len[2] = SHAWLENTH2;
    phash_tx->hash_len[1] = SHAWLENTH1;
    phash_tx->hash_len[0] = SHAWLENTH0;
}

void SHA_Final(Hash_tx *phash_tx, unsigned int *pRes)
{
	UINT8 i;
	UINT8 len;
  
	SHACSR = (SHACSR & (~FIRSTB)) | SHADONE;
	
	SHACSRH8 = 0x1;        //sha final

    if(phash_tx->HASHMode == 0)
    {
    	len = SM3_R_Len;      //sm3
    }
    else if(phash_tx->HASHMode == 1)
    {
    	len = SHA0_R_Len;     //SHA-0
    }
    else if(phash_tx->HASHMode == 2)
    {
    	len = SHA1_R_Len;     //SHA_1
    }
    else if(phash_tx->HASHMode == 3)
    {
    	len = SHA224_R_Len;   //SHA-224
    }
    else if(phash_tx->HASHMode == 4)
    {
    	len = SHA256_R_Len;   //SHA-256
    }
    else if(phash_tx->HASHMode == 5)
    {
    	len = SHA384_R_Len;   //SHA-384
    }
    else if(phash_tx->HASHMode == 6)
    {
    	len = SHA512_R_Len;   //SHA-512
    }

    //while(SHACSRT8&SHA_BUSY);
		//while(SHACSR&SHA_BUSY);
    while((SHACSR & (SHA_BUSY | SHADONE)) != SHADONE);

    for(i = 0 ; i < len ; i++)
    {
    	pRes[i] = SHARDR;
        //pRes[i] =( pRes[i]);
    }
}



void sha_read_result(UINT32 *res, UINT32 len)
{
	UINT8 i;

	while(SHACSR&SHA_BUSY);

	for(i=0;i<len;i++)
	{
		res[i]=SHARDR;
	}
}

static void sha_read_mid_length(UINT32 *deal_len)
{
	(deal_len[3]) = __Word_Reverse_(SHAWLENTH3);
	(deal_len[2]) = __Word_Reverse_(SHAWLENTH2);
	(deal_len[1]) = __Word_Reverse_(SHAWLENTH1);
	(deal_len[0]) = __Word_Reverse_(SHAWLENTH0);
}

static void sha_read_mid_result(UINT32 *mid_res)
{
	UINT8 mode, len;
	mode = SHA_GET_MODE;
	mode >>=4;
	len = SHA_MID_LEN(mode);
	sha_read_result(mid_res, len);
}

void sha_resume(UINT8 mode, UINT32 *deal_len, UINT32 *mid_res)
{
	UINT8 i, mid_res_len;

	SHACSR = RESET;
	SHACSR = 0;

	mid_res_len = SHA_MID_LEN(mode);

	SHACSR = (mode<<4)|SHA_START;
	SHAWLENTH3 = __Word_Reverse_(deal_len[3]);
	SHAWLENTH2 = __Word_Reverse_(deal_len[2]);
	SHAWLENTH1 = __Word_Reverse_(deal_len[1]);
	SHAWLENTH0 = __Word_Reverse_(deal_len[0]);

	for(i=0;i<mid_res_len;i=i+1)
	{
		SHAWSR = ((mid_res[i]));
	}
}

void sha_read_mid_data(UINT32 *deal_len, UINT32 *mid_res)
{
	sha_read_mid_result(mid_res);
	sha_read_mid_length(deal_len);
}

void SHA_Init_Algo(Hash_Cx *phash_Cx,unsigned char HashModeNum)
{
	UINT8 i,len;

	SHACSR = RESET;
	SHACSR = 0;
	SHACSR = (HashModeNum<<4)|FIRSTB|SHA_START;

	if(phash_Cx != 0){
		//save mid data
		phash_Cx->bHASHMode = HashModeNum;
		phash_Cx->block_length = 0x40;

		switch(HashModeNum)
		{
			case HASH_SM3:
				len = sha_mid_res_len[HashModeNum][1];
				memcpy(phash_Cx->wMidRes, (U8*)SM3_IV, len*4);
				break;
			case HASH_SHA0:
				len = sha_mid_res_len[HashModeNum][1];
				memcpy(phash_Cx->wMidRes, (U8*)SHA1_IV, len*4);
				break;
			case HASH_SHA1:
				len = sha_mid_res_len[HashModeNum][1];
				memcpy(phash_Cx->wMidRes, (U8*)SHA1_IV, len*4);
				break;
			case HASH_SHA224:
				len = sha_mid_res_len[HashModeNum][1];
				memcpy(phash_Cx->wMidRes, (U8*)SHA224_IV, len*4);
				break;
			case HASH_SHA256:
				len = sha_mid_res_len[HashModeNum][1];
				memcpy(phash_Cx->wMidRes, (U8*)SHA256_IV, len*4);
				break;

			default:
					break;
			}

		for(i=0;i<4;i++)
		{
			phash_Cx->hash_len[i] = 0;
		}

	}
	
}


void SHA_Update_Algo( Hash_Cx *phash_Cx,
				 unsigned char *pMessageBlock, 
				 unsigned int  DataLen)
{
	if(phash_Cx != 0){
		sha_resume(phash_Cx->bHASHMode, (phash_Cx->hash_len), phash_Cx->wMidRes);}
	
	EDMACCSR0 &= ~(EW_EN);
	EDMACCR0 = TTYPE2|PN10|CHANNEL_VALID;
	EDMACRBAR0 = (unsigned int)pMessageBlock;
	EDMACMINSUMR0 = DataLen;
	//TODO
	EDMACMAJSUMR0 = 0;
	EDMACCSR0 |= EW_EN;
	while((EDMACCSR0&0x00000008)==0);
	EDMACCSR0 |= 0x0000000a;
	EDMACCSR0 =0;
	//save mid data
	if(phash_Cx != 0){
		sha_read_mid_data((phash_Cx->hash_len), phash_Cx->wMidRes);}
	
}


void SHA_Final_Algo( Hash_Cx 	 *phash_Cx, unsigned char *pMessageBlock, 
				 unsigned int  DataLen, unsigned int *pRes)
{		
	U8 mode, len;
	if(phash_Cx != 0){
		sha_resume(phash_Cx->bHASHMode, (phash_Cx->hash_len), phash_Cx->wMidRes);}

	//update op in Final
	if(DataLen>0){
	
		EDMACCSR0 &= ~(EW_EN);
		EDMACCR0 = TTYPE2|PN10|CHANNEL_VALID;
		EDMACRBAR0 = (unsigned int)pMessageBlock;
		EDMACMINSUMR0 = DataLen;
		//TODO
		EDMACMAJSUMR0 = 0;
		EDMACCSR0 |= EW_EN;
		while((EDMACCSR0&0x00000008)==0);
		EDMACCSR0 |= 0x0000000a;
		EDMACCSR0 =0;
	}		

	SHACSR = (SHACSR & (~FIRSTB)) | SHADONE;
	
	SHA_FINAL_OP;
	mode = SHA_GET_MODE;
	mode >>=4;
	len = SHA_RES_LEN(mode);
	sha_read_result(pRes, len);

}
