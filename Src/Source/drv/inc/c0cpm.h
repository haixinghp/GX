//----------------------
//  CPM REG DEFINE
//----------------------

#define C0CPM_BASE            (0x08140000)

#define C0CPM_SLPCFGR         (*(volatile unsigned int *)(C0CPM_BASE + 0x00))
#define C0CPM_SLPCR           (*(volatile unsigned int *)(C0CPM_BASE + 0x04))
#define C0CPM_SCDIVR          (*(volatile unsigned int *)(C0CPM_BASE + 0x08))
#define C0CPM_PCDIVR          (*(volatile unsigned int *)(C0CPM_BASE + 0x0c))
#define C0CPM_OCSR            (*(volatile unsigned int *)(C0CPM_BASE + 0x10))
#define C0CPM_CSWCFGR         (*(volatile unsigned int *)(C0CPM_BASE + 0x14))
#define C0CPM_CSWUPDR         (*(volatile unsigned int *)(C0CPM_BASE + 0x18))
//0x1c:reserved
#define C0CPM_CGTCR           (*(volatile unsigned int *)(C0CPM_BASE + 0x20))
#define C0CPM_SYSCGTCR        (*(volatile unsigned int *)(C0CPM_BASE + 0x24))
//0x28:reserved
//0x2c:reserved
#define C0CPM_PWRCR           (*(volatile unsigned int *)(C0CPM_BASE + 0x30))
#define C0CPM_SLPCNTR         (*(volatile unsigned int *)(C0CPM_BASE + 0x34))
#define C0CPM_WKPCNTR         (*(volatile unsigned int *)(C0CPM_BASE + 0x38))
#define C0CPM_SRSTMASKR       (*(volatile unsigned int *)(C0CPM_BASE + 0x3c))
#define C0CPM_VCCQGTRIMR      (*(volatile unsigned int *)(C0CPM_BASE + 0x40))
#define C0CPM_VCCQLTRIMR      (*(volatile unsigned int *)(C0CPM_BASE + 0x44))
#define C0CPM_VCCQVTRIMR      (*(volatile unsigned int *)(C0CPM_BASE + 0x48))
#define C0CPM_VCCQCTMR        (*(volatile unsigned int *)(C0CPM_BASE + 0x4c))
//0x50:reserved
//0x54:reserved
//0x58:reserved
//0x5c:reserved
#define C0CPM_O8MTRIMR        (*(volatile unsigned int *)(C0CPM_BASE + 0x60))
//0x64:reserved
//0x68:reserved
#define C0CPM_WKPFILTCR       (*(volatile unsigned int *)(C0CPM_BASE + 0x6c))
#define C0CPM_OSCLSTIMER      (*(volatile unsigned int *)(C0CPM_BASE + 0x70))
//0x74:reserved
//0x78:reserved
#define C0CPM_PWRSR           (*(volatile unsigned int *)(C0CPM_BASE + 0x7c))
#define C0CPM_MULTISRSTMR     (*(volatile unsigned int *)(C0CPM_BASE + 0x80))
#define C0CPM_SYSSRSTMR       (*(volatile unsigned int *)(C0CPM_BASE + 0x84))
//0x88:reserved
//0x8c:reserved
#define C0CPM_CSWSSR          (*(volatile unsigned int *)(C0CPM_BASE + 0x90))
#define C0CPM_FILTCNTR        (*(volatile unsigned int *)(C0CPM_BASE + 0x94))
#define C0CPM_HIBERCNTR       (*(volatile unsigned int *)(C0CPM_BASE + 0x98))
#define C0CPM_WKPCSR          (*(volatile unsigned int *)(C0CPM_BASE + 0x9c))
#define C0CPM_MPDSLPCR	      (*(volatile unsigned int *)(C0CPM_BASE + 0xa0))
#define C0CPM_APDSLPCR	      (*(volatile unsigned int *)(C0CPM_BASE + 0xa4))
#define C0CPM_LVDTFILTCNTR    (*(volatile unsigned int *)(C0CPM_BASE + 0xa8))
#define C0CPM_PWRINDICR       (*(volatile unsigned int *)(C0CPM_BASE + 0xac))
#define C0CPM_LPWKPSCR        (*(volatile unsigned int *)(C0CPM_BASE + 0xb0))
#define C0CPM_LPWKPSSR        (*(volatile unsigned int *)(C0CPM_BASE + 0xb4))
#define C0CPM_POFFWKPSCR      (*(volatile unsigned int *)(C0CPM_BASE + 0xb8))
#define C0CPM_POFFWKPSSR      (*(volatile unsigned int *)(C0CPM_BASE + 0xbc))
//0xc0:reserved
#define C0CPM_WKPSPOLR        (*(volatile unsigned int *)(C0CPM_BASE + 0xc4))
#define C0CPM_WKPPADCR        (*(volatile unsigned int *)(C0CPM_BASE + 0xc8))
//0xcc:reserved
//==================================================================

#define C0WDT_CR              (*(volatile unsigned short*)(0x08141000 + 0x00))
#define C0TC_CR               (*(volatile unsigned short*)(0x08142000 + 0x00))


//#define SAVE_CNT(n)        (n<<24)
//#define RESTORE_CNT(n)     (n<<16)
//#define EMMC_RETENT        (1<<8)
//#define VDD_WK_SWOFF       (1<<5)
//#define VDD_WK_RETENT      (1<<4)
//#define AUTO_I_ISOEN       (1<<3)
//#define RET_BK_SWOFF       (1<<1)
//#define VDD_PD_RETENT      (1<<0)
//#define CACHE_IDLE_SLPWAIT (1<<12)
////----------------------
////  CPM BIT DEFINE
////----------------------
////CPM_SLPCFGR
//#define AUTO_HIBER          (0x1<<31)
//#define HIBERCLK_SLPEN      (0x1<<30)
//#define CORE_F_SLPEN        (0x1<<29)
//#define CLKOUT_SLPEN        (0x1<<28)
//#define DETCLK_SLPEN        (0x1<<27)
//#define TCCLK_SLPEN         (0x1<<26)
//#define DECCLK_SLPEN        (0x1<<25)
//#define EMMC_SLPD           (0x1<<24)
//#define EMMC_SLPCNT(n)      (n<<20)
//#define EMMC_128K_SEL(n)    (n<<19)
//#define EMMCCLK_SLPEN       (0x1<<18)
//#define OSC128K_SLPEN(n)    (n<<17)
//#define OSC24M_SLPEN(n)     (n<<16)
//#define OSCEXT_SLPEN(n)     (n<<15)
//#define SD_HPPD             (0x1<<14)
//#define VCC_HPRDY_MASK      (0x1<<11)
//#define HIBER_ATONCE        (0x1<<10)
//#define VCC_POFF            (0x1<<9)
//#define VCC_LP              (0x1<<8)
//#define FLASH_IPSLP(n)      (n<<7)
//#define FLASH_HPPD          (0x1<<6)
//#define FLASH_SWOFF         (0x1<<5)
//#define EMMC_SWOFF          (0x1<<3)
//#define USBPHY_SWOFF        (0x1<<2)
//#define SYS_POFF            (0x1<<1)
//#define SYS_LP              (0x1<<0)

//#define CFG_VCC_POFF(n)     (n<<9)
//#define CFG_VCC_LP(n)       (n<<8)
//#define CFG_EMMC_SWOFF(n)   (n<<3)
//#define CFG_USBPHY_SWOFF(n) (n<<2)
//#define CFG_SYS_POFF(n)     (n<<1)
//#define CFG_SYS_LP(n)       (n<<0)

////CPM_SLPCR
//#define SLEEP               (0x1<<0)

////CPM_SCDIVR
//#define NFCDP_DIV(n)        (n<<24)
//#define NFCSRAM_DIV(n)      (n<<20)
//#define NFCPHY_DIV(n)       (n<<16)
//#define EPT_DIV(n)          (n<<8)
//#define SYS_DIV(n)          (n<<0)

////CPM_PCDIVR
//#define DET_DIV(n)          (n<<28)
//#define TC_DIV(n)           (n<<24)
//#define DIV_LED(n)          (n<<20)
//#define INIT_DIV(n)         (n<<16)
//#define EFM_DIV(n)          (n<<12)
//#define ARITH_DIV(n)        (n<<8)
//#define CLKOUT_DIV(n)       (n<<4)
//#define IPS_DIV(n)          (n<<0)

////CPM_OCSR
//#define OSCEXT_STABLE       (0x1<<12)
//#define OSC533M_STABLE      (0x1<<11)
//#define OSC400M_STABLE      (0x1<<10)
//#define OSC128K_STABLE      (0x1<<9)
//#define OSC8M_STABLE       (0x1<<8)
//#define OSCEXT_EN           (0x1<<4)
//#define OSC533M_EN          (0x1<<3)
//#define OSC400M_EN          (0x1<<2)
//#define OSC128K_EN          (0x1<<1)
//#define OSC24M_EN           (0x1<<0)

////CPM_CSWCFGR
//#define EFM_DIVSEL          (0x1<<13)
//#define IPS_DIVSEL          (0x1<<12)
//#define DET_DIVSEL          (0x1<<11)
//#define TC_DIVSEL           (0x1<<10)
//#define NFCDP_DIVSEL        (0x1<<9)
//#define ARITH_DIVSEL        (0x1<<8)
//#define BOSC_SEL            (0x1<<6)
//#define HOSC_SEL            (0x1<<5)
//#define LOSC_SEL            (0x1<<4)
//#define DETCLK_SEL          (0x1<<3)
//#define TCCLK_SEL           (0x1<<2)
//#define NFCCLK_SEL          (0x1<<1)
//#define SYSCLK_SEL          (0x1<<0)

////CPM_CSWUPDR
//#define DIVUPD              (0x1<<1)
//#define CSWUPD              (0x1<<0)

////CPM_CSWDSR
//#define EFMDIV_SWD          (0x1<<13)
//#define IPSDIV_SWD          (0x1<<12)
//#define DETDIV_SWD          (0x1<<11)
//#define TCDIV_SWD           (0x1<<10)
//#define NFCDPDIV_SWD        (0x1<<9)
//#define ARITHDIV_SWD        (0x1<<8)
//#define HOSC_SWD            (0x1<<5)
//#define LOSC_SWD            (0x1<<4)
//#define DETCLK_SWD          (0x1<<3)
//#define TCCLK_SWD           (0x1<<2)
//#define NFCCLK_SWD          (0x1<<1)
//#define SYSCLK_SWD          (0x1<<0)

////CPM_CGTCR
//#define CLK_GTE(n)          (0x1<<n)
//#define MULTICLK_GTN        17
//#define SYSCLK_GTN          25
//#define ARITHCLK_GTN        9
//#define IPSCLK_GTN          26

//#define CGATE_CPM_GTE           0x00000001
//#define CGATE_PWR_GTE           0x00000002
//#define CGATE_EMMC_GTE          0x00000004
//#define CGATE_EMMC_PAD_GTE      0x00000008
//#define CGATE_USBPHY_REF_GTE    0x00000010
//#define CGATE_USBPHY_X_GTE      0x00000020
//#define CGATE_TC_GTE            0x00000040
//#define CGATE_DET_GTE           0x00000080
//#define CGATE_NFCPHY_GTE        0x00000100
//#define CGATE_NFC_DPSRAM_GTE    0x00000200
//#define CGATE_NFC_DP_GTE        0x00000400
//#define CGATE_SWP_DECODE_GTE    0x00000800
//#define CGATE_INIT_GTE          0x00001000
//#define CGATE_LED_GTE           0x00002000
//#define CGATE_EFM_AHB_GTE       0x00004000
//#define CGATE_CLKOUT_GTE        0x00008000
//#define CGATE_HIBER_GTE         0x00010000
//#define CGATE_USBC_GTE          0x00020000
//#define CGATE_MPU_GTE           0x00040000
//#define CGATE_ROM_GTE           0x00080000
//#define CGATE_ROM_CODE_GTE      0x00100000
//#define CGATE_RIMSCLK_GTE       0x00200000
//#define CGATE_FIG_GTE           0x00400000
//#define CGATE_XSTPCLK_GTE       0x00800000

//#define SYS_SRAM0_GTE           0x00000001
//#define SYS_RSVD1_GTE           0x00000002
//#define SYS_RSVD2_GTE           0x00000004
//#define SYS_RSVD3_GTE           0x00000008
//#define SYS_CAMC_GTE            0x00000010
//#define SYS_HACC_GTE            0x00000020
//#define SYS_RSVD6_GTE           0x00000040
//#define SYS_NFC_GTE             0x00000080
//#define SYS_RSVD8_GTE           0x00000100
//#define SYS_RSVD9_GTE           0x00000200
//#define SYS_RSVD10_GTE          0x00000400
//#define SYS_RSVD11_GTE          0x00000800
//#define SYS_RSVD12_GTE          0x00001000
//#define SYS_MIM_GTE             0x00002000
//#define SYS_DMA_GTE             0x00004000
//#define SYS_CRC_GTE             0x00008000
//#define SYS_SRAM1_GTE           0x00010000
//#define SYS_SRAM2_GTE           0x00020000
//#define SYS_SRAM3_GTE           0x00040000
//#define SYS_RSVD19_GTE          0x00080000
//#define SYS_SCU_GTE             0x00100000
//#define SYS_RSVD21_GTE          0x00200000
//#define SYS_RSVD22_GTE          0x00400000
//#define SYS_CRC1_GTE            0x00800000

//#define ARITH_ENCR_A_GTE        0x00000001
//#define ARITH_AES_GTE           0x00000002
//#define ARITH_SCB2_GTE          0x00000004
//#define ARITH_SMS4_GTE          0x00000008
//#define ARITH_SSF33_GTE         0x00000010
//#define ARITH_CRYPTO_GTE        0x00000020
//#define ARITH_SHA_GTE           0x00000040
//#define ARITH_DES3_GTE          0x00000080
//#define ARITH_RFIFO_GTE         0x00000100

//#define IPS_EFM_APB_GTE         0x00000001
//#define IPS_WDT_GTE             0x00000002
//#define IPS_TRNG_GTE            0x00000004
//#define IPS_PIT1_GTE            0x00000008
//#define IPS_PIT2_GTE            0x00000010
//#define IPS_LED_GTE             0x00000020
//#define IPS_LD_GTE              0x00000040
//#define IPS_PGD_GTE             0x00000080
//#define IPS_RSVD8_GTE           0x00000100
//#define IPS_SEC_DET_GTE         0x00000200
//#define IPS_TC_GTE              0x00000400
//#define IPS_SWP_GTE             0x00000800
//#define IPS_USI1_GTE            0x00001000
//#define IPS_USI2_GTE            0x00002000
//#define IPS_USI3_GTE            0x00004000
//#define IPS_I2C_GTE             0x00008000
//#define IPS_SCI1_GTE            0x00010000
//#define IPS_SCI2_GTE            0x00020000
//#define IPS_EPORT_GTE           0x00040000
//#define IPS_SPI1_GTE            0x00080000
//#define IPS_SPI2_GTE            0x00100000
//#define IPS_SPI3_GTE            0x00200000
//#define IPS_ENCR_P_GTE          0x00400000
//#define IPS_EPORT2_GTE          0x00800000
//#define IPS_IOMUX_APB_GTE       0x01000000
//#define IPS_IPS_BUS_GTE         0x02000000



////CPM_PWRCR
//#define INDIC_BIT           (0x1<<31)
//#define EMMC_O_ISOEN        (0x1<<30)
//#define EMMC_RSTMASK        (0x1<<29)
//#define EMMC_PSWEN          (0x1<<28)
//#define USBPHY_I_ISOEN      (0x1<<27)
//#define USBPHY_O_ISOEN      (0x1<<26)
//#define USBPHY_RSTMASK      (0x1<<25)
//#define USBPHY_PSWEN        (0x1<<24)
//#define VCC_RE_LVDT33       (0x1<<23)
//#define VCC_RE_LVDT18       (0x1<<22)
//#define VCCQ_RE_LVDT33      (0x1<<21)
//#define VCCQ_RE_LVDT18      (0x1<<20)
//#define EMMC_FILTEN         (0x1<<19)
//#define USBPHY_FILTEN       (0x1<<18)
//#define WAKEUP_FILTEN       (0x1<<17)
//#define SD_PWRSEL           (0x1<<16)
//#define VCC_IE_LVDT33       (0x1<<15)
//#define VCC_IE_LVDT18       (0x1<<14)
//#define VCCQ_IE_LVDT33      (0x1<<13)
//#define VCCQ_IE_LVDT18      (0x1<<12)
//#define VCC_CLR_LATCH       (0x1<<11)
//#define VCC_SET_LATCH       (0x1<<10)
//#define VCCQ_CLR_LATCH      (0x1<<9)
//#define VCCQ_SET_LATCH      (0x1<<8)
//#define VCC_OE_LVDT33       (0x1<<7)
//#define VCC_OE_LVDT18       (0x1<<6)
//#define VCCQ_OE_LVDT33      (0x1<<14)
//#define VCCQ_OE_LVDT18      (0x1<<6)
//#define VCC_EN_LVDT33       (0x1<<3)
//#define VCC_EN_LVDT18       (0x1<<2)
//#define VCCQ_EN_LVDT33      (0x1<<1)
//#define VCCQ_EN_LVDT18      (0x1<<0)
//#define VCCQ_EN_LVDT33_W    (0x1<<15)
//#define VCCQ_EN_LVDT18_W    (0x1<<7)

////CPM_SLPCNTR
//#define SLPCNT0(n)          (n<<24)
//#define SLPCNT1(n)          (n<<16)
//#define SLPCNT2(n)          (n<<8)
//#define SLPCNT3(n)          (n<<0)

////CPM_WKPCNTR
//#define WKPCNT0(n)          (n<<24)
//#define WKPCNT1(n)          (n<<16)
//#define WKPCNT2(n)          (n<<8)
//#define WKPCNT3(n)          (n<<0)

////CPM_SRSTMASKR
//#define EMMCRST_SCUSEL      (0x1<<17)
//#define EMMCRST_CPMSEL      (0x1<<16)
//#define NFCPHY_SRM          (0x1<<12)
//#define NFCPHYREG_SRM       (0x1<<11)
//#define NFCCOM_SRM          (0x1<<10)
//#define NFCBUS_SRM          (0x1<<9)
//#define NFC_SRM             (0x1<<8)
//#define EMMCFIFO_SRM        (0x1<<2)
//#define EMMCBUS_SRM         (0x1<<1)
//#define EMMC_SRM            (0x1<<0)

//#define VCCQ_TRIM_128KHZ(n) (n<<24)
//#define VCCQ_TRIM_2KHZ(n)   (n<<16)
//#define VCCQ_ST_1V          (0x1<<15)
//#define VCCQ_SAMPLE_DIV(n)  (n<<8)
//#define VCCQ_TEST_BIAS      (0x1<<7)
//#define VCCQ_BIAS1_RES(n)   (n<<4)
//#define VCCQ_BIAS2_RES(n)   (n<<0)

//#define VD_PWRSEL           (0x1<<31)
//#define VCCQ_COARSE_EN      (0x1<<24)
//#define VCCQ_LVDT33_MSB(n)  (n<<16)
//#define VCCQ_LVDT33_HYS(n)  (n<<12)
//#define VCCQ_OE_LVDT33      (0x1<<14)
//#define VCCQ_LVDT33_TRIM(n) (n<<8)
//#define CCQ_OE_LVDT18      (0x1<<6)
//#define VCCQ_LVDT18_HYS(n)  (n<<4)
//#define VCCQ_LVDT18_TRIM(n) (n<<0)

//#define VCCQ_VREF_FORCE_ST  (0x1<<11)
//#define VCCQ_VREF_TRIM_EN   (0x1<<10)
//#define VCCQ_VREF_LOAD      (0x1<<9)
//#define VCCQ_VREF_STORE_EN  (0x1<<8)
//#define VCCQ_VREF_TRIM(n)   (n<<0)

//#define VCCQ_CTM_KEY(n)     (n<<30)
//#define VCCQ_CTM_OFF_WK     (0x1<<8)
//#define VCCQ_CTM_SD_LP_OFF  (0x1<<6)
//#define VCCQ_CTM_FLS_LP_OFF (0x1<<5)
//#define VCCQ_CTM_LP_OFF     (0x1<<4)
//#define VCCQ_CTM_OFF_MODE2  (0x1<<2)
//#define VCCQ_CTM_OFF_MODE   (0x1<<1)
//#define VCCQ_CTM_EN_LP      (0x1<<0)

//#define XSTPCLK_SLPEN	    (0x1<<12)
//#define XSTP_SRC_SEL        (0x1<<30)
//#define XSTP_EN             (0x1<<29)
//#define EMMC_XSTP           (0x1<<23)
//#define EMMC_XSTP_RST       (0x1<<23)
//#define XSTP_DIV(n)         (n<<12)

//#define PORCLK_GTE          (0x1<<28)
//#define DIVUPD_SLPEN        (0x1<<13)


//#define VCC_TRIM_128KHZ     (0x10<<24)
//#define VCC_TRIM_2KHZ(n)    (n<<16)
//#define VCC_ST_1V           (0x0<<15)
//#define VCC_SAMPLE_DIV(n)   (n<<8)
//#define VCC_TEST_BIAS       (0x1<<7)
//#define VCC_BIAS1_RES(n)    (n<<4)
//#define VCC_BIAS2_RES(n)    (n<<0)

//#define VCC_COARSE_EN       (0x1<<24)
//#define VCC_LVDT33_MSB(n)   (n<<16)
//#define VCC_LVDT33_HYS(n)   (n<<12)
//#define VCC_LVDT33_TRIM(n)  (n<<8)
//#define VCC_LVDT18_HYS(n)   (n<<4)
//#define VCC_LVDT18_TRIM(n)  (n<<0)

//#define VCC_VREF_FORCE_ST   (0x1<<11)
//#define VCC_VREF_TRIM_EN    (0x1<<10)
//#define VCC_VREF_LOAD       (0x1<<9)
//#define VCC_VREF_STORE_EN   (0x1<<8)
//#define VCC_VREF_TRIM(n)    (n<<0)

//#define VCC_CTM_KEY(n)      (n<<30)
//#define VCC_CTM_OFF_WK      (0x1<<8)
//#define VCC_CTM_TEST_B      (0x1<<7)
//#define VCC_CTM_OFF_MODE2   (0x1<<2)
//#define VCC_CTM_OFF_MODE    (0x1<<1)
//#define VCC_CTM_EN_LP       (0x1<<0)

//#define O24M_CTRIM(n)       (n<<15)
//#define O24M_CTRIM_TD(n)    (n<<8)
//#define O24M_FTRIM(n)       (n<<3)
//#define O24M_TEMPTRIM(n)    (n<<0)

//#define O400M_CTRIM(n)      (n<<15)
//#define O400M_CTRIM_TD(n)   (n<<8)
//#define O400M_FTRIM(n)      (n<<3)
//#define O400M_TEMPTRIM(n)   (n<<0)

//#define O533M_CTRIM(n)      (n<<15)
//#define O533M_CTRIM_TD(n)   (n<<8)
//#define O533M_FTRIM(n)      (n<<3)
//#define O533M_TEMPTRIM(n)   (n<<0)

//#define WKP_AFILT_BYPS(n)   (n<<15)
//#define WKP_AFILT_TRIM(n)   (n<<8)
//#define ULDO_VREF_TRIM(n)   (n<<0)

//#define OSCEXT_WKPWAIT      (0x1<<28)

////CPM_PWRSR
//#define EMMC_O_ISOEN_OUT    (0x1<<30)
//#define EMMC_RSTMASK_OUT    (0x1<<29)
//#define EMMC_PSWEN_OUT      (0x1<<28)
//#define USBPHY_I_ISOEN_OUT  (0x1<<27)
//#define USBPHY_O_ISOEN_OUT  (0x1<<26)
//#define USBPHY_RSTMASK_OUT  (0x1<<25)
//#define USBPHY_PSWEN_OUT    (0x1<<24)
//#define VCC_LVDT33_FLAG     (0x1<<23)
//#define VCC_LVDT18_FLAG     (0x1<<22)
//#define VCCQ_LVDT33_FLAG    (0x1<<21)
//#define VCCQ_LVDT18_FLAG    (0x1<<20)
//#define VCC_LVDT33_ST       (0x1<<19)
//#define VCC_LVDT18_ST       (0x1<<18)
//#define VCCQ_LVDT33_ST      (0x1<<17)
//#define VCCQ_LVDT18_ST      (0x1<<16)
//#define VCC_OFF_MODE2_OUT   (0x1<<12)
//#define VCC_HPRDY_ST        (0x1<<11)
//#define VCCQ_HPRDY_ST       (0x1<<3)
//#define VCCQ_POR33_ST       (0x1<<2)
//#define VCCQ_POR11_ST       (0x1<<1)
//#define VCCQ_POR11_V2_ST    (0x1<<0)

////CPM_MULTISRSTMR
//#define RST_MASK(n)         (0x1<<n)

////CPM_CSWSSR
//#define EFMDIV_SEL1         (0x1<<29)
//#define IPSDIV_SEL1         (0x1<<28)
//#define DETDIV_SEL1         (0x1<<27)
//#define TCDIV_SEL1          (0x1<<26)
//#define NFCDPDIV_SEL1       (0x1<<25)
//#define ARITHDIV_SEL1       (0x1<<24)
//#define BOSC_SEL1           (0x1<<22)
//#define HOSC_SEL1           (0x1<<21)
//#define LOSC_SEL1           (0x1<<20)
//#define DETCLK_SEL1         (0x1<<19)
//#define TCCLK_SEL1          (0x1<<18)
//#define NFCCLK_SEL1         (0x1<<17)
//#define SYSCLK_SEL1         (0x1<<16)
//#define EFMDIV_SEL0         (0x1<<13)
//#define IPSDIV_SEL0         (0x1<<12)
//#define DETDIV_SEL0         (0x1<<11)
//#define TCDIV_SEL0          (0x1<<10)
//#define NFCDPDIV_SEL0       (0x1<<9)
//#define ARITHDIV_SEL0       (0x1<<8)
//#define BOSC_SEL0           (0x1<<6)
//#define HOSC_SEL0           (0x1<<5)
//#define LOSC_SEL0           (0x1<<4)
//#define DETCLK_SEL0         (0x1<<3)
//#define TCCLK_SEL0          (0x1<<2)
//#define NFCCLK_SEL0         (0x1<<1)
//#define SYSCLK_SEL0         (0x1<<0)

////CPM_FILTCNTR
//#define FILTCNT0(n)         (n<<24)
//#define FILTCNT1(n)         (n<<16)
//#define FILTCNT2(n)         (n<<8)
//#define FILTCNT3(n)         (n<<0)

////CPM_WKPCSR
//#define FLASH_TRIM_MASK(n)  (n<<9)

