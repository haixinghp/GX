/*
 * CPM.h
 *
 *  Created on: 2016年9月14日
 *      
 */

#ifndef CPM_H_
#define CPM_H_
#include "CCM_Types.h"

typedef struct{
	VU32  SLPCFGR;       //Sleep Config Register
	VU32  SLPCR;         //Sleep Control Register
	VU32  SCDIVR;        //System Clock Divider Register
	VU32  PCDIVR1;       //Peripheral Clock Divider Register1
	
	VU32  PCDIVR2;       //Peripheral Clock Divider Register2
	VU32  PCDIVR3;       //Peripheral Clock Divider Register3
	VU32  CDIVUPDR;      //Clock Switch UpdateRegister
	VU32  CDIVENR;

	VU32  OCSR;          //Oscillator Control and Status Register
	VU32  CSWCFGR;       //Clock Switch Config Register
	VU32  CTICKR;
	VU32  CHIPCFGR;

	VU32  PWRCR;         //Power Control Register
	VU32  SLPCNTR;       //Sleep Counter Register
	VU32  WKPCNTR;       //Wakeup Counter Register
	VU32  MULTICGTCR;    //Multi Clock Gate Control Register

	VU32  SYSCGTCR;      //System Clock Gate Control Register
	VU32  AHB3CGTCR;     //AHB3 Clock Gate Control Register
	VU32  ARITHCGTCR;    //Arith Clock Gate Control Register
	VU32  IPSCGTCR;      //Ips Clock Gate Control Register

	VU32  VCCGTRIMR;     //VCC General Trim Register
	VU32  VCCLTRIMR;     //VCC Lvdt Trim Register
	VU32  VCCVTRIMR;     //VCC Vref Trim Register
	VU32  VCCCTMR;       //VCC Core Test Mode Register

	VU32  O8MTRIMR;      //OSC8M Trim Register
	VU32  RESERVED1;
	VU32  O600MTRIMR;    //OS600M Trim Register
	VU32  CARDTRIMR;	 //Card LDO Trim Register

	VU32  OSCLSTIMER;    //OSCL Stable Time Register
	VU32  OSCHSTIMER;	 //OSCH Stable Time Register
	VU32  OSCESTIMER;	 //OSCE Stable Time Register
	VU32  PWRSR;         //Power Status Register

	VU32  RESERVED2[3];
	VU32  RTCTRIMR;		 //RTC

	VU32  PADWKINTCR;
	VU32  FILTCNTR;
	VU32  CARDPOCR;
	VU32  RTCSTIMER;

	VU32  MPDSLPCR;
	VU32  MRMCR;
	VU32  RESERVED3;
	VU32  MULTIRSTCR;

	VU32  SYSRSTCR;
	VU32  AHB2RSTCR;
	VU32  ARITHRSTTCR;
	VU32  IPRSTCR;

	VU32  SLPCFGR2;
	VU32  RESERVED4[3];

	VU32  PDNCNTR;
	VU32  PONCNTR;
}CPM_t;



/* CPM_SYSCGTCR */
#define DMAC1_GTE				(1<<1)
#define DMAC2_GTE				(1<<2)
#define CRC0_GTE				(1<<4)
#define CRC1_GTE				(1<<5)
#define AHB2_MUX_GTE		(1<<11)
#define SRAMD_GTE				(1<<12)
#define SRAM0_GTE				(1<<13)
#define SRAM1_GTE				(1<<14)
#define SRAM2_GTE				(1<<15)
#define SRAM3_GTE				(1<<16)
#define SSI4_GTE				(1<<17)
#define SSI5_GTE				(1<<18)
#define ROM_GTE					(1<<19)
#define M2S_BUS_M_GTE		(1<<22)

/* CPM_AHB3CGTCR */
#define USBC_GTE					(1<<3)
#define AHB3_MUX_GTE			(1<<5)

/* CPM_IPSCGTCR */
#define IO_CTRL_GTE			(1<<0)
#define WDT_GTE					(1<<1)
#define RTC_GTE					(1<<2)
#define PIT1_GTE				(1<<3)
#define PIT2_GTE				(1<<4)
#define USI1_GTE				(1<<5)
#define EDMAC1_GTE			(1<<6)
#define SPI1_GTE				(1<<7)
#define SPI2_GTE				(1<<8)
#define SPI3_GTE				(1<<9)
#define SCI1_GTE				(1<<10)
#define SCI2_GTE				(1<<11)
#define USI2_GTE				(1<<12)
#define I2C1_GTE				(1<<14)
#define PWM0_GTE				(1<<15)
#define I2C2_GTE				(1<<16)
#define I2C3_GTE				(1<<17)
#define SCI3_GTE				(1<<18)
#define QADC_GTE				(1<<20)
#define DAC_GTE					(1<<21)
#define MCC_GTE					(1<<22)
#define TSI_GTE					(1<<23)
#define LD_GTE					(1<<24)
#define TRNG_GTE				(1<<25)
#define PGD_GTE					(1<<26)
#define SEC_DET_APB			(1<<27)
#define PCI_GTE					(1<<28)
#define PMURTC_GTE			(1<<29)
#define AHB2IPS_GTE			(1<<30)
#define RIMS_APB				(1<<31)

/* CPM_MULTICGTCR */
#define EFM_BUS_CGT			(1<<1)
#define MCC_CGT					(1<<5)
#define MCCADR_CGT			(1<<6)
#define ADC_CGT					(1<<7)
#define MESH_CGT				(1<<9)
#define TC_CGT					(1<<10)
#define CLKOUT_CGT			(1<<15)
#define KEY_CTRL_CGT		(1<<16)
#define EFM_CGT					(1<<18)
#define CPM_CGT					(1<<19)
#define EPORT_CGT				(1<<20)
#define EPORT1_CGT			(1<<21)
#define EPORT2_CGT			(1<<22)
#define EPORT3_CGT			(1<<23)
#define EPORT4_CGT			(1<<24)
#define TRACE_CGT				(1<<26)


/* CPM_ARITHCGTCR */
#define AES_CGT					(1<<1)
#define SMS4_CGT				(1<<2)
#define RF_AES_SM4_CGT	(1<<4)
#define CRYPTO_CGT			(1<<5)
#define SHA_CGT					(1<<6)
#define EDMAC0_CGT			(1<<7)
#define DES3_CGT				(1<<8)
#define ZUC_CGT					(1<<9)
#define AHB2MLB_CGT			(1<<10)
#define ADB2IPS2_CGT		(1<<11)


/* CPM_SCDIVR */
#define SYS_DIV_MASK			(0xff)
#define TRACE_DIV_MASK		(0xff00)
#define CLKOUT_DIV_MASK		(0xff0000)

#define SYS_DIV_SHIFT				(0)
#define TRACE_DIV_SHIFT			(8)
#define CLKOUT_DIV_SHIFT		(16)

/*CPM_PCDIVER1 MACRO*/
#define IPS_DIV_MASK					(0xf)
#define AHB3_DIV_MASK					(0xf00)
#define ARITH_DIV_MASK				(0xf000)
#define SDRAM_DIV_MASK				(0xf0000)
#define SDRAM_SM_MASK					(0x7000000)
#define SDRAM2LCD_DIV_MASK		(0xf0000000)

#define IPS_DIV_SHIFT					(0)
#define AHB3_DIV_SHIFT				(8)
#define ARITH_DIV_SHIFT				(12)
#define SDRAM_DIV_SHIFT				(16)
#define SDRAM_SM_SHIFT				(24)
#define SDRAM2LCD_DIV_SHIFT		(28)

/*CPM_PCDIVER2 MACRO*/
#define MCC_DIV_MASK			(0x1ff)
#define MCCADR_DIV_MASK		(0xe00)
#define ADC_DIV_MASK			(0xf000)
#define CLCD_DIV_MASK			(0xf0000)
#define MESH_DIV_MASK			(0xf000000)
#define TC_DIV_MASK				(0xf0000000)

#define MCC_DIV_SHIFT				(0)
#define MCCADR_DIV_SHIFT		(9)
#define ADC_DIV_SHIFT				(12)
#define CLCD_DIV_SHIFT			(16)
#define MESH_DIV_SHIFT			(24)
#define TC_DIV_SHIFT				(28)

/*CPM_PCDIVER3 MACRO*/
#define DMA2D_SRAM_DIV_MASK			(0xf)
#define MIPI_SAMPLE_DIV_MASK		(0xf0)
#define DCMI_PIX_DIV_MASK				(0xf00)
#define DCMI_SENSOR_DIV_MASK		(0x3f0000)

#define DMA2D_SRAM_DIV_SHIFT		(0)
#define MIPI_SAMPLE_DIV_SHIFT		(4)
#define DCMI_PIX_DIV_SHIFT			(8)
#define DCMI_SENSOR_DIV_SHIFT		(16)

/*CPM_CDIVUPDR MACRO*/
#define PERDIV_UPD		(1)
#define SYSDIV_UPD		(2)

/*CPM_CDIVENR MACRO*/
#define IPS_DIVEN						(1<<0)
#define AHB3_DIVEN					(1<<2)
#define ARITH_DIVEN					(1<<3)
#define SDRAM_DIVEN					(1<<4)
#define SDRAM_SM_DIVEN			(1<<6)
#define SDRAM2LCD_DIVEN			(1<<7)
#define MCC_DIVEN						(1<<8)
#define ADC_DIVEN						(1<<10)
#define CLCD_DIVEN					(1<<11)
#define MESH_DIVEN					(1<<12)
#define TC_DIVEN						(1<<13)
#define TRACE_DIVEN					(1<<14)
#define CLKOUT_DIVEN				(1<<15)
#define DMA2D_SRAM_DIVEN		(1<<16)
#define MIPI_SAMPLE_DIVEN		(1<<17)
#define DCMI_PIX_DIVEN			(1<<18)
#define DCMI_SENSOR_DIVEN		(1<<19)

/*CPM_OCSR MACRO*/
#define OSC8M_EN							(1<<0)
#define PMU128K_EN						(1<<1)
#define USB_PHY240M_EN				(1<<2)
#define OSC160M_EN						(1<<3)
#define OSCEXT_EN							(1<<4)
#define RTC32K_EN							(1<<5)
#define OSC8M_STABLE					(1<<8)
#define PMU128K_STABLE				(1<<9)
#define USB_PHY240M_STABLE		(1<<10)
#define OSC160M_STABLE				(1<<11)
#define OSCEXT_STABLE					(1<<12)
#define RTC32K_STABLE					(1<<13)
#define PMU2K_VALID						(1<<14)
#define TRNG_OSCEN_MASK				(0xf000000)

#define TRNG_OSCEN_SHIFT			(24)

/* CPM_CSWCFGR MACRO*/
#define SYSCLK_SEL_MASK							(0x3)
#define DCMI_SENSOR_SEL_MASK				(0x3f)
#define OSCL_SEL										(1<<6)
#define OSCH_SEL										(1<<7)
#define SYS_SEL_ST_MASK							(0xf00)
#define DCMI_SENSOR_SEL_ST_MASK			(0x70000)
#define OSCL_SEL_ST_MASK						(0x300000)
#define OSCH_SEL_ST_MASK						(0xc00000)
#define CLKOUT_SEL_MASK							(0x3000000)
#define CLKOUT_SEL_ST_MASK					(0xf0000000)

#define SYS_SEL_SHIFT								(0)
#define DCMI_SENSOR_SEL_SHIFT				(4)
#define SYS_SEL_ST_SHIFT						(8)
#define DCMI_SENSOR_SEL_ST_SHIFT		(16)
#define OSCL_SEL_ST_SHIFT						(20)
#define OSCH_SEL_ST_SHIFT						(22)
#define CLKOUT_SEL_SHIFT						(24)
#define CLKOUT_SEL_ST_SHIFT					(28)

#define SYSCLK_SEL_OSC8M				(0x00)
#define SYSCLK_SEL_OSC160M			(0x01)
#define SYSCLK_SEL_USBPHY240M		(0x02)
#define SYSCLK_SEL_OSCEXT				(0x03)

#define CLKSEL_ST_OSC8M					(0x0100)
#define CLKSEL_ST_OSC160M				(0x0200)
#define CLKSEL_ST_USBPHY240M		(0x0400)
#define CLKSEL_ST_OSCEXT				(0x0800)


/* CPM_SRSTCR1 */
#define USBC_RST_RELEASE_BIT			(1<<8)
#define USBPHY_RST_RELEASE_BIT		(1<<13)
#define PWRCR_PHY_PSWEN_BIT				(1<<24)
#define PWRCR_PHY_RSTMASK_BIT			(1<<25)
#define PWRCR_PHY_O_ISOEN_BIT			(1<<26)
#define PWRCR_PHY_I_ISOEN_BIT			(1<<27)
#define USBPHY_PLL_SRM						(1<<28)
#define USBPHY_CFG_SRM						(1<<29)

typedef enum _SYSCLK_SEL_
{
	SYSCLK_OSC8M = 0,
	SYSCLK_OSC160M,		
	SYSCLK_USBPHY240M,
	SYSCLK_OSCEXT
}SysClkSelect;


typedef enum _DIVIDER_TYPE_
{
	SYS_DIVIDER = 0,
	TRACE_DIVIDER,
	CLKOUT_DIVIDER,
	IPS_DIVIDER,
	AHB3_DIVIDER,
	ARITH_DIVIDER,
	SDRAM_DIVIDER,
	SDRAM_SM_DIVIER,
	SDRAM2LCD_DIVIDER,
	MCC_DIVIDER,
	MCCADR_DIVIDER,
	ADC_DIVIDER,
	CLCD_DIVIDER,
	MESH_DIVIDER,
	TC_DIVIDER,
	DMA2D_SRAM_DIVIDER,
	MIPI_SAMPLE_DIVIDER,
	DCMI_PIX_DIVIDER,
	DCMI_SENSOR_DIVIDER
}CLK_DIVIDER;

typedef enum _CLK_DIVIDER_OPERATION_
{
	DIVIDER_DISABLE = 0,
	UPDATA_DIVIDER,
	GET_NOW_DIVIDER,
	GET_NOW_CLKGATE,
	CLKGATE_RESTORE
}CLK_DIV_OP;

#define CPM_VREF_TRIM_090			0x10		//1.05V 
#define CPM_VREF_TRIM_105			0x00		//1.05V 
#define CPM_VREF_TRIM_110			0x01		//1.10V
#define CPM_VREF_TRIM_115			0x02		//1.15V
#define CPM_VREF_TRIM_121			0x03		//1.21V

#define VDD_PD_RETENT    (1<<0)
#define VDD_WK_SWOFF     (1<<5)
#define	CPM_IPS_SLPEN    (1<<9)
#define WKP_AFILT_BYPASS (1<<15)
#define WKP_DFILT_GTE    (1<<28)
#define WKP_DFILT_BYPASS (1<<29)
#define WKP_DFILT_EN     (1<<30)

#define OSC108M		108*1000*1000
#define OSC120M		120*1000*1000
#define OSC150M		150*1000*1000
#define OSC160M		160*1000*1000	//必须二分频使用

#endif /* CPM_H_ */
