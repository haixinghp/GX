#include "SensorSpi.h"
#include <string.h>
#include "ccm_mem.h"
#include "CCM_MEM.h"
#define SENSOR_INTERFACE_SPI   (SPI2)


void sensorSpiInit(void)
{
	//*((U32* )(0x63f04000+0x002c))|=0x40000000;
	SPI_Disable(&SENSOR_INTERFACE_SPI);

	SPI_FrameConfig_t frame_config;
	SPI_ControlConfig_t control_config;
	SPI_PinIOConfig_t pin_io_config;

	frame_config.CS_Keep_Low = 0;
	frame_config.GuardTime_Enable = 0;
	frame_config.GuardTime_PreBits = 0;
	frame_config.GuardTime_Bits = 0;
	frame_config.LoopBack_Mode = 0;
	frame_config.TI_Frame_Format = 0;
	SPI_FrameConfig(&SENSOR_INTERFACE_SPI, &frame_config);

	control_config.Stop_In_Doze = 1;
	control_config.Serial_Pin_Control = 0;
	control_config.WireOR_Mode = 0;
	control_config.MasterMode = 1;
	control_config.CPOL = 0;
	control_config.CPHA = 0;
	control_config.SSOuput_Enable = 0;
	control_config.LSB_First = 0;
	control_config.Frame_Bits = 7;
	SPI_ControlConfig(&SENSOR_INTERFACE_SPI, &control_config);

	SPI_SetBaudrate(&SENSOR_INTERFACE_SPI, 0, 2);

	pin_io_config.GPIO_Prior_Normal = 0;
	pin_io_config.Direction_Output = 1;

	SPI_PinIOConfig(&SENSOR_INTERFACE_SPI, PIN_SS, &pin_io_config);
	SPI_PinIOConfig(&SENSOR_INTERFACE_SPI, PIN_SCK, &pin_io_config);
	SPI_PinIOConfig(&SENSOR_INTERFACE_SPI, PIN_MOSI, &pin_io_config);
	pin_io_config.Direction_Output = 0;
	SPI_PinIOConfig(&SENSOR_INTERFACE_SPI, PIN_MISO, &pin_io_config);
	SPI_WritePin(&SENSOR_INTERFACE_SPI, PIN_SS, SENSOR_SS_DISABLE);

	SPI_ResetTXFIFO(&SENSOR_INTERFACE_SPI);
	SPI_ResetRXFIFO(&SENSOR_INTERFACE_SPI);

	SPI_Enable(&SENSOR_INTERFACE_SPI);

}
extern UINT8 test_spi;
void sensorSpiWriteRead(const void* cpWriteBuf, int nWriteSize, void* pReadBuf, int nReadSize)
{
	
	EDMAC_ControlConfig_t config;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = (U8 *)cpWriteBuf;
	config.In_Buff = (U8 *)pReadBuf;
	config.Transfer_Num = nWriteSize;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SENSOR_INTERFACE_SPI, PIN_SS, SENSOR_SS_ENABLE);

	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);

	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));
//  if(test_spi==1)
//	{
	SPI_WritePin(&SENSOR_INTERFACE_SPI, PIN_SS, SENSOR_SS_DISABLE);
//	}
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);

}

