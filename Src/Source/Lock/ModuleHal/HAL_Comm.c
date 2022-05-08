/***************************************************************************************
**�ļ���:     HAL_Comm.c
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
#include "HAL_Comm.h"
#include "DRV_Comm.h"
#if HAL_COMM_MODULE_EN
#warning HAL_COMMģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_COMM_WIFI_EN 0

#define HAL_COMM_RECE_OVER_TIME (500)	  //3s
#define HAL_COMM_RECE_OVER_TIME_RETRY (3) //��ʱ�ط�����
#define HAL_COMM_ERROR_RETRY (2)		  //���մ����ط�����
#define HAL_COMM_SECRET_KEY_SIZE (6)
#define HAL_COMM_TX_BUFF (512)
#define HAL_COMM_RX_BUFF (1024)
#define HAL_COMM_BLE_CHK_START_ADDR (1)
#define HAL_COMM_BLE_HAED_SIZE (1)
#define HAL_COMM_BLE_HEADER (0xFE)

#define HAL_COMM_WIFI_CHK_START_ADDR (1)
#define HAL_COMM_WIFI_HAED_SIZE (1)
#define HAL_COMM_WIFI_HEADER (0xFE)
#define COMM_WAKEUP_BACK_TIME (100) //100���Ѻ��ʱ��


/*************************.C�����ṹ����***********************/
//typedef enum
//{
//	eCommStatusNone,			//����
//	eCommStatusSending,			//���ݷ��ʹ�����
//	eCommStatusACKNeed,			//��Ҫ�ȴ�����
//	eCommStatusAckNot,			//��Ҫ����Ҫ��Ӧ����
//	eCommStatusReceive,			//����������
//	eCommStatusReceiveOverTime, //���ճ�ʱ
//	eCommStatusReceiveError,	//���մ���
//	eCommStatusReceiveSucceed,	//���ճɹ�
//} TYPEe_CommStatus;
typedef enum
{
	eCommAckNeed, //��Ҫ�ظ�
	eCommAckNot,  //��Ҫ�ظ�
} TYPEe_CommAck;
typedef struct
{
	uint32_t rxTime;				 //���ճ�ʱʱ��
	uint32_t rxTimeCnt;				 //���ճ�ʱ������
	uint8_t retry;					 //���Դ���
	uint8_t retryCnt;				 //���Դ���������
	TYPEe_CommStatus Status;		 //����״̬
	TYPEe_Comm Cmd;					 //��¼���е�ָ��
	TYPEe_Comm ReceCmd;				 //���յ�ָ��
	TYPEe_CommAck ack;				 //��Ҫ�ظ�
	TYPEe_CommEncrypt encryptFlag;	 //���ܱ�־
	TYPEe_CommWaitWakeup waitWakeup; //�豸����״̬
	uint32_t waitTimeCnt;			 //������ʱ��	
    uint16_t upBat;                  //�ϵ�ص���
    uint16_t downBat;                //�µ�ص���
} TYPEs_CommCtrl;
#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
typedef enum
{
	eBleProtocolTypeNone = 0x00,	  //ָ���Ϊ��
	eBleProtocolTypeCmd = 0x01,		  // ָ���
	eBleProtocolTypeCmdNotAck = 0x02, // ��Ӧ���
	eBleProtocolTypeAck = 0x09,		  //Ӧ���
} TYPEe_BleProtocolType;
typedef struct
{
	uint8_t header;				//��ͷ
	TYPEe_BleProtocolType type; //������
	uint8_t cmd;				//ָ��
	uint16_t len;				//����
} TYPEs_BleProtocolFront;
typedef struct
{
	TYPEs_BleProtocolFront front; //Э��ǰ����
	uint8_t *dat;				  //���ݰ�ָ��
	uint16_t chk;				  //У���
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
	eWifiProtocolTypeNone = 0x00,	   //ָ���Ϊ��
	eWifiProtocolTypeCmd = 0x01,	   // ָ���
	eWifiProtocolTypeCmdNotAck = 0x02, // ��Ӧ���
	eWifiProtocolTypeAck = 0x09,	   //Ӧ���
} TYPEe_WifiProtocolType;
typedef struct
{
	uint8_t header;				 //��ͷ
	TYPEe_WifiProtocolType type; //������
	uint8_t cmd;				 //ָ��
	uint16_t len;				 //����
} TYPEs_WifiProtocolFront;
typedef struct
{
	TYPEs_WifiProtocolFront front; //Э��ǰ����
	uint8_t *dat;				   //���ݰ�ָ��
	uint16_t chk;				   //У���
} TYPEs_WifiProtocol;
#endif
#pragma pack() //ȡ������
/*************************˽��ȫ�ֱ���*************************/
static uint8_t secretKey[HAL_COMM_SECRET_KEY_SIZE] = {
	0x00,
	0x00,
}; //��Կ
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
/*************************����ȫ�ֱ���*************************/
#define COMM_DATA_OUT (bleCom.tx)
#define COMM_DATA_IN (bleCom.rx)
#define COMM_DATA_BLE_IN_START (bleCom.rx+(sizeof(commCtrl.upBat) + sizeof(commCtrl.downBat)))
#define COMM_DATA_BLE_OUT ((COMM_DATA_OUT + sizeof(TYPEs_BleProtocolFront)))
#define COMM_DATA_BLE_IN ((COMM_DATA_BLE_IN_START +sizeof(TYPEs_BleProtocolFront))) //�������ݴ��е�����Ϣ
/*
��������Ϊֻ�� ָ�볣�� ��ַ�����޸�
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
��������Ϊֻ�� ָ�볣�� ��ַ�����޸� BLEͨѶ����
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
uint8_t *pCommMac = NULL; //ָ��MAC��ָ��
uint8_t *pCommRootKeyA = NULL; //ָ��ROOT��ָ��
uint8_t *const pRxData = (uint8_t *)(COMM_DATA_IN);

/*
��������Ϊֻ�� ָ�볣�� ��ַ�����޸� wifiͨѶ����
*/
TYPEs_WifiSetRouter *const pWifiSetRouter = (TYPEs_WifiSetRouter *)(COMM_DATA_OUT + sizeof(TYPEs_CommWifiUpload));
TYPEs_WifiServerDomain *const pWifiServerDomain = (TYPEs_WifiServerDomain *)(COMM_DATA_OUT + sizeof(TYPEs_CommWifiUpload));

#undef COMM_DATA_OUT
#undef COMM_DATA_IN
#undef COMM_DATA_BLE_OUT 
#undef COMM_DATA_BLE_IN 
/*************************�ֲ�����*****************************/

/***************************************************************************************
**������:       HAL_Comm_CmdSend
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
static void HAL_Comm_CmdSend(TYPEs_ComSendParm *p)
{
	DRV_Comm_CmdSend(p);
}
/***************************************************************************************
**������:       HAL_Comm_BleDataPack
**��������:     �������ݴ��
**�������:    
uint16_t len  //Ҫ�������ݵĳ���
**�������:     
uint16_t
��������ݵĳ���
**��ע:         ����������й���С�˵�ת������Ҫȷ�ϳ����Ƿ����
****************************************************************************************/
static uint16_t HAL_Comm_BleDataPack(uint16_t len)
{
	uint16_t lenCnt = 0;
	//	uint16_t chkSum =0;
	bleCom.Protocol.front.len = len; //��д���ݳ���
	BSWAP_SAVE_16(bleCom.Protocol.front.len);
	memcpy((uint8_t *)(bleCom.tx),
		   (uint8_t *)(&bleCom.Protocol.front),
		   sizeof(bleCom.Protocol.front)); //����ͷ
	lenCnt = sizeof(bleCom.Protocol.front);
	// memcpy((uint8_t *)(bleCom.tx + lenCnt),
	// 	   dat,
	// 	   len); //�������
	lenCnt += len;																										  //��ȡ���ݳ���
	bleCom.Protocol.chk = DRV_Comm_GetSumChk((bleCom.tx + HAL_COMM_BLE_CHK_START_ADDR), lenCnt - HAL_COMM_BLE_HAED_SIZE); //����У���
	BSWAP_SAVE_16(bleCom.Protocol.chk);
	memcpy((uint8_t *)(bleCom.tx + lenCnt),
		   &bleCom.Protocol.chk,
		   sizeof(bleCom.Protocol.chk));   //�������
	lenCnt += sizeof(bleCom.Protocol.chk); //����У��͵ĳ���
	return lenCnt;
}
/***************************************************************************************
**������:       HAL_Comm_BleCmdSend
**��������:     �������ݷ���
**�������:    
TYPEe_Ble cmd ����ָ��
**�������:     
uint16_t
��������ݵĳ���
**��ע:         
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
		break; //(0x00)����
	case eBleVerifyChannelPwd:
		break; //(0x01)��֤�ŵ�����
	case eBleVerifyPhone:
		break; //(0x03)�ֻ�������֤
	case eBleUnlockUpload:
		break; //(0x04)����ʵʱ�ϴ�
	case eBleUnlockUploadEncrypt:
		break; //(0x05)����ʵʱ�����ϴ�wifi
	case eBleUnlockUploadEncryptWithVersion:
		break; //(0x06)���ϰ汾������Ϣʵʱ�����ϴ�wifi
	case eBleAlarmInforUpload:
		break; //(0x07)������Ϣ�ϴ�
	case eBleAlarmInforUploadEncrypt:
		break; //(0x08)���ܱ�����Ϣ�ϴ�
	case eBleLockStorageInforGet:
		break; //(0x20)���洢��Ϣ��ȡ
	case eBleUnlockRecordGet:
		break; //(0x21)������¼�����ѯ
	case eBleUnlockModeSetting:
		break; //(0x22)������ʽ����
	case eBleTimeSync:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x23)ʱ��ͬ��
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
		break; //(0x24)����ָ��
	case eBleAddPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x25)�����ֻ��˺�
	case eBleAddSmartKeyAccredit:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x26)��������Կ����Ȩ
	case eBleAddSmartKey:
		break; //(0x27)��������Կ��
	case eBleDelFinger:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x28)ɾ��ָ��
	case eBleDelPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x29)ɾ���ֻ��˺�
	case eBleDelSmartKey:
		 bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2A)ɾ������Կ��
	case eBleFingerAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2B)ָ�������޸�
	case eBleSmartKeyAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x2D)����Կ�������޸�
	case eBleUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x30)������������
	case eBleUnlockPwdSetAdmin:
		break; //(0x31)��������������
	case eBleFactory:
		break; //(0x32)�ָ���������
	case eBleEquipmentReg:
		bleCom.Protocol.front.type = eBleProtocolTypeAck;
		bleCom.Protocol.front.len = sizeof(TYPEs_BleEquipmentRegAck);
		break; //(0x34)�豸ע��
	case eBleEmptyUser:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x35)����û�
	case eBleLockPwdVerifyAdmin:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x36)������������֤
	case eBleLockFirmwareVersionGet:
		bleCom.Protocol.front.type = eBleProtocolTypeAck;
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockFirmwareVersionGetAck);
		break; //(0x37)���̼��汾��ȡ
	case eBleAlarmPwdSet:
		break; //(0x38)������������
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
		break; //(0x39)�ֻ�������֤���ܰ�
	case eBlePhoneSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BlePhoneSecretKeyGetAck);
		break; //(0x3A)�ֻ���Կ��ȡ
	case eBleUnlockSmartKeyVerify:
		bleCom.Protocol.front.len = sizeof(TYPEe_BleAck);
		break; //(0x3B)����Կ�׿�����֤
	case eBleSmartKeySecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSmartKeySecretKeyGet);
		break; //(0X3C)����Կ�׿�����Կ��ȡ
	case eBleSmartKeySecretKeyGetRandom:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSmartKeySecretKeyGetRandom);
		break; //(0X3C)����Կ����Ȩ��ȡ�������Կ
	case eBleLockRegSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockRegSecretKeyGet);
		break; //(0x3E)��ע����Կ��ȡ
	case eBleTmpeSecretKeyGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleTmpeSecretKeyGetAck);
		break; //(0X3F)��ʱ��Կ����
	case eBleTestServerConnectionStatus:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x40)���Է���������״̬
	case eBleWifiFunctionSwitchSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x41)WIFI���ܿ����ر�����
	case eBleDisconnectBluetooth:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xF0)�Ͽ���������
	case eBleWifiSetStart:
		break; //(0xF2)WIFI��������
	case eBleWifiSetEnd:
		break; //(0xf3)WIFI���ý���
	case eBleLockUpdateProgramMode:
		break; //(0xF4)LOCK���³���ģʽ
	case eBleWifiSetParm1:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWifiSetParmAck);
		break; //(0xf5)WIFI���ò���1
	case eBleWifiSetParm2:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWifiSetParm);
		break; //(0xf6)WIFI���ò���2
	case eBleWifiSetDomainNameParm1:
		break; //(0xf7)WIFI������������1
	case eBleWifiSetDomainNameParm2:
		break; //(0xf8)WIFI������������2
	case eBleUnlockUploadWifiSW:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xF9)������¼ʵʱ�ϴ���������(Ĭ�Ϲ����ǹر�)
	case eBlePhoneBleSecretKeyGet:
		break; //(0x50)�ֻ�����BLE��Կ��ȡָ��
	case eBleBleKeyHaveLockChk:
		break; //(0x51)��ѯBLEԿ����������(ע���������Ϊ20)
	case eBlePhoneDelBleKeyAllAccount:
		break; //(0x52)�ֻ�ɾ��BLEԿ����ȫ���˺�
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
		break; //(0x53)��APPЭ������ָ��
	case eBleNewConfirmFingerRegSucced:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x54)��APPЭ��ȷ��ָ��¼��ɹ�
	case eBleLockAddSmartKey:
		break; //(0x55)�Ž���������Կ��
	case eBleLockDelSmartKey:
		break; //(0x56)�Ž�ɾ������Կ��
	case eBleLocalFingerSync:
		break; //(0x57)����ָ��ͬ��
	case eBleLocalFingerDel:
		break; //(0x58)����ָ��ɾ��
	case eBleLocalFingerAttributeModif:
		break; //(0x59)����ָ�������޸�
	case eBleEncryptChipSEIDGet:
		break; //(0x60)����оƬSEID��ȡ
	case eBleEncryptChipChallengeGet:
		break; //(0x61)����оƬchallenge��ȡ
	case eBleEncryptChipVerifyMac:
		break; //(0x62)����оƬverifymac����
	case eBleServerTokenVerify:
		break; //(0x65)������token��֤����
	case eBleServerIssueChallenge:
		break; //(0x66)�������·�challenge
	case eBleID2ServerSwitch:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x67)����/����ID2����
	case eBleDHKeyExchangeAlgorithm:
		break; //(0x70)DH��Կ�����㷨
	case eBleLockVolumeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x71)������������
	case eBleGyroscopeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x72)����������
	case eBleWakeSourceQuery:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleWakeSourceQueryAck);
		break; //(0x73)����Դ��ѯ(������APP����Ҫ����)
	case eBleDESUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x74)DES������������
	case eBleDESUnlockAlarmPwdSet:
		break; //(0x75)DES������������
	case eBleDESTmpeSecretKeyGet:
		break; //(0x76)DES��ʱ��Կ����
	case eBleDESAddPhoneAccount:
		break; //(0x77)DES�����ֻ��˺�
	case eBleDESEncrytPhoneUnlockVerify:
		break; //(0x78)DES�ֻ�������֤���ܰ�
	case eBleAESUnlockPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x79)AES������������
	case eBleAESUnlockAlarmPwdSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x7A)AES������������
	case eBleAESAddPhoneAccount:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x7C)AES�����ֻ��˺�
	case eBleAESEncrytPhoneUnlockVerify:
		break; //(0x7D)AES�ֻ�������֤���ܰ�
	case eBleBaiduSEEncryptActivationRequest:
		break; //(0x7E)�ٶ�SE���ܼ�������
	case eBleBaiduSEUnlockPwdSet:
		break; //(0x7F)BaiduSE������������
	case eBleBaiduSEUnlockAlarmPwdSet:
		break; //(0x80)BaiduSE������������
	case eBleBaiduSETmpeSecretKeyGet:
		break; //(0x81)BaiduSE��ʱ��Կ����
	case eBleBaiduSEAddPhoneAccount:
		break; //(0x82)BaiduSE�����ֻ��˺�
	case eBleBaiduSEEncrytPhoneUnlockVerify:
		break; //(0x83)BaiduSE�ֻ�������֤���ܰ�
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
		break; //(0X85)APP����¼��
	case eBleFaceRegConfirm:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x86)APPȷ������¼��ɹ�
	case eBleFaceDelId:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x87)APPɾ������ID
	case eBleInfraredMonitoringDistanceSet:
		break; //(0x88)�������������
	case eBleDoorOpeningModeSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x89)���ŷ�ʽ����
	case eBlePhoneDelBleKey:
		break; //(0x90)�ֻ�ɾ��BLEԿ��
	case eBleMotorTorqueSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x91)���Ť������
	case eBleMotorDirSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x92)�����������
	case eBleAutomaticLockIntervalSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x93)�Զ���������ʱ��������
	case eBleQueryLockConfigTable:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleQueryLockConfigTableAck);
		break; //(0x94)��ѯ�������ñ�
	case eBleAKeyProtection:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x95)һ������
	case eBleActiveDefense:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x96)��������
	case eBleChannelPwdGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleChannelPwdGetAck);
		break; //(0x97)��ȡ�ŵ�����
	case eBleNFCSwitchSet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x98)NFC��������
	case eBleDoorbellCapture:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0x99)����ץ��
	case eBleFaceVersionGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleFaceVersionGetAck);
		break; //(0x9A)����ģ��汾�Ż�ȡ
	case eBleGetEventRecords:
		if(0 ==pBleGetEventRecords->num  ) //�¼���¼Ϊ��
		{
			bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		}
		else
		{
			bleCom.Protocol.front.len = sizeof(TYPEs_BleGetEventRecords);
		}
		break; //(0x9B)��ȡ�¼���¼
	case eBlefaceEnterFirmwareUpdateMode:
		break; //(0x9C)��������̼�����ģʽ
	case eBleFaceAttributeModify:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0X9D)���������޸�
	case eBleBleSinglePackageMaxlength:
		break; //(0x9E)��ȡ�����������Է��͵���󳤶�
	case eBleDHKeyExchangeAlgorithm1:
		break; //(0x9F)DH��Կ�����㷨
	case eBleCatEyeSN:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleCatEyeSNAck);
		break; //(0xA3)��ȡè�����к�
	case eBleShangTangCatEyeSingleDouble:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xA4)����è�۵�˫���л�����
	case eBleAddICCard:
		break; //(0xA5)app����IC������
	case eBleDelICCard:
		break; //(0xA6)appɾ��IC������
	case eBleAutoLockConfig:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xA7)ȫ�Զ���������
	case eBleAutoLockKeyConfig:
        bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
        break; //(0xA8)ȫ�Զ�����ȷ�ϼ�����
	case eBleToolAddModel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0XD5)������װ��������ͺ�
	case eBleToolTestRWLockModel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleToolTestRWLockModelAck);
		break; //(0xD6)������װ���Զ�����д�������ͺ�
	case eBleToolTestFail:
		break; //(0XD9)��װ���Բ��ϸ�
	case eBleFlashWriteTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xDB)FLASHд�����
	case eBleFlashRWComp:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleFlashRWCompAck);
		break; //(0xDC)FLASH������д��Ա�
	case eBleLedOledTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xDD)LED/OLED����
	case eBleVoltageTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleVoltageTestAck);
		break; //(0xDE)��ѹ����
	case eBleLockTimeGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockTimeGetAck);
		break; //(0xDF)��ȡ����ʱ��
	case eBleLockAgingRecord:
		break; //(0xB0)��ѯ�����ϻ���¼
	case eBleCatEyeTestParm:
		break; //(0xE0)����è�۲������
	case eBleWifiToolTest:
		break; //(0xD2)WIFI��װ���
	case eBleXiongmaiCatEyeIPGet:
		break; //(0xE1)����è��IP��ѯ
	case eBleSecondBatteryVoltageTest:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleSecondBatteryVoltageTestAck);
		break; //(0xE2)�·���أ��ڶ�·����ѹ����
	case eBleLockMacGet:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleLockMacGetAck);
		break; //(0xE3)��MAC��ѯ
	case eBleCatEyePIRToolTest:
		break; //(0xE4)è��PIR��װ���
	case eBleAliyunSecretKeySetParm:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xE5)��������Կ���ò���
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
		break; //(0XE7)R8è��OTA
	case eBleR8CatEyeDebugModeEn:
		break; //(0xE8)R8è��DEBUGģʽʹ��
	case eBleR8CatEyeDebugDataTransfer:
		break; //(0xE9)R8è��DEBUG���ݴ���
	case eBleR8EventLog:
		break; //(0XEA)R8WIFI�¼���¼
	case eBleWifiTestDR:
		break; //(0xEB)WIFI����ɨ��ָ��·����
	case eBleToolTestIC:
		break; //(0xEC)���⹤װ���IC��
	case eBleModifFaceSafetyLevel:
		bleCom.Protocol.front.len = sizeof(TYPEs_BleAck);
		break; //(0xED)�޸�����ʶ��ȫ�ȼ�
	default:
		break;
	}
	return HAL_Comm_BleDataPack(bleCom.Protocol.front.len); //�������
}
#if HAL_COMM_WIFI_EN
/***************************************************************************************
**������:       HAL_Comm_WifiDataPack
**��������:     �������ݴ��
**�������:    
uint32_t addr ���ʱ��ƫ�Ƶ�ַ
uint16_t len  //Ҫ�������ݵĳ���
**�������:     
uint16_t
��������ݵĳ���(����ƫ�Ƴ���)
**��ע:         ����������й���С�˵�ת������Ҫȷ�ϳ����Ƿ����
****************************************************************************************/
static uint16_t HAL_Comm_WifiDataPack(uint16_t addrOffset, uint16_t len)
{
	uint16_t lenCnt = 0;
	//	uint16_t chkSum =0;
	WifiProtocol.front.len = len; //��д���ݳ���
	BSWAP_SAVE_16(WifiProtocol.front.len);
	memcpy((uint8_t *)(bleCom.tx + addrOffset),
		   (uint8_t *)(&WifiProtocol.front),
		   sizeof(WifiProtocol.front)); //����ͷ
	lenCnt = sizeof(WifiProtocol.front);
	// memcpy((uint8_t *)(bleCom.tx + lenCnt),
	// 	   dat,
	// 	   len); //�������
	lenCnt += len;																													  //��ȡ���ݳ���
	WifiProtocol.chk = DRV_Comm_GetSumChk((bleCom.tx + addrOffset + HAL_COMM_WIFI_CHK_START_ADDR), lenCnt - HAL_COMM_WIFI_HAED_SIZE); //����У���
	BSWAP_SAVE_16(WifiProtocol.chk);
	memcpy((uint8_t *)(bleCom.tx + addrOffset + lenCnt),
		   &WifiProtocol.chk,
		   sizeof(WifiProtocol.chk));	//�������
	lenCnt += sizeof(WifiProtocol.chk); //����У��͵ĳ���
	lenCnt += addrOffset;				//ƴ�����е�����
	return lenCnt;
}

/***************************************************************************************
**������:       HAL_CommWifiCmdSend
**��������:     wifi���ݷ���
**�������:    
TYPEe_Wifi cmd wifiָ��
**�������:     
uint16_t
��������ݵĳ���
**��ע:         
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
		break; //0xF6)����·����
	case eWifiServerDomain:
		addrOffset = sizeof(TYPEs_CommWifiUpload);
		WifiProtocol.front.len = sizeof(TYPEs_WifiServerDomain);
		break; //0xF7)���÷���������
	case eWifiOpenMsgUpload:
		break; //0x04)������Ϣ�ϴ�
	case eWifiFaultMsgUpload:
		break; //0x07)������Ϣ�ϴ�
	case eWifiImageUpload:
		break; //0x09)ͼƬ�ϴ�
	case eWifiOTAUpdate:
		break; //0x10)OTA����
	case eWifiChkOTA:
		break; //0x11)ģ��������ѯOTA����
	case eWifiSetBPS:
		break; //0xFA)ͨѶ����������
	case eWifiChkMAC:
		break; //0xFB)��ѯWIFIģ��MAC��ַ
	case eWifiProductScanRouter:
		break; //0xFC)����ɨ��·����
	case eWifiChkSoft:
		break; //0xFD)����ɨ��·����
	case eWifiChkPskPwd:
		break; //0xFE)��ѯ��ǰģ����PSK����
	default:
		break;
	}
	return HAL_Comm_WifiDataPack(addrOffset, WifiProtocol.front.len);
}
#endif

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_Comm_Init
**��������:     ��ʼ��ͨѶ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_Init(void)
{
	DRV_Comm_Init();
	memset((uint8_t *)&commCtrl, 0, sizeof(TYPEs_CommCtrl));
	HAL_Comm_WakeupBack();
//    #ifdef USE_COMM_ENCRYPT
//	commCtrl.encryptFlag = eCommEncryptEn; //����
//    #else
//	commCtrl.encryptFlag = eCommEncryptDis; //�Ǽ���
//    #endif
}
/***************************************************************************************
**������:       HAL_Comm_BleSendStart
**��������:     ������ʼ�������ݣ��¹��߲���״̬���ķ�ʽ
**�������:     
TYPEe_Ble cmd, ����ָ��
uint32_t sendWait ���͵ȴ�ʱ��

**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_BleSendStart(TYPEe_Ble cmd,uint32_t sendWait)
{
	bleCom.Protocol.front.cmd = cmd;
	HAL_Comm_SendDealStart(eCommBleDataPass, sendWait); //������������
	
}
/***************************************************************************************
**������:       HAL_Comm_BleSendDir
**��������:     ������������ֱ�ӷ��Ͳ�ͨ��״̬��
**�������:     
TYPEe_Ble cmd, ����ָ��
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_BleSendDir(TYPEe_Ble cmd)
{
	bleCom.Protocol.front.cmd = cmd;
	HAL_Comm_Send(eCommBleDataPass);
}
/***************************************************************************************
**������:       HAL_Comm_BleSendDirWithLen
**��������:     ������������ֱ�ӷ��Ͳ�ͨ��״̬��,�������ݳ���
**�������:     
TYPEe_Ble cmd, ����ָ��

**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_BleSendDirWithLen(TYPEe_Ble cmd,uint16_t len)
{
	bleCom.Protocol.front.cmd = cmd;
	bleCom.Protocol.front.len = len;
	HAL_Comm_Send(eCommBleDataPass);
}
/***************************************************************************************
**������:       HAL_Comm_BleGetRecvMsgType
**��������:     ������ȡ��Ϣ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
TYPEe_Ble HAL_Comm_BleGetRecvMsgType(void)
{
	memcpy((uint8_t *)(&bleCom.ProtocolRx.front.header),
		   (uint8_t *)COMM_DATA_BLE_IN_START,
		   sizeof(TYPEs_BleProtocolFront));
	return bleCom.ProtocolRx.front.cmd;
}

/***************************************************************************************
**������:       HAL_Comm_Init
**��������:     ��ʼ��ͨѶ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
//uint8_t asekeyTest[6]={1,2,3,4,5,6};
/***************************************************************************************
**������:       HAL_Comm_Send
**��������:     ���ݷ���
**�������:     --
**�������:     --
**��ע:         
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
        commCtrl.encryptFlag = eCommEncryptEn; //ʹ����Ϻ��Զ���ɼ���״̬
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
		break; //(0x20)      ֪ͨ���������
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
		break; //(0x21) ���ż���Ϣ�ϴ�
	case eCommWifiUpload:
		#if HAL_COMM_WIFI_EN
		#endif
		break; //(0x23) ͸��WIFI��Ϣ
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
		break; //(0x28)ǰ��������ȡ��ص���
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
		break; //(0x29)ǰ��֪ͨ�Ͽ�BLE
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
		break; //(0x2A)ǰ��֪ͨ�������豸����
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
		break; //(0x2B)ǰ��֪ͨ���򿪱��õ�ع���
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
		break; //(0x2A)ǰ��֪ͨ�������豸����
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
		break; //(0x30)       �����BLE����͸��
	case eCommZigbeeExitNet:
		break; //(0xF0)     ZIGBEEģ������
	case eCommZigbeeEnterNet:
		break; //(0xF1)    ZIGBEEģ������
	case eCommWirelessMAC:
		break; //(0xFB)       ��ѯ����ģ��MAC��ַ
	case eCommScanGateway:
		break; //(0XFC)       ����ɨ��·����/����ģ������
	default:
		break;
	}
}
/***************************************************************************************
**������:       HAL_Comm_SendDealStart
**��������:     ���Ϳ�ʼָ��
**�������:     TYPEe_Comm cmd ��Ҫ���͵�����
**�������:  
int8_t
0; //�ɹ�
-1; //æµ
**��ע:         
****************************************************************************************/
int8_t HAL_Comm_SendDealStart(TYPEe_Comm cmd, uint32_t sendWait)
{
	if ((eCommStatusNone == commCtrl.Status) || (eCommStatusReceiveSucceed == commCtrl.Status))
	{
		commCtrl.Cmd = cmd;
		commCtrl.retryCnt = 0;				  //����ظ�����
		commCtrl.Status = eCommStatusSending; //���뷢��
		commCtrl.waitTimeCnt = sendWait;	  //���͵ȴ�
        DEBUG_LOG_OUT("comm ok\n");
		return 0;							  //�ɹ�
	}
	else
	{
        DEBUG_LOG_OUT("comm busy\n");
		return -1; //æµ
	}
}
/***************************************************************************************
**������:       HAL_Comm_SendDeal
**��������:     ���ʹ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
TYPEe_CommStatus HAL_Comm_SendDeal(void)
{
	switch (commCtrl.Status)
	{
	case eCommStatusNone:
		break; //����
	case eCommStatusSending:
		if (0 == commCtrl.waitTimeCnt)
		{
			HAL_Comm_Send(commCtrl.Cmd);
			if (eCommAckNeed == commCtrl.ack)
			{
				commCtrl.Status = eCommStatusReceive;		  //�������ݵȴ�
				commCtrl.rxTimeCnt = HAL_COMM_RECE_OVER_TIME; //���ʱʱ��
				commCtrl.ReceCmd = eCommNone;				  //�������������
			}
			else
			{
				commCtrl.Status = eCommStatusNone; //����ֱ�ӽ���
			}
		}
		else
		{
			commCtrl.waitTimeCnt--;
		}
		break; //���ݷ��ʹ�����
	case eCommStatusWaitWakeup:
		commCtrl.waitTimeCnt--;
		if (0 == commCtrl.waitTimeCnt)
		{
			commCtrl.waitWakeup = eCommWaitWakeupWork;
			commCtrl.Status = eCommStatusSending;
		}
		break; //��Ҫ�ȴ�����
	case eCommStatusACKNeed:
		break; //��Ҫ�ȴ�����
	case eCommStatusAckNot:
		break; //��Ҫ����Ҫ��Ӧ����
	case eCommStatusReceive:
		if (0 == commCtrl.rxTimeCnt) //��ʱ��
		{
			commCtrl.retryCnt++;
			if (commCtrl.retryCnt >= HAL_COMM_RECE_OVER_TIME_RETRY) //������������Դ���
			{
				commCtrl.Status = eCommStatusReceiveOverTime; //���ճ�ʱ
			}
			else
			{
				commCtrl.Status = eCommStatusSending; //�ط�һ��
			}
		}
		else
		{
			commCtrl.rxTimeCnt--;
		}
		if (commCtrl.Cmd == commCtrl.ReceCmd) //��ȡ����Ч������
		{
			//��Ҫ���ظ����ǲ�����ȷ��
			commCtrl.Status = eCommStatusReceiveSucceed;
		}
		break; //����������
	case eCommStatusReceiveOverTime:
		break; //���ճ�ʱ
	case eCommStatusReceiveError:
		break; //���մ�
	case eCommStatusReceiveSucceed:
		//���ճɹ���Ĵ���
		//		commCtrl.Status = eCommStatusNone; //�������
		break; //���ճɹ�
	default:
		break;
	}
	return commCtrl.Status;
}
/***************************************************************************************
**������:       HAL_Comm_GetRecvReply
**��������:     ����reply���͵���Ϣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_GetRecvReply(void)
{
	DRV_COMM_GetRecvContent((uint8_t *)(bleCom.rx), DRV_COMM_GetRecvContentSize());
}
/***************************************************************************************
**������:       HAL_Comm_RevcDeal
**��������:     ���մ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_RevcDeal(void)
{
    
	switch (DRV_Comm_RecvDataUnblock())
	{
	case 1: //�������,δ������ѭ����ȡ
		if (0 == DRV_Comm_RecvDataAndAnalysis(commCtrl.encryptFlag))
		{
			commCtrl.ReceCmd = (TYPEe_Comm)(DRV_COMM_GetRecvMsgType()); //��ȡ��Ϣ����
			HAL_Comm_GetRecvReply();									//��ȡ��Ϣ����
		}
		DRV_Comm_ClearRxPar(); //�������
		break;
	case -1:				   //��������
	case -2:				   //û������
	case -3:				   //���ղ�������
		DRV_Comm_ClearRxPar(); //�������
		break;
	default:
		break;
	}
}
/***************************************************************************************
**������:       HAL_Comm_GetReadAddr
**��������:     ��ȡ��ȡ��ַ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t *HAL_Comm_GetReadAddr(void)
{
	return bleCom.rx; //��ȡ��ȡ��ַ
}
/***************************************************************************************
**������:       HAL_Comm_GetRecvMsgType
**��������:     ��ȡ��Ϣ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
TYPEe_Comm HAL_Comm_GetRecvMsgType(void)
{
	return commCtrl.ReceCmd; //��ȡ��ȡ��ַ
}
/***************************************************************************************
**������:       HAL_Comm_CealrRecv
**��������:     �������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_CealrRecv(void)
{
	commCtrl.ReceCmd = eCommNone;
	memset((uint8_t *)(bleCom.rx), 0, HAL_COMM_RX_BUFF);
}

/***************************************************************************************
**������:       HAL_Comm_CealrSend
**��������:     �������
**�������:     --
**�������:     --
**��ע:         
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
**������:       HAL_COMM_EnterSleepConfig
**��������:     ��������IO����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_COMM_EnterSleepConfig(void)
{
	DRV_COMM_EnterSleepConfig();
}
/***************************************************************************************
**������:       ������Կ
**��������:     �������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_AesKeySet(uint8_t *p)
{
	memcpy(secretKey, p, HAL_COMM_SECRET_KEY_SIZE);
}
/***************************************************************************************
**������:       HAL_Comm_WakeupBack
**��������:     ���Ѻ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_WakeupBack(void)
{
	DRV_Comm_WakeupBack();
}
#if HAL_COMM_WIFI_EN
/***************************************************************************************
**������:       HAL_Comm_WifiGetRecvMsgType
**��������:     wifi��ȡ��Ϣ����
**�������:     
uint16_t addrOffset ��ַƫ��
**�������:     
TYPEe_Wifi
wifi��ָ��
**��ע:         
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
**������:       HAL_Comm_ChkBusy
**��������:     ��æ
**�������:     
**�������:  
TYPEe_CommBusy
**��ע:         
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
**������:       HAL_Comm_RstBack
**��������:     ��λ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_RstBack(void)
{
	DRV_Comm_RstBack();
}
/***************************************************************************************
**������:       HAL_Comm_InitBack
**��������:     ��ʼ�����
**�������:     --
**�������:     --
**��ע:         
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
**������:       DRV_Comm_WifiRtSet
**��������:     ����wifi��RT
**�������:     --
**�������:     --
**��ע:         
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
**������:       HAL_Comm_WifiCtRead
**��������:     ��ȡwifiCt������
**�������:     --
**�������:     --
**��ע:         
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
**������:       HAL_Comm_WifiCtInit
**��������:     ��ȡwifiCt������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t HAL_Comm_WifiCtInit(void)
{
//    M_WIFI_CT_IN_NOPULL();
}

/***************************************************************************************
**������:       HAL_Comm_WifiSend
**��������:     wifi���ݷ���
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
****************************************************************************************/
int8_t HAL_Comm_WifiSend(uint8_t *sendBuf, uint32_t len)
{
    return DRV_Comm_WifiSend(sendBuf,len);
}
/***************************************************************************************
**������:       HAL_Comm_EncryptionParam
**��������:     ������Ҫ�Ĳ�����ȡ
**�������:     --
**�������:     
int8_t
0; //�ɹ�
**��ע:         
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
**������:       HAL_Comm_SetEncrypt
**��������:     ���ܱ�־λ
**�������:     uint8_t sw
0 ����
!0 �Ǽ���
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Comm_SetEncrypt(uint8_t sw)
{
    if(0 == sw)
    {
        commCtrl.encryptFlag = eCommEncryptEn; //����
    }
    else
    {
        commCtrl.encryptFlag = eCommEncryptDis; //�Ǽ���
    }
}
#endif /*HAL_COMM_MODULE_EN*/
/************************************Code End******************************************/
