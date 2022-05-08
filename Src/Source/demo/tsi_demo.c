// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : tsi_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "tsi_app.h"
#include "pci_demo.h"
#include "cache_drv.h"
#include "libTSI.h"


/**
  * @brief  ��������ɨ�� ��ȡ����ֵ
  * @note   
  *      
  * @return ���ذ���ֵ���ް�����������0
  */
uint8_t Touch_GetKeyValue(void)
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
static uint8_t lastKey = 0xff;
static uint16_t longPressCnt = 1;
#define FFPRINTF  printf   //������ӡ
/**
  * @brief  ��ȡ��ť����
  * @note   �̰� keyValue = CHx 
  * 		���� keyValue = CHx + 0x10
  * 		���� keyValue = CHx + 0x20
  * 		�ް��� keyValue = 0xFF
  *         
  * @return �ж�������SUCCESS  �޶�������ERROR
  */
uint8_t TouchTask_ReadAction(void)
{
	uint8_t keyValue = 0xff;
	uint8_t keyValue2 = 0xff;

	keyValue = Touch_GetKeyValue();
	if (keyValue != 0xff) //������
	{
		if (keyValue != lastKey) //��һ�ζ����������̰�
		{
			longPressCnt = 1;
			if (lastKey != 0xff)
			{	

				return keyValue2; //����һ�εİ���û���ɿ���������һ����ʱ�������Ͷ̰���Ϣ
			}
			FFPRINTF("�̰� %d\n", keyValue);
			keyValue2 = keyValue;
		}
		else if (longPressCnt) //��ȡ��ͬ���İ���ֵ������
		{
			DelayMS(1);
			if (++longPressCnt >= 3000) //������ʱ��Լ3s
			{
				FFPRINTF("longPressCnt %d\n", longPressCnt);
				longPressCnt = 0;
				FFPRINTF("���� %d\n", keyValue);
				keyValue2 = keyValue + 0x10;
			}
		}
	}
	else //������
	{
		longPressCnt = 1; //�����������
		if (lastKey != 0xff)
		{
			keyValue2 = keyValue + 0x20;
			FFPRINTF("���� %d\n", lastKey);
		}
	}
	lastKey = keyValue;
	return keyValue2;
}

void TSI_Demo(void)
{
	UINT32 i;
	  e_CPM_wakeupSource_Status rst_source = *(e_CPM_wakeupSource_Status *)CPM_WAKEUPSOURCE_REG;
    printf("\r\nReset source is:%d\r\n", rst_source);

    if ((rst_source == WAKEUP_SOURCE_POR) || (rst_source == WAKEUP_SOURCE_NONE))
	{
		Sys_Init();
		PCI_Init(EXTERNAL_CLK_SEL);
		PCI->PCI_CR &= ~(0xffu << 24); // NVSRAM don`t Clear
		TC_selectClockSource(1);

		// EPORT_Init(EPORT_PIN5, FALLING_EDGE_INT);
	}
	else
	{
		// ����EFLASH����ģ��
		DCACHE_Init(cacheOff, cacheThrough, cacheOff, cacheOff);
		ICACHE_Init(cacheOff, cacheThrough, cacheOff, cacheOff);
		PCI_InitWakeup(); // PCI reinit.
	}
 
	printf("\r\nReset source is:%d\r\n", rst_source);

	app_tsi_init(&htsi);
//	for(i=0;i<10000;i++)
	while(1)
{
	TouchTask_ReadAction();	

}
	printf("Enter POFF1.5...\r\n");
//	app_tsi_sleep(&htsi);
	// PCI_AsyncTC_Init(2000);
    printf_tsiReg();
	CPM_PowerOff_1p5();
}

