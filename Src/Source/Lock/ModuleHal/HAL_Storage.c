/***************************************************************************************
**文件名:     HAL_Storage.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    


**备注:      
****************************************************************************************/
#include "HAL_Storage.h"
#include "DRV_Storage.h"
#if HAL_STORAGE_MODULE_EN
#warning HAL_STORAGE模块使能!
/*******************************宏定义***************************/
#define HAL_STORAGE_RW_BUF_LEN (150)    //读写缓存大小
#define HAL_STORAGE_DEFAULT_BYTE (0XFF) //存储介质的默认值
#define HAL_STORAGE_DELAY_MS(X) (DelayMS(X)) //延迟

/*************************.C变量结构声明***********************/
typedef enum
{
    eFingerNumModAdd, //增加
    eFingerNumModDel, //减少
} TYPEe_StorageFingerNumMod;
/*************************私有全局变量*************************/
static INT8U gRwBuf[HAL_STORAGE_RW_BUF_LEN] = {
    0,
};
/*************************公共全局变量*************************/
TYPEs_StorageSave storageSave; //注意！未初始化
/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       HAL_StorageRw
**功能描述:     存储读写
**输入参数:     
TYPEe_StorageDire dir, 读写方向
INT32U addr, 读写地址 
INT8U *data, 读写数据
INT16U length 读写长度
**输出参数:     
INT8S
0,执行成功
-1,通讯失败
-2,参数错误

**备注:         EEPROM写入需要时间，这里需要预留.
****************************************************************************************/
//static INT8S HAL_Storage_Rw(TYPEe_StorageDire dir, INT32U addr, INT8U *data, INT16U length)
INT8S HAL_Storage_Rw(TYPEe_StorageDire dir, INT32U addr, INT8U *data, INT16U length)
{
    if (NULL == data)
    {
        return -2; //参数错误
    }
    if (0 == length)
    {
        return -2; //参数错误
    }
    return DRV_Storage_ReadWrite((INT8U)(dir),
                                 addr,
                                 data,
                                 length);
}

/***************************************************************************************
**函数名:       HAL_Storage_GetAddressWithId
**功能描述:     根据ID号获取地址
**输入参数:     
INT16U id,//id号
INT32U StartAddress,//起始地址
INT32U oneSize//单个数据占用的大小
**输出参数:     
INT32U 真实的物理地址。
**备注:         
****************************************************************************************/
static INT32U HAL_Storage_GetAddressWithId(INT16U id, INT32U StartAddress, INT32U oneSize)
{
    return (StartAddress + id * oneSize);
}

/***************************************************************************************
**函数名:       HAL_Storage_Write16Bit
**功能描述:     写入16位的数据
**输入参数:     
INT32U addr, 地址
INT16U dat 要写入的数据
**输出参数:     
INT8S 0,成功
**备注:         
****************************************************************************************/
static INT8S HAL_Storage_Write16Bit(INT32U addr, INT16U dat)
{
    gRwBuf[0] = (INT8U)(dat >> 8);
    gRwBuf[1] = (INT8U)(dat);
    HAL_Storage_Rw(eStorageDireWrite,
                   addr,
                   gRwBuf,
                   2);
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_WriteOneReg
**功能描述:     写入一个指纹信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageFinger *msg 指纹控制结构体
**输出参数:     
INT8S 0,成功
**备注: 
存储结构如下：
指纹起始	单指纹占64字节	768+38*180=7608	60个用户180个指纹				
768（0x300）	769	770	771	772	773	774	775
指纹有效W	上一编号H	上一编号L	下一编号H	下一编号L	不分主人客人	组号H	组号L
776	777	778	779	780	781	782	783
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	当前编号H	当前编号L
784	785	786	787	788	789	790	791
密码标志	密码	密码	密码	时效标志	开始年	月	日
792	793	794	795	796	797	798	799
时	分	秒	结束年	月	日	时	分
800	801	802	803	804	805	806	807
秒	周	亲情标志	SOS胁迫标志	校验H	校验L		
****************************************************************************************/
static INT8S HAL_Storage_WriteOneReg(INT32U addr, TYPEs_StorageFinger *msg)
{
    INT16U i = 0, j = 0;                                               //循环变量
    INT16U chkSum = 0;                                                 //校验和
    gRwBuf[HAL_STORAGE_VAILD_ADDR] = MSG_FINGERREGMEM_FIG;             //指纹存在标志.
    gRwBuf[HAL_STORAGE_UP_ID_ADDR] = (INT8U)(msg->up_pageID >> 8);     //上一个编号高
    gRwBuf[HAL_STORAGE_UP_ID_ADDR + 1] = (INT8U)(msg->up_pageID);      //上一个编号低
    gRwBuf[HAL_STORAGE_DOWN_ID_ADDR] = (INT8U)(msg->down_pageID >> 8); //下一个编号高
    gRwBuf[HAL_STORAGE_DOWN_ID_ADDR + 1] = (INT8U)(msg->down_pageID);  //下一个编号低
    gRwBuf[HAL_STORAGE_LIM_ADDR] = msg->Lim;                           //指纹属性.
    gRwBuf[HAL_STORAGE_GROUP_ADDR] = (INT8U)(msg->group >> 8);         //分组
    gRwBuf[HAL_STORAGE_GROUP_ADDR + 1] = (INT8U)(msg->group);          //分组
    i = HAL_STORAGE_NAME_ADDR;
    for (j = 0; j < 6; j++)
    {
        gRwBuf[i++] = msg->name[j]; //复制用户名
    }
    gRwBuf[HAL_STORAGE_PAGE_ID_ADDR] = (INT8U)(msg->pageID >> 8); //写入当前地址高
    gRwBuf[HAL_STORAGE_PAGE_ID_ADDR + 1] = (INT8U)(msg->pageID);  //写入当前地址低
#ifdef LOCK_FUNCTION_FINGERPWD_ON
    gRwBuf[HAL_STORAGE_PWD_FIG_ADDR] = msg->pwd.fig;
    gRwBuf[HAL_STORAGE_PWD_ADDR] = msg->pwd.pwd[0];
    gRwBuf[HAL_STORAGE_PWD_ADDR + 1] = msg->pwd.pwd[1];
    gRwBuf[HAL_STORAGE_PWD_ADDR + 2] = msg->pwd.pwd[2];
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON //时效
    gRwBuf[HAL_STORAGE_TM_VAILD_FIG_ADDR] = msg->TimeVaild.fig;
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR] = (INT8U)(msg->TimeVaild.str.year);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 1] = (INT8U)(msg->TimeVaild.str.mon);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 2] = (INT8U)(msg->TimeVaild.str.day);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 3] = (INT8U)(msg->TimeVaild.str.hour);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 4] = (INT8U)(msg->TimeVaild.str.min);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 5] = (INT8U)(msg->TimeVaild.str.sec);

    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR] = (INT8U)(msg->TimeVaild.stp.year);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 1] = (INT8U)(msg->TimeVaild.stp.mon);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 2] = (INT8U)(msg->TimeVaild.stp.day);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 3] = (INT8U)(msg->TimeVaild.stp.hour);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 4] = (INT8U)(msg->TimeVaild.stp.min);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 5] = (INT8U)(msg->TimeVaild.stp.sec);

    gRwBuf[HAL_STORAGE_TM_VAILD_WDAY_ADDR] = (INT8U)(msg->TimeVaild.wday);
#endif

#ifdef LOCK_FUNCTION_KINDRED_ON
    gRwBuf[HAL_STORAGE_KINDRED_ADDR] = msg->kindred;
#endif

#ifdef LOCK_FUNCTION_SOS_ON
    gRwBuf[HAL_STORAGE_SOS_ADDR] = msg->sos;
#endif
    for (j = 0; j < (HAL_STORAGE_ONE_FINGER_SIZE - HAL_STORAGE_CHKSUM_LEN); j++)
    {
        chkSum += gRwBuf[j];
    } //计算校验和
    gRwBuf[HAL_STORAGE_CHKSUM_ADDR] = (INT8U)(chkSum >> 8);
    gRwBuf[HAL_STORAGE_CHKSUM_ADDR + 1] = (INT8U)(chkSum); //写入校验和

    HAL_Storage_Rw(eStorageDireWrite,
                   addr,
                   gRwBuf,
                   HAL_STORAGE_ONE_FINGER_SIZE); //写入数据
                                                 //	HAL_Storage_Rw(eStorageDireRead,
                                                 //                   addr,
                                                 //                   gRwBuf,
                                                 //                   HAL_STORAGE_ONE_FINGER_SIZE); //读出测试数据是否正确
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ChkSum
**功能描述:     计算校验和
**输入参数:  
INT8U *dat, //数据
INT32U len  //数据长度
**输出参数:     
INT16U 校验和
**备注: 
****************************************************************************************/
static INT16U HAL_Storage_ChkSum(INT8U *dat, INT32U len)
{
    INT32U i = 0;
    INT16U sumTmp = 0;
    for (i = 0; i < len; i++)
    {
        sumTmp += *dat;
        dat++;
    }
    return sumTmp;
}

/***************************************************************************************
**函数名:       HAL_Storage_FingerLeaveNumSet
**功能描述:     指纹剩余数量变更
**输入参数:     
TYPEe_StorageFingerNumMod Mod, 加 减
TYPEs_StorageNum *num 数量
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
static INT8S HAL_Storage_FingerLeaveNumSet(TYPEe_StorageFingerNumMod Mod, TYPEs_StorageNum *num)
{
    if (eFingerNumModAdd == Mod)
    {
        num->leave += 1;
    }
    else
    {
        num->leave -= 1;
    }
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_LEAVE_ONLINEH, num->leave);
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_WriteOneUserReg
**功能描述:     写入一个用户信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
存储结构如下：
有效W	上一编号H	上一编号L	下一编号H	下一编号L	不分主人客人	组号H	组号L
							
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	当前编号H	当前编号L
							
密码标志	密码	密码	密码	时效标志	开始年	月	日
							
时	分	秒	结束年	月	日	时	分
							
秒	周	亲情标志	SOS胁迫标志	ID	ID	ID	ID
							
ID	ID	ID	ID	ID	ID	ID	ID
							
ID	校验H	校验L					
		
****************************************************************************************/
static INT8S HAL_Storage_WriteOneUserReg(INT32U addr, TYPEs_StorageUserDat *msg)
{
    msg->checksum = HAL_Storage_ChkSum((INT8U *)msg, sizeof(TYPEs_StorageUserDat) - sizeof(INT16U)); //计算校验和
    return HAL_Storage_Rw(eStorageDireWrite,
                          addr,
                          (INT8U *)msg,
                          sizeof(TYPEs_StorageUserDat)); //写入数据
}
/***************************************************************************************
**函数名:       HAL_Storage_ReadOneUserReg
**功能描述:     读取一个用户信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 					
****************************************************************************************/
static INT8S HAL_Storage_ReadOneUserReg(INT32U addr, TYPEs_StorageUserDat *msg)
{
    return HAL_Storage_Rw(eStorageDireRead,
                          addr,
                          (INT8U *)msg,
                          sizeof(TYPEs_StorageUserDat));
}

/*************************全局函数****************************/

/***************************************************************************************
**函数名:       HAL_Storage_SearchEmptyFinger
**功能描述:     查找空位指纹头
**输入参数:     --
**输出参数:     
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFinger(void)
{
    return HAL_Storage_SearchEmpty(0,
                                   MEM_FINGER_ALL_ONLINE,
                                   MSG_FINGER_REG_MEM_ONLINE_ST,
                                   MSG_FINGER_ONE_SIZE);
}

/***************************************************************************************
**函数名:       HAL_Storage_GetOneFingerReg
**功能描述:     获取一个指纹信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageFinger *msg 指纹控制结构体
**输出参数:     
INT8S 0,成功
**备注: 
存储结构如下：
指纹起始	单指纹占64字节	768+38*180=7608	60个用户180个指纹				
768（0x300）	769	770	771	772	773	774	775
指纹有效W	上一编号H	上一编号L	下一编号H	下一编号L	不分主人客人	组号H	组号L
776	777	778	779	780	781	782	783
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	当前编号H	当前编号L
784	785	786	787	788	789	790	791
密码标志	密码	密码	密码	时效标志	开始年	月	日
792	793	794	795	796	797	798	799
时	分	秒	结束年	月	日	时	分
800	801	802	803	804	805	806	807
秒	周	亲情标志	SOS胁迫标志	校验H	校验L		
****************************************************************************************/
INT8S HAL_Storage_GetOneFingerReg(INT32U addr, TYPEs_StorageFinger *msg)
{
    INT16U i = 0, j = 0;
    HAL_Storage_Rw(eStorageDireRead,
                   addr,
                   gRwBuf,
                   HAL_STORAGE_ONE_FINGER_SIZE);                                                       //获取数据
    msg->vaild = gRwBuf[HAL_STORAGE_VAILD_ADDR];                                                       //指纹存在标志.
    msg->up_pageID = (gRwBuf[HAL_STORAGE_UP_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_UP_ID_ADDR + 1];       //上一个编号
    msg->down_pageID = (gRwBuf[HAL_STORAGE_DOWN_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_DOWN_ID_ADDR + 1]; //下一个编号
    msg->Lim = gRwBuf[HAL_STORAGE_LIM_ADDR];                                                           //指纹属性.
    msg->group = (gRwBuf[HAL_STORAGE_GROUP_ADDR] << 8) + gRwBuf[HAL_STORAGE_GROUP_ADDR + 1];           //分组
    i = HAL_STORAGE_NAME_ADDR;
    for (j = 0; j < 6; j++)
    {
        msg->name[j] = gRwBuf[i++]; //复制用户名
    }
    msg->pageID = (gRwBuf[HAL_STORAGE_PAGE_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_PAGE_ID_ADDR + 1]; //写入地址
#ifdef LOCK_FUNCTION_FINGERPWD_ON
    msg->pwd.fig = gRwBuf[HAL_STORAGE_PWD_FIG_ADDR];
    msg->pwd.pwd[0] = gRwBuf[HAL_STORAGE_PWD_ADDR];
    msg->pwd.pwd[1] = gRwBuf[HAL_STORAGE_PWD_ADDR + 1];
    msg->pwd.pwd[2] = gRwBuf[HAL_STORAGE_PWD_ADDR + 2];
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON //时效
    msg->TimeVaild.fig = gRwBuf[HAL_STORAGE_TM_VAILD_FIG_ADDR];
    msg->TimeVaild.str.year = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR];
    msg->TimeVaild.str.mon = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 1];
    msg->TimeVaild.str.day = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 2];
    msg->TimeVaild.str.hour = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 3];
    msg->TimeVaild.str.min = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 4];
    msg->TimeVaild.str.sec = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 5];

    msg->TimeVaild.stp.year = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR];
    msg->TimeVaild.stp.mon = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 1];
    msg->TimeVaild.stp.day = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 2];
    msg->TimeVaild.stp.hour = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 3];
    msg->TimeVaild.stp.min = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 4];
    msg->TimeVaild.stp.sec = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 5];

    msg->TimeVaild.wday = gRwBuf[HAL_STORAGE_TM_VAILD_WDAY_ADDR];
#endif

#ifdef LOCK_FUNCTION_KINDRED_ON
    msg->kindred = gRwBuf[HAL_STORAGE_KINDRED_ADDR];
#endif

#ifdef LOCK_FUNCTION_SOS_ON
    msg->sos = gRwBuf[HAL_STORAGE_SOS_ADDR];
#endif
    msg->checksum = (gRwBuf[HAL_STORAGE_CHKSUM_ADDR] << 8) + gRwBuf[HAL_STORAGE_CHKSUM_ADDR + 1]; //校验和
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_SaveOneFingerReg
**功能描述:     保存一个指纹数据
**输入参数:     
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneFingerReg(TYPEs_StorageSave *msg)
{
    INT32U addrTmp = 0;
    HAL_Storage_FingerLeaveNumSet(eFingerNumModDel, &(msg->fingerNum)); //更新剩余数量
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msg->fingerNum.overgroupid,
                                                                      MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                      MSG_FINGER_ONE_SIZE); //获取上一个指纹的down id地址
    HAL_Storage_Write16Bit(addrTmp, msg->finger.pageID);                                    //修改上一个指纹的down id
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE);                              //获取保存地址
    HAL_Storage_WriteOneReg(addrTmp, &(msg->finger));                                         //保存一个指纹信息
    msg->fingerNum.overpageid = msg->finger.pageID;                                           //末尾pageID
    HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //写入末尾pageID
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_DelOneFingerReg
**功能描述:     删除一个指纹数据
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFingerReg(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE);  //获取保存地址
    HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //获取指纹信息
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //清除有效值
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //写入有效值
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
                                                                      MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                      MSG_FINGER_ONE_SIZE); //获取保存地址
    HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //修改上一个指纹的链尾
    if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //内容为空的情况
    {
        msg->fingerNum.overpageid = msg->finger.pageID;                                           //末尾pageID
        HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //写入末尾pageID
    }
    else
    {
        addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
                                                                        MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                        MSG_FINGER_ONE_SIZE); //获取保存地址
        HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //修改下一个指纹的链头
    }
    HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //更新剩余数量
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_ReadOneFingerReg
**功能描述:     读取一个指纹数据
**输入参数:     
TYPEs_StorageFinger *msg 指纹头结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ReadOneFingerReg(TYPEs_StorageFinger *msg)
{
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE); //获取保存地址
    HAL_Storage_GetOneFingerReg(addrTmp, msg);                   //获取指纹信息
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ChkFingerNum
**功能描述:     检查指纹空间
**输入参数:     
TYPEs_StorageSave *msg 指纹头存储结构
**输出参数:     
INT8S 
0; //无指纹
-1; //指纹库已满
1; //有指纹，但是库还没有满
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ChkFingerNum(TYPEs_StorageSave *msg)
{
    if (msg->fingerNum.all == msg->fingerNum.leave)
    {
        return 0; //无指纹
    }
    else if (0 == msg->fingerNum.leave)
    {
        return -1; //指纹库已满
    }
    else
    {
        return 1; //有指纹，但是库还没有满
    }
}
/***************************************************************************************
**函数名:       HAL_Storage_ResetFingerStruct
**功能描述:     清除指纹结构
**输入参数:     
TYPEs_StorageSave *msg 指纹头存储结构
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ResetFingerStruct(TYPEs_StorageSave *msg)
{
    memset(&(msg->finger), 0x00, sizeof(msg->finger));
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_EraseAllFingerReg
**功能描述:     擦除所有的指纹数据
**输入参数:     --
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_EraseAllFingerReg(void)
{
    INT16U i;
    INT32U addr = 0;
    memset(gRwBuf, 0, HAL_STORAGE_RW_BUF_LEN); //清空缓存
    for (i = 0; i < MEM_FINGER_ALL_ONLINE; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i,
                                            MSG_FINGER_REG_MEM_ONLINE_ST,
                                            MSG_FINGER_ONE_SIZE); //获取保存地址
        HAL_Storage_Rw(eStorageDireWrite,
                       addr,
                       gRwBuf,
                       HAL_STORAGE_ONE_FINGER_SIZE); //写入数据
    }

    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_lockRegStatusGet
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
INT8S HAL_Storage_lockRegStatusGet(TYPEs_SystemDat *dat)
{
    INT8S reTmp = 0;
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    switch (dat->lockRegStatus)
    {
    case MSG_FACTMEM_REG_STATUS_NONE:
        reTmp = 0; //未注册
        break;
    case MSG_FACTMEM_REG_STATUS_ING:
        reTmp = 1; //注册中
        break;
    case MSG_FACTMEM_REG_STATUS_OK:
        reTmp = 2; //本地管理员已注册
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Storage_lockRegStatusSet
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
INT8S HAL_Storage_lockRegStatusSet(TYPEs_SystemDat *dat, INT8U status)
{
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    switch (status)
    {
    case 0:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE;
        break;
    case 1:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_ING;
        break;
    case 2:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_OK;
        break;
    default:
        break;
    }
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_AdminFingerNumChk
**功能描述:     管理指纹用户指纹数量检查
**输入参数:     --
**输出参数:     
INT8S 
1 有空间
-1 参数错误
-2 数量溢出
**备注: 
****************************************************************************************/
INT8S HAL_Storage_AdminFingerNumChk(TYPEs_SystemDat *dat)
{
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    if (dat->adminFingerNum >= MEM_FINGER_ADMIN_ALL_LOCAL)
    {
        return -2; //数量溢出
    }
    else
    {
        return 1; //有空间
    }
}

/***************************************************************************************
**函数名:       HAL_Storage_AdminFingerIdChk
**功能描述:     管理指纹检查ID是否是管理指纹
**输入参数:     --
**输出参数:     
INT8S 
1 有空间
-1 参数错误
-2 数量溢出
**备注: 
****************************************************************************************/
INT8S HAL_Storage_AdminFingerIdChk(INT16U id)
{
    if (id < MEM_FINGER_ADMIN_ALL_LOCAL)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
/***************************************************************************************
**函数名:       HAL_Storage_SaveAdminFingerNum
**功能描述:     保存管理员指纹数量
**输入参数:     
INT16U dat 数量
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveAdminFingerNum(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_ADMIN_LOCALH,
                           dat);
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_GeneralFingerNumChk
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
INT8S HAL_Storage_GeneralFingerNumChk(TYPEs_SystemDat *dat)
{
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    if (dat->generalFingerNum >= MEM_FINGER_GUEST_ALL_LOCAL)
    {
        return -2; //数量溢出
    }
    else
    {
        if (dat->generalFingerNum == 0)
        {
            return 0; //数量为空
        }
        else
        {
            return 1; //有空间
        }
    }
}
/***************************************************************************************
**函数名:       HAL_Storage_SaveGeneralFingerNum
**功能描述:     普通指纹数量
**输入参数:     
INT16U dat 数量
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveGeneralFingerNum(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_GUEST_LOCALH,
                           dat);
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_SaveLockRegStatus
**功能描述:     保存系统注册标志
**输入参数:     --
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveLockRegStatus(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_ADMIN_REGISTER,
                           dat);
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_SystemDataInit
**功能描述:     系统数据初始化
**输入参数:     
TYPEs_StorageSave *pSave, 数据存储结构
TYPEs_SystemDat *pDat 系统数据
**输出参数:     
INT8S 
0 成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SystemDataInit(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat)
{
    gRwBuf[MEM_FACTMEM_FACTORY_FIG] = MSG_FACTMEM_FACTORY_FIG_OK;                            //写入成功标志
    gRwBuf[MEM_FACTMEM_LOCKMODE] = (MSG_FACTMEM_LOCKMODE_FINGER | MSG_FACTMEM_LOCKMODE_BLE); //开锁模式（指纹/密码蓝牙/蓝牙钥匙）
    gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEH] = MSG_FACTMEM_FINGER_ALL_ONLINEH;                 //全部指纹H（60*3）
    gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEL] = MSG_FACTMEM_FINGER_ALL_ONLINEL;                 //全部指纹L（60*3）
    gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEH] = MSG_FACTMEM_FINGER_LEAVE_ONLINEH;             //剩余可用指纹H（60*3）
    gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEL] = MSG_FACTMEM_FINGER_LEAVE_ONLINEL;             //剩余可用指纹L（60*3）
    gRwBuf[MEM_FACTMEM_PHONE_ALLH] = MSG_FACTMEM_PHONE_ALLH;                                 //ALL卡H
    gRwBuf[MEM_FACTMEM_PHONE_ALLL] = MSG_FACTMEM_PHONE_ALLL;                                 //ALL卡L
    gRwBuf[MEM_FACTMEM_PHONE_LEAVEH] = MSG_FACTMEM_PHONE_LEAVEH;                             //LEAVE卡H
    gRwBuf[MEM_FACTMEM_PHONE_LEAVEL] = MSG_FACTMEM_PHONE_LEAVEL;                             //LEAVE卡L
    gRwBuf[MEM_FACTMEM_SMARTKEY_ALLH] = MSG_FACTMEM_SMARTKEY_ALLH;                           //全部蓝牙钥匙数量H （20个）
    gRwBuf[MEM_FACTMEM_SMARTKEY_ALLL] = MSG_FACTMEM_SMARTKEY_ALLL;                           //全部蓝牙钥匙数量L  （20个）
    gRwBuf[MEM_FACTMEM_SMARTKEY_LEAVEH] = MSG_FACTMEM_SMARTKEY_LEAVEH;                       //剩余蓝牙钥匙数量H
    gRwBuf[MEM_FACTMEM_SMARTKEY_LEAVEL] = MSG_FACTMEM_SMARTKEY_LEAVEL;                       //剩余蓝牙钥匙数量L
    gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALH] = 0;                                             //本地管理员指纹数量H
    gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALL] = 0;                                             //本地管理员指纹数量L
    gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALH] = 0;                                             //本地用户指纹数量H
    gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALL] = 0;                                             //本地用户指纹数量L
    gRwBuf[MEM_FACTMEM_WIFI_MAIN_SW] = 0;                                                    //WIFI主开关
    gRwBuf[MEM_FACTMEM_WIFI_LOGUP_SW] = 0;                                                   //WIFI开门信号上传开关
    gRwBuf[MEM_FACTMEM_ADMIN_REGISTER] = (MSG_FACTMEM_REG_STATUS_NONE >> 8) & 0xff;          //本地管理员是否注册H
    gRwBuf[MEM_FACTMEM_ADMIN_REGISTER + 1] = (MSG_FACTMEM_REG_STATUS_NONE >> 0) & 0xff;      //本地管理员是否注册L
    gRwBuf[MEM_FACTMEM_VOICE] = 0x55;                                                        //语音开关
    gRwBuf[MEM_DH_G] = 0;                                                                    //dh原根dh_key.g_tmp;
    gRwBuf[MEM_DH_P] = 0;                                                                    //DH质数dh_key.p_tmp;
    gRwBuf[MEM_FACTMEM_DONE_FIG] = MSG_FACTMEM_FACTORY_FIG_OK;                               //工厂标志  G      结尾标志
    gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH] = MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEH;   //最后指纹ID  H（注册或者删除才用）最后用户
    gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL] = MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEL;   //最后指纹ID  L（注册或者删除才用）
    gRwBuf[MEM_FACTMEM_SMARTKEYOVER_PAGEIDH] = MSG_FACTMEM_SMARTKEYOVER_PAGEIDH;             //最后一个蓝牙钥匙存储位置H（注册或者删除才
    gRwBuf[MEM_FACTMEM_SMARTKEYOVER_PAGEIDL] = MSG_FACTMEM_SMARTKEYOVER_PAGEIDL;             //最后一个蓝牙钥匙存储位置L（注册或者删除才
    gRwBuf[MEM_FACTMEM_CARDOVER_PAGEIDH] = MSG_FACTMEM_PHONEOVER_PAGEIDH;                    //卡位置ID  H（注册或者删除才用）
    gRwBuf[MEM_FACTMEM_CARDOVER_PAGEIDL] = MSG_FACTMEM_PHONEOVER_PAGEIDL;                    //卡位置ID L（注册或者删除才用）
    gRwBuf[MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALH] = 0x00;                                    //最后一个组号H（注册或者删除才用）组管理,
    gRwBuf[MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALL] = 0x00;                                    //最后一个组号L（注册或者删除才用）组管理,
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_ST] = MSG_FACTMEM_LOCKLOG_LINK_ST;                       //开锁记录4字节
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_2] = MSG_FACTMEM_LOCKLOG_LINK_2;                         //开锁记录4字节
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_3] = MSG_FACTMEM_LOCKLOG_LINK_3;                         //开锁记录4字节
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_OV] = MSG_FACTMEM_LOCKLOG_LINK_OV;                       //开锁记录4字节
    gRwBuf[MEM_FACTMEM_LOCKLOG_SN_H] = MSG_FACTMEM_LOCKLOG_SN_H;                             //开锁记录序列号H
    gRwBuf[MEM_FACTMEM_LOCKLOG_SN_L] = MSG_FACTMEM_LOCKLOG_SN_L;                             //开锁记录序列号L
    gRwBuf[MEM_FACTMEM_ADMIN_PWD1] = MSG_FACTMEM_ADMIN_PWD1;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD2] = MSG_FACTMEM_ADMIN_PWD2;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD3] = MSG_FACTMEM_ADMIN_PWD3;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD4] = MSG_FACTMEM_ADMIN_PWD4;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD5] = MSG_FACTMEM_ADMIN_PWD5;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD6] = MSG_FACTMEM_ADMIN_PWD6;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD7] = MSG_FACTMEM_ADMIN_PWD7;                                 //密码
    gRwBuf[MEM_FACTMEM_ADMIN_PWD8] = MSG_FACTMEM_ADMIN_PWD8;                                 //密码
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR,
                   gRwBuf,
                   MEM_FACT_MEM_SIZE); //保存数据

    // pwd.admin.cur[0] = gRwBuf[MEM_FACTMEM_ADMIN_PWD1];
    // pwd.admin.cur[1] = gRwBuf[MEM_FACTMEM_ADMIN_PWD2];
    // pwd.admin.cur[2] = gRwBuf[MEM_FACTMEM_ADMIN_PWD3];
    // pwd.admin.cur[3] = gRwBuf[MEM_FACTMEM_ADMIN_PWD4];
    // pwd.admin.cur[4] = gRwBuf[MEM_FACTMEM_ADMIN_PWD5];
    // pwd.admin.cur[5] = gRwBuf[MEM_FACTMEM_ADMIN_PWD6];
    // pwd.admin.cur[6] = gRwBuf[MEM_FACTMEM_ADMIN_PWD7];
    // pwd.admin.cur[7] = gRwBuf[MEM_FACTMEM_ADMIN_PWD8];

    pSave->fingerNum.all = ((MSG_FACTMEM_FINGER_ALL_ONLINEH << 8) | MSG_FACTMEM_FINGER_ALL_ONLINEL);       //指纹总数
    pSave->fingerNum.leave = ((MSG_FACTMEM_FINGER_LEAVE_ONLINEH << 8) | MSG_FACTMEM_FINGER_LEAVE_ONLINEL); //可用指纹
    pSave->fingerNum.overpageid = ((MSG_FACTMEM_PHONEOVER_PAGEIDH << 8) | MSG_FACTMEM_PHONEOVER_PAGEIDL);  //最后一枚指纹的ID

    //    pDat->adminFingerNum = 0;                          //清空客户指纹数量
    //    pDat->generalFingerNum = 0;                        //清空普通指纹数量
    //    pDat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE; //无注册

    HAL_Storage_DelAllFingerLocal(pSave, pDat);        //清除所有的本地指纹
    pDat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE; //无注册
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_SystemDataRead
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
INT8S HAL_Storage_SystemDataRead(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat)
{
    if (NULL == pSave)
    {
        return -2; //参数错误
    }
    if (NULL == pDat)
    {
        return -2; //参数错误
    }
    if (-1 == HAL_Storage_Rw(eStorageDireRead,
                             MEM_FACTMEM_START_ADDR,
                             gRwBuf,
                             MEM_FACT_MEM_SIZE))
    {
        return -1; //通讯失败
    }
    if ((MSG_FACTMEM_FACTORY_FIG_OK == gRwBuf[MEM_FACTMEM_FACTORY_FIG]) && (MSG_FACTMEM_FACTORY_FIG_OK == gRwBuf[MEM_FACTMEM_DONE_FIG]))
    {

        pSave->fingerNum.all = gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEL];                      //全部指纹
        pSave->fingerNum.leave = gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEL];                //剩余可用指纹
        pSave->fingerNum.overpageid = gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL]; //最后指纹ID
        pDat->lockRegStatus = gRwBuf[MEM_FACTMEM_ADMIN_REGISTER] << 8 | gRwBuf[MEM_FACTMEM_ADMIN_REGISTER + 1];                           //本地管理员注册状态
        pDat->adminFingerNum = gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALL];                    //本地管理员指纹数量
        pDat->generalFingerNum = gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALL];                  //本地用户指纹数量
        return 0;                                                                                                                         //成功                                                                                                                        //成功
    }
    else
    {
        return -3; //读取错误
    }
}
/***************************************************************************************
**函数名:       HAL_Storage_SearchEmptyFingerLocalAdmin
**功能描述:     查找空位指纹头本地管理员
**输入参数:     --
**输出参数:     
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFingerLocalAdmin(void)
{
    return HAL_Storage_SearchEmpty(0,
                                   MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MSG_FINGER_REG_MEM_LOCAL_ST,
                                   MSG_FINGER_ONE_SIZE);
}
/***************************************************************************************
**函数名:       HAL_Storage_SearchEmptyFingerLocalGeneral
**功能描述:     查找空位指纹头本地普通用户
**输入参数:     --
**输出参数:     
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFingerLocalGeneral(void)
{
    return HAL_Storage_SearchEmpty(MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MEM_FINGER_GUEST_ALL_LOCAL + MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MSG_FINGER_REG_MEM_LOCAL_ST,
                                   MSG_FINGER_ONE_SIZE);
}
/***************************************************************************************
**函数名:       HAL_Storage_SaveOneFingerRegLocal
**功能描述:     保存一个指纹数据本地
**输入参数:     
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneFingerRegLocal(TYPEs_StorageSave *msg)
{
    INT32U addrTmp = 0;
    //    HAL_Storage_FingerLeaveNumSet(eFingerNumModDel, &(msg->fingerNum)); //更新剩余数量
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msg->fingerNum.overgroupid,
                                                                      MSG_FINGER_REG_MEM_LOCAL_ST,
                                                                      MSG_FINGER_ONE_SIZE); //获取上一个指纹的down id地址
    HAL_Storage_Write16Bit(addrTmp, msg->finger.pageID);                                    //修改上一个指纹的down id
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_LOCAL_ST,
                                           MSG_FINGER_ONE_SIZE); //获取保存地址
    HAL_Storage_WriteOneReg(addrTmp, &(msg->finger));            //保存一个指纹信息
    msg->fingerNum.overpageid = msg->finger.pageID;              //末尾pageID

    //   HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //写入末尾pageID
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_DelOneFingerRegLocal
**功能描述:     删除一个指纹数据本地
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFingerRegLocal(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_LOCAL_ST,
                                           MSG_FINGER_ONE_SIZE);  //获取保存地址
    HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //获取指纹信息
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //清除有效值
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //写入有效值
    return 0;
    // addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
    //                                                                   MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                   MSG_FINGER_ONE_SIZE); //获取保存地址
    // HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //修改上一个指纹的链尾
    // if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //内容为空的情况
    // {
    //     msg->fingerNum.overpageid = msg->finger.pageID;                                           //末尾pageID
    //     HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //写入末尾pageID
    // }
    // else
    // {
    //     addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
    //                                                                     MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                     MSG_FINGER_ONE_SIZE); //获取保存地址
    //     HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //修改下一个指纹的链头
    // }
    // HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //更新剩余数量
}
/***************************************************************************************
**函数名:       HAL_Storage_DelAllFingerLocalGeneral
**功能描述:     删除所有的本地普通指纹
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocalGeneral(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    INT16U i;
    for (i = MEM_FINGER_ADMIN_ALL_LOCAL;
         i < (MEM_FINGER_ADMIN_ALL_LOCAL + MEM_FINGER_GUEST_ALL_LOCAL);
         i++)
    {
        msg->finger.pageID = i;
        HAL_Storage_DelOneFingerRegLocal(msg);
    }
    pDat->generalFingerNum = 0;
    HAL_Storage_SaveGeneralFingerNum(pDat->generalFingerNum); //更新普通指纹数量
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_DelAllFingerLocalAdmin
**功能描述:     删除所有的本地管理指纹
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocalAdmin(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    INT16U i;
    for (i = MEM_FINGER_ADMIN_START_ID;
         i < (MEM_FINGER_ADMIN_START_ID + MEM_FINGER_ADMIN_ALL_LOCAL);
         i++)
    {
        msg->finger.pageID = i;
        HAL_Storage_DelOneFingerRegLocal(msg);
    }
    pDat->adminFingerNum = 0;
    HAL_Storage_SaveGeneralFingerNum(pDat->adminFingerNum); //更新普通指纹数量
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_DelAllFingerLocal
**功能描述:     删除所有的本地指纹
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
TYPEs_SystemDat *pDat 系统该参数
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocal(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    HAL_Storage_DelAllFingerLocalAdmin(msg, pDat);   //清空所有管理指纹
    HAL_Storage_DelAllFingerLocalGeneral(msg, pDat); //清空所有普通指纹
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_SavePwd
**功能描述:     保存用户密码
**输入参数:    
const INT8U *dat, 数据
**输出参数:     
INT8S 0,成功
-1; //参数错误
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SavePwd(const INT8U *dat)
{
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    memcpy(gRwBuf, dat, MEM_BOARDPWD_PWDLEN); //拷贝数据
    HAL_Storage_Rw(eStorageDireWrite,
                   (MEM_BOARDPWD + MEM_BOARDPWD_PWDLEN),
                   gRwBuf,
                   MEM_BOARDPWD_PWDLEN);
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_ReadPwd
**功能描述:     保存用户密码
**输入参数:    
const INT8U *dat, 数据
**输出参数:     
INT8S 0,成功
-1; //参数错误
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ReadPwd(INT8U *dat)
{
    if (NULL == dat)
    {
        return -1; //参数错误
    }
    HAL_Storage_Rw(eStorageDireRead,
                   (MEM_BOARDPWD + MEM_BOARDPWD_PWDLEN),
                   gRwBuf,
                   MEM_BOARDPWD_PWDLEN);
    memcpy(dat, gRwBuf, MEM_BOARDPWD_PWDLEN); //拷贝数据
    return 0;
}


/***************************************************************************************
**函数名:       HAL_Storage_DelOneFingerReg
**功能描述:     删除一个指纹数据
**输入参数:    
TYPEs_StorageSave *msg 指纹头保存控制结构
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFaceReg(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    if (NULL == msg)
    {
        return -1; //参数错误
    }
    if (msg->sysDat.leaveNumFaceApp >= MEM_FACE_ALL) //没有空闲剩余
    {
        return -2; //用户数量为空
    }
    msg->sysDat.leaveNumFaceApp++;
    HAL_Storage_WriteSysData(&msg->sysDat); //更新数据
    addrTmp = HAL_Storage_GetAddressWithId(msg->UserDat.pageID,
                                           MSG_FACE_REG_MEM_ST,
                                           MSG_FACE_REG_SIZE);    //获取保存地址
                                                                  //   HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //获取指纹信息
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //清除有效值
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //写入有效值

    // addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
    //                                                                   MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                   MSG_FINGER_ONE_SIZE); //获取保存地址
    // HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //修改上一个指纹的链尾
    // if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //内容为空的情况
    // {
    //     msg->fingerNum.overpageid = msg->finger.pageID;                                           //末尾pageID
    //     HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //写入末尾pageID
    // }
    // else
    // {
    //     addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
    //                                                                     MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                     MSG_FINGER_ONE_SIZE); //获取保存地址
    //     HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //修改下一个指纹的链头
    // }
    // HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //更新剩余数量
    return 0;
}

/***************************************************************************************
**函数名:       HAL_Storage_WriteSysData
**功能描述:     写入用户数据
**输入参数:     
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_WriteSysData(TYPEs_StorageSysData *msg)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR,
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageSysData)); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ReadSysData
**功能描述:     读取用户数据
**输入参数:     
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ReadSysData(TYPEs_StorageSysData *msg)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR,
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageSysData)); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ChkSysData
**功能描述:     检查用户数据
**输入参数:     --
**输出参数:     
INT8S 
0,成功
-1; //失败
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ChkSysData(void)
{
    #if 0
    memset((uint8_t *)&storageSave.sysDat, 0XFF, sizeof(storageSave.sysDat));
     return -1; //失败 
    #endif
    memset((uint8_t *)&storageSave.sysDat, 0, sizeof(storageSave.sysDat));
    HAL_Storage_ReadSysData(&storageSave.sysDat); //读取系统参数
    if ((MSG_FACTMEM_FACTORY_FIG_OK == storageSave.sysDat.factoryFlag) &&
        (MSG_FACTMEM_FACTORY_FIG_OK == storageSave.sysDat.factoryFlag2))
    {
        return 0; //成功
    }
    else
    {
        return -1; //失败
    }
}
/***************************************************************************************
**函数名:       HAL_Storage_ClearSysData
**功能描述:     清除用户数据
**输入参数:     --
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ClearSysData(void)
{
    memset((uint8_t *)&storageSave.sysDat, 0, sizeof(storageSave.sysDat));            //清空系统参数
    memset((uint8_t *)&storageSave.sysDat.pwd, 0x0, sizeof(storageSave.sysDat.pwd)); //设置默认密码
	storageSave.sysDat.factoryFlag = MSG_FACTMEM_FACTORY_FIG_OK;
	storageSave.sysDat.factoryFlag2 = MSG_FACTMEM_FACTORY_FIG_OK;
	storageSave.sysDat.allFinger = MEM_FINGER_ALL_ONLINE;
	storageSave.sysDat.leaveFinger = MEM_FINGER_ALL_ONLINE;
	storageSave.sysDat.allPhone = MEM_PHONE_ALL; 
	storageSave.sysDat.leavePhone = MEM_PHONE_ALL; 
	storageSave.sysDat.allSmartKey = MEM_SMARTKEY_ALL;
	storageSave.sysDat.leaveSmartKey = MEM_SMARTKEY_ALL;
	storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_NONE;
	storageSave.sysDat.voiceSw = MSG_FACTMEM_VOICE_OPEN;
	storageSave.sysDat.openLog = MSG_FACTMEM_LOCKLOG_ALL;
	storageSave.sysDat.openLogSn = MSG_FACTMEM_LOCKLOG_SN;
	storageSave.sysDat.leaveNumFaceApp = MEM_FACE_ALL_ONLINE;
	storageSave.sysDat.irDef = 0; //主动防御的参数
	#ifndef LOCK_OPEN_PARM_NO_CLEAR
	storageSave.sysDat.torque[0]=MOTOR_TORQUE_LOW;
	#endif
	HAL_Storage_WriteSysData(&storageSave.sysDat);                                    //保存系统参数
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_SaveOneUserReg
**功能描述:     保存一个用户信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg)
{
    INT8S reTmp = 0;
    INT32U addr;
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_WriteOneUserReg(addr, msg);
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Storage_GetOneUserReg
**功能描述:     获取一个用户信息
**输入参数:     
INT32U addr, 地址
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_GetOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg)
{
    INT8S reTmp = 0;
    INT32U addr;
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_ReadOneUserReg(addr, msg);
    return reTmp;
}

/***************************************************************************************
**函数名:       HAL_Storage_SearchEmpty
**功能描述:     查找空位
**输入参数:     
INT16U StartId, //开始地址
INT16U endId //结束地址
**输出参数:     
INT16U 获取到的空闲位置,0XFFFF为未查询到
**备注:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmpty(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize)
{
    INT16U i;
    for (i = StartId; i < endId; i++)
    {
        HAL_Storage_Rw(eStorageDireRead,
                       HAL_Storage_GetAddressWithId(i, StartAddress, oneSize),
                       gRwBuf,
                       1);
        if (MSG_FINGERREGMEM_FIG != gRwBuf[0]) //检查空闲标志
        {
            return i;
        }
    }
    return 0XFFFF;
}
/***************************************************************************************
**函数名:       HAL_Storage_MatchId
**功能描述:     匹配用户
**输入参数:     
INT16U StartId, 开始ID
INT16U endId,   结束地址
INT32U StartAddress, 开始物理地址
INT32U oneSize, 各条数据的占用空间大小
TYPEs_StorageUserDat *msg //用户数据,注意在外面需要配置pageID用于匹配数据
**输出参数:     
INT8S 
0; //匹配成功
-1; //匹配失败
**备注:         
****************************************************************************************/
INT16S HAL_Storage_MatchId(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize, TYPEs_StorageUserDat *msg,TYPEe_MatchType matchType)
{
    INT16S i,j;
    INT32U addr;
    TYPEs_StorageUserDat msgTmp;
    for (i = StartId; i < endId; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i, StartAddress, oneSize);
        HAL_Storage_Rw(eStorageDireRead,
                       addr,
                       gRwBuf,
                       1);
        if (MSG_FINGERREGMEM_FIG == gRwBuf[0]) //检查空闲标志
        {
            HAL_Storage_ReadOneUserReg(addr, &msgTmp);
			switch (matchType)
            {
            	case eMatchTypeGroup:
					if (msgTmp.group == msg->group)
					{
						memcpy(msg, &msgTmp, sizeof(msgTmp)); //拷贝数据
						return i;                             //匹配成功
					}
            		break;
				case eMatchTypeUserID:
					for(j=0;j<sizeof(msgTmp.UserID);j++)
                    {
						if(msgTmp.UserID[j] != msg->UserID[j])
						{
							break;
						}
                    }
					if(sizeof(msgTmp.UserID)  == j)
					{
						memcpy(msg, &msgTmp, sizeof(msgTmp)); //拷贝数据
						return i;
					}
            		break;	
            	default:
            		break;
            }
            
        }
    }
    return -1; //匹配失败
}

/***************************************************************************************
**函数名:       HAL_Storage_DelOneUserReg
**功能描述:     保存一个用户信息
**输入参数:     
INT16U ID, 用户ID
INT32U StartAddress, 物理开始地址  
INT32U oneSize 单个数据占用的空间
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_DelOneUserReg(INT16U ID, INT32U StartAddress, INT32U oneSize)
{
    INT8S reTmp = 0;
    INT32U addr;
    TYPEs_StorageUserDat msg;
    memset(&msg, 0, sizeof(msg)); //清除数据
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_WriteOneUserReg(addr, &msg);
    return reTmp;
}
/***************************************************************************************
**函数名:       HAL_Storage_DelNUserReg
**功能描述:     删除N个数据
**输入参数:     
INT16U StartId, 开始ID
INT16U endId,   结束地址
INT32U StartAddress, 开始物理地址
INT32U oneSize, 各条数据的占用空间大小
**输出参数:     
INT8S 
0; //成功
**备注:         
****************************************************************************************/
INT8S HAL_Storage_DelNUserReg(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize)
{
    INT32U addr;
    INT16U i = 0;
    TYPEs_StorageUserDat msg;
    memset(&msg, 0, sizeof(msg)); //清除数据
    for (i = StartId; i < endId; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i, StartAddress, oneSize);
        HAL_Storage_WriteOneUserReg(addr, &msg);
    }
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_EnterSleepConfig
**功能描述:     进入休眠
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Storage_EnterSleepConfig(void)
{
    DRV_Storage_EnterSleepConfig();
}

/***************************************************************************************
**函数名:       HAL_Storage_WriteSysData
**功能描述:     写入用户数据
**输入参数:     
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_WriteHardwareData(TYPEs_StorageHardwareData *msg)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR+sizeof(TYPEs_StorageSysData),
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageHardwareData)); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ReadSysData
**功能描述:     读取用户数据
**输入参数:     
TYPEs_StorageUserDat *msg 数据结构体
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ReadHardwareData(TYPEs_StorageHardwareData *msg)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR+sizeof(TYPEs_StorageSysData),
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageHardwareData)); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_WriteLockLog
**功能描述:     写入事件记录
**输入参数:     
uint8_t *msg,
uint32_t id
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_WriteLockLog(uint8_t *msg,uint32_t id)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MSG_FACTMEM_LOCKLOG_LINK+id*MSG_LOCKLOGMEM_ONCE,
                   (INT8U *)msg,
                   8); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ReadLockLog
**功能描述:     读取事件记录
**输入参数:     
uint8_t *msg,
uint32_t id
**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ReadLockLog(uint8_t *msg,uint32_t id)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MSG_FACTMEM_LOCKLOG_LINK+id*MSG_LOCKLOGMEM_ONCE,
                   (INT8U *)msg,
                   8); //写入数据
    return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_ClearUserData
**功能描述:     清除所有的用户数据
**输入参数:     

**输出参数:     
INT8S 0,成功
**备注: 
****************************************************************************************/
INT8S HAL_Storage_ClearUserData(void)
{
	INT32U i;
	uint8_t test1=0xaa;
	uint32_t addr ;
	HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR,
                   &test1,
                   1); //这里不知道为啥需要先读取一下，才能擦除，这个问题以后在查。
	for(i=0;i<5;i++)
   {
		DRV_Storage_EraseSector((MEM_FACTMEM_START_ADDR/4096)+i);
    }
	DEBUG_LOG_OUT("HAL_Storage_ClearUserData is:%d\n",test1);
	return 0;
}
/***************************************************************************************
**函数名:       HAL_Storage_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void HAL_Storage_Init(void)
{
	DRV_Storage_Init();
	//给存储设备上电等
}

#endif /*HAL_STORAGE_MODULE_EN*/
/************************************Code End******************************************/
