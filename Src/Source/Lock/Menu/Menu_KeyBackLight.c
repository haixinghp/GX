/***************************************************************************************
**文件名:	  Menu_KeyBackLight.c
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间
**
**备注:
****************************************************************************************/
#include "menu.h"
#include "HAL_KeyLight.h"
#include "Function.h"
#if MENU_MODULE_EN
//#warning MENU_MODULE_EN模块使能!
/*******************************宏定义***************************/
#define MENU_KEY_BACK_LIGHT_RUN_TIME_MAX (10)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_1 (HAL_KEY_LIGHT_ONCE_1)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_2 (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_3 (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2 | \
//                                           HAL_KEY_LIGHT_ONCE_3)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_4 (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2 | \
//                                           HAL_KEY_LIGHT_ONCE_3 | \
//                                           HAL_KEY_LIGHT_ONCE_4)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_5 (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2 | \
//                                           HAL_KEY_LIGHT_ONCE_3 | \
//                                           HAL_KEY_LIGHT_ONCE_4 | \
//                                           HAL_KEY_LIGHT_ONCE_5)
//#define MENU_KEY_BACK_LIGHT_FINGER_STEP_6 (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2 | \
//                                           HAL_KEY_LIGHT_ONCE_3 | \
//                                           HAL_KEY_LIGHT_ONCE_4 | \
//                                           HAL_KEY_LIGHT_ONCE_5 | \
//                                           HAL_KEY_LIGHT_ONCE_6)
//#define MENU_KEY_BACK_LIGHT_ALL_ON (HAL_KEY_LIGHT_ONCE_1 | \
//                                    HAL_KEY_LIGHT_ONCE_2 | \
//                                    HAL_KEY_LIGHT_ONCE_3 | \
//                                    HAL_KEY_LIGHT_ONCE_4 | \
//                                    HAL_KEY_LIGHT_ONCE_5 | \
//                                    HAL_KEY_LIGHT_ONCE_6 | \
//                                    HAL_KEY_LIGHT_ONCE_7 | \
//                                    HAL_KEY_LIGHT_ONCE_8 | \
//                                    HAL_KEY_LIGHT_ONCE_9 | \
//                                    HAL_KEY_LIGHT_ONCE_0 | \
//                                    HAL_KEY_LIGHT_ONCE_M | \
//                                    HAL_KEY_LIGHT_ONCE_E)
//#define MENU_KEY_BACK_LIGHT_ALL_OFF (~(MENU_KEY_BACK_LIGHT_ALL_ON))
//#define MENU_KEY_BACK_LIGHT_ADD_FINGER (HAL_KEY_LIGHT_ONCE_1 | \
//                                        HAL_KEY_LIGHT_ONCE_2 | \
//                                        HAL_KEY_LIGHT_ONCE_3 | \
//                                        HAL_KEY_LIGHT_ONCE_M)
//#define MENU_KEY_BACK_LIGHT_ADMIN (HAL_KEY_LIGHT_ONCE_1 | \
//                                   HAL_KEY_LIGHT_ONCE_2 | \
//                                   HAL_KEY_LIGHT_ONCE_3 | \
//                                   HAL_KEY_LIGHT_ONCE_4 | \
//                                   HAL_KEY_LIGHT_ONCE_M)
//#define MENU_KEY_BACK_LIGHT_FACTORY (HAL_KEY_LIGHT_ONCE_E | \
//                                     HAL_KEY_LIGHT_ONCE_M)
//#define MENU_KEY_BACK_LIGHT_FACTORY_ING (HAL_KEY_LIGHT_ONCE_M)
//#define MENU_KEY_BACK_LIGHT_X (HAL_KEY_LIGHT_ONCE_1 | \
//                               HAL_KEY_LIGHT_ONCE_3 | \
//                               HAL_KEY_LIGHT_ONCE_5 | \
//                               HAL_KEY_LIGHT_ONCE_7 | \
//                               HAL_KEY_LIGHT_ONCE_9)
//#define MENU_KEY_BACK_LIGHT_APP (HAL_KEY_LIGHT_ONCE_2 | \
//                                 HAL_KEY_LIGHT_ONCE_4 | \
//                                 HAL_KEY_LIGHT_ONCE_5 | \
//                                 HAL_KEY_LIGHT_ONCE_6 | \
//                                 HAL_KEY_LIGHT_ONCE_8)
//#define MENU_KEY_BACK_LIGHT_OPEN (HAL_KEY_LIGHT_ONCE_E)
//#define MENU_KEY_BACK_LIGHT_STORAGE_ERROR (HAL_KEY_LIGHT_ONCE_1 | \
//                                           HAL_KEY_LIGHT_ONCE_2 | \
//                                           HAL_KEY_LIGHT_ONCE_3 | \
//                                           HAL_KEY_LIGHT_ONCE_4 | \
//                                           HAL_KEY_LIGHT_ONCE_5 | \
//                                           HAL_KEY_LIGHT_ONCE_7 | \
//                                           HAL_KEY_LIGHT_ONCE_8 | \
//                                           HAL_KEY_LIGHT_ONCE_9)
//#define MENU_KEY_BACK_BELL (DRV_MBI5024_LED_M)
//#define MENU_KEY_BACK_WIFI (DRV_MBI5024_LED_0)
//#define MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER (20)    //添加人脸时闪动时间
//#define MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT (1000)   //添加人脸时重复次数
//#define MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER_NOT (0) //添加人脸时闪动时间
//#define MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT_NOT (0)  //添加人脸时重复次数
/*************************.C变量结构声明***********************/
typedef struct
{
    INT32U runTime; //系统运行时间
} TYPEs_MenuKeyBackLightCtrl;
/*************************私有全局变量*************************/
static TYPEs_MenuKeyBackLightCtrl menuKeyBackLightCtrl =
    {
        .runTime = 0,
};
static const INT16U backLightDataAddFaceStep1[] = {
    MENU_KEY_BACK_LIGHT_ALL_OFF,
    HAL_KEY_LIGHT_ONCE_5}; //添加人脸
static const INT16U backLightDataAddFaceStep2[] = {
    HAL_KEY_LIGHT_ONCE_5,
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2}; //添加人脸
static const INT16U backLightDataAddFaceStep3[] = {
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2,
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8}; //添加人脸
static const INT16U backLightDataAddFaceStep4[] = {
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8,
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8 | HAL_KEY_LIGHT_ONCE_6}; //添加人脸
static const INT16U backLightDataAddFaceStep5[] = {
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8 | HAL_KEY_LIGHT_ONCE_6,
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8 | HAL_KEY_LIGHT_ONCE_6 | HAL_KEY_LIGHT_ONCE_4}; //添加人脸
static const INT16U backLightDataAddFaceUserOk[] = {
    HAL_KEY_LIGHT_ONCE_5 | HAL_KEY_LIGHT_ONCE_2 | HAL_KEY_LIGHT_ONCE_8 | HAL_KEY_LIGHT_ONCE_6 | HAL_KEY_LIGHT_ONCE_4}; //添加人脸
static const INT16U testNoError = {
    HAL_KEY_LIGHT_ONCE_M | HAL_KEY_LIGHT_ONCE_0 | HAL_KEY_LIGHT_ONCE_E }; //未检测到错误
static const INT16U backLightDataWifiConnecting[] = {
	HAL_KEY_LIGHT_ONCE_0,
	MENU_KEY_BACK_LIGHT_ALL_OFF}; //wifi连接中
/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/***************************************************************************************
**函数名:       Menu_WakeUpLight
**功能描述:     唤醒灯光处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static int8_t Menu_WakeUpLight(void)
{
    if (ekeyRandomSetEnable == st_InputInfo.keyRandomSetFlag) //按键有锁
    {
		st_InputInfo.keyRandomLockFlag = eKeyRandomLockFlagEnable; 
        Function_KeyRandomGet(); //获取随机数
        Function_KeyRandomLight();
        DEBUG_LOG_OUT("Menu_WakeUpLight ekeyRandomSetEnable\n");
    }
    else
    {
		st_InputInfo.keyRandomLockFlag = eKeyRandomLockFlagDisable; 
        DEBUG_LOG_OUT("Menu_WakeUpLight MENU_KEY_BACK_LIGHT_ALL_ON\n");
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
    }
    return 0;
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_KeyBackLightUpdate
**功能描述:     按键背光刷新
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_KeyBackLightUpdate(TYPEe_MenuIndex e_JumpMenu)
{
	uint16_t tmp=0;
    switch (e_JumpMenu)
    {
    case enum_Menu_OpenLock:
	#if 1
    case enum_Menu_CloseLock:
	#endif
    case enum_Menu_Desktop:
        Menu_WakeUpLight();
        break;
    case enum_Menu_AdminChk:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
        break;
    case enum_Menu_Admin:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
        break;
	case enum_Menu_FingerSet:
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
		break;
    case enum_Menu_AddFingerStep1:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_1);
        break;
    case enum_Menu_AddFingerStep2:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_2);
        break;
    case enum_Menu_AddFingerStep3:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_3);
        break;
    case enum_Menu_AddFingerStep4:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_4);
        break;
    case enum_Menu_AddFingerStep5:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_5);
        break;
    case enum_Menu_AddFingerStep6:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_6);
        break;
    case enum_Menu_BackDeal: //后台处理
        #if 0
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_OFF);
        #endif
        break;
    case enum_Menu_AddFaceStep1:
        HAL_KeyLightDataSet(backLightDataAddFaceStep1,
                            sizeof(backLightDataAddFaceStep1) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_AddFaceStep2:
        HAL_KeyLightDataSet(backLightDataAddFaceStep2,
                            sizeof(backLightDataAddFaceStep2) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_AddFaceStep3:
        HAL_KeyLightDataSet(backLightDataAddFaceStep3,
                            sizeof(backLightDataAddFaceStep3) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_AddFaceStep4:
        HAL_KeyLightDataSet(backLightDataAddFaceStep4,
                            sizeof(backLightDataAddFaceStep4) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_AddFaceStep5:
        HAL_KeyLightDataSet(backLightDataAddFaceStep5,
                            sizeof(backLightDataAddFaceStep5) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;

    case enum_Menu_AddFaceUserOk:
        HAL_KeyLightDataSet(backLightDataAddFaceUserOk,
                            sizeof(backLightDataAddFaceUserOk) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER_NOT,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT_NOT);
        break;
    case enum_Menu_Factory:
        HAL_KeyLight_sysStatusSet(eColorClose);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FACTORY); 
        break;
    case enum_Menu_FactoryConfirm:
        HAL_KeyLight_sysStatusSet(eColorClose);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FACTORY_ING);
        break;
	case enum_Menu_FactoryOK:
        HAL_KeyLight_sysStatusSet(eColorClose);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FACTORY_ING);
        break;
    case enum_Menu_FactoryIng:
        HAL_KeyLight_sysStatusSet(eColorClose);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FACTORY_ING);
        break;
    case enum_Menu_PryAlarm:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_X);
        HAL_KeyLight_sysStatusSet(eColorRed);
        break;
    case enum_Menu_CommError:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_X);
        HAL_KeyLight_sysStatusSet(eColorRed);
        break;
    case enum_Menu_PassWdPass:     //密码验证通过
    case enum_Menu_FingerVerifyOK: //指纹校验成功
    case enum_Menu_FaceVerifyOk:
    case enum_Menu_BleVerifyPhoneEncryptSuccess:    
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_OPEN);
        HAL_KeyLight_sysStatusSet(eColorGreen);
        break;
	case enum_Menu_BleAddFaceUserConfirm:
    case enum_Menu_APPMode:
        HAL_KeyLight_sysStatusSet(eColorClose);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_APP);
        break;
    case enum_Menu_BleAddFingerStep1:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_1);
        break;
    case enum_Menu_BleAddFingerStep2:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_2);
        break;
    case enum_Menu_BleAddFingerStep3:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_3);
        break;
    case enum_Menu_BleAddFingerStep4:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_4);
        break;
    case enum_Menu_BleAddFingerStep5:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_5);
        break;
    case enum_Menu_BleAddFingerStep6:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_6);
        break;
    case enum_Menu_BleAddFingerOK:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FINGER_STEP_6);
        break;
    case enum_Menu_StorageError:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_STORAGE_ERROR);
        break;
    case enum_Menu_PassWdFail:       //密码验证失败
    case enum_Menu_FingerVerifyFail: //指纹校验失败
    case enum_Menu_TryAlarm:
	case enum_Menu_FaceVerifyFail:
    case enum_Menu_AdminChkFail:    
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_X);
        HAL_KeyLight_sysStatusSet(eColorRed);
        break;
    case enum_Menu_FaceSet:
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
        break;
    case enum_Menu_BleAddFaceStep1:
        HAL_KeyLightDataSet(backLightDataAddFaceStep1,
                            sizeof(backLightDataAddFaceStep1) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_BleAddFaceStep2:
        HAL_KeyLightDataSet(backLightDataAddFaceStep2,
                            sizeof(backLightDataAddFaceStep2) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_BleAddFaceStep3:
        HAL_KeyLightDataSet(backLightDataAddFaceStep3,
                            sizeof(backLightDataAddFaceStep3) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_BleAddFaceStep4:
        HAL_KeyLightDataSet(backLightDataAddFaceStep4,
                            sizeof(backLightDataAddFaceStep4) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
    case enum_Menu_BleAddFaceStep5:
        HAL_KeyLightDataSet(backLightDataAddFaceStep5,
                            sizeof(backLightDataAddFaceStep5) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
        break;
	case enum_Menu_TestError:
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_OFF);
		break;
	case enum_Menu_TestErrorReport:		
		DEBUG_LOG_OUT("enum_Menu_TestErrorReport %x\n",tmp);
		if(0 == (*((int8_t *)(&st_InputInfo.modeuleFail)))) //无故障
		{
			tmp = testNoError;
		}
		else
		{
			if(1 == st_InputInfo.modeuleFail.RTC)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_1;
			}
			if(1 == st_InputInfo.modeuleFail.Finger)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_3;
			}
			if(1 == st_InputInfo.modeuleFail.Face)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_4;
			}
			if(1 == st_InputInfo.modeuleFail.hall)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_5;
			}
			if(1 == st_InputInfo.modeuleFail.Ir)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_7;
			}
			if(1 == st_InputInfo.modeuleFail.Wifi)
			{
				tmp |= HAL_KEY_LIGHT_ONCE_8;
			}
		}
		HAL_KeyLightOnce(tmp);
        break;
	case enum_Menu_WakeUp:
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_OFF);
		break;
	case enum_Menu_Bell:
	case enum_Menu_CatEyeConn:
        HAL_KeyLight_sysStatusSet(eColorClose);
		HAL_KeyLightOnce(MENU_KEY_BACK_BELL);
		break;
	case enum_Menu_BleShangTangCatEyeSingleDouble:
	case enum_Menu_WifiConnect:
		HAL_KeyLightDataSet(backLightDataWifiConnecting,
                            sizeof(backLightDataWifiConnecting) / sizeof(INT16U),
                            MENU_KEY_BACK_LIGHT_ADD_FACE_FLICKER,
                            MENU_KEY_BACK_LIGHT_ADD_FACE_REPEAT);
		break;
	case enum_Menu_CloseLockConfirm:	
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_LOCK_CONFIRM);
		break;
    case enum_Menu_KeyDefAlarm:	
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_X);
        HAL_KeyLight_sysStatusSet(eColorRed);
		break;
    case enum_Menu_ActiveDefense:	
		HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_X);
        HAL_KeyLight_sysStatusSet(eColorRed);
		break;
    case enum_Menu_FaceVerifyRetry:
        HAL_KeyLight_sysStatusSet(eColorRed);
        break;
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_KeyBackLightService
**功能描述:     按键背光服务程序
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_KeyBackLightService(void)
{
    menuKeyBackLightCtrl.runTime++;
    if (menuKeyBackLightCtrl.runTime < MENU_KEY_BACK_LIGHT_RUN_TIME_MAX)
    {
        return;
    }
    menuKeyBackLightCtrl.runTime = 0;
    HAL_KeyLightDeal();
}
#endif /*MENU_MODULE_EN*/
/************************************Code End******************************************/
