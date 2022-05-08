// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : dmac_drv.c
// Version      : V0.1
//NOTE          :MCC使用了DMAC1的中断，因此在使用MCC的情况下只能使用DMAC1的查询方式。对于DMAC2不存在该问题
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "dma.h"
#include "spi.h"
#include "ssi.h"
#include "debug.h"
//#include "syscfg.h"



//DMA_CHANNEL_REG *m_dma_channel[4];//global struct variable for for Channel registers
//DMA_CONTROL_REG *m_dma_control;//global struct variable for for DMAC registers


static DMA_LLI g_dma_lli_rx;
static void dma_reg_init(UINT32 dma_base_addr)
{
	UINT8 n = 0;
	UINT32 ch_addr[4] = {0x0000, 0x0058, 0x00b0, 0x0108};

	m_dma_control = (DMA_CONTROL_REG*)(dma_base_addr+0x2c0);

	for(n =0; n<4; n++)
	{
		m_dma_channel[n] = (DMA_CHANNEL_REG*)(dma_base_addr+ch_addr[n]);
	}
}


