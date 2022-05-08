/***************************************************************************************
**�ļ���:     DRV_Comm.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
****************************************************************************************/
#ifndef   __DRV_COMM_H__
#define   __DRV_COMM_H__
#include "MyConfig.h"
#define DRV_COMM_MODULE_EN (1) //ģ��ʹ��
#if DRV_COMM_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  
typedef enum
{
    eCommHeadNodat = 0x02, //�޺�������
	eCommHeadHavedat = 0x03, //�к�������
} TYPEe_CommHead;
typedef enum
{
    eCommEncryptEn, //����
	eCommEncryptDis, //�Ǽ���
} TYPEe_CommEncrypt;
typedef enum
{
	eSecretKeyDis, //û����Կ
    eSecretKeyEn, //����Կ
} TYPEe_SecretKey;

typedef struct
{
	TYPEe_CommHead datType; //������
	uint8_t mid; //������
	TYPEe_SecretKey secretKeyFlag; //������Կ��־
	uint8_t *secretKey; //��Կָ��
	uint8_t *dat; //����ָ��
	uint32_t datLen; //���ݳ���
	TYPEe_CommEncrypt cmdType; //�Ƿ����
}TYPEs_ComSendParm;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_COMM_GLOBALS
#define DRV_COMM_EXT
#else
#define DRV_COMM_EXT extern
#endif     /*DRV_COMM_GLOBALS*/

/*************************��������*************************/  
DRV_COMM_EXT uint8_t *pDrvCommMac;
DRV_COMM_EXT uint8_t *pDrvCommRootKeyA;
/*************************��������*************************/
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

