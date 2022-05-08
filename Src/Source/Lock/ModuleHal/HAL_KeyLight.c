/***************************************************************************************
**�ļ���:     HAL_KeyLight.c
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
#include "HAL_KeyLight.h"
#include "DRV_MBI5024.h"
#if HAL_KEY_LIGHT_MODULE_EN
#warning HAL_KEY_LIGHTģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_KEY_LIGHT_RUN_TIME_MAX (10) //����ʱ��
#define HAL_KEY_LIGHT_DATA_LEN (16)     //���ݳ���
#define HAL_KEY_LIGHT_0 (0)             //����
#define HAL_KEY_LIGHT_1 (1)             //����
#define HAL_KEY_LIGHT_2 (2)             //����
#define HAL_KEY_LIGHT_3 (3)             //����
#define HAL_KEY_LIGHT_4 (4)             //����
#define HAL_KEY_LIGHT_5 (5)             //����
#define HAL_KEY_LIGHT_6 (6)             //����
#define HAL_KEY_LIGHT_7 (7)             //����
#define HAL_KEY_LIGHT_8 (8)             //����
#define HAL_KEY_LIGHT_9 (9)             //����
#define HAL_KEY_LIGHT_ENTER (11)        //����
#define HAL_KEY_LIGHT_MENU (10)         //����
//#define HAL_KEY_LIGHT_0                             (12)        //����
/*************************.C�����ṹ����***********************/
typedef enum
{
    ekeyLightStatusNone,        //����
    ekeyLightStatusPowerUp,     //�ϵ�
    ekeyLightStatusWaitPowerOk, //�ȴ���Դ��ʼ�����
    ekeyLightStatusStandby,     //����
    ekeyLightStatusWorking,     //������
} TYPEe_keyLightStatus;

typedef struct
{
    TYPEe_keyLightStatus keyLightStatus;
    INT16U runTimeCnt;                      //����ʱ��
    INT16U dataTab[HAL_KEY_LIGHT_DATA_LEN]; //�����ʾ�ĳ���
    INT16U battery;                         //�����ʾ
    INT16U sysStatus;                       //ϵͳ״ָ̬ʾ��
    INT16U flickerCnt;                      //����������
    INT16U flickerTimeCnt;                  //����ʱ�������
    INT16U flickerTime;                     //����ʱ��
    INT16U repeatCnt;                       //����ʱ��
    INT16U flickerCntTmp;                   //��������������
} TYPEs_keyLightCtrl;
/*************************˽��ȫ�ֱ���*************************/
static TYPEs_keyLightCtrl keyLightCtrl =
    {
        ekeyLightStatusNone, //    TYPEe_keyLightStatus keyLightStatus;
        0,                   //    UINT16 runTimeCnt;                              //����ʱ��
        {
            0,
        }, //    UINT8 dataTab[HAL_KEY_LIGHT_DATA_LEN];          //�����ʾ�ĳ���
        0, //    UINT16 flickerCnt;                              //����������
        0, //    UINT16 flickerTimeCnt;                          //����ʱ�������
        0, //    UINT16 flickerTime;                             //����ʱ��
        0, // INT16U RepeatCnt;                               //����ʱ��
        0, // INT16U flickerCntTmp;                           //��������������
};
static const INT16U batteryLocation = 0x3000;   //�����ʾλ��
static const INT16U sysStatusLocation = 0xC000; //ϵͳ״ָ̬ʾ����ʾλ��
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_KeyLightInit
**��������:     ϵͳ���е�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
INT8S HAL_KeyLightInit(void)
{
    DRV_MBI5024_Init();
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLight_sysStatusSet
**��������:     ϵͳ״̬�ƹ�����
**�������:     --
**�������:     0
INT8S
0 ����
-1 æµ
**��ע:          
****************************************************************************************/
INT8S HAL_KeyLight_sysStatusSet(TYPEe_Color sysStatus)
{
#if 1
    INT16U datTmp = 0;
    INT16U sysStatusTmp = 0;
    INT16U len = 0;
    switch (sysStatus)
    {
    case eColorGreen:
        sysStatusTmp = HAL_SYS_GREEN;
        HAL_KeyLightStatusRun(0);
        break;
    case eColorRed:
        sysStatusTmp = HAL_SYS_RED;
        HAL_KeyLightStatusRun(0);
        break;
    case eColorBlue:
        HAL_KeyLightStatusRun(1);
        break;
    default:
        HAL_KeyLightStatusRun(0);
        break;
    }
    if (0 != keyLightCtrl.flickerCnt)
    {
        len = keyLightCtrl.flickerCnt - 1;
    }
    else
    {
        keyLightCtrl.flickerCnt = 1; //����û�������ڷ��ͣ���������һ������
    }
    keyLightCtrl.sysStatus = sysStatusTmp & sysStatusLocation;                            //��������
    datTmp = (keyLightCtrl.dataTab[len % HAL_KEY_LIGHT_DATA_LEN] & (~sysStatusLocation)); //���ԭ����ֵ
    datTmp |= keyLightCtrl.sysStatus;
    keyLightCtrl.dataTab[len] = datTmp;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking; //��������
#endif
#if 0
    DRV_MBI5024_SysStatusSet(sysStatus);
#endif   
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLight_batterySet
**��������:     ��صƹ�����
**�������:     --
**�������:     
INT8S
0 ����
-1 æµ
**��ע:          
****************************************************************************************/
INT8S HAL_KeyLight_batterySet(TYPEe_Color battery)
{
    INT16U datTmp = 0;
    INT16U bat = 0;
    INT16U len = 0;
    switch (battery)
    {
    case eColorGreen:
        bat = HAL_BAT_GREEN;
        break;
    case eColorRed:
        bat = HAL_BAT_RED;
        break;
    default:
        break;
    }
    if (0 != keyLightCtrl.flickerCnt)
    {
        len = keyLightCtrl.flickerCnt - 1;
    }
    else
    {
        keyLightCtrl.flickerCnt = 1; //����û�������ڷ��ͣ���������һ������
    }
    keyLightCtrl.battery = bat & batteryLocation;                                                           //��������
    datTmp = (keyLightCtrl.dataTab[keyLightCtrl.flickerCnt % HAL_KEY_LIGHT_DATA_LEN] & (~batteryLocation)); //���ԭ����ֵ
    datTmp |= keyLightCtrl.battery;                                                                         //��������
    keyLightCtrl.dataTab[len] = datTmp;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking; //��������
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLight_batterySetDir
**��������:     ��صƹ�����ֱ�����÷�ʽ
**�������:     --
**�������:     
INT8S
0 ����
-1 æµ
**��ע:          
****************************************************************************************/
INT8S HAL_KeyLight_batterySetDir(TYPEe_Color battery)
{
    INT16U datTmp = 0;
    INT16U bat = 0;
    INT16U len = 0;
    switch (battery)
    {
    case eColorGreen:
        bat = HAL_BAT_GREEN;
        break;
    case eColorRed:
        bat = HAL_BAT_RED;
        break;
    default:
        break;
    }
    if (0 != keyLightCtrl.flickerCnt)
    {
        len = keyLightCtrl.flickerCnt - 1;
    }
    else
    {
        keyLightCtrl.flickerCnt = 1; //����û�������ڷ��ͣ���������һ������
    }
    keyLightCtrl.battery = bat & batteryLocation;    //��������
	datTmp = keyLightCtrl.battery;
	DRV_MBI5024_Out(datTmp, 1);             //ע�����ﲻҪԽ�����
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLight_ChkBusy
**��������:     ���ƹ�ʱ����æµ״̬
**�������:     --
**�������:     
INT8S
0 ����
-1 æµ
**��ע:          
****************************************************************************************/
INT8S HAL_KeyLight_ChkBusy(void)
{
    if (ekeyLightStatusStandby == keyLightCtrl.keyLightStatus)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
/***************************************************************************************
**������:       HAL_KeyLightDeal
**��������:     �������⴦��
**�������:     --
**�������:     --
**��ע:         �ƹ����ݴ�β���ȳ� 
****************************************************************************************/
void HAL_KeyLightDeal(void)
{
    //    keyLightCtrl.runTimeCnt++;
    //    if(keyLightCtrl.runTimeCnt < HAL_KEY_LIGHT_RUN_TIME_MAX)
    //    {
    //        return;
    //    }
    //	keyLightCtrl.runTimeCnt = 0;
    INT16U datTmp = 0;
    switch (keyLightCtrl.keyLightStatus)
    {
    case ekeyLightStatusNone:
        break;
    case ekeyLightStatusPowerUp:
        break;
    case ekeyLightStatusWaitPowerOk:
        break;
    case ekeyLightStatusStandby:
        break;
    case ekeyLightStatusWorking:
        if (0 == keyLightCtrl.flickerCnt)
        {
            if (0 == keyLightCtrl.repeatCnt)
            {
                keyLightCtrl.keyLightStatus = ekeyLightStatusStandby; //ֹͣ
            }
            keyLightCtrl.repeatCnt--;                             //�ظ�������1
            keyLightCtrl.flickerCnt = keyLightCtrl.flickerCntTmp; //�Զ��������
        }
        else
        {
            if (0 != keyLightCtrl.flickerTimeCnt)
            {
                keyLightCtrl.flickerTimeCnt--;
            }
            else
            {
                keyLightCtrl.flickerCnt--;
                keyLightCtrl.flickerTimeCnt = keyLightCtrl.flickerTime; //�����ʱ��
                datTmp = keyLightCtrl.dataTab[keyLightCtrl.flickerCnt % HAL_KEY_LIGHT_DATA_LEN];
                datTmp = datTmp & (~batteryLocation);   //���ԭ����ֵ
                datTmp |= keyLightCtrl.battery;         //��������
                datTmp = datTmp & (~sysStatusLocation); //���ԭ����ֵ
                datTmp |= keyLightCtrl.sysStatus;       //��������
                DRV_MBI5024_Out(datTmp, 1);             //ע�����ﲻҪԽ�����
            }
        }
        break;
    default:
        break;
    }
}

/***************************************************************************************
**������:       HAL_KeyLightPressKey
**��������:     �����ƹ���Ƴ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_KeyLightPressKey(INT8U key,INT16U dataTmp)
{
    INT8U i = 0;
//    INT16U dataTmp = DRV_MBI5024_LED_0 | DRV_MBI5024_LED_1 | DRV_MBI5024_LED_2 | DRV_MBI5024_LED_3 | DRV_MBI5024_LED_4 |
//                     DRV_MBI5024_LED_5 | DRV_MBI5024_LED_6 | DRV_MBI5024_LED_7 | DRV_MBI5024_LED_8 | DRV_MBI5024_LED_9 |
//                     DRV_MBI5024_LED_E | DRV_MBI5024_LED_M;
    keyLightCtrl.dataTab[i++] = dataTmp;
    switch (key)
    {
    case HAL_KEY_LIGHT_1:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_1;
        break;
    case HAL_KEY_LIGHT_2:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_2;
        break;
    case HAL_KEY_LIGHT_3:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_3;
        break;
    case HAL_KEY_LIGHT_4:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_4;
        break;
    case HAL_KEY_LIGHT_5:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_5;
        break;
    case HAL_KEY_LIGHT_6:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_6;
        break;
    case HAL_KEY_LIGHT_7:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_7;
        break;
    case HAL_KEY_LIGHT_8:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_8;
        break;
    case HAL_KEY_LIGHT_9:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_9;
        break;
    case HAL_KEY_LIGHT_ENTER:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_E;
        break;
    case HAL_KEY_LIGHT_MENU:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_M;
        break;
    case HAL_KEY_LIGHT_0:
        keyLightCtrl.dataTab[i++] = dataTmp - DRV_MBI5024_LED_0;
        break;
    default:
        keyLightCtrl.dataTab[i++] = dataTmp;
        break;
    }
    keyLightCtrl.flickerCnt = i;
    keyLightCtrl.flickerCntTmp = i;
    keyLightCtrl.flickerTimeCnt = 0;
    keyLightCtrl.flickerTime = 20;
    keyLightCtrl.repeatCnt = 0;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking;
}

/***************************************************************************************
**������:       HAL_KeyLightOnce
**��������:     ��ʾһ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_KeyLightOnce(INT16U dat)
{
    INT8U i = 0;
    keyLightCtrl.dataTab[i++] = dat;
    keyLightCtrl.flickerCnt = i;
    keyLightCtrl.flickerCntTmp = i;
    keyLightCtrl.flickerTimeCnt = 0;
    keyLightCtrl.flickerTime = 20;
    keyLightCtrl.repeatCnt = 0;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking;
}
/***************************************************************************************
**������:       HAL_KeyLightDataSet
**��������:     �������ݴ���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
INT8S HAL_KeyLightDataSet(INT16U const *dat, INT16U len, INT16U flickerTime, INT16U repeatCnt)
{
    INT8U i = 0;
    if (NULL == dat)
    {
        return -1; //��������
    }
    if (len > HAL_KEY_LIGHT_DATA_LEN)
    {
        return -1;
    }
    //    memcpy(keyLightCtrl.dataTab, dat, len*2); //����������word
    for (i = 0; i < len; i++)
    {
        keyLightCtrl.dataTab[i] = dat[i];
    }
    keyLightCtrl.flickerCnt = len;
    keyLightCtrl.flickerCntTmp = len;
    keyLightCtrl.flickerTimeCnt = 0;
    keyLightCtrl.flickerTime = flickerTime;
    keyLightCtrl.repeatCnt = repeatCnt;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking;
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLightStatusRun
**��������:     ϵͳ���е�
**�������:     
INT8U cmd 0 ,�ر�;1,����
**�������:     --
**��ע:         
****************************************************************************************/
INT8S HAL_KeyLightStatusRun(INT8U cmd)
{
    if (0 == cmd)
    {
        DRV_MBI5024_LEDClose();
    }
    else
    {
        DRV_MBI5024_LEDOpen();
    }
    return 0;
}
/***************************************************************************************
**������:       HAL_KeyLightUniteData
**��������:     �����ƹ���������
**�������:     
INT8U *dat, ����
INT8U len ���ݳ���
**�������:     --
INT8S
-1; //��������
0; //�ɹ�
**��ע:         
****************************************************************************************/
INT8S HAL_KeyLightUniteData(INT8U *dat, INT8U len)
{
    INT8U i = 0;
    INT8U j = 0;
    INT16U dataTmp = 0;

    if (NULL == dat)
    {
        return -1; //��������
    }
    if (0 == len)
    {
        return -1;
    }
    for (j = 0; j < len; j++)
    {
        switch (*dat)
        {
        case HAL_KEY_LIGHT_0:
            dataTmp |= DRV_MBI5024_LED_0;
            break;
        case HAL_KEY_LIGHT_1:
            dataTmp |= DRV_MBI5024_LED_1;
            break;
        case HAL_KEY_LIGHT_2:
            dataTmp |= DRV_MBI5024_LED_2;
            break;
        case HAL_KEY_LIGHT_3:
            dataTmp |= DRV_MBI5024_LED_3;
            break;
        case HAL_KEY_LIGHT_4:
            dataTmp |= DRV_MBI5024_LED_4;
            break;
        case HAL_KEY_LIGHT_5:
            dataTmp |= DRV_MBI5024_LED_5;
            break;
        case HAL_KEY_LIGHT_6:
            dataTmp |= DRV_MBI5024_LED_6;
            break;
        case HAL_KEY_LIGHT_7:
            dataTmp |= DRV_MBI5024_LED_7;
            break;
        case HAL_KEY_LIGHT_8:
            dataTmp |= DRV_MBI5024_LED_8;
            break;
        case HAL_KEY_LIGHT_9:
            dataTmp |= DRV_MBI5024_LED_9;
            break;
        case HAL_KEY_LIGHT_ENTER:
            dataTmp |= DRV_MBI5024_LED_E;
            break;
        case HAL_KEY_LIGHT_MENU:
            dataTmp |= DRV_MBI5024_LED_M;
            break;
        default:
            break;
        }
        dat++;
    }
    keyLightCtrl.dataTab[i++] = dataTmp;
    keyLightCtrl.flickerCnt = i;
    keyLightCtrl.flickerCntTmp = i;
    keyLightCtrl.flickerTimeCnt = 0;
    keyLightCtrl.flickerTime = 20;
    keyLightCtrl.repeatCnt = 0;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking;
    return 0;
}

/***************************************************************************************
**������:       HAL_MBI5024_EnterSleepConfig
**��������:     ����ź�
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_KeyLight_EnterSleepConfig(void)
{
    DRV_MBI5024_EnterSleepConfig();
}
/***************************************************************************************
**������:       HAL_KeyLight_DirDispaly
**��������:     ֱ���������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_KeyLight_DirDispaly(uint16_t data)
{
	DRV_MBI5024_Out(data, 1);
}
#endif /*HAL_KEY_LIGHT_MODULE_EN*/
/************************************Code End******************************************/
