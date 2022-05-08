/***************************************************************************************
**文件名:     DRV_Finger.h
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
#ifndef __DRV_FINGER_H__
#define __DRV_FINGER_H__
#include "MyConfig.h"
#include "spi_drv.h"
#include "spi_reg.h"
//#include "memmap.h"

#define DRV_FINGER_MODULE_EN (1) //模块使能
#if DRV_FINGER_MODULE_EN
/*************************专用宏定义*************************/
#define DRV_FINGER_USE_FINGER_LIB (1) //使用指纹算法库
#define DRV_FINGER_ALL (80) //全部指纹数量
//#define DRV_FINGER_USE_FINGER_MODULE (1) //使用指纹模组
#define FINGER_UART_TX_BUF_SIZE 256                                           /**< UART TX buffer size. */
#define FINGER_UART_RX_BUF_SIZE 256                                           /**< UART RX buffer size. */

/***************************************************************************/ /**
* @brief
*   FINGER_SCK驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_SCK_PIN
#define M_FINGER_SCK_PIN (SPI_SCK)
#define M_FINGER_SCK_PORT (SPI2_2)
#define M_FINGER_SCK_OUT_PP() SPI_ConfigGpio(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN, GPIO_OUTPUT);
#define M_FINGER_SCK_IN_NOPULL()                    \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SCK_IN_IPU()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SCK_IN_IPD()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SCK_OUT_0() (SPI_WriteGpioData(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN,Bit_RESET))
#define M_FINGER_SCK_OUT_1() (SPI_WriteGpioData(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN,Bit_SET))
#define M_FINGER_SCK_OUT_TOGGLE()
#define M_FINGER_SCK_READIN() (SPI_ReadGpioData(M_FINGER_SCK_PORT, M_FINGER_SCK_PIN,Bit_SET))
#define M_FINGER_SCK_DETECT_ENABLE()
#define M_FINGER_SCK_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   FINGER_MOSI驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_MOSI_PIN
#define M_FINGER_MOSI_PIN (SPI_MOSI)
#define M_FINGER_MOSI_PORT (SPI2_2)
#define M_FINGER_MOSI_OUT_PP() SPI_ConfigGpio(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN, GPIO_OUTPUT);
#define M_FINGER_MOSI_IN_NOPULL()                    \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MOSI_IN_IPU()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MOSI_IN_IPD()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MOSI_OUT_0() (SPI_WriteGpioData(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN,Bit_RESET))
#define M_FINGER_MOSI_OUT_1() (SPI_WriteGpioData(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN,Bit_SET))
#define M_FINGER_MOSI_OUT_TOGGLE()
#define M_FINGER_MOSI_READIN() (SPI_ReadGpioData(M_FINGER_MOSI_PORT, M_FINGER_MOSI_PIN,Bit_SET))
#define M_FINGER_MOSI_DETECT_ENABLE()
#define M_FINGER_MOSI_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   FINGER_SS驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_SS_PIN
#define M_FINGER_SS_PIN (SPI_SS)
#define M_FINGER_SS_PORT (SPI2_2)
#define M_FINGER_SS_OUT_PP() SPI_ConfigGpio(M_FINGER_SS_PORT, M_FINGER_SS_PIN, GPIO_OUTPUT);
#define M_FINGER_SS_IN_NOPULL()                    \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SS_PORT, M_FINGER_SS_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SS_IN_IPU()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SS_PORT, M_FINGER_SS_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SS_IN_IPD()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_SS_PORT, M_FINGER_SS_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_SS_OUT_0() (SPI_WriteGpioData(M_FINGER_SS_PORT, M_FINGER_SS_PIN,Bit_RESET))
#define M_FINGER_SS_OUT_1() (SPI_WriteGpioData(M_FINGER_SS_PORT, M_FINGER_SS_PIN,Bit_SET))
#define M_FINGER_SS_OUT_TOGGLE()
#define M_FINGER_SS_READIN() (SPI_ReadGpioData(M_FINGER_SS_PORT, M_FINGER_SS_PIN,Bit_SET))
#define M_FINGER_SS_DETECT_ENABLE()
#define M_FINGER_SS_LPM()
#endif

/***************************************************************************/ /**
* @brief
*   FINGER_MISO驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_MISO_PIN
#define M_FINGER_MISO_PIN (SPI_MISO)
#define M_FINGER_MISO_PORT (SPI2_2)
#define M_FINGER_MISO_OUT_PP() SPI_ConfigGpio(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN, GPIO_OUTPUT);
#define M_FINGER_MISO_IN_NOPULL()                    \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MISO_IN_IPU()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MISO_IN_IPD()                       \
  do                                                \
  {                                                 \
    SPI_ConfigGpio(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN, GPIO_INPUT); \
  } while (0)
#define M_FINGER_MISO_OUT_0() (SPI_WriteGpioData(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN,Bit_RESET))
#define M_FINGER_MISO_OUT_1() (SPI_WriteGpioData(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN,Bit_SET))
#define M_FINGER_MISO_OUT_TOGGLE()
#define M_FINGER_MISO_READIN() (SPI_ReadGpioData(M_FINGER_MISO_PORT, M_FINGER_MISO_PIN,Bit_SET))
#define M_FINGER_MISO_DETECT_ENABLE()
#define M_FINGER_MISO_LPM()
#endif
/***************************************************************************/ /**
* @brief
*   FINGER_POW驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_POW_PIN
#define M_FINGER_POW_PIN (17)
#define M_FINGER_POW_PORT
#define M_FINGER_POW_OUT_PP() nrf_gpio_cfg_output(M_FINGER_POW_PIN)
#define M_FINGER_POW_IN_NOPULL() nrf_gpio_cfg_input(M_FINGER_POW_PIN, NRF_GPIO_PIN_NOPULL)
#define M_FINGER_POW_IN_IPU() nrf_gpio_cfg_input(M_FINGER_POW_PIN, NRF_GPIO_PIN_PULLUP)
#define M_FINGER_POW_IN_IPD() nrf_gpio_cfg_input(M_FINGER_POW_PIN, NRF_GPIO_PIN_PULLDOWN)
#define M_FINGER_POW_OUT_0() nrf_gpio_pin_clear(M_FINGER_POW_PIN)
#define M_FINGER_POW_OUT_1() nrf_gpio_pin_set(M_FINGER_POW_PIN)
#define M_FINGER_POW_OUT_TOGGLE() nrf_gpio_pin_toggle(M_FINGER_POW_PIN)
#define M_FINGER_POW_READIN() nrf_gpio_pin_read(M_FINGER_POW_PIN)
#define M_FINGER_POW_DETECT_ENABLE()
#define M_FINGER_POW_LPM() M_FINGER_POW_OUT_0()
#define M_FINGER_POW_EXIT_HITOLO_ENABLE()
#define M_FINGER_POW_EXIT_LOTOHI_ENABLE()
#define M_FINGER_POW_EXIT_HITOLO_DISABLE()
#define M_FINGER_POW_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   FINGER_RX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_RX_PIN
#define M_FINGER_RX_PIN (19)
#define M_FINGER_RX_PORT
#define M_FINGER_RX_OUT_PP() nrf_gpio_cfg_output(M_FINGER_RX_PIN)
#define M_FINGER_RX_IN_NOPULL() nrf_gpio_cfg_input(M_FINGER_RX_PIN, NRF_GPIO_PIN_NOPULL)
#define M_FINGER_RX_IN_IPU() nrf_gpio_cfg_input(M_FINGER_RX_PIN, NRF_GPIO_PIN_PULLUP)
#define M_FINGER_RX_IN_IPD() nrf_gpio_cfg_input(M_FINGER_RX_PIN, NRF_GPIO_PIN_PULLDOWN)
#define M_FINGER_RX_OUT_0() nrf_gpio_pin_clear(M_FINGER_RX_PIN)
#define M_FINGER_RX_OUT_1() nrf_gpio_pin_set(M_FINGER_RX_PIN)
#define M_FINGER_RX_OUT_TOGGLE() nrf_gpio_pin_toggle(M_FINGER_RX_PIN)
#define M_FINGER_RX_READIN() nrf_gpio_pin_read(M_FINGER_RX_PIN)
#define M_FINGER_RX_DETECT_ENABLE()
#define M_FINGER_RX_LPM() M_FINGER_RX_OUT_0()
#define M_FINGER_RX_EXIT_HITOLO_ENABLE()
#define M_FINGER_RX_EXIT_LOTOHI_ENABLE()
#define M_FINGER_RX_EXIT_HITOLO_DISABLE()
#define M_FINGER_RX_EXIT_LOTOHI_DISABLE()
#endif

/***************************************************************************/ /**
* @brief
*   FINGER_TX驱动接口
* 注意，外设需要配置的到对应的配置文件里配置
* LPM() IO 低功耗配置，不同的单片机，不同电路结构处理方式不同。
*
******************************************************************************/
#ifndef M_FINGER_TX_PIN
#define M_FINGER_TX_PIN (20)
#define M_FINGER_TX_PORT
#define M_FINGER_TX_OUT_PP() nrf_gpio_cfg_output(M_FINGER_TX_PIN)
#define M_FINGER_TX_IN_NOPULL() nrf_gpio_cfg_input(M_FINGER_TX_PIN, NRF_GPIO_PIN_NOPULL)
#define M_FINGER_TX_IN_IPU() nrf_gpio_cfg_input(M_FINGER_TX_PIN, NRF_GPIO_PIN_PULLUP)
#define M_FINGER_TX_IN_IPD() nrf_gpio_cfg_input(M_FINGER_TX_PIN, NRF_GPIO_PIN_PULLDOWN)
#define M_FINGER_TX_OUT_0() nrf_gpio_pin_clear(M_FINGER_TX_PIN)
#define M_FINGER_TX_OUT_1() nrf_gpio_pin_set(M_FINGER_TX_PIN)
#define M_FINGER_TX_OUT_TOGGLE() nrf_gpio_pin_toggle(M_FINGER_TX_PIN)
#define M_FINGER_TX_READIN() nrf_gpio_pin_read(M_FINGER_TX_PIN)
#define M_FINGER_TX_DETECT_ENABLE()
#define M_FINGER_TX_LPM() M_FINGER_TX_OUT_0()
#define M_FINGER_TX_EXIT_HITOLO_ENABLE()
#define M_FINGER_TX_EXIT_LOTOHI_ENABLE()
#define M_FINGER_TX_EXIT_HITOLO_DISABLE()
#define M_FINGER_TX_EXIT_LOTOHI_DISABLE()
#endif
/******************************************************
指令代码表
*******************************************************/
#define DRV_FINGER_CMD_GenImg 0x01        //录指纹图像
#define DRV_FINGER_CMD_Img2Tz 0x02        //图像转特征
#define DRV_FINGER_CMD_Match 0x03         //特征比对
#define DRV_FINGER_CMD_Serach 0x04        //搜索指纹
#define DRV_FINGER_CMD_RegModel 0x05      //特征合成模板
#define DRV_FINGER_CMD_Store 0x06         //存储模板
#define DRV_FINGER_CMD_LoadChar 0x07      //读出模板
#define DRV_FINGER_CMD_UpChar 0x08        //上传特征
#define DRV_FINGER_CMD_DownChr 0x09       //下载特征
#define DRV_FINGER_CMD_UpImage 0x0a       //上传图像
#define DRV_FINGER_CMD_DownImage 0x0b     //下载图像
#define DRV_FINGER_CMD_DeletChar 0x0c     //删除模板
#define DRV_FINGER_CMD_Empty 0x0d         //清空指纹库
#define DRV_FINGER_CMD_SetSysPara 0x0e    //设置系统参数
#define DRV_FINGER_CMD_ReadSysPara 0x0f   //读系统参数
#define DRV_FINGER_CMD_Identify 0x11      //验证指纹
#define DRV_FINGER_CMD_SetPwd 0x12        //设置口令
#define DRV_FINGER_CMD_VfyPwd 0x13        //效验口令
#define DRV_FINGER_CMD_GetRandomCode 0x14 //采样随机数
#define DRV_FINGER_CMD_SetAddr 0x15       //设置地址
#define DRV_FINGER_CMD_WriteNotepad 0x18  //写记事本
#define DRV_FINGER_CMD_ReadNotepad 0x19   //读记事本
#define DRV_FINGER_CMD_TemplateNum 0x1d   //读指纹模板数
#define DRV_FINGER_CMD_ReadConList 0x1f   //读指纹模板索引表
#define DRV_FINGER_CMD_OpenLED 0x50       //打开指纹照明背景灯
#define DRV_FINGER_CMD_CloseLED 0x51      //关闭指纹照明背景灯
#define DRV_FINGER_CMD_GetImageFree 0x52  //无灯控采集指纹图像
#define DRV_FINGER_CMD_GetEcho 0x53       //握手
#define DRV_FINGER_CMD_AutoLogin 0x54     //自动登记
#define DRV_FINGER_CMD_AutoSearch 0x55    //自动搜索
#define DRV_FINGER_CMD_SearchResBack 0x56 //搜索指纹（带残留判断）
#define DRV_FINGER_CMD_Sleep 0x33         //进入休眠，注意，这个指令目前适用于FTW-8003指纹模块。
#define DRV_FINGER_GetChipSN 0x34         //获取芯片唯一序列号
#define DRV_FINGER_ControlBLN 0x3C        //LED控制灯指令
/******************************************************
确认码定义表
*******************************************************/
#define DRV_FINGER_R_Ok 0x00              //指令执行完毕或OK；
#define DRV_FINGER_R_Error 0x01           //数据包接收错误；
#define DRV_FINGER_R_NoFinger 0x02        //传感器上没有手指；
#define DRV_FINGER_R_GenImgFail 0x03      //录入指纹图像失败；
#define DRV_FINGER_R_TzFail 0x06          //指纹图像太乱而生不成特征；
#define DRV_FINGER_R_TzLittle 0x07        //指纹图像正常，但特征点太少（或面积太小）而生不成特征；
#define DRV_FINGER_R_MatchFail 0x08       //指纹不匹配；
#define DRV_FINGER_R_SerachFail 0x09      //没搜索到指纹；
#define DRV_FINGER_R_RegModelFail 0x0a    //特征合并失败；
#define DRV_FINGER_R_AddressOverflow 0x0b //访问指纹库时地址序号超出指纹库范围；
#define DRV_FINGER_R_LoadCharFail 0x0c    //从指纹库读模板出错或无效；
#define DRV_FINGER_R_UpCharFail 0x0d      //上传特征失败；
#define DRV_FINGER_R_AcceptFail 0x0e      //模块不能接受后续数据包；
#define DRV_FINGER_R_UpImageFail 0x0f     //上传图像失败；
#define DRV_FINGER_R_DeletCharFail 0x10   //删除模板失败；
#define DRV_FINGER_R_EmptyFail 0x11       //清空指纹库失败；
#define DRV_FINGER_R_VfyPwdFail 0x13      //口令不正确；
#define DRV_FINGER_R_NoImg 0x15           //缓冲区内没有有效原始图而生不成图像；
#define DRV_FINGER_R_FlashFail 0x18       //读写FLASH出错；
#define DRV_FINGER_R_InvalidRegister 0x1a //无效寄存器号；
#define DRV_FINGER_R_AddressError 0x20    //地址码错误
#define DRV_FINGER_R_VfyPwdFirst 0x21     //必须验证口令；
#define DRV_FINGER_R_Residual 0x22        //表示残留指纹
#define DRV_FINGER_R_NoChar 0x23          //表示指定区间不存在有效指纹模板
/*************************变量结构声明*************************/
typedef void (*FingerIntHandler_t)(void);
#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef DRV_FINGER_GLOBALS
#define DRV_FINGER_EXT
#else
#define DRV_FINGER_EXT extern
#endif /*DRV_FINGER_GLOBALS*/

  /*************************公共变量*************************/
  DRV_FINGER_EXT FingerIntHandler_t AlarmIntHandler;
  /*************************公共函数*************************/
  DRV_FINGER_EXT int8_t DRV_Finger_Init(void);
  DRV_FINGER_EXT int8_t DRV_Finger_GenImg(uint16_t id, uint8_t cmd);
  DRV_FINGER_EXT uint8_t *DRV_Finger_GetAckData(void);
  DRV_FINGER_EXT int8_t DRV_Finger_Img2Tz(uint8_t id);
  DRV_FINGER_EXT int8_t DRV_Finger_RegModel(void);
  DRV_FINGER_EXT int8_t DRV_Finger_Store(uint8_t charBuf, uint16_t model);
  DRV_FINGER_EXT int8_t DRV_Finger_Serach(uint8_t charBuf, uint16_t model, uint16_t num);
  DRV_FINGER_EXT int8_t DRV_Finger_DeletChar(uint16_t model, uint16_t num);
  DRV_FINGER_EXT int8_t DRV_Finger_Empty(void);
  DRV_FINGER_EXT int8_t DRV_Finger_ResetCmd(void);
  DRV_FINGER_EXT void DRV_Finger_EnterSleepConfig(void);
  DRV_FINGER_EXT int8_t DRV_Finger_ChkFingerLeave(void);

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*DRV_FINGER_MODULE_EN*/

#endif /*DRV_FINGER_MODULE_EN*/
#endif /*__DRV_FINGER_H__*/
/************************************Code End******************************************/
