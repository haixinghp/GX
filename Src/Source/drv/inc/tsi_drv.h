/**
 * @file tsi_drv.h
 * @author System Application Team
 * @brief TSIģ��driver������ͷ�ļ�
 * @version 3.0
 * @date 2020-07-27
 * 
 * @copyright C*Core Copyright (c) 2020
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TSI_DRV_H_
#define _TSI_DRV_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "tsi_reg.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /** @addtogroup Driver
  * @{
  */

    /** @addtogroup TSI
  * @{
  */

    /*** �궨�� *******************************************************************/
    /** @addtogroup TSI Exported Macros
  * @{
  */
#define _CLEAR_SCANFINISHFLAG(TSIx) do{TSIx->TSI_ISR |= TSI_ISR_SCANENDFINISH;}while(0) /*!< ���ɨ����ɱ�־λ */
#define _CLEAR_AWDFLAG(TSIx) do{TSIx->TSI_ISR |= TSI_ISR_AWD;}while(0) /*!< ���TSI�жϻ��ѱ�־λ */

#define _SET_IE_SCANFINISH(TSIx) do{TSIx->TSI_IER |= TSI_IER_SCANFINISH_IE;}while(0) /*!< ʹ��TSIͨ��ɨ������жϹ��� */
#define _CLEAR_IE_SCANFINISH(TSIx) do{TSIx->TSI_IER &= ~TSI_IER_SCANFINISH_IE;}while(0) /*!< ��ʹ��TSIͨ��ɨ������жϹ��� */
#define _SET_IE_AWD(TSIx) do{TSIx->TSI_IER |= TSI_IER_ANALOGWDT_IE;}while(0) /*!< ʹ��TSI�͹��Ĵ������ѹ��� */
#define _CLEAR_IE_AWD(TSIx) do{TSIx->TSI_IER &= ~TSI_IER_ANALOGWDT_IE;}while(0) /*!< ��ʹ��TSI�͹��Ĵ������ѹ��� */

#define _SET_DEFAULT_CR(TSIx) do{TSIx->TSI_CR = 0x50000008;}while(0) /*!< ����TSI_WDCFGR�Ĵ���ΪĬ��ֵ */
#define _SET_TSI_EN(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_EN;}while(0) /*!< ʹ��TSI */
#define _SET_TSI_DISABLE(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_EN;}while(0) /*!< �ر�TSI */
#define _SET_CR_DEFAULAT(TSIx) do{TSIx->TSI_CR |= TSI_CR_DEFAULT;}while(0) /*!< ����TSI_CR�Ĵ���ΪĬ��ֵ */
#define _SET_WORKMODE_SCAN(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_MODE;}while(0) /*!< ����TSIɨ��ģʽΪͨ��ѭ��ɨ��ģʽ */
#define _SET_WORKMODE_SINGLE(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_MODE;}while(0) /*!< ����TSIɨ��ģʽΪ��ͨ��ɨ��ģʽ */
#define _SET_SCANALWAYS(TSIx) do{TSIx->TSI_CR |= TSI_CR_AVALID_ALWAYS;}while(0) /*!< ʹ��TSI����ɨ�����ģʽ */
#define _SET_SCANCYCLICITY(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_AVALID_ALWAYS;}while(0) /*!< ��ʹ��TSI����ɨ�����ģʽ */
#define _SET_SCAN_SEQUENCE_MODE_00001111(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_SEQUENCE_MODE;}while(0) /*!< ����ͨ��ɨ��ģʽΪ00001111... */
#define _SET_SCAN_SEQUENCE_MODE_01234567(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_SEQUENCE_MODE;}while(0) /*!< ����ͨ��ɨ��ģʽΪ012345678... */

#define _SET_TSTR_ANALOG(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_ANALOG;}while(0) /*!< trim the tsi verf */
#define _SET_DISCARDMODE(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_DISCARDMODE;}while(0) /*!< =1ͨ���л�ʱ�������� */
#define _SET_DISCARDMODE_NONE(TSIx) do{TSIx->TSI_TSTR &= ~TSI_TSTR_DISCARDMODE;}while(0) /*!< =1ͨ���л�ʱ�������� */
#define _SET_DISCARD_LEN(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_DISCARDLEN;}while(0) /*!< ��ʧ���ݵĳ��� */
#define _SET_DISCARD_LEN_0(TSIx) do{TSIx->TSI_TSTR &= ~TSI_TSTR_DISCARDLEN;}while(0) /*!< ��ʧ���ݵĳ��� */

#define _SET_DEFAULT_WDCFGR(TSIx) do{TSIx->TSI_WDCFGR = 0x00080100;}while(0) /*!< ����TSI_WDCFGR�Ĵ���ΪĬ��ֵ */
#define _SET_WDCFGR_LP(TSIx) do{TSIx->TSI_WDCFGR = 0x000002e0;}while(0) /*!< ����TSI_WDCFGR�Ĵ���Ϊ�͹������� */

#define _ENABLE_FINEMODE(TSIx) do{TSIx->TSI_CHCR &= ~TSI_CHCR_MODE_COARSE;}while(0) /*!< ���ù���ģʽΪFineģʽ */
#define _ENABLE_COARSEMODE(TSIx) do{TSIx->TSI_CHCR |= TSI_CHCR_MODE_COARSE;}while(0) /*!< ���ù���ģʽΪCoarseģʽ */
#define _SET_CURRENT(TSIx) do{TSIx->TSI_CHCR |= TSI_CHCR_CURRENT_MAX;}while(0) /*!< ���ó�ŵ���� */

#define _CLEAR_REGISTER_CHEN(TSIx) do{TSIx->TSI_CHEN = 0;}while(0) /*!< trim the tsi verf */

    /**
  * @}
  */

    /*** �ṹ�塢ö�ٱ������� *****************************************************/
    /** @addtogroup TSI Exported Types
  * @{
  */

    /**
  * @}
  */

    /*** ȫ�ֱ������� **************************************************************/
    /** @addtogroup TSI Exported Variables
  * @{
  */

    /**
  * @}
  */

    /*** �������� ******************************************************************/
    /** @addtogroup TSI Exported Functions
  * @{
  */
    extern inline void _SET_CHANNLENABLE(TSI_TypeDef *TSIx, const uint16_t channles);
    extern inline void _SET_SCANTIME(TSI_TypeDef *TSIx, const uint8_t time);
    extern inline void _SET_DISABLETIME(TSI_TypeDef *TSIx, const uint8_t time);
    extern inline uint8_t _GET_IF_SCANFINISH(TSI_TypeDef *TSIx);
    extern inline uint8_t _GET_IE_SCANFINISH(TSI_TypeDef *TSIx);
    extern inline uint8_t _GET_IF_AWD(TSI_TypeDef *TSIx);
    extern inline uint8_t _GET_IE_AWD(TSI_TypeDef *TSIx);
    extern void drv_tsi_reset(TSI_TypeDef *TSIx);
    /**
  * @}
  */

    /**
  * @}
  */

    /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif //_TSI_DRV_H_

/************************ (C) COPYRIGHT C*Core *****END OF FILE****/
