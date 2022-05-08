/***************************************************************************************
**文件名:     DRV_FaceId.h
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
#ifndef __DRV_FACE_ID_H__
#define __DRV_FACE_ID_H__
#include "MyConfig.h"
#define DRV_FACE_ID_MODULE_EN (1) //模块使能
#if DRV_FACE_ID_MODULE_EN
/*************************专用宏定义*************************/
#define DRV_FACE_ID_ENCRYPT_EN //加密使能
//#define USER_NAME_SIZE 32

/***********************************************************
 * mid Code
 * ********************************************************/
#define FACE_ID_MID_REPLY 0x00
#define FACE_ID_MID_NOTE 0x01
#define FACE_ID_MID_IMAGE 0x02
#define FACE_ID_MID_INIT 0x25 //0x50
#define FACE_ID_MID_RESET 0x10
#define FACE_ID_MID_VERIFY 0x12
#define FACE_ID_MID_ENROLL 0x13
#define FACE_ID_MID_SNAPIMAGE 0x16
#define FACE_ID_MID_GETSAVEDIMAGE 0x17
#define FACE_ID_MID_UPLOADIMAGE 0x18
#define FACE_ID_MID_TRIGGER_CAPTURE 0x19
#define FACE_ID_MID_DELETE_MEDIA_FILE 0x27
#define FACE_ID_MID_SET_VOLUME 0x1F
#define FACE_ID_MID_DELUSER 0x20
#define FACE_ID_MID_DELALL 0x21
#define FACE_ID_MID_FACERESET 0x23
#define FACE_ID_MID_GET_VERSION 0x30
#define FACE_ID_MID_START_OTA 0x40
#define FACE_ID_MID_STOP_OTA 0x41
#define FACE_ID_MID_CONFIG_BAUDRATE 0x51
#define FACE_ID_MID_CONFIG_WIFI 0x70
#define FACE_ID_MID_GET_WIFIMAC 0x71
#define FACE_ID_MID_GET_WIFISTATUS 0x72
#define FACE_ID_MID_SET_LINKKEY 0x82
#define FACE_ID_MID_DATA_PASSTHROUGH 0x83
#define FACE_ID_MID_TRIGGER_VIDEOCALL 0x84
#define FACE_ID_MID_DEVICE_NAME 0x85
#define FACE_ID_MID_GET_MODULE_STATUS 0x86
#define FACE_ID_MID_POWERDOWN 0xED
#define FACE_ID_MID_DEBUG_MODE 0xF0
#define FACE_ID_MID_GET_DEBUG_INFO 0xF1
#define FACE_ID_MID_UPLOAD_DEBUG_INFO 0xF2
#define FACE_ID_MID_DEMOMODE 0xFE
#define FACE_ID_MID_SET_THRESHOLD_LEVEL 0xD4

/***********************************************************
 * 录入类型
 * ********************************************************/
#define FACE_ID_ENROLL_TYPE_INTERACTIVE 0 //交互式录入
#define FACE_ID_ENROLL_TYPE_SINGLE 1      //单帧录入



/***********************************************************
 * Media state
 * ********************************************************/
#define FACE_ID_MEDIA_STATE_IDLE 0    //模组为空闲状态，可以直接下电
#define FACE_ID_MEDIA_STATE_WAITING 1 //模组为等待状态，等待执行下一条指令
#define FACE_ID_MEDIA_STATE_WORKING 2 //模组正在工作中
#define FACE_ID_MEDIA_STATE_ERROR 3   //模组运行出错，错误类型参考表4-14-2

#define FACE_ID_MEDIA_ERROR_NONE 0                      //初始状态，无错误信息
#define FACE_ID_MEDIA_ERROR_WIFI_CONNECTION_FAILED 1    //联网失败
#define FACE_ID_MEDIA_ERROR_CLOUD_CONNECTION_FAILED 2   //连接云端失败
#define FACE_ID_MEDIA_ERROR_CLOUD_TIMEOUT 3             //连接云端超时
#define FACE_ID_MEDIA_ERROR_VIDEOCALL_FAILED 11         //触发视频对讲失败
#define FACE_ID_MEDIA_ERROR_VIDEOCALL_REJECTED 12       //触发视频对讲成功但是用户拒绝接听
#define FACE_ID_MEDIA_ERROR_CAP_NOTIFY_CLOUD_FAILED 13  //抓拍视频完成后通知云端失败
#define FACE_ID_MEDIA_ERROR_PASSTHROUGH_FAILED 14       //数据透传失败
#define FACE_ID_MEDIA_ERROR_CAPTURE_VIDEO_FAILED 21     //抓拍视频失败
#define FACE_ID_MEDIA_ERROR_CAPTURE_AUDIO_FAILED 22     //抓拍音频失败
#define FACE_ID_MEDIA_ERROR_OPEN_CAMERA_FAILED 23       //打开 camera 出错
#define FACE_ID_MEDIA_ERROR_OPEN_AUDIO_DEVICE_FAILED 24 //打开音频设备失败
#define FACE_ID_MEDIA_ERROR_UNKNOWNREASON 50            //未知错误

#define FACE_ID_OTA_STATE_IDLE 0    //OTA 空闲状态
#define FACE_ID_OTA_STATE_WORKING 1 //OTA 升级过程中
#define FACE_ID_OTA_STATE_DONE 2    //OTA 升级完成
#define FACE_ID_OTA_STATE_ERROR 3   //OTA 升级出错，参考表 4-14-4

#define FACE_ID_OTA_ERROR_NONE 0             //初始状态，无错误信息
#define FACE_ID_OTA_ERROR_NO_SUCH_VERSION 1  //没有此版本信息
#define FACE_ID_OTA_ERROR_QUERY_TIMEOUT 2    //查询云端版本超时，无版本升级信息
#define FACE_ID_OTA_ERROR_MD5_CHECK_FAILED 3 //MD5 校验失败
#define FACE_ID_OTA_ERROR_UPDATE_FAILED 4    //OTA 更新失败

/*************************变量结构声明*************************/

#ifdef DRV_FACE_ID_ENCRYPT_EN //加密使能
#endif
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_FACE_ID_GLOBALS
#define DRV_FACE_ID_EXT
#else
#define DRV_FACE_ID_EXT extern
#endif /*DRV_FACE_ID_GLOBALS*/

    /*************************公共变量*************************/

    /*************************公共函数*************************/
    DRV_FACE_ID_EXT void DRV_FaceId_Init(void);
    DRV_FACE_ID_EXT void DRV_FaceId_OpenPow(void);
    DRV_FACE_ID_EXT void DRV_FaceId_ClosePow(void);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_RecvDataUnblock(void);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_CmdSend(uint8_t mid, uint8_t *dat, uint32_t datLen, uint8_t cmdType);
    DRV_FACE_ID_EXT uint8_t DRV_FaceId_GetRecvMsgType(void);
    DRV_FACE_ID_EXT uint8_t DRV_FaceId_GetRecvReplyMid(uint8_t *readDat, uint32_t len);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_GetRecvReplyResult(uint8_t *readDat, uint32_t len);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_GetRecvReplyData(uint8_t *readDat, uint32_t len);
    DRV_FACE_ID_EXT uint8_t DRV_FaceId_GetRecvNoteNid(uint8_t *readDat, uint32_t len);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_GetRecvNoteData(uint8_t *readDat, uint32_t len);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_ClearRxPar(void);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_RecvDataAndAnalysis(uint8_t cmdType);
    DRV_FACE_ID_EXT int8_t DRV_FaceId_GetAesKey(uint8_t *dat);
    DRV_FACE_ID_EXT void DRV_Face_EnterSleepConfig(void);
	DRV_FACE_ID_EXT void DRV_Face_WifiPowOpen(void);
	DRV_FACE_ID_EXT void DRV_Face_WifiPowClose(void);
    DRV_FACE_ID_EXT uint16_t DRV_FaceId_GetRecvMsgSize(void);
    DRV_FACE_ID_EXT void DRV_Face_CateyeFaceSw(uint8_t sw);
	DRV_FACE_ID_EXT void DRV_FaceId_IRSleepConfig(uint8_t sw);
#ifdef DRV_FACE_ID_ENCRYPT_EN
//    DRV_FACE_ID_EXT int8_t DRV_FaceId_EncryptReg(FaceIdEncrypt_t fun);
#endif
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_FACE_ID_MODULE_EN*/

#endif /*DRV_FACE_ID_MODULE_EN*/
#endif /*__DRV_FACE_ID_H__*/
/************************************Code End******************************************/
