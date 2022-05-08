/***************************************************************************************
**文件名:     HAL_Storage.h
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
#ifndef __HAL_STORAGE_H__
#define __HAL_STORAGE_H__
#include "MyConfig.h"
#define HAL_STORAGE_MODULE_EN (1) //模块使能
#if HAL_STORAGE_MODULE_EN
/*************************专用宏定义*************************/
#define HAL_STORAGE_SYS_DATA_PWD_SIZE (8)  //系统参数中密码的长度
#define HAL_STORAGE_SYS_TORQUE_SIZE (3)    //系统参数中的扭力长度
#define HAL_STORAGE_SYS_MODEL_SIZE (8)     //系统参数中的锁具型号长度
#define HAL_STORAGE_SYS_FACE_UID_SIZE (20) //系统参数中的face uid长度
#define MOTOR_TORQUE_HIGH (2) //高电机扭力
#define MOTOR_TORQUE_LOW (1) //低电机扭力
#define HAL_STORAGE_SYS_VIOCE_SW_OPEN (3)  //语音开
#define HAL_STORAGE_SYS_VIOCE_SW_CLOSE (0) //语音关
#define HAL_STORAGE_SYS_ALL_USER_MEM_SIZE (20) //所有用户的存储大小,单位为KB
#define HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE (10)  //临时密码组数
#define HAL_STORAGE_PWD_LEN (6)  //临时密码长度
/*********************************************************************************
 * 指纹结构
 * ********************************************************************************/
#define LOCK_FUNCTION_TIMEVAILD_ON                            //时效功能开启
#define LOCK_FUNCTION_KINDRED_ON                              //亲情功能开启
#define LOCK_FUNCTION_SOS_ON                                  //报警功能开启
#define LOCK_FUNCTION_FINGERPWD_ON                            //单用户密码
#define FLM_CONFIG_MEM_CARD                                   //卡
#define HAL_STORAGE_VAILD_ADDR (0)                            //指纹有效
#define HAL_STORAGE_UP_ID_ADDR (1)                            //上一编号H
#define HAL_STORAGE_DOWN_ID_ADDR (3)                          //下一编号H
#define HAL_STORAGE_LIM_ADDR (5)                              //类型
#define HAL_STORAGE_GROUP_ADDR (6)                            //组号H
#define HAL_STORAGE_NAME_ADDR (8)                             //组号H
#define HAL_STORAGE_PAGE_ID_ADDR (14)                         //当前编号H
#define HAL_STORAGE_PWD_FIG_ADDR (16)                         //密码标志
#define HAL_STORAGE_PWD_ADDR (17)                             //密码
#define HAL_STORAGE_TM_VAILD_FIG_ADDR (20)                    //时效标志
#define HAL_STORAGE_TM_VAILD_STAR_ADDR (21)                   //时效开始
#define HAL_STORAGE_TM_VAILD_END_ADDR (27)                    //时效结束
#define HAL_STORAGE_TM_VAILD_WDAY_ADDR (33)                   //周周时效
#define HAL_STORAGE_KINDRED_ADDR (34)                         //亲情标志
#define HAL_STORAGE_SOS_ADDR (35)                             //SOS胁迫标志
#define HAL_STORAGE_CHKSUM_ADDR (36)                          //校验H
#define HAL_STORAGE_CHKSUM_LEN (2)                            //校验和的长度
#define HAL_STORAGE_ONE_FINGER_SIZE (MSG_FINGER_REG_ONE_SIZE) //一个指纹数据占用的空间

#define MEM_FINGER_FACE_ADMIN_LOCAL 5                            //5个用户
#define MEM_FINGER_FACE_GUEST_LOCAL 15                           //15个用户
#define MEM_FINGER_ADMIN_START_ID (0)                            //管理指纹起始id
#define MEM_FINGER_ADMIN_ALL_LOCAL (5)                           //5个管理指纹用户
#define MEM_FINGER_GENERAL_START_ID (MEM_FINGER_ADMIN_ALL_LOCAL) //普通指纹起始id
#define MEM_FINGER_GUEST_ALL_LOCAL (20)                          //普通指纹用户
#define MEM_FACE_ALL_ONLINE		  (20)
/*********************************************************************************
 * 系统的存储结构
 * ********************************************************************************/
#define MEM_FACTMEM_START_ADDR (0x300000)        //存储开始地址
//#define MEM_FACTMEM_START_ADDR (0x000000)        //存储开始地址
#define MEM_FACT_MEM_SIZE (128)                  //存储区大小
#define MEM_FACTMEM_FACTORY_FIG 0                //工厂标志  G
#define MEM_FACTMEM_LOCKMODE 1                   //开锁模式（指纹/密码蓝牙/蓝牙钥匙）
#define MEM_FACTMEM_FINGER_ALL_ONLINEH 2         //全部指纹H（60*3）
#define MEM_FACTMEM_FINGER_ALL_ONLINEL 3         //全部指纹L（60*3）
#define MEM_FACTMEM_FINGER_LEAVE_ONLINEH 4       //剩余可用指纹H（60*3）
#define MEM_FACTMEM_FINGER_LEAVE_ONLINEL 5       //剩余可用指纹L（60*3）
#define MEM_FACTMEM_PHONE_ALLH 6                 //ALL卡H
#define MEM_FACTMEM_PHONE_ALLL 7                 //ALL卡L
#define MEM_FACTMEM_PHONE_LEAVEH 8               //LEAVE卡H
#define MEM_FACTMEM_PHONE_LEAVEL 9               //LEAVE卡L
#define MEM_FACTMEM_SMARTKEY_ALLH 10             //全部蓝牙钥匙数量H （20个）
#define MEM_FACTMEM_SMARTKEY_ALLL 11             //全部蓝牙钥匙数量L  （20个）
#define MEM_FACTMEM_SMARTKEY_LEAVEH 12           //剩余蓝牙钥匙数量H
#define MEM_FACTMEM_SMARTKEY_LEAVEL 13           //剩余蓝牙钥匙数量L
#define MEM_FACTMEM_FINGER_ADMIN_LOCALH 14       //本地管理员指纹数量H
#define MEM_FACTMEM_FINGER_ADMIN_LOCALL 15       //本地管理员指纹数量L
#define MEM_FACTMEM_FINGER_GUEST_LOCALH 16       //本地用户指纹数量H
#define MEM_FACTMEM_FINGER_GUEST_LOCALL 17       //本地用户指纹数量L
#define MEM_FACTMEM_WIFI_MAIN_SW 18              //WIFI主开关
#define MEM_FACTMEM_WIFI_LOGUP_SW 19             //WIFI开门信号上传开关
#define MEM_FACTMEM_ADMIN_REGISTER 20            //本地管理员是否注册H \
                                                 //本地管理员是否注册L
#define MEM_FACTMEM_VOICE 22                     //语音开关
#define MEM_DH_G 23                              //dh原根
#define MEM_DH_P 24                              //DH质数
#define MEM_FACTMEM_DONE_FIG 25                  //工厂标志  G
#define MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH 26 //最后指纹ID  H（注册或者删除才用）最后用户地址
#define MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL 27 //最后指纹ID  L（注册或者删除才用）
#define MEM_FACTMEM_SMARTKEYOVER_PAGEIDH 28      //最后一个蓝牙钥匙存储位置H（注册或者删除才用）
#define MEM_FACTMEM_SMARTKEYOVER_PAGEIDL 29      //最后一个蓝牙钥匙存储位置L（注册或者删除才用）
#define MEM_FACTMEM_CARDOVER_PAGEIDH 30          //卡位置ID  H（注册或者删除才用）
#define MEM_FACTMEM_CARDOVER_PAGEIDL 31          //卡位置ID L（注册或者删除才用）
#define MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALH 32 //最后一个组号H（注册或者删除才用）组管理, 最后一个组号.
#define MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALL 33 //最后一个组号L（注册或者删除才用）组管理, 最后一个组号.
#define MEM_FACTMEM_LOCKLOG_LINK_ST 34           //开锁记录4字节
#define MEM_FACTMEM_LOCKLOG_LINK_2 35            //开锁记录4字节
#define MEM_FACTMEM_LOCKLOG_LINK_3 36            //开锁记录4字节
#define MEM_FACTMEM_LOCKLOG_LINK_OV 37           //开锁记录4字节
#define MEM_FACTMEM_LOCKLOG_SN_H 38              //开锁记录序列号H
#define MEM_FACTMEM_LOCKLOG_SN_L 39              //开锁记录序列号L
#define MEM_FACTMEM_ADMIN_PWD1 40                //密码
#define MEM_FACTMEM_ADMIN_PWD2 41                //密码
#define MEM_FACTMEM_ADMIN_PWD3 42                //密码
#define MEM_FACTMEM_ADMIN_PWD4 43                //密码
#define MEM_FACTMEM_ADMIN_PWD5 44                //密码
#define MEM_FACTMEM_ADMIN_PWD6 45                //密码
#define MEM_FACTMEM_ADMIN_PWD7 46                //密码
#define MEM_FACTMEM_ADMIN_PWD8 47                //密码
#define MEM_EEPROM_TEST_L 48                     //EE测试
#define MEM_EEPROM_TEST_H 49                     //EE测试
#define MEM_FINGER_AES_EN 51                     //指纹AES使能
#define MEM_FINGER_AES_MANUFACTUER 52            //指纹AES厂家
#define MEM_FACTMEM_EEPROM_LOGO 64               //logo 8字节
#define MEM_FACTMEM_EEPROM_MODEL 72              //锁具型号 8字节
#define MEM_FACTMEM_FINGER_ASEKEY 88             //指纹头AES根秘钥16字节
#define MEM_LOCK_SN_CODE (120)                   //用户SN 8字节
#define AUDIO_SWITCH 0x19F                       //语音开关
#define MEM_BOARDINFO 0x200                      //出厂信息区域备份地址 未使用
#define MSG_BOARDINFO_LEN 50                     //出厂信息区域备份地址长度 未使用
#define MEM_BOARDPWD (0x280)                     //密码
#define MEM_BOARDPWD_PWDLEN 8
#define MEM_BOARDPWD_PWDSOSLEN 8
#define MEM_BOARDPWD_PWDTMP_ALL 10
#define MEM_BOARDPWD_PWDTMP_LEN 8
/*********************************************************************************
 * REG2   起始位置为768(0x300) 目前的范围是768 ~ (768+64*180 = 0x3000)
 * ********************************************************************************/
#define MSG_FINGER_REG_MEM_ONLINE_ST (MEM_FACTMEM_START_ADDR + 0x0300) //指纹注册起始地址 <<注:最后一个字节为00>>
#define MSG_FINGER_REG_ONE_SIZE 38                                     //单指纹所用字节数量
#define MSG_FINGER_ONE_SIZE 64                                         //单指纹实际空间
#define MEM_FINGER_ALL_ONLINE (60 * 3)                                 //180个指纹. 60个用户
/*********************************************************************************
 * local fig   起始位置为12544(0X3100) 目前的范围是0X3100 ~ 12544+64*40 = 0x3B00
 * ********************************************************************************/
#define MSG_FINGER_REG_MEM_LOCAL_ST (((MSG_FINGER_REG_MEM_ONLINE_ST + (MEM_FINGER_ALL_ONLINE)*MSG_FINGER_ONE_SIZE) & 0xFFFFFF00) + 0x0100)
#define MSG_FINGER_REG_ONE_SIZE 38 //单指纹所用字节数量
#define FINGER_LOCAL_MULTI 2
#define MEM_FINGER_ALL_LOCAL (20 * FINGER_LOCAL_MULTI) //40个指纹, 20个用户
/*********************************************************************************
 * REG3 phone 起始位置为15360(0x3C00)    目前的范围是0x3C00 ~ 15360+64*150 = 0x6180
 * ********************************************************************************/
#define MSG_CARD_REG_MEM_ST (((MSG_FINGER_REG_MEM_LOCAL_ST + (MEM_FINGER_ALL_LOCAL)*MSG_FINGER_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //卡注册起始地址 <<注:最后一个字节为00>>//F00
#define MSG_CARD_REG_ONE_SIZE (38 + 6 + 7)                                                                                       //单卡所用字节数量
#define MSG_CARD_REG_SIZE (64)
#define MEM_PHONE_ALL 150 //单卡实际空间
/*********************************************************************************
 * REG4 SMARTKEY 起始位置为25088(0X6200)    目前的范围是0X6200 ~ 25088+64*20 = 0x6700
 * ********************************************************************************/
#define MSG_SMARTKEY_REG_MEM_ST (((MSG_CARD_REG_MEM_ST + MEM_PHONE_ALL * MSG_CARD_REG_SIZE) & 0xFFFFFF00) + 0x0100) //卡注册起始地址 <<注:最后一个字节为00>>
#define MSG_SMARTKEY_REG_ONE_SIZE (38 + 6 + 7)
#define MSG_SMARTKEY_REG_SIZE (64) //单卡实际空间
#define MEM_SMARTKEY_ALL 20        //20个智能钥匙.

/*********************************************************************************
 * REG5FACE 起始位置为26,624(0X6800)    目前的范围是0X6800 ~ 26,624+64*20 = 0X6D00
 * ********************************************************************************/
#define MSG_FACE_REG_MEM_ST (((MSG_CARD_REG_MEM_ST + MEM_SMARTKEY_ALL * MSG_SMARTKEY_REG_SIZE) & 0xFFFFFF00) + 0x0100) //卡注册起始地址 <<注:最后一个字节为00>>
#define MSG_FACE_REG_ONE_SIZE (38 + 6 + 7)
#define MSG_FACE_REG_SIZE (64) //单卡实际空间
#define MEM_FACE_ALL 20        //20个人脸

/*********************************************************************************
 *  LOG 起始位置为28,160(0X6E00)    目前的范围是0X6E00 ~ 28,160+17*600 = 0x90D8
 * ********************************************************************************/
#define MSG_FACTMEM_LOCKLOG_LINK (((MSG_FACE_REG_MEM_ST + MEM_FACE_ALL * MSG_FACE_REG_SIZE) & 0xFFFFFF00) + 0x0100) //计算 <<注:最后一个字节为00>>
#define MSG_LOCKLOGMEM_ONCE 16                                                                                      //单条记录占用的空间
#define MEM_LOCKLOG_ALL 600                                                                                         //
/*********************************************************************************
 * ID2_TOKEN_ST 
 * ********************************************************************************/
#define MSG_ID2_TOKEN_ST (((MSG_FACTMEM_LOCKLOG_LINK + MEM_LOCKLOG_ALL * MSG_LOCKLOGMEM_ONCE) & 0xFFFFFF00) + 0x0100) //TOKEN
/*********************************************************************************
 * ID2_ONorOFF_ST 
 * ********************************************************************************/
#define MSG_ID2_ONorOFF_ST (MSG_ID2_TOKEN_ST & 0xFFFFFF00) + 0x0100 //启用或者禁用ID2业务
/*********************************************************************************
 * MSG_ID2_id_ST 
 * ********************************************************************************/
#define MSG_ID2_id_ST (MSG_ID2_ONorOFF_ST + 1) //保存ID2的ID，共25个字节，第一个字节是标志位，1表示有效, 0 表示无效
/*********************************************************************************
 * FLAG_ST 
 * ********************************************************************************/
#define NFC_FLAG_ST (MSG_ID2_id_ST + 10) //保存NFC使能标志
/*********************************************************************************
 * LOCK_ONE_CLICK_DEFENE_MEM_ST 
 * ********************************************************************************/
#define LOCK_ONE_CLICK_DEFENE_MEM_ST (NFC_FLAG_ST + 1) //一键布防标准
/*********************************************************************************
 * ROOT_KEY_MEM_ST   
 * ********************************************************************************/
#define ROOT_KEY_MEM_ST (LOCK_ONE_CLICK_DEFENE_MEM_ST + 10) //根秘钥序号


// #ifdef M_CARD_FUNCTION
// #ifdef CPU_CARD_ENABLE
// #define MEM_CARD_ALL 30
// #define MSG_CPU_CARD_REG_ONE_SIZE 275 //274//CPU卡 ID 秘钥等
// #ifdef T11H
// #define MSG_CARD_REG_ONE_SIZE MSG_CARD_REG_SIZE //MSG_FINGER_REG_ONE_SIZE//单卡用户所用字节数量    <实际使用少于38,方便于finger统一>
// #else

// #endif
// //MSG_CPU_CARD_REG_MEM_ST (0x9200)
// #define MSG_CPU_CARD_REG_MEM_ST ((ROOT_KEY_MEM_ST & 0xFFFFFF00) + 0x0100)
// #define MSG_CPU_CARD_SE_CUR_KEY_ST (((MSG_CPU_CARD_REG_MEM_ST + MEM_CARD_ALL * MSG_CPU_CARD_REG_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //计算 <<注:最后一个字节为00>>
// #define MSG_CPU_CARD_NUM_ST (MSG_CPU_CARD_SE_CUR_KEY_ST + 1)                                                                      //华为钱包门卡数量
// #else
// #define MEM_CARD_ALL 100
// #define MSG_CARD_REG_ONE_SIZE MSG_FINGER_REG_ONE_SIZE //单卡用户所用字节数量    <实际使用少于38,方便于finger统一>
// #endif
// #else
// #define MSG_FACTMEM_LOCKLOG_LINK (((MSG_FINGER_REG_MEM_ST + MEM_FINGER_ALL * MSG_FINGER_REG_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //计算 <<注:最后一个字节为00>>
// #endif
/*********************************************************************************
 * 以下为系统的默认值  
 * ********************************************************************************/
#define MSG_FINGERREGMEM_FIG ('W')
#define MSG_FINGERREGMEM_PID_MASTER ('M')
#define MSG_FINGERREGMEM_PID_GUEST ('G')
#define MSG_FACTMEM_FACTORY_FIG_OK ('W')
#define MSG_FACTMEM_FACTORY_CLEAR ('N')
#define MSG_FACTMEM_CHANNEL_PWD1 0
#define MSG_FACTMEM_CHANNEL_PWD2 0
#define MSG_FACTMEM_CHANNEL_PWD3 0
#define MSG_FACTMEM_CHANNEL_PWD4 0
#define MSG_FACTMEM_ADMIN_PWD1 0x38
#define MSG_FACTMEM_ADMIN_PWD2 0x38
#define MSG_FACTMEM_ADMIN_PWD3 0x38
#define MSG_FACTMEM_ADMIN_PWD4 0x38
#define MSG_FACTMEM_ADMIN_PWD5 0x38
#define MSG_FACTMEM_ADMIN_PWD6 0x38
#define MSG_FACTMEM_ADMIN_PWD7 0x38
#define MSG_FACTMEM_ADMIN_PWD8 0x38
#define MSG_FACTMEM_DEVICENAME ' '
#define MSG_FACTMEM_FINGER_ALL_ONLINEH (INT8U)(MEM_FINGER_ALL_ONLINE >> 8)
#define MSG_FACTMEM_FINGER_ALL_ONLINEL (INT8U)(MEM_FINGER_ALL_ONLINE)
#define MSG_FACTMEM_FINGER_LEAVE_ONLINEH (INT8U)(MEM_FINGER_ALL_ONLINE >> 8) //默认剩余
#define MSG_FACTMEM_FINGER_LEAVE_ONLINEL (INT8U)(MEM_FINGER_ALL_ONLINE)
#define MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEH 0 //默认最后一个注册PAGEID为 0
#define MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEL 0

#define MSG_FACTMEM_FINGER_ALL_LOCALH (INT8U)(MEM_FINGER_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_ALL_LOCALL (INT8U)(MEM_FINGER_ALL_LOCAL)
#define MSG_FACTMEM_FINGER_LEAVE_LOCALH (INT8U)(MEM_FINGER_ALL_LOCAL >> 8) //默认剩余
#define MSG_FACTMEM_FINGER_LEAVE_LOCALL (INT8U)(MEM_FINGER_ALL_LOCAL)

#define MSG_FACTMEM_FINGER_ADMIN_ALL_LOCALH (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_ADMIN_ALL_LOCALL (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 0)

#define MSG_FACTMEM_FINGER_GUEST_ALL_LOCALH (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_GUEST_ALL_LOCALL (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 0)

#define MSG_FACTMEM_REGMSGOVER_PAGEID_LOCAL_START 0x00B4 //  180 本地指纹起始

#define MSG_FACTMEM_LOCKLOG_LINK_ST 0 //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>24)
#define MSG_FACTMEM_LOCKLOG_LINK_2 0  //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>16)
#define MSG_FACTMEM_LOCKLOG_LINK_3 0  //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>8)
#define MSG_FACTMEM_LOCKLOG_LINK_OV 1 //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK) 记录序号,到最大值后不变
#define MSG_FACTMEM_LOCKLOG_SN_H 0    //默认开锁记录序列号为0
#define MSG_FACTMEM_LOCKLOG_SN_L 1
#define MSG_FACTMEM_LOCKLOG_SN (INT16U)(1)
#define MSG_FACTMEM_LOCKLOG_ALL (INT32U)(1)
#define MSG_FACTMEM_PHONE_ALLH (INT8U)(MEM_PHONE_ALL >> 8)
#define MSG_FACTMEM_PHONE_ALLL (INT8U)(MEM_PHONE_ALL)
#define MSG_FACTMEM_PHONE_LEAVEH MSG_FACTMEM_PHONE_ALLH
#define MSG_FACTMEM_PHONE_LEAVEL MSG_FACTMEM_PHONE_ALLL
#define MSG_FACTMEM_PHONEOVER_PAGEIDH 0
#define MSG_FACTMEM_PHONEOVER_PAGEIDL 0
#define MSG_FACTMEM_SMARTKEY_ALLH (INT8U)(MEM_SMARTKEY_ALL >> 8)
#define MSG_FACTMEM_SMARTKEY_ALLL (INT8U)(MEM_SMARTKEY_ALL)
#define MSG_FACTMEM_SMARTKEY_LEAVEH MSG_FACTMEM_SMARTKEY_ALLH
#define MSG_FACTMEM_SMARTKEY_LEAVEL MSG_FACTMEM_SMARTKEY_ALLL
#define MSG_FACTMEM_SMARTKEYOVER_PAGEIDH 0
#define MSG_FACTMEM_SMARTKEYOVER_PAGEIDL 0

#define MSG_FACTMEM_REG_PAGEID_NULL (0XFFFF) //指示位置为空
#define MSG_FACTMEM_REG_VAILD_NULL (0XFF)    //指示清空有效值

#define MSG_FACTMEM_REG_STATUS_NONE (0xA5B7) //无注册
#define MSG_FACTMEM_REG_STATUS_OK (0x0001)   //本地管理员已注册
#define MSG_FACTMEM_REG_STATUS_ING (0x0002)  //注册中
#define MSG_FACTMEM_LIM_ADMIN ('M')          //管理指纹
#define MSG_FACTMEM_LIM_GENERAL ('G')        //普通指纹
#define MSG_FACTMEM_LIM_PHONE ('P')        	 //手机
#if 1
#define MSG_FACTMEM_LOCKMODE_FINGER (0X01)   //指纹开门方式
#define MSG_FACTMEM_LOCKMODE_BLE (0X02)      //蓝牙开门方式
#define MSG_FACTMEM_LOCKMODE_SMARTKEY (0X04) //智能钥匙开门方式
#define MSG_FACTMEM_LOCKMODE_PWD (0X08)      //密码开门方式
#define MSG_FACTMEM_LOCKMODE_KEY (0X10)      //钥匙开门方式
#endif
#define MSG_FACTMEM_VOICE_OPEN (0X03) 		//语音打开 
/*************************变量结构声明*************************/
#pragma pack(1) //这里强制使用1字节对齐的方式
//typedef enum
//{
//    eVoiceSwClose=0, //关闭
//    eVoiceSwOpen=3,  //开启
//} TYPEe_VoiceSw;

typedef struct
{
    INT8U factoryFlag;                            //工厂标志  G
    INT8U openLockMode;                           //开锁模式（指纹/密码蓝牙/蓝牙钥匙）
    INT16U allFinger;                             //全部指纹
    INT16U leaveFinger;                           //剩余可用指纹
	INT16U allPhone;                               //全部手机号
    INT16U leavePhone;                             //剩余可用手机号
    INT16U allCard;                               //全部指纹
    INT16U leaveCard;                             //剩余可用指纹
    INT16U allSmartKey;                            //全部蓝牙钥匙(智能钥匙)
    INT16U leaveSmartKey;                          //剩余蓝牙钥匙(智能钥匙)
    INT16U localAdminNum;                         //本地管理员数量
    INT16U localUserNum;                          //本地用户数量
    INT8U wifiMainSw;                             //WIFI主开关
    INT8U wifiOpenUpdataSw;                       //WIFI开门信号上传开关
    INT16U localAdminRegFlag;                     //本地管理员是否注册
    INT8U voiceSw;                        		  //语音开关	
    INT8U factoryFlag2;                           //工厂标志  G
    INT16U lastFingerId;                          //最后指纹ID
    INT16U lastBlueKeyId;                         //最后一个蓝牙钥匙存储位置
    INT16U cradId;                                //卡位置ID注册或者删除才用
    INT16U lasetGroup;                            //最后一个组号 注册或者删除才用
    INT32U openLog;                               //开锁记录
    INT16U openLogSn;                             //开锁记录序列号
    INT8U pwd[HAL_STORAGE_SYS_DATA_PWD_SIZE];     //密码
	INT8U pwdSos[HAL_STORAGE_SYS_DATA_PWD_SIZE];  //SOS密码
    INT16U eeTest;                                //EE测试
    INT8U keyDef;                                 //一键布防
    INT8U irDef;                                  //主动防御
    INT8U bellDef;                                //门铃防护,防止锁门键误触
    INT8U speed;                                  //加速度开关
    INT8U autoLockTime;                           //自动上锁时间
    INT8U pwdSetFlag;                             //密码设置标志
    INT8U localAdminNumFace;                      //本地管理人脸
    INT8U localUserNumFace;                       //本地用户人脸
    INT8U leaveNumFaceApp;                        //剩余APP人脸
	INT8U faceNumApp;							  //APP人脸数量
    INT16U fingerAseKey;                          //AES KEY  
    INT16U IrDistance;							  //红外距离
	INT8U ShangTangCatEyeSingleDouble; 			  //商汤猫眼单双向切换
	INT8U tempPwd[HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE][HAL_STORAGE_SYS_DATA_PWD_SIZE];							//临时密码
	INT8U ID2Flag; //ID2标志
	INT8U ID2Data[24]; //ID2数据
} TYPEs_StorageSysData;                           //系统参数
typedef struct
{
	INT8U mac[6];                                 //MAC地址
    INT8U version[9];                             //后板版本号
    INT8U encryptKey[6];                          //会话密钥
	INT8U channelPwd[4];						  //信道密码
	INT16U dir;                                   //方向
    INT8U torque[HAL_STORAGE_SYS_TORQUE_SIZE];    //扭力调节
    INT8U model[HAL_STORAGE_SYS_MODEL_SIZE];      //锁具型号
    INT8U faceUID[HAL_STORAGE_SYS_FACE_UID_SIZE]; //人脸UID
	INT8U lockBodyType; //锁体类型
	INT8U dhG;                                    //dh原根
    INT8U dhP;                                    //DH质数
} TYPEs_StorageHardwareData; //系统硬件数据,只有在硬件清空时修改
typedef enum
{
    eStorageDireWrite, //写入
    eStorageDireRead,  //读取
} TYPEe_StorageDire;

typedef struct
{
    INT16U all;         // 180个
    INT16U leave;       // APP可用指纹
    INT8U master;       //无作用
    INT16U overpageid;  //最后一枚指纹的ID
    INT16U overgroupid; //本地指纹的结束组号.	0-65535
} TYPEs_StorageNum;     //指纹数量

typedef struct
{
    INT8U year;
    INT8U mon;
    INT8U day;
    INT8U hour;
    INT8U min;
    INT8U sec;
} TYPEs_StorageFingerTime; //时间

typedef struct
{
    INT8U fig;                   //使能标志
    TYPEs_StorageFingerTime str; //开始时间
    TYPEs_StorageFingerTime stp; //结束时间
    INT8U wday;                  //周周时效 BIT1 -BIT 7表示周一到周日
} TYPEs_StorageTimeVaild;        //时效

typedef struct
{
    INT8U fig;      //密码使能标志
    INT8U pwd[3];   //密码
} TYPEs_Storagepwd; //密码结构

typedef struct
{
    INT8U fingerLim;      //限制Limite 主人,还是客人?
    INT8U name[6];        //指纹名称
    INT16U down_pageID;   //下一个ID
    INT16U pageID;        //当前ID
    INT16U group;         //组别.
} TYPEs_StoragFingerName; // 指纹名

typedef struct
{
    INT8U vaild; //指纹有效
    // INT8U dis_sw;                 //显示开关
    INT16U sn;                    //序号
    INT16U up_pageID;             //上一个编号
    INT16U down_pageID;           //下一个编号
    INT8U Lim;                    //限制Limite 限制Limite 主人,还是客人?
    INT16U group;                 //组号
    INT8S name[6];                //名字
    INT16U pageID;                //当前编号
#ifdef LOCK_FUNCTION_FINGERPWD_ON //单用户密码
    TYPEs_Storagepwd pwd;         //密码结构
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON
    TYPEs_StorageTimeVaild TimeVaild; //时效
#endif
#ifdef LOCK_FUNCTION_KINDRED_ON
    INT8U kindred;
#endif
#ifdef LOCK_FUNCTION_SOS_ON
    INT8U sos;
#endif
    // #ifdef FLM_CONFIG_MEM_CARD
    //     INT8U cardid[15];
    // #endif
    INT16U checksum;   //校验和
} TYPEs_StorageFinger; //指纹属性
typedef struct
{
    INT8U vaild; //指纹有效
    // INT8U dis_sw;                 //显示开关
    INT16U sn;                    //序号
    INT16U up_pageID;             //上一个编号
    INT16U down_pageID;           //下一个编号
    INT8U Lim;                    //限制Limite 限制Limite 主人,还是客人?
    INT16U group;                 //组号
    INT8S name[6];                //名字
    INT16U pageID;                //当前编号
#ifdef LOCK_FUNCTION_FINGERPWD_ON //单用户密码
    TYPEs_Storagepwd pwd;         //密码结构
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON
    TYPEs_StorageTimeVaild TimeVaild; //时效
#endif
#ifdef LOCK_FUNCTION_KINDRED_ON
    INT8U kindred;
#endif
#ifdef LOCK_FUNCTION_SOS_ON
    INT8U sos;
#endif
#ifdef FLM_CONFIG_MEM_CARD
    INT8U UserID[13];
#endif
    INT16U checksum;    //校验和
} TYPEs_StorageUserDat; //用户信息
typedef enum
{
    eMatchTypeSn, //序号
    eMatchTypeGroup,  //组别
	eMatchTypeUserID, //用户ID
} TYPEe_MatchType;
typedef struct
{
    TYPEs_StorageSysData sysDat;       //系统参数
	TYPEs_StorageHardwareData hardwareData; //硬件参数
    TYPEs_StorageFinger finger;        //指纹属性
    TYPEs_StorageUserDat UserDat;      //用户数据
    TYPEs_StorageNum fingerNum;        //指纹数量
    TYPEs_StoragFingerName fingerName; //指纹用户名
} TYPEs_StorageSave;                   //指纹存储

typedef struct
{
    INT16U lockRegStatus;    //注册状态
    INT16U adminFingerNum;   //管理指纹数量
    INT16U generalFingerNum; //普通指纹数量
    INT8U timeKey[6];        //时间参数
    INT8U macKey[6];         //MAC
    INT8U reservedKey[6];    //预留
} TYPEs_SystemDat;           //系统参数
#pragma pack()               //取消对齐

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_STORAGE_GLOBALS
#define HAL_STORAGE_EXT
#else
#define HAL_STORAGE_EXT extern
#endif /*HAL_STORAGE_GLOBALS*/

    /*************************公共变量*************************/
    HAL_STORAGE_EXT TYPEs_StorageSave storageSave;
    /*************************公共函数*************************/
    // HAL_STORAGE_EXT INT8S HAL_Storage_ChkFingerNum(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFinger(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ReadOneFingerReg(TYPEs_StorageFinger *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneFingerReg(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelOneFingerReg(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ResetFingerStruct(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_EraseAllFingerReg(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_lockRegStatusGet(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_lockRegStatusSet(TYPEs_SystemDat *dat, INT8U status);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveLockRegStatus(INT16U dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerNumChk(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerIdChk(INT16U id);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveAdminFingerNum(INT16U dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_GeneralFingerNumChk(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveGeneralFingerNum(INT16U dat);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFingerLocalAdmin(void);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFingerLocalGeneral(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneFingerRegLocal(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelOneFingerRegLocal(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelAllFingerLocalGeneral(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SystemDataInit(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SystemDataRead(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelAllFingerLocal(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SavePwd(const INT8U *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ReadPwd(INT8U *dat);

    HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_GetOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_WriteSysData(TYPEs_StorageSysData *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_ReadSysData(TYPEs_StorageSysData *msg);
    HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmpty(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT16S HAL_Storage_MatchId(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize, TYPEs_StorageUserDat *msg,TYPEe_MatchType matchType);
    HAL_STORAGE_EXT INT8S HAL_Storage_DelOneUserReg(INT16U ID, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT8S HAL_Storage_DelNUserReg(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT8S HAL_Storage_ChkSysData(void);
    HAL_STORAGE_EXT INT8S HAL_Storage_ClearSysData(void);
    HAL_STORAGE_EXT void HAL_Storage_EnterSleepConfig(void);
	HAL_STORAGE_EXT INT8S HAL_Storage_WriteHardwareData(TYPEs_StorageHardwareData *msg);
	HAL_STORAGE_EXT INT8S HAL_Storage_ReadHardwareData(TYPEs_StorageHardwareData *msg);
	HAL_STORAGE_EXT INT8S HAL_Storage_WriteLockLog(uint8_t *msg,uint32_t id);
	HAL_STORAGE_EXT INT8S HAL_Storage_ReadLockLog(uint8_t *msg,uint32_t id);
	HAL_STORAGE_EXT INT8S HAL_Storage_ClearUserData(void);
	HAL_STORAGE_EXT void HAL_Storage_Init(void);
	HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerIdChk(INT16U id);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_STORAGE_MODULE_EN*/

#endif /*HAL_STORAGE_MODULE_EN*/
#endif /*__HAL_STORAGE_H__*/
/************************************Code End******************************************/
