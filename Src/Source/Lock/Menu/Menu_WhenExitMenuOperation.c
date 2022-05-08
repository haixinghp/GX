/***************************************************************************************
**文件名:     Menu_WhenExitMenuOperation.c
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
#include "HAL_Finger.h"
#include "HAL_Storage.h"
#include "HAL_FaceId.h"
#if MENU_MODULE_EN
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/

/***************************************************************************************
**函数名:       fn_WhenExitMenuOperation
**功能描述:     退出菜单时执行的操作,在显示新菜单前执行
**输入参数:     TYPEe_MenuIndex e_JumpMenu
**输出参数:     --
**备注:         
****************************************************************************************/
void fn_WhenExitMenuOperation(TYPEe_MenuIndex e_JumpMenu)
{
    switch (e_JumpMenu)
    {
    case enum_Menu_Desktop:
        break;
    case enum_Menu_FirstUse:
        fn_JumpMenu(enum_Menu_AddAdminFace); //默认进入添加界面
        break;
	
    case enum_Menu_AddFingerStep1OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep2);\n");
#if 0 
        fn_JumpMenu(enum_Menu_AddFingerStep2);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN 
#endif 
        break;
    case enum_Menu_AddFingerStep2OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep3);\n");
#if 0 
        fn_JumpMenu(enum_Menu_AddFingerStep3);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN 
#endif 
        break;
    case enum_Menu_AddFingerStep3OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep4);\n");
#if 0    
        fn_JumpMenu(enum_Menu_AddFingerStep4);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN 
#endif 
        break;
    case enum_Menu_AddFingerStep4OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep5);\n");
#if 0     
        fn_JumpMenu(enum_Menu_AddFingerStep5);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN 
#endif 
        break;
    case enum_Menu_AddFingerStep5OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep6);\n");
#if 0     
        fn_JumpMenu(enum_Menu_AddFingerStep6);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN 
#endif 
        break;
	case enum_Menu_BleAddFingerStep1OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleAddFingerStep2);\n");
#if 0
        fn_JumpMenu(enum_Menu_BleAddFingerStep2);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN
#endif	
        break;
    case enum_Menu_BleAddFingerStep2OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleAddFingerStep3);\n");
#if 0
        fn_JumpMenu(enum_Menu_BleAddFingerStep3);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN
#endif	
        break;
    case enum_Menu_BleAddFingerStep3OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleAddFingerStep4);\n");
#if 0	
        fn_JumpMenu(enum_Menu_BleAddFingerStep4);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN
#endif	
        break;
    case enum_Menu_BleAddFingerStep4OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleAddFingerStep5);\n");
#if 0
        fn_JumpMenu(enum_Menu_BleAddFingerStep5);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN
#endif	
        break;
    case enum_Menu_BleAddFingerStep5OK:
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleAddFingerStep6);\n");
#if 0
        fn_JumpMenu(enum_Menu_BleAddFingerStep6);
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegReStart();
#endif // SYSTEM_FINGER_EN
#endif	
        break;
	case enum_Menu_LowPower:
		Menu_CommMsgKeyStatusMenuEnter();
		break;
	case enum_Menu_AddFaceUserFail:
		if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) 
        {
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_SystemSleep);\n");
            fn_JumpMenu(enum_Menu_SystemSleep);		//首次注册 登记失败后不是进入锁具休眠状态
        }
		break;
	case enum_Menu_FaceVerifyRetry:
		Menu_Face_PowerOnStart(0);	  //人脸上电
		HAL_FaceId_VerifyUserStart(); //人脸开始验证		
		break;
    case enum_Menu_AdminChkFail:
        if ((0 != Menu_ChkError(&st_InputInfo.errorCnt,eErrorCheckTimeNo)))
		{
			wifiSendParm.sendType = eWifiSendTypeAlarmTry;
			wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
            Menu_Face_PowerOffStart();
            
			fn_JumpMenu(enum_Menu_TryAlarm); //禁试报警 
		}	
        break;
	case enum_Menu_FaceVerifyFail:
		if ((0 != Menu_ChkError(&st_InputInfo.errorCnt,eErrorCheckTimeNo)))
		{
			wifiSendParm.sendType = eWifiSendTypeAlarmTry;
			wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
            Menu_Face_PowerOffStart();
            
			fn_JumpMenu(enum_Menu_TryAlarm); //禁试报警 
		}	
		break;	
    case enum_Menu_OpenLockSuccess:    
        if((0 == st_InputInfo.insideOpenLockFlag))      //一键布防开启后门外开门未取消一键布防
        {
            if(1 == storageSave.sysDat.keyDef)  
            {
                storageSave.sysDat.keyDef = 0;
                HAL_Storage_WriteSysData(&storageSave.sysDat);
            }
        }
        if((1 == st_InputInfo.insideOpenLockFlag) && (1 == storageSave.sysDat.keyDef)) //打开了一键布防并确认是后板开的门。
        {
            st_InputInfo.insideOpenLockFlag = 0; //清除开门标志
            fn_JumpMenu(enum_Menu_KeyDefAlarm); //进入一键布防
            
            wifiSendParm.sendType = eWifiSendTypeAlarmProtection;
			wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
            
            Menu_Face_PowerOffStart();
        }
        else if(1 == st_InputInfo.insideOpenLockFlag)
        {
            st_InputInfo.insideOpenLockFlag = 0; //清除开门标志
            fn_JumpMenu(enum_Menu_Desktop); //进入桌面，方便用于开门
            st_MenuItem.u16_KeepTime = M_MENU_DISPLAY_TIMER_LEVEL_10; //工作10秒的时间
        }
        break;
    case enum_Menu_WifiConnect:
        Menu_Face_PowerOffStart();
        Menu_EnterDesktopVerifyStart();
        break;
    default:
        Menu_ClearInputInfo(); //切换界面时，清楚非必要的数据
        break;
    }
}

#endif
/************************************Code End******************************************/
