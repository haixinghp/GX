/***************************************************************************************
**�ļ���:     DRV_IR.h
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
#ifndef   __DRV_IR_H__
#define   __DRV_IR_H__
#include "MyConfig.h"
#define DRV_IR_MODULE_EN (1) //ģ��ʹ��
#if DRV_IR_MODULE_EN
/*************************ר�ú궨��*************************/  
#define DRV_IR_RX_MAX_TIME (200)   //һ֡�������
#define DRV_IR_RX_BUF_SIZE (128)  //���ջ���
#define DRV_IR_TX_BUF_SIZE (128)  //���ͻ���
#define DRV_IR_UART (SCI3)     //ʹ�ô���2
/*************************�����ṹ����*************************/  
typedef struct
{
    uint32_t rxCnt;                      //���ռ�����
    uint32_t rxTim;                      //���ռ�ʱ��
    uint8_t rxBuf[DRV_IR_RX_BUF_SIZE]; //���ջ�����
    uint32_t txCnt;                      //���ͼ�����
    uint8_t txBuf[DRV_IR_TX_BUF_SIZE]; //���ͻ�����
    uint16_t dataLen;                    //���ݳ���
} TYPEs_IRTrans;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_IR_GLOBALS
#define DRV_IR_EXT
#else
#define DRV_IR_EXT extern
#endif     /*DRV_IR_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_IR_EXT void DRV_IR_Init(void);
DRV_IR_EXT void DRV_IR_IOInit(void);
DRV_IR_EXT int8_t DRV_IR_RecvDataUnblock(TYPEs_IRTrans *p,uint32_t RxMaxTime);
DRV_IR_EXT void DRV_IR_SendData(uint8_t *sendBuf, uint32_t len);
DRV_IR_EXT int8_t  DRV_IR_ClearRxPar(TYPEs_IRTrans *p);
DRV_IR_EXT void DRV_IR_UartColse(void);
DRV_IR_EXT uint8_t DRV_IR_ReadIRQ(void);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_IR_MODULE_EN*/
#endif    /*DRV_IR_MODULE_EN*/
#endif     /*__DRV_IR_H__*/
/************************************Code End******************************************/