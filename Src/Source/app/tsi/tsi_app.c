/**
	**********************************************************************************
             Copyright(c) 2019 China Core Co. Ltd.
                      All Rights Reserved
  **********************************************************************************
  * @file    tsi_app.c
  * @author  System Application Team
  * @version V3.4.12
  * @date    2021.03.31
  * @brief   TSIӦ�ó���
  * 
  */

#include "tsi_app.h"
#include "Iomacros.h"

/*** ȫ�ֱ������� ***************************************************************/
/*** volatile **********/
/*** @ static **********/
/*** �������� *******************************************************************/
TSI_HandleTypeDef htsi;
/*** �������� *******************************************************************/

void printf_tsiReg(void)
{
    volatile uint32_t *pu32 = &(TSI->TSI_ISR);
    printf("TSI Reg:\r\n");
    for (uint16_t i = 0; i < (0xa0 / 4); i++)
    {
        printf("%08x ", pu32[i]);
    }
    printf("\r\n");
}

void printf_BackupAlgBase(void)
{
    volatile uint8_t **ppu8 = &(htsi.Backup.AlgBase.ch0);
    printf("AlgBase: ");
    for (uint8_t i = 0; i < 16; i++)
    {
        printf("%02x ", **ppu8);
        ppu8++;
    }
    printf("\r\n");
}

void printf_Trend(void)
{
    // volatile uint8_t **ppu8 = &(htsi.Backup.Trend.ch0);
    // printf("Trend:  ");
    // for (uint8_t i = 0; i < 16; i++)
    // {
    //     printf("%02x ", **ppu8);
    //     ppu8++;
    // }
    // printf("\r\n");
}

void printf_fine(void)
{
    volatile uint32_t *pu32 = &(TSI->TSI_CH0DR);
    printf("Fine: ");
    for (uint16_t i = 0; i < 16; i++)
    {
        printf("%08x ", pu32[i]);
    }
    printf("\r\n");
}

/**
 * @brief TSI����.
 * 
 * @return 
 */
__weak void app_tsi_wakeup_callback(void)
{
    // UART_Debug_Init(SCI1, g_ips_clk, 115200);
    // IO_Latch_Clr();
    // printf("TSI AWD\n");
}

/**
 * @brief TSI�жϴ���ص�����
 * 1. 
 * 2. 
 * @param htsi 
 */
void tsi_callback(TSI_HandleTypeDef *htsi)
{
    hal_tsi_transferFineData(htsi);
}

void TSI_IRQHandler()
{
    if (_GET_IE_AWD(TSI)) // TSI�͹��Ļ���
    {
        if (hal_tsi_getIFAWD())
        {
            // if (htsi.Fuction.Status == TSI_FUCTION_SLEEP)
            // {
            //     hal_tsi_closeIEAWD(&htsi); // �жϻ���֮���ȹر� TSI IE�ж�
            //     hal_tsi_closeIEScan(&htsi);
            // }
            do
            {
                _CLEAR_AWDFLAG(TSI);
            } while (hal_tsi_getIFAWD());
            app_tsi_wakeup_callback();
        }
    }
    else if (_GET_IE_SCANFINISH(TSI)) // ����ɨ������ж�
    {
        if (hal_tsi_getIFScanFinish())
        {
            hal_tsi_clearIFScanFinish(&htsi); // clear TSI�ж�

            if (htsi.Fuction.Status == TSI_FUCTION_RUN)
            {
                if (htsi.Cache.Data[htsi.Cache.Index_Cache].Ready == 0)
                {
                    tsi_callback(&htsi);
                }
            }
            else if (htsi.Fuction.Status == TSI_FUCTION_WAIT) // T=4.09ms
            {
                if (htsi.Fuction.lossPacketCnt > 0)
                {
                    htsi.Fuction.lossPacketCnt = htsi.Fuction.lossPacketCnt - 1;
                }
                else
                {
                    htsi.Fuction.Status = TSI_FUCTION_RUN;
                    htsi.Cache.Offset_Cache = 0;
                }
            }
            else if (htsi.Fuction.Status == TSI_FUCTION_STOP)
            {
                htsi.Cache.Offset_Cache = 0;
            }

            while (hal_tsi_getIFScanFinish()) // 56.8us
            {
                hal_tsi_clearIFScanFinish(&htsi);
            }
        }
    }
}

/**
 * @brief 
 * 1. save algorithm base to address
 * 2. save coarse data to address
 * @param htsi
 * @param uint8_t
 */
void app_tsi_saveData(TSI_HandleTypeDef *htsi)
{
    volatile float *pf_baseline = (volatile float *)&(htsi->Baseline.Data[htsi->Baseline.point].ch0);
    volatile uint8_t **ppu8_AlgBase = &(htsi->Backup.AlgBase.ch0);

    for (uint16_t i = 0; i < 16; i++)
    {
        // save baseline
        **ppu8_AlgBase = (uint8_t)(pf_baseline[i]);
        ppu8_AlgBase++;
    }
}

/**
 * @brief fine����Ԥ����
 * 1. ��ͨ�˲�
 * 2. ��ͨ�˲�
 * 3. ��̬�ֲ��˲�
 * 4. ���ھ�ֵ�˲�
 * 5. �����˲�֮�󷽲�
 * 6. �����˲�֮���ֵ
 * @param[in] htsi 
 */
void app_tsi_preprocess(TSI_HandleTypeDef *htsi)
{
    volatile uint8_t *pu8_ChannleData = (volatile uint8_t *)&(htsi->Cache.Data[htsi->Cache.Index_Read].Buffer[0]);
    volatile float *pd_variance = (volatile float *)&(htsi->Algorithm.VarianceLast.ch0);
    volatile float *pd_average = (volatile float *)&(htsi->Algorithm.AverageLast.ch0);

    for (uint8_t i = 0; i < 16; i++)
    {
        if (hal_tsi_getChannleValid(htsi, i))
        {
            /* 1.��ͨ�˲� */
            // hal_algorithm_filter_lowpass(pu8_ChannleData, 4 * LOOP_SCAN_NUM, *pu8_Base, FILTER_LOWPASS_DELTA);

            /* 2.��ͨ�˲� */
            // hal_algorithm_filter_highpass(pu8_ChannleData, 4 * LOOP_SCAN_NUM, *pu8_Base, FILTER_HIGHPASS_DELTA);

            /* 3.��̬�ֲ��˲� */
            hal_algorithm_filter_NormalDistribution(pu8_ChannleData, 4 * LOOP_SCAN_NUM, pu8_ChannleData, FILTER_NORMALDISTRI_PER);

            /* 4. ���ھ�ֵ�˲� */
            hal_algorithm_algMeanRoll((uint8_t *)pu8_ChannleData, 4 * LOOP_SCAN_NUM - FILTER_NORMALDISTRI_PER * 2, (uint8_t *)pu8_ChannleData, MEAN_WINDOWS_SIZE);

            /* 5.calculate variance */
            hal_algorithm_variance((uint8_t *)pu8_ChannleData, 4 * LOOP_SCAN_NUM - FILTER_NORMALDISTRI_PER * 2 - MEAN_WINDOWS_SIZE + 1, (float *)&pd_variance[i], 1);

            /* 6.calculate average */
            pd_average[i] = hal_algorithm_average((uint8_t *)pu8_ChannleData, 4 * LOOP_SCAN_NUM - FILTER_NORMALDISTRI_PER * 2 - MEAN_WINDOWS_SIZE + 1);
        }
        pu8_ChannleData = pu8_ChannleData + (4 * LOOP_SCAN_NUM);
    }
}

/**
 * @brief �����ж�.
 * 1. ����.
 * 2. �ж���ǿ�ź�.
 * @param[in] htsi
 */
void app_tsi_updateTouch(TSI_HandleTypeDef *htsi)
{
    volatile float *pd_baseline = (volatile float *)&(htsi->Baseline.Data[(htsi->Baseline.point + 1) % BASELINE_CAHCE_LEN].ch0);
    volatile float *pd_average = (volatile float *)&(htsi->Algorithm.AverageLast.ch0);
    volatile uint8_t *pu8_delta = (volatile uint8_t *)&(htsi->Algorithm.Delta.ch0);
    volatile uint8_t *pu8_Continue = &(htsi->Touch.Continue.ch0); // ��¼���������256���ڵİ������, ��������, ����

    float max = 0;
    float delta = 0;

    htsi->Touch.Current = TSI_NONE;
    htsi->Touch.Previous = htsi->Touch.Current;
    htsi->Touch.TouchCurrent = 0;

    for (uint8_t channel = 0; channel < 16; channel++)
    {
        if (hal_tsi_getChannleValid(htsi, channel)) // channel is valid
        {
            if (pd_average[channel] >= (pd_baseline[channel] + pu8_delta[channel]))
            {
                htsi->Touch.TouchCurrent |= (1u << channel);

                delta = pd_average[channel] - pd_baseline[channel];
                if (delta > max)
                {
                    max = delta;
                    htsi->Touch.Current = (e_TSI_Status)channel;
                }

                pu8_Continue[channel]++;
            }
            else
            {
                if (pu8_Continue[channel] > 0)
                {
                    pu8_Continue[channel]--;
                }
            }
        }
    }
}
uint32_t wakeuptimes;
uint32_t bufangtimes;
uint32_t bufangtimes1;
/**
 * @brief ����Դ���� & ����Ԥ����.
 * 1. ��cpm_drv.h�д򿪶�Ӧ��, ȥ���Ƹû���Դ�ж�ʹ�ܹر�.
 * 2. CPM_WAKEUPSOURCE_REG: ��8bit�����Ż���Դ(RST_SOURCE_STA);
 *                          ��24bit��Ż���������ʱ����״̬(CPM_Wakeup_Pin)
 * 3. �뱣֤�ú����� __main ֮ǰ������.
 */
void cpm_handleWakeup(void)
{
//     UART_Debug_Init(SCI2, 60000000, 115200);
//     printf("IRQ:%02x, 2:%d, 3:%d\r\n", EPORT->EPFR, cpm_getIRQ_Eport(EPORT, 2), cpm_getIRQ_Eport(EPORT, 3));
    //printf("go to wakeup start\r\n");
    const volatile uint32_t CPM_PADWKINTCR = CPM->CPM_PADWKINTCR;
    volatile uint32_t *pu32_wakeupSource = (uint32_t *)CPM_WAKEUPSOURCE_REG;
    *pu32_wakeupSource &= 0xFFFFFF00;
    *pu32_wakeupSource |= (WAKEUP_SOURCE_NONE);

#if CPM_HANDLEWAKEUP_WDT
    if (CHIP_RESET->RSR & (0x01 << 4))
    {
#if CPM_HANDLEWAKEUP_WDT_LOG
        PCI_InitWakeup(); // PCI reinit.
        TC_selectClockSource(1);
        PCI_Clear_Status();
        Nvram_Init();

        PCI_NVSRAM_WtiteByte(0x4003416C, 0x55);
        PCI_NVSRAM_WtiteByte(0x4003416D, 0xaa);
        PCI_NVSRAM_WtiteByte(0x4003416E, 0x55);
        PCI_NVSRAM_WtiteByte(0x4003416F, 0xaa);
        TC_Sleep_DelayMS(10);
#endif
        Set_POR_Reset();
        return; //wdt reset
    }
#endif

    if (CPM_PADWKINTCR == 0x00ff0000) // POR
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_POR;
    }
#if CPM_HANDLEWAKEUP_GINT0
    else if ((EPORT->EPFR & (1u << 0)) || cpm_getIRQ_Eport(EPORT, 0))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN0;
    }
#endif
#if CPM_HANDLEWAKEUP_GINT1
    else if ((EPORT->EPFR & (1u << 1)) )
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN1;
    }
#endif
#if CPM_HANDLEWAKEUP_GINT2
    else if ((EPORT->EPFR & (1u << 2)) || cpm_getIRQ_Eport(EPORT, 2))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN2;
    }
#endif
#if CPM_HANDLEWAKEUP_GINT3
    else if ((EPORT->EPFR & (1u << 3)) || cpm_getIRQ_Eport(EPORT, 3))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN3;
    }
#endif
#if CPM_HANDLEWAKEUP_GINT4
    else if ((EPORT->EPFR & (1u << 4)) || cpm_getIRQ_Eport(EPORT, 4))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN4;
    }
#endif
#if CPM_HANDLEWAKEUP_GINT5
    else if ((EPORT->EPFR & (1u << 5)) || cpm_getIRQ_Eport(EPORT, 5))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_GIN5;
    }
#endif
#if (CPM_HANDLEWAKEUP_C0GINT0 || \
     CPM_HANDLEWAKEUP_C0GINT1 || \
     CPM_HANDLEWAKEUP_C0GINT4 || \
     CPM_HANDLEWAKEUP_C0GINT5)
    else if (drv_SubC0_getWakeupSource(pu32_wakeupSource) == 0)
    {
        // goto main
        
    }
#endif
#if CPM_HANDLEWAKEUP_SS3
    else if (CPM_PADWKINTCR & (1u << 4))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_SS3;
    }
#endif
#if CPM_HANDLEWAKEUP_WAKEUP
    else if (CPM_PADWKINTCR & (1u << 1))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_WAKEUP;
    }
#endif
#if CPM_HANDLEWAKEUP_USBDET
    else if (CPM_PADWKINTCR & (1u << 0))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_USBDET;
    }
#endif
#if CPM_HANDLEWAKEUP_SDIO0
    else if (CPM_PADWKINTCR & (1u << 2))
    {
        *pu32_wakeupSource = WAKEUP_SOURCE_SDIO0;
    }
#endif
#if CPM_HANDLEWAKEUP_TSI
    else if ((TSI->TSI_ISR & (1u << 7)) && (TSI->TSI_IER & (1u << 29)))
    {
         
        TSI_HandleTypeDef htsi;

        DCACHE_Init((cache_com)0, (cache_com)1, (cache_com)0, (cache_com)0);
        ICACHE_Init((cache_com)0, (cache_com)1, (cache_com)0, (cache_com)0);

        hal_tsi_configBackupStruct(&htsi);
// printf("go to  tsi wakeup\r\n");
        /* clear PCI Asyc & reload PCI Asyc */
        TC_Sleep_DelayMS(2);
        PCI_InitWakeup(); // PCI reinit.
        PCI->PCI_ATIMPR = ((CPM_HANDLEWAKEUP_ASYCTIMER_CYCLE * 2000 << 16) | 0x0103);

        // open NVIC for sleep
        NVIC_Init(3, 3, EPORT0_0_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_1_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_2_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_3_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_4_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_5_IRQn, 2);
        // NVIC_Init(3, 3, TSI_IRQn, 2);
        // NVIC_Init(3, 3, CPM_IRQn, 2);

        // volatile uint8_t **ppu8_Trend = &(htsi.Backup.Trend.ch0);
        volatile uint8_t *pu8_data = (uint8_t *)&(TSI->TSI_CH0DR);
        volatile uint8_t **ppu8_AlgBase = &(htsi.Backup.AlgBase.ch0);
        uint8_t delta[16] = {TSI_CH0_THIN_VAL, TSI_CH1_THIN_VAL, TSI_CH2_THIN_VAL, TSI_CH3_THIN_VAL,
                             TSI_CH4_THIN_VAL, TSI_CH5_THIN_VAL, TSI_CH6_THIN_VAL, TSI_CH7_THIN_VAL,
                             TSI_CH8_THIN_VAL, TSI_CH9_THIN_VAL, TSI_CH10_THIN_VAL, TSI_CH11_THIN_VAL,
                             TSI_CH12_THIN_VAL, TSI_CH13_THIN_VAL, TSI_CH14_THIN_VAL, TSI_CH15_THIN_VAL};
        uint8_t touch_cnt;
        uint8_t touch_channle_cnt = 0;

        /* TSI check */
        _SET_SCANALWAYS(TSI); //ʹ��always onģʽ
        TC_Sleep_DelayMS(4);  // ������sleepһ��ʱ��, С�ڳ�ǿ����
        
        for (uint8_t channle = 0; channle < 16; channle++)
        {
            if (TSI->TSI_CHEN & (0x01u << channle))
            {
                touch_cnt = 0;
                for (uint8_t i = 0; i < 4; i++)
                {
                    if (*pu8_data > (**ppu8_AlgBase + delta[channle] + 0))
                    {
                        touch_cnt++;
                    }
                    pu8_data++;
                }
                if (touch_cnt > 2)
                {
                    touch_channle_cnt++;
                }
            }
            else
            {
                pu8_data = pu8_data + 4;
            }
            ppu8_AlgBase++;
        }
       // printf("go to  tsi wakeup12\r\n");
        _SET_SCANCYCLICITY(TSI); //ʹ������ɨ��ģʽ

        if (touch_channle_cnt > 1)
        {
          //  printf("go to  tsi wakeup1\r\n");
            // just for log
            // volatile uint32_t *pu32_data = &(TSI->TSI_CH0DR);
            // for (uint8_t i = 0; i < 16; i++)
            // {
            //     **ppu8_Trend = ((*pu32_data & 0xff) +
            //                     ((*pu32_data >> 8) & 0xff) +
            //                     ((*pu32_data >> 16) & 0xff) +
            //                     ((*pu32_data >> 24) & 0xff)) >>
            //                    2;
            //     pu32_data++;
            //     ppu8_Trend++;
            // }
            *pu32_wakeupSource = WAKEUP_SOURCE_TSI; // goto main
        }
        else
        {
            NVIC_Init(3, 3, TSI_IRQn, 2);
          //   printf("go to  tsi wakeup2\r\n");
            CPM->CPM_PADWKINTCR |= (1u << 3);
            NVIC->ICPR[0] = (1 << 14);              // clear cpm wake-up interrupt pending.
            while (CPM->CPM_PADWKINTCR & (1u << 3)) // PCI Asyc IRQ
            {
                TC_Sleep_DelayMS(2);

                PCI->PCI_ATIMPR |= (1 << 8); // clear async timet int
                CPM->CPM_PADWKINTCR |= 0xff;
                NVIC->ICPR[0] = (1 << 14); // clear cpm wake-up interrupt pending.
            }
            while (1)
            {
                CPM_PowerOff_1p5();
            }
        }
    }
#endif
#if CPM_HANDLEWAKEUP_ASYCTIMER
    else
    {
        TSI_HandleTypeDef htsi;

        /* clear PCI Asyc & reload PCI Asyc */
        TC_Sleep_DelayMS(2);
        PCI_InitWakeup(); // PCI reinit.
        PCI->PCI_ATIMPR = ((CPM_HANDLEWAKEUP_ASYCTIMER_CYCLE * 2000 << 16) | 0x0103);

        DCACHE_Init((cache_com)0, (cache_com)1, (cache_com)0, (cache_com)0);
        ICACHE_Init((cache_com)0, (cache_com)1, (cache_com)0, (cache_com)0);

        hal_tsi_configBackupStruct(&htsi);
        //        htsi.Fuction.Status = TSI_s;

        // open NVIC for sleep
        NVIC_Init(3, 3, EPORT0_0_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_1_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_2_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_3_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_4_IRQn, 2);
        NVIC_Init(3, 3, EPORT0_5_IRQn, 2);
        // NVIC_Init(3, 3, TSI_IRQn, 2);
        // NVIC_Init(3, 3, CPM_IRQn, 2);
        _SET_IE_AWD(TSI);

        // TSI
        volatile uint32_t *pu32_data = &(TSI->TSI_CH0DR);
        volatile uint8_t **pu8_AlgBase = &(htsi.Backup.AlgBase.ch0);
        uint8_t fine_data;
        for (uint8_t channle = 0; channle < 16; channle++)
        {
            if (TSI->TSI_CHEN & (0x01u << channle))
            {
                fine_data = ((*pu32_data & 0xff) +
                             ((*pu32_data >> 8) & 0xff) +
                             ((*pu32_data >> 16) & 0xff) +
                             ((*pu32_data >> 24) & 0xff)) >>
                            2;
                if (fine_data > **pu8_AlgBase) // ���ϸ���
                {
                    //                    **pu8_AlgBase = **pu8_AlgBase +
                    //                                    ((fine_data - **pu8_AlgBase) >> 2);
                    **pu8_AlgBase = fine_data;
                }
                else // ���¸���
                {
                    **pu8_AlgBase = **pu8_AlgBase -
                                    ((**pu8_AlgBase - fine_data) >> 1);
                }
            }
            pu32_data++;
            pu8_AlgBase++;
        }

        NVIC_Init(3, 3, TSI_IRQn, 2);
        TC_Sleep_DelayMS(2);
        CPM->CPM_PADWKINTCR |= (1u << 3);
        NVIC->ICPR[0] = (1 << 14);              // clear cpm wake-up interrupt pending.
        while (CPM->CPM_PADWKINTCR & (1u << 3)) // PCI Asyc IRQ
        {
			
            TC_Sleep_DelayMS(2);

            PCI->PCI_ATIMPR |= (1 << 8); // clear async timet int
            CPM->CPM_PADWKINTCR |= 0xff;
            NVIC->ICPR[0] = (1 << 14); // clear cpm wake-up interrupt pending.
		}
		PCI_InitWakeup(); // PCI reinit.
        
		
			/*����ʱ�䴦�����̿�ʼ*/
		bufangtimes1= IO_READ8(SRAM_START_ADDR_M4 + 0x30);
		bufangtimes =IO_READ8(SRAM_START_ADDR_M4+ 0x20);
       //  printf("bufangtimes=%d\r\n",bufangtimes);
     if((bufangtimes<=bufangtimes1)&&(bufangtimes!=0xaa))//���С���趨ʱ��
		 {
			 
			 bufangtimes += 2;//��ʱʱ���2s
             // DEBUG_LOG_OUT("bufangtimes=%d\r\n",bufangtimes);
             if(EPORT_ReadGpioData(EPORT_PIN1)==1)//���������ŵ�ƽ,���û��
                 bufangtimes=0;
            
			// printf("bufangtimes=%d\r\n",bufangtimes);
			 PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20, bufangtimes);
		 }			 
		  else if(bufangtimes!=0xaa)//����ϵͳ
			{
              //   DEBUG_LOG_OUT("EPORT_ReadGpioData(EPORT_PIN1)=%d\r\n",EPORT_ReadGpioData(EPORT_PIN1));
			//	printf("bufangtimes=%d\r\n",bufangtimes);
				 	if(EPORT_ReadGpioData(EPORT_PIN1)==0)//���������ŵ�ƽ��������Ǹ�,����ϵͳ
					{
						*pu32_wakeupSource = WAKEUP_SOURCE_ALARM;
						return;
					}
					else//����͹����²���Ҫ��������
					{
						Nvram_Init();
						PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x20 , 0xaa);
				
					}
			}
					/*����ʱ�䴦�����̽���*/
			/*��ʱ�������̿�ʼ*/
			if(IO_READ8(SRAM_START_ADDR_M4+ 0x40) == 0x0)
			{
               
			//	printf("dingshichonglian\r\n");
				wakeuptimes = 0;
				wakeuptimes++;
				Nvram_Init();
			   PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x40, wakeuptimes);
			  while (1)
						{
							CPM_PowerOff_1p5();
						}
			}
			else
			{
				Nvram_Init();
				wakeuptimes = IO_READ8(SRAM_START_ADDR_M4+ 0x40);				
				wakeuptimes++;
				if(wakeuptimes< IO_READ8(SRAM_START_ADDR_M4)||IO_READ8(SRAM_START_ADDR_M4)==0x0)
				{
					Nvram_Init();
					PCI_NVSRAM_WtiteByte(SRAM_START_ADDR_M4 + 0x40, wakeuptimes);
					//printf("wakeuptimes=%d\r\n",wakeuptimes);
					while (1)
						{
							CPM_PowerOff_1p5();
						}
					
				}
				  *pu32_wakeupSource = WAKEUP_SOURCE_TIME;
				
				
			}
			/*��ʱ�������̽���*/
        }
        // printf("IIIRQ:%02x, 2:%d, 3:%d\r\n", EPORT->EPFR, cpm_getIRQ_Eport(EPORT, 2), cpm_getIRQ_Eport(EPORT, 3));
       
#endif
}
/**
 * @brief ��������ͨ�����㷨base.
 * 1.�Ƿ��а���
 * 2.�����Ƿ�С 
 * @param[in] htsi
 */
void app_tsi_updateBase(TSI_HandleTypeDef *htsi)
{
    volatile float *pd_baseline_old = (volatile float *)&(htsi->Baseline.Data[htsi->Baseline.point].ch0);
    htsi->Baseline.point = (htsi->Baseline.point + 1) % BASELINE_CAHCE_LEN;
    volatile float *pd_baseline_new = (volatile float *)&(htsi->Baseline.Data[htsi->Baseline.point].ch0);
    volatile float *pd_baseline_middle = (volatile float *)&(htsi->Baseline.Data[(htsi->Baseline.point + 1) % BASELINE_CAHCE_LEN].ch0);

    volatile float *pd_variace = (volatile float *)&(htsi->Algorithm.VarianceLast.ch0);
    volatile float *pd_average = (volatile float *)&(htsi->Algorithm.AverageLast.ch0);

    volatile uint8_t *pu8_Continue = &(htsi->Touch.Continue.ch0);

    for (uint8_t channel = 0; channel < 16; channel++)
    {
        if (hal_tsi_getChannleValid(htsi, channel)) // channel is valid
        {
            if (pu8_Continue[channel] < 76)
            {
                /* 1.�ж�ͨ���а��� */
                if (htsi->Touch.TouchCurrent & (1u << channel))
                {
                    pd_baseline_new[channel] = pd_baseline_old[channel];
                }
                else
                {
                    /* 2.�жϷ����㹻С */
                    if (pd_variace[channel] < (float)BASELINE_VARIANCE_TH)
                    {
                        pd_baseline_new[channel] = pd_average[channel];
                    }
                    else
                    {
                        if (pd_average[channel] > pd_baseline_old[channel])
                        {
                            pd_baseline_new[channel] = pd_baseline_old[channel] +
                                                       (pd_average[channel] - pd_baseline_old[channel]) / BASELINE_APPROACH_SCALE;
                        }
                        else
                        {
                            pd_baseline_new[channel] = pd_baseline_old[channel] -
                                                       (pd_baseline_old[channel] - pd_average[channel]) / BASELINE_APPROACH_SCALE;
                        }
                    }
                }
            }
            else // ��ʱ�䰴��, ����, 3.5s
            {
                pu8_Continue[channel] = 0;

                pd_baseline_new[channel] = pd_average[channel] + 5;
                pd_baseline_old[channel] = pd_baseline_new[channel];
                pd_baseline_middle[channel] = pd_baseline_new[channel];
            }
        }
    }
}

/**
 * @brief TSIģ���ʼ��.
 * 1. TSI�ṹ���ʼ��
 * 2. ��¼Cbase and Coarse
 * 3. reset TSI
 * 4. config TSI register
 * 5. Load Cbase and Coarse
 * 6. enable Fine scan mode.
 * 7. Check fine area.
 * 8. enable Interrupt mode.
 * @param[in] htsi 
 * @return uint32_t return 0
 */
uint32_t app_tsi_init(TSI_HandleTypeDef *htsi)
{
    TSI_InitialTypeDef initial;

    /* 1. TSI�ṹ���ʼ�� */
    hal_tsi_configInitStruct(htsi);

    do
    {
        /* 2. ��¼Cbase and Coarse */
        hal_tsi_rememberConfiguration(htsi, &initial);

        /* 3. reset TSI */
        hal_tsi_reset(htsi);

        /* 4. config TSI register */
        hal_tsi_configRegister(htsi);

        /* 5. Load Cbase and Coarse */
        hal_tsi_loadCbaseCoarse(htsi, &initial);

        /* 6. enable Fine scan mode */
        hal_tsi_switchFineMode(htsi);

        /* 7. Check fine area */
        hal_tsi_checkFine(htsi, &initial);
    } while (initial.Status == TSI_INIT_ADJUST);

    /* 8. switch to Interrupt mode */
    NVIC_Init(3, 3, TSI_IRQn, 2);
    hal_tsi_switchInterrupt(htsi);

    return 0x00;
}

/**
 * @brief TSI�㷨������.
 * 1. got data
 * 2. data previous handle
 * 3. judge touch
 * 4. update base
 * 5. adjust Cbase & Coarse dynamicly
 * 6. update state
 * @param[in] htsi 
 * @return uint32_t 
 */
uint32_t app_tsi_handle(TSI_HandleTypeDef *htsi)
{
    // 1. got fine data
    if (htsi->Cache.Data[htsi->Cache.Index_Read].Ready != 0)
    {
        //        cjson_printf_data("TSI_Fine_Data", "All", "CRC16", (uint8_t *)htsi->Data, 16 * 4 * LOOP_SCAN_NUM);

        // 2. data pre handle
        app_tsi_preprocess(htsi);

        // 6. update state
        htsi->Cache.Data[htsi->Cache.Index_Read].Ready = 0;
        htsi->Cache.Index_Read = (htsi->Cache.Index_Read + 1) % CACHE_DATA_NUM;

        // 3. judge touch
        app_tsi_updateTouch(htsi);

        // 4. update algorithm base value
        app_tsi_updateBase(htsi);

        // 5. adjust Cbase & Coarse dynamicly
        hal_tsi_adjustRegister(htsi);
    }
    return 0x00;
}

/**
 * @brief TSI goto sleep.
 * 1. ����coarseֵ���������������
 * 2. ����TSI�Ĵ������͹���ģʽ 
 * @pre hal_tsi_configInitStruct
 * @pre TSI��������֮ǰ, ���ȷ��NFC��ǿ�ر�.
 */
void app_tsi_sleep(TSI_HandleTypeDef *htsi)
{
    // TC_Sleep_DelayMS(3); // ��ʱ����, ����NFC��ǿ����.

    /* 1. ����coarseֵ��������������� */
    app_tsi_saveData(htsi);
    /* 2. ����TSI�Ĵ������͹���ģʽ */
    hal_tsi_switchLowpower(htsi);
}

/**
 * @brief TSI����.
 * 1. TSI_FUCTION_STOP, App��ֹͣ��������, ֱ���ֶ��л���TSI_FUCTION_INTERRUPTģʽ.
 * 2. TSI_FUCTION_WAIT, ����ģʽ, ��lossPacketCnt���ʹ��, ����һ��Ϊ487us.
 * 3. TSI_FUCTION_INTERRUPT, ��������ģʽ.
 * @param[in] htsi @ref TSI_HandleTypeDef
 * @param[in] status @ref e_TSI_Status_Function
 * @param[in] lossPacketCnt ����count(������TSI_FUCTION_WAITʱ). �����ֵ>1.
 * @pre app_tsi_init
 */
void app_tsi_stop(TSI_HandleTypeDef *htsi, const e_TSI_Status_Function status, const uint8_t lossPacketCnt)
{

    if (status == TSI_FUCTION_STOP)
    {
        htsi->Fuction.Status = TSI_FUCTION_STOP;
    }
    else if (status == TSI_FUCTION_WAIT) // �Ӻ�TSI����ת��, һ������4.09ms
    {
        htsi->Fuction.Status = TSI_FUCTION_WAIT;
        htsi->Fuction.lossPacketCnt = lossPacketCnt;
    }
    else if (status == TSI_FUCTION_RUN)
    {
        htsi->Fuction.Status = TSI_FUCTION_RUN;
    }
}

/************************ (C) COPYRIGHT C*Core *****END OF FILE**********************/
