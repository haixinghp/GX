/***************************************************************************************
**�ļ���:     YY.h
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
#ifndef   __DRV_SPI_H__
#define   __DRV_SPI_H__
#include "config.h"
#define DRV_SPI_MODULE_EN (1) //ģ��ʹ��
#if DRV_SPI_MODULE_EN
/*************************ר�ú궨��*************************/  

/***************************************************************************//**
* @brief
*   CARD_PW�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
*
******************************************************************************/
#define M_CARD_PW_PIN                                 	(0)        
#define M_CARD_PW_PORT                              	
#define M_CARD_PW_OUT_PP()                            	(nrf_gpio_cfg_output(M_CARD_PW_PIN))                                                     
#define M_CARD_PW_IN_NOPULL()                       		(nrf_gpio_cfg_input(M_CARD_PW_PIN,NRF_GPIO_PIN_NOPULL))
#define M_CARD_PW_IN_IPU()                          		(nrf_gpio_cfg_input(M_CARD_PW_PIN,NRF_GPIO_PIN_PULLUP))
#define M_CARD_PW_IN_IPD()                          		(nrf_gpio_cfg_input(M_CARD_PW_PIN,NRF_GPIO_PIN_PULLDOWN))
#define M_CARD_PW_OUT_0()                               (nrf_gpio_pin_clear(M_CARD_PW_PIN))
#define M_CARD_PW_OUT_1()                              	(nrf_gpio_pin_set(M_CARD_PW_PIN))
#define M_CARD_PW_OUT_TOGGLE()                      		(nrf_gpio_pin_toggle(M_CARD_PW_PIN))
#define M_CARD_PW_READIN()                             	(nrf_gpio_pin_read(M_CARD_PW_PIN))
#define M_CARD_PW_DETECT_ENABLE()                   			
#define M_CARD_PW_LPM()    
#define M_CARD_PW_EXIT_HITOLO_ENABLE()                                            
#define M_CARD_PW_EXIT_LOTOHI_ENABLE()                     
#define M_CARD_PW_EXIT_HITOLO_DISABLE()                      
#define M_CARD_PW_EXIT_LOTOHI_DISABLE() 
/***************************************************************************//**
* @brief
*   SPI_SOFT_SS�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
*
******************************************************************************/
#define M_SPI_SOFT_SS_PIN                                 	(29)        
#define M_SPI_SOFT_SS_PORT                              	
#define M_SPI_SOFT_SS_OUT_PP()                            	(nrf_gpio_cfg_output(M_SPI_SOFT_SS_PIN))                                                     
#define M_SPI_SOFT_SS_IN_NOPULL()                       		(nrf_gpio_cfg_input(M_SPI_SOFT_SS_PIN,NRF_GPIO_PIN_NOPULL))
#define M_SPI_SOFT_SS_IN_IPU()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_SS_PIN,NRF_GPIO_PIN_PULLUP))
#define M_SPI_SOFT_SS_IN_IPD()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_SS_PIN,NRF_GPIO_PIN_PULLDOWN))
#define M_SPI_SOFT_SS_OUT_0()                               (nrf_gpio_pin_clear(M_SPI_SOFT_SS_PIN))
#define M_SPI_SOFT_SS_OUT_1()                              	(nrf_gpio_pin_set(M_SPI_SOFT_SS_PIN))
#define M_SPI_SOFT_SS_OUT_TOGGLE()                      		(nrf_gpio_pin_toggle(M_SPI_SOFT_SS_PIN))
#define M_SPI_SOFT_SS_READIN()                             	(nrf_gpio_pin_read(M_SPI_SOFT_SS_PIN))
#define M_SPI_SOFT_SS_DETECT_ENABLE()                   			
#define M_SPI_SOFT_SS_LPM()    
#define M_SPI_SOFT_SS_EXIT_HITOLO_ENABLE()                                            
#define M_SPI_SOFT_SS_EXIT_LOTOHI_ENABLE()                     
#define M_SPI_SOFT_SS_EXIT_HITOLO_DISABLE()                      
#define M_SPI_SOFT_SS_EXIT_LOTOHI_DISABLE()  

/***************************************************************************//**
* @brief
*   SPI_SOFT_MOSI�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
*
******************************************************************************/
#define M_SPI_SOFT_MOSI_PIN                                 	(29)        
#define M_SPI_SOFT_MOSI_PORT                              	
#define M_SPI_SOFT_MOSI_OUT_PP()                            	(nrf_gpio_cfg_output(M_SPI_SOFT_MOSI_PIN))                                                     
#define M_SPI_SOFT_MOSI_IN_NOPULL()                       		(nrf_gpio_cfg_input(M_SPI_SOFT_MOSI_PIN,NRF_GPIO_PIN_NOPULL))
#define M_SPI_SOFT_MOSI_IN_IPU()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_MOSI_PIN,NRF_GPIO_PIN_PULLUP))
#define M_SPI_SOFT_MOSI_IN_IPD()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_MOSI_PIN,NRF_GPIO_PIN_PULLDOWN))
#define M_SPI_SOFT_MOSI_OUT_0()                               (nrf_gpio_pin_clear(M_SPI_SOFT_MOSI_PIN))
#define M_SPI_SOFT_MOSI_OUT_1()                              	(nrf_gpio_pin_set(M_SPI_SOFT_MOSI_PIN))
#define M_SPI_SOFT_MOSI_OUT_TOGGLE()                      		(nrf_gpio_pin_toggle(M_SPI_SOFT_MOSI_PIN))
#define M_SPI_SOFT_MOSI_READIN()                             	(nrf_gpio_pin_read(M_SPI_SOFT_MOSI_PIN))
#define M_SPI_SOFT_MOSI_DETECT_ENABLE()                   			
#define M_SPI_SOFT_MOSI_LPM()    
#define M_SPI_SOFT_MOSI_EXIT_HITOLO_ENABLE()                                            
#define M_SPI_SOFT_MOSI_EXIT_LOTOHI_ENABLE()                     
#define M_SPI_SOFT_MOSI_EXIT_HITOLO_DISABLE()                      
#define M_SPI_SOFT_MOSI_EXIT_LOTOHI_DISABLE()  

/***************************************************************************//**
* @brief
*   SPI_SOFT_MISO�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
*
******************************************************************************/
#define M_SPI_SOFT_MISO_PIN                                 	(30)        
#define M_SPI_SOFT_MISO_PORT                              	
#define M_SPI_SOFT_MISO_OUT_PP()                            	(nrf_gpio_cfg_output(M_SPI_SOFT_MISO_PIN))                                                     
#define M_SPI_SOFT_MISO_IN_NOPULL()                       		(nrf_gpio_cfg_input(M_SPI_SOFT_MISO_PIN,NRF_GPIO_PIN_NOPULL))
#define M_SPI_SOFT_MISO_IN_IPU()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_MISO_PIN,NRF_GPIO_PIN_PULLUP))
#define M_SPI_SOFT_MISO_IN_IPD()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_MISO_PIN,NRF_GPIO_PIN_PULLDOWN))
#define M_SPI_SOFT_MISO_OUT_0()                               (nrf_gpio_pin_clear(M_SPI_SOFT_MISO_PIN))
#define M_SPI_SOFT_MISO_OUT_1()                              	(nrf_gpio_pin_set(M_SPI_SOFT_MISO_PIN))
#define M_SPI_SOFT_MISO_OUT_TOGGLE()                      		(nrf_gpio_pin_toggle(M_SPI_SOFT_MISO_PIN))
#define M_SPI_SOFT_MISO_READIN()                             	(nrf_gpio_pin_read(M_SPI_SOFT_MISO_PIN))
#define M_SPI_SOFT_MISO_DETECT_ENABLE()                   			
#define M_SPI_SOFT_MISO_LPM()    
#define M_SPI_SOFT_MISO_EXIT_HITOLO_ENABLE()                                            
#define M_SPI_SOFT_MISO_EXIT_LOTOHI_ENABLE()                     
#define M_SPI_SOFT_MISO_EXIT_HITOLO_DISABLE()                      
#define M_SPI_SOFT_MISO_EXIT_LOTOHI_DISABLE()  

/***************************************************************************//**
* @brief
*   SPI_SOFT_SCLK�����ӿ�
* ע�⣬������Ҫ���õĵ���Ӧ�������ļ�������
* LPM() IO �͹������ã���ͬ�ĵ�Ƭ������ͬ��·�ṹ����ʽ��ͬ��
*
******************************************************************************/
#define M_SPI_SOFT_SCLK_PIN                                 	(28)        
#define M_SPI_SOFT_SCLK_PORT                              	
#define M_SPI_SOFT_SCLK_OUT_PP()                            	(nrf_gpio_cfg_output(M_SPI_SOFT_SCLK_PIN))                                                     
#define M_SPI_SOFT_SCLK_IN_NOPULL()                       		(nrf_gpio_cfg_input(M_SPI_SOFT_SCLK_PIN,NRF_GPIO_PIN_NOPULL))
#define M_SPI_SOFT_SCLK_IN_IPU()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_SCLK_PIN,NRF_GPIO_PIN_PULLUP))
#define M_SPI_SOFT_SCLK_IN_IPD()                          		(nrf_gpio_cfg_input(M_SPI_SOFT_SCLK_PIN,NRF_GPIO_PIN_PULLDOWN))
#define M_SPI_SOFT_SCLK_OUT_0()                               (nrf_gpio_pin_clear(M_SPI_SOFT_SCLK_PIN))
#define M_SPI_SOFT_SCLK_OUT_1()                              	(nrf_gpio_pin_set(M_SPI_SOFT_SCLK_PIN))
#define M_SPI_SOFT_SCLK_OUT_TOGGLE()                      		(nrf_gpio_pin_toggle(M_SPI_SOFT_SCLK_PIN))
#define M_SPI_SOFT_SCLK_READIN()                             	(nrf_gpio_pin_read(M_SPI_SOFT_SCLK_PIN))
#define M_SPI_SOFT_SCLK_DETECT_ENABLE()                   			
#define M_SPI_SOFT_SCLK_LPM()    
#define M_SPI_SOFT_SCLK_EXIT_HITOLO_ENABLE()                                            
#define M_SPI_SOFT_SCLK_EXIT_LOTOHI_ENABLE()                     
#define M_SPI_SOFT_SCLK_EXIT_HITOLO_DISABLE()                      
#define M_SPI_SOFT_SCLK_EXIT_LOTOHI_DISABLE() 
/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_SPI_GLOBALS
#define DRV_SPI_EXT
#else
#define DRV_SPI_EXT extern
#endif     /*DRV_SPI_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_SPI_EXT void DRV_SPI_init(void);
DRV_SPI_EXT int8_t DRV_SPI_WriteReadNByte(const uint8_t *WirteDat,uint16_t WirteLen,uint8_t *ReadDat,uint16_t ReadLen);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_SPI_MODULE_EN*/

#endif    /*DRV_SPI_MODULE_EN*/
#endif     /*__DRV_SPI_H__*/
/************************************Code End******************************************/



