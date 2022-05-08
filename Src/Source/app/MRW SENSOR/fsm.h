/*********************************************************************************************************
 ** file:	    fsm.h
 ** description:	头文件
 ** author:      li
 ** date :       2017/05/16
 ***********************************************************************************************************/

#ifndef __FSM_H__
#define __FSM_H__
#include "lib/CCM_Types.h"



//帧描述   帧头(2 Byte)  + 模块地址(0xffffffff) +  包标识(1Byte) + 包长度(2Bytes) + data(n Byte) + 校验(1Byte)
#define FRAME_START     0xEF01
#define FRAME_PID       0x06        //标识位
#define FRAME_STA_LEN   9           //


#define FRAME_LEN_HIGH  0x07
#define FRAME_LEN_LOW   0x08
#define FRAME_CRC 1


//标识
#define PID_COMMAND    0x01   //命令包
#define PID_DATA       0x02   //数据包
#define PID_LAST_DATA  0x08   //最后一包数据包
#define PID_ACK        0x07   //应答包
#define PID_DATA_ACK   0x09   //数据应答包

#define PID_DEBUG_PRINT 0x0a

#define FRAME_MOST_LEN 256




enum
{
	ENROLL =0x01 ,                    //注册指纹
	SERACH=0x04,                      //匹配指纹
	UPLOAD_IMAGE =0x0a,               //上传图像
	DOWN_IMAGE=0x0b,                  //下载图像
	Delete_ID=0x0c,                   //删除指纹id
	EMPTY_ID=0x0d,                    //清空指纹库
	SET_SYS_PARA=0x0e,                //设置系统参数
	GET_SYS_PARA=0x0f,                //获取系统参数
	SET_PWD=0x12,                     //设置密码
	VFY_PWD=0x13,                     //获取密码
	GET_RANDOM=0x14,                  //获取采样随机数
	SET_ADDR=0x15,                    //设置地址
	Delete_MULTI_ID=0x1c,             //删除多个指纹id
	GET_ID_NUM=0x1d,                  //获取指纹数量
	GET_ID_LIST=0x1f,                 //获取指纹索引
	INIT_SYSTEM=0x21,                 //恢复出厂设置
	DEVICE_READY=0x22,                //device 初始化完成
	HAND_SHAKE=0x23,                  //握手
	INIT_SENSOR=0x26,                  //初始化sensor
	SWITCH_PROTOCOL=0x27,              //切换协议
	DEBUG_PRINT_EN=0x40,              //使能或者关闭fsmDebugPrint
	DEBUG_PRINT_MODE=0x41,            //用于fsmDebugPrint
	DEBUG_GET_PARA=0x42,
	DEBUG_UPLOAD=0x43,
	DEBUG_UPDATE=0x44,
	DEBUG_ENROLL_AFTER_DOWNLOAD=0x45,
	DEBUG_MATCH_AFTER_DOWNLOAD=0x46,
    DEBUG_RESET_TEMPLATE_STORAGE=0x47,
    DEBUG_READ_ALL_FLASH=0x48,
	FRAME_MAX
};

//确认码
#define PS_OK                   0x00     //OK
#define PS_COMM_ERR             0x01     //通信错误
#define PS_GET_IMG_ERR          0x03     //获取图像失败
#define PS_NOT_SEARCHED         0x09     //匹配失败
#define PS_ADDRESS_OVER         0x0b     //指纹ID越界
#define PS_UP_CHAR_ERR          0x0d
#define PS_RECV_ERR             0x0e     //接收帧长度大于最长长度
#define PS_UP_IMG_ERR           0x0f     //上传图像错误
#define PS_DEL_TEMP_ERR         0x10     //删除ID失败
#define PS_CLEAR_TEMP_ERR       0x11     //清空指纹库失败
#define PS_INVALID_PASSWORD     0x13     //口令错误
#define PS_INVALID_REG          0x1a     //错误的寄存器
#define PS_ENROLL_ERR           0x1e     //注册错误
#define PS_DEVICE_ADDR_ERR      0x20     //器件地址错误
#define PS_MUST_VERIFY_PWD      0x21     //必须验证口令
#define PS_PARAM_ERROR          0x22     //参数错误
#define PS_END_UPLOAD           0x23
#define PS_DUL_ENROLL_ERROR     0x24     //指纹ID被占用
#define PS_NO_ERROR             0x25     //代表单个手指录入一次成功
#define PS_TIMEOUT              0x26     //超时
#define PS_DOWN_IMG_ERR         0x27     //下载失败
#define PS_COMMAND_ERR          0x28     //命令错误
#define PS_PID_ERR              0x29     //包标识错误
#define PS_ENROLL_TIMES_WRONG   0x30    //注册次数不匹配
#define PS_UPDATE_FAIL          0x31    //更新失败
#define PS_SWITCH_PROTOCOL_FAIL 0x32    //切换协议失败
#define PS_RESPONSE             0x33    //应答包
#define PS_GET_IMG_OK           0x34




#define SYS_PARA_BAUDRATE	0x04
#define SYS_PARA_SECLVL		0x05
#define SYS_PARA_PACKLEN	0x06




#define FSM_MPU_START (0x004fee00)
#define FSM_MPU_END (0x00500000)
#define FSM_MPU_SIZE (FSM_MPU_END-FSM_MPU_START)


#define FSM_EFLASH_CODE_SIZE  (128*1024)    //128K code


#define FSM_EFLASH1_BASE (EFM_BASE)
#define FSM_EFLASH1_READ_START_ADDR  (FSM_EFLASH1_BASE + FSM_EFLASH_CODE_SIZE )
#define FSM_EFLASH1_READ_SIZE  (FSM_MPU_START -FSM_EFLASH1_BASE-FSM_EFLASH_CODE_SIZE)


#define FSM_EFLASH2_BASE (FSM_MPU_END)
#define FSM_EFLASH2_READ_START_ADDR  (FSM_EFLASH2_BASE)
#define FSM_EFLASH2_READ_SIZE  (EFM_END-FSM_EFLASH2_BASE)


#define FSM_SPIFLASH_READ_START_ADDR  (0x0)
#define FSM_SPIFLASH_READ_SIZE  (2*1024*1024)


#define FSM_SEND_DATA_IMAGE 0x01
#define FSM_SEND_DATA_EFLASH1_DATA 0x02
#define FSM_SEND_DATA_EFLASH2_DATA 0x03
#define FSM_SEND_DATA_SPIFLASH_DATA 0x04

// flag = 0 send image
// flag = 1 send eflash data
// flag = 2 send spiflash data

#define IMAGE_BACKGROUD                 0
#define IMAGE_RMV_BKG                   1
#define IMAGE_REALTIME                  2
#define IMAGE_TEMPORARY                 3
#define IMAGE_ENROLL                    4
#define IMAGE_AUTHEN                    5
#define IMAGE_TESTING                   6


#define IMAGE_DOWN_RAW                  0
#define IMAGE_DOWN_RMV_BKG              1
#define IMAGE_DOWN_TEMPLATE             2
#define IMAGE_DOWN_SENSOR_CFG           3


extern void fsmInit(void);
extern void fsmTask(S32 ch);
extern void deviceReady(void);
extern void fsmDebugPrint(const char *fmt, ...);

U8 SendData(char *name,U8 *buf,U32 len);



#endif
