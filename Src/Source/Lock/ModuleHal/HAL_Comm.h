/***************************************************************************************
**�ļ���:     HAL_Comm.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:       ��Ҫע���ֽں�λ��Ĵ�С��ת��������
****************************************************************************************/
#ifndef __HAL_COMM_H__
#define __HAL_COMM_H__
#include "MyConfig.h"
#define HAL_COMM_MODULE_EN (1) //ģ��ʹ��
#if HAL_COMM_MODULE_EN
/*************************ר�ú궨��*************************/

#define LOCKS_FIRMWARE_VERSION_SIZE (20)
#define MAC_SIZE (6)                 //MAC����
#define ROUTER_SSID_SIZE (33)        //·����SSID����
#define ROUTER_PWD_SIZE (66)         //·�������볤��
#define DOMAIN_NAME_SERVER_SIZE (30) //������������
#define HAL_COMM_SEND_WAIT_TIEM (20) //���͵ȴ�ʱ��
// #define BLE_DATA_STAR_OFFSET (5)     //��������ƫ�Ƶ�ַ
/*************************�����ṹ����*************************/
#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
typedef enum
{
    eCommStatusNone,            //����
    eCommStatusSending,         //���ݷ��ʹ�����
    eCommStatusWaitWakeup,      //�ȴ����շ����ѳɹ�
    eCommStatusACKNeed,         //��Ҫ�ȴ�����
    eCommStatusAckNot,          //��Ҫ����Ҫ��Ӧ����
    eCommStatusReceive,         //����������
    eCommStatusReceiveOverTime, //���ճ�ʱ
    eCommStatusReceiveError,    //���մ���
    eCommStatusReceiveSucceed,  //���ճɹ�
} TYPEe_CommStatus;
typedef enum
{
    eCommWaitWakeupSleep, //���շ�δ����
    eCommWaitWakeupWork,  //���շ��Ѿ�������
} TYPEe_CommWaitWakeup;
typedef enum
{
    eCommNone = 0x00,                //����
    eCommMsgKeyStatus = 0x01,        //֪ͨǰ�尴��״̬
    eCommMsgMotorLock = 0x20,        //֪ͨ���������
    eCommOpenAndInfoUpload = 0x21,   //���ż���Ϣ�ϴ�
    eCommWifiUpload = 0x23,          //͸��WIFI��Ϣ
    eCommGetBattery = 0x28,          //������ȡ��ص���
    eCommMsgDisconnectBLE = 0x29,    //ǰ��֪ͨ�Ͽ�BLE
    eCommMsgUpdateDeviceName = 0x2A, //ǰ��֪ͨ�������豸����
	eCommMsgOpenBackupBat = 0x2B, 	 //ǰ��֪ͨ���򿪱��õ�ع���
	eCommMsgEnterOTA = 0x2C, 		 //ǰ��֪ͨ������OTAģʽ
    eCommBleDataPass = 0x30,         //�����BLE����͸��
    eCommwifiDataPass = 0x31,        //�����WIFI����͸��
    eCommZigbeeExitNet = 0xF0,       //ZIGBEEģ������
    eCommZigbeeEnterNet = 0xF1,      //ZIGBEEģ������
    eCommWirelessMAC = 0xFB,         //��ѯ����ģ��MAC��ַ
    eCommScanGateway = 0XFC,         //����ɨ��·����/����ģ������
} TYPEe_Comm;                        //ͨѶ����
typedef struct
{
    uint8_t year;           //��
    uint8_t month;          //��
    uint8_t day;            //��
    uint8_t week;           //��
    uint8_t hour;           //Сʱ
    uint8_t minute;         //����
    uint8_t second;         //��
} TYPEs_TimeFormatHaveWeek; //ʱ���ʽ(������)
typedef struct
{
    uint8_t year;   //��
    uint8_t month;  //��
    uint8_t day;    //��
    uint8_t hour;   //Сʱ
    uint8_t minute; //����
    uint8_t second; //��
} TYPEs_TimeFormat; //ʱ���ʽ���������ܣ�
typedef enum
{
    eBackKeyStatusNone = 0x00,  //�޲���
    eBackKeyStatusShort = 0x01, //�̰�
    eBackKeyStatusLong = 0x02,  //����
} TYPEe_BackKeyStatus;
typedef enum
{
    eInsideHandNone = 0x00,  //�޲���
    eInsideHandOpen = 0x01,  //����
    eInsideHandClose = 0x02, //����
} TYPEe_InsideHand;
typedef enum
{
    eBackBleNone = 0x00,   //�޲���
    eBackBleWakeup = 0x01, //����
} TYPEe_BackBle;
typedef enum
{
    eBackWifiNone = 0x00,   //�޲���
    eBackWifiWakeup = 0x01, //����
} TYPEe_BackWifi;
typedef enum
{
    eModuleInsertMSGNone = 0x00, //�޲���
    eModuleInsertMSGHave = 0x01, //��ģ�����
} TYPEe_ModuleInsertMSG;
typedef struct
{
    uint8_t MAC[6];                        //MAC
    uint8_t rootKey[6];                    //����Կ
    TYPEe_BackKeyStatus openKey;           //���ż���
    TYPEe_BackKeyStatus closeKey;          //���ż���
    TYPEe_InsideHand inside;               //����ֿ���
    TYPEe_BackBle backBle;                 //�������
    TYPEe_BackWifi backWifi;               //���WIFI
    TYPEe_ModuleInsertMSG moduleInsertMSG; //ģ�������ʾ
    uint16_t upBatBattery;                 //�Ϸ���ص���
    uint16_t downBatBattery;               //�·���ص���
    uint8_t version[9];                    //���汾��
} TYPEs_CommMsgKeyStatus;

typedef enum
{
    eMsgModuleUnknown = 0x00,  //δ֪
    eMsgModuleNone = 0x01,     //����Ϣģ��
    eMsgModuleWifi = 0x02,     //wifiģ��
    eMsgModuleWifiLive = 0x03, //����wifiģ��
    eMsgModuleNB = 0x04,       //NBģ��
    eMsgModuleZigbee = 0x05,   //zigbeeģ��
} TYPEe_MsgModule;             //��Ϣģ��
typedef struct
{
	uint8_t MAC[6];                        //MAC
    uint16_t upBatBattery;     //�Ϸ���ص���
    uint16_t downBatBattery;   //�·���ص���
    TYPEe_MsgModule msgModule; //��Ϣģ��
} TYPEs_CommGetBatteryAck;
typedef struct
{
    uint16_t up; 	//�Ϸ���ص���
    uint16_t down;   //�·���ص���
} TYPEs_BatteryStatus; //���״̬

typedef struct
{
    TYPEs_BatteryStatus battery; //���״̬
} TYPEs_CommMsgDisconnectBLE; 
typedef struct
{
	uint8_t sysStatus;	 //ϵͳ״̬
	TYPEs_BatteryStatus battery; //���״̬
    uint16_t ovetTime; //��ʱʱ��
    uint8_t nameLen;   //�豸���Ƴ���
    uint8_t nameFirst; //���ֵ�һ���ַ�,����ʱ��Ҫ��д������
} TYPEs_CommMsgUpdateDeviceName;

typedef struct
{
    uint16_t ovetTime; //��ʱʱ��
	uint8_t frontOtaFlag; //ǰ��OTAģʽ
	uint8_t backOtaFlag; //���OTAģʽ
	uint8_t updataFlag; //�Ƿ��������
    uint8_t nameLen;   //�豸���Ƴ���
    uint8_t nameFirst; //���ֵ�һ���ַ�,����ʱ��Ҫ��д������
} TYPEs_CommMsgEnterOTA;


typedef struct
{
    uint16_t upBat; 	//�Ϸ���ص���
    uint16_t downBat;   //�·���ص���
} TYPEs_CommMsgOpenBackupBat; 
typedef enum
{
    eAckStatusFail = 0x33,          //ʧ��
    eAckStatusSucceed = 0x55,       //�ɹ�
    eAckStatusLockFail = 0x66,      //����ʧ��
    eAckStatusLockSucceed = 0x77,   //�����ɹ�
    eAckStatusUplistFail = 0x88,    //�ϴ�ʧ��
    eAckStatusUplistSucceed = 0x99, //�ϴ��ɹ�
    //    eAckStatusSetNetFail = 0xA0,    //�����쳣
    //    eAckStatusSetNetSucceed = 0xA1, //�����ɹ�
} TYPEe_AckStatus;
typedef struct
{
    TYPEe_AckStatus ack; //�ظ�״̬
} TYPEs_CommAckStatus;
//typedef struct
//{
//    uint8_t MAC[6];     //MAC
//    uint8_t version[9]; //���汾��
//} TYPEs_CommMsgHardEmpty;

typedef enum
{
    ePlatformX, //X
    ePlatformY, //Y
    ePlatformZ, //Z
    ePlatformR, //R
    ePlatformS, //S
    ePlatformQ, //Q
} TYPEe_Platform;
typedef enum
{
    eOpenDirLeft = 'L',      //L��
    eOpenDirrRight = 'R',    //�ҿ�
    eOpenDirrSemiAuto = 'N', //���Զ�
} TYPEe_OpenDir;
typedef enum
{
    eLockBodyType = 'A', //��Ť�����
} TYPEe_LockBodyType;
typedef enum
{
    eWirelessModuleWifi = 'W',   //WIFIģ��
    eWirelessModuleZigbee = 'Z', //ZIGBEESģ��
    eWirelessModuleNB = 'N',     //NBģ��
} TYPEe_WirelessModule;

typedef enum
{
    eOpenDoorDirLeft = 0x11,  //L��
    eOpenDoorDirRight = 0x22, //�ҿ�
} TYPEe_OpenDoorDir;
typedef struct
{
    uint8_t OEM;                         //OEM
    uint8_t outdoorLock;                 //��������
    uint8_t indoorLock;                  //���ڰ�������
    TYPEe_OpenDoorDir openDoorDir;       //���ŷ���
    TYPEe_LockBodyType lockBodyType;     //��������
    uint8_t motorTorque;                 //���Ť��
    uint16_t quantity;                   //����
    TYPEe_WirelessModule wirelessModule; //����ģ��
	TYPEs_BatteryStatus battery; //���״̬
} TYPEs_CommMsgMotorLock;

typedef enum
{
    eOpenTypeFinger = 0x01,    //ָ��
    eOpenTypePwd = 0x02,       //����
    eOpenTypeCard = 0x03,      //��
    eOpenTypeFace = 0x04,      //����
    eOpenTypeVenous = 0x05,    //����
    eOpenTypeMechaKey = 0x06,  //��еԿ��
    eOpenTypeIris = 0x07,      //��Ĥ
    eOpenTypeRemote = 0x08,    //Զ�̿���
    eOpenTypeInsideKey = 0x09, //����尴������
    eOpenTypeNone = 0xFF,      //������
} TYPEe_OpenType;
typedef enum
{
    eOpenAndInfoUploadEventAttributeClose = 0x00, //���ţ�Ĭ��ֵ���߲��ϱ���
    eOpenAndInfoUploadEventAttributeOpen = 0x01,  //����
} TYPEe_OpenAndInfoUploadEventAttribute;
typedef enum
{
    eUserAttributeAverageUser = 0x00, //��ͨ�û�
    eUserAttributeAdminUser = 0x01,   //����Ա
    eUserAttributeTempUser = 0x02,    //��ʱ�û�
    eUserAttributeStressUser = 0x03,  //в���û�
    eUserAttributeAgingUser = 0x04,   //ʱЧ�û�
} TYPEe_UserAttribute;
typedef enum
{
    eAutoLockNot = 0x00,  //����Ҫ
    eAutoLockNeed = 0x01, //��Ҫ
} TYPEe_AutoLock;
typedef enum
{
    eWifiAttributeNone = 0x00,      //������
    eWifiAttributePushNoAck = 0x01, //���Ͳ��ظ�
    eWifiAttributePushAck = 0x02,   //���ͳɹ���ظ�
    eWifiAttributeConfig = 0x03,    //����
} TYPEe_WifiAttribute;
typedef struct
{
    uint8_t OEM;                       //OEM
    uint8_t MAC[6];                    //MAC
    TYPEe_OpenType openType;           //���ŷ�ʽ
    TYPEe_OpenDoorDir openDoorDir;     //���ŷ���
    TYPEe_LockBodyType lockBodyType;   //��������
    uint8_t motorTorque;               //���Ť��
    TYPEe_AutoLock autoLock;           //�Ƿ��Զ�����
    uint8_t autoLockTime;              //�Զ�����ʱ��
    uint16_t quantity;                 //����
    TYPEe_WifiAttribute wifiAttribute; //WIFI����
    uint16_t wifiDataLen;              //WIFI���ݰ���
	TYPEs_BatteryStatus battery; //���״̬
} TYPEs_CommOpenAndInfoUpload;
typedef enum
{
    eAlarmEventAttributeNone = 0x00,       //�޸澯��Ĭ��ֵ���߲��ϱ���
    eAlarmEventAttributeLowPower = 0x01,   //�͵����澯
    eAlarmEventAttributePry = 0x02,        //��������
    eAlarmEventAttributePwd = 0x03,        //���뱨��
    eAlarmEventAttributeFinger = 0x04,     //ָ�Ʊ���
    eAlarmEventAttributeCard = 0x05,       //�ſ�����
    eAlarmEventAttributeWallet = 0x06,     //Ǯ������
    eAlarmEventAttributeDoorUnlock = 0x07, //��δ���ñ���
    eAlarmEventAttributeDeployed = 0x08,   //��������
} TYPEe_AlarmEventAttribute;

typedef struct
{
    uint8_t OEM;                              //OEM
    uint8_t MAC[6];                           //MAC
    TYPEe_AlarmEventAttribute eventAttribute; //�¼�����
    uint16_t quantity;                        //����
    TYPEe_WifiAttribute wifiAttribute;        //WIFI����
    uint16_t wifiDataLen;                     //WIFI���ݰ���
	uint16_t upBat; 	//�Ϸ���ص���
    uint16_t downBat;   //�·���ص���
} TYPEs_CommWifiUpload;

typedef enum
{
    eFaultEventAttributeNone = 0x00,         //�޹��ϣ�Ĭ��ֵ���߲��ϱ���
    eFaultEventAttributeUnderVoltage = 0x01, //Ƿѹ����
    eFaultEventAttributeFinger = 0x02,       //ָ�ƹ���
    eFaultEventAttributeTouch = 0x03,        //����������
    eAlarmEventAttributeClock = 0x04,        //ʱ�ӹ���
} TYPEe_FaultEventAttribute;
typedef enum
{
    eUserListFinger = 0x01,   //ָ��
    eUserListPwd = 0x02,      //����
    eUserListCard = 0x03,     //��
    eUserListFace = 0x04,     //����
    eUserListVenous = 0x05,   //����
    eUserListMechaKey = 0x06, //��еԿ��
    eUserListIris = 0x07,     //��Ĥ
    eUserListRemote = 0x08,   //Զ�̿���
} TYPEe_UserList;
typedef enum
{
    eGetServerTimeAckSetFail = 0x33,     //����ʧ��
    eGetServerTimeAckDecryptFail = 0x44, //����ʧ��
    eGetServerTimeAckSuccess = 0x55,     //�ɹ�
    eGetServerTimeAckNetError = 0xA0,    //�����쳣
    eGetServerTimeAckNetNormal = 0xA1,   //��������
} TYPEe_GetServerTimeAck;
typedef struct
{
    uint8_t openDir;           //���ŷ���
    uint8_t lockBodyType;      //��������
    uint8_t motorTorque;       //���Ť��
    uint8_t automaticLockTime; //�Զ�����ʱ��
    uint16_t quantity;         //����
    uint8_t wirelessModule;    //����ģ��
} TYPEs_CommMsgBackOpen;
typedef struct
{
    uint16_t quantity; //����
} TYPEs_CommWirelessExitNet;

typedef enum
{
    ePwdAttributeTmpPwdOnce = 0x02, //һ�������루�������1�飩
    ePwdAttributeAging = 0x04,      //ʱЧ����
    ePwdAttributeRemote = 0x05,     //Զ�̿�������
} TYPEe_PwdAttribute;
typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //��������
    uint8_t Pwd[6];                  //��������
    uint32_t UTC;                    //UTC Time_4byte
} TYPEs_CommTmpPwdOnce;

typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //��������
    uint8_t Pwd[6];                  //��������
    uint32_t currentUTC;             //��ǰUTC
    uint32_t startUTC;               //��ʼUTC
    uint32_t endUTC;                 //����UTC
} TYPEs_CommAgingPwd;
typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //��������
    uint8_t Pwd[6];                  //��������
    uint32_t UTC;                    //UTC Time_4byte
} TYPEs_CommRemoteOpen;

typedef struct
{
    uint8_t SSID[ROUTER_SSID_SIZE]; //SSID
    uint8_t Pwd[ROUTER_PWD_SIZE];   //·��������
} TYPEs_CommWifiNetSet;

typedef struct
{
    uint8_t domainNameServer[DOMAIN_NAME_SERVER_SIZE]; //������������
    uint16_t port;                                     //�˿�
} TYPEs_CommWifiPortSet;

typedef struct
{
    uint8_t MAC[MAC_SIZE]; //mac
} TYPEs_CommWirelessMACAck;

typedef struct
{
    uint8_t SSID[ROUTER_SSID_SIZE]; //SSID
} TYPEs_CommScanGateway;

typedef enum
{
    eBleNone = 0x00,                            //����
    eBleVerifyChannelPwd = 0x01,                //��֤�ŵ�����
    eBleVerifyPhone = 0x03,                     //�ֻ�������֤
    eBleUnlockUpload = 0x04,                    //����ʵʱ�ϴ�
    eBleUnlockUploadEncrypt = 0x05,             //����ʵʱ�����ϴ� wifi
    eBleUnlockUploadEncryptWithVersion = 0x06,  //���ϰ汾������Ϣʵʱ�����ϴ� wifi
    eBleAlarmInforUpload = 0x07,                //������Ϣ�ϴ�
    eBleAlarmInforUploadEncrypt = 0x08,         //���ܱ�����Ϣ�ϴ�
    eBleLockStorageInforGet = 0x20,             //���洢��Ϣ��ȡ
    eBleUnlockRecordGet = 0x21,                 //������¼�����ѯ
    eBleUnlockModeSetting = 0x22,               //������ʽ����
    eBleTimeSync = 0x23,                        //ʱ��ͬ��
    eBleAddFinger = 0x24,                       //����ָ��
    eBleAddPhoneAccount = 0x25,                 //�����ֻ��˺�
    eBleAddSmartKeyAccredit = 0x26,             //��������Կ����Ȩ
    eBleAddSmartKey = 0x27,                     //��������Կ��
    eBleDelFinger = 0x28,                       //ɾ��ָ��
    eBleDelPhoneAccount = 0x29,                 //ɾ���ֻ��˺�
    eBleDelSmartKey = 0x2A,                     //ɾ������Կ��
    eBleFingerAttributeModify = 0x2B,           //ָ�������޸�
    eBleSmartKeyAttributeModify = 0x2D,         //����Կ�������޸�
    eBleGetMcuVersion = 0x2E,                   //��ȡ��ΧMCU�汾��
    eBleMcuEnterUpdataMode = 0x2F,              //֪ͨ��Χ����оƬ��������ģʽ
    eBleUnlockPwdSet = 0x30,                    //������������
    eBleUnlockPwdSetAdmin = 0x31,               //��������������
    eBleFactory = 0x32,                         //�ָ���������
    eBleEquipmentReg = 0x34,                    //�豸ע��
    eBleEmptyUser = 0x35,                       //����û�
    eBleLockPwdVerifyAdmin = 0x36,              //������������֤
    eBleLockFirmwareVersionGet = 0x37,          //���̼��汾��ȡ
    eBleAlarmPwdSet = 0x38,                     //������������
    eBleVerifyPhoneEncrypt = 0x39,              //�ֻ�������֤���ܰ�
    eBlePhoneSecretKeyGet = 0x3A,               //�ֻ���Կ��ȡ
    eBleUnlockSmartKeyVerify = 0x3B,            //����Կ�׿�����֤
    eBleSmartKeySecretKeyGet = 0X3C,            //����Կ�׿�����Կ��ȡ
    eBleSmartKeySecretKeyGetRandom = 0X3D,      //����Կ����Ȩ��ȡ�������Կ
    eBleLockRegSecretKeyGet = 0x3E,             //��ע����Կ��ȡ
    eBleTmpeSecretKeyGet = 0X3F,                //��ʱ��Կ����
    eBleTestServerConnectionStatus = 0x40,      //���Է���������״̬
    eBleWifiFunctionSwitchSet = 0x41,           //WIFI���ܿ����ر�����
    eBleDisconnectBluetooth = 0xF0,             //�Ͽ���������
    eBleWifiSetStart = 0xF2,                    //WIFI��������
    eBleWifiSetEnd = 0xf3,                      //WIFI���ý���
    eBleLockUpdateProgramMode = 0xF4,           //LOCK���³���ģʽ
    eBleWifiSetParm1 = 0xf5,                    //WIFI���ò���1
    eBleWifiSetParm2 = 0xf6,                    //WIFI���ò���2
    eBleWifiSetDomainNameParm1 = 0xf7,          //WIFI������������1
    eBleWifiSetDomainNameParm2 = 0xf8,          //WIFI������������2
    eBleUnlockUploadWifiSW = 0xF9,              //������¼ʵʱ�ϴ��������� (Ĭ�Ϲ����ǹر�)
    eBlePhoneBleSecretKeyGet = 0x50,            //�ֻ�����BLE ��Կ��ȡָ��
    eBleBleKeyHaveLockChk = 0x51,               //��ѯBLEԿ����������(ע���������Ϊ20)
    eBlePhoneDelBleKeyAllAccount = 0x52,        //�ֻ�ɾ��BLEԿ����ȫ���˺�
    eBleNewAddFinger = 0x53,                    //��APPЭ������ָ��
    eBleNewConfirmFingerRegSucced = 0x54,       //��APPЭ��ȷ��ָ��¼��ɹ�
    eBleLockAddSmartKey = 0x55,                 //�Ž���������Կ��
    eBleLockDelSmartKey = 0x56,                 //�Ž�ɾ������Կ��
    eBleLocalFingerSync = 0x57,                 //����ָ��ͬ��
    eBleLocalFingerDel = 0x58,                  //����ָ��ɾ��
    eBleLocalFingerAttributeModif = 0x59,       //����ָ�������޸�
    eBleEncryptChipSEIDGet = 0x60,              //����оƬSEID��ȡ
    eBleEncryptChipChallengeGet = 0x61,         //����оƬchallenge��ȡ
    eBleEncryptChipVerifyMac = 0x62,            //����оƬverify mac����
    eBleServerTokenVerify = 0x65,               //������token ��֤����
    eBleServerIssueChallenge = 0x66,            //�������·�challenge
    eBleID2ServerSwitch = 0x67,                 //����/����ID2����
    eBleDHKeyExchangeAlgorithm = 0x70,          //DH��Կ�����㷨
    eBleLockVolumeSet = 0x71,                   //������������
    eBleGyroscopeSet = 0x72,                    // ����������
    eBleWakeSourceQuery = 0x73,                 //����Դ��ѯ(������APP����Ҫ����)
    eBleDESUnlockPwdSet = 0x74,                 //DES������������
    eBleDESUnlockAlarmPwdSet = 0x75,            // DES������������
    eBleDESTmpeSecretKeyGet = 0x76,             //DES��ʱ��Կ����
    eBleDESAddPhoneAccount = 0x77,              //DES�����ֻ��˺�
    eBleDESEncrytPhoneUnlockVerify = 0x78,      // DES�ֻ�������֤���ܰ�
    eBleAESUnlockPwdSet = 0x79,                 //AES������������
    eBleAESUnlockAlarmPwdSet = 0x7A,            // AES������������
    eBleAESTmpeSecretKeyGet = 0x7B,             //AES��ʱ��Կ����
    eBleAESAddPhoneAccount = 0x7C,              //AES�����ֻ��˺�
    eBleAESEncrytPhoneUnlockVerify = 0x7D,      // AES�ֻ�������֤���ܰ�
    eBleBaiduSEEncryptActivationRequest = 0x7E, //�ٶ�SE���ܼ�������
    eBleBaiduSEUnlockPwdSet = 0x7F,             //BaiduSE������������
    eBleBaiduSEUnlockAlarmPwdSet = 0x80,        // BaiduSE������������
    eBleBaiduSETmpeSecretKeyGet = 0x81,         //BaiduSE��ʱ��Կ����
    eBleBaiduSEAddPhoneAccount = 0x82,          //BaiduSE�����ֻ��˺�
    eBleBaiduSEEncrytPhoneUnlockVerify = 0x83,  //BaiduSE�ֻ�������֤���ܰ�
    eBleFaceReg = 0X85,                         //APP����¼��
    eBleFaceRegConfirm = 0x86,                  //APPȷ������¼��ɹ�
    eBleFaceDelId = 0x87,                       //APPɾ������ID
    eBleInfraredMonitoringDistanceSet = 0x88,   //�������������
    eBleDoorOpeningModeSet = 0x89,              //���ŷ�ʽ����
    eBlePhoneDelBleKey = 0x90,                  //�ֻ�ɾ��BLEԿ��
    eBleMotorTorqueSet = 0x91,                  //���Ť������
    eBleMotorDirSet = 0x92,                     //�����������
    eBleAutomaticLockIntervalSet = 0x93,        //�Զ���������ʱ��������
    eBleQueryLockConfigTable = 0x94,            //��ѯ�������ñ�
    eBleAKeyProtection = 0x95,                  //һ������
    eBleActiveDefense = 0x96,                   //��������
    eBleChannelPwdGet = 0x97,                   //��ȡ�ŵ�����
    eBleNFCSwitchSet = 0x98,                    //NFC��������
    eBleDoorbellCapture = 0x99,                 //����ץ��
    eBleFaceVersionGet = 0x9A,                  //����ģ��汾�Ż�ȡ
    eBleGetEventRecords = 0x9B,                 //��ȡ�¼���¼
    eBlefaceEnterFirmwareUpdateMode = 0x9C,     //��������̼�����ģʽ
    eBleFaceAttributeModify = 0X9D,             //���������޸�
    eBleBleSinglePackageMaxlength = 0x9E,       //��ȡ�����������Է��͵���󳤶�
    eBleDHKeyExchangeAlgorithm1 = 0x9F,         //DH��Կ�����㷨
    eBleCatEyeSN = 0xA3,                        //��ȡè�����к�
    eBleShangTangCatEyeSingleDouble = 0xA4,     //����è�۵�˫���л�����
    eBleAddICCard = 0xA5,                       //app����IC������
    eBleDelICCard = 0xA6,                       //appɾ��IC������
    eBleAutoLockConfig = 0xA7,                  //ȫ�Զ���������
	eBleAutoLockKeyConfig = 0xA8,                  //ȫ�Զ�����ȷ�ϼ�����
    eBleToolAddModel = 0XD5,                    //������װ��������ͺ�
    eBleToolTestRWLockModel = 0xD6,             //������װ���Զ�����д�������ͺ�
    eBleToolTestFail = 0XD9,                    //��װ���Բ��ϸ�
    eBleFlashWriteTest = 0xDB,                  //FLASHд�����
    eBleFlashRWComp = 0xDC,                     //FLASH������д��Ա�
    eBleLedOledTest = 0xDD,                     //LED/OLED����
    eBleVoltageTest = 0xDE,                     //��ѹ����
    eBleLockTimeGet = 0xDF,                     //��ȡ����ʱ��
    eBleLockAgingRecord = 0xB0,                 //��ѯ�����ϻ���¼
    eBleCatEyeTestParm = 0xE0,                  //����è�۲������
    eBleWifiToolTest = 0xD2,                    //WIFI��װ���
    eBleXiongmaiCatEyeIPGet = 0xE1,             //����è�� IP��ѯ
    eBleSecondBatteryVoltageTest = 0xE2,        //�·���أ��ڶ�·����ѹ����
    eBleLockMacGet = 0xE3,                      //��MAC��ѯ
    eBleCatEyePIRToolTest = 0xE4,               //è��PIR��װ���
    eBleAliyunSecretKeySetParm = 0xE5,          //��������Կ���ò���
    eBleR8CatEyeOTA = 0XE7,                     //R8 è��OTA
    eBleR8CatEyeDebugModeEn = 0xE8,             //R8 è��DEBUGģʽʹ��
    eBleR8CatEyeDebugDataTransfer = 0xE9,       //R8 è��DEBUG���ݴ���
    eBleR8EventLog = 0XEA,                      //R8 WIFI�¼���¼
    eBleWifiTestDR = 0xEB,                      //WIFI����ɨ��ָ��·����
    eBleToolTestIC = 0xEC,                      //���⹤װ���IC��
    eBleModifFaceSafetyLevel = 0xED,            //�޸�����ʶ��ȫ�ȼ�
} TYPEe_Ble;                                    //ͨѶ����
typedef enum
{
    eBleAckSuccess = 0x00, //�ɹ�
    eBleAckFail = 0x01,    //ʧ��
} TYPEe_BleAck;
typedef struct
{
    TYPEe_BleAck ack;
} TYPEs_BleAck;
typedef struct
{
    TYPEs_TimeFormatHaveWeek time;
} TYPEs_BleTimeSync;
typedef struct
{
    uint32_t ID1;
	uint32_t ID2;
	uint32_t ID3;
} TYPEs_UserPageId;

typedef struct
{
    TYPEs_UserPageId pageId;
} TYPEs_BleDelFinger;

typedef enum
{
    eBleAddFingerAckSuccess = 0x00,   //�ɹ�
    eBleAddFingerAckFail = 0x01,      //ʧ��
    eBleAddFingerAckIng = 0x02,       //�����
    eBleAddFingerAckAppNotReg = 0x04, //APPδע��
} TYPEe_BleAddFingerAck;

typedef struct
{
    uint32_t id;
    uint8_t cnt;
} TYPEs_BleAddFingerSuccess;

typedef struct
{
    uint8_t cnt;
} TYPEs_BleAddFingerIng;
union uAdd
{
    TYPEs_BleAddFingerSuccess addSuccess;
    TYPEs_BleAddFingerIng addIng;
};
typedef struct
{
    TYPEe_BleAddFingerAck ack;
    union uAdd add;
} TYPEs_BleAddFingerAck;




typedef enum
{
    eBleAddFaceAckSuccess = 0x00,   //�ɹ�
    eBleAddFaceAckFail = 0x01,      //ʧ��
    eBleAddFaceAckIng = 0x02,       //�����
    eBleAddFaceAckAppNotReg = 0x04, //APPδע��
} TYPEe_BleAddFaceAck;
typedef struct
{
    uint32_t id;
    uint8_t cnt;
} TYPEs_BleAddFaceSuccess;
typedef enum
{
    eBleAddFaceDirEnter = 0x00,
    eBleAddFaceDirMiddle = 0x01, //����
    eBleAddFaceDirUP = 0x02,     //̧ͷ
    eBleAddFaceDirRight = 0x03,  //����
    eBleAddFaceDirDown = 0x04,   //��ͷ
    eBleAddFaceDirLeft = 0x05,   //����
} TYPEe_BleAddFaceDir;
typedef struct
{
    TYPEe_BleAddFaceDir cnt;
} TYPEs_BleAddFaceIng;
union uAddFace
{
    TYPEs_BleAddFaceSuccess addSuccess;
    TYPEs_BleAddFaceIng addIng;
};
typedef struct
{
    TYPEe_BleAddFaceAck ack;
    union uAddFace add;
} TYPEs_BleAddFaceAck;
typedef enum
{
    eBleFaceRegResultSuccess = 0x00, //�洢�ɹ�
    eBleFaceRegResultFail = 0x01,    //����ʧ��
} TYPEe_BleFaceRegResult;
typedef struct
{
    TYPEe_BleFaceRegResult result;
} TYPEs_BleFaceRegConfirm;

typedef struct
{
    uint16_t id;
} TYPEs_BleFaceDelId;

typedef enum
{
    eInfraredDistanceLong = 0x0000, //Զ����
    eInfraredDistanceShort = 0x0001, //Զ����
    eInfraredDistanceClose = 0x0002, //�ر�
} TYPEe_InfraredDistance;
typedef struct
{
   uint16_t distance;
} TYPEs_BleInfraredMonitoringDistanceSet;


typedef struct
{
	uint8_t mode; //����ģʽ
	uint8_t unlockType; //��������
} TYPEs_BleDoorOpeningModeSet;

typedef enum
{
    eBleMotorTorqueDefault = 0x00, //Ĭ�ϣ���Ť��
    eBleMotorTorqueLow = 0x01,     //2.7A����Ť��
    eBleMotorTorqueHigh = 0x02,    //3.7A,��Ť��
} TYPEe_BleMotorTorque;
typedef struct
{
    TYPEe_BleMotorTorque torque;
} TYPEs_BleMotorTorqueSet;

typedef enum
{
    eBleMotorDirRight = 0x55, //Ĭ�ϣ��ҿ�
    eBleMotorDirLife = 0x01,  //��
} TYPEe_BleMotorDir;
typedef struct
{
    TYPEe_BleMotorDir dir;
} TYPEs_BleMotorDirSet;
typedef struct
{
    uint8_t closeTime;  //0,��ʾ�رգ���������
} TYPEs_BleAutomaticLockIntervalSet;
typedef enum
{
    eBleAKeySwClose = 0, //�رգ�Ĭ�ϣ�
    eBleAKeySwOpen = 1,  //��
} TYPEe_BleAKeySw;
typedef struct
{
    TYPEe_BleAKeySw sw;
} TYPEs_BleAKeyProtection;

typedef struct
{
    uint8_t time;
} TYPEs_BleActiveDefense;

typedef struct
{
    TYPEe_BleAck ack;
    uint8_t pwd[4];
} TYPEs_BleChannelPwdGetAck;
typedef struct
{
    uint8_t sw;
} TYPEs_BleNFCSwitchSet;
typedef struct
{
    uint8_t sw;
} TYPEs_BleDoorbellCapture;
typedef struct
{
    TYPEe_BleAck ack;
    uint8_t version[32];
} TYPEs_BleFaceVersionGetAck;

typedef struct
{
    TYPEe_BleAck ack;
	uint16_t num;
    uint8_t log[14];
} TYPEs_BleGetEventRecords;

typedef struct
{
    uint8_t phoneID[13];
} TYPEs_BleAddPhoneAccount;

typedef struct
{
    uint8_t key[13];
} TYPEs_BleAddSmartKeyAccredit;

typedef struct
{
    uint8_t phoneID[13];
} TYPEs_BleDelPhoneAccount;
typedef struct
{
    uint8_t key[13];
} TYPEs_BleDelSmartKey;

typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t reserved : 1;
    uint8_t Mon : 1;  //��һ
    uint8_t Tues : 1; //�ܶ�
    uint8_t Wed : 1;  //����
    uint8_t Thur : 1; //����
    uint8_t Fri : 1;  //����
    uint8_t Sat : 1;  //����
    uint8_t Sun : 1;  //����
#else
    uint8_t Sun : 1;  //����
    uint8_t Sat : 1;  //����
    uint8_t Fri : 1;  //����
    uint8_t Thur : 1; //����
    uint8_t Wed : 1;  //����
    uint8_t Tues : 1; //�ܶ�
    uint8_t Mon : 1;  //��һ
    uint8_t reserved : 1;
#endif // USE_LITTLE_ENDIAN
} TYPEs_Week;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t SOS : 4;    //����
    uint8_t kindle : 4; //����
#else
    uint8_t kindle : 4; //����
    uint8_t SOS : 4;    //����
#endif // USE_LITTLE_ENDIAN

} TYPEs_kindleAndSos;
typedef enum
{
    eAgingStatusOn = 0x0F,  //����
    eAgingStatusOff = 0x00, //�ر�
} TYPEe_AgingStatus;
typedef struct
{
    uint16_t pageId;              //ָ��PageID(12byte)
    TYPEs_kindleAndSos kindleAndSos; //����/����(1byte)
    TYPEe_AgingStatus aging;         //ʱЧ��1byte��
    TYPEs_TimeFormat startTm;        //��ʼʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_TimeFormat agingTm;        //ʱЧʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_Week week;                 //��(1byte)
} TYPEs_BleFaceAttributeModify;      //�����޸�

typedef struct
{
	TYPEe_BleAck ack; 
    uint8_t sn[20]; //�������ݰ���16Byte ���к� �������ݰ���20Byte ���к�
} TYPEs_BleCatEyeSNAck;  

typedef struct
{
	uint8_t sw; //0x00˫��ģʽ ��0x01����ģʽ������ע��Ĭ��00
} TYPEs_BleShangTangCatEyeSingleDouble; 
typedef struct
{
    uint8_t lockType; //1	Ĭ�ϣ��������� 2	��Ԫ������ 3	�Ű�����
} TYPEs_BleAutoLockConfig;

typedef struct
{
    uint8_t sw;
} TYPEs_BleAutoLockKeyConfig;

typedef struct
{
    uint8_t model[8];
} TYPEs_BleToolAddModel;

typedef struct
{
    TYPEe_BleAck ack;
	uint8_t model[8];
} TYPEs_BleToolTestRWLockModelAck;    
typedef struct
{
    uint8_t test[2];
} TYPEs_BleFlashWriteTest;  


typedef struct
{
	TYPEe_BleAck ack;
    uint8_t test[2];
} TYPEs_BleFlashRWCompAck;
typedef struct
{
    TYPEe_BleAck ack;
	uint16_t Value;
} TYPEs_BleVoltageTestAck;    

typedef struct
{
    TYPEe_BleAck ack;
	uint8_t time[7];
} TYPEs_BleLockTimeGetAck;

typedef struct
{
     TYPEe_BleAck ack;
	uint16_t Value;
} TYPEs_BleSecondBatteryVoltageTestAck;    
typedef struct
{
    uint8_t num[3];
} TYPEs_BleLockMacGet; 
typedef struct
{
	TYPEe_BleAck ack;
    uint8_t BleMAC[7];
	uint8_t WifiMAC[7];
} TYPEs_BleLockMacGetAck; 

typedef struct
{
    uint8_t recognition; //
	uint8_t biopsy; //��찲ȫ�ȼ�
} TYPEs_BleModifFaceSafetyLevel;
typedef struct
{
    uint8_t productKey[32]; 
	uint8_t productSecret[64]; 
	uint8_t deviceName[32];
	uint8_t deviceSecre[64];
} TYPEs_BleAliyunSecretKeySetParm;

typedef struct
{
    uint8_t updatSelect; 
} TYPEs_BleR8CatEyeOTA;

typedef enum
{
    eFaceOTAAckSuccess = 0x00,  
	eFaceOTAAckFail = 0x01,  
    eFaceOTAAckUpdataing = 0x02,  
	eFaceOTAAckUpdataSuccess = 0x03, 
	eFaceOTAAckTimeOver = 0x04,
} TYPEe_FaceOTAAck;
typedef enum
{
    eFaceOTAStatusNone= 0x00,  
    eFaceOTAStatusWorking = 0x01,  
	eFaceOTAStatusDone= 0x02,  
	eFaceOTAStatusError= 0x03,
} TYPEe_FaceOTAStatus;
typedef struct
{
	TYPEe_FaceOTAStatus status;
	uint8_t	error;
	uint8_t progress;
}TYPEs_FaceOTAIng;
#if 0
typedef struct
{
	
}TYPEs_FaceOTASuccess;

union uFaceOTA
{
	TYPEs_FaceOTAIng faceOTAing;
	TYPEs_FaceOTASuccess faceOTASuccess;
};
#endif
typedef struct
{
	TYPEe_FaceOTAAck ack;
    TYPEs_FaceOTAIng faceOTAing;
} TYPEs_BleR8CatEyeOTAAck;

typedef struct
{
    TYPEe_BleAck ack;
    uint8_t lockSN[15];
    uint8_t MAC[6];
    uint8_t channelPwd[4];
} TYPEs_BleEquipmentRegAck;

typedef struct
{
    TYPEe_BleAck ack;
    uint8_t version[19];
} TYPEs_BleLockFirmwareVersionGetAck;

typedef struct
{
	TYPEe_BleAck ack;
    uint8_t key[8];
} TYPEs_BlePhoneSecretKeyGetAck;
typedef struct
{
	TYPEe_BleAck ack;
    uint8_t key[8];
} TYPEs_BleSmartKeySecretKeyGetRandom;
typedef struct
{
	TYPEe_BleAck ack;
    uint8_t key[8];
} TYPEs_BleSmartKeySecretKeyGet;
typedef struct
{
	TYPEe_BleAck ack;
    uint8_t key[8];
} TYPEs_BleLockRegSecretKeyGet;

typedef struct
{
	TYPEe_BleAck ack;
    uint8_t pwd[60];
} TYPEs_BleTmpeSecretKeyGetAck;    
typedef struct
{
	uint8_t channelPwd[4]; //�ŵ�����(4byte)
	uint8_t phone[13]; //�ֻ�����(�˺�ID 13byte)
	TYPEs_TimeFormatHaveWeek time; //������ʱ��(ʱ���ʽ(��,��,��,��,Сʱ,����,��)(7byte))
} TYPEs_VerifyPhone;  	
typedef struct
{
	union  
	{
		uint8_t tab[sizeof(TYPEs_VerifyPhone)];
		TYPEs_VerifyPhone verifyPhone;
	}unData;
} TYPEs_BleVerifyPhoneEncrypt;    
typedef struct
{
	TYPEe_BleAck ack;
	uint16_t bat; //��ص���
} TYPEs_BleVerifyPhoneEncryptAck;   

typedef struct
{
	uint8_t channelPwd[4]; //�ŵ�����(4byte)
	uint8_t phone[13]; //�ֻ�����(�˺�ID 13byte)
} TYPEs_VerifySmartKey;  	
typedef struct
{
	union  
	{
		uint8_t tab[sizeof(TYPEs_VerifySmartKey)];
		TYPEs_VerifySmartKey verifySmartKey;
	}unData;
} TYPEs_BleUnlockSmartKeyVerify;   

typedef enum
{
    eManufacturerDSM = 0x00,       //��ʩ��
    eManufacturerKLN = 0x01,       //������
    eManufacturerJKD = 0x02,       //�𿭵�
    eManufacturerPanasonic = 0x03, //����
} TYPEe_Manufacturer;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    TYPEe_Manufacturer manufacturer;
    uint8_t reserved[7];
#else
    uint8_t reserved[7];
    TYPEe_Manufacturer manufacturer;
#endif // USE_LITTLE_ENDIAN
} TYPEs_BleQueryLockConfigTable;
typedef enum
{
	eOpenEncryptionTypeOld = 0x00,  //��Э��
    eOpenEncryptionTypeDH = 0x01,  //DH����
    eOpenEncryptionTypeID2 = 0x02, //ID2����
    eOpenEncryptionTypeDES = 0X03, //DES
    eOpenEncryptionTypeAES = 0X04, //AES-128 bits/ECB/NoPadding �㷨�ԳƼ���
    eOpenEncryptionTypeSE = 0X05,  //�ٶ�SE����
} TYPEe_OpenEncryptionType;
typedef enum
{
    eIssuePwdAESAddPhoneOld = 0x00, //��Э�飨�����·���
    eIssuePwdAESAddPhoneDES = 0x01, //DES(��������)
    eIssuePwdAESAddPhoneAES = 0x02, //AES128 bits/ECB/NoPadding �㷨�ԳƼ���
    eIssuePwdAESAddPhoneSE = 0x03,  //�ٶ�SE����
} TYPEe_IssuePwdAESAddPhone;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t xPlatform : 1;   //Xƽ̨����֧������ָ�ƣ�
    uint8_t yPlatform : 1;   //Yƽ̨��֧������ָ�ƣ�
    uint8_t newprotocol : 1; //��Э�飨0x53+0x54��
    uint8_t oldprotocol : 1; //��Э�飨0x24��
    uint8_t reserved : 4;    //Ԥ��
#else
    uint8_t reserved : 4;    //Ԥ��
    uint8_t oldprotocol : 1; //��Э�飨0x24��
    uint8_t newprotocol : 1; //��Э�飨0x53+0x54��
    uint8_t yPlatform : 1;   //Yƽ̨��֧������ָ�ƣ�
    uint8_t xPlatform : 1;   //Xƽ̨����֧������ָ�ƣ�
#endif // USE_LITTLE_ENDIAN
} TYPEs_RegFingPram;
//typedef enum
//{
//    eWirelessModuleWifi = 0x00, //WIFIģ��
//    eWirelessModuleNB = 0x01,   //NBģ��
//} TYPEe_WirelessModule;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t reserved;
    uint8_t pwd : 1;                  //�������
    uint8_t finger : 1;               //ָ��ʶ�����
    uint8_t smartKey : 1;             //����Կ�׽���������Կ��/�����ֻ���
    uint8_t face : 1;                 //����ʶ�����
    uint8_t ICCard : 1;               //IC������
    uint8_t fingerVein : 1;           //ָ��������
    uint8_t electroMechanicalKey : 1; //���ӻ�еԿ�׽���
    uint8_t Huawei : 1;               //��ΪǮ������
#else
    uint8_t reserved;
    uint8_t Huawei : 1;               //��ΪǮ������
    uint8_t electroMechanicalKey : 1; //���ӻ�еԿ�׽���
    uint8_t fingerVein : 1;           //ָ��������
    uint8_t ICCard : 1;               //IC������
    uint8_t face : 1;                 //����ʶ�����
    uint8_t smartKey : 1;             //����Կ�׽���������Կ��/�����ֻ���
    uint8_t finger : 1;               //ָ��ʶ�����
    uint8_t pwd : 1;                  //�������
#endif // USE_LITTLE_ENDIAN
} TYPEs_UnlockType;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t enSpeed : 1;           //������ģ��
    uint8_t enOpenDirSet : 1;      //���ŷ�����������ң�
    uint8_t enMotorTorqueSet : 1;  //���Ť������
    uint8_t enAutoLockTimeSet : 1; //�Զ�����ʱ�����
	uint8_t enAutoLockAffirm : 1; 	//ȫ�Զ�����ȷ�Ϲ��� 
    uint8_t reserved : 3;
    uint8_t swSpeed : 1;          //������ģ��0=�ر�
    uint8_t swOpenDirSet : 1;     //���ŷ�����������ң�0=�ҿ�
    uint8_t swMotorTorqueSet : 1; //���Ť������0=��Ť��
    uint8_t swAutoLockTime : 5;   //�Զ�����ʱ�����(0-32)*5��
#else
    uint8_t swAutoLockTime : 5;       //�Զ�����ʱ�����(0-32)*5��
    uint8_t swMotorTorqueSet : 1;     //���Ť������0=��Ť��
    uint8_t swOpenDirSet : 1;         //���ŷ�����������ң�0=�ҿ�
    uint8_t swSpeed : 1;              //������ģ��0=�ر�
    uint8_t reserved : 4;
    uint8_t enAutoLockTimeSet : 1;               //�Զ�����ʱ�����
    uint8_t enMotorTorqueSet : 1;                //���Ť������
    uint8_t enOpenDirSet : 1;                    //���ŷ�����������ң�
    uint8_t enSpeed : 1;                         //������ģ��
#endif // USE_LITTLE_ENDIAN
} TYPEs_AutoConfig;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t enAKeyProtection : 1;                //һ������
    uint8_t enLockVolumeSet : 1;                 //��������
    uint8_t enActiveDefense : 1;                 //��������������30���⣩
    uint8_t enNFC : 1;                           //NFC����
    uint8_t enOpenModeSet : 1;                   //����ģʽ��ѡ
    uint8_t enDoorbellCapture : 1;               //����ץ��
    uint8_t enInfraredMonitoringDistanceSet : 1; //�������
    uint8_t enEventLog : 1;                      //�¼���¼

    uint8_t swAKeyProtection : 1;                //һ������
    uint8_t swLockVolumeSet : 1;                 //��������
    uint8_t swActiveDefense : 1;                 //��������������30���⣩
    uint8_t swNFC : 1;                           //NFC����
    uint8_t swOpenModeSet : 1;                   //����ģʽ��ѡ
    uint8_t swDoorbellCapture : 1;               //����ץ��
    uint8_t swInfraredMonitoringDistanceSet : 1; //�������
    uint8_t swEventLog : 1;                      //�¼���¼
#else
    uint8_t swEventLog : 1;                      //�¼���¼
    uint8_t swInfraredMonitoringDistanceSet : 1; //�������
    uint8_t swDoorbellCapture : 1;               //����ץ��
    uint8_t swOpenModeSet : 1;                   //����ģʽ��ѡ
    uint8_t swNFC : 1;                           //NFC����
    uint8_t swActiveDefense : 1;                 //��������������30���⣩
    uint8_t swLockVolumeSet : 1;                 //��������
    uint8_t swAKeyProtection : 1;                //һ������
    uint8_t enEventLog : 1;                      //�¼���¼
    uint8_t enInfraredMonitoringDistanceSet : 1; //�������
    uint8_t enDoorbellCapture : 1;               //����ץ��
    uint8_t enOpenModeSet : 1;                   //����ģʽ��ѡ
    uint8_t enNFC : 1;                           //NFC����
    uint8_t enActiveDefense : 1;                 //��������������30���⣩
    uint8_t enLockVolumeSet : 1;                 //��������
    uint8_t enAKeyProtection : 1;                //һ������
#endif // USE_LITTLE_ENDIAN
} TYPEs_LockFunctionConfig1;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t xiaoDiOemNewRing : 1;                //С��OEM���ֻ�
    uint8_t catSEye : 1;                        //è��
    uint8_t shangTangCatSEye : 1;                //����è��
    uint8_t aoBiCatSEye : 1;                     //�±�è��
    uint8_t icCard : 1;                          //IC��
    uint8_t R8InfraredMonitoringDistanceSet : 1; //R8�������ɵ��ڣ�Զ�������رգ�
    uint8_t NewRemote : 1;                       //��ң����
    uint8_t reserved : 1;

    uint8_t reservedB : 1; 
	uint8_t McuGX: 1;
    uint8_t shangTangCatSEyesingleAndDouble : 1; //����è�۵�˫��
    uint8_t reservedA : 5;
#else
    uint8_t reservedA : 5;
    uint8_t shangTangCatSEyesingleAndDouble : 1; //����è�۵�˫��
    uint8_t reservedB : 2;
    uint8_t reserved : 1;
    uint8_t NewRemote : 1;                       //��ң����
    uint8_t R8InfraredMonitoringDistanceSet : 1; //R8�������ɵ��ڣ�Զ�������رգ�
    uint8_t icCard : 1;                          //IC��
    uint8_t aoBiCatSEye : 1;                     //�±�è��
    uint8_t shangTangCatSEye : 1;                //����è��
    uint8_t catSEye : 1;                         //è��
    uint8_t xiaoDiOemNewRing : 1;                //С��OEM���ֻ�
#endif // USE_LITTLE_ENDIAN
} TYPEs_LockFunctionConfig2;
typedef enum
{
    eLockVolumeSetMute = 0x00,   //����
    eLockVolumeSetLow = 0x01,    //��
    eLockVolumeSetMiddle = 0x02, //��
    eLockVolumeSetHigh = 0x03,   //��
} TYPEe_LockVolumeSet;           //������������
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t openModePwd : 1;          //�������
    uint8_t openModeFinger : 1;       //ָ��ʶ�����
    uint8_t openModeSmartKey : 1;     //����Կ�׽���������Կ��/�����ֻ���
    uint8_t openModeFace : 1;         //����ʶ�����
    uint8_t openModeIcCard : 1;       //IC������
    uint8_t openModeFingerVein : 1;   //ָ��������
    uint8_t openModeElecMechaKey : 1; //���ӻ�еԿ�׽���
    uint8_t openModeAndOr : 1;        //1������ģʽ0�����ģʽ
#else
    uint8_t openModeAndOr : 1;                   //1������ģʽ0�����ģʽ
    uint8_t openModeElecMechaKey : 1;            //���ӻ�еԿ�׽���
    uint8_t openModeFingerVein : 1;              //ָ��������
    uint8_t openModeIcCard : 1;                  //IC������
    uint8_t openModeFace : 1;                    //����ʶ�����
    uint8_t openModeSmartKey : 1;                //����Կ�׽���������Կ��/�����ֻ���
    uint8_t openModeFinger : 1;                  //ָ��ʶ�����
    uint8_t openModePwd : 1;                     //�������
#endif            // USE_LITTLE_ENDIAN
} TYPEs_OpenMode; //����ģʽ״̬
typedef enum
{
    eInfraredRangeStatusFar = 0x0000,    //Զ����
    eInfraredRangeStatusNearly = 0x0001, //������
    eInfraredRangeStatusClose = 0x0002,  //�ر�
} TYPEe_InfraredRangeStatus;             //�������״̬
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t nfcDetection : 1; //NFC���
    uint8_t versionQuery : 1; //�汾��ѯ
    uint8_t macAddress : 1;   //��mac��ַ
    uint8_t voltage_1 : 1;    //��ѹ1
    uint8_t voltage_2 : 1;    //��ѹ2
    uint8_t pirTest : 1;      //PIR���ԣ��ޣ�
    uint8_t writeFlash : 1;   //дflash
    uint8_t readFlash : 1;    //��flash
#else
    uint8_t readFlash : 1;                       //��flash
    uint8_t writeFlash : 1;                      //дflash
    uint8_t pirTest : 1;                         //PIR���ԣ��ޣ�
    uint8_t voltage_2 : 1;                       //��ѹ2
    uint8_t voltage_1 : 1;                       //��ѹ1
    uint8_t macAddress : 1;                      //��mac��ַ
    uint8_t versionQuery : 1;                    //�汾��ѯ
    uint8_t nfcDetection : 1;                    //NFC���
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig1;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t ScreenTest : 1;         //��Ļ����
    uint8_t recordFinger : 1;       //¼ָ��
    uint8_t faceComm : 1;           //����ͨѶ
    uint8_t wifiSelfInspection : 1; //wifi�Լ�
    uint8_t R8SecretKeyIssued : 1;  //R8��Կ�·�
    uint8_t LiBattery : 1;          //����﮵��(3.0V-4.3V)��Ϊ����
    uint8_t emptyUser : 1;          //����û�
    uint8_t Q5MV9realTimeVideo : 1; //Q5M/V9ʵʱ��Ƶ
#else
    uint8_t Q5MV9realTimeVideo : 1;              //Q5M/V9ʵʱ��Ƶ
    uint8_t emptyUser : 1;                       //����û�
    uint8_t LiBattery : 1;                       //����﮵��(3.0V-4.3V)��Ϊ����
    uint8_t R8SecretKeyIssued : 1;               //R8��Կ�·�
    uint8_t wifiSelfInspection : 1;              //wifi�Լ�
    uint8_t faceComm : 1;                        //����ͨѶ
    uint8_t recordFinger : 1;                    //¼ָ��
    uint8_t ScreenTest : 1;                      //��Ļ����
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig2;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t huaweiSnWrite : 1;   //��ΪSN��д��
    uint8_t disconnect : 1;      //�Ͽ�
    uint8_t R8realTimeVideo : 1; //R8ʵʱ��Ƶ
    uint8_t wifiScanSignal : 1;  //WIFIģ�����ɨ��·���ź�ǿ�ȼ��
    uint8_t icTest : 1;          //IC����⣨0xEC��
    uint8_t autoLockModel : 1;   //ȫ�Զ������ͺ��·�
    uint8_t reserved : 2;
#else
    uint8_t reserved : 2;
    uint8_t autoLockModel : 1;   //ȫ�Զ������ͺ��·�
    uint8_t icTest : 1;          //IC����⣨0xEC��
    uint8_t wifiScanSignal : 1;  //WIFIģ�����ɨ��·���ź�ǿ�ȼ��
    uint8_t R8realTimeVideo : 1; //R8ʵʱ��Ƶ
    uint8_t disconnect : 1;      //�Ͽ�
    uint8_t huaweiSnWrite : 1;   //��ΪSN��д��
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig3;
typedef struct
{
    TYPEe_BleAck ack;
    uint8_t model[8];                              //�����ͺţ�8byte��
    uint8_t MAC[6];                                //��MAC(6byte)
    uint8_t channelPwd[4];                         //�ŵ�����(4byte)
    TYPEe_OpenEncryptionType openEncryptionType;   //���ż�������(1byte)
    TYPEe_IssuePwdAESAddPhone issuePwdAESAddPhone; //�·�����/AES�����ֻ��˺�(1byte)
    TYPEs_RegFingPram regFingPram;                 //¼ָ�Ʋ���(1byte)
    uint8_t wirelessModule;           				//����ģ��(1byte)
    uint8_t rootSecretKeySet;                      //����Կ���ã�1byte��
    TYPEs_UnlockType unlockType;                   //������ʽ���ͣ�2byte��
    uint8_t offlineFingerNum;                      //����ָ��������1byte��
    uint8_t onlineFingerNum;                       //����ָ��������1byte��
    TYPEs_AutoConfig autoConfig;                   //ȫ����������(2byte)
    uint8_t fingerRecordNum;                       //¼��ָ�ƴ�����1byte��
    TYPEs_LockFunctionConfig1 lockFunctionConfig1; //����������1��2byte��
    TYPEs_LockFunctionConfig2 lockFunctionConfig2; //����������2��2byte��
    TYPEe_LockVolumeSet volumeStatus;              //������״̬��1byte��
    uint8_t activeDefenseStatus;                   //��������״̬��1byte��
    TYPEs_OpenMode openModeStatus;                 //����ģʽ״̬��1byte��
    TYPEe_InfraredRangeStatus infraredRangeStatus; //�������״̬��1byte��
    TYPEs_ToolConfig1 toolConfig1;                 //С�ֹ�װ���ñ�1(һ��8byte)
    TYPEs_ToolConfig2 toolConfig2;                 //С�ֹ�װ���ñ�2
    TYPEs_ToolConfig3 toolConfig3;                 //С�ֹ�װ���ñ�3
    uint8_t toolConfig4;                           //С�ֹ�װ���ñ�4
    uint8_t toolConfig5;                           //С�ֹ�װ���ñ�5
    uint8_t toolConfig6;                           //С�ֹ�װ���ñ�6
    uint8_t toolConfig7;                           //С�ֹ�װ���ñ�7
    uint8_t toolConfig8;                           //С�ֹ�װ���ñ�8
	uint8_t CompositionOpen;					   //��Ͽ���
} TYPEs_BleQueryLockConfigTableAck;                //��ѯ�������ñ�

typedef struct
{
    uint8_t lockSN[15];    //�����кţ�15byte��
    uint8_t lockMAC[6];    //��MAC(6byte)
    uint8_t ChannelPwd[4]; //�ŵ�����(4byte)
} TYPEs_EquipmentRegAck;   //�豸ע��

typedef struct
{
    uint8_t phoneNum[13]; //�ֻ��˺�ID (13byte)
} TYPEs_AddPhoneAccount;  //�����ֻ��˺�

typedef struct
{
    TYPEs_TimeFormatHaveWeek time;
} TYPEs_TimeSync; //ʱ��ͬ��

typedef struct
{
    uint8_t PageID[4]; //ָ��PageID (4byte)
    uint8_t fingerCnt; //ָ�Ʋɼ�����(1byte)
} TYPEs_AddFingerAck;  //����ָ��

typedef struct
{
    TYPEs_UserPageId pageId;              //ָ��PageID(12byte)
    TYPEs_kindleAndSos kindleAndSos; //����/����(1byte)
    TYPEe_AgingStatus aging;         //ʱЧ��1byte��
    TYPEs_TimeFormat startTm;        //��ʼʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_TimeFormat agingTm;        //ʱЧʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_Week week;                 //��(1byte)
} TYPEs_BleFingerAttributeModify;    //ָ�������޸�
typedef struct
{
    uint16_t Id[13];              //ID(13byte)
    TYPEs_kindleAndSos kindleAndSos; //����/����(1byte)
    TYPEe_AgingStatus aging;         //ʱЧ��1byte��
    TYPEs_TimeFormat startTm;        //��ʼʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_TimeFormat agingTm;        //ʱЧʱ�䣨��+��+��+ʱ+��+�룩(6byte)
    TYPEs_Week week;                 //��(1byte)
} TYPEs_BleSmartKeyAttributeModify;    //�����޸�
typedef struct
{
    uint8_t pwd[8];      //��������
} TYPEs_BleUnlockPwdSet; //������������
typedef struct
{
    uint32_t cmd;      //����
} TYPEs_BleEmptyUser; //������������
typedef struct
{
    uint8_t dhG;               //ԭ��
    uint8_t dhP;               //����
    uint8_t reserved[6];       //Ԥ��
} TYPEs_BleLockPwdVerifyAdmin; //������������
typedef enum
{
    eBleLockPwdVerifyAdminSuccess = 0x00,  //�ɹ�
    eBleLockPwdVerifyAdminFail = 0x01,     //ʧ��
    eBleLockPwdVerifyAdminEmpty = 0x02,    //�����
    eBleLockPwdVerifyAdminNoFinger = 0x03, //��ʾ�û����������ӹ���ָ��
} TYPEe_BleLockPwdVerifyAdmin;
typedef struct
{
    TYPEe_BleLockPwdVerifyAdmin ack;
} TYPEs_BleLockPwdVerifyAdminACK; //�����������ûظ�

typedef struct
{
    uint8_t SSID[33];    //SSID(33byte)
    uint8_t wifiKey[65]; //wifi����(65byte)
} TYPEs_BleWifiSetParm;  //WIFI���ò���
typedef enum
{
    eBleWifiSetParmAckSuccess = 0x00, //�ɹ�
    eBleWifiSetParmAckFail = 0x01,    //ʧ��
    eBleWifiSetParmAckFailLow = 0x02, //ʧ�ܵ�ѹ��
} TYPEe_BleWifiSetParmAck;
typedef struct
{
    TYPEe_BleWifiSetParmAck ack;
} TYPEs_BleWifiSetParmAck;
typedef enum
{
    eBleUnlockUploadWifiSWSuccess = 0x00, //�ɹ�
    eBleUnlockUploadWifiSWFail = 0x01,    //ʧ��
} TYPEe_BleUnlockUploadWifiSW;
typedef struct
{
    TYPEe_BleUnlockUploadWifiSW sw;
} TYPEs_BleUnlockUploadWifiSW;
typedef enum
{
    eWifiFunctionSwitchSetOpen = 0x00,  //����
    eWifiFunctionSwitchSetClose = 0x11, //�ر�
} TYPEe_WifiFunctionSwitchSet;
typedef struct
{
    TYPEe_WifiFunctionSwitchSet sw;
} TYPEs_BleWifiFunctionSwitchSet;
typedef struct
{
    uint16_t ID;    //����ID
} TYPEs_FaceRegAck; //����ָ��


typedef enum
{
    eWifiSetRouter = 0xF6,         //����·����
    eWifiServerDomain = 0xF7,      //���÷���������
    eWifiOpenMsgUpload = 0x04,     //������Ϣ�ϴ�
    eWifiFaultMsgUpload = 0x07,    //������Ϣ�ϴ�
    eWifiImageUpload = 0x09,       //ͼƬ�ϴ�
    eWifiOTAUpdate = 0x10,         //OTA����
    eWifiChkOTA = 0x11,            //ģ��������ѯOTA����
    eWifiSetBPS = 0xFA,            //ͨѶ����������
    eWifiChkMAC = 0xFB,            //��ѯWIFIģ��MAC��ַ
    eWifiProductScanRouter = 0xFC, //����ɨ��·����
    eWifiChkSoft = 0xFD,           //����ɨ��·����
    eWifiChkPskPwd = 0xFE,         //��ѯ��ǰģ����PSK����
} TYPEe_Wifi;

typedef struct
{
    uint8_t SSID[ROUTER_SSID_SIZE]; //SSID
    uint8_t Pwd[ROUTER_PWD_SIZE];   //·��������
} TYPEs_WifiSetRouter;

typedef struct
{
    uint8_t domainNameServer[DOMAIN_NAME_SERVER_SIZE]; //������������
    uint16_t port;                                     //�˿�
} TYPEs_WifiServerDomain;

//typedef enum
//{
//    eFaceRegConfirmSuccess = 0x00, //�ɹ�
//    eFaceRegConfirmFail = 0x01,    //ʧ��
//} TYPEe_FaceRegConfirm;            //APPȷ������¼��ɹ�

typedef struct
{
    uint8_t sw;              
} TYPEs_BleID2ServerSwitch;   //����/����ID2���� (0x67)(APPģʽ���ܽ���)

typedef struct
{
    uint16_t A1;
	uint16_t A2;
} TYPEs_BleDHKeyExchangeAlgorithm; 

typedef struct
{
	TYPEe_BleAck ack;
    uint16_t A1;
	uint16_t A2;
} TYPEs_BleDHKeyExchangeAlgorithmAck;
typedef enum
{
    eCommBusyNo = 0x00,  //����
    eCommBusyYes = 0x01, //æµ
} TYPEe_CommBusy;        //ͨѶæµ���

typedef struct
{
    TYPEe_LockVolumeSet volumeStatus;              //������״̬
} TYPEs_BleLockVolumeSet;        

typedef enum
{
    eGyroscopeSetClose = 0x00,  //�ر�
    eGyroscopeSetOpen = 0x01, //��
} TYPEe_GyroscopeSet;       

typedef struct
{
    TYPEe_GyroscopeSet gyroscope;              
} TYPEs_BleGyroscopeSet;        //������

typedef struct
{
    TYPEe_BleAck ack;
	uint8_t wakeSource;
} TYPEs_BleWakeSourceQueryAck;        

typedef struct
{
    uint8_t pwd[16];
} TYPEs_BleAESUnlockPwdSet;

typedef struct
{
    uint8_t pwd[16];
} TYPEs_BleAESUnlockAlarmPwdSet; 
typedef struct
{
	TYPEe_BleAck ack;
    uint8_t pwd[64];
} TYPEs_BleAESTmpeSecretKeyGetAck;   

typedef struct
{
    uint8_t phoneID[16];
} TYPEs_BleAESAddPhoneAccount;    
#pragma pack() //ȡ������
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_COMM_GLOBALS
#define HAL_COMM_EXT
#else
#define HAL_COMM_EXT extern
#endif /*HAL_COMM_GLOBALS*/

    /*************************��������*************************/
    HAL_COMM_EXT TYPEs_CommMsgKeyStatus *const pCommMsgKeyStatus;
    HAL_COMM_EXT TYPEs_CommGetBatteryAck *const pCommGetBatteryAck;
    HAL_COMM_EXT TYPEs_CommWifiUpload *const pCommWifiUpload;
    HAL_COMM_EXT TYPEs_WifiSetRouter *const pWifiSetRouter;
    HAL_COMM_EXT TYPEs_WifiServerDomain *const pWifiServerDomain;
    HAL_COMM_EXT TYPEs_CommAckStatus *const pCommAckStatusSend;
    HAL_COMM_EXT TYPEs_CommAckStatus *const pCommAckStatusRcve;
    HAL_COMM_EXT TYPEs_CommMsgMotorLock *const pCommMsgMotorLock;
    HAL_COMM_EXT TYPEs_CommOpenAndInfoUpload *const pCommOpenAndInfoUpload;
    HAL_COMM_EXT TYPEs_CommWifiNetSet *const pCommWifiNetSet;
    HAL_COMM_EXT TYPEs_CommWifiPortSet *const pCommWifiPortSet;
    HAL_COMM_EXT TYPEs_CommWirelessMACAck *const pCommWirelessMACAck;
    HAL_COMM_EXT TYPEs_CommScanGateway *const pCommScanGateway;
    HAL_COMM_EXT TYPEs_CommMsgUpdateDeviceName *const pCommMsgUpdateDeviceName;
	HAL_COMM_EXT TYPEs_CommMsgEnterOTA *const pCommMsgEnterOTA;
	HAL_COMM_EXT TYPEs_CommMsgOpenBackupBat *const pCommMsgOpenBackupBat;
	HAL_COMM_EXT TYPEs_CommMsgDisconnectBLE *const pCommMsgDisconnectBLE;

    HAL_COMM_EXT TYPEs_BleAck *const pBleAck;
    HAL_COMM_EXT TYPEs_BleTimeSync *const pBleTimeSync;
	HAL_COMM_EXT TYPEs_BleDelFinger *const pBleDelFinger;
    HAL_COMM_EXT TYPEs_BleAddFingerAck *const pBleAddFingerAck;
    HAL_COMM_EXT TYPEs_BleAddPhoneAccount *const pBleAddPhoneAccount;
    HAL_COMM_EXT TYPEs_BleDelPhoneAccount *const pBleDelPhoneAccount;
    HAL_COMM_EXT TYPEs_BleFingerAttributeModify *const pBleFingerAttributeModify;
    HAL_COMM_EXT TYPEs_BleEquipmentRegAck *const pBleEquipmentRegAck;
    HAL_COMM_EXT TYPEs_BleWifiFunctionSwitchSet *const pBleWifiFunctionSwitchSet;
    HAL_COMM_EXT TYPEs_BleLockFirmwareVersionGetAck *const pBleLockFirmwareVersionGetAck;
    HAL_COMM_EXT TYPEs_BleAddFingerAck *const pBleNewAddFingerAck;
    HAL_COMM_EXT TYPEs_BleQueryLockConfigTable *const pBleQueryLockConfigTable;
    HAL_COMM_EXT TYPEs_BleQueryLockConfigTableAck *const pBleQueryLockConfigTableAck;
    HAL_COMM_EXT TYPEs_BleWifiSetParm *const pBleWifiSetParm1;
    HAL_COMM_EXT TYPEs_BleWifiSetParm *const pBleWifiSetParm2;
    HAL_COMM_EXT TYPEs_BleWifiSetParmAck *const pBleWifiSetParmAck;
    HAL_COMM_EXT TYPEs_BleUnlockUploadWifiSW *const pBleUnlockUploadWifiSW;
    HAL_COMM_EXT TYPEs_BleAddFaceAck *const pBleAddFaceAck;
    HAL_COMM_EXT TYPEs_BleFaceRegConfirm *const pBleFaceRegConfirm;
    HAL_COMM_EXT TYPEs_BleFaceDelId *const pBleFaceDelId;
    HAL_COMM_EXT TYPEs_BleMotorTorqueSet *const pBleMotorTorqueSet;
    HAL_COMM_EXT TYPEs_BleMotorDirSet *const pBleMotorDirSet;
    HAL_COMM_EXT TYPEs_BleAutomaticLockIntervalSet *const pBleAutomaticLockIntervalSet;
    HAL_COMM_EXT TYPEs_BleAKeyProtection *const pBleAKeyProtection;
    HAL_COMM_EXT TYPEs_BleActiveDefense *const pBleActiveDefense;
    HAL_COMM_EXT TYPEs_BleChannelPwdGetAck *const pBleChannelPwdGetAck;
	HAL_COMM_EXT TYPEs_BleEmptyUser *const pBleEmptyUser;
    HAL_COMM_EXT TYPEs_BleFaceVersionGetAck *const pBleFaceVersionGetAck;
    HAL_COMM_EXT TYPEs_BleFaceAttributeModify *const pBleFaceAttributeModify;
    HAL_COMM_EXT TYPEs_BleUnlockPwdSet *const pBleUnlockPwdSet;
    HAL_COMM_EXT TYPEs_BleLockPwdVerifyAdmin *const pBleLockPwdVerifyAdmin;
	HAL_COMM_EXT TYPEs_BleDoorOpeningModeSet *const pBleDoorOpeningModeSet;
    HAL_COMM_EXT TYPEs_BleLockPwdVerifyAdminACK *const pBleLockPwdVerifyAdminACK;
    HAL_COMM_EXT TYPEs_BlePhoneSecretKeyGetAck *const pBlePhoneSecretKeyGetAck;
	HAL_COMM_EXT TYPEs_BleID2ServerSwitch *const pBleID2ServerSwitch;
	HAL_COMM_EXT TYPEs_BleDHKeyExchangeAlgorithm *const pBleDHKeyExchangeAlgorithm;
	HAL_COMM_EXT TYPEs_BleDHKeyExchangeAlgorithmAck *const pBleDHKeyExchangeAlgorithmAck;
	HAL_COMM_EXT TYPEs_BleSmartKeySecretKeyGetRandom *const pBleSmartKeySecretKeyGetRandom;
	HAL_COMM_EXT TYPEs_BleSmartKeySecretKeyGet *const pBleSmartKeySecretKeyGet;
	HAL_COMM_EXT TYPEs_BleLockRegSecretKeyGet *const pBleLockRegSecretKeyGet;
	HAL_COMM_EXT TYPEs_BleGetEventRecords *const pBleGetEventRecords;
	HAL_COMM_EXT TYPEs_BleLockVolumeSet *const pBleLockVolumeSet;
	HAL_COMM_EXT TYPEs_BleGyroscopeSet *const pBleGyroscopeSet;
	HAL_COMM_EXT TYPEs_BleWakeSourceQueryAck *const pBleWakeSourceQueryAck;
	HAL_COMM_EXT TYPEs_BleInfraredMonitoringDistanceSet *const pBleInfraredMonitoringDistanceSet;
	HAL_COMM_EXT TYPEs_BleNFCSwitchSet *const pBleNFCSwitchSet;
	HAL_COMM_EXT TYPEs_BleDoorbellCapture *const pBleDoorbellCapture;
	HAL_COMM_EXT TYPEs_BleAutoLockConfig *const pBleAutoLockConfig;
	HAL_COMM_EXT TYPEs_BleAutoLockKeyConfig *const pBleAutoLockKeyConfig;
	HAL_COMM_EXT TYPEs_BleModifFaceSafetyLevel *const pBleModifFaceSafetyLevel;
	HAL_COMM_EXT TYPEs_BleToolAddModel *const pBleToolAddModel;
	HAL_COMM_EXT TYPEs_BleAddSmartKeyAccredit *const pBleAddSmartKeyAccredit;
	HAL_COMM_EXT TYPEs_BleDelSmartKey *const pBleDelSmartKey;
	HAL_COMM_EXT TYPEs_BleSmartKeyAttributeModify *const pBleSmartKeyAttributeModify;
	HAL_COMM_EXT TYPEs_BleAESUnlockPwdSet *const pBleAESUnlockPwdSet;
	HAL_COMM_EXT TYPEs_BleAESUnlockAlarmPwdSet *const pBleAESUnlockAlarmPwdSet;
	HAL_COMM_EXT TYPEs_BleAESTmpeSecretKeyGetAck *const pBleAESTmpeSecretKeyGetAck;
	HAL_COMM_EXT TYPEs_BleAESAddPhoneAccount *const pBleAESAddPhoneAccount;
	HAL_COMM_EXT TYPEs_BleToolTestRWLockModelAck *const pBleToolTestRWLockModelAck;
	HAL_COMM_EXT TYPEs_BleUnlockSmartKeyVerify *const pBleUnlockSmartKeyVerify;
	HAL_COMM_EXT TYPEs_BleFlashWriteTest *const pBleFlashWriteTest;
	HAL_COMM_EXT TYPEs_BleFlashRWCompAck *const pBleFlashRWCompAck;
	HAL_COMM_EXT TYPEs_BleVoltageTestAck *const pBleVoltageTestAck;
	HAL_COMM_EXT TYPEs_BleLockTimeGetAck *const pBleLockTimeGetAck;
	HAL_COMM_EXT TYPEs_BleSecondBatteryVoltageTestAck *const pBleSecondBatteryVoltageTestAck;
	HAL_COMM_EXT TYPEs_BleLockMacGet *const pBleLockMacGet;
	HAL_COMM_EXT TYPEs_BleLockMacGetAck *const pBleLockMacGetAck;
	HAL_COMM_EXT TYPEs_BleTmpeSecretKeyGetAck *const pBleTmpeSecretKeyGetAck;
	HAL_COMM_EXT TYPEs_BleAliyunSecretKeySetParm *const pBleAliyunSecretKeySetParm;
	HAL_COMM_EXT TYPEs_BleCatEyeSNAck *const pBleCatEyeSNAck;
	HAL_COMM_EXT TYPEs_BleShangTangCatEyeSingleDouble *const pBleShangTangCatEyeSingleDouble;
	HAL_COMM_EXT TYPEs_BleR8CatEyeOTA *const pBleR8CatEyeOTA;
	HAL_COMM_EXT TYPEs_BleR8CatEyeOTAAck *const pBleR8CatEyeOTAAck;
	HAL_COMM_EXT TYPEs_BleVerifyPhoneEncrypt *const pBleVerifyPhoneEncrypt;
	HAL_COMM_EXT TYPEs_BleVerifyPhoneEncryptAck *const pBleVerifyPhoneEncryptAck;
	HAL_COMM_EXT uint16_t *const pBleRxLen;
	HAL_COMM_EXT uint8_t *const pBleRxData;
	HAL_COMM_EXT uint8_t *const pBleTxData;
    HAL_COMM_EXT uint8_t *const pRxData;
    /*************************��������*************************/
    HAL_COMM_EXT void HAL_Comm_Init(void);
    HAL_COMM_EXT TYPEe_CommStatus HAL_Comm_SendDeal(void);
    HAL_COMM_EXT int8_t HAL_Comm_SendDealStart(TYPEe_Comm cmd, uint32_t sendWait);
    HAL_COMM_EXT void HAL_Comm_Send(TYPEe_Comm cmd);
    HAL_COMM_EXT void HAL_Comm_RevcDeal(void);
    HAL_COMM_EXT uint8_t *HAL_Comm_GetReadAddr(void);
    HAL_COMM_EXT TYPEe_Comm HAL_Comm_GetRecvMsgType(void);
    HAL_COMM_EXT TYPEe_Ble HAL_Comm_BleGetRecvMsgType(void);
    HAL_COMM_EXT void HAL_Comm_CealrRecv(void);
    HAL_COMM_EXT void HAL_Comm_CealrSend(void);
    HAL_COMM_EXT void HAL_COMM_EnterSleepConfig(void);
    HAL_COMM_EXT void HAL_Comm_AesKeySet(uint8_t *p);
    HAL_COMM_EXT void HAL_Comm_WakeupBack(void);
    HAL_COMM_EXT TYPEe_Wifi HAL_Comm_WifiGetRecvMsgType(uint16_t addrOffset);
	HAL_COMM_EXT void HAL_Comm_BleSendDir(TYPEe_Ble cmd);
	HAL_COMM_EXT void HAL_Comm_BleSendDirWithLen(TYPEe_Ble cmd,uint16_t len);
	HAL_COMM_EXT void HAL_Comm_BleSendStart(TYPEe_Ble cmd,uint32_t sendWait);
	HAL_COMM_EXT TYPEe_CommBusy HAL_Comm_ChkBusy(void);
	HAL_COMM_EXT void HAL_Comm_RstBack(void);
    HAL_COMM_EXT void HAL_Comm_InitBack(void);
    HAL_COMM_EXT void HAL_Comm_WifiRtSet(uint8_t sw);
    HAL_COMM_EXT uint8_t HAL_Comm_WifiCtRead(void);
    HAL_COMM_EXT uint8_t HAL_Comm_WifiCtInit(void);
    HAL_COMM_EXT int8_t HAL_Comm_WifiSend(uint8_t *sendBuf, uint32_t len);
    HAL_COMM_EXT int8_t HAL_Comm_EncryptionParam(uint8_t *mac, uint8_t *rootkey);
    HAL_COMM_EXT void HAL_Comm_SetEncrypt(uint8_t sw);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_COMM_MODULE_EN*/

#endif /*HAL_COMM_MODULE_EN*/
#endif /*__HAL_COMM_H__*/
/************************************Code End******************************************/
