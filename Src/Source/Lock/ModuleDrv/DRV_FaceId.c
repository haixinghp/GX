/***************************************************************************************
**�ļ���:     DRV_FaceId.c
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
#include "DRV_FaceId.h"
#include "uart_drv.h"
#include "usi_drv.h"
#ifdef DRV_FACE_ID_ENCRYPT_EN //����ʹ��
#include "rijndael.h"
#include "MD5.h"
#endif

#if DRV_FACE_ID_MODULE_EN
#warning DRV_FACE_IDģ��ʹ��!
/*******************************�궨��***************************/
#define DRV_FACE_ID_RX_BUF_SIZE (2048)
#define DRV_FACE_ID_TX_BUF_SIZE (512) //���ͻ�����
#define DRV_FACE_ID_RX_MAX_TIME (2) //һ֡���ʱ��
#define DRV_FACE_ID_RX_MIN_LEN (6)  //һ֡��̳���
#define DRV_FACE_ID_UART (SCI3)     //ʹ�ô���3
#define DRV_FACE_ID_SYNCWORD_H (0xEF)
#define DRV_FACE_ID_SYNCWORD_L (0xAA)
#define DRV_FACE_ID_SYNCWORD_ADDR (0)
#define DRV_FACE_ID_SYNCWORD_LEN (2)
#define DRV_FACE_ID_MID_ID_ADDR (DRV_FACE_ID_SYNCWORD_ADDR + DRV_FACE_ID_SYNCWORD_LEN)
#define DRV_FACE_ID_MID_ID_LEN (1)
#define DRV_FACE_ID_SIZE_ADDR (DRV_FACE_ID_MID_ID_ADDR + DRV_FACE_ID_MID_ID_LEN)
#define DRV_FACE_ID_SIZE_LEN (2)
#define DRV_FACE_ID_DATA_ADDR (DRV_FACE_ID_SIZE_ADDR + DRV_FACE_ID_SIZE_LEN)
#define DRV_FACE_ID_REPLY_MID_ADDR (DRV_FACE_ID_DATA_ADDR)
#define DRV_FACE_ID_REPLY_MID_LEN (1)
#define DRV_FACE_ID_REPLY_RESULT_ADDR (DRV_FACE_ID_REPLY_MID_ADDR + DRV_FACE_ID_REPLY_MID_LEN)
#define DRV_FACE_ID_REPLY_RESULT_LEN (1)
#define DRV_FACE_ID_REPLY_DATA_ADDR (DRV_FACE_ID_REPLY_RESULT_ADDR + DRV_FACE_ID_REPLY_RESULT_LEN)
#define DRV_FACE_ID_NOTE_NID_ADDR (DRV_FACE_ID_DATA_ADDR)
#define DRV_FACE_ID_NOTE_NID_LEN (1)
#define DRV_FACE_ID_NOTE_DATA_ADDR (DRV_FACE_ID_NOTE_NID_ADDR + DRV_FACE_ID_NOTE_NID_LEN)
#define DRV_FACE_ID_SW_ID_SIZE (DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_MID_ID_LEN + DRV_FACE_ID_SIZE_LEN) //SyncWord	MsgID	Size�ܺ�
#define DRV_FACE_ID_PARITYCHECK_LEN (1)
#ifdef DRV_FACE_ID_ENCRYPT_EN                                                                //����ʹ��
#define DRV_FACE_ID_ENCRYPT_SIZE_ADDR (DRV_FACE_ID_SYNCWORD_ADDR + DRV_FACE_ID_SYNCWORD_LEN) //����Body Size
#define DRV_FACE_ID_ENCRYPT_SIZE_LEN (2)
#define DRV_FACE_ID_ENCRYPT_ALIGN (16) //��16����
#define DRV_FACE_ID_ENCRYPT_DATA_ADDR (DRV_FACE_ID_ENCRYPT_SIZE_ADDR + DRV_FACE_ID_ENCRYPT_SIZE_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_MID_ADDR (DRV_FACE_ID_ENCRYPT_DATA_ADDR)
#define DRV_FACE_ID_ENCRYPT_M2H_MID_LEN (1)
#define DRV_FACE_ID_ENCRYPT_M2H_SIZE_ADDR (DRV_FACE_ID_ENCRYPT_REPLY_MID_ADDR + DRV_FACE_ID_ENCRYPT_REPLY_MID_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_SIZE_LEN (2)
#define DRV_FACE_ID_ENCRYPT_M2H_DATA_ADDR (DRV_FACE_ID_ENCRYPT_REPLY_SIZE_ADDR + DRV_FACE_ID_ENCRYPT_REPLY_SIZE_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_SW_ID_SIZE (DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_ENCRYPT_SIZE_LEN)
#define DRV_FACE_ID_AES_KEY_SIZE (16) //����ĳ���
#endif

/*************************.C�����ṹ����***********************/
typedef struct
{
    uint32_t rxCnt;                         //���ռ�����
    uint32_t rxTim;                         //���ռ�ʱ��
    uint8_t rxBuf[DRV_FACE_ID_RX_BUF_SIZE]; //���ջ�����
    uint32_t txCnt;                         //���ͼ�����
    uint8_t txBuf[DRV_FACE_ID_TX_BUF_SIZE]; //���ͻ�����
} TYPEs_Com;
typedef struct
{
    uint16_t syncWord;   //�̶�����Ϣ��ͷͬ����0xEF 0xAA
    uint16_t bodySize;   //�����¼������ݰ����ȣ�MsgID+ Size+ Size��һ��Ϊ16������SizeΪ��Ч���ݳ��ȣ�
    uint8_t msgID;       //��ϢID (����RESET)
    uint16_t size;       //Data size, ��λbyte
    uint8_t *data;       //��Ϣ��Ӧ��data, ��command��Ϣ��Ӧ�Ĳ����� 65535>=N>=0��N=0��ʾ����Ϣ�޲�����
    uint8_t parityCheck; //Э�����żУ���룬���㷽ʽΪ����Э���ȥSync Word���ֺ������ֽڰ�λ��XOR���㡣
} TYPEs_FaceProtocol;
typedef struct
{
    TYPEs_Com com;                   //Ӳ��ͨѶ
//    TYPEs_FaceProtocol faceProtocol; //ͨѶЭ��
} TYPEs_FaceCom;

/*************************˽��ȫ�ֱ���*************************/
static TYPEs_FaceCom faceCom = {

}; //δ��ʼ��
static UART_TypeDef *faceUart = DRV_FACE_ID_UART;
#ifdef DRV_FACE_ID_ENCRYPT_EN //����ʹ��
//static FaceIdEncrypt_t faceIdEncrypt = NULL;
static uint8_t FaceIdAesKey[DRV_FACE_ID_AES_KEY_SIZE] = {
    0x00,
};                                                                                                 //AES����
static const uint8_t FaceIdAesExtract[16] = {6, 18, 7, 3, 13, 13, 23, 4, 8, 1, 0, 25, 9, 2, 2, 7}; //˫��Э���� size Ϊ 32 ���ַ����а�ָ�����ɳ�ȡ 16 ��byte
#endif
/*************************����ȫ�ֱ���*************************/
//IntHandler_t FaceIntHendler = NULL; 
/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       DRV_FaceId_RecvDatClear
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         ��������ǰ����Ҫ����ղ���.
****************************************************************************************/
static int8_t DRV_FaceId_RecvDatClear(void)
{
    faceCom.com.rxCnt = 0;
    faceCom.com.rxTim = 0;
    memset(faceCom.com.rxBuf, 0, DRV_FACE_ID_RX_BUF_SIZE); //��ս�����
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_SyncWordSet
**��������:     ��Ϣ��ͷ����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_SyncWordSet(void)
{
    faceCom.com.txBuf[DRV_FACE_ID_SYNCWORD_ADDR] = DRV_FACE_ID_SYNCWORD_H;
    faceCom.com.txBuf[DRV_FACE_ID_SYNCWORD_ADDR + 1] = DRV_FACE_ID_SYNCWORD_L;
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_MsgIdSet
**��������:     ��ϢID����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_MsgIdSet(uint8_t cmd)
{
    faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR] = cmd;
    faceCom.com.txCnt = DRV_FACE_ID_DATA_ADDR; //�Զ�������DATA��
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_DataAdd(uint8_t dat)
{
    faceCom.com.txBuf[faceCom.com.txCnt] = dat;
    faceCom.com.txCnt++;
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_DataAddBatch(uint8_t *dat, uint32_t len)
{
    memcpy(&faceCom.com.txBuf[faceCom.com.txCnt], dat, len);
    faceCom.com.txCnt += len;
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_SizeSet(void)
{
    uint16_t lenTmp = 0;
    lenTmp = faceCom.com.txCnt - DRV_FACE_ID_SW_ID_SIZE;
    faceCom.com.txBuf[DRV_FACE_ID_SIZE_ADDR] = (uint8_t)(lenTmp >> 8);
    faceCom.com.txBuf[DRV_FACE_ID_SIZE_ADDR + 1] = (uint8_t)(lenTmp);
    return 0;
}
/***************************************************************************************
**������:       DRV_FaceId_ClearTxPar
**��������:     ��շ��Ͳ���
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_ClearTxPar(void)
{
    memset(faceCom.com.txBuf, 0, DRV_FACE_ID_SIZE_ADDR);
    faceCom.com.txCnt = 0;
}
/***************************************************************************************
**������:       DRV_FaceId_GetOrChk
**��������:     ��ȡOR�Ľ��
**�������:     
uint8_t *dat, ����
uint16_t len ����
**�������:     
uint8_t
���
**��ע:         
****************************************************************************************/
static uint8_t DRV_FaceId_GetOrChk(uint8_t *dat, uint16_t len)
{
    uint16_t i;
    uint8_t chkTmp = NULL;
    if (NULL == dat)
    {
        return 0; //����
    }
    chkTmp = *dat;
    for (i = 1; i < len; i++)
    {
        if (NULL == (dat + i))
        {
            return 0; //����
        }
        chkTmp ^= *(dat + i);
    }
    return chkTmp;
}
/***************************************************************************************
**������:       DRV_FaceId_SendData
**��������:     ������������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_FaceId_SendData(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(faceUart, sendBuf, len);
}
/***************************************************************************************
**������:       DRV_FaceId_GetRecvData
**��������:     ��ȡ���յ�����
**�������:   
uint32_t addr, ��ȡ��ַ  
uint8_t *readDat,����ָ��
uint32_t len�����ݳ���
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_GetRecvData(uint32_t addr, uint8_t *readDat, uint32_t len)
{
    if (NULL == readDat)
    {
        return -1; //��������
    }
    memcpy(readDat, &faceCom.com.rxBuf[addr], len); //��������
    return 0;                                       //�ɹ�
}
#ifdef DRV_FACE_ID_ENCRYPT_EN //����ʹ��
/***************************************************************************************
**������:       DRV_FaceId_MoveData
**��������:     �����ƶ�
**�������:     
uint8_t *dat, //����ָ��
uint16_t size, //���ݴ�С
int16_t offset //����ƫ����
**�������:     
int8_t
0; //�ɹ�
-1; //�ڴ����
**��ע:       PS.��֤���ݲ����  
****************************************************************************************/
static int8_t DRV_FaceId_MoveData(uint8_t *dat, int32_t size, int16_t offset)
{
    int32_t i;
    uint8_t *tmp;
    if (offset >= 0)
    {
        for (i = size; i >= 0; i--)
        {
            if (NULL == (dat + i))
            {
                return -1; //�ڴ����
            }
            if (NULL == (dat + i + offset))
            {
                return -1; //�ڴ����
            }
            *(dat + i + offset) = *(dat + i); //�ƶ�����
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            if (NULL == (dat + i))
            {
                return -1; //�ڴ����
            }
            if (NULL == (dat + i + offset))
            {
                return -1; //�ڴ����
            }
            *(dat + i + offset) = *(dat + i); //�ƶ�����
        }
    }
    return 0; //�����ɹ�
}
/***************************************************************************************
**������:       DRV_FaceId_EncryptSizeSet
**��������:     ���ü��ܺ�size
**�������:     --
**�������:     
int16_t
�µĳ���
**��ע:       PS.������Ҫ����ָ����С����,Ĭ����16bit  
****************************************************************************************/
static int16_t DRV_FaceId_EncryptSizeSet(void)
{
    uint16_t size = (((faceCom.com.txCnt - DRV_FACE_ID_DATA_ADDR) / DRV_FACE_ID_ENCRYPT_ALIGN) * DRV_FACE_ID_ENCRYPT_ALIGN) + DRV_FACE_ID_ENCRYPT_ALIGN;
    faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR] = (uint8_t)(size >> 8);
    faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR + 1] = (uint8_t)(size);
    faceCom.com.txCnt = size + DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_ENCRYPT_SIZE_LEN; //���¼��ܺ����ݵĳ���
    return size;
}
#endif
/***************************************************************************************
**������:       DRV_FaceId_PackDataAndSend
**��������:     ������ݲ�����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_FaceId_PackDataAndSend(uint8_t cmdType)
{
    int16_t lentmp = 0;
    uint8_t chkTmp = 0;
    if (0 != cmdType) //����
    {
#if 1	
        DEBUG_LOG_OUT("Face encrypt send data:");
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
        DRV_FaceId_MoveData(&faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR],
                            faceCom.com.txCnt - DRV_FACE_ID_SYNCWORD_LEN,
                            2);               //����������ƶ�2Byte
        lentmp = DRV_FaceId_EncryptSizeSet(); //�������ݳ���
#if 0
		DEBUG_LOG_OUT("Face encrypt send data move:"); //�ƶ���Ĳ�������
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
        aes_encrypt_ecb(AES_CYPHER_128,
                        &faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                        lentmp,
                        FaceIdAesKey); //����
        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                     lentmp); //����У��
                                              //        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR],
                                              //                                     lentmp); //����У��
        faceCom.com.txBuf[lentmp + DRV_FACE_ID_ENCRYPT_DATA_ADDR] = chkTmp;
        faceCom.com.txCnt += DRV_FACE_ID_PARITYCHECK_LEN;          //����У����ĳ���
        DRV_FaceId_SendData(faceCom.com.txBuf, faceCom.com.txCnt); //��������
#if 0
		DEBUG_LOG_OUT("Face encrypt send data encrypt:"); //���͵�����
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
    }
    else
    {
        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR],
                                     faceCom.com.txCnt - DRV_FACE_ID_SYNCWORD_LEN); //����У��
        faceCom.com.txBuf[faceCom.com.txCnt] = chkTmp;
        faceCom.com.txCnt += DRV_FACE_ID_PARITYCHECK_LEN;          //����У����ĳ���
        DRV_FaceId_SendData(faceCom.com.txBuf, faceCom.com.txCnt); //��������
        DEBUG_LOG_OUT("Face send data:");
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
    }
	return 0;
}
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_FaceId_Init
**��������:     ��ʼ������ʶ��ģ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_FaceId_Init(void)
{

    UART_InitTypeDef UART_InitStruct;
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_1(); //����ģ���Դ
    M_X_IRQ_9555_IN_NOPULL();
    M_X_IRQ_9555_EXIT_RISE_DISABLE(); //�ر�è��֪ͨ�ж�
	M_4052_EN_OUT_PP();
	M_4052_A0_OUT_PP();
	M_4052_A1_OUT_PP();
	M_4052_EN_OUT_0();
	M_4052_A0_OUT_1();
	M_4052_A1_OUT_0(); //ʹ��Y1
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
    UART_InitStruct.UART_Mode = UART_INT_MODE; //����CPU�жϷ�ʽ
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(DRV_FACE_ID_UART, &UART_InitStruct);
	
}
/***************************************************************************************
**������:       DRV_FaceId_OpenPow
**��������:     �ر�������Դ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_FaceId_OpenPow(void)
{
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_1(); //��ģ���Դ
}
/***************************************************************************************
**������:       DRV_FaceId_ClosePow
**��������:     �ر�������Դ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_FaceId_ClosePow(void)
{
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_0(); //�ر�ģ���Դ
}
/***************************************************************************************
**������:       DRV_FaceId_RecvDataUnblock
**��������:     ������ʽ��������
**�������:     
uint8_t *recvBuf, ����ָ��
uint16_t *len ���ݳ���
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
int8_t DRV_FaceId_RecvDataUnblock(void)
{
    uint8_t receTmp; //��ȡ���ݵ���ʱ����
                     //    int8_t reTmp = 0;
    do
    {
        switch (UART_RecvByte(faceUart, &receTmp))
        {
        case STATUS_OK:
            faceCom.com.rxBuf[faceCom.com.rxCnt] = receTmp; //�ѻ��������е����ݶ���
            faceCom.com.rxCnt++;
			if(faceCom.com.rxCnt >= DRV_FACE_ID_RX_BUF_SIZE) //���������Ĵ���ռ�
			{
				return 1; //�������,�ȴ���
			}
            faceCom.com.rxTim = 0;
            break;
        case STATUS_NULL:
            faceCom.com.rxTim++;
			if(0 == faceCom.com.rxCnt)
			{
				return -4; //δ��ʼ����
			}
            if (faceCom.com.rxTim >= DRV_FACE_ID_RX_MAX_TIME) //һ֡�������
            {
                if (0 == faceCom.com.rxCnt) //��ѯ���ݳ���
                {
                    faceCom.com.rxTim = 0; //�����ʱ����������
                    return -2;             //û������
                }
                else
                {
                    return 1; //�������
                }
            }
            else
            {
                return 0; //���ݽ�����
            }
            break;
        default:
            return -3; //���ղ�������
            break;
        }

    } while (1);
}

/***************************************************************************************
**������:       DRV_FaceId_RecvDataAndAnalysis
**��������:     ��������
**�������:     
uint8_t cmdType 0 �����ܣ���0 ����
**�������:     
int8_t
0; //У��ͨ��
-1; //ͬ����У�鲻ͨ��
-2; //У�鲻ͨ��
**��ע:         
****************************************************************************************/
int8_t DRV_FaceId_RecvDataAndAnalysis(uint8_t cmdType)
{
    uint16_t lenTmp = 0;
    int8_t reTmp = 0;
    uint8_t chksumTmp = 0;
    if ((DRV_FACE_ID_SYNCWORD_H == faceCom.com.rxBuf[DRV_FACE_ID_SYNCWORD_ADDR]) &&
        (DRV_FACE_ID_SYNCWORD_L == faceCom.com.rxBuf[DRV_FACE_ID_SYNCWORD_ADDR + 1])) //���ͬ����0XEFAA
    {
        if (0 != cmdType) //����
        {
            lenTmp = (faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR] << 8) +
                     faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR + 1];
            chksumTmp = DRV_FaceId_GetOrChk(&faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR], lenTmp); //����У���
            if (chksumTmp == faceCom.com.rxBuf[lenTmp + DRV_FACE_ID_ENCRYPT_M2H_SW_ID_SIZE])            //У��ͨ��
            {
				#if 0
				DEBUG_LOG_OUT("face decrypt revc data ecrypt:"); //ԭʼ��������
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
				#endif
                aes_decrypt_ecb(AES_CYPHER_128,
                                &faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                lenTmp,
                                FaceIdAesKey); //����
                #if 0
				DEBUG_LOG_OUT("face decrypt revc data move :"); //���ܺ�δ�ƶ�������
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
                #endif
                DRV_FaceId_MoveData(&faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                    lenTmp + DRV_FACE_ID_PARITYCHECK_LEN,
                                    -2); //����������ǰ�ƶ�2Byte,��ȡ��ԭʼ����
				#if 1
                DEBUG_LOG_OUT("face decrypt revc data:");
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
				#endif
                return 0; //У��ͨ��
            }
            else
            {

                return -2; //У�鲻ͨ��
            }
        }
        else //δ����
        {
            lenTmp = (faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR] << 8) +
                     faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR + 1];
            chksumTmp = DRV_FaceId_GetOrChk(&faceCom.com.rxBuf[DRV_FACE_ID_DATA_ADDR], lenTmp); //����У���
            if (chksumTmp == faceCom.com.rxBuf[lenTmp + DRV_FACE_ID_SW_ID_SIZE])
            {
                DEBUG_LOG_OUT("face revc data:");
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
                return 0; //У��ͨ��
            }
            else
            {
                return -2; //У�鲻ͨ��
            }
        }
    }
    else
    {
        return -1; //ͬ����У�鲻ͨ��
    }
}
/***************************************************************************************
**������:       DRV_FaceId_CmdSend
**��������:     ����ֵָ��
**�������:     
uint8_t mid, MID
uint8_t *dat, ����ָ��
uint32_t datLen ���ݳ���
**�������:     
int8_t
0 �ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_FaceId_CmdSend(uint8_t mid, uint8_t *dat, uint32_t datLen, uint8_t cmdType)
{
    DRV_FaceId_ClearTxPar();
    DRV_FaceId_SyncWordSet();
    DRV_FaceId_MsgIdSet(mid);
    DRV_FaceId_DataAddBatch(dat, datLen);
    DRV_FaceId_SizeSet();
    DRV_FaceId_PackDataAndSend(cmdType); //��������
    return 0;
}

/***************************************************************************************
**������:       DRV_FaceId_GetRecvMsgType
**��������:     ��ȡ���յ���Ϣ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvMsgType(void)
{
    return faceCom.com.rxBuf[DRV_FACE_ID_MID_ID_ADDR];
}
/***************************************************************************************
**������:       DRV_FaceId_GetRecvReplyMid
**��������:     ��ȡ���յ�mid
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvReplyMid(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_MID_ADDR, readDat, len);
}
///***************************************************************************************
//**������:       DRV_FaceId_GetRecvReplyResult
//**��������:     ����reply�Ľ��
//**�������:
//uint8_t *readDat,����ָ��
//uint32_t len�����ݳ���
//**�������:
//int8_t
//-1; //��������
//0; //�ɹ�
//**��ע:
//****************************************************************************************/
//int8_t DRV_FaceId_GetRecvReplyResult(uint8_t *readDat, uint32_t len)
//{
//    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_MID_ADDR, readDat, len);
//}
/***************************************************************************************
**������:       DRV_FaceId_GetRecvRepley
**��������:     ����reply����
**�������:    
uint8_t *readDat,����ָ��
uint32_t len�����ݳ���
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_FaceId_GetRecvReplyData(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_DATA_ADDR, readDat, len);
}
/***************************************************************************************
**������:       DRV_FaceId_GetRecvNoteNid
**��������:     ��ȡ���յ�nid
**�������:     --
**�������:  
uint8_t
��ȡ��������
**��ע:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvNoteNid(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_NOTE_NID_ADDR, readDat, len);
}

/***************************************************************************************
**������:       DRV_FaceId_GetRecvNoteData
**��������:     ����note����
**�������:    
uint8_t *readDat,����ָ��
uint32_t len�����ݳ���
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_FaceId_GetRecvNoteData(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_NOTE_DATA_ADDR, readDat, len);
}
/***************************************************************************************
**������:       DRV_FaceId_ClearRxPar
**��������:     �������
**�������:     --
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_FaceId_ClearRxPar(void)
{
    memset(faceCom.com.rxBuf, 0, DRV_FACE_ID_RX_BUF_SIZE);
    faceCom.com.rxCnt = 0;
    faceCom.com.rxTim = 0;
    return 0;
}
#ifdef DRV_FACE_ID_ENCRYPT_EN //����ʹ��
///***************************************************************************************
//**������:       DRV_FaceId_SendEncryptReg
//**��������:     ע����ܳ���
//**�������:
//fFaceIdSendencrypt_t fun
//**�������:
//int8_t
//-1; //��������
//0; //������ȷ
//**��ע:
//****************************************************************************************/
//int8_t DRV_FaceId_SendEncryptReg(FaceIdEncrypt_t fun)
//{
//    if (NULL == fun)
//    {
//        return -1; //��������
//    }
//    faceIdEncrypt = fun;
//    return 0; //������ȷ
//}
/***************************************************************************************
**������:       DRV_FaceId_GetAesKey
**��������:     ��ȡAES key
**�������:     
uint8_t *dat
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:       
�������
������ж���Ϊ char data[4];
4 ���ַ����û��ɲ���ʱ����Ϊ��������һ�����������
������������� 16byte ����
4byte ����������� md5 ���� 16 byte ��32 ���ַ��� �����磺
fbe0 aa53 6fc3 49cb dc45 1ff5 970f 9357 
[0	->	31]
ת��Ϊ�ַ���
˫��Э���� size Ϊ 32 ���ַ����а�ָ�����ɳ�ȡ 16 ��byte����ȡ���߼�Ϊ��
[6-17-8-5	11-15-3-17	6-23-1-6	31-2-2-7]
�����г�ȡ�����������Ϊ��5 c6 a	3 b 0 c	5 5 b 5	7e e 3
****************************************************************************************/
int8_t DRV_FaceId_GetAesKey(uint8_t *dat)
{
    uint8_t md5Tmp[16] = {
        0,
    };
    uint8_t md5TmpStr[32] = {
        0,
    };
    uint8_t md5TmpByte = 0;
    uint16_t i;
    if (NULL == dat)
    {
        return -1; //��������
    }
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, dat, 4);
    MD5Final(&md5, md5Tmp);
    for (i = 0; i < 16; i++)
    {
        md5TmpByte = md5Tmp[i] >> 4; //�����λ
        if (md5TmpByte <= 9)
        {
            md5TmpByte += '0';
        }
        else
        {
            md5TmpByte += ('a' - 10);
        }
        md5TmpStr[i * 2] = md5TmpByte;
        md5TmpByte = md5Tmp[i] & 0x0f; //�����λ
        if (md5TmpByte <= 9)
        {
            md5TmpByte += '0';
        }
        else
        {
            md5TmpByte += ('a' - 10);
        }
        md5TmpStr[i * 2 + 1] = md5TmpByte;
    } // 16 byte ��32 ���ַ��� ת��Ϊ�ַ���
    for (i = 0; i < 16; i++)
    {
        FaceIdAesKey[i] = md5TmpStr[FaceIdAesExtract[i]];
        //������Ҫ����eas key����
    }
    return 0; //�ɹ�
}
#endif
/***************************************************************************************
**������:       DRV_Face_EnterSleepConfig
**��������:     ����͹�������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Face_EnterSleepConfig(void)
{
    M_X_IRQ_9555_IN_NOPULL();
    M_X_IRQ_9555_EXIT_RISE_ENABLE();
    M_CAM_RX_OUT_PP();
    M_CAM_RX_OUT_0();
	DEBUG_LOG_OUT("M_CAM_RX_OUT_0\n");
    M_CAM_TX_OUT_PP();
    M_CAM_TX_OUT_0();
	DEBUG_LOG_OUT("M_CAM_TX_OUT_0\n");
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_0();
	DEBUG_LOG_OUT("M_CAM_POW_EN_OUT_0\n");
    #ifdef USE_FACE_AOBI //ʹ�ð±�����
	M_CAM_WIFI_POW_EN_OUT_PP();
	M_CAM_WIFI_POW_EN_OUT_0();
    #endif
//    #ifdef IR_ENABLE
//	M_IR_IRQ_IN_NOPULL();
//	M_IR_IRQ_EXIT_FALL_ENABLE();
//    #endif
    M_4052_A1_OUT_PP();
	M_4052_EN_OUT_1();
	M_4052_A0_OUT_0();
	M_4052_A1_OUT_0(); //ʹ��Y0
}
/***************************************************************************************
**������:       DRV_Face_WifiPowOpen
**��������:     ����wifi��Դ�� 
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Face_WifiPowOpen(void)
{
    #ifdef USE_FACE_AOBI //ʹ�ð±�����
    M_X_FACE_POW_EN_OUT_PP();
    M_X_FACE_POW_EN_OUT_1();
    #if 0
    M_X_CHANGE_9555_OUT_PP();
    M_X_CHANGE_9555_OUT_1();
    #endif
    #endif
    #ifdef USE_FACE_SHANGTANG //ʹ����������
    M_CAM_WIFI_POW_EN_OUT_PP();
	M_CAM_WIFI_POW_EN_OUT_1();
    #endif
}
/***************************************************************************************
**������:       DRV_Face_WifiPowClose
**��������:     ����wifi��Դ�ر� 
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Face_WifiPowClose(void)
{
    #ifdef USE_FACE_AOBI //ʹ�ð±�����
    M_X_FACE_POW_EN_OUT_PP();
    M_X_FACE_POW_EN_OUT_0();
	M_CAM_POW_EN1_OUT_PP();
	M_CAM_POW_EN1_OUT_0();
    M_X_CHANGE_9555_OUT_PP();
    M_X_CHANGE_9555_OUT_0();
    #if 0
    M_X_CHANGE_9555_OUT_PP();
    M_X_CHANGE_9555_OUT_0();
    #endif
    #endif
    #ifdef USE_FACE_SHANGTANG //ʹ����������
    M_CAM_WIFI_POW_EN_OUT_PP();
    M_CAM_WIFI_POW_EN_OUT_0();
    #endif
}
/***************************************************************************************
**������:       DRV_Face_CateyeFaceSw
**��������:     è�������л� ���ڰ±�  
**�������:     --
**�������:     --
**��ע:         0 ������1è��
****************************************************************************************/
void DRV_Face_CateyeFaceSw(uint8_t sw)
{
    M_X_CHANGE_9555_OUT_PP();
    if(0 == sw)
    {
        M_X_CHANGE_9555_OUT_0();
    }
    else
    {
        M_X_CHANGE_9555_OUT_1();
    }
}
/***************************************************************************************
**������:       DRV_FaceId_GetRecvMsgSize
**��������:     ��ȡ���յ���Ϣ�ĳ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint16_t DRV_FaceId_GetRecvMsgSize(void)
{
    return ((faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR] << 8) + faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR+1]);
}

/***************************************************************************************
**������:       DRV_FaceId_IRSleepConfig
**��������:     IR�����Ӧ��������
**�������:     
uint8_t sw		0,�ر��жϣ���0�������ж�
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_FaceId_IRSleepConfig(uint8_t sw)
{
	if (0 == sw)
	{
		M_IR_IRQ_IN_NOPULL();
		M_IR_IRQ_EXIT_INT_DISABLE();
	}
	else
	{
		M_IR_IRQ_IN_NOPULL();
		M_IR_IRQ_EXIT_FALL_ENABLE();
	}
}
#endif /*DRV_FACE_ID_MODULE_EN*/
/************************************Code End******************************************/
