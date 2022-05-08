/***************************************************************************************
**文件名:     HAL_FaceId.h
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
#ifndef __HAL_FACE_ID_H__
#define __HAL_FACE_ID_H__
#include "MyConfig.h"
#define HAL_FACE_ID_MODULE_EN (1) //模块使能
#ifdef DRV_FACE_ID_ENCRYPT_EN
#define HAL_FACE_ID_ENCRYPT_EN
#endif
//#define USE_HAL_FACE_POWER_ON (1) //使用HAL库的上电接口
//#define USE_HAL_FACE_POWER_OFF (1) //使用HAL库的上电接口
#if HAL_FACE_ID_MODULE_EN
/*************************专用宏定义*************************/
#define HAL_FACE_ID_WAIT_RECE_TIME_VERSOIN (25) //获取版本,等待时间.
#define USER_NAME_SIZE 32
#define COMM_WIFI_HEADER (0xFE) //wifi通讯数据头
/***********************************************************
 * 人脸状态(state)
 * ********************************************************/
#define FACE_ID_STATE_NORMAL 0                     //人脸正常
#define FACE_ID_STATE_NOFACE 1                     //未检测到人脸
#define FACE_ID_STATE_TOOUP 2                      //人脸太靠近图片上边沿，未能录入
#define FACE_ID_STATE_TOODOWN 3                    //人脸太靠近图片下边沿，未能录入
#define FACE_ID_STATE_TOOLEFT 4                    //人脸太靠近图片左边沿，未能录入
#define FACE_ID_STATE_TOORIGHT 5                   //人脸太靠近图片右边沿，未能录入
#define FACE_ID_STATE_FAR 6                        //人脸距离太远，未能录入
#define FACE_ID_STATE_CLOSE 7                      //人脸距离太近，未能录入
#define FACE_ID_STATE_EYEBROW_OCCLUSION 8          //眉毛遮挡
#define FACE_ID_STATE_EYE_OCCLUSION 9              //眼睛遮挡
#define FACE_ID_STATE_FACE_OCCLUSION 10            //脸部遮挡
#define FACE_ID_STATE_DIRECTION_ERROR 11           //录入人脸方向错误
#define FACE_ID_STATE_EYE_CLOSE_STATUS_OPEN_EYE 12 //在闭眼模式检测到睁眼状态
#define FACE_ID_STATE_EYE_CLOSE_STATUS 13          //闭眼状态
#define FACE_ID_STATE_EYE_CLOSE_UNKNOW_STATUS 14   //在闭眼模式检测中无法判定睁闭眼状态
/***********************************************************
 * result
 * ********************************************************/
#define FACE_ID_MR_SUCCESS 0                //成功
#define FACE_ID_MR_REJECTED 1               //模块拒绝该命令
#define FACE_ID_MR_ABORTED 2                //录入/解锁算法已终止
#define FACE_ID_MR_FAILED4_CAMERA 4         //相机打开失败
#define FACE_ID_MR_FAILED4_UNKNOWNREASON 5  //未知错误
#define FACE_ID_MR_FAILED4_INVALIDPARAM 6   //无效的参数
#define FACE_ID_MR_FAILED4_NOMEMORY 7       //内存不足
#define FACE_ID_MR_FAILED4_UNKNOWNUSER 8    //没有已录入的用户
#define FACE_ID_MR_FAILED4_MAXUSER 9        //录入超过最大用户数量
#define FACE_ID_MR_FAILED4_FACEENROLLED 10  //人脸已录入
#define FACE_ID_MR_FAILED4_LIVENESSCHECK 12 //活体检测失败
#define FACE_ID_MR_FAILED4_TIMEOUT 13       //录入或解锁超时
#define FACE_ID_MR_FAILED4_AUTHORIZATION 14 //加密芯片授权失败
#define FACE_ID_MR_FAILED4_READ_FILE 19     //读文件失败
#define FACE_ID_MR_FAILED4_WRITE_FILE 20    //写文件失败
#define FACE_ID_MR_FAILED4_NO_ENCRYPT 21    //通信协议未加密
#define FACE_ID_MR_FAILED4_NO_RGBIMAGE 23   //RGB 图片未准备好
/***********************************************************
 * NOTE
 * ********************************************************/
#define FACE_ID_NID_READY 0           //模块已启动并进入待命状态
#define FACE_ID_NID_FACE_STATE 1      //算法执行成功，并且返回人脸信息
#define FACE_ID_NID_UNKNOWNERROR 2    //未知错误
#define FACE_ID_NID_OTA_STATE 3       //OTA
#define FACE_ID_NID_EYE_STATE 4       //解锁过程中睁闭眼状态
#define FACE_ID_NID_MEDIA_CAP_STATE 5 //媒体类型
#define FACE_ID_NID_MEDIA_STATE 6     //Media 业务
/*************************变量结构声明*************************/
typedef enum
{
    eFaceIdAddNone,        //空闲
    eFaceIdAddReady,       //准备开始
    eFaceIdAddMiddle,      //添加人脸中间
    eFaceIdAddMiddleReply, //等待添加人脸中间完成
    eFaceIdAddMiddleOK,    //添加人脸中间完成
    eFaceIdAddUP,          //添加人脸上
    eFaceIdAddUPReply,     //等待添加人脸上完成
    eFaceIdAddUPOK,        //添加人脸中间完成
    eFaceIdAddDown,        //添加人脸下
    eFaceIdAddDownReply,   //等待添加人脸下完成
    eFaceIdAddDownOK,      //添加人脸下完成
    eFaceIdAddLeft,        //添加人脸左
    eFaceIdAddLeftReply,   //等待添加人脸左完成
    eFaceIdAddLeftOK,      //添加人脸左完成
    eFaceIdAddRight,       //添加人脸右
    eFaceIdAddRightReply,  //等待添加人脸右完成
    eFaceIdAddRightOK,     //添加人脸右完成
    eFaceIdAddOk,          //添加完成
    eFaceIdAddFail,        //添加完成
    eFaceIdAddTimeOver,    //超时
} TYPEe_FaceIdAdd;         //添加流程
typedef enum
{
    eFaceIdVerifyNone,    //空闲
    eFaceIdVerifyReady,   //准备开始
    eFaceIdVerifyReply,   //等待回复
    eFaceIdVerifyOk,      //验证成功
    eFaceIdVerifyFail,    //验证失败
    eFaceIdVerifyTimeOut, //超时
	eFaceIdVerifyFailLiveNessCheck, //活体检测失败
} TYPEe_FaceIdVerify;     //验证流程

typedef enum
{
    eFaceIdDelOneNone,    //空闲
    eFaceIdDelOneReady,   //准备开始
    eFaceIdDelOneReply,   //等待回复
    eFaceIdDelOneOk,      //成功
    eFaceIdDelOneFail,    //失败
    eFaceIdDelOneTimeOut, //超时
} TYPEe_FaceIdDelOne;     //删除一个指纹流程

typedef enum
{
    eFaceIdDelAllNone,    //空闲
    eFaceIdDelAllReady,   //准备开始
    eFaceIdDelAllReply,   //等待回复
    eFaceIdDelAllOk,      //成功
    eFaceIdDelAllFail,    //失败
    eFaceIdDelAllTimeOut, //超时
} TYPEe_FaceIdDelAll;     //删除全部指纹流程
typedef enum
{
    eFaceIdComNone,     //空闲
    eFaceIdComReady,    //准备开始
    eFaceIdComReply,    //等待回复
    eFaceIdComOk,       //成功
    eFaceIdComFail,     //失败
    eFaceIdComTimeOut,  //超时
    eFaceIdComCmdError, //命令不匹配
	eFaceIdComCmdTaskIng, //等待任务完成
	eFaceIdComCmdTaskOk, //等待任务完成
} TYPEe_FaceIdCom;      //通用通讯格式
typedef enum
{
    eFaceIdCmdNone = 0x00, //指令为空
    eFaceIdCmdTriggerCaptureData = 0x19, //抓拍
	eFaceIdCmdStartOTA = 0X40, //开始OTA
    eFaceIdCmdVideoCallData = 0x84, //视频对讲
    eFaceIdCmdGetModuleStatus = 0x86, //获取模块状态
    eFaceIdCmdDataPassThrough = 0x83, //数据透传
	eFaceIdCmdSetThresholdLevel = 0xD4, //修改人脸识别安全等级
    eFaceIdCmdDeleteMediaFileData = 0x27, //删除媒体文件
} TYPEe_FaceIdCmd;                    //通讯命令

typedef enum
{
    eFaceIdShutdownNone = 0x00, //空闲
    eFaceIdShutdownIng = 0x01, //关机中
	eFaceIdShutdownYes = 0x02, //已经关机完成
} TYPEe_FaceIdShutdown;                   
#pragma pack(1)                       //这里强制使用1字节对齐的方式
typedef struct
{
    uint8_t seed[4];       // 一组 4 字节的随机数
    uint8_t mode;          //加密模式：0：明文 1：AES 加密（默认）
    uint8_t crtTime[4];    //当前时间，用于同步模组的系统时间
    uint8_t enableNetwork; //是否联网 Enable：1;Disable：0
    uint8_t waitingTime;   //云业务完成后的等待时间
    uint8_t reserved[5];   //保留字段
} TYPEs_MsgInitData;       //MID_INIT code:人脸0x50 人脸+猫眼0x25
typedef struct
{
    uint8_t deviceId[20]; //设备 ID（与模组后面的二维码一致）
} TYPEs_MsgReplyInitData; //MID_INIT REPLY code:人脸0x50 人脸+猫眼0x25
typedef struct
{
    uint8_t pdRightAway; //解锁成功后是否立刻断电 yes:1 no:0
    uint8_t timeOut;     //解锁超时时间 单位:s
} TYPEs_MsgVerifyData;   //MID_VERIFY code:0x12
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;      //已验证用户的 ID
    uint8_t userName[32];   //用户名字
    uint8_t admin;          //是否为管理员
    uint8_t unlockStatus;   //解锁过程中睁闭眼状态
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
    uint8_t admin;                    //是否设置为管理员yes:1 no:0
    uint8_t userName[USER_NAME_SIZE]; //录入用户姓名
    TYPEs_FaceDir faceDirection;      //用户需要录入的方向
    uint8_t timeOut;                  //录入超时时间 单位:s
} TYPEs_MsgEnrollData;                //MID_ENROLL 0x13
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;           //已注册用户的 ID
    TYPEs_FaceDir faceDirection; //各个方向人脸的录入状态
} TYPEs_MsgReplyEnrollData;      //MID_ENROLL 0x13
typedef struct
{
    uint8_t imageCounts;  //抓拍图片的数量
    uint8_t startNumber;  //抓拍图片的起始编号（1-50）
} TYPEs_MsgSnapImageData; //MID_SNAPIMAGE 0x16
typedef struct
{
    uint8_t imageNumber;      //待传输图片的编号
} TYPEs_MsgGetSavdeImageData; //MID_GETSAVEDIMAGE 0x17
typedef struct
{
    uint8_t imageSize[4];          //待上传图片大小，image_size[0]为最高8位
} TYPEs_MsgReplyGetSavdeImageData; //MID_GETSAVEDIMAGE 0x17
typedef struct
{
    uint8_t uploadImageOffset[4]; //待上传图片的偏移量
    uint8_t uploadImageSize[4];   //此次上传图片的大小，最大4K
} TYPEs_MsgUploadImageData;       //MID_UPLOADIMAGE 0x18
typedef struct
{
    uint8_t mediaType;          //媒体类型 0:VIDEO 1:AUDIO
    uint32_t captureTime;       //捕获时间 单位:s
    uint8_t enableCloudStorage; //是否使能云端存储 1:enable 0:disable
    uint8_t videoCaptureType;   //视频的抓拍类型，当media_type 为 VIDEO时有效
} TYPEs_MsgTriggerCaptureData;  //MID_TRIGGER_CAPTURE 0x19
typedef struct
{
    uint8_t mediaType;          //媒体类型 0:VIDEO 1:AUDIO
    uint8_t force;              //是否强制删除1：是 0：否
} TYPEs_MsgDeleteMediaFileData; //MID_DELETE_MEDIA_FILE 0x27
typedef struct
{
    uint8_t volume;    //设置播放音量
} TYPEs_MsgVolumeData; //MID_SET_VOLUME 0x1F
typedef struct
{
    uint8_t userIdHeb;
    uint8_t userIdLeb;  //待删除用户的 ID
} TYPEs_MsgDeluserData; //MID_DELUSER 0x20

typedef struct
{
    uint8_t vPrimary;    //固件版本号最高位
    uint8_t vSecondary;  //固件版本号中间位
    uint8_t vRevision;   //固件版本号最低位
} TYPEs_MsgStartOtaData; //MID_START_OTA 0x40 单机版本
typedef struct
{
    uint8_t baudrateIndex; //波特率 1:115200 2:230400 3:460800 4:1500000
} TYPEs_MsgConfigBaudrate; //MID_CONFIG_BAUDRATE 0x51
typedef struct
{
    uint8_t ssid[33];      //WiFi 名称
    uint8_t psk[65];       //WiFi 密码
} TYPEs_MsgConfigWifiData; //MID_CONFIG_WIFI 0x70
typedef struct
{
    uint8_t address[6];    //MAC 地址
} TYPEs_MsgGetWifiMacData; //MID_GET_WIFIMAC 0x71
typedef struct
{
    uint8_t connectStatus;         //1:wifi已经链接 其他:未连接
    int8_t signalLevel;            //一般信号强度在-30~-120 之间。正常信号强度应该是-40 dbm ~ -85 dbm 之 间。小于 -90 dbm 就很差了，几乎没法连接
    int8_t noise;                  //噪声
} TYPEs_MsgReplyGetWifiStatusData; //MID_GET_WIFISTATUS 0x72
typedef struct
{
    uint8_t ProductKey[32];
    uint8_t productSecret[64];
    uint8_t deviceName[32];
    uint8_t deviceSecret[64]; //阿里云秘钥
} TYPEs_MsgLinkkeyData;       //MID_SET_LINKKEY 0x82
typedef struct
{
    uint8_t data[128];      //数据透传
} TYPEs_MsgDataPassThrough; //MID_DATA_PASSTHROUG H 0x83

typedef struct
{
    uint32_t captureTime; //门铃抓拍时间
    uint8_t enableRecord; //是否开启门铃视频抓拍 1：enbale 0：disable
    uint8_t reserved[3];  //保留字段
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
    uint8_t unlockState;         //解锁业务运行状态，1：解锁业务运行中；0：解锁业务未在运行
} TYPEs_MsgReplyGetModuleStatus; // 0x86
typedef struct
{
    uint8_t enableLowPower; //模块准备关机，结束当前在处理的任务，清除文件系统缓存，指定 wifi 是否进入低保活模式，0：不开启低保活 1：开启低保活
} TYPEs_MsgPowerDownData;   //MID_POWER DOWN 0xED
typedef struct
{
    uint8_t enable;       //进入/退出 debug 模式
} TYPEs_MsgDebugModeData; //MID_DEBUG_MODE 0xF0
typedef struct
{
    uint8_t uploadDebugInfoOffset[4]; //待上传 debug 信息的偏移量
    uint8_t uploadDebugInfoSize[4];   //待上传 debug 信息的大小，最大 4K
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
    uint8_t recognition; //对比
	uint8_t biopsy;	//活体
} TYPEs_MsgSetThresholdLevel; //SET_THRESHOLD_LEVEL	0xD4
/************************************************************
表 4-13
人脸状态(state)	code	说明
FACE_STATE_NORMAL	0	人脸正常
FACE_STATE_NOFACE	1	未检测到人脸
FACE_STATE_TOOUP	2	人脸太靠近图片上边沿，未能录入
FACE_STATE_TOODOWN	3	人脸太靠近图片下边沿，未能录入
FACE_STATE_TOOLEFT	4	人脸太靠近图片左边沿，未能录入
FACE_STATE_TOORIGHT	5	人脸太靠近图片右边沿，未能录入
FACE_STATE_FAR	6	人脸距离太远，未能录入
FACE_STATE_CLOSE	7	人脸距离太近，未能录入
FACE_STATE_EYEBROW_OCCLUSIO
N	8	眉毛遮挡
FACE_STATE_EYE_OCCLUSION	9	眼睛遮挡
FACE_STATE_FACE_OCCLUSION	10	脸部遮挡
FACE_STATE_DIRECTION_ERROR	11	录入人脸方向错误
FACE_STATE_EYE_CLOSE_STATUS_
OPEN_EYE	12	在闭眼模式检测到睁眼状态
FACE_STATE_EYE_CLOSE_STATUS	13	闭眼状态
FACE_STATE_EYE_CLOSE_UNKNOW
_STATUS	14	在闭眼模式检测中无法判定睁闭眼状态
*************************************************************/
typedef struct
{
    int16_t state;    //人脸当前状态，详见表 4-13
    int16_t left;     //人脸框距离图片最左侧的距离（负数表示人脸框已超出图片最左侧）
    int16_t top;      //人脸框距离图片最上方的距离（负数表示人脸框已超出图片上方）
    int16_t right;    //人脸框距离图片最右侧的距离（负数表示人脸框已超图片最右侧）
    int16_t bottom;   //人脸框距离图片最下 方的距离（负数表示人脸框已超出图片最下方）
    int16_t yaw;      //yaw 为负表示向左转头，yaw 为正表示向右转头,参考下面的示意图 4-1
    int16_t pitch;    //pitch 为负表示向上抬头，pitch 为正表示向下低头,参考下面的示意图 4-1
    int16_t roll;     //roll 为负表示向右歪头，roll 为正表示向左歪头,参考下面的示意图 4-1
} TYPEs_NoteDataFace; //算法执行成功，并且返回人脸信息

typedef struct
{
    uint8_t code;
    uint8_t result;
} TYPEs_ReplyResult; //获取执行结果

typedef struct
{
    uint8_t nid;
} TYPEs_NoteNid; //获取执行结果
/*********************************
OTA 状态，参考表格
4-14-3
OTA_STATE_IDLE	0	OTA 空闲状态
OTA_STATE_WORKING	1	OTA 升级过程中
OTA_STATE_DONE	2	OTA 升级完成
OTA_STATE_ERROR	3	OTA 升级出错，参考表 4-14-4
表 4-14-4
OTA_ERROR_NONE	0	初始状态，无错误信息
OTA_ERROR_NO_SUCH_VERSION	1	没有此版本信息
OTA_ERROR_QUERY_TIMEOUT	2	查询云端版本超时，无版本升级信息
OTA_ERROR_MD5_CHECK_FAILED	3	MD5 校验失败
OTA_ERROR_UPDATE_FAILED	4	OTA 更新失败
*********************************/
typedef struct
{
    uint8_t state;       //OTA 状态，参考表格4-14-3
    uint8_t error;       //OTA 执行出错的状态，参考表格 4-14-4
    uint8_t progress;    //OTA 升级进度，范围[0,100]
    uint8_t Reserved[5]; //保留字段
} TYPEs_NoteOTAState;    //OTA状态
typedef enum
{
    eNoteEyeStateOpen = 12,      //闭眼模式下检测到睁眼状态
    eNoteEyeState1SecClose = 13, //持续一秒均闭眼状态
    eNoteEyeStateNoConfirm = 14, //无法确认的状态
} TYPEe_NoteEyeState;            //解锁过程中睁闭眼状态

typedef struct
{
    TYPEe_NoteEyeState eyeState;
} TYPEs_NoteEyeState; //解锁过程中睁闭眼状态

typedef enum
{
    eMediaTypeVideo = 0,
    eMediaTypeAudio = 1,
} TYPEe_MediaType;

typedef enum
{
    eMediaCapNo = 0, //未完成
    eMediaCapOk = 1, //完成
} TYPEe_MediaCap;

typedef struct
{
    TYPEe_MediaType mediaType; //媒体类型：0:VIDEO 1:AUDIO
    uint32_t recordedTime;     //已完成时间
    TYPEe_MediaCap mediaCap;   //是否捕获完成 0：未完成 1：完成
} TYPEs_NoteMediaCap;
/*********************************
表 4-14-1
MEDIA_STATE_IDLE	0	模组为空闲状态，可以直接下电
MEDIA_STATE_WAITING	1	模组为等待状态，等待执行下一条指令
MEDIA_STATE_WORKING	2	模组正在工作中
MEDIA_STATE_ERROR	3	模组运行出错，错误类型参考表4-14-2
表 4-14-2
MEDIA_ERROR_NONE	0	初始状态，无错误信息
MEDIA_ERROR_WIFI_CONNECTION_FAILED	1	联网失败
MEDIA_ERROR_CLOUD_CONNECTION_FAILED	2	连接云端失败
MEDIA_ERROR_CLOUD_TIMEOUT	3	连接云端超时
MEDIA_ERROR_VIDEOCALL_FAILED	11	触发视频对讲失败
MEDIA_ERROR_VIDEOCALL_REJECTED	12	触发视频对讲成功但是用户拒绝接听
MEDIA_ERROR_CAP_NOTIFY_CLOUD_FAILED	13	抓拍视频完成后通知云端失败
MEDIA_ERROR_PASSTHROUGH_FAILED	14	数据透传失败
MEDIA_ERROR_CAPTURE_VIDEO_FAILED	21	抓拍视频失败
MEDIA_ERROR_CAPTURE_AUDIO_FAILED	22	抓拍音频失败
MEDIA_ERROR_OPEN_CAMERA_FAILED	23	打开 camera 出错
MEDIA_ERROR_OPEN_AUDIO_DEVICE_FAILED	24	打开音频设备失败
MEDIA_ERROR_UNKNOWNREASON	50	未知错误
*********************************/

typedef struct
{
    TYPEe_MediaState mediaState; //Media 业务的执行状态，参考表格 4-14-1
    uint8_t mediaError;          //Media 业务执行出错时的状态，参考表格 4-14-2
    uint8_t Reserved[6];         //保留字段
} TYPEs_NoteMediaState;

#pragma pack() //取消对齐
typedef enum
{
    eFaceIdStatusNone,                  //空闲
    eFaceIdStatusPowerOn,               //上电
    eFaceIdStatusPowerOnWaitRst,        //上电时的复位。
    eFaceIdStatusWaitReady,             //等待设备上电完成
    eFeceIdStatusGetDeiceId,            //获取设备ID
    eFeceIdStatusGetDeiceIdReply,       //等待获取ID回复
    eFeceIdStatusDemoMod,               //demo模式
    eFeceIdStatusDemoModReply,          //demo模式回复
    eFaceIdStatusStandBy,               //待机
    eFaceIdStatusErrorTimeOut,          //超时
    eFaceIdStatusPowerOff,              //下电
	eFaceIdStatusPowerOffWaitRst,       //等待下电复位
    eFaceIdStatusWaitPowerDown,         //等待power down 指令完成。
    eFaceIdStatusWaitShutDown,          //等待一段时间后直接关闭
	eFaceIdStatusWaitShutDownComplete,          //关机完成
    eFaceIdStatusPowerDownErrorTimeOut, //下电超时
} TYPEe_FaceIdStatus;

typedef struct
{
    TYPEe_FaceIdAdd step; //添加步骤
    //    uint32_t OverTimr;    //超时时间
    //    uint32_t OverTimrCnt; //超时时间计数器
} TYPEs_FaceIdAdd;
typedef struct
{
    TYPEe_FaceIdVerify step; //验证步骤
    //    uint32_t OverTimr;       //超时时间
    //    uint32_t OverTimrCnt;    //超时时间计数器
} TYPEs_FaceIdVerify;
typedef struct
{
    TYPEe_FaceIdDelOne step; //验证步骤
    //    uint32_t OverTimr;       //超时时间
    //    uint32_t OverTimrCnt;    //超时时间计数器
} TYPEs_FaceIdDelOne;
typedef struct
{
    TYPEe_FaceIdDelAll step; //验证步骤
    //    uint32_t OverTimr;       //超时时间
    //    uint32_t OverTimrCnt;    //超时时间计数器
} TYPEs_FaceIdDelAll;

typedef struct
{
    uint32_t OverTimr;    //超时时间
    uint32_t OverTimrCnt; //超时时间计数器
    uint32_t waitTime;    //等待时间 
} TYPEs_FaceIdOperate;    //操作
typedef struct
{
    TYPEe_FaceIdStatus status;        //运行状态
    TYPEe_FaceIdStatus statusBackCmd; //系统空闲后运行的状态
    uint32_t runTim;                  //运行时间计数器
    uint32_t waitReadyTim;            //等待设备初始化完成
    uint8_t readyFlag;                //初始化完成标志
    uint8_t encryptFlag;              //加密标志
    TYPEs_FaceIdAdd add;              //添加流程
    TYPEs_FaceIdVerify verify;        //验证流程
    TYPEs_FaceIdDelOne delOne;        //删除一个指纹
    TYPEs_FaceIdDelAll delAll;        //删除全部指纹
    TYPEs_FaceIdOperate operate;      //操作控制
    TYPEe_FaceIdCom com;              //通用通讯
    TYPEe_FaceIdCmd cmd;              //通讯指令
	TYPEe_FaceIdShutdown shotdownFlag; //关机标志
    uint8_t enableNetworkBack;        //联网备份
} TYPEs_FaceIdCtrl;

#pragma pack(1) //这里强制使用1字节对齐的方式
typedef enum
{

    eWifiCmdUnlockRealUpdata = 0x04,           //开锁实时上传
    eWifiCmdUnlockRealUpdataEncrypt = 0x05,    //开锁实时加密上传
    eWifiCmdUnlockRealUpdataEncryptAll = 0x06, // 整合版本锁具信息实时加密上传
    eWifiCmdAlarmUpdata = 0x07,                //报警信息上传
    eWifiCmdAlarmUpdataEncrypt = 0x08,         //加密报警信息上传
    eWifiCmdSetRouter = 0xF6,                  //设置路由器(0XF6)
} TYPEe_WifiCmd;
typedef enum
{
    eWifiProtocolTypeNone = 0x00,      //指令包为空
    eWifiProtocolTypeCmd = 0x01,       // 指令包
    eWifiProtocolTypeCmdNotAck = 0x02, // 不应答包
    eWifiProtocolTypeAck = 0x09,       //应答包
} TYPEe_WifiProtocolType;
typedef struct
{
    uint8_t header;              //包头
    TYPEe_WifiProtocolType type; //包属性
    uint8_t cmd;                 //指令
    uint16_t len;                //长度
} TYPEs_WifiProtocolFront;
typedef struct
{
    TYPEs_WifiProtocolFront front; //协议前部分
    uint8_t *dat;                  //数据包指针
    uint16_t chk;                  //校验和
} TYPEs_WifiProtocol;
typedef struct
{
    uint8_t MAC[6];      //锁MAC地址，MAC固定长度为6byte
    int16_t signalPower; //SignalPower:信号强度RSRP(2字节），反馈模组接收的信号强度，有符号数
    uint16_t SNR;        //SNR:信噪比SINR(2字节），反映模组的信号和噪声的情况，无符号数
    int16_t txPower;     //TxPower:终端发射功率（2字节），反映终端发射功率情况，有符号数
    uint32_t cellID;     //CellID:小区位置信息（4个字节），反映终端接入的基站扇区标号，无符号数
    uint8_t reserved[2]; //其余2个字节以0x00填充。
} TYPEs_DeviceInformNB;  //设备信息NB模版
typedef struct
{
    uint8_t MAC[6];        //锁MAC地址，MAC固定长度为6byte
    uint8_t reserved;      //预留补0，1byte
    uint8_t OEM;           //OEM/ODM厂家信息1byte,（0x00德施曼）
    uint8_t fingerInfor;   //指纹模块厂家信息1byte（0x55/0x00未知，0x66迈瑞微，0x67指安，0x68集创）
    uint8_t openCnt;       //电机开门，工作次数1byte
    uint16_t openTime;     //开门时间电机工作时间2byte（单位ms）
    uint16_t openAverage;  //开门电机平均值2byte
    uint32_t openLastSamp; //开门电机最后2采样值4byte
} TYPEs_DeviceInformWifi;  //设备信息WIFI模版
union unionDeviceInform
{
    TYPEs_DeviceInformNB NB;
    TYPEs_DeviceInformWifi Wifi;
};
typedef struct
{
    uint8_t deviceType;                   //设备类型
    union unionDeviceInform deviceInform; //设备信息
    uint16_t bat;                         //电量
    uint8_t unlockType;                   //开锁方式
    uint8_t pageId[4];                    //PageID
    uint8_t attribute;                    //属性
    uint8_t unlockTime[6];                //开锁时间(年,月,日,时,分,秒)
} TYPEs_WifiCmdUnlockRealUpdata;

typedef struct
{
    uint8_t MAC[6];        //锁MAC地址，MAC固定长度为6byte
    uint16_t wifiTestData[19];  //wifi测试数据19byte
} TYPEs_WifiCmdUpdataTest;

typedef struct
{
    uint8_t deviceType;    //设备类型
    uint8_t MAC[6];        //设备信息MAC(6byte)
    uint8_t unlockTime[6]; //开锁时间(年,月,日,时,分,秒)
    uint8_t reserved[5];   //预留5个byte(0x00)
    uint8_t OEM;           //OEM/ODM厂家信息1byte,（0x00德施曼）
    uint16_t bat;          //电量信息(2byte)
    uint8_t alarmType;     //报警信息(1byte)
} TYPEs_WifiCmdAlarmUpdata;

typedef struct
{
    uint8_t ssid[33];      //WiFi 名称
    uint8_t psk[65];       //WiFi 密码
} TYPEs_WifiConfig;        //wifi配置 
#pragma pack() //取消对齐

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
    /*************************公共变量*************************/
    HAL_FACE_ID_EXT TYPEs_MsgInitData msgInitData;           //未初始化
    HAL_FACE_ID_EXT TYPEs_MsgReplyInitData msgReplyInitData; //未初始化
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
    /****HAL_FACE_ID_EXT*********************公共函数*************************/
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
