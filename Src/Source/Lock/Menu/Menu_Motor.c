/***************************************************************************************
**文件名:     YY.c
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
#include "HAL_Motor.h"
#if MENU_MODULE_EN
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       Menu_Motor_Service
**功能描述:    	电机服务程序
**输入参数: 	--
**输出参数: 
**备注:         
****************************************************************************************/
void Menu_Motor_Service(void)
{
	switch (st_MenuItem.e_CurrentIndex)
	{
	case enum_Menu_Desktop:
		HAL_Motor_Deal();
		break;
	case enum_Menu_PassWdPass:		  //密码验证通过
	case enum_Menu_FingerVerifyOK:
		switch (HAL_Motor_Deal()) //电机处理
		{
		case 1:
			break;
		case 2:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BackDeal);\n");
			fn_JumpMenu(enum_Menu_BackDeal);
			break;
		case 3:

			break;
		default:
			break;
		}
		break;
	case enum_Menu_TryFingerVerifyOK: //试用模式通过
		switch (HAL_Motor_Deal()) //电机处理
		{
		case 1:
			HAL_VoicePlay(eVoiceDirFirstUse,MENU_VOICE_FIRST_USE_WAIT, MENU_VOICE_FIRST_USE_WORK);
			break;
		case 2:
			DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_BackDeal);\n");
			fn_JumpMenu(enum_Menu_BackDeal);
			break;
		case 3:

			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
/***************************************************************************************
**函数名:       --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
/*
void YY_Init(void)
{
    
}
*/
#endif /*XX_MODULE_EN*/
	   /************************************Code End******************************************/