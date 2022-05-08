/***************************************************************************************
**�ļ���:     DRV_FaceId.h
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
#ifndef __DRV_FACE_ID_H__
#define __DRV_FACE_ID_H__
#include "MyConfig.h"
#define DRV_FACE_ID_MODULE_EN (1) //ģ��ʹ��
#if DRV_FACE_ID_MODULE_EN
/*************************ר�ú궨��*************************/
#define DRV_FACE_ID_ENCRYPT_EN //����ʹ��
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
 * ¼������
 * ********************************************************/
#define FACE_ID_ENROLL_TYPE_INTERACTIVE 0 //����ʽ¼��
#define FACE_ID_ENROLL_TYPE_SINGLE 1      //��֡¼��



/***********************************************************
 * Media state
 * ********************************************************/
#define FACE_ID_MEDIA_STATE_IDLE 0    //ģ��Ϊ����״̬������ֱ���µ�
#define FACE_ID_MEDIA_STATE_WAITING 1 //ģ��Ϊ�ȴ�״̬���ȴ�ִ����һ��ָ��
#define FACE_ID_MEDIA_STATE_WORKING 2 //ģ�����ڹ�����
#define FACE_ID_MEDIA_STATE_ERROR 3   //ģ�����г����������Ͳο���4-14-2

#define FACE_ID_MEDIA_ERROR_NONE 0                      //��ʼ״̬���޴�����Ϣ
#define FACE_ID_MEDIA_ERROR_WIFI_CONNECTION_FAILED 1    //����ʧ��
#define FACE_ID_MEDIA_ERROR_CLOUD_CONNECTION_FAILED 2   //�����ƶ�ʧ��
#define FACE_ID_MEDIA_ERROR_CLOUD_TIMEOUT 3             //�����ƶ˳�ʱ
#define FACE_ID_MEDIA_ERROR_VIDEOCALL_FAILED 11         //������Ƶ�Խ�ʧ��
#define FACE_ID_MEDIA_ERROR_VIDEOCALL_REJECTED 12       //������Ƶ�Խ��ɹ������û��ܾ�����
#define FACE_ID_MEDIA_ERROR_CAP_NOTIFY_CLOUD_FAILED 13  //ץ����Ƶ��ɺ�֪ͨ�ƶ�ʧ��
#define FACE_ID_MEDIA_ERROR_PASSTHROUGH_FAILED 14       //����͸��ʧ��
#define FACE_ID_MEDIA_ERROR_CAPTURE_VIDEO_FAILED 21     //ץ����Ƶʧ��
#define FACE_ID_MEDIA_ERROR_CAPTURE_AUDIO_FAILED 22     //ץ����Ƶʧ��
#define FACE_ID_MEDIA_ERROR_OPEN_CAMERA_FAILED 23       //�� camera ����
#define FACE_ID_MEDIA_ERROR_OPEN_AUDIO_DEVICE_FAILED 24 //����Ƶ�豸ʧ��
#define FACE_ID_MEDIA_ERROR_UNKNOWNREASON 50            //δ֪����

#define FACE_ID_OTA_STATE_IDLE 0    //OTA ����״̬
#define FACE_ID_OTA_STATE_WORKING 1 //OTA ����������
#define FACE_ID_OTA_STATE_DONE 2    //OTA �������
#define FACE_ID_OTA_STATE_ERROR 3   //OTA ���������ο��� 4-14-4

#define FACE_ID_OTA_ERROR_NONE 0             //��ʼ״̬���޴�����Ϣ
#define FACE_ID_OTA_ERROR_NO_SUCH_VERSION 1  //û�д˰汾��Ϣ
#define FACE_ID_OTA_ERROR_QUERY_TIMEOUT 2    //��ѯ�ƶ˰汾��ʱ���ް汾������Ϣ
#define FACE_ID_OTA_ERROR_MD5_CHECK_FAILED 3 //MD5 У��ʧ��
#define FACE_ID_OTA_ERROR_UPDATE_FAILED 4    //OTA ����ʧ��

/*************************�����ṹ����*************************/

#ifdef DRV_FACE_ID_ENCRYPT_EN //����ʹ��
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

    /*************************��������*************************/

    /*************************��������*************************/
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
