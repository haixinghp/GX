/***************************************************************************************
**�ļ���:     MyConfig.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
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
#define MY_CONFIG_MODULE_EN (1) //ģ��ʹ��
#if MY_CONFIG_MODULE_EN
/*************************ר�ú궨��*************************/
#if 1
#define DEBUG_LOG_EN  //�������ʹ��
#endif

#ifdef DEBUG_LOG_EN

#if 0
#define DIS_RAM_MODE //��ֹRAMģʽ
#endif

#define PWD_TEST_DEBUG

#if 1
#define DEBUG_OUT_USE_UART //ʹ�ô�����Ϊ���Կ�
#endif

#if 0
#define DEBUG_OUT_USE_JLINK_RTT //ʹ��jlink��rtt��Ϊ���Կ�
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
#define USE_LITTLE_ENDIAN //ʹ��С��ģʽ
#else
#define USE_BIG_ENDIAN    //ʹ�ô�˶�ģʽ
#endif

#if 1
#define BSWAP_EN //��С��ת��ʹ��
#endif

#ifdef BSWAP_EN
#define BSWAP_16(x) (uint16_t)((((uint16_t)(x)&0x00ff) << 8) | (((uint16_t)(x)&0xff00) >> 8))
#define BSWAP_32(x) (uint32_t)((((uint32_t)(x)&0xff000000) >> 24) | \
                               (((uint32_t)(x)&0x00ff0000) >> 8) |  \
                               (((uint32_t)(x)&0x0000ff00) << 8) |  \
                               (((uint32_t)(x)&0x000000ff) << 24))
#define BSWAP_SAVE_16(x) (x = BSWAP_16(x)) //��С��ת������������
#define BSWAP_SAVE_32(x) (x = BSWAP_32(x)) //��С��ת������������
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
#define SYSTEM_STORAGE_EN (1) //�洢ʹ��

#if 0
#define SYSTEM_VOICE_EN (1) //����ʹ��
#endif

#define SYSTEM_FINGER_EN (1) //ָ��ʹ��

#if 0
#define SYSTEM_BREATHING_LIGHT_EN (1) //������ʹ��
#endif

#if 0
#define SYSTEM_MOTOR_EN (1) //���ʹ��
#endif

#define M_VOS_GUI_ENABLE                      //�������˵�
#define MENU_TIME_CHANGE                      //�˵�ʱ��ɱ仯
#define KEY_RANDOM_LOCK_EN (1)                //���������
#define LOCK_VERSION "DR0.C.A02_20210224"     //���߰汾
#define LOCK_SN "S70000000123456"             //�������к�
#define PRE_ALARM_TIME_LIMIT (15 * 60 * 1000) //���˱���15����
#define POWER_VOLTAGE_WIFI_NOT_UPLOAD (690) //WIFI����
#define POWER_VOLTAGE_LOW (690)             //�͵�ѹ
#define POWER_VOLTAGE_LOW_CANNOT_WORK (650) //�͵�ѹ,�޷�����
#define AES_ROOT_KEY (5)                    //�±�

#define DELAY_MS(X) \
  do                \
  {                 \
    DelayMS(X);     \
  } while (0)

#define WIFI_DEVICE_TYPE (0X0B)      //�����豸����
#define LOCK_OPEN_PARM_NO_CLEAR (1)  //�����Ų����������,���ݱ�����Ӳ����������
#define LOCK_BODY_TYPE_LIUFU (1)     //��������
#define LOCK_BODY_TYPE_KAIYUANLE (2) //��Ԫ������
#define MSG_WIFI_LOGUP_MAIN_SW_ON_CONFIG		(1) //����WIFI�ϱ�
#define MSG_WIFI_LOGUP_MAIN_SW_OFF_CONFIG		(0) //�ر�wifi�ϱ�
#define MSG_WIFI_LOGUP_ON_CONFIG		1
#define MSG_WIFI_LOGUP_OFF_CONFIG		0
#if 1  
#define USE_FACE_WIFI (1) //ʹ������ģ���wifi  
#endif
  
#if 0  
#define USE_WIFI_MODLE (1) //ʹ��wifiģ��  
#endif

#if 1  
#define USE_CATEYE (1) //ʹ��è��  
#endif

#ifdef USE_CATEYE
#if 0
#define USE_CATEYE_SHANGTANG //ʹ��������è��
#endif
#if 1
#define USE_CATEYE_AOBI //ʹ�ð±ȵ�è��
#endif
#endif
 
#if 1
#define USE_FACE_AOBI //ʹ�ð±����� 
#endif
#if 0
#define USE_FACE_SHANGTANG //ʹ���������� 
#endif

#if 1
#define USE_COMM_ENCRYPT //ǰ���ͨѶʹ�ü��� 
#endif

#if 1
#define IR_ENABLE //����ʹ�� 
#endif

#if 1
#define BACK_GET_BAT_STATUS //����ȡ���״̬
#endif

#define DEBUG_LOW_POWER  (0) //���Ե͹���

#define HAVE_BOOTLOADR  (1) //�̼�����BOOT����Ҫ�޸Ĺ���������IROM1��ֵ

#define BACK_LOW_RST (0) //������͸�λ

#define USE_ID2  (1) //ʹ��ID2���� 

#define USE_WDT (1) //ʹ�ÿ��Ź�

#define SPI1_1 ((SPI_TypeDef *)(SPI1_BASE_ADDR))
#define SPI2_2 ((SPI_TypeDef *)(SPI2_BASE_ADDR))
#define SPI3_3 ((SPI_TypeDef *)(SPI3_BASE_ADDR))

/******************************
boot��صĶ���
*******************************/
/********************************************
  ���´������оƬ�ڲ�FLASH��ע��д��ʱ�ֽڶ���
********************************************/  
#define FLASH_SECTOR_SIZE (0x200ul)                                          //�ڲ�FLASHһ�������Ĵ�С  512byte
#define FLASH_BASE_BOOT ((uint32_t)0x8000000)                                //flash����ַ
#define BOOT_BASE FLASH_BASE_BOOT                                            //BootLoader ����ַ
#define BOOT_SIZE (80 * FLASH_SECTOR_SIZE)                                   //BootLoader flash�ߴ�
#define BOOT_PARA_01_ADDRESS (BOOT_BASE + BOOT_SIZE - 2 * FLASH_SECTOR_SIZE) //BootLoader para�洢��ַ
#define BOOT_PARA_02_ADDRESS (BOOT_BASE + BOOT_SIZE - 1 * FLASH_SECTOR_SIZE) //BootLoader para�洢��ַ
#define APP_EXTIST_ADDRESS BOOT_PARA_01_ADDRESS                              //�洢�Ƿ���ڿ��õ�APP��ַ
#define APP_PROGRAM_TYPE_ADDRESS BOOT_PARA_02_ADDRESS                        //�洢������ʽ��ַ
#define APP_PROGRAM_PARA_ADDRESS (BOOT_PARA_02_ADDRESS + 3*4)                 //�洢MCU͸�����ݵĵ�ַ
#define APP_BASE (BOOT_BASE + BOOT_SIZE)                                     //APP flash����ַ
#define APP_SIZE (400 * FLASH_SECTOR_SIZE)                                   //APP flash�ߴ�
#define APP_PARA_ADDRESS (APP_BASE + APP_SIZE - 0x100u)                      //APP para�洢��ַ
#define BACKUP_BASE (APP_BASE + APP_SIZE)                                    //BACKUP flash����ַ
#define BACKUP_SIZE (400 * FLASH_SECTOR_SIZE)                                //BACKUP flash�ߴ�
#define BACKUP_PARA_ADDRESS (BACKUP_BASE + BACKUP_SIZE - 0x100u)             //BACKUP para�洢��ַ



/***************************************************************************/ /**
* @brief
*   CC4202_INT�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   RXD�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   TXD�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   IR_TX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   IR_RX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CAM_TX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CAM_RX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   LED_B_PWM�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   WT_POW�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   WAKE_UP0�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SDA�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SCL�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SDA2�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SCL2�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   X_IRQ_9555�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   HS6621_INT�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FING_POW�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CAM_WIFI_POW_EN�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   POW_CARD�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   NFC_SCK�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   NFC_MOSI�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   NFC_CS�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   NFC_MISO�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_0�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_1�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_2�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_3�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_SCK�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   SPI4_SS�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   X_FACE_POW_EN�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   ALARM�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   HS6621_RSTB�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   X_CHANGE_9555�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CAM_POW_EN�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   BELL�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   3802_IRQ�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   IR_IRQ�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CAM_POW_EN1�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   4052_A0�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   4052_A1�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FING_IRQ�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   4052_EN�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   MBI5024_POW�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   MBI5024_CLK�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   MBI5024_SDI�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   MBI5024_LE�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   CCM4202_INT�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   BAT_MS�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   WIFI_TX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   WIFI_RX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   LED_W�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
/*************************�����ṹ����*************************/
typedef void (*IntHandler_t)(void); //�жϻص����
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef MY_CONFIG_GLOBALS
#define MY_CONFIG_EXT
#else
#define MY_CONFIG_EXT extern
#endif /*MY_CONFIG_GLOBALS*/

  /*************************��������*************************/
  MY_CONFIG_EXT const uint8_t gRootKey[16][8];
  /*************************��������*************************/
  MY_CONFIG_EXT void Sub_C0_Init(void);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*MY_CONFIG_MODULE_EN*/

#endif /*MY_CONFIG_MODULE_EN*/
#endif /*__MY_CONFIG_H__*/
/************************************Code End******************************************/
