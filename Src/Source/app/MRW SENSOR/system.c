/*
 * system.c
 *
 *  Created on: 2017Äê5ÔÂ2ÈÕ
 *      Author: li
 */
#include<system.h>
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include"uart.h"
#include"Flash.h"
#include "algorithm_bin_interface.h"
#include "fsm.h"

static U8 paraGenCRC(U8 *dat,U32 len);
static void initParaTab(void);

U16 Frame_Data_Size;
U32 Gl_Password=0;
U8 Index_Tab[4][32];
U8 Baud_Rate=12;
U8 Device_Ready_Flag=0;
U32 Protocol;
_SysParaTag Sys_ParaTag;


const U8 CRC8Table[]=
{
	0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};




U8 ReadParaTab(_SysParaTag * sys_tag,U32 len)
{

	if(sys_tag)
	{
		memcpy((U8*)sys_tag,(U8 *)(SYS_PARA_ADDR),len);
		return 0;
	}
	return  1;
}


U8 WriteParaTab(_SysParaTag * sys_tag,U32 len)
{
	if(sys_tag)
	{
		sys_tag->crc=paraGenCRC((U8*)sys_tag,PARA_CRC_LEN );
		eflashWrite(SYS_PARA_ADDR, len, (U8*)sys_tag);
		ReadParaTab(sys_tag,len);
		return 0;
	}
	return 1;
}



static U8 paraGenCRC(U8 *dat,U32 len)
{
	U8 crc8 = 0;
	while(len--){
		crc8 = CRC8Table[crc8 ^ *dat];
		dat++;
	}

	return crc8;
}





static U8 Version[8] = VERSION_NUM;

static void initParaTab(void)
{
	memset(&Sys_ParaTag,0,sizeof(_SysParaTag));
	Sys_ParaTag.en_passwd=0;
	Sys_ParaTag.flag=SYS_CHANGE;
	Sys_ParaTag.pkt_size=3;
	Sys_ParaTag.baudrate=12;
	Sys_ParaTag.device_add=0xffffffffUL;
	Sys_ParaTag.en_dul_enroll=0;
	Sys_ParaTag.pwd=0;
	Sys_ParaTag.secur_level=1;
	memcpy(&Sys_ParaTag.version[0],&Version[0],sizeof(Version));
	Sys_ParaTag.index=MAX_ID;
	Sys_ParaTag.en_debug_print=0;
	Sys_ParaTag.protocol=PROTOCOL;
	Sys_ParaTag.crc=paraGenCRC((U8*)&Sys_ParaTag,PARA_CRC_LEN);
}



void initDeviceInfo(void)
{
	U32 crc=0;
	ReadParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	if( Sys_ParaTag.flag!=SYS_CHANGE)
	{
		initParaTab();
		WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	}


	Baud_Rate=Sys_ParaTag.baudrate;
	Protocol=Sys_ParaTag.protocol;
	crc=paraGenCRC((U8 *)&Sys_ParaTag,PARA_CRC_LEN);
	if(Sys_ParaTag.crc!=crc)
	{
		Device_Ready_Flag|=DEVICE_READY_SYS_PARA_CRC_FAILE;
		Baud_Rate=12;  //baudrate 115200
		Protocol=PROTOCOL_MICROARRAY ;
	}

	switch(Sys_ParaTag.pkt_size)
	{
		case 0: Frame_Data_Size=32; break;
		case 1: Frame_Data_Size=64; break;
		case 2: Frame_Data_Size=128; break;
		case 3: Frame_Data_Size=256; break;
		default:Frame_Data_Size=256;break;
	}

	if(Protocol==PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP)
	{
		initSynocDeviceInfo();
	}
	else if(Protocol==PROTOCOL_MICROARRAY_COMPATIBLE_YALE)
	{
		initYaleDeviceInfo();
	}

	return;
}


void initDeviceErase(void)
{
	eflash_page_erase_withkey(EFM0_MAIN_BASEADDR+CODE_SIZE+SYS_BRUN_CODE_TIME_SIZE, EFLASH_OPERATION_KEY);
}



_SynocSysParaTag Synoc_Sys_ParaTag;
U16 Synoc_Pkt_Size;

U8 readSynocParaTab(_SynocSysParaTag * sys_tag,U32 len)
{

	if(sys_tag)
	{
		memcpy((U8*)sys_tag,(U8 *)(SYNOC_PARA_ADDR),len);
		return 0;
	}
	return  1;
}


U8 writeSynocParaTab(_SynocSysParaTag * sys_tag,U32 len)
{
	if(sys_tag)
	{
		sys_tag->crc=paraGenCRC((U8*)sys_tag,SYNOC_PARA_CRC_LEN );
		eflashWrite(SYNOC_PARA_ADDR, len, (U8*)sys_tag);
		readSynocParaTab(sys_tag,len);
		return 0;
	}
	return 1;
}


static void initSynocParaTab(void)
{
	memset((U8 *)&Synoc_Sys_ParaTag,0,sizeof(Synoc_Sys_ParaTag));
	Synoc_Sys_ParaTag.device_addr[0]=0xffff;
	Synoc_Sys_ParaTag.device_addr[1]=0xffff;
	Synoc_Sys_ParaTag.cfg_baudrate=6;
	Synoc_Sys_ParaTag.password[0]=0;
	Synoc_Sys_ParaTag.password[1]=0;
	Synoc_Sys_ParaTag.secur_level=2;
	Synoc_Sys_ParaTag.data_base_size=MAX_ID;
	Synoc_Sys_ParaTag.cfg_pkt_size=3;
	Synoc_Sys_ParaTag.ParaTabFlg=SYNOC_SYS_CHANGE;
	Synoc_Sys_ParaTag.crc=paraGenCRC((U8 *)&Synoc_Sys_ParaTag,SYNOC_PARA_CRC_LEN);
}


void initSynocDeviceInfo(void)
{
	U32 crc=0;
	readSynocParaTab(&Synoc_Sys_ParaTag,sizeof(_SynocSysParaTag));
	if( Synoc_Sys_ParaTag.ParaTabFlg!=SYNOC_SYS_CHANGE)
	{
		initSynocParaTab();
		writeSynocParaTab(&Synoc_Sys_ParaTag,sizeof(_SynocSysParaTag));
	}
	Baud_Rate=Synoc_Sys_ParaTag.cfg_baudrate;
	crc=paraGenCRC((U8 *)&Synoc_Sys_ParaTag,SYNOC_PARA_CRC_LEN);
	if(Synoc_Sys_ParaTag.crc!=crc)
	{
		Baud_Rate=6;  //baudrate 115200
	}

	switch(Synoc_Sys_ParaTag.cfg_pkt_size)
	{
		case 0: Synoc_Pkt_Size=32;  break;
		case 1: Synoc_Pkt_Size=64;  break;
		case 2: Synoc_Pkt_Size=128; break;
		case 3: Synoc_Pkt_Size=256; break;
		default:Synoc_Pkt_Size=256; break;
	}
	return;

}


void initYaleDeviceInfo(void)
{
	Baud_Rate=1;
}



U8 readYaleEncryptionKey(U8 * key,U8 len)
{

	if(key)
	{
		memcpy((U8*)key,(U8 *)(YALE_EKEY_ADDR),len);
		return 0;
	}
	return  1;
}


U8 writeYaleEncryptionKey(U8 * key,U8 len)
{
	if(key)
	{
		eflashWrite(YALE_EKEY_ADDR, len, (U8*)key);
		return 0;
	}
	return 1;
}



U8 readBurnCodeTime(U8 * data,U8 len)
{

	if(data)
	{
		memcpy((U8*)data,(U8 *)(BURN_CODE_TIME_ADDR),len);
		return 0;
	}
	return  1;
}

U8 writeBurnCodeTime(U8 * data,U8 len)
{

	if(data)
	{
		eflashWrite(BURN_CODE_TIME_ADDR, len, (U8*)data);
		return 0;
	}
	return  1;
}



/*
#define PID_COMMAND    0x01
#define PID_DATA       0x02
#define PID_LAST_DATA  0x08
#define PID_ACK        0x07
#define PID_DATA_ACK   0x09
#define PID_DEBUG_PRINT 0x0a
#define FRAME_MOST_LEN 256
#define 	DEVICE_READY 0x22
*/

enum {
	NORMAL_PRESSED   = 1,
	NORMAL_UNPRESSED = 0,

	ERROR_BASE       = -10000,
	ERROR_BAD_PARAMETERS,
	
	ERROR_LOAD_PROPERTIES,
	ERROR_INIT_TEMPLATES,
	ERROR_INIT_ALGO,
	ERROR_CHIP_ID,
	ERROR_CHIP_IDENTIFY,
	ERROR_MODE_SWITCH,
	ERROR_MODE_NO_DETECT,
	ERROR_CALC_INIT_PARAM,
	ERROR_CALC_INT_PARAM,

	ERROR_GET_IMAGE,
	ERROR_BAD_IMAGE,
	ERROR_INVALID_IMAGE,
	ERROR_CHECK_MOVEMENT,
	
	ERROR_SPI_READ,
	ERROR_SPI_WRITE,

	ERROR_AUTH_NO_TOUCH,
	ERROR_AUTH_ALGO_FAIL,
	ERROR_AUTH_NO_MATCH,
	
	ERROR_ENROLL_LAST_INCOMPLETE,
	ERROR_ENROLL_ALGO_FAIL,
	ERROR_ENROLL_NO_TOUCH,
	ERROR_ENROLL_EXCEED_MAX_TIMES,
	ERROR_ENROLL_NO_PROGRESS,
	ERROR_ENROLL_SAVE_TEMPLATE,

	ERROR_REMOVE_FID,
	ERROR_CLEAR_FIDS,
};


/*
void deviceReady(void)
{
	S32 res;//bad_blocks = 0;
	//U8 tmp_buf[4]={0};
	//tmp_buf[0]=DEVICE_READY;   //???

	//res=mafp_init(&bad_blocks);
	res = binlib_init(bin_funcs_table);

	if (res) {
		printf("Init Sensor Param Failed\r\n");
		//return -1;
		return;
	}else{
		printf("Init Sensor successful\r\n");
	}
	

	if(res == ERROR_LOAD_PROPERTIES)
	{
		Device_Ready_Flag|=DEVICE_READY_SENSOR_CRC_FAILE;
	}
	else if( (res!=0))
	{
		Device_Ready_Flag|=DEVICE_READY_SENSOR_BREAKDOWN;
	}
	else
	{
		Device_Ready_Flag&=~(DEVICE_READY_SENSOR_BREAKDOWN|DEVICE_READY_SENSOR_CRC_FAILE);
	}
	//tmp_buf[1]=Device_Ready_Flag;
	//tmp_buf[2]=(bad_blocks>>8)&0xff;
	//tmp_buf[3]=(bad_blocks&0xff);

	if(Protocol == PROTOCOL_MICROARRAY)
	{
		//fsmSendFrame(PID_ACK ,0x04,&tmp_buf[0]);
	}
	else  if(Protocol == PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP)
	{
		U8 synoc_start_flag=0x55;
		uartSendBuff(&synoc_start_flag,1);
	}

}
*/

