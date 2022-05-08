/***************************************************************************************
**文件名:     HAL_Finger.c
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
#include "HAL_Finger.h"
#include "DRV_Finger.h"
#if HAL_FINGER_MODULE_EN
#warning HAL_FINGER模块使能!
/*******************************宏定义***************************/
#define HAL_FINGER_REG_REPEAT_MAX (6) //注册时候按压指纹头的次数
/*************************.C变量结构声明***********************/
typedef enum
{
    eOptionStatusNone,        //空闲
    eOptionStatusStart,       //开始
    eOptionStatusPowerUp,     //上电
    eOptionStatusWaitPowerOk, //等待上电完成
    eOptionStatusStandby,     //待机中
    eOptionStatusReg,         //指纹注册
    eOptionStatusSerach,      //指纹搜索
    eOptionStatusGenImg,      //录指纹图像
    eOptionStatusImg2Tz,      //阶段图像转特征
    eOptionStatusRegModel,    //阶段合并模板
    eOptionStatusStoreChar,   //阶段储存模板
    eOptionStatusDel,         //删除指纹
} TYPEe_OptionStatus;         //指纹工作状态
typedef struct
{
    TYPEe_OptionStatus optionStatus;
    uint8_t regSaveBuf; //存储缓存区
    uint8_t *pAck;      //回复数据的指针

} TYPEs_FingerOption;
/*************************私有全局变量*************************/
static TYPEs_FingerOption fingerOption =
    {
        .optionStatus = eOptionStatusNone,
};
/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_Finger_Init
**功能描述:     指纹头初始化
**输入参数:     --
**输出参数:     
int8_t
0 成功
-1 失败
**备注:         
****************************************************************************************/
int8_t HAL_Finger_Init(void)
{
    fingerOption.pAck = DRV_Finger_GetAckData(); //获取接收数据的地址。
	return DRV_Finger_Init();
}

/***************************************************************************************
**函数名:       HAL_Finger_SaveBufSet
**功能描述:     指纹头特征缓存设置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Finger_SaveBufSet(uint8_t dat)
{
    fingerOption.regSaveBuf = dat;
}
/***************************************************************************************
**函数名:       HAL_Finger_SaveBufGet
**功能描述:     指纹头特征缓存获取
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t HAL_Finger_SaveBufGet(void)
{
    return fingerOption.regSaveBuf;
}
/***************************************************************************************
**函数名:       HAL_Finger_RegStart
**功能描述:     开始注册
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Finger_RegStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    fingerOption.regSaveBuf = 1;
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Finger_RegReStart
**功能描述:     重启注册
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
int8_t HAL_Finger_RegReStart(void)
{
    fingerOption.optionStatus = eOptionStatusGenImg;
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Finger_Reg
**功能描述:     指纹注册流程
**输入参数:     uint16_t model 注册的模板地址
**输出参数:     
int8_t
0,执行中，等待结果
1、开始录入过程
2、检测到有效手指
3、图像转特征成功
-3、图像转特征失败。
4、转特征合成
5、特征合成成功
-5、错误,特征合成失败
6、保存成功
-6、错误,特征保存失败
**备注:         
****************************************************************************************/
int8_t HAL_Finger_Reg(uint16_t model)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.regSaveBuf = 1; //特征,存储缓存区
        fingerOption.optionStatus = eOptionStatusGenImg;
        DRV_Finger_ResetCmd();
        reTmp = 1; //开始
        break;
    case eOptionStatusGenImg:
        switch (DRV_Finger_GenImg(model,0))
        {
        case 0: //接收正确
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusImg2Tz;
                reTmp = 2; //检测到有效手指
                DEBUG_LOG_OUT("Have Finger\n");
                break;
            default:
                //                DEBUG_LOG_OUT("No Finger\n");
                fingerOption.optionStatus = eOptionStatusGenImg; //重试
                break;
            }
            break;
        case -2: //接收超时
            DEBUG_LOG_OUT("GenImg Timer Over\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        case -3: //参数错误
            DEBUG_LOG_OUT("GenImg parm error\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        }
        break;
    case eOptionStatusImg2Tz:
        switch (DRV_Finger_Img2Tz(fingerOption.regSaveBuf))
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                DEBUG_LOG_OUT("Img2Tz OK\n");
                DEBUG_LOG_OUT("fingerOption.regSaveBuf is %d\n", fingerOption.regSaveBuf);
                fingerOption.regSaveBuf++;                               //准备存入下一个buffer
                if (fingerOption.regSaveBuf > HAL_FINGER_REG_REPEAT_MAX) //重复放置指纹头次数
                {
                    fingerOption.optionStatus = eOptionStatusRegModel; //合并模板
                    reTmp = 4;                                         //图像特征合成
                }
                else
                {
                    fingerOption.optionStatus = eOptionStatusNone; //提示用户后，继续进入
                    reTmp = 3;                                     //图像转特征成功。
                }
                break;
            default:
                reTmp = -3;                                      //图像转特征失败。
                fingerOption.optionStatus = eOptionStatusGenImg; //重试
                break;
            }
            break;
        case -1: //运行中
            break;
        case -2:                                             //接收超时
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        case -3:                                             //参数错误
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        }
        break;
    case eOptionStatusRegModel:
		DEBUG_LOG_OUT("eOptionStatusRegModel\n");
        switch (DRV_Finger_RegModel())
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusStoreChar;
                reTmp = 5; //特征合成成功
                break;
            default:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = -5; //错误,特征合成失败
                break;
            }
            break;
        case -1: //运行中
            break;
        case -2: //接收超时
        case -3: //参数错误
        default:
			DEBUG_LOG_OUT("eOptionStatusRegModel -4\n");
            fingerOption.optionStatus = eOptionStatusNone;
            reTmp = -4; //错误,特征合成失败
            break;
        }
        break;
    case eOptionStatusStoreChar:
        switch (DRV_Finger_Store(1, model))
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = 6; //保存成功
                break;
            default:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = -6; //错误,特征保存失败
                break;
            }
            break;
        case -1: //运行中
            break;
        case -2: //接收超时
        case -3: //参数错误
        default:
            fingerOption.optionStatus = eOptionStatusNone;
            reTmp = -5; //错误,特征保存失败
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Finger_VerifyStart
**功能描述:     启动指纹校验流程
**输入参数:     --
**输出参数:     
int8_t
0,成功
**备注:         
****************************************************************************************/
int8_t HAL_Finger_VerifyStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Finger_Verify
**功能描述:     指纹校验流程
**输入参数:     uint16_t id 获取到的ID号
**输出参数:     
int8_t
0,执行中，等待结果
1、开始录入过程
2、检测到有效手指
3、图像转特征成功
-3、图像转特征失败。
4、指纹搜索成功
-4、指纹搜索失败
**备注:         
****************************************************************************************/
int8_t HAL_Finger_Verify(uint16_t *id)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusGenImg;
        DRV_Finger_ResetCmd();
        reTmp = 1; //开始
        break;
    case eOptionStatusGenImg:
        switch (DRV_Finger_GenImg(*id,1))
        {
        case 0: //接收正确
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusImg2Tz;
                DRV_Finger_ResetCmd();
                reTmp = 2; //检测到有效手指
                DEBUG_LOG_OUT("Have Finger\n");
                break;
            default:
                //				DEBUG_LOG_OUT("No Finger\n");
                fingerOption.optionStatus = eOptionStatusGenImg; //重试
                break;
            }
            break;
        case -2: //接收超时
            DEBUG_LOG_OUT("GenImg Timer Over\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        case -3: //参数错误
            DEBUG_LOG_OUT("GenImg parm error\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        }
        break;
    case eOptionStatusImg2Tz:
        switch (DRV_Finger_Img2Tz(1)) //生成特征,储缓存区01
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                DEBUG_LOG_OUT("Img2Tz OK\n");
                DEBUG_LOG_OUT("fingerOption.regSaveBuf is %d\n", fingerOption.regSaveBuf);
                fingerOption.optionStatus = eOptionStatusSerach; //搜索指纹
                reTmp = 3;                                       //图像转特征成功。
                break;
            default:
                reTmp = -3;                                      //图像转特征失败。
                fingerOption.optionStatus = eOptionStatusGenImg; //重试
                break;
            }
            break;
        case -1: //运行中
            break;
        case -2:                                             //接收超时
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        case -3:                                             //参数错误
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //重试
            break;
        }
        break;
    case eOptionStatusSerach:
        switch (DRV_Finger_Serach(1, 0, 100)) //
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                *id = (uint16_t)((fingerOption.pAck[1] << 8) + fingerOption.pAck[2]); //获取指纹ID
                reTmp = 4;                                                            //指纹搜索成功
                fingerOption.optionStatus = eOptionStatusGenImg;
                break;
            default:
                reTmp = -4; //指纹搜索失败
                fingerOption.optionStatus = eOptionStatusGenImg;
                break;
            }
            break;
        case -1: //运行中
            break;
        case -2: //接收超时
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        case -3: //参数错误
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Finger_DelStart
**功能描述:     删除指纹开启
**输入参数:     --
**输出参数:     
int8_t
0,成功
**备注:         
****************************************************************************************/
int8_t HAL_Finger_DelStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Finger_Del
**功能描述:     删除指纹
**输入参数:     
uint16_t model			模板号
uint16_t num 			删除个数
**输出参数:     
int8_t
0,执行中，等待结果
1、操作成功
-1、超时
-2、参数错误
-3、删除失败
**备注:         
****************************************************************************************/
int8_t HAL_Finger_Del(uint16_t model, uint16_t num)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusDel;
        DRV_Finger_ResetCmd();
        break;
    case eOptionStatusDel:
        switch (DRV_Finger_DeletChar(model, num))
        {
        case 0: //操作成功
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                reTmp = 1; //操作成功
                break;
            default:
                reTmp = -3; //删除失败
                break;
            }
            //            reTmp = 1;
            break;
        case -2: //超时
            reTmp = -1;
            break;
        case -3: //参数错误
            reTmp = -2;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Finger_EmptyStart
**功能描述:     清空指纹开启
**输入参数:     --
**输出参数:     
int8_t
0,成功
**备注:         
****************************************************************************************/
int8_t HAL_Finger_EmptyStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Finger_Empty
**功能描述:     清空指纹
**输入参数:     --
**输出参数:     
int8_t
0,执行中，等待结果
1、操作成功
-1、超时
-2、参数错误
-3、清空失败
**备注:         
****************************************************************************************/
int8_t HAL_Finger_Empty(void)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusDel;
        DRV_Finger_ResetCmd();
        break;
    case eOptionStatusDel:
        switch (DRV_Finger_Empty())
        {
        case 0: //操作成功
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                reTmp = 1; //操作成功
                break;
            default:
                reTmp = -3; //清空失败
                break;
            }
            break;
        case -2: //超时
            reTmp = -1;
            break;
        case -3: //参数错误
            reTmp = -2;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Finger_EnterSleepConfig
**功能描述:     进入休眠状态
**输入参数:     --
**输出参数:     
**备注:         
****************************************************************************************/
void HAL_Finger_EnterSleepConfig(void)
{
	DRV_Finger_EnterSleepConfig();
}
/***************************************************************************************
**函数名:       HAL_Finger_HavePress
**功能描述:     有按键按压
**输入参数:     --
**输出参数:     
**备注:         
****************************************************************************************/
int8_t HAL_Finger_HavePress(void)
{
	if (1 == M_FING_IRQ_READIN())
	{
		return 0; //有手指
	}
	else
	{
		return -1; //没有手指
	}
}
/***************************************************************************************
**函数名:       HAL_Finger_ChkFingerLeave
**功能描述:     检查指纹是否离开
**输入参数:     --
**输出参数:     
int8_t
0;		//操作成功
-1;		//操作中
-2;		//超时
-3;		//参数错误
**备注:         
****************************************************************************************/
int8_t HAL_Finger_ChkFingerLeave(void)
{
    return DRV_Finger_ChkFingerLeave();
}
#endif /*HAL_FINGER_MODULE_EN*/
/************************************Code End******************************************/
