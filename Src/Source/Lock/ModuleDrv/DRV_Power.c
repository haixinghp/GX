/***************************************************************************************
**文件名:     DRV_Power.c
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
#warning DRV_POWER模块使能!
/*******************************宏定义***************************/
#define DRV_POWER_RAM_HOLD_ADDR (0x08100C00)
//#define DRV_POWER_RAM_HOLD_ADDR (0x08100300)
//#define DRV_POWER_IN_PIN_HANDLER_NUM (10)
//#define DRV_POWER_NULL_PIN (0XFF)
//#define DRV_POWER_KEY_IN_PIN_HANDLER_INDEX (0)
/*************************.C变量结构声明***********************/
//typedef struct
//{
//	nrf_drv_gpiote_pin_t pin;
//	FunInPinHandler_t keyInPinHandler;
//} TYPEs_InPinHandler;
/*************************私有全局变量*************************/

//static TYPEs_InPinHandler inPinHandler[DRV_POWER_IN_PIN_HANDLER_NUM] =
//	{
//		{
//			M_TOUCH_INT_GPIO_PIN,
//			NULL,
//		}, //0
//		{DRV_POWER_NULL_PIN, NULL},
//};

/*************************公共全局变量*************************/
ReuseIntHandler_t ReuseIntHandler = NULL; //中断指针
/*************************局部函数*****************************/

/***************************************************************************************
**函数名:       DRV_Power_ReuseChk
**功能描述:     复用中断查询中断源 
**输入参数:     --
**输出参数:     --
**备注:         
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
			if (TimeCnt[i] > DRV_POWER_FILTER_TIME) //连续读取10次
			{
				return i;
			}
		}
		if (overTime >= DRV_POWER_FILTER_OVET_TIME) //超时，唤醒源为空
		{
			return eReuseIntNull;
		}
	}
}
///***************************************************************************************
//**函数名:       in_pin_hadler
//**功能描述:     回调处理
//**输入参数:     --
//**输出参数:     --
//**备注:
//****************************************************************************************/
//static void in_pin_hadler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
//{
//	uint8 i = 0;
//	for (i = 0; i < DRV_POWER_IN_PIN_HANDLER_NUM; i++)
//	{
//		if (inPinHandler[i].pin == pin)
//		{
//			if (NULL != inPinHandler[i].keyInPinHandler) //保证指针不为空
//			{
//				inPinHandler[i].keyInPinHandler();
//				return; //直接退出
//			}
//		}
//	}
//}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_Power_TouchExitInPinHandlerReg
**功能描述:     注册触摸中断回调句柄
**输入参数:     
FunInPinHanler_t fun
要注册的函数
**输出参数:     
int8
0 成功
-1 失败
**备注:         
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
**函数名:       DRV_Power_GpioInterrputInit
**功能描述:     初始化系统中断
**输入参数:     --
**输出参数:     --
**备注:         注意！由于NRF52832芯片nrf_drv_gpiote_init();只能初始化一次，所以这个
函数要放在最开始调用。
****************************************************************************************/
void DRV_Power_GpioInterrputInit(void)
{
}

/***************************************************************************************
**函数名:       DRV_Power_GpioSetWakeup
**功能描述:     唤醒GPIO配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_GpioSetWakeup(void)
{
}
/***************************************************************************************
**函数名:       DRV_Power_GPIOInterrputSetSleep
**功能描述:     休眠GPIO中断配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_GpioInterrputSetSleep(void)
{
}
/***************************************************************************************
**函数名:       DRV_Power_ClosePeripheral
**功能描述:     关闭外设
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_ClosePeripheral(void)
{
}
/***************************************************************************************
**函数名:       DRV_Power_GpioSetSleep
**功能描述:     休眠GPIO配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_GpioSetSleep(void)
{
}
/***************************************************************************************
**函数名:       DRV_Power_ReuseInterruptConfig
**功能描述:     复用中断配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_ReuseInterruptConfig(void)
{
    
}
/***************************************************************************************
**函数名:       DRV_Power_ReuseInterruptWakeupConfig
**功能描述:     复用中断配置唤醒
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_ReuseInterruptWakeupConfig(void)
{
	//	M_REUSE_IRQ_IN_NOPULL();
	//	M_REUSE_IRQ_EXIT_LOTOHI_DISABLE();
	//	M_REUSE_IRQ_EXIT_LOTOHI_ENABLE();
}
/***************************************************************************************
**函数名:       DRV_Power_ReservedSleepConfig
**功能描述:     未使用到的IO初始化
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       DRV_Power_ReuseInterrptService
**功能描述:     中断服务程序
**输入参数:     --
**输出参数:     --
**备注:         
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
/*判断是否需要清空标志位，CLEAR表示要清空，NO_CLEAR表示不用清空*/
uint8_t DRV_Bufang_Clear(void)
{
	if (IO_READ8(SRAM_START_ADDR_M4 + 0x20) == 0xaa)
		return CLEAR;
	else
		return NO_CLEAR;
}
/*当你检测到门外有人，需要启动布防告警时候，调用此接口*/
void DRV_BuFang_Time_Init(uint8_t time, uint32_t *systickCounter)
{
	uint32_t i, time_test;
	(*systickCounter) = 0;
	time_test = IO_READ8(SRAM_START_ADDR_M4 + 0x20); //先去读取休眠下计数值
													 //    printf("time_test=0x%0x\r\n",time_test);
	if ((time_test >= 0) && (time_test <= time))	 //如果是已经有值，则赋值给唤醒后的计数值
	{
		(*systickCounter) = time_test * 1000;
	}
	else //如果没有，则从头开始计数
	{
		(*systickCounter) = 0;
	}
}
/*工作模式下布防检测结果，BUFANG_START表示开始检测，BUFANG_DETET表示到人，BUFANG_NO_DETET没检测到人*/

uint8_t DRV_BuFang_Time_Detet(uint8_t time, uint32_t *systickCounter)
{

	if ((*systickCounter) == 0) //工作情况下,当有人在门口停留，开始检测红外电平
	{
		//		printf("EPORT_ReadGpioData(EPORT_PIN1)=%d\r\n",EPORT_ReadGpioData(EPORT_PIN1));
		if (M_IR_IRQ_READIN() == 0) //检测红外引脚电平，如果是高
		{
#if 0
SysTick_Config(g_sys_clk/1000);  //启动计数
SysTick_Enable(TRUE);
#endif
			return BUFANG_START;
		}
	}
	else if (((*systickCounter) > 0) && ((*systickCounter) > time * 1000)) //当计时时间到25s时候
	{
		if (M_IR_IRQ_READIN() == 0) //25s时间到，检测红外引脚电平，如果还是高
		{
			(*systickCounter) = 0;
			DEBUG_LOG_OUT("bufangjiancedaoren\r\n");
			return BUFANG_DETET;
		}
	}
	else if (((*systickCounter) > 0) && ((*systickCounter) <= time * 1000))
	{
#if 0
SysTick_Config(g_sys_clk/1000);  //启动计数
SysTick_Enable(TRUE);
#endif
		return BUFANG_NO_DETET;
	}
	return BUFANG_NONE;
}

/*进入低功耗时候判断是否需要布防检测*/
void DRV_BuFang_Time_Sleep(uint8_t time, uint32_t *systickCounter)
{
	DEBUG_LOG_OUT("time=%d,systickCounter=%d\r\n", time, *systickCounter);
	if (((*systickCounter) <= time * 1000) && ((*systickCounter) != 0)) //工作情况下得计数小于设定时间
	{

		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20, (*systickCounter) / 1000); //保存起来,低功耗继续计数
		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x30, time);					   //保存起来,低功耗继续计数
	}
	else
	{
		PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20, 0xaa); //否则低功耗不需要布防检测
	}
}

/*定时重连时间设置,//设置死机唤醒时间，基准是2s，设置为3，就是6秒，如果设置为0，就是不要唤醒功能*/
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
**函数名:       DRV_Power_EnterSleep
**功能描述:     进入系统休眠
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_EnterSleep(void)
{
	//	while(1);
	UINT8 interval_time = 0;							   //设置死机唤醒时间，基准是5s，设置为3，就是15秒，如果设置为0，就是不要唤醒功能
	(*(volatile unsigned int *)(0xe000e010)) = 0x00000000; //disable SYS TICK
	(*(volatile unsigned int *)(0xe000e010));
	(*(volatile unsigned int *)(0xe000e010));
	DEBUG_LOG_OUT("DRV_Power_EnterSleep\n");

	//	M_WAKE_UP0_OUT_PP();
	//	M_WAKE_UP0_OUT_0(); //关闭C0
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
	//	Enable_Interrupts; //开启中断
	while (1)
	{
		DEBUG_LOG_OUT("!!!!!!!!!!!!!!!!!!!!!!!\n"); //太可怕了，我特么休眠不了
													//		(*(volatile unsigned int*)(0xe000e180)) = 0xffffffff;
													//		(*(volatile unsigned int*)(0xe000e010)) = 0x00000000;
													//		(*(volatile unsigned int*)(0xe000e010));
													//		(*(volatile unsigned int*)(0xe000e010));
													//		_CLEAR_IE_SCANFINISH(TSI);
													//		_CLEAR_IE_AWD(TSI);
		CPM_PowerOff_1p5();
	}
	//	M_WAKE_UP0_OUT_PP();
	//	M_WAKE_UP0_OUT_1(); //开启C0
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
**函数名:       DRV_Power_CPUInit
**功能描述:     CPU初始化
**输入参数:     --
**输出参数:     --
**备注:         
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
    INTX_ENABLE();//打开中断，因为boot里边关掉了
    #endif
    printf("APP go to start\r\n");
	M_WAKE_UP0_OUT_PP();
	M_WAKE_UP0_OUT_1();
#if 1
//	if ((rst_source == WAKEUP_SOURCE_NONE) || (rst_source == WAKEUP_SOURCE_POR))
//	{
//		PCI_Init(EXTERNAL_CLK_SEL); //如果使用SDIO必须要
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
		PCI_Init(EXTERNAL_CLK_SEL); //如果使用SDIO必须要
	}
	//else
	//{
		m4_m2s_int_enable();
	//}
	   CPM->CPM_CHIPCFGR &= ~(1<<22);	//m2s int.
#endif

	//	Sub_C0_Init(); //C0初始化
	*(volatile unsigned int *)(0x4000001c) &= ~(1 << 15);
	*(volatile unsigned int *)(0x4000001c) &= ~(1 << 14);
	//PCI_Init(EXTERNAL_CLK_SEL); //如果使用SDIO必须要
}

/***************************************************************************************
**函数名:       DRV_Power_SysWakeupChk
**功能描述:     系统中断查询 
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       DRV_Power_RAMToHold
**功能描述:     把数据存入RAM保持区 
**输入参数:     
int8_t *dat,  数据
uint32_t len  数据长度
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t DRV_Power_RAMToHold(uint8_t *dat, uint32_t len)
{
#if 1
	DEBUG_LOG_OUT("RAM to Hold\n");
	if (NULL == dat)
	{
		return -1; //参数错误
	}
	if ((CPM->CPM_PWRSR & 0x2100) != 0x2100)
	{
		return -2; //系统不允许写入
	}

	memcpy((uint8_t *)(DRV_POWER_RAM_HOLD_ADDR), dat, len);
	DEBUG_LOG_OUT("RAM to Hold OK\n");
#endif
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Power_HoldToRAM
**功能描述:     RAM保持区读取数据 
**输入参数:     
int8_t *dat,  数据
uint32_t len  数据长度
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t DRV_Power_HoldToRAM(uint8_t *dat, uint32_t len)
{
#if 1
	DEBUG_LOG_OUT("Hold to RAM \n");
	if (NULL == dat)
	{
		return -1; //参数错误
	}
	if ((CPM->CPM_PWRSR & 0x2100) != 0x2100)
	{
		return -2; //系统不允许写入
	}
	memcpy(dat, (uint8_t *)(DRV_POWER_RAM_HOLD_ADDR), len);
	DEBUG_LOG_OUT("Hold to RAM OK\n");
#endif
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Power_ReservedWakeupConfig
**功能描述:     未使用到的IO初始化
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       DRV_Power_WakeupIoConfig
**功能描述:     唤醒后IO配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_WakeupIoConfig(void)
{
}
/***************************************************************************************
**函数名:       DRV_Power_CpuRst
**功能描述:     CPU重启
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_CPURst(void)
{
	Set_POR_Reset();
}

/***************************************************************************************
**函数名:       DRV_Power_TrngInit
**功能描述:     随机数发生器初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Power_TrngInit(void)
{
	Init_Trng();
}
/***************************************************************************************
**函数名:       DRV_Power_GetRandomWord
**功能描述:     随机数发生器
**输入参数:     --
**输出参数:     
uint32_t 随机数
**备注:         
****************************************************************************************/
uint32_t DRV_Power_GetRandomWord(void)
{
	return GetRandomWord();
}

/***************************************************************************************
**函数名:       DRV_InitWDT
**功能描述:     初始化看门狗
**输入参数:     --
**输出参数:     
uint32_t
看门狗时间
**备注:         
 国芯使用这个功能需要在
startup_ARMCM4.s 文件中屏蔽如下的代码
;				MOV     r0,#0xe
;                LDR     R1,=0x40005000
;				STRH    r0,[r1,#0]       ;Close WDT
****************************************************************************************/
void DRV_InitWDT(uint16_t t)
{
	WDT_Init(t);
}	
/***************************************************************************************
**函数名:       DRV_WDTFeedDog
**功能描述:     喂狗
**输入参数:     --
**输出参数:     
uint32_t
看门狗时间
**备注:         
****************************************************************************************/
void DRV_WDTFeedDog(void)
{
	WDT_FeedDog();
}	

#endif /*DRV_POWER_MODULE_EN*/
/************************************Code End******************************************/
