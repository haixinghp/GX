/***************************************************************************************
**文件名:     HAL_Comm.c
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
#include "HAL_Comm.h"
#include "DRV_Comm.h"
#if HAL_COMM_MODULE_EN
#warning HAL_COMM模块使能!
/*******************************宏定义***************************/
#define HAL_COMM_WIFI_EN 0

#define HAL_COMM_RECE_OVER_TIME (500)	  //3s
#define HAL_COMM_RECE_OVER_TIME_RETRY (3) //超时重发次数
#define HAL_COMM_ERROR_RETRY (2)		  //接收错误重发次数
#define HAL_COMM_SECRET_KEY_SIZE (6)
#define HAL_COMM_TX_BUFF (512)
#define HAL_COMM_RX_BUFF (1024)
#define HAL_COMM_BLE_CHK_START_ADDR (1)
#define HAL_COMM_BLE_HAED_SIZE (1)
#define HAL_COMM_BLE_HEADER (0xFE)

#define HAL_COMM_WIFI_CHK_START_ADDR (1)
#define HAL_COMM_WIFI_HAED_SIZE (1)
#define HAL_COMM_WIFI_HEADER (0xFE)
#define COMM_WAKEUP_BACK_TIME (100) //100唤醒后板时间


/*************************.C变量结构声明***********************/
//typedef enum
//{
//	eCommStatusNone,			//空闲
//	eCommStatusSending,			//数据发送处理中
//	eCommStatusACKNeed,			//需要等待接收
//	eCommStatusAckNot,			//需要不需要响应接收
//	eCommStatusReceive,			//接收数据中
//	eCommStatusReceiveOverTime, //接收超时
//	eCommStatusReceiveError,	//接收错误
//	eCommStatusReceiveSucceed,	//接收成功
//} TYPEe_CommStatus;
typedef enum
{
	eCommAckNeed, //需要回复
	eCommAckNot,  //不要回复
} TYPEe_CommAck;
typedef struct
{
	uint32_t rxTime;				 //接收超时时间
	uint32_t rxTimeCnt;				 //接收超时计数器
	uint8_t retry;					 //重试次数
	uint8_t retryCnt;				 //重试次数计数器
	TYPEe_CommStatus Status;		 //运行状态
	TYPEe_Comm Cmd;					 //记录运行的指令
	TYPEe_Comm ReceCmd;				 //接收的指令
	TYPEe_CommAck ack;				 //需要回复
	TYPEe_CommEncrypt encryptFlag;	 //加密标志
	TYPEe_CommWaitWakeup waitWakeup; //设备唤醒状态
	uint32_t waitTimeCnt;			 //计数器时间	
    uint16_t upBat;                  //上电池电量
    uint16_t downBat;                //下电池电量
} TYPEs_CommCtrl;
#pragma pack(1) //这里强制使用1字节对齐的方式
typedef enum
{
	eBleProtocolTypeNone = 0x00,	  //指令包为空
	eBleProtocolTypeCmd = 0x01,		  // 指令包
	eBleProtocolTypeCmdNotAck = 0x02, // 不应答包
	eBleProtocolTypeAck = 0x09,		  //应答包
} TYPEe_BleProtocolType;
typedef struct
{
	uint8_t header;				//包头
	TYPEe_BleProtocolType type; //包属性
	uint8_t cmd;				//指令
	uint16_t len;				//长度
} TYPEs_BleProtocolFront;
typedef struct
{
	TYPEs_BleProtocolFront front; //协议前部分
	uint8_t *dat;				  //数据包指针
	uint16_t chk;				  //校验和
} TYPEs_BleProtocol;
typedef struct
{
	uint8_t tx[HAL_COMM_TX_BUFF];
	uint8_t rx[HAL_COMM_RX_BUFF];
	TYPEs_BleProtocol Protocol;
	TYPEs_BleProtocol ProtocolRx;
} TYPEs_BleCom;
#if HAL_COMM_WIFI_EN
typedef enum
{
	eWifiProtocolTypeNone = 0x00,	   //指令包为空
	eWifiProtocolTypeCmd = 0x01,	   // 指令包
	eWifiProtocolTypeCmdNotAck = 0x02, // 不应答包
	eWifiProtocolTypeAck = 0x09,	   //应答包
} TYPEe_WifiProtocolType;
typedef struct
{
	uint8_t header;				 //包头
	TYPEe_WifiProtocolType type; //包属性
	uint8_t cmd;				 //指令
	uint16_t len;				 //长度
} TYPEs_WifiProtocolFront;
typedef struct
{
	TYPEs_WifiProtocolFront front; //协议前部分
	uint8_t *dat;				   //数据包指针
	uint16_t chk;				   //校验和
} TYPEs_WifiProtocol;
#endif
#pragma pack() //取消对齐
/*************************私有全局变量*************************/
static uint8_t secretKey[HAL_COMM_SECRET_KEY_SIZE] = {
	0x00,
	0x00,
}; //密钥
TYPEs_CommCtrl commCtrl = {
	.rxTime = 0,
	.rxTimeCnt = 0,
	.retry = 0,
	.retryCnt = 0,
	.Status = eCommStatusNone,
	.Cmd = eCommNone,
	.ReceCmd = eCommNone,
	.ack = eCommAckNot,
	.encryptFlag = eCommEncryptDis,
	.waitWakeup = eCommWaitWakeupSleep,
    
	//	.secretKey ={0x08,0x03,},
};
static TYPEs_BleCom bleCom = {
	.tx = {
		0x00,
	},
	.rx = {
		0x00,
	},
	.Protocol.front.header = HAL_COMM_BLE_HEADER,
	.Protocol.front.type = eBleProtocolTypeNone,
	.Protocol.front.cmd = 0,
	.Protocol.front.len = 0,
	.Protocol.dat = NULL,
	.Protocol.chk = 0,
	.ProtocolRx.front.header = 0,
	.ProtocolRx.front.type = eBleProtocolTypeNone,
	.ProtocolRx.front.cmd = 0,
	.ProtocolRx.front.len = 0,
	.ProtocolRx.dat = NULL,
	.ProtocolRx.chk = 0,
};
static TYPEs_ComSendParm comSendParm = {
	.datType = eCommHeadNodat,
	.mid = 0,
	.secretKeyFlag = eSecretKeyDis,
	.secretKey = NULL,
	.dat = NULL,
	.datLen = 0,
	.cmdType = eCommEncryptDis,
};
#if HAL_COMM_WIFI_EN
TYPEs_WifiProtocol WifiProtocol = {
	.front.header = HAL_COMM_WIFI_HEADER,
	.front.type = eWifiProtocolTypeNone,
	.front.cmd = 0,
	.front.len = 0,
	.dat = NULL,
	.chk = 0,
};
TYPEs_WifiProtocol WifiProtocolRx = {
	.front.header = 0,
	.front.type = eWifiProtocolTypeNone,
	.front.cmd = 0,
	.front.len = 0,
	.dat = NULL,
	.chk = 0,
};
#endif
/*************************公共全局变量*************************/
#define COMM_DATA_OUT (bleCom.tx)
#define COMM_DATA_IN (bleCom.rx)
#define COMM_DATA_BLE_IN_START (bleCom.rx+(sizeof(commCtrl.upBat) + sizeof(commCtrl.downBat)))
#define COMM_DATA_BLE_OUT ((COMM_DATA_OUT + sizeof(TYPEs_BleProtocolFront)))
#define COMM_DATA_BLE_IN ((COMM_DATA_BLE_IN_START +sizeof(TYPEs_BleProtocolFront))) //蓝牙数据带有电量信息
/*
以下数据为只读 指针常量 地址不可修改
*/
TYPEs_CommMsgKeyStatus *const pCommMsgKeyStatus = (TYPEs_CommMsgKeyStatus *)(COMM_DATA_IN);
TYPEs_CommGetBatteryAck *const pCommGetBatteryAck = (TYPEs_CommGetBatteryAck *)(COMM_DATA_IN);
TYPEs_CommAckStatus *const pCommAckStatusSend = (TYPEs_CommAckStatus *)(COMM_DATA_OUT);
TYPEs_CommAckStatus *const pCommAckStatusRcve = (TYPEs_CommAckStatus *)(COMM_DATA_IN);
TYPEs_CommWifiUpload *const pCommWifiUpload = (TYPEs_CommWifiUpload *)(COMM_DATA_OUT);
TYPEs_CommMsgMotorLock *const pCommMsgMotorLock = (TYPEs_CommMsgMotorLock *)(COMM_DATA_OUT);
TYPEs_CommOpenAndInfoUpload *const pCommOpenAndInfoUpload = (TYPEs_CommOpenAndInfoUpload *)(COMM_DATA_OUT);
TYPEs_CommWifiNetSet *const pCommWifiNetSet = (TYPEs_CommWifiNetSet *)(COMM_DATA_OUT);
TYPEs_CommWifiPortSet *const pCommWifiPortSet = (TYPEs_CommWifiPortSet *)(COMM_DATA_OUT);
TYPEs_CommWirelessMACAck *const pCommWirelessMACAck = (TYPEs_CommWirelessMACAck *)(COMM_DATA_IN);
TYPEs_CommScanGateway *const pCommScanGateway = (TYPEs_CommScanGateway *)(COMM_DATA_IN);
TYPEs_CommMsgUpdateDeviceName *const pCommMsgUpdateDeviceName = (TYPEs_CommMsgUpdateDeviceName *)(COMM_DATA_OUT);
TYPEs_CommMsgEnterOTA *const pCommMsgEnterOTA = (TYPEs_CommMsgEnterOTA *)(COMM_DATA_OUT);
TYPEs_CommMsgOpenBackupBat *const pCommMsgOpenBackupBat = (TYPEs_CommMsgOpenBackupBat *)(COMM_DATA_OUT);
TYPEs_CommMsgDisconnectBLE *const pCommMsgDisconnectBLE = (TYPEs_CommMsgDisconnectBLE *)(COMM_DATA_OUT);
/*
以下数据为只读 指针常量 地址不可修改 BLE通讯数据
*/
TYPEs_BleAck *const pBleAck = (TYPEs_BleAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleTimeSync *const pBleTimeSync = (TYPEs_BleTimeSync *)(COMM_DATA_BLE_IN);
TYPEs_BleDelFinger *const pBleDelFinger = (TYPEs_BleDelFinger *)(COMM_DATA_BLE_IN);
TYPEs_BleAddFingerAck *const pBleAddFingerAck = (TYPEs_BleAddFingerAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleAddPhoneAccount *const pBleAddPhoneAccount = (TYPEs_BleAddPhoneAccount *)(COMM_DATA_BLE_IN);
TYPEs_BleDelPhoneAccount *const pBleDelPhoneAccount = (TYPEs_BleDelPhoneAccount *)(COMM_DATA_BLE_IN);
TYPEs_BleFingerAttributeModify *const pBleFingerAttributeModify = (TYPEs_BleFingerAttributeModify *)(COMM_DATA_BLE_IN);
TYPEs_BleEquipmentRegAck *const pBleEquipmentRegAck = (TYPEs_BleEquipmentRegAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleWifiFunctionSwitchSet *const pBleWifiFunctionSwitchSet = (TYPEs_BleWifiFunctionSwitchSet *)(COMM_DATA_BLE_IN);
TYPEs_BleLockFirmwareVersionGetAck *const pBleLockFirmwareVersionGetAck = (TYPEs_BleLockFirmwareVersionGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleAddFingerAck *const pBleNewAddFingerAck = (TYPEs_BleAddFingerAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleQueryLockConfigTable *const pBleQueryLockConfigTable = (TYPEs_BleQueryLockConfigTable *)(COMM_DATA_BLE_IN);
TYPEs_BleQueryLockConfigTableAck *const pBleQueryLockConfigTableAck = (TYPEs_BleQueryLockConfigTableAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleWifiSetParm *const pBleWifiSetParm1 = (TYPEs_BleWifiSetParm *)(COMM_DATA_BLE_IN);
TYPEs_BleWifiSetParm *const pBleWifiSetParm2 = (TYPEs_BleWifiSetParm *)(COMM_DATA_BLE_OUT);
TYPEs_BleWifiSetParmAck *const pBleWifiSetParmAck = (TYPEs_BleWifiSetParmAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleUnlockUploadWifiSW *const pBleUnlockUploadWifiSW = (TYPEs_BleUnlockUploadWifiSW *)(COMM_DATA_BLE_IN);
TYPEs_BleAddFaceAck *const pBleAddFaceAck = (TYPEs_BleAddFaceAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleFaceRegConfirm *const pBleFaceRegConfirm = (TYPEs_BleFaceRegConfirm *)(COMM_DATA_BLE_IN);
TYPEs_BleFaceDelId *const pBleFaceDelId = (TYPEs_BleFaceDelId *)(COMM_DATA_BLE_IN);
TYPEs_BleMotorTorqueSet *const pBleMotorTorqueSet = (TYPEs_BleMotorTorqueSet *)(COMM_DATA_BLE_IN);
TYPEs_BleMotorDirSet *const pBleMotorDirSet = (TYPEs_BleMotorDirSet *)(COMM_DATA_BLE_IN);
TYPEs_BleAutomaticLockIntervalSet *const pBleAutomaticLockIntervalSet = (TYPEs_BleAutomaticLockIntervalSet *)(COMM_DATA_BLE_IN);
TYPEs_BleAKeyProtection *const pBleAKeyProtection = (TYPEs_BleAKeyProtection *)(COMM_DATA_BLE_IN);
TYPEs_BleActiveDefense *const pBleActiveDefense = (TYPEs_BleActiveDefense *)(COMM_DATA_BLE_IN);
TYPEs_BleChannelPwdGetAck *const pBleChannelPwdGetAck = (TYPEs_BleChannelPwdGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleFaceVersionGetAck *const pBleFaceVersionGetAck = (TYPEs_BleFaceVersionGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleGetEventRecords *const pBleGetEventRecords = (TYPEs_BleGetEventRecords *)(COMM_DATA_BLE_OUT);; 
TYPEs_BleFaceAttributeModify *const pBleFaceAttributeModify = (TYPEs_BleFaceAttributeModify *)(COMM_DATA_BLE_IN);
TYPEs_BleUnlockPwdSet *const pBleUnlockPwdSet = (TYPEs_BleUnlockPwdSet *)(COMM_DATA_BLE_IN);
TYPEs_BleEmptyUser *const pBleEmptyUser = (TYPEs_BleEmptyUser *)(COMM_DATA_BLE_IN);
TYPEs_BleLockPwdVerifyAdmin *const pBleLockPwdVerifyAdmin = (TYPEs_BleLockPwdVerifyAdmin *)(COMM_DATA_BLE_IN);
TYPEs_BleLockPwdVerifyAdminACK *const pBleLockPwdVerifyAdminACK = (TYPEs_BleLockPwdVerifyAdminACK *)(COMM_DATA_BLE_OUT);
TYPEs_BlePhoneSecretKeyGetAck *const pBlePhoneSecretKeyGetAck = (TYPEs_BlePhoneSecretKeyGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleSmartKeySecretKeyGetRandom *const pBleSmartKeySecretKeyGetRandom = (TYPEs_BleSmartKeySecretKeyGetRandom *)(COMM_DATA_BLE_OUT);
TYPEs_BleSmartKeySecretKeyGet *const pBleSmartKeySecretKeyGet = (TYPEs_BleSmartKeySecretKeyGet *)(COMM_DATA_BLE_OUT);
TYPEs_BleLockRegSecretKeyGet *const pBleLockRegSecretKeyGet = (TYPEs_BleLockRegSecretKeyGet *)(COMM_DATA_BLE_OUT);
TYPEs_BleLockVolumeSet *const pBleLockVolumeSet = (TYPEs_BleLockVolumeSet *)(COMM_DATA_BLE_IN);
TYPEs_BleGyroscopeSet *const pBleGyroscopeSet = (TYPEs_BleGyroscopeSet *)(COMM_DATA_BLE_IN);
TYPEs_BleID2ServerSwitch *const pBleID2ServerSwitch = (TYPEs_BleID2ServerSwitch *)(COMM_DATA_BLE_IN);
TYPEs_BleDHKeyExchangeAlgorithm *const pBleDHKeyExchangeAlgorithm = (TYPEs_BleDHKeyExchangeAlgorithm *)(COMM_DATA_BLE_IN);
TYPEs_BleDHKeyExchangeAlgorithmAck *const pBleDHKeyExchangeAlgorithmAck = (TYPEs_BleDHKeyExchangeAlgorithmAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleWakeSourceQueryAck *const pBleWakeSourceQueryAck = (TYPEs_BleWakeSourceQueryAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleInfraredMonitoringDistanceSet *const pBleInfraredMonitoringDistanceSet = (TYPEs_BleInfraredMonitoringDistanceSet *)(COMM_DATA_BLE_IN);
TYPEs_BleDoorOpeningModeSet *const pBleDoorOpeningModeSet = (TYPEs_BleDoorOpeningModeSet *)(COMM_DATA_BLE_IN);
TYPEs_BleNFCSwitchSet *const pBleNFCSwitchSet = (TYPEs_BleNFCSwitchSet *)(COMM_DATA_BLE_IN);
TYPEs_BleDoorbellCapture *const pBleDoorbellCapture = (TYPEs_BleDoorbellCapture *)(COMM_DATA_BLE_IN);
TYPEs_BleAutoLockConfig *const pBleAutoLockConfig = (TYPEs_BleAutoLockConfig *)(COMM_DATA_BLE_IN);
TYPEs_BleAutoLockKeyConfig *const pBleAutoLockKeyConfig = (TYPEs_BleAutoLockKeyConfig *)(COMM_DATA_BLE_IN);
TYPEs_BleModifFaceSafetyLevel *const pBleModifFaceSafetyLevel = (TYPEs_BleModifFaceSafetyLevel *)(COMM_DATA_BLE_IN);
TYPEs_BleToolAddModel *const pBleToolAddModel = (TYPEs_BleToolAddModel *)(COMM_DATA_BLE_IN);
TYPEs_BleAddSmartKeyAccredit *const pBleAddSmartKeyAccredit = (TYPEs_BleAddSmartKeyAccredit *)(COMM_DATA_BLE_IN);
TYPEs_BleDelSmartKey *const pBleDelSmartKey = (TYPEs_BleDelSmartKey *)(COMM_DATA_BLE_IN);
TYPEs_BleSmartKeyAttributeModify *const pBleSmartKeyAttributeModify = (TYPEs_BleSmartKeyAttributeModify *)(COMM_DATA_BLE_IN);
TYPEs_BleAESUnlockPwdSet *const pBleAESUnlockPwdSet = (TYPEs_BleAESUnlockPwdSet *)(COMM_DATA_BLE_IN);
TYPEs_BleAESUnlockAlarmPwdSet *const pBleAESUnlockAlarmPwdSet = (TYPEs_BleAESUnlockAlarmPwdSet *)(COMM_DATA_BLE_IN);
TYPEs_BleAESTmpeSecretKeyGetAck *const pBleAESTmpeSecretKeyGetAck = (TYPEs_BleAESTmpeSecretKeyGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleAESAddPhoneAccount *const pBleAESAddPhoneAccount = (TYPEs_BleAESAddPhoneAccount *)(COMM_DATA_BLE_IN);
TYPEs_BleToolTestRWLockModelAck *const pBleToolTestRWLockModelAck = (TYPEs_BleToolTestRWLockModelAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleFlashWriteTest *const pBleFlashWriteTest = (TYPEs_BleFlashWriteTest *)(COMM_DATA_BLE_IN);
TYPEs_BleFlashRWCompAck *const pBleFlashRWCompAck = (TYPEs_BleFlashRWCompAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleVoltageTestAck *const pBleVoltageTestAck = (TYPEs_BleVoltageTestAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleLockTimeGetAck *const pBleLockTimeGetAck = (TYPEs_BleLockTimeGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleSecondBatteryVoltageTestAck *const pBleSecondBatteryVoltageTestAck = (TYPEs_BleSecondBatteryVoltageTestAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleLockMacGet *const pBleLockMacGet = (TYPEs_BleLockMacGet *)(COMM_DATA_BLE_IN);
TYPEs_BleLockMacGetAck *const pBleLockMacGetAck = (TYPEs_BleLockMacGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleTmpeSecretKeyGetAck *const pBleTmpeSecretKeyGetAck = (TYPEs_BleTmpeSecretKeyGetAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleAliyunSecretKeySetParm *const pBleAliyunSecretKeySetParm = (TYPEs_BleAliyunSecretKeySetParm *)(COMM_DATA_BLE_IN);
TYPEs_BleCatEyeSNAck *const pBleCatEyeSNAck = (TYPEs_BleCatEyeSNAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleShangTangCatEyeSingleDouble *const pBleShangTangCatEyeSingleDouble = (TYPEs_BleShangTangCatEyeSingleDouble *)(COMM_DATA_BLE_IN);
TYPEs_BleR8CatEyeOTA *const pBleR8CatEyeOTA= (TYPEs_BleR8CatEyeOTA *)(COMM_DATA_BLE_IN);
TYPEs_BleR8CatEyeOTAAck *const pBleR8CatEyeOTAAck= (TYPEs_BleR8CatEyeOTAAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleVerifyPhoneEncrypt *const pBleVerifyPhoneEncrypt= (TYPEs_BleVerifyPhoneEncrypt *)(COMM_DATA_BLE_IN );
TYPEs_BleVerifyPhoneEncryptAck *const pBleVerifyPhoneEncryptAck= (TYPEs_BleVerifyPhoneEncryptAck *)(COMM_DATA_BLE_OUT);
TYPEs_BleUnlockSmartKeyVerify *const pBleUnlockSmartKeyVerify= (TYPEs_BleUnlockSmartKeyVerify *)(COMM_DATA_BLE_IN );
uint16_t *const pBleRxLen = (uint16_t *)(&bleCom.ProtocolRx.front.len);
uint8_t *const pBleRxData = (uint8_t *)(COMM_DATA_BLE_IN);
uint8_t *const pBleTxData = (uint8_t *)(COMM_DATA_BLE_OUT);
uint8_t *pCommMac = NULL; //指向MAC的指针
uint8_t *pCommRootKeyA = NULL; //指向ROOT的指针
uint8_t *const pRxData = (uint8_t *)(COMM_DATA_IN);

/*
以下数据为只读 指针常量 地址不可修改 wifi通讯数据
*/
TYPEs_WifiSetRouter *const pWifiSetRouter = (TYPEs_WifiSetRouter *)(COMM_DATA_OUT + sizeof(TYPEs_CommWifiUpload));
TYPEs_WifiServerDomain *const pWifiServerDomain = (TYPEs_WifiServerDomain *)(COMM_DATA_OUT + sizeof(TYPEs_CommWifiUpload));

#undef COMM_DATA_OUT
#undef COMM_DATA_IN
#undef COMM_DATA_BLE_OUT 
#undef COMM_DATA_BLE_IN 
/*************************局部函数*****************************/

/***************************************************************************************
**函数名:       HAL_Comm_CmdSend
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
static void HAL_Comm_CmdSend(TYPEs_ComSendParm *p)
{
	DRV_Comm_CmdSend(p);
}
/***************************************************************************************
**函数名:       HAL_Comm_BleDataPack
**功能描述:     蓝牙数据打包
**输入参数:    
uint16_t len  //要发送数据的长度
**输出参数:     
uint16_t
打包好数据的长度
**备注:         由于这里进行过大小端的转换，需要确认长度是否可用
****************************************************************************************/
static uint16_t HAL_Comm_BleDataPack(uint16_t len)
{
	uint16_t lenCnt = 0;
	//	uint16_t chkSum =0;
	bleCom.Protocol.front.len = len; //填写数据长度
	BSWAP_SAVE_16(bleCom.Protocol.front.len);
	memcpy((uint8_t *)(bleCom.tx),
		   (uint8_t *)(&bleCom.Protocol.front),
		   sizeof(bleCom.Protocol.front)); //数据头
	lenCnt = sizeof(bleCom.Protocol.front);
	// memcpy((uint8_t *)(bleCom.tx + lenCnt),
	// 	   dat,
	// 	   len); //添加数据
	lenCnt += len;																										  //获取数据长度
	bleCom.Protocol.chk = DRV_Comm_GetSumChk((bleCom.tx + HAL_COMM_BLE_CHK_START_ADDR), lenCnt - HAL_COMM_BLE_HAED_SIZE); //计算校验和
	BSWAP_SAVE_16(bleCom.Protocol.chk);
	memcpy((uint8_t *)(bleCom.tx + lenCnt),
		   &bleCom.Protocol.chk,
		   sizeof(bleCom.Protocol.chk));   //添加数据
	lenCnt += sizeof(bleCom.Protocol.chk); //接上校验和的长度
	return lenCnt;
}
/***************************************************************************************
**函数名:       HAL_Comm_BleCmdSend
**功能描述:     蓝牙数据发送
**输入参数:    
TYPEe_Ble cmd 蓝牙指令
**输出参数:     
uint16_t
打包好数据的长度
**备注:         
****************************************************************************************/
static uint16_t HAL_Comm_BleCmdSend(TYPEe_Ble cmd)
{
	uint32_t i;
	uint8_t tmp=0;
	bleCom.Protocol.front.header = 0XFE;
	bleCom.Protocol.front.type = eBleProtocolTypeAck;
	switch (cmd)
	{
	case eBleNone:
		break; //(0x00)空闲
	case eBleVerifyChannelPwd:
		break; //(0x01)验证信道密码
	case eBleVerifyPhone:
		break; //(0x03)手机开锁验证
	case eBleUnlockUpload:
		break; //(0x04)开锁实时上传
	case eBleUnlockUploadEncrypt:
		break; //(0x05)开锁实时加密上传wifi
	case eBleUnlockUploadEncryptWithVersion:
		break; //(0x06)整合版本锁具信息实时加密上传wifi
	case eBleAlarmInforUpload:
		break; //(0x07)报警信息上传
	case eBleAlarmInforUploadEncrypt:
		break; //(0x08)加密报警信息上传
	case eBleLockStorageInforGet:
		break; //(0x20)锁存储信息获取
	case eBleUnlockRecordGet:
		break; //(0x21)开锁记录请求查询
	case eBleUnlockModeSetting:
		break; //(0x22)开锁方式设置
	case eBleTimeSync:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x23)时间同步
	case eBleAddFinger:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAddFingerAck);
		switch (pBleAddFingerAck->ack)
		{
		case eBleAddFingerAckSuccess:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFingerSuccess);
			break;
		case eBleAddFingerAckIng:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFingerIng);
		default:
			break;
		}
		break; //(0x24)增加指纹
	case eBleAddPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x25)增加手机账号
	case eBleAddSmartKeyAccredit:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x26)增加智能钥匙授权
	case eBleAddSmartKey:
		break; //(0x27)增加智能钥匙
	case eBleDelFinger:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x28)删除指纹
	case eBleDelPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x29)删除手机账号
	case eBleDelSmartKey:
		 bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2A)删除智能钥匙
	case eBleFingerAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2B)指纹属性修改
	case eBleSmartKeyAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2D)智能钥匙属性修改
	case eBleUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x30)开锁密码设置
	case eBleUnlockPwdSetAdmin:
		break; //(0x31)锁管理密码设置
	case eBleFactory:
		break; //(0x32)恢复出厂设置
	case eBleEquipmentReg:
		bleCom.Protocol.front.type = eBleProtocolTypeAck;
		bleCom.Protocol.front.len = sizeof(TYPEs_BleEquipmentRegAck);
		break; //(0x34)设备注册
	case eBleEmptyUser:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x35)清空用户
	case eBleLockPwdVerifyAdmin:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x36)锁管理密码验证
	case eBleLockFirmwareVersionGet:
		bleCom.Protocol.front.type = eBleProtocolTypeAck;
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockFirmwareVersionGetAck);
		break; //(0x37)锁固件版本获取
	case eBleAlarmPwdSet:
		break; //(0x38)报警密码设置
	case eBleVerifyPhoneEncrypt:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAck);
		switch (pBleVerifyPhoneEncryptAck->ack)
		{
		case eBleAckSuccess:
			bleCom.Protocol.front.len += sizeof(pBleVerifyPhoneEncryptAck->bat);
			break;
		default:
			break;
		}
		break; //(0x39)手机开锁验证加密版
	case eBlePhoneSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BlePhoneSecretKeyGetAck);
		break; //(0x3A)手机秘钥获取
	case eBleUnlockSmartKeyVerify:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAck);
		break; //(0x3B)智能钥匙开锁验证
	case eBleSmartKeySecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSmartKeySecretKeyGet);
		break; //(0X3C)智能钥匙开锁秘钥获取
	case eBleSmartKeySecretKeyGetRandom:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSmartKeySecretKeyGetRandom);
		break; //(0X3C)智能钥匙授权获取随机数秘钥
	case eBleLockRegSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockRegSecretKeyGet);
		break; //(0x3E)锁注册秘钥获取
	case eBleTmpeSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleTmpeSecretKeyGetAck);
		break; //(0X3F)临时秘钥生成
	case eBleTestServerConnectionStatus:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x40)测试服务器连接状态
	case eBleWifiFunctionSwitchSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x41)WIFI功能开启关闭设置
	case eBleDisconnectBluetooth:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xF0)断开蓝牙连接
	case eBleWifiSetStart:
		break; //(0xF2)WIFI设置启动
	case eBleWifiSetEnd:
		break; //(0xf3)WIFI设置结束
	case eBleLockUpdateProgramMode:
		break; //(0xF4)LOCK更新程序模式
	case eBleWifiSetParm1:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWifiSetParmAck);
		break; //(0xf5)WIFI设置参数1
	case eBleWifiSetParm2:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWifiSetParm);
		break; //(0xf6)WIFI设置参数2
	case eBleWifiSetDomainNameParm1:
		break; //(0xf7)WIFI设置域名参数1
	case eBleWifiSetDomainNameParm2:
		break; //(0xf8)WIFI设置域名参数2
	case eBleUnlockUploadWifiSW:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xF9)开锁记录实时上传功能设置(默认功能是关闭)
	case eBlePhoneBleSecretKeyGet:
		break; //(0x50)手机操作BLE秘钥获取指令
	case eBleBleKeyHaveLockChk:
		break; //(0x51)查询BLE钥匙下已有锁(注：容量最大为20)
	case eBlePhoneDelBleKeyAllAccount:
		break; //(0x52)手机删除BLE钥匙上全部账号
	case eBleNewAddFinger:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAddFingerAck);
		switch (pBleAddFingerAck->ack)
		{
		case eBleAddFingerAckSuccess:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFingerSuccess);
			break;
		case eBleAddFingerAckIng:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFingerIng);
		default:
			break;
		}
		break; //(0x53)新APP协议增加指纹
	case eBleNewConfirmFingerRegSucced:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x54)新APP协议确认指纹录入成功
	case eBleLockAddSmartKey:
		break; //(0x55)门禁增加智能钥匙
	case eBleLockDelSmartKey:
		break; //(0x56)门禁删除智能钥匙
	case eBleLocalFingerSync:
		break; //(0x57)本地指纹同步
	case eBleLocalFingerDel:
		break; //(0x58)本地指纹删除
	case eBleLocalFingerAttributeModif:
		break; //(0x59)本地指纹属性修改
	case eBleEncryptChipSEIDGet:
		break; //(0x60)加密芯片SEID获取
	case eBleEncryptChipChallengeGet:
		break; //(0x61)加密芯片challenge获取
	case eBleEncryptChipVerifyMac:
		break; //(0x62)加密芯片verifymac请求
	case eBleServerTokenVerify:
		break; //(0x65)服务器token验证请求
	case eBleServerIssueChallenge:
		break; //(0x66)服务器下发challenge
	case eBleID2ServerSwitch:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x67)启用/禁用ID2服务
	case eBleDHKeyExchangeAlgorithm:
		break; //(0x70)DH密钥交换算法
	case eBleLockVolumeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x71)锁具音量设置
	case eBleGyroscopeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x72)陀螺仪设置
	case eBleWakeSourceQuery:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWakeSourceQueryAck);
		break; //(0x73)唤醒源查询(调试用APP不需要增加)
	case eBleDESUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x74)DES开锁密码设置
	case eBleDESUnlockAlarmPwdSet:
		break; //(0x75)DES报警密码设置
	case eBleDESTmpeSecretKeyGet:
		break; //(0x76)DES临时秘钥生成
	case eBleDESAddPhoneAccount:
		break; //(0x77)DES增加手机账号
	case eBleDESEncrytPhoneUnlockVerify:
		break; //(0x78)DES手机开锁验证加密版
	case eBleAESUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x79)AES开锁密码设置
	case eBleAESUnlockAlarmPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x7A)AES报警密码设置
	case eBleAESAddPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x7C)AES增加手机账号
	case eBleAESEncrytPhoneUnlockVerify:
		break; //(0x7D)AES手机开锁验证加密版
	case eBleBaiduSEEncryptActivationRequest:
		break; //(0x7E)百度SE加密激活请求
	case eBleBaiduSEUnlockPwdSet:
		break; //(0x7F)BaiduSE开锁密码设置
	case eBleBaiduSEUnlockAlarmPwdSet:
		break; //(0x80)BaiduSE报警密码设置
	case eBleBaiduSETmpeSecretKeyGet:
		break; //(0x81)BaiduSE临时秘钥生成
	case eBleBaiduSEAddPhoneAccount:
		break; //(0x82)BaiduSE增加手机账号
	case eBleBaiduSEEncrytPhoneUnlockVerify:
		break; //(0x83)BaiduSE手机开锁验证加密版
	case eBleFaceReg:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAddFaceAck);
		switch (pBleAddFaceAck->ack)
		{
		case eBleAddFaceAckSuccess:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFaceSuccess);
			break;
		case eBleAddFaceAckIng:
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFaceIng);
		default:
			break;
		}
		break; //(0X85)APP人脸录入
	case eBleFaceRegConfirm:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x86)APP确认人脸录入成功
	case eBleFaceDelId:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x87)APP删除人脸ID
	case eBleInfraredMonitoringDistanceSet:
		break; //(0x88)红外监测距离设置
	case eBleDoorOpeningModeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x89)开门方式设置
	case eBlePhoneDelBleKey:
		break; //(0x90)手机删除BLE钥匙
	case eBleMotorTorqueSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x91)电机扭力调节
	case eBleMotorDirSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x92)电机方向设置
	case eBleAutomaticLockIntervalSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x93)自动关门上锁时间间隔设置
	case eBleQueryLockConfigTable:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleQueryLockConfigTableAck);
		break; //(0x94)查询锁具配置表
	case eBleAKeyProtection:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x95)一键布防
	case eBleActiveDefense:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x96)主动防御
	case eBleChannelPwdGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleChannelPwdGetAck);
		break; //(0x97)获取信道密码
	case eBleNFCSwitchSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x98)NFC开关设置
	case eBleDoorbellCapture:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x99)门铃抓拍
	case eBleFaceVersionGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleFaceVersionGetAck);
		break; //(0x9A)人脸模块版本号获取
	case eBleGetEventRecords:
		if(0 ==pBleGetEventRecords->num  ) //事件记录为空
		{
			bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		}
		else
		{
			bleCom.Protocol.front.len = sizeof(TYPEs_BleGetEventRecords);
		}
		break; //(0x9B)获取事件记录
	case eBlefaceEnterFirmwareUpdateMode:
		break; //(0x9C)人脸进入固件更新模式
	case eBleFaceAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0X9D)人脸属性修改
	case eBleBleSinglePackageMaxlength:
		break; //(0x9E)获取蓝牙单包可以发送的最大长度
	case eBleDHKeyExchangeAlgorithm1:
		break; //(0x9F)DH密钥交换算法
	case eBleCatEyeSN:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleCatEyeSNAck);
		break; //(0xA3)获取猫眼序列号
	case eBleShangTangCatEyeSingleDouble:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xA4)商汤猫眼单双向切换功能
	case eBleAddICCard:
		break; //(0xA5)app增加IC卡请求
	case eBleDelICCard:
		break; //(0xA6)app删除IC卡请求
	case eBleAutoLockConfig:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xA7)全自动锁体配置
	case eBleAutoLockKeyConfig:
        bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
        break; //(0xA8)全自动锁门确认键配置
	case eBleToolAddModel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0XD5)蓝牙工装添加锁具型号
	case eBleToolTestRWLockModel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleToolTestRWLockModelAck);
		break; //(0xD6)蓝牙工装测试读出与写入锁具型号
	case eBleToolTestFail:
		break; //(0XD9)工装测试不合格
	case eBleFlashWriteTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xDB)FLASH写入测试
	case eBleFlashRWComp:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleFlashRWCompAck);
		break; //(0xDC)FLASH读出与写入对比
	case eBleLedOledTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xDD)LED/OLED测试
	case eBleVoltageTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleVoltageTestAck);
		break; //(0xDE)电压测试
	case eBleLockTimeGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockTimeGetAck);
		break; //(0xDF)读取锁具时间
	case eBleLockAgingRecord:
		break; //(0xB0)查询锁具老化记录
	case eBleCatEyeTestParm:
		break; //(0xE0)设置猫眼产测参数
	case eBleWifiToolTest:
		break; //(0xD2)WIFI工装检测
	case eBleXiongmaiCatEyeIPGet:
		break; //(0xE1)雄迈猫眼IP查询
	case eBleSecondBatteryVoltageTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSecondBatteryVoltageTestAck);
		break; //(0xE2)下方电池（第二路）电压测试
	case eBleLockMacGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockMacGetAck);
		break; //(0xE3)锁MAC查询
	case eBleCatEyePIRToolTest:
		break; //(0xE4)猫眼PIR工装检测
	case eBleAliyunSecretKeySetParm:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xE5)阿里云秘钥设置参数
	case eBleR8CatEyeOTA:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAddFingerAck);
		switch (pBleR8CatEyeOTAAck->ack)
		{
		case eFaceOTAAckFail:
		case eFaceOTAAckUpdataing:
		case eFaceOTAAckTimeOver:		
			bleCom.Protocol.front.len += sizeof(TYPEs_BleAddFingerSuccess);
			break;
		default:
			break;
		}
		break; //(0XE7)R8猫眼OTA
	case eBleR8CatEyeDebugModeEn:
		break; //(0xE8)R8猫眼DEBUG模式使能
	case eBleR8CatEyeDebugDataTransfer:
		break; //(0xE9)R8猫眼DEBUG数据传输
	case eBleR8EventLog:
		break; //(0XEA)R8WIFI事件记录
	case eBleWifiTestDR:
		break; //(0xEB)WIFI产测扫描指定路由器
	case eBleToolTestIC:
		break; //(0xEC)厂测工装检测IC卡
	case eBleModifFaceSafetyLevel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xED)修改人脸识别安全等级
	default:
		break;
	}
	return HAL_Comm_BleDataPack(bleCom.Protocol.front.len); //打包数据
}
#if HAL_COMM_WIFI_EN
/***************************************************************************************
**函数名:       HAL_Comm_WifiDataPack
**功能描述:     发送数据打包
**输入参数:    
uint32_t addr 打包时的偏移地址
uint16_t len  //要发送数据的长度
**输出参数:     
uint16_t
打包好数据的长度(包括偏移长度)
**备注:         由于这里进行过大小端的转换，需要确认长度是否可用
****************************************************************************************/
static uint16_t HAL_Comm_WifiDataPack(uint16_t addrOffset, uint16_t len)
{
	uint16_t lenCnt = 0;
	//	uint16_t chkSum =0;
	WifiProtocol.front.len = len; //填写数据长度
	BSWAP_SAVE_16(WifiProtocol.front.len);
	memcpy((uint8_t *)(bleCom.tx + addrOffset),
		   (uint8_t *)(&WifiProtocol.front),
		   sizeof(WifiProtocol.front)); //数据头
	lenCnt = sizeof(WifiProtocol.front);
	// memcpy((uint8_t *)(bleCom.tx + lenCnt),
	// 	   dat,
	// 	   len); //添加数据
	lenCnt += len;																													  //获取数据长度
	WifiProtocol.chk = DRV_Comm_GetSumChk((bleCom.tx + addrOffset + HAL_COMM_WIFI_CHK_START_ADDR), lenCnt - HAL_COMM_WIFI_HAED_SIZE); //计算校验和
	BSWAP_SAVE_16(WifiProtocol.chk);
	memcpy((uint8_t *)(bleCom.tx + addrOffset + lenCnt),
		   &WifiProtocol.chk,
		   sizeof(WifiProtocol.chk));	//添加数据
	lenCnt += sizeof(WifiProtocol.chk); //接上校验和的长度
	lenCnt += addrOffset;				//拼接所有的数据
	return lenCnt;
}

/***************************************************************************************
**函数名:       HAL_CommWifiCmdSend
**功能描述:     wifi数据发送
**输入参数:    
TYPEe_Wifi cmd wifi指令
**输出参数:     
uint16_t
打包好数据的长度
**备注:         
****************************************************************************************/
static uint16_t HAL_CommWifiCmdSend(TYPEe_Wifi cmd)
{
	uint16_t addrOffset = 0;
	WifiProtocol.front.header = 0xFE;
	WifiProtocol.front.type = eWifiProtocolTypeAck;
	switch (cmd)
	{
	case eWifiSetRouter:
		addrOffset = sizeof(TYPEs_CommWifiUpload);
		WifiProtocol.front.len = sizeof(TYPEs_WifiSetRouter);
		break; //0xF6)设置路由器
	case eWifiServerDomain:
		addrOffset = sizeof(TYPEs_CommWifiUpload);
		WifiProtocol.front.len = sizeof(TYPEs_WifiServerDomain);
		break; //0xF7)设置服务器域名
	case eWifiOpenMsgUpload:
		break; //0x04)开锁信息上传
	case eWifiFaultMsgUpload:
		break; //0x07)故障信息上传
	case eWifiImageUpload:
		break; //0x09)图片上传
	case eWifiOTAUpdate:
		break; //0x10)OTA更新
	case eWifiChkOTA:
		break; //0x11)模组自主查询OTA更新
	case eWifiSetBPS:
		break; //0xFA)通讯波特率设置
	case eWifiChkMAC:
		break; //0xFB)查询WIFI模块MAC地址
	case eWifiProductScanRouter:
		break; //0xFC)产测扫描路由器
	case eWifiChkSoft:
		break; //0xFD)产测扫描路由器
	case eWifiChkPskPwd:
		break; //0xFE)查询当前模块内PSK密码
	default:
		break;
	}
	return HAL_Comm_WifiDataPack(addrOffset, WifiProtocol.front.len);
}
#endif

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_Comm_Init
**功能描述:     初始化通讯
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_Init(void)
{
	DRV_Comm_Init();
	memset((uint8_t *)&commCtrl, 0, sizeof(TYPEs_CommCtrl));
	HAL_Comm_WakeupBack();
//    #ifdef USE_COMM_ENCRYPT
//	commCtrl.encryptFlag = eCommEncryptEn; //加密
//    #else
//	commCtrl.encryptFlag = eCommEncryptDis; //非加密
//    #endif
}
/***************************************************************************************
**函数名:       HAL_Comm_BleSendStart
**功能描述:     蓝牙开始传输数据，事故踢不过状态机的方式
**输入参数:     
TYPEe_Ble cmd, 蓝牙指令
uint32_t sendWait 发送等待时间

**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_BleSendStart(TYPEe_Ble cmd,uint32_t sendWait)
{
	bleCom.Protocol.front.cmd = cmd;
	HAL_Comm_SendDealStart(eCommBleDataPass, sendWait); //发送蓝牙数据
	
}
/***************************************************************************************
**函数名:       HAL_Comm_BleSendDir
**功能描述:     蓝牙传输数据直接发送不通过状态机
**输入参数:     
TYPEe_Ble cmd, 蓝牙指令
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_BleSendDir(TYPEe_Ble cmd)
{
	bleCom.Protocol.front.cmd = cmd;
	HAL_Comm_Send(eCommBleDataPass);
}
/***************************************************************************************
**函数名:       HAL_Comm_BleSendDirWithLen
**功能描述:     蓝牙传输数据直接发送不通过状态机,带有数据长度
**输入参数:     
TYPEe_Ble cmd, 蓝牙指令

**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_BleSendDirWithLen(TYPEe_Ble cmd,uint16_t len)
{
	bleCom.Protocol.front.cmd = cmd;
	bleCom.Protocol.front.len = len;
	HAL_Comm_Send(eCommBleDataPass);
}
/***************************************************************************************
**函数名:       HAL_Comm_BleGetRecvMsgType
**功能描述:     蓝牙获取消息类型
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
TYPEe_Ble HAL_Comm_BleGetRecvMsgType(void)
{
	memcpy((uint8_t *)(&bleCom.ProtocolRx.front.header),
		   (uint8_t *)COMM_DATA_BLE_IN_START,
		   sizeof(TYPEs_BleProtocolFront));
	return bleCom.ProtocolRx.front.cmd;
}

/***************************************************************************************
**函数名:       HAL_Comm_Init
**功能描述:     初始化通讯
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
//uint8_t asekeyTest[6]={1,2,3,4,5,6};
/***************************************************************************************
**函数名:       HAL_Comm_Send
**功能描述:     数据发送
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_Send(TYPEe_Comm cmd)
{
	//	TYPEs_ComSendParm comSendParm;
	switch (cmd)
	{
	case eCommMsgKeyStatus:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        if(eCommEncryptEn == commCtrl.encryptFlag )
        {
            comSendParm.secretKeyFlag = eSecretKeyEn;
        }
        else
        {
            comSendParm.secretKeyFlag = eSecretKeyDis;
        }
        commCtrl.encryptFlag = eCommEncryptEn; //使用完毕后自动变成加密状态
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommAckStatus);
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNot;
		break;

	case eCommMsgMotorLock:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommMsgMotorLock);
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x20)      通知后板电机上锁
	case eCommOpenAndInfoUpload:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommOpenAndInfoUpload);
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x21) 开门及信息上传
	case eCommWifiUpload:
		#if HAL_COMM_WIFI_EN
		#endif
		break; //(0x23) 透传WIFI信息
	case eCommGetBattery:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = 0;
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x28)前板主动获取电池电量
	case eCommMsgDisconnectBLE:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = 0;
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x29)前板通知断开BLE
	case eCommMsgUpdateDeviceName:
		DEBUG_LOG_OUT("HAL_Comm_Send eCommMsgUpdateDeviceName\n");
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommMsgUpdateDeviceName)+pCommMsgUpdateDeviceName->nameLen;
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x2A)前板通知后板更新设备名称
	case eCommMsgOpenBackupBat:
		DEBUG_LOG_OUT("HAL_Comm_Send eCommMsgOpenBackupBat\n");
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommMsgOpenBackupBat);;
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x2B)前板通知后板打开备用电池供电
	case eCommMsgEnterOTA:
		DEBUG_LOG_OUT("HAL_Comm_Send eCommMsgEnterOTA\n");
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = sizeof(TYPEs_CommMsgEnterOTA)+pCommMsgEnterOTA->nameLen;
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNeed;
		break; //(0x2A)前板通知后板更新设备名称
	case eCommBleDataPass:
		memset((uint8_t *)(&comSendParm), 0, sizeof(TYPEs_ComSendParm));
		comSendParm.secretKey = pCommRootKeyA;
		comSendParm.datType = eCommHeadNodat;
		comSendParm.mid = (TYPEe_Comm)(cmd);
		#ifdef USE_COMM_ENCRYPT
        comSendParm.secretKeyFlag = eSecretKeyEn; 
        #else
        comSendParm.secretKeyFlag = eSecretKeyDis;
        #endif
		comSendParm.dat = (uint8_t *)&bleCom.tx;
		comSendParm.datLen = HAL_Comm_BleCmdSend(bleCom.Protocol.front.cmd);
		HAL_Comm_CmdSend(&comSendParm);
		commCtrl.ack = eCommAckNot;
		break; //(0x30)       后面板BLE数据透传
	case eCommZigbeeExitNet:
		break; //(0xF0)     ZIGBEE模块退网
	case eCommZigbeeEnterNet:
		break; //(0xF1)    ZIGBEE模块入网
	case eCommWirelessMAC:
		break; //(0xFB)       查询无线模块MAC地址
	case eCommScanGateway:
		break; //(0XFC)       产测扫描路由器/无线模块网关
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       HAL_Comm_SendDealStart
**功能描述:     发送开始指令
**输入参数:     TYPEe_Comm cmd 需要发送的命令
**输出参数:  
int8_t
0; //成功
-1; //忙碌
**备注:         
****************************************************************************************/
int8_t HAL_Comm_SendDealStart(TYPEe_Comm cmd, uint32_t sendWait)
{
	if ((eCommStatusNone == commCtrl.Status) || (eCommStatusReceiveSucceed == commCtrl.Status))
	{
		commCtrl.Cmd = cmd;
		commCtrl.retryCnt = 0;				  //清空重复次数
		commCtrl.Status = eCommStatusSending; //进入发送
		commCtrl.waitTimeCnt = sendWait;	  //发送等待
        DEBUG_LOG_OUT("comm ok\n");
		return 0;							  //成功
	}
	else
	{
        DEBUG_LOG_OUT("comm busy\n");
		return -1; //忙碌
	}
}
/***************************************************************************************
**函数名:       HAL_Comm_SendDeal
**功能描述:     发送处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
TYPEe_CommStatus HAL_Comm_SendDeal(void)
{
	switch (commCtrl.Status)
	{
	case eCommStatusNone:
		break; //空闲
	case eCommStatusSending:
		if (0 == commCtrl.waitTimeCnt)
		{
			HAL_Comm_Send(commCtrl.Cmd);
			if (eCommAckNeed == commCtrl.ack)
			{
				commCtrl.Status = eCommStatusReceive;		  //进入数据等待
				commCtrl.rxTimeCnt = HAL_COMM_RECE_OVER_TIME; //回填超时时间
				commCtrl.ReceCmd = eCommNone;				  //清除接收命令字
			}
			else
			{
				commCtrl.Status = eCommStatusNone; //发完直接结束
			}
		}
		else
		{
			commCtrl.waitTimeCnt--;
		}
		break; //数据发送处理中
	case eCommStatusWaitWakeup:
		commCtrl.waitTimeCnt--;
		if (0 == commCtrl.waitTimeCnt)
		{
			commCtrl.waitWakeup = eCommWaitWakeupWork;
			commCtrl.Status = eCommStatusSending;
		}
		break; //需要等待接收
	case eCommStatusACKNeed:
		break; //需要等待接收
	case eCommStatusAckNot:
		break; //需要不需要响应接收
	case eCommStatusReceive:
		if (0 == commCtrl.rxTimeCnt) //计时器
		{
			commCtrl.retryCnt++;
			if (commCtrl.retryCnt >= HAL_COMM_RECE_OVER_TIME_RETRY) //超过了最大重试次数
			{
				commCtrl.Status = eCommStatusReceiveOverTime; //接收超时
			}
			else
			{
				commCtrl.Status = eCommStatusSending; //重发一次
			}
		}
		else
		{
			commCtrl.rxTimeCnt--;
		}
		if (commCtrl.Cmd == commCtrl.ReceCmd) //获取到有效的命令
		{
			//需要检查回复的是不是正确的
			commCtrl.Status = eCommStatusReceiveSucceed;
		}
		break; //接收数据中
	case eCommStatusReceiveOverTime:
		break; //接收超时
	case eCommStatusReceiveError:
		break; //接收错
	case eCommStatusReceiveSucceed:
		//接收成功后的处理
		//		commCtrl.Status = eCommStatusNone; //进入空闲
		break; //接收成功
	default:
		break;
	}
	return commCtrl.Status;
}
/***************************************************************************************
**函数名:       HAL_Comm_GetRecvReply
**功能描述:     解析reply类型的信息
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_GetRecvReply(void)
{
	DRV_COMM_GetRecvContent((uint8_t *)(bleCom.rx), DRV_COMM_GetRecvContentSize());
}
/***************************************************************************************
**函数名:       HAL_Comm_RevcDeal
**功能描述:     接收处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_RevcDeal(void)
{
    
	switch (DRV_Comm_RecvDataUnblock())
	{
	case 1: //接收完成,未来考虑循环读取
		if (0 == DRV_Comm_RecvDataAndAnalysis(commCtrl.encryptFlag))
		{
			commCtrl.ReceCmd = (TYPEe_Comm)(DRV_COMM_GetRecvMsgType()); //获取消息类型
			HAL_Comm_GetRecvReply();									//获取消息内容
		}
		DRV_Comm_ClearRxPar(); //清除接收
		break;
	case -1:				   //参数错误
	case -2:				   //没有数据
	case -3:				   //接收参数错误
		DRV_Comm_ClearRxPar(); //清除接收
		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       HAL_Comm_GetReadAddr
**功能描述:     获取读取地址
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t *HAL_Comm_GetReadAddr(void)
{
	return bleCom.rx; //获取读取地址
}
/***************************************************************************************
**函数名:       HAL_Comm_GetRecvMsgType
**功能描述:     获取消息类型
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
TYPEe_Comm HAL_Comm_GetRecvMsgType(void)
{
	return commCtrl.ReceCmd; //获取读取地址
}
/***************************************************************************************
**函数名:       HAL_Comm_CealrRecv
**功能描述:     清楚接收
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_CealrRecv(void)
{
	commCtrl.ReceCmd = eCommNone;
	memset((uint8_t *)(bleCom.rx), 0, HAL_COMM_RX_BUFF);
}

/***************************************************************************************
**函数名:       HAL_Comm_CealrSend
**功能描述:     清除发送
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_CealrSend(void)
{
	commCtrl.rxTime = 0;
	commCtrl.rxTimeCnt = 0;
	commCtrl.retry = 0;
	commCtrl.retryCnt = 0;
	commCtrl.Status = eCommStatusNone;
	commCtrl.Cmd = eCommNone;
	commCtrl.ReceCmd = eCommNone;
	memset((uint8_t *)(bleCom.tx), 0, HAL_COMM_TX_BUFF);
}
/***************************************************************************************
**函数名:       HAL_COMM_EnterSleepConfig
**功能描述:     进入休眠IO配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_COMM_EnterSleepConfig(void)
{
	DRV_COMM_EnterSleepConfig();
}
/***************************************************************************************
**函数名:       设置密钥
**功能描述:     清除发送
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_AesKeySet(uint8_t *p)
{
	memcpy(secretKey, p, HAL_COMM_SECRET_KEY_SIZE);
}
/***************************************************************************************
**函数名:       HAL_Comm_WakeupBack
**功能描述:     唤醒后板
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_WakeupBack(void)
{
	DRV_Comm_WakeupBack();
}
#if HAL_COMM_WIFI_EN
/***************************************************************************************
**函数名:       HAL_Comm_WifiGetRecvMsgType
**功能描述:     wifi获取消息类型
**输入参数:     
uint16_t addrOffset 地址偏移
**输出参数:     
TYPEe_Wifi
wifi的指令
**备注:         
****************************************************************************************/
TYPEe_Wifi HAL_Comm_WifiGetRecvMsgType(uint16_t addrOffset)
{
	memcpy((uint8_t *)(&WifiProtocolRx.front.header),
		   (uint8_t *)(bleCom.rx + addrOffset),
		   sizeof(TYPEs_WifiProtocolFront));
	return bleCom.ProtocolRx.front.cmd;
}
#endif
/***************************************************************************************
**函数名:       HAL_Comm_ChkBusy
**功能描述:     查忙
**输入参数:     
**输出参数:  
TYPEe_CommBusy
**备注:         
****************************************************************************************/
TYPEe_CommBusy HAL_Comm_ChkBusy(void)
{
	if ((eCommStatusNone == commCtrl.Status) || (eCommStatusReceiveSucceed == commCtrl.Status))
	{
		return eCommBusyNo;
	}
	else
	{
		return eCommBusyYes;
	}
}

/***************************************************************************************
**函数名:       HAL_Comm_RstBack
**功能描述:     复位后板
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_RstBack(void)
{
	DRV_Comm_RstBack();
}
/***************************************************************************************
**函数名:       HAL_Comm_InitBack
**功能描述:     初始化后板
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_InitBack(void)
{
	#if BACK_LOW_RST >0
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_0();
	#else
	M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_1();
	#endif
    
}
/***************************************************************************************
**函数名:       DRV_Comm_WifiRtSet
**功能描述:     设置wifi的RT
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_WifiRtSet(uint8_t sw)
{
    #if 0
    M_WIFI_RT_OUT_PP();
    if(0 == sw)
    {
        M_WIFI_RT_OUT_0();
    }
    else
    {
        M_WIFI_RT_OUT_1();
    }
    #endif
}

/***************************************************************************************
**函数名:       HAL_Comm_WifiCtRead
**功能描述:     读取wifiCt的数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t HAL_Comm_WifiCtRead(void)
{
//    M_WIFI_CT_IN_NOPULL();
//    if(0 == M_WIFI_CT_READIN())
//    {
//        return 0;
//    }
//    else
    {
        return 1;
    }
}
/***************************************************************************************
**函数名:       HAL_Comm_WifiCtInit
**功能描述:     读取wifiCt的数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t HAL_Comm_WifiCtInit(void)
{
//    M_WIFI_CT_IN_NOPULL();
}

/***************************************************************************************
**函数名:       HAL_Comm_WifiSend
**功能描述:     wifi数据发送
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
int8_t HAL_Comm_WifiSend(uint8_t *sendBuf, uint32_t len)
{
    return DRV_Comm_WifiSend(sendBuf,len);
}
/***************************************************************************************
**函数名:       HAL_Comm_EncryptionParam
**功能描述:     加密需要的参数获取
**输入参数:     --
**输出参数:     
int8_t
0; //成功
**备注:         
****************************************************************************************/
int8_t HAL_Comm_EncryptionParam(uint8_t *mac, uint8_t *rootkey)
{
    pCommMac = mac;
    pDrvCommMac = mac;
    pCommRootKeyA = rootkey;
    pDrvCommRootKeyA = rootkey;
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Comm_SetEncrypt
**功能描述:     加密标志位
**输入参数:     uint8_t sw
0 加密
!0 非加密
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Comm_SetEncrypt(uint8_t sw)
{
    if(0 == sw)
    {
        commCtrl.encryptFlag = eCommEncryptEn; //加密
    }
    else
    {
        commCtrl.encryptFlag = eCommEncryptDis; //非加密
    }
}
#endif /*HAL_COMM_MODULE_EN*/
/************************************Code End******************************************/
