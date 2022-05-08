/***************************************************************************************
**文件名:     DRV_Finger.c
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
#include "DRV_Finger.h"

//#include "sys.h"
//#include "debug.h"
//#include "type.h"
//#include "uart.h"
//#include "gpio.h"
//#include "ClockAndPower.h"
#include "Timer.h"
#include "Flash.h"
//#include "cache.h"
#include "sensorSPI.h"
#include "algorithm_bin_interface.h"
//#include "string.h"
//#include "fsm.h"
//#include "system.h"
#if DRV_FINGER_MODULE_EN
#warning DRV_FINGER模块使能!
/*******************************宏定义***************************/
#define DRV_FINGER_DATA_LEN (30)   //接收包数据的长度
#define DRV_FINGER_REC_MIN_LEN (5) //接收包最短数据的长度
#define DRV_FINGER_REC_TIME (5)	   //接收包完成一帧的时间
#define DRV_FINGER_DELAY_MS(X) \
	while (0)                  \
	{                          \
	}									   //延时函数
#define DRV_FINGER_BLOCK_TIM_OUT (10)	   //阻塞读取超时时间
#define DRV_FINGER_PROTOCOL_START_H (0XEF) //开始标志H
#define DRV_FINGER_PROTOCOL_START_L (0X01) //开始标志L
#define DRV_FINGER_PROTOCOL_ADDR1 (0XFF)   //地址1
#define DRV_FINGER_PROTOCOL_ADDR2 (0XFF)   //地址2
#define DRV_FINGER_PROTOCOL_ADDR3 (0XFF)   //地址3
#define DRV_FINGER_PROTOCOL_ADDR4 (0XFF)   //地址4
#define DRV_FINGER_CMD_LEN (1)			   //命令长度
#define DRV_FINGER_ANSWER_BUF_BYTES (0x2C) //
#define DRV_FINGER_ANSWER_MIN_BYTES (11)   //最小包大小
#define DRV_FINGER_RX_WAIT_ACK (100)	   //等待ACK包时间
#define DRV_FINGER_BUFFER_SIZE (32)		   //buffer大小
/*************************.C变量结构声明***********************/
typedef enum
{
	eCommandPacket = 0x01, //命令包
	eDataPacket = 0x02,	   //数据包
	eAckPacket = 0x07,	   //应答包
	eEndDataPacket = 0x08, //应答包
} TYPEe_pid;			   //包标识

typedef struct
{
	uint8_t txBuf[FINGER_UART_TX_BUF_SIZE]; //发送缓存
	uint16_t txCnt;							//发送计数器
	uint16_t txLen;							//发送长度
	uint16_t txTimCnt;						//发送计时器
	uint8_t rxBuf[FINGER_UART_RX_BUF_SIZE]; //接收缓存
	uint16_t rxCnt;							//接收计数器
	uint16_t rxLen;							//接收长度
	uint16_t rxTimCnt;						//接收计时器
} TYPEs_fingerCom;							//硬件发送接收
typedef struct
{
	uint16_t start;					   //包头
	uint32_t addr;					   //地址
	uint8_t pid;					   //包标识
	uint16_t length;				   //包长度
	uint8_t data[DRV_FINGER_DATA_LEN]; //包内容
	uint16_t sum;					   //校验和
} TYPEs_fingerProtocol;				   //协议处理
typedef enum
{
	eCmdStatusExecute, //执行命令
	eCmdStatusWaiting, //等待命令
} TYPEe_CmdStatus;	   //命令状态
typedef struct
{
	TYPEs_fingerCom Com;		   //硬件发送接收
	TYPEs_fingerProtocol Protocol; //协议处理
	TYPEe_CmdStatus CmdStatus;	   //命令状态
	uint16_t OverTimeCnt;		   //超时计数器
	uint8_t buffer[DRV_FINGER_BUFFER_SIZE];
} TYPEs_fingerCtrl; //指纹控制
/*************************私有全局变量*************************/
static TYPEs_fingerCtrl fingerCtrl =
	{
		.Com.txCnt = 0,
		.Com.txLen = 0,
		.Com.txTimCnt = 0,
		.Com.rxBuf = 0,
		.Com.rxLen = 0,
		.Com.rxTimCnt = 0,
		.Protocol.start = 0,
		.Protocol.addr = 0,
		.Protocol.pid = 0,
		.Protocol.length = 0,
		.Protocol.sum = 0,
};
int tmpBuf[79 * 1024 / 4] = {0x00}; /* Buffer must >= 79k bytes */
//int tmpBuf[79 * 1024] = {0x00}; /* Buffer must >= 79k bytes */
/*************************公共全局变量*************************/
FingerIntHandler_t FingerIntHandler = NULL; //中断指针
/*************************局部函数*****************************/
#if defined(DRV_FINGER_USE_FINGER_MODULE)
/***************************************************************************************
**函数名:       DRV_Finger_PowerOn
**功能描述:     给指纹头上电
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Finger_PowerOn(void)
{
	//	M_FINGER_POW_OUT_PP();
	//	M_FINGER_POW_OUT_1();
}

/***************************************************************************************
**函数名:       DRV_Finger_RecData
**功能描述:     接收数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Finger_RecData(uint8_t dat)
{
	//	fingerCtrl.Com.rxBuf[fingerCtrl.Com.rxCnt] = dat;
	//	fingerCtrl.Com.rxCnt++;
	//	if (fingerCtrl.Com.rxCnt >= FINGER_UART_RX_BUF_SIZE) //如果超过了最大的范围
	//	{
	//		fingerCtrl.Com.rxCnt = 0; //覆盖第一个数据
	//	}
	//	fingerCtrl.Com.rxTimCnt = 0; //清除接收超时
}
/***************************************************************************************
**函数名:       DRV_Finger_UartClrError
**功能描述:     清除串口错误
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Finger_UartClrError(void)
{
	//	uint32_t error;
	//	if (NRF_UART0->EVENTS_ERROR != 0)
	//	{
	//		NRF_UART0->EVENTS_ERROR = 0;
	//		error = NRF_UART0->ERRORSRC;
	//		NRF_UART0->ERRORSRC = error;
	//		// flush the RX FIFO by reading and ignoring them
	//		while (NRF_UART0->EVENTS_RXDRDY != 0)
	//		{
	//			error = NRF_UART0->RXD;
	//			NRF_UART0->EVENTS_RXDRDY = 0;
	//			//Give a moment for more data in FIFO to move into RXD register
	//			//nrf_delay_ms(1);
	//		}
	//	}
}
/***************************************************************************************
**函数名:       DRV_Finger_UartEventHandle
**功能描述:     串口中断回调函数。
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
//static void DRV_Finger_UartEventHandle(app_uart_evt_t *p_event)
//{
//	//	static uint8_t fingerBuf; //指纹接收缓存
//	//	switch (p_event->evt_type)
//	//	{
//	//	case APP_UART_DATA_READY:
//	//		UNUSED_VARIABLE(app_uart_get(&fingerBuf));
//	//		DRV_Finger_RecData(fingerBuf);
//	//		break;
//	//	case APP_UART_COMMUNICATION_ERROR:
//	//		UNUSED_VARIABLE(app_uart_get(&fingerBuf));
//	//		DRV_Finger_UartClrError();
//	//		APP_ERROR_HANDLER(p_event->data.error_communication);
//	//		break;
//	//	case APP_UART_FIFO_ERROR:
//	//		UNUSED_VARIABLE(app_uart_get(&fingerBuf));
//	//		DRV_Finger_UartClrError();
//	//		//			APP_ERROR_HANDLER(p_event->data.error_communication);
//	//		break;
//	//	default:
//	//		break;
//	//	}
//}
/***************************************************************************************
**函数名:       DRV_Finger_UartTxData
**功能描述:     串口发送函数。
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Finger_UartTxData(uint8_t *pData, uint16_t len)
{
	//	uint32_t i;
	//	for (i = 0; i < len; i++)
	//	{
	//		while (app_uart_put(pData[i]) != NRF_SUCCESS)
	//			;
	//	}
}

/***************************************************************************************
**函数名:       DRV_Finger_UartInit
**功能描述:     指纹头串口初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Finger_UartInit(void)
{
	//	M_FINGER_RX_IN_NOPULL();
	//	M_FINGER_TX_OUT_PP();

	//	uint32_t err_code = 0;
	//	const app_uart_comm_params_t comm_params =
	//		{
	//			.rx_pin_no = M_FINGER_RX_PIN,
	//			.tx_pin_no = M_FINGER_TX_PIN,
	//			.flow_control = APP_UART_FLOW_CONTROL_DISABLED,
	//			.use_parity = false,
	//			.baud_rate = UART_BAUDRATE_BAUDRATE_Baud57600};
	//	APP_UART_FIFO_INIT(&comm_params,
	//					   FINGER_UART_RX_BUF_SIZE,
	//					   FINGER_UART_TX_BUF_SIZE,
	//					   DRV_Finger_UartEventHandle,
	//					   APP_IRQ_PRIORITY_HIGH,
	//					   err_code);
	//	APP_ERROR_CHECK(err_code);
}
/***************************************************************************************
**函数名:       DRV_Finger_UartUnblockRead
**功能描述:     非阻塞式读取数据
**输入参数:     uint8_t *pData 读取指针 uint16_t *len 长度指针
**输出参数:     
int8_t
0;		//接收成功
-1;		//一帧未接收完成
-2;		//无数据
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_UartUnblockRead(uint8_t *pData, uint16_t *len)
{
	uint8_t i;
	fingerCtrl.Com.rxTimCnt++;
	if (fingerCtrl.Com.rxTimCnt < DRV_FINGER_REC_TIME)
	{
		return -1; //一帧未接收完成
	}
	if (0 == fingerCtrl.Com.rxCnt)
	{
		return -2; //无数据
	}
	for (i = 0; i < fingerCtrl.Com.rxCnt; i++)
	{
		*(pData + i) = fingerCtrl.Com.rxBuf[i];
	}
	*len = i; //获取长度
	return 0; //接收成功
}

/***************************************************************************************
**函数名:       DRV_Finger_UartblockRead
**功能描述:     阻塞式读取数据
**输入参数:     uint8_t *pData 读取指针 uint16_t *len 长度指针 uint16_t tim 超时时间
**输出参数:     
int8_t
0;		//接收成功
-1;		//接收超时，数据无效
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_UartblockRead(uint8_t *pData, uint16_t *len, uint16_t tim)
{
	while (1)
	{
		tim--;
		DRV_FINGER_DELAY_MS(1);
		if (0 == DRV_Finger_UartUnblockRead(pData, len))
		{
			return 0;
		}
		if (0 == tim)
		{
			return -1;
		}
	}
}

/***************************************************************************************
**函数名:       DRV_Finger_UartWrite
**功能描述:     阻塞式读取数据
**输入参数:     uint8_t *pData 写入指针 uint16_t len 长度指针 
**输出参数:     
int8_t
0;		//发送成功
-1;		//长度错误
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_UartWrite(uint8_t *pData, uint16_t len)
{
	if (len > FINGER_UART_TX_BUF_SIZE)
	{
		return -1; //长度错误
	}
	DRV_Finger_UartTxData(pData, len);
	return 0; //发送成功
}
/***************************************************************************************
**函数名:       DRV_Finger_UartRxClear
**功能描述:     清除接收数据
**输入参数:     --
**输出参数:     
int8_t
0;		//接收成功
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_UartRxClear(void)
{
	memset(fingerCtrl.Com.rxBuf, 0, FINGER_UART_RX_BUF_SIZE);
	fingerCtrl.Com.rxCnt = 0;
	fingerCtrl.Com.rxLen = 0;
	fingerCtrl.Com.rxTimCnt = 0;
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Finger_Get16SumCheck
**功能描述:     获取16位的校验和
**输入参数:     
uint8_t *data, 数据指针
uint16_t len   数据长度
**输出参数:     
uint16_t 检验和
**备注:         
****************************************************************************************/
static uint16_t DRV_Finger_Get16SumCheck(uint8_t *data, uint16_t len)
{
	uint16_t sunTmp = 0;
	uint16_t i;
	for (i = 0; i < len; i++)
	{
		sunTmp += *(data + i);
	}
	return sunTmp;
}
/***************************************************************************************
**函数名:       DRV_Finger_PackDataAndWriter
**功能描述:     打包数据并发送
**输入参数:     
uint8_t pid, 包标识
uint8_t *data,包内容
uint16_t len 包长度 这里需要在打包程序中自动+2（校验和）
**输出参数:     
int8_t
0;		//发送成功
-1;		//长度错误
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_PackDataAndWriter(uint8_t *pBuf, TYPEe_pid pid, uint8_t *data, uint16_t len)
{
	uint8_t i = 0;
	uint16_t sum = 0;
	pBuf[i++] = DRV_FINGER_PROTOCOL_START_H;
	pBuf[i++] = DRV_FINGER_PROTOCOL_START_L;
	pBuf[i++] = DRV_FINGER_PROTOCOL_ADDR1;
	pBuf[i++] = DRV_FINGER_PROTOCOL_ADDR2;
	pBuf[i++] = DRV_FINGER_PROTOCOL_ADDR3;
	pBuf[i++] = DRV_FINGER_PROTOCOL_ADDR4;
	pBuf[i++] = (uint8_t)(pid);
	pBuf[i++] = (uint8_t)((len + 2) >> 8);
	pBuf[i++] = (uint8_t)(len + 2);
	memcpy(&pBuf[i], data, len);
	i += len;
	sum = DRV_Finger_Get16SumCheck(&pBuf[6], i - 6);
	pBuf[i++] = (uint8_t)(sum >> 8);
	pBuf[i++] = (uint8_t)(sum);
	DRV_Finger_UartWrite(pBuf, i); //发送数据
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Finger_RxAck
**功能描述:     接收处理
**输入参数:     
uint8_t *data,数据内容
uint16_t len
**输出参数:     
int8_t
0;	//读取成功
-1;		//错误
**备注:         
****************************************************************************************/
static int8_t DRV_Finger_RxAck(uint8_t *data, uint16_t len)
{
	uint16_t ackLen = 0;
	if (len < DRV_FINGER_ANSWER_MIN_BYTES) //长度错误
	{
		return -1; //错误
	}
	if (data[0] != DRV_FINGER_PROTOCOL_START_H)
	{
		return -1;
	}
	if (data[1] != DRV_FINGER_PROTOCOL_START_L)
	{
		return -1;
	}
	if (data[2] != DRV_FINGER_PROTOCOL_ADDR1)
	{
		return -1;
	}
	if (data[3] != DRV_FINGER_PROTOCOL_ADDR2)
	{
		return -1;
	}
	if (data[4] != DRV_FINGER_PROTOCOL_ADDR3)
	{
		return -1;
	}
	if (data[5] != DRV_FINGER_PROTOCOL_ADDR4) //检查start和addr
	{
		return -1;
	}
	if (data[6] != ((uint8_t)(eAckPacket))) //检查pid
	{
		return -1;
	}
	ackLen = (data[7] << 8) + data[8]; //获取长度
	//未作校验
	fingerCtrl.Protocol.length = ackLen;
	memcpy(fingerCtrl.Protocol.data, &data[9], ackLen - 2); //拷贝数据
	return 0;												//读取成功
}
#endif // DRV_FINGER_USE_FINGER_MODULE
#if defined(DRV_FINGER_USE_FINGER_LIB)
void DRV_Finger_HardWareInit(void)
{
	M_FING_POW_OUT_PP();
	M_FING_POW_OUT_1(); //开打指纹头电源
//	M_FING_POW_OUT_0(); //开打指纹头电源
	timerInit();
	flashInit();
	sensorSpiInit();
}
void DRV_Finger_CleanAll(void)
{

	U8 res = 0;
	U8 buffer[32] = {0x00};

	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 10;

	res = binlib_api(LIB_DELETE_ID, buffer, 4);

	if ((res & 0x01) == 0)
	{
		printf("empty is ok  id \r\n");
	}
	else
	{
		printf("empty is failed  id\r\n");
	}
}
#endif // DRV_FINGER_USE_FINGER_LIB
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_Finger_Init
**功能描述:     指纹头初始化
**输入参数:     --
**输出参数:     
int8_t
0 成功
-1 失败
**备注:         
****************************************************************************************/
int8_t DRV_Finger_Init(void)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t buff[2] = {0};
	uint32_t res = 0;
	M_FING_IRQ_IN_NOPULL();
	M_FING_IRQ_EXIT_INT_DISABLE();
	DRV_Finger_HardWareInit();
	printf("SPI FLASH ID is:%x\r\n", SPI_Flash_ReadID());
	binlib_init_data_bss_sections();
	binlib_dbg_en(1);
	tmpBuf[0] = 0x55AAEE44;
	tmpBuf[1] = 0xA5668899;
	binlib_init_buffer(tmpBuf, sizeof(tmpBuf));
	res = binlib_init(bin_funcs_table);
	if (!res)
	{
		printf("Init bin library successful\r\n");
		return 0;
	}
	else
	{
		buff[0] = 0;
		res = binlib_api(LIB_INIT_SENSOR, buff, 2);
		if (res)
		{
			printf("Init Sensor Param Failed\r\n");
			//			while (1);
			return -1;
		}
		else
		{
			printf("Init sensor successful...\r\n");
			return 0;
		}

		buff[0] = 1;
		res = binlib_api(LIB_INIT_SENSOR, buff, 2);
		if (res)
		{
			printf("Init Flash Failed\r\n");
			return -1;
			//			while (1);
		}
		else
		{
			printf("Init Flash successful...\r\n");
			return 0;
		}
	}
    #if 0
	DRV_Finger_CleanAll();
    #endif
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	DRV_Finger_PowerOn();  //给指纹头上电
	fn_delay_100us(50);	   //5ms
	DRV_Finger_UartInit(); //配置串口
#endif					   // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**函数名:       DRV_Finger_GenImg
**功能描述:     录指纹图像 GenImg
**输入参数:     
uint16_t id, 要录入的ID，再注册阶段有效
uint8_t cmd，0 注册;1 指纹对比
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_GenImg(uint16_t id, uint8_t cmd)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	memset(fingerCtrl.buffer, 0, DRV_FINGER_BUFFER_SIZE);
	if (0 == cmd)
	{
		fingerCtrl.buffer[1] = (id & 0x7F) << 1;
		//调用这句之前，可能需要关闭系统滴答时钟。
		if (1 == M_FING_IRQ_READIN()) //处理这句之前需要判断FING_IRQ的状态
		{
			DEBUG_LOG_OUT("CMD= 0 DRV_Finger_GenImg binlib_api before\n");
			res = binlib_api(LIB_PRE_ENROLL, fingerCtrl.buffer, 0);
			DEBUG_LOG_OUT("CMD= 0 DRV_Finger_GenImg binlib_api after\n");
			if ((res & 0x01) == 0)
			{
				fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
				return 0;									   //操作成功
			}
			else
			{
				return -2; //操作失败
			}
		}
		else
		{
			return -1; //操作中
		}
	}
	else if (1 == cmd)
	{
		if (1 == M_FING_IRQ_READIN()) //处理这句之前需要判断FING_IRQ的状态
		{
			DEBUG_LOG_OUT("CMD= 1 DRV_Finger_GenImg binlib_api before\n");
			res = binlib_api(LIB_PRE_MATCH, fingerCtrl.buffer, 0);
			DEBUG_LOG_OUT("CMD= 1 DRV_Finger_GenImg binlib_api after\n");
			if ((res & 0x01) == 0)
			{
				fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
				return 0;									   //操作成功
			}
			else
			{
				return -2; //操作失败
			}
		}
		else
		{
			return -1; //操作中
		}
	}
	else
	{
		return -2; //操作失败
	}
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_GenImg;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.OverTimeCnt = 0; //清除超时
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**函数名:       DRV_Finger_RegModel
**功能描述:     图像转特征
**输入参数:     
uint8_t id
特征存储区
目前分为2种：2次录入范围是{1，2}，6次录入范围是{1，2，3，4，5，6}
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:  
注意！这里的ID在6次指纹头不需要id。
****************************************************************************************/
int8_t DRV_Finger_Img2Tz(uint8_t id)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
	return 0;
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t res = 0;
	res = binlib_api(LIB_ENROLL, buffer, 2);
	if ((res & 0x01) == 0)
	{

		return 0; //操作成功
	}
	else
	{
		return -2; //操作失败
	}

	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_Img2Tz;
		cmdTmp[i++] = id;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}

/***************************************************************************************
**函数名:       DRV_Finger_RegModel
**功能描述:     特征合成模板
**输入参数:     
uint8_t id
特征存储区
目前分为2种：2次录入范围是{1，2}，6次录入范围是{1，2，3，4，5，6}
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:  
注意！这里的ID在6次指纹头不需要id。
****************************************************************************************/
int8_t DRV_Finger_RegModel(void)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	DEBUG_LOG_OUT("DRV_Finger_RegModel\n");
//	for(uint8_t i = 0;i<2;i++)
//	printf("fingerCtrl.buffer[%d]=0x%0x\r\n",i,fingerCtrl.buffer[i]);
	res = binlib_api(LIB_ENROLL, fingerCtrl.buffer, 2);
	DEBUG_LOG_OUT("DRV_Finger_RegModelA\n");
	DEBUG_LOG_OUT("res:%d\n",res);
	if ((res & 0x01) == 0)
	{
		DEBUG_LOG_OUT("DRV_Finger_RegModel Ok\n");
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
		return 0;									   //操作成功
	}
	else
	{
		return -2; //操作失败
	}
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_RegModel;
		//		cmdTmp[i++] = id;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**函数名:       DRV_Finger_Store
**功能描述:     存储模板 Store
**输入参数:     
uint8_t charBuf,		缓冲区号
uint16_t model			模板号
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_Store(uint8_t charBuf, uint16_t model)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	memset(fingerCtrl.buffer, 0, DRV_FINGER_BUFFER_SIZE);
	fingerCtrl.buffer[1] = (model & 0x7F) << 1;
	fingerCtrl.buffer[1] |= 0x01;
	res = binlib_api(LIB_ENROLL, fingerCtrl.buffer, 2);
	if ((res & 0x01) == 0)
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
		return 0;									   //操作成功
	}
	else
	{
		return -2; //操作失败
	}
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_Store;
		cmdTmp[i++] = charBuf;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**函数名:       DRV_Finger_Serach
**功能描述:     搜索指纹
**输入参数:     
uint8_t charBuf,		缓冲区号
uint16_t model			模板起始号
uint16_t num 			搜索个数
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_Serach(uint8_t charBuf, uint16_t model, uint16_t num)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	uint16_t idTmp = 0;
	res = binlib_api(LIB_MATCH, fingerCtrl.buffer, 2);
	if ((res & 0x01) == 0)
	{
		idTmp = fingerCtrl.buffer[0] * 10 + fingerCtrl.buffer[1]; //获取id号
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok;			  //指令执行完毕或OK
		fingerCtrl.Protocol.data[1] = (uint8_t)(idTmp >> 8);	  //ID H
		fingerCtrl.Protocol.data[2] = (uint8_t)(idTmp);			  //ID L
		return 0;												  //操作成功
	}
	else
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_SerachFail; //没搜索到指纹；
		return 0;
	}
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_Serach;
		cmdTmp[i++] = charBuf;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		cmdTmp[i++] = (uint8_t)(num >> 8);
		cmdTmp[i++] = (uint8_t)(num);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**函数名:       DRV_Finger_DeletChar
**功能描述:     删除模板
**输入参数:     
uint16_t model			模板号
uint16_t num 			删除个数
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_DeletChar(uint16_t model, uint16_t num)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	fingerCtrl.buffer[0] = (uint8_t)(model >> 8);
	fingerCtrl.buffer[1] = (uint8_t)(model);
	fingerCtrl.buffer[2] = (uint8_t)(num >> 8);
	fingerCtrl.buffer[3] = (uint8_t)(num);
    DEBUG_LOG_OUT("fingerCtrl.buffer:\n");
    DEBUG_LOG_OUT_HEX(fingerCtrl.buffer,4);
	res = binlib_api(LIB_DELETE_ID, fingerCtrl.buffer, 4);
	if ((res & 0x01) == 0)
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
		return 0;
		printf("del is ok  id \r\n");
	}
	else
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_DeletCharFail; //删除模板失败；
		return -2;
		printf("del is failed  id\r\n");
	}
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_DeletChar;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		cmdTmp[i++] = (uint8_t)(num >> 8);
		cmdTmp[i++] = (uint8_t)(num);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}

/***************************************************************************************
**函数名:       DRV_Finger_Empty
**功能描述:     清空指纹库
**输入参数:     --
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_Empty(void)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	return DRV_Finger_DeletChar(0,DRV_FINGER_ALL); //清除所有指纹
#endif // DRV_FINGER_USE_FINGER_LIB	
#if defined(DRV_FINGER_USE_FINGER_MODULE)	
	uint8_t cmdTmp[30] = {
		0x00,
	};
	uint8_t i = 0;
	uint8_t txRxBuf[DRV_FINGER_ANSWER_BUF_BYTES] = {
		0,
	};
	uint16_t recLen = 0;
	int8_t reTmp = -1;
	switch (fingerCtrl.CmdStatus)
	{
	case eCmdStatusExecute:
		DRV_Finger_UartRxClear(); //清除接收
		cmdTmp[i++] = DRV_FINGER_CMD_Empty;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //发送数据
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //操作成功
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //超时
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //参数错误
		break;
	}
	return reTmp; //操作中
#endif			  // DRV_FINGER_USE_FINGER_MODULE

}

/***************************************************************************************
**函数名:       DRV_Finger_GetAckData
**功能描述:     获取接收数据的指针
**输入参数:     --
**输出参数:     
int8_t *接收的指针头
**备注:         
****************************************************************************************/
uint8_t *DRV_Finger_GetAckData(void)
{
	return fingerCtrl.Protocol.data;
}
/***************************************************************************************
**函数名:       DRV_Finger_ResetCmd
**功能描述:     重置命令
**输入参数:     --
**输出参数:     
int8_t
0;		//操作成功
**备注:         
****************************************************************************************/
int8_t DRV_Finger_ResetCmd(void)
{
	//	fingerCtrl.CmdStatus = eCmdStatusExecute;
	//	fingerCtrl.OverTimeCnt = 0;
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Finger_EnterSleepConfig
**功能描述:     进入休眠状态
**输入参数:     --
**输出参数:     
**备注:         
****************************************************************************************/
void DRV_Finger_EnterSleepConfig(void)
{
	M_FING_IRQ_IN_NOPULL();
	M_FING_IRQ_EXIT_RISE_ENABLE();
	M_FINGER_SCK_OUT_PP();
	M_FINGER_SCK_OUT_0();
	M_FINGER_MOSI_OUT_PP();
	M_FINGER_MOSI_OUT_0();
	M_FINGER_SS_OUT_PP();
	M_FINGER_SS_OUT_0();
	M_FINGER_MISO_OUT_PP();
	M_FINGER_MISO_OUT_0();
	M_FING_POW_OUT_PP();
	M_FING_POW_OUT_0();
}

///***************************************************************************************
//**函数名:       DRV_Finger_InterrptService
//**功能描述:     中断服务程序
//**输入参数:     --
//**输出参数:     --
//**备注:
//****************************************************************************************/
//void DRV_Finger_InterrptService(EPORT_PINx pin)
//{
//	if(M_FINGER_IRQ_PIN == pin )
//	{
//		if (NULL !=FingerIntHandler)
//		 {
//			FingerIntHandler();
//		 }
//	 }
//}
/***************************************************************************************
**函数名:       DRV_Finger_ChkFingerLeave
**功能描述:     检查指纹是否离开
**输入参数:     --
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t DRV_Finger_ChkFingerLeave(void)
{
	#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
//	fingerCtrl.buffer[0] = (uint8_t)(0 >> 8);
//	fingerCtrl.buffer[1] = (uint8_t)(0);
//	fingerCtrl.buffer[2] = (uint8_t)(0 >> 8);
//	fingerCtrl.buffer[3] = (uint8_t)(0);
	res = binlib_api(LIB_IS_FINGERLEAVE, fingerCtrl.buffer, 0);
	if ((res & 0x01) == 0)
	{
//		DEBUG_LOG_OUT("finger leave\n");
		
//		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //指令执行完毕或OK
		return 0;
	}
	else
	{
//		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_DeletCharFail; //删除模板失败；
		return -2;
	}
#endif // DRV_FINGER_USE_FINGER_LIB
}
#endif /*DRV_FINGER_MODULE_EN*/
/************************************Code End******************************************/
