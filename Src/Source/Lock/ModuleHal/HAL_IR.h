/***************************************************************************************
**文件名:     HAL_IR.h
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
#ifndef   __HAL_IR_H__
#define   __HAL_IR_H__
#include "MyConfig.h"
#define HAL_IR_MODULE_EN (1) //模块使能
#if HAL_IR_MODULE_EN
/*************************专用宏定义*************************/  
#define HAL_IR_WAIT_REC_TIME (500) //接收等待时间
/*************************变量结构声明*************************/  
typedef struct
{
    uint32_t *rxCnt;                      //接收计数器指针
    uint8_t *rxBuf; //接收缓冲区指针
} TYPEs_IRTransPointer;

typedef enum
{
    eIrStateNone, //没有结果
	eIrStateReady, //准备运行
	eIrStateWaitReply, //等待回复 
	eIrStateSuccess, //成功
	eIrStateFail, //失败
	eIrStateTimeover, //超时
	eIrStateCmdError, // 指令错误
	eIrStateRetry, //重试
} TYPEe_IrState;

typedef enum
{
    eIrCmdNone=0, //空闲
	eIrCmdUartMode=1, //串口模式
	eIrCmdIOMode=2, //IO模式
	eIrCmdShortDistance=3, //短距离
	eIrCmdMiddleDistance=4, //中距离
	eIrCmdLongDistance=5, //长距离
	eIrCmdSuperShortDistance=6, //超短距
	eIrCmdCheckAD=7, //查询AD
	eIrCmdSetADMax=8, //设置AD最大
	eIrCmdSetADMin=9, //设置AD最小
	eIrCmdReadAD = 10, //读AD值
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
    TYPEe_IrState irState; //运行状态
	uint32_t OverTimeCnt; //超时计数器
	TYPEe_IrCmd cmd; //当前运行的指令
	uint8_t retry; //重试
	TYPEe_IrSetDistance irSetDistance; //设置距离状态字
	TYPEe_IrCmd  distanceType; //距离
} TYPEs_IRCtrl;
#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_IR_GLOBALS
#define HAL_IR_EXT
#else
#define HAL_IR_EXT extern
#endif     /*HAL_IR_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
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

