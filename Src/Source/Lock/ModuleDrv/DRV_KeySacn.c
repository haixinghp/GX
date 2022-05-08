/***************************************************************************************
**文件名:     DRV_KeySacn.c
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
#include "DRV_KeySacn.h"
/************以下头文件为国芯提供************/
#include "tsi_app.h"
#include "pci_demo.h"
#include "cache_drv.h"
#include "libTSI.h"

#if DRV_KEY_SACN_MODULE_EN
#warning DRV_KEY_SACN模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/
static uint8_t lastKey = 0xff;
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       Touch_GetKeyValue
**功能描述:     读取按键原始值
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static uint8_t Touch_GetKeyValue(void)
{
     app_tsi_handle(&htsi);

    if (htsi.Touch.Current < TSI_MULTI_VALID)
    {
        return htsi.Touch.Current;
    }
    else
    {
        return 0xFF;
    }
    // uint32_t keyValue = 0;
    // uint32_t channel = 0;
    // if (app_tsi_handle(&channel) == 1)
    // {
    //     keyValue |= (channel);
    // }
    // else
    // {
    //     return 0xff;
    // }

    // for (unsigned char i = 0; i < 16; i++)
    // {
    //     if (keyValue & (1 << i))
    //     {
    //         //  printf("key %d\n",i);
    //         return i;
    //     }
    // }
    // return 0xff;
}
/***************************************************************************************
**函数名:       TouchTask_ReadAction
**功能描述:     读取按钮动作
**输入参数:     --
**输出参数:     
短按 keyValue = CHx 
长按 keyValue = CHx + 0x10 预留
松手 keyValue = CHx + 0x20 预留
无按键 keyValue = 0xFF
**备注:         
****************************************************************************************/
static uint8_t TouchTask_ReadAction(void)
{
    uint8_t keyValue = 0xff;
    uint8_t keyValue2 = 0xff;

    keyValue = Touch_GetKeyValue();
    #if 1
    return keyValue;
    #else
    if (keyValue != 0xff) //按下了
    {
        
        if (keyValue != lastKey) //第一次读到按键：短按
        {
            // longPressCnt = 1;
            if (lastKey != 0xff)
            {

                return keyValue2; //当上一次的按键没有松开继续按另一个键时，不发送短按消息
            }
            //            FFPRINTF("短按 %d\n", keyValue);
            //            DEBUG_LOG_OUT("短按 %d\n", keyValue);
            keyValue2 = keyValue;
        }
        
        // else if (longPressCnt) //读取到同样的按键值：长按
        // {
        // 	DelayMS(1);
        // 	if (++longPressCnt >= 3000) //长按计时：约3s
        // 	{
        // 		FFPRINTF("longPressCnt %d\n", longPressCnt);
        // 		longPressCnt = 0;
        // 		FFPRINTF("长按 %d\n", keyValue);
        // 		keyValue2 = keyValue + 0x10;
        // 	}
        // }
    }
    // else //松手了
    // {
    // 	// longPressCnt = 1; //清除长按计数
    // 	if (lastKey != 0xff)
    // 	{
    // 		// keyValue2 = keyValue + 0x20;
    // 		FFPRINTF("松手 %d\n", lastKey);
    // 	}
    // }
    lastKey = keyValue;
    return keyValue2;
    #endif
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_KeySacn_Init
**功能描述:     按键初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_KeySacn_Init(void)
{
    PCI_Init(EXTERNAL_CLK_SEL);
    app_tsi_init(&htsi);
}
/***************************************************************************************
**函数名:       DRV_KeySacn_Init
**功能描述:     按键初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t DRV_KeySacn_GetValue(void)
{
    uint8_t reTmp = DRV_KEY_SACN_GET_NOKEY;
    switch (TouchTask_ReadAction()) //获取按键值
    {
    case DRV_KEY_SACN_ORIGINAL_0:
        reTmp = DRV_KEY_SACN_GET_1 ;
        break;
    case DRV_KEY_SACN_ORIGINAL_1:
        reTmp = DRV_KEY_SACN_GET_BELL;
        break;
    case DRV_KEY_SACN_ORIGINAL_2:
        reTmp = DRV_KEY_SACN_GET_CLOSE;
        break;
    case DRV_KEY_SACN_ORIGINAL_3:
        reTmp = DRV_KEY_SACN_GET_5;
        break;
    case DRV_KEY_SACN_ORIGINAL_4:
        reTmp = DRV_KEY_SACN_GET_4;
        break;
    case DRV_KEY_SACN_ORIGINAL_5:
        reTmp = DRV_KEY_SACN_GET_2;
        break;
    case DRV_KEY_SACN_ORIGINAL_6:
        reTmp = DRV_KEY_SACN_GET_7;
        break;
    case DRV_KEY_SACN_ORIGINAL_7:
        reTmp = DRV_KEY_SACN_GET_3;
        break;
    case DRV_KEY_SACN_ORIGINAL_8:
        reTmp = DRV_KEY_SACN_GET_ENTER;
        break;
    case DRV_KEY_SACN_ORIGINAL_9:
        reTmp = DRV_KEY_SACN_GET_6;
        break;
    case DRV_KEY_SACN_ORIGINAL_10:
        reTmp = DRV_KEY_SACN_GET_0;
        break;
    case DRV_KEY_SACN_ORIGINAL_11:
        reTmp = DRV_KEY_SACN_GET_8;
        break;
    case DRV_KEY_SACN_ORIGINAL_12:
        reTmp = DRV_KEY_SACN_GET_NOKEY;
        break;
    case DRV_KEY_SACN_ORIGINAL_13:
        reTmp = DRV_KEY_SACN_GET_NOKEY;
        break;
    case DRV_KEY_SACN_ORIGINAL_14:
        reTmp = DRV_KEY_SACN_GET_MENU;
        break;
    case DRV_KEY_SACN_ORIGINAL_15:
        reTmp = DRV_KEY_SACN_GET_9;
        break;
    default:
        break;
    }
    return reTmp;
}

/***************************************************************************************
**函数名:       DRV_KeySacn_EnterSleepConfig
**功能描述:     按键进入休眠初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_KeySacn_EnterSleepConfig(void)
{
    app_tsi_sleep(&htsi);
}

#endif /*DRV_KEY_SACN_MODULE_EN*/
       /************************************Code End******************************************/