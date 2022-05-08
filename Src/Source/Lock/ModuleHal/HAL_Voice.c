/***************************************************************************************
**文件名:     HAL_Voice.c
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
#include "HAL_Voice.h"
#include "DRV_Voice.h"
#if HAL_VOICE_MODULE_EN
#warning HAL_VOICE模块使能!
/*******************************宏定义***************************/
#define HAL_VOICE_WAIT_CHIP_OK (5) //等到芯片响应
#define HAL_VOICE_RUN_TIME_MAX (10) //运行时间
#define HAL_VOICE_DATA_LEN (16)     //数据长度
#define HAL_VOICE_0 (0)             //按键
#define HAL_VOICE_1 (1)             //按键
#define HAL_VOICE_2 (2)             //按键
#define HAL_VOICE_3 (3)             //按键
#define HAL_VOICE_4 (4)             //按键
#define HAL_VOICE_5 (5)             //按键
#define HAL_VOICE_6 (6)             //按键
#define HAL_VOICE_7 (7)             //按键
#define HAL_VOICE_8 (8)             //按键
#define HAL_VOICE_9 (9)             //按键
#define HAL_VOICE_ENTER (11)        //按键
#define HAL_VOICE_MENU (10)         //按键
#define HAL_VOICE_CLOSE (12)
#define HAL_VOICE_BELL (13)
//#define HAL_VOICE_WAIT_CHIP_OK (1) //等到芯片响应
/*************************.C变量结构声明***********************/
//typedef enum
//{
//	eVoiceStatusNone,	 //空闲
//	eVoiceStatusPowerUp, //上电
//	eVoiceStatusPowerOk, //上电完成
//	eVoiceStatusStandby, //待机
//	eVoiceStatusWorking, //工作中
//} TYPEe_VoiceStatus;
//typedef struct
//{
//	TYPEe_VoiceStatus voiceStatus; //语音工作状态
//	TYPEe_VoiceDir voiceDir;	   //语音播报状态
//	INT32U waitTime;			   //等待时间
//	INT32U waitTimeCnt;			   //等待时间计数器
//	INT32U worktime;			   //运行时间
//	INT32U worktimeCnt;			   //运行时间计数器
//	INT32U repeat;				   //重复
//	INT32U repeatCnt;			   //重复计数器
//} TYPEs_VoiceCtrl;
/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
TYPEs_VoiceCtrl voiceCtrl =
	{
		.voiceStatus = eVoiceStatusNone,
		.voiceDir = eVoiceDirStop,
		.waitTimeCnt = 0,
		.worktime = 0,
		.worktimeCnt = 0,
		.repeat=0,
		.repeatCnt = 0,
};

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_Voice_Init
**功能描述:     语音初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Voice_Init(void)
{
	DRV_Voice_Init();
	voiceCtrl.repeat = 0;	//重复次数清空
	voiceCtrl.waitTime = 0; //不等待
	voiceCtrl.worktime = 0; //工作时间
}
/***************************************************************************************
**函数名:       HAL_VoicePlay
**功能描述:     语音播放，使用状态机播放
**输入参数:     
TYPEe_VoiceDir dat, //播放的语音
INT32U workTime //播放时间，0 表示不强制等待。

**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_VoicePlay(void)
{
//	voiceCtrl.voiceDir = dat;
//	voiceCtrl.waitTime = waitTime;
	voiceCtrl.waitTimeCnt = 0;
	voiceCtrl.voiceStatus = eVoiceStatusStandby;
//	voiceCtrl.worktime = workTime;
	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.repeat = repeat;
	voiceCtrl.repeatCnt = 0;
}
//void HAL_VoicePlay(TYPEe_VoiceDir dat, INT32U waitTime, INT32U workTime, INT32U repeat)
//{
//	voiceCtrl.voiceDir = dat;
//	voiceCtrl.waitTime = waitTime;
//	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.voiceStatus = eVoiceStatusStandby;
//	voiceCtrl.worktime = workTime;
//	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.repeat = repeat;
//	voiceCtrl.repeatCnt = 0;
//}
/***************************************************************************************
**函数名:       HAL_VoicePlayDir
**功能描述:     直接播放，不通过状态机。
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_VoicePlayDir(TYPEe_VoiceDir dat)
{
	INT8U datTmp = 0;
	datTmp = (INT8U)(dat);
	DRV_Voice_Play(datTmp);
	//	WTN4_data(datTmp);
}
/***************************************************************************************
**函数名:       HAL_Voice_ChkBusy
**功能描述:     检查语音是否处于忙碌状态
**输入参数:     --
**输出参数:     
INT8S
0; //空闲
-1; //忙碌
**备注:         
****************************************************************************************/
INT8S HAL_Voice_ChkBusy(void)
{
	if (eVoiceStatusNone == voiceCtrl.voiceStatus)
	{
		return 0; //空闲
	}
	else
	{
		return -1; //忙碌
	}
}

/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_VoiceDeal(void)
{
	if (0 != voiceCtrl.bellTimeCnt)
	{
		if(1 == voiceCtrl.bellTimeCnt)
		{
			DRV_Voice_Bell(eVoiceBellClose);
		}
		else if (voiceCtrl.bellTimeCnt >= HAL_VOICE_BELL_CLOSE_TIME)
		{
			DRV_Voice_Bell(eVoiceBellClose);
		}
		else
		{
			DRV_Voice_Bell(eVoiceBellOpen);
		}
		voiceCtrl.bellTimeCnt --;
	}
	switch (voiceCtrl.voiceStatus)
	{
	case eVoiceStatusNone:
		break;
	case eVoiceStatusPowerUp:
		break;
	case eVoiceStatusPowerOk:
		break;
	case eVoiceStatusStandby:
		voiceCtrl.waitTimeCnt++;
		if (voiceCtrl.waitTimeCnt >= voiceCtrl.waitTime)
		{
			HAL_VoicePlayDir(voiceCtrl.voiceDir); //播放语音
			DEBUG_LOG_OUT("HAL_VoicePlayDir(voiceCtrl.voiceDir); \n");
//			DEBUG_LOG_OUT("voiceCtrl.worktime:%X\n",voiceCtrl.worktime);
			voiceCtrl.waitTimeCnt = 0;
			voiceCtrl.voiceStatus = eVoiceStatusWorking; //进入工作流程
		}
		break;
	case eVoiceStatusWorking: //预留
		voiceCtrl.waitTimeCnt++;
		if (voiceCtrl.waitTimeCnt >= voiceCtrl.worktime)
		{
			voiceCtrl.waitTimeCnt = 0;
			voiceCtrl.repeatCnt ++;
			if(voiceCtrl.repeatCnt >= voiceCtrl.repeat)
			{
				voiceCtrl.repeat = 0;	//重复次数清空
				voiceCtrl.waitTime = 0; //不等待
				voiceCtrl.worktime = 0; //工作时间
				voiceCtrl.voiceStatus = eVoiceStatusNone; //工作结束
			}
			else
			{
				voiceCtrl.voiceStatus = eVoiceStatusStandby; //再次播放
			}
		}
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       DRV_Voice_Bell
**功能描述:     控制门铃开关
**输入参数:     
uint8_t dat
0 开启
其他 关闭
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Voice_Bell(uint8_t dat)
{
	switch (dat)
	{
	case HAL_VOICE_BELL:
		voiceCtrl.bellTimeCnt = HAL_VOICE_BELL_WORK_TIME;
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       HAL_Voice_EnterSleepConfig
**功能描述:     进入低功耗配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Voice_EnterSleepConfig(void)
{
	DRV_Voice_EnterSleepConfig();
	DRV_Voice_BellEnterSleepConfig();
}
/***************************************************************************************
**函数名:       HAL_Voice_VolumeSet
**功能描述:     语音音量设置
**输入参数:     --
**输出参数:     --
**备注:         设置语音需要电源稳定。
****************************************************************************************/
void HAL_Voice_VolumeSet(uint8_t dat)
{
	DRV_Voice_VolumeSet(dat);
}
#endif /*HAL_VOICE_MODULE_EN*/
/************************************Code End******************************************/
