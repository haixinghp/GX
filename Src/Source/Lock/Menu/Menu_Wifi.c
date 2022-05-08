/***************************************************************************************
**�ļ���:     Menu_Wifi.c
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
#include "menu.h"
#include "HAL_Comm.h"
#include "HAL_FaceId.h"
#if MENU_MODULE_EN
/***************************************************************************************
**������:       Menu_BleConfigWifiData
**��������:     wifi������
**�������: 		--
**�������: 		--
**��ע:         
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
        
        //��wifi�ϵ�
        HAL_Comm_WifiRtSet(0);       //����Ϊ��������
        HAL_Comm_WifiCtInit();       //��ʼ��CT
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
        if (1 == HAL_Comm_WifiCtRead()) //����
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
                                            sizeof(TYPEs_WifiConfig)); //���������
        HAL_Comm_WifiSend((uint8_t *)&msgDataPassThrough,
                           sendLen);//�������ݰ�
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
            //�ر�wifi
        }
        if (0 == HAL_Comm_WifiCtRead()) //���
        {
            wifiSendParm.wifiConnectStatus = eWifiConnectStatusWaitComp;
            wifiSendParm.timerCnt = 100;
            //�ر�wifi
        }
        break;
    case eWifiConnectStatusWaitComp:
        wifiSendParm.timerCnt--;
        if (0 == wifiSendParm.timerCnt)
        {
            DEBUG_LOG_OUT("wifi comp:/n");
            pBleWifiSetParmAck->ack = eBleWifiSetParmAckSuccess;
            HAL_Comm_BleSendDir(eBleWifiSetParm1);
            fn_JumpMenu(enum_Menu_APPMode); //�������
        }
        break;
    default:
        break;
    }
}
#ifdef USE_WIFI_MODLE
/***************************************************************************************
**������:       Menu_WifiDeal
**��������:     
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void Menu_WifiDeal(void)
{
    switch (wifiSendParm.wifiSendStatus)
    {
    	case eWifiSendStatusNone:
    		break;
    	case eWifiSendStatusReady:
            //��wifi�ϵ�
            DEBUG_LOG_OUT("eWifiSendStatusReady \n");
            HAL_Comm_WifiRtSet(1);       //����Ϊ��������
            HAL_Comm_WifiCtInit();       //��ʼ��CT
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
            if (1 == HAL_Comm_WifiCtRead()) //����
            {
                wifiSendParm.wifiSendStatus = eWifiSendStatusWaitSendData;
            }
    		break;
        case eWifiSendStatusWaitSendData:
            DEBUG_LOG_OUT(" eWifiSendStatusWaitSendData\n");
            HAL_Comm_WifiSend((uint8_t *)&msgDataPassThrough,
                                wifiSendParm.sendLen);//�������ݰ�
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
                //�������
            }
    		break;
    	default:
    		break;
    }
}
#endif
/***************************************************************************************
**������:       Menu_WifiService
**��������:     wifi����
**�������: 		--
**�������: 		--
**��ע:         
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