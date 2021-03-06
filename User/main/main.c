#include "sys.h"
#include "debug.h"
#include "type.h"
#include "reboot_demo.h"
#include "main.h"
#include "i2c_reg.h"
#include "cpm_drv.h"
#include "common.h"
#include "dac_audio.h"
#include "DRV_MBI5024.h"
#include "pci_drv.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "HAL_FaceId.h"
#include "DRV_SysTimer.h"
#include "AppUser.h"
#include "HAL_Storage.h"
#include "menu.h"
#include "HAL_KeyLight.h"
#include "cpm_drv.h"
#include "HAL_Storage.h"
#include "HAL_KeySacn.h"
#include "HAL_FaceId.h"
#include "HAL_RTC.h"
#include "HAL_Comm.h"
#include "HAL_Voice.h"
#include "DRV_Voice.h"



/*！！！！！！！！！！！！！！！！！撃廾窃侏式哈重蛍塘！！！！！！|
哈重       | QFN32 | QFN40 | QFN48 | QFN88 |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
SPI        |   2   |   1   |   3   |   3   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
I2C        |   1   |   1   |   1   |   3   |   
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
SSI        |   0   |   1   |   0   |   1   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
UART       |   1   |   1   |   3   |   3   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
ISO7816    |   2   |   0   |   0   |   1   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
USB_OTG    |   1   |   1   |   1   |   1   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
ADC        |   0   |   1   |   2   |   3   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
DAC        |   0   |   0   |   1   |   1   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
PWM(鹸喘)  |   3   |   4   |   2   |   4   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
MCC        |   0   |   0   |   0   |   0   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
TSI(鹸喘)  |   8   |   5   |  10   |  16   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
EPORT(鹸喘)|  10   |  18   |  19   |  20   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
SDIO(契介) |   0   |   0   |   0   |   1   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
C0 SUB I/O |   0   |   0   |   0   |   4   |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|*/

/*！！！！！！！！！！！！！！！USB 屶隔秤趨！！！！！！！！！！！！！！|
撃廾   | USB2.0 | USB1.1 |  host  | device |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
QFN32  | 音辛參 |  辛參  | 音屶隔 |  屶隔  |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
QFN40  |  辛參  |  辛參  | 音屶隔 |  屶隔  |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
QFN48  | 音辛參 |  辛參  | 音屶隔 |  屶隔  |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|
QFN88  | 音辛參 |  辛參  | 音屶隔 |  屶隔  |
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！|*/

static void Printf_Version(void)
{
	printf("/**************************************************/\n");
	printf(" date = %s,time = %s    \r\n",( char *)__DATE__,( char *)__TIME__);
	printf(" C*CORE CCM4202S Demo V2.0  \r\n");
	printf(" system clk = %dMHz         \r\n",g_sys_clk/1000000);
	printf(" ips clk    = %dMHz         \r\n",g_ips_clk/1000000);
	printf("/**************************************************/\n");
}

static void Reback_Boot(void)
{
 	I2C_Config_Gpio_Reboot(I2C_SDA, GPIO_INPUT);
	if (I2C_Read_Gpio_Data_Reboot(I2C_SDA) == Bit_RESET)//L level
	{
		Reboot_Demo();
		while(1);
	}
}


void App_Demo(void)
{
    //庁方廬算??屶隔撃廾??QFN40、QFN48、QFN88、BGA121
	#ifdef ADC_DEMO
		ADC_Demo();
	#endif
	
    //麻隈: SM2/SM3/SM4/SHA/DES/3DES/AES/RSA?? 屶隔撃廾??ALL
	#ifdef ALGO_DEMO
		Algo_Demo();
	#endif	
	//USI(7816)slave demo,屶隔撃廾??QFN32、QFN88、BGA121
	#ifdef APDU_DEMO	
		APDU_Demo();
	#endif
	
    //USI(7816)master demo,屶隔撃廾??QFN32、QFN88、BGA121
	#ifdef BCTC_DEMO	
		BCTC_Test_Demo();
	#endif	
	
    //清兩??屶隔撃廾??QFN48、QFN88、BGA121
	#ifdef BT_DEMO
		BT_Demo();
	#endif	
	
    //扮嶝窮坿庁翠??屶隔撃廾??ALL
	#ifdef CPM_DEMO
		CPM_Demo();
	#endif
	
    //仂頭塘崔庁翠??屶隔撃廾??ALL
	#ifdef CCM_DEMO
		CCM_Demo();
	#endif
	
    //DAC囂咄demo??屶隔撃廾??QFN48、QFN88、BGA121
	#ifdef DAC_AUDIO_DEMO
//		DAC_Audio_Demo();
//	DAC_Audio_Demo(C_AUDIO_0);
	#endif
	
    //方庁廬算??屶隔撃廾??QFN48、QFN88、BGA121
	#ifdef DAC_DEMO
		DAC_Demo();
	#endif
	
    //鋤峭ROM boot尼強??屶隔撃廾??ALL
	#ifdef DISBOOT_DEMO
		Disboot_Demo();
	#endif	
	
    //DMA??屶隔撃廾??ALL
	#ifdef DMA_DEMO
		DMA_Demo();
	#endif
	
    //検泣塰麻??屶隔撃廾??ALL
	#ifdef DSP_DEMO
		DSP_Demo();
	#endif
	
    //EDMA??屶隔撃廾??ALL
	#ifdef EDMA_DEMO
		EDMA_Demo();
	#endif
	
    //EFLASH??屶隔撃廾??ALL
	#ifdef EFLASH_DEMO
		EFlash_Demo();
	#endif	
	
    //醤嗤円冽乾窟嶄僅議GPIO??屶隔撃廾??ALL
	#ifdef EPORT_DEMO
		Eport_Demo();
	#endif
	
    //資函仂頭率匯會双催??屶隔撃廾??ALL
	#ifdef GETSN_DEMO
	  GetSN_Demo();
	#endif
	
    //I2C??屶隔撃廾??ALL
	#ifdef I2C_DEMO
		I2C_Demo();
	#endif
	
    //鋤峭径寔??峇佩朔仂頭繍音嬬径寔??屶隔撃廾??ALL
	#ifdef LOCK_DISJTAG_DEMO
		Lock_DisJtag_Demo();
	#endif
	
    //掲俊哘喘??屶隔撃廾??
	#ifdef NFC_DEMO
		NFC_Demo();
	#endif
	
    //契介??恷謹屶隔8揃床蓑??4揃強蓑??屶隔撃廾??QFN88、BGA121
	#ifdef PCI_DEMO
		PCI_Demo();
	#endif
	
    //32bits 辛園殻嶄僅協扮匂??屶隔撃廾??ALL
	#ifdef PIT32_DEMO
		PIT32_Demo();
	#endif
	
    //PWM??屶隔撃廾??ALL
	#ifdef PWM_DEMO
		PWM_Demo();
	#endif

    //鹸了??屶隔撃廾??ALL
	#ifdef RESET_DEMO
		Reset_Demo();
	#endif

    //糞扮扮嶝??屶隔撃廾??QFN48、QFN88、BGA121
	#ifdef RTC_DEMO
//	extern void RTC_TIME_TEST();
//	RTC_TIME_TEST();
		RTC_Demo();
//RTC_TIME_TEST();
	#endif
	
    //扮寂漢??屶隔撃廾??QFN48、QFN88、BGA121
	#ifdef RTC_TIMESTAMP_DEMO
		RTC_TimeStamp_Demo();
	#endif

    //SPI??屶隔撃廾??ALL
	#ifdef SPI_DEMO
		SPI_Demo();
	#endif
	
    //互堀SPI??屶隔汽??(噸宥SPI)、褒?澪曜掴滷?峪屶隔麼庁塀??屶隔撃廾??QFN40、QFN48(汽?澪曜???)、QFN88、BGA121
	#ifdef SSI_DEMO
	   SSI_Demo();
	#endif
	
    //C0徨狼由箭殻??旗鷹貫M4麼狼由衣欺C0徨狼由塰佩??屶隔撃廾??QFN88、BGA121
	#ifdef SUBC0_DEMO
		Sub_C0_Demo();
	#endif
	
    //M4坪宰扮嶝??屶隔撃廾??ALL
	#ifdef SYSTICK_DEMO
		SysTick_Demo();
	#endif
	
    //協扮柴方匂 1揃??屶隔撃廾??ALL
	#ifdef TC_DEMO
		TC_Demo();
	#endif
	
    //乾陣??屶隔撃廾??ALL
	#ifdef TSI_DEMO
		TSI_Demo();
	#endif
	
    //堪笥??屶隔撃廾??ALL
	#ifdef UART_DEMO
		UART_Demo();
	#endif
	
    //USB??屶隔撃廾??ALL
	#ifdef USB_DEMO
		USB_Demo();
	#endif

    //心壇昂??屶隔撃廾??ALL
	#ifdef WDT_DEMO
		WDT_Demo();
	#endif
	
    //SSI俊笥??SPI Flash塰佩旗鷹??峪屶隔SSI1??屶隔撃廾??QFN40、QFN88、BGA121
	#ifdef XIP_DEMO
	    XIP_Demo();
	#endif
	#ifdef MRI_SENSOR_DEMO
	MRI_Sensor_Demo();
	#endif
}
uint8_t test[5]={0x15,0x01,0xaa};
int main()
{
//	UINT8 i;
//	/*狼由塘崔??資函扮嶝?犢慍諒?吉 */
//	Sys_Init(); 
//	Sub_C0_Init(); //C0兜兵晒
//	App_Demo();
//	Disboot_Demo();
	
//	SEGGER_RTT_Init();
//	DEBUG_LOG_OUT("System powerup ok??\n");
//	DEBUG_LOG_OUT("繁然佚連譜崔萩梓1\n");
	
	
//	DEBUG_LOG_OUT_HEX(test,5);
//	do                                           
//  {                                            
//    uint32_t i;                                
//    for (i = 0; i < 5; i++)                  
//    {                                          
//      SEGGER_RTT_printf(0,"%x ", test[i]); 
//    }                                          
//  } while (0);
//	//指欺ROM BOOT庁塀
//	Reback_Boot();

	/*塘崔堪笥恬葎debug喘??扮嶝功象音揖桟廠塘崔音揖議扮嶝歌方*/
//	UART_Debug_Init(SCI1,g_ips_clk, 115200);
//HAL_Comm_Init();
//UART_Debug_Init(SCI1,g_ips_clk, 57600);
//HAL_Comm_Send(eCommPowerOnSync);
//	DEBUG_LOG_OUT("System powerup ok!\n");
    //UART嬉咫井云佚連
//	Printf_Version();
//	PCI_Init(EXTERNAL_CLK_SEL);
//	SDIO_ConfigGpio(SDIO_PIN0,GPIO_OUTPUT);
//	SDIO_WriteGpioData(SDIO_PIN0,Bit_SET);
//	DelayMS(100);
//	extern void audio_play(AUDIO_VOICE_TYPE VoiceType);
//	

//	EPORT_ConfigGpio(EPORT_PIN27, GPIO_OUTPUT);
//	EPORT_WriteGpioData(EPORT_PIN27, Bit_RESET);
//	DRV_MBI5024_Out(0XFFF5,1);
//	M_VOICE_OUT_PP();
//	while(1)
//	{
//		DelayUS(1);
//		M_VOICE_OUT_1();
//		DelayUS(1);
//		M_VOICE_OUT_0();
//	}
//	DRV_MBI5024_Out(0X0,1);
//		while(1)
//	{
//for(i=0;i<3;i++)
//		audio_play(i);
		
//}
//	while(1);
//	M_BELL_OUT_PP();
//	M_BELL_OUT_1(); //蝕尼朔医議壇槽
//HAL_FaceId_ClosePow();
//DelayMS(1500);
//	HAL_FaceId_Init();
//	DelayMS(5000);
//	msgInitData.enableNetwork = 0; //音選利
//	msgInitData.mode = 0; //苧猟
//	HAL_FaceId_CmdSend(0x25);
//HAL_FaceId_Init();    //兜兵晒堪笥
//	HAL_FaceId_CmdPowerOn(); //貧窮
//	HAL_FaceId_AddUserStart(); //蝕兵耶紗送殻
//	M_FINGER_IRQ_IN_IPD(); //和性
////	M_FINGER_IRQ_EXIT_LOTOHI_ENABLE(); //貧幅冽嶄僅
//	M_X_IRQ_9555_IN_NOPULL();
//	M_X_IRQ_9555_EXIT_HITOLO_ENABLE(); //和週冽嶄僅
//	M_REUSE_IRQ_IN_NOPULL();
//	M_REUSE_IRQ_EXIT_LOTOHI_ENABLE(); //貧幅冽乾窟
//	M_CC4202_INT_IN_IPD();
//	M_CC4202_INT_EXIT_LOTOHI_ENABLE(); //貧幅冽乾窟
//	HAL_FaceId_CmdSend(0x25);
//	while(1)
//	{
//		DelayMS(1);
//		HAL_FaceId_PowerOnDeal();
//		HAL_FaceId_RevcDeal();
//		HAL_FaceId_AddUser(); //耶紗送殻
//	}
//MRI_Sensor_Demo();
//	//峇佩Demo殻會
	//

//	DRV_SysTimerInit(); //蝕尼狼由扮嶝1ms距喘匯肝
	AppUser_Init();
//	st_InputInfo.FingerIdTmp =0;
//				memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //賠腎歌方
//        storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;                           //贋壓炎崗
//        storageSave.UserDat.pageID = st_InputInfo.FingerIdTmp;						//隠贋ID
//        HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                   MSG_FINGER_ONE_SIZE,
//                                   st_InputInfo.FingerIdTmp,
//                                   &storageSave.UserDat); //隠贋峺瞭佚連
//								   
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);
//		DEBUG_LOG_OUT("MSG_FINGER_REG_MEM_LOCAL_ST:%x\n",MSG_FINGER_REG_MEM_LOCAL_ST);
//		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //賠腎歌方
//        HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                  MSG_FINGER_ONE_SIZE,
//                                  st_InputInfo.FingerIdTmp,
//                                  &storageSave.UserDat);       //響函歌方	
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);							
//	st_InputInfo.FingerIdTmp = HAL_Storage_SearchEmpty(0,
//														   MEM_FINGER_ALL_LOCAL,
//														   MSG_FINGER_REG_MEM_LOCAL_ST,
//														   MSG_FINGER_ONE_SIZE); //資函腎了
//																				 //短嗤腎了議扮昨??音氏序秘欺宸戦
//		DEBUG_LOG_OUT("Menu_Comm_BleNewAddFinger st_InputInfo.FingerIdTmp:%d\n", st_InputInfo.FingerIdTmp);

//		DelayMS(100);
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);
//		DEBUG_LOG_OUT("MSG_FINGER_REG_MEM_LOCAL_ST:%x\n",MSG_FINGER_REG_MEM_LOCAL_ST);
//		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //賠腎歌方
//        HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                  MSG_FINGER_ONE_SIZE,
//                                  st_InputInfo.FingerIdTmp,
//                                  &storageSave.UserDat);       //響函歌方
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);
//	#ifdef DISBOOT_DEMO
//		Disboot_Demo();
//	#endif	
//	HAL_RTC_Init();
//	struct tm test;
//	uint32_t tick;
//	while(1)
//	{
//		DelayMS(1000);
//		test = HAL_RTC_TimeGetStm();
//		tick = HAL_RTC_Stm2Tick(test);
////		&test = HAL_RTC_Tick2Stm(tick);
//		DEBUG_LOG_OUT("HAL_RTC_Stm2Tick: %X",tick);
//		DEBUG_LOG_OUT("test.tm_sec: %d",test.tm_sec);
//		DEBUG_LOG_OUT("test.tm_min: %d",test.tm_min);
//		DEBUG_LOG_OUT("test.tm_hour: %d",test.tm_hour);
//		DEBUG_LOG_OUT("test.tm_mday: %d",test.tm_mday);
//		DEBUG_LOG_OUT("test.tm_mon: %d",test.tm_mon);
//		DEBUG_LOG_OUT("test.tm_year: %d",test.tm_year);
//	}
	
/**********************贋刈響亟***************************/
//	uint8_t testTab[100]={0x44,0xaa,0x00};
//	uint8_t testTab1[100]={0x00,};
//	extern INT8S HAL_Storage_Rw(TYPEe_StorageDire dir, INT32U addr, INT8U *data, INT16U length);
//	
//	HAL_Storage_Rw(eStorageDireWrite,0,testTab,100);
//	HAL_Storage_Rw(eStorageDireRead,0,testTab1,100);
//	while(1);
//	HAL_KeyLightOnce();
//	M_LED_B_PWM_OUT_PP();
//	M_LED_B_PWM_OUT_0();
//DRV_MBI5024_Out(0xffff,1);
	
//	uint16_t test =0x3000;
//	uint16_t test1 =0x7800;
//	uint16_t test2 =0;
//	uint16_t test3 =0;
//	uint16_t mb =0x2000;
//	test2 = test1 & (~test);
//	test3 = test2 | mb;
//	DRV_MBI5024_Out(test2,1);
//	DRV_MBI5024_Out(test3,1);
//DRV_MBI5024_Out(DRV_MBI5024_LED_G,1);
//	while(1);
//	HAL_KeyLightInit();
//	HAL_KeyLightStatusRun(1);

//	HAL_KeyLight_batterySet(0x8000);
//	HAL_KeyLight_sysStatusSet(0x2000);
//	
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //贋壓炎崗
//					//功象暇汽耶紗議窃侏僉夲砿尖埀珊頁噸宥
//	storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN; //砿尖埀
//	storageSave.UserDat.pageID = 4; //隠贋方象

//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;
//	storageSave.UserDat.sn = 2;
//	storageSave.UserDat.up_pageID = 3;
//	storageSave.UserDat.down_pageID = 4;
//	storageSave.UserDat.Lim = 5;
//	storageSave.UserDat.group = 6;
//	storageSave.UserDat.name[0] = 0;
//	storageSave.UserDat.name[1] = 1;
//	storageSave.UserDat.name[2] = 2;
//	storageSave.UserDat.name[3] = 3;
//	storageSave.UserDat.name[4] = 4;
//	storageSave.UserDat.name[5] = 5;
//	storageSave.UserDat.pageID = 1;
//	storageSave.UserDat.pwd.fig = 2;
//	storageSave.UserDat.pwd.pwd[0] = 3;
//	storageSave.UserDat.pwd.pwd[1] = 4;
//	storageSave.UserDat.pwd.pwd[2] = 5;
//	storageSave.UserDat.TimeVaild.fig = 1;
//	storageSave.UserDat.TimeVaild.str.year=0;
//	storageSave.UserDat.TimeVaild.str.mon=1;
//	storageSave.UserDat.TimeVaild.str.day=2;
//	storageSave.UserDat.TimeVaild.str.hour=3;
//	storageSave.UserDat.TimeVaild.str.min=4;
//	storageSave.UserDat.TimeVaild.str.sec=5;
//	storageSave.UserDat.TimeVaild.stp.year=0;
//	storageSave.UserDat.TimeVaild.stp.mon=1;
//	storageSave.UserDat.TimeVaild.stp.day=2;
//	storageSave.UserDat.TimeVaild.stp.hour=3;
//	storageSave.UserDat.TimeVaild.stp.min=4;
//	storageSave.UserDat.TimeVaild.stp.sec=5;
//	storageSave.UserDat.TimeVaild.wday = 6;
//	storageSave.UserDat.kindred = 0;
//	storageSave.UserDat.sos =1;
//	HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,&storageSave.UserDat); //隠贋喘薩方象
//	HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST+MSG_FACE_REG_SIZE,&storageSave.UserDat); //隠贋喘薩方象
//	st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
//                                                              MEM_FACE_ALL,
//                                                              MSG_FACE_REG_MEM_ST,
//                                                              MSG_FACE_REG_SIZE); //資函ID

//	HAL_Storage_GetOneUserReg(MSG_FACE_REG_MEM_ST,&storageSave.UserDat);
//	memset(&storageSave.sysDat,1,sizeof(storageSave.sysDat));
//	HAL_Storage_WriteSysData(&storageSave.sysDat);
//	HAL_Storage_ReadSysData(&storageSave.sysDat);
//	memset(&storageSave.UserDat,0,sizeof(storageSave.UserDat));
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //贋壓炎崗
//	storageSave.UserDat.pageID = 5;              //隠贋喘薩ID
//	st_InputInfo.userID = 5;
//                    HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
//                                               MSG_FACE_REG_SIZE,
//                                               st_InputInfo.userID,
//                                               &storageSave.UserDat); //隠贋喘薩方象
//											  
//	if (0 == HAL_Storage_MatchId(0,
//                                             MEM_FACE_ALL,
//                                             MSG_FACE_REG_MEM_ST,
//                                             MSG_FACE_REG_SIZE,
//                                             &storageSave.UserDat))
//                {
//					DEBUG_LOG_OUT("oK\n");
//				}
//				else
//				{
//					DEBUG_LOG_OUT("fail\n");
//				}
//	while(1);
//	while(1)
//	{
//		st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
//                                                              MEM_FACE_ALL,
//                                                              MSG_FACE_REG_MEM_ST,
//                                                              MSG_FACE_REG_SIZE); //資函ID
//                DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //贋壓炎崗
//                                                                      //功象暇汽耶紗議窃侏僉夲砿尖埀珊頁噸宥
//                    storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN;  //砿尖埀
//                    storageSave.UserDat.pageID = 1;              //隠贋喘薩ID
//                    HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
//                                               MSG_FACE_REG_SIZE,
//                                               st_InputInfo.userID,
//                                               &storageSave.UserDat); //隠贋喘薩方象
//	}


//	HAL_Storage_DelNUserReg(0,20,MSG_FACE_REG_MEM_ST,MSG_FACE_REG_SIZE); //賠茅侭嗤議繁然
//	HAL_FaceId_DelAllUserStart();
//	while(1)
//	{
//		DelayMS(1);
//		Menu_Face_Ser vice();
//		HAL_FaceId_DelAllUser();
//	}
	
//	fn_JumpMenu(enum_Menu_AddFaceStep1);                //序秘廣過送殻。
//	HAL_FaceId_AddUserStart();
	
//	HAL_FaceId_VerifyUserStart();
//	fn_JumpMenu(enum_Menu_Desktop);                //彑中
//	fn_JumpMenu(enum_Menu_AddFingerStep1);                //序秘廣過送殻。
//	fn_JumpMenu(enum_Menu_DelFinger);                //序秘廣過送殻。
//	HAL_Finger_RegStart();
//霞編梓囚
//	INT8U tmp =0xff;
//	while(1)
//	{
//		DelayMS(1);
////		DEBUG_LOG_OUT("玉梓 %d\n", HAL_KeySacnGet());
//		tmp = HAL_KeySacnGet();
//		if(tmp != 0xff)
//		{
//			DEBUG_LOG_OUT("玉梓 %d\n", tmp);
//		}
//	}
//	voiceCtrl.voiceDir = eVoiceDirFirstUse;
//	HAL_VoicePlay();
// 	extern void HAL_VoicePlayDir(TYPEe_VoiceDir dat);
//	HAL_VoicePlayDir(eVoiceDirSirensSound_110);
//	HAL_FaceId_CmdPowerOn(); //貧窮
//	HAL_FaceId_VerifyUserStart();
//	fn_JumpMenu(enum_Menu_Desktop);                //彑中
	while(1)
	{
		AppUser_Service();
	}
	
//	DEBUG_LOG_OUT("M_IR_TX_IN_NOPULL();\n");
////	M_IR_TX_IN_IPU();
//	M_IR_TX_IN_NOPULL();
////	M_IR_RX_IN_NOPULL();
//	while(1)
//	{
//		if(0 == M_IR_TX_READIN())
//		{
//			DelayMS(10);
//			if(0 == M_IR_TX_READIN())
//			{
//				DEBUG_LOG_OUT("Have man\n");
//				while(0 == M_IR_TX_READIN());
//			}
//		}
////		if(0 == M_IR_RX_READIN())
////		{
////			DelayMS(10);
////			if(0 == M_IR_RX_READIN())
////			{
////				DEBUG_LOG_OUT("Have man\n");
////				while(0 == M_IR_RX_READIN());
////			}
////		}		
//	}
//	M_BAT_MS_OUT_PP();
//	M_BAT_MS_OUT_1();
	App_Demo();
   
	//契峭殻會怒敬??序秘呟械
	while(1);   
}	

