/***************************************************************************************
**�ļ���:     DRV_Power.c
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
#include "DRV_Power.h"
#include "sub_c0_demo.h"
#include "cpm_drv.h"
#include "eport_drv.h"
#include "tsi_reg.h"
#include "reset_drv.h"
#include "pci_drv.h"
#include "rtc_drv.h"
#include "ccm_drv.h"
#include "trng_drv.h"
#include "Iomacros.h"
#include "wdt_drv.h"
//#include "nrf_pwr_mgmt.h"
#if DRV_POWER_MODULE_EN
#warning DRV_POWERģ��ʹ��!
/*******************************�궨��***************************/
#define DRV_POWER_RAM_HOLD_ADDR (0x08100C00)
//#define DRV_POWER_RAM_HOLD_ADDR (0x08100300)
//#define DRV_POWER_IN_PIN_HANDLER_NUM (10)
//#define DRV_POWER_NULL_PIN (0XFF)
//#define DRV_POWER_KEY_IN_PIN_HANDLER_INDEX (0)
/*************************.C�����ṹ����***********************/
//typedef struct
//{
//	nrf_drv_gpiote_pin_t pin;
//	FunInPinHandler_t keyInPinHandler;
//} TYPEs_InPinHandler;
/*************************˽��ȫ�ֱ���*************************/

//static TYPEs_InPinHandler inPinHandler[DRV_POWER_IN_PIN_HANDLER_NUM] =
//	{
//		{
//			M_TOUCH_INT_GPIO_PIN,
//			NULL,
//		}, //0
//		{DRV_POWER_NULL_PIN, NULL},
//};

/*************************����ȫ�ֱ���*************************/
ReuseIntHandler_t ReuseIntHandler = NULL; //�ж�ָ��
/*************************�ֲ�����*****************************/

/***************************************************************************************
**������:       DRV_Power_ReuseChk
**��������:     �����жϲ�ѯ�ж�Դ 
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static TYPEe_ReuseInt DRV_Power_ReuseChk(void)
{
	uint16_t i;
	M_CCM4202_INT_IN_NOPULL();
	M_FING_IRQ_IN_NOPULL();
	
	//	M_X_IRQ_9555_IN_NOPULL();
	uint32_t overTime = 0;
	uint16_t TimeCnt[eReuseIntMAX] = {
		0,
	};
	while (1)
	{
		overTime++;
		TimeCnt[eReuseIntNull]++;
		if (1 == M_CCM4202_INT_READIN())
		{
			TimeCnt[eReuseIntNull] = 0;
			TimeCnt[eReuseIntBack]++;
		}
		if (1 == M_FING_IRQ_READIN())
		{
			TimeCnt[eReuseIntNull] = 0;
			TimeCnt[eReuseIntFinger]++;
		}
		for (i = 0; i < eReuseIntMAX; i++)
		{
			if (TimeCnt[i] > DRV_POWER_FILTER_TIME) //������ȡ10��
			{
				return i;
			}
		}
		if (overTime >= DRV_POWER_FILTER_OVET_TIME) //��ʱ������ԴΪ��
		{
			return eReuseIntNull;
		}
	}
}
///***************************************************************************************
//**������:       in_pin_hadler
//**��������:     �ص�����
//**�������:     --
//**�������:     --
//**��ע:
//****************************************************************************************/
//static void in_pin_hadler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
//{
//	uint8 i = 0;
//	for (i = 0; i < DRV_POWER_IN_PIN_HANDLER_NUM; i++)
//	{
//		if (inPinHandler[i].pin == pin)
//		{
//			if (NULL != inPinHandler[i].keyInPinHandler) //��ָ֤�벻Ϊ��
//			{
//				inPinHandler[i].keyInPinHandler();
//				return; //ֱ���˳�
//			}
//		}
//	}
//}
/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_Power_TouchExitInPinHandlerReg
**��������:     ע�ᴥ���жϻص����
**�������:     
FunInPinHanler_t fun
Ҫע��ĺ���
**�������:     
int8
0 �ɹ�
-1 ʧ��
**��ע:         
****************************************************************************************/
//int8_t DRV_Power_TouchExitInPinHandlerReg(FunInPinHandler_t fun)
//{
//	if (NULL != fun)
//	{
////		inPinHandler[DRV_POWER_KEY_IN_PIN_HANDLER_INDEX].keyInPinHandler = fun;
//		return 0;
//	}
//	else
//	{
//		return -1;
//	}
//}
/***************************************************************************************
**������:       DRV_Power_GpioInterrputInit
**��������:     ��ʼ��ϵͳ�ж�
**�������:     --
**�������:     --
**��ע:         ע�⣡����NRF52832оƬnrf_drv_gpiote_init();ֻ�ܳ�ʼ��һ�Σ��������
����Ҫ�����ʼ���á�
****************************************************************************************/
void DRV_Power_GpioInterrputInit(void)
{
}

/***************************************************************************************
**������:       DRV_Power_GpioSetWakeup
**��������:     ����GPIO����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_GpioSetWakeup(void)
{
}
/***************************************************************************************
**������:       DRV_Power_GPIOInterrputSetSleep
**��������:     ����GPIO�ж�����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_GpioInterrputSetSleep(void)
{
}
/***************************************************************************************
**������:       DRV_Power_ClosePeripheral
**��������:     �ر�����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ClosePeripheral(void)
{
}
/***************************************************************************************
**������:       DRV_Power_GpioSetSleep
**��������:     ����GPIO����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_GpioSetSleep(void)
{
}
/***************************************************************************************
**������:       DRV_Power_ReuseInterruptConfig
**��������:     �����ж�����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ReuseInterruptConfig(void)
{
    
}
/***************************************************************************************
**������:       DRV_Power_ReuseInterruptWakeupConfig
**��������:     �����ж����û���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ReuseInterruptWakeupConfig(void)
{
	//	M_REUSE_IRQ_IN_NOPULL();
	//	M_REUSE_IRQ_EXIT_LOTOHI_DISABLE();
	//	M_REUSE_IRQ_EXIT_LOTOHI_ENABLE();
}
/***************************************************************************************
**������:       DRV_Power_ReservedSleepConfig
**��������:     δʹ�õ���IO��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ReservedSleepConfig(void)
{
    M_SDA2_IN_NOPULL();
	M_SCL2_IN_NOPULL();
	M_3802_IRQ_OUT_PP();
	M_3802_IRQ_OUT_0();
	M_POW_CARD_OUT_PP();
	M_POW_CARD_OUT_0();
//	M_NFC_SCK_OUT_PP();
//	M_NFC_SCK_OUT_0();
	M_NFC_MOSI_OUT_PP();
	M_NFC_MOSI_OUT_0();
	M_NFC_CS_OUT_PP();
	M_NFC_CS_OUT_0();
	M_NFC_MISO_OUT_PP();
	M_NFC_MISO_OUT_0();
}
/***************************************************************************************
**������:       DRV_Power_ReuseInterrptService
**��������:     �жϷ������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ReuseInterrptService(EPORT_PINx pin)
{
	//	if (M_REUSE_IRQ_PIN == pin)
	//	{
	//		if (NULL != ReuseIntHandler)
	//		{
	//			ReuseIntHandler();
	//		}
	//	}
}

//extern volatile UINT32 g_systickCounter;
#include "tsi_app.h"
typedef enum
{
	/* System define */
	BUFANG_START = 0,
	BUFANG_DETET,
	BUFANG_NO_DETET,
	BUFANG_NONE,
} e_BUFANG_Status;
typedef enum
{
	/* System define */
	CLEAR = 0,
	NO_CLEAR

} e_Clear_Status;
/*�ж��Ƿ���Ҫ��ձ�־λ��CLEAR��ʾҪ��գ�NO_CLEAR��ʾ�������*/
uint8_t DRV_Bufang_Clear(void)
{
	if (IO_READ8(SRAM_START_ADDR_M4 + 0x20) == 0xaa)
		return CLEAR;
	else
		return NO_CLEAR;
}
/*�����⵽�������ˣ���Ҫ���������澯ʱ�򣬵��ô˽ӿ�*/
void DRV_BuFang_Time_Init(uint8_t time, uint32_t *systickCounter)
{
	uint32_t i, time_test;
	(*systickCounter) = 0;
	time_test = IO_READ8(SRAM_START_ADDR_M4 + 0x20); //��ȥ��ȡ�����¼���ֵ
													 //    printf("time_test=0x%0x\r\n",time_test);
	if ((time_test >= 0) && (time_test <= time))	 //������Ѿ���ֵ����ֵ�����Ѻ�ļ���ֵ
	{
		(*systickCounter) = time_test * 1000;
	}
	else //���û�У����ͷ��ʼ����
	{
		(*systickCounter) = 0;
	}
}
/*����ģʽ�²����������BUFANG_START��ʾ��ʼ��⣬BUFANG_DETET��ʾ���ˣ�BUFANG_NO_DETETû��⵽��*/

uint8_t DRV_BuFang_Time_Detet(uint8_t time, uint32_t *systickCounter)
{

	if ((*systickCounter) == 0) //���������,���������ſ�ͣ������ʼ�������ƽ
	{
		//		printf("EPORT_ReadGpioData(EPORT_PIN1)=%d\r\n",EPORT_ReadGpioData(EPORT_PIN1));
		if (M_IR_IRQ_READIN() == 0) //���������ŵ�ƽ������Ǹ�
		{
#if 0
SysTick_Config(g_sys_clk/1000);  //��������
SysTick_Enable(TRUE);
#endif
			return BUFANG_START;
		}
	}
	else if (((*systickCounter) > 0) && ((*systickCounter) > time * 1000)) //����ʱʱ�䵽25sʱ��
	{
		if (M_IR_IRQ_READIN() == 0) //25sʱ�䵽�����������ŵ�ƽ��������Ǹ�
		{
			(*systickCounter) = 0;
			DEBUG_LOG_OUT("bufangjiancedaoren\r\n");
			return BUFANG_DETET;
		}
	}
	else if (((*systickCounter) > 0) && ((*systickCounter) <= time * 1000))
	{
#if 0
SysTick_Config(g_sys_clk/1000);  //��������
SysTick_Enable(TRUE);
#endif
		return BUFANG_NO_DETET;
	}
	return BUFANG_NONE;
}

/*����͹���ʱ���ж��Ƿ���Ҫ�������*/
void DRV_BuFang_Time_Sleep(uint8_t time, uint32_t *systickCounter)
{
	DEBUG_LOG_OUT("time=%d,systickCounter=%d\r\n", time, *systickCounter);
	if (((*systickCounter) <= time * 1000) && ((*systickCounter) != 0)) //��������µü���С���趨ʱ��
	{

		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20, (*systickCounter) / 1000); //��������,�͹��ļ�������
		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x30, time);					   //��������,�͹��ļ�������
	}
	else
	{
		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20, 0xaa); //����͹��Ĳ���Ҫ�������
	}
}

/*��ʱ����ʱ������,//������������ʱ�䣬��׼��2s������Ϊ3������6�룬�������Ϊ0�����ǲ�Ҫ���ѹ���*/
void DRV_ReTry_Time_Set(uint8_t time)
{
	Nvram_Init();
	PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4, time);

	//	wakeuptimes1 = IO_READ8(SRAM_START_ADDR_M4);
	//printf("	wakeuptimes1 = 0x%d\r\n",wakeuptimes1);
	Nvram_Init();
	PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x40, 0);
}
/***************************************************************************************
**������:       DRV_Power_EnterSleep
**��������:     ����ϵͳ����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_EnterSleep(void)
{
	//	while(1);
	UINT8 interval_time = 0;							   //������������ʱ�䣬��׼��5s������Ϊ3������15�룬�������Ϊ0�����ǲ�Ҫ���ѹ���
	(*(volatile unsigned int *)(0xe000e010)) = 0x00000000; //disable SYS TICK
	(*(volatile unsigned int *)(0xe000e010));
	(*(volatile unsigned int *)(0xe000e010));
	DEBUG_LOG_OUT("DRV_Power_EnterSleep\n");

	//	M_WAKE_UP0_OUT_PP();
	//	M_WAKE_UP0_OUT_0(); //�ر�C0
	DEBUG_LOG_OUT("Sub_C0_into_poweroff2\n");
	//	Sub_C0_into_poweroff2();
	drv_SubC0_sleep();
	DEBUG_LOG_OUT("CPM_PowerOff_1p5\n");

	DRV_ReTry_Time_Set(0);
//	 HAL_BuFang_Time_Sleep(30,3000);
	//	wakeuptimes1 = IO_READ8(SRAM_START_ADDR_M4+ 0x40);
	//printf("	wakeuptimes2 = 0x%d\r\n",wakeuptimes1);
	//	(*(volatile unsigned int*)(0xe000e184)) = 0xffffffff;
	//	(*(volatile unsigned int*)(0xe000e180)) = 0x00004004;
	PCI_AsyncTC_Init(5000);

	//DEBUG_LOG_OUT("RTC->RTC_PRT1R=0x%04x\r\n",RTC->RTC_PRT1R);
	//DEBUG_LOG_OUT("RTC->RTC_PRT2R=0x%04x\r\n",RTC->RTC_PRT2R);
	//DEBUG_LOG_OUT("RTC->RTC_PRA1R=0x%04x\r\n",RTC->RTC_PRA1R);
	//DEBUG_LOG_OUT("RTC->RTC_PRA2R=0x%04x\r\n",RTC->RTC_PRA2R);
	//DEBUG_LOG_OUT("RTC->RTC_PRTCR=0x%04x\r\n",RTC->RTC_PRTCR);
	//DEBUG_LOG_OUT("RTC->RTC_PRC1R=0x%04x\r\n",RTC->RTC_PRC1R);
	//DEBUG_LOG_OUT("RTC->RTC_PRC1R=0x%04x\r\n",RTC->RTC_PRENR);
	//DEBUG_LOG_OUT("RTC->RTC_PRC1R=0x%04x\r\n",RTC->RTC_PRKEYR);

	DEBUG_LOG_OUT("PCI_AsyncTC_Init\n");
	//	while(1);
//    M_WIFI_RX_OUT_PP();
//    M_WIFI_RX_OUT_0();
//    M_WIFI_TX_OUT_PP();
//    M_WIFI_TX_OUT_0();
	CPM_PowerOff_1p5();
	DEBUG_LOG_OUT("CPM_PowerOff_1p5 ERROR\n");
	//	Enable_Interrupts; //�����ж�
	while (1)
	{
		DEBUG_LOG_OUT("!!!!!!!!!!!!!!!!!!!!!!!\n"); //̫�����ˣ�����ô���߲���
													//		(*(volatile unsigned int*)(0xe000e180)) = 0xffffffff;
													//		(*(volatile unsigned int*)(0xe000e010)) = 0x00000000;
													//		(*(volatile unsigned int*)(0xe000e010));
													//		(*(volatile unsigned int*)(0xe000e010));
													//		_CLEAR_IE_SCANFINISH(TSI);
													//		_CLEAR_IE_AWD(TSI);
		CPM_PowerOff_1p5();
	}
	//	M_WAKE_UP0_OUT_PP();
	//	M_WAKE_UP0_OUT_1(); //����C0
	//	Sub_C0_Init();
	//	DEBUG_LOG_OUT("CPM->CPM_PADWKINTCR:%X\n", CPM->CPM_PADWKINTCR);
	//	DEBUG_LOG_OUT("EPORTx->EPFR=0x%0x\r\n", EPORT->EPFR);
	//	DEBUG_LOG_OUT("TSI->TSI_IER=0x%04x\r\n", TSI->TSI_IER);
	//	DEBUG_LOG_OUT("TSI->TSI_ISR=0x%04x\r\n", TSI->TSI_ISR);
	//	//	DEBUG_LOG_OUT("EPORTx->EPFR=0x%0x\r\n",EPORTx->EPFR);
	//	//	DEBUG_LOG_OUT("TSI->TSI_IER=0x%04x\r\n",TSI->TSI_IER);
	//	DEBUG_LOG_OUT("CPM_PowerOff_1p5 fail\n");
}
unsigned int testcpm;
#if HAVE_BOOTLOADR > 0
void Interrput_Init(unsigned int IntAddr)
{
	SCB->VTOR = FLASH_BASE | IntAddr;
	NVIC_PriorityGroupConfig(2);
}
#endif
/***************************************************************************************
**������:       DRV_Power_CPUInit
**��������:     CPU��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_CPUInit(void)
{
	e_CPM_wakeupSource_Status rst_source = *(e_CPM_wakeupSource_Status *)CPM_WAKEUPSOURCE_REG;
	#if BACK_LOW_RST >0
    M_HS6621_RSTB_OUT_PP();
	M_HS6621_RSTB_OUT_0();
	#endif
	extern void IO_Latch_Clr(void);
	IO_Latch_Clr();
	Sys_Init();
    #if HAVE_BOOTLOADR > 0
    Interrput_Init(0xA000);
    INTX_ENABLE();//���жϣ���Ϊboot��߹ص���
    #endif
    printf("APP go to start\r\n");
	M_WAKE_UP0_OUT_PP();
	M_WAKE_UP0_OUT_1();
#if 1
//	if ((rst_source == WAKEUP_SOURCE_NONE) || (rst_source == WAKEUP_SOURCE_POR))
//	{
//		PCI_Init(EXTERNAL_CLK_SEL); //���ʹ��SDIO����Ҫ
//		TC_selectClockSource(1);
//		drv_SubC0_init(hSubC0);
//	}
//	else
//	{
//		PCI_InitWakeup(); // PCI reinit.
//		drv_SubC0_wakeup();
//	}
#if 0
	M_KEY_IRQ_EXIT_INT_DISABLE();
#endif
#if 0
	M_KEY_IRQ_IN_IPU();
	M_KEY_IRQ_EXIT_FALL_ENABLE();
#endif
#endif
#if 0
	if((rst_source1 ==RST_SOURCE_NONE )||(rst_source1 ==RST_SOURCE_POR))
	{
		PCI_Init(EXTERNAL_CLK_SEL); //���ʹ��SDIO����Ҫ
	}
	//else
	//{
		m4_m2s_int_enable();
	//}
	   CPM->CPM_CHIPCFGR &= ~(1<<22);	//m2s int.
#endif

	//	Sub_C0_Init(); //C0��ʼ��
	*(volatile unsigned int *)(0x4000001c) &= ~(1 << 15);
	*(volatile unsigned int *)(0x4000001c) &= ~(1 << 14);
	//PCI_Init(EXTERNAL_CLK_SEL); //���ʹ��SDIO����Ҫ
}

/***************************************************************************************
**������:       DRV_Power_SysWakeupChk
**��������:     ϵͳ�жϲ�ѯ 
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
TYPEe_SysWakeup DRV_Power_SysWakeupChk(void)
{
	e_CPM_wakeupSource_Status rst_source = *(e_CPM_wakeupSource_Status *)CPM_WAKEUPSOURCE_REG;
	TYPEe_SysWakeup Tmp = eSysWakeupNull;
	DEBUG_LOG_OUT("rst_source1:%d\n", rst_source);
	printf("CPM->CPM_PADWKINTCR=0x%04x\r\n", testcpm);
	printf("CPM->CPM_PADWKINTCR=0x%04x\r\n", CPM->CPM_PADWKINTCR);
	switch (rst_source)
	{
	case WAKEUP_SOURCE_GIN0:
		
		break;
	case WAKEUP_SOURCE_GIN1:
		
		break;
	case WAKEUP_SOURCE_GIN2:
		Tmp = eSysWakeupAlarm;
		break;
	case WAKEUP_SOURCE_GIN3:
        Tmp = eSysWakeupCatEye;
		
		break;
	case WAKEUP_SOURCE_GIN4:
		break;
	case WAKEUP_SOURCE_C0GIN5:
		
		break;
	case WAKEUP_SOURCE_POR:
		Tmp = eSysWakeupPOR;
		break;
	case WAKEUP_SOURCE_TIME:
		Tmp = eSysWakeupTime;
		break;
	case WAKEUP_SOURCE_TSI:
        Tmp = eSysWakeupTSI;
		break;
	case WAKEUP_SOURCE_ALARM:
		Tmp = eSysWakeupActiveDefense;
		break;
    case  WAKEUP_SOURCE_C0GIN0:
        Tmp = eSysWakeupFinger;
        break;
    case  WAKEUP_SOURCE_C0GIN1:
        Tmp = eSysWakeupBack;
        break;
    case  WAKEUP_SOURCE_C0GIN4:
        Tmp = eSysWakeupIr;
        break;
	default:
		break;
	}
	return Tmp;
}
/***************************************************************************************
**������:       DRV_Power_RAMToHold
**��������:     �����ݴ���RAM������ 
**�������:     
int8_t *dat,  ����
uint32_t len  ���ݳ���
**�������:     --
**��ע:         
****************************************************************************************/
int8_t DRV_Power_RAMToHold(uint8_t *dat, uint32_t len)
{
#if 1
	DEBUG_LOG_OUT("RAM to Hold\n");
	if (NULL == dat)
	{
		return -1; //��������
	}
	if ((CPM->CPM_PWRSR & 0x2100) != 0x2100)
	{
		return -2; //ϵͳ������д��
	}

	memcpy((uint8_t *)(DRV_POWER_RAM_HOLD_ADDR), dat, len);
	DEBUG_LOG_OUT("RAM to Hold OK\n");
#endif
	return 0;
}
/***************************************************************************************
**������:       DRV_Power_HoldToRAM
**��������:     RAM��������ȡ���� 
**�������:     
int8_t *dat,  ����
uint32_t len  ���ݳ���
**�������:     --
**��ע:         
****************************************************************************************/
int8_t DRV_Power_HoldToRAM(uint8_t *dat, uint32_t len)
{
#if 1
	DEBUG_LOG_OUT("Hold to RAM \n");
	if (NULL == dat)
	{
		return -1; //��������
	}
	if ((CPM->CPM_PWRSR & 0x2100) != 0x2100)
	{
		return -2; //ϵͳ������д��
	}
	memcpy(dat, (uint8_t *)(DRV_POWER_RAM_HOLD_ADDR), len);
	DEBUG_LOG_OUT("Hold to RAM OK\n");
#endif
	return 0;
}
/***************************************************************************************
**������:       DRV_Power_ReservedWakeupConfig
**��������:     δʹ�õ���IO��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_ReservedWakeupConfig(void)
{
	M_SDA_IN_NOPULL();
	M_SCL_IN_NOPULL();
	M_3802_IRQ_OUT_PP();
	M_3802_IRQ_OUT_0();
	M_POW_CARD_OUT_PP();
	M_POW_CARD_OUT_0();
	M_NFC_SCK_OUT_PP();
	M_NFC_SCK_OUT_0();
	M_NFC_MOSI_OUT_PP();
	M_NFC_MOSI_OUT_0();
	M_NFC_CS_OUT_PP();
	M_NFC_CS_OUT_0();
	M_NFC_MISO_OUT_PP();
	M_NFC_MISO_OUT_0();
}
/***************************************************************************************
**������:       DRV_Power_WakeupIoConfig
**��������:     ���Ѻ�IO����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_WakeupIoConfig(void)
{
}
/***************************************************************************************
**������:       DRV_Power_CpuRst
**��������:     CPU����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_CPURst(void)
{
	Set_POR_Reset();
}

/***************************************************************************************
**������:       DRV_Power_TrngInit
**��������:     �������������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Power_TrngInit(void)
{
	Init_Trng();
}
/***************************************************************************************
**������:       DRV_Power_GetRandomWord
**��������:     �����������
**�������:     --
**�������:     
uint32_t �����
**��ע:         
****************************************************************************************/
uint32_t DRV_Power_GetRandomWord(void)
{
	return GetRandomWord();
}

/***************************************************************************************
**������:       DRV_InitWDT
**��������:     ��ʼ�����Ź�
**�������:     --
**�������:     
uint32_t
���Ź�ʱ��
**��ע:         
 ��оʹ�����������Ҫ��
startup_ARMCM4.s �ļ����������µĴ���
;				MOV     r0,#0xe
;                LDR     R1,=0x40005000
;				STRH    r0,[r1,#0]       ;Close WDT
****************************************************************************************/
void DRV_InitWDT(uint16_t t)
{
	WDT_Init(t);
}	
/***************************************************************************************
**������:       DRV_WDTFeedDog
**��������:     ι��
**�������:     --
**�������:     
uint32_t
���Ź�ʱ��
**��ע:         
****************************************************************************************/
void DRV_WDTFeedDog(void)
{
	WDT_FeedDog();
}	

#endif /*DRV_POWER_MODULE_EN*/
/************************************Code End******************************************/
