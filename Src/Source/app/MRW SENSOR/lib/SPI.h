/*
 * SPI.h
 *
 *  Created on: 2016Äê9ÔÂ14ÈÕ
 *      
 */

#ifndef SPI_H_
#define SPI_H_
#include "CCM_Types.h"

typedef struct {
    VU8  BR;            //SPI Baud Rate Register
    VU8  FR;            //SPI Frame Register
    VU8  CR1;           //SPI Control Register 1
    VU8  CR2;           //SPI Control Register 2
    VU8  RXFTOCTR;      //SPI RX FIFO Timeout Counter Register
    VU8  TXFTOCTR;      //SPI TX FIFO Timeout Counter Register
    VU8  RXFCR;         //SPI RX FIFO Control Register
    VU8  TXFCR;         //SPI TX FIFO Control Register
    VU8  ASCDR;         //SPI After SCK Delay Register
    VU8  BSCDR;         //SPI Before SCK Delay Register
    VU8  DDR;           //SPI Port Data Direction Register
    VU8  PURD;          //SPI Pullup and Reduced Drive Register
    VU8  TCNT_M;        //SPI Transmit Counter Register Mid
    VU8  TCNT_H;        //SPI Transmit Counter Register High
    VU8  PORT;          //SPI Port Data Register
    VU8  TCNT_L;        //SPI Transmit Counter Register Low
    VU8  IRSP;          //Interrupt Register of SS Pin
    VU8  Reserved_11;
    VU8  DR_L;          //SPI Data Register Low
    VU8  DR_H;          //SPI Data Register High
    VU8  RXFSR;         //SPI RX FIFO Status Register
    VU8  TXFSR;         //SPI TX FIFO Status Register
    VU8  SR_L;          //SPI Status Register Low
    VU8  SR_H;          //SPI Status Register High
    VU8  FDCR;          //SPI FIFO Debug Control Register
    VU8  ICR;           //SPI Interrupt Control Register
    VU8  DMACR;         //SPI DMA Control Register
    VU8  DMATHR;        //SPI DMA Threshold Register
    VU8  RXFDBGR;       //SPI RX FIFO Debug Register
    VU8  Reserved_1D;
    VU8  TXFDBGR;       //SPI TX FIFO Debug Register
}SPI_t;


typedef struct{
    U8 Stop_In_Doze;
    U8 Serial_Pin_Control;
    U8 WireOR_Mode;
    U8 MasterMode;
    U8 CPOL;
    U8 CPHA;
    U8 SSOuput_Enable;
    U8 LSB_First;
    U8 Frame_Bits;
}SPI_ControlConfig_t;

typedef struct{
    U8 CS_Keep_Low;
    U8 GuardTime_Enable;
    U8 GuardTime_PreBits;
    U8 GuardTime_Bits;
    U8 LoopBack_Mode;
    U8 TI_Frame_Format;
}SPI_FrameConfig_t;

typedef struct{
    U8 TX_DMA_Enable;
    U8 TX_DMA_Threshold;
    U8 RX_DMA_Enable;
    U8 RX_DMA_Threshold;
}SPI_DMAConfig_t;

typedef struct{
    U8 HighSpeedMode_Enable;
    U8 PinSwitch_Enable;
    U8 SamplePointDelay;
    U8 Polldown_Enable;
    U8 Pollup_Enable;
}SPI_PinConfig_t;

typedef enum{
    PIN_MISO  = 0,
    PIN_MOSI  = 1,
    PIN_SCK   = 2,
    PIN_SS    = 3,
}SPI_Pin_t;

typedef struct{
    U8 GPIO_Prior_Normal;
    U8 Direction_Output;
}SPI_PinIOConfig_t;

typedef enum{
    SPI_MODE_FAULT,
    SPI_END_OF_TRANS,
    SPI_FRAME_LOST,
}SPI_Interrupt_t;

typedef enum{
    SS_HIGH_LEVEL_TRIGGER   = 0x08,
    SS_LOW_LEVEL_TRIGGER    = 0x00,
    SS_RISING_EDGE_TRIGGER  = 0x01,
    SS_FALLING_EDGE_TRIGGER = 0x02,
    SS_BOTH_EDGE_TRIGGER    = 0x03,
}SPI_SSPinTrigger_t;

typedef struct{
    U8 Timeout_Enable;
    U8 Timeout_Count;
    U8 Service_Threshold;
}SPI_FIFOConfig_t;

typedef enum{
    TIMEOUT_INTERRUPT,
    OVERFLOW_INTERRUPT,
    UNDERFLOW_INTERRUPT,
    THRESHOLD_INTERRUPT,
}SPI_FIFO_Interrupt_t;

typedef struct{
    U8 Before_SCK_Delay_Enable;
    U8 Pre_Before_SCK_Delay_Bits;
    U8 Before_SCK_Delay_Bits;//Before_SCK_Delay = (Pre_Before_SCK_Delay_Bits + 1) * 2 ^ (Before_SCK_Delay_Bits + 1)
    U8 After_SCK_Delay_Enable;
    U8 Pre_After_SCK_Delay_Bits;
    U8 After_SCK_Delay_Bits;//After_SCK_Delay = (Pre_After_SCK_Delay_Bits + 1) * 2 ^ (After_SCK_Delay_Bits + 1)
}SPI_SCKDelayConfig_t;



void SPI_ControlConfig(SPI_t *spi, SPI_ControlConfig_t *config);
void SPI_FrameConfig(SPI_t *spi, SPI_FrameConfig_t *config);
void SPI_SetTransCount(SPI_t *spi, U32 count);
void SPI_DMAConfig(SPI_t *spi, SPI_DMAConfig_t *config);
void SPI_SetBaudrate(SPI_t *spi, U8 sppr, U8 spr);
U16 SPI_ReadData(SPI_t *spi);
void SPI_WriteData(SPI_t *spi, U16 data);
U8 SPI_ReadByte(SPI_t *spi);
void SPI_WriteByte(SPI_t *spi, U8 data);
void SPI_Enable(SPI_t *spi);
void SPI_Disable(SPI_t *spi);

void SPI_PinConfig(SPI_t *spi, SPI_PinConfig_t *config);
void SPI_PinIOConfig(SPI_t *spi, SPI_Pin_t pin, SPI_PinIOConfig_t *config);
void SPI_WritePin(SPI_t *spi, SPI_Pin_t pin, U8 level);
U8 SPI_ReadPin(SPI_t *spi, SPI_Pin_t pin);

void SPI_EnableInterrupt(SPI_t *spi, SPI_Interrupt_t intp);
void SPI_DisableInterrupt(SPI_t *spi, SPI_Interrupt_t intp);
U8 SPI_QueryInterrupt(SPI_t *spi, SPI_Interrupt_t intp);
void SPI_ClearInterrupt(SPI_t *spi, SPI_Interrupt_t intp);
U8 SPI_QuerySPIFinish(SPI_t *spi);

void SPI_SetSSPinTrigger(SPI_t *spi, SPI_SSPinTrigger_t trigger);
void SPI_EnableSSPinInterrupt(SPI_t *spi);
void SPI_DisableSSPinInterrupt(SPI_t *spi);
U8 SPI_QuerySSPinInterrupt(SPI_t *spi);
U8 SPI_ReadSSPinLevel(SPI_t *spi);

U8 SPI_GetTXFIFOPos(SPI_t *spi);
U8 SPI_GetRXFIFOPos(SPI_t *spi);
U8 SPI_GetTXFIFOCount(SPI_t *spi);
U8 SPI_GetRXFIFOCount(SPI_t *spi);
U8 SPI_DebugReadTXFIFO(SPI_t *spi, U8 pos);
U8 SPI_DebugReadRXFIFO(SPI_t *spi, U8 pos);

void SPI_TXFIFOConfig(SPI_t *spi, SPI_FIFOConfig_t *config);
U8 SPI_QueryTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_EnableTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_DisableTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_ResetTXFIFO(SPI_t *spi);
U8 SPI_IsTXFIFO_Full(SPI_t *spi);
U8 SPI_IsTXFIFO_Empty(SPI_t *spi);

void SPI_RXFIFOConfig(SPI_t *spi, SPI_FIFOConfig_t *config);
U8 SPI_QueryRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_EnableRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_DisableRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp);
void SPI_ResetRXFIFO(SPI_t *spi);
U8 SPI_IsRXFIFO_Full(SPI_t *spi);
U8 SPI_IsRXFIFO_Empty(SPI_t *spi);

void SPI_SCKDelayConfig(SPI_t *spi, SPI_SCKDelayConfig_t *config);

#endif /* SPI_H_ */
