/***************************************************************************************
**�ļ���:     APP_Power.c
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
#include "sys.h"
#include "debug.h"
#include "type.h"
#include "reboot_demo.h"
#include "main.h"
#include "i2c_reg.h"
#include "cpm_drv.h"
#include "common.h"
#include "APP_Power.h"
#include "HAL_Power.h"
#include "HAL_KeySacn.h"
//#include "APP_Storage.h"
#include "HAL_Motor.h"
#include "HAL_SysTimer.h"
#include "HAL_Finger.h"
#include "Function.h"
#include "HAL_RTC.h"
#include "HAL_Comm.h"
#include "HAL_Voice.h"
#include "pci_drv.h"
#include "HAL_KeySacn.h"
#include "HAL_FaceId.h"
#include "HAL_RTC.h"
#include "HAL_Comm.h"
#include "HAL_Voice.h"
#include "HAL_Adc.h"
#include "HAL_Alarm.h"
#include "HAL_Voice.h"
#include "HAL_KeyLight.h"
#include "HAL_Storage.h"
#include "ccm_drv.h"
#include "id2_client.h"
#include "HAL_IR.h"

//����ʹ�õ���ͷ�ļ�
#include <cm_backtrace.h>
#include "DRV_MBI5024.h"
//#include "reset_drv.h"
#if APP_POWER_MODULE_EN
#warning APP_POWERģ��ʹ��!
/*******************************�궨��***************************/
#define HARDWARE_VERSION "V1.0.0" //����׷�ٿ�
#define SOFTWARE_VERSION "V0.1.0" //����׷�ٿ�
//#define DELAY_MS(X) DelayMS(X)	  //��ʱ
//extern void IO_Latch_Clr(void);
/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/
//RST_SOURCE_STA rst_source1;
/*************************�ֲ�����*****************************/
static void log_init(void)
{
    //    ret_code_t err_code = NRF_LOG_INIT(NULL);
    //    APP_ERROR_CHECK(err_code);

    //    NRF_LOG_DEFAULT_BACKENDS_INIT();
}
/***************************************************************************************
**������:       APP_Power_Init
**��������:     �ϵ��ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_SysWakeToMenu(void)
{
}
///***************************************************************************************
//**������:       APP_Power_Init
//**��������:     �ϵ��ʼ��
//**�������:     --
//**�������:     --
//**��ע:
//****************************************************************************************/
//static void APP_Power_KeyRandomGet(void)
//{
//
//}
static void Printf_Version(void)
{
    printf("/**************************************************/\n");
    printf(" date = %s,time = %s    \r\n", (char *)__DATE__, (char *)__TIME__);
    printf(" C*CORE CCM4202S Demo V2.0  \r\n");
    printf(" system clk = %dMHz         \r\n", g_sys_clk / 1000000);
    printf(" ips clk    = %dMHz         \r\n", g_ips_clk / 1000000);
    printf("/**************************************************/\n");
}
//#ifdef DEBUG_LOG_EN
/***************************************************************************************
**������:       Debug_Uart_Init
**��������:     ���Կڳ�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void Debug_Uart_Init(void)
{
    UART_InitTypeDef UART_InitStruct;
#if 1
//    UART_InitStruct.UART_BaudRate = 115200;
	UART_InitStruct.UART_BaudRate = 921600;
#endif
#if 0
    UART_InitStruct.UART_BaudRate = 19200;
#endif
    //	UART_InitStruct.UART_BaudRate = 56000;
    UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
    UART_InitStruct.UART_Mode = UART_INT_MODE; //����CPU�жϷ�ʽ
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(SCI2, &UART_InitStruct);
}
//#endif /*DEBUG_LOG_EN*/
/*************************ȫ�ֺ���****************************/
//uint16_t temp;
//#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
//typedef struct
//{
//	uint8_t b1 :1;
//	uint8_t b2 :1;
//	uint8_t b3 :1;
//	uint8_t b4 :1;
//	uint8_t b5 :1;
//	uint8_t b6 :1;
//	uint8_t b7 :1;
//	uint8_t b8 :1;
//} TYPEs_test;
//union test1
//{
//	TYPEs_test a;
//	uint8_t B;
//};
//union test1 testA;
//#pragma pack()                     //ȡ������
void Clear_IO_LAUCH(void)
{
    CPM->CPM_PWRCR = (CPM->CPM_PWRCR & ~(1 << 30)) | (1 << 31);
    CPM->CPM_PWRCR &= ~(1 << 31);
}
/***************************************************************************************
**������:       APP_DefInit
**��������:     ����������ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_DefInit(void)
{
    if(0 == HAL_Bufang_Clear()) //�����־λ
    {
        st_InputInfo.defSysTickCntEn = 0; //�����־λ
        st_InputInfo.defSysTickCnt = 0;
    }
}
/***************************************************************************************
**������:       APP_DefSet
**��������:     ������������
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_DefSet(void)
{
    if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance))
    {
        DEBUG_LOG_OUT("APP_DefSet\n");
        st_InputInfo.defSysTickCntEn = 1; //������ʱ��
        HAL_BuFang_Time_Init(st_InputInfo.defEn,(uint32_t *)&st_InputInfo.defSysTickCnt);
    }
}

/***************************************************************************************
**������:       APP_GetBackBat
**��������:     ��ȡ���ĵ�Դ������ʽ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_GetBackBat(void)
{

    uint32_t timeCnt = 2000; //�������ʱ����3��
    DEBUG_LOG_OUT("WakeUpSource HEX %X\n",WakeUpSource);

    if(eWakeUpSourceBack != WakeUpSource) //���Ǻ�廽�ѵ����
    {
        if(eStorageStatusError != st_InputInfo.storageStatus) //ֻ�д洢��ȷ��ʱ���ѯ��
        {
            HAL_Comm_SendDealStart(eCommGetBattery, 0); //��ѯ��ص���
        }
    }
    else
    {
        fn_JumpMenu(enum_Menu_WakeUp); //�ȴ�����Դ����
    }
    while(timeCnt --)
    {
        DELAY_MS(1);
        Menu_Comm_Service();
        if(1 == st_InputInfo.backbatGetFlag) //������ȡ���
        {
            DEBUG_LOG_OUT("Get back bat ok!\n");
            break;
        }
    }
    if(0 == timeCnt) //�����ʱ
    {
        DEBUG_LOG_OUT("Get back bat Erorr!\n");
        //ϵͳ������������
    }

}
/***************************************************************************************
**������:       APP_SysWakeUpToMenu
**��������:     ���ݻ���Դ����˵�
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_SysWakeUpToMenu(void)
{
    uint8_t tryTestFlag = 0;
	if(1 == st_InputInfo.lowPowerCannotWork) //ϵͳ�޷�����
	{
		fn_JumpMenu(enum_Menu_SystemSleep); //��������
		return;
	}
    if ((eStorageStatusError == st_InputInfo.storageStatus) && (st_MenuItem.e_CurrentIndex != enum_Menu_Factory))
    {
        DEBUG_LOG_OUT("APP_SysWakeUpToMenu fn_JumpMenu(enum_Menu_StorageError); \n");
#if 1
        fn_JumpMenu(enum_Menu_StorageError);
#else
        fn_JumpMenu(enum_Menu_WakeUp); //�ȴ�����Դ����
#endif
    }
    else if (eWakeUpSourceAlarm == WakeUpSource) //���ȼ��Ƚϸ�
    {
        DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceAlarm\n");
        //		HAL_Comm_SendDealStart(eCommGetBattery, 0); //��ѯ��ص���
        fn_JumpMenu(enum_Menu_WakeUp); //�ȴ�����Դ����
    }
    else
    {

        switch (WakeUpSource)
        {
        case eWakeUpSourceNull:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceNull \n");
            tryTestFlag = 1;
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourceKey:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceKey\n");
            tryTestFlag = 1;
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourcePOR:
            tryTestFlag = 1;

            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourcePOR\n");
#if 0
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
#else
            fn_JumpMenu(enum_Menu_WifiConnect); //��һ���ϵ�wifi��������
#endif
            break;
        case eWakeUpSourceBack:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceBack \n");
#ifndef BACK_GET_BAT_STATUS
            fn_JumpMenu(enum_Menu_WakeUp); //�ȴ�����Դ����
#endif
            break;
        case eWakeUpSourceCatEye:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceCatEye \n");
            fn_JumpMenu(enum_Menu_CatEyeConn);
            break;
        case eWakeUpSourceFinger:
            APP_DefSet(); //��������
            tryTestFlag = 1;
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceFinger \n");
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourceIr:
            APP_DefSet(); //��������
            tryTestFlag = 1;
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceIr \n");
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourceActiveDefense:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceActiveDefense\n");
            st_InputInfo.defSysTickCntEn = 0; //�����־λ
            st_InputInfo.defSysTickCnt = 0;   //���������
            fn_JumpMenu(enum_Menu_ActiveDefense);
            break;
		case eWakeUpSourceBac:
            break;
        default:
            tryTestFlag = 1;
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu other \n");
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        }
        if (1 == tryTestFlag)
        {
            if ((0 != Menu_ChkError(&st_InputInfo.errorCnt,eErrorCheckTimeYes)))
            {
                wifiSendParm.sendType = eWifiSendTypeAlarmTry;
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //��ʼ��������
                Menu_Face_PowerOffStart();

                fn_JumpMenu(enum_Menu_TryAlarm); //���Ա���
            }
        }
    }
}
/***************************************************************************************
**������:       APP_StorageInit
**��������:     ϵͳ�洢��ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_StorageInit(void)
{
#if 0
    HAL_Storage_Init();
#endif
#if 0
    HAL_Storage_ClearSysData();                                   //���ϵͳ����,��д���ʼ����
    st_InputInfo.storageStatus = eStorageStatusSuccess;		 //�洢������
#endif
#if 1
    if (0 != HAL_Storage_ChkSysData()) //��ȡ�����
    {
        st_InputInfo.storageStatus = eStorageStatusError; //�洢������
        storageSave.sysDat.irDef = 0 ;
        st_InputInfo.defEn = storageSave.sysDat.irDef; //���������Ĳ���
        DEBUG_LOG_OUT("HAL_Storage_ChkSysData() eStorageStatusError\n");
        //		DelayMS(1);
        //�洢������
    }
    else
    {
        DEBUG_LOG_OUT("HAL_Storage_ChkSysData() eStorageStatusSuccess\n");
		DEBUG_LOG_OUT("storageSave.sysDat.localAdminRegFlag X:%X\n",storageSave.sysDat.localAdminRegFlag);
        if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag)
        {
            st_InputInfo.keyRandomSetFlag = ekeyRandomSetEnable;
        }
        else
        {
            st_InputInfo.keyRandomSetFlag = ekeyRandomSetDisable;
        }
        st_InputInfo.storageStatus = eStorageStatusSuccess;		 //�洢������
        HAL_Storage_ReadHardwareData(&storageSave.hardwareData); //��ȡӲ����Ϣ
        if ((LOCK_BODY_TYPE_LIUFU != storageSave.hardwareData.lockBodyType) &&
                (LOCK_BODY_TYPE_KAIYUANLE != storageSave.hardwareData.lockBodyType))
        {
            storageSave.hardwareData.lockBodyType = LOCK_BODY_TYPE_LIUFU; //Ĭ������������
        }
        if ((storageSave.hardwareData.dir == 0xff) ||
                ((storageSave.hardwareData.dir != 0x55) && (storageSave.hardwareData.dir != 0x01))) //ѡ���ŷ���
        {
            storageSave.hardwareData.dir = 0x55;
        }
        if ((storageSave.hardwareData.torque[0] > 3) ||
                (storageSave.hardwareData.torque[0] == 0)) //��������Ť��
        {
            storageSave.hardwareData.torque[0] = 1;
        }
        DEBUG_LOG_OUT("storageSave.hardwareData.dir:%X\n",storageSave.hardwareData.dir);
        DEBUG_LOG_OUT("storageSave.hardwareData.torque[0]:%X\n",storageSave.hardwareData.torque[0]);
#if 0
        uint8_t test[]= {0x13,0x28,0x81,0x18,0xE5,0x03};
        memcpy(storageSave.hardwareData.encryptKey,
               test,
               6);
#endif
#if 0
        HAL_Comm_AesKeySet(storageSave.hardwareData.encryptKey); //����AES����
#endif
#if 0
        pCommRootKeyA= storageSave.hardwareData.encryptKey; //AES����
#endif
#if 0
        pCommMac = storageSave.hardwareData.mac; //MACָ��
#endif
#if 0
        DEBUG_LOG_OUT("mac:\n");
        DEBUG_LOG_OUT_HEX(pCommMac,6);
        DEBUG_LOG_OUT("root:\n");
        DEBUG_LOG_OUT_HEX(pCommRootKeyA,6);
#endif
        HAL_Comm_EncryptionParam(storageSave.hardwareData.mac,
                                 storageSave.hardwareData.encryptKey);
        st_InputInfo.defEn = storageSave.sysDat.irDef; //���������Ĳ���
    }
#endif
}
/***************************************************************************************
**������:       APP_Finger_Init
**��������:     ָ�Ƴ�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Finger_Init(void)
{
    if (0 == HAL_Finger_Init())
    {
        st_InputInfo.modeuleFail.Finger = 0; //ָ����Ч
    }
    else
    {
        st_InputInfo.modeuleFail.Finger = 1;
    }
}
/***************************************************************************************
**������:       APP_GetPowerValue
**��������:     ��ȡ��ѹֵ
**�������:     --
**�������:     --
**��ע:         ������ʽ
****************************************************************************************/
void APP_GetPowerValue(void)
{
#ifdef BACK_GET_BAT_STATUS

#else
    HAL_Adc_Conversion(&st_InputInfo.upBatBattery, &st_InputInfo.downBatBattery);
#if 1
    st_InputInfo.upBatBattery = 800;
    st_InputInfo.downBatBattery = 800;
#endif
#endif
    DEBUG_LOG_OUT("st_InputInfo.upBatBattery:%d\n", st_InputInfo.upBatBattery);
    DEBUG_LOG_OUT("st_InputInfo.downBatBattery:%d\n", st_InputInfo.downBatBattery);
#if 0
    if ((st_InputInfo.upBatBattery < DC_STATUS_LOW) && (st_InputInfo.upBatBattery > DC_STATUS_LOW_SYSTEM_NOT_WORK))
    {
        HAL_KeyLight_batterySetDir(eColorRed); //���
        HAL_VoicePlayDir(eVoiceDirLowVoltageReplaceBattery);
        DELAY_MS(2000);
    }
    else if (st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK)
    {
        HAL_KeyLight_batterySetDir(eColorRed); //���
        HAL_VoicePlayDir(eVoiceDirLowVoltageUnableToWork);
        DELAY_MS(2000);
    }
    else
    {
        HAL_KeyLight_batterySetDir(eColorGreen); //�̵�
    }
#endif
#if 1
    if (((st_InputInfo.upBatBattery >= DC_STATUS_LOW) || (st_InputInfo.upBatBattery <= 100)) && ((st_InputInfo.downBatBattery >= DC_STATUS_LOW) || (st_InputInfo.downBatBattery <= 100)))
    {
        DEBUG_LOG_OUT("eColorGreen\n");
        HAL_KeyLight_batterySetDir(eColorGreen); //�̵�
    }
    else
    {
        DEBUG_LOG_OUT("eColorRed\n");
        HAL_KeyLight_batterySetDir(eColorRed); //���
    }
    if (st_InputInfo.upBatBattery <= 100)
    {

        DEBUG_LOG_OUT("eVoiceDirPleaseInputUpBat\n");
        HAL_VoicePlayDir(eVoiceDirPleaseInputUpBat); //����룬�Ϸ������
        DELAY_MS(2500);
    }
    if (st_InputInfo.downBatBattery <= 100)
    {
        DEBUG_LOG_OUT("eVoiceDirPleaseInputDownBat\n");
        HAL_VoicePlayDir(eVoiceDirPleaseInputDownBat); //����룬�·������
        DELAY_MS(2500);
    }
    if ((st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK) && (st_InputInfo.downBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK))
    {
        if (st_InputInfo.upBatBattery > 100)
        {
            DEBUG_LOG_OUT("eVoiceDirUpBatLowCanotWork\n");
            HAL_VoicePlayDir(eVoiceDirUpBatLowCanotWork); //�Ϸ�,���,��ѹ�ͣ��޷�����
            DELAY_MS(3000);
        }
        if (st_InputInfo.downBatBattery > 100)
        {
            DEBUG_LOG_OUT("eVoiceDirDownBatLowCanotWork\n");
            HAL_VoicePlayDir(eVoiceDirDownBatLowCanotWork); //�·�,���,��ѹ�ͣ��޷�����
            DELAY_MS(3000);
        }
		st_InputInfo.lowPowerCannotWork = 1;
        
		return;
    }
    if ((st_InputInfo.upBatBattery < DC_STATUS_LOW) && (st_InputInfo.upBatBattery > 100)) //&& //�Ϸ���
    {
        // if (st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK)
        // {
        // 	M_VBAT2_EN2_OUT_PP();
        // 	M_VBAT2_EN2_FUN_ON;
        // }
        DEBUG_LOG_OUT("eVoiceDirUpBatLow\n");
        HAL_VoicePlayDir(eVoiceDirUpBatLow); //�Ϸ�,���,��ѹ�ͣ�����
        DELAY_MS(3000);
    }
    if ((st_InputInfo.downBatBattery < DC_STATUS_LOW) && (st_InputInfo.downBatBattery > 100)) //�·���
    {
        DEBUG_LOG_OUT("eVoiceDirDownBatLow\n");
        HAL_VoicePlayDir(eVoiceDirDownBatLow); //�·�,���,��ѹ�ͣ�����
        DELAY_MS(3000);
    }
#endif
}
/***************************************************************************************
**������:       APP_Comm_Init
**��������:     ͨѶ���̳�ʼ��
**�������:     --
**�������:     --
**��ע:         ������ʽ
****************************************************************************************/
static void APP_Comm_Init(void)
{
    if(eWakeUpSourcePOR == WakeUpSource) //�ϵ縴λ
    {
#if 0
        HAL_Comm_RstBack(); //�ϵ�ͬʱ��λ���
#else
        HAL_Comm_InitBack();
#endif
    }
    HAL_Comm_Init();	  //ǰ�����ͨѶ��ʼ��
}
/***************************************************************************************
**������:       APP_WakeupRTCTimerGet
**��������:     ��ȡRTC
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
int8_t APP_WakeupRTCTimerGet(void)
{
    uint32_t tick;
    HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
    if(st_InputInfo.RTCTimer.tm_year < 2020) //ʱ����2020��֮ǰ,ʱ�����õ�2020-01-01 00:00:00
    {
        DEBUG_LOG_OUT("reset RTC\n");
        HAL_RTC_SetByTick(1577808000);
        HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer); //���¶�ȡʱ��
    }
    return 0;
}

/***************************************************************************************
**������:       APP_FaceCatEyeInit
**��������:     ������è�۳�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_FaceCatEyeInit(void)
{
    HAL_FaceId_ClosePow(); //�ر�������Դ
    if((1 == storageSave.sysDat.ShangTangCatEyeSingleDouble)||
            (MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) )    //����,����è��
    {
        HAL_Face_WifiPowClose(); //�ر�è�۵�Դ
    }
	HAL_IR_IOInit();
}
/***************************************************************************************
**������:       APP_FaceCatEyeInit
**��������:     ������è�۳�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_VolumeSetInit(void)
{
    DEBUG_LOG_OUT("storageSave.sysDat.voiceSw:%d\n",storageSave.sysDat.voiceSw);
    HAL_Voice_VolumeSet((uint8_t)(storageSave.sysDat.voiceSw));
}
/***************************************************************************************
**������:       APP_SysTimerHandler
**��������:     ϵͳʱ���жϷ������
**�������:     --
**�������:     --
**��ע:         ��Ҫ��ӵ�ϵͳ�δ�ʱ�ӵı�����������ӵ����
****************************************************************************************/
void APP_SysTimerHandler(void)
{
    sysTimerCtrl.tick++;  //ϵͳʱ���ۼ�
    if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance))
    {
        if(1 == st_InputInfo.defSysTickCntEn) //��Ҫ��������ʹ�ܲ��ܼ���
        {
            st_InputInfo.defSysTickCnt ++;
        }
    }
}
/***************************************************************************************
**������:       APP_SysTimerInit
**��������:     ϵͳʱ�ӳ�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_SysTimerInit(void)
{
    HAL_SysTimerInit();			  //ϵͳʱ�ӳ�ʼ��
    sysTimerCtrl.tick = 0;
    HAL_SysTimerHandlerReg(APP_SysTimerHandler);
}

#if USE_WDT >0
#define WDT_FEED_DOG do{HAL_WDTFeedDog();}while(0)
#else
#define WDT_FEED_DOG
#endif

/***************************************************************************************
**������:       APP_Power_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_Init(void)
{
	WDT_FEED_DOG;
    HAL_Power_CPUInit();	  //CPU��ʼ��
	WDT_FEED_DOG;
    HAL_Power_SysWakeupChk(); //��ѯϵͳ����Դ
#ifdef DEBUG_OUT_USE_JLINK_RTT
    SEGGER_RTT_Init();
#endif
//#ifdef DEBUG_LOG_EN
	WDT_FEED_DOG;
    Debug_Uart_Init();
#ifdef DIS_RAM_MODE
    Disboot_Demo();
#endif
#if 0
    void App_Demo(void);
    while(1)
    {
        App_Demo();
    }
#endif
//#endif					  /*DEBUG_LOG_EN*/
	WDT_FEED_DOG;
    HAL_Power_TrngInit(); //�����������
	WDT_FEED_DOG;
    APP_Comm_Init(); //ǰ��ͨѶ��ʼ��
	WDT_FEED_DOG;
    HAL_Voice_Init();	  //������ʼ��
	WDT_FEED_DOG;
    HAL_KeyLightInit();	  //ע�⣬��������ĵ�Դ��FLASH����һ����;
    DEBUG_LOG_OUT("START!\r\n");
    HAL_Power_ReservedWakeupConfig();		//δʹ�õ���IO��ʼ��
	WDT_FEED_DOG;
    HAL_Power_ReuseInterruptWakeupConfig(); //�ж�����
    DEBUG_LOG_OUT("CCM->RTCCFG12=0x%04x\r\n", CCM->RTCCFG12);
	DEBUG_LOG_OUT("V3.11 HX   @@\r\n");
	DEBUG_LOG_OUT("V3.11 HX   @@\r\n");
	WDT_FEED_DOG;
    HAL_RTC_Init();		   //RTC��ʼ��
	WDT_FEED_DOG;
    Function_SysRamInit(); //ϵͳRAM����
	WDT_FEED_DOG;
#if USE_ID2 >0
    st_InputInfo.iD2Status = eID2StatusSuccess;
	WDT_FEED_DOG;
    id2_client_init();  //ID2��ʼ��
#else
    st_InputInfo.iD2Status = eID2StatusError;
#endif
	WDT_FEED_DOG;
    HAL_Alarm_SetWakeUp(); //��������
    e_CPM_wakeupSource_Status rst_source = *(e_CPM_wakeupSource_Status *)CPM_WAKEUPSOURCE_REG;
    DEBUG_LOG_OUT("rst_source1:%d\n", rst_source);
//	DEBUG_LOG_OUT("WakeUpSource:%d\n", WakeUpSource);
    DEBUG_LOG_OUT("System powerup ok!\n");
    cm_backtrace_init("ccm4202s_demo_efm", HARDWARE_VERSION, SOFTWARE_VERSION);
	WDT_FEED_DOG;
    APP_StorageInit(); //��ʼ���洢
	WDT_FEED_DOG;
    APP_FaceCatEyeInit(); //������è�۳�ʼ��
	WDT_FEED_DOG;
    APP_WakeupRTCTimerGet();		  //��ȡRTCʱ��
	WDT_FEED_DOG;
    HAL_KeySacnInit();			  //������ʼ��
	WDT_FEED_DOG;
    APP_Finger_Init();			  //ָ�Ƴ�ʼ��
	WDT_FEED_DOG;
    APP_VolumeSetInit();		  //��������
	WDT_FEED_DOG;
    APP_DefInit();                //����������ʼ��
	WDT_FEED_DOG;
#if 0	//һ����������
	storageSave.sysDat.keyDef = 1;
#endif		
#if DEBUG_LOW_POWER > 0
#else
    APP_GetBackBat();             //��ȡ������Ϣ
#endif
#ifndef BACK_GET_BAT_STATUS
    APP_GetPowerValue();		  //��ȡ��ѹֵ,�������������������Ϣ��
#endif
	WDT_FEED_DOG;
    APP_SysWakeUpToMenu();		  //���ݻ���Դ����˵�
	WDT_FEED_DOG;
    APP_SysTimerInit();           //ϵͳ�δ�ʱ�ӳ�ʼ��
	WDT_FEED_DOG;
#if USE_WDT >0
//	HAL_InitWDT(0x08ff);
#endif
}
/***************************************************************************************
**������:       APP_Power_WakeupInit
**��������:     ϵͳ���ѳ�ʼ��
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_WakeupInit(void)
{

}
/***************************************************************************************
**������:       APP_Power_WifiShutdown
**��������:     wifi�ػ�
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_WifiShutdown(void)
{
    if((1 == storageSave.sysDat.ShangTangCatEyeSingleDouble)||
            (0 != Menu_LowPowerChk())||
            (MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw))     //����ǰ,����è��
    {
        HAL_Face_WifiPowClose(); //�ر�è�۵�Դ
    }
}

/***************************************************************************************
**������:       APP_Power_IRSleepConfig
**��������:     IR�����Ӧ��������
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_IRSleepConfig(void)
{
	if(2 == storageSave.sysDat.IrDistance)
	{
		HAL_FaceId_IRSleepConfig(0);
	}
	else
	{
		HAL_FaceId_IRSleepConfig(1);
	}
}
/***************************************************************************************
**������:       APP_Power_EnterSleepConfig
**��������:     ϵͳ������������
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_EnterSleepConfig(void)
{
    HAL_Voice_EnterSleepConfig(); //����
    DEBUG_LOG_OUT("HAL_Voice_EnterSleepConfig();\n");
    HAL_Alarm_SetInterrpt(); //����
    DEBUG_LOG_OUT("HAL_Alarm_SetInterrpt\n");
    HAL_Finger_EnterSleepConfig(); //ָ��
    DEBUG_LOG_OUT("HAL_Finger_EnterSleepConfig\n");
    HAL_KeySacn_EnterSleepConfig(); //����
    DEBUG_LOG_OUT("HAL_KeySacn_EnterSleepConfig\n");
    HAL_Face_EnterSleepConfig(); //����
	APP_Power_IRSleepConfig(); //�����Ӧ����
    APP_Power_WifiShutdown(); //wifi�رյ�Դ
    DEBUG_LOG_OUT("HAL_Face_EnterSleepConfig\n");
    HAL_KeyLight_EnterSleepConfig(); //�ƹ�
    DEBUG_LOG_OUT("HAL_KeyLight_EnterSleepConfig\n");
    HAL_Power_ReservedSleepConfig(); //δʹ�õ�IO
    DEBUG_LOG_OUT("HAL_Power_ReservedSleepConfig\n");
    HAL_COMM_EnterSleepConfig(); //ͨѶ
    DEBUG_LOG_OUT("HAL_COMM_EnterSleepConfig\n");
    HAL_Storage_EnterSleepConfig(); //�洢
    DEBUG_LOG_OUT("HAL_Storage_EnterSleepConfig\n");
    HAL_Adc_EnterSleepConfig(); //ADC
    DEBUG_LOG_OUT("HAL_Adc_EnterSleepConfig\n");
}
/***************************************************************************************
**������:       APP_Power_EnterSleep
**��������:     ϵͳ��������
**�������:     --
**�������:     --
**��ע:
****************************************************************************************/
void APP_Power_EnterSleep(void)
{
    HAL_Power_EnterSleep();
}
#endif /*APP_POWER_MODULE_EN*/
/************************************Code End******************************************/
