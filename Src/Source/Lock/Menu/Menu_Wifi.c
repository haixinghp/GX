/***************************************************************************************
**文件名:     Menu_Wifi.c
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
#include "menu.h"
#include "HAL_Comm.h"
#include "HAL_FaceId.h"
#if MENU_MODULE_EN
/***************************************************************************************
**函数名:       Menu_BleConfigWifiData
**功能描述:     wifi配网络
**输入参数: 		--
**输出参数: 		--
**备注:         
****************************************************************************************/
static void Menu_BleConfigWifiData(void)
{
    uint16_t sendLen=0;
    switch (wifiSendParm.wifiConnectStatus)
    {
    case eWifiConnectStatusNone:
        break;
    case eWifiConnectStatusReady:
        DEBUG_LOG_OUT("eWifiConnectStatusReady\n");
        printf("eWifiConnectStatusReady\n");
        
        //给wifi上电
        HAL_Comm_WifiRtSet(0);       //设置为参数配置
        HAL_Comm_WifiCtInit();       //初始化CT
        wifiSendParm.timerCnt = 500; //500ms
        wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitSwMode;
        break;
    case eWifiConnectStatusWaitSwMode:
        wifiSendParm.timerCnt--;
        if (0 == wifiSendParm.timerCnt)
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitIdle;
            wifiSendParm.timerCnt = 10000; //10s
        }
        break;
    case eWifiConnectStatusWaitIdle:
        wifiSendParm.timerCnt--;
        if (0 == wifiSendParm.timerCnt)
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitSendData;
        }
        if (1 == HAL_Comm_WifiCtRead()) //空闲
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitSendData;
        }
        break;
    case eWifiConnectStatusWaitSendData:
        #if 0
        DEBUG_LOG_OUT("msgConfigWifiData SSID:");
        DEBUG_LOG_OUT_HEX(msgConfigWifiData.ssid, sizeof(msgConfigWifiData.ssid));
        DEBUG_LOG_OUT("msgConfigWifiData wifiKey:");
        DEBUG_LOG_OUT_HEX(msgConfigWifiData.psk, sizeof(msgConfigWifiData.psk));
        #endif
        sendLen = Comm_WifiCmdSendWithLen(  eWifiCmdSetRouter,
                                            eWifiProtocolTypeCmdNotAck,
                                            sizeof(TYPEs_WifiConfig)); //打包好数据
        HAL_Comm_WifiSend((uint8_t *)&msgDataPassThrough,
                           sendLen);//发送数据包
        #if 0
        DEBUG_LOG_OUT("wifi send:/n");
        DEBUG_LOG_OUT_HEX((uint8_t *)&msgDataPassThrough,sendLen);
        #endif
        wifiSendParm.timerCnt = 20000; //20s
        wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitConn;
        break;
    case eWifiConnectStatusWaitConn:
        wifiSendParm.timerCnt--;
        if (0 == wifiSendParm.timerCnt)
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitComp;
            wifiSendParm.timerCnt = 100;
            //关闭wifi
        }
        if (0 == HAL_Comm_WifiCtRead()) //完成
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitComp;
            wifiSendParm.timerCnt = 100;
            //关闭wifi
        }
        break;
    case eWifiConnectStatusWaitComp:
        wifiSendParm.timerCnt--;
        if (0 == wifiSendParm.timerCnt)
        {
            DEBUG_LOG_OUT("wifi comp:/n");
            pBleWifiSetParmAck->ack = eBleWifiSetParmAckSuccess;
            HAL_Comm_BleSendDir(eBleWifiSetParm1);
            fn_JumpMenu(enum_Menu_APPMode); //配网完成
        }
        break;
    default:
        break;
    }
}
#ifdef USE_WIFI_MODLE
/***************************************************************************************
**函数名:       Menu_WifiDeal
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_WifiDeal(void)
{
    switch (wifiSendParm.wifiSendStatus)
    {
    	case eWifiSendStatusNone:
    		break;
    	case eWifiSendStatusReady:
            //给wifi上电
            DEBUG_LOG_OUT("eWifiSendStatusReady \n");
            HAL_Comm_WifiRtSet(1);       //设置为参数配置
            HAL_Comm_WifiCtInit();       //初始化CT
            wifiSendParm.timerCnt = 500; //500ms
            wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSwMode;
    		break;
        case eWifiSendStatusWaitSwMode:
            wifiSendParm.timerCnt--;
            if (0 == wifiSendParm.timerCnt)
            {
                DEBUG_LOG_OUT(" wifiSendParm.wifiSendStatus = eWifiSendStatusWaitIdle; \n");
                wifiSendParm.wifiSendStatus = eWifiSendStatusWaitIdle;
                wifiSendParm.timerCnt = 10000; //10s
            }
    		break;
    	case eWifiSendStatusWaitIdle:
            wifiSendParm.timerCnt--;
            if (0 == wifiSendParm.timerCnt)
            {
                wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSendData;
            }
            if (1 == HAL_Comm_WifiCtRead()) //空闲
            {
                wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSendData;
            }
    		break;
        case eWifiSendStatusWaitSendData:
            DEBUG_LOG_OUT(" eWifiSendStatusWaitSendData\n");
            HAL_Comm_WifiSend((uint8_t *)&msgDataPassThrough,
                                wifiSendParm.sendLen);//发送数据包
            wifiSendParm.timerCnt = 20; 
            wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSendComp;
    		break;
    	case eWifiSendStatusWaitSendComp:
            wifiSendParm.timerCnt--;
            if (0 == wifiSendParm.timerCnt)
            {
                wifiSendParm.wifiSendStatus = eWifiSendStatusWaitComp;
                wifiSendParm.timerCnt = 20000;
            }
    		break;
        case eWifiSendStatusWaitComp:
            wifiSendParm.timerCnt--;
            if (0 == wifiSendParm.timerCnt)
            {
                wifiSendParm.wifiSendStatus = eWifiSendStatusNone;
                //发送完成
            }
    		break;
    	default:
    		break;
    }
}
#endif
/***************************************************************************************
**函数名:       Menu_WifiService
**功能描述:     wifi服务
**输入参数: 		--
**输出参数: 		--
**备注:         
****************************************************************************************/
void Menu_WifiService(void)
{
    #if USE_WIFI_MODLE
    switch (st_MenuItem.e_CurrentIndex)
    {
        
    case enum_Menu_BleConfigWifiData:
        
        Menu_BleConfigWifiData();
        
        break;
    default:
        break;
    }
    Menu_WifiDeal();
    #endif
}
#endif /*Menu_FACE_MODULE_EN*/
       /************************************Code End******************************************/