/***************************************************************************************
**文件名:     Menu_CurrentOperate.C
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**
**备注:      
****************************************************************************************/
#include "Menu.h"
#include "Menu_CurrentOperate.h"
#include "HAL_RTC.h"
#include "HAL_Storage.h"
#include "HAL_FaceId.h"
#include "APP_Power.h"
#include "HAL_Voice.h"
#if MENU_MODULE_EN
/*******************************宏定义***************************/
//#define WAIT_CAPTURE_OVER_TIME (20 * 1500) //抓拍超时时间
//#define WAIT_BELL_CHECK_OVER_TIME (20 * 50*15) //定时检查时间
//#define WAIT_BELL_CHECK_OVER_TIME (20 ) //定时检查时间
//#define WAIT_BELL_CAN_OUT_OVER_TIME (20 * 50*8) //可退出时间
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
TYPEs_WifiSendParm wifiSendParm;

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_ClearError
**功能描述:     清除错误
**输入参数: 
TYPEs_ErrorCnt 错误结构指针
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_ClearError(TYPEs_ErrorCnt *p)
{
	p->allErr = 0;
	p->finger = 0;
	p->blueKey = 0;
	p->Face = 0;
	p->pwd = 0;
	p->card = 0;
	p->endTime = 0;
	p->startTime = 0;
}
/***************************************************************************************
**函数名:       Menu_AddError
**功能描述:     错误次数增加
**输入参数: 
TYPEs_ErrorCnt 错误结构指针
TYPEe_UserType 错误的类型
**输出参数:
int8_t 
0 正常
-1 禁试
**备注:         
****************************************************************************************/
int8_t Menu_AddError(TYPEs_ErrorCnt *p, TYPEe_UserType type)
{
	switch (type)
	{
	case eUserTypePWD:
		p->pwd++;
		p->allErr++;
		break;
	case eUserTypeFinger:
		p->finger++;
		p->allErr++;
		break;
	case eUserTypeCard:
		p->card++;
		p->allErr++;
		break;
	case eUserTypeBlueKey:
		p->blueKey++;
		p->allErr++;
		break;
	case eUserTypeFace:
		p->Face++;
		p->allErr++;
		break;
	case eUserTypeAll:
		p->allErr++;
		break;
	default:
		break;
	}
	p->startTime = HAL_RTC_GetTick(); //获取时间戳
	if (p->Face >= ERROR_CNT_FACE_MAX)
	{
		return -1;
	}
	if (p->finger >= ERROR_CNT_FINGER_MAX)
	{
		return -1;
	}
	if (p->pwd >= ERROR_CNT_PWD_MAX)
	{
		return -1;
	}
	if (p->allErr >= ERROR_CNT_ALL_MAX)
	{
		return -1;
	}
	DEBUG_LOG_OUT("Menu_AddError\n");
	DEBUG_LOG_OUT("p->finger:%d\n", p->finger);
	DEBUG_LOG_OUT("p->Face:%d\n", p->Face);
	DEBUG_LOG_OUT("p->pwd:%d\n", p->pwd);
	DEBUG_LOG_OUT("p->allErr:%d\n", p->allErr);
	return 0;
}
/***************************************************************************************
**函数名:       Menu_ChkError
**功能描述:     检查错误是否结束
**输入参数: 
TYPEs_ErrorCnt 错误结构指针
**输出参数: 
int8_t 
0 正常
-1 禁试
**备注:         
****************************************************************************************/
int8_t Menu_ChkError(TYPEs_ErrorCnt *p,TYPEe_ErrorCheckTime chkTimeFlag)
{
	if ((p->allErr >= ERROR_CNT_ALL_MAX) ||
		(p->finger >= ERROR_CNT_FINGER_MAX) ||
		(p->Face >= ERROR_CNT_FACE_MAX) ||
		(p->pwd >= ERROR_CNT_PWD_MAX))
	{
		if(eErrorCheckTimeYes == chkTimeFlag)
		{
			p->endTime = HAL_RTC_GetTick();		//获取时间戳
			if ((p->endTime) <= (p->startTime)) //时钟有问题的情况
			{
				Menu_ClearError(p);
				return 0;
			}
			else
			{
				if ((p->endTime) <= (p->startTime + ERROR_LOCK_TIME)) //还没超过锁定时间
				{
					return -1;
				}
				else
				{
					Menu_ClearError(p);
					return 0;
				}
			}
		}
		else
		{
			DEBUG_LOG_OUT("eErrorCheckTimeNo -1;\n");
			return -1;
		}
	}
	else
	{
		DEBUG_LOG_OUT("eErrorCheckTimeNo 0;\n");
		return 0;
	}
}
/***************************************************************************************
**函数名:       Menu_None
**功能描述:     没有执行任何的操作
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_None(void)
{
}
/***************************************************************************************
**函数名:       Menu_SystemSleep
**功能描述:     Menu_SystemSleep执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_SystemSleep(void)
{
}
/***************************************************************************************
**函数名:       Menu_Welcome
**功能描述:     Menu_Welcome执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_Welcome(void)
{
}

/***************************************************************************************
**函数名:       Menu_Desktop
**功能描述:     Menu_Desktop执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_Desktop(void)
{
}
/***************************************************************************************
**函数名:       Menu_PassWdPass
**功能描述:     Menu_PassWdPass执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_PassWdPass(void)
{
}
/***************************************************************************************
**函数名:       Menu_PassWdFail
**功能描述:     Menu_PassWdFail执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_PassWdFail(void)
{
}
/***************************************************************************************
**函数名:       Menu_FirstUse
**功能描述:     Menu_FirstUse执行的代码
**输入参数: 
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_FirstUse(void)
{
}
/***************************************************************************************
**函数名:       Menu_RecordLockLog
**功能描述:     事件记录
**输入参数: 
TYPEe_LogType ty,  事件类型
uint8_t *dat, 数据指针
uint8_t datLen 数据长度
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_RecordLockLog(TYPEe_LogType ty, uint8_t *dat, uint8_t datLen)
{
	uint8_t tmp[32] = {
		0,
	};
	uint8_t tmpLen = 0;
	uint16_t snTmp = 0;
	if (storageSave.sysDat.openLog < MEM_LOCKLOG_ALL)
	{
		storageSave.sysDat.openLogSn += 1;
	}
	else
	{
		storageSave.sysDat.openLogSn = 2;
	}
	storageSave.sysDat.openLog++;
	switch (ty)
	{
	case eLogTypeKtemp:
		tmp[0] = 0X20;
		tmpLen = sizeof("KTEMP") - 1;
		memcpy(tmp + 1, "KTEMP", tmpLen);
		break; //模式temp
	case eLogTypeKsos:
		tmp[0] = 0X20;
		tmpLen = sizeof("KSOS") - 1;
		memcpy(tmp + 1, "KSOS", tmpLen);
		break; //模式SOS
	case eLogTypeOpenByFinger:
		tmp[0] = 0X20;
		tmpLen = sizeof("F") - 1;
		memcpy(tmp + 1, "F", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + 1 + tmpLen, dat, datLen);
		break; //指纹开门
	case eLogTypeOpenBySmartKey:
		tmp[0] = 0X20;
		tmpLen = sizeof("S") - 1;
		memcpy(tmp + 1, "S", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + 1 + tmpLen, dat, datLen);
		break; //智能钥匙开门
	case eLogTypeOpenByPwd:
		tmp[0] = 0X20;
		tmpLen = sizeof("K") - 1;
		memcpy(tmp + 1, "K", tmpLen);
		break; //密码开门
	case eLogTypeOpenByFace:
		tmp[0] = 0X20;
		tmpLen = sizeof("R") - 1;
		memcpy(tmp + 1, "R", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + 1 + tmpLen, dat, datLen);
		break; //人脸开门
	case eLogTypeOpenByPhone:
		tmp[0] = 0X20;
		tmpLen = sizeof("P") - 1;
		memcpy(tmp + 1, "P", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + 1 + tmpLen, dat, datLen);
		break; //手机开门
	case eLogTypeAddPwd:
		tmpLen = sizeof("+KPWD") - 1;
		memcpy(tmp, "+KPWD", tmpLen);
		break; //修改密码
	case eLogTypeAddPwdSos:
		tmpLen = sizeof("+KSOSPWD") - 1;
		memcpy(tmp, "+KSOSPWD", sizeof("+KSOSPWD"));
		break; //修改sos密码
	case eLogTypeAddAesPhone:
		tmpLen = sizeof("+P") - 1;
		memcpy(tmp, "+P", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //AES增加手机账号
	case eLogTypeDelPhone:
		tmpLen = sizeof("-P") - 1;
		memcpy(tmp, "-P", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //删除手机号
	case eLogTypeAddFinger:
		tmpLen = sizeof("+F") - 1;
		memcpy(tmp, "+F", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //添加指纹
	case eLogTypeDelFinger:
		tmpLen = sizeof("-F") - 1;
		memcpy(tmp, "-F", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //删除指纹
	case eLogTypeAddFace:
		tmpLen = sizeof("+R") - 1;
		memcpy(tmp, "+R", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //添加人脸
	case eLogTypeDelFace:
		tmpLen = sizeof("-R") - 1;
		memcpy(tmp, "-R", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //删除人脸
	case eLogTypeAddSmartKey:
		tmpLen = sizeof("+S") - 1;
		memcpy(tmp, "+S", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //添加智能钥匙
	case eLogTypeDelSmartKey:
		tmpLen = sizeof("-S") - 1;
		memcpy(tmp, "-S", sizeof("-S"));
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //删除智能钥匙
	case eLogTypeEnterAdminFace:
		tmpLen = sizeof("AR") - 1;
		memcpy(tmp, "AR", tmpLen);
		if (NULL == dat)
		{
			break;
		}
		memcpy(tmp + tmpLen, dat, datLen);
		break; //进入管理菜单人脸
	case eLogTypeAlarm:
		tmpLen = sizeof(" AALARM") - 1;
		memcpy(tmp, " AALARM", tmpLen);
		break; //报警
	case eLogTypeTest:
		tmpLen = sizeof(" TTEST") - 1;
		memcpy(tmp, " TTEST", tmpLen);
		break; //测试
	case eLogTypeEmpty:
		tmpLen = sizeof("EEEMPTY") - 1;
		memcpy(tmp, "EEEMPTY", tmpLen);
		storageSave.sysDat.openLogSn = 1;
		storageSave.sysDat.openLog = 1;
		break; //清空
	default:
		break;
	}
	tmp[8] = st_InputInfo.RTCTimer.tm_year; //注意这里用
	tmp[9] = st_InputInfo.RTCTimer.tm_mon;
	tmp[10] = st_InputInfo.RTCTimer.tm_yday;
	tmp[11] = st_InputInfo.RTCTimer.tm_hour;
	tmp[12] = st_InputInfo.RTCTimer.tm_min;
	tmp[13] = st_InputInfo.RTCTimer.tm_sec;
	snTmp = storageSave.sysDat.openLogSn;
	if (snTmp >= 1)
	{
		snTmp--;
	}
	HAL_Storage_WriteLockLog(tmp, snTmp);
	HAL_Storage_WriteSysData(&storageSave.sysDat);
}
/***************************************************************************************
**函数名:       Menu_AppRegChk
**功能描述:     检查系统是否注册
**输入参数:     --
**输出参数:     
-1 未注册
0 已经注册 
**备注:         
****************************************************************************************/
int8_t Menu_AppRegChk(void)
{
	if (storageSave.sysDat.allPhone == storageSave.sysDat.leavePhone)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
/***************************************************************************************
**函数名:       Menu_RTCTimerGet
**功能描述:     获取RTC
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t Menu_RTCTimerGet(void)
{
	HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
	return 0;
}
/***************************************************************************************
**函数名:       Menu_GetchannelPwd
**功能描述:     获取信道密码 
**输入参数:     
uint8_t *p,uint8_t len
**输出参数:  
int8_t   
-1; //参数错误
0; //成功
**备注:         

****************************************************************************************/
int8_t Menu_GetchannelPwd(uint8_t *p, uint8_t len)
{
	uint16_t i;
	if (NULL == p)
	{
		return -1; //参数错误
	}
	memset(p,0,len);
	for (i = 0; i < len; i++)
	{
		if (NULL == p)
		{
			return -1; //参数错误
		}
		#if 1
		*(p + i) = ~(storageSave.hardwareData.mac[5 -i]);
		#endif
		#if 0
		*(p + i) = ~(storageSave.hardwareData.mac[i]);
		#endif
	}
	return 0; //成功
}

/***************************************************************************************
**函数名:       Menu_WifiLockLogUpdata
**功能描述:     wifi上报开门数据 
**输入参数:     
uint32_t id
**输出参数:    --
**备注:         

****************************************************************************************/
void Menu_WifiLockLogUpdata(void)
{
    if((MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) ||
            (0 != Menu_LowPowerChk()))     //检查wifi主开关开启
    {
        return; //wifi不进行推送
    }
	DEBUG_LOG_OUT("Menu_WifiLockLogUpdata\n");
    #if 1
	memset((uint8_t *)pWifiCmdUnlockRealUpdata,
		   0,
		   sizeof(TYPEs_WifiCmdUnlockRealUpdata));
    #endif
	pWifiCmdUnlockRealUpdata->deviceType = WIFI_DEVICE_TYPE; //设备类型
	memcpy(pWifiCmdUnlockRealUpdata->deviceInform.Wifi.MAC,
		   storageSave.hardwareData.mac,
		   sizeof(pWifiCmdUnlockRealUpdata->deviceInform.Wifi.MAC)); //MAC
	//电机工作的参数先不填写。
	pWifiCmdUnlockRealUpdata->bat = st_InputInfo.upBatBattery; //填写电池电量
	BSWAP_SAVE_16(pWifiCmdUnlockRealUpdata->bat);
	pWifiCmdUnlockRealUpdata->unlockType = wifiSendParm.unlockType; 
    #if 0
    pWifiCmdUnlockRealUpdata->pageId[0] = (uint8_t)(wifiSendParm.pageId >> 24);
    pWifiCmdUnlockRealUpdata->pageId[1] = (uint8_t)(wifiSendParm.pageId >> 16);
    pWifiCmdUnlockRealUpdata->pageId[2] = (uint8_t)(wifiSendParm.pageId >> 8);
    pWifiCmdUnlockRealUpdata->pageId[3] = (uint8_t)(wifiSendParm.pageId); //PageID
    #else
    memcpy(pWifiCmdUnlockRealUpdata->pageId,
           wifiSendParm.pageId,
           sizeof(pWifiCmdUnlockRealUpdata->pageId)); //PageID
    #endif
    
    pWifiCmdUnlockRealUpdata->attribute = wifiSendParm.limt;
	DEBUG_LOG_OUT("storageSave.hardwareData.mac:");
	DEBUG_LOG_OUT_HEX(storageSave.hardwareData.mac, sizeof(storageSave.hardwareData.mac));
	DEBUG_LOG_OUT("pWifiCmdUnlockRealUpdata->deviceType:%x\n", pWifiCmdUnlockRealUpdata->deviceType);
	pWifiCmdUnlockRealUpdata->unlockTime[0] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_year - 2000);
	pWifiCmdUnlockRealUpdata->unlockTime[1] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_mon);
	pWifiCmdUnlockRealUpdata->unlockTime[2] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_mday);
	pWifiCmdUnlockRealUpdata->unlockTime[3] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_hour);
	pWifiCmdUnlockRealUpdata->unlockTime[4] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_min);
	pWifiCmdUnlockRealUpdata->unlockTime[5] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_sec);
    DEBUG_LOG_OUT("pWifiCmdUnlockRealUpdata->attribute:%x\n", pWifiCmdUnlockRealUpdata->attribute);
    #ifdef USE_FACE_WIFI
	Comm_WifiCmdSend(eWifiCmdUnlockRealUpdata); //打包好数据
    #if 0
	HAL_FaceId_CmdDealStart(eFaceIdCmdDataPassThrough,400); //开始透传
    #endif
    #if 0
    wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
    #endif
    #endif
    #ifdef USE_WIFI_MODLE
    wifiSendParm.sendLen = Comm_WifiCmdSendWithLen( eWifiCmdUnlockRealUpdata,
                                                    eWifiProtocolTypeCmdNotAck,
                                                    sizeof(TYPEs_WifiCmdUnlockRealUpdata)); //打包好数据
    wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
    #endif
}
/***************************************************************************************
**函数名:       Menu_WifiUpdataTest
**功能描述:     wifi上报开门测试 
**输入参数:     
uint32_t id
**输出参数:    --
**备注:         

****************************************************************************************/
void Menu_WifiUpdataTest(void)
{
    DEBUG_LOG_OUT("Menu_WifiUpdataTest\n");
	
	pWifiCmdUnlockRealUpdata->deviceType = WIFI_DEVICE_TYPE; //设备类型
	memcpy(pWifiCmdUnlockRealUpdata->deviceInform.Wifi.MAC,
		   storageSave.hardwareData.mac,
		   sizeof(pWifiCmdUnlockRealUpdata->deviceInform.Wifi.MAC)); //MAC
}
/***************************************************************************************
**函数名:       Menu_WifiCmdAlarmUpdata
**功能描述:     wifi报警信息上传
**输入参数:     
uint32_t id
**输出参数:    --
**备注:         

****************************************************************************************/
void Menu_WifiCmdAlarmUpdata(void)
{
    if((MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) ||
            (0 != Menu_LowPowerChk()))     //检查wifi主开关开启
    {
        return; //wifi不进行推送
    }
	DEBUG_LOG_OUT("Menu_WifiCmdAlarmUpdata\n");
	memset((uint8_t *)pWifiCmdAlarmUpdata,
		   0,
		   sizeof(TYPEs_WifiCmdAlarmUpdata));
	pWifiCmdAlarmUpdata->deviceType = WIFI_DEVICE_TYPE; //设备类型
	memcpy(pWifiCmdAlarmUpdata->MAC,
		   storageSave.hardwareData.mac,
		   sizeof(pWifiCmdAlarmUpdata->MAC)); //MAC
	pWifiCmdAlarmUpdata->unlockTime[0] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_year - 2000);
	pWifiCmdAlarmUpdata->unlockTime[1] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_mon);
	pWifiCmdAlarmUpdata->unlockTime[2] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_mday);
	pWifiCmdAlarmUpdata->unlockTime[3] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_hour);
	pWifiCmdAlarmUpdata->unlockTime[4] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_min);
	pWifiCmdAlarmUpdata->unlockTime[5] = BCD_TO_BIN(st_InputInfo.RTCTimer.tm_sec);
	pWifiCmdAlarmUpdata->bat = st_InputInfo.upBatBattery; //填写电池电量
	BSWAP_SAVE_16(pWifiCmdAlarmUpdata->bat);
	pWifiCmdAlarmUpdata->alarmType = wifiSendParm.sendType;
    #ifdef USE_FACE_WIFI
	Comm_WifiCmdSend(eWifiCmdAlarmUpdata); //打包好数据
    #if 1
	#if 0
	HAL_FaceId_CmdDealStart(eFaceIdCmdDataPassThrough,400); //开始透传
    #endif
    #if 0
    wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
    #endif
    #endif
    #endif
    #ifdef USE_WIFI_MODLE
    wifiSendParm.sendLen = Comm_WifiCmdSendWithLen( eWifiCmdAlarmUpdata,
                                                    eWifiProtocolTypeCmdNotAck,
                                                    sizeof(TYPEs_WifiCmdAlarmUpdata)); //打包好数据
    wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
    #endif
}


/***************************************************************************************
**函数名:       Menu_CheckWifiPower
**功能描述:     检查wifi能工作的电池状态
**输入参数:     --
**输出参数:  
TYPEe_InPowerStatus  电源状态
**备注:         

****************************************************************************************/
TYPEe_InPowerStatus Menu_CheckWifiPower(void)
{
	if(st_InputInfo.upBatBattery <= DC_WIFI_V_LIMT)
	{
		return eInPowerStatusLow;
	}
	else
	{
		return eInPowerStatusNormal;
	}
}

#if 0
/***************************************************************************************
**函数名:       Menu_WifiDeal
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_WifiDeal(void)
{
	//检查wifi时候开启
	//wifi空闲不操作
	//低电压不操作
	//电机动作时不操作
	//重连时不操作。
	static uint8_t waitConnFlag = 0; //等待联网标志
    static uint32_t overTime = 0;    //超时计数器
	switch (HAL_FaceId_CmdDeal(eFaceIdCmdDataPassThrough,0, 50))
	{
	case eFaceIdComNone:
		break; //空闲
	case eFaceIdComReady:
		break; //准备开始
	case eFaceIdComReply:
		waitConnFlag = 0;
		break;		   //等待回复
	case eFaceIdComOk: //命令完成
		HAL_FaceId_CmdStop(); //停止此命令
		DEBUG_LOG_OUT("Menu_WifiDeal eFaceIdComOk\n");
	
		break; //成功
	case eFaceIdComFail:
		HAL_FaceId_CmdStop(); //停止此命令
		DEBUG_LOG_OUT("Menu_WifiDeal eFaceIdComFail\n");
		
		break;
	case eFaceIdComTimeOut:
		HAL_FaceId_CmdStop(); //停止此命令
		DEBUG_LOG_OUT("Menu_WifiDeal eFaceIdComTimeOut\n");
	
		break; //超时
	default:
		break;
	}
	if (1 == waitConnFlag)
    {
        if (eMediaStateWaiting == noteMediaState.mediaState)
        {
            waitConnFlag = 0;
            DEBUG_LOG_OUT("Menu_Face_BleConfigWifiData eMediaStateWaiting\n");
        }
        overTime++;
        if (overTime >= WAIT_CONN_OVER_TIME)
        {
            waitConnFlag = 0;
        }
    }
	
}
#endif
#if 0
/***************************************************************************************
**函数名:       Menu_TriggerCaptureData
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_TriggerCaptureData(void)
{
	static uint8_t captureFlag = 0;
	static uint32_t overTime = 0;
	switch (HAL_FaceId_CmdDeal(eFaceIdCmdTriggerCaptureData,0, 100))
	{
	case eFaceIdComNone:
		break; //空闲
	case eFaceIdComReady:
		memset((uint8_t *)&noteMediaState, 0, sizeof(TYPEs_NoteMediaState));
		break; //准备开始
	case eFaceIdComReply:
		captureFlag = 0;
		break;		   //等待回复
	case eFaceIdComOk: //命令完成
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComOk\n");
		captureFlag = 1;
		overTime = 0;
		faceIdCtrl.com = eFaceIdComCmdTaskIng; //进入任务等待 
		break;				  //成功
	case eFaceIdComFail:
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComFail\n");
		captureFlag = 1;
		HAL_FaceId_CmdStop(); //停止此命令
		break;
	case eFaceIdComTimeOut:
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComTimeOut\n");
		captureFlag = 1;
		HAL_FaceId_CmdStop(); //停止此命令
		break;				  //超时
	default:
		break;
	}

	if (1 == captureFlag)
	{
		overTime++;
		if (overTime >= WAIT_CAPTURE_OVER_TIME)
		{
			DEBUG_LOG_OUT("Menu_TriggerCaptureData WAIT_CAPTURE_OVER_TIME\n");
			captureFlag = 0;
			wifiSendParm.catEyeStatus = eCatEyeStatusNone;
		}
		if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
		{
			if ((eMediaStateIdle == noteMediaState.mediaState) ||
				(eMediaStateError == noteMediaState.mediaState)) //空闲或者超时
			{
				DEBUG_LOG_OUT("Menu_TriggerCaptureData eMediaStateError eMediaStateIdle \n");
				captureFlag = 0;
				wifiSendParm.catEyeStatus = eCatEyeStatusNone;
			}
		}
	}
}
#endif
#if 0
/***************************************************************************************
**函数名:       Menu_WifiDeal
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_VideoCallData(void)
{
	//检查wifi时候开启
	//wifi处理。
	//	wifiSendParm.timerCnt ++;
	//	if (eFaceIdCmdVideoCallData == faceIdCtrl.cmd)
	//	{
	//		switch (HAL_FaceId_CmdDeal(100)) //
	//		{
	//		case eFaceIdComNone:
	//			break; //空闲
	//		case eFaceIdComReady:
	//			break; //准备开始
	//		case eFaceIdComReply:
	//			break;		   //等待回复
	//		case eFaceIdComOk: //命令完成
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComOk\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//			break; //成功
	//		case eFaceIdComFail:
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComFail\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//			break;
	//		case eFaceIdComTimeOut:
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComTimeOut\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//			break; //超时
	//		default:
	//			break;
	//		}
	//	}
	//
	//	if(wifiSendParm.timerCnt >= WAIT_BELL_CHECK_OVER_TIME) //每隔一段时间查询一次工作状态
	//	{
	//		HAL_FaceId_CmdDealStart(eFaceIdCmdGetModuleStatus,0);
	//	}
	//	if (eFaceIdCmdGetModuleStatus == faceIdCtrl.cmd)
	//	{
	//		switch (HAL_FaceId_CmdDeal(100)) //
	//		{
	//		case eFaceIdComNone:
	//			break; //空闲
	//		case eFaceIdComReady:
	//			break; //准备开始
	//		case eFaceIdComReply:
	//			break;		   //等待回复
	//		case eFaceIdComOk: //命令完成
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComOk\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//
	//			break; //成功
	//		case eFaceIdComFail:
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComFail\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//			fn_JumpMenu(enum_Menu_SystemSleep); //进入休眠
	//			break;
	//		case eFaceIdComTimeOut:
	//			DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComTimeOut\n");
	//			HAL_FaceId_CmdStop(); //停止此命令
	//			fn_JumpMenu(enum_Menu_SystemSleep); //进入休眠
	//			break; //超时
	//		default:
	//			break;
	//		}
	//	}
	//	if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
	//	{
	//		if (eMediaStateError == noteMediaState.mediaState)
	//		{
	//			DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateError \n");
	//			//门铃，请稍后再拨
	//			fn_JumpMenu(enum_Menu_SystemSleep); //进入休眠
	//		}
	//		else if(eMediaStateIdle == noteMediaState.mediaState)
	//		{
	//			DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateIdle \n");
	//			fn_JumpMenu(enum_Menu_SystemSleep); //进入休眠
	//		}
	//	}


	switch (HAL_FaceId_VideoCallData()) //
	{
	case eFaceIdComNone:
		break; //空闲
	case eFaceIdComReady:
		break; //准备开始
	case eFaceIdComReply:
		break;		   //等待回复
	case eFaceIdComOk: //命令完成
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComOk\n");
		HAL_FaceId_CmdStop(); //停止此命令
		break; //成功
	case eFaceIdComFail:
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComFail\n");
		HAL_FaceId_CmdStop(); //停止此命令
		break;
	case eFaceIdComTimeOut:
		DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComTimeOut\n");
		HAL_FaceId_CmdStop(); //停止此命令
		break; //超时
	default:
		break;
	}
}
#endif
/***************************************************************************************
**函数名:       Menu_CheckCombine
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
TYPEe_CombineType Menu_CheckCombine(void)
{
//	DEBUG_LOG_OUT("storageSave.sysDat.openLockMode:%X\n",storageSave.sysDat.openLockMode);
	if(storageSave.sysDat.openLockMode & 0x80)
	{
		return eCombineTypeAND; 
	}
	else
	{
		return eCombineTypeOR;
	}
}


/***************************************************************************************
**函数名:       Menu_CheckCombine
**功能描述:     
**输入参数:     --
**输出参数:     
int8_t
0 通过
-1 不通过
**备注:         
周用1byte表示。0b00111110 代表周一到周五时间有效，用
bit1代表星期一，
bit2代表星期二，
bit3代表星期三，
bit4代表星期四，
bit5代表星期五，
bit6代表星期六，
bit7代表礼拜天
时效范围：内容起始时间（年+月+日+时+分+秒）+时效时间（年+月+日+时+分+秒）+周
****************************************************************************************/
int8_t Menu_CheckTimeVaild(struct tm *timeNow,struct tm *timeStart,struct tm *timeStop,uint8_t wDay)
{
	struct tm timeNowTmp;
	struct tm timeStartTmp;
	struct tm timeStopTmp;
	uint32_t secNow=0;
	uint32_t secStart=0;
	uint32_t secStop=0;
	uint8_t wdayTmp = 0;
	memcpy((uint8_t *)&timeNowTmp,(uint8_t *)timeNow,sizeof(struct tm));
	memcpy((uint8_t *)&timeStartTmp,(uint8_t *)timeStart,sizeof(struct tm));
	memcpy((uint8_t *)&timeStopTmp,(uint8_t *)timeStop,sizeof(struct tm));
	if(timeNowTmp.tm_wday == 0) //days since Sunday, 0 to 6
	{
		wdayTmp = 0x01<<7;
	}
	else
	{
		wdayTmp = 0x01<< timeNowTmp.tm_wday;
	}
	timeStartTmp.tm_sec =0; //开始xx:xx:00
	timeStopTmp.tm_sec =59; //结束xx:xx:59
	secNow = ((uint32_t)(timeNowTmp.tm_hour)*60 + (uint32_t)(timeNowTmp.tm_min))*60+(uint32_t)(timeNowTmp.tm_sec);
	secStart = ((uint32_t)(timeStartTmp.tm_hour)*60 + (uint32_t)(timeStartTmp.tm_min))*60+(uint32_t)(timeStartTmp.tm_sec);
	secStop = ((uint32_t)(timeStopTmp.tm_hour)*60 + (uint32_t)(timeStopTmp.tm_min))*60+(uint32_t)(timeStopTmp.tm_sec);
	if((secStart > secNow) || (secNow > secStop)) //时分秒不在范围内
	{
		return -1;
	}
	timeStartTmp.tm_hour = 0; //从起始的日期00:00:00到截止日期的23:59:59 判断日期段和周
	timeStartTmp.tm_min =0;
	timeStopTmp.tm_hour =23;
	timeStopTmp.tm_min =59;
	timeStopTmp.tm_sec = 59;
	secStart = HAL_RTC_Stm2Tick(timeStartTmp);
	secStop = HAL_RTC_Stm2Tick(timeStopTmp);
	secNow = HAL_RTC_Stm2Tick(timeNowTmp);
	if((secStart > secNow) || (secNow > secStop)) //不在范围内
	{
		return -1;
	}
	if((wdayTmp & wDay) == 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/***************************************************************************************
**函数名:       Menu_LowPowerChk
**功能描述:     低电压检测
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t Menu_LowPowerChk(void)
{
    #if 0
    if(st_InputInfo.downBatBattery < 650)
    {
        return 1;
    }
    if(st_InputInfo.upBatBattery < DC_WIFI_V_LIMT)
    {
        return 1;
    }
    #endif
    return 0;
}

/***************************************************************************************
**函数名:       Menu_WaitWifiComplete
**功能描述:     等待wifi完成任务
**输入参数:     --
**输出参数:     
int8_t
0 空闲
-1 忙碌
**备注:
****************************************************************************************/
int8_t Menu_WaitWifiComplete(void)
{
    if (eWifiSendStatusNone == wifiSendParm.wifiSendStatus)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/***************************************************************************************
**函数名:       Menu_EnterDesktopVerifyStart
**功能描述:     进入桌面时，启动人脸检测
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_EnterDesktopVerifyStart(void)
{
    #if 1
//    if ((eKeyRandomLockFlagEnable == st_InputInfo.keyRandomLockFlag)) //按键有锁
    {
        faceIdCtrl.enableNetworkBack = 0;
        Menu_Face_PowerOnStart(0);
        HAL_FaceId_VerifyUserStart();
        Menu_VoiceDesktop(); //语音播放
        DEBUG_LOG_OUT("HAL_FaceId_VerifyUserStart();\n");
    }
    #endif
}
#endif

/************************************Code End******************************************/
