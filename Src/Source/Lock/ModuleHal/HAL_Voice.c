/***************************************************************************************
**�ļ���:     HAL_Voice.c
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
#include "HAL_Voice.h"
#include "DRV_Voice.h"
#if HAL_VOICE_MODULE_EN
#warning HAL_VOICEģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_VOICE_WAIT_CHIP_OK (5) //�ȵ�оƬ��Ӧ
#define HAL_VOICE_RUN_TIME_MAX (10) //����ʱ��
#define HAL_VOICE_DATA_LEN (16)     //���ݳ���
#define HAL_VOICE_0 (0)             //����
#define HAL_VOICE_1 (1)             //����
#define HAL_VOICE_2 (2)             //����
#define HAL_VOICE_3 (3)             //����
#define HAL_VOICE_4 (4)             //����
#define HAL_VOICE_5 (5)             //����
#define HAL_VOICE_6 (6)             //����
#define HAL_VOICE_7 (7)             //����
#define HAL_VOICE_8 (8)             //����
#define HAL_VOICE_9 (9)             //����
#define HAL_VOICE_ENTER (11)        //����
#define HAL_VOICE_MENU (10)         //����
#define HAL_VOICE_CLOSE (12)
#define HAL_VOICE_BELL (13)
//#define HAL_VOICE_WAIT_CHIP_OK (1) //�ȵ�оƬ��Ӧ
/*************************.C�����ṹ����***********************/
//typedef enum
//{
//	eVoiceStatusNone,	 //����
//	eVoiceStatusPowerUp, //�ϵ�
//	eVoiceStatusPowerOk, //�ϵ����
//	eVoiceStatusStandby, //����
//	eVoiceStatusWorking, //������
//} TYPEe_VoiceStatus;
//typedef struct
//{
//	TYPEe_VoiceStatus voiceStatus; //��������״̬
//	TYPEe_VoiceDir voiceDir;	   //��������״̬
//	INT32U waitTime;			   //�ȴ�ʱ��
//	INT32U waitTimeCnt;			   //�ȴ�ʱ�������
//	INT32U worktime;			   //����ʱ��
//	INT32U worktimeCnt;			   //����ʱ�������
//	INT32U repeat;				   //�ظ�
//	INT32U repeatCnt;			   //�ظ�������
//} TYPEs_VoiceCtrl;
/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/
TYPEs_VoiceCtrl voiceCtrl =
	{
		.voiceStatus = eVoiceStatusNone,
		.voiceDir = eVoiceDirStop,
		.waitTimeCnt = 0,
		.worktime = 0,
		.worktimeCnt = 0,
		.repeat=0,
		.repeatCnt = 0,
};

/*************************�ֲ�����*****************************/

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_Voice_Init
**��������:     ������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Voice_Init(void)
{
	DRV_Voice_Init();
	voiceCtrl.repeat = 0;	//�ظ��������
	voiceCtrl.waitTime = 0; //���ȴ�
	voiceCtrl.worktime = 0; //����ʱ��
}
/***************************************************************************************
**������:       HAL_VoicePlay
**��������:     �������ţ�ʹ��״̬������
**�������:     
TYPEe_VoiceDir dat, //���ŵ�����
INT32U workTime //����ʱ�䣬0 ��ʾ��ǿ�Ƶȴ���

**�������:     --
**��ע:         
****************************************************************************************/
void HAL_VoicePlay(void)
{
//	voiceCtrl.voiceDir = dat;
//	voiceCtrl.waitTime = waitTime;
	voiceCtrl.waitTimeCnt = 0;
	voiceCtrl.voiceStatus = eVoiceStatusStandby;
//	voiceCtrl.worktime = workTime;
	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.repeat = repeat;
	voiceCtrl.repeatCnt = 0;
}
//void HAL_VoicePlay(TYPEe_VoiceDir dat, INT32U waitTime, INT32U workTime, INT32U repeat)
//{
//	voiceCtrl.voiceDir = dat;
//	voiceCtrl.waitTime = waitTime;
//	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.voiceStatus = eVoiceStatusStandby;
//	voiceCtrl.worktime = workTime;
//	voiceCtrl.waitTimeCnt = 0;
//	voiceCtrl.repeat = repeat;
//	voiceCtrl.repeatCnt = 0;
//}
/***************************************************************************************
**������:       HAL_VoicePlayDir
**��������:     ֱ�Ӳ��ţ���ͨ��״̬����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_VoicePlayDir(TYPEe_VoiceDir dat)
{
	INT8U datTmp = 0;
	datTmp = (INT8U)(dat);
	DRV_Voice_Play(datTmp);
	//	WTN4_data(datTmp);
}
/***************************************************************************************
**������:       HAL_Voice_ChkBusy
**��������:     ��������Ƿ���æµ״̬
**�������:     --
**�������:     
INT8S
0; //����
-1; //æµ
**��ע:         
****************************************************************************************/
INT8S HAL_Voice_ChkBusy(void)
{
	if (eVoiceStatusNone == voiceCtrl.voiceStatus)
	{
		return 0; //����
	}
	else
	{
		return -1; //æµ
	}
}

/***************************************************************************************
**������:       --
**��������:     --
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_VoiceDeal(void)
{
	if (0 != voiceCtrl.bellTimeCnt)
	{
		if(1 == voiceCtrl.bellTimeCnt)
		{
			DRV_Voice_Bell(eVoiceBellClose);
		}
		else if (voiceCtrl.bellTimeCnt >= HAL_VOICE_BELL_CLOSE_TIME)
		{
			DRV_Voice_Bell(eVoiceBellClose);
		}
		else
		{
			DRV_Voice_Bell(eVoiceBellOpen);
		}
		voiceCtrl.bellTimeCnt --;
	}
	switch (voiceCtrl.voiceStatus)
	{
	case eVoiceStatusNone:
		break;
	case eVoiceStatusPowerUp:
		break;
	case eVoiceStatusPowerOk:
		break;
	case eVoiceStatusStandby:
		voiceCtrl.waitTimeCnt++;
		if (voiceCtrl.waitTimeCnt >= voiceCtrl.waitTime)
		{
			HAL_VoicePlayDir(voiceCtrl.voiceDir); //��������
			DEBUG_LOG_OUT("HAL_VoicePlayDir(voiceCtrl.voiceDir); \n");
//			DEBUG_LOG_OUT("voiceCtrl.worktime:%X\n",voiceCtrl.worktime);
			voiceCtrl.waitTimeCnt = 0;
			voiceCtrl.voiceStatus = eVoiceStatusWorking; //���빤������
		}
		break;
	case eVoiceStatusWorking: //Ԥ��
		voiceCtrl.waitTimeCnt++;
		if (voiceCtrl.waitTimeCnt >= voiceCtrl.worktime)
		{
			voiceCtrl.waitTimeCnt = 0;
			voiceCtrl.repeatCnt ++;
			if(voiceCtrl.repeatCnt >= voiceCtrl.repeat)
			{
				voiceCtrl.repeat = 0;	//�ظ��������
				voiceCtrl.waitTime = 0; //���ȴ�
				voiceCtrl.worktime = 0; //����ʱ��
				voiceCtrl.voiceStatus = eVoiceStatusNone; //��������
			}
			else
			{
				voiceCtrl.voiceStatus = eVoiceStatusStandby; //�ٴβ���
			}
		}
		break;
	default:
		break;
	}
}

/***************************************************************************************
**������:       DRV_Voice_Bell
**��������:     �������忪��
**�������:     
uint8_t dat
0 ����
���� �ر�
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Voice_Bell(uint8_t dat)
{
	switch (dat)
	{
	case HAL_VOICE_BELL:
		voiceCtrl.bellTimeCnt = HAL_VOICE_BELL_WORK_TIME;
		break;
	default:
		break;
	}
}

/***************************************************************************************
**������:       HAL_Voice_EnterSleepConfig
**��������:     ����͹�������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Voice_EnterSleepConfig(void)
{
	DRV_Voice_EnterSleepConfig();
	DRV_Voice_BellEnterSleepConfig();
}
/***************************************************************************************
**������:       HAL_Voice_VolumeSet
**��������:     ������������
**�������:     --
**�������:     --
**��ע:         ����������Ҫ��Դ�ȶ���
****************************************************************************************/
void HAL_Voice_VolumeSet(uint8_t dat)
{
	DRV_Voice_VolumeSet(dat);
}
#endif /*HAL_VOICE_MODULE_EN*/
/************************************Code End******************************************/
