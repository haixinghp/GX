/***************************************************************************************
**文件名:     Menu_GetInputInfo.c
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
#include "Maths.h"
#include "Menu_CurrentOperate.h"
#include "HAL_Storage.h"
#include "Function.h"
#include "HAL_KeyLight.h"
#include "HAL_FaceId.h"
#include "HAL_Voice.h"
#if MENU_MODULE_EN
/*******************************宏定义***************************/
#define PROJERT_MODE_CODE \
    {                     \
        6, 6, 8, 8        \
    } //开启工程模式
#define TEST_MODE_CODE      \
    {                       \
        2, 5, 8, 7, 4, 5, 1 \
    } //开启测试模式
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/
static const INT8U projertModeCode[] = PROJERT_MODE_CODE;
static const INT8U testModeCode[] = TEST_MODE_CODE;

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       EnterProjectModeCheck
**功能描述:     工程模式暗码校验
**输入参数:     
INT8U *cmd,  命令
INT8U len,   命令长度
INT8U *cnt   命令计数器  
INT8U dat,   数据指针
**输出参数:     
INT8S
0,未检测到有效的指令
1,检测到有效指令
**备注:         
****************************************************************************************/
static INT8S EnterProjectModeCheck(INT8U const *cmd, INT8U len, INT8U *cnt, INT8U dat)
{
    INT8S reTmp = 0; //未检测到有效的指令
    INT8U cmdLen = len;
    if ((*cnt) > cmdLen)
    {
        (*cnt) = 0;
    }
    else
    {
        if (cmd[(*cnt)] == dat)
        {
            DEBUG_LOG_OUT("st_InputInfo.EnterProjectModeCnt = %d\n", (*cnt));
            (*cnt)++;
            if ((*cnt) == cmdLen)
            {
                reTmp = 1; //检测到有效指令
            }
        }
        else
        {
            (*cnt) = 0;
        }
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       EnterProjectModeCheck
**功能描述:     工程模式暗码校验
**输入参数:     
INT8U *data,    数据指针
INT8U len       数据长度
**输出参数:     
INT8S
0,校验正确
-1,参数错误
-2,暗码校验错误
**备注:         
****************************************************************************************/
static INT8S EnterProjectModeCheckA(INT8U *data, INT8U len)
{
    INT8U codeLen = sizeof(projertModeCode) - 1; //获取工程模式暗码长度
    INT8U i;
    if (NULL == data)
    {
        return -1; //参数错误
    }
    if (len < 1)
    {
        return -1; //参数错误
    }
    for (i = 0; i < codeLen; i++) //倒叙查询
    {
        if (data[len - 1 - i] != projertModeCode[codeLen - 1 - i])
        {
            return -2; //暗码校验错误
        }
    }
    return 0; //校验正确
}
///***************************************************************************************
//**函数名:       ChkPwd
//**功能描述:     校验密码
//**输入参数:     --
//**输出参数:
//INT8S
//0; //密码正确
//-1; //密码错误
//**备注:
//****************************************************************************************/
//static INT8S ChkPwd(void)
//{

//    //校对密码
//    INT8U pwdBuf[M_PASSWORD_BUF_LENGTH] = {
//        '\0',
//    };
//    memcpy(pwdBuf,
//           storageSave.sysDat.pwd,
//           M_PASSWORD_MAX_LEN);        //读取密码
//    pwdBuf[M_PASSWORD_MAX_LEN] = '\0'; //设定密码长度
//	#ifdef PWD_TEST_DEBUG
//	DEBUG_LOG_OUT("ChkPwd:\n");
//	DEBUG_LOG_OUT("ChkPwd A:\n");
//	DEBUG_LOG_OUT_HEX(storageSave.sysDat.pwd,sizeof(storageSave.sysDat.pwd));
//	DEBUG_LOG_OUT("ChkPwd B:\n");
//	DEBUG_LOG_OUT_HEX(st_InputInfo.a_u8_PassWordBuf,sizeof(st_InputInfo.a_u8_PassWordBuf));
//	#endif
//    if (NULL != strstr((const char *)(st_InputInfo.a_u8_PassWordBuf), (const char *)(pwdBuf)))
//    {
//        return 0; //密码正确
//    }
//    return -1; //密码错误
//}
/***************************************************************************************
**函数名:       ChkPwd
**功能描述:     校验密码
**输入参数:     --
**输出参数:     
INT8S
0; //正确
-1; //错误     
**备注: 
****************************************************************************************/
//static INT8S ChkPwd(uint8_t *savePwd, uint8_t savePwdLen, uint8_t *inputPwd, uint8_t inputPwdLen)
INT8S ChkPwd(uint8_t *savePwd, uint8_t savePwdLen, uint8_t *inputPwd, uint8_t inputPwdLen)
{
	char *pTmp = NULL;
	int32_t i;
	#ifdef PWD_TEST_DEBUG
    DEBUG_LOG_OUT("ChkPwd:\n");
    DEBUG_LOG_OUT("ChkPwd A:\n");
    DEBUG_LOG_OUT_HEX(savePwd, savePwdLen);
    DEBUG_LOG_OUT("ChkPwd B:\n");
    DEBUG_LOG_OUT_HEX(inputPwd, inputPwdLen);
	#endif
	if(savePwdLen > M_PASSWORD_BUF_LENGTH)
	{
		return -1;
	}
	for(i=0;i<M_PASSWORD_MAX_LEN;i++)
    {
		if(((*(savePwd+i)) < '0') || ((*(savePwd+i)) > '9') )
		{
			return -1;
		}
    } //保证数据在'0'~'9'之间
	if(inputPwdLen > M_PASSWORD_BUF_LENGTH)
	{
		return -1;
	}
	for(i=0;i<M_PASSWORD_MAX_LEN;i++)
    {
		if(((*(inputPwd+i)) < '0') || ((*(inputPwd+i)) > '9') )
		{
			return -1;
		}
    } //保证数据在'0'~'9'之间
	DEBUG_LOG_OUT("ChkPwd data Ok\n");
	*(savePwd+savePwdLen) = '\0'; //末尾加停止符
	*(inputPwd+inputPwdLen) = '\0'; //末尾加停止符
	pTmp = strstr((const char *)(inputPwd), (const char *)(savePwd));
	DEBUG_LOG_OUT("ptmp:%d\n",*pTmp);
    if (NULL != pTmp)
    {
        return 0; //密码正确
    }
    return -1; //密码错误
}
/***************************************************************************************
**函数名:       ChkPwdNormal
**功能描述:     校验常规密码
**输入参数:     --
**输出参数:     
INT8S
0; //密码正确
-1; //密码错误     
**备注: 
****************************************************************************************/
static INT8S ChkPwdNormal(void)
{
    if (0 == ChkPwd(storageSave.sysDat.pwd,
                    M_PASSWORD_MAX_LEN,
                    st_InputInfo.a_u8_PassWordBuf,
                    sizeof(st_InputInfo.a_u8_PassWordBuf))) //校对密码
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
/***************************************************************************************
**函数名:       ChkPwdTmp
**功能描述:     校验临时密码
**输入参数:     --
**输出参数:     
INT8S
0; //密码正确
-1; //密码错误     
**备注: 
****************************************************************************************/
static INT8S ChkPwdTmp(void)
{
    uint32_t i = 0;
    for (i = 0; i < HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE; i++)
    {
        if (0 == ChkPwd(&storageSave.sysDat.tempPwd[i][0],
                        M_PASSWORD_MAX_LEN,
                        st_InputInfo.a_u8_PassWordBuf,
                        sizeof(st_InputInfo.a_u8_PassWordBuf)))
        {
            memset(&storageSave.sysDat.tempPwd[i][0], 0XFF, HAL_STORAGE_SYS_DATA_PWD_SIZE); //清空临时密码
            HAL_Storage_WriteSysData(&storageSave.sysDat);                                  //保存数据
            return 0;
        }
    }
    return -1;
}

/***************************************************************************************
**函数名:       ChkPwdSOS
**功能描述:     校验sos密码
**输入参数:     --
**输出参数:     
INT8S
0; //密码正确
-1; //密码错误     
**备注: 
****************************************************************************************/
static INT8S ChkPwdSOS(void)
{
    if (0 == ChkPwd(storageSave.sysDat.pwdSos,
                    M_PASSWORD_MAX_LEN,
                    st_InputInfo.a_u8_PassWordBuf,
                    sizeof(st_InputInfo.a_u8_PassWordBuf))) //校对密码
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/***************************************************************************************
**函数名:       ChkTestMode
**功能描述:     校验测试模式
**输入参数:     --
**输出参数:     
INT8S
0; //正确
-1; //错误     
**备注: 
****************************************************************************************/
static INT8S ChkTestMode(void)
{
    INT8U pwdBuf[] = "2587451";
    if (st_InputInfo.u8_PassWordBytes != (sizeof(pwdBuf) - 1)) //检查长度
    {
        return -1;
    }
    if (NULL != strstr((const char *)(st_InputInfo.a_u8_PassWordBuf), (const char *)(pwdBuf))) //检查数据
    {
        return 0; //正确
    }
    return -1; //错误
}
/***************************************************************************************
**函数名:       DesktopKeyEnter
**功能描述:     桌面执行的程序 按下ENTER键车操作
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void DesktopKeyEnter(void)
{
	#if 0
	storageSave.sysDat.localAdminRegFlag =MSG_FACTMEM_REG_STATUS_OK;
	#endif
    if (MSG_FACTMEM_REG_STATUS_OK == storageSave.sysDat.localAdminRegFlag) //检查是否注册了管理员
    {
        if (st_InputInfo.u8_PassWordBytes >= M_PASSWORD_MAX_LEN)
        {
            if (0 == ChkTestMode()) //2587451 指令
            {
				Menu_Face_PowerOnStart(0); //给人脸模块上电
                fn_JumpMenu(enum_Menu_TestError); //错误检测
                DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_TestError);\n");
                return;
            }
            if (0 == ChkPwdNormal()) //校对密码
            {
				wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
                wifiSendParm.limt = 0; 
                if (eCombineTypeAND == Menu_CheckCombine()) //与模式
                {
                    wifiSendParm.pageId[2] = 0;
                    wifiSendParm.pageId[3] = 0;
                }
                else
                {
                    wifiSendParm.unlockType = eWifiunlockTypePwd;
                    memset(wifiSendParm.pageId,0,sizeof(wifiSendParm.pageId));
                    
                }
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据                
                Menu_RecordLockLog(eLogTypeOpenByPwd, NULL, 0); //保存事件记录
                fn_JumpMenu(enum_Menu_PassWdPass);              //密码验证通过
                DEBUG_LOG_OUT("ChkPwdNormal fn_JumpMenu(enum_Menu_PassWdPass); \n");
                return; //直接出去
            }
            if (0 == ChkPwdTmp()) //校对临时密码。
            {
                wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
                wifiSendParm.limt = 0; 
                if (eCombineTypeAND == Menu_CheckCombine()) //与模式
                {
                    wifiSendParm.pageId[2] = 0;
                    wifiSendParm.pageId[3] = 0;
                }
                else
                {
                    wifiSendParm.unlockType = eWifiunlockTypePwd;
                    memset(wifiSendParm.pageId,0,sizeof(wifiSendParm.pageId)); 
                }
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据
                
                Menu_RecordLockLog(eLogTypeOpenByPwd, NULL, 0); //保存事件记录
                fn_JumpMenu(enum_Menu_PassWdPass);              //密码验证通过
                DEBUG_LOG_OUT("ChkPwdTmp fn_JumpMenu(enum_Menu_PassWdPass); \n");
                return; //直接出去
            }  
            if(0 == ChkPwdSOS())//校对sos密码
            {
				wifiSendParm.sendType = eWifiSendTypeUnlockUpdata;
                if (eCombineTypeAND == Menu_CheckCombine()) //与模式
                {
                    wifiSendParm.pageId[2] |= MENU_UNLOCK_ATTR_SOS;
                    wifiSendParm.pageId[3] = 0;
                }
                else
                {
                    memset(wifiSendParm.pageId,0,sizeof(wifiSendParm.pageId));
                }
                wifiSendParm.limt |= MENU_UNLOCK_ATTR_SOS;  //填入紧急功能
				wifiSendParm.unlockType = eWifiunlockTypePwd;
                memset(wifiSendParm.pageId,0,sizeof(wifiSendParm.pageId));
                wifiSendParm.wifiSendStatus = eWifiSendStatusReady; //开始发送数据       
                fn_JumpMenu(enum_Menu_PassWdPass);              //密码验证通过
                DEBUG_LOG_OUT("ChkPwdSOS ok \n");
                return;
            }
            fn_JumpMenu(enum_Menu_PassWdFail); //密码验证失败
            DEBUG_LOG_OUT(" fn_JumpMenu(enum_Menu_PassWdFail); \n");
        }
    }
}
/***************************************************************************************
**函数名:       GetInputInfoMenuDesktopFun
**功能描述:     桌面执行的程序 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void GetInputInfoMenuDesktopFun(INT8U u8_KeyVal)
{
    INT8U u8_i;
    switch (u8_KeyVal) //按键处理
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
#if defined(KEY_RANDOM_LOCK_EN)
        if (eKeyRandomLockFlagEnable == st_InputInfo.keyRandomLockFlag) //按键有锁
        {
            if (u8_KeyVal == st_InputInfo.keyRandom[0])
            {
                st_InputInfo.keyRandom[0] = KEY_RANDOM_PRESS;
                HAL_VoicePlayDir(eVoiceDirButtonDrip); //按键音处理
                Menu_KeepTimeRefresh();                //工作时间刷新
            }
            if (u8_KeyVal == st_InputInfo.keyRandom[1])
            {
                st_InputInfo.keyRandom[1] = KEY_RANDOM_PRESS;
                HAL_VoicePlayDir(eVoiceDirButtonDrip); //按键音处理
                Menu_KeepTimeRefresh();                //工作时间刷新
            }
            Function_KeyRandomLight();                                                                              //更新灯光
            if ((KEY_RANDOM_PRESS == st_InputInfo.keyRandom[0]) && (KEY_RANDOM_PRESS == st_InputInfo.keyRandom[1])) //所有的随即键被按下
            {
                st_InputInfo.keyRandomLockFlag = eKeyRandomLockFlagDisable; //清除锁定状态
                HAL_KeyLightOnce(HAL_KEY_BACK_LIGHT_ALL_ON);                //点亮全部键盘
                DEBUG_LOG_OUT("Menu_Face_PowerOffStart KEY_RANDOM_PRESS == st_InputInfo.keyRandom[0])\n");
				Menu_Face_PowerOffStart(); //人脸下电
				HAL_FaceId_VerifyUserStop();                                //关闭人脸模块
                Menu_KeepTimeRefresh();                //工作时间刷新
            }
        }
        else
        {
			//屏幕显示
            if (st_InputInfo.u8_CommandBytes < M_COMMAND_BUF_LENGTH) //指令处理
            {
                st_InputInfo.a_u8_CommandBuf[st_InputInfo.u8_CommandBytes++] = u8_KeyVal + '0';
            }
            st_InputInfo.enterProjectModeFlag = EnterProjectModeCheck(projertModeCode,
                                                                      sizeof(projertModeCode),
                                                                      &st_InputInfo.enterProjectModeCnt,
                                                                      (u8_KeyVal + '0'));
			if((eCombineTypeAND == Menu_CheckCombine()) && 
			   (0 == st_InputInfo.faceVerifySuccessFlag)) //在与模式下，人脸还未验证完成，不进行密码校验
			{
				return ;
			}
			#if 0
			DEBUG_LOG_OUT("enter pwd\n");
			#endif
            if (st_InputInfo.u8_PassWordBytes < (M_PASSWORD_BUF_LENGTH - 1))
            {
                st_InputInfo.a_u8_PassWordBuf[st_InputInfo.u8_PassWordBytes++] = u8_KeyVal + '0';
            }
            
        }
#endif //KEY_RANDOM_LOCK_EN
        break;
    case MENU_KEY_MENU:
		DEBUG_LOG_OUT("Menu_Face_PowerOffStart MENU_KEY_MENU\n");
		Menu_Face_PowerOffStart(); //人脸下电
        HAL_FaceId_VerifyUserStop();
#if defined(KEY_RANDOM_LOCK_EN)
        if (eKeyRandomLockFlagEnable == st_InputInfo.keyRandomLockFlag) //按键处于锁定状态
        {
            break;
        }
#endif //KEY_RANDOM_LOCK_EN                    
        break;
    case MENU_KEY_ENTER:
#if defined(KEY_RANDOM_LOCK_EN)
        if (eKeyRandomLockFlagEnable == st_InputInfo.keyRandomLockFlag) //按键处于锁定状态
        {
            break;
        }
#endif //KEY_RANDOM_LOCK_EN
        DesktopKeyEnter();
        break;
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       GetInputInfoMenuModPwdFun
**功能描述:     修改密码 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void GetInputInfoMenuModPwdFun(INT8U u8_KeyVal)
{
    INT8U u8_i;
    switch (u8_KeyVal) //按键处理
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
        if (st_InputInfo.u8_PassWordBytes < M_PASSWORD_MAX_LEN) //只获取输入的前面6位密码
        {
            st_InputInfo.a_u8_PassWordBuf[st_InputInfo.u8_PassWordBytes++] = u8_KeyVal + '0';
        }
        break;
    case MENU_KEY_MENU:
        //退出
        break;
    case MENU_KEY_ENTER:
        if (st_InputInfo.u8_PassWordBytes >= M_PASSWORD_MAX_LEN) //校对输入数量
        {
            memcpy(st_InputInfo.a_u8_PassWordBufBak,
                   st_InputInfo.a_u8_PassWordBuf,
                   M_PASSWORD_MAX_LEN);                                       //备份密码
            st_InputInfo.u8_PassWordBytesBak = st_InputInfo.u8_PassWordBytes; //备份输入长度
            st_InputInfo.u8_PassWordBytes = 0;                                //清除密码长度
            memset(st_InputInfo.a_u8_PassWordBuf,
                   M_PASSWORD_DEFAULT_NONE,
                   M_PASSWORD_BUF_LENGTH);           //进入之前，需要清除输入缓存.
            fn_JumpMenu(enum_Menu_ModPwdInputAgain); //密码再次输入
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ModPwdInputAgain);\n");
        }
        else
        {
            //暂时不处理
        }
        break;
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       ChkPwdIsTooSimple
**功能描述:     检查密码是否过于简单
**输入参数:     
INT8U *dat, //数据指针
INT8U len //数据长度
**输出参数:     
INT8S 
-1; //过于简单
0; //符合标准
**备注:   
认为简单密码的标准如下：   
重复的密码:555555
连续的密码:123456,765432  
****************************************************************************************/
static INT8S ChkPwdIsTooSimple(const INT8U *dat, INT8U len)
{
    INT16U i;
    for (i = 0; i < (len - 1); i++) //重复的密码
    {
        if (*(dat) != (*(dat + i)))
        {
            break;
        }
    }
    if (i == (len - 1)) //所有的数据都相同
    {
        return -1; //过于简单
    }
    for (i = 0; i < (len - 1); i++) //连续的密码升序:如123456
    {
        if (*(dat + i) != (*(dat + i + 1) + 1))
        {
            break;
        }
    }
    if (i == (len - 1))
    {
        return -1; //过于简单
    }
    for (i = 0; i < (len - 1); i++) //连续的密码降序:如54321
    {
        if (*(dat + i) != (*(dat + i + 1) - 1))
        {
            break;
        }
    }
    if (i == (len - 1))
    {
        return -1; //过于简单
    }
    return 0; //符合标准
}
/***************************************************************************************
**函数名:       GetInputInfoMenuModPwdInputAgainFun
**功能描述:     修改密码再次输入 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void GetInputInfoMenuModPwdInputAgainFun(INT8U u8_KeyVal)
{
    INT8U u8_i;
    INT8S Tmp = 0;
    switch (u8_KeyVal) //按键处理
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
        if (st_InputInfo.u8_PassWordBytes < M_PASSWORD_MAX_LEN) //只获取输入的前面6位密码
        {
            st_InputInfo.a_u8_PassWordBuf[st_InputInfo.u8_PassWordBytes++] = u8_KeyVal + '0';
        }
        break;
    case MENU_KEY_MENU:
        //退出
        break;
    case MENU_KEY_ENTER:
        if (st_InputInfo.u8_PassWordBytes >= M_PASSWORD_MAX_LEN) //校对输入数量
        {
            Tmp = memcmp((const INT8U *)(&st_InputInfo.a_u8_PassWordBuf),
                         (const INT8U *)(&st_InputInfo.a_u8_PassWordBufBak),
                         M_PASSWORD_MAX_LEN); //校对密码

            if (0 == Tmp)
            {
                if (0 == ChkPwdIsTooSimple((const INT8U *)&st_InputInfo.a_u8_PassWordBuf,
                                           M_PASSWORD_MAX_LEN)) //密码符合规则
                {
                    //对密码进行加密处理
                    memcpy((INT8U *)&storageSave.sysDat.pwd,
                           st_InputInfo.a_u8_PassWordBuf,
                           M_PASSWORD_MAX_LEN);
					storageSave.sysDat.pwdSetFlag |= 0x01;

#ifdef PWD_TEST_DEBUG
                    //					DEBUG_LOG_OUT("GetInputInfoMenuModPwdInputAgainFun:\n");
                    //					DEBUG_LOG_OUT("sys Pwd :\n");
                    //					DEBUG_LOG_OUT_HEX(storageSave.sysDat.pwd,sizeof(storageSave.sysDat.pwd));
                    //					DEBUG_LOG_OUT("GetInputInfoMenuModPwdInputAgainFun:\n");
                    //					DEBUG_LOG_OUT("Pwd A:\n");
                    //					DEBUG_LOG_OUT_HEX(st_InputInfo.a_u8_PassWordBuf,sizeof(st_InputInfo.a_u8_PassWordBuf));
                    //					DEBUG_LOG_OUT("Pwd B:\n");
                    //					DEBUG_LOG_OUT_HEX(st_InputInfo.a_u8_PassWordBufBak,sizeof(st_InputInfo.a_u8_PassWordBufBak));
                    DELAY_MS(1);                                   //这里必须要延迟一下才能写入？
                    HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
                                                                   //					HAL_Storage_ReadSysData(&storageSave.sysDat);                          //保存密码
                                                                   //					DEBUG_LOG_OUT("Pwd C:\n");
                                                                   //					DEBUG_LOG_OUT_HEX(storageSave.sysDat.pwd,sizeof(storageSave.sysDat.pwd));
#else
                    DELAY_MS(1);                                   //这里必须要延迟一下才能写入？
                    HAL_Storage_WriteSysData(&storageSave.sysDat); //保存密码
#endif
                    Menu_RecordLockLog(eLogTypeAddPwd, NULL, 0);                            //保存事件记录
                    memset((INT8U *)&st_InputInfo.a_u8_PassWordBuf, 0, M_PASSWORD_MAX_LEN); //清空密码缓存
                    fn_JumpMenu(enum_Menu_ModPwdOk);                                        //密码设定成功
                    DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ModPwdOk);\n");
                }
                else
                {
                    fn_JumpMenu(enum_Menu_ModPwdTooSimpleFail); //密码过于简单
                    DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ModPwdTooSimpleFail);\n");
                }
            }
            else
            {
                fn_JumpMenu(enum_Menu_ModPwdFail); //密码设定失败
                DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ModPwdFail);\n");
            }
        }
        else
        {
            fn_JumpMenu(enum_Menu_ModPwdFail); //密码设定失败
            DEBUG_LOG_OUT("fn_JumpMenu(enum_Menu_ModPwdFail);\n");
        }
        break;
    default:
        break;
    }
}

/***************************************************************************************
**函数名:       GetInputInfoMenuFactoryFun
**功能描述:     恢复出厂设置 
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
static void GetInputInfoMenuFactoryFun(INT8U u8_KeyVal)
{
    INT8U u8_i;
    INT8S Tmp = 0;
    switch (u8_KeyVal) //按键处理
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
        break;
    case MENU_KEY_MENU:
        //        fn_JumpMenu(enum_Menu_SystemSleep);
        break;
    case MENU_KEY_ENTER:
        fn_JumpMenu(enum_Menu_FactoryConfirm); //恢复出厂设置确认。
        break;
    default:
        break;
    }
}
/*************************全局函数****************************/
/***************************************************************************************
**函数名:       fn_GetInputInfo
**功能描述:     获取输入信息
**输入参数:     INT8U u8_KeyVal 按键值
**输出参数:     --
**备注:         
****************************************************************************************/
void fn_GetInputInfo(INT8U u8_KeyVal)
{
    INT8U u8_i;
    //    u8_KeyVal = u8_KeyVal + '0'; //这里使用字符
    switch (st_MenuItem.e_CurrentIndex)
    {
    case enum_Menu_Desktop:
        GetInputInfoMenuDesktopFun(u8_KeyVal);
        break;
    case enum_Menu_ModPwd:
        GetInputInfoMenuModPwdFun(u8_KeyVal);
        break;
    case enum_Menu_ModPwdInputAgain:
        GetInputInfoMenuModPwdInputAgainFun(u8_KeyVal);
        break;
    case enum_Menu_Factory:
        GetInputInfoMenuFactoryFun(u8_KeyVal);
        break;
    default:
        break;
    }
}
#endif
/************************************Code End******************************************/
