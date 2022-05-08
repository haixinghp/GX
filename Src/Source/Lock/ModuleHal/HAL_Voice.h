/***************************************************************************************
**�ļ���:     HAL_Voice.h
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
#ifndef __HAL_VOICE_H__
#define __HAL_VOICE_H__
#include "MyConfig.h"
#define HAL_VOICE_MODULE_EN (1) //ģ��ʹ��
#if HAL_VOICE_MODULE_EN
/*************************ר�ú궨��*************************/
#define HAL_VOICE_DONOT_WAIT_END (0)
#define HAL_VOICE_WAIT_START (5)
#define HAL_VOICE_BELL_WORK_TIME (3000) //���幤��ʱ��
#define HAL_VOICE_BELL_CLOSE_TIME (2900)
/*************************�����ṹ����*************************/

typedef enum
{
	eVoiceDirFaceSetPress_1 = 0,			  //������Ϣ�����밴1
	eVoiceDirFingerSetPress_2 = 1,			  //ָ�������밴2
	eVoiceDirPwdSetPress_3 = 2,				  //���������밴3
	eVoiceDirMotorTorqueSetPress_4 = 3,		  //���Ť�������밴4
	eVoiceDirVoiceSettPress_5 = 4,			  //���������밴5
	eVoiceDirReturnPleasePress_6 = 5,		  //�����밴6
	eVoiceDirAddAdminFacePress_1 = 6,		  //���ӹ���Ա�����밴1
	eVoiceDirAddGeneralFacePress_2 = 7,		  //������ͨ�û������밴2
	eVoiceDirDelGeneralFacePress_3 = 8,		  //ɾ����ͨ�û������밴3
	eVoiceDirReturnPress_4 = 9,				  //�����밴4
	eVoiceDirAddFingerPress_1 = 10,			  //����ָ���밴1
	eVoiceDirDelFingerPress_2 = 11,			  //ɾ��ָ���밴2
	eVoiceDirReturnPress_3 = 12,			  //�����밴3
	eVoiceDirEnterSixNewPwd = 13,			  //������6λ�¿�������
	eVoiceDirPleaseInputAgain = 14,			  //��������һ��
	eVoiceDirPressHashKeyEnd = 15,			  //��#�ż�����
	eVoiceDirPwdTooSimplePleaseReset = 16,	  //����������ڼ򵥣�������
	eVoiceDirModifPwdFailedRetry = 17,		  //�޸�����ʧ�ܣ���ȷ�����������
	eVoiceDirLowMotorTorquePress_1 = 18,	  //�͵��Ť���밴1
	eVoiceDirHighTorqueMotorPress_2 = 19,	  //�ߵ��Ť���밴2
	eVoiceDirCloseVoicePress_1 = 20,		  //�ر������밴1
	eVoiceDirOpenVoicePress_2 = 21,			  //�������밴2
	eVoiceDirReturnPress_5 = 22,			  //�����밴5
	eVoiceDirDelSuccess = 23,				  //ɾ���ɹ�
	eVoiceDirDelFailed = 24,				  //ɾ��ʧ��
	eVoiceDirSetSuccess = 25,				  //���óɹ�
	eVoiceDirSetFailed = 26,				  //����ʧ��
	eVoiceDirFaceIsFull = 27,				  //������Ϣ����
	eVoiceDirPleaseVerifyFace = 28,			  //����֤����
	eVoiceDirVerifySuccess = 29,			  //��֤�ɹ�
	eVoiceDirVerifyFail = 30,				  //��֤ʧ��
	eVoiceDirPleaseVerifyAdminiFace = 31,	  //����֤����Ա����
	eVoiceDirPleaseEnterFinger = 32,		  //������ָ��
	eVoiceDirPleaseRemoveFinger = 33,		  //���ÿ���ָ
	eVoiceDirRegSuccess = 34,				  //�Ǽǳɹ�
	eVoiceDirRegFailed = 35,				  //�Ǽ�ʧ��
	eVoiceDirPleaseEnterPwd = 36,			  //����������
	eVoiceDirFirstUse = 37,					  //�״�ʹ����¼�����Ա����
	eVoiceDirLowVoltageReplaceBattery = 38,	  //��ѹ�ͣ���������
	eVoiceDirLowVoltageUnableToWork = 39,	  //��ѹ��,�޷�����
	eVoiceDirFactorySet = 40,				  //�ָ����������밴#�ż�ȡ���밴*�ż�
	eVoiceDirFingerFull = 41,				  //ָ������
	eVoiceDirAppSetOpened = 42,				  //APP���ù����Ѵ�
	eVoiceDirDoorIsNotLocked = 43,			  //��δ����
	eVoiceDirFaceInputMiddle = 44,			  //��¼������
	eVoiceDirFaceInputUp = 45,				  //��΢΢̧ͷ
	eVoiceDirFaceInputLow = 46,				  //��΢΢��ͷ
	eVoiceDirFaceInputRight = 47,			  //�����ƫ�����ֱ�
	eVoiceDirFaceInputLeft = 48,			  //�����ƫ�����ֱ�
	eVoiceDirSirensSound_110 = 49,			  //��110��������
	eVoiceDirSystemHasBeenLocked = 50,		  //�Ƿ�����ϵͳ����������ȴ�3�����ٲ���
	eVoiceDirHasBeenLocked = 51,			  //������
	eVoiceDirButtonDing = 52,				  //������"����"
	eVoiceDirButtonDrip = 53,				  //������"����ˮ����"
	eVoiceDirPleaseLiftHandleLocked = 54,	  //��������ַ���
	eVoiceDirHaveProtection = 55,			  //�Ѳ���
	eVoiceDirHaveBeenAbandoned = 56,		  //�ѳ���
	eVoiceDirNoOrdinaryUsers = 57,			  //û����ͨ�û�
	eVoiceDirInsuranceFunctionToCancel = 58,  //���չ���ȡ��
	eVoiceDirInsuranceFunctionStart = 59,	  //���չ�������
	eVoiceDirHelloIHaveInformedMyMaster = 60, //���ã�����֪ͨ����
	eVoiceDirSetupSuccess = 61,				  //�趨�ɹ�
	eVoiceDirChargePleaseLater = 62,		  //����У����Ժ�
	eVoiceDirOpenTheDoorSuccess = 63,		  //���ųɹ�
	eVoiceDirDingDongDingDong = 64,			  //���˶���
	eVoiceDirIdentFail = 65,				  //δʶ��ɹ��������λ������
	eVoiceDirFaceNotDetected = 66,			  //δ��⵽����
	eVoiceDirFaceDistanceTooClose = 67,		  //��������̫��
	eVoiceDirFaceDistanceTooFar = 68,		  //��������̫Զ
	eVoiceDirFaceCovered = 69,				  //�������ڵ�
	eVoiceDirPleaseEnterFingerOrPwd = 70,	  //������ָ�ƻ�����
	eVoiceDirPleaseInputUpBat = 71,			  //������Ϸ����
	eVoiceDirPleaseInputDownBat = 72,			  //������·����
	eVoiceDirUpBatLowCanotWork = 73,			  //�Ϸ�,���,��ѹ�ͣ��޷�����
	eVoiceDirDownBatLowCanotWork = 74,			  //�·�,���,��ѹ�ͣ��޷�����
	eVoiceDirUpBatLow = 75,			  //�Ϸ�,���,��ѹ�ͣ�����
	eVoiceDirDownBatLow = 76,			  //�·�,���,��ѹ�ͣ�����
	eVoiceDirCallingWait = 161,			  //�����У���ȴ�
	eVoiceDirPleaseCallAgainLater = 162,			  //���Ժ��ٲ�
	eVoiceDirNetworkingPleaseHoldOn = 163,     //�����У����Ժ�
    eVoiceFactoryingWait = 164,     //�ָ��������������Ժ�
											  //	eVoiceDirPleaseEnterFinger = 71,		  //������ָ��
											  //	eVoiceDirPleaseRemoveFinger = 72,		  //���ÿ���ָ
	eVoiceDirStop = 0xFE,					  //��
} TYPEe_VoiceDir;
typedef enum
{
	eVoiceStatusNone,	 //����
	eVoiceStatusPowerUp, //�ϵ�
	eVoiceStatusPowerOk, //�ϵ����
	eVoiceStatusStandby, //����
	eVoiceStatusWorking, //������
} TYPEe_VoiceStatus;
typedef struct
{
	TYPEe_VoiceStatus voiceStatus; //��������״̬
	TYPEe_VoiceDir voiceDir;	   //��������״̬
	INT32U waitTime;			   //�ȴ�ʱ��
	INT32U waitTimeCnt;			   //�ȴ�ʱ�������
	INT32U worktime;			   //����ʱ��
	INT32U worktimeCnt;			   //����ʱ�������
	INT32U repeat;				   //�ظ�
	INT32U repeatCnt;			   //�ظ�������
	INT32U bellTimeCnt;			   //ʱ�������
} TYPEs_VoiceCtrl;

typedef enum
{
	eVoiceBellClose = 0, //�ر�
	eVoiceBellOpen = 1,	 //����
} TYPEe_VoiceBell;
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_VOICE_GLOBALS
#define HAL_VOICE_EXT
#else
#define HAL_VOICE_EXT extern
#endif /*HAL_VOICE_GLOBALS*/

	/*************************��������*************************/
	//HAL_VOICE_EXT TYPEe_voiceDir voiceDir;
	HAL_VOICE_EXT TYPEs_VoiceCtrl voiceCtrl;
	/*************************��������*************************/
	HAL_VOICE_EXT void HAL_Voice_Init(void);
	HAL_VOICE_EXT void HAL_VoicePlay(void);
	HAL_VOICE_EXT void HAL_VoicePlayDir(TYPEe_VoiceDir dat);
	HAL_VOICE_EXT void HAL_VoiceDeal(void);
	HAL_VOICE_EXT INT8S HAL_Voice_ChkBusy(void);
	HAL_VOICE_EXT void HAL_Voice_Bell(uint8_t dat);
	HAL_VOICE_EXT void HAL_Voice_EnterSleepConfig(void);
	HAL_VOICE_EXT void HAL_Voice_VolumeSet(uint8_t dat);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_VOICE_MODULE_EN*/

#endif /*HAL_VOICE_MODULE_EN*/
#endif /*__HAL_VOICE_H__*/
/************************************Code End******************************************/
