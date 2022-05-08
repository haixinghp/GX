/**
 * @file tsi_drv.h
 * @author System Application Team
 * @brief TSI模块driver层驱动头文件
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

    /*** 宏定义 *******************************************************************/
    /** @addtogroup TSI Exported Macros
  * @{
  */
#define _CLEAR_SCANFINISHFLAG(TSIx) do{TSIx->TSI_ISR |= TSI_ISR_SCANENDFINISH;}while(0) /*!< 清除扫描完成标志位 */
#define _CLEAR_AWDFLAG(TSIx) do{TSIx->TSI_ISR |= TSI_ISR_AWD;}while(0) /*!< 清除TSI中断唤醒标志位 */

#define _SET_IE_SCANFINISH(TSIx) do{TSIx->TSI_IER |= TSI_IER_SCANFINISH_IE;}while(0) /*!< 使能TSI通道扫描完成中断功能 */
#define _CLEAR_IE_SCANFINISH(TSIx) do{TSIx->TSI_IER &= ~TSI_IER_SCANFINISH_IE;}while(0) /*!< 不使能TSI通道扫描完成中断功能 */
#define _SET_IE_AWD(TSIx) do{TSIx->TSI_IER |= TSI_IER_ANALOGWDT_IE;}while(0) /*!< 使能TSI低功耗触摸唤醒功能 */
#define _CLEAR_IE_AWD(TSIx) do{TSIx->TSI_IER &= ~TSI_IER_ANALOGWDT_IE;}while(0) /*!< 不使能TSI低功耗触摸唤醒功能 */

#define _SET_DEFAULT_CR(TSIx) do{TSIx->TSI_CR = 0x50000008;}while(0) /*!< 设置TSI_WDCFGR寄存器为默认值 */
#define _SET_TSI_EN(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_EN;}while(0) /*!< 使能TSI */
#define _SET_TSI_DISABLE(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_EN;}while(0) /*!< 关闭TSI */
#define _SET_CR_DEFAULAT(TSIx) do{TSIx->TSI_CR |= TSI_CR_DEFAULT;}while(0) /*!< 设置TSI_CR寄存器为默认值 */
#define _SET_WORKMODE_SCAN(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_MODE;}while(0) /*!< 设置TSI扫描模式为通道循环扫描模式 */
#define _SET_WORKMODE_SINGLE(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_MODE;}while(0) /*!< 设置TSI扫描模式为单通道扫描模式 */
#define _SET_SCANALWAYS(TSIx) do{TSIx->TSI_CR |= TSI_CR_AVALID_ALWAYS;}while(0) /*!< 使能TSI连续扫描搬运模式 */
#define _SET_SCANCYCLICITY(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_AVALID_ALWAYS;}while(0) /*!< 不使能TSI连续扫描搬运模式 */
#define _SET_SCAN_SEQUENCE_MODE_00001111(TSIx) do{TSIx->TSI_CR |= TSI_CR_SCAN_SEQUENCE_MODE;}while(0) /*!< 设置通道扫描模式为00001111... */
#define _SET_SCAN_SEQUENCE_MODE_01234567(TSIx) do{TSIx->TSI_CR &= ~TSI_CR_SCAN_SEQUENCE_MODE;}while(0) /*!< 设置通道扫描模式为012345678... */

#define _SET_TSTR_ANALOG(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_ANALOG;}while(0) /*!< trim the tsi verf */
#define _SET_DISCARDMODE(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_DISCARDMODE;}while(0) /*!< =1通道切换时丢弃数据 */
#define _SET_DISCARDMODE_NONE(TSIx) do{TSIx->TSI_TSTR &= ~TSI_TSTR_DISCARDMODE;}while(0) /*!< =1通道切换时丢弃数据 */
#define _SET_DISCARD_LEN(TSIx) do{TSIx->TSI_TSTR |= TSI_TSTR_DISCARDLEN;}while(0) /*!< 丢失数据的长度 */
#define _SET_DISCARD_LEN_0(TSIx) do{TSIx->TSI_TSTR &= ~TSI_TSTR_DISCARDLEN;}while(0) /*!< 丢失数据的长度 */

#define _SET_DEFAULT_WDCFGR(TSIx) do{TSIx->TSI_WDCFGR = 0x00080100;}while(0) /*!< 设置TSI_WDCFGR寄存器为默认值 */
#define _SET_WDCFGR_LP(TSIx) do{TSIx->TSI_WDCFGR = 0x000002e0;}while(0) /*!< 设置TSI_WDCFGR寄存器为低功耗配置 */

#define _ENABLE_FINEMODE(TSIx) do{TSIx->TSI_CHCR &= ~TSI_CHCR_MODE_COARSE;}while(0) /*!< 设置工作模式为Fine模式 */
#define _ENABLE_COARSEMODE(TSIx) do{TSIx->TSI_CHCR |= TSI_CHCR_MODE_COARSE;}while(0) /*!< 设置工作模式为Coarse模式 */
#define _SET_CURRENT(TSIx) do{TSIx->TSI_CHCR |= TSI_CHCR_CURRENT_MAX;}while(0) /*!< 设置充放电电流 */

#define _CLEAR_REGISTER_CHEN(TSIx) do{TSIx->TSI_CHEN = 0;}while(0) /*!< trim the tsi verf */

    /**
  * @}
  */

    /*** 结构体、枚举变量定义 *****************************************************/
    /** @addtogroup TSI Exported Types
  * @{
  */

    /**
  * @}
  */

    /*** 全局变量声明 **************************************************************/
    /** @addtogroup TSI Exported Variables
  * @{
  */

    /**
  * @}
  */

    /*** 函数声明 ******************************************************************/
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
