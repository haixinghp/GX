#ifndef _TSI_REG_H_
#define _TSI_REG_H_

#include "type.h"
#include "memmap.h"

#define TSI ((TSI_TypeDef *)(TSI_BASE_ADDR))

typedef struct
{
	volatile unsigned int TSI_ISR;			/*!< TSIÖÐ¶Ï×´Ì¬¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0000 */
	volatile unsigned int TSI_IER;			/*!< TSIÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0004 */
	volatile unsigned int TSI_CR;			/*!< TSI¿ØÖÆ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0008 */
	volatile unsigned int TSI_TSTR;			/*!< TSIÄ£ÄâÌØÐÔÅäÖÃ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x000c */
	volatile unsigned int TSI_WDCFGR;		/*!< TSIÄ£Äâ¿´ÃÅ¹·ÅäÖÃ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0010 */
	volatile unsigned int TSI_AWDTR;		/*!< TSIÄ£Äâ¿´ÃÅ¹·ãÐÖµ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0014 */
	volatile unsigned int TSI_CHCR;			/*!< TSIÄ£Ê½ºÍÉ¨ÃèµçÁ÷ÅäÖÃ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0018 */
	volatile unsigned int TSI_CH0DR;		/*!< TSI-CH0Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x001c */
	volatile unsigned int TSI_CH1DR;		/*!< TSI-CH1Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0020 */
	volatile unsigned int TSI_CH2DR;		/*!< TSI-CH2Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0024 */
	volatile unsigned int TSI_CH3DR;		/*!< TSI-CH3Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0028 */
	volatile unsigned int TSI_CH4DR;		/*!< TSI-CH4Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x002c */
	volatile unsigned int TSI_CH5DR;		/*!< TSI-CH5Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0030 */
	volatile unsigned int TSI_CH6DR;		/*!< TSI-CH6Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0034 */
	volatile unsigned int TSI_CH7DR;		/*!< TSI-CH7Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0038 */
	volatile unsigned int TSI_CH8DR;		/*!< TSI-CH8Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x003c */
	volatile unsigned int TSI_CH9DR;		/*!< TSI-CH9Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0040 */
	volatile unsigned int TSI_CHaDR;		/*!< TSI-CH10Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0044 */
	volatile unsigned int TSI_CHbDR;		/*!< TSI-CH11Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0048 */
	volatile unsigned int TSI_CHcDR;		/*!< TSI-CH12Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x004c */
	volatile unsigned int TSI_CHdDR;		/*!< TSI-CH13Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0050 */
	volatile unsigned int TSI_CHeDR;		/*!< TSI-CH14Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0054 */
	volatile unsigned int TSI_CHfDR;		/*!< TSI-CH15Êý¾Ý¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0058 */
	volatile unsigned int TSI_RESERVED;		/*!< TSI ±£Áô¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x005c */
	volatile unsigned int TSI_RESERVED1;	/*!< TSI ±£Áô¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0060 */
	volatile unsigned int TSI_BASE03;		/*!< TSI CH0~3 Cbase¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0064 */
	volatile unsigned int TSI_BASE47;		/*!< TSI CH4~7 Cbase¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0068 */
	volatile unsigned int TSI_BASE8b;		/*!< TSI CH8~b Cbase¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x006c */
	volatile unsigned int TSI_BASEcf;		/*!< TSI CHc~f Cbase¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0070 */
	volatile unsigned int TSI_CHEN;			/*!< TSIÍ¨µÀÊ¹ÄÜ¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0074 */
	volatile unsigned int TSI_RESERVED2;	/*!< TSI ±£Áô¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0078 */
	volatile unsigned int TSI_RESERVED3;	/*!< TSI ±£Áô¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x007c */
	volatile unsigned int TSI_DELTA03;		/*!< TSI °´¼ü»½ÐÑãÐÖµÔöÁ¿¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0080 */
	volatile unsigned int TSI_DELTA47;		/*!< TSI °´¼ü»½ÐÑãÐÖµÔöÁ¿¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0084 */
	volatile unsigned int TSI_DELTA8b;		/*!< TSI °´¼ü»½ÐÑãÐÖµÔöÁ¿¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0088 */
	volatile unsigned int TSI_DELTAcf;		/*!< TSI °´¼ü»½ÐÑãÐÖµÔöÁ¿¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x008c */
	volatile unsigned int TSI_Cid_coarse03; /*!< TSI CH0~3 »ù´¡Coarse¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0090 */
	volatile unsigned int TSI_Cid_coarse47; /*!< TSI CH4~7 »ù´¡Coarse¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0094 */
	volatile unsigned int TSI_Cid_coarse8b; /*!< TSI CH8~b »ù´¡Coarse¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x0098 */
	volatile unsigned int TSI_Cid_coarsecf; /*!< TSI CHc~f »ù´¡Coarse¼Ä´æÆ÷, Æ«ÒÆµØÖ·: 0x009c */
	volatile unsigned int TSI_DATA;			//0xc0
} TSI_TypeDef;

/******************************************************************************/
/*                                                                            */
/*                                  TSI                                       */
/*                                                                            */
/******************************************************************************/

/*****************  Bit definition for TSI_ISR register  *****************/
// TSI_ISR, TSI interrupt and status register, 0x00
#define TSI_ISR_SCANENDFINISH (uint32_t)(1U << 17) /*!< TSIËùÓÐÊ¹ÄÜÍ¨µÀÉ¨ÃèÍê³É±êÖ¾, Ð´1ÇåÁã */
#define TSI_ISR_AWD (uint32_t)(1U << 7) /*!< TSIÄ£Äâ¿´ÃÅ¹·ÖÐ¶Ï±êÖ¾Î», Ð´1ÇåÁã */
/*****************  Bit definition for TSI_IER register  *****************/
// TSI_IER, TSI interrupt and status register, 0x04
#define TSI_IER_SCANFINISH_IE (uint32_t)(1U << 30) /*!< TSIÍ¨µÀÉ¨ÃèÍê³ÉÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷ */
#define TSI_IER_ANALOGWDT_IE (uint32_t)(1U << 29)  /*!< TSIÄ£Äâ¿´ÃÅ¹·ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷ */
/*****************  Bit definition for TSI_CR register  *****************/
// TSI_CR, TSI control register, 0x08
#define TSI_CR_DEFAULT (uint32_t)(1U << 30)
#define TSI_CR_SCAN_MODE (uint32_t)(1U << 7)	 /*!< =1Í¨µÀÑ­»·É¨ÃèÄ£Ê½; =0µ¥Í¨µÀÉ¨ÃèÄ£Ê½ */
#define TSI_CR_SCAN_SEQUENCE_MODE (uint32_t)(1U << 5)	 /*!< =1 Channel switch sequence is 000011112222; =0 Channel switch sequence is 0123456.... */
#define TSI_CR_AVALID_ALWAYS (uint32_t)(1U << 3) /*!< TSI³£¿ªÆô */
#define TSI_CR_SCAN_EN (uint32_t)(1U << 0)		 /*!< Ê¹ÄÜTSI */
/*****************  Bit definition for TSI_TSTR register  *****************/
//  TSI_TSTR, TSI test register, 0x0c
#define TSI_TSTR_ANALOG (1U) /*!< TSIÄ£ÄâÏà¹ØtrimÖµ */
#define TSI_TSTR_DISCARDLEN (uint32_t)(3U << 29) /*!< ¶ªÊ§Êý¾ÝµÄ³¤¶È*/
#define TSI_TSTR_DISCARDMODE (uint32_t)(1U << 31) /*!< =1Í¨µÀÇÐ»»Ê±¶ªÆúÊý¾Ý */	
/*****************  Bit definition for TSI_CHCR register  *****************/
// TSI_CHCR, TSI channel config register, 0x18
#define TSI_CHCR_MODE_COARSE (uint32_t)(1U << 30)
#define TSI_CHCR_CURRENT_MAX 0xF0U /*!< ÉèÖÃ³ä·ÅµçËÙ¶È */
/*****************  Bit definition for TSI_CHEN register  *****************/
// TSI_CHEN, TSI Channel Enable register, 0x74
#define TSI_CHEN_SCANTIME (uint32_t)(0xFFU << 24)	/*!< TSI scan times */
#define TSI_CHEN_DISABLETIME (uint32_t)(0x00U << 16) /*!< TSI disable time = (disable len+1) * 500us */

#endif /*_TSI_REG_H_*/
