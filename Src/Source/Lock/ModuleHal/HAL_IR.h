/***************************************************************************************
**�ļ���:     HAL_IR.h
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
#ifndef   __HAL_IR_H__
#define   __HAL_IR_H__
#include "MyConfig.h"
#define HAL_IR_MODULE_EN (1) //ģ��ʹ��
#if HAL_IR_MODULE_EN
/*************************ר�ú궨��*************************/  
#define HAL_IR_WAIT_REC_TIME (500) //���յȴ�ʱ��
/*************************�����ṹ����*************************/  
typedef struct
{
    uint32_t *rxCnt;                      //���ռ�����ָ��
    uint8_t *rxBuf; //���ջ�����ָ��
} TYPEs_IRTransPointer;

typedef enum
{
    eIrStateNone, //û�н��
	eIrStateReady, //׼������
	eIrStateWaitReply, //�ȴ��ظ� 
	eIrStateSuccess, //�ɹ�
	eIrStateFail, //ʧ��
	eIrStateTimeover, //��ʱ
	eIrStateCmdError, // ָ�����
	eIrStateRetry, //����
} TYPEe_IrState;

typedef enum
{
    eIrCmdNone=0, //����
	eIrCmdUartMode=1, //����ģʽ
	eIrCmdIOMode=2, //IOģʽ
	eIrCmdShortDistance=3, //�̾���
	eIrCmdMiddleDistance=4, //�о���
	eIrCmdLongDistance=5, //������
	eIrCmdSuperShortDistance=6, //���̾�
	eIrCmdCheckAD=7, //��ѯAD
	eIrCmdSetADMax=8, //����AD���
	eIrCmdSetADMin=9, //����AD��С
	eIrCmdReadAD = 10, //��ADֵ
} TYPEe_IrCmd;

typedef enum
{
    eIrSetDistanceNone, 
	eIrSetDistanceUartMode,
	eIrSetDistanceUartModeWait,
	eIrSetDistanceDistance,
	eIrSetDistanceDistanceWait,
	eIrSetDistanceIOMode,
	eIrSetDistanceIOModeWait,
	eIrSetDistanceSuccss, 
} TYPEe_IrSetDistance;
typedef struct
{
    TYPEe_IrState irState; //����״̬
	uint32_t OverTimeCnt; //��ʱ������
	TYPEe_IrCmd cmd; //��ǰ���е�ָ��
	uint8_t retry; //����
	TYPEe_IrSetDistance irSetDistance; //���þ���״̬��
	TYPEe_IrCmd  distanceType; //����
} TYPEs_IRCtrl;
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_IR_GLOBALS
#define HAL_IR_EXT
#else
#define HAL_IR_EXT extern
#endif     /*HAL_IR_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
HAL_IR_EXT void HAL_IR_Init(void);
HAL_IR_EXT void HAL_IR_IOInit(void);
HAL_IR_EXT int8_t HAL_IR_RecvDataUnblock(void);
HAL_IR_EXT void HAL_IR_SendData(uint8_t *sendBuf, uint32_t len);
HAL_IR_EXT int8_t  HAL_IR_ClearRxPar(void);
HAL_IR_EXT int8_t HAL_IR_SetDistanceStart(uint8_t mode);
HAL_IR_EXT TYPEe_IrSetDistance HAL_IR_SetDistance(void);
HAL_IR_EXT void HAL_IR_UartColse(void);
HAL_IR_EXT uint8_t HAL_IR_ReadIRQ(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_IR_MODULE_EN*/
#endif    /*HAL_IR_MODULE_EN*/
#endif     /*__HAL_IR_H__*/
/************************************Code End******************************************/

