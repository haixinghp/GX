/***************************************************************************************
**文件名:     DRV_FaceId.c
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
#include "DRV_FaceId.h"
#include "uart_drv.h"
#include "usi_drv.h"
#ifdef DRV_FACE_ID_ENCRYPT_EN //加密使能
#include "rijndael.h"
#include "MD5.h"
#endif

#if DRV_FACE_ID_MODULE_EN
#warning DRV_FACE_ID模块使能!
/*******************************宏定义***************************/
#define DRV_FACE_ID_RX_BUF_SIZE (2048)
#define DRV_FACE_ID_TX_BUF_SIZE (512) //发送缓冲区
#define DRV_FACE_ID_RX_MAX_TIME (2) //一帧完成时间
#define DRV_FACE_ID_RX_MIN_LEN (6)  //一帧最短长度
#define DRV_FACE_ID_UART (SCI3)     //使用串口3
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
#define DRV_FACE_ID_SW_ID_SIZE (DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_MID_ID_LEN + DRV_FACE_ID_SIZE_LEN) //SyncWord	MsgID	Size总和
#define DRV_FACE_ID_PARITYCHECK_LEN (1)
#ifdef DRV_FACE_ID_ENCRYPT_EN                                                                //加密使能
#define DRV_FACE_ID_ENCRYPT_SIZE_ADDR (DRV_FACE_ID_SYNCWORD_ADDR + DRV_FACE_ID_SYNCWORD_LEN) //加密Body Size
#define DRV_FACE_ID_ENCRYPT_SIZE_LEN (2)
#define DRV_FACE_ID_ENCRYPT_ALIGN (16) //按16对齐
#define DRV_FACE_ID_ENCRYPT_DATA_ADDR (DRV_FACE_ID_ENCRYPT_SIZE_ADDR + DRV_FACE_ID_ENCRYPT_SIZE_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_MID_ADDR (DRV_FACE_ID_ENCRYPT_DATA_ADDR)
#define DRV_FACE_ID_ENCRYPT_M2H_MID_LEN (1)
#define DRV_FACE_ID_ENCRYPT_M2H_SIZE_ADDR (DRV_FACE_ID_ENCRYPT_REPLY_MID_ADDR + DRV_FACE_ID_ENCRYPT_REPLY_MID_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_SIZE_LEN (2)
#define DRV_FACE_ID_ENCRYPT_M2H_DATA_ADDR (DRV_FACE_ID_ENCRYPT_REPLY_SIZE_ADDR + DRV_FACE_ID_ENCRYPT_REPLY_SIZE_LEN)
#define DRV_FACE_ID_ENCRYPT_M2H_SW_ID_SIZE (DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_ENCRYPT_SIZE_LEN)
#define DRV_FACE_ID_AES_KEY_SIZE (16) //密码的长度
#endif

/*************************.C变量结构声明***********************/
typedef struct
{
    uint32_t rxCnt;                         //接收计数器
    uint32_t rxTim;                         //接收计时器
    uint8_t rxBuf[DRV_FACE_ID_RX_BUF_SIZE]; //接收缓冲区
    uint32_t txCnt;                         //发送计数器
    uint8_t txBuf[DRV_FACE_ID_TX_BUF_SIZE]; //发送缓冲区
} TYPEs_Com;
typedef struct
{
    uint16_t syncWord;   //固定的消息开头同步字0xEF 0xAA
    uint16_t bodySize;   //密文下加密数据包长度（MsgID+ Size+ Size）一般为16倍数，Size为有效数据长度，
    uint8_t msgID;       //消息ID (例如RESET)
    uint16_t size;       //Data size, 单位byte
    uint8_t *data;       //消息对应的data, 如command消息对应的参数。 65535>=N>=0，N=0表示此消息无参数。
    uint8_t parityCheck; //协议的奇偶校验码，计算方式为整条协议除去Sync Word部分后，其余字节按位做XOR运算。
} TYPEs_FaceProtocol;
typedef struct
{
    TYPEs_Com com;                   //硬件通讯
//    TYPEs_FaceProtocol faceProtocol; //通讯协议
} TYPEs_FaceCom;

/*************************私有全局变量*************************/
static TYPEs_FaceCom faceCom = {

}; //未初始化
static UART_TypeDef *faceUart = DRV_FACE_ID_UART;
#ifdef DRV_FACE_ID_ENCRYPT_EN //加密使能
//static FaceIdEncrypt_t faceIdEncrypt = NULL;
static uint8_t FaceIdAesKey[DRV_FACE_ID_AES_KEY_SIZE] = {
    0x00,
};                                                                                                 //AES密码
static const uint8_t FaceIdAesExtract[16] = {6, 18, 7, 3, 13, 13, 23, 4, 8, 1, 0, 25, 9, 2, 2, 7}; //双方协定从 size 为 32 的字符串中按指定规律抽取 16 个byte
#endif
/*************************公共全局变量*************************/
//IntHandler_t FaceIntHendler = NULL; 
/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       DRV_FaceId_RecvDatClear
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         接收数据前，需要做清空操作.
****************************************************************************************/
static int8_t DRV_FaceId_RecvDatClear(void)
{
    faceCom.com.rxCnt = 0;
    faceCom.com.rxTim = 0;
    memset(faceCom.com.rxBuf, 0, DRV_FACE_ID_RX_BUF_SIZE); //清空接收器
    return 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_SyncWordSet
**功能描述:     消息开头设置
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_SyncWordSet(void)
{
    faceCom.com.txBuf[DRV_FACE_ID_SYNCWORD_ADDR] = DRV_FACE_ID_SYNCWORD_H;
    faceCom.com.txBuf[DRV_FACE_ID_SYNCWORD_ADDR + 1] = DRV_FACE_ID_SYNCWORD_L;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_MsgIdSet
**功能描述:     消息ID设置
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_MsgIdSet(uint8_t cmd)
{
    faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR] = cmd;
    faceCom.com.txCnt = DRV_FACE_ID_DATA_ADDR; //自动到设置DATA段
    return 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_DataAdd(uint8_t dat)
{
    faceCom.com.txBuf[faceCom.com.txCnt] = dat;
    faceCom.com.txCnt++;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_DataAddBatch(uint8_t *dat, uint32_t len)
{
    memcpy(&faceCom.com.txBuf[faceCom.com.txCnt], dat, len);
    faceCom.com.txCnt += len;
    return 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_DataAdd
**功能描述:     添加数据
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
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
**函数名:       DRV_FaceId_ClearTxPar
**功能描述:     清空发送参数
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_ClearTxPar(void)
{
    memset(faceCom.com.txBuf, 0, DRV_FACE_ID_SIZE_ADDR);
    faceCom.com.txCnt = 0;
}
/***************************************************************************************
**函数名:       DRV_FaceId_GetOrChk
**功能描述:     获取OR的结果
**输入参数:     
uint8_t *dat, 数据
uint16_t len 长度
**输出参数:     
uint8_t
结果
**备注:         
****************************************************************************************/
static uint8_t DRV_FaceId_GetOrChk(uint8_t *dat, uint16_t len)
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
**函数名:       DRV_FaceId_SendData
**功能描述:     发送人脸数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_FaceId_SendData(uint8_t *sendBuf, uint32_t len)
{
    UART_SendData(faceUart, sendBuf, len);
}
/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvData
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
static int8_t DRV_FaceId_GetRecvData(uint32_t addr, uint8_t *readDat, uint32_t len)
{
    if (NULL == readDat)
    {
        return -1; //参数错误
    }
    memcpy(readDat, &faceCom.com.rxBuf[addr], len); //拷贝数据
    return 0;                                       //成功
}
#ifdef DRV_FACE_ID_ENCRYPT_EN //加密使能
/***************************************************************************************
**函数名:       DRV_FaceId_MoveData
**功能描述:     数据移动
**输入参数:     
uint8_t *dat, //数据指针
uint16_t size, //数据大小
int16_t offset //数据偏移量
**输出参数:     
int8_t
0; //成功
-1; //内存溢出
**备注:       PS.保证数据不溢出  
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
                return -1; //内存溢出
            }
            if (NULL == (dat + i + offset))
            {
                return -1; //内存溢出
            }
            *(dat + i + offset) = *(dat + i); //移动数据
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            if (NULL == (dat + i))
            {
                return -1; //内存溢出
            }
            if (NULL == (dat + i + offset))
            {
                return -1; //内存溢出
            }
            *(dat + i + offset) = *(dat + i); //移动数据
        }
    }
    return 0; //操作成功
}
/***************************************************************************************
**函数名:       DRV_FaceId_EncryptSizeSet
**功能描述:     设置加密后size
**输入参数:     --
**输出参数:     
int16_t
新的长度
**备注:       PS.这里需要按照指定大小对齐,默认是16bit  
****************************************************************************************/
static int16_t DRV_FaceId_EncryptSizeSet(void)
{
    uint16_t size = (((faceCom.com.txCnt - DRV_FACE_ID_DATA_ADDR) / DRV_FACE_ID_ENCRYPT_ALIGN) * DRV_FACE_ID_ENCRYPT_ALIGN) + DRV_FACE_ID_ENCRYPT_ALIGN;
    faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR] = (uint8_t)(size >> 8);
    faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR + 1] = (uint8_t)(size);
    faceCom.com.txCnt = size + DRV_FACE_ID_SYNCWORD_LEN + DRV_FACE_ID_ENCRYPT_SIZE_LEN; //更新加密后数据的长度
    return size;
}
#endif
/***************************************************************************************
**函数名:       DRV_FaceId_PackDataAndSend
**功能描述:     打包数据并发送
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
static int8_t DRV_FaceId_PackDataAndSend(uint8_t cmdType)
{
    int16_t lentmp = 0;
    uint8_t chkTmp = 0;
    if (0 != cmdType) //加密
    {
#if 1	
        DEBUG_LOG_OUT("Face encrypt send data:");
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
        DRV_FaceId_MoveData(&faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR],
                            faceCom.com.txCnt - DRV_FACE_ID_SYNCWORD_LEN,
                            2);               //数据整体后移动2Byte
        lentmp = DRV_FaceId_EncryptSizeSet(); //更新数据长度
#if 0
		DEBUG_LOG_OUT("Face encrypt send data move:"); //移动后的测试数据
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
        aes_encrypt_ecb(AES_CYPHER_128,
                        &faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                        lentmp,
                        FaceIdAesKey); //加密
        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                     lentmp); //计算校验
                                              //        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR],
                                              //                                     lentmp); //计算校验
        faceCom.com.txBuf[lentmp + DRV_FACE_ID_ENCRYPT_DATA_ADDR] = chkTmp;
        faceCom.com.txCnt += DRV_FACE_ID_PARITYCHECK_LEN;          //加上校验码的长度
        DRV_FaceId_SendData(faceCom.com.txBuf, faceCom.com.txCnt); //发送数据
#if 0
		DEBUG_LOG_OUT("Face encrypt send data encrypt:"); //发送的数据
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
#endif
    }
    else
    {
        chkTmp = DRV_FaceId_GetOrChk(&faceCom.com.txBuf[DRV_FACE_ID_MID_ID_ADDR],
                                     faceCom.com.txCnt - DRV_FACE_ID_SYNCWORD_LEN); //计算校验
        faceCom.com.txBuf[faceCom.com.txCnt] = chkTmp;
        faceCom.com.txCnt += DRV_FACE_ID_PARITYCHECK_LEN;          //加上校验码的长度
        DRV_FaceId_SendData(faceCom.com.txBuf, faceCom.com.txCnt); //发送数据
        DEBUG_LOG_OUT("Face send data:");
        DEBUG_LOG_OUT_HEX(faceCom.com.txBuf, faceCom.com.txCnt);
    }
	return 0;
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_FaceId_Init
**功能描述:     初始化人脸识别模块
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_FaceId_Init(void)
{

    UART_InitTypeDef UART_InitStruct;
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_1(); //开启模块电源
    M_X_IRQ_9555_IN_NOPULL();
    M_X_IRQ_9555_EXIT_RISE_DISABLE(); //关闭猫眼通知中断
	M_4052_EN_OUT_PP();
	M_4052_A0_OUT_PP();
	M_4052_A1_OUT_PP();
	M_4052_EN_OUT_0();
	M_4052_A0_OUT_1();
	M_4052_A1_OUT_0(); //使能Y1
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
    UART_InitStruct.UART_Mode = UART_INT_MODE; //采用CPU中断方式
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(DRV_FACE_ID_UART, &UART_InitStruct);
	
}
/***************************************************************************************
**函数名:       DRV_FaceId_OpenPow
**功能描述:     关闭人脸电源
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_FaceId_OpenPow(void)
{
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_1(); //开模块电源
}
/***************************************************************************************
**函数名:       DRV_FaceId_ClosePow
**功能描述:     关闭人脸电源
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_FaceId_ClosePow(void)
{
    M_CAM_POW_EN_OUT_PP();
    M_CAM_POW_EN_OUT_0(); //关闭模块电源
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
int8_t DRV_FaceId_RecvDataUnblock(void)
{
    uint8_t receTmp; //获取数据的临时变量
                     //    int8_t reTmp = 0;
    do
    {
        switch (UART_RecvByte(faceUart, &receTmp))
        {
        case STATUS_OK:
            faceCom.com.rxBuf[faceCom.com.rxCnt] = receTmp; //把缓冲区所有的数据读完
            faceCom.com.rxCnt++;
			if(faceCom.com.rxCnt >= DRV_FACE_ID_RX_BUF_SIZE) //超过了最大的处理空间
			{
				return 1; //接收完成,先处理
			}
            faceCom.com.rxTim = 0;
            break;
        case STATUS_NULL:
            faceCom.com.rxTim++;
			if(0 == faceCom.com.rxCnt)
			{
				return -4; //未开始接收
			}
            if (faceCom.com.rxTim >= DRV_FACE_ID_RX_MAX_TIME) //一帧接收完成
            {
                if (0 == faceCom.com.rxCnt) //查询数据长度
                {
                    faceCom.com.rxTim = 0; //清除超时，继续接收
                    return -2;             //没有数据
                }
                else
                {
                    return 1; //接收完成
                }
            }
            else
            {
                return 0; //数据接收中
            }
            break;
        default:
            return -3; //接收参数错误
            break;
        }

    } while (1);
}

/***************************************************************************************
**函数名:       DRV_FaceId_RecvDataAndAnalysis
**功能描述:     分析数据
**输入参数:     
uint8_t cmdType 0 不加密，非0 加密
**输出参数:     
int8_t
0; //校验通过
-1; //同步字校验不通过
-2; //校验不通过
**备注:         
****************************************************************************************/
int8_t DRV_FaceId_RecvDataAndAnalysis(uint8_t cmdType)
{
    uint16_t lenTmp = 0;
    int8_t reTmp = 0;
    uint8_t chksumTmp = 0;
    if ((DRV_FACE_ID_SYNCWORD_H == faceCom.com.rxBuf[DRV_FACE_ID_SYNCWORD_ADDR]) &&
        (DRV_FACE_ID_SYNCWORD_L == faceCom.com.rxBuf[DRV_FACE_ID_SYNCWORD_ADDR + 1])) //检查同步字0XEFAA
    {
        if (0 != cmdType) //加密
        {
            lenTmp = (faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR] << 8) +
                     faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_SIZE_ADDR + 1];
            chksumTmp = DRV_FaceId_GetOrChk(&faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR], lenTmp); //计算校验和
            if (chksumTmp == faceCom.com.rxBuf[lenTmp + DRV_FACE_ID_ENCRYPT_M2H_SW_ID_SIZE])            //校验通过
            {
				#if 0
				DEBUG_LOG_OUT("face decrypt revc data ecrypt:"); //原始加密数据
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
				#endif
                aes_decrypt_ecb(AES_CYPHER_128,
                                &faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                lenTmp,
                                FaceIdAesKey); //解密
                #if 0
				DEBUG_LOG_OUT("face decrypt revc data move :"); //解密后未移动的数据
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
                #endif
                DRV_FaceId_MoveData(&faceCom.com.rxBuf[DRV_FACE_ID_ENCRYPT_DATA_ADDR],
                                    lenTmp + DRV_FACE_ID_PARITYCHECK_LEN,
                                    -2); //数据整体向前移动2Byte,获取到原始数据
				#if 1
                DEBUG_LOG_OUT("face decrypt revc data:");
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
				#endif
                return 0; //校验通过
            }
            else
            {

                return -2; //校验不通过
            }
        }
        else //未加密
        {
            lenTmp = (faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR] << 8) +
                     faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR + 1];
            chksumTmp = DRV_FaceId_GetOrChk(&faceCom.com.rxBuf[DRV_FACE_ID_DATA_ADDR], lenTmp); //计算校验和
            if (chksumTmp == faceCom.com.rxBuf[lenTmp + DRV_FACE_ID_SW_ID_SIZE])
            {
                DEBUG_LOG_OUT("face revc data:");
                DEBUG_LOG_OUT_HEX(faceCom.com.rxBuf, faceCom.com.rxCnt);
                return 0; //校验通过
            }
            else
            {
                return -2; //校验不通过
            }
        }
    }
    else
    {
        return -1; //同步字校验不通过
    }
}
/***************************************************************************************
**函数名:       DRV_FaceId_CmdSend
**功能描述:     发送值指令
**输入参数:     
uint8_t mid, MID
uint8_t *dat, 数据指针
uint32_t datLen 数据长度
**输出参数:     
int8_t
0 成功
**备注:         
****************************************************************************************/
int8_t DRV_FaceId_CmdSend(uint8_t mid, uint8_t *dat, uint32_t datLen, uint8_t cmdType)
{
    DRV_FaceId_ClearTxPar();
    DRV_FaceId_SyncWordSet();
    DRV_FaceId_MsgIdSet(mid);
    DRV_FaceId_DataAddBatch(dat, datLen);
    DRV_FaceId_SizeSet();
    DRV_FaceId_PackDataAndSend(cmdType); //发送数据
    return 0;
}

/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvMsgType
**功能描述:     获取接收的消息类型
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvMsgType(void)
{
    return faceCom.com.rxBuf[DRV_FACE_ID_MID_ID_ADDR];
}
/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvReplyMid
**功能描述:     获取接收的mid
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvReplyMid(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_MID_ADDR, readDat, len);
}
///***************************************************************************************
//**函数名:       DRV_FaceId_GetRecvReplyResult
//**功能描述:     接收reply的结果
//**输入参数:
//uint8_t *readDat,数据指针
//uint32_t len，数据长度
//**输出参数:
//int8_t
//-1; //参数错误
//0; //成功
//**备注:
//****************************************************************************************/
//int8_t DRV_FaceId_GetRecvReplyResult(uint8_t *readDat, uint32_t len)
//{
//    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_MID_ADDR, readDat, len);
//}
/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvRepley
**功能描述:     接收reply数据
**输入参数:    
uint8_t *readDat,数据指针
uint32_t len，数据长度
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t DRV_FaceId_GetRecvReplyData(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_REPLY_DATA_ADDR, readDat, len);
}
/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvNoteNid
**功能描述:     获取接收的nid
**输入参数:     --
**输出参数:  
uint8_t
获取到的数据
**备注:         
****************************************************************************************/
uint8_t DRV_FaceId_GetRecvNoteNid(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_NOTE_NID_ADDR, readDat, len);
}

/***************************************************************************************
**函数名:       DRV_FaceId_GetRecvNoteData
**功能描述:     接收note数据
**输入参数:    
uint8_t *readDat,数据指针
uint32_t len，数据长度
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t DRV_FaceId_GetRecvNoteData(uint8_t *readDat, uint32_t len)
{
    return DRV_FaceId_GetRecvData(DRV_FACE_ID_NOTE_DATA_ADDR, readDat, len);
}
/***************************************************************************************
**函数名:       DRV_FaceId_ClearRxPar
**功能描述:     清除接收
**输入参数:     --
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
int8_t DRV_FaceId_ClearRxPar(void)
{
    memset(faceCom.com.rxBuf, 0, DRV_FACE_ID_RX_BUF_SIZE);
    faceCom.com.rxCnt = 0;
    faceCom.com.rxTim = 0;
    return 0;
}
#ifdef DRV_FACE_ID_ENCRYPT_EN //加密使能
///***************************************************************************************
//**函数名:       DRV_FaceId_SendEncryptReg
//**功能描述:     注册加密程序
//**输入参数:
//fFaceIdSendencrypt_t fun
//**输出参数:
//int8_t
//-1; //参数错误
//0; //参数正确
//**备注:
//****************************************************************************************/
//int8_t DRV_FaceId_SendEncryptReg(FaceIdEncrypt_t fun)
//{
//    if (NULL == fun)
//    {
//        return -1; //参数错误
//    }
//    faceIdEncrypt = fun;
//    return 0; //参数正确
//}
/***************************************************************************************
**函数名:       DRV_FaceId_GetAesKey
**功能描述:     获取AES key
**输入参数:     
uint8_t *dat
**输出参数:     
int8_t
-1; //参数错误
0; //成功
**备注:       
随机序列
随机序列定义为 char data[4];
4 个字符，用户可采用时间作为种子生成一个任意随机数
从随机序列生成 16byte 密码
4byte 的随机序列做 md5 生成 16 byte （32 个字符） 序列如：
fbe0 aa53 6fc3 49cb dc45 1ff5 970f 9357 
[0	->	31]
转化为字符串
双方协定从 size 为 32 的字符串中按指定规律抽取 16 个byte，抽取的逻辑为：
[6-17-8-5	11-15-3-17	6-23-1-6	31-2-2-7]
本例中抽取到的随机密码为：5 c6 a	3 b 0 c	5 5 b 5	7e e 3
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
        return -1; //参数错误
    }
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, dat, 4);
    MD5Final(&md5, md5Tmp);
    for (i = 0; i < 16; i++)
    {
        md5TmpByte = md5Tmp[i] >> 4; //处理高位
        if (md5TmpByte <= 9)
        {
            md5TmpByte += '0';
        }
        else
        {
            md5TmpByte += ('a' - 10);
        }
        md5TmpStr[i * 2] = md5TmpByte;
        md5TmpByte = md5Tmp[i] & 0x0f; //处理低位
        if (md5TmpByte <= 9)
        {
            md5TmpByte += '0';
        }
        else
        {
            md5TmpByte += ('a' - 10);
        }
        md5TmpStr[i * 2 + 1] = md5TmpByte;
    } // 16 byte （32 个字符） 转化为字符串
    for (i = 0; i < 16; i++)
    {
        FaceIdAesKey[i] = md5TmpStr[FaceIdAesExtract[i]];
        //这里需要保存eas key数据
    }
    return 0; //成功
}
#endif
/***************************************************************************************
**函数名:       DRV_Face_EnterSleepConfig
**功能描述:     进入低功耗配置
**输入参数:     --
**输出参数:     --
**备注:         
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
    #ifdef USE_FACE_AOBI //使用奥比人脸
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
	M_4052_A1_OUT_0(); //使能Y0
}
/***************************************************************************************
**函数名:       DRV_Face_WifiPowOpen
**功能描述:     人脸wifi电源打开 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Face_WifiPowOpen(void)
{
    #ifdef USE_FACE_AOBI //使用奥比人脸
    M_X_FACE_POW_EN_OUT_PP();
    M_X_FACE_POW_EN_OUT_1();
    #if 0
    M_X_CHANGE_9555_OUT_PP();
    M_X_CHANGE_9555_OUT_1();
    #endif
    #endif
    #ifdef USE_FACE_SHANGTANG //使用商汤人脸
    M_CAM_WIFI_POW_EN_OUT_PP();
	M_CAM_WIFI_POW_EN_OUT_1();
    #endif
}
/***************************************************************************************
**函数名:       DRV_Face_WifiPowClose
**功能描述:     人脸wifi电源关闭 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Face_WifiPowClose(void)
{
    #ifdef USE_FACE_AOBI //使用奥比人脸
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
    #ifdef USE_FACE_SHANGTANG //使用商汤人脸
    M_CAM_WIFI_POW_EN_OUT_PP();
    M_CAM_WIFI_POW_EN_OUT_0();
    #endif
}
/***************************************************************************************
**函数名:       DRV_Face_CateyeFaceSw
**功能描述:     猫眼人脸切换 用于奥比  
**输入参数:     --
**输出参数:     --
**备注:         0 人脸，1猫眼
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
**函数名:       DRV_FaceId_GetRecvMsgSize
**功能描述:     获取接收的消息的长度
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint16_t DRV_FaceId_GetRecvMsgSize(void)
{
    return ((faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR] << 8) + faceCom.com.rxBuf[DRV_FACE_ID_SIZE_ADDR+1]);
}

/***************************************************************************************
**函数名:       DRV_FaceId_IRSleepConfig
**功能描述:     IR身体感应休眠配置
**输入参数:     
uint8_t sw		0,关闭中断；非0，开启中断
**输出参数:     --
**备注:         
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
