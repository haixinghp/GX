/***************************************************************************************
**文件名:     Menu_Finger.c
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
#include "HAL_Storage.h"
#include "HAL_Finger.h"
#include "HAL_Comm.h"
#include "HAL_FaceId.h"
#include "HAL_SysTimer.h"
#if MENU_MODULE_EN
//#warning MENU_MODULE_EN模块使能!
/*******************************宏定义***************************/
#define MENU_FINGER_RUN_TIME_MAX (20) //运行时间
#define MENU_FINGER_REG_RESTART_TIME (100) //注册过程中，重新开始时间
//#define MENU_FINGER_RUN_TIME_MAX				(100)		//运行时间 测试
/*************************.C变量结构声明***********************/
typedef struct
{
	INT32U runTime; //运行时间
	INT32U regRestartTime; //注册过程中，重新开始时间
} TYPEs_MenuFingerCtrl;
/*************************私有全局变量*************************/
static TYPEs_MenuFingerCtrl menuFingerCtrl =
	{
		.runTime = 0,
};
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_Finger_VerifyOk
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_VerifyOk(void)
{
	struct tm timeStartTmp;
	struct tm timeStopTmp;
	memset((uint8_t *)&storageSave.UserDat, 0, sizeof(storageSave.UserDat)); //清空数据包
	HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
							  MSG_FINGER_ONE_SIZE,
							  st_InputInfo.userID,
							  &storageSave.UserDat); //读取参数
	if (MSG_FINGERREGMEM_FIG == storageSave.UserDat.vaild)
	{
		if (MENU_FUNCTION_ON == storageSave.UserDat.TimeVaild.fig)
		{
			DEBUG_LOG_OUT("TimeVaild.fig ok\n");
			timeStartTmp.tm_year = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.year) + 2000;
			timeStartTmp.tm_mon = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.mon) - 1;
			timeStartTmp.tm_mday = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.day);
			timeStartTmp.tm_hour = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.hour);
			timeStartTmp.tm_min = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.min);
			timeStartTmp.tm_sec = BCD_TO_BIN(storageSave.UserDat.TimeVaild.str.sec);
			timeStopTmp.tm_year = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.year) + 2000;
			timeStopTmp.tm_mon = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.mon) - 1;
			timeStopTmp.tm_mday = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.day);
			timeStopTmp.tm_hour = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.hour);
			timeStopTmp.tm_min = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.min);
			timeStopTmp.tm_sec = BCD_TO_BIN(storageSave.UserDat.TimeVaild.stp.sec);
            #if 1
            DEBUG_LOG_OUT("timeStartTmp.tm_year %d\n",timeStartTmp.tm_year);
            DEBUG_LOG_OUT("timeStartTmp.tm_mon %d\n",timeStartTmp.tm_mon);
            DEBUG_LOG_OUT("timeStartTmp.tm_mday %d\n",timeStartTmp.tm_mday);
            DEBUG_LOG_OUT("timeStartTmp.tm_hour %d\n",timeStartTmp.tm_hour);
            DEBUG_LOG_OUT("timeStartTmp.tm_min %d\n",timeStartTmp.tm_min);
            DEBUG_LOG_OUT("timeStartTmp.tm_sec %d\n",timeStartTmp.tm_sec);
            DEBUG_LOG_OUT("\n");
            
            
            DEBUG_LOG_OUT("timeStopTmp.tm_year %d\n",timeStopTmp.tm_year);
            DEBUG_LOG_OUT("timeStopTmp.tm_mon %d\n",timeStopTmp.tm_mon);
            DEBUG_LOG_OUT("timeStopTmp.tm_mday %d\n",timeStopTmp.tm_mday);
            DEBUG_LOG_OUT("timeStopTmp.tm_hour %d\n",timeStopTmp.tm_hour);
            DEBUG_LOG_OUT("timeStopTmp.tm_min %d\n",timeStopTmp.tm_min);
            DEBUG_LOG_OUT("timeStopTmp.tm_sec %X",timeStopTmp.tm_sec);  
            DEBUG_LOG_OUT("\n");
            
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_year %d\n",st_InputInfo.RTCTimer.tm_year);
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_mon %d\n",st_InputInfo.RTCTimer.tm_mon);
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_mday %d\n",st_InputInfo.RTCTimer.tm_mday);
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_hour %d\n",st_InputInfo.RTCTimer.tm_hour);
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_min %d\n",st_InputInfo.RTCTimer.tm_min);
            DEBUG_LOG_OUT("st_InputInfo.RTCTimer.tm_sec %d\n",st_InputInfo.RTCTimer.tm_sec);
            DEBUG_LOG_OUT("\n");
            #endif
			if (0 == Menu_CheckTimeVaild(&st_InputInfo.RTCTimer,
										 &timeStartTmp,
										 &timeStopTmp,
										 storageSave.UserDat.TimeVaild.wday))
			{
				DEBUG_LOG_OUT("TimeVaild enter\n");
				Menu_RecordLockLog(eLogTypeOpenByFinger,
								   (uint8_t *)&(st_InputInfo.userIdTmp),
								   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                #if 1
                memset((uint8_t *)pWifiCmdUnlockRealUpdata,
                        0,
                        sizeof(TYPEs_WifiCmdUnlockRealUpdata));
                #endif
				wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
				if(1 == storageSave.UserDat.kindred)
                {
                    wifiSendParm.limt |= MENU_UNLOCK_ATTR_KINDNESS;
                }
                if(1 == storageSave.UserDat.sos)
                {
                    wifiSendParm.limt |= MENU_UNLOCK_ATTR_SOS;
                }
                if (eCombineTypeAND == Menu_CheckCombine()) //与模式
                {
                    #define INDEX (2)
                    if(1 == storageSave.UserDat.kindred)
                    {
                        wifiSendParm.pageId[INDEX] |= MENU_UNLOCK_ATTR_KINDNESS;
                    }
                    if(1 == storageSave.UserDat.sos)
                    {
                        wifiSendParm.pageId[INDEX] |= MENU_UNLOCK_ATTR_SOS;
                    }
                    wifiSendParm.pageId[INDEX+1] =(uint8_t)storageSave.UserDat.pageID;
                    #undef INDEX
                }    
                else
                {
                    wifiSendParm.unlockType = eWifiunlockTypeFinger;
                    wifiSendParm.pageId[0] = (uint8_t)(0);
                    wifiSendParm.pageId[1] = (uint8_t)(0);
                    wifiSendParm.pageId[2] = (uint8_t)(st_InputInfo.userID >> 8);
                    wifiSendParm.pageId[3] = (uint8_t)(st_InputInfo.userID); //PageID
                }
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
                
				fn_JumpMenu(enum_Menu_FingerVerifyOK); //指纹校验成功
			}
			else
			{
				DEBUG_LOG_OUT("TimeVaild over\n");
				fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
			}
		}
		else //未开启时效
		{
			Menu_RecordLockLog(eLogTypeOpenByFinger,
							   (uint8_t *)&(st_InputInfo.userIdTmp),
							   sizeof(st_InputInfo.userIdTmp)); //保存事件记录

			wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
            if(1 == storageSave.UserDat.kindred)
            {
                wifiSendParm.limt |= MENU_UNLOCK_ATTR_KINDNESS;
            }
            if(1 == storageSave.UserDat.sos)
            {
                wifiSendParm.limt |= MENU_UNLOCK_ATTR_SOS;
            }
            if (eCombineTypeAND == Menu_CheckCombine()) //与模式
            {
                #define INDEX (2)
                if(1 == storageSave.UserDat.kindred)
                {
                    wifiSendParm.pageId[INDEX] |= MENU_UNLOCK_ATTR_KINDNESS;
                }
                if(1 == storageSave.UserDat.sos)
                {
                    wifiSendParm.pageId[INDEX] |= MENU_UNLOCK_ATTR_SOS;
                }
                wifiSendParm.pageId[INDEX+1] =(uint8_t)storageSave.UserDat.pageID;
                #undef INDEX
            }    
            else
            {
                wifiSendParm.unlockType = eWifiunlockTypeFinger;
                wifiSendParm.pageId[0] = (uint8_t)(0);
                wifiSendParm.pageId[1] = (uint8_t)(0);
                wifiSendParm.pageId[2] = (uint8_t)(st_InputInfo.userID >> 8);
                wifiSendParm.pageId[3] = (uint8_t)(st_InputInfo.userID); //PageID
            }
            wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
            
			fn_JumpMenu(enum_Menu_FingerVerifyOK); //指纹校验成功
		}
	}
	else
	{
		DEBUG_LOG_OUT("finger error\n");
		fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_Desktop
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_Desktop(void)
{
	uint16_t IdTmp = 0;
	if (eFaceRunEnable == st_InputInfo.faceRun)
	{
		if (0 == HAL_Finger_HavePress() && (eCombineTypeOR == Menu_CheckCombine()))
		{
			st_InputInfo.faceRun = eFaceRunDisable;
			DEBUG_LOG_OUT("Menu_Face_PowerOffStart 0 == HAL_Finger_HavePress()\n");
			Menu_Face_PowerOffStart();	 //人脸下电
			HAL_FaceId_VerifyUserStop(); //关闭人脸模块
			HAL_Finger_VerifyStart();	 //打开指纹
		}
	}
	switch (HAL_Finger_Verify(&IdTmp))
	{
	case 1:
		DEBUG_LOG_OUT("Finger_Verify Step 1\n");
		break;
	case 2:
		DEBUG_LOG_OUT("Finger_Verify Step 2\n");
#if defined(SYSTEM_STORAGE_EN)
//			if (0 == APP_Storage_lockRegStatusGet()) //检查系统是否注册
//			{
//				fn_JumpMenu(enum_Menu_TryFingerVerifyOK); //试用模式通过
//			}
#endif

		break;
	case 3:
		DEBUG_LOG_OUT("Finger_Verify Step 3\n");
		break;
	case -3:
		DEBUG_LOG_OUT("Finger_Verify Step -3\n");
		break;
	case 4:
		DEBUG_LOG_OUT("Finger_Verify Step 4 find ok\n");
		DEBUG_LOG_OUT("ID is=%d\n", IdTmp);
		st_InputInfo.userIdTmp = BSWAP_16(IdTmp);
		st_InputInfo.userID = IdTmp;
		Menu_Finger_VerifyOk();
		break;
	case -4:
		DEBUG_LOG_OUT("Finger_Verify Step -4 find error\n");
		if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) //如果在演示模式，直接开门
		{
			fn_JumpMenu(enum_Menu_FingerVerifyOK); //指纹校验成功
		}
		else
		{
			fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
		}
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       Menu_Finger_AdminChk
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_AdminChk(void)
{
	uint16_t IdTmp = 0;
	switch (HAL_Finger_Verify(&IdTmp))
	{
	case 1:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step 1\n");
		break;
	case 2:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step 2\n");
		break;
	case 3:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step 3\n");
		break;
	case -3:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step -3\n");
		fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
		break;
	case 4:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step 4 find ok\n");
		DEBUG_LOG_OUT("ID is=%d\n", IdTmp);
		//		if (0 == HAL_Storage_AdminFingerIdChk(IdTmp)) //检查是否是管理指纹
		//		{
		//			fn_JumpMenu(enum_Menu_Admin); //进入管理模式
		//		}
		//		else
		//		{
		//			fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
		//		}
		break;
	case -4:
		DEBUG_LOG_OUT("enum_Menu_AdminChk Step -4 find error\n");
		fn_JumpMenu(enum_Menu_FingerVerifyFail); //指纹校验失败
		//验证错误
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       Menu_Finger_AddFingerStep
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_AddFingerStep(void)
{
	switch (HAL_Finger_Reg(st_InputInfo.FingerIdTmp))
	{
	case 1:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 1\n");
		break;
	case 2:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 2\n");
		break;
	case 3:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 3\n");
		menuFingerCtrl.regRestartTime = MENU_FINGER_REG_RESTART_TIME;
		switch (HAL_Finger_SaveBufGet())
		{
		case 2:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep1OK);\n");
			fn_JumpMenu(enum_Menu_AddFingerStep1OK);
			break;
		case 3:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep2OK);\n");
			fn_JumpMenu(enum_Menu_AddFingerStep2OK);
			break;
		case 4:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep3OK);\n");
			fn_JumpMenu(enum_Menu_AddFingerStep3OK);
			break;
		case 5:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep4OK);\n");
			fn_JumpMenu(enum_Menu_AddFingerStep4OK);
			break;
		case 6:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddFingerStep5OK);\n");
			fn_JumpMenu(enum_Menu_AddFingerStep5OK);
			break;
		default:
			break;
		}
		break;
	case -3:
        HAL_SysTimer_ClearCnt();
		fn_JumpMenu(enum_Menu_AddFingerUserFail);
		DEBUG_LOG_OUT("eenum_Menu_AddFingerUserFail -3\n");
		break;
	case 4:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 4\n");
		break;
	case -4:
        HAL_SysTimer_ClearCnt();
		fn_JumpMenu(enum_Menu_AddFingerUserFail);
		DEBUG_LOG_OUT("eenum_Menu_AddFingerUserFail -4\n");
		break;
	case 5:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 5\n");
		break;
	case -5:
         HAL_SysTimer_ClearCnt();
		fn_JumpMenu(enum_Menu_AddFingerUserFail);
		DEBUG_LOG_OUT("eenum_Menu_AddFingerUserFail -5\n");
		break;
	case 6:
        HAL_SysTimer_ClearCnt();
		fn_JumpMenu(enum_Menu_AddFingerUserOk); //进入添加成功
		DEBUG_LOG_OUT("enum_Menu_AddFingerUserOk 6\n");
		break;
	case -6:
        HAL_SysTimer_ClearCnt();
		fn_JumpMenu(enum_Menu_AddFingerUserFail);
		DEBUG_LOG_OUT("eenum_Menu_AddFingerUserFail -6\n");
		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_AddFingerStepOK
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_AddFingerStepOK(void)
{
	//这里需要延迟一段时间，让“请移开手指播放完”
	if(menuFingerCtrl.regRestartTime >0)
	{
		menuFingerCtrl.regRestartTime --;
	}
	else
	{
		switch (HAL_Finger_SaveBufGet())
	{
	case 2:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_AddFingerStep2);
		}
		break;
	case 3:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_AddFingerStep3);
		}
		break;
	case 4:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_AddFingerStep4);
		}
		break;
	case 5:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_AddFingerStep5);
		}
		break;
	case 6:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_AddFingerStep6);
		}
		break;
	default:
		break;
	}
	}
	
}
/***************************************************************************************
**函数名:       Menu_Finger_AddFingerStep
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_BleAddFingerStep(void)
{
	
	switch (HAL_Finger_Reg(st_InputInfo.FingerIdTmp))
	{
	case 1:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 1\n");
		break;
	case 2:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 2\n");
		break;
	case 3:
		DEBUG_LOG_OUT("enum_Menu_AddUser Step 3\n");
		menuFingerCtrl.regRestartTime = MENU_FINGER_REG_RESTART_TIME;
		switch (HAL_Finger_SaveBufGet())
		{
		case 2:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ BleAddFingerStep1OK);\n");
			pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
			pBleNewAddFingerAck->add.addIng.cnt = 1;
			HAL_Comm_BleSendDir(eBleNewAddFinger);
			fn_JumpMenu(enum_Menu_BleAddFingerStep1OK);
			break;
		case 3:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ BleAddFingerStep2OK);\n");
			pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
			pBleNewAddFingerAck->add.addIng.cnt = 2;
			HAL_Comm_BleSendDir(eBleNewAddFinger);
			pBleNewAddFingerAck->ack = eBleAddFingerAckSuccess;
			pBleNewAddFingerAck->add.addSuccess.cnt = 2;
			pBleNewAddFingerAck->add.addSuccess.id = st_InputInfo.FingerIdTmp;
			BSWAP_SAVE_32(pBleNewAddFingerAck->add.addSuccess.id);
			HAL_Comm_BleSendStart(eBleNewAddFinger, 100);
			fn_JumpMenu(enum_Menu_BleAddFingerStep2OK);
			break;
		case 4:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ BleAddFingerStep3OK);\n");
			pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
			pBleNewAddFingerAck->add.addIng.cnt = 3;
			HAL_Comm_BleSendDir(eBleNewAddFinger);
			fn_JumpMenu(enum_Menu_BleAddFingerStep3OK);
			break;
		case 5:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ BleAddFingerStep4OK);\n");
			pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
			pBleNewAddFingerAck->add.addIng.cnt = 4;
			HAL_Comm_BleSendDir(eBleNewAddFinger);
			pBleNewAddFingerAck->ack = eBleAddFingerAckSuccess;
			pBleNewAddFingerAck->add.addSuccess.cnt = 4;
			pBleNewAddFingerAck->add.addSuccess.id = st_InputInfo.FingerIdTmp;
			BSWAP_SAVE_32(pBleNewAddFingerAck->add.addSuccess.id);
			HAL_Comm_BleSendStart(eBleNewAddFinger, 100);
			fn_JumpMenu(enum_Menu_BleAddFingerStep4OK);
			break;
		case 6:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ BleAddFingerStep5OK);\n");
			pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
			pBleNewAddFingerAck->add.addIng.cnt = 5;
			HAL_Comm_BleSendDir(eBleNewAddFinger);
			fn_JumpMenu(enum_Menu_BleAddFingerStep5OK);
			break;
		default:
			break;
		}
		break;
	case -3:
		memset((uint8_t *)(pBleNewAddFingerAck), 0, sizeof(TYPEs_BleAddFingerAck));
		fn_JumpMenu(enum_Menu_BleAddFingerFail);
		DEBUG_LOG_OUT("eenum_Menu_BleAddFingerUserFail -3\n");
		break;
	case 4:
		DEBUG_LOG_OUT("enum_Menu_BleAddUser Step 4\n");
		break;
	case -4:
		memset((uint8_t *)(pBleNewAddFingerAck), 0, sizeof(TYPEs_BleAddFingerAck));
		fn_JumpMenu(enum_Menu_BleAddFingerFail);
		DEBUG_LOG_OUT("eenum_Menu_BleAddFingerUserFail -4\n");
		break;
	case 5:
		DEBUG_LOG_OUT("enum_Menu_BleAddUser Step 5\n");
		break;
	case -5:
		memset((uint8_t *)(pBleNewAddFingerAck), 0, sizeof(TYPEs_BleAddFingerAck));
		fn_JumpMenu(enum_Menu_BleAddFingerFail);
		DEBUG_LOG_OUT("eenum_Menu_BleAddFingerUserFail -5\n");
		break;
	case 6:
		if (0 != storageSave.sysDat.leaveFinger)
		{
			storageSave.sysDat.leaveFinger--;
		}
		HAL_Storage_WriteSysData(&storageSave.sysDat);								//保存用户信息
		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
		storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;							//存在标志
		storageSave.UserDat.pageID = st_InputInfo.FingerIdTmp;						//保存ID
		HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
								   MSG_FINGER_ONE_SIZE,
								   st_InputInfo.FingerIdTmp,
								   &storageSave.UserDat); //保存指纹信息

		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
		HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
								  MSG_FINGER_ONE_SIZE,
								  st_InputInfo.FingerIdTmp,
								  &storageSave.UserDat); //读取参数
		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x", storageSave.UserDat.vaild);
		DEBUG_LOG_OUT("storageSave.UserDat.pageID%x", storageSave.UserDat.pageID);
		memset((uint8_t *)(pBleNewAddFingerAck), 0, sizeof(TYPEs_BleAddFingerAck));
		pBleNewAddFingerAck->ack = eBleAddFingerAckSuccess;
		pBleNewAddFingerAck->add.addSuccess.cnt = 6;
		pBleNewAddFingerAck->add.addSuccess.id = st_InputInfo.FingerIdTmp;
		BSWAP_SAVE_32(pBleNewAddFingerAck->add.addSuccess.id);
		HAL_Comm_BleSendDir(eBleNewAddFinger);
		fn_JumpMenu(enum_Menu_BleAddFingerConfirm); //进入添加确认
		DEBUG_LOG_OUT("enum_Menu_BleAddFingerConfirm 6\n");
		st_InputInfo.userIdTmp = BSWAP_16(st_InputInfo.FingerIdTmp);
		Menu_RecordLockLog(eLogTypeAddFinger,
						   (uint8_t *)&(st_InputInfo.userIdTmp),
						   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
		break;
	case -6:
        fn_JumpMenu(enum_Menu_BleAddFingerFail);
		DEBUG_LOG_OUT("eenum_Menu_AddFingerUserFail -6\n");
		break;
	default:
		break;
	}
}

/***************************************************************************************
**函数名:       Menu_Finger_BleAddFingerStepOK
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
//static void Menu_Finger_BleAddFingerStep(void)
void Menu_Finger_BleAddFingerStepOK(void)
{
	if(menuFingerCtrl.regRestartTime >0)
	{
		menuFingerCtrl.regRestartTime --;
	}
	else
	{
	switch (HAL_Finger_SaveBufGet())
	{
	case 2:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_BleAddFingerStep2);
		}
		break;
	case 3:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_BleAddFingerStep3);
		}
		break;
	case 4:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_BleAddFingerStep4);
		}
		break;
	case 5:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_BleAddFingerStep5);
		}
		break;
	case 6:
		if (0 == HAL_Finger_ChkFingerLeave())
		{
            HAL_Finger_RegReStart();
			fn_JumpMenu(enum_Menu_BleAddFingerStep6);
		}
		break;
	default:
		break;
	}
}
}
/***************************************************************************************
**函数名:       Menu_Finger_DelFinger
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_DelFinger(void)
{
	switch (HAL_Finger_Del(0, MEM_FINGER_GUEST_ALL_LOCAL)) //执行删除过程
	{
	case 1:
		DEBUG_LOG_OUT("HAL_Finger_Del Step 1 Del finger ok\n");
		HAL_Storage_DelNUserReg(0,
								MEM_FINGER_GUEST_ALL_LOCAL,
								MSG_FINGER_REG_MEM_LOCAL_ST,
								MSG_FINGER_ONE_SIZE);
		storageSave.sysDat.localUserNum = 0;		   //清空数量
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存用户信息
		fn_JumpMenu(enum_Menu_DelFingerOk);			   //删除成功
		break;
	case -1:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -1 Del finger error\n");
		fn_JumpMenu(enum_Menu_DelFingerFail); //删除失败
		break;
	case -2:
		fn_JumpMenu(enum_Menu_DelFingerFail); //删除失败
		DEBUG_LOG_OUT("HAL_Finger_Del Step -2 Del finger error\n");
		break;
	case -3:
		fn_JumpMenu(enum_Menu_DelFingerFail); //删除失败
		DEBUG_LOG_OUT("HAL_Finger_Del Step -3 Del finger error\n");
		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_ClearAll
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_ClearAll(void)
{
	if (eEquipResultNone != st_InputInfo.emptyFinger)
	{
		return; //已经获得了结果不再执行
	}
	switch (HAL_Finger_Empty())
	{
	case 1:
		DEBUG_LOG_OUT("HAL_Finger_Empty Step 1 Empty finger ok\n");
		st_InputInfo.emptyFinger = eEquipResultSuccess;
		break;
	case -1:
		DEBUG_LOG_OUT("HAL_Finger_Empty Step -1 \n");
		st_InputInfo.emptyFinger = eEquipResultFail;
		break;
	case -2:
		DEBUG_LOG_OUT("HAL_Finger_Empty Step -2 \n");
		st_InputInfo.emptyFinger = eEquipResultFail;
		break;
	case -3:
		DEBUG_LOG_OUT("HAL_Finger_Empty Step -3 \n");
		st_InputInfo.emptyFinger = eEquipResultFail;
		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_BleAddFingerFaill
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_BleAddFingerFaill(void)
{
	switch (HAL_Finger_Del(st_InputInfo.userID, 1)) //执行删除过程
	{
	case 1:
		DEBUG_LOG_OUT("Menu_BleAddFaceUserFail ok\n");
		fn_JumpMenu(enum_Menu_APPMode);
		//删除成功
		break;
	case -1:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -1 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -2:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -2 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -3:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -3 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败

		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_BleAddFingerConfirmFail
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_BleAddFingerConfirmFail(void)
{
	switch (HAL_Finger_Del(st_InputInfo.userID, 1)) //执行删除过程
	{
	case 1:
		DEBUG_LOG_OUT("Menu_Finger_BleAddFingerConfirmFail ok\n");
		fn_JumpMenu(enum_Menu_APPMode);
		//删除成功
		break;
	case -1:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -1 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -2:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -2 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -3:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -3 Del finger error\n");
		fn_JumpMenu(enum_Menu_APPMode); //删除失败

		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       Menu_Finger_BleDelFinger
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
static void Menu_Finger_BleDelFinger(void)
{
	switch (HAL_Finger_Del(st_InputInfo.userID, 1)) //执行删除过程
	{
	case 1:
		DEBUG_LOG_OUT("Menu_Finger_BleDelFinger ok\n");
		pBleAck->ack = eBleAckSuccess;
		HAL_Comm_BleSendDir(eBleDelFinger);
		fn_JumpMenu(enum_Menu_APPMode);
		//删除成功
		break;
	case -1:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -1 Del finger error\n");
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleDelFinger);
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -2:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -2 Del finger error\n");
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleDelFinger);
		fn_JumpMenu(enum_Menu_APPMode); //删除失败
		break;
	case -3:
		DEBUG_LOG_OUT("HAL_Finger_Del Step -3 Del finger error\n");
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleDelFinger);
		fn_JumpMenu(enum_Menu_APPMode); //删除失败

		break;
	default:
		break;
	}
}

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_FingerService
**功能描述:     指纹服务程序 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_FingerService(void)
{
	uint16_t IdTmp = 0;
	menuFingerCtrl.runTime++;
	if (menuFingerCtrl.runTime < MENU_FINGER_RUN_TIME_MAX)
	{
		return;
	}
	menuFingerCtrl.runTime = 0;
	if (1 == st_InputInfo.modeuleFail.Finger)
	{
		return; //如果指纹异常，不处理指纹操作。
	}
	switch (st_MenuItem.e_CurrentIndex)
	{
	case enum_Menu_Desktop:
		Menu_Finger_Desktop();
		break;
	case enum_Menu_AdminChk: //进入管理菜单检查
		Menu_Finger_AdminChk();
		break;
	case enum_Menu_AddFingerStep1: //添加指纹过程
	case enum_Menu_AddFingerStep2: //添加指纹过程2
	case enum_Menu_AddFingerStep3: //添加指纹过程3
	case enum_Menu_AddFingerStep4: //添加指纹过程4
	case enum_Menu_AddFingerStep5: //添加指纹过程5
	case enum_Menu_AddFingerStep6: //添加指纹过程6
		Menu_Finger_AddFingerStep();
        break;
    case enum_Menu_AddFingerStep1OK:
	case enum_Menu_AddFingerStep2OK:
	case enum_Menu_AddFingerStep3OK:
	case enum_Menu_AddFingerStep4OK:
	case enum_Menu_AddFingerStep5OK:
        Menu_Finger_AddFingerStepOK();
        break;
		break;
	case enum_Menu_BleAddFingerStep1: //添加指纹过程
	case enum_Menu_BleAddFingerStep2: //添加指纹过程
	case enum_Menu_BleAddFingerStep3: //添加指纹过程
	case enum_Menu_BleAddFingerStep4: //添加指纹过程
	case enum_Menu_BleAddFingerStep5: //添加指纹过程
	case enum_Menu_BleAddFingerStep6: //添加指纹过程
		Menu_Finger_BleAddFingerStep();
        break;
	case enum_Menu_BleAddFingerStep1OK:
	case enum_Menu_BleAddFingerStep2OK:
	case enum_Menu_BleAddFingerStep3OK:
	case enum_Menu_BleAddFingerStep4OK:
	case enum_Menu_BleAddFingerStep5OK:
		Menu_Finger_BleAddFingerStepOK();
		break;
	case enum_Menu_DelFinger: //删除指纹
		Menu_Finger_DelFinger();
		break;
	case enum_Menu_BleEquipmentReg:
	case enum_Menu_FactoryIng: //恢复出厂中
		Menu_Finger_ClearAll();
		break;
	case enum_Menu_BleAddFingerFail: //错误
		Menu_Finger_BleAddFingerFaill();
		break;
    case enum_Menu_BleAddFingerConfirmFail: //错误
		Menu_Finger_BleAddFingerConfirmFail();
		break;
	case enum_Menu_BleDelFinger: //蓝牙删除指纹。
		Menu_Finger_BleDelFinger();
		break;
	default:
		break;
	}
}

#ifdef M_FINGERPRINT_IDENTIFICATION_ENABLE
void FingerService(void)
{
	if (st_FingerControlStruct.u16_FingerWaitPowerOn < M_FINGER_WAIT_PW_TIME) //500ms
	{
		st_FingerControlStruct.u16_FingerWaitPowerOn++;
		return;
	}
#ifdef M_FINGERPRINT_EXTEND_LED
	if (1 == FingerLedControl.RefreshFlag)
	{
		return;
	}
#endif /*M_FINGERPRINT_EXTEND_LED*/
	switch (st_MenuItem.e_CurrentIndex)
	{
	case enum_Menu_Desktop:
	case enum_Menu_PassWordPassVerifyAgain:
#ifdef M_CARD_IDENTIFICATION_ENABLE
	case enum_Menu_IcCardPassVerifyPassWord:
#endif
#if USE_MULTI_PASSWORD_MODE
	case enum_Menu_DesktopWaitAdminInput:
#endif /*USE_MULTI_PASSWORD_MODE*/
		//        fn_FingerTask_AutoVerify();
		//		FingerServiceAutoVerify();
		break;
	case enum_Menu_AddFingerInputId:
	case enum_Menu_AddFinger1st: //添加指纹第1次
	case enum_Menu_AddFinger2rd: //添加指纹第2次
#if FINGERPRINT_MANY_TIMES
	case enum_Menu_AddFinger3: //第3次按压指纹。
	case enum_Menu_AddFinger4: //第4次按压指纹。
	case enum_Menu_AddFinger5: //第5次按压指纹。
	case enum_Menu_AddFinger6: //第6次按压指纹。
#endif						   /*FINGERPRINT_MANY_TIMES*/
							   //        fn_FingerTask_AddUser();
							   //		FingerServiceAddUser();
		break;
	case enum_Menu_DelFingerSingle:
		//        fn_FingerTask_DeleteUser();
		//		FingerServiceDeleteUser();
		break;
//    case enum_Menu_DelFingerAll:
//        fn_FingerTask_DeleteAllUser();
//        break;
#if USE_MULTI_FINGERPRINT_MODE
	case enum_Menu_FingerAllGeneral:		 //删除指纹全部普通
		if (0 < GetUserFingerPrintCounts(2)) //检测指纹库是否为空
		{
			FingerServiceDelAllWithRange(MULTI_FINGERPRINT_GENERAL_INDEX_MIN,
										 MULTI_FINGERPRINT_GENERAL_INDEX_MAX - MULTI_FINGERPRINT_GENERAL_INDEX_MIN + 1);
		}
		else
		{
			fn_JumpMenu(enum_Menu_FingerNull);
		}
		break;
	case enum_Menu_FingerAllTimelimit:		 //删除指纹全部限制时间
		if (0 < GetUserFingerPrintCounts(3)) //检测指纹库是否为空
		{
			FingerServiceDelAllWithRange(MULTI_FINGERPRINT_TIME_LIMIT_INDEX_MIN,
										 MULTI_FINGERPRINT_TIME_LIMIT_INDEX_MAX - MULTI_FINGERPRINT_TIME_LIMIT_INDEX_MIN + 1);
		}
		else
		{
			fn_JumpMenu(enum_Menu_FingerNull);
		}
		break;
	case enum_Menu_FingerAllKidnap:			 //删除指纹全部劫持
		if (0 < GetUserFingerPrintCounts(4)) //检测指纹库是否为空
		{
			FingerServiceDelAllWithRange(MULTI_FINGERPRINT_KIDNAP_INDEX_MIN,
										 MULTI_FINGERPRINT_KIDNAP_INDEX_MAX - MULTI_FINGERPRINT_KIDNAP_INDEX_MIN + 1);
		}
		else
		{
			fn_JumpMenu(enum_Menu_FingerNull);
		}
		break;
#endif /*USE_MULTI_FINGERPRINT_MODE*/
	default:
		//        fn_FingerTask_CloseLED();
		break;
	}
}
#endif
#endif /*MENU_MODULE_EN*/
/************************************Code End******************************************/
