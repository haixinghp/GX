/***************************************************************************************
**�ļ���:     YY.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
���ļ�ʹ����NRF52832��Ӳ��SPI��Ϊ�ײ�����.

**��ע:      
****************************************************************************************/
#include "DRV_SPI.h"
#if  DRV_SPI_MODULE_EN
#warning DRV_SPIģ��ʹ��!
/*******************************�궨��***************************/  
#define SPI_INSTANCE                    0                       /**< SPI instance index. */
#define DRV_SPI_TX_BUF_SIZE             (256)
#define DRV_SPI_RX_BUF_SIZE             (256)
#define DRV_SPI_CS_INIT()               do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_1();}while(0)
#define DRV_SPI_CS_ENABLE()             do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_0();}while(0)
#define DRV_SPI_CS_DISABLE()            do{M_SPI_SOFT_SS_OUT_PP(); M_SPI_SOFT_SS_OUT_1();}while(0)
/*************************.C�����ṹ����***********************/  

/*************************˽��ȫ�ֱ���*************************/  
static volatile bool spi_xfer_done;  //SPI���ݴ�����ɱ�־
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static uint8_t    spi_tx_buf[DRV_SPI_TX_BUF_SIZE];   /**< TX buffer. */
static uint8_t    spi_rx_buf[DRV_SPI_RX_BUF_SIZE];   /**< RX buffer. */
/*************************����ȫ�ֱ���*************************/  


/*************************�ֲ�����*****************************/ 
/***************************************************************************************
**������:       DRV_SPI_WriteOneByte
**��������:     spiд��һ���ֽ�
**�������:     
uint8_t Dat
**�������:     --
**��ע:         
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
**������:       DRV_SPI_ReadOneByte
**��������:     spi��ȡһ���ֽ�
**�������:     
**�������:     uint8_t
**��ע:         
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
/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       spi_event_handler
**��������:     spi�жϷ������
**�������:     
nrf_drv_spi_evt_t const * p_event,
void *                    p_context
**�������:     --
**��ע:         
****************************************************************************************/
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
  spi_xfer_done = true;
}
/***************************************************************************************
**������:       spi_init
**��������:     spi��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_SPI_init(void)
{
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	//    spi_config.ss_pin   = SPI_SS_PIN;     //����ʹ���������
	spi_config.miso_pin = SPI_MISO_PIN;
	spi_config.mosi_pin = SPI_MOSI_PIN;
	spi_config.sck_pin  = SPI_SCK_PIN;
	spi_config.mode = NRF_DRV_SPI_MODE_0;
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
	DRV_SPI_CS_INIT();
}
/***************************************************************************************
**������:       DRV_SPI_WriteReadNByte
**��������:     SPIд��N���ֽڣ����Ҷ�ȡN������
**�������:     
const uint8_t *WirteDat,ֻ�������ͻ����������ΪNULL���򲻷���
uint16_t WirteLen,	���ͳ���
uint8_t *ReadDat,		��ȡ������	���ΪNULL���򲻽���	
uint16_t ReadLen		��ȡ���ȡ�
**�������:     0�������ɹ�
**��ע:   
��д���
��Ҫע�⣡������ʱ�򣬲���Խ���������Ȼ�ᵼ��δ֪����!
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