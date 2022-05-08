/*
 * SPI.c
 *
 *  Created on: 2016年9月14日
 *      
 */
#include "SPI.h"

/************************************************************************/
/*SPI基本通信设置                                                       */
/************************************************************************/

//设置SPI的基本设置
void SPI_ControlConfig(SPI_t *spi, SPI_ControlConfig_t *config)
{
    spi->CR2 = ((config->Stop_In_Doze) ? 0x02 : 0x00) |
        ((config->Serial_Pin_Control) ? 0x01 : 0x00)  |
        (spi->CR2 & 0xFC);
    spi->CR1 = ((config->WireOR_Mode) ? 0x20 : 0x00)  |
        ((config->MasterMode)  ? 0x10 : 0x00)  |
        ((config->CPOL)        ? 0x08 : 0x00)  |
        ((config->CPHA)        ? 0x04 : 0x00)  |
        ((config->SSOuput_Enable) ? 0x02 : 0x00) |
        ((config->LSB_First)   ? 0x01 : 0x00)  |
        (spi->CR1 & 0xC0);
    spi->FR = (spi->FR & 0xF0) | (config->Frame_Bits & 0xF);
}

//设置SPI帧传输设置
void SPI_FrameConfig(SPI_t *spi, SPI_FrameConfig_t *config)
{
    spi->FR = ((config->CS_Keep_Low) ? 0x80 : 0x00) |
        ((config->GuardTime_Enable)  ? 0x40 : 0x00) |
        ((config->LoopBack_Mode)     ? 0x20 : 0x00) |
        ((config->TI_Frame_Format)   ? 0x10 : 0x00) |
        (spi->FR & 0xF);
    spi->CR2 = (spi->CR2 & 0x3) |
        ((config->GuardTime_PreBits & 0x7) << 5) |
        ((config->GuardTime_Bits & 0x7) << 2);
}

//设置传输计数
void SPI_SetTransCount(SPI_t *spi, U32 count)
{
    spi->TCNT_H = (count >> 16) & 0xFF;
    spi->TCNT_M = (count >>  8) & 0xFF;
    spi->TCNT_L = (count >>  0) & 0xFF;
}

//设置SPI的DMA
void SPI_DMAConfig(SPI_t *spi, SPI_DMAConfig_t *config)
{
    spi->DMATHR = ((config->TX_DMA_Threshold & 0x7) << 4) |
    		       (config->RX_DMA_Threshold & 0x7);
    spi->DMACR  = ((config->TX_DMA_Enable) ? 0x2 : 0x0) |
    		      ((config->RX_DMA_Enable) ? 0x1 : 0x0);
}

//设置SPI波特率，Baudrate = 10MHz / ((sppr + 1) * 2 ^ (spr + 1))
void SPI_SetBaudrate(SPI_t *spi, U8 sppr, U8 spr)
{
    spi->BR = ((sppr & 0x7) << 4) | (spr & 0x7);
}

//SPI读
U16 SPI_ReadData(SPI_t *spi)
{
    return (((U16)spi->DR_H << 8) | ((U16)spi->DR_L));
}

//SPI写
void SPI_WriteData(SPI_t *spi, U16 data)
{
    spi->DR_L = data & 0xFF;
    spi->DR_H = data >> 8;
}


U8 SPI_ReadByte(SPI_t *spi)
{
	return spi->DR_L;
}

void SPI_WriteByte(SPI_t *spi, U8 data)
{
	spi->DR_L = data;
}


//启动SPI模块
void SPI_Enable(SPI_t *spi)
{
    spi->CR1 |= 0x40;
}

//停止SPI模块
void SPI_Disable(SPI_t *spi)
{
    spi->CR1 &= ~0x40;
}

/************************************************************************/
/* SPI引脚设置                                                          */
/************************************************************************/
//SPI引脚设置
void SPI_PinConfig(SPI_t *spi, SPI_PinConfig_t *config)
{
    spi->PURD = ((config->HighSpeedMode_Enable) ? 0x80 : 0x00) |
        ((config->PinSwitch_Enable) ? 0x40 : 0x00)     |
        ((config->SamplePointDelay & 0x3) << 2)        |
        ((config->Polldown_Enable) ? 0x02 : 0x00)      |
        ((config->Pollup_Enable) ? 0x01 : 0x00);
}

//设置SPI引脚GPIO功能
void SPI_PinIOConfig(SPI_t *spi, SPI_Pin_t pin, SPI_PinIOConfig_t *config)
{
    U8 reg_mask = 0x11 << pin;
    U8 reg_value = (((config->GPIO_Prior_Normal)? 0x10 : 0x00) |
                    ((config->Direction_Output) ? 0x01 : 0x00) ) << pin;
    spi->DDR = (spi->DDR & ~(reg_mask)) | reg_value;
}

//写SPI引脚GPIO
void SPI_WritePin(SPI_t *spi, SPI_Pin_t pin, U8 level)
{
    spi->PORT = (spi->PORT & (~(1 << pin))) | (((level) ? 1 : 0) << pin);
}

//读SPI引脚GPIO
U8 SPI_ReadPin(SPI_t *spi, SPI_Pin_t pin)
{
    return ((spi->PORT >> pin) & 1);
}

/************************************************************************/
/* SPI模块中断                                                          */
/************************************************************************/
//设置SPI中断
void SPI_EnableInterrupt(SPI_t *spi, SPI_Interrupt_t intp)
{
    switch (intp){
    case SPI_MODE_FAULT:
        spi->ICR |= 0x10;
        break;
    case SPI_END_OF_TRANS:
        spi->CR1 |= 0x80;
        break;
    case SPI_FRAME_LOST:
        spi->ICR |= 0x40;
        break;
    }
}

//禁止SPI中断
void SPI_DisableInterrupt(SPI_t *spi, SPI_Interrupt_t intp)
{
    switch (intp){
    case SPI_MODE_FAULT:
        spi->ICR &= ~0x10;
        break;
    case SPI_END_OF_TRANS:
        spi->CR1 &= ~0x80;
        break;
    case SPI_FRAME_LOST:
        spi->ICR &= ~0x40;
        break;
    }
}

//查询SPI中断状态
U8 SPI_QueryInterrupt(SPI_t *spi, SPI_Interrupt_t intp)
{
    switch (intp){
    case SPI_MODE_FAULT:
        return ((spi->SR_L >> 4) & 1);
    case SPI_END_OF_TRANS:
        return ((spi->SR_L >> 5) & 1);
    case SPI_FRAME_LOST:
        return ((spi->SR_L >> 6) & 1);
    }
    return 0;
}

//清除SPI中断
void SPI_ClearInterrupt(SPI_t *spi, SPI_Interrupt_t intp)
{
	U8 tmp;
    switch (intp){
    case SPI_MODE_FAULT:
    	tmp = spi->SR_L;
        tmp = spi->CR1;
        spi->CR1 = tmp;
        break;
    case SPI_END_OF_TRANS:
        spi->SR_L |= 0x20;
        break;
    case SPI_FRAME_LOST:
        spi->SR_L |= 0x40;
        break;
    }
}

U8 SPI_QuerySPIFinish(SPI_t *spi)
{
    return ((spi->SR_L >> 7) & 1);
}

/************************************************************************/
/* SSPin中断                                                            */
/************************************************************************/
//设置SS脚的中断触发模式
void SPI_SetSSPinTrigger(SPI_t *spi, SPI_SSPinTrigger_t trigger)
{
    spi->IRSP = (spi->IRSP & 0xF4) | trigger;
}

//允许SS脚中断
void SPI_EnableSSPinInterrupt(SPI_t *spi)
{
    spi->IRSP |= 0x80;
}

//禁止SS脚中断
void SPI_DisableSSPinInterrupt(SPI_t *spi)
{
    spi->IRSP &= ~0x80;
}

//读SS脚中断状态
U8 SPI_QuerySSPinInterrupt(SPI_t *spi)
{
    return ((spi->IRSP >> 4) & 1);
}

//读SS脚电平
U8 SPI_ReadSSPinLevel(SPI_t *spi)
{
    return ((spi->IRSP >> 2) & 1);
}

/************************************************************************/
/* TX/RX FIFO读写                                                       */
/************************************************************************/
//读TX FIFO当前指针位置
U8 SPI_GetTXFIFOPos(SPI_t *spi)
{
    return ((spi->TXFSR >> 4) & 0x7);
}

//读RX FIFO当前指针位置
U8 SPI_GetRXFIFOPos(SPI_t *spi)
{
    return ((spi->RXFSR >> 4) & 0x7);
}

//读TX FIFO当前数据数量
U8 SPI_GetTXFIFOCount(SPI_t *spi)
{
    return (spi->TXFSR & 0xF);
}

//读RX FIFO当前数据数量
U8 SPI_GetRXFIFOCount(SPI_t *spi)
{
    return (spi->RXFSR & 0xF);
}

//调试读TX FIFO
U8 SPI_DebugReadTXFIFO(SPI_t *spi, U8 pos)
{
    spi->FDCR = (spi->FDCR & 0x8F) | ((pos & 0x7) << 4);
    return spi->TXFDBGR;
}

//调试读RX FIFO
U8 SPI_DebugReadRXFIFO(SPI_t *spi, U8 pos)
{
    spi->FDCR = (spi->FDCR & 0xF8) | (pos & 0x7) ;
    return spi->RXFDBGR;
}


/************************************************************************/
/* TX/RX FIFO中断                                                            */
/************************************************************************/
//设置TX FIFO
void SPI_TXFIFOConfig(SPI_t *spi, SPI_FIFOConfig_t *config)
{
    spi->TXFTOCTR = ((config->Timeout_Enable) ? 0x40 : 0x00) |
        (config->Timeout_Count & 0x3F)           |
        (spi->TXFTOCTR & 0x80);
    spi->TXFCR = (spi->TXFCR & 0xF8) | (config->Service_Threshold & 0x7);
}

//查询TX FIFO中断状态
U8 SPI_QueryTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        return ((spi->SR_H >> 7) & 1);
    case OVERFLOW_INTERRUPT:
        return ((spi->SR_H >> 6) & 1);
    case UNDERFLOW_INTERRUPT:
        return ((spi->SR_H >> 5) & 1);
    case THRESHOLD_INTERRUPT:
        return ((spi->SR_H >> 4) & 1);
    }
    return 0;
}

//允许TX FIFO相关中断
void SPI_EnableTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        spi->TXFTOCTR |= 0x80;
        break;
    case OVERFLOW_INTERRUPT:
        spi->TXFCR |= 0x40;
        break;
    case UNDERFLOW_INTERRUPT:
        spi->TXFCR |= 0x20;
        break;
    case THRESHOLD_INTERRUPT:
        spi->TXFCR |= 0x10;
        break;
    }
}

//禁止TX FIFO相关中断
void SPI_DisableTXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        spi->TXFTOCTR &= ~0x80;
        break;
    case OVERFLOW_INTERRUPT:
        spi->TXFCR &= ~0x40;
        break;
    case UNDERFLOW_INTERRUPT:
        spi->TXFCR &= ~0x20;
        break;
    case THRESHOLD_INTERRUPT:
        spi->TXFCR &= ~0x10;
        break;
    }
}

//复位TX FIFO
void SPI_ResetTXFIFO(SPI_t *spi)
{
    spi->TXFCR |= 0x80;
}

//查询TX FIFO是否满
U8 SPI_IsTXFIFO_Full(SPI_t *spi)
{
    return ((spi->SR_L >> 3) & 1);
}

//查询TX FIFO是否空
U8 SPI_IsTXFIFO_Empty(SPI_t *spi)
{
    return ((spi->SR_L >> 2) & 1);
}



//设置RX FIFO
void SPI_RXFIFOConfig(SPI_t *spi, SPI_FIFOConfig_t *config)
{
    spi->RXFTOCTR = ((config->Timeout_Enable) ? 0x40 : 0x00) |
        (config->Timeout_Count & 0x3F)           |
        (spi->RXFTOCTR & 0x80);
    spi->RXFCR = (spi->RXFCR & 0xF8) | (config->Service_Threshold & 0x7);
}

//查询RX FIFO中断状态
U8 SPI_QueryRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        return ((spi->SR_H >> 3) & 1);
    case OVERFLOW_INTERRUPT:
        return ((spi->SR_H >> 2) & 1);
    case UNDERFLOW_INTERRUPT:
        return ((spi->SR_H >> 1) & 1);
    case THRESHOLD_INTERRUPT:
        return ((spi->SR_H >> 0) & 1);
    }
    return 0;
}

//允许RX FIFO相关中断
void SPI_EnableRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        spi->RXFTOCTR |= 0x80;
        break;
    case OVERFLOW_INTERRUPT:
        spi->RXFCR |= 0x40;
        break;
    case UNDERFLOW_INTERRUPT:
        spi->RXFCR |= 0x20;
        break;
    case THRESHOLD_INTERRUPT:
        spi->RXFCR |= 0x10;
        break;
    }
}

//禁止RX FIFO相关中断
void SPI_DisableRXFIFOInterrupt(SPI_t *spi, SPI_FIFO_Interrupt_t intp)
{
    switch(intp){
    case TIMEOUT_INTERRUPT:
        spi->RXFTOCTR &= ~0x80;
        break;
    case OVERFLOW_INTERRUPT:
        spi->RXFCR &= ~0x40;
        break;
    case UNDERFLOW_INTERRUPT:
        spi->RXFCR &= ~0x20;
        break;
    case THRESHOLD_INTERRUPT:
        spi->RXFCR &= ~0x10;
        break;
    }
}

//复位RX FIFO
void SPI_ResetRXFIFO(SPI_t *spi)
{
    spi->RXFCR |= 0x80;
}

//查询RX FIFO是否满
U8 SPI_IsRXFIFO_Full(SPI_t *spi)
{
    return ((spi->SR_L >> 1) & 1);
}

//查询RX FIFO是否空
U8 SPI_IsRXFIFO_Empty(SPI_t *spi)
{
    return ((spi->SR_L >> 0) & 1);
}

/************************************************************************/
/*SCK Delay 设置                                                        */
/************************************************************************/
//设置SCK Delay设置
void SPI_SCKDelayConfig(SPI_t *spi, SPI_SCKDelayConfig_t *config)
{
    spi->BSCDR = ((config->Before_SCK_Delay_Enable) ? 0x80 : 0x00) |
        ((config->Pre_Before_SCK_Delay_Bits & 0x7) << 4)  |
        (config->Before_SCK_Delay_Bits & 0x7);
    spi->ASCDR = ((config->After_SCK_Delay_Enable) ? 0x80 : 0x00)  |
        ((config->Pre_After_SCK_Delay_Bits * 0x7) << 4)   |
        (config->After_SCK_Delay_Bits & 0x7);
}

