/***************************************************************************************
**文件名:     HAL_Motor.c
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
#include "HAL_Motor.h"
#include "DRV_Motor.h"
#if HAL_MOTOR_MODULE_EN
#warning HAL_MOTOR模块使能!
/*******************************宏定义***************************/
#define HAL_MOTOR_OPEN_TIME (200)           //开门时间
#define HAL_MOTOR_OPEN_WAIT_TIME (10)       //开门过程停顿时间
#define HAL_MOTOR_OPEN_OK_TIME (6000)       //开门完成等待时间
#define HAL_MOTOR_CLOSE_TIME (200)          //关门时间
#define HAL_MOTOR_CLOSE_WAIT_TIME (10)      //关门时间
#define HAL_MOTOR_LOCK_HANDLE_CHK_TIME (10) //把手状态计数器
/*************************.C变量结构声明***********************/
typedef struct
{
    INT32U timeLimit;            //工作时间
    INT32U timeCnt;              //时间计数器
    INT32U lockHandleCnt;        //把手操作计数器
    INT8U passageLockFlag;       //通道功能
    TYPEe_MotorRunStatus status; //运行状态
} TYPEs_MotorRun;
/*************************私有全局变量*************************/
static TYPEs_MotorRun MotorRun; //未初始化!
/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       HAL_Motor_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Motor_Init(void)
{
    MotorRun.timeCnt = 0;
    MotorRun.lockHandleCnt = 0;
    MotorRun.status = eMotorRunStatusNone;
    DRV_Motor_Init();
}
/***************************************************************************************
**函数名:       HAL_Motor_OpenClose
**功能描述:     开锁并关锁
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Motor_OpenClose(void)
{
    MotorRun.status = eMotorRunStatusOpen; //开启开锁状态机
    MotorRun.passageLockFlag = 0;
}
/***************************************************************************************
**函数名:       HAL_Motor_Open
**功能描述:     开锁不自动关锁
**输入参数:     --
**输出参数:     --
**备注:         预留做通道锁
****************************************************************************************/
void HAL_Motor_Open(void)
{
    MotorRun.status = eMotorRunStatusOpen; //开启开锁状态机
    MotorRun.passageLockFlag = 1;
}
/***************************************************************************************
**函数名:       HAL_Motor_Close
**功能描述:     关锁
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Motor_Close(void)
{
    MotorRun.status = eMotorRunStatusClose; //开启关锁状态机
    MotorRun.passageLockFlag = 0;
}
/***************************************************************************************
**函数名:       HAL_Motor_Deal
**功能描述:     电机运行处理
**输入参数:     --
**输出参数:     
INT8S
0 运行中
1; //开门成功
2; //开门后关门成功
3; //通道锁开门成功
**备注:         
基于1ms运行
****************************************************************************************/
INT8S HAL_Motor_Deal(void)
{
    INT8S reTmp = 0;
    if (MotorRun.timeCnt > 0) //计数器减1
    {
        MotorRun.timeCnt--;
    }
    switch (MotorRun.status)
    {
    case eMotorRunStatusNone: //无动作
        break;
    case eMotorRunStatusOpen: //开门
        DRV_Motor_Open();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_TIME;
        MotorRun.status = eMotorRunStatusOpenWait;
        break;
    case eMotorRunStatusOpenWait: //开门中
        if (MotorRun.timeCnt > 0) //运行时间未到
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_WAIT_TIME;
        MotorRun.status = eMotorRunStatusOpen1;
        break;
    case eMotorRunStatusOpen1: //再次开门
        DRV_Motor_Open();
        MotorRun.timeCnt = HAL_MOTOR_OPEN_TIME;
        MotorRun.status = eMotorRunStatusOpen1Wait;
        break;
    case eMotorRunStatusOpen1Wait: //再次开门中
        if (MotorRun.timeCnt > 0)  //运行时间未到
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusOpenOk;
        break;
    case eMotorRunStatusOpenOk: //开门成功
        MotorRun.timeCnt = HAL_MOTOR_OPEN_OK_TIME;
        if (1 == MotorRun.passageLockFlag)
        {
            MotorRun.status = eMotorRunStatusNone;
            reTmp = 3; //通道锁开门成功
        }
        else
        {
            MotorRun.status = eMotorRunStatusOpenOkWait;
            reTmp = 1; //开门成功
        }
        break;
    case eMotorRunStatusOpenOkWait:      //开门成功等待
        if (1 == DRV_Motor_LockHandle()) //这里检测把手状态
        {
            MotorRun.lockHandleCnt++;
            if (MotorRun.lockHandleCnt >= HAL_MOTOR_LOCK_HANDLE_CHK_TIME)
            {
                MotorRun.timeCnt = 0; //不在等待，直接认为客户已经开门
            }
        }
        else
        {
            MotorRun.lockHandleCnt = 0; //把手检测计数器清零
        }
        if (MotorRun.timeCnt > 0) //运行时间未到
        {
            break;
        }
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusClose;
        break;
    case eMotorRunStatusClose: //关门
        DRV_Motor_Close();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_TIME;
        MotorRun.status = eMotorRunStatusCloseWait;
        break;
    case eMotorRunStatusCloseWait: //关门中
        if (MotorRun.timeCnt > 0)  //运行时间未到
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_WAIT_TIME;
        MotorRun.status = eMotorRunStatusClose1;
        break;
    case eMotorRunStatusClose1: //再次关门
        DRV_Motor_Close();
        MotorRun.timeCnt = HAL_MOTOR_CLOSE_TIME;
        MotorRun.status = eMotorRunStatusClose1Wait;
        break;
    case eMotorRunStatusClose1Wait: //再次关门中
        if (MotorRun.timeCnt > 0)   //运行时间未到
        {
            break;
        }
        DRV_Motor_Stop();
        MotorRun.timeCnt = 0;
        MotorRun.status = eMotorRunStatusCloseOK;
        break;
    case eMotorRunStatusCloseOK:               //关门完成
        reTmp = 2;                             //开门后关门成功
        MotorRun.status = eMotorRunStatusNone; //进入空闲
        break;
    default:
        break;
    }
    return reTmp;
}
#endif /*HAL_MOTOR_MODULE_EN*/
       /************************************Code End******************************************/
