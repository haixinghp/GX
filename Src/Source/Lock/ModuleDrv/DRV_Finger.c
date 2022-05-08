/***************************************************************************************
**�ļ���:     DRV_Finger.c
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
#warning DRV_FINGERģ��ʹ��!
/*******************************�궨��***************************/
#define DRV_FINGER_DATA_LEN (30)   //���հ����ݵĳ���
#define DRV_FINGER_REC_MIN_LEN (5) //���հ�������ݵĳ���
#define DRV_FINGER_REC_TIME (5)	   //���հ����һ֡��ʱ��
#define DRV_FINGER_DELAY_MS(X) \
	while (0)                  \
	{                          \
	}									   //��ʱ����
#define DRV_FINGER_BLOCK_TIM_OUT (10)	   //������ȡ��ʱʱ��
#define DRV_FINGER_PROTOCOL_START_H (0XEF) //��ʼ��־H
#define DRV_FINGER_PROTOCOL_START_L (0X01) //��ʼ��־L
#define DRV_FINGER_PROTOCOL_ADDR1 (0XFF)   //��ַ1
#define DRV_FINGER_PROTOCOL_ADDR2 (0XFF)   //��ַ2
#define DRV_FINGER_PROTOCOL_ADDR3 (0XFF)   //��ַ3
#define DRV_FINGER_PROTOCOL_ADDR4 (0XFF)   //��ַ4
#define DRV_FINGER_CMD_LEN (1)			   //�����
#define DRV_FINGER_ANSWER_BUF_BYTES (0x2C) //
#define DRV_FINGER_ANSWER_MIN_BYTES (11)   //��С����С
#define DRV_FINGER_RX_WAIT_ACK (100)	   //�ȴ�ACK��ʱ��
#define DRV_FINGER_BUFFER_SIZE (32)		   //buffer��С
/*************************.C�����ṹ����***********************/
typedef enum
{
	eCommandPacket = 0x01, //�����
	eDataPacket = 0x02,	   //���ݰ�
	eAckPacket = 0x07,	   //Ӧ���
	eEndDataPacket = 0x08, //Ӧ���
} TYPEe_pid;			   //����ʶ

typedef struct
{
	uint8_t txBuf[FINGER_UART_TX_BUF_SIZE]; //���ͻ���
	uint16_t txCnt;							//���ͼ�����
	uint16_t txLen;							//���ͳ���
	uint16_t txTimCnt;						//���ͼ�ʱ��
	uint8_t rxBuf[FINGER_UART_RX_BUF_SIZE]; //���ջ���
	uint16_t rxCnt;							//���ռ�����
	uint16_t rxLen;							//���ճ���
	uint16_t rxTimCnt;						//���ռ�ʱ��
} TYPEs_fingerCom;							//Ӳ�����ͽ���
typedef struct
{
	uint16_t start;					   //��ͷ
	uint32_t addr;					   //��ַ
	uint8_t pid;					   //����ʶ
	uint16_t length;				   //������
	uint8_t data[DRV_FINGER_DATA_LEN]; //������
	uint16_t sum;					   //У���
} TYPEs_fingerProtocol;				   //Э�鴦��
typedef enum
{
	eCmdStatusExecute, //ִ������
	eCmdStatusWaiting, //�ȴ�����
} TYPEe_CmdStatus;	   //����״̬
typedef struct
{
	TYPEs_fingerCom Com;		   //Ӳ�����ͽ���
	TYPEs_fingerProtocol Protocol; //Э�鴦��
	TYPEe_CmdStatus CmdStatus;	   //����״̬
	uint16_t OverTimeCnt;		   //��ʱ������
	uint8_t buffer[DRV_FINGER_BUFFER_SIZE];
} TYPEs_fingerCtrl; //ָ�ƿ���
/*************************˽��ȫ�ֱ���*************************/
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
/*************************����ȫ�ֱ���*************************/
FingerIntHandler_t FingerIntHandler = NULL; //�ж�ָ��
/*************************�ֲ�����*****************************/
#if defined(DRV_FINGER_USE_FINGER_MODULE)
/***************************************************************************************
**������:       DRV_Finger_PowerOn
**��������:     ��ָ��ͷ�ϵ�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_Finger_PowerOn(void)
{
	//	M_FINGER_POW_OUT_PP();
	//	M_FINGER_POW_OUT_1();
}

/***************************************************************************************
**������:       DRV_Finger_RecData
**��������:     ��������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_Finger_RecData(uint8_t dat)
{
	//	fingerCtrl.Com.rxBuf[fingerCtrl.Com.rxCnt] = dat;
	//	fingerCtrl.Com.rxCnt++;
	//	if (fingerCtrl.Com.rxCnt >= FINGER_UART_RX_BUF_SIZE) //������������ķ�Χ
	//	{
	//		fingerCtrl.Com.rxCnt = 0; //���ǵ�һ������
	//	}
	//	fingerCtrl.Com.rxTimCnt = 0; //������ճ�ʱ
}
/***************************************************************************************
**������:       DRV_Finger_UartClrError
**��������:     ������ڴ���
**�������:     --
**�������:     --
**��ע:         
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
**������:       DRV_Finger_UartEventHandle
**��������:     �����жϻص�������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
//static void DRV_Finger_UartEventHandle(app_uart_evt_t *p_event)
//{
//	//	static uint8_t fingerBuf; //ָ�ƽ��ջ���
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
**������:       DRV_Finger_UartTxData
**��������:     ���ڷ��ͺ�����
**�������:     --
**�������:     --
**��ע:         
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
**������:       DRV_Finger_UartInit
**��������:     ָ��ͷ���ڳ�ʼ��
**�������:     --
**�������:     --
**��ע:         
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
**������:       DRV_Finger_UartUnblockRead
**��������:     ������ʽ��ȡ����
**�������:     uint8_t *pData ��ȡָ�� uint16_t *len ����ָ��
**�������:     
int8_t
0;		//���ճɹ�
-1;		//һ֡δ�������
-2;		//������
**��ע:         
****************************************************************************************/
static int8_t DRV_Finger_UartUnblockRead(uint8_t *pData, uint16_t *len)
{
	uint8_t i;
	fingerCtrl.Com.rxTimCnt++;
	if (fingerCtrl.Com.rxTimCnt < DRV_FINGER_REC_TIME)
	{
		return -1; //һ֡δ�������
	}
	if (0 == fingerCtrl.Com.rxCnt)
	{
		return -2; //������
	}
	for (i = 0; i < fingerCtrl.Com.rxCnt; i++)
	{
		*(pData + i) = fingerCtrl.Com.rxBuf[i];
	}
	*len = i; //��ȡ����
	return 0; //���ճɹ�
}

/***************************************************************************************
**������:       DRV_Finger_UartblockRead
**��������:     ����ʽ��ȡ����
**�������:     uint8_t *pData ��ȡָ�� uint16_t *len ����ָ�� uint16_t tim ��ʱʱ��
**�������:     
int8_t
0;		//���ճɹ�
-1;		//���ճ�ʱ��������Ч
**��ע:         
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
**������:       DRV_Finger_UartWrite
**��������:     ����ʽ��ȡ����
**�������:     uint8_t *pData д��ָ�� uint16_t len ����ָ�� 
**�������:     
int8_t
0;		//���ͳɹ�
-1;		//���ȴ���
**��ע:         
****************************************************************************************/
static int8_t DRV_Finger_UartWrite(uint8_t *pData, uint16_t len)
{
	if (len > FINGER_UART_TX_BUF_SIZE)
	{
		return -1; //���ȴ���
	}
	DRV_Finger_UartTxData(pData, len);
	return 0; //���ͳɹ�
}
/***************************************************************************************
**������:       DRV_Finger_UartRxClear
**��������:     �����������
**�������:     --
**�������:     
int8_t
0;		//���ճɹ�
**��ע:         
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
**������:       DRV_Finger_Get16SumCheck
**��������:     ��ȡ16λ��У���
**�������:     
uint8_t *data, ����ָ��
uint16_t len   ���ݳ���
**�������:     
uint16_t �����
**��ע:         
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
**������:       DRV_Finger_PackDataAndWriter
**��������:     ������ݲ�����
**�������:     
uint8_t pid, ����ʶ
uint8_t *data,������
uint16_t len ������ ������Ҫ�ڴ���������Զ�+2��У��ͣ�
**�������:     
int8_t
0;		//���ͳɹ�
-1;		//���ȴ���
**��ע:         
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
	DRV_Finger_UartWrite(pBuf, i); //��������
	return 0;
}
/***************************************************************************************
**������:       DRV_Finger_RxAck
**��������:     ���մ���
**�������:     
uint8_t *data,��������
uint16_t len
**�������:     
int8_t
0;	//��ȡ�ɹ�
-1;		//����
**��ע:         
****************************************************************************************/
static int8_t DRV_Finger_RxAck(uint8_t *data, uint16_t len)
{
	uint16_t ackLen = 0;
	if (len < DRV_FINGER_ANSWER_MIN_BYTES) //���ȴ���
	{
		return -1; //����
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
	if (data[5] != DRV_FINGER_PROTOCOL_ADDR4) //���start��addr
	{
		return -1;
	}
	if (data[6] != ((uint8_t)(eAckPacket))) //���pid
	{
		return -1;
	}
	ackLen = (data[7] << 8) + data[8]; //��ȡ����
	//δ��У��
	fingerCtrl.Protocol.length = ackLen;
	memcpy(fingerCtrl.Protocol.data, &data[9], ackLen - 2); //��������
	return 0;												//��ȡ�ɹ�
}
#endif // DRV_FINGER_USE_FINGER_MODULE
#if defined(DRV_FINGER_USE_FINGER_LIB)
void DRV_Finger_HardWareInit(void)
{
	M_FING_POW_OUT_PP();
	M_FING_POW_OUT_1(); //����ָ��ͷ��Դ
//	M_FING_POW_OUT_0(); //����ָ��ͷ��Դ
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
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_Finger_Init
**��������:     ָ��ͷ��ʼ��
**�������:     --
**�������:     
int8_t
0 �ɹ�
-1 ʧ��
**��ע:         
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
	DRV_Finger_PowerOn();  //��ָ��ͷ�ϵ�
	fn_delay_100us(50);	   //5ms
	DRV_Finger_UartInit(); //���ô���
#endif					   // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**������:       DRV_Finger_GenImg
**��������:     ¼ָ��ͼ�� GenImg
**�������:     
uint16_t id, Ҫ¼���ID����ע��׶���Ч
uint8_t cmd��0 ע��;1 ָ�ƶԱ�
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
****************************************************************************************/
int8_t DRV_Finger_GenImg(uint16_t id, uint8_t cmd)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	memset(fingerCtrl.buffer, 0, DRV_FINGER_BUFFER_SIZE);
	if (0 == cmd)
	{
		fingerCtrl.buffer[1] = (id & 0x7F) << 1;
		//�������֮ǰ��������Ҫ�ر�ϵͳ�δ�ʱ�ӡ�
		if (1 == M_FING_IRQ_READIN()) //�������֮ǰ��Ҫ�ж�FING_IRQ��״̬
		{
			DEBUG_LOG_OUT("CMD= 0 DRV_Finger_GenImg binlib_api before\n");
			res = binlib_api(LIB_PRE_ENROLL, fingerCtrl.buffer, 0);
			DEBUG_LOG_OUT("CMD= 0 DRV_Finger_GenImg binlib_api after\n");
			if ((res & 0x01) == 0)
			{
				fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
				return 0;									   //�����ɹ�
			}
			else
			{
				return -2; //����ʧ��
			}
		}
		else
		{
			return -1; //������
		}
	}
	else if (1 == cmd)
	{
		if (1 == M_FING_IRQ_READIN()) //�������֮ǰ��Ҫ�ж�FING_IRQ��״̬
		{
			DEBUG_LOG_OUT("CMD= 1 DRV_Finger_GenImg binlib_api before\n");
			res = binlib_api(LIB_PRE_MATCH, fingerCtrl.buffer, 0);
			DEBUG_LOG_OUT("CMD= 1 DRV_Finger_GenImg binlib_api after\n");
			if ((res & 0x01) == 0)
			{
				fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
				return 0;									   //�����ɹ�
			}
			else
			{
				return -2; //����ʧ��
			}
		}
		else
		{
			return -1; //������
		}
	}
	else
	{
		return -2; //����ʧ��
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_GenImg;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.OverTimeCnt = 0; //�����ʱ
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**������:       DRV_Finger_RegModel
**��������:     ͼ��ת����
**�������:     
uint8_t id
�����洢��
Ŀǰ��Ϊ2�֣�2��¼�뷶Χ��{1��2}��6��¼�뷶Χ��{1��2��3��4��5��6}
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:  
ע�⣡�����ID��6��ָ��ͷ����Ҫid��
****************************************************************************************/
int8_t DRV_Finger_Img2Tz(uint8_t id)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
	return 0;
#endif // DRV_FINGER_USE_FINGER_LIB
#if defined(DRV_FINGER_USE_FINGER_MODULE)
	uint8_t res = 0;
	res = binlib_api(LIB_ENROLL, buffer, 2);
	if ((res & 0x01) == 0)
	{

		return 0; //�����ɹ�
	}
	else
	{
		return -2; //����ʧ��
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_Img2Tz;
		cmdTmp[i++] = id;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}

/***************************************************************************************
**������:       DRV_Finger_RegModel
**��������:     �����ϳ�ģ��
**�������:     
uint8_t id
�����洢��
Ŀǰ��Ϊ2�֣�2��¼�뷶Χ��{1��2}��6��¼�뷶Χ��{1��2��3��4��5��6}
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:  
ע�⣡�����ID��6��ָ��ͷ����Ҫid��
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
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
		return 0;									   //�����ɹ�
	}
	else
	{
		return -2; //����ʧ��
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_RegModel;
		//		cmdTmp[i++] = id;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**������:       DRV_Finger_Store
**��������:     �洢ģ�� Store
**�������:     
uint8_t charBuf,		��������
uint16_t model			ģ���
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
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
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
		return 0;									   //�����ɹ�
	}
	else
	{
		return -2; //����ʧ��
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_Store;
		cmdTmp[i++] = charBuf;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**������:       DRV_Finger_Serach
**��������:     ����ָ��
**�������:     
uint8_t charBuf,		��������
uint16_t model			ģ����ʼ��
uint16_t num 			��������
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
****************************************************************************************/
int8_t DRV_Finger_Serach(uint8_t charBuf, uint16_t model, uint16_t num)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	uint8_t res = 0;
	uint16_t idTmp = 0;
	res = binlib_api(LIB_MATCH, fingerCtrl.buffer, 2);
	if ((res & 0x01) == 0)
	{
		idTmp = fingerCtrl.buffer[0] * 10 + fingerCtrl.buffer[1]; //��ȡid��
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok;			  //ָ��ִ����ϻ�OK
		fingerCtrl.Protocol.data[1] = (uint8_t)(idTmp >> 8);	  //ID H
		fingerCtrl.Protocol.data[2] = (uint8_t)(idTmp);			  //ID L
		return 0;												  //�����ɹ�
	}
	else
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_SerachFail; //û������ָ�ƣ�
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_Serach;
		cmdTmp[i++] = charBuf;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		cmdTmp[i++] = (uint8_t)(num >> 8);
		cmdTmp[i++] = (uint8_t)(num);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}
/***************************************************************************************
**������:       DRV_Finger_DeletChar
**��������:     ɾ��ģ��
**�������:     
uint16_t model			ģ���
uint16_t num 			ɾ������
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
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
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
		return 0;
		printf("del is ok  id \r\n");
	}
	else
	{
		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_DeletCharFail; //ɾ��ģ��ʧ�ܣ�
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_DeletChar;
		cmdTmp[i++] = (uint8_t)(model >> 8);
		cmdTmp[i++] = (uint8_t)(model);
		cmdTmp[i++] = (uint8_t)(num >> 8);
		cmdTmp[i++] = (uint8_t)(num);
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE
}

/***************************************************************************************
**������:       DRV_Finger_Empty
**��������:     ���ָ�ƿ�
**�������:     --
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
****************************************************************************************/
int8_t DRV_Finger_Empty(void)
{
#if defined(DRV_FINGER_USE_FINGER_LIB)
	return DRV_Finger_DeletChar(0,DRV_FINGER_ALL); //�������ָ��
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
		DRV_Finger_UartRxClear(); //�������
		cmdTmp[i++] = DRV_FINGER_CMD_Empty;
		DRV_Finger_PackDataAndWriter(fingerCtrl.Com.txBuf, eCommandPacket, cmdTmp, i); //��������
		fingerCtrl.CmdStatus = eCmdStatusWaiting;
		fingerCtrl.OverTimeCnt = 0;
		break;
	case eCmdStatusWaiting:
		if (0 == DRV_Finger_UartUnblockRead(txRxBuf, &recLen))
		{
			DRV_Finger_RxAck(txRxBuf, recLen);
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = 0; //�����ɹ�
		}
		else if (fingerCtrl.OverTimeCnt >= DRV_FINGER_RX_WAIT_ACK)
		{
			fingerCtrl.CmdStatus = eCmdStatusExecute;
			reTmp = -2; //��ʱ
		}
		else
		{
			fingerCtrl.OverTimeCnt++;
		}
		break;
	default:
		fingerCtrl.CmdStatus = eCmdStatusExecute;
		reTmp = -3; //��������
		break;
	}
	return reTmp; //������
#endif			  // DRV_FINGER_USE_FINGER_MODULE

}

/***************************************************************************************
**������:       DRV_Finger_GetAckData
**��������:     ��ȡ�������ݵ�ָ��
**�������:     --
**�������:     
int8_t *���յ�ָ��ͷ
**��ע:         
****************************************************************************************/
uint8_t *DRV_Finger_GetAckData(void)
{
	return fingerCtrl.Protocol.data;
}
/***************************************************************************************
**������:       DRV_Finger_ResetCmd
**��������:     ��������
**�������:     --
**�������:     
int8_t
0;		//�����ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_Finger_ResetCmd(void)
{
	//	fingerCtrl.CmdStatus = eCmdStatusExecute;
	//	fingerCtrl.OverTimeCnt = 0;
	return 0;
}
/***************************************************************************************
**������:       DRV_Finger_EnterSleepConfig
**��������:     ��������״̬
**�������:     --
**�������:     
**��ע:         
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
//**������:       DRV_Finger_InterrptService
//**��������:     �жϷ������
//**�������:     --
//**�������:     --
//**��ע:
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
**������:       DRV_Finger_ChkFingerLeave
**��������:     ���ָ���Ƿ��뿪
**�������:     --
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
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
		
//		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_Ok; //ָ��ִ����ϻ�OK
		return 0;
	}
	else
	{
//		fingerCtrl.Protocol.data[0] = DRV_FINGER_R_DeletCharFail; //ɾ��ģ��ʧ�ܣ�
		return -2;
	}
#endif // DRV_FINGER_USE_FINGER_LIB
}
#endif /*DRV_FINGER_MODULE_EN*/
/************************************Code End******************************************/
