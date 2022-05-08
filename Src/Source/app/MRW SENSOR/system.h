/*
 * system.h
 *
 *  Created on: 2017Äê5ÔÂ2ÈÕ
 *      Author: li
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include "lib/CCM_Types.h"
#include "config.h"

#define FINGER_NUM                      (100)


#define DEBUG_PRINT 0
#define MAX_ID FINGER_NUM

#define SYS_CHANGE (paraGenCRC(Version,8))

#define PAGE_TAP 8
#define PAGE_SIZE 256
#define PARA_CRC_LEN  (sizeof(_SysParaTag)-4)

#define DEVICE_READY_SENSOR_BREAKDOWN 0x04
#define DEVICE_READY_SENSOR_CRC_FAILE 0x02
#define DEVICE_READY_SYS_PARA_CRC_FAILE 0x01


#define CODE_SIZE (CODE_LENGTH * 1024)


#define SYS_BRUN_CODE_TIME_SIZE (512)
#define SYS_PARA_ADDR  (EFM0_MAIN_BASEADDR+CODE_SIZE+SYS_BRUN_CODE_TIME_SIZE)
#define BURN_CODE_TIME_ADDR  (EFM0_MAIN_BASEADDR+CODE_SIZE)

#pragma pack(1)


typedef struct SysParaTag
{
	U32 flag;
	U32 en_passwd;
	U32 secur_level;
	U32 pkt_size;
	U32 baudrate;
	U32 device_add;
	U32 pwd;
	U8  version[8];
	U32 en_dul_enroll;
	U32 index;
	U32 en_debug_print;
	U32 protocol;
	U8 notepad[32];
	U32 crc;
}_SysParaTag;

#pragma pack()

extern U16 Frame_Data_Size;
extern U32 Gl_Password;
extern U8 Index_Tab[4][32];
extern U8 Baud_Rate;
extern U8  Device_Ready_Flag;
extern const U8 CRC8Table[];
extern _SysParaTag Sys_ParaTag;
extern U32 Protocol;


void initDeviceInfo(void);
void initDeviceErase(void);
U8 WriteParaTab(_SysParaTag * sys_tag,U32 len);
U8 ReadParaTab(_SysParaTag * sys_tag,U32 len);
U8 readBurnCodeTime(U8 * data,U8 len);
U8 writeBurnCodeTime(U8 * data,U8 len);


#define SYNOC_SYS_CHANGE (paraGenCRC(Version,8))
#define SYNOC_PARA_CRC_LEN  (sizeof(_SynocSysParaTag)-2)
#define SYNOC_PARA_ADDR  (SYS_PARA_ADDR +sizeof(_SysParaTag))

#pragma pack(1)

typedef struct SynocSysParaTag
{
	U16 ssr;
	U16 sensor_type;         
	U16 data_base_size;      
	U16 secur_level;         
	U16 device_addr[2];	
	U16 cfg_pkt_size;
	U16 cfg_baudrate;
	U16 cfg_vid;
	U16 cfg_pid;
	U16 Reserved_a[4];
	U16 product_sn[4];
	U16 sw_Version[4];
	U16 manu_facture[4];
	U16 sensor_name[4];
	U16 password[2];
	U16 jtag_Lock_flag[2];
	U16 sensor_init_entry;
	U16 sensor_getimage_entry;
	U16 Reserved_b[27];
	U16 ParaTabFlg;
	U16 crc;
} _SynocSysParaTag;

#pragma pack()

extern _SynocSysParaTag Synoc_Sys_ParaTag;

extern void initSynocDeviceInfo(void);
extern void initYaleDeviceInfo(void);
extern U8 writeSynocParaTab(_SynocSysParaTag * sys_tag,U32 len);



#define YALE_EKEY_ADDR  (SYNOC_PARA_ADDR+sizeof(_SynocSysParaTag))
extern U8 readYaleEncryptionKey(U8 * key,U8 len);
extern U8 writeYaleEncryptionKey(U8 * key,U8 len);


#endif





