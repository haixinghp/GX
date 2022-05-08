/***************************************************************************************
**�ļ���:     HAL_FaceId.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
****************************************************************************************/
#include "HAL_FaceId.h"
#include "DRV_FaceId.h"
#if HAL_FACE_ID_MODULE_EN
#warning HAL_FACE_IDģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_FACE_ID_RUN_TIME_MAX (1)
#define HAL_FACE_ID_WAIT_RST_TIME_MAX (30)           //�ȴ���λ���ʱ��
#if 0
#define HAL_FACE_ID_WAIT_READY_TIME_MAX (200)        //�ȴ��������ʱ��
#else
#define HAL_FACE_ID_WAIT_READY_TIME_MAX (500)        //�ȴ��������ʱ��
#endif

#define HAL_FACE_ID_WAIT_INIT_TIME_MAX (10)          //�ȴ���ʼ������ظ�ʱ��
#define HAL_FACE_ID_WAIT_MODE_TIME_MAX (100)          //�ȴ�����ģʽ����ʱ��
#define HAL_FACE_ID_WAIT_SHUT_DOWN_CMD_TIME_MAX (150) //�ȴ��ػ�����ظ�ʱ��
#define HAL_FACE_ID_WAIT_SHUT_DOWN_TIME_MAX (5)      //�ȴ��ػ����ʱ��
#define HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX (100)   //�ȴ��ϵ����ʱ��
#define HAL_FACE_ID_SEND_TYPE_ENCRYPT (1)            //���ķ���
#define HAL_FACE_ID_SEND_TYPE_UNENCRYPT (0)          //�Ǽ��ܷ���
#define HAL_FACE_ID_NOTE_DAFAULT (0XFF)
#define HAL_FACE_ID_ENROLL_TIME_OUT (20)      //�������ģ�鳬ʱʱ��
#define HAL_FACE_ID_VERIFY_TIME_OUT (9)       //��֤����ģ�鳬ʱʱ��
#define HAL_FACE_ID_WAIT_RECE_TIME_ADD (750)  //�ȴ�ʱ��
#define HAL_FACE_ID_WAIT_NEXT_TIME_ADD (50)  //�ȴ�ʱ��
//#define HAL_FACE_ID_WAIT_RECE_OVER_TIME (250) //5S�ĵȴ�ʱ��
#define HAL_FACE_ID_WAIT_RECE_OVER_TIME (950) //5S�ĵȴ�ʱ��
#define HAL_FACE_ID_WAIT_RECE_TIME_500MS (25) //500MS�ĵȴ�ʱ��
#define HAL_FACE_ID_WAIT_RECE_TIME_200MS (10) //200ms�ĵȴ�ʱ��
/***********************************************************
 * ����������
 * ********************************************************/
#define FACE_ID_DIRECTION_UP 0x10       //¼�볯�ϵ�����
#define FACE_ID_DIRECTION_DOWN 0x08     //¼�볯�µ�����
#define FACE_ID_DIRECTION_LEFT 0x04     //¼�볯�������
#define FACE_ID_DIRECTION_RIGHT 0x02    //¼�볯�ҵ�����
#define FACE_ID_DIRECTION_MIDDLE 0x01   //¼�����������
#define FACE_ID_DIRECTION_UNDEFINE 0x00 //δ���壬Ĭ��Ϊ����
/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/
TYPEs_MsgInitData msgInitData;           //δ��ʼ��
TYPEs_MsgReplyInitData msgReplyInitData; //δ��ʼ��
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
/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       HAL_FaceId_CmdStart
**��������:     ����ָ���
**�������:     --
**�������:
uint32_t overTime ��ʱʱ�� 
**��ע:
****************************************************************************************/
static void HAL_FaceId_CmdStart(uint32_t overTime)
{
    faceIdCtrl.com = eFaceIdComReady;
    faceIdCtrl.operate.OverTimr = overTime;
}
/***************************************************************************************
**������:       HAL_FaceId_Cmd
**��������:     �����ģ��
**�������:     --
**�������:
uint8_t cmd, ����
uint8_t waitStandBy, �ȴ�������ʼ����� 0���ȴ���ʼ�����;1�����ȴ� 
uint32_t overTime ��ʱʱ��
**��ע:
****************************************************************************************/
static TYPEe_FaceIdCom HAL_FaceId_Cmd(uint8_t cmd, uint8_t waitStandBy, uint32_t overTime)
{
    if (0 != faceIdCtrl.operate.OverTimr) //�Զ�������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //��ʱ
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:
        if ((eFaceIdStatusStandBy == faceIdCtrl.status) || (waitStandBy)) //��ʼ�����
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
            HAL_FaceId_CmdSend(cmd);
            faceIdCtrl.operate.OverTimr = overTime;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (cmd == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
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
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_FaceId_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_Init(void)
{
    DRV_FaceId_Init();
}
/***************************************************************************************
**������:       HAL_FaceId_ClosePow
**��������:     �ر�������Դ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_ClosePow(void)
{
    DRV_FaceId_ClosePow(); //�ر�ģ���Դ
    faceIdCtrl.status = eFaceIdStatusNone;
}
/***************************************************************************************
**������:       HAL_FaceId_CmdSend
**��������:     ��������
**�������:     
uint8_t mid mid
**�������:     --
**��ע:         
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
                           HAL_FACE_ID_SEND_TYPE_UNENCRYPT); //���ĵķ�ʽ����
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
**������:       HAL_FaceId_GetRecvReply
**��������:     ����reply���͵���Ϣ
**�������:     --
**�������:     --
**��ע:         
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
        //����û��׷�Ӱ�
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
**������:       HAL_FaceId_GetRecvNote
**��������:     ����Note���͵���Ϣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_GetRecvNote(void)
{
    DRV_FaceId_GetRecvNoteNid((uint8_t *)(&noteNid), sizeof(noteNid));
    switch (noteNid.nid)
    {
    case FACE_ID_NID_READY:
        // faceIdCtrl.readyFlag = 1;               //��ʼ�����
        // msgInitData.enableNetwork = 0;          //������
        // msgInitData.mode = 1;                   //���ķ�ʽ
        // memset(msgInitData.seed, 0, 4);         //��������Ĭ��ֵ��0
        // DRV_FaceId_GetAesKey(msgInitData.seed); //��ȡAES key
        // memset(msgInitData.crtTime, 0, 4);      //��������Ĭ��ֵ��0
        // msgInitData.waitingTime = 0;            //�ȴ�ʱ��Ϊ0
        // HAL_FaceId_CmdSend(FACE_ID_MID_INIT);   //ֱ�ӷ���
        // faceIdCtrl.encryptFlag = 1;             //����ʹ������ͨѶ
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
**������:       HAL_FaceId_GetRecvReply
**��������:     ����reply���͵���Ϣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_RevcDeal(void)
{
    switch (DRV_FaceId_RecvDataUnblock())
    {
    case 1: //�������,δ������ѭ����ȡ
        if (0 == DRV_FaceId_RecvDataAndAnalysis(faceIdCtrl.encryptFlag))
        {
            switch (DRV_FaceId_GetRecvMsgType()) //ֱ�ӽ�������
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
        DRV_FaceId_ClearRxPar(); //�������
        break;
    case -1:                     //��������
    case -2:                     //û������
    case -3:                     //���ղ�������
        DRV_FaceId_ClearRxPar(); //�������
        break;
    default:
        //		DRV_FaceId_ClearRxPar(); //�������
        break;
    }
}
/***************************************************************************************
**������:       HAL_FaceId_CmdPowerOn
**��������:     ����ʶ���ϵ�
**�������:     --
**�������:     --
**��ע:         
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
**������:       HAL_FaceId_CmdPowerOff
**��������:     ����ʶ���µ�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_CmdPowerOff(void)
{
	if(eFaceIdShutdownNone == faceIdCtrl.shotdownFlag)
	{
		faceIdCtrl.status = eFaceIdStatusPowerOff;
        faceIdCtrl.statusBackCmd = eFaceIdStatusNone; 
        faceIdCtrl.enableNetworkBack = 0; //�����־
		DEBUG_LOG_OUT("faceIdCtrl.status = eFaceIdStatusPowerOff;\n");
	}
    else
	{
		DEBUG_LOG_OUT("HAL_FaceId_CmdPowerOff shutdown ing....\n");
	}
}
/***************************************************************************************
**������:       HAL_FaceId_CmdPowerOffDir
**��������:     ����ʶ���µ�ֱ�ӹرյ�Դ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_CmdPowerOffDir(void)
{
    DEBUG_LOG_OUT("HAL_FaceId_CmdPowerOffDir");
    DRV_FaceId_ClosePow(); //�ر�������Դ
    M_CAM_RX_OUT_PP();
    M_CAM_RX_OUT_0();
    M_CAM_TX_OUT_PP();
    M_CAM_TX_OUT_0();
    faceIdCtrl.status = eFaceIdStatusWaitShutDown; 
}
/***************************************************************************************
**������:       HAL_FaceId_PowerDeal
**��������:     ����ʶ���Դ����
**�������:     --
**�������:     --
**��ע: 
�±�����è��ģ��Ӧ��ע�����
CHANGE
�����л�/è��ģ���ϵ�ʹ��
0���л�������ģ��
1���л���è��ģ��
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
        faceIdCtrl.statusBackCmd = eFaceIdStatusNone; //��պ�ָ̨��
        break;
    case eFaceIdStatusPowerOn:
        msgInitData.enableNetwork = faceIdCtrl.enableNetworkBack; //������־
        faceIdCtrl.enableNetworkBack = 0; //�����־
		faceIdCtrl.shotdownFlag = eFaceIdShutdownNone;
        DEBUG_LOG_OUT("emsgInitData.enableNetwork:%x\n",msgInitData.enableNetwork);
        #ifdef USE_FACE_AOBI //ʹ�ð±�����
        DRV_FaceId_ClosePow(); //��ģ��ϵ�
        DRV_Face_CateyeFaceSw(0); //�л�������
        #if 0
        DRV_Face_WifiPowClose(); //è�۶ϵ�
        #endif
        #endif
        #ifdef USE_FACE_SHANGTANG //ʹ����������
        DRV_FaceId_ClosePow(); //��ģ��ϵ�
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
                   sizeof(noteNid)); //��ս���
            HAL_FaceId_Init();       //��ʼ������
            #ifdef USE_FACE_AOBI //ʹ�ð±�����
            if (0 != msgInitData.enableNetwork)
            {
                DRV_Face_CateyeFaceSw(1); //�л���è��
                DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
            }
            else
            {
                DRV_Face_CateyeFaceSw(0); //�л�������
                DRV_FaceId_OpenPow();    //��ģ���ϵ�
            }
            #endif
            #ifdef USE_FACE_SHANGTANG //ʹ����������
            #if 1
            if (0 != msgInitData.enableNetwork)
            {
                DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
            }
            #endif
            DRV_FaceId_OpenPow();    //��ģ���ϵ�
            #endif
            faceIdCtrl.status = eFaceIdStatusWaitReady;
            DEBUG_LOG_OUT("faceIdCtrl.status = eFaceIdStatusWaitReady;\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.encryptFlag = 0; //����ͨѶ
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
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //��ʱ�������������⡣
            DRV_FaceId_ClosePow();                         //�ر�������Դ
        }
        break;
    case eFeceIdStatusGetDeiceId:
        
        msgInitData.mode = 1;                   //���ķ�ʽ
        memset(msgInitData.seed, 0, 4);         //��������Ĭ��ֵ��0
        DRV_FaceId_GetAesKey(msgInitData.seed); //��ȡAES key
        memset((uint8_t *)(&noteNid),
               HAL_FACE_ID_NOTE_DAFAULT,
               sizeof(noteNid));              //��ս���		
        
        HAL_FaceId_CmdSend(FACE_ID_MID_INIT); //ֱ�ӷ���
        faceIdCtrl.encryptFlag = 1;           //����ʹ������ͨѶ
        faceIdCtrl.waitReadyTim = 0;
        faceIdCtrl.status = eFeceIdStatusGetDeiceIdReply;
        DEBUG_LOG_OUT("eFeceIdStatusGetDeiceIdReply\n");
        break;
    case eFeceIdStatusGetDeiceIdReply:
        if (FACE_ID_MID_INIT == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                                     //�����豸ID��
				DEBUG_LOG_OUT("eFeceIdStatusGetDeiceIdReply ok\n");
                #if 0
                DEBUG_LOG_OUT("enter eFeceIdStatusDemoMod \n");
                faceIdCtrl.status = eFeceIdStatusDemoMod; //�����Ƿ�����ʾģʽ
                #endif
                #if 1
				if(1 == msgDemoModeoData.enable)
				{
					DEBUG_LOG_OUT("enter eFeceIdStatusDemoMod \n");
					faceIdCtrl.status = eFeceIdStatusDemoMod; //�����Ƿ�����ʾģʽ

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
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //��ʱ�������������⡣
            DRV_FaceId_ClosePow();                         //�ر�������Դ
        }
        break;
    case eFeceIdStatusDemoMod:
        HAL_FaceId_CmdSend(FACE_ID_MID_DEMOMODE); //ֱ�ӷ���
        faceIdCtrl.status = eFeceIdStatusDemoModReply;
        faceIdCtrl.waitReadyTim = 0;
        break;
    case eFeceIdStatusDemoModReply:
        if (FACE_ID_MID_DEMOMODE == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                //�����豸ID��
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
            faceIdCtrl.status = eFaceIdStatusErrorTimeOut; //��ʱ�������������⡣
            DRV_FaceId_ClosePow();                         //�ر�������Դ
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
		memset((uint8_t *)(&replyResult), 0, sizeof(replyResult)); //��ջظ�
		HAL_FaceId_CmdSend(FACE_ID_MID_RESET);
		faceIdCtrl.status = eFaceIdStatusPowerOffWaitRst;
		faceIdCtrl.shotdownFlag = eFaceIdShutdownIng;
		faceIdCtrl.waitReadyTim = 0;
		#else
		faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
		faceIdCtrl.waitReadyTim = 0;
		HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //ֱ�ӷ���
		DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
		#endif
        break;
	case eFaceIdStatusPowerOffWaitRst:
		if (FACE_ID_MID_RESET == replyResult.code) //���ճɹ�
        {
            msgInitData.enableNetwork = 0; //�������
            DEBUG_LOG_OUT("eFaceIdStatusPowerOffWaitRst replyResult.code:%x\n", replyResult.code);
            faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
            faceIdCtrl.waitReadyTim = 0;
			HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //ֱ�ӷ���
			DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
			break;
		}
		faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > 100)
        {
            msgInitData.enableNetwork = 0; //�������
            DEBUG_LOG_OUT("eFaceIdStatusPowerOffWaitRst Timeout\n");
            faceIdCtrl.status = eFaceIdStatusWaitPowerDown;
            faceIdCtrl.waitReadyTim = 0;
			HAL_FaceId_CmdSend(FACE_ID_MID_POWERDOWN); //ֱ�ӷ���
			DEBUG_LOG_OUT("msgPowerDownData.enableLowPower:%x\n", msgPowerDownData.enableLowPower);
			break;
        }
		break;
    case eFaceIdStatusWaitPowerDown:
        if (FACE_ID_MID_POWERDOWN == replyResult.code) //���ճɹ�
        {
            DEBUG_LOG_OUT("replyResult.code:%x\n", replyResult.code);
			DEBUG_LOG_OUT("replyResult.result:%x\n", replyResult.result);
            faceIdCtrl.status = eFaceIdStatusWaitShutDown;
            faceIdCtrl.waitReadyTim = 0;
            DEBUG_LOG_OUT("eFaceIdStatusWaitPowerDown -> FACE_ID_MID_POWERDOWN FACE_ID_MR_SUCCESS\n");
#if 0
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                //�����豸ID��
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
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_SHUT_DOWN_CMD_TIME_MAX) //��ʱ����Ϊ�豸��Ч
        {
            DEBUG_LOG_OUT("eFaceIdStatusWaitPowerDown Time over\n");
            faceIdCtrl.waitReadyTim = 0;
            faceIdCtrl.status = eFaceIdStatusPowerDownErrorTimeOut; //��ʱ�������������⡣
            DRV_FaceId_ClosePow();                                  //�ر�������Դ
            #ifdef USE_FACE_AOBI
            DRV_Face_CateyeFaceSw(0); //�л���è��,è�۵�Դ���Ͽ�
            #endif
                                                                    //������Ҫ�ṩ���ط��Ľӿ�
        }
        break;
    case eFaceIdStatusWaitShutDown:
        faceIdCtrl.waitReadyTim++;
        if (faceIdCtrl.waitReadyTim > HAL_FACE_ID_WAIT_SHUT_DOWN_TIME_MAX) //����ػ�ʱ��
        {
            DRV_FaceId_ClosePow(); //�ر�������Դ
            #ifdef USE_FACE_AOBI
            DRV_Face_CateyeFaceSw(0); //�л���è��,è�۵�Դ���Ͽ�
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
**������:       HAL_FaceId_AddUser
**��������:     ����ʶ������û�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_AddUserStart(void)
{
    faceIdCtrl.add.step = eFaceIdAddReady; //����
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
}
/***************************************************************************************
**������:       HAL_FaceId_AddUser
**��������:     ����ʶ������û�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_AddUserStop(void)
{
    faceIdCtrl.add.step = eFaceIdAddNone;
}

/***************************************************************************************
**������:       HAL_FaceId_AddUser
**��������:     ����ʶ������û�
**�������:     --
**�������:     
TYPEe_FaceIdAdd
������������ڵ�״̬
**��ע:         
****************************************************************************************/
TYPEe_FaceIdAdd HAL_FaceId_AddUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //��ʱ������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.add.step = eFaceIdAddTimeOver; //��ʱ
            DEBUG_LOG_OUT("HAL_FaceId_AddUser eFaceIdAddTimeOver\n");
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.add.step)
    {
    case eFaceIdAddNone:

        break;
    case eFaceIdAddReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            faceIdCtrl.add.step = eFaceIdAddMiddle; //��ʽ��ʼ�������
        }
        break;
    case eFaceIdAddMiddle:
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace));   //��ջظ�
        memset((uint8_t *)(&msgEnrollData), 0, sizeof(msgEnrollData)); //��ջظ�
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));     //��ջظ�
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
        if (FACE_ID_MID_ENROLL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                faceIdCtrl.operate.OverTimr = 0; //�رճ�ʱ������
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
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_UP;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddUPReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddUP\n");
        break;
    case eFaceIdAddUPReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                faceIdCtrl.operate.OverTimr = 0; //�رճ�ʱ������
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
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_DOWN;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddDownReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddDown\n");
        break;
    case eFaceIdAddDownReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                faceIdCtrl.operate.OverTimr = 0; //�رճ�ʱ������
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
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_RIGHT;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddRightReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddRight\n");
        break;
    case eFaceIdAddRightReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                faceIdCtrl.operate.OverTimr = 0; //�رճ�ʱ������
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
        memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
        memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
        msgEnrollData.faceDirection = FACE_ID_DIRECTION_LEFT;
        HAL_FaceId_CmdSend(FACE_ID_MID_ENROLL);
        faceIdCtrl.add.step = eFaceIdAddLeftReply;
        faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_ADD;
        faceIdCtrl.operate.OverTimrCnt = 0;
        DEBUG_LOG_OUT("eFaceIdAddLeft\n");
        break;
    case eFaceIdAddLeftReply:
        if (FACE_ID_MID_ENROLL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                faceIdCtrl.operate.OverTimr = 0; //�رճ�ʱ������
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
**������:       HAL_FaceId_VerifyUserStart
**��������:     ����ʶ��У���û���ʼ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_VerifyUserStart(void)
{
    #if 1
    faceIdCtrl.verify.step = eFaceIdVerifyReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
    #endif
}
/***************************************************************************************
**������:       HAL_FaceId_VerifyUserStop
**��������:     ����ʶ��У���û�ֹͣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_VerifyUserStop(void)
{
    faceIdCtrl.verify.step = eFaceIdVerifyNone;
    HAL_FaceId_ComStart(); //����ֹͣ����
    faceIdCtrl.operate.OverTimr = 0;
}

/***************************************************************************************
**������:       HAL_FaceId_VerifyUser
**��������:     ����ʶ��У���û�
**�������:     --
**�������:     
TYPEe_FaceIdVerify
���������ڵ�״̬
**��ע:         
****************************************************************************************/
TYPEe_FaceIdVerify HAL_FaceId_VerifyUser(void)
{
    if ((0 != faceIdCtrl.operate.OverTimr) &&
        (eFaceIdVerifyNone != faceIdCtrl.verify.step)) //��ʱ������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.verify.step = eFaceIdVerifyTimeOut; //��ʱ
            DEBUG_LOG_OUT("HAL_FaceId_VerifyUser eFaceIdAddTimeOver\n");
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.verify.step)
    {
    case eFaceIdVerifyNone:

        break;
    case eFaceIdVerifyReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            memset((uint8_t *)(&msgVerifyData), 0, sizeof(msgVerifyData));           //���ָ��
            memset((uint8_t *)(&msgReplyVerifyData), 0, sizeof(msgReplyVerifyData)); //��ջظ�
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace));             //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));               //��ջظ�
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
        if (FACE_ID_MID_VERIFY == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
                //��ȡ���������Ѿ���msgReplyVerifyData
                faceIdCtrl.verify.step = eFaceIdVerifyOk;
                DEBUG_LOG_OUT("eFaceIdVerifyReply ok\n");
                break;
			case FACE_ID_MR_FAILED4_TIMEOUT: //��ⳬʱ
				DEBUG_LOG_OUT("HAL_FaceId_VerifyUser eFaceIdVerifyTimeOut\n");
				faceIdCtrl.verify.step = eFaceIdVerifyTimeOut;
				break;
            default:
                faceIdCtrl.verify.step = eFaceIdVerifyFail;
                DEBUG_LOG_OUT("eFaceIdVerifyFail\n");
                break;
            }
        }
		//������note
		
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
**������:       HAL_FaceId_DelOneUserStart
**��������:     ɾ��һ��ָ��
**�������:     
uint16_t userId �û�ID
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_DelOneUserStart(uint16_t userId)
{

    memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData)); //���ָ��
    msgDeluserData.userIdHeb = (uint8_t)(userId >> 8);
    msgDeluserData.userIdLeb = (uint8_t)(userId);
    faceIdCtrl.delOne.step = eFaceIdDelOneReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX+HAL_FACE_ID_WAIT_MODE_TIME_MAX+HAL_FACE_ID_WAIT_RST_TIME_MAX;
}
/***************************************************************************************
**������:       HAL_FaceId_DelOneUserStop
**��������:     ɾ��һ��ָ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_DelOneUserStop(void)
{
    faceIdCtrl.delOne.step = eFaceIdDelOneNone;
}
/***************************************************************************************
**������:       HAL_FaceId_DelOneUser
**��������:     ����ʶ��ɾ��һ���û�
**�������:     --
**�������:     
TYPEe_FaceIdDelOne
���������ڵ�״̬
**��ע:         
****************************************************************************************/
TYPEe_FaceIdDelOne HAL_FaceId_DelOneUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //�Զ�������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.delOne.step = eFaceIdDelOneTimeOut; //��ʱ
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.delOne.step)
    {
    case eFaceIdDelOneNone:
        break;
    case eFaceIdDelOneReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //���ָ��
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
            HAL_FaceId_CmdSend(FACE_ID_MID_DELUSER);
            DEBUG_LOG_OUT("eFaceIdDelOneReady\n");
            faceIdCtrl.delOne.step = eFaceIdDelOneReply;
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME;
            faceIdCtrl.operate.OverTimrCnt = 0;
        }
        break;
    case eFaceIdDelOneReply:
        if (FACE_ID_MID_DELUSER == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
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
**������:       HAL_FaceId_DelAllUserStart
**��������:     ɾ��ȫ��ָ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_DelAllUserStart(void)
{
    faceIdCtrl.delAll.step = eFaceIdDelAllReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME + HAL_FACE_ID_WAIT_STDNDBY_OK_TIME_MAX;
}
/***************************************************************************************
**������:       HAL_FaceId_DelAllUserStop
**��������:     ɾ��ȫ��ָ��ֹͣ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_DelAllUserStop(void)
{
    faceIdCtrl.delAll.step = eFaceIdDelAllNone;
}
/***************************************************************************************
**������:       HAL_FaceId_DelAllUser
**��������:     ����ʶ��ɾ��ȫ���û�
**�������:     --
**�������:     
TYPEe_FaceIdDelAll
���������ڵ�״̬
**��ע:         
****************************************************************************************/
TYPEe_FaceIdDelAll HAL_FaceId_DelAllUser(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //�Զ�������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.delAll.step = eFaceIdDelAllTimeOut; //��ʱ
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.delAll.step)
    {
    case eFaceIdDelAllNone:
        break;
    case eFaceIdDelAllReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //���ָ��
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
            HAL_FaceId_CmdSend(FACE_ID_MID_DELALL);
            DEBUG_LOG_OUT("eFaceIdDelAllReady\n");
            faceIdCtrl.delAll.step = eFaceIdDelAllReply;
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_OVER_TIME;
            faceIdCtrl.operate.OverTimrCnt = 0;
        }
        break;
    case eFaceIdDelAllReply:
        if (FACE_ID_MID_DELALL == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
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
**������:       HAL_FaceId_SetDemoMode
**��������:     ����demoģʽ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_SetDemoMode(void)
{
    HAL_FaceId_CmdSend(FACE_ID_MID_DEMOMODE);
}
/***************************************************************************************
**������:       HAL_Face_EnterSleepConfig
**��������:     ����͹�������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Face_EnterSleepConfig(void)
{
    DRV_Face_EnterSleepConfig();
}
/***************************************************************************************
**������:       HAL_FaceId_ComStart
**��������:     ��ʼͨ��ָ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_ComStart(void)
{
    faceIdCtrl.com = eFaceIdComReady;
}
/***************************************************************************************
**������:       HAL_FaceId_Reset
**��������:     ֹͣ��ǰ���ڴ������Ϣ��ģ��������״̬
**�������:     --
**�������:
TYPEe_FaceIdCom
���������ڵ�״̬
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_Reset(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //�Զ�������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //��ʱ
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:

        //       if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            //        memset((uint8_t *)(&msgDeluserData), 0, sizeof(msgDeluserData));           //���ָ��
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
            HAL_FaceId_CmdSend(FACE_ID_MID_RESET);
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (FACE_ID_MID_RESET == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
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
**������:       HAL_FaceId_GetVersionStart
**��������:     ��ʼ��ȡ�����汾ָ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_GetVersionStart(void)
{
    faceIdCtrl.com = eFaceIdComReady;
    faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
}
/***************************************************************************************
**������:       HAL_FaceId_GetVersion
**��������:     ��ȡ�汾
**�������:     --
**�������:
TYPEe_FaceIdCom
���������ڵ�״̬
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_GetVersion(void)
{
    if (0 != faceIdCtrl.operate.OverTimr) //�Զ�������
    {
        if (1 == faceIdCtrl.operate.OverTimr)
        {
            faceIdCtrl.com = eFaceIdComTimeOut; //��ʱ
        }
        faceIdCtrl.operate.OverTimr--;
    }
    switch (faceIdCtrl.com)
    {
    case eFaceIdComNone:
        break;
    case eFaceIdComReady:
        if (eFaceIdStatusStandBy == faceIdCtrl.status) //��ʼ�����
        {
            DEBUG_LOG_OUT("faceIdCtrl.status:%d\n", faceIdCtrl.status);
            memset((uint8_t *)(&noteDataFace), 0, sizeof(noteDataFace)); //��ջظ�
            memset((uint8_t *)(&replyResult), 0, sizeof(replyResult));   //��ջظ�
            HAL_FaceId_CmdSend(FACE_ID_MID_GET_VERSION);
            faceIdCtrl.operate.OverTimr = HAL_FACE_ID_WAIT_RECE_TIME_200MS;
            DEBUG_LOG_OUT("eFaceIdComReady\n");
            faceIdCtrl.com = eFaceIdComReply;
        }
        break;
    case eFaceIdComReply:

        if (FACE_ID_MID_GET_VERSION == replyResult.code) //���ճɹ�
        {
            switch (replyResult.result)
            {
            case FACE_ID_MR_SUCCESS: //���ճɹ�
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
**������:       HAL_FaceId_ConfigWifiDataStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_ConfigWifiDataStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_ConfigWifiDataStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_ConfigWifiData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_CONFIG_WIFI,
                          0,
                          200); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}
/***************************************************************************************
**������:       HAL_FaceId_GetStatus
**��������:     ����ʶ����״̬
**�������:     --
**�������:     
TYPEe_FaceIdStatus
**��ע:         
****************************************************************************************/
TYPEe_FaceIdStatus HAL_FaceId_GetStatus(void)
{
    return faceIdCtrl.status;
}

/***************************************************************************************
**������:       HAL_FaceId_CmdStop
**��������:     ָֹͣ���
**�������:     --
**�������:		--
**��ע:
****************************************************************************************/
void HAL_FaceId_CmdStop(void)
{
    faceIdCtrl.com = eFaceIdComNone;
    faceIdCtrl.operate.OverTimr = 0;
    faceIdCtrl.cmd = eFaceIdCmdNone;
}

/***************************************************************************************
**������:       HAL_FaceId_SetLinkKeyStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_SetLinkKeyStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_SetLinkKey
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_SetLinkKey(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_SET_LINKKEY,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**������:       HAL_FaceId_GetDeviceNameStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_GetDeviceNameStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_GetDeviceName
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_GetDeviceName(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_DEVICE_NAME,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}
/***************************************************************************************
**������:       HAL_FaceId_DataPassThroughStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_DataPassThroughStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_DataPassThrough
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_DataPassThrough(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_DATA_PASSTHROUGH,
                          0,
                          50); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**������:       Comm_WifiDataPack
**��������:     ���ݴ��
**�������:    
uint16_t len  //Ҫ�������ݵĳ���
**�������:     
uint16_t
��������ݵĳ���
**��ע:         ����������й���С�˵�ת������Ҫȷ�ϳ����Ƿ����
****************************************************************************************/
static uint16_t Comm_WifiDataPack(uint8_t *pDat, uint16_t len)
{
    uint16_t lenCnt = 0;
    //	uint16_t chkSum =0;
    wifiProtocol.front.len = len; //��д���ݳ���
    BSWAP_SAVE_16(wifiProtocol.front.len);
    memcpy(pDat,
           (uint8_t *)(&wifiProtocol.front),
           sizeof(wifiProtocol.front)); //����ͷ
    lenCnt = sizeof(wifiProtocol.front);
    lenCnt += len; //��ȡ���ݳ���
    wifiProtocol.chk = BasicFunc_GetSumChk((pDat + sizeof(wifiProtocol.front.header)),
                                           lenCnt - sizeof(wifiProtocol.front.header)); //����У���
    BSWAP_SAVE_16(wifiProtocol.chk);
    memcpy((uint8_t *)(pDat + lenCnt),
           &wifiProtocol.chk,
           sizeof(wifiProtocol.chk));   //�������
    lenCnt += sizeof(wifiProtocol.chk); //����У��͵ĳ���
    return lenCnt;
}
/***************************************************************************************
**������:       Comm_WifiCmdSend
**��������:     wifi���ݷ���
**�������:    
TYPEe_WifiCmd cmd ָ��
**�������:     
uint16_t
��������ݵĳ���
**��ע:         
****************************************************************************************/
uint16_t Comm_WifiCmdSend(TYPEe_WifiCmd cmd)
{
    uint16_t lenTmp = 0;
    wifiProtocol.front.header = COMM_WIFI_HEADER;         //�������ͷ
    wifiProtocol.front.type = eWifiProtocolTypeCmdNotAck; //���ָ������
    wifiProtocol.front.cmd = cmd;                         //��д������
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
**������:       Comm_WifiCmdSend
**��������:     wifi���ݷ���
**�������:    
TYPEe_WifiCmd cmd ָ��
TYPEe_WifiProtocolType tp wifi��Ϣ����
uint16_t len ���峤��
**�������:     
uint16_t
��������ݵĳ���
**��ע:         
****************************************************************************************/
uint16_t Comm_WifiCmdSendWithLen(TYPEe_WifiCmd cmd,TYPEe_WifiProtocolType tp,uint16_t len)
{
    uint16_t lenTmp = 0;
    wifiProtocol.front.header = COMM_WIFI_HEADER;         //�������ͷ
    wifiProtocol.front.type = tp; //���ָ������
    wifiProtocol.front.cmd = cmd;                         //��д������
    return Comm_WifiDataPack((uint8_t *)(WIFI_OUT_BUFF_ADDR), len);
}
/***************************************************************************************
**������:       HAL_FaceId_TriggerCaptureDataStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_TriggerCaptureDataStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_TriggerCaptureData
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_TriggerCaptureData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_TRIGGER_CAPTURE,
                          0,
                          100); //HAL_FACE_ID_WAIT_RECE_TIME_200MS
}

/***************************************************************************************
**������:       HAL_FaceId_VideoCallDataStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_VideoCallDataStart(void)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    HAL_FaceId_CmdStart(0);
}
/***************************************************************************************
**������:       HAL_FaceId_VideoCallData
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
TYPEe_FaceIdCom HAL_FaceId_VideoCallData(void)
{
    return HAL_FaceId_Cmd(FACE_ID_MID_TRIGGER_VIDEOCALL,
                          0,
                          100);
}

/***************************************************************************************
**������:       HAL_FaceId_CmdDealStart
**��������:     
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void HAL_FaceId_CmdDealStart(TYPEe_FaceIdCmd cmd, uint32_t overTime)
{
    DRV_Face_WifiPowOpen(); //��wifi�ĵ�Դ
    faceIdCtrl.cmd = cmd;
    HAL_FaceId_CmdStart(overTime);
	DEBUG_LOG_OUT("HAL_FaceId_CmdDealStart\n");
}
/***************************************************************************************
**������:       HAL_FaceId_CmdDeal
**��������:     
**�������:     
uint8_t cmd, ����
uint8_t waitStandBy, �ȴ�������ʼ����� 0���ȴ���ʼ�����;1�����ȴ� 
uint32_t overTime ��ʱʱ��
**�������:     --
**��ע:
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
**������:       HAL_FaceId_ChkBusy
**��������:     ��ѯ����ʱ��æµ�����
**�������:     --
**�������:     
int8_t
0 ����
-1 æµ
**��ע:
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
**������:       HAL_FaceId_ChkBusy
**��������:     ��ѯ������Դæµ�����
**�������:     --
**�������:     
int8_t
0 ����
-1 æµ
**��ע:
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
**������:       HAL_Face_WifiPowClose
**��������:     ����wifi��Դ�ر� 
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Face_WifiPowClose(void)
{
    DRV_Face_WifiPowClose();
}

/***************************************************************************************
**������:       HAL_FaceId_IRSleepConfig
**��������:     IR�����Ӧ��������
**�������:     
uint8_t sw		0,�ر��жϣ���0�������ж�
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_FaceId_IRSleepConfig(uint8_t sw)
{
	DRV_FaceId_IRSleepConfig(sw);
}
#endif /*HAL_FACE_ID_MODULE_EN*/
/************************************Code End******************************************/
