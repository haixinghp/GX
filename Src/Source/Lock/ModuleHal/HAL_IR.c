/***************************************************************************************
**文件名:     HAL_IR.c
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
#include "HAL_IR.h"
#include "DRV_IR.h"
#if  HAL_IR_MODULE_EN
#warning HAL_IR模块使能!
/*******************************宏定义***************************/  

/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  
static TYPEs_IRTrans irTrans=
{
	.rxCnt =0,
	.rxTim = 0,
	.rxBuf={0,},
	.txCnt = 0,
	.txBuf={0,},
	.dataLen =0,
};
/*************************公共全局变量*************************/  
TYPEs_IRTransPointer irTransPointer=
{
	.rxCnt = &irTrans.rxCnt,
	.rxBuf = irTrans.rxBuf,
};
const uint8_t irCmd[11][8]=
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// NULL
	{0xAA,0xAA,0x02,0x07,0x00,0x09,0x55,0x55},//N101=0,串口模式,1000
	{0xAA,0xAA,0x02,0x07,0x02,0x0b,0x55,0x55},//N102=0,IO模式,1000
	{0xAA,0xAA,0x02,0x03,0x01,0x06,0x55,0x55},//N103=0,短距离,1000
	{0xAA,0xAA,0x02,0x03,0x02,0x07,0x55,0x55},//N104=0,中距离,1000
	{0xAA,0xAA,0x02,0x03,0x03,0x08,0x55,0x55},//N105=0,长距离,1000
	{0xAA,0xAA,0x02,0x03,0x00,0x05,0x55,0x55},//N106=0,超短距,1000
	{0xAA,0xAA,0x01,0x30,0x00,0x31,0x55,0x55},//N107=0,查询AD,1000
	//{0xAA,0xAA,0x02,0x05,0xC8,0xCF,0x55,0x55}, //200//N108=0,设置AD最大,1000
	{0xAA,0xAA,0x02,0x05,0x64,0x6B,0x55,0x55}, //100
	{0xAA,0xAA,0x02,0x05,0x19,0x20,0x55,0x55}, //25//N109=0,设置AD最小,1000
	{0xAA,0xAA,0x01,0x05,0x00,0x06,0x55,0x55},//N110=0,读AD值,1000
};

TYPEs_IRCtrl irCtrl=
{
	.irState= eIrStateNone,
	.OverTimeCnt = 0,
	.cmd = eIrCmdNone,
};
/*************************局部函数*****************************/  


/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       HAL_IR_IOInit
**功能描述:     IO初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_IR_IOInit(void)
{
	DRV_IR_IOInit();
}

/***************************************************************************************
**函数名:       HAL_IR_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_IR_Init(void)
{
    DRV_IR_Init();
}
/***************************************************************************************
**函数名:       HAL_IR_RecvDataUnblock
**功能描述:     非阻塞式接收数据
**输入参数:     --
**输出参数:     
int8_t
1; //接收完成
0; //数据接收中
-1; //参数错误
-2; //没有数据
-3; //接收参数错误
-4; //未开始接收
**备注:         
****************************************************************************************/
int8_t HAL_IR_RecvDataUnblock(void)
{
	return DRV_IR_RecvDataUnblock(&irTrans,DRV_IR_RX_MAX_TIME);
}
/***************************************************************************************
**函数名:       HAL_IR_SendData
**功能描述:     发送数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_IR_SendData(uint8_t *sendBuf, uint32_t len)
{
	DRV_IR_SendData(sendBuf,len);
}

/***************************************************************************************
**函数名:       DRV_BleComm_ClearRxPar
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t  HAL_IR_ClearRxPar(void)
{
	return DRV_IR_ClearRxPar(&irTrans);
}
/***************************************************************************************
**函数名:       DRV_BleComm_ClearRxPar
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t  HAL_IR_SetStart(TYPEe_IrCmd cmd,uint32_t overTimeMax,uint8_t retry)
{
	HAL_IR_ClearRxPar();
	irCtrl.OverTimeCnt = overTimeMax;
	irCtrl.retry = retry;
	irCtrl.cmd = cmd;
	irCtrl.irState = eIrStateReady;
}
/***************************************************************************************
**函数名:       HAL_IR_Set
**功能描述:     IR设置
**输入参数:     
TYPEe_IrCmd cmd, 指令
uint32_t overTimeMax 超时时间
**输出参数:     
TYPEe_IrState
**备注:         
****************************************************************************************/
TYPEe_IrState HAL_IR_Set(TYPEe_IrCmd cmd,uint32_t overTimeMax)
{
	static uint8_t tmpTab[8]={0,};
	if(cmd != irCtrl.cmd)
	{
		return eIrStateCmdError;
	}
	if (0 != irCtrl.OverTimeCnt) //自动计数器
    {
        if (1 == irCtrl.OverTimeCnt)
        {
            irCtrl.irState = eIrStateTimeover; //超时
        }
        irCtrl.OverTimeCnt--;
    }
	switch (irCtrl.irState)
    {
    	case eIrStateNone:
    		break;
    	case eIrStateReady:
			memcpy(tmpTab,&irCmd[(uint8_t)cmd][0],8);
			HAL_IR_SendData(tmpTab,8); //发送指令
			irCtrl.irState = eIrStateWaitReply;
			irCtrl.OverTimeCnt = overTimeMax;
    		break;
		case eIrStateWaitReply:
			if(1 == HAL_IR_RecvDataUnblock())
			{
				DEBUG_LOG_OUT_HEX(irTrans.rxBuf,8);
				DEBUG_LOG_OUT_HEX(tmpTab,8);
				if((*(irTrans.rxBuf+4) == tmpTab[4]) &&
				   (*(irTrans.rxBuf) == tmpTab[0]))
				{
					DEBUG_LOG_OUT("eIrStateSuccess\n");
					irCtrl.irState = eIrStateSuccess; //成功
				}
				else
				{
					irCtrl.retry --;
					if(irCtrl.retry == 0)
					{
						DEBUG_LOG_OUT("eIrStateFail\n");
						irCtrl.irState = eIrStateFail; //失败
					}
					else
					{
						DEBUG_LOG_OUT("eIrStateReady\n");
						irCtrl.irState = eIrStateReady; //重试
					}
				}
				HAL_IR_ClearRxPar();
			}
			
    		break;
		case eIrStateSuccess:
    		break;
		case eIrStateFail:
    		break;
		case eIrStateTimeover:
    		break;
    	default:
    		break;
    }
}
/***************************************************************************************
**函数名:       HAL_IR_SetDistanceStart
**功能描述:     IR设置距离
**输入参数:     --
**输出参数: 
int8_t
**备注:         
****************************************************************************************/
int8_t HAL_IR_SetDistanceStart(uint8_t mode)
{
	switch (mode)
    {
    	case 0:
			irCtrl.distanceType = eIrCmdLongDistance;
    		break;
    	case 1:
			irCtrl.distanceType = eIrCmdShortDistance;
    		break;
		case 2:
			irCtrl.distanceType = eIrCmdSuperShortDistance;
    		break;
    	default:
			irCtrl.distanceType = eIrCmdShortDistance;
    		break;
    }
	irCtrl.irSetDistance = eIrSetDistanceUartMode; //启动状态机
	return 0;
}
/***************************************************************************************
**函数名:       HAL_IR_SetDistance
**功能描述:     IR设置距离
**输入参数:     --
**输出参数:     
**备注:       
设置步骤：
把模块配置成串口模式。
配置距离。
恢复成IO模式。
****************************************************************************************/
TYPEe_IrSetDistance HAL_IR_SetDistance(void)
{
	switch (irCtrl.irSetDistance)
    {
    	case eIrSetDistanceNone:
    		break;
    	case eIrSetDistanceUartMode:
			DEBUG_LOG_OUT("eIrSetDistanceUartMode\n");
			HAL_IR_SetStart(eIrCmdUartMode,0,3);
			irCtrl.irSetDistance = eIrSetDistanceUartModeWait;
    		break;
		case eIrSetDistanceUartModeWait:
			switch (HAL_IR_Set(eIrCmdUartMode,500))
            {
            	case eIrStateSuccess:
            	case eIrStateFail:
				case eIrStateTimeover:
					irCtrl.irSetDistance = eIrSetDistanceDistance;		
            		break;
            	default:
            		break;
            }
    		break;
		case eIrSetDistanceDistance:
			HAL_IR_SetStart(irCtrl.distanceType,0,3);
			irCtrl.irSetDistance = eIrSetDistanceDistanceWait;
			break;
		case eIrSetDistanceDistanceWait:
			switch (HAL_IR_Set(irCtrl.distanceType,500))
            {
            	case eIrStateSuccess:
            	case eIrStateFail:
				case eIrStateTimeover:
					irCtrl.irSetDistance = eIrSetDistanceIOMode;		
            		break;
            	default:
            		break;
            }
			break;
		case eIrSetDistanceIOMode:
			HAL_IR_SetStart(eIrCmdIOMode,0,3);
			irCtrl.irSetDistance = eIrSetDistanceIOModeWait;
    		break;
		case eIrSetDistanceIOModeWait:
			switch (HAL_IR_Set(eIrCmdIOMode,500))
            {
            	case eIrStateSuccess:
            	case eIrStateFail:
				case eIrStateTimeover:
					irCtrl.irSetDistance = eIrSetDistanceSuccss;		
            		break;
            	default:
            		break;
            }
    		break;
    	default:
    		break;
    }
	return irCtrl.irSetDistance;
}

/***************************************************************************************
**函数名:       HAL_IR_UartColse
**功能描述:     串口模式关闭
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_IR_UartColse(void)
{
	DRV_IR_UartColse();
}

/***************************************************************************************
**函数名:       HAL_IR_ReadIRQ
**功能描述:     读取IO
**输入参数:     --
**输出参数:     
uint8_t
**备注:         
****************************************************************************************/
uint8_t HAL_IR_ReadIRQ(void)
{
	return DRV_IR_ReadIRQ();
}
#endif  /*HAL_IR_MODULE_EN*/
/************************************Code End******************************************/