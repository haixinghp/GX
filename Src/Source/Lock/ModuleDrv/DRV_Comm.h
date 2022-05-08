/***************************************************************************************
**文件名:     DRV_Comm.h
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件 
**备注:      
****************************************************************************************/
#ifndef   __DRV_COMM_H__
#define   __DRV_COMM_H__
#include "MyConfig.h"
#define DRV_COMM_MODULE_EN (1) //模块使能
#if DRV_COMM_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  
typedef enum
{
    eCommHeadNodat = 0x02, //无后续数据
	eCommHeadHavedat = 0x03, //有后续数据
} TYPEe_CommHead;
typedef enum
{
    eCommEncryptEn, //加密
	eCommEncryptDis, //非加密
} TYPEe_CommEncrypt;
typedef enum
{
	eSecretKeyDis, //没有密钥
    eSecretKeyEn, //有密钥
} TYPEe_SecretKey;

typedef struct
{
	TYPEe_CommHead datType; //包类型
	uint8_t mid; //命令字
	TYPEe_SecretKey secretKeyFlag; //有无密钥标志
	uint8_t *secretKey; //密钥指针
	uint8_t *dat; //数据指针
	uint32_t datLen; //数据长度
	TYPEe_CommEncrypt cmdType; //是否加密
}TYPEs_ComSendParm;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_COMM_GLOBALS
#define DRV_COMM_EXT
#else
#define DRV_COMM_EXT extern
#endif     /*DRV_COMM_GLOBALS*/

/*************************公共变量*************************/  
DRV_COMM_EXT uint8_t *pDrvCommMac;
DRV_COMM_EXT uint8_t *pDrvCommRootKeyA;
/*************************公共函数*************************/
DRV_COMM_EXT void DRV_Comm_Init(void);
DRV_COMM_EXT int8_t DRV_Comm_RecvDataUnblock(void);
DRV_COMM_EXT int8_t DRV_Comm_CmdSend(TYPEs_ComSendParm *p);
DRV_COMM_EXT int8_t DRV_Comm_ClearRxPar(void);
DRV_COMM_EXT int8_t DRV_Comm_RecvDataAndAnalysis(TYPEe_CommEncrypt cmdType);
DRV_COMM_EXT uint8_t DRV_COMM_GetRecvMsgType(void);
DRV_COMM_EXT int8_t DRV_COMM_GetRecvContent(uint8_t *readDat, uint32_t len);
DRV_COMM_EXT int16_t DRV_COMM_GetRecvContentSize(void);
DRV_COMM_EXT uint16_t DRV_Comm_GetSumChk(uint8_t *dat, uint16_t len);
DRV_COMM_EXT void DRV_COMM_EnterSleepConfig(void);
DRV_COMM_EXT void DRV_Comm_WakeupBack(void);
DRV_COMM_EXT void DRV_Comm_RstBack(void);
DRV_COMM_EXT int8_t DRV_Comm_WifiSend(uint8_t *sendBuf, uint32_t len);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_COMM_MODULE_EN*/

#endif    /*DRV_COMM_MODULE_EN*/
#endif     /*__DRV_COMM_H__*/
/************************************Code End******************************************/

