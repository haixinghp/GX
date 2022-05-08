/***************************************************************************************
**文件名:     APP_Power.c
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

//测试使用到的头文件
#include <cm_backtrace.h>
#include "DRV_MBI5024.h"
//#include "reset_drv.h"
#if APP_POWER_MODULE_EN
#warning APP_POWER模块使能!
/*******************************宏定义***************************/
#define HARDWARE_VERSION "V1.0.0" //用于追踪库
#define SOFTWARE_VERSION "V0.1.0" //用于追踪库
//#define DELAY_MS(X) DelayMS(X)	  //延时
//extern void IO_Latch_Clr(void);
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
//RST_SOURCE_STA rst_source1;
/*************************局部函数*****************************/
static void log_init(void)
{
    //    ret_code_t err_code = NRF_LOG_INIT(NULL);
    //    APP_ERROR_CHECK(err_code);

    //    NRF_LOG_DEFAULT_BACKENDS_INIT();
}
/***************************************************************************************
**函数名:       APP_Power_Init
**功能描述:     上电初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_SysWakeToMenu(void)
{
}
///***************************************************************************************
//**函数名:       APP_Power_Init
//**功能描述:     上电初始化
//**输入参数:     --
//**输出参数:     --
//**备注:
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
**函数名:       Debug_Uart_Init
**功能描述:     调试口初始化
**输入参数:     --
**输出参数:     --
**备注:
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
    UART_InitStruct.UART_Mode = UART_INT_MODE; //采用CPU中断方式
    UART_InitStruct.UART_Parity = UART_PARITY_NONE;
    UART_InitStruct.UART_TimeoutCounter = 0x40;
    UART_Init(SCI2, &UART_InitStruct);
}
//#endif /*DEBUG_LOG_EN*/
/*************************全局函数****************************/
//uint16_t temp;
//#pragma pack(1) //这里强制使用1字节对齐的方式
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
//#pragma pack()                     //取消对齐
void Clear_IO_LAUCH(void)
{
    CPM->CPM_PWRCR = (CPM->CPM_PWRCR & ~(1 << 30)) | (1 << 31);
    CPM->CPM_PWRCR &= ~(1 << 31);
}
/***************************************************************************************
**函数名:       APP_DefInit
**功能描述:     主动防御初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_DefInit(void)
{
    if(0 == HAL_Bufang_Clear()) //清除标志位
    {
        st_InputInfo.defSysTickCntEn = 0; //清除标志位
        st_InputInfo.defSysTickCnt = 0;
    }
}
/***************************************************************************************
**函数名:       APP_DefSet
**功能描述:     主动防御设置
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_DefSet(void)
{
    if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance))
    {
        DEBUG_LOG_OUT("APP_DefSet\n");
        st_InputInfo.defSysTickCntEn = 1; //启动计时器
        HAL_BuFang_Time_Init(st_InputInfo.defEn,(uint32_t *)&st_InputInfo.defSysTickCnt);
    }
}

/***************************************************************************************
**函数名:       APP_GetBackBat
**功能描述:     获取后板的电源阻塞方式。
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_GetBackBat(void)
{

    uint32_t timeCnt = 2000; //最长的运行时间是3秒
    DEBUG_LOG_OUT("WakeUpSource HEX %X\n",WakeUpSource);

    if(eWakeUpSourceBack != WakeUpSource) //不是后板唤醒的情况
    {
        if(eStorageStatusError != st_InputInfo.storageStatus) //只有存储正确的时候才询问
        {
            HAL_Comm_SendDealStart(eCommGetBattery, 0); //查询电池电量
        }
    }
    else
    {
        fn_JumpMenu(enum_Menu_WakeUp); //等待唤醒源数据
    }
    while(timeCnt --)
    {
        DELAY_MS(1);
        Menu_Comm_Service();
        if(1 == st_InputInfo.backbatGetFlag) //电量获取完成
        {
            DEBUG_LOG_OUT("Get back bat ok!\n");
            break;
        }
    }
    if(0 == timeCnt) //如果超时
    {
        DEBUG_LOG_OUT("Get back bat Erorr!\n");
        //系统报错，进入休眠
    }

}
/***************************************************************************************
**函数名:       APP_SysWakeUpToMenu
**功能描述:     根据唤醒源进入菜单
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_SysWakeUpToMenu(void)
{
    uint8_t tryTestFlag = 0;
	if(1 == st_InputInfo.lowPowerCannotWork) //系统无法工作
	{
		fn_JumpMenu(enum_Menu_SystemSleep); //进入休眠
		return;
	}
    if ((eStorageStatusError == st_InputInfo.storageStatus) && (st_MenuItem.e_CurrentIndex != enum_Menu_Factory))
    {
        DEBUG_LOG_OUT("APP_SysWakeUpToMenu fn_JumpMenu(enum_Menu_StorageError); \n");
#if 1
        fn_JumpMenu(enum_Menu_StorageError);
#else
        fn_JumpMenu(enum_Menu_WakeUp); //等待唤醒源数据
#endif
    }
    else if (eWakeUpSourceAlarm == WakeUpSource) //优先级比较高
    {
        DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceAlarm\n");
        //		HAL_Comm_SendDealStart(eCommGetBattery, 0); //查询电池电量
        fn_JumpMenu(enum_Menu_WakeUp); //等待唤醒源数据
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
            fn_JumpMenu(enum_Menu_WifiConnect); //第一次上电wifi重新连接
#endif
            break;
        case eWakeUpSourceBack:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceBack \n");
#ifndef BACK_GET_BAT_STATUS
            fn_JumpMenu(enum_Menu_WakeUp); //等待唤醒源数据
#endif
            break;
        case eWakeUpSourceCatEye:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceCatEye \n");
            fn_JumpMenu(enum_Menu_CatEyeConn);
            break;
        case eWakeUpSourceFinger:
            APP_DefSet(); //主动防御
            tryTestFlag = 1;
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceFinger \n");
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourceIr:
            APP_DefSet(); //主动防御
            tryTestFlag = 1;
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceIr \n");
            Menu_EnterDesktopVerifyStart();
            fn_JumpMenu(enum_Menu_Desktop);
            break;
        case eWakeUpSourceActiveDefense:
            DEBUG_LOG_OUT("APP_SysWakeUpToMenu eWakeUpSourceActiveDefense\n");
            st_InputInfo.defSysTickCntEn = 0; //清除标志位
            st_InputInfo.defSysTickCnt = 0;   //清除计数器
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
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
                Menu_Face_PowerOffStart();

                fn_JumpMenu(enum_Menu_TryAlarm); //禁试报警
            }
        }
    }
}
/***************************************************************************************
**函数名:       APP_StorageInit
**功能描述:     系统存储初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_StorageInit(void)
{
#if 0
    HAL_Storage_Init();
#endif
#if 0
    HAL_Storage_ClearSysData();                                   //清空系统数据,并写入初始数据
    st_InputInfo.storageStatus = eStorageStatusSuccess;		 //存储器正常
#endif
#if 1
    if (0 != HAL_Storage_ChkSysData()) //读取并检查
    {
        st_InputInfo.storageStatus = eStorageStatusError; //存储器错误
        storageSave.sysDat.irDef = 0 ;
        st_InputInfo.defEn = storageSave.sysDat.irDef; //主动防御的参数
        DEBUG_LOG_OUT("HAL_Storage_ChkSysData() eStorageStatusError\n");
        //		DelayMS(1);
        //存储器错误
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
        st_InputInfo.storageStatus = eStorageStatusSuccess;		 //存储器正常
        HAL_Storage_ReadHardwareData(&storageSave.hardwareData); //读取硬件信息
        if ((LOCK_BODY_TYPE_LIUFU != storageSave.hardwareData.lockBodyType) &&
                (LOCK_BODY_TYPE_KAIYUANLE != storageSave.hardwareData.lockBodyType))
        {
            storageSave.hardwareData.lockBodyType = LOCK_BODY_TYPE_LIUFU; //默认是六浮锁体
        }
        if ((storageSave.hardwareData.dir == 0xff) ||
                ((storageSave.hardwareData.dir != 0x55) && (storageSave.hardwareData.dir != 0x01))) //选择开门返现
        {
            storageSave.hardwareData.dir = 0x55;
        }
        if ((storageSave.hardwareData.torque[0] > 3) ||
                (storageSave.hardwareData.torque[0] == 0)) //修正开门扭力
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
        HAL_Comm_AesKeySet(storageSave.hardwareData.encryptKey); //设置AES密码
#endif
#if 0
        pCommRootKeyA= storageSave.hardwareData.encryptKey; //AES密码
#endif
#if 0
        pCommMac = storageSave.hardwareData.mac; //MAC指针
#endif
#if 0
        DEBUG_LOG_OUT("mac:\n");
        DEBUG_LOG_OUT_HEX(pCommMac,6);
        DEBUG_LOG_OUT("root:\n");
        DEBUG_LOG_OUT_HEX(pCommRootKeyA,6);
#endif
        HAL_Comm_EncryptionParam(storageSave.hardwareData.mac,
                                 storageSave.hardwareData.encryptKey);
        st_InputInfo.defEn = storageSave.sysDat.irDef; //主动防御的参数
    }
#endif
}
/***************************************************************************************
**函数名:       APP_Finger_Init
**功能描述:     指纹初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Finger_Init(void)
{
    if (0 == HAL_Finger_Init())
    {
        st_InputInfo.modeuleFail.Finger = 0; //指纹有效
    }
    else
    {
        st_InputInfo.modeuleFail.Finger = 1;
    }
}
/***************************************************************************************
**函数名:       APP_GetPowerValue
**功能描述:     获取电压值
**输入参数:     --
**输出参数:     --
**备注:         阻塞方式
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
        HAL_KeyLight_batterySetDir(eColorRed); //红灯
        HAL_VoicePlayDir(eVoiceDirLowVoltageReplaceBattery);
        DELAY_MS(2000);
    }
    else if (st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK)
    {
        HAL_KeyLight_batterySetDir(eColorRed); //红灯
        HAL_VoicePlayDir(eVoiceDirLowVoltageUnableToWork);
        DELAY_MS(2000);
    }
    else
    {
        HAL_KeyLight_batterySetDir(eColorGreen); //绿灯
    }
#endif
#if 1
    if (((st_InputInfo.upBatBattery >= DC_STATUS_LOW) || (st_InputInfo.upBatBattery <= 100)) && ((st_InputInfo.downBatBattery >= DC_STATUS_LOW) || (st_InputInfo.downBatBattery <= 100)))
    {
        DEBUG_LOG_OUT("eColorGreen\n");
        HAL_KeyLight_batterySetDir(eColorGreen); //绿灯
    }
    else
    {
        DEBUG_LOG_OUT("eColorRed\n");
        HAL_KeyLight_batterySetDir(eColorRed); //红灯
    }
    if (st_InputInfo.upBatBattery <= 100)
    {

        DEBUG_LOG_OUT("eVoiceDirPleaseInputUpBat\n");
        HAL_VoicePlayDir(eVoiceDirPleaseInputUpBat); //请插入，上方，电池
        DELAY_MS(2500);
    }
    if (st_InputInfo.downBatBattery <= 100)
    {
        DEBUG_LOG_OUT("eVoiceDirPleaseInputDownBat\n");
        HAL_VoicePlayDir(eVoiceDirPleaseInputDownBat); //请插入，下方，电池
        DELAY_MS(2500);
    }
    if ((st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK) && (st_InputInfo.downBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK))
    {
        if (st_InputInfo.upBatBattery > 100)
        {
            DEBUG_LOG_OUT("eVoiceDirUpBatLowCanotWork\n");
            HAL_VoicePlayDir(eVoiceDirUpBatLowCanotWork); //上方,电池,电压低，无法工作
            DELAY_MS(3000);
        }
        if (st_InputInfo.downBatBattery > 100)
        {
            DEBUG_LOG_OUT("eVoiceDirDownBatLowCanotWork\n");
            HAL_VoicePlayDir(eVoiceDirDownBatLowCanotWork); //下方,电池,电压低，无法工作
            DELAY_MS(3000);
        }
		st_InputInfo.lowPowerCannotWork = 1;
        
		return;
    }
    if ((st_InputInfo.upBatBattery < DC_STATUS_LOW) && (st_InputInfo.upBatBattery > 100)) //&& //上方低
    {
        // if (st_InputInfo.upBatBattery <= DC_STATUS_LOW_SYSTEM_NOT_WORK)
        // {
        // 	M_VBAT2_EN2_OUT_PP();
        // 	M_VBAT2_EN2_FUN_ON;
        // }
        DEBUG_LOG_OUT("eVoiceDirUpBatLow\n");
        HAL_VoicePlayDir(eVoiceDirUpBatLow); //上方,电池,电压低，请充电
        DELAY_MS(3000);
    }
    if ((st_InputInfo.downBatBattery < DC_STATUS_LOW) && (st_InputInfo.downBatBattery > 100)) //下方低
    {
        DEBUG_LOG_OUT("eVoiceDirDownBatLow\n");
        HAL_VoicePlayDir(eVoiceDirDownBatLow); //下方,电池,电压低，请充电
        DELAY_MS(3000);
    }
#endif
}
/***************************************************************************************
**函数名:       APP_Comm_Init
**功能描述:     通讯过程初始化
**输入参数:     --
**输出参数:     --
**备注:         阻塞方式
****************************************************************************************/
static void APP_Comm_Init(void)
{
    if(eWakeUpSourcePOR == WakeUpSource) //上电复位
    {
#if 0
        HAL_Comm_RstBack(); //上电同时复位后板
#else
        HAL_Comm_InitBack();
#endif
    }
    HAL_Comm_Init();	  //前后板子通讯初始化
}
/***************************************************************************************
**函数名:       APP_WakeupRTCTimerGet
**功能描述:     获取RTC
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
int8_t APP_WakeupRTCTimerGet(void)
{
    uint32_t tick;
    HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer);
    if(st_InputInfo.RTCTimer.tm_year < 2020) //时间在2020年之前,时间设置到2020-01-01 00:00:00
    {
        DEBUG_LOG_OUT("reset RTC\n");
        HAL_RTC_SetByTick(1577808000);
        HAL_RTC_TimeGetStm(&st_InputInfo.RTCTimer); //重新读取时间
    }
    return 0;
}

/***************************************************************************************
**函数名:       APP_FaceCatEyeInit
**功能描述:     人脸和猫眼初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_FaceCatEyeInit(void)
{
    HAL_FaceId_ClosePow(); //关闭人脸电源
    if((1 == storageSave.sysDat.ShangTangCatEyeSingleDouble)||
            (MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw) )    //唤醒,处理猫眼
    {
        HAL_Face_WifiPowClose(); //关闭猫眼电源
    }
	HAL_IR_IOInit();
}
/***************************************************************************************
**函数名:       APP_FaceCatEyeInit
**功能描述:     人脸和猫眼初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_VolumeSetInit(void)
{
    DEBUG_LOG_OUT("storageSave.sysDat.voiceSw:%d\n",storageSave.sysDat.voiceSw);
    HAL_Voice_VolumeSet((uint8_t)(storageSave.sysDat.voiceSw));
}
/***************************************************************************************
**函数名:       APP_SysTimerHandler
**功能描述:     系统时钟中断服务程序
**输入参数:     --
**输出参数:     --
**备注:         需要添加到系统滴答时钟的变量都可以添加到这里。
****************************************************************************************/
void APP_SysTimerHandler(void)
{
    sysTimerCtrl.tick++;  //系统时间累加
    if((0 != st_InputInfo.defEn) && (2 != storageSave.sysDat.IrDistance))
    {
        if(1 == st_InputInfo.defSysTickCntEn) //需要主动防御使能才能计数
        {
            st_InputInfo.defSysTickCnt ++;
        }
    }
}
/***************************************************************************************
**函数名:       APP_SysTimerInit
**功能描述:     系统时钟初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_SysTimerInit(void)
{
    HAL_SysTimerInit();			  //系统时钟初始化
    sysTimerCtrl.tick = 0;
    HAL_SysTimerHandlerReg(APP_SysTimerHandler);
}

#if USE_WDT >0
#define WDT_FEED_DOG do{HAL_WDTFeedDog();}while(0)
#else
#define WDT_FEED_DOG
#endif

/***************************************************************************************
**函数名:       APP_Power_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_Init(void)
{
	WDT_FEED_DOG;
    HAL_Power_CPUInit();	  //CPU初始化
	WDT_FEED_DOG;
    HAL_Power_SysWakeupChk(); //查询系统唤醒源
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
    HAL_Power_TrngInit(); //随机数生成器
	WDT_FEED_DOG;
    APP_Comm_Init(); //前后通讯初始化
	WDT_FEED_DOG;
    HAL_Voice_Init();	  //语音初始化
	WDT_FEED_DOG;
    HAL_KeyLightInit();	  //注意，这里这里的电源和FLASH连在一起了;
    DEBUG_LOG_OUT("START!\r\n");
    HAL_Power_ReservedWakeupConfig();		//未使用到的IO初始化
	WDT_FEED_DOG;
    HAL_Power_ReuseInterruptWakeupConfig(); //中断配置
    DEBUG_LOG_OUT("CCM->RTCCFG12=0x%04x\r\n", CCM->RTCCFG12);
	DEBUG_LOG_OUT("V3.11 HX   @@\r\n");
	DEBUG_LOG_OUT("V3.11 HX   @@\r\n");
	WDT_FEED_DOG;
    HAL_RTC_Init();		   //RTC初始化
	WDT_FEED_DOG;
    Function_SysRamInit(); //系统RAM数据
	WDT_FEED_DOG;
#if USE_ID2 >0
    st_InputInfo.iD2Status = eID2StatusSuccess;
	WDT_FEED_DOG;
    id2_client_init();  //ID2初始化
#else
    st_InputInfo.iD2Status = eID2StatusError;
#endif
	WDT_FEED_DOG;
    HAL_Alarm_SetWakeUp(); //报警配置
    e_CPM_wakeupSource_Status rst_source = *(e_CPM_wakeupSource_Status *)CPM_WAKEUPSOURCE_REG;
    DEBUG_LOG_OUT("rst_source1:%d\n", rst_source);
//	DEBUG_LOG_OUT("WakeUpSource:%d\n", WakeUpSource);
    DEBUG_LOG_OUT("System powerup ok!\n");
    cm_backtrace_init("ccm4202s_demo_efm", HARDWARE_VERSION, SOFTWARE_VERSION);
	WDT_FEED_DOG;
    APP_StorageInit(); //初始化存储
	WDT_FEED_DOG;
    APP_FaceCatEyeInit(); //人脸和猫眼初始化
	WDT_FEED_DOG;
    APP_WakeupRTCTimerGet();		  //获取RTC时间
	WDT_FEED_DOG;
    HAL_KeySacnInit();			  //按键初始化
	WDT_FEED_DOG;
    APP_Finger_Init();			  //指纹初始化
	WDT_FEED_DOG;
    APP_VolumeSetInit();		  //音量设置
	WDT_FEED_DOG;
    APP_DefInit();                //主动防御初始化
	WDT_FEED_DOG;
#if 0	//一键布防测试
	storageSave.sysDat.keyDef = 1;
#endif		
#if DEBUG_LOW_POWER > 0
#else
    APP_GetBackBat();             //获取电量信息
#endif
#ifndef BACK_GET_BAT_STATUS
    APP_GetPowerValue();		  //获取电压值,在这里阻塞处理电量信息。
#endif
	WDT_FEED_DOG;
    APP_SysWakeUpToMenu();		  //根据唤醒源进入菜单
	WDT_FEED_DOG;
    APP_SysTimerInit();           //系统滴答时钟初始化
	WDT_FEED_DOG;
#if USE_WDT >0
//	HAL_InitWDT(0x08ff);
#endif
}
/***************************************************************************************
**函数名:       APP_Power_WakeupInit
**功能描述:     系统唤醒初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_WakeupInit(void)
{

}
/***************************************************************************************
**函数名:       APP_Power_WifiShutdown
**功能描述:     wifi关机
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_WifiShutdown(void)
{
    if((1 == storageSave.sysDat.ShangTangCatEyeSingleDouble)||
            (0 != Menu_LowPowerChk())||
            (MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG != storageSave.sysDat.wifiMainSw))     //休眠前,处理猫眼
    {
        HAL_Face_WifiPowClose(); //关闭猫眼电源
    }
}

/***************************************************************************************
**函数名:       APP_Power_IRSleepConfig
**功能描述:     IR身体感应休眠配置
**输入参数:     --
**输出参数:     --
**备注:
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
**函数名:       APP_Power_EnterSleepConfig
**功能描述:     系统进入休眠配置
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_EnterSleepConfig(void)
{
    HAL_Voice_EnterSleepConfig(); //语音
    DEBUG_LOG_OUT("HAL_Voice_EnterSleepConfig();\n");
    HAL_Alarm_SetInterrpt(); //报警
    DEBUG_LOG_OUT("HAL_Alarm_SetInterrpt\n");
    HAL_Finger_EnterSleepConfig(); //指纹
    DEBUG_LOG_OUT("HAL_Finger_EnterSleepConfig\n");
    HAL_KeySacn_EnterSleepConfig(); //按键
    DEBUG_LOG_OUT("HAL_KeySacn_EnterSleepConfig\n");
    HAL_Face_EnterSleepConfig(); //人脸
	APP_Power_IRSleepConfig(); //人体感应配置
    APP_Power_WifiShutdown(); //wifi关闭电源
    DEBUG_LOG_OUT("HAL_Face_EnterSleepConfig\n");
    HAL_KeyLight_EnterSleepConfig(); //灯光
    DEBUG_LOG_OUT("HAL_KeyLight_EnterSleepConfig\n");
    HAL_Power_ReservedSleepConfig(); //未使用的IO
    DEBUG_LOG_OUT("HAL_Power_ReservedSleepConfig\n");
    HAL_COMM_EnterSleepConfig(); //通讯
    DEBUG_LOG_OUT("HAL_COMM_EnterSleepConfig\n");
    HAL_Storage_EnterSleepConfig(); //存储
    DEBUG_LOG_OUT("HAL_Storage_EnterSleepConfig\n");
    HAL_Adc_EnterSleepConfig(); //ADC
    DEBUG_LOG_OUT("HAL_Adc_EnterSleepConfig\n");
}
/***************************************************************************************
**函数名:       APP_Power_EnterSleep
**功能描述:     系统进入休眠
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void APP_Power_EnterSleep(void)
{
    HAL_Power_EnterSleep();
}
#endif /*APP_POWER_MODULE_EN*/
/************************************Code End******************************************/
