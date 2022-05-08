/***************************************************************************************
**文件名:     Menu_WhenEnterMenuOperation.c
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
#include "Function.h"
#include "Menu_CurrentOperate.h"
#include "HAL_Storage.h"
#include "HAL_Motor.h"
#include "HAL_Finger.h"
#include "HAL_FaceId.h"
#include "HAL_Comm.h"
#include "HAL_KeyLight.h"
#include "HAL_Power.h"

#if MENU_MODULE_EN
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_Face_SetDemoMode
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void Menu_Face_SetDemoMode(void)
{
    if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag) //检查是否注册了管理员
    {
        msgDemoModeoData.enable = 0;
    }
    else
    {
        msgDemoModeoData.enable = 1;
    }
    HAL_FaceId_SetDemoMode(); //设置系统模式
}
/*************************全局函数****************************/

/***************************************************************************************
**函数名:       fn_WhenEnterMenuOperation
**功能描述:     进入菜单时执行的操作,在显示新菜单前执行
**输入参数:     TYPEe_MenuIndex e_JumpMenu
**输出参数:     --
**备注:         
****************************************************************************************/
void fn_WhenEnterMenuOperation(TYPEe_MenuIndex e_JumpMenu)
{
    INT16U i;
    switch (e_JumpMenu)
    {
    case enum_Menu_Desktop:
#if defined(KEY_RANDOM_LOCK_EN)
        DEBUG_LOG_OUT("enter enum_Menu_Desktop\n");
        HAL_KeyLight_sysStatusSet(eColorBlue);
        #if 0
        if ((eKeyRandomLockFlagEnable == st_InputInfo.keyRandomLockFlag)) //按键有锁
        {
            st_InputInfo.disFaceVerifyFlag = 0; //清除
            faceIdCtrl.enableNetworkBack = 0;
            Menu_Face_PowerOnStart(0);
            HAL_FaceId_VerifyUserStart();
            DEBUG_LOG_OUT("HAL_FaceId_VerifyUserStart();\n");
        }
        #endif
#endif //KEY_RANDOM_LOCK_EN
#if defined(SYSTEM_BREATHING_LIGHT_EN)
        HAL_BreathingLightBlue(); //呼吸灯颜色调节
#endif                            //SYSTEM_BREATHING_LIGHT_EN
        break;
    case enum_Menu_AdminChk:
        HAL_KeyLight_sysStatusSet(eColorClose);
        Menu_Face_PowerOnStart(1); //直接重启的方式
        HAL_FaceId_VerifyUserStart(); //开启人脸识别
        break;
    case enum_Menu_FirstUse:

        break;
    case enum_Menu_SystemSleep:
        if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance)) //判断系统是否打开了主动防御
        {
            DEBUG_LOG_OUT("dakaizhudongfangyu\r\n");
           HAL_BuFang_Time_Sleep(st_InputInfo.defEn,(uint32_t *)&st_InputInfo.defSysTickCnt); 
        }
		else
		{
			DEBUG_LOG_OUT("关闭zhudongfangyu\r\n");
			HAL_BuFang_Time_Sleep(30,0);//关闭主要防御
		}
        st_InputInfo.appMode = eAPPModeNone; //清除app管理
        Menu_Face_PowerOffStart(); //人脸下电
        DEBUG_LOG_OUT("enter enum_Menu_SystemSleep\n");
        break;
    case enum_Menu_FaceSet: //人脸信息设置
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart enum_Menu_FaceSet\n");
        Menu_Face_PowerOffStart();
        HAL_FaceId_AddUserStop();
        break;
    case enum_Menu_AddAdminFace: //增加管理员人脸
        DEBUG_LOG_OUT("enter enum_Menu_AddAdminFace\n");
        memset(&storageSave.UserDat, 0, sizeof(storageSave.UserDat)); //清除数据
        DEBUG_LOG_OUT("storageSave.sysDat.localAdminNumFace:%d\n", storageSave.sysDat.localAdminNumFace);
        if (storageSave.sysDat.localAdminNumFace >= MEM_FINGER_FACE_ADMIN_LOCAL) //检查用户数量
        {
            fn_JumpMenu(enum_Menu_AddAdminFaceFull); //管理员已经满了
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_AddAdminFaceFull);\n");
        }
        else
        {
            if (MSG_FACTMEM_REG_STATUS_OK != storageSave.sysDat.localAdminRegFlag) //锁未注册管理员
            {
                storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_ING; //进入注册流程
            }
            DEBUG_LOG_OUT("storageSave.sysDat.localAdminRegFlag:%x\n", storageSave.sysDat.localAdminRegFlag);
            //进入管理人脸添加
            storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN; //类别为管理员
            fn_JumpMenu(enum_Menu_AddFaceStep1);             //进入注册流程。
            #if 0
			Menu_Face_PowerOnStart(0);
			#else
			Menu_Face_PowerOnStart(1);
			#endif
            msgEnrollData.admin = 1; //管理员
            HAL_FaceId_AddUserStart(); //开始添加
        }

        break;
    case enum_Menu_AddGeneralFace: //增加普通人脸
        DEBUG_LOG_OUT("enter enum_Menu_AddGeneralFace\n");
        if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag) //检查是否注册了管理员
        {
            memset(&storageSave.UserDat, 0, sizeof(storageSave.UserDat));           //清除数据
            if (storageSave.sysDat.localUserNumFace >= MEM_FINGER_FACE_GUEST_LOCAL) //检查用户数量
            {
                fn_JumpMenu(enum_Menu_AddGeneralFaceFull); //普通人脸已满
                DEBUG_LOG_OUT("enum_Menu_AddGeneralFaceFull\n");
            }
            else
            {
                storageSave.UserDat.Lim = MSG_FACTMEM_LIM_GENERAL; //普通
                fn_JumpMenu(enum_Menu_AddFaceStep1);               //进入注册流程。
                DEBUG_LOG_OUT("enum_Menu_AddFaceStep1\n");
				#if 0
				Menu_Face_PowerOnStart(0);
				#else
				Menu_Face_PowerOnStart(1);
				#endif
                msgEnrollData.admin = 0; 
                HAL_FaceId_AddUserStart(); //开始添加
            }
        }
        else
        {
            fn_JumpMenu(enum_Menu_FirstUse); //首次使用
            DEBUG_LOG_OUT("enum_Menu_FirstUse\n");
        }
        break;
    case enum_Menu_DelGeneralFace: //删除普通用户人脸
                                   //		HAL_FaceId_DelOneUserStart();
        Menu_Face_DelGeneralFaceStart();
        DEBUG_LOG_OUT("enter enum_Menu_DelGeneralFace\n");
        break;
    case enum_Menu_FaceSetReturn: //人脸识别菜单返回
        DEBUG_LOG_OUT("enter enum_Menu_FaceSetReturn\n");
        fn_JumpMenu(enum_Menu_Admin);
        break;
    case enum_Menu_BleAddFaceUserOk:
    case enum_Menu_AddFaceUserOk:
        //        Menu_Face_SetDemoMode();
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart enum_Menu_AddFaceUserOk\n");
        Menu_Face_PowerOffStart();
        HAL_FaceId_AddUserStop();

        break;
        break;
    case enum_Menu_BleAddFaceUserFail: //添加人脸失败蓝牙
    case enum_Menu_AddFaceUserFail:    //添加人脸失败
        DEBUG_LOG_OUT("enter enum_Menu_AddFaceUserFail\n");
        Menu_Face_PowerOffStart();
        HAL_FaceId_AddUserStop();
        if (MSG_FACTMEM_REG_STATUS_ING == storageSave.sysDat.localAdminRegFlag)
        {
            storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_NONE; //注册失败
        }
        break;
    case enum_Menu_FaceVerifyOk:                 //人脸验证成功
        Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
        pCommOpenAndInfoUpload->OEM = 0;
        memset(pCommOpenAndInfoUpload->MAC, 0x88, sizeof(pCommOpenAndInfoUpload->MAC));
        pCommOpenAndInfoUpload->openType = eOpenTypeFace;
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
        
        pCommOpenAndInfoUpload->quantity = 45;
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
        st_InputInfo.motorRuningFlag = 1;
        
        wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
        wifiSendParm.unlockType = eWifiunlockTypeFace;
        wifiSendParm.pageId[0] = (uint8_t)(0);
        wifiSendParm.pageId[1] = (uint8_t)(0);
        wifiSendParm.pageId[2] = (uint8_t)(st_InputInfo.userID >> 8);
        wifiSendParm.pageId[3] = (uint8_t)(st_InputInfo.userID); //PageID
        if(1 == storageSave.UserDat.kindred)    //这里需要推送亲情和SOS
        {
            wifiSendParm.limt |= MENU_UNLOCK_ATTR_KINDNESS;
        }
        if(1 == storageSave.UserDat.sos)
        {
            wifiSendParm.limt |= MENU_UNLOCK_ATTR_SOS;
        }
        #if 1
        DEBUG_LOG_OUT("wifiSendParm.limt:%X\n",wifiSendParm.limt);
        
        #endif
        wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
        break;
    case enum_Menu_FaceVerifyFail: //人脸验证失败
        DEBUG_LOG_OUT("enter enum_Menu_FaceVerifyFail\n");
        break;
    case enum_Menu_DelGeneralFaceOK: //删除普通成功
        Menu_Face_PowerOffStart();   //人脸下电
        HAL_FaceId_DelOneUserStop();
        DEBUG_LOG_OUT("enter enum_Menu_DelGeneralFaceOK\n");
        break;
    case enum_Menu_DelGeneralFaceFail:
        Menu_Face_PowerOffStart(); //人脸下电
        HAL_FaceId_DelOneUserStop();
        DEBUG_LOG_OUT("enter enum_Menu_DelGeneralFaceFail\n");
        break;
    case enum_Menu_AddFingerStep1:
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_RegStart(); //开指纹注册
#endif                         // SYSTEM_FINGER_EN

        break;
    case enum_Menu_Admin:
        DEBUG_LOG_OUT("WhenEnterMenuOperation enum_Menu_Admin\n");
        break;
    case enum_Menu_AdminReturn:
        fn_JumpMenu(enum_Menu_BackDeal); //后台处理后休眠
        DEBUG_LOG_OUT("WhenEnterMenuOperation enum_Menu_AdminReturn\n");
        break;
    case enum_Menu_FingerSetReturn:
        fn_JumpMenu(enum_Menu_Admin); //返回管理菜单
        DEBUG_LOG_OUT("JumpMenu(enum_Menu_Admin);\n");
    case enum_Menu_AddAdminFinger:
        DEBUG_LOG_OUT("WhenEnterMenuOperation enum_Menu_AddAdminFinger\n");
        break;
    case enum_Menu_AddGeneralFinger:
        DEBUG_LOG_OUT("WhenEnterMenuOperation enum_Menu_AddGeneralFinger\n");
        if (storageSave.sysDat.localUserNum < MEM_FINGER_GUEST_ALL_LOCAL) //检查普通普通数量
        {
            storageSave.UserDat.Lim = MSG_FACTMEM_LIM_GENERAL; //普通类型
            st_InputInfo.FingerIdTmp = HAL_Storage_SearchEmpty(0,
                                                               MEM_FINGER_ALL_LOCAL,
                                                               MSG_FINGER_REG_MEM_LOCAL_ST,
                                                               MSG_FINGER_ONE_SIZE); //获取空位
                                                                                     //没有空位的时候，不会进入到这里
            DEBUG_LOG_OUT("Empty id is:%x\n",st_InputInfo.FingerIdTmp);
            fn_JumpMenu(enum_Menu_AddFingerStep1);                                   //进入注册流程。
#if defined(SYSTEM_FINGER_EN)
            HAL_Finger_RegStart(); //开启注册过程
#endif                             // SYSTEM_FINGER_EN
        }
        else
        {
            fn_JumpMenu(enum_Menu_AddGeneralFingerFull); //普通指纹已满
        }
        break;
    case enum_Menu_AddFingerReturn:
        fn_JumpMenu(enum_Menu_Admin); //返回管理菜单
        DEBUG_LOG_OUT("JumpMenu(enum_Menu_Admin);\n");
        break;
    case enum_Menu_AddFingerUserOk:
        switch (storageSave.UserDat.Lim)
        {
        case MSG_FACTMEM_LIM_ADMIN: //添加管理指纹本地

            break;
        case MSG_FACTMEM_LIM_GENERAL: //添加普通指纹本地
            storageSave.sysDat.localUserNum++;
            HAL_Storage_WriteSysData(&storageSave.sysDat); //保存用户信息
			DEBUG_LOG_OUT("enum_Menu_AddFingerUserOk st_InputInfo.FingerIdTmp X:%X\n",st_InputInfo.FingerIdTmp);
			memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
            storageSave.UserDat.pageID = st_InputInfo.FingerIdTmp;
			storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
            HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
                                       MSG_FINGER_ONE_SIZE,
                                       st_InputInfo.FingerIdTmp,
                                       &storageSave.UserDat); //保存指纹信息
            st_InputInfo.userIdTmp = BSWAP_16(st_InputInfo.FingerIdTmp);
            Menu_RecordLockLog(eLogTypeAddFinger,
                               (uint8_t *)&(st_InputInfo.userIdTmp),
                               sizeof(st_InputInfo.userIdTmp)); //保存事件记录
            break;
        default:
            break;
        }
        break;
    case enum_Menu_DelFinger: //删除指纹
#if defined(SYSTEM_FINGER_EN)
        HAL_Finger_DelStart(); //开启指令
#endif                         // SYSTEM_FINGER_EN
        //#if defined(SYSTEM_STORAGE_EN)
        ////        APP_Storage_DelAllFingerLocalGeneral(); //删除所有的普通指纹
        //#endif

        break;
    case enum_Menu_DelFingerOk:
        DEBUG_LOG_OUT("WhenEnterMenuOperation enum_Menu_DelFingerOk \n");
        break;
    case enum_Menu_MotorTorsionLow:
#ifndef LOCK_OPEN_PARM_NO_CLEAR
        storageSave.sysDat.torque[0] = MOTOR_TORQUE_LOW;
        HAL_Storage_WriteSysData(&storageSave.sysDat);
#else
        storageSave.hardwareData.torque[0] = MOTOR_TORQUE_LOW;
        HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
#endif

        fn_JumpMenu(enum_Menu_MotorTorsionSetOk);
        break;
    case enum_Menu_MotorTorsionHigh:
#ifndef LOCK_OPEN_PARM_NO_CLEAR
        storageSave.sysDat.torque[0] = MOTOR_TORQUE_HIGH;
        HAL_Storage_WriteSysData(&storageSave.sysDat);
#else
        storageSave.hardwareData.torque[0] = MOTOR_TORQUE_HIGH;
        HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
#endif
        HAL_Storage_WriteSysData(&storageSave.sysDat);
        fn_JumpMenu(enum_Menu_MotorTorsionSetOk);
        break;
    case enum_Menu_MotorTorsionSetReturn:
        fn_JumpMenu(enum_Menu_Admin);
        break;
    case enum_Menu_VioceClose:
        storageSave.sysDat.voiceSw = HAL_STORAGE_SYS_VIOCE_SW_CLOSE;
        HAL_Storage_WriteSysData(&storageSave.sysDat);
        fn_JumpMenu(enum_Menu_VioceSetOk);
        break;
    case enum_Menu_VioceOpen:
        storageSave.sysDat.voiceSw = HAL_STORAGE_SYS_VIOCE_SW_OPEN;
        HAL_Storage_WriteSysData(&storageSave.sysDat);
        fn_JumpMenu(enum_Menu_VioceSetOk);
        break;
    case enum_Menu_VioceSetReturn:
        fn_JumpMenu(enum_Menu_Admin);
        break;
    case enum_Menu_BleEmptyUser:
		st_InputInfo.emptyFinger = eEquipResultNone;
        st_InputInfo.emptyFinger = eEquipResultNone;
        HAL_Storage_ReadLockLog((uint8_t *)&st_InputInfo.lockLog, 0); //读取位置0的数据
        DEBUG_LOG_OUT("enum_Menu_BleEquipmentReg lockLog:\n");
        DEBUG_LOG_OUT_HEX(st_InputInfo.lockLog, sizeof(st_InputInfo.lockLog));
        HAL_Storage_ClearUserData();                                   //清除所有的用户数据
        HAL_Storage_ClearSysData();                                    //清空系统数据
		st_InputInfo.defEn = storageSave.sysDat.irDef; //主动防御的参数
		st_InputInfo.keyRandomSetFlag = ekeyRandomSetDisable; 
        HAL_Storage_WriteLockLog((uint8_t *)&st_InputInfo.lockLog, 0); //把0位置的数据恢复
        HAL_Storage_WriteHardwareData(&storageSave.hardwareData);         //保存硬件数据
        HAL_Comm_AesKeySet(storageSave.hardwareData.encryptKey);          //设置AES密码
        Menu_Face_PowerOnStart(0);                                         //给人脸模块上电
        HAL_FaceId_DelAllUserStart();                                     //开始人脸清空指令
        HAL_Finger_EmptyStart();  
		break;
    case enum_Menu_BleEquipmentReg:
        st_InputInfo.emptyFinger = eEquipResultNone;
        st_InputInfo.emptyFinger = eEquipResultNone;
        HAL_Storage_ReadLockLog((uint8_t *)&st_InputInfo.lockLog, 0); //读取位置0的数据
        DEBUG_LOG_OUT("enum_Menu_BleEquipmentReg lockLog:\n");
        DEBUG_LOG_OUT_HEX(st_InputInfo.lockLog, sizeof(st_InputInfo.lockLog));
        HAL_Storage_ClearUserData();                                   //清除所有的用户数据
        HAL_Storage_ClearSysData();                                    //清空系统数据
		st_InputInfo.defEn = storageSave.sysDat.irDef; //主动防御的参数
		st_InputInfo.keyRandomSetFlag = ekeyRandomSetDisable; 
        HAL_Storage_WriteLockLog((uint8_t *)&st_InputInfo.lockLog, 0); //把0位置的数据恢复
#if 0
        HAL_Storage_DelNUserReg(0,
                                MEM_FINGER_GUEST_ALL_LOCAL,
                                MSG_FINGER_REG_MEM_LOCAL_ST,
                                MSG_FINGER_ONE_SIZE); //清空指纹
        HAL_Storage_DelNUserReg(0,
                                MEM_FACE_ALL,
                                MSG_FINGER_REG_MEM_LOCAL_ST,
                                MSG_FINGER_ONE_SIZE); //清空人脸
        HAL_Storage_DelNUserReg(0,
                                MEM_FACE_ALL,
                                MSG_FACE_REG_MEM_ST,
                                MSG_FACE_REG_SIZE);                       //清空人脸
#endif
            //清空用户数据
        storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_OK; //注册成功
        HAL_Storage_WriteSysData(&storageSave.sysDat);                    //保存数据
        HAL_Storage_WriteHardwareData(&storageSave.hardwareData);         //保存硬件数据
        HAL_Comm_AesKeySet(storageSave.hardwareData.encryptKey);          //设置AES密码
        Menu_Face_PowerOnStart(0);                                         //给人脸模块上电
        HAL_FaceId_DelAllUserStart();                                     //开始人脸清空指令
        HAL_Finger_EmptyStart();                                          //开始指纹清空
        break;
    case enum_Menu_Factory:
        HAL_FaceId_VerifyUserStop();
        Menu_Face_PowerOffStart(); //人脸下电
        break;
    case enum_Menu_FactoryIng:
//        HAL_KeyLight_sysStatusSet(eColorBlue);
        st_InputInfo.emptyFinger = eEquipResultNone;
        st_InputInfo.emptyFinger = eEquipResultNone;
        HAL_Storage_ClearUserData();                                  //清除所有的用户数据
        HAL_Storage_ClearSysData();                                   //清空系统数据,并写入初始数据
        st_InputInfo.defEn = storageSave.sysDat.irDef; //主动防御的参数
		st_InputInfo.keyRandomSetFlag = ekeyRandomSetDisable; 
	
		Menu_RecordLockLog(eLogTypeEmpty, NULL, 0);                   //保存事件记录
        HAL_Storage_ReadLockLog((uint8_t *)&st_InputInfo.lockLog, 0); //读取位置0的数据
        DEBUG_LOG_OUT("enum_Menu_FactoryIng lockLog:\n");
        DEBUG_LOG_OUT_HEX(st_InputInfo.lockLog, sizeof(st_InputInfo.lockLog));
#if 0
        HAL_Storage_ClearSysData(); //清空系统数据
        HAL_Storage_DelNUserReg(0,
                                MEM_FINGER_GUEST_ALL_LOCAL,
                                MSG_FINGER_REG_MEM_LOCAL_ST,
                                MSG_FINGER_ONE_SIZE); //清空指纹
        HAL_Storage_DelNUserReg(0,
                                MEM_FACE_ALL,
                                MSG_FINGER_REG_MEM_LOCAL_ST,
                                MSG_FINGER_ONE_SIZE); //清空指纹
		HAL_Storage_DelNUserReg(0,
							  MEM_FACE_ALL,
							  MSG_FACE_REG_MEM_ST,
							  MSG_FACE_REG_SIZE); //清空人脸
#endif
        //清空用户数据
        #if 0
        memset(&storageSave.hardwareData, 0, sizeof(storageSave.hardwareData)); //清空硬件数据
        #endif
        
        memcpy(storageSave.hardwareData.encryptKey,
               st_InputInfo.rootKey,
               sizeof(storageSave.hardwareData.encryptKey)); //保存根密钥
               DEBUG_LOG_OUT("storageSave.hardwareData.encryptKey:");
        DEBUG_LOG_OUT_HEX(storageSave.hardwareData.encryptKey, sizeof(storageSave.hardwareData.encryptKey));       
        memcpy(storageSave.hardwareData.mac,
               st_InputInfo.bleMac,
               sizeof(storageSave.hardwareData.mac)); //保存MAC
        DEBUG_LOG_OUT("storageSave.hardwareData.mac:");
        DEBUG_LOG_OUT_HEX(storageSave.hardwareData.mac, sizeof(storageSave.hardwareData.mac));

        Menu_GetchannelPwd(storageSave.hardwareData.channelPwd, sizeof(storageSave.hardwareData.channelPwd)); //获取信道密码
        memcpy(storageSave.hardwareData.version,
               st_InputInfo.version,
               sizeof(storageSave.hardwareData.version));         //保存后板版本号
        HAL_Storage_WriteSysData(&storageSave.sysDat);            //保存数据
        HAL_Storage_WriteHardwareData(&storageSave.hardwareData); //保存硬件数据
        Menu_Face_PowerOnStart(0);                                 //给人脸模块上电
        HAL_Comm_EncryptionParam(storageSave.hardwareData.mac,
                                 storageSave.hardwareData.encryptKey); //设置通讯AES密码和MAC
        #if 1
        DEBUG_LOG_OUT("storageSave.hardwareData.dir:%X\n",storageSave.hardwareData.dir);
        DEBUG_LOG_OUT("storageSave.hardwareData.torque[0]:%X\n",storageSave.hardwareData.torque[0]);
        #endif
        HAL_FaceId_DelAllUserStart();                             //开始人脸清空指令
        HAL_Finger_EmptyStart();                                  //开始清空指纹
        break;
    case enum_Menu_TryFingerVerifyOK:              //试用模式通过
    case enum_Menu_PassWdPass:                     //密码验证通过
    case enum_Menu_FingerVerifyOK:                 //指纹校验成功
    case enum_Menu_BleVerifyPhoneEncryptSuccess:   //手机开锁成功
    case enum_Menu_BleUnlockSmartKeyVerifySuccess: //智能钥匙开门成功
        Menu_ClearError(&st_InputInfo.errorCnt);   //清除错误次数
        pCommOpenAndInfoUpload->OEM = 0;
        memset(pCommOpenAndInfoUpload->MAC, 0x88, sizeof(pCommOpenAndInfoUpload->MAC));
        pCommOpenAndInfoUpload->openType = eOpenTypeFace;
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
        pCommOpenAndInfoUpload->quantity = 45;
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
        st_InputInfo.motorRuningFlag = 1;
#if defined(SYSTEM_MOTOR_EN)
        HAL_Motor_OpenClose();
#endif // SYSTEM_MOTOR_EN
#if defined(SYSTEM_BREATHING_LIGHT_EN)
        HAL_BreathingLightGreen(); //呼吸灯颜色调节
#endif                             //SYSTEM_BREATHING_LIGHT_EN

        break;
    case enum_Menu_PassWdFail: //密码验证失败
        Menu_AddError(&st_InputInfo.errorCnt, eUserTypePWD);
#if defined(SYSTEM_BREATHING_LIGHT_EN)
        HAL_BreathingLightRed(); //呼吸灯颜色调节
#endif                           //SYSTEM_BREATHING_LIGHT_EN
        break;
    case enum_Menu_FingerVerifyFail: //指纹校验失败

        Menu_AddError(&st_InputInfo.errorCnt, eUserTypeFinger);
        break;
    case enum_Menu_BleUnlockSmartKeyVerifyFail:
        Menu_AddError(&st_InputInfo.errorCnt, eUserTypeAll);
        break;
    case enum_Menu_BackDeal: //后台处理
//关闭灯光
#if defined(SYSTEM_BREATHING_LIGHT_EN)
        HAL_BreathingLightOff(); //关闭呼吸灯
#endif                           //SYSTEM_BREATHING_LIGHT_EN

        //开启后台处理的功能。
        //处理完成后休眠
        break;
    case enum_Menu_CloseLock:
        pCommMsgMotorLock->outdoorLock = 0;
        pCommMsgMotorLock->indoorLock = 1;
        pCommMsgMotorLock->lockBodyType = eLockBodyType;
        pCommMsgMotorLock->motorTorque = storageSave.hardwareData.torque[0]; //开门扭力

        if(0x01 == storageSave.hardwareData.dir)
        {
            DEBUG_LOG_OUT("<---------------- eOpenDoorDirLeft\n");
            pCommMsgMotorLock->openDoorDir = eOpenDoorDirLeft; //开门方向     
        }
        else
        {
            DEBUG_LOG_OUT("----------------> eOpenDoorDirRight\n");
            pCommMsgMotorLock->openDoorDir = eOpenDoorDirRight; //开门方向
        }
        pCommMsgMotorLock->quantity = 0;
        BSWAP_SAVE_16(pCommMsgMotorLock->quantity);
        pCommMsgMotorLock->wirelessModule = eWirelessModuleWifi;
        HAL_Comm_SendDealStart(eCommMsgMotorLock, HAL_COMM_SEND_WAIT_TIEM);
        break;
    case enum_Menu_BleAddFingerOK:
        if (0 != storageSave.sysDat.leaveFinger)
        {
            storageSave.sysDat.leaveFinger--;
        }
        HAL_Storage_WriteSysData(&storageSave.sysDat); //保存用户信息
        storageSave.UserDat.pageID = st_InputInfo.FingerIdTmp;
        HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
                                   MSG_FINGER_ONE_SIZE,
                                   st_InputInfo.FingerIdTmp,
                                   &storageSave.UserDat); //保存指纹信息
        break;
    case enum_Menu_OpenLock:
        Menu_ClearError(&st_InputInfo.errorCnt); //清除错误
        pCommOpenAndInfoUpload->OEM = 0;
        memset(pCommOpenAndInfoUpload->MAC, 0x88, sizeof(pCommOpenAndInfoUpload->MAC));
        pCommOpenAndInfoUpload->openType = eOpenTypeFace;
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
        st_InputInfo.motorRuningFlag = 1;
        break;
    case enum_Menu_APPMode:
        st_InputInfo.appMode = eAPPModeEnter; //进入APP模式
                                              //		HAL_FaceId_AddUserStop();
        break;
    case enum_Menu_FactoryOK:
    case enum_Menu_FactoryFail:
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart enum_Menu_FactoryOK\n");
        Menu_Face_PowerOffStart(); //人脸下电
        HAL_FaceId_DelAllUserStop();
        break;
    #if 0
    case enum_Menu_BleFaceVersionGet:
        HAL_FaceId_GetVersionStart();
        break;
    #endif
    case enum_Menu_Bell:
        HAL_FaceId_VerifyUserStop();
        Menu_Face_PowerOffStart(); //人脸下电
        if((MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) || 
            (0 != Menu_LowPowerChk()))     //检查wifi主开关开启
        {
            st_MenuItem.u16_KeepTime = M_MENU_DISPLAY_TIMER_LEVEL_2; //持续2秒退出
            return;
        } 
        msgVideoCallData.captureTime = 8;
        BSWAP_SAVE_32(msgVideoCallData.captureTime);
        msgVideoCallData.enableRecord = 1; //使能抓拍
        wifiSendParm.timerCnt = 0;
        wifiSendParm.connectWaitOver = eConnectWaitOverNo;
        wifiSendParm.bellOutFlag = eBellOutFlagDis; //禁止退出
        Menu_Face_PowerOnStart(0);  //给人脸模块上电
        faceIdCtrl.enableNetworkBack = 1;
        HAL_FaceId_CmdDealStart(eFaceIdCmdVideoCallData, 0);
        break;
    case enum_Menu_CatEyeConn:
        //		HAL_FaceId_VerifyUserStop();
        //		Menu_Face_PowerOffStart(); //人脸下电
        wifiSendParm.timerCnt = 0;
        wifiSendParm.connectWaitOver = eConnectWaitOverNo;
        Menu_Face_PowerOnStart(0);  //给人脸模块上电
        faceIdCtrl.enableNetworkBack = 1;
        DEBUG_LOG_OUT("enter enum_Menu_CatEyeConn\n");
        break;
    case enum_Menu_CatEyeDisConn:
        Menu_Face_PowerOffStart(); //人脸下电
        DEBUG_LOG_OUT("enter enum_Menu_CatEyeDisConn\n");
        break;
    case enum_Menu_BleShangTangCatEyeSingleDouble:
    case enum_Menu_WifiConnect:
        //低电压不操作。
        //存储错误不操作。
        //wifi关闭不操作
        if((MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) 
            || (0 != Menu_LowPowerChk()))     //检查wifi主开关开启
        {
            DEBUG_LOG_OUT("enum_Menu_WifiConnect to enum_Menu_Desktop\n");
            DEBUG_LOG_OUT("storageSave.sysDat.wifiMainSw:%X\n",storageSave.sysDat.wifiMainSw);
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            return;
        } 
        memset((uint8_t *)&noteNid, 0, sizeof(noteNid));
        memset((uint8_t *)&noteMediaState, 0, sizeof(noteMediaState));
        faceIdCtrl.enableNetworkBack = 1;
        Menu_Face_PowerOnStart(0); //给人脸模块上电
                                  //设置0号键闪动
        break;
    case enum_Menu_BleAddFingerFail:
        DEBUG_LOG_OUT("enter enum_Menu_BleAddFingerFail\n");
        pBleNewAddFingerAck->ack = eBleAddFingerAckFail;
		HAL_Comm_BleSendDir(eBleNewAddFinger);
        break;
	case enum_Menu_TestError:
		DEBUG_LOG_OUT("enter enum_Menu_TestError\n");
		st_InputInfo.modeuleFail.Ir = 1;
		st_InputInfo.modeuleFail.hall = 1;
        break;
    default:
        break;
    }
}
#endif
/************************************Code End******************************************/
