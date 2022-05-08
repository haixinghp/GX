/*
 * apdu_dispatcher.c
 *
 *  Created on: 2018年8月13日
 *      Author: YangWenfeng
 */

#include <string.h>
#include "apdu_drv.h"
#include "apdu_dispatcher.h"
#include "get_sn_drv.h"
#include "usi_drv.h"
#include "cpm_drv.h"
#include "delay.h"
#include "id2_client.h"
#include "apdu_commands.h"
UINT16 response_len = 0;
UINT16 apdu_offset  = 0;
UINT8   EnterSleep = 0;
UINT8 apdu_buf[256]={0};	
const UINT8 version[] = "CCM4202S DEMO COS V1.0 2020-02-25";

uint8_t get_id2=0;
uint8_t get_digest=0;
uint8_t get_encrypt=0;
uint8_t rbuf1[1000];
irot_result_t se_transmit(void* handle, const uint8_t* sbuf, const uint32_t slen, uint8_t* rbuf, uint32_t* rlen)
{
	 
	switch (sbuf[0]){
		case CLA_STANDARD:   //00
			switch (sbuf[1]){
				case INS_GET_RESPONSE:   //c0
					if(get_id2==1)
					{
					memcpy((U8*)(rbuf), (U8*)pbuffer1, 15);
				  rbuf[15]=0x90;
			  	rbuf[16]=0x00;
				  *rlen = 17;
				  get_id2=0;
					}
					if(get_digest==1)
					{
					
				  memcpy(rbuf,rbuf1,*rlen);
//					apdu_hash(rbuf,sbuf[4],rlen,sbuf[2],sbuf[3]);
				  get_digest=0;
					*rlen = sbuf[4];
					rbuf[*rlen]=0x90;
					rbuf[*rlen+1]=0x00;
						*rlen +=2;
					}
					if(get_encrypt==1)
					{
					memcpy(apdu_buf,sbuf+5,sbuf[4]+5);
					apdu_sym_op(sbuf[4],rlen);
					get_encrypt=0;
					*rlen = sbuf[4];
					memcpy(rbuf,pbuffer1,*rlen);
					rbuf[*rlen]=0x90;
					rbuf[*rlen+1]=0x00;
						*rlen +=2;
					}
					break;

				default:
					resultCode = SW_CLA_NOT_SUPPORTED;
					adpu_status = APDU_STATE_IDLE;
					break;
			}
			break;

		case CLA_USER80:       //80
			switch (sbuf[1]){
				case INS_GET_ID2:   //f8
					apdu_get_ID(sbuf[4]); 
				  get_id2=1;
					rbuf[0]= 0x61;
				  rbuf[1] = 0x0c;
				  *rlen = 2;
					      
					break;
         case INS_COMPUTE_DIGEST:     //f0
				  get_digest=1;
				  memcpy(apdu_buf,sbuf+5,sbuf[4]);
				  apdu_hash(rbuf1,sbuf[4]-1,rlen,sbuf[2],sbuf[3]);
					rbuf[0]= 0x61;
				  rbuf[1] = sbuf[4]-5;
				  *rlen = 2;
					break;
				 
				 case INS_SYMMETRIC_ENCRYPT:  //f6
				  get_encrypt=1;
					rbuf[0]= 0x61;
				  rbuf[1] = sbuf[4]-5;
				  *rlen = 2;
					break;
				default:
					resultCode = SW_CLA_NOT_SUPPORTED;
					adpu_status = APDU_STATE_IDLE;
					break;
			}
			break;

		default:
			resultCode = SW_CLA_NOT_SUPPORTED;
			adpu_status = APDU_STATE_IDLE;
			break;
	}
	return IROT_SUCCESS;
}


void InsGetResponse()
{
	u16 len = 0;

	if ( apdu_cmdbuf[4] == 0 )
	{
		len = 256;
	}
	else
	{
		len = apdu_cmdbuf[4];
	}

	if(response_len < len){
		resultCode = SW_WARNING_6C | response_len;
		adpu_status = APDU_STATE_IDLE;
		return;
	}
	SendPB(apdu_cmdbuf[1]);
	Transmitdata(apdu_rspbuf+apdu_offset, len);

	memset(apdu_rspbuf+apdu_offset, 0, len);
	if((response_len - len) == 0)
	{
		apdu_offset = 0;
		response_len = 0;
		resultCode = SW_SUCCESS;
	}
	else
	{
		apdu_offset = len;
		response_len -= len;
		resultCode = SW61_GET_RESPONSE | response_len;
	}
	adpu_status = APDU_STATE_IDLE;
}

void InsReadVersion(void)
{
	DelayMS(5000);
	memcpy((UINT8*)apdu_rspbuf, (UINT8*)version, sizeof(version));
	response_len = sizeof(version);
	resultCode = SW61_GET_RESPONSE | response_len;
}

void InsReadSn()
{
	response_len = 8;
	get_serial_number(apdu_rspbuf);

	resultCode = SW61_GET_RESPONSE | response_len;
}

void InsTestSleep(void)
{
	resultCode = SW_SUCCESS;
	EnterSleep = apdu_cmdbuf[4];
}

/**
 * void ProcessAPDU(void)
 * Dispatcher commands
 *
 * @param	void
 * @return	void
 * @see		N/A
 */
void ProcessAPDU(void)
{

	switch ( apdu_cmdbuf[1] )
	{
		case INS_READ_VERSION:
			InsReadVersion();
			adpu_status = APDU_STATE_IDLE;
			break;
		case INS_READ_SN:
			InsReadSn();
			adpu_status = APDU_STATE_IDLE;
			break;
		case INS_TEST_SLEEP:
			InsTestSleep();
			adpu_status = APDU_STATE_IDLE;
		    break;
		case INS_GET_RESPONSE:
			InsGetResponse();
			break;
		default:
			resultCode = SW_INS_NOT_SUPPORTED;
			adpu_status = APDU_STATE_IDLE;
			break;
	}
}

void ProcessData(void)
{
	switch ( apdu_cmdbuf[1])
	{
		default:
			resultCode = SW_CLA_NOT_SUPPORTED;
			adpu_status = APDU_STATE_IDLE;
			break;
	}
}


void SendStatus(void)
{
	UINT8 sw1 = 0;
	UINT8 sw2 = 0;

	sw1 = (UINT8)(resultCode >> 8);
	sw2 = (UINT8)resultCode;

	if(resultCode == SW_NULL)
	{
		SendPB(apdu_cmdbuf[1]);
		resultCode = 0;
		EN_WTO;//防止等待数据超时
	}
	else
	{
		TransmitByte(sw1);
		TransmitByte(sw2);
		apdu_cnt = 0;
	}
}

void SendLeData(void)
{
	UINT16 i = 0,len = 0;

	if (apdu_cmdbuf[4] == 0)
	{
		len = 256;
	}
	else
	{
		len = apdu_cmdbuf[4];
	}

	for ( i = 0; i < len; i++ )
	{
		TransmitByte(apdu_rspbuf[i]);
	}
}

void usi_main(void)
{
	if(adpu_status == APDU_STATE_CMD)
	{
		ProcessAPDU();
		SendStatus();
		
		if(EnterSleep != 0)
		{
			EnterSleep = 0;
			if(EnterSleep == 1)
			{
				CPM_Card_Sleep();
			}
			else if(EnterSleep ==  2)
			{
				CPM_Card_PowerOff1();
			}
			else
			{
				CPM_Card_PowerOff1p5();
			}
			
		}
	}
	if(adpu_status == APDU_STATE_DATA_OK)
	{
		ProcessData();
		SendStatus();
	}
}

