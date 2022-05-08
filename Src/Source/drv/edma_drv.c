// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : edma_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "debug.h"
#include "cpm_drv.h"
#include "edma_drv.h"
#include "spi_drv.h"


volatile UINT8 g_Edmac0ChMinDoneFlag = 0;
volatile UINT8 g_Edmac1ChMinDoneFlag = 0;
/*******************************************************************************
* Function Name  : EDMA_ISR
* Description    : EDMA中断处理
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void ENCR_IRQHandler(void)
{
	if((EDMA_CH0->EDMACCSR)&(EDMAC_MINORDONE_MASK))
	{
		g_Edmac0ChMinDoneFlag = 1;
		EDMA_CH0->EDMACCSR |= EDMAC_MINORDONE_MASK;
//		printf("EDMA-CH0-INT\r\n");
	}
	if((EDMA_CH1->EDMACCSR)&(EDMAC_MINORDONE_MASK))
	{
		g_Edmac1ChMinDoneFlag = 1;
		EDMA_CH1->EDMACCSR |= EDMAC_MINORDONE_MASK;
//		printf("EDMA-CH1-INT\r\n");
	}
}

/*******************************************************************************
* Function Name  : EDMA_CHxTran
* Description    : EDMA 传输处理
* Input          :  - EDMA_CHx： where x can be 0 to 1 to select the EDMA peripheral；
*                   - sendAddr：EDMAC Read Buffer Address
*                   - recvAddr：EDMAC Write Buffer Address
*                   - ttpye:    Channel Transfer Type 取值：EDMAC_TTYPE0~EDMAC_TTYPE3
*                   - pn：    Channel Periperal Number
*                   - len： Channel Minor Transfer Data Sum(in byte)
*
* Output         : None
* Return         : STATUS
******************************************************************************/
UINT16 EDMA_CHx(EDMA_TypeDef *EDMA_CHx, UINT32 sendAddr,  UINT32 recvAddr, UINT32 ttpye, UINT32 pn, UINT32 len)
{
	UINT32 timeout = SPI_TIMEOUT_MS(1000);
	UINT16 ret = STATUS_OK;

	/* Check the parameters */
	assert_param(IS_EDMA_ALL_PERIPH(EDMA_CHx));//
	assert_param(IS_EDMA_TTYPE(ttpye));
	assert_param(IS_EDMA_EDMA_PN(pn));

//	printf("dma_sendAddr = %08x\r\n", sendAddr);
//	printf("dma_recvAddr = %08x\r\n", recvAddr);

	EDMA_CHx->EDMACCR = 0;
	EDMA_CHx->EDMACCR = ttpye|pn|EDMAC_CHVALID_MASK|0x04;

#ifdef EDMA_INT
	EDMA_CHx->EDMACCR |=  EDMAC_MINDONEIE_MASK;//enable edmac interrupt
#endif
	EDMA_CHx->EDMACRBAR = sendAddr;//(UINT32)(sendAddr&0x3FFFF);//send
	EDMA_CHx->EDMACWBAR = recvAddr;//(UINT32)(recvAddr&0x3FFFF);//recv
	EDMA_CHx->EDMACMINSUMR = len;
	EDMA_CHx->EDMACMAJSUMR = 0;

	EDMA_CHx->EDMACCSR |= EDMAC_EDMAEN_MASK;//enable edmac transfer
#ifdef EDMA_INT
	if (EDMA_CH0 == EDMA_CHx)
	{
		while(g_Edmac0ChMinDoneFlag == 0)
		{
			if(0 == timeout)
			{
				ret = STATUS_TIMEOUT;
			}
			timeout--;
		}
		g_Edmac0ChMinDoneFlag = 0;
	}
	else
	{
		while(g_Edmac1ChMinDoneFlag == 0)
		{
			if(0 == timeout)
			{
				ret = STATUS_TIMEOUT;
			}
			timeout--;
		}
		g_Edmac1ChMinDoneFlag = 0;
	}
#else
	while(((EDMA_CHx->EDMACCSR)&(EDMAC_MINORDONE_MASK))==0)// channelx done
	{
		if(0 == timeout)
		{
			ret = STATUS_TIMEOUT;
            break;
		}
		timeout--;
	}
//	EDMA_CHx->EDMACCSR |= EDMAC_MINORDONE_MASK; // clear done
#endif

	EDMA_CHx->EDMACCSR |= EDMAC_MINORDONE_MASK|EDMAC_MAJORDONE_MASK;
	EDMA_CHx->EDMACCSR &= ~EDMAC_EDMAEN_MASK;
    
    return ret;
}

/*******************************************************************************
* Function Name  : EDMA_CHx_SAMEDATA
* Description    : EDMA 传输固定数值
* Input          :  - EDMA_CHx： where x can be 0 to 1 to select the EDMA peripheral；
*                   - sendAddr：EDMAC Read Buffer Address
*                   - recvAddr：EDMAC Write Buffer Address
*                   - ttpye:    Channel Transfer Type 取值：EDMAC_TTYPE0~EDMAC_TTYPE3
*                   - pn：    Channel Periperal Number
*                   - len： Channel Minor Transfer Data Sum(in byte)
*
* Output         : None
* Return         : None
******************************************************************************/
void EDMA_CHx_SAMEDATA(EDMA_TypeDef *EDMA_CHx, UINT32 sendAddr,  UINT32 recvAddr, UINT32 ttpye, UINT32 pn, UINT32 len)
{
	/* Check the parameters */
	assert_param(IS_EDMA_ALL_PERIPH(EDMA_CHx));//
	assert_param(IS_EDMA_TTYPE(ttpye));
	assert_param(IS_EDMA_EDMA_PN(pn));

	EDMA_CHx->EDMACCR = 0;
	EDMA_CHx->EDMACCR = ttpye|pn|EDMAC_CHVALID_MASK;
#ifdef EDMA_INT
	EDMA_CHx->EDMACCR |=  EDMAC_MINDONEIE_MASK;//enable edmac interrupt
#endif
	EDMA_CHx->EDMACRBAR = sendAddr;//(UINT32)(sendAddr&0x3FFFF);//send
	EDMA_CHx->EDMACRBAR |= 0x80000000;//地址不增加
	EDMA_CHx->EDMACWBAR = recvAddr;//(UINT32)(recvAddr&0x3FFFF);//recv
	EDMA_CHx->EDMACMINSUMR = len;
	EDMA_CHx->EDMACMAJSUMR = 0;

	EDMA_CHx->EDMACCSR |= EDMAC_EDMAEN_MASK;//enable edmac transfer
#ifdef EDMA_INT
	if (EDMA_CH0 == EDMA_CHx)
	{
		while(g_Edmac0ChMinDoneFlag == 0)
		{
			;
		}
		g_Edmac0ChMinDoneFlag = 0;
	}
	else
	{
		while(g_Edmac1ChMinDoneFlag == 0)
		{
			;
		}
		g_Edmac1ChMinDoneFlag = 0;
	}
#else
	while(((EDMA_CHx->EDMACCSR)&(EDMAC_MINORDONE_MASK))==0)// channelx done
	{
		;
	}
	EDMA_CHx->EDMACCSR |= EDMAC_MINORDONE_MASK; // clear done
#endif

	EDMA_CHx->EDMACCSR |= EDMAC_MINORDONE_MASK|EDMAC_MAJORDONE_MASK;
	EDMA_CHx->EDMACCSR &= ~EDMAC_EDMAEN_MASK;
}

/*******************************************************************************
* Function Name  : EDMA_Init
* Description    : EDMA初始化
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void EDMA_Init(void)
{
	ModuleClk_On(MODULE_CLK_EDMAC0);
#ifdef EDMA_INT
	NVIC_Init(3, 3, ENCR_IRQn, 2);
#endif
}

void set_edmac_new (unsigned int _EDMACCR,
				unsigned int _EDMACRBAR,
				unsigned int _EDMACWBAR,
				unsigned int _EDMACMINSUM,
				unsigned int _EDMACMAJSUM,
				unsigned short _EDMACSPAR,
				unsigned int channel_index,
				unsigned int edmac_index) 
{
	if (edmac_index == 0) {
		if (channel_index == 1) {
			EDMACCR1 = _EDMACCR;
			EDMACRBAR1 = _EDMACRBAR;
			EDMACWBAR1 = _EDMACWBAR;
			EDMACSPAR1 = _EDMACSPAR;
			EDMACMINSUMR1 = _EDMACMINSUM;
			EDMACMAJSUMR1 = _EDMACMAJSUM;
		}
		if (channel_index == 0) {
			EDMACCR0 = _EDMACCR;
			EDMACRBAR0 = _EDMACRBAR;
			EDMACWBAR0 = _EDMACWBAR;
			EDMACSPAR0 = _EDMACSPAR;
			EDMACMINSUMR0 = _EDMACMINSUM;
			EDMACMAJSUMR0 = _EDMACMAJSUM;
		}
	}
	if (edmac_index == 1) {
		if (channel_index == 1) {
//			ENCR1CR1 = _EDMACCR;
//			ENCR1RBAR1 = _EDMACRBAR;
//			ENCR1WBAR1 = _EDMACWBAR;
//			ENCR1SPAR1 = _EDMACSPAR;
//			ENCR1MINSUMR1 = _EDMACMINSUM;
//			ENCR1MAJSUMR1 = _EDMACMAJSUM;
		}
		if (channel_index == 0) {
//			ENCR1CR0 = _EDMACCR;
//			ENCR1RBAR0 = _EDMACRBAR;
//			ENCR1WBAR0 = _EDMACWBAR;
//			ENCR1SPAR0 = _EDMACSPAR;
//			ENCR1MINSUMR0 = _EDMACMINSUM;
//			ENCR1MAJSUMR0 = _EDMACMAJSUM;
		}
	}
}

/***************************************
 * edmac_enable  选择edma通道并使能
 ****************************************/
inline void edmac_enable (int channel_index, int edmac_index) 
{
	if (edmac_index == 0) {
		if (channel_index == 0) {
			EDMACCSR0 |= 0x00010000;
		} else {
			EDMACCSR1 |= 0x00010000;
		}
	}
	if (edmac_index == 1) {
//		if (channel_index == 0) {
//			ENCR1CSR0 |= 0x00010000;
//		} else {
//			ENCR1CSR1 |= 0x00010000;
//		}
	}
}

/***************************************
 * edmac_disable  选择edma通道并关闭
 ****************************************/
inline void edmac_disable (int channel_index, int edmac_index) 
{
	if (edmac_index == 0) {
		if (channel_index == 0) {
			EDMACCSR0 &= ~(0x00010000);
		} else {
			EDMACCSR1 &= ~(0x00010000);
		}
	}
//	if (edmac_index == 1) {
//		if (channel_index == 0) {
//			ENCR1CSR0 &= ~(0x00010000);
//		} else {
//			ENCR1CSR1 &= ~(0x00010000);
//		}
//	}
}


/***************************************
 * edmac_check_majdone  ,Majdone是否完成
 ****************************************/
inline void edmac_check_majdone (int channel_index, int edmac_index) 
{
	if (edmac_index == 0) {
		if (channel_index == 0) {
			while ((EDMACCSR0 & 0x00000008) == 0);
		}
		if (channel_index == 1) {
			while ((EDMACCSR1 & 0x00000008) == 0);
		}
	}
//	if (edmac_index == 1) {
//		if (channel_index == 0) {
//			while ((ENCR1CSR0 & 0x00000008) == 0);
//		}
//		if (channel_index == 1) {
//			while ((ENCR1CSR1 & 0x00000008) == 0);
//		}
//	}
}
/***************************************
 * EDMAC_2ALG  ,对称运算edma调用
 ****************************************/
void EDMAC_2ALG(unsigned int pn, unsigned int src,  unsigned int dst, unsigned int len)
{
	U32 type;

	if(pn == PN10)     //SHA
		type = TTYPE2;
	else               //other
		type = TTYPE3;  //sram to peripheral then peripheral to sram

	EDMACCR0 = 0;
	EDMACCSR0 = 0;

	EDMACLASTMINSUMR0 = 0;
	EDMACRBARSTEP0 = 0x00000000;
	EDMACWBARSTEP0 = 0x00000000;

	if(len > 7){
		set_edmac_new (type|pn|PRELOAD_6|CHANNEL_VALID|(EDMA1_CH0_DONE<<17), (UINT32)src, (UINT32)dst, len, 0, 0, 0, 0);
	}else{
		set_edmac_new (type|pn|((len-1)<<2)|CHANNEL_VALID|(EDMA1_CH0_DONE<<17), (UINT32)src, (UINT32)dst, len, 0, 0, 0, 0);
	}

	edmac_enable(0, 0);
	edmac_check_majdone(0, 0);
	EDMACCSR0 |= 0x0000000a;
	EDMACCSR0 = 0;
}