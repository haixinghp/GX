/***************************************************************************************
**文件名:     HAL_Voice.h
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
#ifndef __HAL_VOICE_H__
#define __HAL_VOICE_H__
#include "MyConfig.h"
#define HAL_VOICE_MODULE_EN (1) //模块使能
#if HAL_VOICE_MODULE_EN
/*************************专用宏定义*************************/
#define HAL_VOICE_DONOT_WAIT_END (0)
#define HAL_VOICE_WAIT_START (5)
#define HAL_VOICE_BELL_WORK_TIME (3000) //门铃工作时间
#define HAL_VOICE_BELL_CLOSE_TIME (2900)
/*************************变量结构声明*************************/

typedef enum
{
	eVoiceDirFaceSetPress_1 = 0,			  //人脸信息设置请按1
	eVoiceDirFingerSetPress_2 = 1,			  //指纹设置请按2
	eVoiceDirPwdSetPress_3 = 2,				  //密码设置请按3
	eVoiceDirMotorTorqueSetPress_4 = 3,		  //电机扭力调节请按4
	eVoiceDirVoiceSettPress_5 = 4,			  //语音设置请按5
	eVoiceDirReturnPleasePress_6 = 5,		  //返回请按6
	eVoiceDirAddAdminFacePress_1 = 6,		  //增加管理员人脸请按1
	eVoiceDirAddGeneralFacePress_2 = 7,		  //增加普通用户人脸请按2
	eVoiceDirDelGeneralFacePress_3 = 8,		  //删除普通用户人脸请按3
	eVoiceDirReturnPress_4 = 9,				  //返回请按4
	eVoiceDirAddFingerPress_1 = 10,			  //增加指纹请按1
	eVoiceDirDelFingerPress_2 = 11,			  //删除指纹请按2
	eVoiceDirReturnPress_3 = 12,			  //返回请按3
	eVoiceDirEnterSixNewPwd = 13,			  //请输入6位新开门密码
	eVoiceDirPleaseInputAgain = 14,			  //请再输入一次
	eVoiceDirPressHashKeyEnd = 15,			  //按#号键结束
	eVoiceDirPwdTooSimplePleaseReset = 16,	  //设置密码过于简单，请重设
	eVoiceDirModifPwdFailedRetry = 17,		  //修改密码失败，请确认输入的密码
	eVoiceDirLowMotorTorquePress_1 = 18,	  //低电机扭力请按1
	eVoiceDirHighTorqueMotorPress_2 = 19,	  //高电机扭力请按2
	eVoiceDirCloseVoicePress_1 = 20,		  //关闭语音请按1
	eVoiceDirOpenVoicePress_2 = 21,			  //打开语音请按2
	eVoiceDirReturnPress_5 = 22,			  //返回请按5
	eVoiceDirDelSuccess = 23,				  //删除成功
	eVoiceDirDelFailed = 24,				  //删除失败
	eVoiceDirSetSuccess = 25,				  //设置成功
	eVoiceDirSetFailed = 26,				  //设置失败
	eVoiceDirFaceIsFull = 27,				  //人脸信息已满
	eVoiceDirPleaseVerifyFace = 28,			  //请验证人脸
	eVoiceDirVerifySuccess = 29,			  //验证成功
	eVoiceDirVerifyFail = 30,				  //验证失败
	eVoiceDirPleaseVerifyAdminiFace = 31,	  //请验证管理员人脸
	eVoiceDirPleaseEnterFinger = 32,		  //请输入指纹
	eVoiceDirPleaseRemoveFinger = 33,		  //请拿开手指
	eVoiceDirRegSuccess = 34,				  //登记成功
	eVoiceDirRegFailed = 35,				  //登记失败
	eVoiceDirPleaseEnterPwd = 36,			  //请输入密码
	eVoiceDirFirstUse = 37,					  //首次使用请录入管理员人脸
	eVoiceDirLowVoltageReplaceBattery = 38,	  //电压低，请更换电池
	eVoiceDirLowVoltageUnableToWork = 39,	  //电压低,无法工作
	eVoiceDirFactorySet = 40,				  //恢复出厂设置请按#号键取消请按*号键
	eVoiceDirFingerFull = 41,				  //指纹已满
	eVoiceDirAppSetOpened = 42,				  //APP设置功能已打开
	eVoiceDirDoorIsNotLocked = 43,			  //门未锁好
	eVoiceDirFaceInputMiddle = 44,			  //请录入正脸
	eVoiceDirFaceInputUp = 45,				  //请微微抬头
	eVoiceDirFaceInputLow = 46,				  //请微微低头
	eVoiceDirFaceInputRight = 47,			  //请把脸偏向右手边
	eVoiceDirFaceInputLeft = 48,			  //请把脸偏向左手边
	eVoiceDirSirensSound_110 = 49,			  //“110警笛音”
	eVoiceDirSystemHasBeenLocked = 50,		  //非法操作系统已锁定，请等待3分钟再操作
	eVoiceDirHasBeenLocked = 51,			  //已上锁
	eVoiceDirButtonDing = 52,				  //按键声"叮声"
	eVoiceDirButtonDrip = 53,				  //按键声"按键水滴声"
	eVoiceDirPleaseLiftHandleLocked = 54,	  //请上提把手反锁
	eVoiceDirHaveProtection = 55,			  //已布防
	eVoiceDirHaveBeenAbandoned = 56,		  //已撤防
	eVoiceDirNoOrdinaryUsers = 57,			  //没有普通用户
	eVoiceDirInsuranceFunctionToCancel = 58,  //保险功能取消
	eVoiceDirInsuranceFunctionStart = 59,	  //保险功能启动
	eVoiceDirHelloIHaveInformedMyMaster = 60, //您好，我已通知主人
	eVoiceDirSetupSuccess = 61,				  //设定成功
	eVoiceDirChargePleaseLater = 62,		  //充电中，请稍后
	eVoiceDirOpenTheDoorSuccess = 63,		  //开门成功
	eVoiceDirDingDongDingDong = 64,			  //叮咚叮咚
	eVoiceDirIdentFail = 65,				  //未识别成功，请调整位置重试
	eVoiceDirFaceNotDetected = 66,			  //未检测到人脸
	eVoiceDirFaceDistanceTooClose = 67,		  //人脸距离太近
	eVoiceDirFaceDistanceTooFar = 68,		  //人脸距离太远
	eVoiceDirFaceCovered = 69,				  //脸部有遮挡
	eVoiceDirPleaseEnterFingerOrPwd = 70,	  //请输入指纹或密码
	eVoiceDirPleaseInputUpBat = 71,			  //请插入上方电池
	eVoiceDirPleaseInputDownBat = 72,			  //请插入下方电池
	eVoiceDirUpBatLowCanotWork = 73,			  //上方,电池,电压低，无法工作
	eVoiceDirDownBatLowCanotWork = 74,			  //下方,电池,电压低，无法工作
	eVoiceDirUpBatLow = 75,			  //上方,电池,电压低，请充电
	eVoiceDirDownBatLow = 76,			  //下方,电池,电压低，请充电
	eVoiceDirCallingWait = 161,			  //呼叫中，请等待
	eVoiceDirPleaseCallAgainLater = 162,			  //请稍后再拨
	eVoiceDirNetworkingPleaseHoldOn = 163,     //组网中，请稍后
    eVoiceFactoryingWait = 164,     //恢复出厂设置中请稍后
											  //	eVoiceDirPleaseEnterFinger = 71,		  //请输入指纹
											  //	eVoiceDirPleaseRemoveFinger = 72,		  //请拿开手指
	eVoiceDirStop = 0xFE,					  //空
} TYPEe_VoiceDir;
typedef enum
{
	eVoiceStatusNone,	 //空闲
	eVoiceStatusPowerUp, //上电
	eVoiceStatusPowerOk, //上电完成
	eVoiceStatusStandby, //待机
	eVoiceStatusWorking, //工作中
} TYPEe_VoiceStatus;
typedef struct
{
	TYPEe_VoiceStatus voiceStatus; //语音工作状态
	TYPEe_VoiceDir voiceDir;	   //语音播报状态
	INT32U waitTime;			   //等待时间
	INT32U waitTimeCnt;			   //等待时间计数器
	INT32U worktime;			   //运行时间
	INT32U worktimeCnt;			   //运行时间计数器
	INT32U repeat;				   //重复
	INT32U repeatCnt;			   //重复计数器
	INT32U bellTimeCnt;			   //时间计数器
} TYPEs_VoiceCtrl;

typedef enum
{
	eVoiceBellClose = 0, //关闭
	eVoiceBellOpen = 1,	 //开启
} TYPEe_VoiceBell;
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_VOICE_GLOBALS
#define HAL_VOICE_EXT
#else
#define HAL_VOICE_EXT extern
#endif /*HAL_VOICE_GLOBALS*/

	/*************************公共变量*************************/
	//HAL_VOICE_EXT TYPEe_voiceDir voiceDir;
	HAL_VOICE_EXT TYPEs_VoiceCtrl voiceCtrl;
	/*************************公共函数*************************/
	HAL_VOICE_EXT void HAL_Voice_Init(void);
	HAL_VOICE_EXT void HAL_VoicePlay(void);
	HAL_VOICE_EXT void HAL_VoicePlayDir(TYPEe_VoiceDir dat);
	HAL_VOICE_EXT void HAL_VoiceDeal(void);
	HAL_VOICE_EXT INT8S HAL_Voice_ChkBusy(void);
	HAL_VOICE_EXT void HAL_Voice_Bell(uint8_t dat);
	HAL_VOICE_EXT void HAL_Voice_EnterSleepConfig(void);
	HAL_VOICE_EXT void HAL_Voice_VolumeSet(uint8_t dat);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_VOICE_MODULE_EN*/

#endif /*HAL_VOICE_MODULE_EN*/
#endif /*__HAL_VOICE_H__*/
/************************************Code End******************************************/
