/***************************************************************************************
**文件名:	  Menu.c
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间
**
**备注:
****************************************************************************************/
#include "menu.h"
#if  MENU_MODULE_EN
#warning MENU模块使能!
/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  

/*************************公共全局变量*************************/ 
TYPEs_MenuItem st_MenuItem =
{
    enum_Menu_SystemSleep,
    enum_Menu_SystemSleep,
    M_MENU_DISPLAY_TIMER_LEVEL_2,//2S
};//当前菜单状态
/*************************局部函数*****************************/  
/***************************************************************************************
**函数名:       fn_MenuSwitchToParentIndex
**功能描述:     返回父级菜单
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_MenuSwitchToParentIndex(void)
{
    fn_JumpMenu(struct_MenuTab[st_MenuItem.e_CurrentIndex].e_ParentIndex);//上级菜单
}
/***************************************************************************************
**函数名:       fn_MenuSwitchToFrontIndex
**功能描述:     返回前级菜单
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_MenuSwitchToFrontIndex(void)
{
    fn_JumpMenu(st_MenuItem.e_PreviousIndex);//上级菜单
}
/***************************************************************************************
**函数名:       fn_MenuNum
**功能描述:     菜单Num键处理
**输入参数:     TYPEe_MenuIndex e_JumpMenu
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_MenuNum(INT8U u8_KeyVal)
{
    INT8U u8_KeyIndex;

    switch (struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
    {
    case enum_Window_TopDialog:
    case enum_Window_GeneralDialog:
        fn_GetInputInfo(u8_KeyVal);//获得用户输入并处理
        break;
    case enum_Window_Menu:
//        u8_KeyIndex = u8_KeyVal - '0';
		u8_KeyIndex = u8_KeyVal;
        if (struct_MenuTab[st_MenuItem.e_CurrentIndex].u8_SubMenuCount > 0)
        {
            if (u8_KeyIndex > 0)//菜单索引是从1开始的
            {
                if (struct_MenuTab[st_MenuItem.e_CurrentIndex].u8_SubMenuCount >= u8_KeyIndex)
                {
					DEBUG_LOG_OUT(" fn_JumpMenu(struct_MenuTab[st_MenuItem.e_CurrentIndex].p_SubMenuIndex[u8_KeyIndex-1]);\n");	
                    fn_JumpMenu(struct_MenuTab[st_MenuItem.e_CurrentIndex].p_SubMenuIndex[u8_KeyIndex-1]);
                }
                #ifdef M_GUI_INPUT_ERROR_HINT_ENABLE
                else
                {
                    fn_JumpMenu(enum_Menu_InputError);//弹出提示信息，结束后返回原来的菜单
                }
                #endif /*M_GUI_INPUT_ERROR_HINT_ENABLE*/
            }
            #ifdef M_GUI_INPUT_ERROR_HINT_ENABLE
            else
            {
                fn_JumpMenu(enum_Menu_InputError);//弹出提示信息，结束后返回原来的菜单
            }
            #endif /*M_GUI_INPUT_ERROR_HINT_ENABLE*/
        }
        break;
    case enum_Window_Notify:
        fn_MenuSwitchToParentIndex();//提示框按任意键返回，直接跳转
        fn_GetInputInfo(u8_KeyVal);//获得用户输入并处理
        break;
    case enum_Window_NotifyInfo:
        fn_MenuSwitchToFrontIndex();//返回前次菜单
        break;
    case enum_Window_Special://特别窗口
//		fn_GetInputInfo(u8_KeyVal);//获得用户输入并处理
        break;
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       fn_MenuReturn
**功能描述:     菜单MenuReturn键处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_MenuReturn(INT8U u8_KeyVal)
{
    switch (struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
    {
    case enum_Window_TopDialog:
        fn_GetInputInfo(u8_KeyVal);//密码验证处理
        break;
    case enum_Window_GeneralDialog:
        fn_GetInputInfo(u8_KeyVal);//密码验证处理
        fn_MenuSwitchToParentIndex();
        break;
    case enum_Window_Menu:
        fn_MenuSwitchToParentIndex();//直接跳转到父菜单
        break;
    case enum_Window_Notify:
        fn_MenuSwitchToParentIndex();//直接跳转到父菜单
        break;
    case enum_Window_NotifyInfo:
        fn_MenuSwitchToFrontIndex();//返回前次菜单
        break;
	case enum_Window_OnlyReturn:
		 fn_MenuSwitchToParentIndex();//返回前次菜单
        break;
    case enum_Window_Special:
//		fn_GetInputInfo(u8_KeyVal);//密码验证处理
        break;
	
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       fn_MenuEnter
**功能描述:     菜单Enter键处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_MenuEnter(INT8U u8_KeyVal)
{
    switch (struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
    {
    case enum_Window_TopDialog:
    case enum_Window_GeneralDialog:
        fn_GetInputInfo(u8_KeyVal);//密码验证处理
        break;
    case enum_Window_Menu:
        #ifdef M_GUI_INPUT_ERROR_HINT_ENABLE
        fn_JumpMenu(enum_Menu_InputError);//弹出提示信息，结束后返回原来的菜单
        #endif /*M_GUI_INPUT_ERROR_HINT_ENABLE*/
        break;
    case enum_Window_Notify:
        fn_MenuSwitchToParentIndex();//直接跳转到父菜单
        break;
    case enum_Window_NotifyInfo:
        fn_MenuSwitchToFrontIndex();//返回前次菜单
        break;
    case enum_Window_Special:
		 fn_GetInputInfo(u8_KeyVal);//密码验证处理
        break;
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       fn_KeyForMenu
**功能描述:     其他菜单按键处理
**输入参数:     按键值
**输出参数:     --
**备注:         
****************************************************************************************/
//static void fn_KeyForMenu(INT8U u8_KeyVal)
void Menu_KeyForMenu(INT8U u8_KeyVal)
{
    if (enum_Window_NoResponseNotify == struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
    {
        return;//不响应按键
    }
//	KeyForMenuInset(); 
    switch (u8_KeyVal)//按键处理
    {
    case MENU_KEY_1:
    case MENU_KEY_2:
    case MENU_KEY_3:
    case MENU_KEY_4:
    case MENU_KEY_5:
    case MENU_KEY_6:
    case MENU_KEY_7:
    case MENU_KEY_8:
    case MENU_KEY_9:
    case MENU_KEY_0:
		if(enum_Window_OnlyReturn == struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
		{
			break; //响应返回
		}
        fn_MenuNum(u8_KeyVal);
        break;
    case MENU_KEY_MENU:
        fn_MenuReturn(u8_KeyVal);
        break;
    case MENU_KEY_ENTER:
		if(enum_Window_OnlyReturn == struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
		{
			break; //响应返回
		}
        fn_MenuEnter(u8_KeyVal);
        break;
    default:
        break;//未知的按键
    }
}
/***************************************************************************************
**函数名:       fn_Keyboard_Process
**功能描述:     按键处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
//static void fn_Keyboard_Process(INT8U u8_KeyVal)
//{
//    if (0 == u8_KeyVal)//没有按键？
//    {
//        return;//没有按键直接返回
//    }
//    #ifdef M_SLIP_COVER_ENABLE
//    //    if (0x00 != (st_InputInfo.u8_SlipCover_Status & 0x02))
//    //    {
//    //        return;//没有按键直接返回
//    //    }
//    #endif
//    fn_KeyForMenu(u8_KeyVal);//菜单按键处理
//}
/***************************************************************************************
**函数名:       fn_Keyboard_Process
**功能描述:     按键处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_KeyboardProcess(INT8U u8_KeyVal)
{
//    if (0 == u8_KeyVal)//没有按键？
//    {
//        return;//没有按键直接返回
//    }
    #ifdef M_SLIP_COVER_ENABLE
    //    if (0x00 != (st_InputInfo.u8_SlipCover_Status & 0x02))
    //    {
    //        return;//没有按键直接返回
    //    }
    #endif
//    fn_KeyForMenu(u8_KeyVal);//菜单按键处理
}
/***************************************************************************************
**函数名:       fn_TimeOutMenuReturn
**功能描述:     无操作超时菜单返回处理
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void fn_TimeOutMenuReturn(void)
{
    switch (struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)
    {
    case enum_Window_TopDialog:
		DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_SystemSleep);\n");	
        fn_JumpMenu(enum_Menu_SystemSleep);//父级菜单同本级一样时，返回到系统休眠菜单
        break;
    case enum_Window_GeneralDialog:
    case enum_Window_Notify:
    case enum_Window_NoResponseNotify:
		DEBUG_LOG_OUT("fn_MenuSwitchToParentIndex();\n");	
        fn_MenuSwitchToParentIndex();//当子菜单数为0，对应指针不为NULL时，直接跳转
        break;
    case enum_Window_Menu:
		DEBUG_LOG_OUT("enum_Window_Menu fn_MenuSwitchToFrontIndex\n");
		fn_MenuSwitchToParentIndex();//返回前次菜单
        break;
    case enum_Window_NotifyInfo:
		DEBUG_LOG_OUT("fn_MenuSwitchToFrontIndex();\n");
        fn_MenuSwitchToFrontIndex();//返回前次菜单
        break;
    case enum_Window_Special:
        break;
    default:
        break;
    }
}
/***************************************************************************************
**函数名:       Menu_AutoMenuReturn
**功能描述:     自动菜单返回。当菜单时间用完时，软件按下一次返回菜单键
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_AutoMenuReturn(void)
{
    if (0xFFFF == st_MenuItem.u16_KeepTime)
    {
        return;
    }
	#define RUN_TIME
	static INT32U runTime =M_TREMBLING_PRESS;
	if(runTime > 0)
	{
		runTime --;
		return;
	}
	runTime =M_TREMBLING_PRESS;
	#undef RUN_TIME
    #ifdef MENU_TIME_CHANGE
    if (st_MenuItem.u16_KeepTime > (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK))
    {
        st_MenuItem.u16_KeepTime -= (M_TREMBLING_PRESS * SYSTEM_CLOCK_TICK);
        return;
    }
    else
    {
        fn_TimeOutMenuReturn();
    }
    #endif /*MENU_TIME_NO_CHANGE*/
}
/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       fn_KeyMenuTask
**功能描述:     扫描按键的任务。
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void fn_KeyMenuTask(void)
{
    #define M_CYCLE_TIME            (M_TREMBLING_PRESS)         //50mS
	INT8U u8_KeyValue = 0;									//扫描到的键值
    #ifndef UNIFY_TIME_CNT										//按键任务
    static INT8U u8_CycleTime = 0;
    u8_CycleTime++;
    if (u8_CycleTime <= M_CYCLE_TIME)
    {
        return;
    }
	u8_CycleTime = 0;
    #endif /*UNIFY_TIME_CNT*/
//    u8_KeyValue = fn_KeyPad_Scan();							//按键键值处理 
//    fn_Keyboard_Process(u8_KeyValue);						//按键处理
//    fn_AutoMenuReturn();									//自动菜单返回
//    fn_CycleScanMenu();										//菜单中周期性处理的事件
//    fn_UpdateBackLitht(st_MenuItem.e_CurrentIndex);//刷新背光灯状态
    #undef M_CYCLE_TIME
}

/***************************************************************************************
**函数名:       fn_JumpMenu
**功能描述:     强行跳转菜单。
**输入参数:     TYPEe_MenuIndex e_JumpMenu 跳转到的菜单
**输出参数:     --
**备注:         
****************************************************************************************/
void fn_JumpMenu(TYPEe_MenuIndex e_JumpMenu)
{
    st_MenuItem.e_PreviousIndex = st_MenuItem.e_CurrentIndex;//记录先前的状态
    st_MenuItem.e_CurrentIndex = e_JumpMenu;//取菜单
	st_MenuItem.u16_KeepTime = struct_MenuTab[st_MenuItem.e_CurrentIndex].u16_KeepTime;		//刷新工作时间
    if (st_MenuItem.e_PreviousIndex != st_MenuItem.e_CurrentIndex)
    {
        fn_WhenExitMenuOperation(st_MenuItem.e_PreviousIndex);//退出菜单时执行的操作
        #ifdef M_VOS_GUI_ENABLE
        fn_WhenExitMenu_VosGui(st_MenuItem.e_PreviousIndex, st_MenuItem.e_CurrentIndex);//语音GUI控制
        #endif /*M_VOS_GUI_ENABLE*/
//        fn_Clear_Lcd();//两个不同菜单之间切换前清一次屏
        fn_WhenEnterMenuOperation(st_MenuItem.e_CurrentIndex);//进入菜单时执行的操作
        #ifdef M_VOS_GUI_ENABLE
        fn_WhenEnterMenu_VosGui(st_MenuItem.e_PreviousIndex, st_MenuItem.e_CurrentIndex);//语音GUI控制
        #endif /*M_VOS_GUI_ENABLE*/
        if (enum_Window_NotifyInfo != struct_MenuTab[st_MenuItem.e_CurrentIndex].e_WindowType)//enum_Window_NotifyInfo菜单直接返回前菜单,弹出提示信息。这种菜单是没有父级菜单的
        {
            st_MenuItem.e_PreviousIndex = st_MenuItem.e_CurrentIndex;
        }
    }
    Menu_KeyBackLightUpdate(st_MenuItem.e_CurrentIndex);//刷新背光灯状态
    (*struct_MenuTab[st_MenuItem.e_CurrentIndex].fn_CurrentOperate)();//执行当前的菜单操作
//    st_MenuItem.u16_KeepTime = struct_MenuTab[st_MenuItem.e_CurrentIndex].u16_KeepTime;
//	if (st_MenuItem.e_PreviousIndex != st_MenuItem.e_CurrentIndex) //这里考虑到有阻塞播放语音的情况
//    {
//        #ifdef M_VOS_GUI_ENABLE
//        fn_WhenEnterMenu_VosGui(st_MenuItem.e_PreviousIndex, st_MenuItem.e_CurrentIndex);//语音GUI控制
//        #endif /*M_VOS_GUI_ENABLE*/
//	}
}


/***************************************************************************************
**函数名:       Menu_ClearInputInfo
**功能描述:     清除系统临时变量
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_ClearInputInfo(void)
{
	INT8U i;
    for(i = 0; i < M_PASSWORD_BUF_LENGTH; i++)
    {
//        st_InputInfo.a_u8_PassWordBuf[i] = ' ';      //密码输入存放区
		st_InputInfo.a_u8_PassWordBuf[i] = 0;      //密码输入存放区
    }
	st_InputInfo.u8_PassWordBytes = 0;
//	for(i = 0; i < M_PASSWORD_BUF_LENGTH; i++)
//    {
//        st_InputInfo.a_u8_PassWordBufBak[i] = ' ';      //密码输入存放区备份
//    }
//	st_InputInfo.u8_PassWordBytesBak = 0;
	st_InputInfo.enterProjectModeCnt = 0;
	st_InputInfo.enterProjectModeFlag = 0;	
}
/***************************************************************************************
**函数名:       Menu_AddKeepTime
**功能描述:     增加菜单保持时间
**输入参数:     INT16U tim 增加的时间
**输出参数:     --
**备注:         如果超过了最大时间,以最高时间计算
****************************************************************************************/
void Menu_AddKeepTime(INT16U tim)
{
	INT16U timTmp=0;
	timTmp = 0XFFFE - st_MenuItem.u16_KeepTime;	
	if(tim >= timTmp)
	{
		tim = timTmp;
	}
	st_MenuItem.u16_KeepTime += tim;
	DEBUG_LOG_OUT("st_MenuItem.u16_KeepTime = %d\n",st_MenuItem.u16_KeepTime);	
}
/***************************************************************************************
**函数名:       Menu_KeepTimeRefresh
**功能描述:     菜单时间刷新
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void Menu_KeepTimeRefresh(void)
{
	st_MenuItem.u16_KeepTime =  struct_MenuTab[st_MenuItem.e_CurrentIndex].u16_KeepTime;
	DEBUG_LOG_OUT("st_MenuItem.u16_KeepTime = %d\n",st_MenuItem.u16_KeepTime);	
}

#endif
/************************************Code End******************************************/
