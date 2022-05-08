#ifndef __ID2_H_
#define __ID2_H_

#include "type.h"

#define	BLOCK_LEN					(127*1024) //127k,hash数据最长

//#define EFLASH_ID2_SN_ADDR					0x08077000
#define EFLASH_ID2_SN_ADDR					0x08035000
#define EFLASH_ID2_DES_KEY_ADDR			(EFLASH_ID2_SN_ADDR + 0x200)
#define EFLASH_ID2_AES_KEY_ADDR			(EFLASH_ID2_DES_KEY_ADDR + 0x200)
#define EFLASH_ID2_RANDOM_ADDR			(EFLASH_ID2_AES_KEY_ADDR + 0x200)
#define EFLASH_ID2_FLAG_ADDR			  (EFLASH_ID2_RANDOM_ADDR + 0x200)

//*******Hash??????for ID2********//
#define		ID2_HASH_SHA1		0
#define		ID2_HASH_SHA224		1
#define		ID2_HASH_SHA256		2
#define		ID2_HASH_SHA384		3
#define 	ID2_HASH_SHA512		4
#define 	ID2_HASH_SM3		5

/***********transfer config**************************/
//COS设备配置,
/****************************************************/
typedef struct transfer_dev_conf
{
	unsigned char	id;              // 配置ID
	unsigned char	*name;           // 配置名称
	unsigned int	cmd_addr;        // 命令地址
	unsigned int	indata_addr;     // input数据地址
	unsigned int	outdata_addr;	 //  output数据地址
	unsigned int	data_len;        //  数据长度
	unsigned char	cmd_flag;       //  命令flag
}Transfer_Dev_Conf;

typedef struct ID2_cos_dev_info
{
	U8   Vendor_ID[2];
	U8   Verison_Info[8];
	U8   ID2_Config[4];
	U8   Avail_space[2];
	U8   Reserve_B[4];
}ID2_COS_DEV_INFO;

typedef union vendor_info
{
	ID2_COS_DEV_INFO ven_inf;
	U8 buf[sizeof(ID2_COS_DEV_INFO)];
}VENDOR_INFO;

typedef struct ID2_sn_info
{
//	U8		Chip_SN_Len;
	U8		Chip_SN[16];
}ID2_CHIP_SN;

typedef union sn_info
{
	ID2_CHIP_SN sn_inf;
	U8 buf[sizeof(ID2_CHIP_SN)];
}SN_INFO;

/***********Cos Device *************************/
//COS 设备
/****************************************************/

//用户对称密钥结构体
// new added 2017.01.19
typedef	struct{//bytes
	unsigned char id;//密钥ID
	unsigned char ver;//密钥版本
	unsigned char algo;//支持的算法algorithm
	unsigned char uac;//密钥使用权限
	unsigned char wac;//密钥修改权限
	unsigned char size;//密钥的长度
	unsigned char userdef[10];//用户自定义数据，例如关联文件id等
	unsigned char key[48];
}USER_SYMKEY_STRUCT;

extern volatile U32 sm_challenge_len;
extern U32 *sm_challenge;

extern U8 fmm_init(void);
extern void fs_init(U8 flag);
extern void sm_init(void);

#endif