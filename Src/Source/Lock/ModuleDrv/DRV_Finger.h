/***************************************************************************************
**�ļ���:     DRV_Finger.h
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
#ifndef __DRV_FINGER_H__
#define __DRV_FINGER_H__
#include "MyConfig.h"
#include "spi_drv.h"
#include "spi_reg.h"
//#include "memmap.h"

#define DRV_FINGER_MODULE_EN (1) //ģ��ʹ��
#if DRV_FINGER_MODULE_EN
/*************************ר�ú궨��*************************/
#define DRV_FINGER_USE_FINGER_LIB (1) //ʹ��ָ���㷨��
#define DRV_FINGER_ALL (80) //ȫ��ָ������
//#define DRV_FINGER_USE_FINGER_MODULE (1) //ʹ��ָ��ģ��
#define FINGER_UART_TX_BUF_SIZE 256                                           /**< UART TX buffer size. */
#define FINGER_UART_RX_BUF_SIZE 256                                           /**< UART RX buffer size. */

/***************************************************************************/ /**
* @brief
*   FINGER_SCK�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_MOSI�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_SS�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_MISO�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_POW�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_RX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
*   FINGER_TX�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
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
ָ������
*******************************************************/
#define DRV_FINGER_CMD_GenImg 0x01        //¼ָ��ͼ��
#define DRV_FINGER_CMD_Img2Tz 0x02        //ͼ��ת����
#define DRV_FINGER_CMD_Match 0x03         //�����ȶ�
#define DRV_FINGER_CMD_Serach 0x04        //����ָ��
#define DRV_FINGER_CMD_RegModel 0x05      //�����ϳ�ģ��
#define DRV_FINGER_CMD_Store 0x06         //�洢ģ��
#define DRV_FINGER_CMD_LoadChar 0x07      //����ģ��
#define DRV_FINGER_CMD_UpChar 0x08        //�ϴ�����
#define DRV_FINGER_CMD_DownChr 0x09       //��������
#define DRV_FINGER_CMD_UpImage 0x0a       //�ϴ�ͼ��
#define DRV_FINGER_CMD_DownImage 0x0b     //����ͼ��
#define DRV_FINGER_CMD_DeletChar 0x0c     //ɾ��ģ��
#define DRV_FINGER_CMD_Empty 0x0d         //���ָ�ƿ�
#define DRV_FINGER_CMD_SetSysPara 0x0e    //����ϵͳ����
#define DRV_FINGER_CMD_ReadSysPara 0x0f   //��ϵͳ����
#define DRV_FINGER_CMD_Identify 0x11      //��ָ֤��
#define DRV_FINGER_CMD_SetPwd 0x12        //���ÿ���
#define DRV_FINGER_CMD_VfyPwd 0x13        //Ч�����
#define DRV_FINGER_CMD_GetRandomCode 0x14 //���������
#define DRV_FINGER_CMD_SetAddr 0x15       //���õ�ַ
#define DRV_FINGER_CMD_WriteNotepad 0x18  //д���±�
#define DRV_FINGER_CMD_ReadNotepad 0x19   //�����±�
#define DRV_FINGER_CMD_TemplateNum 0x1d   //��ָ��ģ����
#define DRV_FINGER_CMD_ReadConList 0x1f   //��ָ��ģ��������
#define DRV_FINGER_CMD_OpenLED 0x50       //��ָ������������
#define DRV_FINGER_CMD_CloseLED 0x51      //�ر�ָ������������
#define DRV_FINGER_CMD_GetImageFree 0x52  //�޵ƿزɼ�ָ��ͼ��
#define DRV_FINGER_CMD_GetEcho 0x53       //����
#define DRV_FINGER_CMD_AutoLogin 0x54     //�Զ��Ǽ�
#define DRV_FINGER_CMD_AutoSearch 0x55    //�Զ�����
#define DRV_FINGER_CMD_SearchResBack 0x56 //����ָ�ƣ��������жϣ�
#define DRV_FINGER_CMD_Sleep 0x33         //�������ߣ�ע�⣬���ָ��Ŀǰ������FTW-8003ָ��ģ�顣
#define DRV_FINGER_GetChipSN 0x34         //��ȡоƬΨһ���к�
#define DRV_FINGER_ControlBLN 0x3C        //LED���Ƶ�ָ��
/******************************************************
ȷ���붨���
*******************************************************/
#define DRV_FINGER_R_Ok 0x00              //ָ��ִ����ϻ�OK��
#define DRV_FINGER_R_Error 0x01           //���ݰ����մ���
#define DRV_FINGER_R_NoFinger 0x02        //��������û����ָ��
#define DRV_FINGER_R_GenImgFail 0x03      //¼��ָ��ͼ��ʧ�ܣ�
#define DRV_FINGER_R_TzFail 0x06          //ָ��ͼ��̫�Ҷ�������������
#define DRV_FINGER_R_TzLittle 0x07        //ָ��ͼ����������������̫�٣������̫С����������������
#define DRV_FINGER_R_MatchFail 0x08       //ָ�Ʋ�ƥ�䣻
#define DRV_FINGER_R_SerachFail 0x09      //û������ָ�ƣ�
#define DRV_FINGER_R_RegModelFail 0x0a    //�����ϲ�ʧ�ܣ�
#define DRV_FINGER_R_AddressOverflow 0x0b //����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ��
#define DRV_FINGER_R_LoadCharFail 0x0c    //��ָ�ƿ��ģ��������Ч��
#define DRV_FINGER_R_UpCharFail 0x0d      //�ϴ�����ʧ�ܣ�
#define DRV_FINGER_R_AcceptFail 0x0e      //ģ�鲻�ܽ��ܺ������ݰ���
#define DRV_FINGER_R_UpImageFail 0x0f     //�ϴ�ͼ��ʧ�ܣ�
#define DRV_FINGER_R_DeletCharFail 0x10   //ɾ��ģ��ʧ�ܣ�
#define DRV_FINGER_R_EmptyFail 0x11       //���ָ�ƿ�ʧ�ܣ�
#define DRV_FINGER_R_VfyPwdFail 0x13      //�����ȷ��
#define DRV_FINGER_R_NoImg 0x15           //��������û����Чԭʼͼ��������ͼ��
#define DRV_FINGER_R_FlashFail 0x18       //��дFLASH����
#define DRV_FINGER_R_InvalidRegister 0x1a //��Ч�Ĵ����ţ�
#define DRV_FINGER_R_AddressError 0x20    //��ַ�����
#define DRV_FINGER_R_VfyPwdFirst 0x21     //������֤���
#define DRV_FINGER_R_Residual 0x22        //��ʾ����ָ��
#define DRV_FINGER_R_NoChar 0x23          //��ʾָ�����䲻������Чָ��ģ��
/*************************�����ṹ����*************************/
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

  /*************************��������*************************/
  DRV_FINGER_EXT FingerIntHandler_t AlarmIntHandler;
  /*************************��������*************************/
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
