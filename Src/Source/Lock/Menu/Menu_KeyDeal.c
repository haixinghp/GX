/***************************************************************************************
**文件名:	  Menu_KeyDeal.c
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间
**
**备注:
****************************************************************************************/

#include "menu.h"
#include "HAL_KeyLight.h"
#include "HAL_KeySacn.h"
#include "HAL_Voice.h"
#include "HAL_Comm.h"
#include "HAL_Storage.h"
#if MENU_MODULE_EN
//#warning MENU_MODULE_EN模块使能!
/*******************************宏定义***************************/
#define MENU_MODULE_KEY_DEAL_RUN_TIME_MAX (20) //运行时间
//#define DELAY_MS(X) 
// #define MENU_KEY_ALL_ENABLE (0X0000) //全部使能
// #define MENU_KEY_ALL_DISABLE (0XFFFF) //全部失去能
// #define MENU_KEY_EN

/*************************.C变量结构声明***********************/
typedef struct
{
    INT32U runTime; //运行时间
} TYPEs_keyDealCtrl;

typedef struct
{
    INT16U key0 : 1;
    INT16U key1 : 1;
    INT16U key2 : 1;
    INT16U key3 : 1;
    INT16U key4 : 1;
    INT16U key5 : 1;
    INT16U key6 : 1;
    INT16U key7 : 1;
    INT16U key8 : 1;
    INT16U key9 : 1;
    INT16U keyMenu : 1;
    INT16U keyEnter : 1;
    INT16U keyClose : 1;
    INT16U keyBell : 1;
    INT16U reserved : 2;
} TYPEs_KeyEnbale;
/*************************私有全局变量*************************/
static TYPEs_keyDealCtrl keyDealCtrl =
    {
        .runTime = 0,
};
const TYPEs_KeyEnbale keyEnbaleOnlyRetrun = {
    .key0 = 1,
    .key1 = 1,
    .key2 = 1,
    .key3 = 1,
    .key4 = 1,
    .key5 = 1,
    .key6 = 1,
    .key7 = 1,
    .key8 = 1,
    .key9 = 1,
    .keyMenu = 0,
    .keyEnter = 1,
    .keyClose = 1,
    .keyBell = 1,

}; 
const TYPEs_KeyEnbale keyEnbaleBellDis = {
    .key0 = 0,
    .key1 = 0,
    .key2 = 0,
    .key3 = 0,
    .key4 = 0,
    .key5 = 0,
    .key6 = 0,
    .key7 = 0,
    .key8 = 0,
    .key9 = 0,
    .keyMenu = 0,
    .keyEnter = 0,
    .keyClose = 0,
    .keyBell = 1,

}; 
const TYPEs_KeyEnbale keyFactory = {
    .key0 = 1,
    .key1 = 1,
    .key2 = 1,
    .key3 = 1,
    .key4 = 1,
    .key5 = 1,
    .key6 = 1,
    .key7 = 1,
    .key8 = 1,
    .key9 = 1,
    .keyMenu = 0,
    .keyEnter = 0,
    .keyClose = 1,
    .keyBell = 1,

};
TYPEs_KeyEnbale keyEable;
typedef enum
{
    eKeyDispModeAll,  //全部
    eKeyDispModeFactory,  //出厂设置模式
	eKeyDispModeBell, //门铃 
} TYPEe_KeyDispMode;
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_KeyDeal_IsEnable
**功能描述:     判断按键时候被使能
**输入参数:     
INT16U en,
1111 1111 1111 1110 表示只有0号按键被使能
INT8U key 获取到的按键值
**输出参数:  
INT8S
0; //该按键被使能
-1; //该按键被禁止
-2; //参数错误
**备注:         
****************************************************************************************/
static INT8S Menu_KeyDeal_IsEnable(INT16U *en, INT8U key)
{
    INT16U tmp = 0;
    if (NULL == en)
    {
        return -2; //参数错误
    }
    tmp = *en;
    tmp = (tmp >> key) & 0x0001;
    if (0x0001 == tmp)
    {
        return -1; //该按键被禁止
    }
    else
    {
        return 0; //该按键被使能
    }
}
/***************************************************************************************
**函数名:       Menu_KeyDeal_VoiceAndLight
**功能描述:     语音和灯光处理
**输入参数:     
INT8U enable, 使能灯光
INT8U keyVal, 键盘值
INT16U dat 灯光键值
**输出参数:  	--
**备注:         
****************************************************************************************/
static void Menu_KeyDeal_VoiceAndLight(INT8U enable,INT8U keyVal,INT16U dat)
{
	HAL_VoicePlayDir(eVoiceDirButtonDrip); //按键音处理
	if(0 != enable)
	{
		HAL_KeyLightPressKey(keyVal,dat); 		//背光	
	}
	Menu_KeepTimeRefresh();                //工作时间刷新
}

/***************************************************************************************
**函数名:       Menu_KeyDeal_CloseLock
**功能描述:     关锁
**输入参数:     
INT8U keyVal
**输出参数:  	--
**备注:         
****************************************************************************************/
static void Menu_KeyDeal_CloseLock(INT8U keyVal)
{
    if (MENU_KEY_CLOSE == keyVal)
    {
		if(0 == storageSave.sysDat.bellDef)
		{
            
            Menu_Face_PowerOffStart(); //按锁门键人脸下电
			fn_JumpMenu(enum_Menu_CloseLock);
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_CloseLock);"); //进入开锁状态。
			Menu_KeyDeal_VoiceAndLight(0,0,0);
		}
		else
		{
			#if 0
			Menu_KeyDeal_VoiceAndLight(1,0,MENU_KEY_BACK_LIGHT_LOCK_CONFIRM);
			#endif
            Menu_Face_PowerOffStart(); //按锁门键人脸下电
			Menu_KeyDeal_VoiceAndLight(0,0,0);
			fn_JumpMenu(enum_Menu_CloseLockConfirm); //进入锁门键确认。
		}
		
    }
}



/*************************全局函数****************************/
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_KeyDealService(void)
{
    INT8U keyTmp = 0xff;
	static INT8U lastKey = 0xff;
    keyDealCtrl.runTime++;
    if (keyDealCtrl.runTime < MENU_MODULE_KEY_DEAL_RUN_TIME_MAX)
    {
        return;
    }
    keyDealCtrl.runTime = 0;
    keyTmp = HAL_KeySacnGet(); //获取按键值
    if (0xff != keyTmp)
    {
		if(keyTmp == lastKey)
		{
			return; //按下一次只处理一次
		}
		lastKey = keyTmp;
		HAL_Voice_Bell(keyTmp); //任何界面都可以触发门铃
        if (enum_Window_NoResponseNotify != struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
        {
            switch (st_MenuItem.e_CurrentIndex)
            {
            case enum_Menu_Desktop:
				if(MENU_KEY_BELL == keyTmp) //按下了门铃
				{
                    if(MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG == storageSave.sysDat.wifiMainSw)     //检查wifi主开关开启
                    {
                        HAL_VoicePlayDir(eVoiceDirCallingWait); //呼叫中，请等待
                        fn_JumpMenu(enum_Menu_Bell);
                        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_Bell);");
                    }
				}
#if defined(KEY_RANDOM_LOCK_EN)
                if (eKeyRandomLockFlagDisable == st_InputInfo.keyRandomLockFlag) //按键解锁后
                {
                    //键盘锁定的状态，灯光和按键音都不处理
					memset((uint8_t *)&keyEable,0x00,sizeof(keyEable)); //设置为全部有效
					keyEable.keyClose = 1; //解锁后锁门键失效
					keyEable.keyBell = 1; //门铃不响应
                    if (0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))
                    {
                        Menu_KeyDeal_VoiceAndLight(1,keyTmp,MENU_KEY_BACK_LIGHT_ALL_ON);
                    }
                }
				else 
				{
					Menu_KeyDeal_CloseLock(keyTmp);
				}
                if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) //演示模式
                {
                    Menu_KeyDeal_CloseLock(keyTmp);
                }
#else
                Menu_KeyDeal_VoiceAndLight(1,keyTmp,MENU_KEY_BACK_LIGHT_ALL_ON);
#endif //KEY_RANDOM_LOCK_EN
                break;
            case enum_Menu_AddFaceStep1:   //添加人脸过程
            case enum_Menu_AddFaceStep2:   //添加人脸过程2
            case enum_Menu_AddFaceStep3:   //添加人脸过程3
            case enum_Menu_AddFaceStep4:   //添加人脸过程4
            case enum_Menu_AddFaceStep5:   //添加人脸过程5
            case enum_Menu_AddFingerStep1: //添加指纹过程
            case enum_Menu_AddFingerStep2: //添加指纹过程2
            case enum_Menu_AddFingerStep3: //添加指纹过程3
            case enum_Menu_AddFingerStep4: //添加指纹过程4
            case enum_Menu_AddFingerStep5: //添加指纹过程5
            case enum_Menu_AddFingerStep6: //添加指纹过程6
				memset((uint8_t *)&keyEable,0xff,sizeof(keyEable)); //设置为全部无效
				keyEable.keyMenu = 0; //设置*号键有效
                if ((0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))&&
                    (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag)) //未注册管理员的情况下禁止返回
                {
                    Menu_KeyDeal_VoiceAndLight(1,keyTmp,MENU_KEY_BACK_LIGHT_ALL_ON);
                }
                else
                {
                    keyTmp = MENU_KEY_NOKEY; //清除按键
                }
                break;
            case enum_Menu_PryAlarm:
                break;
            case enum_Menu_LowPower:
                break;
            case enum_Menu_AdminChk:
                break;
			case enum_Menu_FirstUse:
                break;
			case enum_Menu_Factory:
				memset((uint8_t *)&keyEable,0xff,sizeof(keyEable)); //设置为全部无效
				keyEable.keyMenu = 0; //设置*号键有效
				if (0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))
                {
                    Menu_KeyDeal_VoiceAndLight(0,keyTmp,0);
                }
                memset((uint8_t *)&keyEable,0xff,sizeof(keyEable)); //设置为全部无效
                keyEable.keyEnter = 0; //设置#号键有效
                if (0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))
                {
                    HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_FACTORY_ING); //把#号键按灭
                }
                break;
			case enum_Menu_Bell:
			case enum_Menu_CatEyeConn:	
				memset((uint8_t *)&keyEable,0xff,sizeof(keyEable)); //设置为全部无效
				keyEable.keyMenu = 0; //设置*号键有效
				if (0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))
                {
                    Menu_KeyDeal_VoiceAndLight(0,keyTmp,0);
                }
				if(eConnectWaitOverNo == wifiSendParm.connectWaitOver) //没有超过最小连接时间，不能退出。
				{
					keyTmp = MENU_KEY_NOKEY; //清除按键
				}
                break;
			case enum_Menu_CloseLockConfirm: //锁门键确认
				memset((uint8_t *)&keyEable,0xff,sizeof(keyEable)); //设置为全部无效
				keyEable.keyEnter = 0; //设置#号键有效
				if (0 == Menu_KeyDeal_IsEnable((INT16U *)(&keyEable), keyTmp))
                {
                    Menu_KeyDeal_VoiceAndLight(1,keyTmp,MENU_KEY_BACK_LIGHT_LOCK_CONFIRM);
					fn_JumpMenu(enum_Menu_CloseLock);
					DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_CloseLock);"); //进入开锁状态。
                }
				keyTmp = MENU_KEY_NOKEY; //清除按键
				break;
            default:
                Menu_KeyDeal_VoiceAndLight(1,keyTmp,MENU_KEY_BACK_LIGHT_ALL_ON);
                break;
            }
            if ((enum_Menu_Admin == st_MenuItem.e_CurrentIndex) && (keyTmp == MENU_KEY_MENU))
            {
                /*
			不做处理,这在本地菜单下，管理页面用*号键不能退出。
			只能按菜单播报提示的退出按键和超时退出
			*/
            }
            else
            {
                Menu_KeyForMenu(keyTmp); //按键菜单处理
            }
        }
    }
	else
	{
		lastKey = 0xff;
	}
}
#endif /*MENU_MODULE_EN*/
/************************************Code End******************************************/
