/***************************************************************************************
**文件名:     DRV_IR.h
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
#ifndef   __DRV_IR_H__
#define   __DRV_IR_H__
#include "MyConfig.h"
#define DRV_IR_MODULE_EN (1) //模块使能
#if DRV_IR_MODULE_EN
/*************************专用宏定义*************************/  
#define DRV_IR_RX_MAX_TIME (200)   //一帧接收完成
#define DRV_IR_RX_BUF_SIZE (128)  //接收缓存
#define DRV_IR_TX_BUF_SIZE (128)  //发送缓存
#define DRV_IR_UART (SCI3)     //使用串口2
/*************************变量结构声明*************************/  
typedef struct
{
    uint32_t rxCnt;                      //接收计数器
    uint32_t rxTim;                      //接收计时器
    uint8_t rxBuf[DRV_IR_RX_BUF_SIZE]; //接收缓冲区
    uint32_t txCnt;                      //发送计数器
    uint8_t txBuf[DRV_IR_TX_BUF_SIZE]; //发送缓冲区
    uint16_t dataLen;                    //数据长度
} TYPEs_IRTrans;

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_IR_GLOBALS
#define DRV_IR_EXT
#else
#define DRV_IR_EXT extern
#endif     /*DRV_IR_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
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