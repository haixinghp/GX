#ifndef _TSI_REG_H_
#define _TSI_REG_H_

#include "type.h"
#include "memmap.h"

#define TSI ((TSI_TypeDef *)(TSI_BASE_ADDR))

typedef struct
{
	volatile unsigned int TSI_ISR;			/*!< TSI�ж�״̬�Ĵ���, ƫ�Ƶ�ַ: 0x0000 */
	volatile unsigned int TSI_IER;			/*!< TSI�ж����üĴ���, ƫ�Ƶ�ַ: 0x0004 */
	volatile unsigned int TSI_CR;			/*!< TSI���ƼĴ���, ƫ�Ƶ�ַ: 0x0008 */
	volatile unsigned int TSI_TSTR;			/*!< TSIģ���������üĴ���, ƫ�Ƶ�ַ: 0x000c */
	volatile unsigned int TSI_WDCFGR;		/*!< TSIģ�⿴�Ź����üĴ���, ƫ�Ƶ�ַ: 0x0010 */
	volatile unsigned int TSI_AWDTR;		/*!< TSIģ�⿴�Ź���ֵ�Ĵ���, ƫ�Ƶ�ַ: 0x0014 */
	volatile unsigned int TSI_CHCR;			/*!< TSIģʽ��ɨ��������üĴ���, ƫ�Ƶ�ַ: 0x0018 */
	volatile unsigned int TSI_CH0DR;		/*!< TSI-CH0���ݼĴ���, ƫ�Ƶ�ַ: 0x001c */
	volatile unsigned int TSI_CH1DR;		/*!< TSI-CH1���ݼĴ���, ƫ�Ƶ�ַ: 0x0020 */
	volatile unsigned int TSI_CH2DR;		/*!< TSI-CH2���ݼĴ���, ƫ�Ƶ�ַ: 0x0024 */
	volatile unsigned int TSI_CH3DR;		/*!< TSI-CH3���ݼĴ���, ƫ�Ƶ�ַ: 0x0028 */
	volatile unsigned int TSI_CH4DR;		/*!< TSI-CH4���ݼĴ���, ƫ�Ƶ�ַ: 0x002c */
	volatile unsigned int TSI_CH5DR;		/*!< TSI-CH5���ݼĴ���, ƫ�Ƶ�ַ: 0x0030 */
	volatile unsigned int TSI_CH6DR;		/*!< TSI-CH6���ݼĴ���, ƫ�Ƶ�ַ: 0x0034 */
	volatile unsigned int TSI_CH7DR;		/*!< TSI-CH7���ݼĴ���, ƫ�Ƶ�ַ: 0x0038 */
	volatile unsigned int TSI_CH8DR;		/*!< TSI-CH8���ݼĴ���, ƫ�Ƶ�ַ: 0x003c */
	volatile unsigned int TSI_CH9DR;		/*!< TSI-CH9���ݼĴ���, ƫ�Ƶ�ַ: 0x0040 */
	volatile unsigned int TSI_CHaDR;		/*!< TSI-CH10���ݼĴ���, ƫ�Ƶ�ַ: 0x0044 */
	volatile unsigned int TSI_CHbDR;		/*!< TSI-CH11���ݼĴ���, ƫ�Ƶ�ַ: 0x0048 */
	volatile unsigned int TSI_CHcDR;		/*!< TSI-CH12���ݼĴ���, ƫ�Ƶ�ַ: 0x004c */
	volatile unsigned int TSI_CHdDR;		/*!< TSI-CH13���ݼĴ���, ƫ�Ƶ�ַ: 0x0050 */
	volatile unsigned int TSI_CHeDR;		/*!< TSI-CH14���ݼĴ���, ƫ�Ƶ�ַ: 0x0054 */
	volatile unsigned int TSI_CHfDR;		/*!< TSI-CH15���ݼĴ���, ƫ�Ƶ�ַ: 0x0058 */
	volatile unsigned int TSI_RESERVED;		/*!< TSI �����Ĵ���, ƫ�Ƶ�ַ: 0x005c */
	volatile unsigned int TSI_RESERVED1;	/*!< TSI �����Ĵ���, ƫ�Ƶ�ַ: 0x0060 */
	volatile unsigned int TSI_BASE03;		/*!< TSI CH0~3 Cbase�Ĵ���, ƫ�Ƶ�ַ: 0x0064 */
	volatile unsigned int TSI_BASE47;		/*!< TSI CH4~7 Cbase�Ĵ���, ƫ�Ƶ�ַ: 0x0068 */
	volatile unsigned int TSI_BASE8b;		/*!< TSI CH8~b Cbase�Ĵ���, ƫ�Ƶ�ַ: 0x006c */
	volatile unsigned int TSI_BASEcf;		/*!< TSI CHc~f Cbase�Ĵ���, ƫ�Ƶ�ַ: 0x0070 */
	volatile unsigned int TSI_CHEN;			/*!< TSIͨ��ʹ�ܼĴ���, ƫ�Ƶ�ַ: 0x0074 */
	volatile unsigned int TSI_RESERVED2;	/*!< TSI �����Ĵ���, ƫ�Ƶ�ַ: 0x0078 */
	volatile unsigned int TSI_RESERVED3;	/*!< TSI �����Ĵ���, ƫ�Ƶ�ַ: 0x007c */
	volatile unsigned int TSI_DELTA03;		/*!< TSI ����������ֵ�����Ĵ���, ƫ�Ƶ�ַ: 0x0080 */
	volatile unsigned int TSI_DELTA47;		/*!< TSI ����������ֵ�����Ĵ���, ƫ�Ƶ�ַ: 0x0084 */
	volatile unsigned int TSI_DELTA8b;		/*!< TSI ����������ֵ�����Ĵ���, ƫ�Ƶ�ַ: 0x0088 */
	volatile unsigned int TSI_DELTAcf;		/*!< TSI ����������ֵ�����Ĵ���, ƫ�Ƶ�ַ: 0x008c */
	volatile unsigned int TSI_Cid_coarse03; /*!< TSI CH0~3 ����Coarse�Ĵ���, ƫ�Ƶ�ַ: 0x0090 */
	volatile unsigned int TSI_Cid_coarse47; /*!< TSI CH4~7 ����Coarse�Ĵ���, ƫ�Ƶ�ַ: 0x0094 */
	volatile unsigned int TSI_Cid_coarse8b; /*!< TSI CH8~b ����Coarse�Ĵ���, ƫ�Ƶ�ַ: 0x0098 */
	volatile unsigned int TSI_Cid_coarsecf; /*!< TSI CHc~f ����Coarse�Ĵ���, ƫ�Ƶ�ַ: 0x009c */
	volatile unsigned int TSI_DATA;			//0xc0
} TSI_TypeDef;

/******************************************************************************/
/*                                                                            */
/*                                  TSI                                       */
/*                                                                            */
/******************************************************************************/

/*****************  Bit definition for TSI_ISR register  *****************/
// TSI_ISR, TSI interrupt and status register, 0x00
#define TSI_ISR_SCANENDFINISH (uint32_t)(1U << 17) /*!< TSI����ʹ��ͨ��ɨ����ɱ�־, д1���� */
#define TSI_ISR_AWD (uint32_t)(1U << 7) /*!< TSIģ�⿴�Ź��жϱ�־λ, д1���� */
/*****************  Bit definition for TSI_IER register  *****************/
// TSI_IER, TSI interrupt and status register, 0x04
#define TSI_IER_SCANFINISH_IE (uint32_t)(1U << 30) /*!< TSIͨ��ɨ������ж�ʹ�ܼĴ��� */
#define TSI_IER_ANALOGWDT_IE (uint32_t)(1U << 29)  /*!< TSIģ�⿴�Ź��ж�ʹ�ܼĴ��� */
/*****************  Bit definition for TSI_CR register  *****************/
// TSI_CR, TSI control register, 0x08
#define TSI_CR_DEFAULT (uint32_t)(1U << 30)
#define TSI_CR_SCAN_MODE (uint32_t)(1U << 7)	 /*!< =1ͨ��ѭ��ɨ��ģʽ; =0��ͨ��ɨ��ģʽ */
#define TSI_CR_SCAN_SEQUENCE_MODE (uint32_t)(1U << 5)	 /*!< =1 Channel switch sequence is 000011112222; =0 Channel switch sequence is 0123456.... */
#define TSI_CR_AVALID_ALWAYS (uint32_t)(1U << 3) /*!< TSI������ */
#define TSI_CR_SCAN_EN (uint32_t)(1U << 0)		 /*!< ʹ��TSI */
/*****************  Bit definition for TSI_TSTR register  *****************/
//  TSI_TSTR, TSI test register, 0x0c
#define TSI_TSTR_ANALOG (1U) /*!< TSIģ�����trimֵ */
#define TSI_TSTR_DISCARDLEN (uint32_t)(3U << 29) /*!< ��ʧ���ݵĳ���*/
#define TSI_TSTR_DISCARDMODE (uint32_t)(1U << 31) /*!< =1ͨ���л�ʱ�������� */	
/*****************  Bit definition for TSI_CHCR register  *****************/
// TSI_CHCR, TSI channel config register, 0x18
#define TSI_CHCR_MODE_COARSE (uint32_t)(1U << 30)
#define TSI_CHCR_CURRENT_MAX 0xF0U /*!< ���ó�ŵ��ٶ� */
/*****************  Bit definition for TSI_CHEN register  *****************/
// TSI_CHEN, TSI Channel Enable register, 0x74
#define TSI_CHEN_SCANTIME (uint32_t)(0xFFU << 24)	/*!< TSI scan times */
#define TSI_CHEN_DISABLETIME (uint32_t)(0x00U << 16) /*!< TSI disable time = (disable len+1) * 500us */

#endif /*_TSI_REG_H_*/
