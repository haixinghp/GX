/***************************************************************************************
**文件名:	  Menu.h
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间
**
**备注:
****************************************************************************************/
#ifndef __MENU_H__
#define __MENU_H__
#include "MyConfig.h"
#include <string.h>
#include <time.h>
#include "Menu_CurrentOperate.h"
#define MENU_MODULE_EN (1) //模块使能
#if MENU_MODULE_EN
/*************************专用宏定义*************************/
#define SYSTEM_CLOCK_TICK (1)
#define M_TREMBLING_PRESS (50) //(50/SYSTEM_CLOCK_TICK)			//按键扫描周期，50ms
#define M_MENU_DISPLAY_TIMER_FOREVER 0xFFFF
#define M_MENU_DISPLAY_TIMER_LEVEL_1 1000
#define M_MENU_DISPLAY_TIMER_LEVEL_2 2000
#define M_MENU_DISPLAY_TIMER_LEVEL_3 3000
#define M_MENU_DISPLAY_TIMER_LEVEL_4 5000
#define M_MENU_DISPLAY_TIMER_LEVEL_5 59000
#define M_MENU_DISPLAY_TIMER_LEVEL_6 6000		 //6s
#define M_MENU_DISPLAY_TIMER_LEVEL_10 (10000)	 //10S
#define M_MENU_DISPLAY_TIMER_LEVEL_15 (15000)	 //15S
#define M_MENU_DISPLAY_TIMER_LEVEL_20 (20000)	 //20S
#define M_MENU_DISPLAY_TIMER_LEVEL_30 (30000)	 //20S
#define M_MENU_DISPLAY_TIMER_LEVEL_60 (60000)	 //60
#define M_PASSWORD_BUF_LENGTH 17				 //密码缓冲区大小,最后一位作为字符停止符
#define M_PASSWORD_MAX_LEN (6)					 //密码长度
#define M_PASSWORD_DEFAULT_NONE (MENU_KEY_NOKEY) //未输入时密码数据
#define M_MAX_ERRORCOUNTS 10					 //验证错误最大次数
#define M_CARDID_INFO_BYTES 6					 //卡片ID信息字节数
//#define M_COMMAND_BUF_LENGTH            8           //指令缓冲区大小
#define M_COMMAND_BUF_LENGTH 15	 //指令缓冲区大小
#define M_TIME_SET_BUF_LENGTH 15 //时间设置输入缓冲区
//#define M_TREMBLING_PRESS               (50/SYSTEM_CLOCK_TICK)//按键扫描周期，50ms
#define M_PASSWD_ERROR_MAXTIME 10		 //最大密码输入错误次数
#define M_USER_ID_BYTES 2				 //用户ID编号位数
#define M_PASSWD_BYTES 6				 //密码输入位数
#define M_SHOW_TIME_BYTES (2)			 //时间显示
#define MENU_ADMIN_REG_NONE (0xA5B7)	 //无注册.
#define MENU_ADMIN_REG_ALREADY (0x0001)	 //本地管理员已注册
#define MENU_ADMIN_REG_PROGRESS (0x0002) //注册中.

#define MENU_KEY_0 (0)
#define MENU_KEY_1 (1)
#define MENU_KEY_2 (2)
#define MENU_KEY_3 (3)
#define MENU_KEY_4 (4)
#define MENU_KEY_5 (5)
#define MENU_KEY_6 (6)
#define MENU_KEY_7 (7)
#define MENU_KEY_8 (8)
#define MENU_KEY_9 (9)
#define MENU_KEY_MENU (10)
#define MENU_KEY_ENTER (11)
#define MENU_KEY_CLOSE (12)
#define MENU_KEY_BELL (13)
#define MENU_KEY_NOKEY (0xFF) //注意，需确认按键的类型‘1’和1的区别
#if 0
#define MENU_KEY_0 ('0')
#define MENU_KEY_1 ('1')
#define MENU_KEY_2 ('2')
#define MENU_KEY_3 ('3')
#define MENU_KEY_4 ('4')
#define MENU_KEY_5 ('5')
#define MENU_KEY_6 ('6')
#define MENU_KEY_7 ('7')
#define MENU_KEY_8 ('8')
#define MENU_KEY_9 ('9')
#define MENU_KEY_MENU (11 + '0')
#define MENU_KEY_ENTER (10 + '0')
#define MENU_KEY_NOKEY (0xFF) //注意，需确认按键的类型‘1’和1的区别
#endif
#define MENU_VOICE_FIRST_USE_WORK (4000) //工作时间
#define MENU_VOICE_FIRST_USE_WAIT (1000) //等待时间
#if defined(KEY_RANDOM_LOCK_EN)
#define KEY_RANDOM_SIZE (2) //随机数数量
#define KEY_RANDOM_PRESS (0xff)
#endif							 // KEY_RANDOM_LOCK_EN
#define ERROR_CNT_FACE_MAX (5)	 //指纹错误最大次数
#define ERROR_CNT_FINGER_MAX (5) //指纹错误最大次数
#define ERROR_CNT_PWD_MAX (3)	 //密码错误最大次数
#define ERROR_CNT_ALL_MAX (5)	 //总错误最大次数
#define ERROR_LOCK_TIME (180)	 //锁定时间
/*************************变量结构声明*************************/
typedef enum
{
    enumShutDownStepNone,			  //空闲
    enumShutDownStepWaitOpenCompleta, //等待开门动作处理完成
    enumShutDownStepCloseDisplay,	  //关闭显示。
    //	enumShutDownStepWaitLight,			//等待灯光处理完成
    enumShutDownStepWaitBack,		  //等待后台完成
    enumShutDownStepEnterLMP,		  //进入低功耗模式
} TYPEe_ShutDownStep;
typedef enum
{
    eUserTypePWD,	  //密码
    eUserTypeFinger,  //指纹
    eUserTypeCard,	  //卡片
    eUserTypeBlueKey, //蓝牙钥匙
    eUserTypeFace,	  //人脸
    eUserTypeAll, //总次数
} TYPEe_UserType;

typedef struct
{
    INT8U allErr;	  //总错误次数
    INT8U finger;	  //指纹错误次数
    INT8U Face;		  //人脸错误次数
    INT8U blueKey;	  //蓝牙钥匙错误次数
    INT8U pwd;		  //密码
    INT8U card;		  //卡
    INT32U startTime; //开始时间
    INT32U endTime;	  //结束时间
} TYPEs_ErrorCnt;	  //错误次数
typedef enum
{
    eEquipResultNone,	 //暂时未有结果
    eEquipResultSuccess, //成功
    eEquipResultFail,	 //失败
} TYPEe_EquipResult;
typedef enum
{
    eErrorCheckTimeYes =0,	 //检查时间
    eErrorCheckTimeNo =1,	 //不检查时间
} TYPEe_ErrorCheckTime;

typedef enum
{
    eAPPModeNone,  //没有进入
    eAPPModeEnter, //进入APP模式
} TYPEe_APPMode;
typedef enum
{
    eStorageStatusError,   //存储状态错误
    eStorageStatusSuccess, //存储状态成功
} TYPEe_StorageStatus;
typedef enum
{
    eID2StatusError,   //ID2错误
    eID2StatusSuccess, //ID2成功
} TYPEe_ID2Status;
typedef enum
{
    ekeyRandomSetEnable = 0, //随机按键生成标志使能
    ekeyRandomSetDisable,	 //随机按键生成标志失能
} TYPEe_keyRandomSet;
typedef enum
{
    eKeyRandomLockFlagEnable = 0, //随机按键锁定标志使能
    eKeyRandomLockFlagDisable,	  //随机按键锁定标志失能
} TYPEe_KeyRandomLockFlag;
typedef enum
{
    eFaceRunEnable = 0,
    eFaceRunDisable,
} TYPEe_FaceRun; //人脸运行使能。
typedef struct
{
#if defined(USE_LITTLE_ENDIAN)
    uint8_t reserved : 1;  //预留
    uint8_t RTC : 1;	   //时钟芯片1
    uint8_t reserved1 : 1; //预留
    uint8_t Finger : 1;	   //指纹模块3
    uint8_t Face : 1;	   //人脸模块4
	uint8_t hall:1;	 //霍尔磁钢
    uint8_t reserved2 : 1; //预留
    uint8_t Ir : 1;		   //红外芯片7
    uint8_t Wifi : 1;	   //WIFI模块8
#else
    uint8_t Wifi : 1;	   //WIFI模块8
    uint8_t Ir : 1;		   //红外芯片7
    uint8_t reserved2 : 2; //预留
    uint8_t Face : 1;	   //人脸模块4
    uint8_t Finger : 1;	   //指纹模块3
    uint8_t reserved : 1;  //预留
    uint8_t RTC : 1;	   //时钟芯片1
    uint8_t reserved1 : 1; //预留
#endif // USE_LITTLE_ENDIAN
} TYPEs_ModeuleFail;
union PrivateKeyDH
{
    UINT32 TmpKey;
    UINT8 key[4];
};
typedef struct
{
    INT8U a_u8_PassWordBuf[M_PASSWORD_BUF_LENGTH];	  //密码输入存放区，用户ID缓存区
    INT8U u8_PassWordBytes;							  //密码输入字节数
    INT8U a_u8_PassWordBufBak[M_PASSWORD_BUF_LENGTH]; //密码输入存放区备份
    INT8U u8_PassWordBytesBak;						  //密码输入字节数备份
    INT8U enterProjectModeCnt;						  //进入工程模式验证过程
    INT8S enterProjectModeFlag;						  //进入工程模式标志
    INT16U adminReg;								  //本地管理员注册情况
    INT16U FingerIdTmp;								  //指纹头ID临时存储
    TYPEe_UserType userType;						  //用户类型 0,密码，1指纹
    INT16U userID;									  //用户ID
//TYPEe_WakeUpSource wakeUpSource;				  //唤醒源
#if defined(KEY_RANDOM_LOCK_EN)
    TYPEe_keyRandomSet keyRandomSetFlag;		 //随机按键生成标志
    TYPEe_KeyRandomLockFlag keyRandomLockFlag;	 //随机按键标志
    INT8U keyRandom[KEY_RANDOM_SIZE];			 //用于保存用户随机数
#endif											 // KEY_RANDOM_LOCK_EN
    TYPEs_ErrorCnt errorCnt;					 //错误次数
    struct tm RTCTimer;							 //Rtc时间
    INT32U RTCTick;								 //时间戳
    TYPEe_EquipResult emptyFace;				 //清空人脸结果
    TYPEe_EquipResult emptyFinger;				 //清空指纹结果
    INT32U TimeCnt;								 //操作计时器
    INT32U bellOutTimeCnt;						 //门铃操作计数器
    INT8U pryAlarmEnableFlag;					 //报警使能
    INT8U pryAlarmFlag;							 //报警之前的状态
    INT32U pryAlarmTimeCnt;						 //报警时间计数
    INT16U upBatBattery;						 //上方电池电量
    INT16U downBatBattery;						 //下方电池电量
    INT8U openKey;								 //开门键盘值
    INT8U closeKey;								 //关门键盘值
    INT8U insideHandOpen;                        //内门把手开门
    TYPEe_APPMode appMode;						 //APP模式
    TYPEe_StorageStatus storageStatus;			 //存储状态
    TYPEe_ID2Status iD2Status;					 //ID2状态
    INT8U bleMac[6];							 //蓝牙MAC
    INT8U rootKey[6];                           //根密钥
    INT8U version[9];                           //后板版本号
    INT8U u8_EnterAdminFlag;					 //是否有按*键
    INT8U u8_EnterAdminFlagBak;					 //是否有按*键备份
    INT16U u16_PreHandleStatus;					 //记录前一次的执手状态
    INT8S u8_UserId;							 //用户ID 00~99 -1表示编号无效
    INT8S u8_UserIdBak;							 //用户ID 00~99 备份
    INT8U u8_CurrentPermission;					 //当前所在开锁权限，0：普通，1：管理员
    INT16U u16_Battery_Indicator;				 //当前的电量
    INT8U u8_Debug_Status;						 //调试状态
    INT8U a_u8_CommandBuf[M_COMMAND_BUF_LENGTH]; //指令输入存放区，用户ID缓存区
    INT8U u8_CommandBytes;						 //指令输入字节数
    INT8U KeepTimeUpdateDisableFlag;			 //菜单持续时间使能标志，0，使能，1失能
    TYPEe_ShutDownStep ShutDownStep;			 //待机处理步骤
    INT8U lockLog[14];							 //锁日志记录
    TYPEe_FaceRun faceRun;						 //人脸运行标志
    TYPEs_ModeuleFail modeuleFail;				 //模块错误记录器
    INT8U enterTestModeCnt;						 //进入工程模式验证过程
    INT16U userIdTmp;							 //用户ID随机变量
    union PrivateKeyDH privateKeyDH;             //DH密钥
    INT8U faceVerifyCnt;						 //人脸验证次数
    INT8U faceVerifySuccessFlag;				 //人脸验证通过标志
    INT8U insideOpenLockFlag;                    //门内开门标志
    INT8U motorRuningFlag;                       //电机运行标志
    INT8U defEn;                                 //主动防御标志
    INT8U defSysTickCntEn;                       //主动防御计时器使能
    INT32U defSysTickCnt;                        //主动防御时间计数器
    INT8U backbatGetFlag;                        //后板电量获取完成标志，0，未获取成功，1获取成功
	INT8U lowPowerCannotWork;					 //电压低无法工作标志 
} TYPEs_InputInfo;
typedef enum
{
    enum_Menu_SystemSleep,			 //系统休眠
    enum_Menu_BackDeal,				 //后台处理
    enum_Menu_WakeUp,				 //唤醒处理
    enum_Menu_LowPower,				 //电量低
    enum_Menu_LowPowerCannotWrok,	 //电量低，无法工作
    enum_Menu_StorageError,			 //存储错误
    enum_Menu_Welcome,				 //欢迎界面
    enum_Menu_Desktop,				 //桌面
    enum_Menu_FirstUse,				 //首次使用
    enum_Menu_PassWdPass,			 //密码验证通过
    enum_Menu_PassWdFail,			 //密码验证失败
    enum_Menu_TryFingerVerifyOK,	 //试用模式通过
    enum_Menu_AdminChk,				 //进入管理菜单检查
    enum_Menu_AdminChkFail,          //管理验证失败
    enum_Menu_Admin,				 //管理菜单
    enum_Menu_AdminReturn,			 //主菜单退出
    enum_Menu_FaceSet,				 //人脸信息设置
    enum_Menu_AddAdminFace,			 //增加管理员人脸
    enum_Menu_AddGeneralFace,		 //增加普通人脸
    enum_Menu_DelGeneralFace,		 //删除普通用户人脸
    enum_Menu_FaceSetReturn,		 //人脸识别菜单返回
    enum_Menu_AddAdminFaceFull,		 //管理人脸已满
    enum_Menu_AddGeneralFaceFull,	 //普通人脸已满
    enum_Menu_AddFaceStep1,			 //添加人脸过程
    enum_Menu_AddFaceStep1OK,		 //添加人脸过程成功
    enum_Menu_AddFaceStep2,			 //添加人脸过程2
    enum_Menu_AddFaceStep2OK,		 //添加人脸过程2成功
    enum_Menu_AddFaceStep3,			 //添加人脸过程3
    enum_Menu_AddFaceStep3OK,		 //添加人脸过程3成功
    enum_Menu_AddFaceStep4,			 //添加人脸过程4
    enum_Menu_AddFaceStep4OK,		 //添加人脸过程4成功
    enum_Menu_AddFaceStep5,			 //添加人脸过程5
    enum_Menu_AddFaceStep5OK,		 //添加人脸过程5成功
    enum_Menu_AddFaceUserOk,		 //添加人脸成功
    enum_Menu_AddFaceUserFail,		 //添加人脸失败
    enum_Menu_FaceVerifyOk,			 //人脸验证成功
    enum_Menu_FaceVerifyFail,		 //人脸验证失败
    enum_Menu_FaceVerifyRetry,       //重新验证人脸
    enum_Menu_DelGeneralFaceOK,		 //删除普通成功
    enum_Menu_DelGeneralFaceFail,	 //删除普通失败
    enum_Menu_FaceNotDetected,		 //未检测到人脸
    enum_Menu_MotorTorsionSet,		 //电机扭力调节
    enum_Menu_MotorTorsionLow,		 //低电机扭力
    enum_Menu_MotorTorsionHigh,		 //高电机扭力
    enum_Menu_MotorTorsionSetReturn, //电机扭力调节返回
    enum_Menu_MotorTorsionSetOk,	 //电机扭力调节设定成功
    enum_Menu_VioceSet,				 //音量设置
    enum_Menu_VioceClose,			 //关闭语音
    enum_Menu_VioceOpen,			 //打开语音
    enum_Menu_VioceSetReturn,		 //音量设置返回
    enum_Menu_VioceSetOk,			 //语音设置成功
    enum_Menu_ModPwd,				 //修改密码
    enum_Menu_ModPwdInputAgain,		 //密码再次输入
    enum_Menu_ModPwdOk,				 //密码设定成功
    enum_Menu_ModPwdFail,			 //密码设定失败
    enum_Menu_ModPwdTooSimpleFail,	 //设置密码过于简单，请重设
    enum_Menu_FingerSet,			 //指纹信息设置
    enum_Menu_AddFinger,			 //添加指纹
    enum_Menu_DelFinger,			 //删除指纹
    enum_Menu_FingerSetReturn,		 //指纹信息设置返回
    enum_Menu_AddAdminFinger,		 //增加管理指纹
    enum_Menu_AddGeneralFinger,		 //增加普通指纹
    enum_Menu_AddFingerReturn,		 //添加指纹界面返回
    enum_Menu_AddAdminFingerFull,	 //管理员指纹已满
    enum_Menu_AddGeneralFingerFull,	 //普通指纹已满
    enum_Menu_DelFingerOk,			 //删除成功
    enum_Menu_DelFingerFail,		 //删除失败
    enum_Menu_DelFingerNoneFail,	 //没有普通用户指纹
    enum_Menu_AddFingerStep1,		 //添加指纹过程
    enum_Menu_AddFingerStep1OK,		 //添加指纹过程成功
    enum_Menu_AddFingerStep2,		 //添加指纹过程2
    enum_Menu_AddFingerStep2OK,		 //添加指纹过程2成功
    enum_Menu_AddFingerStep3,		 //添加指纹过程3
    enum_Menu_AddFingerStep3OK,		 //添加指纹过程3成功
    enum_Menu_AddFingerStep4,		 //添加指纹过程4
    enum_Menu_AddFingerStep4OK,		 //添加指纹过程4成功
    enum_Menu_AddFingerStep5,		 //添加指纹过程5
    enum_Menu_AddFingerStep5OK,		 //添加指纹过程5成功
    enum_Menu_AddFingerStep6,		 //添加指纹过程6
    enum_Menu_AddFingerUserOk,		 //添加指纹成功
    enum_Menu_AddFingerUserFail,	 //添加指纹失败
    enum_Menu_Factory,				 //恢复出厂设置
    enum_Menu_FactoryConfirm,		 //恢复出厂设置确认
    enum_Menu_FactoryIng,			 //恢复出厂中
    enum_Menu_FactoryOK,			 //恢复出厂成功
    enum_Menu_FactoryFail,			 //恢复出厂失败
    enum_Menu_FingerVerifyOK,		 //指纹校验成功
    enum_Menu_FingerVerifyFail,		 //指纹校验失败
    enum_Menu_OpenLock,				 //开锁
    enum_Menu_CloseLockConfirm,		 //关锁确认
    enum_Menu_CloseLock,			 //关锁
    enum_Menu_CloseLockSuccess,		 //关锁成功
    enum_Menu_OpenLockSuccess,		 //开锁成功
    enum_Menu_CommError,			 //通讯错误
    enum_Menu_PryAlarm,				 //撬锁报警
    enum_Menu_TryAlarm,				 //禁试报警
    enum_Menu_TestError,			 //错误错误检测
    enum_Menu_TestErrorReport,		 //错误错误检测报告
    enum_Menu_Bell, 				 //门铃
    enum_Menu_BellCanNotConn, 		 //门铃无法连接
    enum_Menu_WifiConnect, 		 	 //wifi组网
    enum_Menu_CatEyeConn, 		 	 //猫眼连接参看
    enum_Menu_CatEyeDisConn, 		 //断开猫眼连接
    enum_Menu_KeyDefAlarm,           //一键布防报警
    enum_Menu_ActiveDefense,         //主动防御
    enum_Menu_APPMode,				 //进入APP模式
    enum_Menu_BleEquipmentReg,		 //蓝牙注册
    enum_Menu_BleAddFingerStep1,	 //添加指纹过程
    enum_Menu_BleAddFingerStep1OK,	 //步骤完成
    enum_Menu_BleAddFingerStep2,	 //添加指纹过程
    enum_Menu_BleAddFingerStep2OK,	 //步骤完成
    enum_Menu_BleAddFingerStep3,	 //添加指纹过程
    enum_Menu_BleAddFingerStep3OK,	 //步骤完成
    enum_Menu_BleAddFingerStep4,	 //添加指纹过程
    enum_Menu_BleAddFingerStep4OK,	 //步骤完成
    enum_Menu_BleAddFingerStep5,	 //添加指纹过程
    enum_Menu_BleAddFingerStep5OK,	 //步骤完成
    enum_Menu_BleAddFingerStep6,	 //添加指纹过程6
    enum_Menu_BleAddFingerOK,		 //添加成功
    enum_Menu_BleAddFingerFail,		 //添加成失败
    enum_Menu_BleAddFingerConfirm,	 //与APP确认进行确认是否添加成功
    enum_Menu_BleAddFingerConfirmFail, //确认失败
    enum_Menu_BleFingerDelId,		 //删除ID
    enum_Menu_BleAddFaceStep1,		 //蓝牙添加人脸过程
    enum_Menu_BleAddFaceStep1OK,	 //蓝牙添加人脸过程成功
    enum_Menu_BleAddFaceStep2,		 //蓝牙添加人脸过程2
    enum_Menu_BleAddFaceStep2OK,	 //蓝牙添加人脸过程2成功
    enum_Menu_BleAddFaceStep3,		 //蓝牙添加人脸过程3
    enum_Menu_BleAddFaceStep3OK,	 //蓝牙添加人脸过程3成功
    enum_Menu_BleAddFaceStep4,		 //蓝牙添加人脸过程4
    enum_Menu_BleAddFaceStep4OK,	 //蓝牙添加人脸过程4成功
    enum_Menu_BleAddFaceStep5,		 //蓝牙添加人脸过程5
    enum_Menu_BleAddFaceStep5OK,	 //蓝牙添加人脸过程5成功
    enum_Menu_BleAddFaceUserOk,		 //蓝牙添加人脸成功
    enum_Menu_BleAddFaceUserFail,	 //蓝牙添加人脸失败
    enum_Menu_BleAddFaceUserConfirm, //与APP确认进行确认是否添加成功
    enum_Menu_BleFaceDelId,			 //删除人脸ID
    enum_Menu_BleDelFinger,			 //删除指纹ID
    enum_Menu_BleEmptyUser,			 //蓝牙清空用户
    enum_Menu_BleFaceVersionGet,	 //蓝牙人脸版本号获取
    enum_Menu_BleConfigWifiData,	 //蓝牙wifi配置数据处理
    enum_Menu_BleAliyunSecretKeySetParm,	 //阿里云参数配置
    enum_Menu_BleCatEyeSN,	 //获取猫眼序列号
    enum_Menu_BleShangTangCatEyeSingleDouble,	 //商汤猫眼单双向切换
    enum_Menu_BleR8CatEyeOTA,	 //R8猫眼OTA升级
    enum_Menu_BleVerifyPhoneEncryptSuccess,	 //手机开锁成功
    enum_Menu_BleToolTestFail,	 //测试过程中有失败项
    enum_Menu_BleUnlockSmartKeyVerifySuccess,	 //智能钥匙开门成功
    enum_Menu_BleUnlockSmartKeyVerifyFail,	 //智能钥匙开门失败
    enum_Menu_BleModifFaceSafetyLevel,	 //修改人脸识别安全等级
    enum_Menu_BleInfraredMonitoringDistanceSet,	 //红外监测距离设置
    enum_Menu_MAX,					 //用于自动申请菜单缓冲区
} TYPEe_MenuIndex;

typedef enum
{
    enum_Window_TopDialog,		  //顶级对话框，不可返回父菜单，时间到后进入休眠
    enum_Window_GeneralDialog,	  //普通对话框，可返回父菜单，用户可以输入信息
    enum_Window_Menu,			  //菜单,按键时会进入子菜单
    enum_Window_Notify,			  //提示窗口,提示时间到后，窗口自动消失，或按任意键自动消失
    enum_Window_NoResponseNotify, //不响应按键提示窗口,提示时间到后，窗口自动消失，不响应按键
    enum_Window_NotifyInfo,		  //提示信息,提示时间到后，窗口自动消失，或按任意键自动消失，菜单回到先前状态
    enum_Window_OnlyReturn,		  //只响应返回
    enum_Window_Special,		  //特殊操作才起作用
} TYPEe_WindowType;

typedef struct
{
    TYPEe_MenuIndex e_CurrentIndex;				 //当前菜单索引号
    TYPEe_MenuIndex e_ParentIndex;				 //前级菜单索引号
    TYPEe_WindowType e_WindowType;				 //菜单类型
    INT16U u16_KeepTime;						 //菜单存在时间，以系统时钟为单位，0xFFFF为不自动返回
    INT8U u8_SubMenuCount;						 //子菜单数
    const TYPEe_MenuIndex *const p_SubMenuIndex; //指向子菜单的指针
    void (*fn_CurrentOperate)(void);			 //当前状态应该执行的功能操作
} TYPEs_KbdTabStruct;

typedef struct
{
    TYPEe_MenuIndex e_CurrentIndex;	 //当前状态索引号
    TYPEe_MenuIndex e_PreviousIndex; //前一刷新菜单状态索引号,指明是否有必要清屏
    VINT16U u16_KeepTime;			 //菜单存在的剩余时间
} TYPEs_MenuItem;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef MENU_GLOBALS
#define MENU_EXT
#else
#define MENU_EXT extern
#endif
    /*************************公共变量*************************/
    MENU_EXT TYPEs_MenuItem st_MenuItem;
    MENU_EXT TYPEs_InputInfo st_InputInfo;
    MENU_EXT const TYPEs_KbdTabStruct struct_MenuTab[enum_Menu_MAX];
    /*************************公共函数*************************/

    MENU_EXT void fn_JumpMenu(TYPEe_MenuIndex e_JumpMenu);
    MENU_EXT void fn_WhenEnterMenuOperation(TYPEe_MenuIndex e_JumpMenu);

    MENU_EXT void fn_WhenExitMenuOperation(TYPEe_MenuIndex e_JumpMenu);
    MENU_EXT void fn_GetInputInfo(INT8U u8_KeyVal);
    MENU_EXT void fn_KeyMenuTask(void);
    MENU_EXT void KeyForMenuInset(void);
    MENU_EXT void fn_WhenExitMenu_VosGui(TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex);
    MENU_EXT void fn_WhenEnterMenu_VosGui(TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex);
    MENU_EXT void Menu_ClearInputInfo(void);

    MENU_EXT void Menu_KeyForMenu(INT8U u8_KeyVal);
    MENU_EXT void Menu_KeyDealService(void);
    MENU_EXT void Menu_FingerService(void);
    MENU_EXT void Menu_KeyBackLightService(void);
    MENU_EXT void Menu_CycleScan(void);
    MENU_EXT void Menu_KeyBackLightUpdate(TYPEe_MenuIndex e_JumpMenu);
    MENU_EXT void Menu_AutoMenuReturn(void);
    MENU_EXT void Menu_AddKeepTime(INT16U tim);
    MENU_EXT void Menu_VoiceService(void);
    MENU_EXT void Menu_Motor_Service(void);
    MENU_EXT void Menu_Face_PowerOnStart(uint8_t mode);
    MENU_EXT void Menu_Face_Service(void);
    MENU_EXT void Menu_Face_DelGeneralFaceStart(void);
    MENU_EXT void Menu_Comm_Service(void);
    MENU_EXT void Menu_KeepTimeRefresh(void);
    MENU_EXT void Menu_CommMsgKeyStatusMenuEnter(void);
    MENU_EXT void Menu_ClearError(TYPEs_ErrorCnt *p);
    MENU_EXT int8_t Menu_AddError(TYPEs_ErrorCnt *p, TYPEe_UserType type);
    MENU_EXT void Menu_WifiService(void);
    MENU_EXT int8_t Menu_ChkError(TYPEs_ErrorCnt *p,TYPEe_ErrorCheckTime chkTimeFlag);
#ifdef M_FINGERPRINT_IDENTIFICATION_ENABLE
    MENU_EXT void FingerService(void);
#endif /*M_FINGERPRINT_IDENTIFICATION_ENABLE*/
#ifdef __cplusplus
}
#endif
#endif
#endif
/************************************Code End******************************************/
