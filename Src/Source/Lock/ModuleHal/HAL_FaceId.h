/***************************************************************************************
**�ļ���:     HAL_FaceId.h
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
#ifndef __HAL_FACE_ID_H__
#define __HAL_FACE_ID_H__
#include "MyConfig.h"
#define HAL_FACE_ID_MODULE_EN (1) //ģ��ʹ��
#ifdef DRV_FACE_ID_ENCRYPT_EN
#define HAL_FACE_ID_ENCRYPT_EN
#endif
//#define USE_HAL_FACE_POWER_ON (1) //ʹ��HAL����ϵ�ӿ�
//#define USE_HAL_FACE_POWER_OFF (1) //ʹ��HAL����ϵ�ӿ�
#if HAL_FACE_ID_MODULE_EN
/*************************ר�ú궨��*************************/
#define HAL_FACE_ID_WAIT_RECE_TIME_VERSOIN (25) //��ȡ�汾,�ȴ�ʱ��.
#define USER_NAME_SIZE 32
#define COMM_WIFI_HEADER (0xFE) //wifiͨѶ����ͷ
/***********************************************************
 * ����״̬(state)
 * ********************************************************/
#define FACE_ID_STATE_NORMAL 0                     //��������
#define FACE_ID_STATE_NOFACE 1                     //δ��⵽����
#define FACE_ID_STATE_TOOUP 2                      //����̫����ͼƬ�ϱ��أ�δ��¼��
#define FACE_ID_STATE_TOODOWN 3                    //����̫����ͼƬ�±��أ�δ��¼��
#define FACE_ID_STATE_TOOLEFT 4                    //����̫����ͼƬ����أ�δ��¼��
#define FACE_ID_STATE_TOORIGHT 5                   //����̫����ͼƬ�ұ��أ�δ��¼��
#define FACE_ID_STATE_FAR 6                        //��������̫Զ��δ��¼��
#define FACE_ID_STATE_CLOSE 7                      //��������̫����δ��¼��
#define FACE_ID_STATE_EYEBROW_OCCLUSION 8          //üë�ڵ�
#define FACE_ID_STATE_EYE_OCCLUSION 9              //�۾��ڵ�
#define FACE_ID_STATE_FACE_OCCLUSION 10            //�����ڵ�
#define FACE_ID_STATE_DIRECTION_ERROR 11           //¼�������������
#define FACE_ID_STATE_EYE_CLOSE_STATUS_OPEN_EYE 12 //�ڱ���ģʽ��⵽����״̬
#define FACE_ID_STATE_EYE_CLOSE_STATUS 13          //����״̬
#define FACE_ID_STATE_EYE_CLOSE_UNKNOW_STATUS 14   //�ڱ���ģʽ������޷��ж�������״̬
/***********************************************************
 * result
 * ********************************************************/
#define FACE_ID_MR_SUCCESS 0                //�ɹ�
#define FACE_ID_MR_REJECTED 1               //ģ��ܾ�������
#define FACE_ID_MR_ABORTED 2                //¼��/�����㷨����ֹ
#define FACE_ID_MR_FAILED4_CAMERA 4         //�����ʧ��
#define FACE_ID_MR_FAILED4_UNKNOWNREASON 5  //δ֪����
#define FACE_ID_MR_FAILED4_INVALIDPARAM 6   //��Ч�Ĳ���
#define FACE_ID_MR_FAILED4_NOMEMORY 7       //�ڴ治��
#define FACE_ID_MR_FAILED4_UNKNOWNUSER 8    //û����¼����û�
#define FACE_ID_MR_FAILED4_MAXUSER 9        //¼�볬������û�����
#define FACE_ID_MR_FAILED4_FACEENROLLED 10  //������¼��
#define FACE_ID_MR_FAILED4_LIVENESSCHECK 12 //������ʧ��
#define FACE_ID_MR_FAILED4_TIMEOUT 13       //¼��������ʱ
#define FACE_ID_MR_FAILED4_AUTHORIZATION 14 //����оƬ��Ȩʧ��
#define FACE_ID_MR_FAILED4_READ_FILE 19     //���ļ�ʧ��
#define FACE_ID_MR_FAILED4_WRITE_FILE 20    //д�ļ�ʧ��
#define FACE_ID_MR_FAILED4_NO_ENCRYPT 21    //ͨ��Э��δ����
#define FACE_ID_MR_FAILED4_NO_RGBIMAGE 23   //RGB ͼƬδ׼����
/***********************************************************
 * NOTE
 * ********************************************************/
#define FACE_ID_NID_READY 0           //ģ�����������������״̬
#define FACE_ID_NID_FACE_STATE 1      //�㷨ִ�гɹ������ҷ���������Ϣ
#define FACE_ID_NID_UNKNOWNERROR 2    //δ֪����
#define FACE_ID_NID_OTA_STATE 3       //OTA
#define FACE_ID_NID_EYE_STATE 4       //����������������״̬
#define FACE_ID_NID_MEDIA_CAP_STATE 5 //ý������
#define FACE_ID_NID_MEDIA_STATE 6     //Media ҵ��
/*************************�����ṹ����*************************/
typedef enum
{
    eFaceIdAddNone,        //����
    eFaceIdAddReady,       //׼����ʼ
    eFaceIdAddMiddle,      //��������м�
    eFaceIdAddMiddleReply, //�ȴ���������м����
    eFaceIdAddMiddleOK,    //��������м����
    eFaceIdAddUP,          //���������
    eFaceIdAddUPReply,     //�ȴ�������������
    eFaceIdAddUPOK,        //��������м����
    eFaceIdAddDown,        //���������
    eFaceIdAddDownReply,   //�ȴ�������������
    eFaceIdAddDownOK,      //������������
    eFaceIdAddLeft,        //���������
    eFaceIdAddLeftReply,   //�ȴ�������������
    eFaceIdAddLeftOK,      //������������
    eFaceIdAddRight,       //���������
    eFaceIdAddRightReply,  //�ȴ�������������
    eFaceIdAddRightOK,     //������������
    eFaceIdAddOk,          //������
    eFaceIdAddFail,        //������
    eFaceIdAddTimeOver,    //��ʱ
} TYPEe_FaceIdAdd;         //�������
typedef enum
{
    eFaceIdVerifyNone,    //����
    eFaceIdVerifyReady,   //׼����ʼ
    eFaceIdVerifyReply,   //�ȴ��ظ�
    eFaceIdVerifyOk,      //��֤�ɹ�
    eFaceIdVerifyFail,    //��֤ʧ��
    eFaceIdVerifyTimeOut, //��ʱ
	eFaceIdVerifyFailLiveNessCheck, //������ʧ��
} TYPEe_FaceIdVerify;     //��֤����

typedef enum
{
    eFaceIdDelOneNone,    //����
    eFaceIdDelOneReady,   //׼����ʼ
    eFaceIdDelOneReply,   //�ȴ��ظ�
    eFaceIdDelOneOk,      //�ɹ�
    eFaceIdDelOneFail,    //ʧ��
    eFaceIdDelOneTimeOut, //��ʱ
} TYPEe_FaceIdDelOne;     //ɾ��һ��ָ������

typedef enum
{
    eFaceIdDelAllNone,    //����
    eFaceIdDelAllReady,   //׼����ʼ
    eFaceIdDelAllReply,   //�ȴ��ظ�
    eFaceIdDelAllOk,      //�ɹ�
    eFaceIdDelAllFail,    //ʧ��
    eFaceIdDelAllTimeOut, //��ʱ
} TYPEe_FaceIdDelAll;     //ɾ��ȫ��ָ������
typedef enum
{
    eFaceIdComNone,     //����
    eFaceIdComReady,    //׼����ʼ
    eFaceIdComReply,    //�ȴ��ظ�
    eFaceIdComOk,       //�ɹ�
    eFaceIdComFail,     //ʧ��
    eFaceIdComTimeOut,  //��ʱ
    eFaceIdComCmdError, //���ƥ��
	eFaceIdComCmdTaskIng, //�ȴ��������
	eFaceIdComCmdTaskOk, //�ȴ��������
} TYPEe_FaceIdCom;      //ͨ��ͨѶ��ʽ
typedef enum
{
    eFaceIdCmdNone = 0x00, //ָ��Ϊ��
    eFaceIdCmdTriggerCaptureData = 0x19, //ץ��
	eFaceIdCmdStartOTA = 0X40, //��ʼOTA
    eFaceIdCmdVideoCallData = 0x84, //��Ƶ�Խ�
    eFaceIdCmdGetModuleStatus = 0x86, //��ȡģ��״̬
    eFaceIdCmdDataPassThrough = 0x83, //����͸��
	eFaceIdCmdSetThresholdLevel = 0xD4, //�޸�����ʶ��ȫ�ȼ�
    eFaceIdCmdDeleteMediaFileData = 0x27, //ɾ��ý���ļ�
} TYPEe_FaceIdCmd;                    //ͨѶ����

typedef enum
{
    eFaceIdShutdownNone = 0x00, //����
    eFaceIdShutdownIng = 0x01, //�ػ���
	eFaceIdShutdownYes = 0x02, //�Ѿ��ػ����
} TYPEe_FaceIdShutdown;                   
#pragma pack(1)                       //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
typedef struct
{
    uint8_t seed[4];       // һ�� 4 �ֽڵ������
    uint8_t mode;          //����ģʽ��0������ 1��AES ���ܣ�Ĭ�ϣ�
    uint8_t crtTime[4];    //��ǰʱ�䣬����ͬ��ģ���ϵͳʱ��
    uint8_t enableNetwork; //�Ƿ����� Enable��1;Disable��0
    uint8_t waitingTime;   //��ҵ����ɺ�ĵȴ�ʱ��
    uint8_t reserved[5];   //�����ֶ�
} TYPEs_MsgInitData;       //MID_INIT code:����0x50 ����+è��0x25
typedef struct
{
    uint8_t deviceId[20]; //�豸 ID����ģ�����Ķ�ά��һ�£�
} TYPEs_MsgReplyInitData; //MID_INIT REPLY code:����0x50 ����+è��0x25
typedef struct
{
    uint8_t pdRightAway; //�����ɹ����Ƿ����̶ϵ� yes:1 no:0
    uint8_t timeOut;     //������ʱʱ�� ��λ:s
} TYPEs_MsgVerifyData;   //MID_VERIFY code:0x12
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;      //����֤�û��� ID
    uint8_t userName[32];   //�û�����
    uint8_t admin;          //�Ƿ�Ϊ����Ա
    uint8_t unlockStatus;   //����������������״̬
} TYPEs_MsgReplyVerifyData; //MID_VERIFY code:0x12
typedef uint8_t TYPEs_FaceDir;
//const uint8_t FACE_DIRECTION_UP = 0x10;       // face up
//const uint8_t FACE_DIRECTION_DOWN = 0x08;     // face down
//const uint8_t FACE_DIRECTION_LEFT = 0x04;     // face left
//const uint8_t FACE_DIRECTION_RIGHT = 0x02;    // face right
//const uint8_t FACE_DIRECTION_MIDDLE = 0x01;   // face middle
//const uint8_t FACE_DIRECTION_UNDEFINE = 0x00; // face undefined
typedef struct
{
    uint8_t admin;                    //�Ƿ�����Ϊ����Աyes:1 no:0
    uint8_t userName[USER_NAME_SIZE]; //¼���û�����
    TYPEs_FaceDir faceDirection;      //�û���Ҫ¼��ķ���
    uint8_t timeOut;                  //¼�볬ʱʱ�� ��λ:s
} TYPEs_MsgEnrollData;                //MID_ENROLL 0x13
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;           //��ע���û��� ID
    TYPEs_FaceDir faceDirection; //��������������¼��״̬
} TYPEs_MsgReplyEnrollData;      //MID_ENROLL 0x13
typedef struct
{
    uint8_t imageCounts;  //ץ��ͼƬ������
    uint8_t startNumber;  //ץ��ͼƬ����ʼ��ţ�1-50��
} TYPEs_MsgSnapImageData; //MID_SNAPIMAGE 0x16
typedef struct
{
    uint8_t imageNumber;      //������ͼƬ�ı��
} TYPEs_MsgGetSavdeImageData; //MID_GETSAVEDIMAGE 0x17
typedef struct
{
    uint8_t imageSize[4];          //���ϴ�ͼƬ��С��image_size[0]Ϊ���8λ
} TYPEs_MsgReplyGetSavdeImageData; //MID_GETSAVEDIMAGE 0x17
typedef struct
{
    uint8_t uploadImageOffset[4]; //���ϴ�ͼƬ��ƫ����
    uint8_t uploadImageSize[4];   //�˴��ϴ�ͼƬ�Ĵ�С�����4K
} TYPEs_MsgUploadImageData;       //MID_UPLOADIMAGE 0x18
typedef struct
{
    uint8_t mediaType;          //ý������ 0:VIDEO 1:AUDIO
    uint32_t captureTime;       //����ʱ�� ��λ:s
    uint8_t enableCloudStorage; //�Ƿ�ʹ���ƶ˴洢 1:enable 0:disable
    uint8_t videoCaptureType;   //��Ƶ��ץ�����ͣ���media_type Ϊ VIDEOʱ��Ч
} TYPEs_MsgTriggerCaptureData;  //MID_TRIGGER_CAPTURE 0x19
typedef struct
{
    uint8_t mediaType;          //ý������ 0:VIDEO 1:AUDIO
    uint8_t force;              //�Ƿ�ǿ��ɾ��1���� 0����
} TYPEs_MsgDeleteMediaFileData; //MID_DELETE_MEDIA_FILE 0x27
typedef struct
{
    uint8_t volume;    //���ò�������
} TYPEs_MsgVolumeData; //MID_SET_VOLUME 0x1F
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;  //��ɾ���û��� ID
} TYPEs_MsgDeluserData; //MID_DELUSER 0x20

typedef struct
{
    uint8_t vPrimary;    //�̼��汾�����λ
    uint8_t vSecondary;  //�̼��汾���м�λ
    uint8_t vRevision;   //�̼��汾�����λ
} TYPEs_MsgStartOtaData; //MID_START_OTA 0x40 �����汾
typedef struct
{
    uint8_t baudrateIndex; //������ 1:115200 2:230400 3:460800 4:1500000
} TYPEs_MsgConfigBaudrate; //MID_CONFIG_BAUDRATE 0x51
typedef struct
{
    uint8_t ssid[33];      //WiFi ����
    uint8_t psk[65];       //WiFi ����
} TYPEs_MsgConfigWifiData; //MID_CONFIG_WIFI 0x70
typedef struct
{
    uint8_t address[6];    //MAC ��ַ
} TYPEs_MsgGetWifiMacData; //MID_GET_WIFIMAC 0x71
typedef struct
{
    uint8_t connectStatus;         //1:wifi�Ѿ����� ����:δ����
    int8_t signalLevel;            //һ���ź�ǿ����-30~-120 ֮�䡣�����ź�ǿ��Ӧ����-40 dbm ~ -85 dbm ֮ �䡣С�� -90 dbm �ͺܲ��ˣ�����û������
    int8_t noise;                  //����
} TYPEs_MsgReplyGetWifiStatusData; //MID_GET_WIFISTATUS 0x72
typedef struct
{
    uint8_t ProductKey[32];
    uint8_t productSecret[64];
    uint8_t deviceName[32];
    uint8_t deviceSecret[64]; //��������Կ
} TYPEs_MsgLinkkeyData;       //MID_SET_LINKKEY 0x82
typedef struct
{
    uint8_t data[128];      //����͸��
} TYPEs_MsgDataPassThrough; //MID_DATA_PASSTHROUG H 0x83

typedef struct
{
    uint32_t captureTime; //����ץ��ʱ��
    uint8_t enableRecord; //�Ƿ���������Ƶץ�� 1��enbale 0��disable
    uint8_t reserved[3];  //�����ֶ�
} TYPEs_MsgVideoCallData; //MID_TRIGGE R_VIDEOCAL L 0x84

typedef struct
{
    uint8_t name[32];
} TYPEs_MsgReplyDeviceName; //MID_GET_DEVICENAME 0x85
typedef enum
{
    eMediaStateIdle = 0,
    eMediaStateWaiting = 1,
    eMediaStateWorking = 2,
    eMediaStateError = 3,
} TYPEe_MediaState;

typedef struct
{
    uint8_t state;
    TYPEe_MediaState mediaState;
    uint8_t mediaError;
    uint8_t unlockState;         //����ҵ������״̬��1������ҵ�������У�0������ҵ��δ������
} TYPEs_MsgReplyGetModuleStatus; // 0x86
typedef struct
{
    uint8_t enableLowPower; //ģ��׼���ػ���������ǰ�ڴ������������ļ�ϵͳ���棬ָ�� wifi �Ƿ����ͱ���ģʽ��0���������ͱ��� 1�������ͱ���
} TYPEs_MsgPowerDownData;   //MID_POWER DOWN 0xED
typedef struct
{
    uint8_t enable;       //����/�˳� debug ģʽ
} TYPEs_MsgDebugModeData; //MID_DEBUG_MODE 0xF0
typedef struct
{
    uint8_t uploadDebugInfoOffset[4]; //���ϴ� debug ��Ϣ��ƫ����
    uint8_t uploadDebugInfoSize[4];   //���ϴ� debug ��Ϣ�Ĵ�С����� 4K
} TYPEs_MsgUploadDebugInfoData;       //MID_UPLOAD_DEBUG_INF 0xF2
typedef struct
{
    uint8_t enable;       //enable:1 disable: 0
} TYPEs_MsgDemoModeoData; //MID_DEMOMODE 0xFE

typedef struct
{
    uint8_t version[32];    //
} TYPEs_MsgReplyVersionGet; //MID_GET_VERSION	0x30

typedef struct
{
    uint8_t recognition; //�Ա�
	uint8_t biopsy;	//����
} TYPEs_MsgSetThresholdLevel; //SET_THRESHOLD_LEVEL	0xD4
/************************************************************
�� 4-13
����״̬(state)	code	˵��
FACE_STATE_NORMAL	0	��������
FACE_STATE_NOFACE	1	δ��⵽����
FACE_STATE_TOOUP	2	����̫����ͼƬ�ϱ��أ�δ��¼��
FACE_STATE_TOODOWN	3	����̫����ͼƬ�±��أ�δ��¼��
FACE_STATE_TOOLEFT	4	����̫����ͼƬ����أ�δ��¼��
FACE_STATE_TOORIGHT	5	����̫����ͼƬ�ұ��أ�δ��¼��
FACE_STATE_FAR	6	��������̫Զ��δ��¼��
FACE_STATE_CLOSE	7	��������̫����δ��¼��
FACE_STATE_EYEBROW_OCCLUSIO
N	8	üë�ڵ�
FACE_STATE_EYE_OCCLUSION	9	�۾��ڵ�
FACE_STATE_FACE_OCCLUSION	10	�����ڵ�
FACE_STATE_DIRECTION_ERROR	11	¼�������������
FACE_STATE_EYE_CLOSE_STATUS_
OPEN_EYE	12	�ڱ���ģʽ��⵽����״̬
FACE_STATE_EYE_CLOSE_STATUS	13	����״̬
FACE_STATE_EYE_CLOSE_UNKNOW
_STATUS	14	�ڱ���ģʽ������޷��ж�������״̬
*************************************************************/
typedef struct
{
    int16_t state;    //������ǰ״̬������� 4-13
    int16_t left;     //���������ͼƬ�����ľ��루������ʾ�������ѳ���ͼƬ����ࣩ
    int16_t top;      //���������ͼƬ���Ϸ��ľ��루������ʾ�������ѳ���ͼƬ�Ϸ���
    int16_t right;    //���������ͼƬ���Ҳ�ľ��루������ʾ�������ѳ�ͼƬ���Ҳࣩ
    int16_t bottom;   //���������ͼƬ���� ���ľ��루������ʾ�������ѳ���ͼƬ���·���
    int16_t yaw;      //yaw Ϊ����ʾ����תͷ��yaw Ϊ����ʾ����תͷ,�ο������ʾ��ͼ 4-1
    int16_t pitch;    //pitch Ϊ����ʾ����̧ͷ��pitch Ϊ����ʾ���µ�ͷ,�ο������ʾ��ͼ 4-1
    int16_t roll;     //roll Ϊ����ʾ������ͷ��roll Ϊ����ʾ������ͷ,�ο������ʾ��ͼ 4-1
} TYPEs_NoteDataFace; //�㷨ִ�гɹ������ҷ���������Ϣ

typedef struct
{
    uint8_t code;
    uint8_t result;
} TYPEs_ReplyResult; //��ȡִ�н��

typedef struct
{
    uint8_t nid;
} TYPEs_NoteNid; //��ȡִ�н��
/*********************************
OTA ״̬���ο����
4-14-3
OTA_STATE_IDLE	0	OTA ����״̬
OTA_STATE_WORKING	1	OTA ����������
OTA_STATE_DONE	2	OTA �������
OTA_STATE_ERROR	3	OTA ���������ο��� 4-14-4
�� 4-14-4
OTA_ERROR_NONE	0	��ʼ״̬���޴�����Ϣ
OTA_ERROR_NO_SUCH_VERSION	1	û�д˰汾��Ϣ
OTA_ERROR_QUERY_TIMEOUT	2	��ѯ�ƶ˰汾��ʱ���ް汾������Ϣ
OTA_ERROR_MD5_CHECK_FAILED	3	MD5 У��ʧ��
OTA_ERROR_UPDATE_FAILED	4	OTA ����ʧ��
*********************************/
typedef struct
{
    uint8_t state;       //OTA ״̬���ο����4-14-3
    uint8_t error;       //OTA ִ�г����״̬���ο���� 4-14-4
    uint8_t progress;    //OTA �������ȣ���Χ[0,100]
    uint8_t Reserved[5]; //�����ֶ�
} TYPEs_NoteOTAState;    //OTA״̬
typedef enum
{
    eNoteEyeStateOpen = 12,      //����ģʽ�¼�⵽����״̬
    eNoteEyeState1SecClose = 13, //����һ�������״̬
    eNoteEyeStateNoConfirm = 14, //�޷�ȷ�ϵ�״̬
} TYPEe_NoteEyeState;            //����������������״̬

typedef struct
{
    TYPEe_NoteEyeState eyeState;
} TYPEs_NoteEyeState; //����������������״̬

typedef enum
{
    eMediaTypeVideo = 0,
    eMediaTypeAudio = 1,
} TYPEe_MediaType;

typedef enum
{
    eMediaCapNo = 0, //δ���
    eMediaCapOk = 1, //���
} TYPEe_MediaCap;

typedef struct
{
    TYPEe_MediaType mediaType; //ý�����ͣ�0:VIDEO 1:AUDIO
    uint32_t recordedTime;     //�����ʱ��
    TYPEe_MediaCap mediaCap;   //�Ƿ񲶻���� 0��δ��� 1�����
} TYPEs_NoteMediaCap;
/*********************************
�� 4-14-1
MEDIA_STATE_IDLE	0	ģ��Ϊ����״̬������ֱ���µ�
MEDIA_STATE_WAITING	1	ģ��Ϊ�ȴ�״̬���ȴ�ִ����һ��ָ��
MEDIA_STATE_WORKING	2	ģ�����ڹ�����
MEDIA_STATE_ERROR	3	ģ�����г����������Ͳο���4-14-2
�� 4-14-2
MEDIA_ERROR_NONE	0	��ʼ״̬���޴�����Ϣ
MEDIA_ERROR_WIFI_CONNECTION_FAILED	1	����ʧ��
MEDIA_ERROR_CLOUD_CONNECTION_FAILED	2	�����ƶ�ʧ��
MEDIA_ERROR_CLOUD_TIMEOUT	3	�����ƶ˳�ʱ
MEDIA_ERROR_VIDEOCALL_FAILED	11	������Ƶ�Խ�ʧ��
MEDIA_ERROR_VIDEOCALL_REJECTED	12	������Ƶ�Խ��ɹ������û��ܾ�����
MEDIA_ERROR_CAP_NOTIFY_CLOUD_FAILED	13	ץ����Ƶ��ɺ�֪ͨ�ƶ�ʧ��
MEDIA_ERROR_PASSTHROUGH_FAILED	14	����͸��ʧ��
MEDIA_ERROR_CAPTURE_VIDEO_FAILED	21	ץ����Ƶʧ��
MEDIA_ERROR_CAPTURE_AUDIO_FAILED	22	ץ����Ƶʧ��
MEDIA_ERROR_OPEN_CAMERA_FAILED	23	�� camera ����
MEDIA_ERROR_OPEN_AUDIO_DEVICE_FAILED	24	����Ƶ�豸ʧ��
MEDIA_ERROR_UNKNOWNREASON	50	δ֪����
*********************************/

typedef struct
{
    TYPEe_MediaState mediaState; //Media ҵ���ִ��״̬���ο���� 4-14-1
    uint8_t mediaError;          //Media ҵ��ִ�г���ʱ��״̬���ο���� 4-14-2
    uint8_t Reserved[6];         //�����ֶ�
} TYPEs_NoteMediaState;

#pragma pack() //ȡ������
typedef enum
{
    eFaceIdStatusNone,                  //����
    eFaceIdStatusPowerOn,               //�ϵ�
    eFaceIdStatusPowerOnWaitRst,        //�ϵ�ʱ�ĸ�λ��
    eFaceIdStatusWaitReady,             //�ȴ��豸�ϵ����
    eFeceIdStatusGetDeiceId,            //��ȡ�豸ID
    eFeceIdStatusGetDeiceIdReply,       //�ȴ���ȡID�ظ�
    eFeceIdStatusDemoMod,               //demoģʽ
    eFeceIdStatusDemoModReply,          //demoģʽ�ظ�
    eFaceIdStatusStandBy,               //����
    eFaceIdStatusErrorTimeOut,          //��ʱ
    eFaceIdStatusPowerOff,              //�µ�
	eFaceIdStatusPowerOffWaitRst,       //�ȴ��µ縴λ
    eFaceIdStatusWaitPowerDown,         //�ȴ�power down ָ����ɡ�
    eFaceIdStatusWaitShutDown,          //�ȴ�һ��ʱ���ֱ�ӹر�
	eFaceIdStatusWaitShutDownComplete,          //�ػ����
    eFaceIdStatusPowerDownErrorTimeOut, //�µ糬ʱ
} TYPEe_FaceIdStatus;

typedef struct
{
    TYPEe_FaceIdAdd step; //��Ӳ���
    //    uint32_t OverTimr;    //��ʱʱ��
    //    uint32_t OverTimrCnt; //��ʱʱ�������
} TYPEs_FaceIdAdd;
typedef struct
{
    TYPEe_FaceIdVerify step; //��֤����
    //    uint32_t OverTimr;       //��ʱʱ��
    //    uint32_t OverTimrCnt;    //��ʱʱ�������
} TYPEs_FaceIdVerify;
typedef struct
{
    TYPEe_FaceIdDelOne step; //��֤����
    //    uint32_t OverTimr;       //��ʱʱ��
    //    uint32_t OverTimrCnt;    //��ʱʱ�������
} TYPEs_FaceIdDelOne;
typedef struct
{
    TYPEe_FaceIdDelAll step; //��֤����
    //    uint32_t OverTimr;       //��ʱʱ��
    //    uint32_t OverTimrCnt;    //��ʱʱ�������
} TYPEs_FaceIdDelAll;

typedef struct
{
    uint32_t OverTimr;    //��ʱʱ��
    uint32_t OverTimrCnt; //��ʱʱ�������
    uint32_t waitTime;    //�ȴ�ʱ�� 
} TYPEs_FaceIdOperate;    //����
typedef struct
{
    TYPEe_FaceIdStatus status;        //����״̬
    TYPEe_FaceIdStatus statusBackCmd; //ϵͳ���к����е�״̬
    uint32_t runTim;                  //����ʱ�������
    uint32_t waitReadyTim;            //�ȴ��豸��ʼ�����
    uint8_t readyFlag;                //��ʼ����ɱ�־
    uint8_t encryptFlag;              //���ܱ�־
    TYPEs_FaceIdAdd add;              //�������
    TYPEs_FaceIdVerify verify;        //��֤����
    TYPEs_FaceIdDelOne delOne;        //ɾ��һ��ָ��
    TYPEs_FaceIdDelAll delAll;        //ɾ��ȫ��ָ��
    TYPEs_FaceIdOperate operate;      //��������
    TYPEe_FaceIdCom com;              //ͨ��ͨѶ
    TYPEe_FaceIdCmd cmd;              //ͨѶָ��
	TYPEe_FaceIdShutdown shotdownFlag; //�ػ���־
    uint8_t enableNetworkBack;        //��������
} TYPEs_FaceIdCtrl;

#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
typedef enum
{

    eWifiCmdUnlockRealUpdata = 0x04,           //����ʵʱ�ϴ�
    eWifiCmdUnlockRealUpdataEncrypt = 0x05,    //����ʵʱ�����ϴ�
    eWifiCmdUnlockRealUpdataEncryptAll = 0x06, // ���ϰ汾������Ϣʵʱ�����ϴ�
    eWifiCmdAlarmUpdata = 0x07,                //������Ϣ�ϴ�
    eWifiCmdAlarmUpdataEncrypt = 0x08,         //���ܱ�����Ϣ�ϴ�
    eWifiCmdSetRouter = 0xF6,                  //����·����(0XF6)
} TYPEe_WifiCmd;
typedef enum
{
    eWifiProtocolTypeNone = 0x00,      //ָ���Ϊ��
    eWifiProtocolTypeCmd = 0x01,       // ָ���
    eWifiProtocolTypeCmdNotAck = 0x02, // ��Ӧ���
    eWifiProtocolTypeAck = 0x09,       //Ӧ���
} TYPEe_WifiProtocolType;
typedef struct
{
    uint8_t header;              //��ͷ
    TYPEe_WifiProtocolType type; //������
    uint8_t cmd;                 //ָ��
    uint16_t len;                //����
} TYPEs_WifiProtocolFront;
typedef struct
{
    TYPEs_WifiProtocolFront front; //Э��ǰ����
    uint8_t *dat;                  //���ݰ�ָ��
    uint16_t chk;                  //У���
} TYPEs_WifiProtocol;
typedef struct
{
    uint8_t MAC[6];      //��MAC��ַ��MAC�̶�����Ϊ6byte
    int16_t signalPower; //SignalPower:�ź�ǿ��RSRP(2�ֽڣ�������ģ����յ��ź�ǿ�ȣ��з�����
    uint16_t SNR;        //SNR:�����SINR(2�ֽڣ�����ӳģ����źź�������������޷�����
    int16_t txPower;     //TxPower:�ն˷��书�ʣ�2�ֽڣ�����ӳ�ն˷��书��������з�����
    uint32_t cellID;     //CellID:С��λ����Ϣ��4���ֽڣ�����ӳ�ն˽���Ļ�վ������ţ��޷�����
    uint8_t reserved[2]; //����2���ֽ���0x00��䡣
} TYPEs_DeviceInformNB;  //�豸��ϢNBģ��
typedef struct
{
    uint8_t MAC[6];        //��MAC��ַ��MAC�̶�����Ϊ6byte
    uint8_t reserved;      //Ԥ����0��1byte
    uint8_t OEM;           //OEM/ODM������Ϣ1byte,��0x00��ʩ����
    uint8_t fingerInfor;   //ָ��ģ�鳧����Ϣ1byte��0x55/0x00δ֪��0x66����΢��0x67ָ����0x68������
    uint8_t openCnt;       //������ţ���������1byte
    uint16_t openTime;     //����ʱ��������ʱ��2byte����λms��
    uint16_t openAverage;  //���ŵ��ƽ��ֵ2byte
    uint32_t openLastSamp; //���ŵ�����2����ֵ4byte
} TYPEs_DeviceInformWifi;  //�豸��ϢWIFIģ��
union unionDeviceInform
{
    TYPEs_DeviceInformNB NB;
    TYPEs_DeviceInformWifi Wifi;
};
typedef struct
{
    uint8_t deviceType;                   //�豸����
    union unionDeviceInform deviceInform; //�豸��Ϣ
    uint16_t bat;                         //����
    uint8_t unlockType;                   //������ʽ
    uint8_t pageId[4];                    //PageID
    uint8_t attribute;                    //����
    uint8_t unlockTime[6];                //����ʱ��(��,��,��,ʱ,��,��)
} TYPEs_WifiCmdUnlockRealUpdata;

typedef struct
{
    uint8_t MAC[6];        //��MAC��ַ��MAC�̶�����Ϊ6byte
    uint16_t wifiTestData[19];  //wifi��������19byte
} TYPEs_WifiCmdUpdataTest;

typedef struct
{
    uint8_t deviceType;    //�豸����
    uint8_t MAC[6];        //�豸��ϢMAC(6byte)
    uint8_t unlockTime[6]; //����ʱ��(��,��,��,ʱ,��,��)
    uint8_t reserved[5];   //Ԥ��5��byte(0x00)
    uint8_t OEM;           //OEM/ODM������Ϣ1byte,��0x00��ʩ����
    uint16_t bat;          //������Ϣ(2byte)
    uint8_t alarmType;     //������Ϣ(1byte)
} TYPEs_WifiCmdAlarmUpdata;

typedef struct
{
    uint8_t ssid[33];      //WiFi ����
    uint8_t psk[65];       //WiFi ����
} TYPEs_WifiConfig;        //wifi���� 
#pragma pack() //ȡ������

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_FACE_ID_GLOBALS
#define HAL_FACE_ID_EXT
#else
#define HAL_FACE_ID_EXT extern
#endif /*HAL_FACE_ID_GLOBALS*/
    HAL_FACE_ID_EXT TYPEs_FaceIdCtrl faceIdCtrl;
    /*************************��������*************************/
    HAL_FACE_ID_EXT TYPEs_MsgInitData msgInitData;           //δ��ʼ��
    HAL_FACE_ID_EXT TYPEs_MsgReplyInitData msgReplyInitData; //δ��ʼ��
    HAL_FACE_ID_EXT TYPEs_MsgEnrollData msgEnrollData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyEnrollData msgReplyEnrollData;
//    HAL_FACE_ID_EXT TYPEs_MsgEnrollData noteDataFace;
    HAL_FACE_ID_EXT TYPEs_ReplyResult replyResult;
    HAL_FACE_ID_EXT TYPEs_NoteNid noteNid;
    HAL_FACE_ID_EXT TYPEs_MsgVerifyData msgVerifyData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyVerifyData msgReplyVerifyData;
    HAL_FACE_ID_EXT TYPEs_MsgSnapImageData msgSnapImageData;
    HAL_FACE_ID_EXT TYPEs_MsgGetSavdeImageData msgGetSavdeImageData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyGetSavdeImageData msgReplyGetSavdeImageData;
    HAL_FACE_ID_EXT TYPEs_MsgUploadImageData msgUploadImageData;
    HAL_FACE_ID_EXT TYPEs_MsgTriggerCaptureData msgTriggerCaptureData;
    HAL_FACE_ID_EXT TYPEs_MsgDeleteMediaFileData msgDeleteMediaFileData;
    HAL_FACE_ID_EXT TYPEs_MsgVolumeData msgVolumeData;
    HAL_FACE_ID_EXT TYPEs_MsgDeluserData msgDeluserData;
    HAL_FACE_ID_EXT TYPEs_MsgStartOtaData msgStartOtaData;
    HAL_FACE_ID_EXT TYPEs_MsgConfigBaudrate msgConfigBaudrate;
    HAL_FACE_ID_EXT TYPEs_MsgConfigWifiData msgConfigWifiData;
    HAL_FACE_ID_EXT TYPEs_MsgGetWifiMacData msgGetWifiMacData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyGetWifiStatusData msgReplyGetWifiStatusData;
    HAL_FACE_ID_EXT TYPEs_MsgLinkkeyData msgLinkkeyData;
    HAL_FACE_ID_EXT TYPEs_MsgDataPassThrough msgDataPassThrough;
    HAL_FACE_ID_EXT TYPEs_MsgVideoCallData msgVideoCallData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyDeviceName msgReplyDeviceName;
    HAL_FACE_ID_EXT TYPEs_MsgReplyGetModuleStatus mMsgReplyGetModuleStatus;
    HAL_FACE_ID_EXT TYPEs_MsgPowerDownData msgPowerDownData;
    HAL_FACE_ID_EXT TYPEs_MsgDebugModeData msgDebugModeData;
    HAL_FACE_ID_EXT TYPEs_MsgUploadDebugInfoData msgUploadDebugInfoData;
    HAL_FACE_ID_EXT TYPEs_MsgDemoModeoData msgDemoModeoData;
    HAL_FACE_ID_EXT TYPEs_MsgReplyVersionGet msgReplyVersionGet;
	HAL_FACE_ID_EXT TYPEs_MsgSetThresholdLevel msgSetThresholdLevel;
    HAL_FACE_ID_EXT TYPEs_FaceIdCtrl faceIdCtr;
	HAL_FACE_ID_EXT TYPEs_NoteDataFace noteDataFace;
    HAL_FACE_ID_EXT TYPEs_NoteOTAState noteOTAState;
    HAL_FACE_ID_EXT TYPEs_NoteEyeState noteEyeState;
    HAL_FACE_ID_EXT TYPEs_NoteMediaCap noteMediaCap;
    HAL_FACE_ID_EXT TYPEs_NoteMediaState noteMediaState;
    HAL_FACE_ID_EXT TYPEs_WifiCmdUnlockRealUpdata *const pWifiCmdUnlockRealUpdata;
    HAL_FACE_ID_EXT TYPEs_WifiCmdAlarmUpdata *const pWifiCmdAlarmUpdata;
    HAL_FACE_ID_EXT TYPEs_WifiConfig *const pWifiConfig;
    HAL_FACE_ID_EXT TYPEs_WifiCmdUpdataTest *const pWifiCmdUpdataTest;
    /****HAL_FACE_ID_EXT*********************��������*************************/
    HAL_FACE_ID_EXT void HAL_FaceId_Init(void);
    HAL_FACE_ID_EXT void HAL_FaceId_ClosePow(void);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdSend(uint8_t mid);
    HAL_FACE_ID_EXT void HAL_FaceId_GetRecvReply(void);
    HAL_FACE_ID_EXT void HAL_FaceId_RevcDeal(void);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdPowerOn(uint8_t mode);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdPowerOff(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdStatus HAL_FaceId_PowerDeal(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdAdd HAL_FaceId_AddUser(void);
    HAL_FACE_ID_EXT void HAL_FaceId_AddUserStart(void);
    HAL_FACE_ID_EXT void HAL_FaceId_AddUserStop(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdVerify HAL_FaceId_VerifyUser(void);
    HAL_FACE_ID_EXT void HAL_FaceId_VerifyUserStart(void);
    HAL_FACE_ID_EXT void HAL_FaceId_VerifyUserStop(void);
    HAL_FACE_ID_EXT void HAL_FaceId_DelOneUserStart(uint16_t userId);
    HAL_FACE_ID_EXT void HAL_FaceId_DelOneUserStop(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdDelOne HAL_FaceId_DelOneUser(void);
    HAL_FACE_ID_EXT void HAL_FaceId_DelAllUserStart(void);
    HAL_FACE_ID_EXT void HAL_FaceId_DelAllUserStop(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdDelAll HAL_FaceId_DelAllUser(void);
    HAL_FACE_ID_EXT void HAL_FaceId_ComStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_Reset(void);
    HAL_FACE_ID_EXT void HAL_FaceId_SetDemoMode(void);
    HAL_FACE_ID_EXT void HAL_Face_EnterSleepConfig(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdStatus HAL_FaceId_GetStatus(void);
    HAL_FACE_ID_EXT void HAL_FaceId_GetVersionStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_GetVersion(void);
    HAL_FACE_ID_EXT void HAL_FaceId_ConfigWifiDataStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_ConfigWifiData(void);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdStop(void);
    HAL_FACE_ID_EXT void HAL_FaceId_SetLinkKeyStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_SetLinkKey(void);
    HAL_FACE_ID_EXT void HAL_FaceId_GetDeviceNameStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_GetDeviceName(void);
    HAL_FACE_ID_EXT void HAL_FaceId_DataPassThroughStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_DataPassThrough(void);
    HAL_FACE_ID_EXT uint16_t Comm_WifiCmdSend(TYPEe_WifiCmd cmd);
    HAL_FACE_ID_EXT void HAL_FaceId_TriggerCaptureDataStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_TriggerCaptureData(void);
    HAL_FACE_ID_EXT void HAL_FaceId_VideoCallDataStart(void);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_VideoCallData(void);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdDealStart(TYPEe_FaceIdCmd cmd, uint32_t overTime);
    HAL_FACE_ID_EXT TYPEe_FaceIdCom HAL_FaceId_CmdDeal(TYPEe_FaceIdCmd cmd, uint8_t waitStandBy,uint32_t overTime);
	HAL_FACE_ID_EXT int8_t HAL_FaceId_ChkBusy(void);
	HAL_FACE_ID_EXT int8_t HAL_FaceId_ChkPowerBusy(void);
    HAL_FACE_ID_EXT void HAL_FaceId_CmdPowerOffDir(void);
    HAL_FACE_ID_EXT uint16_t Comm_WifiCmdSendWithLen(TYPEe_WifiCmd cmd,TYPEe_WifiProtocolType tp,uint16_t len);
    HAL_FACE_ID_EXT void HAL_Face_WifiPowClose(void);
	HAL_FACE_ID_EXT void HAL_FaceId_IRSleepConfig(uint8_t sw);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_FACE_ID_MODULE_EN*/

#endif /*HAL_FACE_ID_MODULE_EN*/
#endif /*__HAL_FACE_ID_H__*/
/************************************Code End******************************************/
