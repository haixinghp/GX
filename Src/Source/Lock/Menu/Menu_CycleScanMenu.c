/***************************************************************************************
**文件名:     Menu_CycleScanMenu.c
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
#include "HAL_Power.h"
#include "APP_Power.h"
#include "HAL_SysTimer.h"
#include "HAL_Finger.h"
#include "HAL_KeyLight.h"
#include "HAL_FaceId.h"
#include "HAL_Storage.h"
#include "HAL_Alarm.h"
#include "HAL_Voice.h"
#include "Function.h"
#include "HAL_Comm.h"
#include "HAL_IR.h"
#if MENU_MODULE_EN
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_CycleScanMenu
**功能描述:     菜单中周期性处理的事件处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_CycleScan_BleToolTestFail(void)
{
	
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_CycleScanMenu
**功能描述:     菜单中周期性处理的事件处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_CycleScan(void)
{
	//    static INT8U u8_Timer = 0;

	//检测恢复出厂设置
	if (0 == st_InputInfo.pryAlarmEnableFlag)
	{
		if ((0 == st_InputInfo.pryAlarmFlag) && (0 == HAL_Alarm_GetStatus()))
		{
			st_InputInfo.pryAlarmFlag = 1;
			st_InputInfo.pryAlarmTimeCnt = 0; //重新开始计数
			fn_JumpMenu(enum_Menu_PryAlarm); //进入撬锁报警
			wifiSendParm.sendType = eWifiSendTypeAlarmPry; //撬锁报警
			wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
            Menu_Face_PowerOffStart();
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_PryAlarm);\n");
		}
		else if ((1 == HAL_Alarm_GetStatus()) && (1 == st_InputInfo.pryAlarmFlag))
		{
			HAL_VoicePlayDir(eVoiceDirStop);
			st_InputInfo.pryAlarmFlag = 0;
			Menu_RecordLockLog(eLogTypeAlarm,NULL,0);  //保存事件记录
			fn_JumpMenu(enum_Menu_SystemSleep);
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_SystemSleep); \n");
		}
	}

	switch (st_MenuItem.e_CurrentIndex)
	{
	case enum_Menu_SystemSleep:
		if(0 != HAL_FaceId_ChkPowerBusy()) //人脸电源未处理完成
		{
			return;
		}
		(*(volatile UINT32 *)(0x08100300)) = 0x55aa55aa;
		Function_SysRamBackup();		//保存系统数据
		APP_Power_EnterSleepConfig();	//进入休眠配置
		APP_Power_EnterSleep();			//进入休眠
		break;
	case enum_Menu_BackDeal:
		if (0 != HAL_KeyLight_ChkBusy()) //检查灯光时候完成工作
		{
			return;
		}
		if (0 != HAL_Voice_ChkBusy()) //检查语音是否完成
		{
			return;
		}
		if( eCommBusyYes == HAL_Comm_ChkBusy())  //前后通讯完成
		{
			return;
		}
        #if 0
		if(0 != HAL_FaceId_ChkBusy())
		{
			return;
		}
        #endif
        #if 1
		if(0 !=  Menu_FaceId_WaitComplete()) //人脸数据未处理完成
		{
			return;
		}
		#endif
        #if 1
		if(0 !=  Menu_WaitWifiComplete()) //wifi未处理完成
		{
			return;
		}
		#endif
		DEBUG_LOG_OUT("Menu_CycleScan fn_JumpMenu(enum_Menu_SystemSleep)\n");
		//检查后台wifi是否完成
        
		fn_JumpMenu(enum_Menu_SystemSleep); //系统进入休眠
		break;
	case enum_Menu_Desktop:
        if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance)) //判断系统是否打开了主动防御
        {
            if((1 == HAL_BuFang_Time_Detet(st_InputInfo.defEn,(uint32_t *)&st_InputInfo.defSysTickCnt)))
            {
                DEBUG_LOG_OUT("st_InputInfo.defSysTickCnt：%X\n",st_InputInfo.defSysTickCnt);
                DEBUG_LOG_OUT("st_InputInfo.defEn%d\n",st_InputInfo.defEn);
                Menu_Face_PowerOffStart();	 //人脸下电
                HAL_FaceId_VerifyUserStop(); //关闭人脸模块
                wifiSendParm.sendType = eWifiSendTypeAlarmActiveDefense; //主动防御报警
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
                DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ActiveDefense)\n");
                fn_JumpMenu(enum_Menu_ActiveDefense);//进入主动防御状态
            }
        }
        
		break;
	case enum_Menu_BleEquipmentReg:													   //恢复出厂中
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
            Menu_Face_PowerOffStart(); //人脸下电
			DEBUG_LOG_OUT("enum_Menu_BleEquipmentReg clear error!\n");
             HAL_Comm_BleSendDir(eBleEquipmentReg);	//发送数据
			fn_JumpMenu(enum_Menu_APPMode); //恢复到APP模式
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFace) //指纹清空成功
		{
			break;
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFace) //指纹清空成功
		{
			break;
		}
        HAL_Comm_BleSendDir(eBleEquipmentReg);	//发送数据
        Menu_Face_PowerOffStart(); //人脸下电
		fn_JumpMenu(enum_Menu_APPMode); //恢复到APP模式
		break;
	case enum_Menu_BleEmptyUser:													   //恢复出厂中
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
			DEBUG_LOG_OUT("enum_Menu_BleEmptyUser clear error!\n");
			pBleAck->ack = eBleAckSuccess; 
			HAL_Comm_BleSendDir(eBleEmptyUser); //清空失败，也报成功。
			fn_JumpMenu(enum_Menu_APPMode); //恢复到APP模式
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFinger) //指纹清空成功
		{
			break;
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFace) //人脸清空成功
		{
			break;
		}
		pBleAck->ack = eBleAckSuccess;
		HAL_Comm_BleSendDir(eBleEmptyUser);
        Menu_Face_PowerOffStart(); //人脸下电
		fn_JumpMenu(enum_Menu_APPMode); //恢复到APP模式
		break;	
	case enum_Menu_FactoryIng:														   //恢复出厂中
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
			fn_JumpMenu(enum_Menu_FactoryFail); //清空失败
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFinger) //指纹清空成功
		{
			break;
		}
		if (eEquipResultSuccess != st_InputInfo.emptyFace) //人脸清空成功
		{
			break;
		}
		if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag) //检查是否注册了管理员
		{
			msgDemoModeoData.enable = 0;
		}
		else
		{
			msgDemoModeoData.enable = 1;
		}
		HAL_FaceId_SetDemoMode();		  //设置系统模式
		fn_JumpMenu(enum_Menu_FactoryOK); //清空成功
        Menu_Face_PowerOffStart(); //人脸下电
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FactoryOK);");
	case enum_Menu_PryAlarm:			  //撬锁报警
		st_InputInfo.pryAlarmTimeCnt++;
		if (st_InputInfo.pryAlarmTimeCnt >= PRE_ALARM_TIME_LIMIT)
		{
			st_InputInfo.pryAlarmEnableFlag = 1; //报警失能
			HAL_VoicePlayDir(eVoiceDirStop);
			fn_JumpMenu(enum_Menu_SystemSleep);
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_SystemSleep);  \n");
		}
		break;
	case enum_Menu_BleAddFingerConfirm:
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
			DEBUG_LOG_OUT("enum_Menu_BleAddFingerConfirm Time over\n");
			HAL_Finger_DelStart(); //开启指令
			HAL_Storage_DelOneUserReg(st_InputInfo.FingerIdTmp,
									  MSG_FINGER_REG_MEM_LOCAL_ST,
									  MSG_FINGER_ONE_SIZE); //删除本地ID
			st_InputInfo.userIdTmp = BSWAP_16(st_InputInfo.FingerIdTmp);
			Menu_RecordLockLog(eLogTypeDelFinger,
							   (uint8_t *)&(st_InputInfo.userIdTmp),
                               sizeof(st_InputInfo.userIdTmp)); //保存事件记录
			fn_JumpMenu(enum_Menu_BleAddFingerConfirmFail);
		}
		break;
	case enum_Menu_BleAddFaceUserConfirm:
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
			DEBUG_LOG_OUT("enum_Menu_BleAddFaceUserConfirm Time over\n");
			fn_JumpMenu(enum_Menu_BleAddFaceUserFail);
			Menu_Face_PowerOnStart(0);
			HAL_FaceId_DelOneUserStart(st_InputInfo.userID);
			HAL_Storage_DelOneUserReg(st_InputInfo.userID,
									  MSG_FACE_REG_MEM_ST,
									  MSG_FACE_REG_SIZE); //删除本地ID
			st_InputInfo.userIdTmp = BSWAP_16(st_InputInfo.userID);
			Menu_RecordLockLog(eLogTypeDelFace,
							   (uint8_t *)&(st_InputInfo.userIdTmp),
                               sizeof(st_InputInfo.userIdTmp)); //保存事件记录
		}
		break;
	case enum_Menu_APPMode:
		if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
			DEBUG_LOG_OUT("enum_Menu_APPMode Time over\n");
			HAL_Comm_BleSendDir(eBleDisconnectBluetooth);
			HAL_Comm_SendDealStart(eCommMsgDisconnectBLE,200); //发送指令
			fn_JumpMenu(enum_Menu_BackDeal);   //进入后台处理程序
		}
		break;
	case enum_Menu_BleToolTestFail:
		
		break;	
	case enum_Menu_BleInfraredMonitoringDistanceSet:
		if(eIrSetDistanceSuccss == HAL_IR_SetDistance())
		{
			HAL_IR_UartColse(); 
			pBleAck->ack = eBleAckSuccess;
			HAL_Comm_BleSendDirWithLen(eBleInfraredMonitoringDistanceSet,sizeof(TYPEs_BleAck));
			fn_JumpMenu(enum_Menu_APPMode); //恢复到APP模式
		}
		break;
    case enum_Menu_AddFingerStep1: //添加指纹过程
	case enum_Menu_AddFingerStep2: //添加指纹过程2
	case enum_Menu_AddFingerStep3: //添加指纹过程3
	case enum_Menu_AddFingerStep4: //添加指纹过程4
	case enum_Menu_AddFingerStep5: //添加指纹过程5
	case enum_Menu_AddFingerStep6: //添加指纹过程6
    case enum_Menu_AddFingerStep1OK:
	case enum_Menu_AddFingerStep2OK:
	case enum_Menu_AddFingerStep3OK:
	case enum_Menu_AddFingerStep4OK:
	case enum_Menu_AddFingerStep5OK:
        if (st_MenuItem.u16_KeepTime <= (2 * (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))) //使用菜单计时器
		{
            DEBUG_LOG_OUT("add finger over time\n");
            fn_JumpMenu(enum_Menu_AddFingerUserFail);
        }
        break;
	case enum_Menu_TestError:
		if(1 == st_InputInfo.modeuleFail.Ir)
		{
			if(0 == HAL_IR_ReadIRQ())
			{
				st_InputInfo.modeuleFail.Ir = 0;
				DEBUG_LOG_OUT("IR is ok\n");
				HAL_VoicePlayDir(eVoiceDirButtonDrip); //按键音处理
				//发出声音
			}
		}
		
        break;		
	default:
		break;
	}
}
#endif
/************************************Code End******************************************/
