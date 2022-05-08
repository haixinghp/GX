/***************************************************************************************
**文件名:     YY.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
本文件使用了NRF52832的硬件SPI作为底层驱动.

**备注:      
****************************************************************************************/
#include "DRV_SPI.h"
#if  DRV_SPI_MODULE_EN
#warning DRV_SPI模块使能!
/*******************************宏定义***************************/  
#define SPI_INSTANCE                    0                       /**< SPI instance index. */
#define DRV_SPI_TX_BUF_SIZE             (256)
#define DRV_SPI_RX_BUF_SIZE             (256)
#define DRV_SPI_CS_INIT()               do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_1();}while(0)
#define DRV_SPI_CS_ENABLE()             do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_0();}while(0)
#define DRV_SPI_CS_DISABLE()            do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_1();}while(0)
/*************************.C变量结构声明***********************/  

/*************************私有全局变量*************************/  
static volatile bool spi_xfer_done;  //SPI数据传输完成标志
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static uint8_t    spi_tx_buf[DRV_SPI_TX_BUF_SIZE];   /**< TX buffer. */
static uint8_t    spi_rx_buf[DRV_SPI_RX_BUF_SIZE];   /**< RX buffer. */
/*************************公共全局变量*************************/  


/*************************局部函数*****************************/ 
/***************************************************************************************
**函数名:       DRV_SPI_WriteOneByte
**功能描述:     spi写入一个字节
**输入参数:     
uint8_t Dat
**输出参数:     --
**备注:         
****************************************************************************************/ 
static void DRV_SPI_WriteOneByte(uint8_t Dat)
{
	uint8_t len = 1;
	spi_tx_buf[0] = Dat;
	spi_xfer_done = false;
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
	while(!spi_xfer_done);
}
/***************************************************************************************
**函数名:       DRV_SPI_ReadOneByte
**功能描述:     spi读取一个字节
**输入参数:     
**输出参数:     uint8_t
**备注:         
****************************************************************************************/ 
static uint8_t DRV_SPI_ReadOneByte(void) 
{
	uint8_t len = 1;	
	spi_tx_buf[0] = 0xFF;
	spi_xfer_done = false;
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, len, spi_rx_buf, len));
	while(!spi_xfer_done);
	return (spi_rx_buf[0]);
}
/*************************全局函数****************************/  
/***************************************************************************************
**函数名:       spi_event_handler
**功能描述:     spi中断服务程序
**输入参数:     
nrf_drv_spi_evt_t const * p_event,
void *                    p_context
**输出参数:     --
**备注:         
****************************************************************************************/
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
  spi_xfer_done = true;
}
/***************************************************************************************
**函数名:       spi_init
**功能描述:     spi初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_SPI_init(void)
{
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	//    spi_config.ss_pin   = SPI_SS_PIN;     //这里使用软件控制
	spi_config.miso_pin = SPI_MISO_PIN;
	spi_config.mosi_pin = SPI_MOSI_PIN;
	spi_config.sck_pin  = SPI_SCK_PIN;
	spi_config.mode = NRF_DRV_SPI_MODE_0;
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
	DRV_SPI_CS_INIT();
}
/***************************************************************************************
**函数名:       DRV_SPI_WriteReadNByte
**功能描述:     SPI写入N个字节，并且读取N个数据
**输入参数:     
const uint8_t *WirteDat,只读，发送缓冲区，如果为NULL，则不发送
uint16_t WirteLen,	发送长度
uint8_t *ReadDat,		读取缓冲区	如果为NULL，则不接收	
uint16_t ReadLen		读取长度。
**输出参数:     0，操作成功
**备注:   
先写后读
需要注意！操作的时候，不能越界操作！不然会导致未知错误!
****************************************************************************************/
int8_t DRV_SPI_WriteReadNByte(const uint8_t *WirteDat,uint16_t WirteLen,uint8_t *ReadDat,uint16_t ReadLen)
{
	DRV_SPI_CS_ENABLE();
	if(NULL != WirteDat)
	{
		for(uint8_t i=0;i < WirteLen; i++)
		{
			DRV_SPI_WriteOneByte(*(WirteDat+i));
		}
	}
	if(NULL != ReadDat)
	{
		for(uint8_t j=0;j<ReadLen;j++)
		{
			*ReadDat =DRV_SPI_ReadOneByte();
			ReadDat++;
		}
	}
	DRV_SPI_CS_DISABLE();
	return 0;	
}
#endif  /*DRV_SPI_MODULE_EN*/
/************************************Code End******************************************/