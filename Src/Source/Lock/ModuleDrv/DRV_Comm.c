/***************************************************************************************
**文件名:     DRV_Comm.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
通讯协议：
字节属性	字节长度	字节内容
包头H	    1	        FE 
包头L	    1	        02代表无后续数据 03代表有后续数据（服务器有应答）
命令字	    1	        变化
加密	    1	        0不加密，(1-255)为加密
会话密钥	6	        变化
数据长度	2           （数据长度）	变化【帧内容字节数据】
帧内容	    变长	    变化(未尾带1字节检验和)
校验和	    2	        包头第2字节到帧内容之间和
****************************************************************************************/
#include "DRV_Comm.h"
#include "rijndael.h"
#include "trng_drv.h"
#if DRV_COMM_MODULE_EN
#warning DRV_COMM模块使能!
/*******************************宏定义***************************/
#if 0
#define DRV_COMM_UART_BPS (115200)  //通讯波特率
#else
#define DRV_COMM_UART_BPS (460800)  //通讯波特率
#endif
#define DRV_COMM_UART (SCI1)        //使用的串口
#define DRV_COMM_WIFI_UART (SCI2)        //使用的串口
#define DRV_COMM_RX_BUF_SIZE (1024) //接收缓存
#define DRV_COMM_TX_BUF_SIZE (512)  //发送缓存
#define DRV_COMM_RX_MAX_TIME (20)   //一帧接收完成
#define DRV_COMM_SYNCWORD_ADDR (0)  //协议头地址
#define DRV_COMM_SYNCWORD_H (0XFE)  //包头
#define DRV_COMM_SYNCWORD_H_SIZE (1)
#define DRV_COMM_SYNCWORD_L_SIZE (1)
#define DRV_COMM_SYNCWORD_SIZE (2)
#define DRV_COMM_MID_ADDR (DRV_COMM_SYNCWORD_ADDR + DRV_COMM_SYNCWORD_SIZE)      //命令字地址
#define DRV_COMM_MID_SIZE (1)                                                    //命令字长度
#define DRV_COMM_SECRET_KEY_ADDR (DRV_COMM_MID_ADDR + DRV_COMM_MID_SIZE)         //会话密钥地址
#define DRV_COMM_SECRET_KEY_SIZE (6)                                             //会话密钥长度
#define DRV_COMM_SIZE_ADDR (DRV_COMM_SECRET_KEY_ADDR + DRV_COMM_SECRET_KEY_SIZE) //数据长度地址
#define DRV_COMM_SIZE_SIZE (2)                                                   //数据长度的长度
#define DRV_COMM_DATA_ADDR (DRV_COMM_SIZE_ADDR + DRV_COMM_SIZE_SIZE)             //帧内容开始地址
#define DRV_COMM_FIXED_SIZE (DRV_COMM_SYNCWORD_SIZE +   \
                             DRV_COMM_MID_SIZE +        \
                             DRV_COMM_SECRET_KEY_SIZE + \
                             DRV_COMM_SIZE_SIZE) //协议中固定的大小 包头H1 + 包头L1 + 命令字1 + 数据长度2
#define DRV_COMM_CHK_START_ADDR (1)              //校验开始地址
#define DRV_COMM_CHK_FIXED_SIZE (DRV_COMM_SYNCWORD_L_SIZE + \
                                 DRV_COMM_MID_SIZE +        \
                                 DRV_COMM_SECRET_KEY_SIZE + \
                                 DRV_COMM_SIZE_SIZE) //校验时固定的数据大小
#define DRV_COMM_CHK_SIZE (2)                        //校验数据长度
#define DRV_COMM_ENCRYPT_ALIGN (16) //按16对齐
#define DRV_COMM_AES_KEY_SIZE (16)
/*************************.C变量结构声明***********************/
#pragma pack(1) //这里强制使用1字节对齐的方式
typedef struct
{
    uint32_t rxCnt;                      //接收计数器
    uint32_t rxTim;                      //接收计时器
    uint8_t rxBuf[DRV_COMM_RX_BUF_SIZE]; //接收缓冲区
    uint32_t txCnt;                      //发送计数器
    uint8_t txBuf[DRV_COMM_TX_BUF_SIZE]; //发送缓冲区
    uint16_t dataLen;                    //数据长度
} TYPEs_Com;
typedef struct
{
    uint8_t HeardH;        //包头H
    uint8_t HeardL;        //包头L
    uint8_t Cmd;           //命令字
    uint8_t Encrypt;       //加密
    uint8_t EncryptKey[6]; //会话密钥
    uint16_t size;         //数据长度
} TYPEs_CommProtocolFront;
typedef struct
{
    TYPEs_CommProtocolFront front;
    uint8_t *data; //帧内容指针
    uint16_t chk;  //校验和
} TYPEs_CommProtocol;
typedef struct
{
    TYPEs_Com com;                 //硬件通讯
    TYPEs_CommProtocol ProtocolTx; //通讯协议发送
    TYPEs_CommProtocol ProtocolRx; //通讯协议接收
    uint16_t lenAddr;              //长度的地址
    uint16_t fixedLen;             //协议中固定的长度
} TYPEs_CommCom;
#pragma pack() //取消对齐
/*************************私有全局变量*************************/
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
static UART_TypeDef *commUart = DRV_COMM_UART; //前后通讯串口
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
uint8_t rootKeyA[6]={0,}; //根密钥root
uint8_t rootKeyRandom[6]={0,}; //根密钥随机数

/*************************公共全局变量*************************/
uint8_t *pDrvCommMac =NULL; //MAC指针
uint8_t *pDrvCommRootKeyA = NULL; //指向ROOT的指针
/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       DRV_Comm_GetUid
**功能描述:     获取UID
**输入参数:     
uint8_t *dat, 数据
**输出参数:     --
结果
**备注:         
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
**函数名:       DRV_Comm_GetRandom
**功能描述:     获取随机数
**输入参数:     
uint8_t *dat, 数据
**输出参数:     --
结果
**备注:         
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
**函数名:       DRV_Comm_GetRandomuEncrypt
**功能描述:     获取随机数加密位
**输入参数:     --
**输出参数:     
uint8_t
不为0的随机数
结果
**备注:         
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
**函数名:       DRV_Comm_GetAesKey
**功能描述:     获取获取AES密钥
**输入参数:     
uint8_t *dat, 会话密钥
uint8_t encrypt 机密位置
**输出参数:     --
结果
**备注:         
****************************************************************************************/
void DRV_Comm_GetAesKey(uint8_t *dat,uint8_t encrypt)
{
    uint16_t i =0;
    memcpy(CommAesKey,
           pDrvCommMac,
           4); //拷贝MAC
    for(i=0;i<4;i++)
    {
        CommAesKey[i] = ~CommAesKey[i]; //MAC高4byte取反
    }
    memcpy(CommAesKey+4,
           dat,
           6); //会话密钥
    memcpy(CommAesKey+4+6,
           pDrvCommRootKeyA,
           6); //根密钥
    encrypt = encrypt %100/10; //获取十位
    for(i=0;i<16;i++)
    {
        CommAesKey[i] ^= commKey[encrypt][i%6]; //循环异或
    }
}
/***************************************************************************************
**函数名:       DRV_Comm_RecvDatClear
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         接收数据前，需要做清空操作.
****************************************************************************************/
static int8_t DRV_Comm_RecvDatClear(void)
{
    commCom.com.rxCnt = 0;
    commCom.com.rxTim = 0;
    memset(commCom.com.rxBuf, 0, DRV_COMM_RX_BUF_SIZE); //清空接收器
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_SyncWordSet
**功能描述:     消息开头设置
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_SyncWordSet(TYPEe_CommHead datType)
{
    commCom.com.txBuf[commCom.com.txCnt++] = DRV_COMM_SYNCWORD_H;
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(datType);
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_MsgIdSet
**功能描述:     消息ID设置
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_MsgIdSet(uint8_t cmd)
{
    commCom.com.txBuf[commCom.com.txCnt++] = cmd;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_SecretKey
**功能描述:     设置
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_SecretKey(uint8_t *key)
{
    memcpy((uint8_t *)(&commCom.com.txBuf[commCom.com.txCnt]),
           key,
           DRV_COMM_SECRET_KEY_SIZE);
    commCom.com.txCnt += DRV_COMM_SECRET_KEY_SIZE;
    //    commCom.com.txCnt = DRV_COMM_DATA_ADDR; //后续的数据填写到帧内容
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_DataAdd(uint8_t dat)
{
    commCom.com.txBuf[commCom.com.txCnt] = dat;
    commCom.com.txCnt++;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_DataAddBatch(uint8_t *dat, uint32_t len)
{
    memcpy(&commCom.com.txBuf[commCom.com.txCnt], dat, len);
    commCom.com.txCnt += len;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_SizeAddEmpty(void)
{
    commCom.lenAddr = commCom.com.txCnt; //记录地址
    commCom.com.txBuf[commCom.com.txCnt++] = 0x00;
    commCom.com.txBuf[commCom.com.txCnt++] = 0x00; //这里占用2个空位
    commCom.fixedLen = commCom.com.txCnt;          //设置地址后记录系统的固定长度。
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
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
**函数名:       DRV_Comm_ClearTxPar
**功能描述:     清空发送参数
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_ClearTxPar(void)
{
    memset(commCom.com.txBuf, 0, DRV_COMM_TX_BUF_SIZE);
    commCom.com.txCnt = 0;
}
///***************************************************************************************
//**函数名:       DRV_Comm_GetOrChk
//**功能描述:     获取OR的结果
//**输入参数:
//uint8_t *dat, 数据
//uint16_t len 长度
//**输出参数:
//uint8_t
//结果
//**备注:
//****************************************************************************************/
//static uint16_t DRV_Comm_GetSumChk(uint8_t *dat, uint16_t len)
//{
//    uint16_t i;
//    uint16_t chkTmp = NULL;
//    if (NULL == dat)
//    {
//        return 0; //错误
//    }
//    chkTmp = *dat;
//    for (i = 1; i < len; i++)
//    {
//        if (NULL == (dat + i))
//        {
//            return 0; //错误
//        }
//        chkTmp += *(dat + i);
//    }
//    return chkTmp;
//}
/***************************************************************************************
**函数名:       DRV_Comm_GetOrChk
**功能描述:     获取OR的结果
**输入参数:     
uint8_t *dat, 数据
uint16_t len 长度
**输出参数:     
uint8_t
结果
**备注:         
****************************************************************************************/
static uint8_t DRV_Comm_GetOrChk(uint8_t *dat, uint16_t len)
{
    uint16_t i;
    uint8_t chkTmp = NULL;
    if (NULL == dat)
    {
        return 0; //错误
    }
    chkTmp = *dat;
    for (i = 1; i < len; i++)
    {
        if (NULL == (dat + i))
        {
            return 0; //错误
        }
        chkTmp ^= *(dat + i);
    }
    return chkTmp;
}

/***************************************************************************************
**函数名:       DRV_Comm_SetChk
**功能描述:     设置校验和
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static uint16_t DRV_Comm_SetChk(void)
{
    uint16_t chkTmp = 0;
    chkTmp = DRV_Comm_GetSumChk(&commCom.com.txBuf[DRV_COMM_CHK_START_ADDR],
                                commCom.com.dataLen + DRV_COMM_CHK_FIXED_SIZE); //计算校验和
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(chkTmp >> 8);
    commCom.com.txBuf[commCom.com.txCnt++] = (uint8_t)(chkTmp);
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_SendData
**功能描述:     发送数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Comm_SendData(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(commUart, sendBuf, len);
}
/***************************************************************************************
**函数名:       DRV_Comm_GetRecvData
**功能描述:     获取接收的数据
**输入参数:   
uint32_t addr, 读取地址  
uint8_t *readDat,数据指针
uint32_t len，数据长度
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_GetRecvData(uint32_t addr, uint8_t *readDat, uint32_t len)
{
    if (NULL == readDat)
    {
        return -1; //参数错误
    }
    memcpy(readDat, &commCom.com.rxBuf[addr], len); //拷贝数据
    return 0;                                       //成功
}
/***************************************************************************************
**函数名:       DRV_FaceId_PackDataAndSend
**功能描述:     打包数据并发送
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_Comm_PackDataAndSend(TYPEe_CommEncrypt cmdType)
{
    DRV_Comm_SendData(commCom.com.txBuf, commCom.com.txCnt); //发送数据
    DEBUG_LOG_OUT("DRV_Comm_PackDataAndSend:\n");
    DEBUG_LOG_OUT_HEX(commCom.com.txBuf, commCom.com.txCnt);
    return 0;
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_Comm_Init
**功能描述:     通讯初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Comm_Init(void)
{
    UART_InitTypeDef UART_InitStruct;
    UART_InitStruct.UART_BaudRate = DRV_COMM_UART_BPS;
    UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
    UART_InitStruct.UART_Mode = UART_INT_MODE; //采用CPU中断方式
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(DRV_COMM_UART, &UART_InitStruct);
    M_HS6621_INT_OUT_PP();
    M_HS6621_INT_OUT_1();
    M_CCM4202_INT_IN_NOPULL();
    DRV_Comm_ClearRxPar(); //清除接收
    //	memset(&commCom,0,sizeof(commCom));
}
/***************************************************************************************
**函数名:       DRV_FaceId_RecvDataUnblock
**功能描述:     非阻塞式接收数据
**输入参数:     
uint8_t *recvBuf, 数据指针
uint16_t *len 数据长度
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
int8_t DRV_Comm_RecvDataUnblock(void)
{
    uint8_t receTmp; //获取数据的临时变量
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

            commCom.com.rxBuf[commCom.com.rxCnt] = receTmp; //把缓冲区所有的数据读完
            commCom.com.rxCnt++;
            if (commCom.com.rxCnt >= DRV_COMM_RX_BUF_SIZE) //超过了最大的处理空间
            {
                DEBUG_LOG_OUT("DRV_Comm_RecvDataUnblock commCom.com.rxCnt >= DRV_COMM_RX_BUF_SIZE\n");
                return 1; //接收完成,先处理
            }
            commCom.com.rxTim = 1;
        }
        else
        {
            if (0 != commCom.com.rxTim)
            {
                commCom.com.rxTim++;
                if (commCom.com.rxTim >= DRV_COMM_RX_MAX_TIME) //一帧接收完成
                {
                    if (0 == commCom.com.rxCnt) //查询数据长度
                    {
                        commCom.com.rxTim = 0; //清除超时，继续接收
                        return -2;             //没有数据
                    }
                    else
                    {
                        DEBUG_LOG_OUT("DRV_Comm_RecvDataUnblock OK\n");
                        commCom.com.rxTim = 0; //清除超时，继续接收
                        return 1;              //接收完成
                    }
                }
                else
                {
                    return 0; //数据接收中
                }
            }
            else
            {
                return -4; //未开始接收
            }
        }
    } while (1);
}
/***************************************************************************************
**函数名:       DRV_Comm_CmdSend
**功能描述:     发送值指令
**输入参数:    
TYPEe_CommHead datType 数据类型 
uint8_t mid, MID 命令字
uint8_t *secretKey 密钥
uint8_t *dat, 数据指针
uint32_t datLen 数据长度
uint8_t cmdType 加密标志，0明文，1密文
**输出参数:     
int8_t
0 成功
**备注:         
********************************************************************************K********/
int8_t DRV_Comm_CmdSend(TYPEs_ComSendParm *p)
{
    uint16_t chkTmp;        //帧内容之和
    uint16_t const lenHead=sizeof(TYPEs_CommProtocolFront); //数据头的长度
    uint16_t lenTmp =0; //长度缓存
    uint16_t lenEncrypt =0; //加密后测长度
    TYPEs_CommProtocolFront *pProtocolFront = (TYPEs_CommProtocolFront *) commCom.com.txBuf;
    uint8_t *pTxBuf = commCom.com.txBuf;
    DRV_Comm_ClearTxPar(); //清除发送缓存
    pProtocolFront->size = 0;
    pProtocolFront->HeardH = DRV_COMM_SYNCWORD_H;
    pProtocolFront->HeardL = (uint8_t)p->datType;
    pProtocolFront->Cmd = p->mid;
    pProtocolFront->Encrypt = (uint8_t)p->secretKeyFlag;
    DEBUG_LOG_OUT("EncryptKey:\n");
    DEBUG_LOG_OUT_HEX(pProtocolFront->EncryptKey, 6);
    memcpy((uint8_t *)(pTxBuf + lenHead),
           (uint8_t *)(p->dat),
           p->datLen);                                     //添加数据
           
    if (0 != p->datLen)
    {
        chkTmp = DRV_Comm_GetSumChk((pTxBuf +lenHead), p->datLen); //帧内容之和
        *(pTxBuf + lenHead+p->datLen) = (uint8_t)(chkTmp);
        DEBUG_LOG_OUT("chkTmp:%X\n", (uint8_t)(chkTmp));
    }     //加上帧内容之和
    pProtocolFront->size =  p->datLen + 1; //帧内容长度
    commCom.com.txCnt = pProtocolFront->size + lenHead; //更新总长度
    DEBUG_LOG_OUT("old data:\n");
    DEBUG_LOG_OUT_HEX(commCom.com.txBuf, commCom.com.txCnt);
    if (eSecretKeyDis != p->secretKeyFlag) //加密
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
        DRV_Comm_GetAesKey(rootKeyRandom,pProtocolFront->Encrypt); //获取通讯密钥
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
                        DRV_COMM_ENCRYPT_ALIGN;    //计算对齐长度。      
        DEBUG_LOG_OUT("size%d\n", pProtocolFront->size);
        aes_encrypt_ecb(AES_CYPHER_128,
                        &pTxBuf[lenHead],
                        lenEncrypt,
                        CommAesKey); //加密
        commCom.com.txCnt = lenEncrypt + lenHead; //更新总长度
    }    
    BSWAP_SAVE_16(pProtocolFront->size); //大小端转换
    chkTmp = DRV_Comm_GetSumChk((uint8_t *)(&pProtocolFront->HeardL), 
                                                 commCom.com.txCnt - sizeof(pProtocolFront->HeardH)); //计算校验和
    BSWAP_SAVE_16(chkTmp); //大小端转换
    memcpy((uint8_t *)(pTxBuf + commCom.com.txCnt),
           (uint8_t *)(&chkTmp),
           2);              //添加校验和
    commCom.com.txCnt += 2; //加上校验和长度
    
    DEBUG_LOG_OUT("commCom.com.txCnt:%d\n", commCom.com.txCnt);
    DRV_Comm_PackDataAndSend(p->cmdType); //发送数据
    return 0;
}
/***************************************************************************************
**函数名:       DRV_Comm_ClearRxPar
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t DRV_Comm_ClearRxPar(void)
{
    memset(commCom.com.rxBuf, 0, DRV_COMM_RX_BUF_SIZE);
    commCom.com.rxCnt = 0;
    commCom.com.rxTim = 0;
    return 0;
}

/***************************************************************************************
**函数名:       DRV_Comm_RecvDataAndAnalysis
**功能描述:     分析数据
**输入参数:     
uint8_t cmdType 0 不加密，非0 加密
**输出参数:     
int8_t
0; //校验通过
-1; //同步字校验不通过
-2; //校验不通过
-3; //内容帧校验错误
**备注:         
****************************************************************************************/
int8_t DRV_Comm_RecvDataAndAnalysis(TYPEe_CommEncrypt cmdType)
{
    uint16_t lenTmp = 0;
    uint16_t lenTmpOld = 0; //原长
    int8_t reTmp = 0;
    uint8_t chksumTmp = 0;
    uint16_t const lenHead=sizeof(TYPEs_CommProtocolFront); //数据头的长度
    TYPEs_CommProtocolFront *pProtocolFront = (TYPEs_CommProtocolFront *) commCom.com.rxBuf;
    uint16_t chksum=0;
    if (DRV_COMM_SYNCWORD_H == commCom.com.rxBuf[DRV_COMM_SYNCWORD_ADDR]) //校验文件头
    {
        if (eSecretKeyDis != pProtocolFront->Encrypt) //加密
        {
            DRV_Comm_GetAesKey(pProtocolFront->EncryptKey,pProtocolFront->Encrypt); //获取通讯密钥
            #if 0
            DEBUG_LOG_OUT("CommAesKey:\n");
            DEBUG_LOG_OUT("mac:\n");
            DEBUG_LOG_OUT_HEX(pDrvCommMac,6); 
            DEBUG_LOG_OUT("root:\n");
            DEBUG_LOG_OUT_HEX(rootKeyA,6);
            DEBUG_LOG_OUT("random:\n");
            DEBUG_LOG_OUT_HEX(CommAesKey,sizeof(CommAesKey)); 
            #endif
            lenTmpOld = BSWAP_16(pProtocolFront->size) ; //获取长度
            lenTmp = (((lenTmpOld)/DRV_COMM_ENCRYPT_ALIGN)*DRV_COMM_ENCRYPT_ALIGN)+
                        DRV_COMM_ENCRYPT_ALIGN;    //计算对齐长度。
            chksum = DRV_Comm_GetSumChk((uint8_t *)&pProtocolFront->HeardL, lenTmp+lenHead-1); //计算校验和
            DEBUG_LOG_OUT("chksum:%X\n",chksum);
            if (chksum == (commCom.com.rxBuf[lenTmp + lenHead]<<8)+commCom.com.rxBuf[lenTmp + lenHead+1]) //校验
            {
                #if 0
				DEBUG_LOG_OUT("comm decrypt revc data ecrypt:"); //原始加密数据
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                aes_decrypt_ecb(AES_CYPHER_128,
                                &commCom.com.rxBuf[lenHead],
                                lenTmp,
                                CommAesKey); //解密
                #if 1
				DEBUG_LOG_OUT("comm decrypt revc data move :"); //解密后的数据
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                chksum = DRV_Comm_GetSumChk((uint8_t *)(commCom.com.rxBuf+lenHead), 
                                            lenTmpOld-1); //计算内容帧校验和
                #if 0
                DEBUG_LOG_OUT("chksum:%X\n",chksum);
                #endif
                if((uint8_t)chksum == commCom.com.rxBuf[lenHead+ lenTmpOld-1])
                {
                    DEBUG_LOG_OUT("comm decrypt chk ok\n"); //内容校验通过
                    memcpy((uint8_t *)(&commCom.ProtocolRx.front),
                           (uint8_t *)(commCom.com.rxBuf),
                           sizeof(commCom.ProtocolRx.front));
                    BSWAP_SAVE_16(commCom.ProtocolRx.front.size);
                    return 0; //校验通过
                }
                else
                {
                    return -3; //内容帧校验错误
                }
            }
            else
            {
                DEBUG_LOG_OUT("comm decrypt chk error\n"); //原始加密数据
                return -2; //校验不通过
            }
        }
        else //未加密
        {
            #if 1
            memcpy((uint8_t *)(&commCom.ProtocolRx.front),
                   (uint8_t *)(commCom.com.rxBuf),
                   sizeof(commCom.ProtocolRx.front));
            BSWAP_SAVE_16(commCom.ProtocolRx.front.size);
            lenTmpOld = commCom.ProtocolRx.front.size ; //获取长度
            lenTmp = lenTmpOld;
            chksum = DRV_Comm_GetSumChk((uint8_t *)&pProtocolFront->HeardL, lenTmp+lenHead-1); //计算校验和
            DEBUG_LOG_OUT("chksum:%X\n",chksum);
            if (chksum == (commCom.com.rxBuf[lenTmp + lenHead]<<8)+commCom.com.rxBuf[lenTmp + lenHead+1]) //校验
            {
                #if 1
				DEBUG_LOG_OUT("comm  revc data :"); //原始加密数据
                DEBUG_LOG_OUT_HEX(commCom.com.rxBuf, commCom.com.rxCnt);
                #endif
                
                chksum = DRV_Comm_GetSumChk((uint8_t *)(commCom.com.rxBuf+lenHead), 
                                            lenTmpOld-1); //计算内容帧校验和
                DEBUG_LOG_OUT("chksum:%X\n",chksum);
                if((uint8_t)chksum == commCom.com.rxBuf[lenHead+ lenTmpOld-1])
                {
                    DEBUG_LOG_OUT("comm chk ok\n"); //内容校验通过
                    return 0; //校验通过
                }
                else
                {
                    return -3; //内容帧校验错误
                }
                
            }
            else
            {
                DEBUG_LOG_OUT("comm  chk error\n"); //原始加密数据
                return -2; //校验不通过
            }
            return 0;
            #endif
        }
    }
    else
    {
        return -1; //同步字校验不通过
    }
}
/***************************************************************************************
**函数名:       DRV_COMM_GetRecvMsgType
**功能描述:     获取接收的消息类型
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t DRV_COMM_GetRecvMsgType(void)
{
    return commCom.ProtocolRx.front.Cmd;
    //    return commCom.com.rxBuf[DRV_COMM_MID_ADDR];
}
/***************************************************************************************
**函数名:       DRV_COMM_GetRecvContent
**功能描述:     获取帧内容
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t DRV_COMM_GetRecvContent(uint8_t *readDat, uint32_t len)
{
    return DRV_Comm_GetRecvData(sizeof(commCom.ProtocolRx.front), readDat, len);
}
/***************************************************************************************
**函数名:       DRV_COMM_GetRecvContentSize
**功能描述:     获取帧内容的长度
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int16_t DRV_COMM_GetRecvContentSize(void)
{
    return commCom.ProtocolRx.front.size;
}
/***************************************************************************************
**函数名:       DRV_COMM_EnterSleepConfig
**功能描述:     进入休眠IO配置
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       DRV_Comm_WakeupBack
**功能描述:     唤醒后板
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Comm_WakeupBack(void)
{
    M_HS6621_INT_OUT_PP();
    M_HS6621_INT_OUT_1();
}
/***************************************************************************************
**函数名:       DRV_Comm_GetSumChk
**功能描述:     获取校验和
**输入参数:     
uint8_t *dat, 数据
uint16_t len 长度
**输出参数:     
uint8_t
结果
**备注:         
****************************************************************************************/
uint16_t DRV_Comm_GetSumChk(uint8_t *dat, uint16_t len)
{
    uint16_t i;
    uint16_t chkTmp = NULL;
    if (NULL == dat)
    {
        return 0; //错误
    }
    chkTmp = *dat;
    for (i = 1; i < len; i++)
    {
        if (NULL == (dat + i))
        {
            return 0; //错误
        }
        chkTmp += *(dat + i);
    }
    return chkTmp;
}
/***************************************************************************************
**函数名:       DRV_Comm_RstBack
**功能描述:     复位后板
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Comm_RstBack(void)
{
	#if BACK_LOW_RST >0
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_1();
	DELAY_MS(200); //复位时间
	M_HS6621_RSTB_OUT_0();
	DELAY_MS(100); 
	#else
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_0();
	DELAY_MS(200); //复位时间
	M_HS6621_RSTB_OUT_1();
	DELAY_MS(100); 
	#endif
	
}

/***************************************************************************************
**函数名:       DRV_Comm_WifiSend
**功能描述:     wifi数据发送
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
int8_t DRV_Comm_WifiSend(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(wifiUart, sendBuf, len);
    return 0;
}
#endif /*DRV_COMM_MODULE_EN*/
/************************************Code End******************************************/
