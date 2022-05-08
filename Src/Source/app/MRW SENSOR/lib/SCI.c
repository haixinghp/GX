/*
 * SCI.c
 *
 *  Created on: 2016年9月14日
 *      
 */
#include "SCI.h"

//设置SCI波特率
void SCI_SetBaudrate(SCI_t *sci, U32 sysclk, U32 baudrate)
{
	U32 div;

	if (baudrate == 0){
		return;
	}
	div = sysclk * 8 / baudrate + 1;
	sci->BRDF  = (div >>  1) & 0x3F;
	sci->BRDIH = (div >> 15) & 0xFF;
	sci->BRDIL = (div >>  7) & 0xFF;
}

//设置SCI的基本设置
void SCI_ControlConfig(SCI_t *sci, SCI_ControlConfig_t *config)
{
	sci->CR1 = ((config->Loop_Mode)         ? 0x80 : 0x00) |
		       ((config->WireOR_Mode)       ? 0x40 : 0x00) |
		       ((config->Input_Tied_To_TXD) ? 0x20 : 0x00) |
		       ((config->Use_9_DataBit)     ? 0x10 : 0x00) |
		       ((config->AddrMark_Wake)     ? 0x08 : 0x00) |
		       ((config->Idle_Line_Type)    ? 0x04 : 0x00) |
		       ((config->Parity_Enable)     ? 0x02 : 0x00) |
		       ((config->Odd_Parity)        ? 0x01 : 0x00);
	sci->CR2 = (sci->CR2 & 0xFE) |
	           ((config->Send_Break_Frame)  ? 0x01 : 0x00);
	sci->PURD = ((config->Stop_In_Doze)     ? 0x80 : 0x00);
}

//允许SCI发送
void SCI_EnableTransmit(SCI_t *sci)
{
	sci->CR2 |= 0x08;
}

//禁止SCI发送
void SCI_DisableTransmit(SCI_t *sci)
{
	sci->CR2 &= ~0x08;
}

//允许SCI接收
void SCI_EnableReceive(SCI_t *sci)
{
	sci->CR2 |= 0x04;
}

//禁止SCI接收
void SCI_DisableReceive(SCI_t *sci)
{
	sci->CR2 &= ~0x04;
}

//SCI进入睡眠状态
void SCI_Sleep(SCI_t *sci)
{
	sci->CR2 |= 0x02;
}

//SCI退出睡眠状态
void SCI_Awake(SCI_t *sci)
{
	sci->CR2 &= ~0x02;
}

//SCI允许中断
void SCI_EnableInterrupt(SCI_t *sci, U8 intp_bits)
{
	sci->CR2 = (sci->CR2 & 0x0F) | (intp_bits & 0xF0);
}

//SCI禁止中断
void SCI_DisableInterrupt(SCI_t *sci, U8 intp_bits)
{
	sci->CR2 = (sci->CR2 & 0x0F) | ((~intp_bits) & 0xF0);
}

//SCI发送寄存器是否空
U8 SCI_IsTransmitRegEmpty(SCI_t *sci)
{
	return ((sci->SR1 >> 7) & 1);
}

//SCI是否发送完成
U8 SCI_IsTransmitComplete(SCI_t *sci)
{
	return ((sci->SR1 >> 6) & 1);
}

//SCI接收寄存器是否满
U8 SCI_IsReceiveRegFull(SCI_t *sci)
{
	return ((sci->SR1 >> 5) & 1);
}

//是否接受到IDLE帧
U8 SCI_IsReceiveIdle(SCI_t *sci)
{
	return ((sci->SR1 >> 4) & 1);
}

//是否接收溢出
U8 SCI_IsOverrun(SCI_t *sci)
{
	return ((sci->SR1 >> 3) & 1);
}

//是否接收噪声
U8 SCI_IsNoise(SCI_t *sci)
{
	return ((sci->SR1 >> 2) & 1);
}

//是否有帧错误
U8 SCI_IsFrameError(SCI_t *sci)
{
	return ((sci->SR1 >> 1) & 1);
}

//是否有校验错
U8 SCI_IsParityError(SCI_t *sci)
{
	return ((sci->SR1 >> 0) & 1);
}

//是否处于接收状态
U8 SCI_IsReceiving(SCI_t *sci)
{
	return ((sci->SR2 >> 0) & 1);
}

//发送数据
void SCI_WriteData(SCI_t *sci, U16 data)
{
	sci->DRH = ((data & 0x100) >> 2);
	sci->DRL = (data & 0xFF);
}

//接收数据
U16 SCI_ReadData(SCI_t *sci)
{
	return ((U16)(sci->DRL)) | ((U16)(sci->DRH & 0x80) << 1);
}

//设置CSI的Pin
void SCI_SetPinDirection(SCI_t *sci, U8 pin_bits, SCI_PinIOConfig_t *config)
{
	pin_bits &= 0x03;
	if (config->Pin_Output){
		sci->DDR |= pin_bits;
	}else{
		sci->DDR &= ~pin_bits;
	}
}

//写SCI引脚的GPIO
void SCI_WritePin(SCI_t *sci, U8 pin_bits, U8 level)
{
	pin_bits &= 0x03;
	if (level){
		sci->PORT |= pin_bits;
	}else{
		sci->PORT &= ~pin_bits;
	}
}

//读SCI引脚的GPIO
U8 SCI_ReadPin(SCI_t *sci, U8 pin_bits)
{
	pin_bits &= 0x03;
	return (sci->PORT & pin_bits);
}

//设置IR配置
void SCI_IRControlConfig(SCI_t *sci, SCI_IRControlConfig_t *config)
{
	sci->IRCR = (sci->IRCR & 0x01) |
		((config->Use_IR_Divide)   ? 0x02 : 0x00) |
		((config->Invert_Receive)  ? 0x04 : 0x00) |
		((config->Invert_Trancmit) ? 0x08 : 0x00) |
		((config->Reception_Number & 0x03) << 4)  |
		((config->Transmission_Number & 0x03) << 6);
}

//允许IR接口
void SCI_EnableIR(SCI_t *sci)
{
	sci->IRCR |= 0x01;
}

//禁止IR接口
void SCI_DisableIR(SCI_t *sci)
{
	sci->IRCR &= ~0x01;
}

//设置IR波特率分频
void SCI_SetIRBaudrate(SCI_t *sci, U32 sysclk, U32 baudrate)
{
	sci->IRDR = (sysclk / baudrate) & 0xFF;
}
