/***************************************************************************************
**文件名:     HAL_KeyLight.c
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
#include "HAL_KeyLight.h"
#include "DRV_MBI5024.h"
#if HAL_KEY_LIGHT_MODULE_EN
#warning HAL_KEY_LIGHT模块使能!
/*******************************宏定义***************************/
#define HAL_KEY_LIGHT_RUN_TIME_MAX (10) //运行时间
#define HAL_KEY_LIGHT_DATA_LEN (16)     //数据长度
#define HAL_KEY_LIGHT_0 (0)             //按键
#define HAL_KEY_LIGHT_1 (1)             //按键
#define HAL_KEY_LIGHT_2 (2)             //按键
#define HAL_KEY_LIGHT_3 (3)             //按键
#define HAL_KEY_LIGHT_4 (4)             //按键
#define HAL_KEY_LIGHT_5 (5)             //按键
#define HAL_KEY_LIGHT_6 (6)             //按键
#define HAL_KEY_LIGHT_7 (7)             //按键
#define HAL_KEY_LIGHT_8 (8)             //按键
#define HAL_KEY_LIGHT_9 (9)             //按键
#define HAL_KEY_LIGHT_ENTER (11)        //按键
#define HAL_KEY_LIGHT_MENU (10)         //按键
//#define HAL_KEY_LIGHT_0                             (12)        //按键
/*************************.C变量结构声明***********************/
typedef enum
{
    ekeyLightStatusNone,        //空闲
    ekeyLightStatusPowerUp,     //上电
    ekeyLightStatusWaitPowerOk, //等待电源初始化完成
    ekeyLightStatusStandby,     //待机
    ekeyLightStatusWorking,     //工作中
} TYPEe_keyLightStatus;

typedef struct
{
    TYPEe_keyLightStatus keyLightStatus;
    INT16U runTimeCnt;                      //运行时间
    INT16U dataTab[HAL_KEY_LIGHT_DATA_LEN]; //存放显示的长度
    INT16U battery;                         //电池显示
    INT16U sysStatus;                       //系统状态指示灯
    INT16U flickerCnt;                      //闪动计数器
    INT16U flickerTimeCnt;                  //闪动时间计数器
    INT16U flickerTime;                     //闪动时间
    INT16U repeatCnt;                       //闪动时间
    INT16U flickerCntTmp;                   //闪动计数器缓存
} TYPEs_keyLightCtrl;
/*************************私有全局变量*************************/
static TYPEs_keyLightCtrl keyLightCtrl =
    {
        ekeyLightStatusNone, //    TYPEe_keyLightStatus keyLightStatus;
        0,                   //    UINT16 runTimeCnt;                              //运行时间
        {
            0,
        }, //    UINT8 dataTab[HAL_KEY_LIGHT_DATA_LEN];          //存放显示的长度
        0, //    UINT16 flickerCnt;                              //闪动计数器
        0, //    UINT16 flickerTimeCnt;                          //闪动时间计数器
        0, //    UINT16 flickerTime;                             //闪动时间
        0, // INT16U RepeatCnt;                               //闪动时间
        0, // INT16U flickerCntTmp;                           //闪动计数器缓存
};
static const INT16U batteryLocation = 0x3000;   //电池显示位置
static const INT16U sysStatusLocation = 0xC000; //系统状态指示灯显示位置
/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_KeyLightInit
**功能描述:     系统运行灯
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
INT8S HAL_KeyLightInit(void)
{
    DRV_MBI5024_Init();
    return 0;
}
/***************************************************************************************
**函数名:       HAL_KeyLight_sysStatusSet
**功能描述:     系统状态灯光设置
**输入参数:     --
**输出参数:     0
INT8S
0 空闲
-1 忙碌
**备注:          
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
        keyLightCtrl.flickerCnt = 1; //假设没有数据在发送，自主产生一个发送
    }
    keyLightCtrl.sysStatus = sysStatusTmp & sysStatusLocation;                            //屏蔽其他
    datTmp = (keyLightCtrl.dataTab[len % HAL_KEY_LIGHT_DATA_LEN] & (~sysStatusLocation)); //清除原来的值
    datTmp |= keyLightCtrl.sysStatus;
    keyLightCtrl.dataTab[len] = datTmp;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking; //发送数据
#endif
#if 0
    DRV_MBI5024_SysStatusSet(sysStatus);
#endif   
    return 0;
}
/***************************************************************************************
**函数名:       HAL_KeyLight_batterySet
**功能描述:     电池灯光设置
**输入参数:     --
**输出参数:     
INT8S
0 空闲
-1 忙碌
**备注:          
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
        keyLightCtrl.flickerCnt = 1; //假设没有数据在发送，自主产生一个发送
    }
    keyLightCtrl.battery = bat & batteryLocation;                                                           //屏蔽其他
    datTmp = (keyLightCtrl.dataTab[keyLightCtrl.flickerCnt % HAL_KEY_LIGHT_DATA_LEN] & (~batteryLocation)); //清除原来的值
    datTmp |= keyLightCtrl.battery;                                                                         //填入数据
    keyLightCtrl.dataTab[len] = datTmp;
    keyLightCtrl.keyLightStatus = ekeyLightStatusWorking; //发送数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_KeyLight_batterySetDir
**功能描述:     电池灯光设置直接设置方式
**输入参数:     --
**输出参数:     
INT8S
0 空闲
-1 忙碌
**备注:          
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
        keyLightCtrl.flickerCnt = 1; //假设没有数据在发送，自主产生一个发送
    }
    keyLightCtrl.battery = bat & batteryLocation;    //屏蔽其他
	datTmp = keyLightCtrl.battery;
	DRV_MBI5024_Out(datTmp, 1);             //注意这里不要越界访问
    return 0;
}
/***************************************************************************************
**函数名:       HAL_KeyLight_ChkBusy
**功能描述:     检查灯光时候处于忙碌状态
**输入参数:     --
**输出参数:     
INT8S
0 空闲
-1 忙碌
**备注:          
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
**函数名:       HAL_KeyLightDeal
**功能描述:     按键背光处理
**输入参数:     --
**输出参数:     --
**备注:         灯光数据从尾部先出 
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
                keyLightCtrl.keyLightStatus = ekeyLightStatusStandby; //停止
            }
            keyLightCtrl.repeatCnt--;                             //重复次数减1
            keyLightCtrl.flickerCnt = keyLightCtrl.flickerCntTmp; //自动回填次数
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
                keyLightCtrl.flickerTimeCnt = keyLightCtrl.flickerTime; //回填工作时间
                datTmp = keyLightCtrl.dataTab[keyLightCtrl.flickerCnt % HAL_KEY_LIGHT_DATA_LEN];
                datTmp = datTmp & (~batteryLocation);   //清除原来的值
                datTmp |= keyLightCtrl.battery;         //填入数据
                datTmp = datTmp & (~sysStatusLocation); //清除原来的值
                datTmp |= keyLightCtrl.sysStatus;       //填入数据
                DRV_MBI5024_Out(datTmp, 1);             //注意这里不要越界访问
            }
        }
        break;
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       HAL_KeyLightPressKey
**功能描述:     按键灯光控制程序
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       HAL_KeyLightOnce
**功能描述:     显示一次
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       HAL_KeyLightDataSet
**功能描述:     背光数据传输
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
INT8S HAL_KeyLightDataSet(INT16U const *dat, INT16U len, INT16U flickerTime, INT16U repeatCnt)
{
    INT8U i = 0;
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    if (len > HAL_KEY_LIGHT_DATA_LEN)
    {
        return -1;
    }
    //    memcpy(keyLightCtrl.dataTab, dat, len*2); //这里拷贝的是word
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
**函数名:       HAL_KeyLightStatusRun
**功能描述:     系统运行灯
**输入参数:     
INT8U cmd 0 ,关闭;1,开启
**输出参数:     --
**备注:         
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
**函数名:       HAL_KeyLightUniteData
**功能描述:     按键灯光联合数据
**输入参数:     
INT8U *dat, 数据
INT8U len 数据长度
**输出参数:     --
INT8S
-1; //参数错误
0; //成功
**备注:         
****************************************************************************************/
INT8S HAL_KeyLightUniteData(INT8U *dat, INT8U len)
{
    INT8U i = 0;
    INT8U j = 0;
    INT16U dataTmp = 0;

    if (NULL == dat)
    {
        return -1; //参数错误
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
**函数名:       HAL_MBI5024_EnterSleepConfig
**功能描述:     输出信号
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_KeyLight_EnterSleepConfig(void)
{
    DRV_MBI5024_EnterSleepConfig();
}
/***************************************************************************************
**函数名:       HAL_KeyLight_DirDispaly
**功能描述:     直接输出数据
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_KeyLight_DirDispaly(uint16_t data)
{
	DRV_MBI5024_Out(data, 1);
}
#endif /*HAL_KEY_LIGHT_MODULE_EN*/
/************************************Code End******************************************/
