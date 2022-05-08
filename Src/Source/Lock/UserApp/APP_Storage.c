/***************************************************************************************
**文件名:     APP_Storage.c
**版本:       V1.00
**说明:       本文件是系统的存储结构
**修改记录:
*版本:V1.00
*修改者:hhx
*时间:
*修改说明：
创建了这个文件
**备注:
****************************************************************************************/
#include "APP_Storage.h"
#if APP_STORAGE_MODULE_EN
#warning APP_STORAGE模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/
TYPEs_StorageSave storageSave; //注意！未初始化
TYPEs_SystemDat systemDat;                 //注意！未初始化
/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       APP_Storage_ChkFingerNum
**功能描述:     检查指纹空间
**输入参数:     --
**输出参数:
INT8S
0; //无指纹
-1; //指纹库已满
1; //有指纹，但是库还没有满
**备注:
****************************************************************************************/
INT8S APP_Storage_ChkFingerNum(void)
{
    return HAL_Storage_ChkFingerNum(&storageSave);
}
/***************************************************************************************
**函数名:       APP_Storage_ReadOneFingerReg
**功能描述:     读取一个指纹数据
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_ReadOneFingerReg(void)
{
    return HAL_Storage_ReadOneFingerReg(&(storageSave.finger));
}

/***************************************************************************************
**函数名:       APP_Storage_SaveOneFingerReg
**功能描述:     保存一个指纹数据
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_SaveOneFingerReg(void)
{
    return HAL_Storage_SaveOneFingerReg(&storageSave);
}

/***************************************************************************************
**函数名:       APP_Storage_DelOneFingerReg
**功能描述:     删除一个指纹数据
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_DelOneFingerReg(void)
{
    return HAL_Storage_DelOneFingerReg(&storageSave);
}

/***************************************************************************************
**函数名:       APP_StorageResetFingerStruct
**功能描述:     清除指纹结构
**输入参数:     --
**输出参数:
INT8S
0 成功
**备注:
****************************************************************************************/
INT8S APP_Storage_ResetFingerStruct(void)
{
    return HAL_Storage_ResetFingerStruct(&storageSave);
}

/***************************************************************************************
**函数名:       APP_Storage_GeneralFingerNumChk
**功能描述:     普通指纹用户指纹数量检查
**输入参数:     --
**输出参数:
INT8S
0 数量为空
1 有空间
-1 参数错误
-2 数量溢出
**备注:
****************************************************************************************/
INT8S APP_Storage_GeneralFingerNumChk(void)
{
    return HAL_Storage_GeneralFingerNumChk(&systemDat);
}

/***************************************************************************************
**函数名:       APP_Storage_AdminFingerNumChk
**功能描述:     管理指纹用户指纹数量检查
**输入参数:     --
**输出参数:
INT8S
1 有空间
-1 参数错误
-2 数量溢出
**备注:
****************************************************************************************/
INT8S APP_Storage_AdminFingerNumChk(void)
{
    return HAL_Storage_AdminFingerNumChk(&systemDat);
}
/***************************************************************************************
**函数名:       APP_Storage_lockRegStatusSet
**功能描述:     锁注册状态设置
**输入参数:
INT8U status
0 未注册
1 注册中
2 已注册
**输出参数:
INT8S
0 成功
-1 参数错误

**备注:
****************************************************************************************/
INT8S APP_Storage_lockRegStatusSet(INT8U status)
{
    return HAL_Storage_lockRegStatusSet(&systemDat, status);
}

/***************************************************************************************
**函数名:       APP_Storage_lockRegStatusGet
**功能描述:     锁注册状态获取
**输入参数:     --
**输出参数:
INT8S
-1 参数错误
0 未注册
1 注册中
2 已注册
**备注:
****************************************************************************************/
INT8S APP_Storage_lockRegStatusGet(void)
{
    return HAL_Storage_lockRegStatusGet(&systemDat);
}

/***************************************************************************************
**函数名:       APP_Storage_SavelockRegStatus
**功能描述:     保存系统注册状态
**输入参数:     --
**输出参数:
INT8S
0 成功
-1 参数错误
**备注:
****************************************************************************************/
INT8S APP_Storage_SaveLockRegStatus(void)
{
    return HAL_Storage_SaveLockRegStatus(systemDat.lockRegStatus);
}

/***************************************************************************************
**函数名:       APP_Storage_SaveAdminFingerNum
**功能描述:     保存管理员指纹数量
**输入参数:
INT16U dat 数量
**输出参数:
INT8S
0 成功
**备注:
****************************************************************************************/
INT8S APP_Storage_SaveAdminFingerNum(void)
{
    return HAL_Storage_SaveAdminFingerNum(systemDat.adminFingerNum);
}
/***************************************************************************************
**函数名:       APP_Storage_SaveGeneralFingerNum
**功能描述:     普通指纹数量
**输入参数:
INT16U dat 数量
**输出参数:
INT8S
0 成功
**备注:
****************************************************************************************/
INT8S APP_Storage_SaveGeneralFingerNum(void)
{
    return HAL_Storage_SaveGeneralFingerNum(systemDat.generalFingerNum);
}

/***************************************************************************************
**函数名:       APP_Storage_SearchEmptyFingerLocalAdmin
**功能描述:     查找空位指纹头本地管理员
**输入参数:     --
**输出参数:
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:
****************************************************************************************/
INT16U APP_Storage_SearchEmptyFingerLocalAdmin(void)
{
    return HAL_Storage_SearchEmptyFingerLocalAdmin();
}

/***************************************************************************************
**函数名:       APP_Storage_SearchEmptyFingerLocalGeneral
**功能描述:     查找空位指纹头本地普通用户
**输入参数:     --
**输出参数:
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:
****************************************************************************************/
INT16U APP_Storage_SearchEmptyFingerLocalGeneral(void)
{
    return HAL_Storage_SearchEmptyFingerLocalGeneral();
}

/***************************************************************************************
**函数名:       APP_Storage_SaveOneFingerRegLocal
**功能描述:     保存一个指纹数据本地
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_SaveOneFingerRegLocal(void)
{
    return  HAL_Storage_SaveOneFingerRegLocal(&storageSave);
}
/***************************************************************************************
**函数名:       APP_Storage_DelAllFingerLocalGeneral
**功能描述:     删除所有的本地普通指纹并更新数量
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_DelAllFingerLocalGeneral(void)
{
    return HAL_Storage_DelAllFingerLocalGeneral(&storageSave,&systemDat);
}
/***************************************************************************************
**函数名:       APP_Storage_SystemDataInit
**功能描述:     系统数据初始化
**输入参数:
TYPEs_StorageSave *pSave, 数据存储结构
TYPEs_SystemDat *pDat 系统数据
**输出参数:
INT8S
0 成功
**备注:
****************************************************************************************/
INT8S APP_Storage_SystemDataInit(void)
{
    return HAL_Storage_SystemDataInit(&storageSave,&systemDat);
}
/***************************************************************************************
**函数名:       APP_Storage_SystemDataRead
**功能描述:     系统数据读取
**输入参数:     --
**输出参数:
INT8S
0 成功
-1 通讯失败
-2 参数错误
-3 读取错误
**备注:
****************************************************************************************/
INT8S APP_Storage_SystemDataRead(void)
{
    return HAL_Storage_SystemDataRead(&storageSave,&systemDat);
}

/***************************************************************************************
**函数名:       APP_Storage_DelAllFingerLocal
**功能描述:     删除所有的本地指纹
**输入参数:     --
**输出参数:
INT8S 0,成功
**备注:
****************************************************************************************/
INT8S APP_Storage_DelAllFingerLocal(void)
{
    return HAL_Storage_DelAllFingerLocal(&storageSave,&systemDat);
}
#endif /*APP_STORAGE_MODULE_EN*/
/************************************Code End******************************************/
