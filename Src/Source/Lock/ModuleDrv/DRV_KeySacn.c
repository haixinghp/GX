/***************************************************************************************
**�ļ���:     DRV_KeySacn.c
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
#include "DRV_KeySacn.h"
/************����ͷ�ļ�Ϊ��о�ṩ************/
#include "tsi_app.h"
#include "pci_demo.h"
#include "cache_drv.h"
#include "libTSI.h"

#if DRV_KEY_SACN_MODULE_EN
#warning DRV_KEY_SACNģ��ʹ��!
/*******************************�궨��***************************/

/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/
static uint8_t lastKey = 0xff;
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       Touch_GetKeyValue
**��������:     ��ȡ����ԭʼֵ
**�������:     --
**�������:     --
**��ע:         
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
**������:       TouchTask_ReadAction
**��������:     ��ȡ��ť����
**�������:     --
**�������:     
�̰� keyValue = CHx 
���� keyValue = CHx + 0x10 Ԥ��
���� keyValue = CHx + 0x20 Ԥ��
�ް��� keyValue = 0xFF
**��ע:         
****************************************************************************************/
static uint8_t TouchTask_ReadAction(void)
{
    uint8_t keyValue = 0xff;
    uint8_t keyValue2 = 0xff;

    keyValue = Touch_GetKeyValue();
    #if 1
    return keyValue;
    #else
    if (keyValue != 0xff) //������
    {
        
        if (keyValue != lastKey) //��һ�ζ����������̰�
        {
            // longPressCnt = 1;
            if (lastKey != 0xff)
            {

                return keyValue2; //����һ�εİ���û���ɿ���������һ����ʱ�������Ͷ̰���Ϣ
            }
            //            FFPRINTF("�̰� %d\n", keyValue);
            //            DEBUG_LOG_OUT("�̰� %d\n", keyValue);
            keyValue2 = keyValue;
        }
        
        // else if (longPressCnt) //��ȡ��ͬ���İ���ֵ������
        // {
        // 	DelayMS(1);
        // 	if (++longPressCnt >= 3000) //������ʱ��Լ3s
        // 	{
        // 		FFPRINTF("longPressCnt %d\n", longPressCnt);
        // 		longPressCnt = 0;
        // 		FFPRINTF("���� %d\n", keyValue);
        // 		keyValue2 = keyValue + 0x10;
        // 	}
        // }
    }
    // else //������
    // {
    // 	// longPressCnt = 1; //�����������
    // 	if (lastKey != 0xff)
    // 	{
    // 		// keyValue2 = keyValue + 0x20;
    // 		FFPRINTF("���� %d\n", lastKey);
    // 	}
    // }
    lastKey = keyValue;
    return keyValue2;
    #endif
}
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_KeySacn_Init
**��������:     ������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_KeySacn_Init(void)
{
    PCI_Init(EXTERNAL_CLK_SEL);
    app_tsi_init(&htsi);
}
/***************************************************************************************
**������:       DRV_KeySacn_Init
**��������:     ������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_KeySacn_GetValue(void)
{
    uint8_t reTmp = DRV_KEY_SACN_GET_NOKEY;
    switch (TouchTask_ReadAction()) //��ȡ����ֵ
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
**������:       DRV_KeySacn_EnterSleepConfig
**��������:     �����������߳�ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_KeySacn_EnterSleepConfig(void)
{
    app_tsi_sleep(&htsi);
}

#endif /*DRV_KEY_SACN_MODULE_EN*/
       /************************************Code End******************************************/