/***************************************************************************************
**�ļ���:     DRV_Comm.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
ͨѶЭ�飺
�ֽ�����	�ֽڳ���	�ֽ�����
��ͷH	    1	        FE 
��ͷL	    1	        02�����޺������� 03�����к������ݣ���������Ӧ��
������	    1	        �仯
����	    1	        0�����ܣ�(1-255)Ϊ����
�Ự��Կ	6	        �仯
���ݳ���	2           �����ݳ��ȣ�	�仯��֡�����ֽ����ݡ�
֡����	    �䳤	    �仯(δβ��1�ֽڼ����)
У���	    2	        ��ͷ��2�ֽڵ�֡����֮���
****************************************************************************************/
#include "DRV_Comm.h"
#include "rijndael.h"
#include "trng_drv.h"
#if DRV_COMM_MODULE_EN
#warning DRV_COMMģ��ʹ��!
/*******************************�궨��***************************/
#if 0
#define DRV_COMM_UART_BPS (115200)  //ͨѶ������
#else
#define DRV_COMM_UART_BPS (460800)  //ͨѶ������
#endif
#define DRV_COMM_UART (SCI1)        //ʹ�õĴ���
#define DRV_COMM_WIFI_UART (SCI2)        //ʹ�õĴ���
#define DRV_COMM_RX_BUF_SIZE (1024) //���ջ���
#define DRV_COMM_TX_BUF_SIZE (512)  //���ͻ���
#define DRV_COMM_RX_MAX_TIME (20)   //һ֡�������
#define DRV_COMM_SYNCWORD_ADDR (0)  //Э��ͷ��ַ
#define DRV_COMM_SYNCWORD_H (0XFE)  //��ͷ
#define DRV_COMM_SYNCWORD_H_SIZE (1)
#define DRV_COMM_SYNCWORD_L_SIZE (1)
#define DRV_COMM_SYNCWORD_SIZE (2)
#define DRV_COMM_MID_ADDR (DRV_COMM_SYNCWORD_ADDR + DRV_COMM_SYNCWORD_SIZE)      //�����ֵ�ַ
#define DRV_COMM_MID_SIZE (1)                                                    //�����ֳ���
#define DRV_COMM_SECRET_KEY_ADDR (DRV_COMM_MID_ADDR + DRV_COMM_MID_SIZE)         //�Ự��Կ��ַ
#define DRV_COMM_SECRET_KEY_SIZE (6)                                             //�Ự��Կ����
#define DRV_COMM_SIZE_ADDR (DRV_COMM_SECRET_KEY_ADDR + DRV_COMM_SECRET_KEY_SIZE) //���ݳ��ȵ�ַ
#define DRV_COMM_SIZE_SIZE (2)                                                   //���ݳ��ȵĳ���
#define DRV_COMM_DATA_ADDR (DRV_COMM_SIZE_ADDR + DRV_COMM_SIZE_SIZE)             //֡���ݿ�ʼ��ַ
#define DRV_COMM_FIXED_SIZE (DRV_COMM_SYNCWORD_SIZE +   \
                             DRV_COMM_MID_SIZE +        \
                             DRV_COMM_SECRET_KEY_SIZE + \
                             DRV_COMM_SIZE_SIZE) //Э���й̶��Ĵ�С ��ͷH1 + ��ͷL1 + ������1 + ���ݳ���2
#define DRV_COMM_CHK_START_ADDR (1)              //У�鿪ʼ��ַ
#define DRV_COMM_CHK_FIXED_SIZE (DRV_COMM_SYNCWORD_L_SIZE + \
                                 DRV_COMM_MID_SIZE +        \
                                 DRV_COMM_SECRET_KEY_SIZE + \
                                 DRV_COMM_SIZE_SIZE) //У��ʱ�̶������ݴ�С
#define DRV_COMM_CHK_SIZE (2)                        //У�����ݳ���
#define DRV_COMM_ENCRYPT_ALIGN (16) //��16����
#define DRV_COMM_AES_KEY_SIZE (16)
/*************************.C�����ṹ����***********************/
#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
typedef struct
{
    uint32_t rxCnt;                      //���ռ�����
    uint32_t rxTim;                      //���ռ�ʱ��
    uint8_t rxBuf[DRV_COMM_RX_BUF_SIZE]; //���ջ�����
    uint32_t txCnt;                      //���ͼ�����
    uint8_t txBuf[DRV_COMM_TX_BUF_SIZE]; //���ͻ�����
    uint16_t dataLen;                    //���ݳ���
} TYPEs_Com;
typedef struct
{
    uint8_t HeardH;        //��ͷH
    uint8_t HeardL;        //��ͷL
    uint8_t Cmd;           //������
    uint8_t Encrypt;       //����
    uint8_t EncryptKey[6]; //�Ự��Կ
    uint16_t size;         //���ݳ���
} TYPEs_CommProtocolFront;
typedef struct
{
    TYPEs_CommProtocolFront front;
    uint8_t *data; //֡����ָ��
    uint16_t chk;  //У���
} TYPEs_CommProtocol;
typedef struct
{
    TYPEs_Com com;                 //Ӳ��ͨѶ
    TYPEs_CommProtocol ProtocolTx; //ͨѶЭ�鷢��
    TYPEs_CommProtocol ProtocolRx; //ͨѶЭ�����
    uint16_t lenAddr;              //���ȵĵ�ַ
    uint16_t fixedLen;             //Э���й̶��ĳ���
} TYPEs_CommCom;
#pragma pack() //ȡ������
/*************************˽��ȫ�ֱ���*************************/
static TYPEs_CommCom commCom = {
    .com.rxCnt = 0,
    .com.rxTim = 0,
    .com.rxBuf = {
        0,
    },
    .com.txCnt = 0,
    .com.txBuf = {
        0,
    },
    .com.dataLen = 0,
    .ProtocolTx.front.HeardH = 0,
    .ProtocolTx.front.HeardL = 0,
    .ProtocolTx.front.Cmd = 0,
    .ProtocolTx.front.Encrypt = 0,
    .ProtocolTx.front.EncryptKey = {
        0,
    },
    .ProtocolTx.front.size = 0,
    .ProtocolTx.data = NULL,
    .ProtocolTx.chk = 0,
    .ProtocolRx.front.HeardH = 0,
    .ProtocolRx.front.HeardL = 0,
    .ProtocolRx.front.Cmd = 0,
    .ProtocolRx.front.Encrypt = 0,
    .ProtocolRx.front.EncryptKey = {
        0,
    },
    .ProtocolRx.front.size = 0,
    .ProtocolRx.data = NULL,
    .ProtocolRx.chk = 0,
    .lenAddr = 0,
    .fixedLen = 0,
}; //
static UART_TypeDef *commUart = DRV_COMM_UART; //ǰ��ͨѶ����
static UART_TypeDef *wifiUart = DRV_COMM_WIFI_UART;

uint8_t CommAesKey[DRV_COMM_AES_KEY_SIZE] = {
    0x00,
};   

uint8_t commKey[10][6]={
{0x2b, 0x7e, 0x15, 0x16,0xa6,0xd2,},  //0
{0x45, 0xb3, 0x2a, 0x9b,0x44,0x31,},  //1
{0x62, 0x5d, 0x88, 0x64,0x74,0xf8,},  //2
{0x72, 0x0e, 0x11, 0x28,0x59,0xee,},  //3
{0x62, 0x57, 0x98, 0x63,0xb8,0x38,},  //4
{0x44, 0x91, 0x84, 0x47,0x46,0x85,},  //5
{0xcd, 0x72, 0x51, 0xa8,0xb6,0xde,},  //6
{0x6b, 0x5f, 0x65, 0x11,0x2a,0xa7,},  //7
{0xab, 0x1e, 0x57, 0x12,0x26,0x6d,},  //8
{0xa1, 0x7e, 0x21, 0x15,0x66,0xbc,}   //9
};
uint8_t rootKeyA[6]={0,}; //����Կroot
uint8_t rootKeyRandom[6]={0,}; //����Կ�����

/*************************����ȫ�ֱ���*************************/
uint8_t *pDrvCommMac =NULL; //MACָ��
uint8_t *pDrvCommRootKeyA = NULL; //ָ��ROOT��ָ��
/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       DRV_Comm_GetUid
**��������:     ��ȡUID
**�������:     
uint8_t *dat, ����
**�������:     --
���
**��ע:         
****************************************************************************************/
//void DRV_Comm_GetUid(uint8_t *dat)
//{
//    uint8_t uid[16]={0x00,};
//    sfs_read_uid_ex(uid,16);
//    DEBUG_LOG_OUT("uid:\n");
//    DEBUG_LOG_OUT_HEX(uid,16);
//    for(uint8_t i=0;i<6;i++)
//    {
//        dat[i]= uid[4+i]^uid[10+i];
//    }
//    DEBUG_LOG_OUT("pwd:\n");
//    DEBUG_LOG_OUT_HEX(dat,6);
//}

/***************************************************************************************
**������:       DRV_Comm_GetRandom
**��������:     ��ȡ�����
**�������:     
uint8_t *dat, ����
**�������:     --
���
**��ע:         
****************************************************************************************/
void DRV_Comm_GetRandom(uint8_t *dat)
{
    uint16_t i =0;
    Init_Trng();
    for(i=0;i<6;i++)
    {
        *dat = (uint8_t)(GetRandomWord());
        dat ++;
    }
}
/***************************************************************************************
**������:       DRV_Comm_GetRandomuEncrypt
**��������:     ��ȡ���������λ
**�������:     --
**�������:     
uint8_t
��Ϊ0�������
���
**��ע:         
****************************************************************************************/
uint8_t DRV_Comm_GetRandomuEncrypt(void)
{
    uint16_t i =0;
    uint8_t temp;
    Init_Trng();
    while(1)
    {
        temp = (uint8_t)(GetRandomWord());
        if(0 != temp)
        {
            return temp;
        }
    }
}
/***************************************************************************************
**������:       DRV_Comm_GetAesKey
**��������:     ��ȡ��ȡAES��Կ
**�������:     
uint8_t *dat, �Ự��Կ
uint8_t encrypt ����λ��
**�������:     --
���
**��ע:         
****************************************************************************************/
void DRV_Comm_GetAesKey(uint8_t *dat,uint8_t encrypt)
{
    uint16_t i =0;
    memcpy(CommAesKey,
           pDrvCommMac,
           4); //����MAC
    for(i=0;i<4;i++)
    {
        CommAesKey[i] = ~CommAesKey[i]; //MAC��4byteȡ��
    }
    memcpy(CommAesKey+4,
           dat,
           6); //�Ự��Կ
    memcpy(CommAesKey+4+6,
           pDrvCommRootKeyA,
           6); //����Կ
    encrypt = encrypt %100/10; //��ȡʮλ
    for(i=0;i<16;i++)
    {
        CommAesKey[i] ^= commKey[encrypt][i%6]; //ѭ�����
    }
}
/***************************************************************************************
**������:       DRV_Comm_RecvDatClear
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         ��������ǰ����Ҫ����ղ���.
****************************************************************************************/
static int8_t DRV_Comm_RecvDatClear(void)
{
    commCom.com.rxCnt = 0;
    commCom.com.rxTim = 0;
    memset(commCom.com.rxBuf, 0, DRV_COMM_RX_BUF_SIZE); //��ս�����
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_SyncWordSet
**��������:     ��Ϣ��ͷ����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_SyncWordSet(TYPEe_CommHead datType)
{
    commCom.com.txBuf[commCom.com.txCnt++] = DRV_COMM_SYNCWORD_H;
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(datType);
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_MsgIdSet
**��������:     ��ϢID����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_MsgIdSet(uint8_t cmd)
{
    commCom.com.txBuf[commCom.com.txCnt++] = cmd;
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_SecretKey
**��������:     ����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_SecretKey(uint8_t *key)
{
    memcpy((uint8_t *)(&commCom.com.txBuf[commCom.com.txCnt]),
           key,
           DRV_COMM_SECRET_KEY_SIZE);
    commCom.com.txCnt += DRV_COMM_SECRET_KEY_SIZE;
    //    commCom.com.txCnt = DRV_COMM_DATA_ADDR; //������������д��֡����
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_DataAdd(uint8_t dat)
{
    commCom.com.txBuf[commCom.com.txCnt] = dat;
    commCom.com.txCnt++;
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_DataAddBatch(uint8_t *dat, uint32_t len)
{
    memcpy(&commCom.com.txBuf[commCom.com.txCnt], dat, len);
    commCom.com.txCnt += len;
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_SizeAddEmpty(void)
{
    commCom.lenAddr = commCom.com.txCnt; //��¼��ַ
    commCom.com.txBuf[commCom.com.txCnt++] = 0x00;
    commCom.com.txBuf[commCom.com.txCnt++] = 0x00; //����ռ��2����λ
    commCom.fixedLen = commCom.com.txCnt;          //���õ�ַ���¼ϵͳ�Ĺ̶����ȡ�
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_DataAdd
**��������:     �������
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_SizeSet(void)
{
    uint16_t lenTmp = 0;
    commCom.com.dataLen = commCom.com.txCnt - commCom.fixedLen;
    commCom.com.txBuf[commCom.lenAddr] = (uint8_t)(commCom.com.dataLen >> 8);
    commCom.com.txBuf[commCom.lenAddr + 1] = (uint8_t)(commCom.com.dataLen);
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_ClearTxPar
**��������:     ��շ��Ͳ���
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_ClearTxPar(void)
{
    memset(commCom.com.txBuf, 0, DRV_COMM_TX_BUF_SIZE);
    commCom.com.txCnt = 0;
}
///***************************************************************************************
//**������:       DRV_Comm_GetOrChk
//**��������:     ��ȡOR�Ľ��
//**�������:
//uint8_t *dat, ����
//uint16_t len ����
//**�������:
//uint8_t
//���
//**��ע:
//****************************************************************************************/
//static uint16_t DRV_Comm_GetSumChk(uint8_t *dat, uint16_t len)
//{
//    uint16_t i;
//    uint16_t chkTmp = NULL;
//    if (NULL == dat)
//    {
//        return 0; //����
//    }
//    chkTmp = *dat;
//    for (i = 1; i < len; i++)
//    {
//        if (NULL == (dat + i))
//        {
//            return 0; //����
//        }
//        chkTmp += *(dat + i);
//    }
//    return chkTmp;
//}
/***************************************************************************************
**������:       DRV_Comm_GetOrChk
**��������:     ��ȡOR�Ľ��
**�������:     
uint8_t *dat, ����
uint16_t len ����
**�������:     
uint8_t
���
**��ע:         
****************************************************************************************/
static uint8_t DRV_Comm_GetOrChk(uint8_t *dat, uint16_t len)
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
**������:       DRV_Comm_SetChk
**��������:     ����У���
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static uint16_t DRV_Comm_SetChk(void)
{
    uint16_t chkTmp = 0;
    chkTmp = DRV_Comm_GetSumChk(&commCom.com.txBuf[DRV_COMM_CHK_START_ADDR],
                                commCom.com.dataLen + DRV_COMM_CHK_FIXED_SIZE); //����У���
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(chkTmp >> 8);
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(chkTmp);
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_SendData
**��������:     ��������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_Comm_SendData(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(commUart, sendBuf, len);
}
/***************************************************************************************
**������:       DRV_Comm_GetRecvData
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
static int8_t DRV_Comm_GetRecvData(uint32_t addr, uint8_t *readDat, uint32_t len)
{
    if (NULL == readDat)
    {
        return -1; //��������
    }
    memcpy(readDat, &commCom.com.rxBuf[addr], len); //��������
    return 0;                                       //�ɹ�
}
/***************************************************************************************
**������:       DRV_FaceId_PackDataAndSend
**��������:     ������ݲ�����
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
static int8_t DRV_Comm_PackDataAndSend(TYPEe_CommEncrypt cmdType)
{
    DRV_Comm_SendData(commCom.com.txBuf, commCom.com.txCnt); //��������
    DEBUG_LOG_OUT("DRV_Comm_PackDataAndSend:\n");
    DEBUG_LOG_OUT_HEX(commCom.com.txBuf, commCom.com.txCnt);
    return 0;
}
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_Comm_Init
**��������:     ͨѶ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Comm_Init(void)
{
    UART_InitTypeDef UART_InitStruct;
    UART_InitStruct.UART_BaudRate = DRV_COMM_UART_BPS;
    UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
    UART_InitStruct.UART_Mode = UART_INT_MODE; //����CPU�жϷ�ʽ
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(DRV_COMM_UART, &UART_InitStruct);
    M_HS6621_INT_OUT_PP();
    M_HS6621_INT_OUT_1();
    M_CCM4202_INT_IN_NOPULL();
    DRV_Comm_ClearRxPar(); //�������
    //	memset(&commCom,0,sizeof(commCom));
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
int8_t DRV_Comm_RecvDataUnblock(void)
{
    uint8_t receTmp; //��ȡ���ݵ���ʱ����
    #if 0
    memcpy(commCom.com.rxBuf,
            commCom.com.txBuf,
            DRV_COMM_TX_BUF_SIZE    
            );
    commCom.com.rxCnt = 100;
    return 1;
    #endif
    do
    {
        if (STATUS_OK == UART_RecvByte(commUart, &receTmp))
        {

            commCom.com.rxBuf[commCom.com.rxCnt] = receTmp; //�ѻ��������е����ݶ���
            commCom.com.rxCnt++;
            if (commCom.com.rxCnt >= DRV_COMM_RX_BUF_SIZE) //���������Ĵ���ռ�
            {
                DEBUG_LOG_OUT("DRV_Comm_RecvDataUnblock commCom.com.rxCnt >= DRV_COMM_RX_BUF_SIZE\n");
                return 1; //�������,�ȴ���
            }
            commCom.com.rxTim = 1;
        }
        else
        {
            if (0 != commCom.com.rxTim)
            {
                commCom.com.rxTim++;
                if (commCom.com.rxTim >= DRV_COMM_RX_MAX_TIME) //һ֡�������
                {
                    if (0 == commCom.com.rxCnt) //��ѯ���ݳ���
                    {
                        commCom.com.rxTim = 0; //�����ʱ����������
                        return -2;             //û������
                    }
                    else
                    {
                        DEBUG_LOG_OUT("DRV_Comm_RecvDataUnblock OK\n");
                        commCom.com.rxTim = 0; //�����ʱ����������
                        return 1;              //�������
                    }
                }
                else
                {
                    return 0; //���ݽ�����
                }
            }
            else
            {
                return -4; //δ��ʼ����
            }
        }
    } while (1);
}
/***************************************************************************************
**������:       DRV_Comm_CmdSend
**��������:     ����ֵָ��
**�������:    
TYPEe_CommHead datType �������� 
uint8_t mid, MID ������
uint8_t *secretKey ��Կ
uint8_t *dat, ����ָ��
uint32_t datLen ���ݳ���
uint8_t cmdType ���ܱ�־��0���ģ�1����
**�������:     
int8_t
0 �ɹ�
**��ע:         
********************************************************************************K********/
int8_t DRV_Comm_CmdSend(TYPEs_ComSendParm *p)
{
    uint16_t chkTmp;        //֡����֮��
    uint16_t const lenHead=sizeof(TYPEs_CommProtocolFront); //����ͷ�ĳ���
    uint16_t lenTmp =0; //���Ȼ���
    uint16_t lenEncrypt =0; //���ܺ�ⳤ��
    TYPEs_CommProtocolFront *pProtocolFront = (TYPEs_CommProtocolFront *) commCom.com.txBuf;
    uint8_t *pTxBuf = commCom.com.txBuf;
    DRV_Comm_ClearTxPar(); //������ͻ���
    pProtocolFront->size = 0;
    pProtocolFront->HeardH = DRV_COMM_SYNCWORD_H;
    pProtocolFront->HeardL = (uint8_t)p->datType;
    pProtocolFront->Cmd = p->mid;
    pProtocolFront->Encrypt = (uint8_t)p->secretKeyFlag;
    DEBUG_LOG_OUT("EncryptKey:\n");
    DEBUG_LOG_OUT_HEX(pProtocolFront->EncryptKey, 6);
    memcpy((uint8_t *)(pTxBuf + lenHead),
           (uint8_t *)(p->dat),
           p->datLen);                                     //�������
           
    if (0 != p->datLen)
    {
        chkTmp = DRV_Comm_GetSumChk((pTxBuf +lenHead), p->datLen); //֡����֮��
        *(pTxBuf + lenHead+p->datLen) = (uint8_t)(chkTmp);
        DEBUG_LOG_OUT("chkTmp:%X\n", (uint8_t)(chkTmp));
    }     //����֡����֮��
    pProtocolFront->size =  p->datLen + 1; //֡���ݳ���
    commCom.com.txCnt = pProtocolFront->size + lenHead; //�����ܳ���
    DEBUG_LOG_OUT("old data:\n");
    DEBUG_LOG_OUT_HEX(commCom.com.txBuf, commCom.com.txCnt);
    if (eSecretKeyDis != p->secretKeyFlag) //����
    {
        #if 1
        DRV_Comm_GetRandom(rootKeyRandom);
        #else
        rootKeyRandom[0]= 1;
        rootKeyRandom[1]= 2;
        rootKeyRandom[2]= 3;
        rootKeyRandom[3]= 4;
        rootKeyRandom[4]= 5;
        rootKeyRandom[5]= 6;
        #endif
        memcpy(pProtocolFront->EncryptKey,
               rootKeyRandom,
               sizeof(pProtocolFront->EncryptKey));
        #if 1
        pProtocolFront->Encrypt = DRV_Comm_GetRandomuEncrypt();
        #else
        pProtocolFront->Encrypt =1;
        #endif
        DRV_Comm_GetAesKey(rootKeyRandom,pProtocolFront->Encrypt); //��ȡͨѶ��Կ
        #if 0
        DEBUG_LOG_OUT("mac:\n");
        DEBUG_LOG_OUT_HEX(pDrvCommMac,6); 
        DEBUG_LOG_OUT("root:\n");
        DEBUG_LOG_OUT_HEX(pDrvCommRootKeyA,6);
        DEBUG_LOG_OUT("random:\n");
        DEBUG_LOG_OUT_HEX(rootKeyRandom,6);
        DEBUG_LOG_OUT("CommAesKey:\n");
        DEBUG_LOG_OUT_HEX(CommAesKey,sizeof(CommAesKey));
        #endif
        lenEncrypt = (((pProtocolFront->size)/DRV_COMM_ENCRYPT_ALIGN)*DRV_COMM_ENCRYPT_ALIGN)+
                        DRV_COMM_ENCRYPT_ALIGN;    //������볤�ȡ�      
        DEBUG_LOG_OUT("size%d\n", pProtocolFront->size);
        aes_encrypt_ecb(AES_CYPHER_128,
                        &pTxBuf[lenHead],
                        lenEncrypt,
                        CommAesKey); //����
        commCom.com.txCnt = lenEncrypt + lenHead; //�����ܳ���
    }    
    BSWAP_SAVE_16(pProtocolFront->size); //��С��ת��
    chkTmp = DRV_Comm_GetSumChk((uint8_t *)(&pProtocolFront->HeardL), 
                                                 commCom.com.txCnt - sizeof(pProtocolFront->HeardH)); //����У���
    BSWAP_SAVE_16(chkTmp); //��С��ת��
    memcpy((uint8_t *)(pTxBuf + commCom.com.txCnt),
           (uint8_t *)(&chkTmp),
           2);              //���У���
    commCom.com.txCnt += 2; //����У��ͳ���
    
    DEBUG_LOG_OUT("commCom.com.txCnt:%d\n", commCom.com.txCnt);
    DRV_Comm_PackDataAndSend(p->cmdType); //��������
    return 0;
}
/***************************************************************************************
**������:       DRV_Comm_ClearRxPar
**��������:     �������
**�������:     --
**�������:     
int8_t
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_Comm_ClearRxPar(void)
{
    memset(commCom.com.rxBuf, 0, DRV_COMM_RX_BUF_SIZE);
    commCom.com.rxCnt = 0;
    commCom.com.rxTim = 0;
    return 0;
}

/***************************************************************************************
**������:       DRV_Comm_RecvDataAndAnalysis
**��������:     ��������
**�������:     
uint8_t cmdType 0 �����ܣ���0 ����
**�������:     
int8_t
0; //У��ͨ��
-1; //ͬ����У�鲻ͨ��
-2; //У�鲻ͨ��
-3; //����֡У�����
**��ע:         
****************************************************************************************/
int8_t DRV_Comm_RecvDataAndAnalysis(TYPEe_CommEncrypt cmdType)
{
    uint16_t lenTmp = 0;
    uint16_t lenTmpOld = 0; //ԭ��
    int8_t reTmp = 0;
    uint8_t chksumTmp = 0;
    uint16_t const lenHead=sizeof(TYPEs_CommProtocolFront); //����ͷ�ĳ���
    TYPEs_CommProtocolFront *pProtocolFront = (TYPEs_CommProtocolFront *) commCom.com.rxBuf;
    uint16_t chksum=0;
    if (DRV_COMM_SYNCWORD_H == commCom.com.rxBuf[DRV_COMM_SYNCWORD_ADDR]) //У���ļ�ͷ
    {
        if (eSecretKeyDis != pProtocolFront->Encrypt) //����
        {
            DRV_Comm_GetAesKey(pProtocolFront->EncryptKey,pProtocolFront->Encrypt); //��ȡͨѶ��Կ
            #if 0
            DEBUG_LOG_OUT("CommAesKey:\n");
            DEBUG_LOG_OUT("mac:\n");
            DEBUG_LOG_OUT_HEX(pDrvCommMac,6); 
            DEBUG_LOG_OUT("root:\n");
            DEBUG_LOG_OUT_HEX(rootKeyA,6);
            DEBUG_LOG_OUT("random:\n");
            DEBUG_LOG_OUT_HEX(CommAesKey,sizeof(CommAesKey)); 
            #endif
            lenTmpOld = BSWAP_16(pProtocolFront->size) ; //��ȡ����
            lenTmp = (((lenTmpOld)/DRV_COMM_ENCRYPT_ALIGN)*DRV_COMM_ENCRYPT_ALIGN)+
                        DRV_COMM_ENCRYPT_ALIGN;    //������볤�ȡ�
            chksum = DRV_Comm_GetSumChk((uint8_t *)&pProtocolFront->HeardL, lenTmp+lenHead-1); //����У���
            DEBUG_LOG_OUT("chksum:%X\n",chksum);
            if (chksum == (commCom.com.rxBuf[lenTmp + lenHead]<<8)+commCom.com.rxBuf[lenTmp + lenHead+1]) //У��
            {
                #if 0
				DEBUG_LOG_OUT("comm decrypt revc data ecrypt:"); //ԭʼ��������
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                aes_decrypt_ecb(AES_CYPHER_128,
                                &commCom.com.rxBuf[lenHead],
                                lenTmp,
                                CommAesKey); //����
                #if 1
				DEBUG_LOG_OUT("comm decrypt revc data move :"); //���ܺ������
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                chksum = DRV_Comm_GetSumChk((uint8_t *)(commCom.com.rxBuf+lenHead), 
                                            lenTmpOld-1); //��������֡У���
                #if 0
                DEBUG_LOG_OUT("chksum:%X\n",chksum);
                #endif
                if((uint8_t)chksum == commCom.com.rxBuf[lenHead+ lenTmpOld-1])
                {
                    DEBUG_LOG_OUT("comm decrypt chk ok\n"); //����У��ͨ��
                    memcpy((uint8_t *)(&commCom.ProtocolRx.front),
                           (uint8_t *)(commCom.com.rxBuf),
                           sizeof(commCom.ProtocolRx.front));
                    BSWAP_SAVE_16(commCom.ProtocolRx.front.size);
                    return 0; //У��ͨ��
                }
                else
                {
                    return -3; //����֡У�����
                }
            }
            else
            {
                DEBUG_LOG_OUT("comm decrypt chk error\n"); //ԭʼ��������
                return -2; //У�鲻ͨ��
            }
        }
        else //δ����
        {
            #if 1
            memcpy((uint8_t *)(&commCom.ProtocolRx.front),
                   (uint8_t *)(commCom.com.rxBuf),
                   sizeof(commCom.ProtocolRx.front));
            BSWAP_SAVE_16(commCom.ProtocolRx.front.size);
            lenTmpOld = commCom.ProtocolRx.front.size ; //��ȡ����
            lenTmp = lenTmpOld;
            chksum = DRV_Comm_GetSumChk((uint8_t *)&pProtocolFront->HeardL, lenTmp+lenHead-1); //����У���
            DEBUG_LOG_OUT("chksum:%X\n",chksum);
            if (chksum == (commCom.com.rxBuf[lenTmp + lenHead]<<8)+commCom.com.rxBuf[lenTmp + lenHead+1]) //У��
            {
                #if 1
				DEBUG_LOG_OUT("comm  revc data :"); //ԭʼ��������
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                
                chksum = DRV_Comm_GetSumChk((uint8_t *)(commCom.com.rxBuf+lenHead), 
                                            lenTmpOld-1); //��������֡У���
                DEBUG_LOG_OUT("chksum:%X\n",chksum);
                if((uint8_t)chksum == commCom.com.rxBuf[lenHead+ lenTmpOld-1])
                {
                    DEBUG_LOG_OUT("comm chk ok\n"); //����У��ͨ��
                    return 0; //У��ͨ��
                }
                else
                {
                    return -3; //����֡У�����
                }
                
            }
            else
            {
                DEBUG_LOG_OUT("comm  chk error\n"); //ԭʼ��������
                return -2; //У�鲻ͨ��
            }
            return 0;
            #endif
        }
    }
    else
    {
        return -1; //ͬ����У�鲻ͨ��
    }
}
/***************************************************************************************
**������:       DRV_COMM_GetRecvMsgType
**��������:     ��ȡ���յ���Ϣ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_COMM_GetRecvMsgType(void)
{
    return commCom.ProtocolRx.front.Cmd;
    //    return commCom.com.rxBuf[DRV_COMM_MID_ADDR];
}
/***************************************************************************************
**������:       DRV_COMM_GetRecvContent
**��������:     ��ȡ֡����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
int8_t DRV_COMM_GetRecvContent(uint8_t *readDat, uint32_t len)
{
    return DRV_Comm_GetRecvData(sizeof(commCom.ProtocolRx.front), readDat, len);
}
/***************************************************************************************
**������:       DRV_COMM_GetRecvContentSize
**��������:     ��ȡ֡���ݵĳ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
int16_t DRV_COMM_GetRecvContentSize(void)
{
    return commCom.ProtocolRx.front.size;
}
/***************************************************************************************
**������:       DRV_COMM_EnterSleepConfig
**��������:     ��������IO����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_COMM_EnterSleepConfig(void)
{
    M_RXD_OUT_PP();
    M_RXD_OUT_0();
    M_TXD_OUT_PP();
    M_TXD_OUT_0();
    M_HS6621_INT_OUT_PP();
    M_HS6621_INT_OUT_0();
    #if DEBUG_LOW_POWER
	M_CCM4202_INT_OUT_PP();
	M_CCM4202_INT_OUT_0();    
    #else
    M_CCM4202_INT_IN_NOPULL();
	M_CCM4202_INT_EXIT_RISE_ENABLE();
    #endif
}
/***************************************************************************************
**������:       DRV_Comm_WakeupBack
**��������:     ���Ѻ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Comm_WakeupBack(void)
{
    M_HS6621_INT_OUT_PP();
    M_HS6621_INT_OUT_1();
}
/***************************************************************************************
**������:       DRV_Comm_GetSumChk
**��������:     ��ȡУ���
**�������:     
uint8_t *dat, ����
uint16_t len ����
**�������:     
uint8_t
���
**��ע:         
****************************************************************************************/
uint16_t DRV_Comm_GetSumChk(uint8_t *dat, uint16_t len)
{
    uint16_t i;
    uint16_t chkTmp = NULL;
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
        chkTmp += *(dat + i);
    }
    return chkTmp;
}
/***************************************************************************************
**������:       DRV_Comm_RstBack
**��������:     ��λ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Comm_RstBack(void)
{
	#if BACK_LOW_RST >0
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_1();
	DELAY_MS(200); //��λʱ��
	M_HS6621_RSTB_OUT_0();
	DELAY_MS(100); 
	#else
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_0();
	DELAY_MS(200); //��λʱ��
	M_HS6621_RSTB_OUT_1();
	DELAY_MS(100); 
	#endif
	
}

/***************************************************************************************
**������:       DRV_Comm_WifiSend
**��������:     wifi���ݷ���
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t DRV_Comm_WifiSend(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(wifiUart, sendBuf, len);
    return 0;
}
#endif /*DRV_COMM_MODULE_EN*/
/************************************Code End******************************************/
