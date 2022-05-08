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



/*—————————————————封装类型及引脚分配——————|
引脚       | QFN32 | QFN40 | QFN48 | QFN88 |
———————————————————————————————————————————|
SPI        |   2   |   1   |   3   |   3   |
———————————————————————————————————————————|
I2C        |   1   |   1   |   1   |   3   |   
———————————————————————————————————————————|
SSI        |   0   |   1   |   0   |   1   |
———————————————————————————————————————————|
UART       |   1   |   1   |   3   |   3   |
———————————————————————————————————————————|
ISO7816    |   2   |   0   |   0   |   1   |
———————————————————————————————————————————|
USB_OTG    |   1   |   1   |   1   |   1   |
———————————————————————————————————————————|
ADC        |   0   |   1   |   2   |   3   |
———————————————————————————————————————————|
DAC        |   0   |   0   |   1   |   1   |
———————————————————————————————————————————|
PWM(复用)  |   3   |   4   |   2   |   4   |
———————————————————————————————————————————|
MCC        |   0   |   0   |   0   |   0   |
———————————————————————————————————————————|
TSI(复用)  |   8   |   5   |  10   |  16   |
———————————————————————————————————————————|
EPORT(复用)|  10   |  18   |  19   |  20   |
———————————————————————————————————————————|
SDIO(防拆) |   0   |   0   |   0   |   1   |
———————————————————————————————————————————|
C0 SUB I/O |   0   |   0   |   0   |   4   |
———————————————————————————————————————————|*/

/*———————————————USB 支持情况——————————————|
封装   | USB2.0 | USB1.1 |  host  | device |
———————————————————————————————————————————|
QFN32  | 不可以 |  可以  | 不支持 |  支持  |
———————————————————————————————————————————|
QFN40  |  可以  |  可以  | 不支持 |  支持  |
———————————————————————————————————————————|
QFN48  | 不可以 |  可以  | 不支持 |  支持  |
———————————————————————————————————————————|
QFN88  | 不可以 |  可以  | 不支持 |  支持  |
———————————————————————————————————————————|*/

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
    //模数转换，支持封装：QFN40、QFN48、QFN88、BGA121
	#ifdef ADC_DEMO
		ADC_Demo();
	#endif
	
    //算法: SM2/SM3/SM4/SHA/DES/3DES/AES/RSA， 支持封装：ALL
	#ifdef ALGO_DEMO
		Algo_Demo();
	#endif	
	//USI(7816)slave demo,支持封装：QFN32、QFN88、BGA121
	#ifdef APDU_DEMO	
		APDU_Demo();
	#endif
	
    //USI(7816)master demo,支持封装：QFN32、QFN88、BGA121
	#ifdef BCTC_DEMO	
		BCTC_Test_Demo();
	#endif	
	
    //蓝牙，支持封装：QFN48、QFN88、BGA121
	#ifdef BT_DEMO
		BT_Demo();
	#endif	
	
    //时钟电源模块，支持封装：ALL
	#ifdef CPM_DEMO
		CPM_Demo();
	#endif
	
    //芯片配置模块，支持封装：ALL
	#ifdef CCM_DEMO
		CCM_Demo();
	#endif
	
    //DAC语音demo，支持封装：QFN48、QFN88、BGA121
	#ifdef DAC_AUDIO_DEMO
//		DAC_Audio_Demo();
//	DAC_Audio_Demo(C_AUDIO_0);
	#endif
	
    //数模转换，支持封装：QFN48、QFN88、BGA121
	#ifdef DAC_DEMO
		DAC_Demo();
	#endif
	
    //禁止ROM boot启动，支持封装：ALL
	#ifdef DISBOOT_DEMO
		Disboot_Demo();
	#endif	
	
    //DMA，支持封装：ALL
	#ifdef DMA_DEMO
		DMA_Demo();
	#endif
	
    //浮点运算，支持封装：ALL
	#ifdef DSP_DEMO
		DSP_Demo();
	#endif
	
    //EDMA，支持封装：ALL
	#ifdef EDMA_DEMO
		EDMA_Demo();
	#endif
	
    //EFLASH，支持封装：ALL
	#ifdef EFLASH_DEMO
		EFlash_Demo();
	#endif	
	
    //具有边沿触发中断的GPIO，支持封装：ALL
	#ifdef EPORT_DEMO
		Eport_Demo();
	#endif
	
    //获取芯片唯一序列号，支持封装：ALL
	#ifdef GETSN_DEMO
	  GetSN_Demo();
	#endif
	
    //I2C，支持封装：ALL
	#ifdef I2C_DEMO
		I2C_Demo();
	#endif
	
    //禁止仿真，执行后芯片将不能仿真，支持封装：ALL
	#ifdef LOCK_DISJTAG_DEMO
		Lock_DisJtag_Demo();
	#endif
	
    //非接应用，支持封装：
	#ifdef NFC_DEMO
		NFC_Demo();
	#endif
	
    //防拆，最多支持8路静态，4路动态，支持封装：QFN88、BGA121
	#ifdef PCI_DEMO
		PCI_Demo();
	#endif
	
    //32bits 可编程中断定时器，支持封装：ALL
	#ifdef PIT32_DEMO
		PIT32_Demo();
	#endif
	
    //PWM，支持封装：ALL
	#ifdef PWM_DEMO
		PWM_Demo();
	#endif

    //复位，支持封装：ALL
	#ifdef RESET_DEMO
		Reset_Demo();
	#endif

    //实时时钟，支持封装：QFN48、QFN88、BGA121
	#ifdef RTC_DEMO
//	extern void RTC_TIME_TEST();
//	RTC_TIME_TEST();
		RTC_Demo();
//RTC_TIME_TEST();
	#endif
	
    //时间戳，支持封装：QFN48、QFN88、BGA121
	#ifdef RTC_TIMESTAMP_DEMO
		RTC_TimeStamp_Demo();
	#endif

    //SPI，支持封装：ALL
	#ifdef SPI_DEMO
		SPI_Demo();
	#endif
	
    //高速SPI，支持单线(普通SPI)、双线和四线，只支持主模式，支持封装：QFN40、QFN48(单线和双线)、QFN88、BGA121
	#ifdef SSI_DEMO
	   SSI_Demo();
	#endif
	
    //C0子系统例程，代码从M4主系统搬到C0子系统运行，支持封装：QFN88、BGA121
	#ifdef SUBC0_DEMO
		Sub_C0_Demo();
	#endif
	
    //M4内核时钟，支持封装：ALL
	#ifdef SYSTICK_DEMO
		SysTick_Demo();
	#endif
	
    //定时计数器 1路，支持封装：ALL
	#ifdef TC_DEMO
		TC_Demo();
	#endif
	
    //触控，支持封装：ALL
	#ifdef TSI_DEMO
		TSI_Demo();
	#endif
	
    //串口，支持封装：ALL
	#ifdef UART_DEMO
		UART_Demo();
	#endif
	
    //USB，支持封装：ALL
	#ifdef USB_DEMO
		USB_Demo();
	#endif

    //看门狗，支持封装：ALL
	#ifdef WDT_DEMO
		WDT_Demo();
	#endif
	
    //SSI接口，SPI Flash运行代码，只支持SSI1，支持封装：QFN40、QFN88、BGA121
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
//	/*系统配置，获取时钟相关参数等 */
//	Sys_Init(); 
//	Sub_C0_Init(); //C0初始化
//	App_Demo();
//	Disboot_Demo();
	
//	SEGGER_RTT_Init();
//	DEBUG_LOG_OUT("System powerup ok！\n");
//	DEBUG_LOG_OUT("人脸信息设置请按1\n");
	
	
//	DEBUG_LOG_OUT_HEX(test,5);
//	do                                           
//  {                                            
//    uint32_t i;                                
//    for (i = 0; i < 5; i++)                  
//    {                                          
//      SEGGER_RTT_printf(0,"%x ", test[i]); 
//    }                                          
//  } while (0);
//	//回到ROM BOOT模式
//	Reback_Boot();

	/*配置串口作为debug用，时钟根据不同环境配置不同的时钟参数*/
//	UART_Debug_Init(SCI1,g_ips_clk, 115200);
//HAL_Comm_Init();
//UART_Debug_Init(SCI1,g_ips_clk, 57600);
//HAL_Comm_Send(eCommPowerOnSync);
//	DEBUG_LOG_OUT("System powerup ok!\n");
    //UART打印版本信息
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
//	M_BELL_OUT_1(); //开启后板的门铃
//HAL_FaceId_ClosePow();
//DelayMS(1500);
//	HAL_FaceId_Init();
//	DelayMS(5000);
//	msgInitData.enableNetwork = 0; //不联网
//	msgInitData.mode = 0; //明文
//	HAL_FaceId_CmdSend(0x25);
//HAL_FaceId_Init();    //初始化串口
//	HAL_FaceId_CmdPowerOn(); //上电
//	HAL_FaceId_AddUserStart(); //开始添加流程
//	M_FINGER_IRQ_IN_IPD(); //下拉
////	M_FINGER_IRQ_EXIT_LOTOHI_ENABLE(); //上升沿中断
//	M_X_IRQ_9555_IN_NOPULL();
//	M_X_IRQ_9555_EXIT_HITOLO_ENABLE(); //下降沿中断
//	M_REUSE_IRQ_IN_NOPULL();
//	M_REUSE_IRQ_EXIT_LOTOHI_ENABLE(); //上升沿触发
//	M_CC4202_INT_IN_IPD();
//	M_CC4202_INT_EXIT_LOTOHI_ENABLE(); //上升沿触发
//	HAL_FaceId_CmdSend(0x25);
//	while(1)
//	{
//		DelayMS(1);
//		HAL_FaceId_PowerOnDeal();
//		HAL_FaceId_RevcDeal();
//		HAL_FaceId_AddUser(); //添加流程
//	}
//MRI_Sensor_Demo();
//	//执行Demo程序
	//

//	DRV_SysTimerInit(); //开启系统时钟1ms调用一次
	AppUser_Init();
//	st_InputInfo.FingerIdTmp =0;
//				memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
//        storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG;                           //存在标志
//        storageSave.UserDat.pageID = st_InputInfo.FingerIdTmp;						//保存ID
//        HAL_Storage_SaveOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                   MSG_FINGER_ONE_SIZE,
//                                   st_InputInfo.FingerIdTmp,
//                                   &storageSave.UserDat); //保存指纹信息
//								   
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);
//		DEBUG_LOG_OUT("MSG_FINGER_REG_MEM_LOCAL_ST:%x\n",MSG_FINGER_REG_MEM_LOCAL_ST);
//		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
//        HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                  MSG_FINGER_ONE_SIZE,
//                                  st_InputInfo.FingerIdTmp,
//                                  &storageSave.UserDat);       //读取参数	
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);							
//	st_InputInfo.FingerIdTmp = HAL_Storage_SearchEmpty(0,
//														   MEM_FINGER_ALL_LOCAL,
//														   MSG_FINGER_REG_MEM_LOCAL_ST,
//														   MSG_FINGER_ONE_SIZE); //获取空位
//																				 //没有空位的时候，不会进入到这里
//		DEBUG_LOG_OUT("Menu_Comm_BleNewAddFinger st_InputInfo.FingerIdTmp:%d\n", st_InputInfo.FingerIdTmp);

//		DelayMS(100);
//		DEBUG_LOG_OUT("storageSave.UserDat.vaild:%x\n",storageSave.UserDat.vaild);
//		DEBUG_LOG_OUT("storageSave.UserDat.pageID:%x\n",storageSave.UserDat.pageID);
//		DEBUG_LOG_OUT("MSG_FINGER_REG_MEM_LOCAL_ST:%x\n",MSG_FINGER_REG_MEM_LOCAL_ST);
//		memset((uint16_t *)(&storageSave.UserDat), 0, sizeof(storageSave.UserDat)); //清空参数
//        HAL_Storage_GetOneUserReg(MSG_FINGER_REG_MEM_LOCAL_ST,
//                                  MSG_FINGER_ONE_SIZE,
//                                  st_InputInfo.FingerIdTmp,
//                                  &storageSave.UserDat);       //读取参数
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
	
/**********************存储读写***************************/
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
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
//					//根据菜单添加的类型选择管理员还是普通
//	storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN; //管理员
//	storageSave.UserDat.pageID = 4; //保存数据

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
//	HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,&storageSave.UserDat); //保存用户数据
//	HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST+MSG_FACE_REG_SIZE,&storageSave.UserDat); //保存用户数据
//	st_InputInfo.userID = HAL_Storage_SearchEmpty(0,
//                                                              MEM_FACE_ALL,
//                                                              MSG_FACE_REG_MEM_ST,
//                                                              MSG_FACE_REG_SIZE); //获取ID

//	HAL_Storage_GetOneUserReg(MSG_FACE_REG_MEM_ST,&storageSave.UserDat);
//	memset(&storageSave.sysDat,1,sizeof(storageSave.sysDat));
//	HAL_Storage_WriteSysData(&storageSave.sysDat);
//	HAL_Storage_ReadSysData(&storageSave.sysDat);
//	memset(&storageSave.UserDat,0,sizeof(storageSave.UserDat));
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
//	storageSave.UserDat.pageID = 5;              //保存用户ID
//	st_InputInfo.userID = 5;
//                    HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
//                                               MSG_FACE_REG_SIZE,
//                                               st_InputInfo.userID,
//                                               &storageSave.UserDat); //保存用户数据
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
//                                                              MSG_FACE_REG_SIZE); //获取ID
//                DEBUG_LOG_OUT("st_InputInfo.userID:%d\n", st_InputInfo.userID);
//	storageSave.UserDat.vaild = MSG_FINGERREGMEM_FIG; //存在标志
//                                                                      //根据菜单添加的类型选择管理员还是普通
//                    storageSave.UserDat.Lim = MSG_FACTMEM_LIM_ADMIN;  //管理员
//                    storageSave.UserDat.pageID = 1;              //保存用户ID
//                    HAL_Storage_SaveOneUserReg(MSG_FACE_REG_MEM_ST,
//                                               MSG_FACE_REG_SIZE,
//                                               st_InputInfo.userID,
//                                               &storageSave.UserDat); //保存用户数据
//	}


//	HAL_Storage_DelNUserReg(0,20,MSG_FACE_REG_MEM_ST,MSG_FACE_REG_SIZE); //清除所有的人脸
//	HAL_FaceId_DelAllUserStart();
//	while(1)
//	{
//		DelayMS(1);
//		Menu_Face_Ser vice();
//		HAL_FaceId_DelAllUser();
//	}
	
//	fn_JumpMenu(enum_Menu_AddFaceStep1);                //进入注册流程。
//	HAL_FaceId_AddUserStart();
	
//	HAL_FaceId_VerifyUserStart();
//	fn_JumpMenu(enum_Menu_Desktop);                //桌面
//	fn_JumpMenu(enum_Menu_AddFingerStep1);                //进入注册流程。
//	fn_JumpMenu(enum_Menu_DelFinger);                //进入注册流程。
//	HAL_Finger_RegStart();
//测试按键
//	INT8U tmp =0xff;
//	while(1)
//	{
//		DelayMS(1);
////		DEBUG_LOG_OUT("短按 %d\n", HAL_KeySacnGet());
//		tmp = HAL_KeySacnGet();
//		if(tmp != 0xff)
//		{
//			DEBUG_LOG_OUT("短按 %d\n", tmp);
//		}
//	}
//	voiceCtrl.voiceDir = eVoiceDirFirstUse;
//	HAL_VoicePlay();
// 	extern void HAL_VoicePlayDir(TYPEe_VoiceDir dat);
//	HAL_VoicePlayDir(eVoiceDirSirensSound_110);
//	HAL_FaceId_CmdPowerOn(); //上电
//	HAL_FaceId_VerifyUserStart();
//	fn_JumpMenu(enum_Menu_Desktop);                //桌面
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
   
	//防止程序跑飞，进入异常
	while(1);   
}	

