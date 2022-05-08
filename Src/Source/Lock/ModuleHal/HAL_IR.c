/***************************************************************************************
**�ļ���:     HAL_IR.c
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
#include "HAL_IR.h"
#include "DRV_IR.h"
#if  HAL_IR_MODULE_EN
#warning HAL_IRģ��ʹ��!
/*******************************�궨��***************************/  

/*************************.C�����ṹ����***********************/  

/*************************˽��ȫ�ֱ���*************************/  
static TYPEs_IRTrans irTrans=
{
	.rxCnt =0,
	.rxTim = 0,
	.rxBuf={0,},
	.txCnt = 0,
	.txBuf={0,},
	.dataLen =0,
};
/*************************����ȫ�ֱ���*************************/  
TYPEs_IRTransPointer irTransPointer=
{
	.rxCnt = &irTrans.rxCnt,
	.rxBuf = irTrans.rxBuf,
};
const uint8_t irCmd[11][8]=
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// NULL
	{0xAA,0xAA,0x02,0x07,0x00,0x09,0x55,0x55},//N101=0,����ģʽ,1000
	{0xAA,0xAA,0x02,0x07,0x02,0x0b,0x55,0x55},//N102=0,IOģʽ,1000
	{0xAA,0xAA,0x02,0x03,0x01,0x06,0x55,0x55},//N103=0,�̾���,1000
	{0xAA,0xAA,0x02,0x03,0x02,0x07,0x55,0x55},//N104=0,�о���,1000
	{0xAA,0xAA,0x02,0x03,0x03,0x08,0x55,0x55},//N105=0,������,1000
	{0xAA,0xAA,0x02,0x03,0x00,0x05,0x55,0x55},//N106=0,���̾�,1000
	{0xAA,0xAA,0x01,0x30,0x00,0x31,0x55,0x55},//N107=0,��ѯAD,1000
	//{0xAA,0xAA,0x02,0x05,0xC8,0xCF,0x55,0x55}, //200//N108=0,����AD���,1000
	{0xAA,0xAA,0x02,0x05,0x64,0x6B,0x55,0x55}, //100
	{0xAA,0xAA,0x02,0x05,0x19,0x20,0x55,0x55}, //25//N109=0,����AD��С,1000
	{0xAA,0xAA,0x01,0x05,0x00,0x06,0x55,0x55},//N110=0,��ADֵ,1000
};

TYPEs_IRCtrl irCtrl=
{
	.irState= eIrStateNone,
	.OverTimeCnt = 0,
	.cmd = eIrCmdNone,
};
/*************************�ֲ�����*****************************/  


/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       HAL_IR_IOInit
**��������:     IO��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_IR_IOInit(void)
{
	DRV_IR_IOInit();
}

/***************************************************************************************
**������:       HAL_IR_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_IR_Init(void)
{
    DRV_IR_Init();
}
/***************************************************************************************
**������:       HAL_IR_RecvDataUnblock
**��������:     ������ʽ��������
**�������:     --
**�������:     
int8_t
1; //�������
0; //���ݽ�����
-1; //��������
-2; //û������
-3; //���ղ�������
-4; //δ��ʼ����
**��ע:         
****************************************************************************************/
int8_t HAL_IR_RecvDataUnblock(void)
{
	return DRV_IR_RecvDataUnblock(&irTrans,DRV_IR_RX_MAX_TIME);
}
/***************************************************************************************
**������:       HAL_IR_SendData
**��������:     ��������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_IR_SendData(uint8_t *sendBuf, uint32_t len)
{
	DRV_IR_SendData(sendBuf,len);
}

/***************************************************************************************
**������:       DRV_BleComm_ClearRxPar
**��������:     �������
**�������:     --
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t  HAL_IR_ClearRxPar(void)
{
	return DRV_IR_ClearRxPar(&irTrans);
}
/***************************************************************************************
**������:       DRV_BleComm_ClearRxPar
**��������:     �������
**�������:     --
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
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
**������:       HAL_IR_Set
**��������:     IR����
**�������:     
TYPEe_IrCmd cmd, ָ��
uint32_t overTimeMax ��ʱʱ��
**�������:     
TYPEe_IrState
**��ע:         
****************************************************************************************/
TYPEe_IrState HAL_IR_Set(TYPEe_IrCmd cmd,uint32_t overTimeMax)
{
	static uint8_t tmpTab[8]={0,};
	if(cmd != irCtrl.cmd)
	{
		return eIrStateCmdError;
	}
	if (0 != irCtrl.OverTimeCnt) //�Զ�������
    {
        if (1 == irCtrl.OverTimeCnt)
        {
            irCtrl.irState = eIrStateTimeover; //��ʱ
        }
        irCtrl.OverTimeCnt--;
    }
	switch (irCtrl.irState)
    {
    	case eIrStateNone:
    		break;
    	case eIrStateReady:
			memcpy(tmpTab,&irCmd[(uint8_t)cmd][0],8);
			HAL_IR_SendData(tmpTab,8); //����ָ��
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
					irCtrl.irState = eIrStateSuccess; //�ɹ�
				}
				else
				{
					irCtrl.retry --;
					if(irCtrl.retry == 0)
					{
						DEBUG_LOG_OUT("eIrStateFail\n");
						irCtrl.irState = eIrStateFail; //ʧ��
					}
					else
					{
						DEBUG_LOG_OUT("eIrStateReady\n");
						irCtrl.irState = eIrStateReady; //����
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
**������:       HAL_IR_SetDistanceStart
**��������:     IR���þ���
**�������:     --
**�������: 
int8_t
**��ע:         
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
	irCtrl.irSetDistance = eIrSetDistanceUartMode; //����״̬��
	return 0;
}
/***************************************************************************************
**������:       HAL_IR_SetDistance
**��������:     IR���þ���
**�������:     --
**�������:     
**��ע:       
���ò��裺
��ģ�����óɴ���ģʽ��
���þ��롣
�ָ���IOģʽ��
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
**������:       HAL_IR_UartColse
**��������:     ����ģʽ�ر�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_IR_UartColse(void)
{
	DRV_IR_UartColse();
}

/***************************************************************************************
**������:       HAL_IR_ReadIRQ
**��������:     ��ȡIO
**�������:     --
**�������:     
uint8_t
**��ע:         
****************************************************************************************/
uint8_t HAL_IR_ReadIRQ(void)
{
	return DRV_IR_ReadIRQ();
}
#endif  /*HAL_IR_MODULE_EN*/
/************************************Code End******************************************/