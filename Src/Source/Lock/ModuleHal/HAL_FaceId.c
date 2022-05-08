/***************************************************************************************
**文件名:     HAL_FaceId.c
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
#include "HAL_FaceId.h"
#include "DRV_FaceId.h"
#if HAL_FACE_ID_MODULE_EN
#warning HAL_FACE_ID模块使能!
/*******************************宏定义***************************/
#define HAL_FACE_ID_RUN_TIME_MAX (1)
#define HAL_FACE_ID_WAIT_RST_TIME_MAX (30)           //等待复位完成时间
#if 0
#define HAL_FACE_ID_WAIT_READY_TIME_MAX (200)        //等待启动完成时间
#else
#define HAL_FACE_ID_WAIT_READY_TIME_MAX (500)        //等待启动完成时间
#endif

#define HAL_FACE_ID_WAIT_INIT_TIME_MAX (10)          //等待初始化命令回复时间
#define HAL_FACE_ID_WAIT_MODE_TIME_MAX (100)          //等待接收模式设置时间
#define HAL_FACE_ID_WAIT_SHUT_DOWN_CMD_TIME_MAX (150) //等待关机命令回复时间
#define HAL_FACE_ID_WAIT_SHUT_DOWN_TIME_MAX (5)      //等待关机完成时间
#define HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX (100)   //等待上电完成时间
#define HAL_FACE_ID_SEND_TYPE_ENCRYPT (1)            //密文发送
#define HAL_FACE_ID_SEND_TYPE_UNENCRYPT (0)          //非加密发送
#define HAL_FACE_ID_NOTE_DAFAULT (0XFF)
#define HAL_FACE_ID_ENROLL_TIME_OUT (20)      //添加人脸模块超时时间
#define HAL_FACE_ID_VERIFY_TIME_OUT (9)       //验证人脸模块超时时间
#define HAL_FACE_ID_WAIT_RECE_TIME_ADD (750)  //等待时间
#define HAL_FACE_ID_WAIT_NEXT_TIME_ADD (50)  //等待时间
//#define HAL_FACE_ID_WAIT_RECE_OVER_TIME (250) //5S的等待时间
#define HAL_FACE_ID_WAIT_RECE_OVER_TIME (950) //5S的等待时间
#define HAL_FACE_ID_WAIT_RECE_TIME_500MS (25) //500MS的等待时间
#define HAL_FACE_ID_WAIT_RECE_TIME_200MS (10) //200ms的等待时间
/***********************************************************
 * 人脸方向定义
 * ********************************************************/
#define FACE_ID_DIRECTION_UP 0x10       //录入朝上的人脸
#define FACE_ID_DIRECTION_DOWN 0x08     //录入朝下的人脸
#define FACE_ID_DIRECTION_LEFT 0x04     //录入朝左的人脸
#define FACE_ID_DIRECTION_RIGHT 0x02    //录入朝右的人脸
#define FACE_ID_DIRECTION_MIDDLE 0x01   //录入正向的人脸
#define FACE_ID_DIRECTION_UNDEFINE 0x00 //未定义，默认为正向
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
TYPEs_MsgInitData msgInitData;           //未初始化
TYPEs_MsgReplyInitData msgReplyInitData; //未初始化
TYPEs_MsgEnrollData msgEnrollData;
TYPEs_MsgReplyEnrollData msgReplyEnrollData;
//TYPEs_MsgEnrollData noteDataFace;
TYPEs_ReplyResult replyResult;
TYPEs_NoteNid noteNid;
TYPEs_MsgVerifyData msgVerifyData;
TYPEs_MsgReplyVerifyData msgReplyVerifyData;
TYPEs_MsgSnapImageData msgSnapImageData;
TYPEs_MsgGetSavdeImageData msgGetSavdeImageData;
TYPEs_MsgReplyGetSavdeImageData msgReplyGetSavdeImageData;
TYPEs_MsgUploadImageData msgUploadImageData;
TYPEs_MsgTriggerCaptureData msgTriggerCaptureData;
TYPEs_MsgDeleteMediaFileData msgDeleteMediaFileData;
TYPEs_MsgVolumeData msgVolumeData;
TYPEs_MsgDeluserData msgDeluserData;
TYPEs_MsgStartOtaData msgStartOtaData;
TYPEs_MsgConfigBaudrate msgConfigBaudrate;
TYPEs_MsgConfigWifiData msgConfigWifiData;
TYPEs_MsgGetWifiMacData msgGetWifiMacData;
TYPEs_MsgReplyGetWifiStatusData msgReplyGetWifiStatusData;
TYPEs_MsgLinkkeyData msgLinkkeyData;
TYPEs_MsgDataPassThrough msgDataPassThrough;
TYPEs_MsgVideoCallData msgVideoCallData;
TYPEs_MsgReplyDeviceName msgReplyDeviceName;
TYPEs_MsgReplyGetModuleStatus mMsgReplyGetModuleStatus;
TYPEs_MsgPowerDownData msgPowerDownData;
TYPEs_MsgDebugModeData msgDebugModeData;
TYPEs_MsgUploadDebugInfoData msgUploadDebugInfoData;
TYPEs_MsgDemoModeoData msgDemoModeoData;
TYPEs_MsgSetThresholdLevel msgSetThresholdLevel;
TYPEs_MsgReplyVersionGet msgReplyVersionGet;
TYPEs_NoteDataFace noteDataFace;
TYPEs_NoteOTAState noteOTAState;
TYPEs_NoteEyeState noteEyeState;
TYPEs_NoteMediaCap noteMediaCap;
TYPEs_NoteMediaState noteMediaState;
#define WIFI_OUT_BUFF_ADDR (&msgDataPassThrough)
#define WIFI_OUT_BUFF_PROTOCOL ((uint8_t *)(WIFI_OUT_BUFF_ADDR) + sizeof(TYPEs_WifiProtocolFront))
TYPEs_WifiProtocol wifiProtocol = {
    .front.header = COMM_WIFI_HEADER,
    .front.type = eWifiProtocolTypeNone,
    .front.cmd = 0,
    .front.len = 0,
    .dat = NULL,
    .chk = 0,
};
TYPEs_WifiCmdUnlockRealUpdata *const pWifiCmdUnlockRealUpdata = (TYPEs_WifiCmdUnlockRealUpdata *)(WIFI_OUT_BUFF_PROTOCOL);
TYPEs_WifiCmdAlarmUpdata *const pWifiCmdAlarmUpdata = (TYPEs_WifiCmdAlarmUpdata *)(WIFI_OUT_BUFF_PROTOCOL);
TYPEs_WifiConfig *const pWifiConfig = (TYPEs_WifiConfig *)(WIFI_OUT_BUFF_PROTOCOL); 
TYPEs_WifiCmdUpdataTest *const pWifiCmdUpdataTest = (TYPEs_WifiCmdUpdataTest *)(WIFI_OUT_BUFF_PROTOCOL); 
TYPEs_FaceIdCtrl faceIdCtrl = { 
    .status = eFaceIdStatusNone,
    .runTim = 0,
    .waitReadyTim = 0,
    .readyFlag = 0,
    .encryptFlag = 0,
    .add.step = eFaceIdAddNone,
    .verify.step = eFaceIdVerifyNone,
    .delOne.step = eFaceIdDelOneNone,
    .delAll.step = eFaceIdDelAllNone,
    .operate.OverTimr = 0,
    .operate.OverTimrCnt = 0,
};
/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       HAL_FaceId_CmdStart
**功能描述:     启动指令处理
**输入参数:     --
**输出参数:
uint32_t overTime 超时时间 
**备注:
****************************************************************************************/
static void HAL_FaceId_CmdStart(uint32_t overTime)
{
    faceIdCtrl.com = eFaceIdComReady;
    faceIdCtrl.operate.OverTimr = overTime;
}
/***************************************************************************************
**函数名:       HAL_FaceId_Cmd
**功能描述:     命令发送模板
**输入参数:     --
**输出参数:
uint8_t cmd, 命令
uint8_t waitStandBy, 等待人脸初始化完成 0、等待初始化完成;1、不等待 
uint32_t overTime 超时时间
**备注:
****************************************************************************************/
static TYPEe_FaceIdCom HAL_FaceId_Cmd(uint8_t cmd, uint8_t waitStandBy, uint32_t overTime)
{
    if (0 != faceIdCtrl.operate.OverTimr) //自动计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //超时
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:
        if ((eFaceIdStatusStandBy == faceIdCtrl.status) || (waitStandBy)) //初始化完成
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
            HAL_FaceId_CmdSend(cmd);
            faceIdCtrl.operate.OverTimr = overTime;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (cmd == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.com = eFaceIdComOk;
                DEBUG_LOG_OUT("eFaceIdComReply ok\n");
                break;

            default:
                faceIdCtrl.com = eFaceIdComFail;
                DEBUG_LOG_OUT("eFaceIdComFail\n");
                break;
            }
        }
        break;
    case eFaceIdComOk:
        break;
    case eFaceIdComFail:
        break;
    case eFaceIdComTimeOut:
        break;

    default:
        break;
    }
    return faceIdCtrl.com;
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_FaceId_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_Init(void)
{
    DRV_FaceId_Init();
}
/***************************************************************************************
**函数名:       HAL_FaceId_ClosePow
**功能描述:     关闭人脸电源
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_ClosePow(void)
{
    DRV_FaceId_ClosePow(); //关闭模块电源
    faceIdCtrl.status = eFaceIdStatusNone;
}
/***************************************************************************************
**函数名:       HAL_FaceId_CmdSend
**功能描述:     发送数据
**输入参数:     
uint8_t mid mid
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_CmdSend(uint8_t mid)
{
    uint8_t msgTmp = 0;
    switch (mid)
    {
    case FACE_ID_MID_INIT:
        DRV_FaceId_CmdSend(FACE_ID_MID_INIT,
                           (uint8_t *)(&msgInitData),
                           sizeof(msgInitData),
                           HAL_FACE_ID_SEND_TYPE_UNENCRYPT); //明文的方式发送
        break;
    case FACE_ID_MID_RESET:
        DRV_FaceId_CmdSend(FACE_ID_MID_RESET,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_VERIFY:
        DRV_FaceId_CmdSend(FACE_ID_MID_VERIFY,
                           (uint8_t *)(&msgVerifyData),
                           sizeof(msgVerifyData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_ENROLL:
        DRV_FaceId_CmdSend(FACE_ID_MID_ENROLL,
                           (uint8_t *)(&msgEnrollData),
                           sizeof(msgEnrollData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_SNAPIMAGE:
        DRV_FaceId_CmdSend(FACE_ID_MID_SNAPIMAGE,
                           (uint8_t *)(&msgSnapImageData),
                           sizeof(msgSnapImageData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GETSAVEDIMAGE:
        DRV_FaceId_CmdSend(FACE_ID_MID_GETSAVEDIMAGE,
                           (uint8_t *)(&msgGetSavdeImageData),
                           sizeof(msgGetSavdeImageData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_UPLOADIMAGE:
        DRV_FaceId_CmdSend(FACE_ID_MID_UPLOADIMAGE,
                           (uint8_t *)(&msgUploadImageData),
                           sizeof(msgUploadImageData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_TRIGGER_CAPTURE:
        DRV_FaceId_CmdSend(FACE_ID_MID_TRIGGER_CAPTURE,
                           (uint8_t *)(&msgTriggerCaptureData),
                           sizeof(msgTriggerCaptureData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DELETE_MEDIA_FILE:
        DRV_FaceId_CmdSend(FACE_ID_MID_DELETE_MEDIA_FILE,
                           (uint8_t *)(&msgDeleteMediaFileData),
                           sizeof(msgDeleteMediaFileData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_SET_VOLUME:
        DRV_FaceId_CmdSend(FACE_ID_MID_SET_VOLUME,
                           (uint8_t *)(&msgVolumeData),
                           sizeof(msgVolumeData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DELUSER:
        DRV_FaceId_CmdSend(FACE_ID_MID_DELUSER,
                           (uint8_t *)(&msgDeluserData),
                           sizeof(msgDeluserData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DELALL:
        DRV_FaceId_CmdSend(FACE_ID_MID_DELALL,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_FACERESET:
        DRV_FaceId_CmdSend(FACE_ID_MID_FACERESET,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GET_VERSION:
        DRV_FaceId_CmdSend(FACE_ID_MID_GET_VERSION,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_START_OTA:
        DRV_FaceId_CmdSend(FACE_ID_MID_START_OTA,
                           (uint8_t *)(&msgStartOtaData),
                           sizeof(msgStartOtaData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_STOP_OTA:
        DRV_FaceId_CmdSend(FACE_ID_MID_STOP_OTA,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_CONFIG_BAUDRATE:
        DRV_FaceId_CmdSend(FACE_ID_MID_CONFIG_BAUDRATE,
                           (uint8_t *)(&msgConfigBaudrate),
                           sizeof(msgConfigBaudrate),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_CONFIG_WIFI:
        DRV_FaceId_CmdSend(FACE_ID_MID_CONFIG_WIFI,
                           (uint8_t *)(&msgConfigWifiData),
                           sizeof(msgConfigWifiData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GET_WIFIMAC:
        DRV_FaceId_CmdSend(FACE_ID_MID_GET_WIFIMAC,
                           (uint8_t *)(&msgGetWifiMacData),
                           sizeof(msgGetWifiMacData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GET_WIFISTATUS:
        DRV_FaceId_CmdSend(FACE_ID_MID_GET_WIFISTATUS,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_SET_LINKKEY:
        DRV_FaceId_CmdSend(FACE_ID_MID_SET_LINKKEY,
                           (uint8_t *)(&msgLinkkeyData),
                           sizeof(TYPEs_MsgLinkkeyData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DATA_PASSTHROUGH:
        DRV_FaceId_CmdSend(FACE_ID_MID_DATA_PASSTHROUGH,
                           (uint8_t *)(&msgDataPassThrough),
                           sizeof(TYPEs_MsgDataPassThrough),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_TRIGGER_VIDEOCALL:
        DRV_FaceId_CmdSend(FACE_ID_MID_TRIGGER_VIDEOCALL,
                           (uint8_t *)(&msgVideoCallData),
                           sizeof(TYPEs_MsgVideoCallData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DEVICE_NAME:
        DRV_FaceId_CmdSend(FACE_ID_MID_DEVICE_NAME,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GET_MODULE_STATUS:
        DRV_FaceId_CmdSend(FACE_ID_MID_GET_MODULE_STATUS,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_POWERDOWN:
        DRV_FaceId_CmdSend(FACE_ID_MID_POWERDOWN,
                           (uint8_t *)(&msgPowerDownData),
                           sizeof(msgPowerDownData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DEBUG_MODE:
        DRV_FaceId_CmdSend(FACE_ID_MID_DEBUG_MODE,
                           (uint8_t *)(&msgTmp),
                           0,
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_GET_DEBUG_INFO:
        DRV_FaceId_CmdSend(FACE_ID_MID_GET_DEBUG_INFO,
                           (uint8_t *)(&msgDebugModeData),
                           sizeof(msgDebugModeData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_UPLOAD_DEBUG_INFO:
        DRV_FaceId_CmdSend(FACE_ID_MID_UPLOAD_DEBUG_INFO,
                           (uint8_t *)(&msgUploadDebugInfoData),
                           sizeof(msgUploadDebugInfoData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;
    case FACE_ID_MID_DEMOMODE:
        DRV_FaceId_CmdSend(FACE_ID_MID_DEMOMODE,
                           (uint8_t *)(&msgDemoModeoData),
                           sizeof(TYPEs_MsgDemoModeoData),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;		   
	case FACE_ID_MID_SET_THRESHOLD_LEVEL:
        DRV_FaceId_CmdSend(FACE_ID_MID_SET_THRESHOLD_LEVEL,
                           (uint8_t *)(&msgSetThresholdLevel),
                           sizeof(TYPEs_MsgSetThresholdLevel),
                           HAL_FACE_ID_SEND_TYPE_ENCRYPT);
        break;						  
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetRecvReply
**功能描述:     解析reply类型的信息
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_GetRecvReply(void)
{
    DRV_FaceId_GetRecvReplyMid((uint8_t *)(&replyResult), sizeof(replyResult));
    switch (replyResult.code)
    {
    case FACE_ID_MID_INIT:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyInitData), sizeof(msgReplyInitData));
        break;
    case FACE_ID_MID_RESET:
        break;
    case FACE_ID_MID_VERIFY:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyVerifyData), sizeof(msgReplyVerifyData));
        break;
    case FACE_ID_MID_ENROLL:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyEnrollData), sizeof(msgReplyEnrollData));
        break;
    case FACE_ID_MID_SNAPIMAGE:
        break;
    case FACE_ID_MID_GETSAVEDIMAGE:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyGetSavdeImageData), sizeof(msgReplyGetSavdeImageData));
        break;
    case FACE_ID_MID_UPLOADIMAGE:
        break;
    case FACE_ID_MID_TRIGGER_CAPTURE:
        break;
    case FACE_ID_MID_DELETE_MEDIA_FILE:
        break;
    case FACE_ID_MID_SET_VOLUME:
        break;
    case FACE_ID_MID_DELUSER:
        //后面没有追加包
        break;
    case FACE_ID_MID_DELALL:
        break;
    case FACE_ID_MID_FACERESET:
        break;
    case FACE_ID_MID_GET_VERSION:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyVersionGet), sizeof(TYPEs_MsgReplyVersionGet));
        break;
    case FACE_ID_MID_START_OTA:
        break;
    case FACE_ID_MID_STOP_OTA:
        break;
    case FACE_ID_MID_CONFIG_BAUDRATE:
        break;
    case FACE_ID_MID_CONFIG_WIFI:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgConfigWifiData), sizeof(TYPEs_MsgConfigWifiData));
        break;
    case FACE_ID_MID_GET_WIFIMAC:
        break;
    case FACE_ID_MID_GET_WIFISTATUS:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyGetWifiStatusData), sizeof(msgReplyGetWifiStatusData));
        break;
    case FACE_ID_MID_SET_LINKKEY:

        break;
    case FACE_ID_MID_DATA_PASSTHROUGH:
        break;
    case FACE_ID_MID_TRIGGER_VIDEOCALL:
        break;
    case FACE_ID_MID_DEVICE_NAME:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&msgReplyDeviceName), DRV_FaceId_GetRecvMsgSize());
        break;
    case FACE_ID_MID_GET_MODULE_STATUS:
        DRV_FaceId_GetRecvReplyData((uint8_t *)(&mMsgReplyGetModuleStatus), sizeof(TYPEs_MsgReplyGetModuleStatus));
        break;
    case FACE_ID_MID_POWERDOWN:
        break;
    case FACE_ID_MID_DEBUG_MODE:
        break;
    case FACE_ID_MID_GET_DEBUG_INFO:
        break;
    case FACE_ID_MID_UPLOAD_DEBUG_INFO:
        break;
    case FACE_ID_MID_DEMOMODE:
        break;
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetRecvNote
**功能描述:     解析Note类型的信息
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_GetRecvNote(void)
{
    DRV_FaceId_GetRecvNoteNid((uint8_t *)(&noteNid), sizeof(noteNid));
    switch (noteNid.nid)
    {
    case FACE_ID_NID_READY:
        // faceIdCtrl.readyFlag = 1;               //初始化完成
        // msgInitData.enableNetwork = 0;          //不联网
        // msgInitData.mode = 1;                   //密文方式
        // memset(msgInitData.seed, 0, 4);         //这里这是默认值是0
        // DRV_FaceId_GetAesKey(msgInitData.seed); //获取AES key
        // memset(msgInitData.crtTime, 0, 4);      //这里这是默认值是0
        // msgInitData.waitingTime = 0;            //等待时间为0
        // HAL_FaceId_CmdSend(FACE_ID_MID_INIT);   //直接发送
        // faceIdCtrl.encryptFlag = 1;             //后续使用密文通讯
        break;
    case FACE_ID_NID_FACE_STATE:
        DRV_FaceId_GetRecvNoteData((uint8_t *)&noteDataFace, sizeof(noteDataFace));
        break;
    case FACE_ID_NID_UNKNOWNERROR:
        break;
    case FACE_ID_NID_OTA_STATE:
        DRV_FaceId_GetRecvNoteData((uint8_t *)&noteOTAState, sizeof(TYPEs_NoteOTAState));
        break;
    case FACE_ID_NID_EYE_STATE:
        DRV_FaceId_GetRecvNoteData((uint8_t *)&noteEyeState, sizeof(TYPEs_NoteEyeState));
        break;
    case FACE_ID_MID_GETSAVEDIMAGE:
        break;
    case FACE_ID_MID_UPLOADIMAGE:
        break;
    case FACE_ID_NID_MEDIA_CAP_STATE:
        DRV_FaceId_GetRecvNoteData((uint8_t *)&noteMediaCap, sizeof(TYPEs_NoteMediaCap));
        break;
    case FACE_ID_NID_MEDIA_STATE:
        DRV_FaceId_GetRecvNoteData((uint8_t *)&noteMediaState, sizeof(TYPEs_NoteMediaState));
        break;
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetRecvReply
**功能描述:     解析reply类型的信息
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_RevcDeal(void)
{
    switch (DRV_FaceId_RecvDataUnblock())
    {
    case 1: //接收完成,未来考虑循环读取
        if (0 == DRV_FaceId_RecvDataAndAnalysis(faceIdCtrl.encryptFlag))
        {
            switch (DRV_FaceId_GetRecvMsgType()) //直接解析数据
            {
            case FACE_ID_MID_REPLY:
                HAL_FaceId_GetRecvReply();
                break;
            case FACE_ID_MID_NOTE:
                HAL_FaceId_GetRecvNote();
                break;
            case FACE_ID_MID_IMAGE:

                break;
            default:
                break;
            }
        }
        DRV_FaceId_ClearRxPar(); //清除接收
        break;
    case -1:                     //参数错误
    case -2:                     //没有数据
    case -3:                     //接收参数错误
        DRV_FaceId_ClearRxPar(); //清除接收
        break;
    default:
        //		DRV_FaceId_ClearRxPar(); //清除接收
        break;
    }
}
/***************************************************************************************
**函数名:       HAL_FaceId_CmdPowerOn
**功能描述:     人脸识别上电
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_CmdPowerOn(uint8_t mode)
{
    if(0 == mode)
    {
        #if 1
        faceIdCtrl.shotdownFlag = eFaceIdShutdownNone;
        faceIdCtrl.statusBackCmd = eFaceIdStatusPowerOn;
        DEBUG_LOG_OUT("faceIdCtrl.statusBackCmd = eFaceIdStatusPowerOn;\n");
        #else
        if (eFaceIdStatusStandBy != faceIdCtrl.status)
        {
            faceIdCtrl.shotdownFlag = eFaceIdShutdownNone;
            faceIdCtrl.statusBackCmd = eFaceIdStatusPowerOn;
            DEBUG_LOG_OUT("faceIdCtrl.statusBackCmd = eFaceIdStatusPowerOn;\n");
        }
        else
        {
            DEBUG_LOG_OUT("-> eFaceIdStatusStandBy \n");
        }
        #endif
    }
    else if(1 == mode)
    {
        DEBUG_LOG_OUT("-> face restart \n");
        faceIdCtrl.statusBackCmd = eFaceIdStatusNone; 
        faceIdCtrl.shotdownFlag = eFaceIdShutdownNone;
        faceIdCtrl.status = eFaceIdStatusPowerOn;
    }
    else
    {
         DEBUG_LOG_OUT("-> face power on error! \n");
    }
}
/***************************************************************************************
**函数名:       HAL_FaceId_CmdPowerOff
**功能描述:     人脸识别下电
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_CmdPowerOff(void)
{
	if(eFaceIdShutdownNone == faceIdCtrl.shotdownFlag)
	{
		faceIdCtrl.status = eFaceIdStatusPowerOff;
        faceIdCtrl.statusBackCmd = eFaceIdStatusNone; 
        faceIdCtrl.enableNetworkBack = 0; //清除标志
		DEBUG_LOG_OUT("faceIdCtrl.status = eFaceIdStatusPowerOff;\n");
	}
    else
	{
		DEBUG_LOG_OUT("HAL_FaceId_CmdPowerOff shutdown ing....\n");
	}
}
/***************************************************************************************
**函数名:       HAL_FaceId_CmdPowerOffDir
**功能描述:     人脸识别下电直接关闭电源
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_CmdPowerOffDir(void)
{
    DEBUG_LOG_OUT("HAL_FaceId_CmdPowerOffDir");
    DRV_FaceId_ClosePow(); //关闭人脸电源
    M_CAM_RX_OUT_PP();
    M_CAM_RX_OUT_0();
    M_CAM_TX_OUT_PP();
    M_CAM_TX_OUT_0();
    faceIdCtrl.status = eFaceIdStatusWaitShutDown; 
}
/***************************************************************************************
**函数名:       HAL_FaceId_PowerDeal
**功能描述:     人脸识别电源处理
**输入参数:     --
**输出参数:     --
**备注: 
奥比人脸猫眼模块应用注意事项：
CHANGE
串口切换/猫眼模块上电使能
0：切换到人脸模块
1：切换到猫眼模块
****************************************************************************************/
TYPEe_FaceIdStatus HAL_FaceId_PowerDeal(void)
{
    faceIdCtrl.runTim++;
    if (faceIdCtrl.runTim < HAL_FACE_ID_RUN_TIME_MAX)
    {
        return faceIdCtrl.status;
    }
    faceIdCtrl.runTim = 0;
    switch (faceIdCtrl.status)
    {
    case eFaceIdStatusNone:
        faceIdCtrl.status = faceIdCtrl.statusBackCmd;
        faceIdCtrl.statusBackCmd = eFaceIdStatusNone; //清空后台指令
        break;
    case eFaceIdStatusPowerOn:
        msgInitData.enableNetwork = faceIdCtrl.enableNetworkBack; //联网标志
        faceIdCtrl.enableNetworkBack = 0; //清除标志
		faceIdCtrl.shotdownFlag = eFaceIdShutdownNone;
        DEBUG_LOG_OUT("emsgInitData.enableNetwork:%x\n",msgInitData.enableNetwork);
        #ifdef USE_FACE_AOBI //使用奥比人脸
        DRV_FaceId_ClosePow(); //给模块断电
        DRV_Face_CateyeFaceSw(0); //切换到人脸
        #if 0
        DRV_Face_WifiPowClose(); //猫眼断电
        #endif
        #endif
        #ifdef USE_FACE_SHANGTANG //使用商汤人脸
        DRV_FaceId_ClosePow(); //给模块断电
        #endif
        faceIdCtrl.waitReadyTim = 0;
        faceIdCtrl.status = eFaceIdStatusPowerOnWaitRst;
        break;
    case eFaceIdStatusPowerOnWaitRst:
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_RST_TIME_MAX)
        {
            memset((uint8_t *)(&noteNid),
                   HAL_FACE_ID_NOTE_DAFAULT,
                   sizeof(noteNid)); //清空接收
            HAL_FaceId_Init();       //初始化串口
            #ifdef USE_FACE_AOBI //使用奥比人脸
            if (0 != msgInitData.enableNetwork)
            {
                DRV_Face_CateyeFaceSw(1); //切换到猫眼
                DRV_Face_WifiPowOpen(); //打开wifi的电源
            }
            else
            {
                DRV_Face_CateyeFaceSw(0); //切换到人脸
                DRV_FaceId_OpenPow();    //给模块上电
            }
            #endif
            #ifdef USE_FACE_SHANGTANG //使用商汤人脸
            #if 1
            if (0 != msgInitData.enableNetwork)
            {
                DRV_Face_WifiPowOpen(); //打开wifi的电源
            }
            #endif
            DRV_FaceId_OpenPow();    //给模块上电
            #endif
            faceIdCtrl.status = eFaceIdStatusWaitReady;
            DEBUG_LOG_OUT("faceIdCtrl.status = eFaceIdStatusWaitReady;\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.encryptFlag = 0; //明文通讯
        }
        break;
    case eFaceIdStatusWaitReady:
        switch (noteNid.nid)
        {
        case FACE_ID_NID_READY:
            faceIdCtrl.status = eFeceIdStatusGetDeiceId;
            DEBUG_LOG_OUT("faceIdCtrl.status = eFeceIdStatusGetDeiceId;\n");
            break;
        default:
            break;
        }
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_READY_TIME_MAX)
        {
            DEBUG_LOG_OUT("eFaceIdStatusWaitReady -> eFaceIdStatusErrorTimeOut\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //超时，人脸出了问题。
            DRV_FaceId_ClosePow();                         //关闭人脸电源
        }
        break;
    case eFeceIdStatusGetDeiceId:
        
        msgInitData.mode = 1;                   //密文方式
        memset(msgInitData.seed, 0, 4);         //这里这是默认值是0
        DRV_FaceId_GetAesKey(msgInitData.seed); //获取AES key
        memset((uint8_t *)(&noteNid),
               HAL_FACE_ID_NOTE_DAFAULT,
               sizeof(noteNid));              //清空接收		
        
        HAL_FaceId_CmdSend(FACE_ID_MID_INIT); //直接发送
        faceIdCtrl.encryptFlag = 1;           //后续使用密文通讯
        faceIdCtrl.waitReadyTim = 0;
        faceIdCtrl.status = eFeceIdStatusGetDeiceIdReply;
        DEBUG_LOG_OUT("eFeceIdStatusGetDeiceIdReply\n");
        break;
    case eFeceIdStatusGetDeiceIdReply:
        if (FACE_ID_MID_INIT == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                                     //保存设备ID号
				DEBUG_LOG_OUT("eFeceIdStatusGetDeiceIdReply ok\n");
                #if 0
                DEBUG_LOG_OUT("enter eFeceIdStatusDemoMod \n");
                faceIdCtrl.status = eFeceIdStatusDemoMod; //设置是否是演示模式
                #endif
                #if 1
				if(1 == msgDemoModeoData.enable)
				{
					DEBUG_LOG_OUT("enter eFeceIdStatusDemoMod \n");
					faceIdCtrl.status = eFeceIdStatusDemoMod; //设置是否是演示模式

				}
                else
				{
					DEBUG_LOG_OUT("enter eFaceIdStatusStandBy \n");
					faceIdCtrl.status = eFaceIdStatusStandBy;
				}
                #endif
                break;

            default:
                break;
            }
        }
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_INIT_TIME_MAX)
        {
            DEBUG_LOG_OUT("eFeceIdStatusGetDeiceIdReply -> eFaceIdStatusErrorTimeOut\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //超时，人脸出了问题。
            DRV_FaceId_ClosePow();                         //关闭人脸电源
        }
        break;
    case eFeceIdStatusDemoMod:
        HAL_FaceId_CmdSend(FACE_ID_MID_DEMOMODE); //直接发送
        faceIdCtrl.status = eFeceIdStatusDemoModReply;
        faceIdCtrl.waitReadyTim = 0;
        break;
    case eFeceIdStatusDemoModReply:
        if (FACE_ID_MID_DEMOMODE == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                //保存设备ID号
                faceIdCtrl.status = eFaceIdStatusStandBy; 
                DEBUG_LOG_OUT("eFeceIdStatusDemoModReply ->FACE_ID_MID_DEMOMODE FACE_ID_MR_SUCCESS\n");
                break;

            default:
                break;
            }
        }
        faceIdCtrl.waitReadyTim++;		
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_MODE_TIME_MAX)
        {
            DEBUG_LOG_OUT("eFeceIdStatusDemoModReply -> eFaceIdStatusErrorTimeOut\n");
            faceIdCtrl.waitReadyTim = 0;
			#if 0
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //超时，人脸出了问题。
            DRV_FaceId_ClosePow();                         //关闭人脸电源
			#endif
			#if 1
			faceIdCtrl.status = eFaceIdStatusStandBy; 
			#endif
        }
        break;
    case eFaceIdStatusStandBy:
        break;
    case eFaceIdStatusErrorTimeOut:
    case eFaceIdStatusPowerDownErrorTimeOut:
        faceIdCtrl.status = eFaceIdStatusNone;
        break;
    case eFaceIdStatusPowerOff:
        #if 1
        #define POWER_HAVE_RST
        #endif
		#ifdef POWER_HAVE_RST
		memset((uint8_t *)(&replyResult), 0, sizeof(replyResult)); //清空回复
		HAL_FaceId_CmdSend(FACE_ID_MID_RESET);
		faceIdCtrl.status = eFaceIdStatusPowerOffWaitRst;
		faceIdCtrl.shotdownFlag = eFaceIdShutdownIng;
		faceIdCtrl.waitReadyTim = 0;
		#else
		faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
		faceIdCtrl.waitReadyTim = 0;
		HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //直接发送
		DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
		#endif
        break;
	case eFaceIdStatusPowerOffWaitRst:
		if (FACE_ID_MID_RESET == replyResult.code) //接收成功
        {
            msgInitData.enableNetwork = 0; //清除联网
            DEBUG_LOG_OUT("eFaceIdStatusPowerOffWaitRst replyResult.code:%x\n", replyResult.code);
            faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
            faceIdCtrl.waitReadyTim = 0;
			HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //直接发送
			DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
			break;
		}
		faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > 100)
        {
            msgInitData.enableNetwork = 0; //清除联网
            DEBUG_LOG_OUT("eFaceIdStatusPowerOffWaitRst Timeout\n");
            faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
            faceIdCtrl.waitReadyTim = 0;
			HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //直接发送
			DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
			break;
        }
		break;
    case eFaceIdStatusWaitPowerDown:
        if (FACE_ID_MID_POWERDOWN == replyResult.code) //接收成功
        {
            DEBUG_LOG_OUT("replyResult.code:%x\n", replyResult.code);
			DEBUG_LOG_OUT("replyResult.result:%x\n", replyResult.result);
            faceIdCtrl.status = eFaceIdStatusWaitShutDown;
            faceIdCtrl.waitReadyTim = 0;
            DEBUG_LOG_OUT("eFaceIdStatusWaitPowerDown -> FACE_ID_MID_POWERDOWN FACE_ID_MR_SUCCESS\n");
#if 0
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                //保存设备ID号
                faceIdCtrl.status = eFaceIdStatusWaitShutDown;
                faceIdCtrl.waitReadyTim = 0;
                DEBUG_LOG_OUT("eFaceIdStatusWaitPowerDown -> FACE_ID_MID_POWERDOWN FACE_ID_MR_SUCCESS\n");
                break;

            default:
                break;
            }
#endif
        }
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_SHUT_DOWN_CMD_TIME_MAX) //超时，认为设备无效
        {
            DEBUG_LOG_OUT("eFaceIdStatusWaitPowerDown Time over\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.status = eFaceIdStatusPowerDownErrorTimeOut; //超时，人脸出了问题。
            DRV_FaceId_ClosePow();                                  //关闭人脸电源
            #ifdef USE_FACE_AOBI
            DRV_Face_CateyeFaceSw(0); //切换到猫眼,猫眼电源不断开
            #endif
                                                                    //这里需要提供，重发的接口
        }
        break;
    case eFaceIdStatusWaitShutDown:
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_SHUT_DOWN_TIME_MAX) //到达关机时间
        {
            DRV_FaceId_ClosePow(); //关闭人脸电源
            #ifdef USE_FACE_AOBI
            DRV_Face_CateyeFaceSw(0); //切换到猫眼,猫眼电源不断开
            #endif
            faceIdCtrl.status = eFaceIdStatusNone;
			faceIdCtrl.shotdownFlag = eFaceIdShutdownNone; 
			DEBUG_LOG_OUT("faceIdCtrl.statusBackCmd:%d\n",faceIdCtrl.statusBackCmd);
            DEBUG_LOG_OUT("eFaceIdStatusWaitShutDown\n");
        }
        break;
    default:
        break;
    }
    return faceIdCtrl.status;
}
/***************************************************************************************
**函数名:       HAL_FaceId_AddUser
**功能描述:     人脸识别添加用户
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_AddUserStart(void)
{
    faceIdCtrl.add.step = eFaceIdAddReady; //启动
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
}
/***************************************************************************************
**函数名:       HAL_FaceId_AddUser
**功能描述:     人脸识别添加用户
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_AddUserStop(void)
{
    faceIdCtrl.add.step = eFaceIdAddNone;
}

/***************************************************************************************
**函数名:       HAL_FaceId_AddUser
**功能描述:     人脸识别添加用户
**输入参数:     --
**输出参数:     
TYPEe_FaceIdAdd
添加人脸所处于的状态
**备注:         
****************************************************************************************/
TYPEe_FaceIdAdd HAL_FaceId_AddUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //超时计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.add.step = eFaceIdAddTimeOver; //超时
            DEBUG_LOG_OUT("HAL_FaceId_AddUser eFaceIdAddTimeOver\n");
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.add.step)
    {
    case eFaceIdAddNone:

        break;
    case eFaceIdAddReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            faceIdCtrl.add.step = eFaceIdAddMiddle; //正式开始添加流程
        }
        break;
    case eFaceIdAddMiddle:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace));   //清空回复
        memset((uint8_t *)(&msgEnrollData), 0, sizeof(msgEnrollData)); //清空回复
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));     //清空回复
//        msgEnrollData.admin = 1;
        memset(msgEnrollData.userName, 0, USER_NAME_SIZE);
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_MIDDLE;
        msgEnrollData.timeOut = HAL_FACE_ID_ENROLL_TIME_OUT;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddMiddleReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        break;
    case eFaceIdAddMiddleReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.operate.OverTimr = 0; //关闭超时计数器
                faceIdCtrl.operate.waitTime = HAL_FACE_ID_WAIT_NEXT_TIME_ADD;
                faceIdCtrl.add.step = eFaceIdAddMiddleOK;
                DEBUG_LOG_OUT("eFaceIdAddMiddleReply ok\n");
                break;

            default:
                faceIdCtrl.add.step = eFaceIdAddFail;
                DEBUG_LOG_OUT("eFaceIdAddFail\n");
                break;
            }
        }
        break;
    case eFaceIdAddMiddleOK:
        if(0 != faceIdCtrl.operate.waitTime)
        {
            faceIdCtrl.operate.waitTime --; 
        }
        else
        {
            faceIdCtrl.add.step = eFaceIdAddUP;
        }
        break;
    case eFaceIdAddUP:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_UP;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddUPReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddUP\n");
        break;
    case eFaceIdAddUPReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.operate.OverTimr = 0; //关闭超时计数器
                faceIdCtrl.operate.waitTime = HAL_FACE_ID_WAIT_NEXT_TIME_ADD;
                faceIdCtrl.add.step = eFaceIdAddUPOK;
                DEBUG_LOG_OUT("eFaceIdAddUPReply ok\n");
                break;

            default:
                faceIdCtrl.add.step = eFaceIdAddFail;
                DEBUG_LOG_OUT("eFaceIdAddFail\n");
                break;
            }
        }
        break;
    case eFaceIdAddUPOK:
        if(0 != faceIdCtrl.operate.waitTime)
        {
            faceIdCtrl.operate.waitTime --; 
        }
        else
        {
            faceIdCtrl.add.step = eFaceIdAddDown;
        }
        break;
    case eFaceIdAddDown:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_DOWN;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddDownReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddDown\n");
        break;
    case eFaceIdAddDownReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.operate.OverTimr = 0; //关闭超时计数器
                faceIdCtrl.operate.waitTime = HAL_FACE_ID_WAIT_NEXT_TIME_ADD;
                faceIdCtrl.add.step = eFaceIdAddDownOK;
                DEBUG_LOG_OUT("eFaceIdAddDownReply ok\n");
                break;

            default:
                faceIdCtrl.add.step = eFaceIdAddFail;
                DEBUG_LOG_OUT("eFaceIdAddFail\n");
                break;
            }
        }
        break;
    case eFaceIdAddDownOK:
        if(0 != faceIdCtrl.operate.waitTime)
        {
            faceIdCtrl.operate.waitTime --; 
        }
        else
        {
            faceIdCtrl.add.step = eFaceIdAddRight;
        }
        break;
    case eFaceIdAddRight:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_RIGHT;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddRightReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddRight\n");
        break;
    case eFaceIdAddRightReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.operate.OverTimr = 0; //关闭超时计数器
                faceIdCtrl.operate.waitTime = HAL_FACE_ID_WAIT_NEXT_TIME_ADD;
                faceIdCtrl.add.step = eFaceIdAddRightOK;
                DEBUG_LOG_OUT("eFaceIdAddRightReply ok\n");
                DEBUG_LOG_OUT("eFaceIdAddOk\n");
                break;

            default:
                faceIdCtrl.add.step = eFaceIdAddFail;
                DEBUG_LOG_OUT("eFaceIdAddFail\n");
                break;
            }
        }
        break;
    case eFaceIdAddRightOK:
        if(0 != faceIdCtrl.operate.waitTime)
        {
            faceIdCtrl.operate.waitTime --; 
        }
        else
        {
            faceIdCtrl.add.step = eFaceIdAddLeft;
        }
        break;
    case eFaceIdAddLeft:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_LEFT;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddLeftReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddLeft\n");
        break;
    case eFaceIdAddLeftReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.operate.OverTimr = 0; //关闭超时计数器
                faceIdCtrl.operate.waitTime = HAL_FACE_ID_WAIT_NEXT_TIME_ADD;
                faceIdCtrl.add.step = eFaceIdAddLeftOK;
                DEBUG_LOG_OUT("eFaceIdAddLeftReply ok\n");
                DEBUG_LOG_OUT("eFaceIdAddOk\n");
                break;

            default:
                faceIdCtrl.add.step = eFaceIdAddFail;
                DEBUG_LOG_OUT("eFaceIdAddFail\n");
                break;
            }
        }
        break;
    case eFaceIdAddLeftOK:
        if(0 != faceIdCtrl.operate.waitTime)
        {
            faceIdCtrl.operate.waitTime --; 
        }
        else
        {
            faceIdCtrl.add.step = eFaceIdAddOk;
        }
        break;
    case eFaceIdAddOk:
    case eFaceIdAddFail:
    case eFaceIdAddTimeOver:
        break;
    default:
        break;
    }

    return faceIdCtrl.add.step;
}
/***************************************************************************************
**函数名:       HAL_FaceId_VerifyUserStart
**功能描述:     人脸识别校验用户开始
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_VerifyUserStart(void)
{
    #if 1
    faceIdCtrl.verify.step = eFaceIdVerifyReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
    #endif
}
/***************************************************************************************
**函数名:       HAL_FaceId_VerifyUserStop
**功能描述:     人脸识别校验用户停止
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_VerifyUserStop(void)
{
    faceIdCtrl.verify.step = eFaceIdVerifyNone;
    HAL_FaceId_ComStart(); //开启停止命令
    faceIdCtrl.operate.OverTimr = 0;
}

/***************************************************************************************
**函数名:       HAL_FaceId_VerifyUser
**功能描述:     人脸识别校验用户
**输入参数:     --
**输出参数:     
TYPEe_FaceIdVerify
人脸所处于的状态
**备注:         
****************************************************************************************/
TYPEe_FaceIdVerify HAL_FaceId_VerifyUser(void)
{
    if ((0 != faceIdCtrl.operate.OverTimr) &&
        (eFaceIdVerifyNone != faceIdCtrl.verify.step)) //超时计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.verify.step = eFaceIdVerifyTimeOut; //超时
            DEBUG_LOG_OUT("HAL_FaceId_VerifyUser eFaceIdAddTimeOver\n");
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.verify.step)
    {
    case eFaceIdVerifyNone:

        break;
    case eFaceIdVerifyReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            memset((uint8_t *)(&msgVerifyData), 0, sizeof(msgVerifyData));           //清空指令
            memset((uint8_t *)(&msgReplyVerifyData), 0, sizeof(msgReplyVerifyData)); //清空回复
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace));             //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));               //清空回复
            msgVerifyData.pdRightAway = 0;
            msgVerifyData.timeOut = HAL_FACE_ID_VERIFY_TIME_OUT;
            HAL_FaceId_CmdSend(FACE_ID_MID_VERIFY);
            DEBUG_LOG_OUT("eFaceIdVerifyReady\n");
            faceIdCtrl.verify.step = eFaceIdVerifyReply;
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME;
            faceIdCtrl.operate.OverTimrCnt = 0;
        }
        break;
    case eFaceIdVerifyReply:
        if (FACE_ID_MID_VERIFY == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                //获取到的数据已经在msgReplyVerifyData
                faceIdCtrl.verify.step = eFaceIdVerifyOk;
                DEBUG_LOG_OUT("eFaceIdVerifyReply ok\n");
                break;
			case FACE_ID_MR_FAILED4_TIMEOUT: //检测超时
				DEBUG_LOG_OUT("HAL_FaceId_VerifyUser eFaceIdVerifyTimeOut\n");
				faceIdCtrl.verify.step = eFaceIdVerifyTimeOut;
				break;
            default:
                faceIdCtrl.verify.step = eFaceIdVerifyFail;
                DEBUG_LOG_OUT("eFaceIdVerifyFail\n");
                break;
            }
        }
		//这里检测note
		
        break;
    case eFaceIdVerifyOk:

        break;
    case eFaceIdVerifyFail:

        break;
    case eFaceIdVerifyTimeOut:

        break;
    default:
        break;
    }
    return faceIdCtrl.verify.step;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelOneUserStart
**功能描述:     删除一个指纹
**输入参数:     
uint16_t userId 用户ID
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_DelOneUserStart(uint16_t userId)
{

    memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData)); //清空指令
    msgDeluserData.userIdHeb = (uint8_t)(userId >> 8);
    msgDeluserData.userIdLeb = (uint8_t)(userId);
    faceIdCtrl.delOne.step = eFaceIdDelOneReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX+HAL_FACE_ID_WAIT_MODE_TIME_MAX+HAL_FACE_ID_WAIT_RST_TIME_MAX;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelOneUserStop
**功能描述:     删除一个指纹
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_DelOneUserStop(void)
{
    faceIdCtrl.delOne.step = eFaceIdDelOneNone;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelOneUser
**功能描述:     人脸识别删除一个用户
**输入参数:     --
**输出参数:     
TYPEe_FaceIdDelOne
人脸所处于的状态
**备注:         
****************************************************************************************/
TYPEe_FaceIdDelOne HAL_FaceId_DelOneUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //自动计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.delOne.step = eFaceIdDelOneTimeOut; //超时
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.delOne.step)
    {
    case eFaceIdDelOneNone:
        break;
    case eFaceIdDelOneReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //清空指令
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
            HAL_FaceId_CmdSend(FACE_ID_MID_DELUSER);
            DEBUG_LOG_OUT("eFaceIdDelOneReady\n");
            faceIdCtrl.delOne.step = eFaceIdDelOneReply;
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME;
            faceIdCtrl.operate.OverTimrCnt = 0;
        }
        break;
    case eFaceIdDelOneReply:
        if (FACE_ID_MID_DELUSER == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.delOne.step = eFaceIdDelOneOk;
                DEBUG_LOG_OUT("eFaceIdDelOneReply ok\n");
                break;

            default:
                faceIdCtrl.delOne.step = eFaceIdDelOneFail;
                DEBUG_LOG_OUT("eFaceIdDelOneFail\n");
                break;
            }
        }
        break;
    case eFaceIdDelOneOk:
        break;
    case eFaceIdDelOneFail:
        break;
    case eFaceIdDelOneTimeOut:
        break;

    default:
        break;
    }
    return faceIdCtrl.delOne.step;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelAllUserStart
**功能描述:     删除全部指纹
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_DelAllUserStart(void)
{
    faceIdCtrl.delAll.step = eFaceIdDelAllReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelAllUserStop
**功能描述:     删除全部指纹停止
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_DelAllUserStop(void)
{
    faceIdCtrl.delAll.step = eFaceIdDelAllNone;
}
/***************************************************************************************
**函数名:       HAL_FaceId_DelAllUser
**功能描述:     人脸识别删除全部用户
**输入参数:     --
**输出参数:     
TYPEe_FaceIdDelAll
人脸所处于的状态
**备注:         
****************************************************************************************/
TYPEe_FaceIdDelAll HAL_FaceId_DelAllUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //自动计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.delAll.step = eFaceIdDelAllTimeOut; //超时
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.delAll.step)
    {
    case eFaceIdDelAllNone:
        break;
    case eFaceIdDelAllReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //清空指令
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
            HAL_FaceId_CmdSend(FACE_ID_MID_DELALL);
            DEBUG_LOG_OUT("eFaceIdDelAllReady\n");
            faceIdCtrl.delAll.step = eFaceIdDelAllReply;
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME;
            faceIdCtrl.operate.OverTimrCnt = 0;
        }
        break;
    case eFaceIdDelAllReply:
        if (FACE_ID_MID_DELALL == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.delAll.step = eFaceIdDelAllOk;
                DEBUG_LOG_OUT("eFaceIdDelAllReply ok\n");
                break;

            default:
                faceIdCtrl.delAll.step = eFaceIdDelAllFail;
                DEBUG_LOG_OUT("eFaceIdDelAllFail\n");
                break;
            }
        }
        break;
    case eFaceIdDelAllOk:
        break;
    case eFaceIdDelAllFail:
        break;
    case eFaceIdDelAllTimeOut:
        break;

    default:
        break;
    }
    return faceIdCtrl.delAll.step;
}
/***************************************************************************************
**函数名:       HAL_FaceId_SetDemoMode
**功能描述:     设置demo模式
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_SetDemoMode(void)
{
    HAL_FaceId_CmdSend(FACE_ID_MID_DEMOMODE);
}
/***************************************************************************************
**函数名:       HAL_Face_EnterSleepConfig
**功能描述:     进入低功耗配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Face_EnterSleepConfig(void)
{
    DRV_Face_EnterSleepConfig();
}
/***************************************************************************************
**函数名:       HAL_FaceId_ComStart
**功能描述:     开始通用指令
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_ComStart(void)
{
    faceIdCtrl.com = eFaceIdComReady;
}
/***************************************************************************************
**函数名:       HAL_FaceId_Reset
**功能描述:     停止当前正在处理的消息，模块进入待命状态
**输入参数:     --
**输出参数:
TYPEe_FaceIdCom
人脸所处于的状态
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_Reset(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //自动计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //超时
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:

        //       if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //清空指令
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
            HAL_FaceId_CmdSend(FACE_ID_MID_RESET);
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (FACE_ID_MID_RESET == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.com = eFaceIdComOk;
                DEBUG_LOG_OUT("eFaceIdComReply ok\n");
                break;

            default:
                faceIdCtrl.com = eFaceIdComFail;
                DEBUG_LOG_OUT("eFaceIdComFail\n");
                break;
            }
        }
        break;
    case eFaceIdComOk:
        break;
    case eFaceIdComFail:
        break;
    case eFaceIdComTimeOut:
        break;

    default:
        break;
    }
    return faceIdCtrl.com;
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetVersionStart
**功能描述:     开始获取人脸版本指令
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_GetVersionStart(void)
{
    faceIdCtrl.com = eFaceIdComReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetVersion
**功能描述:     获取版本
**输入参数:     --
**输出参数:
TYPEe_FaceIdCom
人脸所处于的状态
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_GetVersion(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //自动计数器
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //超时
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //初始化完成
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //清空回复
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //清空回复
            HAL_FaceId_CmdSend(FACE_ID_MID_GET_VERSION);
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (FACE_ID_MID_GET_VERSION == replyResult.code) //接收成功
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //接收成功
                faceIdCtrl.com = eFaceIdComOk;
                DEBUG_LOG_OUT("eFaceIdComReply ok\n");
                break;

            default:
                faceIdCtrl.com = eFaceIdComFail;
                DEBUG_LOG_OUT("eFaceIdComFail\n");
                break;
            }
        }
        break;
    case eFaceIdComOk:
        break;
    case eFaceIdComFail:
        break;
    case eFaceIdComTimeOut:
        break;

    default:
        break;
    }
    return faceIdCtrl.com;
}
/***************************************************************************************
**函数名:       HAL_FaceId_ConfigWifiDataStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_ConfigWifiDataStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_ConfigWifiDataStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_ConfigWifiData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_CONFIG_WIFI,
                          0,
                          200); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetStatus
**功能描述:     人脸识别工作状态
**输入参数:     --
**输出参数:     
TYPEe_FaceIdStatus
**备注:         
****************************************************************************************/
TYPEe_FaceIdStatus HAL_FaceId_GetStatus(void)
{
    return faceIdCtrl.status;
}

/***************************************************************************************
**函数名:       HAL_FaceId_CmdStop
**功能描述:     停止指令处理
**输入参数:     --
**输出参数:		--
**备注:
****************************************************************************************/
void HAL_FaceId_CmdStop(void)
{
    faceIdCtrl.com = eFaceIdComNone;
    faceIdCtrl.operate.OverTimr = 0;
    faceIdCtrl.cmd = eFaceIdCmdNone;
}

/***************************************************************************************
**函数名:       HAL_FaceId_SetLinkKeyStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_SetLinkKeyStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_SetLinkKey
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_SetLinkKey(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_SET_LINKKEY,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**函数名:       HAL_FaceId_GetDeviceNameStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_GetDeviceNameStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_GetDeviceName
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_GetDeviceName(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_DEVICE_NAME,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}
/***************************************************************************************
**函数名:       HAL_FaceId_DataPassThroughStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_DataPassThroughStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_DataPassThrough
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_DataPassThrough(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_DATA_PASSTHROUGH,
                          0,
                          50); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**函数名:       Comm_WifiDataPack
**功能描述:     数据打包
**输入参数:    
uint16_t len  //要发送数据的长度
**输出参数:     
uint16_t
打包好数据的长度
**备注:         由于这里进行过大小端的转换，需要确认长度是否可用
****************************************************************************************/
static uint16_t Comm_WifiDataPack(uint8_t *pDat, uint16_t len)
{
    uint16_t lenCnt = 0;
    //	uint16_t chkSum =0;
    wifiProtocol.front.len = len; //填写数据长度
    BSWAP_SAVE_16(wifiProtocol.front.len);
    memcpy(pDat,
           (uint8_t *)(&wifiProtocol.front),
           sizeof(wifiProtocol.front)); //数据头
    lenCnt = sizeof(wifiProtocol.front);
    lenCnt += len; //获取数据长度
    wifiProtocol.chk = BasicFunc_GetSumChk((pDat + sizeof(wifiProtocol.front.header)),
                                           lenCnt - sizeof(wifiProtocol.front.header)); //计算校验和
    BSWAP_SAVE_16(wifiProtocol.chk);
    memcpy((uint8_t *)(pDat + lenCnt),
           &wifiProtocol.chk,
           sizeof(wifiProtocol.chk));   //添加数据
    lenCnt += sizeof(wifiProtocol.chk); //接上校验和的长度
    return lenCnt;
}
/***************************************************************************************
**函数名:       Comm_WifiCmdSend
**功能描述:     wifi数据发送
**输入参数:    
TYPEe_WifiCmd cmd 指令
**输出参数:     
uint16_t
打包好数据的长度
**备注:         
****************************************************************************************/
uint16_t Comm_WifiCmdSend(TYPEe_WifiCmd cmd)
{
    uint16_t lenTmp = 0;
    wifiProtocol.front.header = COMM_WIFI_HEADER;         //添加数据头
    wifiProtocol.front.type = eWifiProtocolTypeCmdNotAck; //添加指令类型
    wifiProtocol.front.cmd = cmd;                         //填写命令字
    switch (cmd)
    {
    case eWifiCmdUnlockRealUpdata:
        lenTmp = sizeof(TYPEs_WifiCmdUnlockRealUpdata);
        break;
    case eWifiCmdUnlockRealUpdataEncrypt:
        break;
    case eWifiCmdUnlockRealUpdataEncryptAll:
        break;
    case eWifiCmdAlarmUpdata:
        lenTmp = sizeof(TYPEs_WifiCmdAlarmUpdata);
        break;
    case eWifiCmdAlarmUpdataEncrypt:
        break;
    
    default:
        break;
    }
    return Comm_WifiDataPack((uint8_t *)(WIFI_OUT_BUFF_ADDR), lenTmp);
}

/***************************************************************************************
**函数名:       Comm_WifiCmdSend
**功能描述:     wifi数据发送
**输入参数:    
TYPEe_WifiCmd cmd 指令
TYPEe_WifiProtocolType tp wifi消息类型
uint16_t len 整体长度
**输出参数:     
uint16_t
打包好数据的长度
**备注:         
****************************************************************************************/
uint16_t Comm_WifiCmdSendWithLen(TYPEe_WifiCmd cmd,TYPEe_WifiProtocolType tp,uint16_t len)
{
    uint16_t lenTmp = 0;
    wifiProtocol.front.header = COMM_WIFI_HEADER;         //添加数据头
    wifiProtocol.front.type = tp; //添加指令类型
    wifiProtocol.front.cmd = cmd;                         //填写命令字
    return Comm_WifiDataPack((uint8_t *)(WIFI_OUT_BUFF_ADDR), len);
}
/***************************************************************************************
**函数名:       HAL_FaceId_TriggerCaptureDataStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_TriggerCaptureDataStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_TriggerCaptureData
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_TriggerCaptureData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_TRIGGER_CAPTURE,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**函数名:       HAL_FaceId_VideoCallDataStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_VideoCallDataStart(void)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**函数名:       HAL_FaceId_VideoCallData
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_VideoCallData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_TRIGGER_VIDEOCALL,
                          0,
                          100);
}

/***************************************************************************************
**函数名:       HAL_FaceId_CmdDealStart
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void HAL_FaceId_CmdDealStart(TYPEe_FaceIdCmd cmd, uint32_t overTime)
{
    DRV_Face_WifiPowOpen(); //打开wifi的电源
    faceIdCtrl.cmd = cmd;
    HAL_FaceId_CmdStart(overTime);
	DEBUG_LOG_OUT("HAL_FaceId_CmdDealStart\n");
}
/***************************************************************************************
**函数名:       HAL_FaceId_CmdDeal
**功能描述:     
**输入参数:     
uint8_t cmd, 命令
uint8_t waitStandBy, 等待人脸初始化完成 0、等待初始化完成;1、不等待 
uint32_t overTime 超时时间
**输出参数:     --
**备注:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_CmdDeal(TYPEe_FaceIdCmd cmd, uint8_t waitStandBy,uint32_t overTime)
{
    if (cmd != faceIdCtrl.cmd)
    {
        return eFaceIdComCmdError;
    }
    return HAL_FaceId_Cmd(faceIdCtrl.cmd,
                          waitStandBy,
                          overTime);
}

/***************************************************************************************
**函数名:       HAL_FaceId_ChkBusy
**功能描述:     查询人脸时候忙碌的情况
**输入参数:     --
**输出参数:     
int8_t
0 空闲
-1 忙碌
**备注:
****************************************************************************************/
int8_t HAL_FaceId_ChkBusy(void)
{
	if(eFaceIdComNone == faceIdCtrl.com )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
/***************************************************************************************
**函数名:       HAL_FaceId_ChkBusy
**功能描述:     查询人脸电源忙碌的情况
**输入参数:     --
**输出参数:     
int8_t
0 空闲
-1 忙碌
**备注:
****************************************************************************************/
int8_t HAL_FaceId_ChkPowerBusy(void)
{
	if(eFaceIdStatusNone ==  faceIdCtrl.status )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/***************************************************************************************
**函数名:       HAL_Face_WifiPowClose
**功能描述:     人脸wifi电源关闭 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Face_WifiPowClose(void)
{
    DRV_Face_WifiPowClose();
}

/***************************************************************************************
**函数名:       HAL_FaceId_IRSleepConfig
**功能描述:     IR身体感应休眠配置
**输入参数:     
uint8_t sw		0,关闭中断；非0，开启中断
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_FaceId_IRSleepConfig(uint8_t sw)
{
	DRV_FaceId_IRSleepConfig(sw);
}
#endif /*HAL_FACE_ID_MODULE_EN*/
/************************************Code End******************************************/
