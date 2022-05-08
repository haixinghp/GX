/***************************************************************************************
**文件名:     HAL_Comm.h
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件 
**备注:       需要注意字节和位域的大小端转换的问题
****************************************************************************************/
#ifndef __HAL_COMM_H__
#define __HAL_COMM_H__
#include "MyConfig.h"
#define HAL_COMM_MODULE_EN (1) //模块使能
#if HAL_COMM_MODULE_EN
/*************************专用宏定义*************************/

#define LOCKS_FIRMWARE_VERSION_SIZE (20)
#define MAC_SIZE (6)                 //MAC长度
#define ROUTER_SSID_SIZE (33)        //路由器SSID长度
#define ROUTER_PWD_SIZE (66)         //路由器密码长度
#define DOMAIN_NAME_SERVER_SIZE (30) //服务器主域名
#define HAL_COMM_SEND_WAIT_TIEM (20) //发送等待时间
// #define BLE_DATA_STAR_OFFSET (5)     //蓝牙数据偏移地址
/*************************变量结构声明*************************/
#pragma pack(1) //这里强制使用1字节对齐的方式
typedef enum
{
    eCommStatusNone,            //空闲
    eCommStatusSending,         //数据发送处理中
    eCommStatusWaitWakeup,      //等待接收方唤醒成功
    eCommStatusACKNeed,         //需要等待接收
    eCommStatusAckNot,          //需要不需要响应接收
    eCommStatusReceive,         //接收数据中
    eCommStatusReceiveOverTime, //接收超时
    eCommStatusReceiveError,    //接收错误
    eCommStatusReceiveSucceed,  //接收成功
} TYPEe_CommStatus;
typedef enum
{
    eCommWaitWakeupSleep, //接收方未唤醒
    eCommWaitWakeupWork,  //接收方已经被唤醒
} TYPEe_CommWaitWakeup;
typedef enum
{
    eCommNone = 0x00,                //空闲
    eCommMsgKeyStatus = 0x01,        //通知前板按键状态
    eCommMsgMotorLock = 0x20,        //通知后板电机上锁
    eCommOpenAndInfoUpload = 0x21,   //开门及信息上传
    eCommWifiUpload = 0x23,          //透传WIFI信息
    eCommGetBattery = 0x28,          //主动获取电池电量
    eCommMsgDisconnectBLE = 0x29,    //前板通知断开BLE
    eCommMsgUpdateDeviceName = 0x2A, //前板通知后板更新设备名称
	eCommMsgOpenBackupBat = 0x2B, 	 //前板通知后板打开备用电池供电
	eCommMsgEnterOTA = 0x2C, 		 //前板通知后板进入OTA模式
    eCommBleDataPass = 0x30,         //后面板BLE数据透传
    eCommwifiDataPass = 0x31,        //后面板WIFI数据透传
    eCommZigbeeExitNet = 0xF0,       //ZIGBEE模块退网
    eCommZigbeeEnterNet = 0xF1,      //ZIGBEE模块入网
    eCommWirelessMAC = 0xFB,         //查询无线模块MAC地址
    eCommScanGateway = 0XFC,         //产测扫描路由器/无线模块网关
} TYPEe_Comm;                        //通讯命令
typedef struct
{
    uint8_t year;           //年
    uint8_t month;          //月
    uint8_t day;            //日
    uint8_t week;           //周
    uint8_t hour;           //小时
    uint8_t minute;         //分钟
    uint8_t second;         //秒
} TYPEs_TimeFormatHaveWeek; //时间格式(包含周)
typedef struct
{
    uint8_t year;   //年
    uint8_t month;  //月
    uint8_t day;    //日
    uint8_t hour;   //小时
    uint8_t minute; //分钟
    uint8_t second; //秒
} TYPEs_TimeFormat; //时间格式（不包含周）
typedef enum
{
    eBackKeyStatusNone = 0x00,  //无操作
    eBackKeyStatusShort = 0x01, //短按
    eBackKeyStatusLong = 0x02,  //长按
} TYPEe_BackKeyStatus;
typedef enum
{
    eInsideHandNone = 0x00,  //无操作
    eInsideHandOpen = 0x01,  //开门
    eInsideHandClose = 0x02, //关门
} TYPEe_InsideHand;
typedef enum
{
    eBackBleNone = 0x00,   //无操作
    eBackBleWakeup = 0x01, //唤醒
} TYPEe_BackBle;
typedef enum
{
    eBackWifiNone = 0x00,   //无操作
    eBackWifiWakeup = 0x01, //唤醒
} TYPEe_BackWifi;
typedef enum
{
    eModuleInsertMSGNone = 0x00, //无操作
    eModuleInsertMSGHave = 0x01, //有模块插上
} TYPEe_ModuleInsertMSG;
typedef struct
{
    uint8_t MAC[6];                        //MAC
    uint8_t rootKey[6];                    //根密钥
    TYPEe_BackKeyStatus openKey;           //开门键按
    TYPEe_BackKeyStatus closeKey;          //关门键按
    TYPEe_InsideHand inside;               //后把手开门
    TYPEe_BackBle backBle;                 //后板蓝牙
    TYPEe_BackWifi backWifi;               //后板WIFI
    TYPEe_ModuleInsertMSG moduleInsertMSG; //模块插入提示
    uint16_t upBatBattery;                 //上方电池电量
    uint16_t downBatBattery;               //下方电池电量
    uint8_t version[9];                    //后板版本号
} TYPEs_CommMsgKeyStatus;

typedef enum
{
    eMsgModuleUnknown = 0x00,  //未知
    eMsgModuleNone = 0x01,     //无信息模块
    eMsgModuleWifi = 0x02,     //wifi模块
    eMsgModuleWifiLive = 0x03, //保活wifi模块
    eMsgModuleNB = 0x04,       //NB模块
    eMsgModuleZigbee = 0x05,   //zigbee模块
} TYPEe_MsgModule;             //信息模块
typedef struct
{
	uint8_t MAC[6];                        //MAC
    uint16_t upBatBattery;     //上方电池电量
    uint16_t downBatBattery;   //下方电池电量
    TYPEe_MsgModule msgModule; //信息模块
} TYPEs_CommGetBatteryAck;
typedef struct
{
    uint16_t up; 	//上方电池电量
    uint16_t down;   //下方电池电量
} TYPEs_BatteryStatus; //电池状态

typedef struct
{
    TYPEs_BatteryStatus battery; //电池状态
} TYPEs_CommMsgDisconnectBLE; 
typedef struct
{
	uint8_t sysStatus;	 //系统状态
	TYPEs_BatteryStatus battery; //电池状态
    uint16_t ovetTime; //超时时长
    uint8_t nameLen;   //设备名称长度
    uint8_t nameFirst; //名字第一个字符,处理时需要填写后续的
} TYPEs_CommMsgUpdateDeviceName;

typedef struct
{
    uint16_t ovetTime; //超时时长
	uint8_t frontOtaFlag; //前板OTA模式
	uint8_t backOtaFlag; //后板OTA模式
	uint8_t updataFlag; //是否更新名称
    uint8_t nameLen;   //设备名称长度
    uint8_t nameFirst; //名字第一个字符,处理时需要填写后续的
} TYPEs_CommMsgEnterOTA;


typedef struct
{
    uint16_t upBat; 	//上方电池电量
    uint16_t downBat;   //下方电池电量
} TYPEs_CommMsgOpenBackupBat; 
typedef enum
{
    eAckStatusFail = 0x33,          //失败
    eAckStatusSucceed = 0x55,       //成功
    eAckStatusLockFail = 0x66,      //开锁失败
    eAckStatusLockSucceed = 0x77,   //开锁成功
    eAckStatusUplistFail = 0x88,    //上传失败
    eAckStatusUplistSucceed = 0x99, //上传成功
    //    eAckStatusSetNetFail = 0xA0,    //网络异常
    //    eAckStatusSetNetSucceed = 0xA1, //配网成功
} TYPEe_AckStatus;
typedef struct
{
    TYPEe_AckStatus ack; //回复状态
} TYPEs_CommAckStatus;
//typedef struct
//{
//    uint8_t MAC[6];     //MAC
//    uint8_t version[9]; //后板版本号
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
    eOpenDirLeft = 'L',      //L左开
    eOpenDirrRight = 'R',    //右开
    eOpenDirrSemiAuto = 'N', //半自动
} TYPEe_OpenDir;
typedef enum
{
    eLockBodyType = 'A', //大扭力电机
} TYPEe_LockBodyType;
typedef enum
{
    eWirelessModuleWifi = 'W',   //WIFI模块
    eWirelessModuleZigbee = 'Z', //ZIGBEES模块
    eWirelessModuleNB = 'N',     //NB模块
} TYPEe_WirelessModule;

typedef enum
{
    eOpenDoorDirLeft = 0x11,  //L左开
    eOpenDoorDirRight = 0x22, //右开
} TYPEe_OpenDoorDir;
typedef struct
{
    uint8_t OEM;                         //OEM
    uint8_t outdoorLock;                 //门外上锁
    uint8_t indoorLock;                  //门内按键上锁
    TYPEe_OpenDoorDir openDoorDir;       //开门方向
    TYPEe_LockBodyType lockBodyType;     //锁体类型
    uint8_t motorTorque;                 //电机扭力
    uint16_t quantity;                   //电量
    TYPEe_WirelessModule wirelessModule; //无线模块
	TYPEs_BatteryStatus battery; //电池状态
} TYPEs_CommMsgMotorLock;

typedef enum
{
    eOpenTypeFinger = 0x01,    //指纹
    eOpenTypePwd = 0x02,       //密码
    eOpenTypeCard = 0x03,      //卡
    eOpenTypeFace = 0x04,      //人脸
    eOpenTypeVenous = 0x05,    //静脉
    eOpenTypeMechaKey = 0x06,  //机械钥匙
    eOpenTypeIris = 0x07,      //虹膜
    eOpenTypeRemote = 0x08,    //远程开锁
    eOpenTypeInsideKey = 0x09, //后面板按键开门
    eOpenTypeNone = 0xFF,      //不开门
} TYPEe_OpenType;
typedef enum
{
    eOpenAndInfoUploadEventAttributeClose = 0x00, //关门（默认值锁具不上报）
    eOpenAndInfoUploadEventAttributeOpen = 0x01,  //开门
} TYPEe_OpenAndInfoUploadEventAttribute;
typedef enum
{
    eUserAttributeAverageUser = 0x00, //普通用户
    eUserAttributeAdminUser = 0x01,   //管理员
    eUserAttributeTempUser = 0x02,    //临时用户
    eUserAttributeStressUser = 0x03,  //胁迫用户
    eUserAttributeAgingUser = 0x04,   //时效用户
} TYPEe_UserAttribute;
typedef enum
{
    eAutoLockNot = 0x00,  //不需要
    eAutoLockNeed = 0x01, //需要
} TYPEe_AutoLock;
typedef enum
{
    eWifiAttributeNone = 0x00,      //不推送
    eWifiAttributePushNoAck = 0x01, //推送不回复
    eWifiAttributePushAck = 0x02,   //推送成功后回复
    eWifiAttributeConfig = 0x03,    //配置
} TYPEe_WifiAttribute;
typedef struct
{
    uint8_t OEM;                       //OEM
    uint8_t MAC[6];                    //MAC
    TYPEe_OpenType openType;           //开门方式
    TYPEe_OpenDoorDir openDoorDir;     //开门方向
    TYPEe_LockBodyType lockBodyType;   //锁体类型
    uint8_t motorTorque;               //电机扭力
    TYPEe_AutoLock autoLock;           //是否自动上锁
    uint8_t autoLockTime;              //自动上锁时间
    uint16_t quantity;                 //电量
    TYPEe_WifiAttribute wifiAttribute; //WIFI属性
    uint16_t wifiDataLen;              //WIFI数据包长
	TYPEs_BatteryStatus battery; //电池状态
} TYPEs_CommOpenAndInfoUpload;
typedef enum
{
    eAlarmEventAttributeNone = 0x00,       //无告警（默认值锁具不上报）
    eAlarmEventAttributeLowPower = 0x01,   //低电量告警
    eAlarmEventAttributePry = 0x02,        //撬锁报警
    eAlarmEventAttributePwd = 0x03,        //密码报警
    eAlarmEventAttributeFinger = 0x04,     //指纹报警
    eAlarmEventAttributeCard = 0x05,       //门卡报警
    eAlarmEventAttributeWallet = 0x06,     //钱包报警
    eAlarmEventAttributeDoorUnlock = 0x07, //门未锁好报警
    eAlarmEventAttributeDeployed = 0x08,   //布防报警
} TYPEe_AlarmEventAttribute;

typedef struct
{
    uint8_t OEM;                              //OEM
    uint8_t MAC[6];                           //MAC
    TYPEe_AlarmEventAttribute eventAttribute; //事件属性
    uint16_t quantity;                        //电量
    TYPEe_WifiAttribute wifiAttribute;        //WIFI属性
    uint16_t wifiDataLen;                     //WIFI数据包长
	uint16_t upBat; 	//上方电池电量
    uint16_t downBat;   //下方电池电量
} TYPEs_CommWifiUpload;

typedef enum
{
    eFaultEventAttributeNone = 0x00,         //无故障（默认值锁具不上报）
    eFaultEventAttributeUnderVoltage = 0x01, //欠压故障
    eFaultEventAttributeFinger = 0x02,       //指纹故障
    eFaultEventAttributeTouch = 0x03,        //触摸屏故障
    eAlarmEventAttributeClock = 0x04,        //时钟故障
} TYPEe_FaultEventAttribute;
typedef enum
{
    eUserListFinger = 0x01,   //指纹
    eUserListPwd = 0x02,      //密码
    eUserListCard = 0x03,     //卡
    eUserListFace = 0x04,     //人脸
    eUserListVenous = 0x05,   //静脉
    eUserListMechaKey = 0x06, //机械钥匙
    eUserListIris = 0x07,     //虹膜
    eUserListRemote = 0x08,   //远程开锁
} TYPEe_UserList;
typedef enum
{
    eGetServerTimeAckSetFail = 0x33,     //设置失败
    eGetServerTimeAckDecryptFail = 0x44, //解密失败
    eGetServerTimeAckSuccess = 0x55,     //成功
    eGetServerTimeAckNetError = 0xA0,    //网络异常
    eGetServerTimeAckNetNormal = 0xA1,   //网络正常
} TYPEe_GetServerTimeAck;
typedef struct
{
    uint8_t openDir;           //开门方向
    uint8_t lockBodyType;      //锁体类型
    uint8_t motorTorque;       //电机扭力
    uint8_t automaticLockTime; //自动上锁时间
    uint16_t quantity;         //电量
    uint8_t wirelessModule;    //无线模块
} TYPEs_CommMsgBackOpen;
typedef struct
{
    uint16_t quantity; //电量
} TYPEs_CommWirelessExitNet;

typedef enum
{
    ePwdAttributeTmpPwdOnce = 0x02, //一次性密码（最大数量1组）
    ePwdAttributeAging = 0x04,      //时效密码
    ePwdAttributeRemote = 0x05,     //远程开门密码
} TYPEe_PwdAttribute;
typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //密码属性
    uint8_t Pwd[6];                  //密码数据
    uint32_t UTC;                    //UTC Time_4byte
} TYPEs_CommTmpPwdOnce;

typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //密码属性
    uint8_t Pwd[6];                  //密码数据
    uint32_t currentUTC;             //当前UTC
    uint32_t startUTC;               //开始UTC
    uint32_t endUTC;                 //结束UTC
} TYPEs_CommAgingPwd;
typedef struct
{
    uint8_t OEM;                     //OEM
    TYPEe_PwdAttribute PwdAttribute; //密码属性
    uint8_t Pwd[6];                  //密码数据
    uint32_t UTC;                    //UTC Time_4byte
} TYPEs_CommRemoteOpen;

typedef struct
{
    uint8_t SSID[ROUTER_SSID_SIZE]; //SSID
    uint8_t Pwd[ROUTER_PWD_SIZE];   //路由器密码
} TYPEs_CommWifiNetSet;

typedef struct
{
    uint8_t domainNameServer[DOMAIN_NAME_SERVER_SIZE]; //服务器主域名
    uint16_t port;                                     //端口
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
    eBleNone = 0x00,                            //空闲
    eBleVerifyChannelPwd = 0x01,                //验证信道密码
    eBleVerifyPhone = 0x03,                     //手机开锁验证
    eBleUnlockUpload = 0x04,                    //开锁实时上传
    eBleUnlockUploadEncrypt = 0x05,             //开锁实时加密上传 wifi
    eBleUnlockUploadEncryptWithVersion = 0x06,  //整合版本锁具信息实时加密上传 wifi
    eBleAlarmInforUpload = 0x07,                //报警信息上传
    eBleAlarmInforUploadEncrypt = 0x08,         //加密报警信息上传
    eBleLockStorageInforGet = 0x20,             //锁存储信息获取
    eBleUnlockRecordGet = 0x21,                 //开锁记录请求查询
    eBleUnlockModeSetting = 0x22,               //开锁方式设置
    eBleTimeSync = 0x23,                        //时间同步
    eBleAddFinger = 0x24,                       //增加指纹
    eBleAddPhoneAccount = 0x25,                 //增加手机账号
    eBleAddSmartKeyAccredit = 0x26,             //增加智能钥匙授权
    eBleAddSmartKey = 0x27,                     //增加智能钥匙
    eBleDelFinger = 0x28,                       //删除指纹
    eBleDelPhoneAccount = 0x29,                 //删除手机账号
    eBleDelSmartKey = 0x2A,                     //删除智能钥匙
    eBleFingerAttributeModify = 0x2B,           //指纹属性修改
    eBleSmartKeyAttributeModify = 0x2D,         //智能钥匙属性修改
    eBleGetMcuVersion = 0x2E,                   //获取外围MCU版本号
    eBleMcuEnterUpdataMode = 0x2F,              //通知外围控制芯片进入升级模式
    eBleUnlockPwdSet = 0x30,                    //开锁密码设置
    eBleUnlockPwdSetAdmin = 0x31,               //锁管理密码设置
    eBleFactory = 0x32,                         //恢复出厂设置
    eBleEquipmentReg = 0x34,                    //设备注册
    eBleEmptyUser = 0x35,                       //清空用户
    eBleLockPwdVerifyAdmin = 0x36,              //锁管理密码验证
    eBleLockFirmwareVersionGet = 0x37,          //锁固件版本获取
    eBleAlarmPwdSet = 0x38,                     //报警密码设置
    eBleVerifyPhoneEncrypt = 0x39,              //手机开锁验证加密版
    eBlePhoneSecretKeyGet = 0x3A,               //手机秘钥获取
    eBleUnlockSmartKeyVerify = 0x3B,            //智能钥匙开锁验证
    eBleSmartKeySecretKeyGet = 0X3C,            //智能钥匙开锁秘钥获取
    eBleSmartKeySecretKeyGetRandom = 0X3D,      //智能钥匙授权获取随机数秘钥
    eBleLockRegSecretKeyGet = 0x3E,             //锁注册秘钥获取
    eBleTmpeSecretKeyGet = 0X3F,                //临时秘钥生成
    eBleTestServerConnectionStatus = 0x40,      //测试服务器连接状态
    eBleWifiFunctionSwitchSet = 0x41,           //WIFI功能开启关闭设置
    eBleDisconnectBluetooth = 0xF0,             //断开蓝牙连接
    eBleWifiSetStart = 0xF2,                    //WIFI设置启动
    eBleWifiSetEnd = 0xf3,                      //WIFI设置结束
    eBleLockUpdateProgramMode = 0xF4,           //LOCK更新程序模式
    eBleWifiSetParm1 = 0xf5,                    //WIFI设置参数1
    eBleWifiSetParm2 = 0xf6,                    //WIFI设置参数2
    eBleWifiSetDomainNameParm1 = 0xf7,          //WIFI设置域名参数1
    eBleWifiSetDomainNameParm2 = 0xf8,          //WIFI设置域名参数2
    eBleUnlockUploadWifiSW = 0xF9,              //开锁记录实时上传功能设置 (默认功能是关闭)
    eBlePhoneBleSecretKeyGet = 0x50,            //手机操作BLE 秘钥获取指令
    eBleBleKeyHaveLockChk = 0x51,               //查询BLE钥匙下已有锁(注：容量最大为20)
    eBlePhoneDelBleKeyAllAccount = 0x52,        //手机删除BLE钥匙上全部账号
    eBleNewAddFinger = 0x53,                    //新APP协议增加指纹
    eBleNewConfirmFingerRegSucced = 0x54,       //新APP协议确认指纹录入成功
    eBleLockAddSmartKey = 0x55,                 //门禁增加智能钥匙
    eBleLockDelSmartKey = 0x56,                 //门禁删除智能钥匙
    eBleLocalFingerSync = 0x57,                 //本地指纹同步
    eBleLocalFingerDel = 0x58,                  //本地指纹删除
    eBleLocalFingerAttributeModif = 0x59,       //本地指纹属性修改
    eBleEncryptChipSEIDGet = 0x60,              //加密芯片SEID获取
    eBleEncryptChipChallengeGet = 0x61,         //加密芯片challenge获取
    eBleEncryptChipVerifyMac = 0x62,            //加密芯片verify mac请求
    eBleServerTokenVerify = 0x65,               //服务器token 验证请求
    eBleServerIssueChallenge = 0x66,            //服务器下发challenge
    eBleID2ServerSwitch = 0x67,                 //启用/禁用ID2服务
    eBleDHKeyExchangeAlgorithm = 0x70,          //DH密钥交换算法
    eBleLockVolumeSet = 0x71,                   //锁具音量设置
    eBleGyroscopeSet = 0x72,                    // 陀螺仪设置
    eBleWakeSourceQuery = 0x73,                 //唤醒源查询(调试用APP不需要增加)
    eBleDESUnlockPwdSet = 0x74,                 //DES开锁密码设置
    eBleDESUnlockAlarmPwdSet = 0x75,            // DES报警密码设置
    eBleDESTmpeSecretKeyGet = 0x76,             //DES临时秘钥生成
    eBleDESAddPhoneAccount = 0x77,              //DES增加手机账号
    eBleDESEncrytPhoneUnlockVerify = 0x78,      // DES手机开锁验证加密版
    eBleAESUnlockPwdSet = 0x79,                 //AES开锁密码设置
    eBleAESUnlockAlarmPwdSet = 0x7A,            // AES报警密码设置
    eBleAESTmpeSecretKeyGet = 0x7B,             //AES临时秘钥生成
    eBleAESAddPhoneAccount = 0x7C,              //AES增加手机账号
    eBleAESEncrytPhoneUnlockVerify = 0x7D,      // AES手机开锁验证加密版
    eBleBaiduSEEncryptActivationRequest = 0x7E, //百度SE加密激活请求
    eBleBaiduSEUnlockPwdSet = 0x7F,             //BaiduSE开锁密码设置
    eBleBaiduSEUnlockAlarmPwdSet = 0x80,        // BaiduSE报警密码设置
    eBleBaiduSETmpeSecretKeyGet = 0x81,         //BaiduSE临时秘钥生成
    eBleBaiduSEAddPhoneAccount = 0x82,          //BaiduSE增加手机账号
    eBleBaiduSEEncrytPhoneUnlockVerify = 0x83,  //BaiduSE手机开锁验证加密版
    eBleFaceReg = 0X85,                         //APP人脸录入
    eBleFaceRegConfirm = 0x86,                  //APP确认人脸录入成功
    eBleFaceDelId = 0x87,                       //APP删除人脸ID
    eBleInfraredMonitoringDistanceSet = 0x88,   //红外监测距离设置
    eBleDoorOpeningModeSet = 0x89,              //开门方式设置
    eBlePhoneDelBleKey = 0x90,                  //手机删除BLE钥匙
    eBleMotorTorqueSet = 0x91,                  //电机扭力调节
    eBleMotorDirSet = 0x92,                     //电机方向设置
    eBleAutomaticLockIntervalSet = 0x93,        //自动关门上锁时间间隔设置
    eBleQueryLockConfigTable = 0x94,            //查询锁具配置表
    eBleAKeyProtection = 0x95,                  //一键布防
    eBleActiveDefense = 0x96,                   //主动防御
    eBleChannelPwdGet = 0x97,                   //获取信道密码
    eBleNFCSwitchSet = 0x98,                    //NFC开关设置
    eBleDoorbellCapture = 0x99,                 //门铃抓拍
    eBleFaceVersionGet = 0x9A,                  //人脸模块版本号获取
    eBleGetEventRecords = 0x9B,                 //获取事件记录
    eBlefaceEnterFirmwareUpdateMode = 0x9C,     //人脸进入固件更新模式
    eBleFaceAttributeModify = 0X9D,             //人脸属性修改
    eBleBleSinglePackageMaxlength = 0x9E,       //获取蓝牙单包可以发送的最大长度
    eBleDHKeyExchangeAlgorithm1 = 0x9F,         //DH密钥交换算法
    eBleCatEyeSN = 0xA3,                        //获取猫眼序列号
    eBleShangTangCatEyeSingleDouble = 0xA4,     //商汤猫眼单双向切换功能
    eBleAddICCard = 0xA5,                       //app增加IC卡请求
    eBleDelICCard = 0xA6,                       //app删除IC卡请求
    eBleAutoLockConfig = 0xA7,                  //全自动锁体配置
	eBleAutoLockKeyConfig = 0xA8,                  //全自动锁门确认键配置
    eBleToolAddModel = 0XD5,                    //蓝牙工装添加锁具型号
    eBleToolTestRWLockModel = 0xD6,             //蓝牙工装测试读出与写入锁具型号
    eBleToolTestFail = 0XD9,                    //工装测试不合格
    eBleFlashWriteTest = 0xDB,                  //FLASH写入测试
    eBleFlashRWComp = 0xDC,                     //FLASH读出与写入对比
    eBleLedOledTest = 0xDD,                     //LED/OLED测试
    eBleVoltageTest = 0xDE,                     //电压测试
    eBleLockTimeGet = 0xDF,                     //读取锁具时间
    eBleLockAgingRecord = 0xB0,                 //查询锁具老化记录
    eBleCatEyeTestParm = 0xE0,                  //设置猫眼产测参数
    eBleWifiToolTest = 0xD2,                    //WIFI工装检测
    eBleXiongmaiCatEyeIPGet = 0xE1,             //雄迈猫眼 IP查询
    eBleSecondBatteryVoltageTest = 0xE2,        //下方电池（第二路）电压测试
    eBleLockMacGet = 0xE3,                      //锁MAC查询
    eBleCatEyePIRToolTest = 0xE4,               //猫眼PIR工装检测
    eBleAliyunSecretKeySetParm = 0xE5,          //阿里云秘钥设置参数
    eBleR8CatEyeOTA = 0XE7,                     //R8 猫眼OTA
    eBleR8CatEyeDebugModeEn = 0xE8,             //R8 猫眼DEBUG模式使能
    eBleR8CatEyeDebugDataTransfer = 0xE9,       //R8 猫眼DEBUG数据传输
    eBleR8EventLog = 0XEA,                      //R8 WIFI事件记录
    eBleWifiTestDR = 0xEB,                      //WIFI产测扫描指定路由器
    eBleToolTestIC = 0xEC,                      //厂测工装检测IC卡
    eBleModifFaceSafetyLevel = 0xED,            //修改人脸识别安全等级
} TYPEe_Ble;                                    //通讯命令
typedef enum
{
    eBleAckSuccess = 0x00, //成功
    eBleAckFail = 0x01,    //失败
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
    eBleAddFingerAckSuccess = 0x00,   //成功
    eBleAddFingerAckFail = 0x01,      //失败
    eBleAddFingerAckIng = 0x02,       //添加中
    eBleAddFingerAckAppNotReg = 0x04, //APP未注册
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
    eBleAddFaceAckSuccess = 0x00,   //成功
    eBleAddFaceAckFail = 0x01,      //失败
    eBleAddFaceAckIng = 0x02,       //添加中
    eBleAddFaceAckAppNotReg = 0x04, //APP未注册
} TYPEe_BleAddFaceAck;
typedef struct
{
    uint32_t id;
    uint8_t cnt;
} TYPEs_BleAddFaceSuccess;
typedef enum
{
    eBleAddFaceDirEnter = 0x00,
    eBleAddFaceDirMiddle = 0x01, //正脸
    eBleAddFaceDirUP = 0x02,     //抬头
    eBleAddFaceDirRight = 0x03,  //左脸
    eBleAddFaceDirDown = 0x04,   //低头
    eBleAddFaceDirLeft = 0x05,   //右脸
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
    eBleFaceRegResultSuccess = 0x00, //存储成功
    eBleFaceRegResultFail = 0x01,    //储存失败
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
    eInfraredDistanceLong = 0x0000, //远距离
    eInfraredDistanceShort = 0x0001, //远距离
    eInfraredDistanceClose = 0x0002, //关闭
} TYPEe_InfraredDistance;
typedef struct
{
   uint16_t distance;
} TYPEs_BleInfraredMonitoringDistanceSet;


typedef struct
{
	uint8_t mode; //解锁模式
	uint8_t unlockType; //解锁类型
} TYPEs_BleDoorOpeningModeSet;

typedef enum
{
    eBleMotorTorqueDefault = 0x00, //默认，低扭力
    eBleMotorTorqueLow = 0x01,     //2.7A，低扭力
    eBleMotorTorqueHigh = 0x02,    //3.7A,高扭力
} TYPEe_BleMotorTorque;
typedef struct
{
    TYPEe_BleMotorTorque torque;
} TYPEs_BleMotorTorqueSet;

typedef enum
{
    eBleMotorDirRight = 0x55, //默认，右开
    eBleMotorDirLife = 0x01,  //左开
} TYPEe_BleMotorDir;
typedef struct
{
    TYPEe_BleMotorDir dir;
} TYPEs_BleMotorDirSet;
typedef struct
{
    uint8_t closeTime;  //0,表示关闭，其他开启
} TYPEs_BleAutomaticLockIntervalSet;
typedef enum
{
    eBleAKeySwClose = 0, //关闭（默认）
    eBleAKeySwOpen = 1,  //打开
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
    uint8_t Mon : 1;  //周一
    uint8_t Tues : 1; //周二
    uint8_t Wed : 1;  //周三
    uint8_t Thur : 1; //周四
    uint8_t Fri : 1;  //周五
    uint8_t Sat : 1;  //周六
    uint8_t Sun : 1;  //周日
#else
    uint8_t Sun : 1;  //周日
    uint8_t Sat : 1;  //周六
    uint8_t Fri : 1;  //周五
    uint8_t Thur : 1; //周四
    uint8_t Wed : 1;  //周三
    uint8_t Tues : 1; //周二
    uint8_t Mon : 1;  //周一
    uint8_t reserved : 1;
#endif // USE_LITTLE_ENDIAN
} TYPEs_Week;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t SOS : 4;    //紧急
    uint8_t kindle : 4; //亲情
#else
    uint8_t kindle : 4; //亲情
    uint8_t SOS : 4;    //紧急
#endif // USE_LITTLE_ENDIAN

} TYPEs_kindleAndSos;
typedef enum
{
    eAgingStatusOn = 0x0F,  //开启
    eAgingStatusOff = 0x00, //关闭
} TYPEe_AgingStatus;
typedef struct
{
    uint16_t pageId;              //指纹PageID(12byte)
    TYPEs_kindleAndSos kindleAndSos; //亲情/紧急(1byte)
    TYPEe_AgingStatus aging;         //时效（1byte）
    TYPEs_TimeFormat startTm;        //起始时间（年+月+日+时+分+秒）(6byte)
    TYPEs_TimeFormat agingTm;        //时效时间（年+月+日+时+分+秒）(6byte)
    TYPEs_Week week;                 //周(1byte)
} TYPEs_BleFaceAttributeModify;      //属性修改

typedef struct
{
	TYPEe_BleAck ack; 
    uint8_t sn[20]; //雄迈数据包：16Byte 序列号 商汤数据包：20Byte 序列号
} TYPEs_BleCatEyeSNAck;  

typedef struct
{
	uint8_t sw; //0x00双向模式 、0x01单向模式，锁具注册默认00
} TYPEs_BleShangTangCatEyeSingleDouble; 
typedef struct
{
    uint8_t lockType; //1	默认，六浮锁体 2	开元乐锁体 3	九绑锁体
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
	uint8_t biopsy; //活检安全等级
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
	uint8_t channelPwd[4]; //信道密码(4byte)
	uint8_t phone[13]; //手机属性(账号ID 13byte)
	TYPEs_TimeFormatHaveWeek time; //服务器时间(时间格式(年,月,日,周,小时,分钟,秒)(7byte))
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
	uint16_t bat; //电池电量
} TYPEs_BleVerifyPhoneEncryptAck;   

typedef struct
{
	uint8_t channelPwd[4]; //信道密码(4byte)
	uint8_t phone[13]; //手机属性(账号ID 13byte)
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
    eManufacturerDSM = 0x00,       //德施曼
    eManufacturerKLN = 0x01,       //科徕尼
    eManufacturerJKD = 0x02,       //金凯德
    eManufacturerPanasonic = 0x03, //松下
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
	eOpenEncryptionTypeOld = 0x00,  //老协议
    eOpenEncryptionTypeDH = 0x01,  //DH加密
    eOpenEncryptionTypeID2 = 0x02, //ID2加密
    eOpenEncryptionTypeDES = 0X03, //DES
    eOpenEncryptionTypeAES = 0X04, //AES-128 bits/ECB/NoPadding 算法对称加密
    eOpenEncryptionTypeSE = 0X05,  //百度SE加密
} TYPEe_OpenEncryptionType;
typedef enum
{
    eIssuePwdAESAddPhoneOld = 0x00, //老协议（明文下发）
    eIssuePwdAESAddPhoneDES = 0x01, //DES(加密流程)
    eIssuePwdAESAddPhoneAES = 0x02, //AES128 bits/ECB/NoPadding 算法对称加密
    eIssuePwdAESAddPhoneSE = 0x03,  //百度SE加密
} TYPEe_IssuePwdAESAddPhone;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t xPlatform : 1;   //X平台（不支持离线指纹）
    uint8_t yPlatform : 1;   //Y平台（支持离线指纹）
    uint8_t newprotocol : 1; //新协议（0x53+0x54）
    uint8_t oldprotocol : 1; //老协议（0x24）
    uint8_t reserved : 4;    //预留
#else
    uint8_t reserved : 4;    //预留
    uint8_t oldprotocol : 1; //老协议（0x24）
    uint8_t newprotocol : 1; //新协议（0x53+0x54）
    uint8_t yPlatform : 1;   //Y平台（支持离线指纹）
    uint8_t xPlatform : 1;   //X平台（不支持离线指纹）
#endif // USE_LITTLE_ENDIAN
} TYPEs_RegFingPram;
//typedef enum
//{
//    eWirelessModuleWifi = 0x00, //WIFI模块
//    eWirelessModuleNB = 0x01,   //NB模块
//} TYPEe_WirelessModule;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t reserved;
    uint8_t pwd : 1;                  //密码解锁
    uint8_t finger : 1;               //指纹识别解锁
    uint8_t smartKey : 1;             //智能钥匙解锁（蓝牙钥匙/蓝牙手环）
    uint8_t face : 1;                 //人脸识别解锁
    uint8_t ICCard : 1;               //IC卡解锁
    uint8_t fingerVein : 1;           //指静脉解锁
    uint8_t electroMechanicalKey : 1; //电子机械钥匙解锁
    uint8_t Huawei : 1;               //华为钱包开锁
#else
    uint8_t reserved;
    uint8_t Huawei : 1;               //华为钱包开锁
    uint8_t electroMechanicalKey : 1; //电子机械钥匙解锁
    uint8_t fingerVein : 1;           //指静脉解锁
    uint8_t ICCard : 1;               //IC卡解锁
    uint8_t face : 1;                 //人脸识别解锁
    uint8_t smartKey : 1;             //智能钥匙解锁（蓝牙钥匙/蓝牙手环）
    uint8_t finger : 1;               //指纹识别解锁
    uint8_t pwd : 1;                  //密码解锁
#endif // USE_LITTLE_ENDIAN
} TYPEs_UnlockType;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t enSpeed : 1;           //陀螺仪模块
    uint8_t enOpenDirSet : 1;      //开门方向调整（左右）
    uint8_t enMotorTorqueSet : 1;  //电机扭力调节
    uint8_t enAutoLockTimeSet : 1; //自动上锁时间调节
	uint8_t enAutoLockAffirm : 1; 	//全自动锁门确认功能 
    uint8_t reserved : 3;
    uint8_t swSpeed : 1;          //陀螺仪模块0=关闭
    uint8_t swOpenDirSet : 1;     //开门方向调整（左右）0=右开
    uint8_t swMotorTorqueSet : 1; //电机扭力调节0=低扭力
    uint8_t swAutoLockTime : 5;   //自动上锁时间调节(0-32)*5秒
#else
    uint8_t swAutoLockTime : 5;       //自动上锁时间调节(0-32)*5秒
    uint8_t swMotorTorqueSet : 1;     //电机扭力调节0=低扭力
    uint8_t swOpenDirSet : 1;         //开门方向调整（左右）0=右开
    uint8_t swSpeed : 1;              //陀螺仪模块0=关闭
    uint8_t reserved : 4;
    uint8_t enAutoLockTimeSet : 1;               //自动上锁时间调节
    uint8_t enMotorTorqueSet : 1;                //电机扭力调节
    uint8_t enOpenDirSet : 1;                    //开门方向调整（左右）
    uint8_t enSpeed : 1;                         //陀螺仪模块
#endif // USE_LITTLE_ENDIAN
} TYPEs_AutoConfig;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t enAKeyProtection : 1;                //一键布防
    uint8_t enLockVolumeSet : 1;                 //音量调节
    uint8_t enActiveDefense : 1;                 //主动防御（红外30秒检测）
    uint8_t enNFC : 1;                           //NFC功能
    uint8_t enOpenModeSet : 1;                   //开门模式可选
    uint8_t enDoorbellCapture : 1;               //门铃抓拍
    uint8_t enInfraredMonitoringDistanceSet : 1; //红外距离
    uint8_t enEventLog : 1;                      //事件记录

    uint8_t swAKeyProtection : 1;                //一键布防
    uint8_t swLockVolumeSet : 1;                 //音量调节
    uint8_t swActiveDefense : 1;                 //主动防御（红外30秒检测）
    uint8_t swNFC : 1;                           //NFC功能
    uint8_t swOpenModeSet : 1;                   //开门模式可选
    uint8_t swDoorbellCapture : 1;               //门铃抓拍
    uint8_t swInfraredMonitoringDistanceSet : 1; //红外距离
    uint8_t swEventLog : 1;                      //事件记录
#else
    uint8_t swEventLog : 1;                      //事件记录
    uint8_t swInfraredMonitoringDistanceSet : 1; //红外距离
    uint8_t swDoorbellCapture : 1;               //门铃抓拍
    uint8_t swOpenModeSet : 1;                   //开门模式可选
    uint8_t swNFC : 1;                           //NFC功能
    uint8_t swActiveDefense : 1;                 //主动防御（红外30秒检测）
    uint8_t swLockVolumeSet : 1;                 //音量调节
    uint8_t swAKeyProtection : 1;                //一键布防
    uint8_t enEventLog : 1;                      //事件记录
    uint8_t enInfraredMonitoringDistanceSet : 1; //红外距离
    uint8_t enDoorbellCapture : 1;               //门铃抓拍
    uint8_t enOpenModeSet : 1;                   //开门模式可选
    uint8_t enNFC : 1;                           //NFC功能
    uint8_t enActiveDefense : 1;                 //主动防御（红外30秒检测）
    uint8_t enLockVolumeSet : 1;                 //音量调节
    uint8_t enAKeyProtection : 1;                //一键布防
#endif // USE_LITTLE_ENDIAN
} TYPEs_LockFunctionConfig1;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t xiaoDiOemNewRing : 1;                //小嘀OEM新手环
    uint8_t catSEye : 1;                        //猫眼
    uint8_t shangTangCatSEye : 1;                //商汤猫眼
    uint8_t aoBiCatSEye : 1;                     //奥比猫眼
    uint8_t icCard : 1;                          //IC卡
    uint8_t R8InfraredMonitoringDistanceSet : 1; //R8红外距离可调节（远、近，关闭）
    uint8_t NewRemote : 1;                       //新遥控器
    uint8_t reserved : 1;

    uint8_t reservedB : 1; 
	uint8_t McuGX: 1;
    uint8_t shangTangCatSEyesingleAndDouble : 1; //商汤猫眼单双向
    uint8_t reservedA : 5;
#else
    uint8_t reservedA : 5;
    uint8_t shangTangCatSEyesingleAndDouble : 1; //商汤猫眼单双向
    uint8_t reservedB : 2;
    uint8_t reserved : 1;
    uint8_t NewRemote : 1;                       //新遥控器
    uint8_t R8InfraredMonitoringDistanceSet : 1; //R8红外距离可调节（远、近，关闭）
    uint8_t icCard : 1;                          //IC卡
    uint8_t aoBiCatSEye : 1;                     //奥比猫眼
    uint8_t shangTangCatSEye : 1;                //商汤猫眼
    uint8_t catSEye : 1;                         //猫眼
    uint8_t xiaoDiOemNewRing : 1;                //小嘀OEM新手环
#endif // USE_LITTLE_ENDIAN
} TYPEs_LockFunctionConfig2;
typedef enum
{
    eLockVolumeSetMute = 0x00,   //静音
    eLockVolumeSetLow = 0x01,    //低
    eLockVolumeSetMiddle = 0x02, //中
    eLockVolumeSetHigh = 0x03,   //高
} TYPEe_LockVolumeSet;           //锁具音量设置
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t openModePwd : 1;          //密码解锁
    uint8_t openModeFinger : 1;       //指纹识别解锁
    uint8_t openModeSmartKey : 1;     //智能钥匙解锁（蓝牙钥匙/蓝牙手环）
    uint8_t openModeFace : 1;         //人脸识别解锁
    uint8_t openModeIcCard : 1;       //IC卡解锁
    uint8_t openModeFingerVein : 1;   //指静脉解锁
    uint8_t openModeElecMechaKey : 1; //电子机械钥匙解锁
    uint8_t openModeAndOr : 1;        //1代表与模式0代表或模式
#else
    uint8_t openModeAndOr : 1;                   //1代表与模式0代表或模式
    uint8_t openModeElecMechaKey : 1;            //电子机械钥匙解锁
    uint8_t openModeFingerVein : 1;              //指静脉解锁
    uint8_t openModeIcCard : 1;                  //IC卡解锁
    uint8_t openModeFace : 1;                    //人脸识别解锁
    uint8_t openModeSmartKey : 1;                //智能钥匙解锁（蓝牙钥匙/蓝牙手环）
    uint8_t openModeFinger : 1;                  //指纹识别解锁
    uint8_t openModePwd : 1;                     //密码解锁
#endif            // USE_LITTLE_ENDIAN
} TYPEs_OpenMode; //开门模式状态
typedef enum
{
    eInfraredRangeStatusFar = 0x0000,    //远距离
    eInfraredRangeStatusNearly = 0x0001, //近距离
    eInfraredRangeStatusClose = 0x0002,  //关闭
} TYPEe_InfraredRangeStatus;             //红外距离状态
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t nfcDetection : 1; //NFC检测
    uint8_t versionQuery : 1; //版本查询
    uint8_t macAddress : 1;   //读mac地址
    uint8_t voltage_1 : 1;    //电压1
    uint8_t voltage_2 : 1;    //电压2
    uint8_t pirTest : 1;      //PIR测试（无）
    uint8_t writeFlash : 1;   //写flash
    uint8_t readFlash : 1;    //读flash
#else
    uint8_t readFlash : 1;                       //读flash
    uint8_t writeFlash : 1;                      //写flash
    uint8_t pirTest : 1;                         //PIR测试（无）
    uint8_t voltage_2 : 1;                       //电压2
    uint8_t voltage_1 : 1;                       //电压1
    uint8_t macAddress : 1;                      //读mac地址
    uint8_t versionQuery : 1;                    //版本查询
    uint8_t nfcDetection : 1;                    //NFC检测
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig1;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t ScreenTest : 1;         //屏幕测试
    uint8_t recordFinger : 1;       //录指纹
    uint8_t faceComm : 1;           //人脸通讯
    uint8_t wifiSelfInspection : 1; //wifi自检
    uint8_t R8SecretKeyIssued : 1;  //R8秘钥下发
    uint8_t LiBattery : 1;          //内置锂电池(3.0V-4.3V)都为正常
    uint8_t emptyUser : 1;          //清空用户
    uint8_t Q5MV9realTimeVideo : 1; //Q5M/V9实时视频
#else
    uint8_t Q5MV9realTimeVideo : 1;              //Q5M/V9实时视频
    uint8_t emptyUser : 1;                       //清空用户
    uint8_t LiBattery : 1;                       //内置锂电池(3.0V-4.3V)都为正常
    uint8_t R8SecretKeyIssued : 1;               //R8秘钥下发
    uint8_t wifiSelfInspection : 1;              //wifi自检
    uint8_t faceComm : 1;                        //人脸通讯
    uint8_t recordFinger : 1;                    //录指纹
    uint8_t ScreenTest : 1;                      //屏幕测试
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig2;
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t huaweiSnWrite : 1;   //华为SN码写入
    uint8_t disconnect : 1;      //断开
    uint8_t R8realTimeVideo : 1; //R8实时视频
    uint8_t wifiScanSignal : 1;  //WIFI模块产测扫描路由信号强度检测
    uint8_t icTest : 1;          //IC卡检测（0xEC）
    uint8_t autoLockModel : 1;   //全自动锁体型号下发
    uint8_t reserved : 2;
#else
    uint8_t reserved : 2;
    uint8_t autoLockModel : 1;   //全自动锁体型号下发
    uint8_t icTest : 1;          //IC卡检测（0xEC）
    uint8_t wifiScanSignal : 1;  //WIFI模块产测扫描路由信号强度检测
    uint8_t R8realTimeVideo : 1; //R8实时视频
    uint8_t disconnect : 1;      //断开
    uint8_t huaweiSnWrite : 1;   //华为SN码写入
#endif // USE_LITTLE_ENDIAN
} TYPEs_ToolConfig3;
typedef struct
{
    TYPEe_BleAck ack;
    uint8_t model[8];                              //锁具型号（8byte）
    uint8_t MAC[6];                                //锁MAC(6byte)
    uint8_t channelPwd[4];                         //信道密码(4byte)
    TYPEe_OpenEncryptionType openEncryptionType;   //开门加密类型(1byte)
    TYPEe_IssuePwdAESAddPhone issuePwdAESAddPhone; //下发密码/AES增加手机账号(1byte)
    TYPEs_RegFingPram regFingPram;                 //录指纹参数(1byte)
    uint8_t wirelessModule;           				//无线模块(1byte)
    uint8_t rootSecretKeySet;                      //根秘钥设置（1byte）
    TYPEs_UnlockType unlockType;                   //解锁方式类型（2byte）
    uint8_t offlineFingerNum;                      //离线指纹容量（1byte）
    uint8_t onlineFingerNum;                       //在线指纹容量（1byte）
    TYPEs_AutoConfig autoConfig;                   //全动自锁配置(2byte)
    uint8_t fingerRecordNum;                       //录入指纹次数（1byte）
    TYPEs_LockFunctionConfig1 lockFunctionConfig1; //锁功能配置1（2byte）
    TYPEs_LockFunctionConfig2 lockFunctionConfig2; //锁功能配置2（2byte）
    TYPEe_LockVolumeSet volumeStatus;              //锁音量状态（1byte）
    uint8_t activeDefenseStatus;                   //主动防御状态（1byte）
    TYPEs_OpenMode openModeStatus;                 //开门模式状态（1byte）
    TYPEe_InfraredRangeStatus infraredRangeStatus; //红外距离状态（1byte）
    TYPEs_ToolConfig1 toolConfig1;                 //小嘀工装配置表1(一共8byte)
    TYPEs_ToolConfig2 toolConfig2;                 //小嘀工装配置表2
    TYPEs_ToolConfig3 toolConfig3;                 //小嘀工装配置表3
    uint8_t toolConfig4;                           //小嘀工装配置表4
    uint8_t toolConfig5;                           //小嘀工装配置表5
    uint8_t toolConfig6;                           //小嘀工装配置表6
    uint8_t toolConfig7;                           //小嘀工装配置表7
    uint8_t toolConfig8;                           //小嘀工装配置表8
	uint8_t CompositionOpen;					   //组合开门
} TYPEs_BleQueryLockConfigTableAck;                //查询锁具配置表

typedef struct
{
    uint8_t lockSN[15];    //锁序列号（15byte）
    uint8_t lockMAC[6];    //锁MAC(6byte)
    uint8_t ChannelPwd[4]; //信道密码(4byte)
} TYPEs_EquipmentRegAck;   //设备注册

typedef struct
{
    uint8_t phoneNum[13]; //手机账号ID (13byte)
} TYPEs_AddPhoneAccount;  //增加手机账号

typedef struct
{
    TYPEs_TimeFormatHaveWeek time;
} TYPEs_TimeSync; //时间同步

typedef struct
{
    uint8_t PageID[4]; //指纹PageID (4byte)
    uint8_t fingerCnt; //指纹采集次数(1byte)
} TYPEs_AddFingerAck;  //增加指纹

typedef struct
{
    TYPEs_UserPageId pageId;              //指纹PageID(12byte)
    TYPEs_kindleAndSos kindleAndSos; //亲情/紧急(1byte)
    TYPEe_AgingStatus aging;         //时效（1byte）
    TYPEs_TimeFormat startTm;        //起始时间（年+月+日+时+分+秒）(6byte)
    TYPEs_TimeFormat agingTm;        //时效时间（年+月+日+时+分+秒）(6byte)
    TYPEs_Week week;                 //周(1byte)
} TYPEs_BleFingerAttributeModify;    //指纹属性修改
typedef struct
{
    uint16_t Id[13];              //ID(13byte)
    TYPEs_kindleAndSos kindleAndSos; //亲情/紧急(1byte)
    TYPEe_AgingStatus aging;         //时效（1byte）
    TYPEs_TimeFormat startTm;        //起始时间（年+月+日+时+分+秒）(6byte)
    TYPEs_TimeFormat agingTm;        //时效时间（年+月+日+时+分+秒）(6byte)
    TYPEs_Week week;                 //周(1byte)
} TYPEs_BleSmartKeyAttributeModify;    //属性修改
typedef struct
{
    uint8_t pwd[8];      //开锁密码
} TYPEs_BleUnlockPwdSet; //开锁密码设置
typedef struct
{
    uint32_t cmd;      //命令
} TYPEs_BleEmptyUser; //开锁密码设置
typedef struct
{
    uint8_t dhG;               //原根
    uint8_t dhP;               //质数
    uint8_t reserved[6];       //预留
} TYPEs_BleLockPwdVerifyAdmin; //开锁密码设置
typedef enum
{
    eBleLockPwdVerifyAdminSuccess = 0x00,  //成功
    eBleLockPwdVerifyAdminFail = 0x01,     //失败
    eBleLockPwdVerifyAdminEmpty = 0x02,    //已清空
    eBleLockPwdVerifyAdminNoFinger = 0x03, //提示用户本地先增加管理指纹
} TYPEe_BleLockPwdVerifyAdmin;
typedef struct
{
    TYPEe_BleLockPwdVerifyAdmin ack;
} TYPEs_BleLockPwdVerifyAdminACK; //开锁密码设置回复

typedef struct
{
    uint8_t SSID[33];    //SSID(33byte)
    uint8_t wifiKey[65]; //wifi密码(65byte)
} TYPEs_BleWifiSetParm;  //WIFI设置参数
typedef enum
{
    eBleWifiSetParmAckSuccess = 0x00, //成功
    eBleWifiSetParmAckFail = 0x01,    //失败
    eBleWifiSetParmAckFailLow = 0x02, //失败电压低
} TYPEe_BleWifiSetParmAck;
typedef struct
{
    TYPEe_BleWifiSetParmAck ack;
} TYPEs_BleWifiSetParmAck;
typedef enum
{
    eBleUnlockUploadWifiSWSuccess = 0x00, //成功
    eBleUnlockUploadWifiSWFail = 0x01,    //失败
} TYPEe_BleUnlockUploadWifiSW;
typedef struct
{
    TYPEe_BleUnlockUploadWifiSW sw;
} TYPEs_BleUnlockUploadWifiSW;
typedef enum
{
    eWifiFunctionSwitchSetOpen = 0x00,  //开启
    eWifiFunctionSwitchSetClose = 0x11, //关闭
} TYPEe_WifiFunctionSwitchSet;
typedef struct
{
    TYPEe_WifiFunctionSwitchSet sw;
} TYPEs_BleWifiFunctionSwitchSet;
typedef struct
{
    uint16_t ID;    //人脸ID
} TYPEs_FaceRegAck; //增加指纹


typedef enum
{
    eWifiSetRouter = 0xF6,         //设置路由器
    eWifiServerDomain = 0xF7,      //设置服务器域名
    eWifiOpenMsgUpload = 0x04,     //开锁信息上传
    eWifiFaultMsgUpload = 0x07,    //故障信息上传
    eWifiImageUpload = 0x09,       //图片上传
    eWifiOTAUpdate = 0x10,         //OTA更新
    eWifiChkOTA = 0x11,            //模组自主查询OTA更新
    eWifiSetBPS = 0xFA,            //通讯波特率设置
    eWifiChkMAC = 0xFB,            //查询WIFI模块MAC地址
    eWifiProductScanRouter = 0xFC, //产测扫描路由器
    eWifiChkSoft = 0xFD,           //产测扫描路由器
    eWifiChkPskPwd = 0xFE,         //查询当前模块内PSK密码
} TYPEe_Wifi;

typedef struct
{
    uint8_t SSID[ROUTER_SSID_SIZE]; //SSID
    uint8_t Pwd[ROUTER_PWD_SIZE];   //路由器密码
} TYPEs_WifiSetRouter;

typedef struct
{
    uint8_t domainNameServer[DOMAIN_NAME_SERVER_SIZE]; //服务器主域名
    uint16_t port;                                     //端口
} TYPEs_WifiServerDomain;

//typedef enum
//{
//    eFaceRegConfirmSuccess = 0x00, //成功
//    eFaceRegConfirmFail = 0x01,    //失败
//} TYPEe_FaceRegConfirm;            //APP确认人脸录入成功

typedef struct
{
    uint8_t sw;              
} TYPEs_BleID2ServerSwitch;   //启用/禁用ID2服务 (0x67)(APP模式下能禁用)

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
    eCommBusyNo = 0x00,  //空闲
    eCommBusyYes = 0x01, //忙碌
} TYPEe_CommBusy;        //通讯忙碌情况

typedef struct
{
    TYPEe_LockVolumeSet volumeStatus;              //锁音量状态
} TYPEs_BleLockVolumeSet;        

typedef enum
{
    eGyroscopeSetClose = 0x00,  //关闭
    eGyroscopeSetOpen = 0x01, //打开
} TYPEe_GyroscopeSet;       

typedef struct
{
    TYPEe_GyroscopeSet gyroscope;              
} TYPEs_BleGyroscopeSet;        //陀螺仪

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
#pragma pack() //取消对齐
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_COMM_GLOBALS
#define HAL_COMM_EXT
#else
#define HAL_COMM_EXT extern
#endif /*HAL_COMM_GLOBALS*/

    /*************************公共变量*************************/
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
    /*************************公共函数*************************/
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
