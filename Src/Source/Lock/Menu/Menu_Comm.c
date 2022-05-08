/***************************************************************************************
**文件名:     Menu_Comm.c
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
#include "HAL_Comm.h"
#include "HAL_Storage.h"
#include "HAL_Voice.h"
#include "HAL_RTC.h"
#include "HAL_Finger.h"
#include "Function.h"
#include "HAL_KeyLight.h"
#include "HAL_FaceId.h"
#include "HAL_Power.h"
#include "rijndael.h"
#include "HAL_IR.h"

#include "base64.h"
#include "id2_config.h"
#include "id2_plat.h"
#include "id2_priv.h"
#include "id2_client.h"
#include "km.h"
#include "sha256.h"
#include "HAL_OnChipFlash.h"

#if MENU_MODULE_EN
//#warning MENU_MODULE_EN模块使能!
/*******************************宏定义***************************/
#define AES_DEBUG 0
/*************************.C变量结构声明***********************/
typedef struct
{
	uint8_t SecretKey[8]; //密钥
} TYPEs_CommDat;		  //通讯数据
/*************************私有全局变量*************************/
static TYPEs_CommDat commDat = {
	.SecretKey = {
		0,
	},
};
static uint8_t const gLockUpdataBleName[] = "L_"; 		//蓝牙升级名称
static uint8_t const gLockRegBleName[] = "REGLOCK_";	//蓝牙管理模式名称
static uint8_t const gLockBleName[] = "LOCK_";			//蓝牙正常模式名称
static uint16_t const gPrime[10] = {811, 919, 991, 1117, 1439, 1627, 1861, 1907, 1951, 1993};
static uint8_t const gPrimary[5] = {2, 3, 5, 7, 11};

const uint8_t AppExistValue[] = {0xA5, 0x5A, 0xAA, 0xBB}; //存在用可用的APP
const uint8_t AppProgramPC[] = {0xA5, 0x5A, 0xAB, 0xBA};	//PC上位机方式升级方式
#if 0
const uint8_t AppProgramMCU[] = {0x5A, 0xA5, 0xBA, 0xAB}; //MCU透传方式升级方式
#else
const uint8_t AppProgramMCU[] = {0xA5, 0x5A, 0xAB, 0xBC}; //MCU透传方式升级方式

#endif
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_Comm_BleAddFinger
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAddFinger(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAddFinger\n");
	pBleAddFingerAck->ack = eBleAddFingerAckIng;
	pBleAddFingerAck->add.addIng.cnt = 1;
	HAL_Comm_BleSendDir(eBleAddFinger);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleTimeSync
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleTimeSync(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	struct tm timTmp;
	uint32_t tick;
	DEBUG_LOG_OUT("Menu_Comm_BleTimeSync\n");
	timTmp.tm_year = BCD_TO_BIN_SAVE(pBleTimeSync->time.year) + 2000;
	timTmp.tm_mon = BCD_TO_BIN_SAVE(pBleTimeSync->time.month);
	timTmp.tm_mday = BCD_TO_BIN_SAVE(pBleTimeSync->time.day);
	timTmp.tm_hour = BCD_TO_BIN_SAVE(pBleTimeSync->time.hour);
	timTmp.tm_min = BCD_TO_BIN_SAVE(pBleTimeSync->time.minute);
	timTmp.tm_sec = BCD_TO_BIN_SAVE(pBleTimeSync->time.second);
	if (((timTmp.tm_mon >= 1) && (timTmp.tm_mon <= 12)) &&
		((timTmp.tm_mday >= 1) && (timTmp.tm_mday <= 31)) &&
		((timTmp.tm_hour >= 0) && (timTmp.tm_hour <= 23)) &&
		((timTmp.tm_min >= 0) && (timTmp.tm_min <= 59)) &&
		((timTmp.tm_sec >= 0) && (timTmp.tm_sec <= 59)))
	{
		tick = HAL_RTC_Stm2Tick(timTmp); //获取时间戳
		HAL_RTC_SetByTick(tick);		 //设置时钟
		DEBUG_LOG_OUT("pre tick is：%d\n", tick);
		//		tick =HAL_RTC_GetTick();
		//		DEBUG_LOG_OUT("now tice is：%d\n",tick);
		pBleAck->ack = eBleAckSuccess;
	}
	else
	{
		pBleAck->ack = eBleAckFail;
	}
	HAL_Comm_BleSendDir(eBleTimeSync);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAddPhoneAccount
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAddPhoneAccount(void)
{
	uint8_t phoneTmp[3] = {
		0,
	};
	uint8_t tmp[sizeof(TYPEs_BleAddPhoneAccount)];
	uint16_t i = 0;
	uint16_t j = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAddPhoneAccount\n");
	memcpy(tmp,
		   pBleAddPhoneAccount->phoneID,
		   sizeof(tmp)); //获取手机号
	DEBUG_LOG_OUT(" pBleAddPhoneAccount->phoneID:\n");
	DEBUG_LOG_OUT_HEX(pBleAddPhoneAccount->phoneID, sizeof(pBleAddPhoneAccount->phoneID));
	st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
												  MEM_PHONE_ALL,
												  MSG_CARD_REG_MEM_ST,
												  MSG_CARD_REG_SIZE);
	DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
	if (0XFFFF == st_InputInfo.userID)
	{
		DEBUG_LOG_OUT("st_InputInfo.userID: ERROR!\n");
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
		memcpy(storageSave.UserDat.UserID,
			   tmp,
			   sizeof(storageSave.UserDat.UserID));
		storageSave.sysDat.leavePhone--;
		storageSave.UserDat.pageID = st_InputInfo.userID;
		storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
		storageSave.UserDat.Lim = MSG_FACTMEM_LIM_PHONE;
		HAL_Storage_SaveOneUserReg(MSG_CARD_REG_MEM_ST,
								   MSG_CARD_REG_SIZE,
								   st_InputInfo.userID,
								   &storageSave.UserDat); //保存用户数据
		HAL_Storage_WriteSysData(&storageSave.sysDat);	  //写入系统参数
	}
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleAddPhoneAccount);
	memcpy(phoneTmp, &storageSave.UserDat.UserID[10], 3);
	phoneTmp[0] ^= storageSave.hardwareData.channelPwd[2];
	phoneTmp[1] ^= storageSave.hardwareData.channelPwd[3];
	phoneTmp[2] ^= storageSave.hardwareData.channelPwd[0]; //对ID进行简单加密
	Menu_RecordLockLog(eLogTypeAddAesPhone, phoneTmp, 3);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleAddSmartKeyAccredit
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:    
对应的下发包：
智能秘钥(13byte)(随机产生,和数据库不一样) (数据加密)
数据包加密方式:数据包分成8个字节一包,每包与锁注册秘钥(指令0x3E)异或成密文,最后打包发送

****************************************************************************************/
void Menu_Comm_BleAddSmartKeyAccredit(void)
{
	uint32_t i = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAddSmartKeyAccredit\n");
	for (i = 0; i < sizeof(pBleAddSmartKeyAccredit->key); i++)
	{
		pBleAddSmartKeyAccredit->key[i] ^= commDat.SecretKey[i % (sizeof(commDat.SecretKey))];
	} //异或成明文,这里对于数据的长度需要注意看下
	DEBUG_LOG_OUT("pBleAddSmartKeyAccredit->key:\n");
	DEBUG_LOG_OUT_HEX(pBleAddSmartKeyAccredit->key, sizeof(pBleAddSmartKeyAccredit->key));
	st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
												  MEM_SMARTKEY_ALL,
												  MSG_SMARTKEY_REG_MEM_ST,
												  MSG_SMARTKEY_REG_SIZE); //查询空位
	DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
	storageSave.sysDat.leaveSmartKey--;										 //减少数量
	memset((uint8_t *)&storageSave.UserDat, 0, sizeof(storageSave.UserDat)); //清空数据指针
	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;						 //存在标志
	memcpy(storageSave.UserDat.UserID,
		   pBleAddSmartKeyAccredit->key,
		   sizeof(storageSave.UserDat.UserID));
	storageSave.UserDat.Lim = 'P';
	storageSave.UserDat.pageID = st_InputInfo.userID;
	HAL_Storage_SaveOneUserReg(MSG_SMARTKEY_REG_MEM_ST,
							   MSG_SMARTKEY_REG_SIZE,
							   st_InputInfo.userID,
							   &storageSave.UserDat); //保存信息
	HAL_Storage_WriteSysData(&storageSave.sysDat);	  //写入系统参数
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleAddSmartKeyAccredit);
	Menu_RecordLockLog(eLogTypeAddSmartKey, &storageSave.UserDat.UserID[10], 3); //保存事件记录
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDelFinger
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDelFinger(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDelFinger\n");
	DEBUG_LOG_OUT("pBleDelFinger->ID1:%x\n", pBleDelFinger->pageId.ID1);
	st_InputInfo.userID = (uint16_t)(BSWAP_32(pBleDelFinger->pageId.ID1)); //获取要删除的ID号
	HAL_Storage_DelOneUserReg(st_InputInfo.userID,
							  MSG_FINGER_REG_MEM_LOCAL_ST,
							  MSG_FINGER_ONE_SIZE);
	storageSave.sysDat.leaveFinger++;
	HAL_Storage_WriteSysData(&storageSave.sysDat); //保存用户信息
	HAL_Finger_DelStart();						   //开启指令
	fn_JumpMenu(enum_Menu_BleDelFinger);		   //进入蓝牙删除指纹
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDelPhoneAccount
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDelPhoneAccount(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDelPhoneAccount\n");
	if (-1 == Menu_AppRegChk()) //查询是否没有手机号
	{
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		memset(storageSave.UserDat.UserID, 0, sizeof(storageSave.UserDat.UserID));
		memcpy(storageSave.UserDat.UserID,
			   pBleDelPhoneAccount->phoneID,
			   sizeof(storageSave.UserDat.UserID)); //导入用户ID

		if (-1 != HAL_Storage_MatchId(0,
									  MEM_PHONE_ALL,
									  MSG_CARD_REG_MEM_ST,
									  MSG_CARD_REG_SIZE,
									  &storageSave.UserDat,
									  eMatchTypeUserID)) //匹配数据
		{
			HAL_Storage_DelOneUserReg(storageSave.UserDat.pageID,
									  MSG_CARD_REG_MEM_ST,
									  MSG_CARD_REG_SIZE);
			storageSave.sysDat.leavePhone++;
			HAL_Storage_WriteSysData(&storageSave.sysDat);
			Menu_RecordLockLog(eLogTypeDelPhone,
							   (uint8_t *)&(storageSave.UserDat.UserID[10]),
							   3); //保存事件记录，这里保存UserId末三位
			pBleAck->ack = eBleAckSuccess;
		}
		else
		{
			pBleAck->ack = eBleAckFail;
		}
	}
	HAL_Comm_BleSendDir(eBleDelPhoneAccount);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDelSmartKey
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDelSmartKey(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDelSmartKey\n");
	if (storageSave.sysDat.allSmartKey == storageSave.sysDat.leaveSmartKey)
	{
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		memset(storageSave.UserDat.UserID, 0, sizeof(storageSave.UserDat.UserID));
		memcpy(storageSave.UserDat.UserID,
			   pBleDelSmartKey->key,
			   sizeof(storageSave.UserDat.UserID));

		if (-1 != HAL_Storage_MatchId(0,
									  MEM_SMARTKEY_ALL,
									  MSG_SMARTKEY_REG_MEM_ST,
									  MSG_SMARTKEY_REG_SIZE,
									  &storageSave.UserDat,
									  eMatchTypeUserID))
		{
			HAL_Storage_DelOneUserReg(storageSave.UserDat.pageID,
									  MSG_SMARTKEY_REG_MEM_ST,
									  MSG_SMARTKEY_REG_SIZE);
			storageSave.sysDat.leaveSmartKey++;
			HAL_Storage_WriteSysData(&storageSave.sysDat);
			Menu_RecordLockLog(eLogTypeDelSmartKey,
							   (uint8_t *)&(storageSave.UserDat.UserID[10]),
							   3); //保存事件记录，这里保存UserId末三位
			pBleAck->ack = eBleAckSuccess;
		}
		else
		{
			pBleAck->ack = eBleAckFail;
		}
	}
	HAL_Comm_BleSendDir(eBleDelSmartKey);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFingerAttributeModify
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
这里只修改了ID1的属性。
****************************************************************************************/
void Menu_Comm_BleFingerAttributeModify(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFingerAttributeModify\n");
	DEBUG_LOG_OUT("PageID:%x\n", BSWAP_16(pBleFingerAttributeModify->pageId.ID1));
	DEBUG_LOG_OUT("aging:%X\n", *((uint8_t *)(&pBleFingerAttributeModify->aging)));
	DEBUG_LOG_OUT("startTm:");
	DEBUG_LOG_OUT_HEX(((uint8_t *)(&pBleFingerAttributeModify->startTm)), sizeof(pBleFingerAttributeModify->startTm));
	DEBUG_LOG_OUT("agingTm:");
	DEBUG_LOG_OUT_HEX(((uint8_t *)(&pBleFingerAttributeModify->agingTm)), sizeof(pBleFingerAttributeModify->agingTm));
	DEBUG_LOG_OUT("week:%X\n", *((uint8_t *)(&pBleFingerAttributeModify->week)));
	storageSave.UserDat.pageID = BSWAP_16(pBleFingerAttributeModify->pageId.ID1);	 //pBleFingerAttributeModify
	memset((uint8_t *)&storageSave.UserDat, 0, sizeof(storageSave.UserDat)); //清空指令包
	HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
							  MSG_FINGER_ONE_SIZE,
							  storageSave.UserDat.pageID,
							  &storageSave.UserDat); //读取参数
	storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_OFF;
	if (0xf0 == (*((uint8_t *)(&pBleFingerAttributeModify->aging))))
	{
		DEBUG_LOG_OUT("storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_ON;
	}
	memcpy((uint8_t *)&storageSave.UserDat.TimeVaild.str,
		   (uint8_t *)(&pBleFingerAttributeModify->startTm),
		   sizeof(storageSave.UserDat.TimeVaild.str)); //时效开始
	memcpy((uint8_t *)&storageSave.UserDat.TimeVaild.stp,
		   (uint8_t *)(&pBleFingerAttributeModify->agingTm),
		   sizeof(storageSave.UserDat.TimeVaild.stp)); //时效结束
#if 0
	if ((0x99 == storageSave.UserDat.TimeVaild.stp.year) &&
		(0x12 == storageSave.UserDat.TimeVaild.stp.mon) &&
		(0x31 == storageSave.UserDat.TimeVaild.stp.day) &&
		(0x23 == storageSave.UserDat.TimeVaild.stp.hour) &&
		(0x59 == storageSave.UserDat.TimeVaild.stp.min) &&
		(0x59 == storageSave.UserDat.TimeVaild.stp.sec))
	{
		storageSave.UserDat.TimeVaild.fig = eAgingStatusOff; //直接关闭时效
	}
#endif
	storageSave.UserDat.TimeVaild.wday = *((uint8_t *)(&pBleFingerAttributeModify->week)); //获取周
	storageSave.UserDat.kindred = MENU_FUNCTION_OFF;
	storageSave.UserDat.sos = MENU_FUNCTION_OFF;
	if (0xf == pBleFingerAttributeModify->kindleAndSos.kindle)
	{
		DEBUG_LOG_OUT("storageSave.UserDat.kindred = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.kindred = MENU_FUNCTION_ON;
	}
	if (0xf == pBleFingerAttributeModify->kindleAndSos.SOS)
	{
		DEBUG_LOG_OUT("storageSave.UserDat.sos = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.sos = MENU_FUNCTION_ON;
	}
	HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
							   MSG_FINGER_ONE_SIZE,
							   storageSave.UserDat.pageID,
							   &storageSave.UserDat); //保存信息
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleFingerAttributeModify);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleSmartKeyAttributeModify
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleSmartKeyAttributeModify(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleSmartKeyAttributeModify\n");
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleSmartKeyAttributeModify);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleGetMcuVersion
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
下发内容：
数据包内容:0x00(国芯)
应答内容：

应答0x00设置成功, 0x01为设置失败
应答为0x00时:
数据长度为0x0011数据包格式：锁具软件版本信息
Ack+V2.0.120_20160307		
应答为0x01时:
数据长度为0x0000数据包内容：为空
****************************************************************************************/
void Menu_Comm_BleGetMcuVersion(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleGetMcuVersion\n");
	pBleAck->ack = eBleAckSuccess;
    memcpy(pBleTxData + sizeof(TYPEs_BleAck),
           &LOCK_VERSION[1],
           sizeof(LOCK_VERSION)-2);
	HAL_Comm_BleSendDirWithLen(eBleGetMcuVersion,sizeof(TYPEs_BleAck)+sizeof(LOCK_VERSION)-2);
}


/***************************************************************************************
**函数名:       Menu_Comm_BleMcuEnterUpdataMode
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleMcuEnterUpdataMode(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleMcuEnterUpdataMode\n");
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDirWithLen(eBleMcuEnterUpdataMode,sizeof(pBleAck->ack));
	DelayMS(200);//稍微等待，让后板回复APP
	pCommMsgEnterOTA->frontOtaFlag = 1;
	pCommMsgEnterOTA->updataFlag = 0;
	pCommMsgEnterOTA->backOtaFlag = 0;
	pCommMsgEnterOTA->nameLen =0;
	HAL_Comm_Send(eCommMsgEnterOTA);	
}
/***************************************************************************************
**函数名:       Menu_Comm_BleUnlockPwdSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleUnlockPwdSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleUnlockPwdSet\n");
	DEBUG_LOG_OUT("pBleUnlockPwdSet->pwd:\n");
	DEBUG_LOG_OUT_HEX(pBleUnlockPwdSet->pwd, sizeof(pBleUnlockPwdSet->pwd));
	if (-1 == Menu_AppRegChk())
	{
		DEBUG_LOG_OUT("eBleAckFail\n");
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		//本地密码加密流程
		memcpy((INT8U *)&storageSave.sysDat.pwd,
			   pBleUnlockPwdSet->pwd,
			   M_PASSWORD_MAX_LEN);
		storageSave.sysDat.pwdSetFlag |= 0x01;
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
		Menu_RecordLockLog(eLogTypeAddPwd, NULL, 0);   //保存事件记录
		pBleAck->ack = eBleAckSuccess;
		DEBUG_LOG_OUT("eBleAckSuccess\n");
	}
	HAL_Comm_BleSendDir(eBleUnlockPwdSet);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleEquipmentReg
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
成功接收数据包为
锁序列号（15byte） + 锁MAC(6byte)+信道密码(4byte)
接收失败数据全为0
****************************************************************************************/
void Menu_Comm_BleEquipmentReg(void)
{
	uint16_t i;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleEquipmentReg\n");
	pBleEquipmentRegAck->ack = eBleAckSuccess;
	Menu_GetchannelPwd(storageSave.hardwareData.channelPwd, sizeof(storageSave.hardwareData.channelPwd)); //获取信道密码
	memcpy(pBleEquipmentRegAck->lockSN,
		   LOCK_SN,
		   sizeof(pBleEquipmentRegAck->lockSN)); //填写序列号
	memcpy(pBleEquipmentRegAck->MAC,
		   storageSave.hardwareData.mac,
		   sizeof(pBleEquipmentRegAck->MAC)); //填写MAC
	memcpy(pBleEquipmentRegAck->channelPwd,
		   storageSave.hardwareData.channelPwd,
		   sizeof(pBleEquipmentRegAck->channelPwd)); //信道密码
#if 0
	memcpy(storageSave.hardwareData.channelPwd,
		   pBleEquipmentRegAck->channelPwd,
		   sizeof(storageSave.hardwareData.channelPwd));	  //保存信道密码
	HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件参数
#endif
    #if 0
	HAL_Comm_BleSendDir(eBleEquipmentReg);	//发送数据
    #endif
	fn_JumpMenu(enum_Menu_BleEquipmentReg); //进入蓝牙注册
}
/***************************************************************************************
**函数名:       Menu_Comm_BleEmptyUser
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleEmptyUser(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleEmptyUser\n");
	BSWAP_SAVE_32(pBleEmptyUser->cmd);
	if (0x25874510 == pBleEmptyUser->cmd)
	{
		fn_JumpMenu(enum_Menu_BleEmptyUser);
	}
	else
	{
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleEmptyUser);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleLockFirmwareVersionGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockFirmwareVersionGet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLockFirmwareVersionGet\n");
	memset((uint8_t *)pBleLockFirmwareVersionGetAck, 0, sizeof(TYPEs_BleLockFirmwareVersionGetAck)); //全部清0
	pBleLockFirmwareVersionGetAck->ack = eBleAckSuccess;
	memcpy((uint8_t *)pBleLockFirmwareVersionGetAck->version,
		   LOCK_VERSION,
		   sizeof(pBleLockFirmwareVersionGetAck->version));
	HAL_Comm_BleSendDir(eBleLockFirmwareVersionGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleVerifyPhoneEncrypt
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
数据包格式:信道密码(4byte)+手机属性(账号ID 13byte)+服务器时间(时间格式(年,月,日,周,小时,分钟,秒)(7byte)) (密文方式)
数据包加密方式:数据包分成8个字节一包,每包与秘钥异或成密文,最后打包发送
****************************************************************************************/
void Menu_Comm_BleVerifyPhoneEncrypt(void)
{
	uint32_t i = 0;
	DEBUG_LOG_OUT("Menu_Comm_BleVerifyPhoneEncrypt\n");
	DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.TmpKey:%x\n", st_InputInfo.privateKeyDH.TmpKey);
	if (st_InputInfo.privateKeyDH.TmpKey)
	{
		//DH加密
		for (i = 0; i < sizeof(pBleVerifyPhoneEncrypt->unData.tab); i++)
		{
#if 1
			/*
			这里采用倒叙循环异或例如：
			tab[0] ^= key[3];
			tab[1] ^= key[2];
			tab[2] ^= key[1];
			tab[3] ^= key[0];
			...
			*/
			pBleVerifyPhoneEncrypt->unData.tab[i] ^= st_InputInfo.privateKeyDH.key[(sizeof(pBleVerifyPhoneEncrypt->unData.tab) - 1 - i) % sizeof(st_InputInfo.privateKeyDH.key)];
#endif
#if 0
			pBleVerifyPhoneEncrypt->unData.tab[i] ^= st_InputInfo.privateKeyDH.key[(i)%sizeof(st_InputInfo.privateKeyDH.key)];
#endif
		}
#if 0
		pBleVerifyPhoneEncrypt->unData.tab[0] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[1] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[2] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[3] ^= st_InputInfo.privateKeyDH.key[0];
		pBleVerifyPhoneEncrypt->unData.tab[4] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[5] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[6] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[7] ^= st_InputInfo.privateKeyDH.key[0];
		pBleVerifyPhoneEncrypt->unData.tab[8] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[9] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[10] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[11] ^= st_InputInfo.privateKeyDH.key[0];
		pBleVerifyPhoneEncrypt->unData.tab[12] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[13] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[14] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[15] ^= st_InputInfo.privateKeyDH.key[0];
		pBleVerifyPhoneEncrypt->unData.tab[16] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[17] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[18] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[19] ^= st_InputInfo.privateKeyDH.key[0];
		pBleVerifyPhoneEncrypt->unData.tab[20] ^= st_InputInfo.privateKeyDH.key[3];
		pBleVerifyPhoneEncrypt->unData.tab[21] ^= st_InputInfo.privateKeyDH.key[2];
		pBleVerifyPhoneEncrypt->unData.tab[22] ^= st_InputInfo.privateKeyDH.key[1];
		pBleVerifyPhoneEncrypt->unData.tab[23] ^= st_InputInfo.privateKeyDH.key[0];
#endif
	}
	else
	{
		//对称加密
		for (i = 0; i < sizeof(pBleVerifyPhoneEncrypt->unData.tab); i++)
		{
			pBleVerifyPhoneEncrypt->unData.tab[i] ^= commDat.SecretKey[i % (sizeof(commDat.SecretKey))];
		}
	}

	Menu_GetchannelPwd(storageSave.hardwareData.channelPwd, sizeof(storageSave.hardwareData.channelPwd)); //获取信道密码
	DEBUG_LOG_OUT("commDat.SecretKey:\n");
	DEBUG_LOG_OUT_HEX(commDat.SecretKey, 8);
	DEBUG_LOG_OUT("hardwareData.channelPwd:\n");
	DEBUG_LOG_OUT_HEX(storageSave.hardwareData.channelPwd, 4);
	DEBUG_LOG_OUT("verifyPhone.channelPwd:\n");
	DEBUG_LOG_OUT_HEX(pBleVerifyPhoneEncrypt->unData.verifyPhone.channelPwd, 4);
	if ((pBleVerifyPhoneEncrypt->unData.verifyPhone.channelPwd[0] == storageSave.hardwareData.channelPwd[0]) &&
		(pBleVerifyPhoneEncrypt->unData.verifyPhone.channelPwd[1] == storageSave.hardwareData.channelPwd[1]) &&
		(pBleVerifyPhoneEncrypt->unData.verifyPhone.channelPwd[2] == storageSave.hardwareData.channelPwd[2]) &&
		(pBleVerifyPhoneEncrypt->unData.verifyPhone.channelPwd[3] == storageSave.hardwareData.channelPwd[3])) //校对信道密码
	{
		DEBUG_LOG_OUT("Menu_Comm_BleVerifyPhoneEncrypt channelPwd pass!\n");
		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
		memcpy((uint8_t *)&storageSave.UserDat.UserID,
			   pBleVerifyPhoneEncrypt->unData.verifyPhone.phone,
			   sizeof(storageSave.UserDat.UserID));
		DEBUG_LOG_OUT("phone:\n");
		DEBUG_LOG_OUT_HEX(pBleVerifyPhoneEncrypt->unData.verifyPhone.phone, sizeof(pBleVerifyPhoneEncrypt->unData.verifyPhone.phone));
		if (-1 != HAL_Storage_MatchId(0,
									  MEM_PHONE_ALL,
									  MSG_CARD_REG_MEM_ST,
									  MSG_CARD_REG_SIZE,
									  &storageSave.UserDat,
									  eMatchTypeUserID)) //校对手机号
		{
			DEBUG_LOG_OUT("Menu_Comm_BleVerifyPhoneEncrypt phone pass!\n");
			pBleVerifyPhoneEncryptAck->ack = eBleAckSuccess;
			pBleVerifyPhoneEncryptAck->bat = st_InputInfo.upBatBattery;
			BSWAP_SAVE_16(pBleVerifyPhoneEncryptAck->bat);
			HAL_Comm_BleSendDir(eBleVerifyPhoneEncrypt); //回复
			st_InputInfo.userIdTmp = 0;
			Menu_RecordLockLog(eLogTypeOpenByPhone,
							   (uint8_t *)&(st_InputInfo.userIdTmp),
							   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
			wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
            #if 1
            memset((uint8_t *)pWifiCmdUnlockRealUpdata,
                    0,
                    sizeof(TYPEs_WifiCmdUnlockRealUpdata));
            #endif
			wifiSendParm.unlockType = eWifiunlockTypePhone;
			wifiSendParm.pageId[0] = (uint8_t)(0);
            wifiSendParm.pageId[1] = (uint8_t)(0);
            wifiSendParm.pageId[2] = (uint8_t)(st_InputInfo.userID >> 8);
            wifiSendParm.pageId[3] = (uint8_t)(st_InputInfo.userID); //PageID
            wifiSendParm.limt = 0;
            wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
			fn_JumpMenu(enum_Menu_BleVerifyPhoneEncryptSuccess); //进入开门。
		}
		else
		{
			//手机号错误
			DEBUG_LOG_OUT("Menu_Comm_BleVerifyPhoneEncrypt phone error!\n");
			Menu_AddError(&st_InputInfo.errorCnt, eUserTypeAll);
			pBleVerifyPhoneEncryptAck->ack = eBleAckFail;
			HAL_Comm_BleSendDir(eBleVerifyPhoneEncrypt);
			fn_JumpMenu(enum_Menu_SystemSleep);
		}
	}
	else
	{
		//信道密码错误
		DEBUG_LOG_OUT("Menu_Comm_BleVerifyPhoneEncrypt channelPwd error\n");
		Menu_AddError(&st_InputInfo.errorCnt, eUserTypeFinger);
		pBleVerifyPhoneEncryptAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleVerifyPhoneEncrypt);
		fn_JumpMenu(enum_Menu_SystemSleep);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleTmpeSecretKeyGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleTmpeSecretKeyGet(void)
{
	uint32_t i = 0;
	uint8_t tmp = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleTmpeSecretKeyGet\n");
	memset((uint8_t *)&pBleTmpeSecretKeyGetAck->ack,
		   0,
		   sizeof(TYPEs_BleTmpeSecretKeyGetAck));

	if (-1 == Menu_AppRegChk())
	{
		pBleTmpeSecretKeyGetAck->ack = eBleAckFail;
	}
	else
	{

		Menu_RTCTimerGet(); //获取时间
		memset((uint8_t *)&storageSave.sysDat.tempPwd,
			   0,
			   sizeof(storageSave.sysDat.tempPwd));
		tmp = st_InputInfo.RTCTimer.tm_sec;

		DEBUG_LOG_OUT_HEX((uint8_t *)&pBleTmpeSecretKeyGetAck->pwd[0], HAL_STORAGE_PWD_LEN);
		for (i = 0; i < HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE; i++)
		{
			storageSave.sysDat.tempPwd[i][0] = ((tmp | i) + 5) % 10 + 0x30;
			storageSave.sysDat.tempPwd[i][1] = 0x39 - i;
			tmp = tmp + 0x03;
			storageSave.sysDat.tempPwd[i][2] = (st_InputInfo.RTCTimer.tm_min * i + tmp) % 10 + 0x30;
			tmp = tmp * i % 10;
			storageSave.sysDat.tempPwd[i][3] = tmp % 10 + 0x30;
			tmp = tmp + st_InputInfo.RTCTimer.tm_min;
			storageSave.sysDat.tempPwd[i][4] = ((tmp | i) + 7) % 10 + 0x30;
			tmp = tmp + i;
			storageSave.sysDat.tempPwd[i][5] = (tmp ^ i + 4) % 10 + 0x30;
			tmp = tmp + st_InputInfo.RTCTimer.tm_hour;
			storageSave.sysDat.tempPwd[i][6] = 0xff;
			storageSave.sysDat.tempPwd[i][7] = 0xff;
			memcpy((uint8_t *)&(pBleTmpeSecretKeyGetAck->pwd[i * HAL_STORAGE_PWD_LEN]),
				   (uint8_t *)&(storageSave.sysDat.tempPwd[i][0]),
				   HAL_STORAGE_PWD_LEN);
		} //获取密码
		DEBUG_LOG_OUT("pBleAESTmpeSecretKeyGetAck->pswAll:\n");
		DEBUG_LOG_OUT_HEX((uint8_t *)&pBleTmpeSecretKeyGetAck->pwd[0], 80);
		DEBUG_LOG_OUT("storageSave.sysDat.tempPwd:\n");
		DEBUG_LOG_OUT_HEX((uint8_t *)&storageSave.sysDat.tempPwd[0], 80);
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
		Menu_RecordLockLog(eLogTypeAddPwd, NULL, 0);   //保存事件记录
		pBleTmpeSecretKeyGetAck->ack = eBleAckSuccess;
	}
	HAL_Comm_BleSendDir(eBleTmpeSecretKeyGet);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleLockPwdVerifyAdmin
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockPwdVerifyAdmin(void)
{
	DEBUG_LOG_OUT("Menu_Comm_BleLockPwdVerifyAdmin\n");
	DEBUG_LOG_OUT("dhG:%d\n", pBleLockPwdVerifyAdmin->dhG);
	DEBUG_LOG_OUT("dhP:%d\n", pBleLockPwdVerifyAdmin->dhP);
#if 0
	pBleLockPwdVerifyAdminACK->ack = eBleLockPwdVerifyAdminEmpty; //测试，已经清空
#endif
#if 1
	if (-1 == Menu_AppRegChk())
	{
		if (st_InputInfo.iD2Status == eID2StatusError)
		{
			if ((pBleLockPwdVerifyAdmin->dhG > 4) ||
				(pBleLockPwdVerifyAdmin->dhP > 9))
			{
				pBleLockPwdVerifyAdminACK->ack = eBleLockPwdVerifyAdminFail;
			}
			else
			{
				pBleLockPwdVerifyAdminACK->ack = eBleLockPwdVerifyAdminEmpty;
				storageSave.hardwareData.dhG = pBleLockPwdVerifyAdmin->dhG;
				storageSave.hardwareData.dhP = pBleLockPwdVerifyAdmin->dhP;
				HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
				DEBUG_LOG_OUT("storageSave.sysDat.dhG:%X\n", storageSave.hardwareData.dhG);
				DEBUG_LOG_OUT("storageSave.sysDat.dhP:%X\n", storageSave.hardwareData.dhP);
#if 0
				storageSave.sysDat.dhG = 0;
				storageSave.sysDat.dhP = 0;
				HAL_Storage_ReadSysData(&storageSave.sysDat);
				DEBUG_LOG_OUT("storageSave.sysDat.dhG:%X\n",storageSave.sysDat.dhG);
				DEBUG_LOG_OUT("storageSave.sysDat.dhP:%X\n",storageSave.sysDat.dhP);
#endif
			}
		}
		else
		{
			pBleLockPwdVerifyAdminACK->ack = eBleLockPwdVerifyAdminEmpty;
		}
	}
	else
	{
		pBleLockPwdVerifyAdminACK->ack = eBleLockPwdVerifyAdminSuccess;
	}
#endif
	HAL_Comm_BleSendDir(eBleLockPwdVerifyAdmin);
}
/***************************************************************************************
**函数名:       Menu_SecretKeyGet
**功能描述:     获取密钥
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
int8_t Menu_SecretKeyGet(uint8_t *p, uint32_t len)
{
	HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
#if 0
	*p = BCD_TO_BIN(BIN_TO_BCD(st_InputInfo.RTCTimer.tm_sec) + 1);
	*(p + 1) = BCD_TO_BIN(BIN_TO_BCD(st_InputInfo.RTCTimer.tm_min) + 2);
	*(p + 2) = BCD_TO_BIN(BIN_TO_BCD(st_InputInfo.RTCTimer.tm_hour) + 3);
	*(p + 3) = st_InputInfo.RTCTimer.tm_wday + 4;
	*(p + 4) = st_InputInfo.RTCTimer.tm_mday | st_InputInfo.RTCTimer.tm_sec;
	*(p + 5) = st_InputInfo.RTCTimer.tm_mon | st_InputInfo.RTCTimer.tm_sec;
	*(p + 6) = (st_InputInfo.RTCTimer.tm_year - 2000) | st_InputInfo.RTCTimer.tm_sec;
	*(p + 7) = (st_InputInfo.RTCTimer.tm_mday | st_InputInfo.RTCTimer.tm_sec) + 5;
#else
		memset(p,0,8);
#endif
	return 0;
}
/***************************************************************************************
**函数名:       Menu_Comm_BlePhoneSecretKeyGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BlePhoneSecretKeyGet(void)
{
	DEBUG_LOG_OUT("Menu_Comm_BlePhoneSecretKeyGet\n");
	pBlePhoneSecretKeyGetAck->ack = eBleAckSuccess;
	Menu_SecretKeyGet(pBlePhoneSecretKeyGetAck->key, 8);
	memcpy(commDat.SecretKey, pBlePhoneSecretKeyGetAck->key, sizeof(commDat.SecretKey));
    DEBUG_LOG_OUT("commDat.SecretKey\n");
    DEBUG_LOG_OUT_HEX(commDat.SecretKey, sizeof(commDat.SecretKey));
	pBlePhoneSecretKeyGetAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBlePhoneSecretKeyGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleSmartKeySecretKeyGetRandom
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleSmartKeySecretKeyGetRandom(void)
{
	DEBUG_LOG_OUT("Menu_Comm_BleSmartKeySecretKeyGetRandom\n");
	pBleSmartKeySecretKeyGetRandom->ack = eBleAckSuccess;
	Menu_SecretKeyGet(pBleSmartKeySecretKeyGetRandom->key, 8);
	memcpy(commDat.SecretKey, pBleSmartKeySecretKeyGetRandom->key, sizeof(commDat.SecretKey));
	pBleSmartKeySecretKeyGetRandom->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleSmartKeySecretKeyGetRandom);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleSmartKeySecretKeyGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleSmartKeySecretKeyGet(void)
{
	DEBUG_LOG_OUT("Menu_Comm_BleSmartKeySecretKeyGet\n");
	pBleSmartKeySecretKeyGet->ack = eBleAckSuccess;
	Menu_SecretKeyGet(pBleSmartKeySecretKeyGet->key, 8);
	memcpy(commDat.SecretKey, pBleSmartKeySecretKeyGet->key, sizeof(commDat.SecretKey));
	pBleSmartKeySecretKeyGet->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleSmartKeySecretKeyGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleLockRegSecretKeyGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockRegSecretKeyGet(void)
{
	DEBUG_LOG_OUT("Menu_Comm_BleLockRegSecretKeyGet\n");
	pBleLockRegSecretKeyGet->ack = eBleAckSuccess;
	HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
	Menu_SecretKeyGet(pBleLockRegSecretKeyGet->key, 8);
	memcpy(commDat.SecretKey, pBleLockRegSecretKeyGet->key, sizeof(commDat.SecretKey));
	pBleLockRegSecretKeyGet->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleLockRegSecretKeyGet);
}
/***************************************************************************************
**函数名:       Menu_SmartKeyMatch
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
1.加密规则,约定根密钥,如下“0x90,0x88,0x42,0x53,0x58,0xA9,0x78,0x1F,0x70,0x18,0x32,0x63,0x98,0xB9,0xC8,0xDF”
0.1手机下发帐号0x27秘钥钥匙帐号13byte。
1.2 取秘钥钥匙帐号数组下标2，4，6，8，10byte，与约定根密钥前5字节异或加密，新生成根密钥。
2.会话密钥：0x3C获取8字节随机数。
3.AES密钥：会话密钥循环异或约定(1.2)新根密钥。
4.通过0x3B指令发送，加密后的帐号密码。

由于缺少密钥，需要对存储的密钥进行加密，校对加密数据!
****************************************************************************************/
int8_t Menu_SmartKeyMatch(uint16_t *id, uint8_t *dat, uint len)
{
	uint16_t i, j;
	uint8_t const rootPwdDef[16] = {0x90, 0x88, 0x42, 0x53, 0x58, 0xA9, 0x78, 0x1F, 0x70, 0x18, 0x32, 0x63, 0x98, 0xB9, 0xC8, 0xDF}; //根密钥
	uint8_t rootPwd[16] = {0x90, 0x88, 0x42, 0x53, 0x58, 0xA9, 0x78, 0x1F, 0x70, 0x18, 0x32, 0x63, 0x98, 0xB9, 0xC8, 0xDF};			 //根密钥
	uint8_t mathTmp[32] = {
		0x00,
	};
	DEBUG_LOG_OUT("dat:\n");
	DEBUG_LOG_OUT_HEX(dat, len);
	Menu_GetchannelPwd(storageSave.hardwareData.channelPwd, sizeof(storageSave.hardwareData.channelPwd));
	for (i = 0; i < MEM_SMARTKEY_ALL; i++)
	{
		memset((uint8_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat));
		memset(mathTmp, 0, sizeof(mathTmp));					 //清除剩余，注意一定要清除
		memcpy(mathTmp, storageSave.hardwareData.channelPwd, 4); //通道密码
		memcpy(rootPwd, rootPwdDef, sizeof(rootPwd));			 //通道密码
		HAL_Storage_GetOneUserReg(MSG_SMARTKEY_REG_MEM_ST,
								  MSG_SMARTKEY_REG_SIZE,
								  i,
								  &storageSave.UserDat); //读取参数
		if (MSG_FINGERREGMEM_FIG == storageSave.UserDat.vaild)
		{
			for (j = 0; j < 5; j++) //根秘钥下标2/4/6/8/10位异或
			{
				rootPwd[j] ^= storageSave.UserDat.UserID[(j + 1) * 2];
			}
			for (j = 0; j < sizeof(rootPwd); j++) //8字节随机数异或
			{
				rootPwd[j] ^= commDat.SecretKey[j % (sizeof(commDat.SecretKey))];
			}
			memcpy(&mathTmp[4], (uint8_t *)&storageSave.UserDat.UserID, 13); //拷贝用户ID
			DEBUG_LOG_OUT("storageSave.UserDat.UserID:\n");
			DEBUG_LOG_OUT_HEX((uint8_t *)&storageSave.UserDat.UserID, sizeof(storageSave.UserDat.UserID));
			aes_encrypt_ecb(AES_CYPHER_128,
							mathTmp,
							32,
							rootPwd); //加密
			DEBUG_LOG_OUT("Menu_SmartKeyMatch match aes:\n");
			DEBUG_LOG_OUT_HEX(mathTmp, sizeof(mathTmp));
			for (j = 0; j < len; j++)
			{
				if (mathTmp[j] != dat[j])
				{
					break;
				}
			}
			if (j == (len))
			{
				*id = i;  //给出id
				return 0; //成功
			}
		}
	}
	return -1; //未匹配
}
/***************************************************************************************
**函数名:       Menu_Comm_BleUnlockSmartKeyVerify
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleUnlockSmartKeyVerify(void)
{
	uint16_t id = 0;
	DEBUG_LOG_OUT("Menu_Comm_BleUnlockSmartKeyVerify\n");
	if (0 == Menu_SmartKeyMatch(&id, pBleUnlockSmartKeyVerify->unData.tab, 17))
	{
		DEBUG_LOG_OUT("Menu_Comm_BleUnlockSmartKeyVerify Success\n");
		pBleAck->ack = eBleAckSuccess;
		HAL_Comm_BleSendDir(eBleUnlockSmartKeyVerify); //蓝牙回复
		st_InputInfo.userIdTmp = 0;
		Menu_RecordLockLog(eLogTypeOpenBySmartKey,
						   (uint8_t *)&(st_InputInfo.userIdTmp),
						   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
		wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
		wifiSendParm.unlockType = eWifiunlockTypeSmartKey;
        wifiSendParm.pageId[0] = (uint8_t)(0);
        wifiSendParm.pageId[1] = (uint8_t)(0);
        wifiSendParm.pageId[2] = (uint8_t)(st_InputInfo.userID >> 8);
        wifiSendParm.pageId[3] = (uint8_t)(st_InputInfo.userID); //PageID
        wifiSendParm.limt = 0;
        wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
        
		fn_JumpMenu(enum_Menu_BleUnlockSmartKeyVerifySuccess);
	}
	else
	{
		DEBUG_LOG_OUT("Menu_Comm_BleUnlockSmartKeyVerify Fail\n");
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleUnlockSmartKeyVerify); //蓝牙回复
		fn_JumpMenu(enum_Menu_BleUnlockSmartKeyVerifyFail);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleTestServerConnectionStatus
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleTestServerConnectionStatus(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleTestServerConnectionStatus\n");
	if (eInPowerStatusLow == Menu_CheckWifiPower()) //如果电压低，回复失败
	{
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDirWithLen(eBleTestServerConnectionStatus, sizeof(TYPEs_BleAck));
	}
	else
	{
        #if 1
        memset((uint8_t *)pWifiCmdUnlockRealUpdata,
                0,
                sizeof(TYPEs_WifiCmdUnlockRealUpdata));
        #endif
        wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
        wifiSendParm.unlockType = eWifiunlockTypeWifiTest;
        memset(wifiSendParm.pageId,0,sizeof(wifiSendParm.pageId));
        wifiSendParm.limt = 0;
        wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
		//进行WIFI测试
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleWifiFunctionSwitchSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleWifiFunctionSwitchSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleWifiFunctionSwitchSet\n");
	//关闭wifi
	if(eWifiFunctionSwitchSetOpen == pBleWifiFunctionSwitchSet->sw)
	{
        DEBUG_LOG_OUT("MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG\n");
		storageSave.sysDat.wifiMainSw = MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG; 
	}
	else
	{
        DEBUG_LOG_OUT("MSG_WIFI_LOGUP_MAIN_SW_OFF_CONFIG\n");
		storageSave.sysDat.wifiMainSw = MSG_WIFI_LOGUP_MAIN_SW_OFF_CONFIG;
	}
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleWifiFunctionSwitchSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDisconnectBluetooth
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDisconnectBluetooth(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDisconnectBluetooth\n");
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleDisconnectBluetooth);
	HAL_Comm_SendDealStart(eCommMsgDisconnectBLE, 200); //发送指令
    Menu_Face_PowerOffStart();                          //关闭人脸电源
	fn_JumpMenu(enum_Menu_BackDeal);					//进入后台处理程序
	HAL_FaceId_CmdStop();								//关闭所有指令
	DEBUG_LOG_OUT("HAL_FaceId_ChkBusy:%d", HAL_FaceId_ChkBusy());
}
/***************************************************************************************
**函数名:       Menu_Comm_BleLockUpdateProgramMode
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockUpdateProgramMode(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLockUpdateProgramMode\n");
	//进入升级
	
	pCommMsgUpdateDeviceName->ovetTime = 100; //
	pCommMsgUpdateDeviceName->nameLen = sizeof(gLockUpdataBleName) - 1;
	memcpy(&pCommMsgUpdateDeviceName->nameFirst,
			gLockUpdataBleName,
			pCommMsgUpdateDeviceName->nameLen); //填写蓝牙名
	pCommMsgUpdateDeviceName->sysStatus = 2; //通知后板OTA
	HAL_Comm_SendDealStart(eCommMsgUpdateDeviceName, HAL_COMM_SEND_WAIT_TIEM);
	
}
/***************************************************************************************
**函数名:       Menu_Comm_BleNewAddFinger
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleNewAddFinger(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleNewAddFinger\n");
	//添加处理代码
	if (storageSave.sysDat.localUserNum < MEM_FINGER_GUEST_ALL_LOCAL) //检查普通普通数量
	{
		//            storageSave.UserDat.Lim = MSG_FACTMEM_LIM_GENERAL; //普通类型
		st_InputInfo.FingerIdTmp = HAL_Storage_SearchEmpty(0,
														   MEM_FINGER_ALL_LOCAL,
														   MSG_FINGER_REG_MEM_LOCAL_ST,
														   MSG_FINGER_ONE_SIZE); //获取空位
																				 //没有空位的时候，不会进入到这里
																				 //		st_InputInfo.FingerIdTmp = 5;
		DEBUG_LOG_OUT("Menu_Comm_BleNewAddFinger st_InputInfo.FingerIdTmp:%d\n", st_InputInfo.FingerIdTmp);
		fn_JumpMenu(enum_Menu_BleAddFingerStep1); //进入注册流程。
#if defined(SYSTEM_FINGER_EN)
		HAL_Finger_RegStart(); //开启注册过程
#endif						   // SYSTEM_FINGER_EN
		pBleNewAddFingerAck->ack = eBleAddFingerAckIng;
		pBleNewAddFingerAck->add.addIng.cnt = 0;
	}
	else
	{
		pBleNewAddFingerAck->ack = eBleAddFingerAckFail; //已满失败
	}
	HAL_Comm_BleSendDir(eBleNewAddFinger);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleNewConfirmFingerRegSucced
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleNewConfirmFingerRegSucced(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleNewConfirmFingerRegSucced\n");
	pBleAck->ack = eBleAckSuccess;
	fn_JumpMenu(enum_Menu_BleAddFingerOK);
	//	pBleAck->ack = eBleAckFail;
	HAL_Comm_BleSendDir(eBleNewConfirmFingerRegSucced);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleLockVolumeSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockVolumeSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLockVolumeSet\n");
	storageSave.sysDat.voiceSw = pBleLockVolumeSet->volumeStatus;
    #if 0
    DEBUG_LOG_OUT("storageSave.sysDat.voiceSw:%d\n",storageSave.sysDat.voiceSw);
    #endif
	HAL_Storage_WriteSysData(&storageSave.sysDat);
    
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleLockVolumeSet);
}
typedef struct
{
	uint8_t token_len;
	uint8_t token_buf[128];
}ID2_SE;
ID2_SE id2_se;
uint8_t output_r1[64]={0};
/*************************************************************************************** 
**函数名:       Menu_Comm_BleServerTokenVerify                                             
**功能描述:                                                                              
**输入参数:     --                                                                       
**输出参数:     --                                                                       
**备注:                                                                                  
                                                                                         
****************************************************************************************/
void Menu_Comm_BleServerTokenVerify(void)                                                  
{  
    int ret = 0;
    uint32_t dec_len = 256;
    uint8_t dec_out[256] = {0};
    uint8_t dec_in[256] = {0};
    uint32_t dec_in_len = 0;
    uint8_t verify_flag = 0; //token验证校验标志，0，未校验通过，1，校验通过   
#if 0                                                                                    
	if (eAPPModeNone == st_InputInfo.appMode)                                            
	{                                                                                    
		return;                                                                          
	}                                                                                    
#endif                                                                                   
	DEBUG_LOG_OUT("Menu_Comm_BleServerTokenVerify\n"); 
    Menu_Face_PowerOffStart(); //人脸下电
    HAL_FaceId_VerifyUserStop();    
        
        dec_len =  BSWAP_16(*pBleRxLen); //获取长度
        DEBUG_LOG_OUT("dec_len %d\n",dec_len);
        my_base64_decode(pBleRxData, dec_len, dec_in, (int *)&dec_in_len);
    DEBUG_LOG_OUT("pBleRxData:");
    DEBUG_LOG_OUT_HEX(pBleRxData,dec_len);

        DEBUG_LOG_OUT("dec_in_len %d\n",dec_in_len);

        //			decrypt cipher text with ID2 key
        ret = id2_client_decrypt(dec_in, dec_in_len, dec_out, &dec_len);

        if (ret)
        {
            for (uint8_t i = 0; i < 2; i++)
            {
                DEBUG_LOG_OUT("\r\n**************  id2_client_decrypt again! try_times=%d!*************\r\n", i);
                ret = id2_client_decrypt(dec_in, dec_in_len, dec_out, &dec_len);

                if (ret != 0)
                {
                    DEBUG_LOG_OUT("\r\n\r\n**************tfs_id2_decrypt fail *************\r\n\r\n");
                    DEBUG_LOG_OUT("tfs_id2_decrypt return %d error!\r\n", ret);
                }
                else
                {
                    DEBUG_LOG_OUT("\r\ndecrypt succuss dec_out = %s\r\n", dec_out);
                    break;
                }
            }
            DEBUG_LOG_OUT("\r\n\r\n**************tfs_id2_decrypt fail   time out*************\r\n\r\n");
            DEBUG_LOG_OUT("tfs_id2_decrypt return %d error!\r\n", ret);
            //					return;
        }
        else
        {
            DEBUG_LOG_OUT("\r\ndecrypt succuss dec_out = %s\r\n", dec_out);
        }

        //step3: verify the token
        //get mac addr
        /*  plaintext */
        uint8_t plaintext[6];
        char mac_addr[18];
        #if 0
        plaintext[5] = storageSave.hardwareData.mac[0];
        plaintext[4] = storageSave.hardwareData.mac[1];
        plaintext[3] = storageSave.hardwareData.mac[2];
        plaintext[2] = storageSave.hardwareData.mac[3];
        plaintext[1] = storageSave.hardwareData.mac[4];
        plaintext[0] = storageSave.hardwareData.mac[5];
        #else
        plaintext[5] = storageSave.hardwareData.mac[5];
        plaintext[4] = storageSave.hardwareData.mac[4];
        plaintext[3] = storageSave.hardwareData.mac[3];
        plaintext[2] = storageSave.hardwareData.mac[2];
        plaintext[1] = storageSave.hardwareData.mac[1];
        plaintext[0] = storageSave.hardwareData.mac[0];    
        #endif
        uint8_t i = 0;
        for (i = 0; i < 6; i++) //mac转字符串
        {
            mac_addr[i * 3] = my_int2char((plaintext[i] & 0xF0) >> 4);
            mac_addr[i * 3 + 1] = my_int2char(plaintext[i] & 0x0f);
            if (i != 5)
                mac_addr[i * 3 + 2] = ':';
            else
                mac_addr[i * 3 + 2] = '\0';
        }

        if (strstr((const char *)dec_out, mac_addr) != NULL /*&&ret== 0*/)
        {

                id2_se.token_buf[0] = (uint8_t)(dec_len >> 8);
                id2_se.token_buf[1] = (uint8_t)(dec_len & 0x000000ff);
                strncpy(&id2_se.token_buf[2], dec_out, dec_len);
			    verify_flag=1;
        }
		else
        {
            verify_flag=0;
        }
        
        if (1 == verify_flag)//计算原文哈希值
        {
			 impl_sha256(dec_out,
                         dec_len,
                         output_r1,
                         0);
            pBleAck->ack = eBleAckSuccess;
            memcpy(pBleTxData + sizeof(TYPEs_BleAck),
                   output_r1,
                   32);
            HAL_Comm_BleSendDirWithLen(eBleServerTokenVerify,33);//回复
            
//			 my_print_hex("impl_sha256= ", output_r1,32);
//			 ble.app.len = 33;
//			 ble.app.buf[0] = 0x0;
//			 memcpy(&ble.app.buf[1], output_r1, 32);	
        }
        else
        {
            //token invalid,get ID2
            uint32_t len = ID2_ID_LEN;
            uint8_t id2[ID2_ID_LEN + 1] = {0};
            uint8_t id2_flag = 0;

            ret = id2_client_get_id(id2, &len); //获取ID2
            if (ret != 0)
            {

                for (uint8_t i = 0; i < 10; i++)
                {
                    //											printf("tfs_get_ID2 return %d error!\r\n", ret);
                    //											printf("\r\n**************  tfs_get_ID2 again!len=%d *************\r\n",len);
                    ret = id2_client_get_id(id2, &len);

                    if (ret != 0)
                    {
                        DEBUG_LOG_OUT("\r\n\r\n************** tfs_get_ID2 return %d error try_times=%d! **************\r\n\r\n", ret, i);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (0 == ret)
            {
				DEBUG_LOG_OUT("the ID2: %s\r\n", id2);
                
				//token invalid,send ID2
//				ble.app.len = 25;                         //1(ack)+24(id2)
//				ble.app.buf[0] = 0x01;                    //验证失败
//				memcpy(&ble.app.buf[1], id2, ID2_ID_LEN); //复制ID2
                pBleAck->ack = eBleAckFail;
                memcpy(pBleTxData + sizeof(TYPEs_BleAck),
                       id2,
                       ID2_ID_LEN);
                HAL_Comm_BleSendDirWithLen(eBleServerTokenVerify,25);//回复
				DEBUG_LOG_OUT("dec_out != plaintext,dec_out = %s,mac_addr=%s check your ciphertext!\r\n", dec_out, mac_addr);
			}

        }                                                                                                     
}                                                                                        

/*************************************************************************************** 
**函数名:       Menu_Comm_BleServerIssueChallenge                                             
**功能描述:                                                                              
**输入参数:     --                                                                       
**输出参数:     --                                                                       
**备注:                                                                                  
                                                                                         
****************************************************************************************/
void Menu_Comm_BleServerIssueChallenge(void)                                                  
{                                                                                        
#if 0                                                                                    
	if (eAPPModeNone == st_InputInfo.appMode)                                            
	{                                                                                    
		return;                                                                          
	}                                                                                    
#endif                                                                                   
	DEBUG_LOG_OUT("Menu_Comm_BleServerIssueChallenge\n");     
    int ret = -1;    
    uint32_t len = 256;
        uint8_t auth_code[256] = {0};
        char challenge[256] = {0};                                                                   /* get from ID2 server! */
        len = BSWAP_16(*pBleRxLen); //获取长度
        memcpy(challenge, pBleRxData, len);                                                       //复制challenge
        len = 256;
        /* test without extra data */
        ret = id2_client_get_challenge_auth_code((const char *)challenge, NULL, 0, auth_code, &len);
        DEBUG_LOG_OUT("id2_client_get_challenge_auth_code ret=%d\r\n",ret);  
        if (ret != 0)
        {
            ret = id2_client_get_challenge_auth_code((const char *)challenge, NULL, 0, auth_code, &len);
             DEBUG_LOG_OUT("id2_client_get_challenge_auth_code ret123=%d\r\n",ret);  
            if (ret != 0)
            {
                pBleAck->ack = eBleAckFail;
                HAL_Comm_BleSendDirWithLen(eBleServerIssueChallenge,1);
//                ble.app.type = BLE_APP_TYPE_RSP;
//                ble.app.cmd = BLE_SE_CHALLENGE;
//                ble.app.len = 1;
//                ble.app.buf[0] = 0x1;     //失败
//                BLE_Respond_Ready_DATA(); //发送数据至手机
                return;
            }
        }
        pBleAck->ack = eBleAckSuccess;
        memcpy(pBleTxData + sizeof(TYPEs_BleAck),
               auth_code,
               len);
        HAL_Comm_BleSendDirWithLen(eBleServerIssueChallenge,1+len);//回复
        
        
        //step2: send authcode to server
//        ble.app.type = BLE_APP_TYPE_RSP;
//        ble.app.cmd = BLE_SE_CHALLENGE;
//        ble.app.len = 1 + len;
//        ble.app.buf[0] = 0x0; //成功
//        memcpy(&ble.app.buf[1], auth_code, len);
//        BLE_Respond_Ready_DATA(); //发送数据至手机                                                                                
//	pBleAck->ack = eBleAckSuccess;                                                       
//	HAL_Comm_BleSendDir(eBleServerIssueChallenge);                                            
}                                                                                        
/***************************************************************************************
**函数名:       Menu_Comm_BleID2ServerSwitch
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleID2ServerSwitch(void)
{
#if 0
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
#endif
	DEBUG_LOG_OUT("Menu_Comm_BleID2ServerSwitch\n");
	if ((0 == pBleID2ServerSwitch->sw) ||
		(1 == pBleID2ServerSwitch->sw))
	{
		storageSave.sysDat.ID2Flag = pBleID2ServerSwitch->sw;
		HAL_Storage_WriteSysData(&storageSave.sysDat);
	}
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleID2ServerSwitch);
}
/***************************************************************************************
**函数名:       quick
**功能描述:     计算啊a的b次方对c取模 
**输入参数:     
uint32_t a,底数
uint32_t b,指数
uint32_t c 模
**输出参数:     --
**备注:         
****************************************************************************************/
uint32_t Quick(uint32_t a, uint32_t b, uint32_t c)
{
	uint32_t ans = 1;
	a = a % c;	  //取余
	while (b > 0) //判断指数是否为0
	{
		if (b & 1)
			ans = (ans * a) % c; //奇数次方先取模
		b >>= 1;				 // 指数除2
		a = (a * a) % c;		 //底数2次方对C取模
	}
	return ans;
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDHKeyExchangeAlgorithm
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
密钥生成过程：注册过程中约定质数P，原根a，双方存储。
1、手机端生成随机数XA1（XA1<P），，计算A1=a^XA1 modp。
1.1手机端生成随机数XA2（XA2<P），，计算A2=a^XA2 modp作为数据包发送。
1.2锁端生成随机数XB1（XB1<P），计算B1=a^XB1 modp。
1.3锁端生成随机数XB2（XB2<P），计算B2=a^XB2 modp回应。
2、手机端计算K1=B1^XA1 modp,K3=B2^XA2 modp,  锁端计算K2=A1^XB1 modp，K4=A2^XB2 modp，此算法中K1=K2,K3=K4，作为替代手机秘钥获取 (0x3A)8字节手机开门密钥使用。4字节（K3 |= K1<<16）与用户数据循环异或加密，通过原通道0x39发送。
3、后续通信中数据包分成4个字节一包,每包与秘钥异或成密文。
4、原根a:  uint8 a[5] = {2,3,5,7,11};  使用数组位号来传递值，见0x36指令。
5、质数P:  uint16 P[10] = {811,919,991,1117,1439,1627,1861,1907,1951,1993};  使用数组位号来传递值，见0x36指令。
6、注：XA,XB随机数小于质数P内的正整理。
7、ACK：0,表示成功；1，表示失败，数据包为空
****************************************************************************************/
void Menu_Comm_BleDHKeyExchangeAlgorithm(void)
{
	uint32_t keyB1 = 0;
	uint32_t keyB2 = 0;
	uint32_t random1 = 0;
	uint32_t random2 = 0;
	uint32_t randomTmp = 0;
	uint32_t keyA1 = 0;
	uint32_t keyA2 = 0;
	uint8_t dhG = storageSave.hardwareData.dhG;
	uint8_t dhP = storageSave.hardwareData.dhP;
#if 0
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
#endif
	DEBUG_LOG_OUT("Menu_Comm_BleDHKeyExchangeAlgorithm\n");
#if 1
	keyB1 = BSWAP_16(pBleDHKeyExchangeAlgorithm->A1);
	keyB2 = BSWAP_16(pBleDHKeyExchangeAlgorithm->A2);
#endif
#if 0
	keyB1 = (pBleDHKeyExchangeAlgorithm->A1);
	keyB2 = (pBleDHKeyExchangeAlgorithm->A2);
#endif
#if 0
	DEBUG_LOG_OUT("keyB1 test x:%X\n",((*pBleRxData) <<8) | ((*(pBleRxData+1))));
	DEBUG_LOG_OUT("keyB1 test d:%d\n",((*pBleRxData) <<8) | ((*(pBleRxData+1))));
	DEBUG_LOG_OUT("keyB2 test x:%X\n",((*(pBleRxData+2)) <<8) | ((*(pBleRxData+3))));
	DEBUG_LOG_OUT("keyB2 test d:%d\n",((*(pBleRxData+2)) <<8) | ((*(pBleRxData+3))));
#endif
	DEBUG_LOG_OUT("keyB2 x:%X\n", keyB2);
	DEBUG_LOG_OUT("keyB1 x:%X\n", keyB1);
	DEBUG_LOG_OUT("keyB2 x:%X\n", keyB2);
	DEBUG_LOG_OUT("keyB1 d:%d\n", keyB1);
	DEBUG_LOG_OUT("keyB2 d:%d\n", keyB2);
	DEBUG_LOG_OUT("dhG:%X\n", dhG);
	DEBUG_LOG_OUT("dhP:%X\n", dhP);
	if ((dhG > 4) ||
		(dhP > 9) ||
		(keyB1 < 2) ||
		(keyB2 < 2))
	{
		pBleAck->ack = eBleAckFail; //失败
		HAL_Comm_BleSendDirWithLen(eBleDHKeyExchangeAlgorithm, sizeof(TYPEs_BleAck));
	}
	else
	{
		randomTmp = HAL_Power_GetRandomWord(); //获取随机数32字节
#if 1
		random1 = ((uint16_t)(randomTmp)) % gPrime[dhP];
		random2 = ((uint16_t)(randomTmp >> 16)) % gPrime[dhP]; // 获取两个随机数
#endif
#if 0
		random1 = 3% gPrime[dhP];
		random2 = 2% gPrime[dhP]; // 获取两个随机数
#endif
		keyA1 = Quick(gPrimary[dhG], random1, gPrime[dhP]);
		keyA2 = Quick(gPrimary[dhG], random2, gPrime[dhP]);
		DEBUG_LOG_OUT("((uint16_t)Quick(keyB1, random1, gPrime[dhP])):%X\n", ((uint16_t)Quick(keyB1, random1, gPrime[dhP])));
		DEBUG_LOG_OUT("((uint16_t)Quick(keyB2, random2, gPrime[dhP])):%X\n", ((uint16_t)Quick(keyB2, random2, gPrime[dhP])));
		st_InputInfo.privateKeyDH.TmpKey = ((((uint16_t)Quick(keyB1, random1, gPrime[dhP])) << 16) |
											((uint16_t)Quick(keyB2, random2, gPrime[dhP])));
#if 0
		BSWAP_SAVE_32(st_InputInfo.privateKeyDH.TmpKey);
#endif
		DEBUG_LOG_OUT("keyA1:%X\n", keyA1);
		DEBUG_LOG_OUT("keyA2:%X\n", keyA2);
		DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.TmpKey:%X\n", st_InputInfo.privateKeyDH.TmpKey);
		DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.key[0]:%X\n", st_InputInfo.privateKeyDH.key[0]);
		DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.key[1]:%X\n", st_InputInfo.privateKeyDH.key[1]);
		DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.key[2]:%X\n", st_InputInfo.privateKeyDH.key[2]);
		DEBUG_LOG_OUT("st_InputInfo.privateKeyDH.key[3]:%X\n", st_InputInfo.privateKeyDH.key[3]);
		pBleDHKeyExchangeAlgorithmAck->ack = eBleAckSuccess;
#if 1
		pBleDHKeyExchangeAlgorithmAck->A1 = BSWAP_16(keyA1);
		pBleDHKeyExchangeAlgorithmAck->A2 = BSWAP_16(keyA2);
#endif
#if 0
		pBleDHKeyExchangeAlgorithmAck->A1 = (keyA1);
		pBleDHKeyExchangeAlgorithmAck->A2 = (keyA2);
#endif
		DEBUG_LOG_OUT("pBleDHKeyExchangeAlgorithmAck->A1:%X\n", pBleDHKeyExchangeAlgorithmAck->A1);
		DEBUG_LOG_OUT("pBleDHKeyExchangeAlgorithmAck->A2:%X\n", pBleDHKeyExchangeAlgorithmAck->A2);
		HAL_Comm_BleSendDirWithLen(eBleDHKeyExchangeAlgorithm, sizeof(TYPEs_BleDHKeyExchangeAlgorithmAck));
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleGyroscopeSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleGyroscopeSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleGyroscopeSet\n");
	storageSave.sysDat.speed = pBleGyroscopeSet->gyroscope;
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleGyroscopeSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleWakeSourceQuery
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleWakeSourceQuery(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleWakeSourceQuery\n");
	pBleWakeSourceQueryAck->ack = eBleAckSuccess;
	pBleWakeSourceQueryAck->wakeSource = WakeUpSource;
	HAL_Comm_BleSendDir(eBleWakeSourceQuery);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAESUnlockPwdSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
加密算法：①采用AES-128 bits/ECB/NoPadding 加密，秘钥通过以下方法生成
秘钥生成：
①.秘钥长度为16byte ,前8byte为秘钥随机数（0x3A），后8byte为前8byte按位取反后的结果，得到key1
②.得到key1与根秘钥（8byte）循环异或，得到key2，key2作为AES-128 bits/ECB/NoPadding 算法的秘钥
AES-128 bits/ECB/NoPadding 测试样例
   秘钥：key[16]={
		0x2b, 0x7e, 0x15, 0x16,
		0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88,
		0x09, 0xcf, 0x4f, 0x3c};
明文：plain_text[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,}
密文：ciper_text[16]={0x8d,0xf4,0xe9,0xaa,0xc5,0xc7,0x57,0x3a,0x27,0xd8,0xd0,0x55,0xd6,0xe4,0xd6,0x4b,}
****************************************************************************************/
void Menu_Comm_BleAESUnlockPwdSet(void)
{
	uint8_t aesKey[16] = {
		0,
	};
	TYPEs_StorageSysData Tmp;
	uint32_t i = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	if (-1 == Menu_AppRegChk())
	{
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		DEBUG_LOG_OUT("Menu_Comm_BleAESUnlockPwdSet\n");
		memcpy(aesKey, commDat.SecretKey, 8); 
		for (i = 0; i < 8; i++)
		{
			aesKey[8 + i] = ~aesKey[i];
		}
		DEBUG_LOG_OUT("aesKeysos key1\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		for (i = 0; i < 8; i++)
		{
			aesKey[i] ^= gRootKey[AES_ROOT_KEY][i];
			aesKey[8 + i] ^= gRootKey[AES_ROOT_KEY][i];
		}
		DEBUG_LOG_OUT("aesKeysos key2\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		aes_decrypt_ecb(AES_CYPHER_128,
						pBleAESUnlockPwdSet->pwd,
						16,
						aesKey); //解密
		DEBUG_LOG_OUT("Pwdsos\n");
		DEBUG_LOG_OUT_HEX(pBleAESUnlockPwdSet->pwd, sizeof(pBleAESUnlockPwdSet->pwd));
		memcpy((INT8U *)&storageSave.sysDat.pwd,
			   pBleAESUnlockPwdSet->pwd,
			   M_PASSWORD_MAX_LEN);
		storageSave.sysDat.pwdSetFlag |= 0x01;
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
		Menu_RecordLockLog(eLogTypeAddPwd, NULL, 0);   //保存事件记录
#if 0
		HAL_Storage_ReadSysData(&Tmp);
		DEBUG_LOG_OUT("Read Sysdat\n");
		DEBUG_LOG_OUT_HEX(Tmp.pwd, sizeof(Tmp.pwd));
#endif
		pBleAck->ack = eBleAckSuccess;
	}
	HAL_Comm_BleSendDir(eBleAESUnlockPwdSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAESUnlockAlarmPwdSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAESUnlockAlarmPwdSet(void)
{
	uint8_t aesKey[16] = {
		0,
	};
	uint32_t i = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	if (-1 == Menu_AppRegChk())
	{
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		DEBUG_LOG_OUT("Menu_Comm_BleAESUnlockAlarmPwdSet\n");
		memcpy(aesKey, commDat.SecretKey, 8);
        DEBUG_LOG_OUT("aesKeysos key\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		for (i = 0; i < 8; i++)
		{
			aesKey[8 + i] = ~aesKey[i];
		}
		DEBUG_LOG_OUT("aesKey key1\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		for (i = 0; i < 8; i++)
		{
			aesKey[i] ^= gRootKey[AES_ROOT_KEY][i];
			aesKey[8 + i] ^= gRootKey[AES_ROOT_KEY][i];
		}
		DEBUG_LOG_OUT("aesKey key2\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
        DEBUG_LOG_OUT("pBleAESUnlockAlarmPwdSet->pwd\n");
		DEBUG_LOG_OUT_HEX(pBleAESUnlockAlarmPwdSet->pwd, sizeof(pBleAESUnlockAlarmPwdSet->pwd));
		aes_decrypt_ecb(AES_CYPHER_128,
						pBleAESUnlockAlarmPwdSet->pwd,
						16,
						aesKey); //解密
		DEBUG_LOG_OUT("Pwd\n");
		DEBUG_LOG_OUT_HEX(pBleAESUnlockAlarmPwdSet->pwd, sizeof(pBleAESUnlockAlarmPwdSet->pwd));
		memcpy((INT8U *)&storageSave.sysDat.pwdSos,
			   pBleAESUnlockAlarmPwdSet->pwd,
			   M_PASSWORD_MAX_LEN);
		//需要对密码进行简单加密
		storageSave.sysDat.pwdSetFlag |= 0x01 << 1;
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
		Menu_RecordLockLog(eLogTypeAddPwd, NULL, 0);   //保存事件记录
		pBleAck->ack = eBleAckSuccess;
	}
	HAL_Comm_BleSendDir(eBleAESUnlockAlarmPwdSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAESTmpeSecretKeyGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAESTmpeSecretKeyGet(void)
{
	uint32_t i = 0;
	uint8_t tmp = 0;
	uint8_t aesKey[16] = {
		0,
	};
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAESTmpeSecretKeyGet\n");
	if (-1 == Menu_AppRegChk())
	{
        DEBUG_LOG_OUT("eBleAckFail\n");
		pBleAESTmpeSecretKeyGetAck->ack = eBleAckFail;
        HAL_Comm_BleSendDirWithLen(eBleAESTmpeSecretKeyGet,1);
	}
	else
	{

		Menu_RTCTimerGet(); //获取时间
		memset((uint8_t *)&storageSave.sysDat.tempPwd,
			   0,
			   sizeof(storageSave.sysDat.tempPwd));
		tmp = st_InputInfo.RTCTimer.tm_sec;
		for (i = 0; i < HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE; i++)
		{
			storageSave.sysDat.tempPwd[i][0] = ((tmp | i) + 5) % 10 + 0x30;
			storageSave.sysDat.tempPwd[i][1] = 0x39 - i;
			tmp = tmp + 0x03;
			storageSave.sysDat.tempPwd[i][2] = (st_InputInfo.RTCTimer.tm_min * i + tmp) % 10 + 0x30;
			tmp = tmp * i % 10;
			storageSave.sysDat.tempPwd[i][3] = tmp % 10 + 0x30;
			tmp = tmp + st_InputInfo.RTCTimer.tm_min;
			storageSave.sysDat.tempPwd[i][4] = ((tmp | i) + 7) % 10 + 0x30;
			tmp = tmp + i;
			storageSave.sysDat.tempPwd[i][5] = (tmp ^ i + 4) % 10 + 0x30;
			tmp = tmp + st_InputInfo.RTCTimer.tm_hour;
			storageSave.sysDat.tempPwd[i][6] = 0xff;
			storageSave.sysDat.tempPwd[i][7] = 0xff;
			memcpy(&pBleAESTmpeSecretKeyGetAck->pwd[i * HAL_STORAGE_PWD_LEN],
				   &storageSave.sysDat.tempPwd[i][0],
				   HAL_STORAGE_PWD_LEN);
		}											   //获取密码
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
		memcpy(aesKey, commDat.SecretKey, 8);
		for (i = 0; i < 8; i++)
		{
			aesKey[8 + i] = ~aesKey[i];
		}
		DEBUG_LOG_OUT("aesKey key1\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		for (i = 0; i < 8; i++)
		{
			aesKey[i] ^= gRootKey[AES_ROOT_KEY][i];
			aesKey[8 + i] ^= gRootKey[AES_ROOT_KEY][i];
		}
		DEBUG_LOG_OUT("aesKey key2\n");
		DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
		aes_encrypt_ecb(AES_CYPHER_128,
						pBleAESTmpeSecretKeyGetAck->pwd,
						64,
						aesKey); //加密
		pBleAESTmpeSecretKeyGetAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDirWithLen(eBleAESTmpeSecretKeyGet,sizeof(TYPEs_BleAESTmpeSecretKeyGetAck));
	}
	
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAESAddPhoneAccount
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAESAddPhoneAccount(void)
{
	uint8_t tmp[sizeof(TYPEs_BleAddPhoneAccount)];
	uint16_t i = 0;
	uint16_t j = 0;
	uint8_t aesKey[16] = {
		0,
	};
	uint8_t phoneTmp[3] = {
		0,
	};
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAESAddPhoneAccount\n");
	memcpy(aesKey, commDat.SecretKey, 8);
	for (i = 0; i < 8; i++)
	{
		aesKey[8 + i] = ~aesKey[i];
	}
	DEBUG_LOG_OUT("aesKey key1\n");
	DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
	for (i = 0; i < 8; i++)
	{
		aesKey[i] ^= gRootKey[AES_ROOT_KEY][i];
		aesKey[8 + i] ^= gRootKey[AES_ROOT_KEY][i];
	}
	DEBUG_LOG_OUT("aesKey key2\n");
	DEBUG_LOG_OUT_HEX(aesKey, sizeof(aesKey));
	aes_decrypt_ecb(AES_CYPHER_128,
					pBleAESUnlockPwdSet->pwd,
					64,
					aesKey); //解密
	memcpy(tmp,
		   pBleAddPhoneAccount->phoneID,
		   sizeof(tmp)); //获取手机号
	DEBUG_LOG_OUT(" pBleAddPhoneAccount->phoneID:\n");
	DEBUG_LOG_OUT_HEX(pBleAddPhoneAccount->phoneID, sizeof(pBleAddPhoneAccount->phoneID));
	st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
												  MEM_PHONE_ALL,
												  MSG_CARD_REG_MEM_ST,
												  MSG_CARD_REG_SIZE);
	DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
	if (0XFFFF == st_InputInfo.userID)
	{
		DEBUG_LOG_OUT("st_InputInfo.userID: ERROR!\n");
		pBleAck->ack = eBleAckFail;
	}
	else
	{
		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
		memcpy(storageSave.UserDat.UserID,
			   tmp,
			   sizeof(storageSave.UserDat.UserID));
		storageSave.sysDat.leavePhone--;
		storageSave.UserDat.pageID = st_InputInfo.userID;
		storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
		storageSave.UserDat.Lim = MSG_FACTMEM_LIM_PHONE;
		HAL_Storage_SaveOneUserReg(MSG_CARD_REG_MEM_ST,
								   MSG_CARD_REG_SIZE,
								   st_InputInfo.userID,
								   &storageSave.UserDat); //保存用户数据
		HAL_Storage_WriteSysData(&storageSave.sysDat);	  //写入系统参数
	}
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleAESAddPhoneAccount);
	memcpy(phoneTmp, &storageSave.UserDat.UserID[10], 3);
	phoneTmp[0] ^= storageSave.hardwareData.channelPwd[2];
	phoneTmp[1] ^= storageSave.hardwareData.channelPwd[3];
	phoneTmp[2] ^= storageSave.hardwareData.channelPwd[0]; //对ID进行简单加密
	Menu_RecordLockLog(eLogTypeAddAesPhone, phoneTmp, 3);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleQueryLockConfigTable
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleQueryLockConfigTable(void)
{
    #if 1
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
    #endif
	DEBUG_LOG_OUT("Menu_Comm_BleQueryLockConfigTable\n");
	DEBUG_LOG_OUT_HEX(((uint8_t *)&pBleQueryLockConfigTable->manufacturer), 1);
	memset((uint8_t *)pBleQueryLockConfigTableAck, 0, sizeof(TYPEs_BleQueryLockConfigTableAck)); //全部清0
	pBleQueryLockConfigTableAck->ack = eBleAckSuccess;											 //ACK成功
	memcpy((uint8_t *)pBleQueryLockConfigTableAck->model, "Q7FPro", sizeof("Q7FPro"));
	memcpy((uint8_t *)pBleQueryLockConfigTableAck->MAC, st_InputInfo.bleMac, sizeof(pBleQueryLockConfigTableAck->MAC)); //MAC
	Menu_GetchannelPwd(storageSave.hardwareData.channelPwd, sizeof(storageSave.hardwareData.channelPwd));				//获取信道密码
	memcpy(pBleQueryLockConfigTableAck->channelPwd,
		   storageSave.hardwareData.channelPwd,
		   sizeof(pBleEquipmentRegAck->channelPwd)); //信道密码
    if(eID2StatusSuccess == st_InputInfo.iD2Status)
    {
        pBleQueryLockConfigTableAck->openEncryptionType = eOpenEncryptionTypeID2;//ID2加密
    }
    else
    {
        pBleQueryLockConfigTableAck->openEncryptionType = eOpenEncryptionTypeDH;//DH加密
    }
	
	pBleQueryLockConfigTableAck->issuePwdAESAddPhone = eIssuePwdAESAddPhoneAES;
	pBleQueryLockConfigTableAck->regFingPram.xPlatform = 1;		  //X平台（不支持离线指纹）
	pBleQueryLockConfigTableAck->regFingPram.newprotocol = 1;	  //录指纹新协议
	pBleQueryLockConfigTableAck->wirelessModule = 0x00;			  //模块
	pBleQueryLockConfigTableAck->rootSecretKeySet = AES_ROOT_KEY; //AES下标
	pBleQueryLockConfigTableAck->unlockType.pwd = 1;			  //密码解锁
	pBleQueryLockConfigTableAck->unlockType.finger = 1;			  //指纹识别解锁
	pBleQueryLockConfigTableAck->unlockType.smartKey = 1;		  //
	pBleQueryLockConfigTableAck->unlockType.face = 1;			  //人脸识别解锁
	pBleQueryLockConfigTableAck->offlineFingerNum = 0;
	pBleQueryLockConfigTableAck->onlineFingerNum = 60;
	pBleQueryLockConfigTableAck->autoConfig.enOpenDirSet = 1;	   //开门方向调整（左右）
	pBleQueryLockConfigTableAck->autoConfig.enMotorTorqueSet = 1;  //电机扭力调节
	pBleQueryLockConfigTableAck->autoConfig.enAutoLockTimeSet = 1; //自动上锁时间调节
	pBleQueryLockConfigTableAck->autoConfig.enAutoLockAffirm = 1;
    if(0x01 == storageSave.hardwareData.dir) //左开
    {
        pBleQueryLockConfigTableAck->autoConfig.swOpenDirSet = 1;							  //开门方向调整（左右）0=右开
    }
    if(0x02 == storageSave.hardwareData.torque[0]) //扭力高
    {
        pBleQueryLockConfigTableAck->autoConfig.swMotorTorqueSet = 1;						  //低电机扭力
    }
	pBleQueryLockConfigTableAck->autoConfig.swAutoLockTime = storageSave.sysDat.autoLockTime /5;    //关闭自动上锁时间
	pBleQueryLockConfigTableAck->fingerRecordNum = 6;									  //录入指纹次数
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enAKeyProtection = 1;				  //一键布防
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enLockVolumeSet = 1;				  //音量调节
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enActiveDefense = storageSave.sysDat.irDef;    //主动防御（红外30秒检测）
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enOpenModeSet = 1;					  //开门模式可选
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enInfraredMonitoringDistanceSet = 1; //红外距离
	pBleQueryLockConfigTableAck->lockFunctionConfig1.swInfraredMonitoringDistanceSet = storageSave.sysDat.IrDistance; //红外距离配置
	pBleQueryLockConfigTableAck->lockFunctionConfig1.enEventLog = 1;					  //事件记录
	pBleQueryLockConfigTableAck->lockFunctionConfig1.swEventLog = 1;
    #ifdef USE_CATEYE
    #ifdef USE_CATEYE_SHANGTANG
	pBleQueryLockConfigTableAck->lockFunctionConfig2.shangTangCatSEye = 1; //商汤猫眼
    #endif
    #ifdef USE_CATEYE_AOBI
	pBleQueryLockConfigTableAck->lockFunctionConfig2.aoBiCatSEye = 1; //奥比猫眼
    #endif
    #endif 
    pBleQueryLockConfigTableAck->lockFunctionConfig2.R8InfraredMonitoringDistanceSet = 1;
	pBleQueryLockConfigTableAck->lockFunctionConfig2.NewRemote = 1;
    pBleQueryLockConfigTableAck->lockFunctionConfig2.McuGX = 1; //国芯单芯片主控
	pBleQueryLockConfigTableAck->volumeStatus =  storageSave.sysDat.voiceSw;	    //锁音量状态
	pBleQueryLockConfigTableAck->activeDefenseStatus = 0;						   //主动防御状态
	pBleQueryLockConfigTableAck->openModeStatus.openModePwd = 1;				   //密码解锁
	pBleQueryLockConfigTableAck->openModeStatus.openModeFinger = 1;				   //指纹识别解锁
	pBleQueryLockConfigTableAck->openModeStatus.openModeSmartKey = 1;			   //智能钥匙解锁（蓝牙钥匙/蓝牙手环）
	pBleQueryLockConfigTableAck->openModeStatus.openModeFace = 1;				   //人脸识别解锁
	pBleQueryLockConfigTableAck->infraredRangeStatus = eInfraredRangeStatusNearly; //红外距离状态
	pBleQueryLockConfigTableAck->CompositionOpen = 0x03;						   //组合开门
	HAL_Comm_BleSendDir(eBleQueryLockConfigTable);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAKeyProtection
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAKeyProtection(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAKeyProtection\n");
	DEBUG_LOG_OUT("pBleAKeyProtection->sw:%d\n", pBleAKeyProtection->sw);
    storageSave.sysDat.keyDef = pBleAKeyProtection->sw;
    HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleAKeyProtection);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleActiveDefense
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleActiveDefense(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleActiveDefense\n");
	DEBUG_LOG_OUT("pBleActiveDefense->time:%d\n", pBleActiveDefense->time);
    storageSave.sysDat.irDef = pBleActiveDefense->time;
    HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleActiveDefense);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleChannelPwdGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleChannelPwdGet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleChannelPwdGet\n");
	pBleChannelPwdGetAck->ack = eBleAckSuccess;
	memcpy((uint8_t *)&pBleChannelPwdGetAck->pwd,
		   storageSave.hardwareData.channelPwd,
		   sizeof(pBleChannelPwdGetAck->pwd));
	HAL_Comm_BleSendDir(eBleChannelPwdGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleNFCSwitchSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleNFCSwitchSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleNFCSwitchSet\n");
	//	pBleNFCSwitchSet->ack = eBleAckSuccess;
	//	pBleNFCSwitchSet->pwd
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleNFCSwitchSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDoorbellCapture
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDoorbellCapture(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDoorbellCapture\n");
	//    pBleDoorbellCapture->ack = eBleAckSuccess;
	//    pBleDoorbellCapture->pwd
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleDoorbellCapture);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFaceVersionGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFaceVersionGet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFaceVersionGet\n");
    Menu_Face_PowerOnStart(0); //人脸上电
    HAL_FaceId_GetVersionStart();
	fn_JumpMenu(enum_Menu_BleFaceVersionGet);
	//	pBleFaceVersionGetAck->ack = eBleAckSuccess;
	//	memcpy(pBleFaceVersionGetAck->version, "ST_SENSEEYE_A-V1.0.127", sizeof(pBleFaceVersionGetAck->version));
	//	//填写人脸版本
	//	HAL_Comm_BleSendDir(eBleFaceVersionGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleGetEventRecords
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleGetEventRecords(void)
{
	uint32_t i;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleGetEventRecords\n");
	if (0 == storageSave.sysDat.openLogSn)
	{
		memset((uint8_t *)&pBleGetEventRecords->log, 0, sizeof(pBleGetEventRecords->log));
		pBleAck->ack = eBleAckSuccess;
		pBleGetEventRecords->num = 0;
		HAL_Comm_BleSendDir(eBleGetEventRecords);
	}
	else
	{
		if (storageSave.sysDat.openLog >= MEM_LOCKLOG_ALL) //500条封顶
		{
			pBleGetEventRecords->ack = eBleAckSuccess;
			pBleGetEventRecords->num = BSWAP_16(MEM_LOCKLOG_ALL);
		}
		else
		{
			pBleGetEventRecords->ack = eBleAckSuccess;
			pBleGetEventRecords->num = BSWAP_16(storageSave.sysDat.openLogSn);
		}
		if (storageSave.sysDat.openLog >= MEM_LOCKLOG_ALL) //500条封顶
		{
			for (i = 0; i < MEM_LOCKLOG_ALL; i++)
			{
				if (storageSave.sysDat.openLogSn > i) //读取前面部分的数据
				{
					HAL_Storage_ReadLockLog(pBleGetEventRecords->log,
											storageSave.sysDat.openLogSn - i); //读取事件记录
				}
				else //读取后面部分的数据
				{
					HAL_Storage_ReadLockLog(pBleGetEventRecords->log,
											MEM_LOCKLOG_ALL - (i - storageSave.sysDat.openLogSn)); //读取事件记录
				}
				HAL_Comm_BleSendDir(eBleGetEventRecords);
				DELAY_MS(30);
			}
		}
		else
		{
			for (i = 0; i < storageSave.sysDat.openLogSn; i++)
			{
				HAL_Storage_ReadLockLog(pBleGetEventRecords->log, i); //读取事件记录
				HAL_Comm_BleSendDir(eBleGetEventRecords);
				DELAY_MS(30);
			}
		}
#if 1
		memset((uint8_t *)&pBleGetEventRecords->log, 0, sizeof(pBleGetEventRecords->log));
		memcpy((uint8_t *)&pBleGetEventRecords->log, "RTRTC", sizeof("RTRTC") - 1);
		pBleGetEventRecords->log[8] = st_InputInfo.RTCTimer.tm_year; //注意这里用
		pBleGetEventRecords->log[9] = st_InputInfo.RTCTimer.tm_mon;
		pBleGetEventRecords->log[10] = st_InputInfo.RTCTimer.tm_yday;
		pBleGetEventRecords->log[11] = st_InputInfo.RTCTimer.tm_hour;
		pBleGetEventRecords->log[12] = st_InputInfo.RTCTimer.tm_min;
		pBleGetEventRecords->log[13] = st_InputInfo.RTCTimer.tm_sec;
		HAL_Comm_BleSendDir(eBleGetEventRecords);
		DELAY_MS(30);
#endif
#if 1
		memset((uint8_t *)&pBleGetEventRecords->log, 0, sizeof(pBleGetEventRecords->log));
		pBleGetEventRecords->num = 0;
		pBleGetEventRecords->ack = eBleAckSuccess;
		HAL_Comm_BleSendDir(eBleGetEventRecords);
#endif
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFaceAttributeModify
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFaceAttributeModify(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFaceAttributeModify\n");
	DEBUG_LOG_OUT("PageID:%x\n", BSWAP_16(pBleFaceAttributeModify->pageId));
	DEBUG_LOG_OUT("aging:%X\n", *((uint8_t *)(&pBleFaceAttributeModify->aging)));
	DEBUG_LOG_OUT("startTm:");
	DEBUG_LOG_OUT_HEX(((uint8_t *)(&pBleFaceAttributeModify->startTm)), sizeof(pBleFaceAttributeModify->startTm));
	DEBUG_LOG_OUT("agingTm:");
	DEBUG_LOG_OUT_HEX(((uint8_t *)(&pBleFaceAttributeModify->agingTm)), sizeof(pBleFaceAttributeModify->agingTm));
	DEBUG_LOG_OUT("week:%X\n", *((uint8_t *)(&pBleFaceAttributeModify->week)));
	storageSave.UserDat.pageID = BSWAP_16(pBleFaceAttributeModify->pageId);	 //pBleFingerAttributeModify
	memset((uint8_t *)&storageSave.UserDat, 0, sizeof(storageSave.UserDat)); //清空指令包
	HAL_Storage_GetOneUserReg(MSG_FACE_REG_MEM_ST,
							  MSG_FACE_REG_SIZE,
							  storageSave.UserDat.pageID,
							  &storageSave.UserDat); //读取参数
	storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_OFF;
	if (0xf0 == (*((uint8_t *)(&pBleFaceAttributeModify->aging))))
	{
		DEBUG_LOG_OUT("storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.TimeVaild.fig = MENU_FUNCTION_ON;
	}
	memcpy((uint8_t *)&storageSave.UserDat.TimeVaild.str,
		   (uint8_t *)(&pBleFaceAttributeModify->startTm),
		   sizeof(storageSave.UserDat.TimeVaild.str)); //时效开始
	memcpy((uint8_t *)&storageSave.UserDat.TimeVaild.stp,
		   (uint8_t *)(&pBleFaceAttributeModify->agingTm),
		   sizeof(storageSave.UserDat.TimeVaild.stp)); //时效结束
#if 0
	if ((0x99 == storageSave.UserDat.TimeVaild.stp.year) &&
		(0x12 == storageSave.UserDat.TimeVaild.stp.mon) &&
		(0x31 == storageSave.UserDat.TimeVaild.stp.day) &&
		(0x23 == storageSave.UserDat.TimeVaild.stp.hour) &&
		(0x59 == storageSave.UserDat.TimeVaild.stp.min) &&
		(0x59 == storageSave.UserDat.TimeVaild.stp.sec))
	{
		storageSave.UserDat.TimeVaild.fig = eAgingStatusOff; //直接关闭时效
	}
#endif
	storageSave.UserDat.TimeVaild.wday = *((uint8_t *)(&pBleFaceAttributeModify->week)); //获取周
	storageSave.UserDat.kindred = MENU_FUNCTION_OFF;
	storageSave.UserDat.sos = MENU_FUNCTION_OFF;
	if (0xf == pBleFaceAttributeModify->kindleAndSos.kindle)
	{
		DEBUG_LOG_OUT("storageSave.UserDat.kindred = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.kindred = MENU_FUNCTION_ON;
	}
	if (0xf == pBleFaceAttributeModify->kindleAndSos.SOS)
	{
		DEBUG_LOG_OUT("storageSave.UserDat.sos = MENU_FUNCTION_ON;\n");
		storageSave.UserDat.sos = MENU_FUNCTION_ON;
	}
	HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
							   MSG_FACE_REG_SIZE,
							   storageSave.UserDat.pageID,
							   &storageSave.UserDat); //保存信息
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleFaceAttributeModify);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleCatEyeSN
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleCatEyeSN(void)
{
    #if 0
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
    #endif
	DEBUG_LOG_OUT("Menu_Comm_BleCatEyeSN\n");
	HAL_FaceId_GetDeviceNameStart();
    faceIdCtrl.enableNetworkBack = 1;
	Menu_Face_PowerOnStart(0); //给人脸模块上电
	fn_JumpMenu(enum_Menu_BleCatEyeSN);
    DEBUG_LOG_OUT("msgInitData.enableNetwork:%x\n",msgInitData.enableNetwork);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleShangTangCatEyeSingleDouble
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleShangTangCatEyeSingleDouble(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleShangTangCatEyeSingleDouble\n");
	DEBUG_LOG_OUT("pBleShangTangCatEyeSingleDouble->sw:%d\n", pBleShangTangCatEyeSingleDouble->sw);
	if ((0 == pBleShangTangCatEyeSingleDouble->sw) ||
		(1 == pBleShangTangCatEyeSingleDouble->sw))
	{
		pBleAck->ack = eBleAckSuccess;
		storageSave.sysDat.ShangTangCatEyeSingleDouble = pBleShangTangCatEyeSingleDouble->sw;
		HAL_Storage_WriteSysData(&storageSave.sysDat); //保存系统参数
	}
	else
	{
		pBleAck->ack = eBleAckFail;
	}
	HAL_Comm_BleSendDir(eBleShangTangCatEyeSingleDouble);
	if (0 == pBleShangTangCatEyeSingleDouble->sw)
	{
		wifiSendParm.sendSw = eWifiSendSwEnable; //复位并重启
		Menu_Face_PowerOffStart();				 //先给模块断电
		fn_JumpMenu(enum_Menu_BleShangTangCatEyeSingleDouble);
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BleShangTangCatEyeSingleDouble);");
	}
	else
	{
		fn_JumpMenu(enum_Menu_APPMode);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAutoLockConfig
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAutoLockConfig(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAutoLockConfig\n");
	if ((1 == pBleAutoLockConfig->lockType) ||
		(2 == pBleAutoLockConfig->lockType))
	{
		storageSave.hardwareData.lockBodyType = pBleAutoLockConfig->lockType;
		HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
		pBleAck->ack = eBleAckSuccess;
	}
	else
	{
		pBleAck->ack = eBleAckFail;
	}
	HAL_Comm_BleSendDirWithLen(eBleAutoLockConfig, sizeof(TYPEs_BleAck));
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAutoLockKeyConfig
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         
长度:1byte 
数据包格式: 0 关闭  1开启
 默认关闭，开启后门外锁门需要按#键确认上锁，避免键盘误触。
****************************************************************************************/
void Menu_Comm_BleAutoLockKeyConfig(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAutoLockKeyConfig\n");
	storageSave.sysDat.bellDef = pBleAutoLockKeyConfig->sw;
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDirWithLen(eBleAutoLockKeyConfig, sizeof(TYPEs_BleAck));
}
/***************************************************************************************
**函数名:       Menu_Comm_BleToolAddModel
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleToolAddModel(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleToolAddModel\n");
#ifndef LOCK_OPEN_PARM_NO_CLEAR
	memcpy(storageSave.sysDat.model, pBleToolAddModel->model, sizeof(storageSave.sysDat.model));
	HAL_Storage_WriteSysData(&storageSave.sysDat);
#else
	memcpy(storageSave.hardwareData.model, pBleToolAddModel->model, sizeof(storageSave.hardwareData.model));
	HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
#endif
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleToolAddModel);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleToolTestRWLockModel
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleToolTestRWLockModel(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleToolTestRWLockModel\n");
	HAL_Storage_ReadSysData(&storageSave.sysDat); //读取参数
	pBleToolTestRWLockModelAck->ack = eBleAckSuccess;
#ifndef LOCK_OPEN_PARM_NO_CLEAR
	memcpy(pBleToolTestRWLockModelAck->model,
		   storageSave.sysDat.model,
		   sizeof(pBleToolTestRWLockModelAck->model));
#else
	memcpy(pBleToolTestRWLockModelAck->model,
		   storageSave.hardwareData.model,
		   sizeof(pBleToolTestRWLockModelAck->model));
#endif
	HAL_Comm_BleSendDir(eBleToolTestRWLockModel);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleToolTestFail
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleToolTestFail(void)
{
	uint32_t i;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleToolTestFail\n");
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleToolTestFail);
	for (i = 0; i < 18; i++)
	{
		HAL_VoicePlayDir(eVoiceDirDingDongDingDong);
		DELAY_MS(500);
		HAL_KeyLight_DirDispaly(MENU_KEY_BACK_LIGHT_ALL_OFF);
		DELAY_MS(500);
		HAL_KeyLight_DirDispaly(MENU_KEY_BACK_LIGHT_ALL_ON);
	}
	DEBUG_LOG_OUT("HAL_Power_CPURst\n");
	HAL_Power_CPURst(); //重启设备
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFlashWriteTest
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFlashWriteTest(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFlashWriteTest\n");
	memcpy((uint8_t *)&storageSave.sysDat.eeTest,
		   pBleFlashWriteTest->test,
		   sizeof(storageSave.sysDat.eeTest));
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleFlashWriteTest);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleLedOledTest
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLedOledTest(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLedOledTest\n");
	HAL_KeyLightOnce(HAL_KEY_BACK_LIGHT_ALL_ON); //点亮全部键盘
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleLedOledTest);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFlashRWComp
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFlashRWComp(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFlashRWComp\n");
	pBleFlashRWCompAck->ack = eBleAckSuccess;
	HAL_Storage_ReadSysData(&storageSave.sysDat); //读取参数
	memcpy(pBleFlashRWCompAck->test,
		   (uint8_t *)&storageSave.sysDat.eeTest,
		   sizeof(pBleFlashRWCompAck->test));
	HAL_Comm_BleSendDir(eBleFlashRWComp);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleVoltageTest
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleVoltageTest(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleVoltageTest\n");
	pBleVoltageTestAck->ack = eBleAckSuccess;
	pBleVoltageTestAck->Value = st_InputInfo.upBatBattery;
	BSWAP_SAVE_16(pBleVoltageTestAck->Value);
	HAL_Comm_BleSendDir(eBleVoltageTest);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleLockTimeGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockTimeGet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLockTimeGet\n");
	pBleLockTimeGetAck->ack = eBleAckSuccess;
	Menu_RTCTimerGet();
	pBleLockTimeGetAck->time[0] = (uint8_t)(st_InputInfo.RTCTimer.tm_year >> 16);
	pBleLockTimeGetAck->time[1] = (uint8_t)(st_InputInfo.RTCTimer.tm_year);
	pBleLockTimeGetAck->time[2] = (uint8_t)(st_InputInfo.RTCTimer.tm_mon);
	pBleLockTimeGetAck->time[3] = (uint8_t)(st_InputInfo.RTCTimer.tm_yday);
	pBleLockTimeGetAck->time[4] = (uint8_t)(st_InputInfo.RTCTimer.tm_hour);
	pBleLockTimeGetAck->time[5] = (uint8_t)(st_InputInfo.RTCTimer.tm_min);
	pBleLockTimeGetAck->time[5] = (uint8_t)(st_InputInfo.RTCTimer.tm_sec);
	HAL_Comm_BleSendDir(eBleLockTimeGet);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleWifiToolTest
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleWifiToolTest(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleWifiToolTest\n");
    #ifdef USE_WIFI_MODLE
    pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDirWithLen(eBleWifiToolTest, sizeof(TYPEs_BleAck));
    memset((uint8_t *)pWifiCmdUpdataTest,
		   0,
		   sizeof(TYPEs_WifiCmdUpdataTest));
    memcpy((uint8_t *)(pWifiCmdUpdataTest->wifiTestData),
            pBleRxData,
            sizeof(pWifiCmdUpdataTest->wifiTestData));
    memcpy(pWifiCmdUpdataTest->MAC,
		   storageSave.hardwareData.mac,
		   sizeof(pWifiCmdUpdataTest->MAC)); //MAC
    
    wifiSendParm.sendLen = Comm_WifiCmdSendWithLen( eWifiCmdUnlockRealUpdata,
                                                    eWifiProtocolTypeCmdNotAck,
                                                    sizeof(TYPEs_WifiCmdUpdataTest)); //打包好数据
    wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
    #endif
    
}
/***************************************************************************************
**函数名:       Menu_Comm_BleSecondBatteryVoltageTest
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleSecondBatteryVoltageTest(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleSecondBatteryVoltageTest\n");
	pBleSecondBatteryVoltageTestAck->ack = eBleAckSuccess;
	pBleSecondBatteryVoltageTestAck->Value = st_InputInfo.downBatBattery;
	BSWAP_SAVE_16(pBleSecondBatteryVoltageTestAck->Value);
	HAL_Comm_BleSendDir(eBleSecondBatteryVoltageTest);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleLockMacGet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleLockMacGet(void)
{
	uint32_t i = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleLockMacGet\n");
	for (i = 0; i < sizeof(pBleLockMacGet->num); i++)
	{
		pBleLockMacGet->num[i] = pBleLockMacGet->num[i] - '0';
	}
	HAL_KeyLightUniteData(pBleLockMacGet->num, sizeof(pBleLockMacGet->num));
	pBleLockMacGetAck->BleMAC[0] = 0x06;
	memcpy(&pBleLockMacGetAck->BleMAC[1],
		   storageSave.hardwareData.mac,
		   sizeof(pBleLockMacGetAck->BleMAC) - 1);
	pBleLockMacGetAck->WifiMAC[0] = 0;
	pBleLockMacGetAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleLockMacGet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAliyunSecretKeySetParm
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAliyunSecretKeySetParm(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAliyunSecretKeySetParm\n");
	memset((uint8_t *)&msgLinkkeyData, 0, sizeof(TYPEs_MsgLinkkeyData));
#if 0
	memcpy( (uint8_t *)&msgLinkkeyData.deviceName,
			"a1V4aqlPM9y",
			sizeof(msgLinkkeyData.deviceName));
	memcpy( (uint8_t *)&msgLinkkeyData.deviceSecret,
			"czgxNbycQIMyapWX",
			sizeof(msgLinkkeyData.deviceSecret));
	memcpy( (uint8_t *)&msgLinkkeyData.ProductKey,
			"WYCtXrtsk7owU9tNRvCG",
			sizeof(msgLinkkeyData.ProductKey));
	memcpy( (uint8_t *)&msgLinkkeyData.productSecret,
			"XiosIUhGGMOYjP2kEExe3FcmZAdaTuAb",
			sizeof(msgLinkkeyData.productSecret));
#endif
#if 0 
	memcpy( (uint8_t *)&msgLinkkeyData.deviceName,
			"a1V4aqlPM9y",
			sizeof(msgLinkkeyData.deviceName));
	memcpy( (uint8_t *)&msgLinkkeyData.deviceSecret,
			"czgxNbycQIMyapWX",
			sizeof(msgLinkkeyData.deviceSecret));
	memcpy( (uint8_t *)&msgLinkkeyData.ProductKey,
			"S1xJdE8CE1dJBh4TgLAN",
			sizeof(msgLinkkeyData.ProductKey));
	memcpy( (uint8_t *)&msgLinkkeyData.productSecret,
			"QJAKYpGTSZcNFG9dLHQ2QqJ62UqRmHmX",
			sizeof(msgLinkkeyData.productSecret));
#endif
#if 0
		
	memcpy( (uint8_t *)&msgLinkkeyData.ProductKey,
			"a1277Y8Gyuc",
			sizeof("a1277Y8Gyuc"));
	memcpy( (uint8_t *)&msgLinkkeyData.productSecret,
			"Y5SkdOa1JjzcKrtj",
			sizeof("Y5SkdOa1JjzcKrtj"));
	memcpy( (uint8_t *)&msgLinkkeyData.deviceName,
			"MtmhZzKQQWRoVDEQdQfz",
			sizeof("MtmhZzKQQWRoVDEQdQfz"));
	memcpy( (uint8_t *)&msgLinkkeyData.deviceSecret,
			"8806f8c86edd0f466245eeb25df8f506",
			sizeof("8806f8c86edd0f466245eeb25df8f506"));
#endif
#if 0
	memcpy( (uint8_t *)&msgLinkkeyData.deviceName,
			"a1277Y8Gyuc",
			sizeof(msgLinkkeyData.deviceName));
	memcpy( (uint8_t *)&msgLinkkeyData.deviceSecret,
			"Y5SkdOa1JjzcKrtj",
			sizeof(msgLinkkeyData.deviceSecret));
	memcpy( (uint8_t *)&msgLinkkeyData.ProductKey,
			"n2OQVNLCAQNErGFqkY3c",
			sizeof(msgLinkkeyData.ProductKey));
	memcpy( (uint8_t *)&msgLinkkeyData.productSecret,
			"17fae50c01ed17179a8c452274ab1bb1",
			sizeof(msgLinkkeyData.productSecret));
#endif
#if 1

	memcpy((uint8_t *)&msgLinkkeyData.ProductKey,
		   (uint8_t *)&pBleAliyunSecretKeySetParm->productKey,
		   sizeof(msgLinkkeyData.ProductKey));
	memcpy((uint8_t *)&msgLinkkeyData.productSecret,
		   (uint8_t *)&pBleAliyunSecretKeySetParm->productSecret,
		   sizeof(msgLinkkeyData.productSecret));
	memcpy((uint8_t *)&msgLinkkeyData.deviceName,
		   (uint8_t *)&pBleAliyunSecretKeySetParm->deviceName,
		   sizeof(msgLinkkeyData.deviceName));
	memcpy((uint8_t *)&msgLinkkeyData.deviceSecret,
		   (uint8_t *)&pBleAliyunSecretKeySetParm->deviceSecre,
		   sizeof(msgLinkkeyData.deviceSecret));
#endif
	Menu_Face_PowerOnStart(1); //不管前面的状态，强制重启
    faceIdCtrl.enableNetworkBack = 1;
	HAL_FaceId_SetLinkKeyStart();
	fn_JumpMenu(enum_Menu_BleAliyunSecretKeySetParm);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleR8CatEyeOTA
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleR8CatEyeOTA(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleR8CatEyeOTA\n");
	pBleR8CatEyeOTAAck->ack = eFaceOTAAckSuccess;
	HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
	HAL_FaceId_CmdDealStart(eFaceIdCmdStartOTA, 0);
	Menu_Face_PowerOnStart(0);			   //给人脸模块上电
	fn_JumpMenu(enum_Menu_BleR8CatEyeOTA); //进入OTA升级
}
/***************************************************************************************
**函数名:       Menu_Comm_BleModifFaceSafetyLevel
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleModifFaceSafetyLevel(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleModifFaceSafetyLevel\n");
	msgSetThresholdLevel.recognition = pBleModifFaceSafetyLevel->recognition;
	msgSetThresholdLevel.biopsy = pBleModifFaceSafetyLevel->biopsy;
	HAL_FaceId_CmdDealStart(eFaceIdCmdStartOTA, 0);
	Menu_Face_PowerOnStart(0); //给人脸模块上电
	fn_JumpMenu(enum_Menu_BleModifFaceSafetyLevel);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleWifiSetParm1
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleWifiSetParm1(void)
{
    #if 0
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
    #endif
    #if 0
    memset(pBleWifiSetParm1->SSID,0,sizeof(pBleWifiSetParm1->SSID));
    memset(pBleWifiSetParm1->wifiKey,0,sizeof(pBleWifiSetParm1->wifiKey));
    memcpy(pBleWifiSetParm1->SSID,"666",sizeof("666"));
    memcpy(pBleWifiSetParm1->wifiKey,"12345678",sizeof("12345678"));
    #endif
    #if 0
    memset(pBleWifiSetParm1->SSID,0,sizeof(pBleWifiSetParm1->SSID));
    memset(pBleWifiSetParm1->wifiKey,0,sizeof(pBleWifiSetParm1->wifiKey));
    memcpy(pBleWifiSetParm1->SSID,"Redmi 10X",sizeof("Redmi 10X"));
    memcpy(pBleWifiSetParm1->wifiKey,"hx12345678",sizeof("hx12345678"));
    #endif
	DEBUG_LOG_OUT("Menu_Comm_BleWifiSetParm1\n");
	DEBUG_LOG_OUT("SSID:");
	DEBUG_LOG_OUT_HEX(pBleWifiSetParm1->SSID, sizeof(pBleWifiSetParm1->SSID));
	DEBUG_LOG_OUT("wifiKey:");
	DEBUG_LOG_OUT_HEX(pBleWifiSetParm1->wifiKey, sizeof(pBleWifiSetParm1->wifiKey));
    memcpy((uint8_t *)(&msgConfigWifiData.ssid),
		   pBleWifiSetParm1->SSID,
		   sizeof(msgConfigWifiData.ssid));
	memcpy((uint8_t *)(&msgConfigWifiData.psk),
		   pBleWifiSetParm1->wifiKey,
		   sizeof(msgConfigWifiData.psk));
	//检查电压低
#ifdef USE_FACE_WIFI   
#if 1
	memcpy((uint8_t *)(&msgConfigWifiData.ssid),
		   pBleWifiSetParm1->SSID,
		   sizeof(msgConfigWifiData.ssid));
	memcpy((uint8_t *)(&msgConfigWifiData.psk),
		   pBleWifiSetParm1->wifiKey,
		   sizeof(msgConfigWifiData.psk));
#endif
	DEBUG_LOG_OUT("msgConfigWifiData SSID:");
	DEBUG_LOG_OUT_HEX(msgConfigWifiData.ssid, sizeof(msgConfigWifiData.ssid));
	DEBUG_LOG_OUT("msgConfigWifiData wifiKey:");
	DEBUG_LOG_OUT_HEX(msgConfigWifiData.psk, sizeof(msgConfigWifiData.psk));
	
    #ifdef USE_FACE_SHANGTANG //使用商汤人脸
    faceIdCtrl.enableNetworkBack = 1; //商汤人脸，这里强制
    #endif

    #ifdef USE_FACE_AOBI //使用奥比人脸
    faceIdCtrl.enableNetworkBack = 1;
    #endif    
    Menu_Face_PowerOnStart(0); //给人脸模块上电
	HAL_FaceId_ConfigWifiDataStart();
#endif    
#ifdef USE_WIFI_MODLE
    memcpy((uint8_t *)(&pWifiConfig->ssid),
		   pBleWifiSetParm1->SSID,
		   sizeof(pWifiConfig->ssid));
	memcpy((uint8_t *)(&pWifiConfig->psk),
		   pBleWifiSetParm1->wifiKey,
		   sizeof(pWifiConfig->psk));
    wifiSendParm.wifiConnectStatus = eWifiConnectStatusReady;
#endif
	fn_JumpMenu(enum_Menu_BleConfigWifiData);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleUnlockUploadWifiSW
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleUnlockUploadWifiSW(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleUnlockUploadWifiSW\n");
    #if 0
	storageSave.sysDat.wifiOpenUpdataSw = pBleUnlockUploadWifiSW->sw;
    #endif
    if(0 == pBleUnlockUploadWifiSW->sw)
	{
        DEBUG_LOG_OUT("MSG_WIFI_LOGUP_ON_CONFIG\n");
		storageSave.sysDat.wifiOpenUpdataSw = MSG_WIFI_LOGUP_ON_CONFIG; 
	}
	else
	{
        DEBUG_LOG_OUT("MSG_WIFI_LOGUP_MAIN_SW_OFF_CONFIG\n");
		storageSave.sysDat.wifiOpenUpdataSw = MSG_WIFI_LOGUP_OFF_CONFIG;
	}
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleUnlockUploadWifiSW);
}

/***************************************************************************************
**函数名:       Menu_Comm_BleFaceReg
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFaceReg(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFaceReg\n");
	if (0 != storageSave.sysDat.leaveNumFaceApp) //检查人脸数量
	{
		DEBUG_LOG_OUT("Menu_Comm_BleFaceReg fn_JumpMenu(enum_Menu_BleAddFaceStep1);\n");
		#if 0
		Menu_Face_PowerOnStart(0);
		#else
		Menu_Face_PowerOnStart(1);
		#endif
        msgEnrollData.admin = 0; 
		HAL_FaceId_AddUserStart(); //开始添加
        fn_JumpMenu(enum_Menu_BleAddFaceStep1);
		pBleAddFaceAck->ack = eBleAddFaceAckIng;
		pBleAddFaceAck->add.addIng.cnt = eBleAddFaceDirEnter;
		HAL_Comm_BleSendDir(eBleFaceReg);
	}
	else //错误直接返回错误
	{
		DEBUG_LOG_OUT("Menu_Comm_BleFaceReg error\n");
		pBleAddFaceAck->ack = eBleAddFaceAckFail;
		HAL_Comm_BleSendDir(eBleFaceReg);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFaceRegConfirm
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFaceRegConfirm(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFaceRegConfirm\n");
	if (eBleFaceRegResultSuccess == pBleFaceRegConfirm->result)
	{
		fn_JumpMenu(enum_Menu_BleAddFaceUserOk);
	}
	else
	{
		Menu_Face_PowerOnStart(0);
		HAL_FaceId_DelOneUserStart(st_InputInfo.userID);
		HAL_Storage_DelOneUserReg(st_InputInfo.userID,
								  MSG_FACE_REG_MEM_ST,
								  MSG_FACE_REG_SIZE); //删除本地ID
		st_InputInfo.userIdTmp = BSWAP_16(st_InputInfo.userID);
		Menu_RecordLockLog(eLogTypeDelFace,
						   (uint8_t *)&(st_InputInfo.userIdTmp),
						   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
		fn_JumpMenu(enum_Menu_BleAddFaceUserFail);
	}
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleFaceRegConfirm);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleMotorTorqueSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleMotorTorqueSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleMotorTorqueSet\n");
	DEBUG_LOG_OUT("pBleMotorTorqueSet->torque:%d\n", pBleMotorTorqueSet->torque);
	switch (pBleMotorTorqueSet->torque)
	{
	case eBleMotorTorqueHigh:
#ifndef LOCK_OPEN_PARM_NO_CLEAR
		storageSave.sysDat.torque[0] = MOTOR_TORQUE_HIGH;
#else
		storageSave.hardwareData.torque[0] = MOTOR_TORQUE_HIGH;
#endif

		break;
	default:
#ifndef LOCK_OPEN_PARM_NO_CLEAR
		storageSave.sysDat.torque[0] = MOTOR_TORQUE_LOW;
#else
		storageSave.hardwareData.torque[0] = MOTOR_TORQUE_LOW;
#endif
		break;
	}
#ifndef LOCK_OPEN_PARM_NO_CLEAR
	HAL_Storage_WriteSysData(&storageSave.sysDat);
#else
	HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
#endif
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleMotorTorqueSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleMotorDirSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleMotorDirSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleMotorDirSet\n");
	DEBUG_LOG_OUT("pBleMotorDirSet->dir:%d\n", pBleMotorDirSet->dir);
#ifndef LOCK_OPEN_PARM_NO_CLEAR
	storageSave.sysDat.dir = pBleMotorDirSet->dir;
	HAL_Storage_WriteSysData(&storageSave.sysDat);
#else
	storageSave.hardwareData.dir = pBleMotorDirSet->dir;
	HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
#endif
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleMotorDirSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleAutomaticLockIntervalSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleAutomaticLockIntervalSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleAutomaticLockIntervalSet\n");
	DEBUG_LOG_OUT("pBleAutomaticLockIntervalSet->closeTime:%d\n", pBleAutomaticLockIntervalSet->closeTime);
	storageSave.sysDat.autoLockTime = pBleAutomaticLockIntervalSet->closeTime;
	HAL_Storage_WriteSysData(&storageSave.sysDat);
	pBleAck->ack = eBleAckSuccess;
	HAL_Comm_BleSendDir(eBleAutomaticLockIntervalSet);
}
/***************************************************************************************
**函数名:       Menu_Comm_BleFaceDelId
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleFaceDelId(void)
{
	int16_t tmp = 0;
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleFaceDelId\n");
	BSWAP_SAVE_16(pBleFaceDelId->id);
	storageSave.UserDat.pageID = pBleFaceDelId->id;
	tmp = HAL_Storage_MatchId(0,
							  MEM_FACE_ALL,
							  MSG_FACE_REG_MEM_ST,
							  MSG_FACE_REG_SIZE,
							  &storageSave.UserDat,
							  eMatchTypeUserID);
	if (-1 != tmp)
	{
		HAL_Storage_DelOneUserReg(tmp,
								  MSG_FACE_REG_MEM_ST,
								  MSG_FACE_REG_SIZE); //删除ID
		storageSave.sysDat.leaveNumFaceApp++;		  //更新数量
		if (0 != storageSave.sysDat.faceNumApp)
		{
			storageSave.sysDat.faceNumApp--;
		}
		HAL_Storage_WriteSysData(&storageSave.sysDat);		   //写入系统参数
		HAL_FaceId_DelOneUserStart(storageSave.UserDat.group); //发送删除命令
		Menu_Face_PowerOnStart(0);							   //给人脸上电
		fn_JumpMenu(enum_Menu_BleFaceDelId);
	}
	else
	{
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDir(eBleFaceDelId);
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleInfraredMonitoringDistanceSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleInfraredMonitoringDistanceSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleInfraredMonitoringDistanceSet\n");
	BSWAP_SAVE_16(pBleInfraredMonitoringDistanceSet->distance);
	DEBUG_LOG_OUT("pBleInfraredMonitoringDistanceSet->distance:%X\n",pBleInfraredMonitoringDistanceSet->distance);
	if (pBleInfraredMonitoringDistanceSet->distance < 3)
	{
		#if 0
		
		if (2 != pBleInfraredMonitoringDistanceSet->distance)
		{
			DEBUG_LOG_OUT("distance != 2 \n");
			HAL_IR_Init(); //配置串口
			storageSave.sysDat.IrDistance = pBleInfraredMonitoringDistanceSet->distance;
			HAL_Storage_WriteSysData(&storageSave.sysDat);
			HAL_IR_SetDistanceStart(storageSave.sysDat.IrDistance);
			fn_JumpMenu(enum_Menu_BleInfraredMonitoringDistanceSet);
		}
		else
		{
			DEBUG_LOG_OUT("distance ==2 \n");
			pBleAck->ack = eBleAckSuccess;
			HAL_Comm_BleSendDirWithLen(eBleInfraredMonitoringDistanceSet, sizeof(TYPEs_BleAck));
		}
		#endif
		
		#if 1
		storageSave.sysDat.IrDistance = pBleInfraredMonitoringDistanceSet->distance;
		HAL_Storage_WriteSysData(&storageSave.sysDat);
		if (2 != pBleInfraredMonitoringDistanceSet->distance)
		{
			DEBUG_LOG_OUT("distance != 2 \n");
			HAL_IR_Init(); //配置串口
			HAL_IR_SetDistanceStart(storageSave.sysDat.IrDistance);
			fn_JumpMenu(enum_Menu_BleInfraredMonitoringDistanceSet);
		}
		else
		{
			DEBUG_LOG_OUT("distance ==2 \n");
			pBleAck->ack = eBleAckSuccess;
			HAL_Comm_BleSendDirWithLen(eBleInfraredMonitoringDistanceSet, sizeof(TYPEs_BleAck));
		}
		#endif
	}
	else
	{
		DEBUG_LOG_OUT("distance error\n");
		pBleAck->ack = eBleAckFail;
		HAL_Comm_BleSendDirWithLen(eBleInfraredMonitoringDistanceSet, sizeof(TYPEs_BleAck));
	}
}
/***************************************************************************************
**函数名:       Menu_Comm_BleDoorOpeningModeSet
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleDoorOpeningModeSet(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		return;
	}
	DEBUG_LOG_OUT("Menu_Comm_BleDoorOpeningModeSet\n");
	pBleAck->ack = eBleAckSuccess;
	if (0x05 == pBleDoorOpeningModeSet->mode) //或模式
	{
		storageSave.sysDat.openLockMode = pBleDoorOpeningModeSet->unlockType;
		DEBUG_LOG_OUT("storageSave.sysDat.openLockMode 0x05：%X", storageSave.sysDat.openLockMode);
		storageSave.sysDat.openLockMode &= ~0x80;
		DEBUG_LOG_OUT("storageSave.sysDat.openLockMode 0x05：%X", storageSave.sysDat.openLockMode);
		HAL_Storage_WriteSysData(&storageSave.sysDat);
	}
	else if (0x55 == pBleDoorOpeningModeSet->mode) //与模式
	{
		storageSave.sysDat.openLockMode = pBleDoorOpeningModeSet->unlockType;
		DEBUG_LOG_OUT("storageSave.sysDat.openLockMode 0x55：%X", storageSave.sysDat.openLockMode);
		storageSave.sysDat.openLockMode |= 0x80;
		DEBUG_LOG_OUT("storageSave.sysDat.openLockMode 0x55：%X", storageSave.sysDat.openLockMode);
		HAL_Storage_WriteSysData(&storageSave.sysDat);
	}
	else
	{
		pBleAck->ack = eBleAckFail;
	}
	HAL_Comm_BleSendDir(eBleDoorOpeningModeSet);
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_Comm_BleRecvDeal
**功能描述:     蓝牙接收处理函数 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_BleRecvDeal(void)
{
	if (eAPPModeNone == st_InputInfo.appMode)
	{
		Menu_KeepTimeRefresh(); //刷新工作时间
	}
    #ifdef BACK_GET_BAT_STATUS
    st_InputInfo.upBatBattery = (*(pRxData)<<8)+ *(pRxData+1); //获取电压高
    st_InputInfo.downBatBattery = (*(pRxData+2)<<8)+ *(pRxData+3); //获取电压
    if(0 == st_InputInfo.backbatGetFlag) 
    {
        st_InputInfo.backbatGetFlag  = 1;
		DEBUG_LOG_OUT("APP_GetPowerValue 2\n");
        APP_GetPowerValue();
        HAL_KeyLight_sysStatusSet(eColorBlue);
        HAL_KeyLightOnce(MENU_KEY_BACK_LIGHT_ALL_ON);
    }
    #endif
	switch (HAL_Comm_BleGetRecvMsgType())
	{
	case eBleNone:
		break; //(0x00)空闲
	case eBleVerifyChannelPwd:
		break; //(0x01)验证信道密码
	case eBleVerifyPhone:
		break; //(0x03)手机开锁验证
	case eBleUnlockUpload:
		break; //(0x04)开锁实时上传
	case eBleUnlockUploadEncrypt:
		break; //(0x05)开锁实时加密上传wifi
	case eBleUnlockUploadEncryptWithVersion:
		break; //(0x06)整合版本锁具信息实时加密上传wifi
	case eBleAlarmInforUpload:
		break; //(0x07)报警信息上传
	case eBleAlarmInforUploadEncrypt:
		break; //(0x08)加密报警信息上传
	case eBleLockStorageInforGet:
		break; //(0x20)锁存储信息获取
	case eBleUnlockRecordGet:
		break; //(0x21)开锁记录请求查询
	case eBleUnlockModeSetting:
		break; //(0x22)开锁方式设置
	case eBleTimeSync:
		Menu_Comm_BleTimeSync();
		break; //(0x23)时间同步
	case eBleAddFinger:
		Menu_Comm_BleAddFinger();
		break; //(0x24)增加指纹
	case eBleAddPhoneAccount:
		Menu_Comm_BleAddPhoneAccount();
		break; //(0x25)增加手机账号
	case eBleAddSmartKeyAccredit:
		Menu_Comm_BleAddSmartKeyAccredit();
		break; //(0x26)增加智能钥匙授权
	case eBleAddSmartKey:
		break; //(0x27)增加智能钥匙
	case eBleDelFinger:
		Menu_Comm_BleDelFinger();
		break; //(0x28)删除指纹
	case eBleDelPhoneAccount:
		Menu_Comm_BleDelPhoneAccount();
		break; //(0x29)删除手机账号
	case eBleDelSmartKey:
		Menu_Comm_BleDelSmartKey();
		break; //(0x2A)删除智能钥匙
	case eBleFingerAttributeModify:
		Menu_Comm_BleFingerAttributeModify();
		break; //(0x2B)指纹属性修改
	case eBleSmartKeyAttributeModify:
		Menu_Comm_BleSmartKeyAttributeModify();
		break; //(0x2D)智能钥匙属性修改
    case eBleGetMcuVersion:
		Menu_Comm_BleGetMcuVersion();
		break; //(0x2E)获取外围MCU版本号
    case eBleMcuEnterUpdataMode:
		Menu_Comm_BleMcuEnterUpdataMode();
		break; //(0x2F)通知外围控制芯片进入升级模式
	case eBleUnlockPwdSet:
		Menu_Comm_BleUnlockPwdSet();
		break; //(0x30)开锁密码设置
	case eBleUnlockPwdSetAdmin:
		break; //(0x31)锁管理密码设置
	case eBleFactory:
		break; //(0x32)恢复出厂设置
	case eBleEquipmentReg:
		Menu_Comm_BleEquipmentReg();
		break; //(0x34)设备注册
	case eBleEmptyUser:
		Menu_Comm_BleEmptyUser();
		break; //(0x35)清空用户
	case eBleLockPwdVerifyAdmin:
		Menu_Comm_BleLockPwdVerifyAdmin();
		break; //(0x36)锁管理密码验证
	case eBleLockFirmwareVersionGet:
		Menu_Comm_BleLockFirmwareVersionGet();
		break; //(0x37)锁固件版本获取
	case eBleAlarmPwdSet:
		break; //(0x38)报警密码设置
	case eBleVerifyPhoneEncrypt:
		Menu_Comm_BleVerifyPhoneEncrypt();
		break; //(0x39)手机开锁验证加密版
	case eBlePhoneSecretKeyGet:
		Menu_Comm_BlePhoneSecretKeyGet();
		break; //(0x3A)手机秘钥获取
	case eBleSmartKeySecretKeyGetRandom:
		Menu_Comm_BleSmartKeySecretKeyGetRandom();
		break; //(0X3D)智能钥匙授权获取随机数秘钥
	case eBleLockRegSecretKeyGet:
		Menu_Comm_BleLockRegSecretKeyGet();
		break;
	case eBleUnlockSmartKeyVerify:
		Menu_Comm_BleUnlockSmartKeyVerify();
		break; //(0x3B)智能钥匙开锁验证
	case eBleSmartKeySecretKeyGet:
		Menu_Comm_BleSmartKeySecretKeyGet();
		break; //(0X3C)智能钥匙开锁秘钥获取
	case eBleTmpeSecretKeyGet:
		Menu_Comm_BleTmpeSecretKeyGet();
		break; //(0X3F)临时秘钥生成
	case eBleTestServerConnectionStatus:
		Menu_Comm_BleTestServerConnectionStatus();
		break; //(0x40)测试服务器连接状态
	case eBleWifiFunctionSwitchSet:
		Menu_Comm_BleWifiFunctionSwitchSet();
		break; //(0x41)WIFI功能开启关闭设置
	case eBleDisconnectBluetooth:
		Menu_Comm_BleDisconnectBluetooth();
		break; //(0xF0)断开蓝牙连接
	case eBleWifiSetStart:
		break; //(0xF2)WIFI设置启动
	case eBleWifiSetEnd:
		break; //(0xf3)WIFI设置结束
	case eBleLockUpdateProgramMode:
		Menu_Comm_BleLockUpdateProgramMode();
		break; //(0xF4)LOCK更新程序模式
	case eBleWifiSetParm1:
		Menu_Comm_BleWifiSetParm1();
		break; //(0xf5)WIFI设置参数1
	case eBleWifiSetParm2:
		break; //(0xf6)WIFI设置参数2
	case eBleWifiSetDomainNameParm1:
		break; //(0xf7)WIFI设置域名参数1
	case eBleWifiSetDomainNameParm2:
		break; //(0xf8)WIFI设置域名参数2
	case eBleUnlockUploadWifiSW:
		Menu_Comm_BleUnlockUploadWifiSW();
		break; //(0xF9)开锁记录实时上传功能设置(默认功能是关闭)
	case eBlePhoneBleSecretKeyGet:
		break; //(0x50)手机操作BLE秘钥获取指令
	case eBleBleKeyHaveLockChk:
		break; //(0x51)查询BLE钥匙下已有锁(注：容量最大为20)
	case eBlePhoneDelBleKeyAllAccount:
		break; //(0x52)手机删除BLE钥匙上全部账号
	case eBleNewAddFinger:
		Menu_Comm_BleNewAddFinger();
		break; //(0x53)新APP协议增加指纹
	case eBleNewConfirmFingerRegSucced:
		Menu_Comm_BleNewConfirmFingerRegSucced();
		break; //(0x54)新APP协议确认指纹录入成功
	case eBleLockAddSmartKey:
		break; //(0x55)门禁增加智能钥匙
	case eBleLockDelSmartKey:
		break; //(0x56)门禁删除智能钥匙
	case eBleLocalFingerSync:

		break; //(0x57)本地指纹同步
	case eBleLocalFingerDel:
		break; //(0x58)本地指纹删除
	case eBleLocalFingerAttributeModif:
		
		break; //(0x59)本地指纹属性修改
	case eBleEncryptChipSEIDGet:
		break; //(0x60)加密芯片SEID获取
	case eBleEncryptChipChallengeGet:
		break; //(0x61)加密芯片challenge获取
	case eBleEncryptChipVerifyMac:
		break; //(0x62)加密芯片verifymac请求
	case eBleServerTokenVerify:
        Menu_Comm_BleServerTokenVerify();
		break; //(0x65)服务器token验证请求
	case eBleServerIssueChallenge:
        Menu_Comm_BleServerIssueChallenge();
		break; //(0x66)服务器下发challenge
	case eBleID2ServerSwitch:
		Menu_Comm_BleID2ServerSwitch();
		break; //(0x67)启用/禁用ID2服务
	case eBleDHKeyExchangeAlgorithm:
		Menu_Comm_BleDHKeyExchangeAlgorithm();
		break; //(0x70)DH密钥交换算法
	case eBleLockVolumeSet:
		Menu_Comm_BleLockVolumeSet();
		break; //(0x71)锁具音量设置
	case eBleGyroscopeSet:
		Menu_Comm_BleGyroscopeSet();
		break; //(0x72)陀螺仪设置
	case eBleWakeSourceQuery:
		Menu_Comm_BleWakeSourceQuery();
		break; //(0x73)唤醒源查询(调试用APP不需要增加)
	case eBleDESUnlockPwdSet:
		break; //(0x74)DES开锁密码设置
	case eBleDESUnlockAlarmPwdSet:
		break; //(0x75)DES报警密码设置
	case eBleDESTmpeSecretKeyGet:
		break; //(0x76)DES临时秘钥生成
	case eBleDESAddPhoneAccount:
		break; //(0x77)DES增加手机账号
	case eBleDESEncrytPhoneUnlockVerify:
		break; //(0x78)DES手机开锁验证加密版
	case eBleAESUnlockPwdSet:
		Menu_Comm_BleAESUnlockPwdSet();
		break; //(0x79)AES开锁密码设置
	case eBleAESUnlockAlarmPwdSet:
		Menu_Comm_BleAESUnlockAlarmPwdSet();
		break; //(0x7A)AES报警密码设置
	case eBleAESTmpeSecretKeyGet:
		Menu_Comm_BleAESTmpeSecretKeyGet();
		break; //(0x7B)AES临时秘钥生成
	case eBleAESAddPhoneAccount:
		Menu_Comm_BleAESAddPhoneAccount();
		break; //(0x7C)AES增加手机账号
	case eBleAESEncrytPhoneUnlockVerify:

		break; //(0x7D)AES手机开锁验证加密版
	case eBleBaiduSEEncryptActivationRequest:
		break; //(0x7E)百度SE加密激活请求
	case eBleBaiduSEUnlockPwdSet:
		break; //(0x7F)BaiduSE开锁密码设置
	case eBleBaiduSEUnlockAlarmPwdSet:
		break; //(0x80)BaiduSE报警密码设置
	case eBleBaiduSETmpeSecretKeyGet:
		break; //(0x81)BaiduSE临时秘钥生成
	case eBleBaiduSEAddPhoneAccount:
		break; //(0x82)BaiduSE增加手机账号
	case eBleBaiduSEEncrytPhoneUnlockVerify:
		break; //(0x83)BaiduSE手机开锁验证加密版
	case eBleFaceReg:
		Menu_Comm_BleFaceReg();
		break; //(0X85)APP人脸录入
	case eBleFaceRegConfirm:
		Menu_Comm_BleFaceRegConfirm();
		break; //(0x86)APP确认人脸录入成功
	case eBleFaceDelId:
		Menu_Comm_BleFaceDelId();
		break; //(0x87)APP删除人脸ID
	case eBleInfraredMonitoringDistanceSet:
		Menu_Comm_BleInfraredMonitoringDistanceSet();
		break; //(0x88)红外监测距离设置
	case eBleDoorOpeningModeSet:
		Menu_Comm_BleDoorOpeningModeSet();
		break; //(0x89)开门方式设置
	case eBlePhoneDelBleKey:
		break; //(0x90)手机删除BLE钥匙
	case eBleMotorTorqueSet:
		Menu_Comm_BleMotorTorqueSet();
		break; //(0x91)电机扭力调节
	case eBleMotorDirSet:
		Menu_Comm_BleMotorDirSet();
		break; //(0x92)电机方向设置
	case eBleAutomaticLockIntervalSet:
		Menu_Comm_BleAutomaticLockIntervalSet();
		break; //(0x93)自动关门上锁时间间隔设置
	case eBleQueryLockConfigTable:
		Menu_Comm_BleQueryLockConfigTable();
		break; //(0x94)查询锁具配置表
	case eBleAKeyProtection:
		Menu_Comm_BleAKeyProtection();
		break; //(0x95)一键布防
	case eBleActiveDefense:
		Menu_Comm_BleActiveDefense();
		break; //(0x96)主动防御
	case eBleChannelPwdGet:
		Menu_Comm_BleChannelPwdGet();
		break; //(0x97)获取信道密码
	case eBleNFCSwitchSet:
		Menu_Comm_BleNFCSwitchSet();
		break; //(0x98)NFC开关设置
	case eBleDoorbellCapture:
		Menu_Comm_BleDoorbellCapture();
		break; //(0x99)门铃抓拍
	case eBleFaceVersionGet:
		Menu_Comm_BleFaceVersionGet();
		break; //(0x9A)人脸模块版本号获取
	case eBleGetEventRecords:
		Menu_Comm_BleGetEventRecords();
		break; //(0x9B)获取事件记录
	case eBlefaceEnterFirmwareUpdateMode:
		break; //(0x9C)人脸进入固件更新模式
	case eBleFaceAttributeModify:
		Menu_Comm_BleFaceAttributeModify();
		break; //(0X9D)人脸属性修改
	case eBleBleSinglePackageMaxlength:
		break; //(0x9E)获取蓝牙单包可以发送的最大长度
	case eBleDHKeyExchangeAlgorithm1:
		break; //(0x9F)DH密钥交换算法
	case eBleCatEyeSN:
		Menu_Comm_BleCatEyeSN();
		break; //(0xA3)获取猫眼序列号
	case eBleShangTangCatEyeSingleDouble:
		Menu_Comm_BleShangTangCatEyeSingleDouble();
		break; //(0xA4)商汤猫眼单双向切换功能
	case eBleAddICCard:
		break; //(0xA5)app增加IC卡请求
	case eBleDelICCard:
		break; //(0xA6)app删除IC卡请求
	case eBleAutoLockConfig:
		Menu_Comm_BleAutoLockConfig();
		break; //(0xA7)全自动锁体配置
	case eBleAutoLockKeyConfig:
		Menu_Comm_BleAutoLockKeyConfig();
		break; //(0xA8)全自动锁门确认键配置
	case eBleToolAddModel:
		Menu_Comm_BleToolAddModel();
		break; //(0XD5)蓝牙工装添加锁具型号
	case eBleToolTestRWLockModel:
		Menu_Comm_BleToolTestRWLockModel();
		break; //(0xD6)蓝牙工装测试读出与写入锁具型号
	case eBleToolTestFail:
		Menu_Comm_BleToolTestFail();
		break; //(0XD9)工装测试不合格
	case eBleFlashWriteTest:
		Menu_Comm_BleFlashWriteTest();
		break; //(0xDB)FLASH写入测试
	case eBleFlashRWComp:
		Menu_Comm_BleFlashRWComp();
		break; //(0xDC)FLASH读出与写入对比
	case eBleLedOledTest:
		Menu_Comm_BleLedOledTest();
		break; //(0xDD)LED/OLED测试
	case eBleVoltageTest:
		Menu_Comm_BleVoltageTest();
		break; //(0xDE)电压测试
	case eBleLockTimeGet:
		Menu_Comm_BleLockTimeGet();
		break; //(0xDF)读取锁具时间
	case eBleLockAgingRecord:
		break; //(0xB0)查询锁具老化记录
	case eBleCatEyeTestParm:
		break; //(0xE0)设置猫眼产测参数
	case eBleWifiToolTest:
        Menu_Comm_BleWifiToolTest();
		break; //(0xD2)WIFI工装检测
	case eBleXiongmaiCatEyeIPGet:
		break; //(0xE1)雄迈猫眼IP查询
	case eBleSecondBatteryVoltageTest:
		Menu_Comm_BleSecondBatteryVoltageTest();
		break; //(0xE2)下方电池（第二路）电压测试
	case eBleLockMacGet:
		Menu_Comm_BleLockMacGet();
		break; //(0xE3)锁MAC查询
	case eBleCatEyePIRToolTest:
		break; //(0xE4)猫眼PIR工装检测
	case eBleAliyunSecretKeySetParm:
		Menu_Comm_BleAliyunSecretKeySetParm();
		break; //(0xE5)阿里云秘钥设置参数
	case eBleR8CatEyeOTA:
		Menu_Comm_BleR8CatEyeOTA();
		break; //(0XE7)R8猫眼OTA
	case eBleR8CatEyeDebugModeEn:
		break; //(0xE8)R8猫眼DEBUG模式使能
	case eBleR8CatEyeDebugDataTransfer:
		break; //(0xE9)R8猫眼DEBUG数据传输
	case eBleR8EventLog:
		break; //(0XEA)R8WIFI事件记录
	case eBleWifiTestDR:
		break; //(0xEB)WIFI产测扫描指定路由器
	case eBleToolTestIC:
		break; //(0xEC)厂测工装检测IC卡
	case eBleModifFaceSafetyLevel:
		Menu_Comm_BleModifFaceSafetyLevel();
		break; //(0xED)修改人脸识别安全等级
	default:
		break;
	}
}
#if 0
/***************************************************************************************
**函数名:       Menu_Comm_WifiRecvDeal
**功能描述:     wifi接收处理函数 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_WifiRecvDeal(void)
{
	switch (HAL_Comm_WifiGetRecvMsgType(0))
	{
	case eWifiSetRouter:
		break; //0xF6)设置路由器
	case eWifiServerDomain:
		break; //0xF7)设置服务器域名
	case eWifiOpenMsgUpload:
		break; //0x04)开锁信息上传
	case eWifiFaultMsgUpload:
		break; //0x07)故障信息上传
	case eWifiImageUpload:
		break; //0x09)图片上传
	case eWifiOTAUpdate:
		break; //0x10)OTA更新
	case eWifiChkOTA:
		break; //0x11)模组自主查询OTA更新
	case eWifiSetBPS:
		break; //0xFA)通讯波特率设置
	case eWifiChkMAC:
		break; //0xFB)查询WIFI模块MAC地址
	case eWifiProductScanRouter:
		break; //0xFC)产测扫描路由器
	case eWifiChkSoft:
		break; //0xFD)产测扫描路由器
	case eWifiChkPskPwd:
		break; //0xFE)查询当前模块内PSK密码
	default:
		break;
	}
}
#endif
/***************************************************************************************
**函数名:       Menu_CommMsgKeyStatusMenuEnter
**功能描述:     通知前板按键及无线模块唤醒状态进入相应的菜单 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_CommMsgKeyStatusMenuEnter(void)
{
	TYPEe_BackKeyStatus openKey = st_InputInfo.openKey;
	TYPEe_BackKeyStatus closeKey = st_InputInfo.closeKey;
	st_InputInfo.openKey = 0;
	st_InputInfo.closeKey = 0;

	if ((eBackKeyStatusLong == openKey) && (eBackKeyStatusLong == closeKey)) //清空
	{
		fn_JumpMenu(enum_Menu_Factory); //进入初始化确认
	}
	else if (eBackKeyStatusShort == openKey) //开门
	{
		//这里需要添加约束条件
        Menu_Face_PowerOffStart(); //人脸下电
        st_InputInfo.insideOpenLockFlag = 1;
		fn_JumpMenu(enum_Menu_OpenLock);
	}
	else if (eBackKeyStatusShort == closeKey) //关门
	{
		//这里需要添加约束条件
        Menu_ClearError(&st_InputInfo.errorCnt); //清除错误
        Menu_Face_PowerOffStart(); //人脸下电
        
		fn_JumpMenu(enum_Menu_CloseLock);
	}
	else if (eBackKeyStatusLong == closeKey) //长按锁门键打开APP模块
	{
        if((0 != storageSave.sysDat.voiceSw)) //非静音模式下播放语音
        {
            HAL_VoicePlayDir(eVoiceDirAppSetOpened);  //APP设置功能已打开,这里设置为直接播放，不然菜单退回，
        }
		pCommMsgUpdateDeviceName->ovetTime = 100; //
		pCommMsgUpdateDeviceName->nameLen = sizeof(gLockRegBleName) - 1;
		memcpy(&pCommMsgUpdateDeviceName->nameFirst,
			   gLockRegBleName,
			   pCommMsgUpdateDeviceName->nameLen); //填写蓝牙名
		HAL_Comm_SendDealStart(eCommMsgUpdateDeviceName, HAL_COMM_SEND_WAIT_TIEM);
		Menu_Face_PowerOffStart(); //人脸下电
		HAL_FaceId_VerifyUserStop();
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_APPMode);\n");
		fn_JumpMenu(enum_Menu_APPMode);
	}
	else if (eBackKeyStatusLong == openKey) //长按开门键（人脸锁进入管理菜单验证界面）
	{
		if (-1 == Menu_AppRegChk()) //未注册
        {
			if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag) //检查是否注册了管理员
			{
				fn_JumpMenu(enum_Menu_AdminChk); //进入管理菜单检查
				DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AdminChk);\n");
			}
			else
			{
				fn_JumpMenu(enum_Menu_FirstUse); //首次使用
				DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FirstUse);\n");
			}
		}
		else
		{
			DEBUG_LOG_OUT("eBackKeyStatusLong == openKey open door\n");
			fn_JumpMenu(enum_Menu_OpenLock); //直接开门
		}
	}
    else if((0 != st_InputInfo.insideHandOpen)) //内门把手开门
    {
		if(enum_Menu_TestError == st_MenuItem.e_CurrentIndex )
		{
			st_InputInfo.modeuleFail.hall = 0;
			return;
		}
		#if 0
        WakeUpSource = eWakeUpSourceAutoHandle;
		#else
		WakeUpSource = eWakeUpSourceBac;
		#endif
        DEBUG_LOG_OUT("WakeUpSource = eWakeUpSourceAutoHandle\n");
        //自动上锁配置
        pCommOpenAndInfoUpload->OEM = 0;
        memset(pCommOpenAndInfoUpload->MAC, 0x88, sizeof(pCommOpenAndInfoUpload->MAC));
        pCommOpenAndInfoUpload->openType = eOpenTypeNone; //这里不开门
        #if 1
        if(0 == storageSave.sysDat.autoLockTime)
        {
            pCommOpenAndInfoUpload->autoLock = 0;
             pCommOpenAndInfoUpload->autoLockTime =0;
        }
        else
        {
            pCommOpenAndInfoUpload->autoLock = 1;
            pCommOpenAndInfoUpload->autoLockTime = storageSave.sysDat.autoLockTime;
        }
        #else
        pCommOpenAndInfoUpload->autoLock = eAutoLockNeed;
        pCommOpenAndInfoUpload->autoLockTime = 15;
        #endif
        pCommOpenAndInfoUpload->quantity = 45; //这里需要填写电量信息
        BSWAP_SAVE_16(pCommOpenAndInfoUpload->quantity);
        pCommOpenAndInfoUpload->motorTorque = storageSave.hardwareData.torque[0]; //开门扭力
        if(0x01 == storageSave.hardwareData.dir)
        {
            pCommOpenAndInfoUpload->openDoorDir = eOpenDoorDirLeft; //开门方向     
        }
        else
        {
            pCommOpenAndInfoUpload->openDoorDir = eOpenDoorDirRight; //开门方向
        }
        HAL_Comm_SendDealStart(eCommOpenAndInfoUpload, HAL_COMM_SEND_WAIT_TIEM); //发送开门指令
		DEBUG_LOG_OUT("storageSave.sysDat.keyDef X:%X\n",storageSave.sysDat.keyDef);
		if(1 == storageSave.sysDat.keyDef)
		{
			fn_JumpMenu(enum_Menu_KeyDefAlarm); //进入一键布防
			
			wifiSendParm.sendType = eWifiSendTypeAlarmProtection;
			wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据           
            Menu_Face_PowerOffStart();
		}
		else
		{
			#if 1
			fn_JumpMenu(enum_Menu_Desktop);
			st_MenuItem.u16_KeepTime = M_MENU_DISPLAY_TIMER_LEVEL_10; //工作10秒的时间
			#endif
		}
        
    }
    #if 0
	else
	{
		fn_JumpMenu(enum_Menu_Desktop);
	}
    #endif
}
/***************************************************************************************
**函数名:       Menu_CommMsgKeyStatus
**功能描述:     通知前板按键及无线模块唤醒状态 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_CommMsgKeyStatus(void)
{
    if ((eBackKeyStatusLong == pCommMsgKeyStatus->openKey) && 
        (eBackKeyStatusLong == pCommMsgKeyStatus->closeKey)) //清空
	{
		pCommAckStatusSend->ack = eAckStatusSucceed; //回复成功 
        HAL_Comm_SetEncrypt(1); //使用明文的方式
        HAL_Comm_Send(eCommMsgKeyStatus);
	}
	else
    {
        pCommAckStatusSend->ack = eAckStatusSucceed; //回复成功 
        HAL_Comm_Send(eCommMsgKeyStatus);
    }
	if (eAPPModeEnter == st_InputInfo.appMode) //如果在app模式中，不响应后板按键
	{
		return;
	}
	if (enum_Menu_PryAlarm == st_MenuItem.e_CurrentIndex) //在防撬报警中不处理
	{
		return;
	}
	DEBUG_LOG_OUT("pCommMsgKeyStatus:\n");
	DEBUG_LOG_OUT("mac:");
	DEBUG_LOG_OUT_HEX(pCommMsgKeyStatus->MAC, sizeof(pCommMsgKeyStatus->MAC));
	DEBUG_LOG_OUT("rootKey:");
	DEBUG_LOG_OUT_HEX(pCommMsgKeyStatus->rootKey, sizeof(pCommMsgKeyStatus->rootKey));
	DEBUG_LOG_OUT("openKey:%X\n", pCommMsgKeyStatus->openKey);
	DEBUG_LOG_OUT("closeKey:%X\n", pCommMsgKeyStatus->closeKey);
	DEBUG_LOG_OUT("inside:%X\n", pCommMsgKeyStatus->inside);
	BSWAP_SAVE_16(pCommMsgKeyStatus->upBatBattery);
	DEBUG_LOG_OUT("upBatBattery:%X\n", pCommMsgKeyStatus->upBatBattery);
	BSWAP_SAVE_16(pCommMsgKeyStatus->downBatBattery);
	DEBUG_LOG_OUT("downBatBattery:%X\n", pCommMsgKeyStatus->downBatBattery);
	DEBUG_LOG_OUT("version:");
	DEBUG_LOG_OUT_HEX(pCommMsgKeyStatus->version, sizeof(pCommMsgKeyStatus->version));
	//	DEBUG_LOG_OUT("sizeof(st_InputInfo.bleMac):%d",sizeof(st_InputInfo.bleMac));
	memcpy(st_InputInfo.bleMac, pCommMsgKeyStatus->MAC, sizeof(st_InputInfo.bleMac));		//获取MAC
	memcpy(st_InputInfo.rootKey, pCommMsgKeyStatus->rootKey, sizeof(st_InputInfo.rootKey)); //根密钥
	memcpy(st_InputInfo.version, pCommMsgKeyStatus->version, sizeof(st_InputInfo.version)); //后板版本号
																							//	DEBUG_LOG_OUT("memcpy(st_InputInfo.bleMac, pCommMsgKeyStatus->MAC, sizeof(st_InputInfo.bleMac));");
	st_InputInfo.openKey = pCommMsgKeyStatus->openKey;
	st_InputInfo.closeKey = pCommMsgKeyStatus->closeKey;
    st_InputInfo.insideHandOpen = pCommMsgKeyStatus->inside;
    #ifdef BACK_GET_BAT_STATUS
    st_InputInfo.upBatBattery = pCommMsgKeyStatus->upBatBattery;
	st_InputInfo.downBatBattery = pCommMsgKeyStatus->downBatBattery;
    if(0 == st_InputInfo.backbatGetFlag) 
    {
        st_InputInfo.backbatGetFlag  = 1;
		DEBUG_LOG_OUT("APP_GetPowerValue 3\n");
        APP_GetPowerValue();
    }
    #endif    
    
#if 0
	st_InputInfo.upBatBattery = pCommMsgKeyStatus->upBatBattery;
	st_InputInfo.downBatBattery = pCommMsgKeyStatus->downBatBattery;
	if (pCommMsgKeyStatus->upBatBattery <= POWER_VOLTAGE_LOW_CANNOT_WORK)
	{
		DEBUG_LOG_OUT("POWER_VOLTAGE_LOW_CANNOT_WORK\n");
		//电压低无法工作
		HAL_KeyLight_batterySet(eColorRed);
		fn_JumpMenu(enum_Menu_LowPowerCannotWrok);
	}
	else if (pCommMsgKeyStatus->upBatBattery <= POWER_VOLTAGE_LOW)
	{
		DEBUG_LOG_OUT("POWER_VOLTAGE_LOW\n");
		//电压低
		//        HAL_KeyLight_batterySet(HAL_BAT_GREEN);
		HAL_KeyLight_batterySet(eColorRed);
		fn_JumpMenu(enum_Menu_LowPower); //这个菜单退出后进入菜单
	}
	else
	{
		HAL_KeyLight_batterySet(eColorGreen);
		Menu_CommMsgKeyStatusMenuEnter();
	}
#endif
#if 1
	Menu_CommMsgKeyStatusMenuEnter();
#endif
	//获取电池电量
}
/***************************************************************************************
**函数名:       Menu_CommGetBattery
**功能描述:     前板主动获取电池电量 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommGetBattery(void)
{
#if 1
	BSWAP_SAVE_16(pCommGetBatteryAck->upBatBattery);
	BSWAP_SAVE_16(pCommGetBatteryAck->downBatBattery);
	DEBUG_LOG_OUT("eCommGetBattery:\n");
	DEBUG_LOG_OUT("upBatBattery:%x\n", pCommGetBatteryAck->upBatBattery);
	DEBUG_LOG_OUT("downBatBattery:%x\n", pCommGetBatteryAck->downBatBattery);
	DEBUG_LOG_OUT("msgModule:%x\n", pCommGetBatteryAck->msgModule);
	memcpy(st_InputInfo.bleMac, pCommGetBatteryAck->MAC, sizeof(st_InputInfo.bleMac));
    #ifdef BACK_GET_BAT_STATUS
    st_InputInfo.upBatBattery = pCommGetBatteryAck->upBatBattery; //获取电压高
    st_InputInfo.downBatBattery = pCommGetBatteryAck->downBatBattery; //获取电压
    if(0 == st_InputInfo.backbatGetFlag) 
    {
        st_InputInfo.backbatGetFlag  = 1;
		DEBUG_LOG_OUT("APP_GetPowerValue 1\n");
        APP_GetPowerValue();
    }
    #endif
    #if 0
	if(enum_Menu_PryAlarm == st_MenuItem.e_CurrentIndex) //在防撬报警中不处理
	{
		return;
	}
	if (pCommGetBatteryAck->upBatBattery <= POWER_VOLTAGE_LOW_CANNOT_WORK)
	{
		DEBUG_LOG_OUT("POWER_VOLTAGE_LOW_CANNOT_WORK\n");
		//电压低无法工作
		HAL_KeyLight_batterySet(eColorRed);
		fn_JumpMenu(enum_Menu_LowPowerCannotWrok);
	}
	else if (pCommGetBatteryAck->upBatBattery <= POWER_VOLTAGE_LOW)
	{
		DEBUG_LOG_OUT("POWER_VOLTAGE_LOW\n");
		//电压低
		//		HAL_KeyLight_batterySet(HAL_BAT_GREEN);
		HAL_KeyLight_batterySet(eColorRed);
		fn_JumpMenu(enum_Menu_LowPower);
	}
	else
	{
		HAL_KeyLight_batterySet(eColorGreen);
		fn_JumpMenu(enum_Menu_Desktop);
		DEBUG_LOG_OUT("power ok!\n");
	}
    #endif
    
#endif
}
/***************************************************************************************
**函数名:       Menu_CommMsgDisconnectBLE
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommMsgDisconnectBLE(void)
{
	DEBUG_LOG_OUT("Menu_CommMsgDisconnectBLE\n");
}
/***************************************************************************************
**函数名:       Menu_CommMsgMotorLock
**功能描述:     前板锁门键通知后板上锁 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommMsgMotorLock(void)
{
	//判断是不是关锁成功
	//和发送的命令是否能对上
	if (eAckStatusLockSucceed == pCommAckStatusRcve->ack)
	{
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_CloseLockSuccess);");
		fn_JumpMenu(enum_Menu_CloseLockSuccess);
	}
	else
	{
		//上锁失败
	}
}

/***************************************************************************************
**函数名:       Menu_CommOpenAndInfoUpload
**功能描述:     开门及wifi信息透传 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommOpenAndInfoUpload(void)
{
	//处理相关数据
	if (eAckStatusLockSucceed == pCommAckStatusRcve->ack)
	{
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_OpenLockSuccess)\n");
        st_InputInfo.motorRuningFlag = 0; //清除开门中标志
		fn_JumpMenu(enum_Menu_OpenLockSuccess);
	}
	else
	{
		//开门失败
	}
}

/***************************************************************************************
**函数名:       Menu_CommMsgEnterOTA
**功能描述:     进入OTA程序 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommMsgEnterOTA(void)
{
	if (eAckStatusSucceed == pCommAckStatusRcve->ack)
	{
			HAL_OnChipFlash_Init();  //片内FLASH初始化
			HAL_OnChipFlash_Write(APP_EXTIST_ADDRESS, AppProgramMCU, 4); //标志置位
			
			DEBUG_LOG_OUT("cpu reset\n");
			HAL_Power_CPURst(); //重启CPU
			while(1);
			//进入升级的步骤.
	}
	else
	{
		DEBUG_LOG_OUT("Fail\n");
	}
}

/***************************************************************************************
**函数名:       Menu_CommMsgUpdateDeviceName
**功能描述:      
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_CommMsgUpdateDeviceName(void)
{
}

/***************************************************************************************
**函数名:       Menu_Comm_Service
**功能描述:     通讯服务程序 
**输入参数:     --
**输出参数:     --
**备注:         

****************************************************************************************/
void Menu_Comm_Service(void)
{
	uint32_t temp = 0;
	HAL_Comm_RevcDeal();
	switch (HAL_Comm_SendDeal())
	{
	case eCommStatusReceiveOverTime:	  //数据接收超时
        #if 1
        HAL_Comm_RstBack(); //复位后板
        #endif
		fn_JumpMenu(enum_Menu_CommError); //通讯错误提示
		HAL_Comm_CealrRecv();			  //清除接收
		HAL_Comm_CealrSend();			  //清除发送
		break;

	default:
		break;
	}
	switch (HAL_Comm_GetRecvMsgType())
	{
	case eCommMsgKeyStatus:
		Menu_CommMsgKeyStatus();
		break;
	case eCommGetBattery:
		Menu_CommGetBattery();
		break;
	case eCommMsgDisconnectBLE:
		Menu_CommMsgDisconnectBLE();
		break;
	case eCommMsgMotorLock:
		Menu_CommMsgMotorLock();
		break; //(0x20)通知后板电机上锁
	case eCommOpenAndInfoUpload:
		Menu_CommOpenAndInfoUpload();
		break; //(0x21)开门及信息上传
	case eCommMsgEnterOTA:
		Menu_CommMsgEnterOTA();
		break; //(0x2C)前板通知后板进入OTA模式
	case eCommBleDataPass:

		Menu_Comm_BleRecvDeal();
		break; //(0x30)后面板BLE数据透传
	case eCommwifiDataPass:

		break; //(0x31)后面板WIFI数据透传
	case eCommMsgUpdateDeviceName:
		Menu_CommMsgUpdateDeviceName();
		break; //(0x2A)前板通知后板更新设备名称
	case eCommZigbeeExitNet:
		break; //(0xF0)ZIGBEE模块退网
	case eCommZigbeeEnterNet:
		break; //(0xF1)ZIGBEE模块入网
	case eCommWirelessMAC:
		break; //(0xFB)查询无线模块MAC地址
	case eCommScanGateway:
		break; //(0XFC)产测扫描路由器/无线模块网关
	default:
		break;
	}
	HAL_Comm_CealrRecv(); //处理完成后清除
}
#endif /*MENU_MODULE_EN*/
/************************************Code End******************************************/
