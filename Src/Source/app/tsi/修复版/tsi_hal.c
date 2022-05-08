/**
	**********************************************************************************
             Copyright(c) 2019 China Core Co. Ltd.
                      All Rights Reserved
  **********************************************************************************
  * @file    tsi_hal.c
  * @author  System Application Team
  * @version V3.4.5
  * @date    2020.11.01
  * @brief   Tsi模块HAL层驱动.
  * @note 
  */

#include "tsi_hal.h"
#include "cpm_drv.h"
/**
 * @brief cpu方式读取fine数据.
 * 
 * @param[in] htsi 
 * @param[in] scan_cnt 
 * @return uint32_t return 0
 */
uint32_t hal_got_fine(TSI_HandleTypeDef *htsi, const uint8_t scan_cnt)
{
    volatile uint8_t *pUINT8_BaseAdd = (uint8_t *)&(htsi->Data[0]);
    volatile uint32_t *pUINT32;
    volatile uint8_t *pUINT8;

    for (uint8_t i = 0; i < scan_cnt; i++)
    {
        pUINT8 = (pUINT8_BaseAdd + i * 4);
        pUINT32 = &(TSI->TSI_CH0DR);
        hal_tsi_clearScanFinishFlag(htsi);
        hal_tsi_waitScanFinished(htsi);
        for (uint8_t ch = 0; ch < 16; ch++)
        {
            memcpy((uint8_t *)pUINT8, (uint8_t *)pUINT32, 4);
            pUINT8 = pUINT8 + scan_cnt * 4;
            pUINT32++;
        }
    }
    return 0x00;
}

/**
 * @brief 判断传入TSI通道是否使能.
 * 
 * @param[in] htsi
 * @param[in] channle 
 * @return uint8_t channle使能返回1, 否则返回0.
 */
inline uint8_t hal_tsi_getChannleValid(TSI_HandleTypeDef *htsi, const uint8_t channle)
{
    if (htsi->ChannlesValid & (1u << channle))
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief 复位TSI.
 * 
 * @param[in] htsi  
 */
void hal_tsi_reset(TSI_HandleTypeDef *htsi)
{
    // must reset TSI, because anolog
#if 1
    CPM->CPM_IPRSTCR |= 0x00800000; // IPS reset
    CPM->CPM_IPRSTCR &= ~0x00800000;
#else
    drv_tsi_reset(((TSI_TypeDef *)htsi->Instance));
#endif
}

/**
 * @brief 关闭SCANFINISHI中断功能.
 * 
 * @param[in] htsi
 */
void hal_tsi_closeIEScan(TSI_HandleTypeDef *htsi)
{
    _CLEAR_IE_SCANFINISH(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 关闭TSI低功耗唤醒功能.
 * 
 * @param[in] htsi  
 */
void hal_tsi_closeIEAWD(TSI_HandleTypeDef *htsi)
{
    _CLEAR_IE_AWD(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 清除SCANFINISH标志位.
 * 
 * @param[in] htsi  
 */
void hal_tsi_clearIFScanFinish(TSI_HandleTypeDef *htsi)
{
    _CLEAR_SCANFINISHFLAG(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 清除TSI低功耗中断唤醒标志位.
 * 
 * @param[in] htsi  
 */
void hal_tsi_clearIFAWD(TSI_HandleTypeDef *htsi)
{
    _CLEAR_AWDFLAG(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 获得SCANFINISH标志位.
 * 
 * @return uint8_t @ref _GET_IF_SCANFINISH
 */
uint8_t hal_tsi_getIFScanFinish(void)
{
    return _GET_IF_SCANFINISH(TSI);
}

/**
 * @brief 获得TSI低功耗中断唤醒标志位.
 * 
 * @return uint8_t @ref _GET_IF_AWD
 */
uint8_t hal_tsi_getIFAWD(void)
{
    return _GET_IF_AWD(TSI);
}

/**
 * @brief 读取SCANFINISH中断使能标志位.
 * 
 * @param[in] htsi 
 * @return uint8_t @ref _GET_IE_SCANFINISH
 */
uint8_t hal_tsi_getIEScanFinish(TSI_HandleTypeDef *htsi)
{
    return _GET_IE_SCANFINISH(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 读取TSI低功耗中断唤醒使能标志位.
 * 
 * @param[in] htsi 
 * @return uint8_t @ref _GET_IE_AWD
 */
uint8_t hal_tsi_getIEAWD(TSI_HandleTypeDef *htsi)
{
    return _GET_IE_AWD(((TSI_TypeDef *)htsi->Instance));
}

/**
 * @brief 阻塞式等待TSI扫描完成.
 * 
 * @param[in] htsi 
 * @return uint32_t  return 0
 */
uint32_t hal_tsi_waitScanFinished(TSI_HandleTypeDef *htsi)
{
    volatile uint8_t tmp;
    do
    {
        tmp = _GET_IF_SCANFINISH(htsi->Instance);
    } while (!tmp);
    return 0x00;
}

/**
 * @brief 阻塞式清除TSI扫描完成标志位.
 * 
 * @param[in] htsi 
 * @return uint32_t return 0
 */
uint32_t hal_tsi_clearScanFinishFlag(TSI_HandleTypeDef *htsi)
{
    volatile uint8_t tmp;
    do
    {
        _CLEAR_SCANFINISHFLAG(htsi->Instance);
        tmp = _GET_IF_SCANFINISH(htsi->Instance);
    } while (tmp);
    return 0x00;
}

/**
 * @brief TSI低功耗处理流程结构体初始化.
 * 1. 初始化TSI指针
 * 2. initial Backup Data
 * @param[in] htsi
 */
void hal_tsi_configBackupStruct(TSI_HandleTypeDef *htsi)
{
    htsi->Instance = TSI;

    htsi->Backup.AlgBase.ch0 = (uint8_t *)TSI_BACKUP_ALGBASE_CH0;
    htsi->Backup.AlgBase.ch1 = (uint8_t *)TSI_BACKUP_ALGBASE_CH1;
    htsi->Backup.AlgBase.ch2 = (uint8_t *)TSI_BACKUP_ALGBASE_CH2;
    htsi->Backup.AlgBase.ch3 = (uint8_t *)TSI_BACKUP_ALGBASE_CH3;
    htsi->Backup.AlgBase.ch4 = (uint8_t *)TSI_BACKUP_ALGBASE_CH4;
    htsi->Backup.AlgBase.ch5 = (uint8_t *)TSI_BACKUP_ALGBASE_CH5;
    htsi->Backup.AlgBase.ch6 = (uint8_t *)TSI_BACKUP_ALGBASE_CH6;
    htsi->Backup.AlgBase.ch7 = (uint8_t *)TSI_BACKUP_ALGBASE_CH7;
    htsi->Backup.AlgBase.ch8 = (uint8_t *)TSI_BACKUP_ALGBASE_CH8;
    htsi->Backup.AlgBase.ch9 = (uint8_t *)TSI_BACKUP_ALGBASE_CH9;
    htsi->Backup.AlgBase.ch10 = (uint8_t *)TSI_BACKUP_ALGBASE_CH10;
    htsi->Backup.AlgBase.ch11 = (uint8_t *)TSI_BACKUP_ALGBASE_CH11;
    htsi->Backup.AlgBase.ch12 = (uint8_t *)TSI_BACKUP_ALGBASE_CH12;
    htsi->Backup.AlgBase.ch13 = (uint8_t *)TSI_BACKUP_ALGBASE_CH13;
    htsi->Backup.AlgBase.ch14 = (uint8_t *)TSI_BACKUP_ALGBASE_CH14;
    htsi->Backup.AlgBase.ch15 = (uint8_t *)TSI_BACKUP_ALGBASE_CH15;

    htsi->Backup.Trend.ch0 = (uint8_t *)TSI_BACKUP_TREND_CH0;
    htsi->Backup.Trend.ch1 = (uint8_t *)TSI_BACKUP_TREND_CH1;
    htsi->Backup.Trend.ch2 = (uint8_t *)TSI_BACKUP_TREND_CH2;
    htsi->Backup.Trend.ch3 = (uint8_t *)TSI_BACKUP_TREND_CH3;
    htsi->Backup.Trend.ch4 = (uint8_t *)TSI_BACKUP_TREND_CH4;
    htsi->Backup.Trend.ch5 = (uint8_t *)TSI_BACKUP_TREND_CH5;
    htsi->Backup.Trend.ch6 = (uint8_t *)TSI_BACKUP_TREND_CH6;
    htsi->Backup.Trend.ch7 = (uint8_t *)TSI_BACKUP_TREND_CH7;
    htsi->Backup.Trend.ch8 = (uint8_t *)TSI_BACKUP_TREND_CH8;
    htsi->Backup.Trend.ch9 = (uint8_t *)TSI_BACKUP_TREND_CH9;
    htsi->Backup.Trend.ch10 = (uint8_t *)TSI_BACKUP_TREND_CH10;
    htsi->Backup.Trend.ch11 = (uint8_t *)TSI_BACKUP_TREND_CH11;
    htsi->Backup.Trend.ch12 = (uint8_t *)TSI_BACKUP_TREND_CH12;
    htsi->Backup.Trend.ch13 = (uint8_t *)TSI_BACKUP_TREND_CH13;
    htsi->Backup.Trend.ch14 = (uint8_t *)TSI_BACKUP_TREND_CH14;
    htsi->Backup.Trend.ch15 = (uint8_t *)TSI_BACKUP_TREND_CH15;
}

/**
 * @brief Run模式下TSI结构体初始化.
 * 
 * @param[in] htsi 
 */
void hal_tsi_configInitStruct(TSI_HandleTypeDef *htsi)
{
    htsi->Instance = TSI;

    htsi->ChannlesValid = 0;
    htsi->ChannlesValid = ((TSI_CH0_ENABLE << 0) | (TSI_CH1_ENABLE << 1) |
                           (TSI_CH2_ENABLE << 2) | (TSI_CH3_ENABLE << 3) |
                           (TSI_CH4_ENABLE << 4) | (TSI_CH5_ENABLE << 5) |
                           (TSI_CH6_ENABLE << 6) | (TSI_CH7_ENABLE << 7) |
                           (TSI_CH8_ENABLE << 8) | (TSI_CH9_ENABLE << 9) |
                           (TSI_CH10_ENABLE << 10) | (TSI_CH11_ENABLE << 11) |
                           (TSI_CH12_ENABLE << 12) | (TSI_CH13_ENABLE << 13) |
                           (TSI_CH14_ENABLE << 14) | (TSI_CH15_ENABLE << 15));

    htsi->Algorithm.Delta.ch0 = TSI_CH0_THIN_VAL;
    htsi->Algorithm.Delta.ch1 = TSI_CH1_THIN_VAL;
    htsi->Algorithm.Delta.ch2 = TSI_CH2_THIN_VAL;
    htsi->Algorithm.Delta.ch3 = TSI_CH3_THIN_VAL;
    htsi->Algorithm.Delta.ch4 = TSI_CH4_THIN_VAL;
    htsi->Algorithm.Delta.ch5 = TSI_CH5_THIN_VAL;
    htsi->Algorithm.Delta.ch6 = TSI_CH6_THIN_VAL;
    htsi->Algorithm.Delta.ch7 = TSI_CH7_THIN_VAL;
    htsi->Algorithm.Delta.ch8 = TSI_CH8_THIN_VAL;
    htsi->Algorithm.Delta.ch9 = TSI_CH9_THIN_VAL;
    htsi->Algorithm.Delta.ch10 = TSI_CH10_THIN_VAL;
    htsi->Algorithm.Delta.ch11 = TSI_CH11_THIN_VAL;
    htsi->Algorithm.Delta.ch12 = TSI_CH12_THIN_VAL;
    htsi->Algorithm.Delta.ch13 = TSI_CH13_THIN_VAL;
    htsi->Algorithm.Delta.ch14 = TSI_CH14_THIN_VAL;
    htsi->Algorithm.Delta.ch15 = TSI_CH15_THIN_VAL;

    memset(&(htsi->Baseline.Continue.ch0), 0, 16);

    htsi->Baseline.point = 0;
    volatile float *pd_baseline = (float *)&(htsi->Baseline.Data[0].ch0);
    for (uint16_t i = 0; i < 16 * BASELINE_CAHCE_LEN; i++)
    {
        pd_baseline[i] = BASELINE_DEFAULT_VAL;
    }

    hal_tsi_configBackupStruct(htsi);

    htsi->Status.Current = TSI_NONE;
    htsi->Status.Previous = TSI_NONE;
    htsi->Status.function = TSI_FUCTION_NONE;

    htsi->Interrupt.State = TSI_INTTERRUPT_NONE;
}

/**
 * @brief 初始化TSI寄存器, 根据TSI结构体.
 * 
 * @param[in] htsi 
 */
void hal_tsi_configRegister(TSI_HandleTypeDef *htsi)
{
    _SET_TSI_DISABLE(htsi->Instance);
    hal_tsi_closeIEAWD(htsi); // 中断唤醒之后，先关闭 TSI IE中断
    hal_tsi_closeIEScan(htsi);

    _SET_TSTR_ANALOG(htsi->Instance); //trim the tsi verf

    _CLEAR_REGISTER_CHEN(htsi->Instance);
    _SET_CHANNLENABLE(htsi->Instance, htsi->ChannlesValid);
    _SET_DISABLETIME(htsi->Instance, 0x00);
    _SET_SCANTIME(htsi->Instance, 0xff);

    _SET_CURRENT(htsi->Instance);
    _ENABLE_COARSEMODE(htsi->Instance);

    _SET_CR_DEFAULAT(htsi->Instance);
    _SET_WORKMODE_SCAN(htsi->Instance);
    _SET_SCANALWAYS(htsi->Instance);
}

/**
 * @brief 通过判断是否是第一次上电, 来设置Cbase&Coarse的初始值.
 * 1. 判断是否第一次配置TSI.
 * 2. 如果第一次配置TSI, 则初始化Cbase和Coarse为default值.
 * 3. 如果不是第一次上电配置TSI, 则reload Cbase 和 Coarse.
 * @param[in] htsi 
 * @param[out] initial 
 */
void hal_tsi_rememberConfiguration(TSI_HandleTypeDef *htsi, TSI_InitialTypeDef *initial)
{
    const volatile uint32_t *pu32Cmp = &(htsi->Instance->TSI_DELTA03);
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (pu32Cmp[i] != 0x40404040) // not default value
        {
            break;
        }
    }
    if (i == 8) // 第一次上电
    {
        memset(&(initial->Cbase.ch0), TSI_INIT_CBASEIS, 16);
        memset(&(initial->Coarse.ch0), TSI_INIT_COARSEIS, 16);
        initial->Status = TSI_INIT_NONE;
    }
    else
    {
        memcpy(&(initial->Cbase.ch0), (uint8_t *)&(htsi->Instance->TSI_BASE03), 16);
        memcpy(&(initial->Coarse.ch0), (uint8_t *)&(htsi->Instance->TSI_Cid_coarse03), 16);
    }
}

/**
 * @brief 装载 Cbase & Coarse.
 * 1. 判断是否第一次上电.
 * 2. 根据Direction, 判断Cbase和Coarse调整方向.
 * @param[in] htsi 
 * @param[in] initial 
 */
void hal_tsi_loadCbaseCoarse(TSI_HandleTypeDef *htsi, TSI_InitialTypeDef *initial)
{
    volatile uint8_t *pu8Cbase = (uint8_t *)&(htsi->Instance->TSI_BASE03);
    volatile uint8_t *pu8Coarse = (uint8_t *)&(htsi->Instance->TSI_Cid_coarse03);
    volatile uint8_t *pu8Direction = (uint8_t *)&(initial->Direction.ch0);

    _ENABLE_COARSEMODE(htsi->Instance);

    memcpy((uint8_t *)&(htsi->Instance->TSI_BASE03), (uint8_t *)&(initial->Cbase.ch0), 16);
    memcpy((uint8_t *)&(htsi->Instance->TSI_Cid_coarse03), (uint8_t *)&(initial->Coarse.ch0), 16);

    if (initial->Status == TSI_INIT_ADJUST)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            if (hal_tsi_getChannleValid(htsi, i))
            {
                if (pu8Direction[i] == TSI_CBASE_INCREASE)
                {
                    if ((pu8Cbase[i] & 0x3F) == 0x3F)
                    {
                        if (pu8Coarse[i] != 0x00)
                        {
                            pu8Coarse[i]--;
                        }
                        else
                        {
                            pu8Direction[i] = TSI_CBASE_OUTRANGE;
                        }
                    }
                    else
                    {
                        pu8Cbase[i]++;
                    }
                }
                else if (pu8Direction[i] == TSI_CBASE_DECREASE)
                {
                    if ((pu8Cbase[i] & 0x3F) == 0x00)
                    {
                        if (pu8Coarse[i] != 0xFF)
                        {
                            pu8Coarse[i]++;
                        }
                        else
                        {
                            pu8Direction[i] = TSI_CBASE_OUTRANGE;
                        }
                    }
                    else
                    {
                        pu8Cbase[i]--;
                    }
                }
                else
                {
                    ;
                }
            }
        }
    }

    _SET_TSI_EN(htsi->Instance);
    initial->Status = TSI_INIT_ADJUST;
}

/**
 * @brief 装载低功耗唤醒阈值寄存器.
 * 
 * @param[in] htsi 
 */
void hal_tsi_loadDelta(TSI_HandleTypeDef *htsi)
{
    volatile uint8_t *pu8_Delta = ((uint8_t *)&(htsi->Instance->TSI_DELTA03));
    volatile uint8_t *pu8_AlgDelta = &(htsi->Algorithm.Delta.ch0);

    for (uint8_t i = 0; i < 16; i++)
    {
        pu8_Delta[i] = (uint8_t)(pu8_AlgDelta[i] * 3 / 4 + TSI_LP_DELTA_REG_INCREASE);
    }
}

/**
 * @brief 检查fine是否落在合理区间内, 否则reload Cbase和Coarse.
 * 
 * @param[in] htsi 
 * @param[out] initial 
 */
void hal_tsi_checkFine(TSI_HandleTypeDef *htsi, TSI_InitialTypeDef *initial)
{
    volatile uint32_t *pu32Data = &(htsi->Instance->TSI_CH0DR);
    volatile uint8_t *pu8Directioin = (uint8_t *)&(initial->Direction.ch0);
    uint32_t temp;
    initial->Status = TSI_INIT_FINISHED;

    for (uint8_t i = 0; i < 16; i++)
    {
        if (hal_tsi_getChannleValid(htsi, i))
        {
            if (pu8Directioin[i] != TSI_CBASE_OUTRANGE) // 超出量程
            // if (pu8Directioin[i] != TSI_CBASE_OUTRANGE) ||
            // (pu8Directioin[i] != TSI_CBASE_NONE))
            {
                temp = ((pu32Data[i] & 0xff) +
                        ((pu32Data[i] >> 8) & 0xff) +
                        ((pu32Data[i] >> 16) & 0xff) +
                        ((pu32Data[i] >> 24) & 0xff)) >>
                       2;
                if (temp < TSI_INIT_FINE1_LOW)
                {
                    pu8Directioin[i] = TSI_CBASE_INCREASE;
                    initial->Status = TSI_INIT_ADJUST;
                }
                else if (temp > TSI_INIT_FINE2_UPPER)
                {
                    pu8Directioin[i] = TSI_CBASE_DECREASE;
                    initial->Status = TSI_INIT_ADJUST;
                }
                else if ((temp > TSI_INIT_FINE1_UPPER) && (temp < TSI_INIT_FINE2_LOW))
                {
                    pu8Directioin[i] = TSI_CBASE_INCREASE;
                    initial->Status = TSI_INIT_ADJUST;
                }
                else
                {
                    pu8Directioin[i] = TSI_CBASE_NONE;
                }
            }
        }
    }
}

/**
 * @brief 从Coarse模式切换到Fine模式.
 * 
 * @param htsi 
 */
void hal_tsi_switchFineMode(TSI_HandleTypeDef *htsi)
{
    _ENABLE_FINEMODE(htsi->Instance);
    hal_tsi_clearScanFinishFlag(htsi);
    hal_tsi_waitScanFinished(htsi);
}

/**
 * @brief 将TSI配置从run模式, 切换到低功耗唤醒模式.
 * 1. disable IE
 * 2. clear IF
 * 3. update state: htsi->Interrupt
 * 4. save data to SRAM. 为了保持接口一致性,放在了app层实现
 * 5. update register to Lowpower
 * 6. enable IE: AWD
 * @param[in] htsi
 */
void hal_tsi_switchLowpower(TSI_HandleTypeDef *htsi)
{
    hal_tsi_closeIEScan(htsi);
    hal_tsi_closeIEAWD(htsi);
    hal_tsi_clearIFScanFinish(htsi);
    hal_tsi_clearIFAWD(htsi);

    htsi->Interrupt.State = TSI_INTTERRUPT_NONE;
    htsi->Interrupt.pAddress = (uint8_t *)(&(htsi->Data[0]));
    htsi->Interrupt.Count = 0;
    htsi->Status.Current = TSI_NONE;
    htsi->Status.Previous = TSI_NONE;
    htsi->Status.function = TSI_FUCTION_LOWPOWER;

    hal_tsi_loadDelta(htsi); // for wakeup at lowpower

    _SET_DISCARDMODE(htsi->Instance); //run mode 3-bit is 0
    _SET_DISCARD_LEN(htsi->Instance);
    _SET_WDCFGR_LP(htsi->Instance);      //bit16-27:lowpower按键判断阈值 bit17:0x002002e0=按下触发唤醒  0x002001e0=抬起触发唤醒
    _SET_SCANTIME(htsi->Instance, 0x08); // RUN mode is 0xff00xxxx
    _SET_DISABLETIME(htsi->Instance, 0xff);
    _SET_SCAN_SEQUENCE_MODE_00001111(htsi->Instance);
    htsi->Instance->TSI_CR &= ~0x08;
    htsi->Instance->TSI_CHCR &= ~(1u << 19);

    _SET_IE_AWD(htsi->Instance);
}

/**
 * @brief 切换到中断处理模式.
 * 1. disable IE
 * 2. clear IF
 * 3. update state: htsi->Interrupt
 * 4. update register to interrupt
 * 5. enable IE: ScanFinishs
 * @param[in] htsi 
 */
void hal_tsi_switchInterrupt(TSI_HandleTypeDef *htsi)
{
    hal_tsi_closeIEScan(htsi);
    hal_tsi_closeIEAWD(htsi);
    hal_tsi_clearIFScanFinish(htsi);
    hal_tsi_clearIFAWD(htsi);

    htsi->Interrupt.State = TSI_INTTERRUPT_TRANSFER;
    htsi->Interrupt.pAddress = (uint8_t *)(&(htsi->Data[0]));
    htsi->Interrupt.Count = 0;
    htsi->Status.Current = TSI_NONE;
    htsi->Status.Previous = TSI_NONE;

    _SET_DISCARDMODE_NONE(htsi->Instance);
    _SET_DISCARD_LEN_0(htsi->Instance);
    _SET_DEFAULT_WDCFGR(htsi->Instance);
    _SET_SCANTIME(htsi->Instance, 0xff);
    _SET_DISABLETIME(htsi->Instance, 0x00);
    _SET_SCAN_SEQUENCE_MODE_01234567(htsi->Instance);
    htsi->Instance->TSI_CR |= 0x08;
    htsi->Instance->TSI_CHCR |= (1u << 19);

    htsi->Status.function = TSI_FUCTION_INTERRUPT;

    _SET_IE_SCANFINISH(htsi->Instance);
}

/**
 * @brief 动态校准Cbase & Coarse寄存器.
 * 
 * @param htsi 
 */
void hal_tsi_adjustRegister(TSI_HandleTypeDef *htsi)
{
    e_TSI_InitStatus Status = TSI_INIT_NONE;
    TSI_Init_AdjustDirectionTypeDef coarseDirection;
    volatile uint8_t *pu8Direction = (uint8_t *)&(coarseDirection.ch0);
    memset((uint8_t *)pu8Direction, TSI_CBASE_NONE, 16);

    volatile float *pf_baseline0 = (volatile float *)&(htsi->Baseline.Data[0].ch0);
    volatile float *pf_baseline1 = (volatile float *)&(htsi->Baseline.Data[1].ch0);
    volatile float *pf_baseline2 = (volatile float *)&(htsi->Baseline.Data[2].ch0);

    // volatile float *pf_variace = (volatile float *)&(htsi->Algorithm.VarianceLast.ch0); // TODO
    volatile float *pf_average = (volatile float *)&(htsi->Algorithm.AverageLast.ch0);
    volatile uint8_t *pu8Cbase = (uint8_t *)&(htsi->Instance->TSI_BASE03);
    volatile uint8_t *pu8Coarse = (uint8_t *)&(htsi->Instance->TSI_Cid_coarse03);

    for (uint8_t i = 0; i < 16; i++)
    {
        if (hal_tsi_getChannleValid(htsi, i)) // channle is valid
        {
            if (pf_average[i] < TSI_INIT_FINE1_LOW)
            {
                if (pu8Cbase[i] != 0x3F)
                {
                    pu8Cbase[i]++;
                    pf_baseline0[i] = BASELINE_DEFAULT_VAL;
                    pf_baseline1[i] = BASELINE_DEFAULT_VAL;
                    pf_baseline2[i] = BASELINE_DEFAULT_VAL;
                }
                else
                {
                    if (pu8Coarse[i] >= TSI_COARSE_STEP_DYNAMICA_DJUST) // adjust coarse
                    {
                        pu8Direction[i] = TSI_CBASE_DECREASE;
                        Status = TSI_INIT_ADJUST;
                    }
                }
            }
            else if (pf_average[i] > TSI_INIT_FINE2_UPPER)
            {
                if (pu8Cbase[i] != 0x00)
                {
                    pu8Cbase[i]--;
                }
                else
                {
                    if (pu8Coarse[i] < (0xFF - TSI_COARSE_STEP_DYNAMICA_DJUST)) // adjust coarse
                    {
                        pu8Direction[i] = TSI_CBASE_INCREASE;
                        Status = TSI_INIT_ADJUST;
                    }
                }
            }
        }
    }

    if (Status == TSI_INIT_ADJUST)
    {
        volatile TSI_TypeDef backupTSIRegs = {0x00};
        volatile TSI_TypeDef *p_backupTSIRegs = (TSI_TypeDef *)&(backupTSIRegs.TSI_ISR);
        memcpy((uint8_t *)&backupTSIRegs, htsi->Instance, 0xa0);
        volatile uint8_t *pu8_adjustCoarse = (uint8_t *)&(backupTSIRegs.TSI_Cid_coarse03);

        hal_tsi_reset(htsi); // reset tsi

        _ENABLE_COARSEMODE(p_backupTSIRegs);
        for (uint8_t i = 0; i < 16; i++)
        {
            if (pu8Direction[i] == TSI_CBASE_INCREASE)
            {
                pu8_adjustCoarse[i] += TSI_COARSE_STEP_DYNAMICA_DJUST;
            }
            else if (pu8Direction[i] == TSI_CBASE_DECREASE)
            {
                pf_baseline0[i] = BASELINE_DEFAULT_VAL;
                pf_baseline1[i] = BASELINE_DEFAULT_VAL;
                pf_baseline2[i] = BASELINE_DEFAULT_VAL;
                pu8_adjustCoarse[i] -= TSI_COARSE_STEP_DYNAMICA_DJUST;
            }
        }
        memcpy(htsi->Instance, (uint8_t *)p_backupTSIRegs, 0xa0);

        hal_tsi_switchFineMode(htsi);
    }
}

/**
 * @brief 将fine数据转存到指定buffer
 * 1. 更新了 htsi->Interrupt.Count
 * 2. 更新了 htsi->Interrupt.pAddress
 * 3. 更新了 htsi->Interrupt.State
 * @param[in] htsi 
 */
void hal_tsi_transferFineData(TSI_HandleTypeDef *htsi)
{
    volatile uint8_t *pu8_address = htsi->Interrupt.pAddress;
    volatile uint32_t *pu32_RegisterData = &(htsi->Instance->TSI_CH0DR);

    for (uint8_t ch = 0; ch < 16; ch++)
    {
        if (hal_tsi_getChannleValid(htsi, ch))
        {
            memcpy((uint8_t *)pu8_address, (uint8_t *)(&pu32_RegisterData[ch]), 4);
        }
        pu8_address = pu8_address + LOOP_SCAN_NUM * 4;
    }

    if (htsi->Interrupt.Count < (LOOP_SCAN_NUM - 1))
    {
        htsi->Interrupt.Count++;
        htsi->Interrupt.pAddress += 4;
    }
    else
    {
        htsi->Interrupt.State = TSI_INTTERRUPT_ALGORITHM;
        htsi->Interrupt.Count = 0;
        htsi->Interrupt.pAddress = (uint8_t *)(&(htsi->Data[0]));
    }
}

/************************ (C) COPYRIGHT C*Core *****END OF FILE**********************/
