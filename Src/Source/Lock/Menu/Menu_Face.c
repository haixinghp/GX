/***************************************************************************************
**文件名:     Menu_Face.c
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
#include "HAL_FaceId.h"
#include "HAL_Storage.h"
#include "HAL_Comm.h"
#include "HAL_RTC.h"
#include "HAL_Voice.h"
#include "HAL_Finger.h"
#if MENU_MODULE_EN
//#if  Menu_FACE_MODULE_EN
//#warning Menu_FACE模块使能!
/*******************************宏定义***************************/
//#define MENU_FACE_RUN_TIME_MAX (20)                                //运行时间
#define WAIT_CONN_OVER_TIME (20000 / MENU_FACE_RUN_TIME_MAX)       //联网超时时间
#define WAIT_BELL_CHECK_OVER_TIME (15000 / MENU_FACE_RUN_TIME_MAX) //定时检查时间

#define WAIT_BELL_CAN_OUT_OVER_TIME (20 * 50 * 8)               //可退出时间
#define WAIT_CAPTURE_OVER_TIME (30000 / MENU_FACE_RUN_TIME_MAX) //抓拍超时时间
#define WAIT_OTA_OVER_TIME (60000 / MENU_FACE_RUN_TIME_MAX)     //抓拍超时时间
/*************************.C变量结构声明***********************/
typedef enum
{
    eMenuFaceDelChkId,   //查询存储的人脸,并判断是否是客人指纹
    eMenuFaceDelCmdSend, //删除命令发送
    eMenuFaceDelWait,    //等待删除完成
    eMenuFaceDelOk,      //成功
    eMenuFaceDelFail,    //失败
} TYPEe_MenuFaceDel;
typedef struct
{
    INT32U runTime;                  //运行时间
    TYPEe_MenuFaceDel FaceDelStatus; //运行状态
    INT16U userId;                   //用户ID
} TYPEs_MenuFaceCtrl;
/*************************私有全局变量*************************/
static TYPEs_MenuFaceCtrl menuFaceCtrl = {
    .runTime = 0,
    .FaceDelStatus = eMenuFaceDelChkId,
    .userId = 0,
};

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Menu_Face_VerifyFail
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static INT8S Menu_Face_VerifyFail()
{
    DEBUG_LOG_OUT("face error\n");
    if (MSG_FACTMEM_REG_STATUS_OK != storageSave.sysDat.localAdminRegFlag) //演示模式下直接开门
    {
        Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
        fn_JumpMenu(enum_Menu_FaceVerifyOk);     //人脸验证成功
        return 0;
    }
    Menu_AddError(&st_InputInfo.errorCnt, eUserTypeFace); //错误次数增加
    DEBUG_LOG_OUT("fst_InputInfo.errorCnt.Face:%d\n", st_InputInfo.errorCnt.Face);
    st_InputInfo.faceVerifyCnt++;
    if ((0 != Menu_ChkError(&st_InputInfo.errorCnt, eErrorCheckTimeNo)))
    {
        fn_JumpMenu(enum_Menu_FaceVerifyFail); //人脸验证失败
        return -1;
    }
    if (((-1 == Menu_AppRegChk()) && (0 == storageSave.sysDat.pwdSetFlag)) ||
            ((eCombineTypeAND == Menu_CheckCombine())))
    {
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyRetry);\n");
        fn_JumpMenu(enum_Menu_FaceVerifyRetry);
    }
    else if (storageSave.sysDat.allFinger != storageSave.sysDat.leaveFinger) //检查指纹
    {
        if (st_InputInfo.faceVerifyCnt >= 3)
        {
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyFail);\n");
            fn_JumpMenu(enum_Menu_FaceVerifyFail); //人脸验证失败
        }
        else
        {
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyRetry);1\n");
            fn_JumpMenu(enum_Menu_FaceVerifyRetry); //重新验证人脸
        }
    }
    else if ((storageSave.sysDat.allFinger == storageSave.sysDat.leaveFinger) &&
             (0 != storageSave.sysDat.pwdSetFlag)) //没有指纹，有密码的情况下
    {
        if (st_InputInfo.faceVerifyCnt >= 3)
        {
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyFail);2\n");
            fn_JumpMenu(enum_Menu_FaceVerifyFail); //人脸验证失败
        }
        else
        {
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyRetry);2\n");
            fn_JumpMenu(enum_Menu_FaceVerifyRetry); //重新验证人脸
        }
    }
    return 0;
}
/***************************************************************************************
**函数名:       Menu_Face_VerifyOk
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static INT8S Menu_Face_VerifyOk()
{
    uint16_t userID = 0;
    uint8_t unlockStatus = 0;
    struct tm timeStartTmp;
    struct tm timeStopTmp;
    if (MSG_FACTMEM_REG_STATUS_NONE == storageSave.sysDat.localAdminRegFlag) //如果在演示模式，直接开门
    {
        fn_JumpMenu(enum_Menu_FaceVerifyOk); //人脸验证成功
        return 0;                            //验证通过
    }
    DEBUG_LOG_OUT("replyResult.result:%X\n",replyResult.result);
    switch (replyResult.result) //校对ID号
    {
    case FACE_ID_MR_SUCCESS:
        userID = (msgReplyVerifyData.userIdHeb << 8) + msgReplyVerifyData.userIdLeb; //获取用户ID
        storageSave.UserDat.group = userID;
        unlockStatus = msgReplyVerifyData.unlockStatus; //获取劫持状态
        DEBUG_LOG_OUT("unlockStatus %X\n",unlockStatus);
        DEBUG_LOG_OUT("storageSave.UserDat.group:%X\n",storageSave.UserDat.group);
        if (-1 != HAL_Storage_MatchId(0,
                                      MEM_FACE_ALL,
                                      MSG_FACE_REG_MEM_ST,
                                      MSG_FACE_REG_SIZE,
                                      &storageSave.UserDat,
                                      eMatchTypeGroup))
        {
            if (-1 == Menu_AppRegChk()) //未注册
            {
                DEBUG_LOG_OUT("-1 == Menu_AppRegChk()\n");
                Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
                st_InputInfo.userID = userID;
                st_InputInfo.userIdTmp = BSWAP_16(userID);
                Menu_RecordLockLog(eLogTypeOpenByFace,
                                   (uint8_t *)&(st_InputInfo.userIdTmp),
                                   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                fn_JumpMenu(enum_Menu_FaceVerifyOk);                //人脸验证成功
            }
            else
            {
                DEBUG_LOG_OUT("system have reg\n");
                DEBUG_LOG_OUT("pageID :%x\n", storageSave.UserDat.pageID);
                DEBUG_LOG_OUT("TimeVaild :%x\n", storageSave.UserDat.TimeVaild.fig);
                DEBUG_LOG_OUT_HEX((uint8_t *)&storageSave.UserDat.TimeVaild.str, sizeof(storageSave.UserDat.TimeVaild.str));
                DEBUG_LOG_OUT_HEX((uint8_t *)&storageSave.UserDat.TimeVaild.stp, sizeof(storageSave.UserDat.TimeVaild.stp));
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
                        DEBUG_LOG_OUT("storageSave.sysDat.openLockMode:%X\n", storageSave.sysDat.openLockMode);

                        if (eCombineTypeAND == Menu_CheckCombine()) //与模式
                        {
                            DEBUG_LOG_OUT("and mode\n");
                            if (0 == st_InputInfo.faceVerifySuccessFlag)
                            {
                                st_InputInfo.faceVerifySuccessFlag = 1; //人脸验证通过
                                DEBUG_LOG_OUT("and mode\n");
                                if(1 == storageSave.UserDat.kindred)
                                {
                                    wifiSendParm.pageId[0] |= MENU_UNLOCK_ATTR_KINDNESS;
                                }
                                if(1 == storageSave.UserDat.sos)
                                {
                                    wifiSendParm.pageId[0] |= MENU_UNLOCK_ATTR_SOS;
                                }
                                if(0xCC == unlockStatus)
                                {
                                    DEBUG_LOG_OUT("0xCC SOS\n");
                                    wifiSendParm.pageId[0] |= MENU_UNLOCK_ATTR_SOS;
                                }
                                wifiSendParm.pageId[1] =(uint8_t)storageSave.UserDat.pageID;
                                if (eCombineOpenFinger == (eCombineOpenFinger & storageSave.sysDat.openLockMode))
                                {
                                    wifiSendParm.unlockType = eWifiunlockTypeFaceAndFinger;
                                    DEBUG_LOG_OUT("please enter finger\n");
                                    HAL_VoicePlayDir(eVoiceDirPleaseEnterFinger); //提示输入指纹
                                    HAL_Finger_VerifyStart();                     //打开指纹
                                    return 1;
                                }
                                else if (eCombineOpenPwd == (eCombineOpenPwd & storageSave.sysDat.openLockMode))
                                {
                                    wifiSendParm.unlockType = eWifiunlockTypeFaceAndPwd;
                                    DEBUG_LOG_OUT("please enter pwd\n");
                                    HAL_VoicePlayDir(eVoiceDirPleaseEnterPwd); //提示输入密码
                                    return 1;
                                }
                            }
                        }
                        DEBUG_LOG_OUT("pass all\n");
                        Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
                        st_InputInfo.userID = userID;
                        st_InputInfo.userIdTmp = BSWAP_16(userID);
                        if(0xCC == unlockStatus)
                        {
                            DEBUG_LOG_OUT("0xCC SOS\n");
                            wifiSendParm.limt |= MENU_UNLOCK_ATTR_SOS;
                        }

                        Menu_RecordLockLog(eLogTypeOpenByFace,
                                           (uint8_t *)&(st_InputInfo.userIdTmp),
                                           sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                        fn_JumpMenu(enum_Menu_FaceVerifyOk);                //人脸验证成功
                    }
                    else //时效过期
                    {
                        DEBUG_LOG_OUT("TimeVaild over\n");
                        fn_JumpMenu(enum_Menu_FaceVerifyFail); //人脸验证失败
                    }
                }
                else //未开启时效
                {
                    DEBUG_LOG_OUT("TimeVaild.fig error\n");
                    Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
                    st_InputInfo.userID = userID;
                    st_InputInfo.userIdTmp = BSWAP_16(userID);
                    Menu_RecordLockLog(eLogTypeOpenByFace,
                                       (uint8_t *)&(st_InputInfo.userIdTmp),
                                       sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                    fn_JumpMenu(enum_Menu_FaceVerifyOk);                //人脸验证成功
                }
            }
        }
        else
        {
            Menu_Face_VerifyFail();
        }
    default:
        break;
    }
    return 0;
}

/***************************************************************************************
**函数名:       Menu_Face_AddFaceIng
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_AddFaceIng(void)
{
    uint16_t userID = 0;
    switch (HAL_FaceId_AddUser()) //添加用户流程
    {
    case eFaceIdAddNone:
        break; //空闲
    case eFaceIdAddReady:
        break; //准备开始
    case eFaceIdAddMiddle:
        //			fn_JumpMenu(enum_Menu_AddFaceStep1);
        break; //添加人脸中间
    case eFaceIdAddMiddleReply:
        break; //等待添加人脸中间完成
    case eFaceIdAddUP:
        fn_JumpMenu(enum_Menu_AddFaceStep2);
        break; //添加人脸上
    case eFaceIdAddUPReply:
        break; //等待添加人脸上完成
    case eFaceIdAddDown:
        fn_JumpMenu(enum_Menu_AddFaceStep3);
        break; //添加人脸下
    case eFaceIdAddDownReply:
        break; //等待添加人脸下完成
    case eFaceIdAddRight:
        fn_JumpMenu(enum_Menu_AddFaceStep4);
        break; //添加人脸左
    case eFaceIdAddRightReply:
        break; //等待添加人脸左完成
    case eFaceIdAddLeft:
        fn_JumpMenu(enum_Menu_AddFaceStep5);
        break; //添加人脸右
    case eFaceIdAddLeftReply:
        break; //等待添加人脸右完成
    case eFaceIdAddOk:
        switch (replyResult.result)
        {
        case FACE_ID_MR_SUCCESS:                                                         //接收成功
            //这里是否需要确认各个方向的结果
            userID = (msgReplyEnrollData.userIdHeb << 8) + msgReplyEnrollData.userIdLeb; //获取数据
            DEBUG_LOG_OUT("face userID:%d\n", userID);
#if defined(SYSTEM_STORAGE_EN)
            st_InputInfo.userType = eUserTypeFace; //获取类型
            st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
                                  MEM_FACE_ALL,
                                  MSG_FACE_REG_MEM_ST,
                                  MSG_FACE_REG_SIZE); //查询空位
            DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
            if (0XFFFF == st_InputInfo.userID)
            {
                //没有找到空闲
                //保存失败
                //菜单跳转
                fn_JumpMenu(enum_Menu_AddAdminFaceFull); //已满
            }
            else
            {
                //保存用户数据
                storageSave.sysDat.leaveNumFaceApp--;                 //剩余数量减1
                if (MSG_FACTMEM_LIM_ADMIN == storageSave.UserDat.Lim) //更新本地管理人脸数量
                {
                    storageSave.sysDat.localAdminNumFace++;
                }
                if (MSG_FACTMEM_LIM_GENERAL == storageSave.UserDat.Lim) //更新本地用户人脸数量
                {
                    storageSave.sysDat.localUserNumFace++;
                }
                DEBUG_LOG_OUT("enter enum_Menu_AddFaceUserOk\n");
                if (MSG_FACTMEM_LIM_ADMIN == storageSave.UserDat.Lim)
                {
                    if (MSG_FACTMEM_REG_STATUS_ING == storageSave.sysDat.localAdminRegFlag) //系统处于注册中
                    {
                        storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_OK; //注册成功
                        DEBUG_LOG_OUT("save reg flag Ok\n");
                    }
                }
                HAL_Storage_WriteSysData(&storageSave.sysDat);                              //写入系统参数
                memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
                storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;                           //存在标志
                //根据菜单添加的类型选择管理员还是普通
                storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN;                            //管理员
                storageSave.UserDat.group = userID;                                         //保存用户ID
                storageSave.UserDat.pageID = st_InputInfo.userID;
                HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
                                           MSG_FACE_REG_SIZE,
                                           storageSave.UserDat.pageID,
                                           &storageSave.UserDat); //保存用户数据
                st_InputInfo.userIdTmp = BSWAP_16(userID);
                Menu_RecordLockLog(eLogTypeAddFace,
                                   (uint8_t *)&(st_InputInfo.userIdTmp),
                                   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                fn_JumpMenu(enum_Menu_AddFaceUserOk);               //保存成功,跳转菜单
                DEBUG_LOG_OUT("enum_Menu_AddFaceUserOk");
            }
#endif // SYSTEM_STORAGE_EN
            break;
        default:
            fn_JumpMenu(enum_Menu_AddFaceUserFail); //保存失败
            break;
        }
        break; //完成
    case eFaceIdAddFail:
        fn_JumpMenu(enum_Menu_AddFaceUserFail);
        break; //失败
    case eFaceIdAddTimeOver:
        fn_JumpMenu(enum_Menu_AddFaceUserFail);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleAddFaceIng
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleAddFaceIng(void)
{
    uint16_t userID = 0;
    switch (HAL_FaceId_AddUser()) //添加用户流程
    {
    case eFaceIdAddNone:
        break; //空闲
    case eFaceIdAddReady:
        break; //准备开始
    case eFaceIdAddMiddle:
        break; //添加人脸中间
    case eFaceIdAddMiddleReply:
        break; //等待添加人脸中间完成
    case eFaceIdAddUP:
        pBleAddFaceAck->ack = eBleAddFaceAckIng;
        pBleAddFaceAck->add.addIng.cnt = eBleAddFaceDirMiddle;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceStep2);
        break; //添加人脸上
    case eFaceIdAddUPReply:
        break; //等待添加人脸上完成
    case eFaceIdAddDown:
        pBleAddFaceAck->ack = eBleAddFaceAckIng;
        pBleAddFaceAck->add.addIng.cnt = eBleAddFaceDirDown;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceStep3);
        break; //添加人脸下
    case eFaceIdAddDownReply:
        break; //等待添加人脸下完成
    case eFaceIdAddRight:
        pBleAddFaceAck->ack = eBleAddFaceAckIng;
        pBleAddFaceAck->add.addIng.cnt = eBleAddFaceDirRight;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceStep4);
        break; //添加人脸左
    case eFaceIdAddRightReply:
        break; //等待添加人脸左完成
    case eFaceIdAddLeft:
        pBleAddFaceAck->ack = eBleAddFaceAckIng;
        pBleAddFaceAck->add.addIng.cnt = eBleAddFaceDirLeft;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceStep5);
        break; //添加人脸右
    case eFaceIdAddLeftReply:
        break; //等待添加人脸右完成
    case eFaceIdAddOk:
        switch (replyResult.result)
        {
        case FACE_ID_MR_SUCCESS:                                                         //接收成功
            //这里是否需要确认各个方向的结果
            userID = (msgReplyEnrollData.userIdHeb << 8) + msgReplyEnrollData.userIdLeb; //获取数据
            DEBUG_LOG_OUT("face userID:%d\n", userID);
#if defined(SYSTEM_STORAGE_EN)
            st_InputInfo.userType = eUserTypeFace; //获取类型
            st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
                                  MEM_FACE_ALL,
                                  MSG_FACE_REG_MEM_ST,
                                  MSG_FACE_REG_SIZE); //查询空位
            DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
            if (0XFFFF == st_InputInfo.userID)
            {
                //没有找到空闲
                //保存失败
                //菜单跳转
                pBleAddFaceAck->ack = eBleAddFaceAckFail;
                HAL_Comm_BleSendDir(eBleFaceReg);
            }
            else
            {
                //保存用户数据
                if (0 != storageSave.sysDat.leaveNumFaceApp)
                {
                    storageSave.sysDat.leaveNumFaceApp--; //剩余数量减1
                }
                storageSave.sysDat.faceNumApp++;
                DEBUG_LOG_OUT("enter enum_Menu_BleAddFaceUserOk\n");
                HAL_Storage_WriteSysData(&storageSave.sysDat);                              //写入系统参数
                memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
                storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;                           //存在标志
                //根据菜单添加的类型选择管理员还是普通
                storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN;                            //管理员
                storageSave.UserDat.group = userID;                                         //保存用户ID
                storageSave.UserDat.pageID = st_InputInfo.userID;
                HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
                                           MSG_FACE_REG_SIZE,
                                           storageSave.UserDat.pageID,
                                           &storageSave.UserDat); //保存用户数据
                pBleAddFaceAck->ack = eBleAddFaceAckSuccess;
                pBleAddFaceAck->add.addSuccess.id = storageSave.UserDat.pageID;
                HAL_Comm_BleSendDir(eBleFaceReg);
                st_InputInfo.userIdTmp = BSWAP_16(userID);
                Menu_RecordLockLog(eLogTypeAddFace,
                                   (uint8_t *)&(st_InputInfo.userIdTmp),
                                   sizeof(st_InputInfo.userIdTmp)); //保存事件记录
                fn_JumpMenu(enum_Menu_BleAddFaceUserConfirm);       //保存成功,跳转菜单
                DEBUG_LOG_OUT("enum_Menu_BleAddFaceUserConfirm");
            }
#endif // SYSTEM_STORAGE_EN
            break;
        default:
            fn_JumpMenu(enum_Menu_BleAddFaceUserFail); //保存失败
            break;
        }
        break; //完成
    case eFaceIdAddFail:
        pBleAddFaceAck->ack = eBleAddFaceAckFail;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceUserFail);
        break; //失败
    case eFaceIdAddTimeOver:
        pBleAddFaceAck->ack = eBleAddFaceAckFail;
        HAL_Comm_BleSendDir(eBleFaceReg);
        fn_JumpMenu(enum_Menu_BleAddFaceUserFail);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_Desktop
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_Desktop(void)
{
#if 0
#define USE_OFF_DIR
#endif
    static uint16_t noFaceCnt =0; //无人脸计数器
    static uint16_t tooFarCnt =0; //人脸太远计数器
    static uint16_t tooColseCnt =0; //人脸太近计数器
    static uint16_t occlusioCnt =0; //人脸遮挡计数器
    switch (HAL_FaceId_VerifyUser())
    {
    case eFaceIdVerifyNone:
        break; //空闲
    case eFaceIdVerifyReady:
        noFaceCnt = 0;
        break; //准备开始
    case eFaceIdVerifyReply:
        break; //等待回复
    case eFaceIdVerifyOk:
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart Menu_Face_Desktop eFaceIdVerifyOk\n");
#ifdef USE_OFF_DIR
        HAL_FaceId_CmdPowerOffDir();              //人脸直接下电
#else
        HAL_FaceId_CmdPowerOff();
#endif
        HAL_FaceId_VerifyUserStop();
        Menu_Face_VerifyOk();
        break; //验证成功
    case eFaceIdVerifyFail:
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart Menu_Face_Desktop eFaceIdVerifyFail\n");
#ifdef USE_OFF_DIR
        HAL_FaceId_CmdPowerOffDir();              //人脸直接下电
#else
        HAL_FaceId_CmdPowerOff();
#endif
        HAL_FaceId_VerifyUserStop();
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceVerifyFail);\n");
        Menu_Face_VerifyFail();
        break;
    case eFaceIdVerifyTimeOut:
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart Menu_Face_Desktop eFaceIdVerifyTimeOut\n");
//        fn_JumpMenu(enum_Menu_FaceNotDetected); //未检测到人脸
#ifdef USE_OFF_DIR
        HAL_FaceId_CmdPowerOffDir();              //人脸直接下电
#else
        HAL_FaceId_CmdPowerOff();
#endif
        HAL_FaceId_VerifyUserStop();
        DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_FaceNotDetected);\n");
        break; //超时
    default:
        break;
    }
    if(FACE_ID_NID_FACE_STATE == noteNid.nid)
    {
        BSWAP_SAVE_16(noteDataFace.state);
#if 0
        DEBUG_LOG_OUT("FACE_ID_NID_FACE_STATE\n");
        DEBUG_LOG_OUT("noteDataFace.state:%d\n",noteDataFace.state);
#endif
        switch (noteDataFace.state)
        {
        case 1: //未检测到人脸
            noFaceCnt ++;
            if(noFaceCnt > 60)
            {
                noFaceCnt = 0;
                HAL_VoicePlayDir(eVoiceDirFaceNotDetected); //未检测到人脸
                st_MenuItem.u16_KeepTime = 2000; //修改当前工作时间
            }
            break;
        case 6: //人脸距离太远，未能录入
            tooFarCnt ++;
            if (tooFarCnt >30)
            {
                tooFarCnt = 0;
                HAL_VoicePlayDir(eVoiceDirFaceDistanceTooFar); //人脸距离太远
                //人脸距离太远
            }
            break;
        case 7: //人脸距离太近，未能录入
            tooColseCnt ++;
            if (tooColseCnt >30)
            {
                tooColseCnt = 0;
                HAL_VoicePlayDir(eVoiceDirFaceDistanceTooClose);  //人脸距离太近
            }
            break;
        case 8: //眉毛遮挡
        case 9: //眼睛遮挡
        case 10: //脸部遮挡
            occlusioCnt ++;
            if (occlusioCnt >30)
            {
                occlusioCnt = 0;
                HAL_VoicePlayDir(eVoiceDirFaceCovered);  //遮挡
            }
            break;
        default:
            break;
        }
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
        memset((uint8_t *)&noteNid, 0, sizeof(noteNid.nid));
    }

#undef USE_OFF_DIR
}
/***************************************************************************************
**函数名:       Menu_Face_AdminChkVerifyOk
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static INT8S Menu_Face_AdminChkVerifyOk()
{
    uint16_t userID = 0;
    //校对ID号
    switch (replyResult.result)
    {
    case FACE_ID_MR_SUCCESS:
        userID = (msgReplyVerifyData.userIdHeb << 8) + msgReplyVerifyData.userIdLeb; //获取用户ID
        storageSave.UserDat.group = userID;
        if (-1 != HAL_Storage_MatchId(0,
                                      MEM_FINGER_FACE_ADMIN_LOCAL,
                                      MSG_FACE_REG_MEM_ST,
                                      MSG_FACE_REG_SIZE,
                                      &storageSave.UserDat,
                                      eMatchTypeGroup)) //查询管理员人脸
        {
            Menu_ClearError(&st_InputInfo.errorCnt); //清除错误次数
            st_InputInfo.userIdTmp = BSWAP_16(userID);
            Menu_RecordLockLog(eLogTypeEnterAdminFace,
                               (uint8_t *)&(st_InputInfo.userIdTmp),
                               sizeof(st_InputInfo.userIdTmp)); //保存事件记录
            fn_JumpMenu(enum_Menu_Admin);
            DEBUG_LOG_OUT("Menu_Face_PowerOffStart Menu_Face_AdminChkVerifyOk\n");
            Menu_Face_PowerOffStart(); //人脸下电
            HAL_FaceId_VerifyUserStop();
        }
        else
        {
            Menu_AddError(&st_InputInfo.errorCnt, eUserTypeFace);
            fn_JumpMenu(enum_Menu_AdminChkFail); //人脸验证失败
        }
    default:
        break;
    }
    return 0;
}
/***************************************************************************************
**函数名:       Menu_Face_AdminChk
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_AdminChk(void)
{
    switch (HAL_FaceId_VerifyUser())
    {
    case eFaceIdVerifyNone:
        break; //空闲
    case eFaceIdVerifyReady:
        break; //准备开始
    case eFaceIdVerifyReply:
        break; //等待回复
    case eFaceIdVerifyOk:
        Menu_Face_AdminChkVerifyOk();
        break;                                  //验证成功
    case eFaceIdVerifyFail:                     //未识别到人脸
        fn_JumpMenu(enum_Menu_AdminChkFail);    //认证失败
        Menu_Face_PowerOffStart(); //人脸下电
        HAL_FaceId_VerifyUserStop();
        break;
    case eFaceIdVerifyTimeOut:
        DEBUG_LOG_OUT("Menu_Face_AdminChk eFaceIdVerifyTimeOut\n");
        fn_JumpMenu(enum_Menu_FaceNotDetected); //未检测到人脸
        Menu_Face_PowerOffStart();              //人脸下电
        HAL_FaceId_VerifyUserStop();
        break; //超时
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       Menu_Face_DelGeneralFace
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_DelGeneralFace(void)
{
    switch (menuFaceCtrl.FaceDelStatus)
    {
    case eMenuFaceDelChkId:
        memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
        HAL_Storage_GetOneUserReg(MSG_FACE_REG_MEM_ST,
                                  MSG_FACE_REG_SIZE,
                                  menuFaceCtrl.userId,
                                  &storageSave.UserDat);       //读取参数
        if (MSG_FINGERREGMEM_FIG == storageSave.UserDat.vaild) //有效
        {
            if (MSG_FACTMEM_LIM_GENERAL == storageSave.UserDat.Lim) //又是普通
            {
                menuFaceCtrl.FaceDelStatus = eMenuFaceDelCmdSend; //进入删除
                Menu_Face_PowerOnStart(0);
                HAL_FaceId_DelOneUserStart(menuFaceCtrl.userId);
                HAL_Storage_DelOneUserReg(menuFaceCtrl.userId,
                                          MSG_FACE_REG_MEM_ST,
                                          MSG_FACE_REG_SIZE); //删除本地ID
            }
            else
            {
                menuFaceCtrl.userId++; //处理下一个
            }
        }
        else
        {
            menuFaceCtrl.userId++; //处理下一个
        }
        if (menuFaceCtrl.userId > MEM_FACE_ALL)
        {
            st_InputInfo.userIdTmp = BSWAP_16(menuFaceCtrl.userId);
            Menu_RecordLockLog(eLogTypeDelFinger,
                               (uint8_t *)&(st_InputInfo.userIdTmp),
                               sizeof(st_InputInfo.userIdTmp)); //保存事件记录
            storageSave.sysDat.localUserNumFace = 0;            //清空数量
            HAL_Storage_WriteSysData(&storageSave.sysDat);      //保存用户信息
            fn_JumpMenu(enum_Menu_DelGeneralFaceOK);            //全部删除成功
        }
        break;
    case eMenuFaceDelCmdSend:
        Menu_Face_PowerOnStart(0);
        HAL_FaceId_DelOneUserStart(menuFaceCtrl.userId);
        menuFaceCtrl.FaceDelStatus = eMenuFaceDelWait;
        break;
    case eMenuFaceDelWait:
        switch (HAL_FaceId_DelOneUser())
        {
        case eFaceIdDelOneNone:
            break; //空闲
        case eFaceIdDelOneReady:
            break; //准备开始
        case eFaceIdDelOneReply:
            break; //等待回复
        case eFaceIdDelOneOk:
            if (menuFaceCtrl.userId >= MEM_FACE_ALL)
            {
                //全部删除成功
                fn_JumpMenu(enum_Menu_DelGeneralFaceOK); //删除成功
            }
            else
            {
                menuFaceCtrl.userId++;                          //处理下一个
                menuFaceCtrl.FaceDelStatus = eMenuFaceDelChkId; //进入检查
            }
            break; //删除成功
        case eFaceIdDelOneFail:
            fn_JumpMenu(enum_Menu_DelGeneralFaceFail);
            break;
        case eFaceIdDelOneTimeOut:
            fn_JumpMenu(enum_Menu_DelGeneralFaceFail);
            break; //超时
        default:
            break;
        }
        break;

    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_Clear
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_Clear(void)
{
#if 0
#define NO_ERROR
#endif
    static uint8_t clearStep =0;
    if (eEquipResultNone != st_InputInfo.emptyFace)
    {
        return; //已经获得了结果不再执行
    }
    switch (HAL_FaceId_DelAllUser())
    {
    case eFaceIdDelAllNone:
        break; //空闲
    case eFaceIdDelAllReady:
        clearStep = 0;
        break; //准备开始
    case eFaceIdDelAllReply:
        break; //等待回复
    case eFaceIdDelAllOk:
        clearStep = 1;
        DEBUG_LOG_OUT("eFaceIdDelAllOk\n");
        msgDeleteMediaFileData.mediaType = 0;
        msgDeleteMediaFileData.force = 1;
        HAL_FaceId_CmdDealStart(eFaceIdCmdDeleteMediaFileData, 0);
        HAL_FaceId_DelAllUserStop();
#ifdef USE_FACE_AOBI
        Menu_Face_PowerOffStart(); //人脸下电
        faceIdCtrl.enableNetworkBack = 1;  //奥比删除媒体文件需要切换到猫眼。
        Menu_Face_PowerOnStart(0);
#endif

        //全部删除成功
        break; //成功
    case eFaceIdDelAllFail:
        Menu_Face_PowerOffStart(); //人脸下电
        clearStep = 1;
#ifdef NO_ERROR
        st_InputInfo.emptyFace = eEquipResultFail;
#endif
        break; //失败
    case eFaceIdDelAllTimeOut:
        Menu_Face_PowerOffStart(); //人脸下电
        clearStep = 1;
#ifdef NO_ERROR
        st_InputInfo.emptyFace = eEquipResultFail;
#endif
        break; //超时
    default:
        break;
    }
    if(1 == clearStep)
    {
        switch (HAL_FaceId_CmdDeal(eFaceIdCmdDeleteMediaFileData, 0, 100))
        {
        case eFaceIdComNone:
            break; //空闲
        case eFaceIdComReady:
            break; //准备开始
        case eFaceIdComReply:
            break;         //等待回复
        case eFaceIdComOk: //命令完成
            clearStep = 0;
            HAL_FaceId_CmdStop(); //停止此命令
            st_InputInfo.emptyFace = eEquipResultSuccess;
            Menu_Face_PowerOffStart(); //人脸下电
            break; //成功
        case eFaceIdComFail:
            clearStep = 0;
            HAL_FaceId_CmdStop(); //停止此命令
            st_InputInfo.emptyFace = eEquipResultSuccess;
            DEBUG_LOG_OUT("eFaceIdCmdDeleteMediaFileData eFaceIdComFail\n");
            Menu_Face_PowerOffStart(); //人脸下电
            break;
        case eFaceIdComTimeOut:
            clearStep = 0;
            HAL_FaceId_CmdStop(); //停止此命令
            st_InputInfo.emptyFace = eEquipResultSuccess;
            DEBUG_LOG_OUT("eFaceIdCmdDeleteMediaFileData eFaceIdComTimeOut\n");
            Menu_Face_PowerOffStart(); //人脸下电
            break; //超时
        default:
            break;
        }
    }
#undef NO_ERROR
}
/***************************************************************************************
**函数名:       Menu_Face_DelOne
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleAddFaceUserFail(void)
{
    switch (HAL_FaceId_DelOneUser())
    {
    case eFaceIdDelOneNone:
        break; //空闲
    case eFaceIdDelOneReady:
        break; //准备开始
    case eFaceIdDelOneReply:
        break; //等待回复
    case eFaceIdDelOneOk:
        fn_JumpMenu(enum_Menu_APPMode);
        break; //删除成功
    case eFaceIdDelOneFail:
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdDelOneTimeOut:
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_DelOne
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleFaceDelId(void)
{
    switch (HAL_FaceId_DelOneUser())
    {
    case eFaceIdDelOneNone:
        break; //空闲
    case eFaceIdDelOneReady:
        break; //准备开始
    case eFaceIdDelOneReply:
        break; //等待回复
    case eFaceIdDelOneOk:
        DEBUG_LOG_OUT("Menu_Face_BleFaceDelId eFaceIdDelOneOk\n");
        pBleAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDir(eBleFaceDelId);
        fn_JumpMenu(enum_Menu_APPMode);
        break; //删除成功
    case eFaceIdDelOneFail:
        DEBUG_LOG_OUT("Menu_Face_BleFaceDelId eFaceIdDelOneFail\n");
        pBleAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDir(eBleFaceDelId);
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdDelOneTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleFaceDelId eFaceIdDelOneTimeOut\n");
        pBleAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDir(eBleFaceDelId);
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleFaceVersionGet
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:         "ST_SENSEEYE_A-V1.0.127"
****************************************************************************************/
static void Menu_Face_BleFaceVersionGet(void)
{
    switch (HAL_FaceId_GetVersion())
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        break; //等待回复
    case eFaceIdComOk:
        pBleFaceVersionGetAck->ack = eBleAckSuccess;
        memcpy(pBleFaceVersionGetAck->version, msgReplyVersionGet.version, sizeof(pBleFaceVersionGetAck->version)); //填写人脸版本
        DEBUG_LOG_OUT("Menu_Face_BleFaceVersionGet eFaceIdComOk\n");
        DEBUG_LOG_OUT("version:\n");
        DEBUG_LOG_OUT_HEX(pBleFaceVersionGetAck->version, sizeof(pBleFaceVersionGetAck->version));
        HAL_Comm_BleSendDir(eBleFaceVersionGet);
        Menu_Face_PowerOffStart();
        fn_JumpMenu(enum_Menu_APPMode);
        break; //删除成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleFaceVersionGet eFaceIdComFail\n");
        pBleFaceVersionGetAck->ack = eBleAckSuccess;
        memcpy(pBleFaceVersionGetAck->version, msgReplyVersionGet.version, sizeof(pBleFaceVersionGetAck->version)); //填写人脸版本
        HAL_Comm_BleSendDir(eBleFaceVersionGet);
        Menu_Face_PowerOffStart();
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleFaceVersionGet eFaceIdComTimeOut\n");
        pBleFaceVersionGetAck->ack = eBleAckSuccess;
        memcpy(pBleFaceVersionGetAck->version, msgReplyVersionGet.version, sizeof(pBleFaceVersionGetAck->version)); //填写人脸版本
        HAL_Comm_BleSendDir(eBleFaceVersionGet);
        Menu_Face_PowerOffStart();
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleConfigWifiData
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleConfigWifiData(void)
{
    static uint8_t waitConnFlag = 0; //等待联网标志
    static uint32_t overTime = 0;    //超时计数器
    switch (HAL_FaceId_ConfigWifiData())
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        waitConnFlag = 0;
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_BleConfigWifiData eFaceIdComOk\n");
        memset((uint8_t *)&noteMediaState, 0, sizeof(TYPEs_NoteMediaState));
        waitConnFlag = 1;     //等待联网
        overTime = 0;         //超时时间
#if 0
        Menu_Face_PowerOffStart(); //人脸下电
#endif
        HAL_FaceId_CmdStop(); //停止此命令
        break;                //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleConfigWifiData eFaceIdComFail\n");
        pBleWifiSetParmAck->ack = eBleWifiSetParmAckFail;
        HAL_Comm_BleSendDir(eBleWifiSetParm1);
        Menu_Face_PowerOffStart(); //人脸下电
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleConfigWifiData eFaceIdComTimeOut\n");
        pBleWifiSetParmAck->ack = eBleWifiSetParmAckFail;
        HAL_Comm_BleSendDir(eBleWifiSetParm1);
        Menu_Face_PowerOffStart(); //人脸下电
        fn_JumpMenu(enum_Menu_APPMode);
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
            pBleWifiSetParmAck->ack = eBleWifiSetParmAckSuccess;
            HAL_Comm_BleSendDir(eBleWifiSetParm1);
            fn_JumpMenu(enum_Menu_APPMode);
        }
        overTime++;
        if (overTime >= WAIT_CONN_OVER_TIME)
        {
            waitConnFlag = 0;
            DEBUG_LOG_OUT("Menu_Face_BleConfigWifiData overTime\n");
            overTime = WAIT_CONN_OVER_TIME; //等待联网超时
            pBleWifiSetParmAck->ack = eBleWifiSetParmAckFail;
            HAL_Comm_BleSendDir(eBleWifiSetParm1);
            fn_JumpMenu(enum_Menu_APPMode);
        }
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleAliyunSecretKeySetParm
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleAliyunSecretKeySetParm(void)
{
    switch (HAL_FaceId_SetLinkKey())
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_BleAliyunSecretKeySetParm eFaceIdComOk\n");
        pBleAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDir(eBleAliyunSecretKeySetParm);
        Menu_Face_PowerOffStart(); //人脸下电
        HAL_FaceId_CmdStop(); //停止此命令
        break;                //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleAliyunSecretKeySetParm eFaceIdComFail\n");
        pBleAck->ack = eBleAckFail;
        HAL_Comm_BleSendDir(eBleAliyunSecretKeySetParm);
        Menu_Face_PowerOffStart(); //人脸下电
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleAliyunSecretKeySetParm eFaceIdComTimeOut\n");
        pBleAck->ack = eBleAckFail;
        HAL_Comm_BleSendDir(eBleAliyunSecretKeySetParm);
        Menu_Face_PowerOffStart(); //人脸下电
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       Menu_Face_BleCatEyeSN
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleCatEyeSN(void)
{
    switch (HAL_FaceId_GetDeviceName())
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_BleCatEyeSN eFaceIdComOk\n");
        pBleCatEyeSNAck->ack = eBleAckSuccess;
        memcpy((uint8_t *)&pBleCatEyeSNAck->sn,
               (uint8_t *)&msgReplyDeviceName.name,
               sizeof(pBleCatEyeSNAck->sn));

        DEBUG_LOG_OUT_HEX(pBleCatEyeSNAck->sn,sizeof(pBleCatEyeSNAck->sn));
        HAL_Comm_BleSendDir(eBleCatEyeSN);
        HAL_FaceId_CmdStop(); //停止此命令
        Menu_Face_PowerOffStart(); //人脸下电
        break;                //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleCatEyeSN eFaceIdComFail\n");
        pBleCatEyeSNAck->ack = eBleAckFail;
        memset((uint8_t *)&pBleCatEyeSNAck->sn,
               0,
               sizeof(pBleCatEyeSNAck->sn));
        HAL_Comm_BleSendDir(eBleCatEyeSN);
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleCatEyeSN eFaceIdComTimeOut\n");
        pBleCatEyeSNAck->ack = eBleAckFail;
        memset((uint8_t *)&pBleCatEyeSNAck->sn,
               0,
               sizeof(pBleCatEyeSNAck->sn));
        HAL_Comm_BleSendDir(eBleCatEyeSN);
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_Face_Bell
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_Bell(void)
{
    static uint8_t callEndFlag =0;
    if((MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw)
            || (0 != Menu_LowPowerChk()))
    {
        return;
    }
    wifiSendParm.timerCnt++;
    switch (HAL_FaceId_CmdDeal(eFaceIdCmdVideoCallData, 0, 1000))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        callEndFlag =0;
        break; //准备开始
    case eFaceIdComReply:
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_Bell eFaceIdComOk\n");
        HAL_FaceId_CmdStop(); //停止此命令
        callEndFlag = 1;
        break;                //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_Bell eFaceIdComFail\n");
        HAL_FaceId_CmdStop(); //停止此命令
        callEndFlag = 1;
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_Bell eFaceIdComTimeOut\n");
        HAL_FaceId_CmdStop(); //停止此命令
        callEndFlag = 1;
        break;                //超时
    default:
        break;
    }
    if(1 == callEndFlag)
    {
        if (wifiSendParm.timerCnt >= WAIT_BELL_CONNECT_MIN_TIME)
        {
            wifiSendParm.connectWaitOver = eConnectWaitOverYes;
        }
        if (wifiSendParm.timerCnt >= WAIT_BELL_CHECK_OVER_TIME) //每隔一段时间查询一次工作状态
        {
            wifiSendParm.timerCnt = 0;
            HAL_FaceId_CmdDealStart(eFaceIdCmdGetModuleStatus, 0);
            DEBUG_LOG_OUT("HAL_FaceId_CmdDealStart(eFaceIdCmdGetModuleStatus,0);\n");
        }
        switch (HAL_FaceId_CmdDeal(eFaceIdCmdGetModuleStatus, 0, 100))
        {
        case eFaceIdComNone:
            break; //空闲
        case eFaceIdComReady:
            break; //准备开始
        case eFaceIdComReply:
            break;         //等待回复
        case eFaceIdComOk: //命令完成
            DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComOk\n");
            HAL_FaceId_CmdStop(); //停止此命令
            if ((eMediaStateError == mMsgReplyGetModuleStatus.mediaState) ||
                    (eMediaStateIdle == mMsgReplyGetModuleStatus.mediaState))
            {
                fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
            }
            break; //成功
        case eFaceIdComFail:
            DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComFail\n");
            HAL_FaceId_CmdStop();                 //停止此命令
            fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
            break;
        case eFaceIdComTimeOut:
            DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComTimeOut\n");
            HAL_FaceId_CmdStop();                 //停止此命令
#if 1
            fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
#endif
            break;                                //超时
        default:
            break;
        }
        if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
        {
            if (eMediaStateError == noteMediaState.mediaState)
            {
                DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateError \n");
                fn_JumpMenu(enum_Menu_BellCanNotConn); //门铃无法连接
            }
            else if (eMediaStateIdle == noteMediaState.mediaState)
            {
                DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateIdle \n");
                fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
            }
            memset((uint8_t *)&noteNid, 0, sizeof(noteNid.nid));
            memset((uint8_t *)&noteMediaState, 0, sizeof(noteMediaState));
        }
    }

}
/***************************************************************************************
**函数名:       Menu_Face_BleShangTangCatEyeSingleDouble
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleShangTangCatEyeSingleDouble(void)
{
    if (1 == st_InputInfo.modeuleFail.Face) //人脸没插的情况
    {
        DEBUG_LOG_OUT("1 == st_InputInfo.modeuleFail.Face\n");
        fn_JumpMenu(enum_Menu_APPMode);
    }
    if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
    {
        if (eMediaStateError == noteMediaState.mediaState) //空闲或者超时
        {
            DEBUG_LOG_OUT("Menu_Face_BleShangTangCatEyeSingleDouble eMediaStateError\n");
            //设置重连标志
            fn_JumpMenu(enum_Menu_APPMode); //失败,回到app模式
        }
        if ((eMediaStateWaiting == noteMediaState.mediaState)) //联网成功
        {
            DEBUG_LOG_OUT("Menu_Face_BleShangTangCatEyeSingleDouble eMediaStateWaiting \n");
            fn_JumpMenu(enum_Menu_APPMode); //成功，回到app模式
        }
    }
}
/***************************************************************************************
**函数名:       Menu_Face_WifiConnect
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_WifiConnect(void)
{
    if (1 == st_InputInfo.modeuleFail.Face) //人脸没插的情况
    {
        DEBUG_LOG_OUT("1 == st_InputInfo.modeuleFail.Face\n");
        fn_JumpMenu(enum_Menu_Desktop);
    }
    if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
    {
        if (eMediaStateError == noteMediaState.mediaState) //空闲或者超时
        {
            DEBUG_LOG_OUT("Menu_Face_WifiConnect eMediaStateError\n");
            //设置重连标志
            fn_JumpMenu(enum_Menu_Desktop); //成功
        }
        if ((eMediaStateWaiting == noteMediaState.mediaState)) //联网成功
        {
            DEBUG_LOG_OUT("Menu_Face_WifiConnect eMediaStateWaiting \n");
#if 1
            Menu_Face_PowerOffStart();
            fn_JumpMenu(enum_Menu_Desktop);
#endif
        }
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleR8CatEyeOTA
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleR8CatEyeOTA(void)
{
    static uint8_t waitOtaIngFlag = 0;
    static uint32_t overTime = 0;
    switch (HAL_FaceId_CmdDeal(eFaceIdCmdStartOTA, 0, 100))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        waitOtaIngFlag = 0;
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_BleR8CatEyeOTA eFaceIdComOk\n");
        HAL_FaceId_CmdStop(); //停止此命令
        waitOtaIngFlag = 1;
        break; //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleR8CatEyeOTA eFaceIdComFail\n");
        HAL_FaceId_CmdStop(); //停止此命令
        pBleR8CatEyeOTAAck->ack = eFaceOTAAckFail;
        pBleR8CatEyeOTAAck->faceOTAing.status = 0;
        pBleR8CatEyeOTAAck->faceOTAing.error = 0;
        pBleR8CatEyeOTAAck->faceOTAing.progress = 0;
        HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleR8CatEyeOTA eFaceIdComTimeOut\n");
        HAL_FaceId_CmdStop(); //停止此命令
        waitOtaIngFlag = 1;
        break; //超时
    default:
        break;
    }
    if (1 == waitOtaIngFlag)
    {
        overTime++;
        if (FACE_ID_NID_OTA_STATE == noteNid.nid)
        {
            if (1 == noteOTAState.state) //上报进度，刷新时间
            {
                overTime = 0;
                pBleR8CatEyeOTAAck->ack = eFaceOTAAckUpdataing;
                pBleR8CatEyeOTAAck->faceOTAing.status = noteOTAState.state;
                pBleR8CatEyeOTAAck->faceOTAing.error = noteOTAState.error;
                pBleR8CatEyeOTAAck->faceOTAing.progress = noteOTAState.progress;
                HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
            }
            else if (3 == noteOTAState.state) //猫眼报错退出
            {
                overTime = 0;
                pBleR8CatEyeOTAAck->ack = eFaceOTAAckFail;
                pBleR8CatEyeOTAAck->faceOTAing.status = noteOTAState.state;
                pBleR8CatEyeOTAAck->faceOTAing.error = noteOTAState.error;
                pBleR8CatEyeOTAAck->faceOTAing.progress = noteOTAState.progress;
                HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
                waitOtaIngFlag = 0; //退出
            }
            if (100 == noteOTAState.progress) //进度完成
            {
                overTime = 0;
                pBleR8CatEyeOTAAck->ack = eFaceOTAAckUpdataSuccess;
                HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
                waitOtaIngFlag = 0; //退出
            }
        }
        if (overTime >= WAIT_OTA_OVER_TIME) //超时
        {
            pBleR8CatEyeOTAAck->ack = eFaceOTAAckTimeOver;
            pBleR8CatEyeOTAAck->faceOTAing.status = 0;
            pBleR8CatEyeOTAAck->faceOTAing.error = 0;
            pBleR8CatEyeOTAAck->faceOTAing.progress = 0;
            HAL_Comm_BleSendDir(eBleR8CatEyeOTA);
            waitOtaIngFlag = 0; //退出
        }
    }
}

/***************************************************************************************
**函数名:       Menu_CatEyeConn
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_CatEyeConn(void)
{
    //电压低
    wifiSendParm.timerCnt++;
    if (wifiSendParm.timerCnt >= WAIT_BELL_CONNECT_MIN_TIME)
    {
        wifiSendParm.connectWaitOver = eConnectWaitOverYes;
    }
    if(eConnectWaitOverYes != wifiSendParm.connectWaitOver) //给足够的时间，不然不好联网
    {
        return;
    }
    if (wifiSendParm.timerCnt >= WAIT_BELL_CHECK_OVER_TIME) //每隔一段时间查询一次工作状态
    {
        wifiSendParm.timerCnt = 0;
        HAL_FaceId_CmdDealStart(eFaceIdCmdGetModuleStatus, 0);
        DEBUG_LOG_OUT("HAL_FaceId_CmdDealStart(eFaceIdCmdGetModuleStatus,0);\n");
    }

    switch (HAL_FaceId_CmdDeal(eFaceIdCmdGetModuleStatus, 0, 100))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComOk\n");
        HAL_FaceId_CmdStop(); //停止此命令
        if ((eMediaStateError == mMsgReplyGetModuleStatus.mediaState) ||
                (eMediaStateIdle == mMsgReplyGetModuleStatus.mediaState))
        {
            fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
        }
        break; //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComFail\n");
        HAL_FaceId_CmdStop();                 //停止此命令
        fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("eFaceIdCmdGetModuleStatus eFaceIdComTimeOut\n");
        HAL_FaceId_CmdStop();                 //停止此命令
        fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
        break;                                //超时
    default:
        break;
    }

    if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
    {
        if (eMediaStateError == noteMediaState.mediaState)
        {

            DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateError \n");
            fn_JumpMenu(enum_Menu_BellCanNotConn); //门铃无法连接
        }
        else if (eMediaStateIdle == noteMediaState.mediaState)
        {
            DEBUG_LOG_OUT("Menu_VideoCallData eMediaStateIdle \n");
            fn_JumpMenu(enum_Menu_CatEyeDisConn); //进入休眠
        }
        memset((uint8_t *)&noteNid, 0, sizeof(noteNid.nid));
        memset((uint8_t *)&noteMediaState, 0, sizeof(noteMediaState));
    }
}
/***************************************************************************************
**函数名:       Menu_Face_BleModifFaceSafetyLevel
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
static void Menu_Face_BleModifFaceSafetyLevel(void)
{
    switch (HAL_FaceId_CmdDeal(eFaceIdCmdSetThresholdLevel, 0, 100))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_Face_BleModifFaceSafetyLevel eFaceIdComOk\n");
        HAL_FaceId_CmdStop(); //停止此命令
        pBleAck->ack = eBleAckSuccess;
        HAL_Comm_BleSendDirWithLen(eBleModifFaceSafetyLevel, sizeof(TYPEs_BleAck));
        fn_JumpMenu(enum_Menu_APPMode);
        break; //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_Face_BleModifFaceSafetyLevel eFaceIdComFail\n");
        HAL_FaceId_CmdStop(); //停止此命令
        pBleAck->ack = eBleAckFail;
        *(pBleTxData + 1) = replyResult.result;
        HAL_Comm_BleSendDirWithLen(eBleModifFaceSafetyLevel, 2);
        fn_JumpMenu(enum_Menu_APPMode);
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_Face_BleModifFaceSafetyLevel eFaceIdComTimeOut\n");
        pBleAck->ack = eBleAckFail;
        HAL_Comm_BleSendDirWithLen(eBleModifFaceSafetyLevel, sizeof(TYPEs_BleAck));
        HAL_FaceId_CmdStop(); //停止此命令
        fn_JumpMenu(enum_Menu_APPMode);
        break; //超时
    default:
        break;
    }
}
#ifdef USE_FACE_WIFI
/***************************************************************************************
**函数名:       Menu_FaceWifiDeal
**功能描述:
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void Menu_FaceWifiDeal(void)
{
    static uint8_t waitConnFlag = 0;    //等待联网标志
    static uint32_t overTime = 0;       //超时计数器
    static uint8_t waitCaptureFlag = 0; //等待抓拍标志
#if 1
    if(MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) 	//检查wifi主开关开启
    {
        wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
        return;
    }
#endif
    if(0 != Menu_LowPowerChk()) //低电压不操作
    {
        return ;
    }
    if(0 != st_InputInfo.motorRuningFlag) //电机动作时不操作
    {
        return;
    }
    if((eWifiSendTypeUnlockUpdata == wifiSendParm.sendType) &&
            (eWifiunlockTypeWifiTest != wifiSendParm.unlockType )) //在推送开锁记录的时候，wifi测试不受日志上传开关的限制。
    {
        if(MSG_WIFI_LOGUP_ON_CONFIG != storageSave.sysDat.wifiOpenUpdataSw) //开门推送被关闭
        {
            wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
            return ;
        }
    }
    //重连时不操作。

    switch ( wifiSendParm.wifiSendStatus)
    {
    case eWifiSendStatusNone://wifi空闲不操作
        break;
    case eWifiSendStatusReady:
        switch (wifiSendParm.sendType)
        {
        case eWifiSendTypeUnlockUpdata:
            Menu_WifiLockLogUpdata(); //wifi推送
            break;
        case eWifiSendTypeAlarmTry:
        case eWifiSendTypeAlarmProtection:
        case eWifiSendTypeAlarmPry:
            Menu_WifiCmdAlarmUpdata();
            break;
        default:
            break;
        }
#if 1
        memset((uint8_t *)&msgInitData, 0, sizeof(msgInitData)); //清空参数
        if (0 == storageSave.sysDat.ShangTangCatEyeSingleDouble)
        {
            faceIdCtrl.enableNetworkBack = 2; //不复位，双向模式
        }
        else
        {
            faceIdCtrl.enableNetworkBack = 1; //复位,单向模式
        }
#endif
        Menu_Face_PowerOnStart(0); //人脸上电
        HAL_FaceId_CmdDealStart(eFaceIdCmdDataPassThrough,400); //开始透传
        wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSendData;
        break;
    case eWifiSendStatusWaitSendData:
#if 1
        switch (HAL_FaceId_CmdDeal(eFaceIdCmdDataPassThrough, 0, 500))
        {
        case eFaceIdComNone:
            break; //空闲
        case eFaceIdComReady:
            break; //准备开始
        case eFaceIdComReply:
            waitConnFlag = 0;
            overTime = 0;
            waitCaptureFlag = 0;
            break;         //等待回复
        case eFaceIdComOk: //命令完成
            DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComOk\n");
            waitConnFlag = 1;
            faceIdCtrl.operate.OverTimr = 0;
            faceIdCtrl.com = eFaceIdComCmdTaskIng; //进入任务等待
            wifiSendParm.waitComplete = eWaitCompleteYes;
            break; //成功
        case eFaceIdComFail:
            HAL_FaceId_CmdStop(); //停止此命令
            DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComFail\n");
            wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
            break;
        case eFaceIdComTimeOut:
            HAL_FaceId_CmdStop(); //停止此命令
            DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComTimeOut\n");
            wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
            break; //超时
        default:
            break;
        }
        if (1 == waitConnFlag)
        {
            if (eMediaStateWaiting == noteMediaState.mediaState)
            {
                waitConnFlag = 0;
                DEBUG_LOG_OUT("Menu_FaceWifiDeal eMediaStateWaiting\n");
                if ((eWifiSendTypeAlarmPry == wifiSendParm.sendType) ||
                        (eWifiSendTypeAlarmTry == wifiSendParm.sendType) ||
                        (eWifiSendTypeAlarmActiveDefense == wifiSendParm.sendType))
                {
                    memset((uint8_t *)&noteNid, 0, sizeof(noteNid));
                    memset((uint8_t *)&noteMediaState, 0, sizeof(noteMediaState));
                    wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
                    HAL_FaceId_CmdDealStart(eFaceIdCmdTriggerCaptureData, 0); //进入抓拍
                }
                else
                {
                    wifiSendParm.waitComplete = eWaitCompleteNo;
                    HAL_FaceId_CmdStop(); //停止此命令
                    wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
                }
            }
            overTime++;
            if (overTime >= WAIT_CONN_OVER_TIME)
            {
                wifiSendParm.waitComplete = eWaitCompleteNo;
                waitConnFlag = 0;
                HAL_FaceId_CmdStop(); //停止此命令
                wifiSendParm.wifiSendStatus = eWifiSendStatusNone; //发送停止
            }
        }
#endif
        break;
    default:
        break;
    }
#if 0
    switch (HAL_FaceId_CmdDeal(eFaceIdCmdDataPassThrough, 0, 50))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        break; //准备开始
    case eFaceIdComReply:
        waitConnFlag = 0;
        overTime = 0;
        waitCaptureFlag = 0;
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComOk\n");
        waitConnFlag = 1;
        faceIdCtrl.operate.OverTimr = 0;
        faceIdCtrl.com = eFaceIdComCmdTaskIng; //进入任务等待
        wifiSendParm.waitComplete = eWaitCompleteYes;
        break; //成功
    case eFaceIdComFail:
        HAL_FaceId_CmdStop(); //停止此命令
        DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComFail\n");

        break;
    case eFaceIdComTimeOut:
        HAL_FaceId_CmdStop(); //停止此命令
        DEBUG_LOG_OUT("Menu_FaceWifiDeal eFaceIdComTimeOut\n");

        break; //超时
    default:
        break;
    }
    if (1 == waitConnFlag)
    {
        if (eMediaStateWaiting == noteMediaState.mediaState)
        {
            waitConnFlag = 0;
            DEBUG_LOG_OUT("Menu_FaceWifiDeal eMediaStateWaiting\n");
            if ((eWifiSendTypeAlarmPry == wifiSendParm.sendType) ||
                    (eWifiSendTypeAlarmTry == wifiSendParm.sendType) ||
                    (eWifiSendTypeAlarmActiveDefense == wifiSendParm.sendType))
            {
                memset((uint8_t *)&noteNid, 0, sizeof(noteNid));
                memset((uint8_t *)&noteMediaState, 0, sizeof(noteMediaState));
                HAL_FaceId_CmdDealStart(eFaceIdCmdTriggerCaptureData, 0); //进入抓拍
            }
            else
            {
                wifiSendParm.waitComplete = eWaitCompleteNo;
                HAL_FaceId_CmdStop(); //停止此命令
            }
        }
        overTime++;
        if (overTime >= WAIT_CONN_OVER_TIME)
        {
            wifiSendParm.waitComplete = eWaitCompleteNo;
            waitConnFlag = 0;
            HAL_FaceId_CmdStop(); //停止此命令
        }
    }
#endif
}
#endif


/***************************************************************************************
**函数名:       Menu_FaceId_WaitComplete
**功能描述:     等待人脸完成任务
**输入参数:     --
**输出参数:
int8_t
0 空闲
-1 忙碌
**备注:
****************************************************************************************/
int8_t Menu_FaceId_WaitComplete(void)
{
    if (eWaitCompleteNo == wifiSendParm.waitComplete)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

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
    switch (HAL_FaceId_CmdDeal(eFaceIdCmdTriggerCaptureData, 0, 100))
    {
    case eFaceIdComNone:
        break; //空闲
    case eFaceIdComReady:
        memset((uint8_t *)&noteMediaState, 0, sizeof(TYPEs_NoteMediaState));
        break; //准备开始
    case eFaceIdComReply:
        captureFlag = 0;
        break;         //等待回复
    case eFaceIdComOk: //命令完成
        DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComOk\n");
        captureFlag = 1;
        overTime = 0;
        faceIdCtrl.operate.OverTimr = 0;
        faceIdCtrl.com = eFaceIdComCmdTaskIng; //进入任务等待
        wifiSendParm.waitComplete = eWaitCompleteYes;
        break; //成功
    case eFaceIdComFail:
        DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComFail\n");
        captureFlag = 1;
        HAL_FaceId_CmdStop(); //停止此命令
        break;
    case eFaceIdComTimeOut:
        DEBUG_LOG_OUT("Menu_TriggerCaptureData eFaceIdComTimeOut\n");
        captureFlag = 1;
        HAL_FaceId_CmdStop(); //停止此命令
        break;                //超时
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
            wifiSendParm.waitComplete = eWaitCompleteNo;
            HAL_FaceId_CmdStop();
        }
        if (FACE_ID_NID_MEDIA_STATE == noteNid.nid)
        {
            if ((eMediaStateIdle == noteMediaState.mediaState) ||
                    (eMediaStateError == noteMediaState.mediaState)) //空闲或者超时
            {
                DEBUG_LOG_OUT("Menu_TriggerCaptureData eMediaStateError eMediaStateIdle \n");
                captureFlag = 0;
                wifiSendParm.catEyeStatus = eCatEyeStatusNone;
                wifiSendParm.waitComplete = eWaitCompleteNo;

                HAL_FaceId_CmdStop();
            }
        }
    }
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_Face_PowerOnStart
**功能描述:     人脸启动
**输入参数:     uint8_t mode 0,等待休眠完成；1直接重新启动
**输出参数:     --
**备注:
进入人脸demo模式:
1、未注册
2、注册了，但是没有添加人脸。
使用商汤猫眼：
联网标志1、表示联网会重启模块 2、表示不重启
****************************************************************************************/
void Menu_Face_PowerOnStart(uint8_t mode)
{
    uint32_t tickTmp;
    HAL_FaceId_CmdPowerOn(mode);                                 //上电
    st_InputInfo.modeuleFail.Face = 0;                       //错误检查
#if 0
    memset((uint8_t *)&msgInitData, 0, sizeof(msgInitData)); //清空参数
    if (0 == storageSave.sysDat.ShangTangCatEyeSingleDouble)
    {
        msgInitData.enableNetwork = 2; //不复位，双向模式
    }
    else
    {
        msgInitData.enableNetwork = 1; //复位,单向模式
    }
#endif
    msgInitData.waitingTime = 10;
    if(0 != faceIdCtrl.enableNetworkBack) //联网
    {
        msgInitData.waitingTime =10; //超时时间
        msgInitData.reserved[2] = 95; //有效比例
    }
    else
    {
        msgInitData.waitingTime = 0;
        memset( msgInitData.reserved,0,sizeof(msgInitData.reserved));
    }
    tickTmp = HAL_RTC_GetTick();
    BSWAP_SAVE_32(tickTmp); //大小端转换
    memcpy((uint8_t *)&msgInitData.crtTime,
           (uint8_t *)&tickTmp,
           sizeof(msgInitData.crtTime)); //填写RTC时间
    DEBUG_LOG_OUT_HEX(msgInitData.crtTime,sizeof(msgInitData.crtTime));
#ifdef USE_FACE_SHANGTANG //使用商汤人脸 
    if ((MSG_FACTMEM_REG_STATUS_OK != storageSave.sysDat.localAdminRegFlag) ||
            ((0 == Menu_AppRegChk()) && (MEM_FACE_ALL_ONLINE == storageSave.sysDat.leaveNumFaceApp)))
    {
        msgDemoModeoData.enable = 1;
    }
    else
    {
        msgDemoModeoData.enable = 0;
    }
#endif
#ifdef USE_FACE_AOBI //使用奥比人脸 
    msgDemoModeoData.enable = 0; //奥比人脸不需要设置演示模式
#endif
}
/***************************************************************************************
**函数名:       Menu_Face_PowerOffStart
**功能描述:     人脸关闭
**输入参数:     --
**输出参数:     --
**备注:

****************************************************************************************/
void Menu_Face_PowerOffStart(void)
{
#if 1
    if(MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw)     //检查wifi主开关开启
    {
        msgPowerDownData.enableLowPower = 0;
    }
    else
    {
        if (0 == storageSave.sysDat.ShangTangCatEyeSingleDouble)
        {
            msgPowerDownData.enableLowPower = 2; //不复位，双向模式
        }
        else
        {
            msgPowerDownData.enableLowPower = 1; //复位,单向模式
        }
    }
#endif
    if((0 == st_InputInfo.modeuleFail.Face) && (eFaceIdStatusStandBy == faceIdCtrl.status))
    {
#if 0
        msgPowerDownData.enableLowPower = 1; //暂时保活,如果不用这个，会导致添加人脸失败???
#endif
        HAL_FaceId_CmdPowerOff();            //关闭人脸
#if 0
        wifiSendParm.waitComplete = eWaitCompleteYes;
#endif
    }
    else
    {
        DEBUG_LOG_OUT("Menu_Face_PowerOffStart \n");
        HAL_FaceId_CmdPowerOffDir(); //人脸错误，直接关闭
    }
}
/***************************************************************************************
**函数名:       Menu_Face_DelGeneralFaceStart
**功能描述:     开始删除指令
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void Menu_Face_DelGeneralFaceStart(void)
{
    menuFaceCtrl.userId = 0;
    menuFaceCtrl.FaceDelStatus = eMenuFaceDelChkId;
}



/***************************************************************************************
**函数名:       Menu_Face_Service
**功能描述:     指纹头添加服务程序
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void Menu_Face_Service(void)
{
    uint16_t userID = 0;
    menuFaceCtrl.runTime++;
    if (menuFaceCtrl.runTime < MENU_FACE_RUN_TIME_MAX)
    {
        return;
    }
    menuFaceCtrl.runTime = 0;
    switch (HAL_FaceId_PowerDeal())
    {
    case eFaceIdStatusErrorTimeOut:
        st_InputInfo.modeuleFail.Face = 1;
        break;
    case eFaceIdStatusStandBy:
        st_InputInfo.modeuleFail.Face = 0;
        break;
    default:
        break;
    }
    HAL_FaceId_RevcDeal(); //处理数据接收
    switch (st_MenuItem.e_CurrentIndex)
    {
    case enum_Menu_AddFaceStep1:
    case enum_Menu_AddFaceStep2:
    case enum_Menu_AddFaceStep3:
    case enum_Menu_AddFaceStep4:
    case enum_Menu_AddFaceStep5:
        Menu_Face_AddFaceIng();
        break;
    case enum_Menu_BleAddFaceStep1:
    case enum_Menu_BleAddFaceStep2:
    case enum_Menu_BleAddFaceStep3:
    case enum_Menu_BleAddFaceStep4:
    case enum_Menu_BleAddFaceStep5:
        Menu_Face_BleAddFaceIng();
        break;
#if 0
    case enum_Menu_BleAddFaceUserOk:
    case enum_Menu_AddFaceUserOk:
        Menu_Face_SetDemoMode();
        break;
#endif
    case enum_Menu_AddFaceUserFail:
        break; //添加人脸失败
    case enum_Menu_Desktop:
        Menu_Face_Desktop();
        break; //桌面
    case enum_Menu_AdminChk:
        Menu_Face_AdminChk();
        break;
    case enum_Menu_DelGeneralFace: //删除普通人脸
        Menu_Face_DelGeneralFace();
        break;
    case enum_Menu_Factory:
        break;
    case enum_Menu_BleEquipmentReg:
    case enum_Menu_FactoryIng:
        Menu_Face_Clear();
        break;
    case enum_Menu_BleAddFaceUserFail: //添加失败，并删除本次添加的人脸
        Menu_Face_BleAddFaceUserFail();
        break;
    case enum_Menu_BleFaceDelId:
        Menu_Face_BleFaceDelId();
        break;
    case enum_Menu_BleFaceVersionGet:
        Menu_Face_BleFaceVersionGet();
        break;
#ifdef USE_FACE_WIFI
    case enum_Menu_BleConfigWifiData: //配置wifi数据
        Menu_Face_BleConfigWifiData();
        break;
#endif
    case enum_Menu_BleAliyunSecretKeySetParm:
        Menu_Face_BleAliyunSecretKeySetParm();
        break;
    case enum_Menu_BleCatEyeSN:
        Menu_Face_BleCatEyeSN();
        break;
    case enum_Menu_Bell:
        Menu_Face_Bell();
        break;
    case enum_Menu_BleShangTangCatEyeSingleDouble:
        Menu_Face_BleShangTangCatEyeSingleDouble();
        break;
    case enum_Menu_WifiConnect:
        Menu_Face_WifiConnect();
        break;
    case enum_Menu_BleR8CatEyeOTA:
        Menu_Face_BleR8CatEyeOTA();
        break;
    case enum_Menu_CatEyeConn:
        Menu_CatEyeConn();
        break;
    case enum_Menu_BleModifFaceSafetyLevel:
        Menu_Face_BleModifFaceSafetyLevel();
        break;
    default:
        break;
    }
#ifdef USE_FACE_WIFI
    Menu_FaceWifiDeal();
    Menu_TriggerCaptureData();
#endif
}
#endif /*Menu_FACE_MODULE_EN*/
/************************************Code End******************************************/
