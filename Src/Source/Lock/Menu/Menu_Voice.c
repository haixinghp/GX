/***************************************************************************************
**文件名:     Menu_Voice.c
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
#include "menu.h"
#include "HAL_Voice.h"
#include "HAL_Storage.h"
#include "HAL_Power.h"
#if MENU_MODULE_EN
//#warning MENU_MODULE_EN模块使能!
/*******************************宏定义***************************/
//#define MENU_VOICE_RUN_TIME_MAX				(1)		//运行时间
#define MENU_VOICE_FIRST_USE (4000)		   //工作时间
#define MENU_VOICE_FACE_PROMPT_TIME (5000) //人脸提醒时间
#define MENU_VOICE_FACE_PROMPT_CNT (3)	   //人脸提醒次数
#define MENU_VOICE_ADMIN_WAIT_TIME (100)	//菜单播放等待
/*************************.C变量结构声明***********************/
typedef struct
{
	INT32U runTime; //运行时间
} TYPEs_MenuVoiceCtrl;
/*************************私有全局变量*************************/
//static TYPEs_MenuVoiceCtrl menuVoiceCtrl =
//{
//	.runTime = 0,
//};
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_VoicePlay
**功能描述:     语音播放
**输入参数: 		--
**输出参数: 		--
**备注:         
****************************************************************************************/
static void Menu_VoicePlay(void)
{
        if((0 == storageSave.sysDat.voiceSw)&&
		((eVoiceDirPleaseVerifyFace == voiceCtrl.voiceDir)||
		 (eVoiceDirVerifySuccess == voiceCtrl.voiceDir)||
		(eVoiceDirHasBeenLocked == voiceCtrl.voiceDir)||
		 (eVoiceDirOpenTheDoorSuccess == voiceCtrl.voiceDir)||
		(eVoiceDirPleaseEnterFinger == voiceCtrl.voiceDir)||
		 (eVoiceDirPleaseEnterPwd == voiceCtrl.voiceDir)||
		(eVoiceDirVerifyFail == voiceCtrl.voiceDir)||
		 (eVoiceDirIdentFail == voiceCtrl.voiceDir)||
		(eVoiceDirFaceNotDetected == voiceCtrl.voiceDir)||
		 (eVoiceDirFaceDistanceTooClose == voiceCtrl.voiceDir)||
		(eVoiceDirFaceDistanceTooFar == voiceCtrl.voiceDir)||
		 (eVoiceDirFaceCovered == voiceCtrl.voiceDir)||
		(eVoiceDirPleaseEnterFingerOrPwd == voiceCtrl.voiceDir)||
        (eVoiceDirAppSetOpened == voiceCtrl.voiceDir)))
	{
		return;
	}
    DEBUG_LOG_OUT("storageSave.sysDat.voiceSw :%x\n",storageSave.sysDat.voiceSw);
    DEBUG_LOG_OUT("voiceCtrl.voiceDir :%x\n",voiceCtrl.voiceDir);
	HAL_VoicePlay();
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_VoiceDesktop
**功能描述:     桌面语音
**输入参数:     --
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_VoiceDesktop(void)
{
    voiceCtrl.repeat = 0;	//重复次数清空
    voiceCtrl.waitTime = 0; //不等待
    voiceCtrl.worktime = 0; //工作时间
    voiceCtrl.voiceDir = eVoiceDirPleaseVerifyFace;
    Menu_VoicePlay();
}
    
/***************************************************************************************
**函数名:       fn_WhenEnterMenu_VosGui
**功能描述:    进入菜单的语音操作
**输入参数: 
TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex
先前菜单，当前菜单
**输出参数: 
**备注:         
****************************************************************************************/
void fn_WhenEnterMenu_VosGui(TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex)
{
	switch (e_CurrentIndex)
	{
	case enum_Menu_Welcome:
		//        fn_Voice_Boot_Tone();
		//		voiceCtrl.repeat = 0;	//重复次数清空
		//		voiceCtrl.waitTime = 0; //不等待
		//		voiceCtrl.worktime = 0; //工作时间
		break;
	case enum_Menu_Desktop:
        #if 0
		if((eWakeUpSourceFinger == WakeUpSource) || //指纹头唤醒不播报
		   (enum_Menu_FaceVerifyFail == e_PreviousIndex) || 
		   (enum_Menu_FaceVerifyRetry == e_PreviousIndex) ||
           (eWakeUpSourceBac == WakeUpSource)||
           (enum_Menu_OpenLockSuccess == e_PreviousIndex))  
		{
			return;
		}	
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseVerifyFace;
		Menu_VoicePlay();
        #endif
		break;
	case enum_Menu_AdminChk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseVerifyAdminiFace;
		Menu_VoicePlay(); //请输入管理员指纹
		break;
	case enum_Menu_Admin:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =eVoiceDirFaceSetPress_1\n");
		voiceCtrl.voiceDir = eVoiceDirFaceSetPress_1;
		Menu_VoicePlay();
		//指纹设置请按2
		//密码设置请按3
		//电机扭力调节请按4
		//语音设置请按5
		//返回请按6
		break;
	case enum_Menu_FaceSet:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =enum_Menu_FaceSet);\n");
		voiceCtrl.voiceDir = eVoiceDirAddAdminFacePress_1;
		Menu_VoicePlay();
		//增加管理员人脸请按1
		//增加普通用户人脸请按2
		//删除普通用户人脸请按3
		//返回请按4
//		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4);					  //增加语音的播放时间
		// voiceCtrl.voiceDir =eVoiceDirAddAdminFinger,HAL_VOICE_WAIT_START, HAL_VOICE_DONOT_WAIT_END); //增加管理员指纹请按1
		break;
	case enum_Menu_AddAdminFaceFull:
	case enum_Menu_AddGeneralFaceFull:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFaceIsFull;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFaceStep1:
        #if 1
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
        #endif
        #if 0
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
        #endif
		voiceCtrl.voiceDir = eVoiceDirFaceInputMiddle;
		Menu_VoicePlay();
		DEBUG_LOG_OUT("Menu_VoicePlay(); eVoiceDirFaceInputMiddle\n");
		break;
	case enum_Menu_AddFaceStep2:
		#if 1
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
        #endif
        #if 0
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
        #endif
        #if 1
		voiceCtrl.voiceDir = eVoiceDirFaceInputUp;
		Menu_VoicePlay();
        #endif
		break;
	case enum_Menu_AddFaceStep3:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputLow;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFaceStep4:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputRight;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFaceStep5:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputLeft;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFaceUserOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegSuccess;
		Menu_VoicePlay();
		break; //添加人脸成功
	case enum_Menu_AddFaceUserFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegFailed;
		Menu_VoicePlay();
		break; //添加人脸失败
	case enum_Menu_FaceVerifyOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		//		voiceCtrl.voiceDir = eVoiceDirVerifySuccess;
		//		Menu_VoicePlay();
		break; //人脸验证成功
    case enum_Menu_AdminChkFail:
	case enum_Menu_FaceVerifyFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirVerifyFail;
		Menu_VoicePlay();
		break; //人脸验证失败
	case enum_Menu_FaceVerifyRetry:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirIdentFail;
		Menu_VoicePlay();
		break; //人脸验证失败
	case enum_Menu_DelGeneralFaceOK:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirDelSuccess;
		Menu_VoicePlay();
		break; //删除普通成功
	case enum_Menu_DelGeneralFaceFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirDelFailed;
		Menu_VoicePlay();
		break; //删除普通用户失败
	case enum_Menu_FaceNotDetected:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFaceNotDetected;
		Menu_VoicePlay();
		break;
	case enum_Menu_FingerSet:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =enum_Menu_FingerSet);\n");
		voiceCtrl.voiceDir = eVoiceDirAddFingerPress_1;
		#if 0
		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4);											//增加语音的播放时间
		#endif
		Menu_VoicePlay();
		//增加指纹请按1
		//删除指纹请按2
		//返回请按3
		break;
	case enum_Menu_ModPwd: //修改密码
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =enum_Menu_ModPwd);\n");
		#if 0
		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4); //增加语音的播放时间
		#endif
		voiceCtrl.voiceDir = eVoiceDirEnterSixNewPwd;
		Menu_VoicePlay(); //请输入6位新开门密码，按#号键结束
		break;
	case enum_Menu_ModPwdInputAgain:					//修改密码
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		#if 0
		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4); //增加语音的播放时间
		#endif
		voiceCtrl.voiceDir = eVoiceDirPleaseInputAgain;
		Menu_VoicePlay(); //请再输入一次，按#号键结束
		break;
	case enum_Menu_ModPwdOk: //密码设定成功
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSetSuccess;
		Menu_VoicePlay(); //密码修改成功
		break;
	case enum_Menu_ModPwdFail: //密码设定失败
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirModifPwdFailedRetry;
		Menu_VoicePlay(); //修改密码失败，请确认输入的密码
		break;
	case enum_Menu_ModPwdTooSimpleFail: //设置密码过于简单，请重设
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPwdTooSimplePleaseReset;
		Menu_VoicePlay(); //密码过于简单请重新输入
		break;
	case enum_Menu_MotorTorsionSet:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =enum_Menu_MotorTorsionSet);\n");
		voiceCtrl.voiceDir = eVoiceDirLowMotorTorquePress_1;
		Menu_VoicePlay();
		//低电机扭力请按1
		//高电机扭力请按2
		#if 0
		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4);					  //增加语音的播放时间
		#endif
	// voiceCtrl.voiceDir =eVoiceDirAddAdminFinger,HAL_VOICE_WAIT_START, HAL_VOICE_DONOT_WAIT_END); //增加管理员指纹请按1
		break;
	case enum_Menu_VioceSet:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = MENU_VOICE_ADMIN_WAIT_TIME; //等待时间
		voiceCtrl.worktime = 0; //工作时间
		DEBUG_LOG_OUT("voiceCtrl.voiceDir =enum_Menu_VioceSet);\n");
		voiceCtrl.voiceDir = eVoiceDirCloseVoicePress_1;
		Menu_VoicePlay();
		//关闭语音请按1
		//打开语音请按2
		//返回请按3

		break;
//	case enum_Menu_AddFinger:
//		DEBUG_LOG_OUT("voiceCtrl.voiceDir =eVoiceDirAddAdminFinger);\n");
//		Menu_AddKeepTime(M_MENU_DISPLAY_TIMER_LEVEL_4);											//增加语音的播放时间
//		voiceCtrl.voiceDir =eVoiceDirAddAdminFinger;
//		Menu_VoicePlay(); //增加管理员指纹请按1
//		break;
	case enum_Menu_FirstUse:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFirstUse;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddAdminFinger:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseEnterFinger;
		Menu_VoicePlay(); //请输入指纹
		break;
	case enum_Menu_AddGeneralFinger:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseEnterFinger;
		Menu_VoicePlay(); //请输入指纹
		break;
	case enum_Menu_AddAdminFingerFull:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFingerFull;
		Menu_VoicePlay(); //指纹已满
		break;
	case enum_Menu_AddGeneralFingerFull:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFingerFull;
		Menu_VoicePlay(); //指纹已满
		break;
	case enum_Menu_DelFingerOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirDelSuccess;
		Menu_VoicePlay(); //删除成功
		break;
	case enum_Menu_DelFingerFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirDelFailed;
		Menu_VoicePlay(); //删除失败
		break;
	case enum_Menu_DelFingerNoneFail: //没有普通用户指纹:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirNoOrdinaryUsers;
		Menu_VoicePlay();
		break;

	case enum_Menu_AddFingerStep1:
	case enum_Menu_AddFingerStep2: //添加指纹过程2
	case enum_Menu_AddFingerStep3: //添加指纹过程3
	case enum_Menu_AddFingerStep4: //添加指纹过程4
	case enum_Menu_AddFingerStep5: //添加指纹过程5
	case enum_Menu_AddFingerStep6: //添加指纹过程6
    case enum_Menu_BleAddFingerStep1: //添加指纹过程
	case enum_Menu_BleAddFingerStep2: //添加指纹过程
	case enum_Menu_BleAddFingerStep3: //添加指纹过程
	case enum_Menu_BleAddFingerStep4: //添加指纹过程
	case enum_Menu_BleAddFingerStep5: //添加指纹过程
	case enum_Menu_BleAddFingerStep6: //添加指纹过程   
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseEnterFinger;
		Menu_VoicePlay(); //请输入指纹
		break;
	case enum_Menu_AddFingerStep1OK: //添加指纹过程1
	case enum_Menu_AddFingerStep2OK: //添加指纹过程2
	case enum_Menu_AddFingerStep3OK: //添加指纹过程3
	case enum_Menu_AddFingerStep4OK: //添加指纹过程4
	case enum_Menu_AddFingerStep5OK: //添加指纹过程5
    case enum_Menu_BleAddFingerStep1OK:
	case enum_Menu_BleAddFingerStep2OK:
	case enum_Menu_BleAddFingerStep3OK:
	case enum_Menu_BleAddFingerStep4OK:
	case enum_Menu_BleAddFingerStep5OK:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseRemoveFinger;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFingerUserOk: //添加指纹过程成功
    case enum_Menu_BleAddFingerOK:    
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegSuccess;
		Menu_VoicePlay();
		break;
	case enum_Menu_AddFingerUserFail: //添加管理员失败
    case enum_Menu_BleAddFingerFail: 
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegFailed;
		Menu_VoicePlay();
		break;
	case enum_Menu_Factory:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirFactorySet;
		Menu_VoicePlay(); //恢复出厂设置请按#号键取消请按
		break;
    case enum_Menu_FactoryConfirm:
        voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceFactoryingWait;
		Menu_VoicePlay(); //恢复出厂设置中请稍后
		break;
    #if 0
	case enum_Menu_FactoryIng:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceFactoryingWait;
		Menu_VoicePlay(); //恢复出厂设置中请稍后
		break;
    #endif
	case enum_Menu_FactoryOK:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSetSuccess;
		Menu_VoicePlay(); //设定成功
		break;
	case enum_Menu_FactoryFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSetFailed;
		Menu_VoicePlay(); //设定成功
		break;
	case enum_Menu_PassWdPass:	   //密码验证通过
	case enum_Menu_FingerVerifyOK: //指纹校验成功
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
//		voiceCtrl.voiceDir = eVoiceDirVerifySuccess;
//		Menu_VoicePlay(); //验证成功
		break;
	case enum_Menu_TryFingerVerifyOK: //试用模式通过
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
									  //		voiceCtrl.voiceDir = eVoiceDirVerifySuccess;
									  //		Menu_VoicePlay();
		break;
	case enum_Menu_PassWdFail:		 //密码验证失败
	case enum_Menu_FingerVerifyFail: //指纹校验失败
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirVerifyFail;
		Menu_VoicePlay();
		break;
	case enum_Menu_MotorTorsionSetOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSetSuccess;
		Menu_VoicePlay();
		break;
	case enum_Menu_VioceSetOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSetSuccess;
		Menu_VoicePlay();
		break;
	case enum_Menu_PryAlarm:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 100; //这里需要等待一段时间
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSirensSound_110;
		Menu_VoicePlay();
		break;
	case enum_Menu_CloseLockSuccess:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirHasBeenLocked; //已上锁
		Menu_VoicePlay();
		break;
	case enum_Menu_OpenLockSuccess:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirOpenTheDoorSuccess;
		Menu_VoicePlay();
		break;
	case enum_Menu_CommError:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSirensSound_110; //通讯错误报警
		Menu_VoicePlay();
		break;
	case enum_Menu_LowPower:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirLowVoltageReplaceBattery; //电压低，请更换电池
		Menu_VoicePlay();
		break;
	case enum_Menu_LowPowerCannotWrok:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirLowVoltageUnableToWork; //电压低,无法工作
		Menu_VoicePlay();
		break;
	case enum_Menu_TryAlarm:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSystemHasBeenLocked; 
		Menu_VoicePlay();
		break;
	case enum_Menu_BleAddFaceStep1:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputMiddle;
		Menu_VoicePlay();
		DEBUG_LOG_OUT("Menu_VoicePlay(); eVoiceDirFaceInputMiddle\n");
		break;
	case enum_Menu_BleAddFaceStep2:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputUp;
		Menu_VoicePlay();
		break;
	case enum_Menu_BleAddFaceStep3:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputLow;
		Menu_VoicePlay();
		break;
	case enum_Menu_BleAddFaceStep4:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputRight;
		Menu_VoicePlay();
		break;
	case enum_Menu_BleAddFaceStep5:
		voiceCtrl.repeat = MENU_VOICE_FACE_PROMPT_CNT;
		voiceCtrl.worktime = MENU_VOICE_FACE_PROMPT_TIME;
		voiceCtrl.voiceDir = eVoiceDirFaceInputLeft;
		Menu_VoicePlay();
		break;
	case enum_Menu_BleAddFaceUserOk:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegSuccess;
		Menu_VoicePlay();
		break; //添加人脸成功
	case enum_Menu_BleAddFaceUserFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirRegFailed;
		Menu_VoicePlay();
		break; //添加人脸失败
	case enum_Menu_BellCanNotConn:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirPleaseCallAgainLater; 
		Menu_VoicePlay();
		break; //添加人脸失败
	case enum_Menu_WifiConnect:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirNetworkingPleaseHoldOn; 
		Menu_VoicePlay();
		break; //
    case enum_Menu_KeyDefAlarm:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirSirensSound_110; 
		Menu_VoicePlay();
		break; //一键布防
    case enum_Menu_ActiveDefense:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		voiceCtrl.voiceDir = eVoiceDirHelloIHaveInformedMyMaster; 
		Menu_VoicePlay();
		break; //主动防御
	case enum_Menu_TestErrorReport:		
		if(0 != (*((int8_t *)(&st_InputInfo.modeuleFail)))) //有故障
		{
			voiceCtrl.repeat = 0;	//重复次数清空
			voiceCtrl.waitTime = 0; //等待
			voiceCtrl.worktime = 0; //工作时间
			voiceCtrl.voiceDir = eVoiceDirSirensSound_110;
			Menu_VoicePlay();
		}
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       fn_WhenExitMenu_VosGui
**功能描述:    退出菜单的语音操作
**输入参数: 
TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex
先前菜单，当前菜单
**输出参数: 
**备注:         
****************************************************************************************/
void fn_WhenExitMenu_VosGui(TYPEe_MenuIndex e_PreviousIndex, TYPEe_MenuIndex e_CurrentIndex)
{
	switch (e_PreviousIndex)
	{
	case enum_Menu_Desktop:
		break;
	case enum_Menu_FaceVerifyOk:
        #if 0
		voiceCtrl.voiceDir = eVoiceDirFirstUse;
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		Menu_VoicePlay();
        #endif
		break;
	case enum_Menu_OpenLockSuccess:
		if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) //如果在演示模式，直接开门
		{
            DEBUG_LOG_OUT("fn_WhenExitMenu_VosGui MSG_FACTMEM_REG_STATUS_NONE\n");
            if(0 == st_InputInfo.insideOpenLockFlag) //门内开门不播报
            {
                voiceCtrl.repeat = 0;	//重复次数清空
                voiceCtrl.waitTime = 0; //不等待
                voiceCtrl.worktime = MENU_VOICE_FIRST_USE;
                voiceCtrl.voiceDir = eVoiceDirFirstUse;
                Menu_VoicePlay();
            }
		}
		break;
    #if 0
    case enum_Menu_CloseLockSuccess:

        if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) //如果在演示模式，直接开门
        {
            if(0 == st_InputInfo.insideOpenLockFlag) //门内开门不播报
            {
                voiceCtrl.voiceDir = eVoiceDirFirstUse;
                voiceCtrl.repeat = 0;	//重复次数清空
                voiceCtrl.waitTime = 0; //不等待
                voiceCtrl.worktime = 0; //工作时间
                voiceCtrl.worktime = MENU_VOICE_FIRST_USE;
                DEBUG_LOG_OUT("voiceCtrl.voiceDir = eVoiceDirFirstUse");
                Menu_VoicePlay(); 
            }
            
        }

        break;
        #endif
	case enum_Menu_FaceVerifyRetry:
		voiceCtrl.voiceDir = eVoiceDirPleaseVerifyFace;
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		Menu_VoicePlay();
		break;	
	case enum_Menu_FaceVerifyFail:
		voiceCtrl.repeat = 0;	//重复次数清空
		voiceCtrl.waitTime = 0; //不等待
		voiceCtrl.worktime = 0; //工作时间
		if ((0 != Menu_ChkError(&st_InputInfo.errorCnt,eErrorCheckTimeNo)))
		{
			return;
		}
//		if (((-1 == Menu_AppRegChk()) && (0 == storageSave.sysDat.pwdSetFlag)) ||
//                ((eCombineTypeAND == Menu_CheckCombine())))
//         
//		{
//			
//		}		
		if(storageSave.sysDat.allFinger != storageSave.sysDat.leaveFinger) //检查指纹
		{
			voiceCtrl.voiceDir = eVoiceDirPleaseEnterFinger; //请输入指纹
		}
		else if((storageSave.sysDat.allFinger == storageSave.sysDat.leaveFinger) &&
					(0 != storageSave.sysDat.pwdSetFlag)) //没有指纹，有密码的情况下
		{
			voiceCtrl.voiceDir = eVoiceDirPleaseEnterPwd; //请输入密码
		}
		Menu_VoicePlay();
		break;	
		//    case enum_Menu_Admin:
		//        break;
		//    case enum_Menu_InputError:
		//        break;
		//    case enum_Menu_MachineOpen:
		////		VoiceControler.SilencePlayFlag = 0;	//禁止在静音条件下播放语音
		//        fn_Voice_SetVolume(GetVolume());//退出报警模式后恢复到原来的音量
		//	case enum_Menu_TryFingerVerifyOK: //试用模式通过
		//		voiceCtrl.voiceDir =eVoiceDirFirstUse, MENU_VOICE_FIRST_USE);
		//		break;
	
	default:
		//		Menu_VoicePlayDir(eVoiceDirStop);
		//        fn_Voice_StopPlay();
		break;
	}
}

/***************************************************************************************
**函数名:       Menu_VoiceService
**功能描述:    	语音服务程序
**输入参数: 	--
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_VoiceService(void)
{
	HAL_VoiceDeal();
}
#endif /*MENU_MODULE_EN*/
/************************************Code End******************************************/
