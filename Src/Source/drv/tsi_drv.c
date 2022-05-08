/**
 * @file tsi_drv.c
 * @author System Application Team
 * @brief TSI模块driver层驱动
 * @version 3.0
 * @date 2020-07-27
 * 
 * @copyright C*Core Copyright (c) 2020
 * 
 */
#include "tsi_drv.h"

/**
 * @brief 设置扫描周期.
 * 
 * @param[in] TSIx 
 * @param[in] time
 */
inline void _SET_SCANTIME(TSI_TypeDef *TSIx, const uint8_t time)
{
    TSIx->TSI_CHEN &= ~(0xffu << 24);
    TSIx->TSI_CHEN |= (time << 24);
}

/**
 * @brief 设置扫描打盹时间.
 * 
 * @param[in] TSIx 
 * @param[in] time 
 */
inline void _SET_DISABLETIME(TSI_TypeDef *TSIx, const uint8_t time)
{
    TSIx->TSI_CHEN &= ~(0xffu << 16);
    TSIx->TSI_CHEN |= (time << 16);
}

/**
 * @brief 设置TSI通道使能.
 * 
 * @param[in] TSIx 
 * @param[in] channles 
 */
inline void _SET_CHANNLENABLE(TSI_TypeDef *TSIx, const uint16_t channles)
{
    TSIx->TSI_CHEN &= (~0x0000FFFFu);
    TSIx->TSI_CHEN |= channles;
}

/**
 * @brief 获取TSI所有使能通道扫描完成标志.
 * 写1清零
 * @param[in] TSIx @ref TSI_TypeDef
 * @return uint8_t 扫描完成返回0x01,否则返回0.
 */
inline uint8_t _GET_IF_SCANFINISH(TSI_TypeDef *TSIx)
{
    if (TSIx->TSI_ISR & TSI_ISR_SCANENDFINISH)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief 获得SCANFINISHI中断使能标志.
 * 
 * @param[in] TSIx 
 * @return uint8_t 使能返回0x01,否则返回0.
 */
inline uint8_t _GET_IE_SCANFINISH(TSI_TypeDef *TSIx)
{
    if (TSIx->TSI_IER & TSI_IER_SCANFINISH_IE)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief 获取TSI低功耗触摸唤醒中断标志位.
 * 
 * @param[in] TSIx 
 * @return uint8_t TSI低功耗触摸唤醒返回0x01, 否则返回0.
 */
inline uint8_t _GET_IF_AWD(TSI_TypeDef *TSIx)
{
    if (TSIx->TSI_ISR & TSI_ISR_AWD)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief 获取TSI低功耗唤醒中断使能标志位.
 * 
 * @param[in] TSIx 
 * @return uint8_t 使能返回0x01,否则返回0.
 */
inline uint8_t _GET_IE_AWD(TSI_TypeDef *TSIx)
{
    if (TSIx->TSI_IER & TSI_IER_ANALOGWDT_IE)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief 恢复TSI寄存器到默认值.
 * 
 * @param TSIx 
 */
void drv_tsi_reset(TSI_TypeDef *TSIx)
{
    const uint32_t default1[7] = {0x00000340, 0x00000000, 0x50000008, 0x10108000,
                                  0x00080100, 0x00000000, 0x00080000};
    const uint32_t default2[17] = {0x00000000, 0x00000AF5, 0x10101010, 0x10101010,
                                   0x10101010, 0x10101010, 0x02C80000, 0x00000000,
                                   0x00000000, 0x40404040, 0x40404040, 0x40404040,
                                   0x40404040, 0x40404040, 0x40404040, 0x40404040,
                                   0x40404040};

    memcpy((uint32_t *)&(TSIx->TSI_ISR), default1, 7);
    memcpy((uint32_t *)&(TSIx->TSI_RESERVED), default2, 17);
}

/************************ (C) COPYRIGHT C*Core *****END OF FILE**********************/
