#include "Sensor.h"
#include <string.h>

#define SENSOR_DUMMY_BYTE         0xFF
#define SENSOR_READLINE_CMD       0x70
#define SENSOR_160_READ_FRAME_REG 0x74
#define SENSOR_160_SET_LINE_CMD   0x78


void sensorInit(void)
{
	*((U32* )(0x63f04000+0x002c))|=0x40000000;
	SPI_Disable(&SPI2);

	SPI_FrameConfig_t frame_config;
	SPI_ControlConfig_t control_config;
	SPI_PinIOConfig_t pin_io_config;

	frame_config.CS_Keep_Low = 0;
	frame_config.GuardTime_Enable = 0;
	frame_config.GuardTime_PreBits = 0;
	frame_config.GuardTime_Bits = 0;
	frame_config.LoopBack_Mode = 0;
	frame_config.TI_Frame_Format = 0;
	SPI_FrameConfig(&SPI2, &frame_config);

	control_config.Stop_In_Doze = 1;
	control_config.Serial_Pin_Control = 0;
	control_config.WireOR_Mode = 0;
	control_config.MasterMode = 1;
	control_config.CPOL = 0;
	control_config.CPHA = 0;
	control_config.SSOuput_Enable = 0;
	control_config.LSB_First = 0;
	control_config.Frame_Bits = 7;
	SPI_ControlConfig(&SPI2, &control_config);

	SPI_SetBaudrate(&SPI2, 0, 2);

	pin_io_config.GPIO_Prior_Normal = 0;
	pin_io_config.Direction_Output = 1;
	SPI_PinIOConfig(&SPI2, PIN_SS, &pin_io_config);
	SPI_PinIOConfig(&SPI2, PIN_SCK, &pin_io_config);
	SPI_PinIOConfig(&SPI2, PIN_MOSI, &pin_io_config);
	pin_io_config.Direction_Output = 0;
	SPI_PinIOConfig(&SPI2, PIN_MISO, &pin_io_config);
	SPI_WritePin(&SPI2, PIN_SS, 0);

	SPI_ResetTXFIFO(&SPI2);
	SPI_ResetRXFIFO(&SPI2);

	SPI_Enable(&SPI2);

}

#if ( SPI_EDMA_ENABLE	==	(1) )//spi use edma

void sensorSetMode(U8 mode)
{
	U32 out_buf = mode, in_buf = 0;
	EDMAC_ControlConfig_t config;

	config.Transfer_Num = 1;
	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = (U8 *)(&out_buf);
	config.In_Buff = (U8 *)(&in_buf);

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 1);
	
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));
	
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 0);
}

void sensorRWReg(U8 addr, U8 w_data, U8 *old_data, U8 *new_data)
{
	U8 out_buf[4], in_buf[4];
	EDMAC_ControlConfig_t config;

	out_buf[0] = addr;
	out_buf[1] = w_data;
	out_buf[2] = SENSOR_DUMMY_BYTE;
	out_buf[3] = SENSOR_DUMMY_BYTE;
	config.Transfer_Num = 4;
	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = out_buf;
	config.In_Buff = in_buf;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 1);
	
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));
	
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 0);

	if (old_data){
		*old_data = in_buf[2];
	}
	if (new_data){
		*new_data = in_buf[3];
	}
}


void sensorReadLine(U8 *buff, U32 len)
{
	U32 dummy = SENSOR_READLINE_CMD | 0xFFFFFF00;
	EDMAC_ControlConfig_t config;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 1;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = (U8 *)(&dummy);
	config.In_Buff = buff;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 1);

	config.Transfer_Num = 15;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	config.Transfer_Num = len;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));
	
	SPI_WritePin(&SPI2, PIN_SS, 0);
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
}

void sensorReadFrame(U8 *buff, U32 len)
{
	U8 dummy = SENSOR_160_READ_FRAME_REG;
	EDMAC_ControlConfig_t config;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 1;
	config.In_Buff_Inc_Disable  = 0;
	config.Out_Buff = (U8 *)(&dummy);
	config.In_Buff = buff;

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 1);
	
	config.Transfer_Num = 15;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	config.Transfer_Num = len;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));
	
	SPI_WritePin(&SPI2, PIN_SS, 0);
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
}


void sensorSetLine(U8 *buff, U32 len)
{
	EDMAC_ControlConfig_t config;
	U8 cmd = SENSOR_160_SET_LINE_CMD, in_buf;

	config.Compare_Enable = 0;
	config.Compare_Skip = 0;
	config.Preload_Disable = 0;
	config.Transfer_Type = EDMAC_TTYPE_SRAM_PERI_TO_SRAM;
	config.Peripheral_Num = EDMAC_PN_SPI2;
	config.Out_Buff_Inc_Disable = 0;
	config.In_Buff_Inc_Disable  = 1;
	config.In_Buff = (U8*)(&in_buf);

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	SPI_WritePin(&SPI2, PIN_SS, 1);

	config.Out_Buff = &cmd;
	config.Transfer_Num = 1;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));

	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
	config.Out_Buff = buff;
	config.Transfer_Num = len;
	EDMAC_ControlConfig(&EDMA, EDMAC_CHANNEL0, &config);
	EDMAC_Enable(&EDMA, EDMAC_CHANNEL0);
	while(!EDMAC_IsDone(&EDMA, EDMAC_CHANNEL0));

	SPI_WritePin(&SPI2, PIN_SS, 0);
	EDMAC_Disable(&EDMA, EDMAC_CHANNEL0);
}

#else


inline U8 sensorRWByte(U8 data)
{
	while(!SPI_IsTXFIFO_Empty(&SPI2));
	SPI_WriteByte(&SPI2, data);
	while(SPI_IsRXFIFO_Empty(&SPI2));
	return SPI_ReadByte(&SPI2);
}


void sensorSetMode(U8 mode)
{
	SPI_WritePin(&SPI2, PIN_SS, 0);
	Sensor_RWByte(mode);
	SPI_WritePin(&SPI2, PIN_SS, 1);
}


void sensorRWReg(U8 addr, U8 w_data, U8 *old_data, U8 *new_data)
{
	U8 tmp;

	SPI_WritePin(&SPI2, PIN_SS, 0);

	while(!SPI_IsTXFIFO_Empty(&SPI2));
	SPI_WriteByte(&SPI2, addr);
	SPI_WriteByte(&SPI2, w_data);
	SPI_WriteByte(&SPI2, SENSOR_DUMMY_BYTE);
	SPI_WriteByte(&SPI2, SENSOR_DUMMY_BYTE);

	while(SPI_IsRXFIFO_Empty(&SPI2));
	tmp = SPI_ReadByte(&SPI2);

	while(SPI_IsRXFIFO_Empty(&SPI2));
	tmp = SPI_ReadByte(&SPI2);

	while(SPI_IsRXFIFO_Empty(&SPI2));
	tmp = SPI_ReadByte(&SPI2);
	if (old_data) *old_data = tmp;

	while(SPI_IsRXFIFO_Empty(&SPI2));
	tmp = SPI_ReadByte(&SPI2);
	if (new_data) *new_data = tmp;

	SPI_WritePin(&SPI2, PIN_SS, 1);
}

void sensorReadLine(U8 *buff, U32 len)
{
	SPI_WritePin(&SPI2, PIN_SS, 0);

	Sensor_RWByte(SENSOR_READLINE_CMD);
	Sensor_RWByte(SENSOR_DUMMY_BYTE);

	while(len--){
		*buff++ = Sensor_RWByte(SENSOR_DUMMY_BYTE);
	}

	SPI_WritePin(&SPI2, PIN_SS, 1);
}

void sensorReadFrame(U8 *buff, U32 len)
{
	SPI_WritePin(&SPI2, PIN_SS, 0);

	Sensor_RWByte(MA_READFRAME_CMD);
	Sensor_RWByte(SENSOR_DUMMY_BYTE);

	while(len--){
		while(!SPI_IsTXFIFO_Empty(&SPI2));
		SPI_WriteByte(&SPI2, SENSOR_DUMMY_BYTE);
		while(SPI_IsRXFIFO_Empty(&SPI2));
		*buff++ =  SPI_ReadByte(&SPI2);
	}

	SPI_WritePin(&SPI2, PIN_SS, 1);
}

#endif
