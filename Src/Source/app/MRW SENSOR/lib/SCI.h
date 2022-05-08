/*
 * SCI.h
 *
 *  Created on: 2016Äê9ÔÂ14ÈÕ
 *      
 */

#ifndef SCI_H_
#define SCI_H_
#include "CCM_Types.h"

//SCI Registers
typedef struct{
	VU8 BRDIL;          //SCI Integer Baud-Rate Divisor Register Low
	VU8 BRDIH;          //SCI Integer Baud-Rate Divisor Register High
	VU8 CR2;            //SCI Control Register2
	VU8 CR1;            //SCI Control Register1
	VU8 SR2;            //SCI Status Register2
	VU8 SR1;            //SCI Status Register1
	VU8 DRL;            //SCI Data Register Low
	VU8 DRH;            //SCI Data Register High
	VU8 PORT;           //SCI Port Data Register
	VU8 PURD;           //SCI Pullup and Reduced Driver Register
	VU8 BRDF;           //SCI Fractional Baud-Rate Divisor Register
	VU8 DDR;            //SCI Data Direction Register
	VU8 IRCR;           //SCI InfraRed Control Register
	VU8 TR;             //Reserved for Test
	VU8 Reserved_E;
	VU8 IRDR;           //SCI InfraRed Divisor Register;
}SCI_t;


//SCI Control Configure
typedef struct{
	U8 Loop_Mode;
	U8 WireOR_Mode;
	U8 Input_Tied_To_TXD;
	U8 Use_9_DataBit;
	U8 AddrMark_Wake;
	U8 Idle_Line_Type;
	U8 Parity_Enable;
	U8 Odd_Parity;
	U8 Send_Break_Frame;
	U8 Stop_In_Doze;
}SCI_ControlConfig_t;

//SCI Interrupts
#define SCI_INT_TRANSMIT           0x80
#define SCI_INT_TRANSMIT_COMPLETE  0x40
#define SCI_INT_RECEIVE            0x20
#define SCI_INT_IDLE_LINE          0x10

//SCI Pins
#define SCI_PIN_RXD                0x01
#define SCI_PIN_TXD                0x02

//SCI Pins Configure
typedef struct{
	U8 Pin_Output;
}SCI_PinIOConfig_t;

//SCI InfraRed Control Configure
typedef struct{
	U8 Use_IR_Divide;
	U8 Invert_Receive;
	U8 Invert_Trancmit;
	U8 Reception_Number;
	U8 Transmission_Number;
}SCI_IRControlConfig_t;

void SCI_SetBaudrate(SCI_t *sci, U32 sysclk, U32 baudrate);
void SCI_ControlConfig(SCI_t *sci, SCI_ControlConfig_t *config);
void SCI_EnableTransmit(SCI_t *sci);
void SCI_DisableTransmit(SCI_t *sci);
void SCI_EnableReceive(SCI_t *sci);
void SCI_DisableReceive(SCI_t *sci);
void SCI_Sleep(SCI_t *sci);
void SCI_Awake(SCI_t *sci);
void SCI_EnableInterrupt(SCI_t *sci, U8 intp_bits);
void SCI_DisableInterrupt(SCI_t *sci, U8 intp_bits);
U8 SCI_IsTransmitRegEmpty(SCI_t *sci);
U8 SCI_IsTransmitComplete(SCI_t *sci);
U8 SCI_IsReceiveRegFull(SCI_t *sci);
U8 SCI_IsReceiveIdle(SCI_t *sci);
U8 SCI_IsOverrun(SCI_t *sci);
U8 SCI_IsNoise(SCI_t *sci);
U8 SCI_IsFrameError(SCI_t *sci);
U8 SCI_IsParityError(SCI_t *sci);
U8 SCI_IsReceiving(SCI_t *sci);
void SCI_WriteData(SCI_t *sci, U16 data);
U16 SCI_ReadData(SCI_t *sci);
void SCI_SetPinDirection(SCI_t *sci, U8 pin_bits, SCI_PinIOConfig_t *config);
void SCI_WritePin(SCI_t *sci, U8 pin_bits, U8 level);
U8 SCI_ReadPin(SCI_t *sci, U8 pin_bits);
void SCI_IRControlConfig(SCI_t *sci, SCI_IRControlConfig_t *config);
void SCI_EnableIR(SCI_t *sci);
void SCI_DisableIR(SCI_t *sci);
void SCI_SetIRBaudrate(SCI_t *sci, U32 sysclk, U32 baudrate);

#endif /* SCI_H_ */
