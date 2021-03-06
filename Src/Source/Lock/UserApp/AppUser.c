/***************************************************************************************
**文件名:     AppUser.c
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
#include "menu.h"
#include "AppUser.h"
#include "HAL_Motor.h"
#include "APP_Power.h"
#include "HAL_SysTimer.h"
#include "HAL_KeyLight.h"
#include "HAL_Comm.h"
#include "HAL_Power.h"

#if  APP_USER_MODULE_EN
#warning APP_USER模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/


/*************************局部函数*****************************/


/*************************全局函数****************************/
/***************************************************************************************
**函数名:       AppUser_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:
****************************************************************************************/
void AppUser_Init(void)
{
    APP_Power_Init();

}
/***************************************************************************************
**函数名:       AppUser_Service
**功能描述:     用户服务程序
**输入参数:     --
**输出参数:     --
**备注:         运行过程不能阻塞.
****************************************************************************************/
void AppUser_Service(void)
{
    while(1)
    {
		
		#if USE_WDT >0
		HAL_WDTFeedDog();
		#endif
        if(0 != HAL_SysTimeDeal())
        {
            return;
        }
//		HAL_BreathingLightDeal();		//呼吸灯处理
        Menu_CycleScan();				//循环执行的菜单
        Menu_KeyDealService();			//按键处理
        Menu_KeyBackLightService();		//背光处理服务
        Menu_Face_Service();			//人脸服务程序
        Menu_FingerService();			//指纹服务
        Menu_AutoMenuReturn();			//用于自动返回菜单
        Menu_VoiceService();			//语音服务
        Menu_Comm_Service(); 		    //通讯服务
        Menu_WifiService();	            //wifi服务
    }
}

#endif  /*APP_USER_MODULE_EN*/
/************************************Code End******************************************/
