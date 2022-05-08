/***************************************************************************************
**文件名:     MyConfig.h
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
#ifndef __MY_CONFIG_H__
#define __MY_CONFIG_H__
#include "string.h"
#include "sys.h"
#include "delay.h"
#include "eport_drv.h"
#include "pwm_drv.h"
#include "sub_c0_demo.h"
#include "c0_drv.h"
#include "delay.h"
#include "debug.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "eport_drv.h"
#include "i2c_drv.h"
#include "spi_drv.h"
#include "spi_reg.h"
#include "BasicFunc.h"
#include "usi_drv.h"
#define MY_CONFIG_MODULE_EN (1) //模块使能
#if MY_CONFIG_MODULE_EN
/*************************专用宏定义*************************/
#if 1
#define DEBUG_LOG_EN  //调试输出使能
#endif

#ifdef DEBUG_LOG_EN

#if 0
#define DIS_RAM_MODE //禁止RAM模式
#endif

#define PWD_TEST_DEBUG

#if 1
#define DEBUG_OUT_USE_UART //使用串口作为调试口
#endif

#if 0
#define DEBUG_OUT_USE_JLINK_RTT //使用jlink的rtt作为调试口
#endif

#ifdef DEBUG_OUT_USE_UART 
#define DEBUG_LOG_OUT(format, ...) printf("" format "", ##__VA_ARGS__)
#define DEBUG_LOG_OUT_HEX(format, len) \
  do                                   \
  {                                    \
    uint32_t i;                        \
    uint8_t *pTmp = format;            \
    for (i = 0; i < len; i++)          \
    {                                  \
      printf("%02X", *(pTmp + i));    \
    }                                  \
    printf("\n ");                     \
  } while (0)
#endif
 
#ifdef DEBUG_OUT_USE_JLINK_RTT
#define DEBUG_LOG_OUT(format, ...) SEGGER_RTT_printf(0, "" format "", ##__VA_ARGS__)
#define DEBUG_LOG_OUT_HEX(format, len)          \
  do                                            \
  {                                             \
    uint32_t i;                                 \
    uint8_t *pTmp = (uint8_t *)(format);            \  
    for (i = 0; i < len; i++)                   \
    {                                           \
      SEGGER_RTT_printf(0, "%02x ", *(pTmp + i)); \
    }                                           \
    SEGGER_RTT_printf(0, "\n ");                \
  } while (0)
#endif
  
#else
#define DEBUG_LOG_OUT(format, ...)
#define DEBUG_LOG_OUT_HEX(format, len)
#endif

#if 1
#define USE_LITTLE_ENDIAN //使用小端模式
#else
#define USE_BIG_ENDIAN    //使用大端端模式
#endif

#if 1
#define BSWAP_EN //大小端转换使能
#endif

#ifdef BSWAP_EN
#define BSWAP_16(x) (uint16_t)((((uint16_t)(x)&0x00ff) << 8) | (((uint16_t)(x)&0xff00) >> 8))
#define BSWAP_32(x) (uint32_t)((((uint32_t)(x)&0xff000000) >> 24) | \
                               (((uint32_t)(x)&0x00ff0000) >> 8) |  \
                               (((uint32_t)(x)&0x0000ff00) << 8) |  \
                               (((uint32_t)(x)&0x000000ff) << 24))
#define BSWAP_SAVE_16(x) (x = BSWAP_16(x)) //大小端转换并保存数据
#define BSWAP_SAVE_32(x) (x = BSWAP_32(x)) //大小端转换并保存数据
#else
#define BSWAP_16(x)
#define BSWAP_32(x)
#define BSWAP_SAVE_16(x)
#define BSWAP_SAVE_32(x)
#endif

#define BCD_TO_BIN(x) (((x)&0x0f) + (((x)&0xf0) >> 4) * 10)
#define BIN_TO_BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4))
#define BCD_TO_BIN_SAVE(x) ((x) = BCD_TO_BIN(x))
#define BIN_TO_BCD_SAVE(x) ((x) = BIN_TO_BCD(x))

#define M_TRUE 1
#define M_FALSE 0
#define SYSTEM_STORAGE_EN (1) //存储使能

#if 0
#define SYSTEM_VOICE_EN (1) //语音使能
#endif

#define SYSTEM_FINGER_EN (1) //指纹使能

#if 0
#define SYSTEM_BREATHING_LIGHT_EN (1) //呼吸灯使能
#endif

#if 0
#define SYSTEM_MOTOR_EN (1) //电机使能
#endif

#define M_VOS_GUI_ENABLE                      //打开语音菜单
#define MENU_TIME_CHANGE                      //菜单时间可变化
#define KEY_RANDOM_LOCK_EN (1)                //随机按键锁
#define LOCK_VERSION "DR0.C.A02_20210224"     //锁具版本
#define LOCK_SN "S70000000123456"             //锁具序列号
#define PRE_ALARM_TIME_LIMIT (15 * 60 * 1000) //防撬报警15分钟
#define POWER_VOLTAGE_WIFI_NOT_UPLOAD (690) //WIFI推送
#define POWER_VOLTAGE_LOW (690)             //低电压
#define POWER_VOLTAGE_LOW_CANNOT_WORK (650) //低电压,无法工作
#define AES_ROOT_KEY (5)                    //下标

#define DELAY_MS(X) \
  do                \
  {                 \
    DelayMS(X);     \
  } while (0)

#define WIFI_DEVICE_TYPE (0X0B)      //锁的设备类型
#define LOCK_OPEN_PARM_NO_CLEAR (1)  //锁开门参数不被清除,数据保存在硬件参数区域
#define LOCK_BODY_TYPE_LIUFU (1)     //六浮锁体
#define LOCK_BODY_TYPE_KAIYUANLE (2) //开元乐锁体
#define MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG		(1) //开启WIFI上报
#define MSG_WIFI_LOGUP_MAIN_SW_OFF_CONFIG		(0) //关闭wifi上报
#define MSG_WIFI_LOGUP_ON_CONFIG		1
#define MSG_WIFI_LOGUP_OFF_CONFIG		0
#if 1  
#define USE_FACE_WIFI (1) //使用人脸模块的wifi  
#endif
  
#if 0  
#define USE_WIFI_MODLE (1) //使用wifi模块  
#endif

#if 1  
#define USE_CATEYE (1) //使用猫眼  
#endif

#ifdef USE_CATEYE
#if 0
#define USE_CATEYE_SHANGTANG //使用商汤的猫眼
#endif
#if 1
#define USE_CATEYE_AOBI //使用奥比的猫眼
#endif
#endif
 
#if 1
#define USE_FACE_AOBI //使用奥比人脸 
#endif
#if 0
#define USE_FACE_SHANGTANG //使用商汤人脸 
#endif

#if 1
#define USE_COMM_ENCRYPT //前后板通讯使用加密 
#endif

#if 1
#define IR_ENABLE //红外使能 
#endif

#if 1
#define BACK_GET_BAT_STATUS //后板获取电池状态
#endif

#define DEBUG_LOW_POWER  (0) //调试低功耗

#define HAVE_BOOTLOADR  (1) //固件带有BOOT，需要修改工程设置里IROM1的值

#define BACK_LOW_RST (0) //后板拉低复位

#define USE_ID2  (1) //使用ID2加密 

#define USE_WDT (1) //使用看门狗

#define SPI1_1 ((SPI_TypeDef *)(SPI1_BASE_ADDR))
#define SPI2_2 ((SPI_TypeDef *)(SPI2_BASE_ADDR))
#define SPI3_3 ((SPI_TypeDef *)(SPI3_BASE_ADDR))

/******************************
boot相关的定义
*******************************/
/********************************************
  以下代码操作芯片内部FLASH，注意写入时字节对齐
********************************************/  
#define FLASH_SECTOR_SIZE (0x200ul)                                          //内部FLASH一个扇区的大小  512byte
#define FLASH_BASE_BOOT ((uint32_t)0x8000000)                                //flash基地址
#define BOOT_BASE FLASH_BASE_BOOT                                            //BootLoader 基地址
#define BOOT_SIZE (80 * FLASH_SECTOR_SIZE)                                   //BootLoader flash尺寸
#define BOOT_PARA_01_ADDRESS (BOOT_BASE + BOOT_SIZE - 2 * FLASH_SECTOR_SIZE) //BootLoader para存储地址
#define BOOT_PARA_02_ADDRESS (BOOT_BASE + BOOT_SIZE - 1 * FLASH_SECTOR_SIZE) //BootLoader para存储地址
#define APP_EXTIST_ADDRESS BOOT_PARA_01_ADDRESS                              //存储是否存在可用的APP地址
#define APP_PROGRAM_TYPE_ADDRESS BOOT_PARA_02_ADDRESS                        //存储升级方式地址
#define APP_PROGRAM_PARA_ADDRESS (BOOT_PARA_02_ADDRESS + 3*4)                 //存储MCU透传数据的地址
#define APP_BASE (BOOT_BASE + BOOT_SIZE)                                     //APP flash基地址
#define APP_SIZE (400 * FLASH_SECTOR_SIZE)                                   //APP flash尺寸
#define APP_PARA_ADDRESS (APP_BASE + APP_SIZE - 0x100u)                      //APP para存储地址
#define BACKUP_BASE (APP_BASE + APP_SIZE)                                    //BACKUP flash基地址
#define BACKUP_SIZE (400 * FLASH_SECTOR_SIZE)                                //BACKUP flash尺寸
#define BACKUP_PARA_ADDRESS (BACKUP_BASE + BACKUP_SIZE - 0x100u)             //BACKUP para存储地址



/***************************************************************************/ /**
* @brief
*   CC4202_INT驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
//#ifndef M_CC4202_INT_PIN
//#define M_CC4202_INT_PIN (EPORT_PIN26)
//#define M_CC4202_INT_PORT
//#define M_CC4202_INT_OUT_PP() EPORT_ConfigGpio(M_CC4202_INT_PIN, GPIO_OUTPUT);
//#define M_CC4202_INT_IN_NOPULL()                    \
//  do                                                \
//  {                                                 \
//    EPORT_ConfigGpio(M_CC4202_INT_PIN, GPIO_INPUT); \
//    EPORT_PullupConfig(M_CC4202_INT_PIN, DISABLE);  \
//  } while (0)
//#define M_CC4202_INT_IN_IPU()                       \
//  do                                                \
//  {                                                 \
//    EPORT_ConfigGpio(M_CC4202_INT_PIN, GPIO_INPUT); \
//    EPORT_PullupConfig(M_CC4202_INT_PIN, ENABLE);   \
//    EPORT_PullUPControl(M_CC4202_INT_PIN, ENABLE);  \
//  } while (0)
//#define M_CC4202_INT_IN_IPD()                       \
//  do                                                \
//  {                                                 \
//    EPORT_ConfigGpio(M_CC4202_INT_PIN, GPIO_INPUT); \
//    EPORT_PullupConfig(M_CC4202_INT_PIN, ENABLE);   \
//    EPORT_PullUPControl(M_CC4202_INT_PIN, DISABLE); \
//  } while (0)
//#define M_CC4202_INT_OUT_0() (EPORT_WriteGpioData(M_CC4202_INT_PIN, Bit_RESET))
//#define M_CC4202_INT_OUT_1() (EPORT_WriteGpioData(M_CC4202_INT_PIN, Bit_SET))
//#define M_CC4202_INT_OUT_TOGGLE()
//#define M_CC4202_INT_READIN() (EPORT_ReadGpioData(M_CC4202_INT_PIN))
//#define M_CC4202_INT_DETECT_ENABLE()
//#define M_CC4202_INT_LPM()
//#define M_CC4202_INT_EXIT_HITOLO_ENABLE()           \
//  do                                                \
//  {                                                 \
//    EPORT_Init(M_CC4202_INT_PIN, FALLING_EDGE_INT); \
//  } while (0)
//#define M_CC4202_INT_EXIT_LOTOHI_ENABLE()                  \
//  do                                                       \
//  {                                                        \
//    EPORT_Init(M_CC4202_INT_PIN, RISING_FALLING_EDGE_INT); \
//  } while (0)
//#define M_CC4202_INT_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_CC4202_INT_PIN, DISABLE))
//#define M_CC4202_INT_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_CC4202_INT_PIN, DISABLE))
//#endif


/***************************************************************************/ /**
* @brief
*   RXD驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_RXD_PIN
#define M_RXD_PIN (SCI1)
#define M_RXD_PORT (UART_RX)
#define M_RXD_OUT_PP() (UART_ConfigGpio(M_RXD_PIN, M_RXD_PORT, GPIO_OUTPUT))
#define M_RXD_IN_NOPULL()                               \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_RXD_PIN, M_RXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_RXD_PIN, M_RXD_PORT, DISABLE);  \
  } while (0)
#define M_RXD_IN_IPU()                                  \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_RXD_PIN, M_RXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_RXD_PIN, M_RXD_PORT, ENABLE);   \
    UART_PullUpControl(M_RXD_PIN, M_RXD_PORT, ENABLE);  \
  } while (0)
#define M_RXD_IN_IPD()                                  \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_RXD_PIN, M_RXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_RXD_PIN, M_RXD_PORT, ENABLE);   \
    UART_PullUpControl(M_RXD_PIN, M_RXD_PORT, DISABLE); \
  } while (0)
#define M_RXD_OUT_0() (UART_WriteGpioData(M_RXD_PIN, M_RXD_PORT, Bit_RESET))
#define M_RXD_OUT_1() (UART_WriteGpioData(M_RXD_PIN, M_RXD_PORT, Bit_SET))
#define M_RXD_OUT_TOGGLE()
#define M_RXD_READIN() (UART_ReadGpioData(M_RXD_PIN, M_RXD_PORT))
#define M_RXD_DETECT_ENABLE()
#define M_RXD_LPM()
#define M_RXD_EXIT_HITOLO_ENABLE()
#define M_RXD_EXIT_HITOLO_DISABLE()
#define M_RXD_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   TXD驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_TXD_PIN
#define M_TXD_PIN (SCI1)
#define M_TXD_PORT (UART_TX)
#define M_TXD_OUT_PP() (UART_ConfigGpio(M_TXD_PIN, M_TXD_PORT, GPIO_OUTPUT))
#define M_TXD_IN_NOPULL()                               \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_TXD_PIN, M_TXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_TXD_PIN, M_TXD_PORT, DISABLE);  \
  } while (0)
#define M_TXD_IN_IPU()                                  \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_TXD_PIN, M_TXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_TXD_PIN, M_TXD_PORT, ENABLE);   \
    UART_PullUpControl(M_TXD_PIN, M_TXD_PORT, ENABLE);  \
  } while (0)
#define M_TXD_IN_IPD()                                  \
  do                                                    \
  {                                                     \
    UART_ConfigGpio(M_TXD_PIN, M_TXD_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_TXD_PIN, M_TXD_PORT, ENABLE);   \
    UART_PullUpControl(M_TXD_PIN, M_TXD_PORT, DISABLE); \
  } while (0)
#define M_TXD_OUT_0() (UART_WriteGpioData(M_TXD_PIN, M_TXD_PORT, Bit_RESET))
#define M_TXD_OUT_1() (UART_WriteGpioData(M_TXD_PIN, M_TXD_PORT, Bit_SET))
#define M_TXD_OUT_TOGGLE()
#define M_TXD_READIN() (UART_ReadGpioData(M_TXD_PIN, M_TXD_PORT))
#define M_TXD_DETECT_ENABLE()
#define M_TXD_LPM()
#define M_TXD_EXIT_HITOLO_ENABLE()
#define M_TXD_EXIT_HITOLO_DISABLE()
#define M_TXD_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   IR_TX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_IR_TX_PIN
#define M_IR_TX_PIN (SCI2)
#define M_IR_TX_PORT (UART_RX)
#define M_IR_TX_OUT_PP() (UART_ConfigGpio(M_IR_TX_PIN, M_IR_TX_PORT, GPIO_OUTPUT))
#define M_IR_TX_IN_NOPULL()                                 \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_TX_PIN, M_IR_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_TX_PIN, M_IR_TX_PORT, DISABLE);  \
  } while (0)
#define M_IR_TX_IN_IPU()                                    \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_TX_PIN, M_IR_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_TX_PIN, M_IR_TX_PORT, ENABLE);   \
    UART_PullUpControl(M_IR_TX_PIN, M_IR_TX_PORT, ENABLE);  \
  } while (0)
#define M_IR_TX_IN_IPD()                                    \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_TX_PIN, M_IR_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_TX_PIN, M_IR_TX_PORT, ENABLE);   \
    UART_PullUpControl(M_IR_TX_PIN, M_IR_TX_PORT, DISABLE); \
  } while (0)
#define M_IR_TX_OUT_0() (UART_WriteGpioData(M_IR_TX_PIN, M_IR_TX_PORT, Bit_RESET))
#define M_IR_TX_OUT_1() (UART_WriteGpioData(M_IR_TX_PIN, M_IR_TX_PORT, Bit_SET))
#define M_IR_TX_OUT_TOGGLE()
#define M_IR_TX_READIN() (UART_ReadGpioData(M_IR_TX_PIN, M_IR_TX_PORT))
#define M_IR_TX_DETECT_ENABLE()
#define M_IR_TX_LPM()
#define M_IR_TX_EXIT_HITOLO_ENABLE()
#define M_IR_TX_EXIT_HITOLO_DISABLE()
#define M_IR_TX_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   IR_RX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_IR_RX_PIN
#define M_IR_RX_PIN (SCI2)
#define M_IR_RX_PORT (UART_TX)
#define M_IR_RX_OUT_PP() (UART_ConfigGpio(M_IR_RX_PIN, M_IR_RX_PORT, GPIO_OUTPUT))
#define M_IR_RX_IN_NOPULL()                                 \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_RX_PIN, M_IR_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_RX_PIN, M_IR_RX_PORT, DISABLE);  \
  } while (0)
#define M_IR_RX_IN_IPU()                                    \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_RX_PIN, M_IR_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_RX_PIN, M_IR_RX_PORT, ENABLE);   \
    UART_PullUpControl(M_IR_RX_PIN, M_IR_RX_PORT, ENABLE);  \
  } while (0)
#define M_IR_RX_IN_IPD()                                    \
  do                                                        \
  {                                                         \
    UART_ConfigGpio(M_IR_RX_PIN, M_IR_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_IR_RX_PIN, M_IR_RX_PORT, ENABLE);   \
    UART_PullUpControl(M_IR_RX_PIN, M_IR_RX_PORT, DISABLE); \
  } while (0)
#define M_IR_RX_OUT_0() (UART_WriteGpioData(M_IR_RX_PIN, M_IR_RX_PORT, Bit_RESET))
#define M_IR_RX_OUT_1() (UART_WriteGpioData(M_IR_RX_PIN, M_IR_RX_PORT, Bit_SET))
#define M_IR_RX_OUT_TOGGLE()
#define M_IR_RX_READIN() (UART_ReadGpioData(M_IR_RX_PIN, M_IR_RX_PORT))
#define M_IR_RX_DETECT_ENABLE()
#define M_IR_RX_LPM()
#define M_IR_RX_EXIT_HITOLO_ENABLE()
#define M_IR_RX_EXIT_HITOLO_DISABLE()
#define M_IR_RX_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   CAM_TX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CAM_TX_PIN
#define M_CAM_TX_PIN (SCI3)
#define M_CAM_TX_PORT (UART_TX)
#define M_CAM_TX_OUT_PP() (UART_ConfigGpio(M_CAM_TX_PIN, M_CAM_TX_PORT, GPIO_OUTPUT))
#define M_CAM_TX_IN_NOPULL()                                  \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_TX_PIN, M_CAM_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_TX_PIN, M_CAM_TX_PORT, DISABLE);  \
  } while (0)
#define M_CAM_TX_IN_IPU()                                     \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_TX_PIN, M_CAM_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_TX_PIN, M_CAM_TX_PORT, ENABLE);   \
    UART_PullUpControl(M_CAM_TX_PIN, M_CAM_TX_PORT, ENABLE);  \
  } while (0)
#define M_CAM_TX_IN_IPD()                                     \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_TX_PIN, M_CAM_TX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_TX_PIN, M_CAM_TX_PORT, ENABLE);   \
    UART_PullUpControl(M_CAM_TX_PIN, M_CAM_TX_PORT, DISABLE); \
  } while (0)
#define M_CAM_TX_OUT_0() (UART_WriteGpioData(M_CAM_TX_PIN, M_CAM_TX_PORT, Bit_RESET))
#define M_CAM_TX_OUT_1() (UART_WriteGpioData(M_CAM_TX_PIN, M_CAM_TX_PORT, Bit_SET))
#define M_CAM_TX_OUT_TOGGLE()
#define M_CAM_TX_READIN() (UART_ReadGpioData(M_CAM_TX_PIN, M_CAM_TX_PORT))
#define M_CAM_TX_DETECT_ENABLE()
#define M_CAM_TX_LPM()
#define M_CAM_TX_EXIT_HITOLO_ENABLE()
#define M_CAM_TX_EXIT_HITOLO_DISABLE()
#define M_CAM_TX_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   CAM_RX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CAM_RX_PIN
#define M_CAM_RX_PIN (SCI3)
#define M_CAM_RX_PORT (UART_RX)
#define M_CAM_RX_OUT_PP() (UART_ConfigGpio(M_CAM_RX_PIN, M_CAM_RX_PORT, GPIO_OUTPUT))
#define M_CAM_RX_IN_NOPULL()                                  \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_RX_PIN, M_CAM_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_RX_PIN, M_CAM_RX_PORT, DISABLE);  \
  } while (0)
#define M_CAM_RX_IN_IPU()                                     \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_RX_PIN, M_CAM_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_RX_PIN, M_CAM_RX_PORT, ENABLE);   \
    UART_PullUpControl(M_CAM_RX_PIN, M_CAM_RX_PORT, ENABLE);  \
  } while (0)
#define M_CAM_RX_IN_IPD()                                     \
  do                                                          \
  {                                                           \
    UART_ConfigGpio(M_CAM_RX_PIN, M_CAM_RX_PORT, GPIO_INPUT); \
    UART_PullUpConfig(M_CAM_RX_PIN, M_CAM_RX_PORT, ENABLE);   \
    UART_PullUpControl(M_CAM_RX_PIN, M_CAM_RX_PORT, DISABLE); \
  } while (0)
#define M_CAM_RX_OUT_0() (UART_WriteGpioData(M_CAM_RX_PIN, M_CAM_RX_PORT, Bit_RESET))
#define M_CAM_RX_OUT_1() (UART_WriteGpioData(M_CAM_RX_PIN, M_CAM_RX_PORT, Bit_SET))
#define M_CAM_RX_OUT_TOGGLE()
#define M_CAM_RX_READIN() (UART_ReadGpioData(M_CAM_RX_PIN, M_CAM_RX_PORT))
#define M_CAM_RX_DETECT_ENABLE()
#define M_CAM_RX_LPM()
#define M_CAM_RX_EXIT_HITOLO_ENABLE()
#define M_CAM_RX_EXIT_HITOLO_DISABLE()
#define M_CAM_RX_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   LED_B_PWM驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_LED_B_PWM_PIN
#define M_LED_B_PWM_PIN (EPORT_PIN12)
#define M_LED_B_PWM_PORT
#define M_LED_B_PWM_OUT_PP() EPORT_ConfigGpio(M_LED_B_PWM_PIN, GPIO_OUTPUT);
#define M_LED_B_PWM_IN_NOPULL()                    \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_B_PWM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_B_PWM_PIN, DISABLE);  \
  } while (0)
#define M_LED_B_PWM_IN_IPU()                       \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_B_PWM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_B_PWM_PIN, ENABLE);   \
    EPORT_PullUPControl(M_LED_B_PWM_PIN, ENABLE);  \
  } while (0)
#define M_LED_B_PWM_IN_IPD()                       \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_B_PWM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_B_PWM_PIN, ENABLE);   \
    EPORT_PullUPControl(M_LED_B_PWM_PIN, DISABLE); \
  } while (0)
#define M_LED_B_PWM_OUT_0() (EPORT_WriteGpioData(M_LED_B_PWM_PIN, Bit_RESET))
#define M_LED_B_PWM_OUT_1() (EPORT_WriteGpioData(M_LED_B_PWM_PIN, Bit_SET))
#define M_LED_B_PWM_OUT_TOGGLE()
#define M_LED_B_PWM_READIN() (EPORT_ReadGpioData(M_LED_B_PWM_PIN))
#define M_LED_B_PWM_DETECT_ENABLE()
#define M_LED_B_PWM_LPM()
#define M_LED_B_PWM_EXIT_HITOLO_ENABLE()           \
  do                                               \
  {                                                \
    EPORT_Init(M_LED_B_PWM_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_LED_B_PWM_EXIT_LOTOHI_ENABLE()                  \
  do                                                      \
  {                                                       \
    EPORT_Init(M_LED_B_PWM_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_LED_B_PWM_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_LED_B_PWM_PIN, DISABLE))
#define M_LED_B_PWM_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_LED_B_PWM_PIN, DISABLE))
#endif

/***************************************************************************/ /**
* @brief
*   WT_POW驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_WT_POW_PIN
#define M_WT_POW_PIN (I2C_SCL)
#define M_WT_POW_PORT (I2C3)
#define M_WT_POW_OUT_PP() I2C_ConfigGpio(M_WT_POW_PORT, M_WT_POW_PIN, GPIO_OUTPUT)
#define M_WT_POW_IN_NOPULL()                                \
  do                                                        \
  {                                                         \
    I2C_ConfigGpio(M_WT_POW_PORT, M_WT_POW_PIN, GPIO_INPUT) \
  } while (0)
#define M_WT_POW_IN_IPU()                                   \
  do                                                        \
  {                                                         \
    I2C_ConfigGpio(M_WT_POW_PORT, M_WT_POW_PIN, GPIO_INPUT) \
  } while (0)
#define M_WT_POW_IN_IPD()                                   \
  do                                                        \
  {                                                         \
    I2C_ConfigGpio(M_WT_POW_PORT, M_WT_POW_PIN, GPIO_INPUT) \
  } while (0)
#define M_WT_POW_OUT_0() I2C_WriteGpioData(M_WT_POW_PORT, M_WT_POW_PIN, Bit_RESET)
#define M_WT_POW_OUT_1() I2C_WriteGpioData(M_WT_POW_PORT, M_WT_POW_PIN, Bit_SET)
#define M_WT_POW_OUT_TOGGLE()
#define M_WT_POW_READIN() I2C_ReadGpioData(M_WT_POW_PORT, M_WT_POW_PIN)
#define M_WT_POW_DETECT_ENABLE()
#define M_WT_POW_LPM()
#define M_WT_POW_EXIT_HITOLO_ENABLE()
#define M_WT_POW_EXIT_LOTOHI_ENABLE()
#define M_WT_POW_EXIT_HITOLO_DISABLE()
#define M_WT_POW_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   WAKE_UP0驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_WAKE_UP0_PIN
#define M_WAKE_UP0_PIN      //(I2C_SCL)
#define M_WAKE_UP0_PORT     //(I2C3)
#define M_WAKE_UP0_OUT_PP() //I2C_ConfigGpio(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, GPIO_OUTPUT)
#define M_WAKE_UP0_IN_NOPULL() \
//  do                                                            \
//  {                                                             \
//    I2C_ConfigGpio(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, GPIO_INPUT) \
//  } while (0)
#define M_WAKE_UP0_IN_IPU() \
//  do                                                            \
//  {                                                             \
//    I2C_ConfigGpio(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, GPIO_INPUT) \
//  } while (0)
#define M_WAKE_UP0_IN_IPD() \
//  do                                                            \
//  {                                                             \
//    I2C_ConfigGpio(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, GPIO_INPUT) \
//  } while (0)
#define M_WAKE_UP0_OUT_0() //I2C_WriteGpioData(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, Bit_RESET)
#define M_WAKE_UP0_OUT_1() //I2C_WriteGpioData(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN, Bit_SET)
#define M_WAKE_UP0_OUT_TOGGLE()
#define M_WAKE_UP0_READIN() //I2C_ReadGpioData(M_WAKE_UP0_PORT, M_WAKE_UP0_PIN)
#define M_WAKE_UP0_DETECT_ENABLE()
#define M_WAKE_UP0_LPM()
#define M_WAKE_UP0_EXIT_HITOLO_ENABLE()
#define M_WAKE_UP0_EXIT_LOTOHI_ENABLE()
#define M_WAKE_UP0_EXIT_HITOLO_DISABLE()
#define M_WAKE_UP0_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   SDA驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SDA_PIN
#define M_SDA_PIN (I2C_SDA)
#define M_SDA_PORT (I2C1)
#define M_SDA_OUT_PP() I2C_ConfigGpio(M_SDA_PORT, M_SDA_PIN, GPIO_OUTPUT)
#define M_SDA_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA_PORT, M_SDA_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA_PORT, M_SDA_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA_PORT, M_SDA_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA_OUT_0() I2C_WriteGpioData(M_SDA_PORT, M_SDA_PIN, Bit_RESET)
#define M_SDA_OUT_1() I2C_WriteGpioData(M_SDA_PORT, M_SDA_PIN, Bit_SET)
#define M_SDA_OUT_TOGGLE()
#define M_SDA_READIN() I2C_ReadGpioData(M_SDA_PORT, M_SDA_PIN)
#define M_SDA_DETECT_ENABLE()
#define M_SDA_LPM()
#define M_SDA_EXIT_HITOLO_ENABLE()
#define M_SDA_EXIT_LOTOHI_ENABLE()
#define M_SDA_EXIT_HITOLO_DISABLE()
#define M_SDA_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   SCL驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SCL_PIN
#define M_SCL_PIN (I2C_SCL)
#define M_SCL_PORT (I2C2)
#define M_SCL_OUT_PP() I2C_ConfigGpio(M_SCL_PORT, M_SCL_PIN, GPIO_OUTPUT)
#define M_SCL_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL_PORT, M_SCL_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL_PORT, M_SCL_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL_PORT, M_SCL_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL_OUT_0() I2C_WriteGpioData(M_SCL_PORT, M_SCL_PIN, Bit_RESET)
#define M_SCL_OUT_1() I2C_WriteGpioData(M_SCL_PORT, M_SCL_PIN, Bit_SET)
#define M_SCL_OUT_TOGGLE()
#define M_SCL_READIN() I2C_ReadGpioData(M_SCL_PORT, M_SCL_PIN)
#define M_SCL_DETECT_ENABLE()
#define M_SCL_LPM()
#define M_SCL_EXIT_HITOLO_ENABLE()
#define M_SCL_EXIT_LOTOHI_ENABLE()
#define M_SCL_EXIT_HITOLO_DISABLE()
#define M_SCL_EXIT_LOTOHI_DISABLE()
#endif
  
/***************************************************************************/ /**
* @brief
*   SDA2驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SDA2_PIN
#define M_SDA2_PIN (I2C_SDA)
#define M_SDA2_PORT (I2C2)
#define M_SDA2_OUT_PP() I2C_ConfigGpio(M_SDA2_PORT, M_SDA2_PIN, GPIO_OUTPUT)
#define M_SDA2_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA2_PORT, M_SDA2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA2_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA2_PORT, M_SDA2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA2_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SDA2_PORT, M_SDA2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SDA2_OUT_0() I2C_WriteGpioData(M_SDA2_PORT, M_SDA2_PIN, Bit_RESET)
#define M_SDA2_OUT_1() I2C_WriteGpioData(M_SDA2_PORT, M_SDA2_PIN, Bit_SET)
#define M_SDA2_OUT_TOGGLE()
#define M_SDA2_READIN() I2C_ReadGpioData(M_SDA2_PORT, M_SDA2_PIN)
#define M_SDA2_DETECT_ENABLE()
#define M_SDA2_LPM()
#define M_SDA2_EXIT_HITOLO_ENABLE()
#define M_SDA2_EXIT_LOTOHI_ENABLE()
#define M_SDA2_EXIT_HITOLO_DISABLE()
#define M_SDA2_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   SCL2驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SCL2_PIN
#define M_SCL2_PIN (I2C_SCL)
#define M_SCL2_PORT (I2C2)
#define M_SCL2_OUT_PP() I2C_ConfigGpio(M_SCL2_PORT, M_SCL2_PIN, GPIO_OUTPUT)
#define M_SCL2_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL2_PORT, M_SCL2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL2_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL2_PORT, M_SCL2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL2_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    I2C_ConfigGpio(M_SCL2_PORT, M_SCL2_PIN, GPIO_INPUT); \
  } while (0)
#define M_SCL2_OUT_0() I2C_WriteGpioData(M_SCL2_PORT, M_SCL2_PIN, Bit_RESET)
#define M_SCL2_OUT_1() I2C_WriteGpioData(M_SCL2_PORT, M_SCL2_PIN, Bit_SET)
#define M_SCL2_OUT_TOGGLE()
#define M_SCL2_READIN() I2C_ReadGpioData(M_SCL2_PORT, M_SCL2_PIN)
#define M_SCL2_DETECT_ENABLE()
#define M_SCL2_LPM()
#define M_SCL2_EXIT_HITOLO_ENABLE()
#define M_SCL2_EXIT_LOTOHI_ENABLE()
#define M_SCL2_EXIT_HITOLO_DISABLE()
#define M_SCL2_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   X_IRQ_9555驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_X_IRQ_9555_PIN
#define M_X_IRQ_9555_PIN (EPORT_PIN3)
#define M_X_IRQ_9555_PORT
#define M_X_IRQ_9555_OUT_PP() EPORT_ConfigGpio(M_X_IRQ_9555_PIN, GPIO_OUTPUT);
#define M_X_IRQ_9555_IN_NOPULL()                    \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_X_IRQ_9555_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_IRQ_9555_PIN, DISABLE);  \
  } while (0)
#define M_X_IRQ_9555_IN_IPU()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_X_IRQ_9555_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_IRQ_9555_PIN, ENABLE);   \
    EPORT_PullUPControl(M_X_IRQ_9555_PIN, ENABLE);  \
  } while (0)
#define M_X_IRQ_9555_IN_IPD()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_X_IRQ_9555_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_IRQ_9555_PIN, ENABLE);   \
    EPORT_PullUPControl(M_X_IRQ_9555_PIN, DISABLE); \
  } while (0)
#define M_X_IRQ_9555_OUT_0() (EPORT_WriteGpioData(M_X_IRQ_9555_PIN, Bit_RESET))
#define M_X_IRQ_9555_OUT_1() (EPORT_WriteGpioData(M_X_IRQ_9555_PIN, Bit_SET))
#define M_X_IRQ_9555_OUT_TOGGLE()
#define M_X_IRQ_9555_READIN() (EPORT_ReadGpioData(M_X_IRQ_9555_PIN))
#define M_X_IRQ_9555_DETECT_ENABLE()
#define M_X_IRQ_9555_LPM()
#define M_X_IRQ_9555_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
    EPORT_Init(M_X_IRQ_9555_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_X_IRQ_9555_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    EPORT_Init(M_X_IRQ_9555_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_X_IRQ_9555_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    EPORT_Init(M_X_IRQ_9555_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_X_IRQ_9555_EXIT_FALL_DISABLE() (EPORT_ITConfig(M_X_IRQ_9555_PIN, DISABLE))
#define M_X_IRQ_9555_EXIT_RISE_DISABLE() (EPORT_ITConfig(M_X_IRQ_9555_PIN, DISABLE))
#define M_X_IRQ_9555_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_X_IRQ_9555_PIN, DISABLE))
#endif	  
/***************************************************************************/ /**
* @brief
*   HS6621_INT驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_HS6621_INT_PIN
#define M_HS6621_INT_PIN (EPORT_PIN29)
#define M_HS6621_INT_PORT
#define M_HS6621_INT_OUT_PP() EPORT_ConfigGpio(M_HS6621_INT_PIN, GPIO_OUTPUT);
#define M_HS6621_INT_IN_NOPULL()                    \
  do                                                \
  {                                                 \
    EPORT_ConfigGpio(M_HS6621_INT_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_INT_PIN, DISABLE);  \
  } while (0)
#define M_HS6621_INT_IN_IPU()                       \
  do                                                \
  {                                                 \
    EPORT_ConfigGpio(M_HS6621_INT_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_INT_PIN, ENABLE);   \
    EPORT_PullUPControl(M_HS6621_INT_PIN, ENABLE);  \
  } while (0)
#define M_HS6621_INT_IN_IPD()                       \
  do                                                \
  {                                                 \
    EPORT_ConfigGpio(M_HS6621_INT_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_INT_PIN, ENABLE);   \
    EPORT_PullUPControl(M_HS6621_INT_PIN, DISABLE); \
  } while (0)
#define M_HS6621_INT_OUT_0() (EPORT_WriteGpioData(M_HS6621_INT_PIN, Bit_RESET))
#define M_HS6621_INT_OUT_1() (EPORT_WriteGpioData(M_HS6621_INT_PIN, Bit_SET))
#define M_HS6621_INT_OUT_TOGGLE()
#define M_HS6621_INT_READIN() (EPORT_ReadGpioData(M_HS6621_INT_PIN))
#define M_HS6621_INT_DETECT_ENABLE()
#define M_HS6621_INT_LPM()
#define M_HS6621_INT_EXIT_HITOLO_ENABLE()           \
  do                                                \
  {                                                 \
    EPORT_Init(M_HS6621_INT_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_HS6621_INT_EXIT_LOTOHI_ENABLE()          \
  do                                               \
  {                                                \
    EPORT_Init(M_HS6621_INT_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_HS6621_INT_EXIT_EDGE_ENABLE()                    \
  do                                                       \
  {                                                        \
    EPORT_Init(M_HS6621_INT_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_HS6621_INT_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_HS6621_INT_PIN, DISABLE))
#define M_HS6621_INT_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_HS6621_INT_PIN, DISABLE))
#define M_HS6621_INT_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_HS6621_INT_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   FING_POW驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FING_POW_PIN
#define M_FING_POW_PIN (EPORT_PIN27)
#define M_FING_POW_PORT
#define M_FING_POW_OUT_PP() EPORT_ConfigGpio(M_FING_POW_PIN, GPIO_OUTPUT);
#define M_FING_POW_IN_NOPULL()                    \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_FING_POW_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_FING_POW_PIN, DISABLE);  \
  } while (0)
#define M_FING_POW_IN_IPU()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_FING_POW_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_FING_POW_PIN, ENABLE);   \
    EPORT_PullUPControl(M_FING_POW_PIN, ENABLE);  \
  } while (0)
#define M_FING_POW_IN_IPD()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_FING_POW_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_FING_POW_PIN, ENABLE);   \
    EPORT_PullUPControl(M_FING_POW_PIN, DISABLE); \
  } while (0)
#define M_FING_POW_OUT_0() (EPORT_WriteGpioData(M_FING_POW_PIN, Bit_RESET))
#define M_FING_POW_OUT_1() (EPORT_WriteGpioData(M_FING_POW_PIN, Bit_SET))
#define M_FING_POW_OUT_TOGGLE()
#define M_FING_POW_READIN() (EPORT_ReadGpioData(M_FING_POW_PIN))
#define M_FING_POW_DETECT_ENABLE()
#define M_FING_POW_LPM()
#define M_FING_POW_EXIT_HITOLO_ENABLE()           \
  do                                              \
  {                                               \
    EPORT_Init(M_FING_POW_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_FING_POW_EXIT_LOTOHI_ENABLE()          \
  do                                             \
  {                                              \
    EPORT_Init(M_FING_POW_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_FING_POW_EXIT_EDGE_ENABLE()                    \
  do                                                     \
  {                                                      \
    EPORT_Init(M_FING_POW_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_FING_POW_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_FING_POW_PIN, DISABLE))
#define M_FING_POW_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_FING_POW_PIN, DISABLE))
#define M_FING_POW_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_FING_POW_PIN, DISABLE))
#endif

/***************************************************************************/ /**
* @brief
*   CAM_WIFI_POW_EN驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CAM_WIFI_POW_EN_PIN
#define M_CAM_WIFI_POW_EN_PIN (EPORT_PIN28)
#define M_CAM_WIFI_POW_EN_PORT
#define M_CAM_WIFI_POW_EN_OUT_PP() EPORT_ConfigGpio(M_CAM_WIFI_POW_EN_PIN, GPIO_OUTPUT);
#define M_CAM_WIFI_POW_EN_IN_NOPULL()                    \
  do                                                     \
  {                                                      \
    EPORT_ConfigGpio(M_CAM_WIFI_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_WIFI_POW_EN_PIN, DISABLE);  \
  } while (0)
#define M_CAM_WIFI_POW_EN_IN_IPU()                       \
  do                                                     \
  {                                                      \
    EPORT_ConfigGpio(M_CAM_WIFI_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_WIFI_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_CAM_WIFI_POW_EN_PIN, ENABLE);  \
  } while (0)
#define M_CAM_WIFI_POW_EN_IN_IPD()                       \
  do                                                     \
  {                                                      \
    EPORT_ConfigGpio(M_CAM_WIFI_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_WIFI_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_CAM_WIFI_POW_EN_PIN, DISABLE); \
  } while (0)
#define M_CAM_WIFI_POW_EN_OUT_0() (EPORT_WriteGpioData(M_CAM_WIFI_POW_EN_PIN, Bit_RESET))
#define M_CAM_WIFI_POW_EN_OUT_1() (EPORT_WriteGpioData(M_CAM_WIFI_POW_EN_PIN, Bit_SET))
#define M_CAM_WIFI_POW_EN_OUT_TOGGLE()
#define M_CAM_WIFI_POW_EN_READIN() (EPORT_ReadGpioData(M_CAM_WIFI_POW_EN_PIN))
#define M_CAM_WIFI_POW_EN_DETECT_ENABLE()
#define M_CAM_WIFI_POW_EN_LPM()
#define M_CAM_WIFI_POW_EN_EXIT_HITOLO_ENABLE()           \
  do                                                     \
  {                                                      \
    EPORT_Init(M_CAM_WIFI_POW_EN_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_CAM_WIFI_POW_EN_EXIT_LOTOHI_ENABLE()          \
  do                                                    \
  {                                                     \
    EPORT_Init(M_CAM_WIFI_POW_EN_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_CAM_WIFI_POW_EN_EXIT_EDGE_ENABLE()                    \
  do                                                            \
  {                                                             \
    EPORT_Init(M_CAM_WIFI_POW_EN_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_CAM_WIFI_POW_EN_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_CAM_WIFI_POW_EN_PIN, DISABLE))
#define M_CAM_WIFI_POW_EN_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_CAM_WIFI_POW_EN_PIN, DISABLE))
#define M_CAM_WIFI_POW_EN_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_CAM_WIFI_POW_EN_PIN, DISABLE))
#endif



/***************************************************************************/ /**
* @brief
*   POW_CARD驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_POW_CARD_PIN
#define M_POW_CARD_PIN (EPORT_PIN10)
#define M_POW_CARD_PORT
#define M_POW_CARD_OUT_PP() EPORT_ConfigGpio(M_POW_CARD_PIN, GPIO_OUTPUT);
#define M_POW_CARD_IN_NOPULL()                    \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_POW_CARD_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_POW_CARD_PIN, DISABLE);  \
  } while (0)
#define M_POW_CARD_IN_IPU()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_POW_CARD_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_POW_CARD_PIN, ENABLE);   \
    EPORT_PullUPControl(M_POW_CARD_PIN, ENABLE);  \
  } while (0)
#define M_POW_CARD_IN_IPD()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_POW_CARD_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_POW_CARD_PIN, ENABLE);   \
    EPORT_PullUPControl(M_POW_CARD_PIN, DISABLE); \
  } while (0)
#define M_POW_CARD_OUT_0() (EPORT_WriteGpioData(M_POW_CARD_PIN, Bit_RESET))
#define M_POW_CARD_OUT_1() (EPORT_WriteGpioData(M_POW_CARD_PIN, Bit_SET))
#define M_POW_CARD_OUT_TOGGLE()
#define M_POW_CARD_READIN() (EPORT_ReadGpioData(M_POW_CARD_PIN))
#define M_POW_CARD_DETECT_ENABLE()
#define M_POW_CARD_LPM()
#define M_POW_CARD_EXIT_HITOLO_ENABLE()           \
  do                                              \
  {                                               \
    EPORT_Init(M_POW_CARD_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_POW_CARD_EXIT_LOTOHI_ENABLE()          \
  do                                             \
  {                                              \
    EPORT_Init(M_POW_CARD_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_POW_CARD_EXIT_EDGE_ENABLE()                    \
  do                                                     \
  {                                                      \
    EPORT_Init(M_POW_CARD_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_POW_CARD_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_POW_CARD_PIN, DISABLE))
#define M_POW_CARD_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_POW_CARD_PIN, DISABLE))
#define M_POW_CARD_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_POW_CARD_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   NFC_SCK驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_NFC_SCK_PIN
#define M_NFC_SCK_PIN (SPI_SCK)
#define M_NFC_SCK_PORT (SPI1_1)
#define M_NFC_SCK_OUT_PP() SPI_ConfigGpio(M_NFC_SCK_PORT, M_NFC_SCK_PIN, GPIO_OUTPUT);
#define M_NFC_SCK_IN_NOPULL()                                  \
  do                                                           \
  {                                                            \
    SPI_ConfigGpio(M_NFC_SCK_PORT, M_NFC_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_SCK_IN_IPU()                                     \
  do                                                           \
  {                                                            \
    SPI_ConfigGpio(M_NFC_SCK_PORT, M_NFC_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_SCK_IN_IPD()                                     \
  do                                                           \
  {                                                            \
    SPI_ConfigGpio(M_NFC_SCK_PORT, M_NFC_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_SCK_OUT_0() (SPI_WriteGpioData(M_NFC_SCK_PORT, M_NFC_SCK_PIN, Bit_RESET))
#define M_NFC_SCK_OUT_1() (SPI_WriteGpioData(M_NFC_SCK_PORT, M_NFC_SCK_PIN, Bit_SET))
#define M_NFC_SCK_OUT_TOGGLE()
#define M_NFC_SCK_READIN() (SPI_ReadGpioData(M_NFC_SCK_PORT, M_NFC_SCK_PIN, Bit_SET))
#define M_NFC_SCK_DETECT_ENABLE()
#define M_NFC_SCK_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   NFC_MOSI驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_NFC_MOSI_PIN
#define M_NFC_MOSI_PIN (SPI_MOSI)
#define M_NFC_MOSI_PORT (SPI1_1)
#define M_NFC_MOSI_OUT_PP() SPI_ConfigGpio(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, GPIO_OUTPUT);
#define M_NFC_MOSI_IN_NOPULL()                                   \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MOSI_IN_IPU()                                      \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MOSI_IN_IPD()                                      \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MOSI_OUT_0() (SPI_WriteGpioData(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, Bit_RESET))
#define M_NFC_MOSI_OUT_1() (SPI_WriteGpioData(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, Bit_SET))
#define M_NFC_MOSI_OUT_TOGGLE()
#define M_NFC_MOSI_READIN() (SPI_ReadGpioData(M_NFC_MOSI_PORT, M_NFC_MOSI_PIN, Bit_SET))
#define M_NFC_MOSI_DETECT_ENABLE()
#define M_NFC_MOSI_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   NFC_CS驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_NFC_CS_PIN
#define M_NFC_CS_PIN (SPI_SS)
#define M_NFC_CS_PORT (SPI1_1)
#define M_NFC_CS_OUT_PP() SPI_ConfigGpio(M_NFC_CS_PORT, M_NFC_CS_PIN, GPIO_OUTPUT);
#define M_NFC_CS_IN_NOPULL()                                 \
  do                                                         \
  {                                                          \
    SPI_ConfigGpio(M_NFC_CS_PORT, M_NFC_CS_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_CS_IN_IPU()                                    \
  do                                                         \
  {                                                          \
    SPI_ConfigGpio(M_NFC_CS_PORT, M_NFC_CS_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_CS_IN_IPD()                                    \
  do                                                         \
  {                                                          \
    SPI_ConfigGpio(M_NFC_CS_PORT, M_NFC_CS_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_CS_OUT_0() (SPI_WriteGpioData(M_NFC_CS_PORT, M_NFC_CS_PIN, Bit_RESET))
#define M_NFC_CS_OUT_1() (SPI_WriteGpioData(M_NFC_CS_PORT, M_NFC_CS_PIN, Bit_SET))
#define M_NFC_CS_OUT_TOGGLE()
#define M_NFC_CS_READIN() (SPI_ReadGpioData(M_NFC_CS_PORT, M_NFC_CS_PIN, Bit_SET))
#define M_NFC_CS_DETECT_ENABLE()
#define M_NFC_CS_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   NFC_MISO驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_NFC_MISO_PIN
#define M_NFC_MISO_PIN (SPI_MISO)
#define M_NFC_MISO_PORT (SPI1_1)
#define M_NFC_MISO_OUT_PP() SPI_ConfigGpio(M_NFC_MISO_PORT, M_NFC_MISO_PIN, GPIO_OUTPUT);
#define M_NFC_MISO_IN_NOPULL()                                   \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MISO_PORT, M_NFC_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MISO_IN_IPU()                                      \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MISO_PORT, M_NFC_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MISO_IN_IPD()                                      \
  do                                                             \
  {                                                              \
    SPI_ConfigGpio(M_NFC_MISO_PORT, M_NFC_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_NFC_MISO_OUT_0() (SPI_WriteGpioData(M_NFC_MISO_PORT, M_NFC_MISO_PIN, Bit_RESET))
#define M_NFC_MISO_OUT_1() (SPI_WriteGpioData(M_NFC_MISO_PORT, M_NFC_MISO_PIN, Bit_SET))
#define M_NFC_MISO_OUT_TOGGLE()
#define M_NFC_MISO_READIN() (SPI_ReadGpioData(M_NFC_MISO_PORT, M_NFC_MISO_PIN, Bit_SET))
#define M_NFC_MISO_DETECT_ENABLE()
#define M_NFC_MISO_LPM()
#endif

/***************************************************************************/ /**
* @brief
*   SPI4_0驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_0_PIN
#define M_SPI4_0_PIN (EPORT_PIN18)
#define M_SPI4_0_PORT
#define M_SPI4_0_OUT_PP() EPORT_ConfigGpio(M_SPI4_0_PIN, GPIO_OUTPUT);
#define M_SPI4_0_IN_NOPULL()                    \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_0_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_0_IN_IPU()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_0_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_0_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_0_IN_IPD()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_0_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_0_PIN, DISABLE); \
  } while (0)
#define M_SPI4_0_OUT_0() (EPORT_WriteGpioData(M_SPI4_0_PIN, Bit_RESET))
#define M_SPI4_0_OUT_1() (EPORT_WriteGpioData(M_SPI4_0_PIN, Bit_SET))
#define M_SPI4_0_OUT_TOGGLE()
#define M_SPI4_0_READIN() (EPORT_ReadGpioData(M_SPI4_0_PIN))
#define M_SPI4_0_DETECT_ENABLE()
#define M_SPI4_0_LPM()
#define M_SPI4_0_EXIT_HITOLO_ENABLE()           \
  do                                            \
  {                                             \
    EPORT_Init(M_SPI4_0_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_0_EXIT_LOTOHI_ENABLE()          \
  do                                           \
  {                                            \
    EPORT_Init(M_SPI4_0_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_0_EXIT_EDGE_ENABLE()                    \
  do                                                   \
  {                                                    \
    EPORT_Init(M_SPI4_0_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_0_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_0_PIN, DISABLE))
#define M_SPI4_0_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_0_PIN, DISABLE))
#define M_SPI4_0_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_0_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   SPI4_1驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_1_PIN
#define M_SPI4_1_PIN (EPORT_PIN19)
#define M_SPI4_1_PORT
#define M_SPI4_1_OUT_PP() EPORT_ConfigGpio(M_SPI4_1_PIN, GPIO_OUTPUT);
#define M_SPI4_1_IN_NOPULL()                    \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_1_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_1_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_1_IN_IPU()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_1_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_1_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_1_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_1_IN_IPD()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_1_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_1_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_1_PIN, DISABLE); \
  } while (0)
#define M_SPI4_1_OUT_0() (EPORT_WriteGpioData(M_SPI4_1_PIN, Bit_RESET))
#define M_SPI4_1_OUT_1() (EPORT_WriteGpioData(M_SPI4_1_PIN, Bit_SET))
#define M_SPI4_1_OUT_TOGGLE()
#define M_SPI4_1_READIN() (EPORT_ReadGpioData(M_SPI4_1_PIN))
#define M_SPI4_1_DETECT_ENABLE()
#define M_SPI4_1_LPM()
#define M_SPI4_1_EXIT_HITOLO_ENABLE()           \
  do                                            \
  {                                             \
    EPORT_Init(M_SPI4_1_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_1_EXIT_LOTOHI_ENABLE()          \
  do                                           \
  {                                            \
    EPORT_Init(M_SPI4_1_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_1_EXIT_EDGE_ENABLE()                    \
  do                                                   \
  {                                                    \
    EPORT_Init(M_SPI4_1_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_1_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_1_PIN, DISABLE))
#define M_SPI4_1_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_1_PIN, DISABLE))
#define M_SPI4_1_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_1_PIN, DISABLE))
#endif

/***************************************************************************/ /**
* @brief
*   SPI4_2驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_2_PIN
#define M_SPI4_2_PIN (EPORT_PIN16)
#define M_SPI4_2_PORT
#define M_SPI4_2_OUT_PP() EPORT_ConfigGpio(M_SPI4_2_PIN, GPIO_OUTPUT);
#define M_SPI4_2_IN_NOPULL()                    \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_2_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_2_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_2_IN_IPU()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_2_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_2_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_2_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_2_IN_IPD()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_2_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_2_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_2_PIN, DISABLE); \
  } while (0)
#define M_SPI4_2_OUT_0() (EPORT_WriteGpioData(M_SPI4_2_PIN, Bit_RESET))
#define M_SPI4_2_OUT_1() (EPORT_WriteGpioData(M_SPI4_2_PIN, Bit_SET))
#define M_SPI4_2_OUT_TOGGLE()
#define M_SPI4_2_READIN() (EPORT_ReadGpioData(M_SPI4_2_PIN))
#define M_SPI4_2_DETECT_ENABLE()
#define M_SPI4_2_LPM()
#define M_SPI4_2_EXIT_HITOLO_ENABLE()           \
  do                                            \
  {                                             \
    EPORT_Init(M_SPI4_2_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_2_EXIT_LOTOHI_ENABLE()          \
  do                                           \
  {                                            \
    EPORT_Init(M_SPI4_2_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_2_EXIT_EDGE_ENABLE()                    \
  do                                                   \
  {                                                    \
    EPORT_Init(M_SPI4_2_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_2_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_2_PIN, DISABLE))
#define M_SPI4_2_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_2_PIN, DISABLE))
#define M_SPI4_2_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_2_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   SPI4_3驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_3_PIN
#define M_SPI4_3_PIN (EPORT_PIN17)
#define M_SPI4_3_PORT
#define M_SPI4_3_OUT_PP() EPORT_ConfigGpio(M_SPI4_3_PIN, GPIO_OUTPUT);
#define M_SPI4_3_IN_NOPULL()                    \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_3_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_3_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_3_IN_IPU()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_3_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_3_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_3_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_3_IN_IPD()                       \
  do                                            \
  {                                             \
    EPORT_ConfigGpio(M_SPI4_3_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_3_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_3_PIN, DISABLE); \
  } while (0)
#define M_SPI4_3_OUT_0() (EPORT_WriteGpioData(M_SPI4_3_PIN, Bit_RESET))
#define M_SPI4_3_OUT_1() (EPORT_WriteGpioData(M_SPI4_3_PIN, Bit_SET))
#define M_SPI4_3_OUT_TOGGLE()
#define M_SPI4_3_READIN() (EPORT_ReadGpioData(M_SPI4_3_PIN))
#define M_SPI4_3_DETECT_ENABLE()
#define M_SPI4_3_LPM()
#define M_SPI4_3_EXIT_HITOLO_ENABLE()           \
  do                                            \
  {                                             \
    EPORT_Init(M_SPI4_3_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_3_EXIT_LOTOHI_ENABLE()          \
  do                                           \
  {                                            \
    EPORT_Init(M_SPI4_3_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_3_EXIT_EDGE_ENABLE()                    \
  do                                                   \
  {                                                    \
    EPORT_Init(M_SPI4_3_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_3_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_3_PIN, DISABLE))
#define M_SPI4_3_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_3_PIN, DISABLE))
#define M_SPI4_3_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_3_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   SPI4_SCK驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_SCK_PIN
#define M_SPI4_SCK_PIN (EPORT_PIN20)
#define M_SPI4_SCK_PORT
#define M_SPI4_SCK_OUT_PP() EPORT_ConfigGpio(M_SPI4_SCK_PIN, GPIO_OUTPUT);
#define M_SPI4_SCK_IN_NOPULL()                    \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_SPI4_SCK_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SCK_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_SCK_IN_IPU()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_SPI4_SCK_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SCK_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_SCK_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_SCK_IN_IPD()                       \
  do                                              \
  {                                               \
    EPORT_ConfigGpio(M_SPI4_SCK_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SCK_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_SCK_PIN, DISABLE); \
  } while (0)
#define M_SPI4_SCK_OUT_0() (EPORT_WriteGpioData(M_SPI4_SCK_PIN, Bit_RESET))
#define M_SPI4_SCK_OUT_1() (EPORT_WriteGpioData(M_SPI4_SCK_PIN, Bit_SET))
#define M_SPI4_SCK_OUT_TOGGLE()
#define M_SPI4_SCK_READIN() (EPORT_ReadGpioData(M_SPI4_SCK_PIN))
#define M_SPI4_SCK_DETECT_ENABLE()
#define M_SPI4_SCK_LPM()
#define M_SPI4_SCK_EXIT_HITOLO_ENABLE()           \
  do                                              \
  {                                               \
    EPORT_Init(M_SPI4_SCK_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_SCK_EXIT_LOTOHI_ENABLE()          \
  do                                             \
  {                                              \
    EPORT_Init(M_SPI4_SCK_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_SCK_EXIT_EDGE_ENABLE()                    \
  do                                                     \
  {                                                      \
    EPORT_Init(M_SPI4_SCK_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_SCK_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_SCK_PIN, DISABLE))
#define M_SPI4_SCK_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_SCK_PIN, DISABLE))
#define M_SPI4_SCK_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_SCK_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   SPI4_SS驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_SPI4_SS_PIN
#define M_SPI4_SS_PIN (EPORT_PIN21)
#define M_SPI4_SS_PORT
#define M_SPI4_SS_OUT_PP() EPORT_ConfigGpio(M_SPI4_SS_PIN, GPIO_OUTPUT);
#define M_SPI4_SS_IN_NOPULL()                    \
  do                                             \
  {                                              \
    EPORT_ConfigGpio(M_SPI4_SS_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SS_PIN, DISABLE);  \
  } while (0)
#define M_SPI4_SS_IN_IPU()                       \
  do                                             \
  {                                              \
    EPORT_ConfigGpio(M_SPI4_SS_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SS_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_SS_PIN, ENABLE);  \
  } while (0)
#define M_SPI4_SS_IN_IPD()                       \
  do                                             \
  {                                              \
    EPORT_ConfigGpio(M_SPI4_SS_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_SPI4_SS_PIN, ENABLE);   \
    EPORT_PullUPControl(M_SPI4_SS_PIN, DISABLE); \
  } while (0)
#define M_SPI4_SS_OUT_0() (EPORT_WriteGpioData(M_SPI4_SS_PIN, Bit_RESET))
#define M_SPI4_SS_OUT_1() (EPORT_WriteGpioData(M_SPI4_SS_PIN, Bit_SET))
#define M_SPI4_SS_OUT_TOGGLE()
#define M_SPI4_SS_READIN() (EPORT_ReadGpioData(M_SPI4_SS_PIN))
#define M_SPI4_SS_DETECT_ENABLE()
#define M_SPI4_SS_LPM()
#define M_SPI4_SS_EXIT_HITOLO_ENABLE()           \
  do                                             \
  {                                              \
    EPORT_Init(M_SPI4_SS_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_SS_EXIT_LOTOHI_ENABLE()          \
  do                                            \
  {                                             \
    EPORT_Init(M_SPI4_SS_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_SPI4_SS_EXIT_EDGE_ENABLE()                    \
  do                                                    \
  {                                                     \
    EPORT_Init(M_SPI4_SS_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_SPI4_SS_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_SPI4_SS_PIN, DISABLE))
#define M_SPI4_SS_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_SPI4_SS_PIN, DISABLE))
#define M_SPI4_SS_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_SPI4_SS_PIN, DISABLE))
#endif
/***************************************************************************/ /**
* @brief
*   X_FACE_POW_EN驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_X_FACE_POW_EN_PIN
#define M_X_FACE_POW_EN_PIN (EPORT_PIN9)
#define M_X_FACE_POW_EN_PORT
#define M_X_FACE_POW_EN_OUT_PP() EPORT_ConfigGpio(M_X_FACE_POW_EN_PIN, GPIO_OUTPUT);
#define M_X_FACE_POW_EN_IN_NOPULL()                    \
  do                                                   \
  {                                                    \
    EPORT_ConfigGpio(M_X_FACE_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_FACE_POW_EN_PIN, DISABLE);  \
  } while (0)
#define M_X_FACE_POW_EN_IN_IPU()                       \
  do                                                   \
  {                                                    \
    EPORT_ConfigGpio(M_X_FACE_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_FACE_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_X_FACE_POW_EN_PIN, ENABLE);  \
  } while (0)
#define M_X_FACE_POW_EN_IN_IPD()                       \
  do                                                   \
  {                                                    \
    EPORT_ConfigGpio(M_X_FACE_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_X_FACE_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_X_FACE_POW_EN_PIN, DISABLE); \
  } while (0)
#define M_X_FACE_POW_EN_OUT_0() (EPORT_WriteGpioData(M_X_FACE_POW_EN_PIN, Bit_RESET))
#define M_X_FACE_POW_EN_OUT_1() (EPORT_WriteGpioData(M_X_FACE_POW_EN_PIN, Bit_SET))
#define M_X_FACE_POW_EN_OUT_TOGGLE()
#define M_X_FACE_POW_EN_READIN() (EPORT_ReadGpioData(M_X_FACE_POW_EN_PIN))
#define M_X_FACE_POW_EN_DETECT_ENABLE()
#define M_X_FACE_POW_EN_LPM()
#define M_X_FACE_POW_EN_EXIT_HITOLO_ENABLE()           \
  do                                                   \
  {                                                    \
    EPORT_Init(M_X_FACE_POW_EN_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_X_FACE_POW_EN_EXIT_LOTOHI_ENABLE()          \
  do                                                  \
  {                                                   \
    EPORT_Init(M_X_FACE_POW_EN_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_X_FACE_POW_EN_EXIT_EDGE_ENABLE()                    \
  do                                                          \
  {                                                           \
    EPORT_Init(M_X_FACE_POW_EN_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_X_FACE_POW_EN_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_X_FACE_POW_EN_PIN, DISABLE))
#define M_X_FACE_POW_EN_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_X_FACE_POW_EN_PIN, DISABLE))
#define M_X_FACE_POW_EN_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_X_FACE_POW_EN_PIN, DISABLE))
#endif


/***************************************************************************/ /**
* @brief
*   ALARM驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_ALARM_PIN
#define M_ALARM_PIN (EPORT_PIN2)
#define M_ALARM_PORT
#define M_ALARM_OUT_PP() EPORT_ConfigGpio(M_ALARM_PIN, GPIO_OUTPUT);
#define M_ALARM_IN_NOPULL()                    \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_ALARM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_ALARM_PIN, DISABLE);  \
  } while (0)
#define M_ALARM_IN_IPU()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_ALARM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_ALARM_PIN, ENABLE);   \
    EPORT_PullUPControl(M_ALARM_PIN, ENABLE);  \
  } while (0)
#define M_ALARM_IN_IPD()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_ALARM_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_ALARM_PIN, ENABLE);   \
    EPORT_PullUPControl(M_ALARM_PIN, DISABLE); \
  } while (0)
#define M_ALARM_OUT_0() (EPORT_WriteGpioData(M_ALARM_PIN, Bit_RESET))
#define M_ALARM_OUT_1() (EPORT_WriteGpioData(M_ALARM_PIN, Bit_SET))
#define M_ALARM_OUT_TOGGLE()
#define M_ALARM_READIN() (EPORT_ReadGpioData(M_ALARM_PIN))
#define M_ALARM_DETECT_ENABLE()
#define M_ALARM_LPM()
#define M_ALARM_EXIT_HITOLO_ENABLE()           \
  do                                           \
  {                                            \
    EPORT_Init(M_ALARM_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_ALARM_EXIT_LOTOHI_ENABLE()          \
  do                                          \
  {                                           \
    EPORT_Init(M_ALARM_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_ALARM_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    EPORT_Init(M_ALARM_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_ALARM_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_ALARM_PIN, DISABLE))
#define M_ALARM_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_ALARM_PIN, DISABLE))
#define M_ALARM_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_ALARM_PIN, DISABLE))
#endif

/***************************************************************************/ /**
* @brief
*   HS6621_RSTB驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_HS6621_RSTB_PIN
#define M_HS6621_RSTB_PIN (EPORT_PIN26)
#define M_HS6621_RSTB_PORT
#define M_HS6621_RSTB_OUT_PP() EPORT_ConfigGpio(M_HS6621_RSTB_PIN, GPIO_OUTPUT);
#define M_HS6621_RSTB_IN_NOPULL()                    \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_HS6621_RSTB_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_RSTB_PIN, DISABLE);  \
  } while (0)
#define M_HS6621_RSTB_IN_IPU()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_HS6621_RSTB_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_RSTB_PIN, ENABLE);   \
    EPORT_PullUPControl(M_HS6621_RSTB_PIN, ENABLE);  \
  } while (0)
#define M_HS6621_RSTB_IN_IPD()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_HS6621_RSTB_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_HS6621_RSTB_PIN, ENABLE);   \
    EPORT_PullUPControl(M_HS6621_RSTB_PIN, DISABLE); \
  } while (0)
#define M_HS6621_RSTB_OUT_0() (EPORT_WriteGpioData(M_HS6621_RSTB_PIN, Bit_RESET))
#define M_HS6621_RSTB_OUT_1() (EPORT_WriteGpioData(M_HS6621_RSTB_PIN, Bit_SET))
#define M_HS6621_RSTB_OUT_TOGGLE()
#define M_HS6621_RSTB_READIN() (EPORT_ReadGpioData(M_HS6621_RSTB_PIN))
#define M_HS6621_RSTB_DETECT_ENABLE()
#define M_HS6621_RSTB_LPM()
#define M_HS6621_RSTB_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
    EPORT_Init(M_HS6621_RSTB_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_HS6621_RSTB_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    EPORT_Init(M_HS6621_RSTB_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_HS6621_RSTB_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    EPORT_Init(M_HS6621_RSTB_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_HS6621_RSTB_EXIT_FALL_DISABLE() (EPORT_ITConfig(M_HS6621_RSTB_PIN, DISABLE))
#define M_HS6621_RSTB_EXIT_RISE_DISABLE() (EPORT_ITConfig(M_HS6621_RSTB_PIN, DISABLE))
#define M_HS6621_RSTB_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_HS6621_RSTB_PIN, DISABLE))
#endif	  
  
/***************************************************************************/ /**
* @brief
*   X_CHANGE_9555驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_X_CHANGE_9555_PIN
#define M_X_CHANGE_9555_PIN (SUBC0_WAKEUP1)
#define M_X_CHANGE_9555_PORT
#define M_X_CHANGE_9555_OUT_PP() drv_SubC0_ConfigGpio(M_X_CHANGE_9555_PIN, GPIO_OUTPUT)
#define M_X_CHANGE_9555_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_X_CHANGE_9555_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_X_CHANGE_9555_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_X_CHANGE_9555_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_X_CHANGE_9555_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_X_CHANGE_9555_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_X_CHANGE_9555_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_X_CHANGE_9555_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_X_CHANGE_9555_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_X_CHANGE_9555_OUT_0() (drv_SubC0_WriteGpioData(M_X_CHANGE_9555_PIN, Bit_RESET))
#define M_X_CHANGE_9555_OUT_1() (drv_SubC0_WriteGpioData(M_X_CHANGE_9555_PIN, Bit_SET))
#define M_X_CHANGE_9555_OUT_TOGGLE()
#define M_X_CHANGE_9555_READIN() (drv_SubC0_ReadGpioData(M_X_CHANGE_9555_PIN))
#define M_X_CHANGE_9555_DETECT_ENABLE()
#define M_X_CHANGE_9555_LPM()
#define M_X_CHANGE_9555_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_X_CHANGE_9555_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN,ENABLE);	\
  } while (0)
#define M_X_CHANGE_9555_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_X_CHANGE_9555_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN,ENABLE);	\
  } while (0)
#define M_X_CHANGE_9555_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_X_CHANGE_9555_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN,ENABLE);	\
  } while (0)
#define M_X_CHANGE_9555_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN, DISABLE))
#define M_X_CHANGE_9555_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN, DISABLE))
#define M_X_CHANGE_9555_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN, DISABLE))
#define M_X_CHANGE_9555_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_X_CHANGE_9555_PIN, DISABLE))  
#endif  

/***************************************************************************/ /**
* @brief
*   CAM_POW_EN驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CAM_POW_EN_PIN
#define M_CAM_POW_EN_PIN (EPORT_PIN22)
#define M_CAM_POW_EN_PORT
#define M_CAM_POW_EN_OUT_PP() EPORT_ConfigGpio(M_CAM_POW_EN_PIN, GPIO_OUTPUT);
#define M_CAM_POW_EN_IN_NOPULL()                    \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_CAM_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_POW_EN_PIN, DISABLE);  \
  } while (0)
#define M_CAM_POW_EN_IN_IPU()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_CAM_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_CAM_POW_EN_PIN, ENABLE);  \
  } while (0)
#define M_CAM_POW_EN_IN_IPD()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_CAM_POW_EN_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_CAM_POW_EN_PIN, ENABLE);   \
    EPORT_PullUPControl(M_CAM_POW_EN_PIN, DISABLE); \
  } while (0)
#define M_CAM_POW_EN_OUT_0() (EPORT_WriteGpioData(M_CAM_POW_EN_PIN, Bit_RESET))
#define M_CAM_POW_EN_OUT_1() (EPORT_WriteGpioData(M_CAM_POW_EN_PIN, Bit_SET))
#define M_CAM_POW_EN_OUT_TOGGLE()
#define M_CAM_POW_EN_READIN() (EPORT_ReadGpioData(M_CAM_POW_EN_PIN))
#define M_CAM_POW_EN_DETECT_ENABLE()
#define M_CAM_POW_EN_LPM()
#define M_CAM_POW_EN_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
    EPORT_Init(M_CAM_POW_EN_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_CAM_POW_EN_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    EPORT_Init(M_CAM_POW_EN_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_CAM_POW_EN_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    EPORT_Init(M_CAM_POW_EN_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_CAM_POW_EN_EXIT_FALL_DISABLE() (EPORT_ITConfig(M_CAM_POW_EN_PIN, DISABLE))
#define M_CAM_POW_EN_EXIT_RISE_DISABLE() (EPORT_ITConfig(M_CAM_POW_EN_PIN, DISABLE))
#define M_CAM_POW_EN_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_CAM_POW_EN_PIN, DISABLE))
#endif	  

/***************************************************************************/ /**
* @brief
*   BELL驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_BELL_PIN
#define M_BELL_PIN (EPORT_PIN25)
#define M_BELL_PORT
#define M_BELL_OUT_PP() EPORT_ConfigGpio(M_BELL_PIN, GPIO_OUTPUT);
#define M_BELL_IN_NOPULL()
#define M_BELL_IN_IPU()                     \
  EPORT_ConfigGpio(M_BELL_PIN, GPIO_INPUT); \
  EPORT_PullupConfig(M_BELL_PIN, ENABLE)
#define M_BELL_IN_IPD()                     \
  EPORT_ConfigGpio(M_BELL_PIN, GPIO_INPUT); \
  EPORT_PullupConfig(M_BELL_PIN, DISABLE)
#define M_BELL_OUT_0() EPORT_WriteGpioData(M_BELL_PIN, Bit_RESET)
#define M_BELL_OUT_1() EPORT_WriteGpioData(M_BELL_PIN, Bit_SET)
#define M_BELL_OUT_TOGGLE()
#define M_BELL_READIN() EPORT_ReadGpioData(M_BELL_PIN)
#define M_BELL_DETECT_ENABLE()
#define M_BELL_LPM()
#define M_BELL_EXIT_HITOLO_ENABLE()
#define M_BELL_EXIT_LOTOHI_ENABLE()
#define M_BELL_EXIT_HITOLO_DISABLE()
#define M_BELL_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   3802_IRQ驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_3802_IRQ_PIN
#define M_3802_IRQ_PIN (SUBC0_GINT5)
#define M_3802_IRQ_PORT
#define M_3802_IRQ_OUT_PP() drv_SubC0_ConfigGpio(M_3802_IRQ_PIN, GPIO_OUTPUT)
#define M_3802_IRQ_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_3802_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_3802_IRQ_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_3802_IRQ_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_3802_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_3802_IRQ_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_3802_IRQ_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_3802_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_3802_IRQ_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_3802_IRQ_OUT_0() (drv_SubC0_WriteGpioData(M_3802_IRQ_PIN, Bit_RESET))
#define M_3802_IRQ_OUT_1() (drv_SubC0_WriteGpioData(M_3802_IRQ_PIN, Bit_SET))
#define M_3802_IRQ_OUT_TOGGLE()
#define M_3802_IRQ_READIN() (drv_SubC0_ReadGpioData(M_3802_IRQ_PIN))
#define M_3802_IRQ_DETECT_ENABLE()
#define M_3802_IRQ_LPM()
#define M_3802_IRQ_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_3802_IRQ_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_3802_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_3802_IRQ_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_3802_IRQ_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_3802_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_3802_IRQ_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_3802_IRQ_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_3802_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_3802_IRQ_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_3802_IRQ_PIN, DISABLE))
#define M_3802_IRQ_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_3802_IRQ_PIN, DISABLE))
#define M_3802_IRQ_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_3802_IRQ_PIN, DISABLE))
#define M_3802_IRQ_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_3802_IRQ_PIN, DISABLE))  
#endif  
      
  
/***************************************************************************/ /**
* @brief
*   IR_IRQ驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_IR_IRQ_PIN
#define M_IR_IRQ_PIN (SUBC0_GINT4)
#define M_IR_IRQ_PORT
#define M_IR_IRQ_OUT_PP() drv_SubC0_ConfigGpio(M_IR_IRQ_PIN, GPIO_OUTPUT)
#define M_IR_IRQ_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_IR_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_IR_IRQ_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_IR_IRQ_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_IR_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_IR_IRQ_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_IR_IRQ_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_IR_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_IR_IRQ_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_IR_IRQ_OUT_0() (drv_SubC0_WriteGpioData(M_IR_IRQ_PIN, Bit_RESET))
#define M_IR_IRQ_OUT_1() (drv_SubC0_WriteGpioData(M_IR_IRQ_PIN, Bit_SET))
#define M_IR_IRQ_OUT_TOGGLE()
#define M_IR_IRQ_READIN() (drv_SubC0_ReadGpioData(M_IR_IRQ_PIN))
#define M_IR_IRQ_DETECT_ENABLE()
#define M_IR_IRQ_LPM()
#define M_IR_IRQ_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_IR_IRQ_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_IR_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_IR_IRQ_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_IR_IRQ_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_IR_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_IR_IRQ_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_IR_IRQ_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_IR_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_IR_IRQ_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_IR_IRQ_PIN, DISABLE))
#define M_IR_IRQ_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_IR_IRQ_PIN, DISABLE))
#define M_IR_IRQ_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_IR_IRQ_PIN, DISABLE))
#define M_IR_IRQ_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_IR_IRQ_PIN, DISABLE))  
#endif


/***************************************************************************/ /**
* @brief
*   CAM_POW_EN1驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CAM_POW_EN1_PIN
#define M_CAM_POW_EN1_PIN (SPI_SCK)
#define M_CAM_POW_EN1_PORT (SPI1)
#define M_CAM_POW_EN1_OUT_PP() SPI_ConfigGpio(M_CAM_POW_EN1_PORT,M_CAM_POW_EN1_PIN, GPIO_OUTPUT);
#define M_CAM_POW_EN1_IN_NOPULL()                    \
  do                                           \
  {                                            \
    SPI_ConfigGpio(M_CAM_POW_EN1_PORT,M_CAM_POW_EN1_PIN, GPIO_INPUT); \
  } while (0)
#define M_CAM_POW_EN1_IN_IPU()                       \
    do                                           \
  {                                            \
    SPI_ConfigGpio(M_CAM_POW_EN1_PORT,M_CAM_POW_EN1_PIN, GPIO_INPUT); \
  } while (0)
#define M_CAM_POW_EN1_IN_IPD()                       \
    do                                           \
  {                                            \
    SPI_ConfigGpio(M_CAM_POW_EN1_PORT,M_CAM_POW_EN1_PIN, GPIO_INPUT); \
  } while (0)
#define M_CAM_POW_EN1_OUT_0() (SPI_WriteGpioData(M_CAM_POW_EN1_PORT, M_CAM_POW_EN1_PIN, Bit_RESET))
#define M_CAM_POW_EN1_OUT_1() (SPI_WriteGpioData(M_CAM_POW_EN1_PORT, M_CAM_POW_EN1_PIN, Bit_SET))
#define M_CAM_POW_EN1_OUT_TOGGLE()
#define M_CAM_POW_EN1_READIN() (SPI_ReadGpioData(M_CAM_POW_EN1_PORT,M_CAM_POW_EN1_PIN))
#define M_CAM_POW_EN1_DETECT_ENABLE()
#define M_CAM_POW_EN1_LPM()  
#endif 
  
/***************************************************************************/ /**
* @brief
*   4052_A0驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_4052_A0_PIN
#define M_4052_A0_PIN (EPORT_PIN31)
#define M_4052_A0_PORT
#define M_4052_A0_OUT_PP() EPORT_ConfigGpio(M_4052_A0_PIN, GPIO_OUTPUT);
#define M_4052_A0_IN_NOPULL()                    \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_4052_A0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_4052_A0_PIN, DISABLE);  \
  } while (0)
#define M_4052_A0_IN_IPU()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_4052_A0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_4052_A0_PIN, ENABLE);   \
    EPORT_PullUPControl(M_4052_A0_PIN, ENABLE);  \
  } while (0)
#define M_4052_A0_IN_IPD()                       \
  do                                           \
  {                                            \
    EPORT_ConfigGpio(M_4052_A0_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_4052_A0_PIN, ENABLE);   \
    EPORT_PullUPControl(M_4052_A0_PIN, DISABLE); \
  } while (0)
#define M_4052_A0_OUT_0() (EPORT_WriteGpioData(M_4052_A0_PIN, Bit_RESET))
#define M_4052_A0_OUT_1() (EPORT_WriteGpioData(M_4052_A0_PIN, Bit_SET))
#define M_4052_A0_OUT_TOGGLE()
#define M_4052_A0_READIN() (EPORT_ReadGpioData(M_4052_A0_PIN))
#define M_4052_A0_DETECT_ENABLE()
#define M_4052_A0_LPM()
#define M_4052_A0_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
    EPORT_Init(M_4052_A0_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_4052_A0_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    EPORT_Init(M_4052_A0_PIN, RISING_EDGE_INT); \
  } while (0)
#define M_4052_A0_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    EPORT_Init(M_4052_A0_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_4052_A0_EXIT_FALL_DISABLE() (EPORT_ITConfig(M_4052_A0_PIN, DISABLE))
#define M_4052_A0_EXIT_RISE_DISABLE() (EPORT_ITConfig(M_4052_A0_PIN, DISABLE))
#define M_4052_A0_EXIT_EDGE_DISABLE() (EPORT_ITConfig(M_4052_A0_PIN, DISABLE))
#endif	   

/***************************************************************************/ /**
* @brief
*   4052_A1驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_4052_A1_PIN
#define M_4052_A1_PIN 
#define M_4052_A1_PORT 
#define M_4052_A1_OUT_PP() 
#define M_4052_A1_IN_NOPULL()                    
#define M_4052_A1_IN_IPU()
#define M_4052_A1_IN_IPD()                       
#define M_4052_A1_OUT_0()
#define M_4052_A1_OUT_1()
#define M_4052_A1_OUT_TOGGLE()
#define M_4052_A1_READIN() 
#define M_4052_A1_DETECT_ENABLE()
#define M_4052_A1_LPM()  
#endif 

/***************************************************************************/ /**
* @brief
*   FING_IRQ驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FING_IRQ_PIN
#define M_FING_IRQ_PIN (SUBC0_GINT0)
#define M_FING_IRQ_PORT
#define M_FING_IRQ_OUT_PP() drv_SubC0_ConfigGpio(M_FING_IRQ_PIN, GPIO_OUTPUT)
#define M_FING_IRQ_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_FING_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_FING_IRQ_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_FING_IRQ_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_FING_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_FING_IRQ_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_FING_IRQ_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_FING_IRQ_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_FING_IRQ_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_FING_IRQ_OUT_0() (drv_SubC0_WriteGpioData(M_FING_IRQ_PIN, Bit_RESET))
#define M_FING_IRQ_OUT_1() (drv_SubC0_WriteGpioData(M_FING_IRQ_PIN, Bit_SET))
#define M_FING_IRQ_OUT_TOGGLE()
#define M_FING_IRQ_READIN() (drv_SubC0_ReadGpioData(M_FING_IRQ_PIN))
#define M_FING_IRQ_DETECT_ENABLE()
#define M_FING_IRQ_LPM()
#define M_FING_IRQ_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_FING_IRQ_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_FING_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_FING_IRQ_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_FING_IRQ_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_FING_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_FING_IRQ_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_FING_IRQ_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_FING_IRQ_PIN,ENABLE);	\
  } while (0)
#define M_FING_IRQ_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_FING_IRQ_PIN, DISABLE))
#define M_FING_IRQ_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_FING_IRQ_PIN, DISABLE))
#define M_FING_IRQ_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_FING_IRQ_PIN, DISABLE))
#define M_FING_IRQ_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_FING_IRQ_PIN, DISABLE))  
#endif
  
/***************************************************************************/ /**
* @brief
*   4052_EN驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_4052_EN_PIN
#define M_4052_EN_PIN (SUBC0_WAKEUP0)
#define M_4052_EN_PORT
#define M_4052_EN_OUT_PP() drv_SubC0_ConfigGpio(M_4052_EN_PIN, GPIO_OUTPUT)
#define M_4052_EN_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_4052_EN_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_4052_EN_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_4052_EN_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_4052_EN_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_4052_EN_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_4052_EN_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_4052_EN_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_4052_EN_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_4052_EN_OUT_0() (drv_SubC0_WriteGpioData(M_4052_EN_PIN, Bit_RESET))
#define M_4052_EN_OUT_1() (drv_SubC0_WriteGpioData(M_4052_EN_PIN, Bit_SET))
#define M_4052_EN_OUT_TOGGLE()
#define M_4052_EN_READIN() (drv_SubC0_ReadGpioData(M_4052_EN_PIN))
#define M_4052_EN_DETECT_ENABLE()
#define M_4052_EN_LPM()
#define M_4052_EN_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_4052_EN_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_4052_EN_PIN,ENABLE);	\
  } while (0)
#define M_4052_EN_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_4052_EN_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_4052_EN_PIN,ENABLE);	\
  } while (0)
#define M_4052_EN_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_4052_EN_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_4052_EN_PIN,ENABLE);	\
  } while (0)
#define M_4052_EN_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_4052_EN_PIN, DISABLE))
#define M_4052_EN_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_4052_EN_PIN, DISABLE))
#define M_4052_EN_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_4052_EN_PIN, DISABLE))
#define M_4052_EN_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_4052_EN_PIN, DISABLE))  
#endif   

/***************************************************************************/ /**
* @brief
*   MBI5024_POW驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_MBI5024_POW_PIN
#define M_MBI5024_POW_PIN (EPORT_PIN11)
#define M_MBI5024_POW_PORT
#define M_MBI5024_POW_OUT_PP() EPORT_ConfigGpio(M_MBI5024_POW_PIN, GPIO_OUTPUT);
#define M_MBI5024_POW_IN_NOPULL()
#define M_MBI5024_POW_IN_IPU()                       \
    EPORT_ConfigGpio(M_MBI5024_POW_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_POW_PIN, ENABLE)
#define M_MBI5024_POW_IN_IPD()                       \
    EPORT_ConfigGpio(M_MBI5024_POW_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_POW_PIN, DISABLE)
#define M_MBI5024_POW_OUT_0() EPORT_WriteGpioData(M_MBI5024_POW_PIN, Bit_RESET)
#define M_MBI5024_POW_OUT_1() EPORT_WriteGpioData(M_MBI5024_POW_PIN, Bit_SET)
#define M_MBI5024_POW_OUT_TOGGLE()
#define M_MBI5024_POW_READIN() EPORT_ReadGpioData(M_MBI5024_POW_PIN)
#define M_MBI5024_POW_DETECT_ENABLE()
#define M_MBI5024_POW_LPM()
#define M_MBI5024_POW_EXIT_HITOLO_ENABLE()
#define M_MBI5024_POW_EXIT_LOTOHI_ENABLE()
#define M_MBI5024_POW_EXIT_HITOLO_DISABLE()
#define M_MBI5024_POW_EXIT_LOTOHI_DISABLE()
#endif
/***************************************************************************/ /**
* @brief
*   MBI5024_CLK驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_MBI5024_CLK_PIN
#define M_MBI5024_CLK_PIN (EPORT_PIN24)
#define M_MBI5024_CLK_PORT
#define M_MBI5024_CLK_OUT_PP() EPORT_ConfigGpio(M_MBI5024_CLK_PIN, GPIO_OUTPUT);
#define M_MBI5024_CLK_IN_NOPULL()
#define M_MBI5024_CLK_IN_IPU()                       \
    EPORT_ConfigGpio(M_MBI5024_CLK_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_CLK_PIN, ENABLE)
#define M_MBI5024_CLK_IN_IPD()                       \
    EPORT_ConfigGpio(M_MBI5024_CLK_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_CLK_PIN, DISABLE)
#define M_MBI5024_CLK_OUT_0() EPORT_WriteGpioData(M_MBI5024_CLK_PIN, Bit_RESET)
#define M_MBI5024_CLK_OUT_1() EPORT_WriteGpioData(M_MBI5024_CLK_PIN, Bit_SET)
#define M_MBI5024_CLK_OUT_TOGGLE()
#define M_MBI5024_CLK_READIN() EPORT_ReadGpioData(M_MBI5024_CLK_PIN)
#define M_MBI5024_CLK_DETECT_ENABLE()
#define M_MBI5024_CLK_LPM()
#define M_MBI5024_CLK_EXIT_HITOLO_ENABLE()
#define M_MBI5024_CLK_EXIT_LOTOHI_ENABLE()
#define M_MBI5024_CLK_EXIT_HITOLO_DISABLE()
#define M_MBI5024_CLK_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   MBI5024_SDI驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_MBI5024_SDI_PIN
#define M_MBI5024_SDI_PIN (EPORT_PIN23)
#define M_MBI5024_SDI_PORT
#define M_MBI5024_SDI_OUT_PP() EPORT_ConfigGpio(M_MBI5024_SDI_PIN, GPIO_OUTPUT);
#define M_MBI5024_SDI_IN_NOPULL()
#define M_MBI5024_SDI_IN_IPU()                       \
    EPORT_ConfigGpio(M_MBI5024_SDI_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_SDI_PIN, ENABLE)
#define M_MBI5024_SDI_IN_IPD()                       \
    EPORT_ConfigGpio(M_MBI5024_SDI_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_SDI_PIN, DISABLE)
#define M_MBI5024_SDI_OUT_0() EPORT_WriteGpioData(M_MBI5024_SDI_PIN, Bit_RESET)
#define M_MBI5024_SDI_OUT_1() EPORT_WriteGpioData(M_MBI5024_SDI_PIN, Bit_SET)
#define M_MBI5024_SDI_OUT_TOGGLE()
#define M_MBI5024_SDI_READIN() EPORT_ReadGpioData(M_MBI5024_SDI_PIN)
#define M_MBI5024_SDI_DETECT_ENABLE()
#define M_MBI5024_SDI_LPM()
#define M_MBI5024_SDI_EXIT_HITOLO_ENABLE()
#define M_MBI5024_SDI_EXIT_LOTOHI_ENABLE()
#define M_MBI5024_SDI_EXIT_HITOLO_DISABLE()
#define M_MBI5024_SDI_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   MBI5024_LE驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_MBI5024_LE_PIN
#define M_MBI5024_LE_PIN (EPORT_PIN30)
#define M_MBI5024_LE_PORT
#define M_MBI5024_LE_OUT_PP() EPORT_ConfigGpio(M_MBI5024_LE_PIN, GPIO_OUTPUT);
#define M_MBI5024_LE_IN_NOPULL()
#define M_MBI5024_LE_IN_IPU()                       \
    EPORT_ConfigGpio(M_MBI5024_LE_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_LE_PIN, ENABLE)
#define M_MBI5024_LE_IN_IPD()                       \
    EPORT_ConfigGpio(M_MBI5024_LE_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_MBI5024_LE_PIN, DISABLE)
#define M_MBI5024_LE_OUT_0() EPORT_WriteGpioData(M_MBI5024_LE_PIN, Bit_RESET)
#define M_MBI5024_LE_OUT_1() EPORT_WriteGpioData(M_MBI5024_LE_PIN, Bit_SET)
#define M_MBI5024_LE_OUT_TOGGLE()
#define M_MBI5024_LE_READIN() EPORT_ReadGpioData(M_MBI5024_LE_PIN)
#define M_MBI5024_LE_DETECT_ENABLE()
#define M_MBI5024_LE_LPM()
#define M_MBI5024_LE_EXIT_HITOLO_ENABLE()
#define M_MBI5024_LE_EXIT_LOTOHI_ENABLE()
#define M_MBI5024_LE_EXIT_HITOLO_DISABLE()
#define M_MBI5024_LE_EXIT_LOTOHI_DISABLE()
#endif  

/***************************************************************************/ /**
* @brief
*   CCM4202_INT驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_CCM4202_INT_PIN
#define M_CCM4202_INT_PIN (SUBC0_GINT1)
#define M_CCM4202_INT_PORT
#define M_CCM4202_INT_OUT_PP() drv_SubC0_ConfigGpio(M_CCM4202_INT_PIN, GPIO_OUTPUT)
#define M_CCM4202_INT_IN_NOPULL()                    \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_CCM4202_INT_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_CCM4202_INT_PIN, SUBC0_DISPULL);  \
  } while (0)
#define M_CCM4202_INT_IN_IPU()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_CCM4202_INT_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_CCM4202_INT_PIN, SUBC0_PULLUP);  \
  } while (0)
#define M_CCM4202_INT_IN_IPD()                       \
  do                                           \
  {                                            \
    drv_SubC0_ConfigGpio(M_CCM4202_INT_PIN, GPIO_INPUT); \
    drv_Subc0_configIOPull(M_CCM4202_INT_PIN, SUBC0_PULLDOWN);  \
  } while (0)
#define M_CCM4202_INT_OUT_0() (drv_SubC0_WriteGpioData(M_CCM4202_INT_PIN, Bit_RESET))
#define M_CCM4202_INT_OUT_1() (drv_SubC0_WriteGpioData(M_CCM4202_INT_PIN, Bit_SET))
#define M_CCM4202_INT_OUT_TOGGLE()
#define M_CCM4202_INT_READIN() (drv_SubC0_ReadGpioData(M_CCM4202_INT_PIN))
#define M_CCM4202_INT_DETECT_ENABLE()
#define M_CCM4202_INT_LPM()
#define M_CCM4202_INT_EXIT_FALL_ENABLE()           \
  do                                           \
  {                                            \
	drv_SubC0_ITTypeConfig(M_CCM4202_INT_PIN,GPIO_INT_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_CCM4202_INT_PIN,ENABLE);	\
  } while (0)
#define M_CCM4202_INT_EXIT_RISE_ENABLE()          \
  do                                          \
  {                                           \
    drv_SubC0_ITTypeConfig(M_CCM4202_INT_PIN,GPIO_INT_RISING_EDGE); \
	drv_SubC0_ITConfig(M_CCM4202_INT_PIN,ENABLE);	\
  } while (0)
#define M_CCM4202_INT_EXIT_EDGE_ENABLE()                    \
  do                                                  \
  {                                                   \
    drv_SubC0_ITTypeConfig(M_CCM4202_INT_PIN,GPIO_INT_RISING_FALLING_EDGE); \
	drv_SubC0_ITConfig(M_CCM4202_INT_PIN,ENABLE);	\
  } while (0)
#define M_CCM4202_INT_EXIT_FALL_DISABLE() (drv_SubC0_ITConfig(M_CCM4202_INT_PIN, DISABLE))
#define M_CCM4202_INT_EXIT_RISE_DISABLE() (drv_SubC0_ITConfig(M_CCM4202_INT_PIN, DISABLE))
#define M_CCM4202_INT_EXIT_EDGE_DISABLE() (drv_SubC0_ITConfig(M_CCM4202_INT_PIN, DISABLE))
#define M_CCM4202_INT_EXIT_INT_DISABLE() (drv_SubC0_ITConfig(M_CCM4202_INT_PIN, DISABLE))  
#endif
  
/***************************************************************************/ /**
* @brief
*   BAT_MS驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_BAT_MS_PIN
#define M_BAT_MS_PIN 
#define M_BAT_MS_PORT
#define M_BAT_MS_OUT_PP() 
#define M_BAT_MS_IN_NOPULL()                   
#define M_BAT_MS_IN_IPU()                      
#define M_BAT_MS_IN_IPD()                       
#define M_BAT_MS_OUT_0() 
#define M_BAT_MS_OUT_1() 
#define M_BAT_MS_OUT_TOGGLE()
#define M_BAT_MS_READIN() 
#define M_BAT_MS_DETECT_ENABLE()
#define M_BAT_MS_LPM()
#define M_BAT_MS_EXIT_FALL_ENABLE()           
#define M_BAT_MS_EXIT_RISE_ENABLE()          
#define M_BAT_MS_EXIT_EDGE_ENABLE()                    
#define M_BAT_MS_EXIT_FALL_DISABLE()
#define M_BAT_MS_EXIT_RISE_DISABLE() 
#define M_BAT_MS_EXIT_EDGE_DISABLE() 
#define M_BAT_MS_EXIT_INT_DISABLE()
#endif   

/***************************************************************************/ /**
* @brief
*   WIFI_TX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_WIFI_TX_PIN
#define M_WIFI_TX_PIN (UART_RX)
#define M_WIFI_TX_PORT (SCI1)
#define M_WIFI_TX_OUT_PP() UART_ConfigGpio(M_WIFI_TX_PORT, M_WIFI_TX_PIN, GPIO_OUTPUT)
#define M_WIFI_TX_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_TX_PORT, M_WIFI_TX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_TX_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_TX_PORT, M_WIFI_TX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_TX_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_TX_PORT, M_WIFI_TX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_TX_OUT_0() UART_WriteGpioData(M_WIFI_TX_PORT, M_WIFI_TX_PIN, Bit_RESET)
#define M_WIFI_TX_OUT_1() UART_WriteGpioData(M_WIFI_TX_PORT, M_WIFI_TX_PIN, Bit_SET)
#define M_WIFI_TX_OUT_TOGGLE()
#define M_WIFI_TX_READIN() I2C_ReadGpioData(M_WIFI_TX_PORT, M_WIFI_TX_PIN)
#define M_WIFI_TX_DETECT_ENABLE()
#define M_WIFI_TX_LPM()
#define M_WIFI_TX_EXIT_HITOLO_ENABLE()
#define M_WIFI_TX_EXIT_LOTOHI_ENABLE()
#define M_WIFI_TX_EXIT_HITOLO_DISABLE()
#define M_WIFI_TX_EXIT_LOTOHI_DISABLE()
#endif
  
/***************************************************************************/ /**
* @brief
*   WIFI_RX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_WIFI_RX_PIN
#define M_WIFI_RX_PIN (UART_TX)
#define M_WIFI_RX_PORT (SCI1)
#define M_WIFI_RX_OUT_PP() UART_ConfigGpio(M_WIFI_RX_PORT, M_WIFI_RX_PIN, GPIO_OUTPUT)
#define M_WIFI_RX_IN_NOPULL()                              \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_RX_PORT, M_WIFI_RX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_RX_IN_IPU()                                 \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_RX_PORT, M_WIFI_RX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_RX_IN_IPD()                                 \
  do                                                   \
  {                                                    \
    UART_ConfigGpio(M_WIFI_RX_PORT, M_WIFI_RX_PIN, GPIO_INPUT); \
  } while (0)
#define M_WIFI_RX_OUT_0() UART_WriteGpioData(M_WIFI_RX_PORT, M_WIFI_RX_PIN, Bit_RESET)
#define M_WIFI_RX_OUT_1() UART_WriteGpioData(M_WIFI_RX_PORT, M_WIFI_RX_PIN, Bit_SET)
#define M_WIFI_RX_OUT_TOGGLE()
#define M_WIFI_RX_READIN() I2C_ReadGpioData(M_WIFI_RX_PORT, M_WIFI_RX_PIN)
#define M_WIFI_RX_DETECT_ENABLE()
#define M_WIFI_RX_LPM()
#define M_WIFI_RX_EXIT_HITOLO_ENABLE()
#define M_WIFI_RX_EXIT_LOTOHI_ENABLE()
#define M_WIFI_RX_EXIT_HITOLO_DISABLE()
#define M_WIFI_RX_EXIT_LOTOHI_DISABLE()
#endif  
  
/***************************************************************************/ /**
* @brief
*   LED_W驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_LED_W_PIN
#define M_LED_W_PIN (EPORT_PIN13)
#define M_LED_W_PORT
#define M_LED_W_OUT_PP() EPORT_ConfigGpio(M_LED_W_PIN, GPIO_OUTPUT);
#define M_LED_W_IN_NOPULL()                    \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_W_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_W_PIN, DISABLE);  \
  } while (0)
#define M_LED_W_IN_IPU()                       \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_W_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_W_PIN, ENABLE);   \
    EPORT_PullUPControl(M_LED_W_PIN, ENABLE);  \
  } while (0)
#define M_LED_W_IN_IPD()                       \
  do                                               \
  {                                                \
    EPORT_ConfigGpio(M_LED_W_PIN, GPIO_INPUT); \
    EPORT_PullupConfig(M_LED_W_PIN, ENABLE);   \
    EPORT_PullUPControl(M_LED_W_PIN, DISABLE); \
  } while (0)
#define M_LED_W_OUT_0() (EPORT_WriteGpioData(M_LED_W_PIN, Bit_RESET))
#define M_LED_W_OUT_1() (EPORT_WriteGpioData(M_LED_W_PIN, Bit_SET))
#define M_LED_W_OUT_TOGGLE()
#define M_LED_W_READIN() (EPORT_ReadGpioData(M_LED_W_PIN))
#define M_LED_W_DETECT_ENABLE()
#define M_LED_W_LPM()
#define M_LED_W_EXIT_HITOLO_ENABLE()           \
  do                                               \
  {                                                \
    EPORT_Init(M_LED_W_PIN, FALLING_EDGE_INT); \
  } while (0)
#define M_LED_W_EXIT_LOTOHI_ENABLE()                  \
  do                                                      \
  {                                                       \
    EPORT_Init(M_LED_W_PIN, RISING_FALLING_EDGE_INT); \
  } while (0)
#define M_LED_W_EXIT_HITOLO_DISABLE() (EPORT_ITConfig(M_LED_W_PIN, DISABLE))
#define M_LED_W_EXIT_LOTOHI_DISABLE() (EPORT_ITConfig(M_LED_W_PIN, DISABLE))
#endif
/*************************变量结构声明*************************/
typedef void (*IntHandler_t)(void); //中断回调句柄
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef MY_CONFIG_GLOBALS
#define MY_CONFIG_EXT
#else
#define MY_CONFIG_EXT extern
#endif /*MY_CONFIG_GLOBALS*/

  /*************************公共变量*************************/
  MY_CONFIG_EXT const uint8_t gRootKey[16][8];
  /*************************公共函数*************************/
  MY_CONFIG_EXT void Sub_C0_Init(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*MY_CONFIG_MODULE_EN*/

#endif /*MY_CONFIG_MODULE_EN*/
#endif /*__MY_CONFIG_H__*/
/************************************Code End******************************************/
