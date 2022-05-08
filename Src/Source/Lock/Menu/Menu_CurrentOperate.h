/***************************************************************************************
**文件名:     Menu_CurrentOperate.h
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**
**备注:      
****************************************************************************************/
#ifndef __MENU_CURRENT_OPERATE_H__
#define __MENU_CURRENT_OPERATE_H__
#include "MyConfig.h"
#include "HAL_RTC.h"
#define MENU_CURRENT_OPERATE_MODULE_EN (1) //模块使能
#if MENU_CURRENT_OPERATE_MODULE_EN
/*************************专用宏定义*************************/
#define MENU_FACE_RUN_TIME_MAX (20)                                //运行时间
#define WAIT_BELL_CONNECT_MIN_TIME (8000 / MENU_FACE_RUN_TIME_MAX) //最少连接时间

#define MENU_KEY_BACK_LIGHT_FINGER_STEP_1 (HAL_KEY_LIGHT_ONCE_1)
#define MENU_KEY_BACK_LIGHT_FINGER_STEP_2 (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2)
#define MENU_KEY_BACK_LIGHT_FINGER_STEP_3 (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2 | \
                                           HAL_KEY_LIGHT_ONCE_3)
#define MENU_KEY_BACK_LIGHT_FINGER_STEP_4 (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2 | \
                                           HAL_KEY_LIGHT_ONCE_3 | \
                                           HAL_KEY_LIGHT_ONCE_4)
#define MENU_KEY_BACK_LIGHT_FINGER_STEP_5 (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2 | \
                                           HAL_KEY_LIGHT_ONCE_3 | \
                                           HAL_KEY_LIGHT_ONCE_4 | \
                                           HAL_KEY_LIGHT_ONCE_5)
#define MENU_KEY_BACK_LIGHT_FINGER_STEP_6 (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2 | \
                                           HAL_KEY_LIGHT_ONCE_3 | \
                                           HAL_KEY_LIGHT_ONCE_4 | \
                                           HAL_KEY_LIGHT_ONCE_5 | \
                                           HAL_KEY_LIGHT_ONCE_6)
#define MENU_KEY_BACK_LIGHT_ALL_ON (HAL_KEY_LIGHT_ONCE_1 | \
                                    HAL_KEY_LIGHT_ONCE_2 | \
                                    HAL_KEY_LIGHT_ONCE_3 | \
                                    HAL_KEY_LIGHT_ONCE_4 | \
                                    HAL_KEY_LIGHT_ONCE_5 | \
                                    HAL_KEY_LIGHT_ONCE_6 | \
                                    HAL_KEY_LIGHT_ONCE_7 | \
                                    HAL_KEY_LIGHT_ONCE_8 | \
                                    HAL_KEY_LIGHT_ONCE_9 | \
                                    HAL_KEY_LIGHT_ONCE_0 | \
                                    HAL_KEY_LIGHT_ONCE_M | \
                                    HAL_KEY_LIGHT_ONCE_E)
#define MENU_KEY_BACK_LIGHT_ALL_OFF (~(MENU_KEY_BACK_LIGHT_ALL_ON))
#define MENU_KEY_BACK_LIGHT_ADD_FINGER (HAL_KEY_LIGHT_ONCE_1 | \
                                        HAL_KEY_LIGHT_ONCE_2 | \
                                        HAL_KEY_LIGHT_ONCE_3 | \
                                        HAL_KEY_LIGHT_ONCE_M)
#define MENU_KEY_BACK_LIGHT_ADMIN (HAL_KEY_LIGHT_ONCE_1 | \
                                   HAL_KEY_LIGHT_ONCE_2 | \
                                   HAL_KEY_LIGHT_ONCE_3 | \
                                   HAL_KEY_LIGHT_ONCE_4 | \
                                   HAL_KEY_LIGHT_ONCE_M)
#define MENU_KEY_BACK_LIGHT_FACTORY (HAL_KEY_LIGHT_ONCE_E | \
                                     HAL_KEY_LIGHT_ONCE_M)
#define MENU_KEY_BACK_LIGHT_FACTORY_ING (HAL_KEY_LIGHT_ONCE_M)
#define MENU_KEY_BACK_LIGHT_X (HAL_KEY_LIGHT_ONCE_1 | \
                               HAL_KEY_LIGHT_ONCE_3 | \
                               HAL_KEY_LIGHT_ONCE_5 | \
                               HAL_KEY_LIGHT_ONCE_7 | \
                               HAL_KEY_LIGHT_ONCE_9)
#define MENU_KEY_BACK_LIGHT_APP (HAL_KEY_LIGHT_ONCE_2 | \
                                 HAL_KEY_LIGHT_ONCE_4 | \
                                 HAL_KEY_LIGHT_ONCE_5 | \
                                 HAL_KEY_LIGHT_ONCE_6 | \
                                 HAL_KEY_LIGHT_ONCE_8)
#define MENU_KEY_BACK_LIGHT_OPEN (HAL_KEY_LIGHT_ONCE_E)
#define MENU_KEY_BACK_LIGHT_STORAGE_ERROR (HAL_KEY_LIGHT_ONCE_1 | \
                                           HAL_KEY_LIGHT_ONCE_2 | \
                                           HAL_KEY_LIGHT_ONCE_3 | \
                                           HAL_KEY_LIGHT_ONCE_4 | \
                                           HAL_KEY_LIGHT_ONCE_5 | \
                                           HAL_KEY_LIGHT_ONCE_7 | \
                                           HAL_KEY_LIGHT_ONCE_8 | \
                                           HAL_KEY_LIGHT_ONCE_9)
#define MENU_KEY_BACK_BELL (MENU_KEY_BACK_LIGHT_ALL_ON)
#define MENU_KEY_BACK_WIFI (DRV_MBI5024_LED_0)
#define MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER (20)    //添加人脸时闪动时间
#define MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT (1000)   //添加人脸时重复次数
#define MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER_NOT (0) //添加人脸时闪动时间
#define MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT_NOT (0)  //添加人脸时重复次数
#define MENU_KEY_BACK_LIGHT_LOCK_CONFIRM (HAL_KEY_LIGHT_ONCE_E)
#define MENU_KEY_BACK_LIGHT_FACTORY (HAL_KEY_LIGHT_ONCE_E | \
                                     HAL_KEY_LIGHT_ONCE_M)
#define MENU_FUNCTION_ON (1)
#define MENU_FUNCTION_OFF (2)

#define MENU_UNLOCK_ATTR_NONE (0X00) //开锁属性啥功能不带
#define MENU_UNLOCK_ATTR_KINDNESS (0X0F) //开锁属性带亲情功能
#define MENU_UNLOCK_ATTR_SOS (0XF0) //开锁属性带紧急功能

/*************************变量结构声明*************************/
typedef enum
{
	eLogTypeKtemp,			//模式temp
	eLogTypeKsos,			//模式SOS
	eLogTypeOpenByFinger,	//指纹开门
	eLogTypeOpenBySmartKey, //智能钥匙开门
	eLogTypeOpenByPwd,		//密码开门
	eLogTypeOpenByFace,		//人脸开门
	eLogTypeOpenByPhone,	//手机开门
	eLogTypeAddPwd,			//修改密码
	eLogTypeAddPwdSos,		//修改sos密码
	eLogTypeAddAesPhone,	//AES增加手机账号
	eLogTypeDelPhone,		//删除手机号
	eLogTypeAddFinger,		//添加指纹
	eLogTypeDelFinger,		//删除指纹
	eLogTypeAddFace,		//添加人脸
	eLogTypeDelFace,		//删除人脸
	eLogTypeAddSmartKey,	//添加智能钥匙
	eLogTypeDelSmartKey,	//删除智能钥匙
	eLogTypeEnterAdminFace, //进入管理菜单人脸
	eLogTypeAlarm,			//报警
	eLogTypeTest,			//测试
	eLogTypeEmpty,			//清空
} TYPEe_LogType;
typedef enum
{
	eWifiSendSwDisenable = 0, // 发送开关关闭
	eWifiSendSwEnable = 1,	  //发送开关开
	eWifiSendSwEnableNoRst,	  //发送开关开不重启
} TYPEe_WifiSendSw;
typedef enum
{
	eWifiSendTypeNone = 0,				 //推送空闲
	eWifiSendTypeAlarmPry = 1,			 //撬锁报警
	eWifiSendTypeAlarmUnlock = 2,		 //门未锁好报警
	eWifiSendTypeAlarmTry = 3,			 //禁试报警
	eWifiSendTypeAlarmProtection = 4,	 //布防报警
	eWifiSendTypeAlarmActiveDefense = 5, //主动防御报警
	eWifiSendTypeUnlockUpdata = 10,		 //推送开门记录
} TYPEe_WifiSendType;
typedef enum
{
    eWifiunlockTypeNone  = 0x00,
	eWifiunlockTypeFinger = 0x01,		   //指纹
	eWifiunlockTypePwd = 0x02,			   //密码 (PageID为0)
	eWifiunlockTypeSmartKey = 0x03,		   //智能钥匙/手环 (PageID位填充第4字节到7字节（日时分秒）数据，后台匹配完整数据包
	eWifiunlockTypePhone = 0x04,		   //手机 (PageID位填充第4字节到7字节（日时分秒）数据，后台匹配完整数据包
	eWifiunlockTypeFace = 0x05,			   //人脸(共用指纹PageID,属性同指纹)
	eWifiunlockTypeHuaweiWallet = 0x06,	   //华为钱包
	eWifiunlockTypeNFC = 0x09,			   //NFC开锁
	eWifiunlockTypeIcCard = 0x0A,		   //IC卡开锁(共用指纹PageID,位置填写IC卡用户ID)
	eWifiunlockTypeFaceAndFinger = 0x50,   //人脸+指纹开门
	eWifiunlockTypeFaceAndPwd = 0x51,	   //人脸+密码开门
	eWifiunlockTypeFingerAndPwd = 0x52,	   //指纹+密码开门
	eWifiunlockTypeFingerAndIcCard = 0x53, //指纹+卡开门
    eWifiunlockTypeWifiTest = 0xf1,        //wifi联网测试。
} TYPEe_WifiunlockType;

typedef enum
{
	eCatEyeStatusNone = 0x00, //空闲
	eCatEyeStatusBusy = 0x01, //忙碌
} TYPEe_CatEyeStatus;

typedef enum
{
	eWaitCompleteNo = 0x00, 
	eWaitCompleteYes = 0x01, 
} TYPEe_WaitComplete;

typedef enum
{
	eConnectWaitOverNo = 0x00, 
	eConnectWaitOverYes = 0x01, 
} TYPEe_ConnectWaitOver;

typedef enum
{
	eWifiConnectStatusNone = 0x00,  //无操作
	eWifiConnectStatusReady , //准备
    eWifiConnectStatusWaitSwMode, //等待数据切换完成
    eWifiConnectStatusWaitIdle,    //等待空闲
    eWifiConnectStatusWaitSendData,    //等待空闲
    eWifiConnectStatusWaitConn , //等待连接完成
    eWifiConnectStatusWaitComp    , //等待连接完成
} TYPEe_WifiConnectStatus;

typedef enum
{
	eWifiSendStatusNone = 0x00,  //无操作
	eWifiSendStatusReady , //准备
    eWifiSendStatusWaitSwMode, //等待数据切换完成
    eWifiSendStatusWaitIdle,    //等待空闲
    eWifiSendStatusWaitSendData,    //等待空闲
    eWifiSendStatusWaitSendComp , //等待发送完成
    eWifiSendStatusWaitComp    , //等待完成
} TYPEe_WifiSendStatus;

typedef enum
{
	eBellOutFlagEn = 0x00,	//能退出
	eBellOutFlagDis = 0x01, //不能退出
} TYPEe_BellOutFlag;

typedef struct
{
	TYPEe_WifiSendSw sendSw;		 //WiFi推送开关
	TYPEe_WifiSendType sendType;	 //推送类型
	TYPEe_WifiunlockType unlockType; //开锁方式
	uint8_t pageId[4];				 //PageID
    uint8_t limt;                    //属性
	TYPEe_CatEyeStatus catEyeStatus; //猫眼状态
	uint32_t timerCnt;				 //运行计数器
	TYPEe_BellOutFlag bellOutFlag;	 //是否能退出bell模式标志
	TYPEe_WaitComplete waitComplete; //等待完成 
	TYPEe_ConnectWaitOver connectWaitOver; //连接等待标志
    TYPEe_WifiConnectStatus wifiConnectStatus; //连接状态
    TYPEe_WifiSendStatus wifiSendStatus; //wifi发送状态
    uint16_t sendLen;                   //要发送数据的长度
} TYPEs_WifiSendParm;				 //WiFi发送数据参数
typedef enum
{
	eInPowerStatusNormal = 0, //正常
    eInPowerStatusLow = 1, //电量低
	eInPowerStatusLowCannotWrok = 2, //电量低，无法工作
} TYPEe_InPowerStatus;

typedef enum
{
	eCombineOpenNull = 0x00, 
	eCombineOpenPwd = 0x01, //密码开门
	eCombineOpenFinger = 0x02, //指纹开门
	eCombineOpenBlue = 0x04, //指纹开门
} TYPEe_CombineOpen;
typedef enum
{
	eCombineTypeOR = 0x00, //或模式
	eCombineTypeAND = 0x01, //与模式
} TYPEe_CombineType;


#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef MENU_CURRENT_OPERATE_GLOBALS
#define MENU_CURRENT_OPERATE_EXT
#else
#define MENU_CURRENT_OPERATE_EXT extern
#endif /*MENU_CURRENT_OPERATE_GLOBALS*/

	/*************************公共变量*************************/
	MENU_CURRENT_OPERATE_EXT TYPEs_WifiSendParm wifiSendParm;
	/*************************公共函数*************************/
	MENU_CURRENT_OPERATE_EXT void Menu_None(void);
	MENU_CURRENT_OPERATE_EXT void Menu_SystemSleep(void);
	MENU_CURRENT_OPERATE_EXT void Menu_Welcome(void);
	MENU_CURRENT_OPERATE_EXT void Menu_Desktop(void);
	MENU_CURRENT_OPERATE_EXT void Menu_PassWdPass(void);
	MENU_CURRENT_OPERATE_EXT void Menu_PassWdFail(void);
	MENU_CURRENT_OPERATE_EXT void Menu_FirstUse(void);
	MENU_CURRENT_OPERATE_EXT void Menu_RecordLockLog(TYPEe_LogType ty, uint8_t *dat, uint8_t datLen);
	MENU_CURRENT_OPERATE_EXT int8_t Menu_AppRegChk(void);
	MENU_CURRENT_OPERATE_EXT int8_t Menu_RTCTimerGet(void);
	MENU_CURRENT_OPERATE_EXT int8_t Menu_GetchannelPwd(uint8_t *p, uint8_t len);
	MENU_CURRENT_OPERATE_EXT void Menu_WifiLockLogUpdata(void);
	MENU_CURRENT_OPERATE_EXT void Menu_WifiDeal(void);
	MENU_CURRENT_OPERATE_EXT void Menu_WifiCmdAlarmUpdata(void);
	MENU_CURRENT_OPERATE_EXT void Menu_TriggerCaptureData(void);
	MENU_CURRENT_OPERATE_EXT void Menu_VideoCallData(void);
	MENU_CURRENT_OPERATE_EXT void Menu_Face_PowerOffStart(void);
	MENU_CURRENT_OPERATE_EXT int8_t Menu_FaceId_WaitComplete(void);
	MENU_CURRENT_OPERATE_EXT TYPEe_InPowerStatus Menu_CheckWifiPower(void);
	MENU_CURRENT_OPERATE_EXT TYPEe_CombineType Menu_CheckCombine(void);
	MENU_CURRENT_OPERATE_EXT int8_t Menu_CheckTimeVaild(struct tm *timeNow,struct tm *timeStart,struct tm *timeStop,uint8_t wDay);
	MENU_CURRENT_OPERATE_EXT void Menu_WifiService(void);
    MENU_CURRENT_OPERATE_EXT int8_t Menu_LowPowerChk(void);
    MENU_CURRENT_OPERATE_EXT int8_t Menu_WaitWifiComplete(void);
    MENU_CURRENT_OPERATE_EXT void Menu_EnterDesktopVerifyStart(void);
    MENU_CURRENT_OPERATE_EXT void Menu_VoiceDesktop(void);
    MENU_CURRENT_OPERATE_EXT void APP_GetPowerValue(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*MENU_CURRENT_OPERATE_MODULE_EN*/

#endif /*MENU_CURRENT_OPERATE_MODULE_EN*/
#endif /*__MENU_CURRENT_OPERATE_H__*/
/************************************Code End******************************************/
