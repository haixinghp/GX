/*
 * Uart.c
 *
 *  Created on: 2016年9月14日
 *      
 */
#include "Uart.h"
#include "SCI.h"
#include "ccm4202s.h"
#include "core_cm4.h"
#include <string.h>
#include "Debug.h"
#include "type.h"

extern UINT32 g_ips_clk;

#define COMM_SCI        SCI2
#define LOOP_BUFF_SIZE  32

#if USART_REC_MODE==REC_COVER_MODE
static U8 g_loop_buf[LOOP_BUFF_SIZE];
static VU32 loop_w_pos = 0, loop_r_pos = 0;
#endif

#if USART_REC_MODE==REC_STOP_MODE
static volatile USARTypDef	uart_tag ;
#endif

void uartInit(U32 baudrate)
{
	SCI_ControlConfig_t cfg;

	cfg.Loop_Mode = 0;
	cfg.WireOR_Mode = 0;
	cfg.Input_Tied_To_TXD = 0;
	cfg.Use_9_DataBit = 0;
	cfg.AddrMark_Wake = 0;
	cfg.Idle_Line_Type = 0;
	cfg.Parity_Enable = 0;
	cfg.Odd_Parity = 0;
	cfg.Send_Break_Frame = 0;
	cfg.Stop_In_Doze = 0;

	SCI_SetBaudrate(&COMM_SCI, g_ips_clk, baudrate);
	SCI_ControlConfig(&COMM_SCI, &cfg);
	SCI_DisableInterrupt(&COMM_SCI,
			     SCI_INT_TRANSMIT | SCI_INT_TRANSMIT_COMPLETE |
			     SCI_INT_RECEIVE  | SCI_INT_IDLE_LINE);
	SCI_EnableInterrupt(&COMM_SCI,
			    SCI_INT_RECEIVE);
	SCI_EnableTransmit(&COMM_SCI);
	SCI_EnableReceive(&COMM_SCI);


	
/*#define SCICR_TX2_GINT5_SWAP_MASK		(1<<15)
#define SCICR_RX2_GINT3_SWAP_MASK		(1<<13)
*(volatile unsigned int *)0x4000000C |= SCICR_TX2_GINT5_SWAP_MASK;
	*(volatile unsigned int *)0x4000000C |= SCICR_RX2_GINT3_SWAP_MASK;*/
	//*(volatile unsigned int *)0x40000000|= (0x01<<3);
	//SCI1 GINT0&GINT4 swap enable 
	//IO_Ctrl_SCI_Swap(TX1_GINT4,TRUE);
	//IO_Ctrl_SCI_Swap(RX1_GINT0,TRUE);


#if USART_REC_MODE==REC_STOP_MODE

	__asm("CPSID I");
	uart_tag.rx_seek = 0;
	uart_tag.rx_size = 0;
	uart_tag.rx_stat |= RX_BUF_E;   //初始化缓冲区状态为空
	memset((U8*)&uart_tag,0,sizeof(USARTypDef));
	__asm("CPSIE I");
#endif

}

#if USART_REC_MODE==REC_COVER_MODE
void uartISR(void)
{
	U8 ch;

	if (SCI_IsReceiveRegFull(&COMM_SCI)){
		ch = SCI_ReadData(&COMM_SCI);
		__asm("CPSID I");
		g_loop_buf[loop_w_pos] = ch;
		if (++loop_w_pos >= LOOP_BUFF_SIZE){
			loop_w_pos = 0;
		}
		if (loop_w_pos == loop_r_pos){
			if (++loop_r_pos >= LOOP_BUFF_SIZE){
				loop_r_pos = 0;
			}
		}
		__asm("CPSIE I");
	}
}

 S32  uartCheckData(void)
{
	S32 ret;
	__asm("CPSID I");
	ret = (loop_w_pos != loop_r_pos);
	__asm("CPSIE I");
	return ret;
}

 U8   uartReadByte(void)
{
	U8 ret = 0;

	__asm("CPSID I");
	if (loop_w_pos != loop_r_pos){
		ret = g_loop_buf[loop_r_pos];
		if (++loop_r_pos >= LOOP_BUFF_SIZE){
			loop_r_pos = 0;
		}
	}
	__asm("CPSIE I");
	return ret;
}
#endif

#if USART_REC_MODE==REC_STOP_MODE
void uartISR(void)
{
	U8 res;
	if (SCI_IsReceiveRegFull(&COMM_SCI))
	{
		res = SCI_ReadData(&COMM_SCI);
		NVIC->ICER[SCI2_IRQn/32] |= 1<<SCI2_IRQn%32;    //关闭SCI1中断
		if(!(uart_tag.rx_stat&RX_BUF_F))
		{
			uart_tag.rx_buf[uart_tag.rx_size] = res;
			uart_tag.rx_size++;
			uart_tag.rx_size = uart_tag.rx_size&RX_MASK;

			if(uart_tag.rx_size == uart_tag.rx_seek)
			{
				uart_tag.rx_stat |= RX_BUF_F;
			}
			uart_tag.rx_stat &= ~RX_BUF_E;
		}
		NVIC->ISER[SCI2_IRQn/32] |=1 <<SCI2_IRQn%32;    //打开SCI1中断
	}
}

 S32  uartCheckData(void)
{
	S32 ret;
	NVIC->ICER[SCI2_IRQn/32] |= 1<<SCI2_IRQn%32;    //关闭SCI1中断
	ret = ( uart_tag.rx_seek!= uart_tag.rx_size);
	NVIC->ISER[SCI2_IRQn/32] |=1 <<SCI2_IRQn%32;    //打开SCI1中断
	return ret;
}

 U8   uartReadByte(S32 *ch)
{

	U8	res=0;
	//判断是否为“空”
	NVIC->ICER[SCI2_IRQn/32] |= 1<<SCI2_IRQn%32;    //关闭SCI1中断
	if(!(uart_tag.rx_stat&RX_BUF_E))
	{
		*ch = uart_tag.rx_buf[uart_tag.rx_seek];
		uart_tag.rx_seek++;
		uart_tag.rx_seek = uart_tag.rx_seek&RX_MASK;
		if(uart_tag.rx_size == uart_tag.rx_seek)
		{
			uart_tag.rx_stat |= RX_BUF_E;
		}
		uart_tag.rx_stat &= ~RX_BUF_F;
		res=1;
	}
	else
	{
		*ch=-1;
	}

	NVIC->ISER[SCI2_IRQn/32] |=1 <<SCI2_IRQn%32;    //打开SCI1中断
	return res;
}

#endif

 void uartSendBuff(U8 *dat, U32 len)
{
//	while(len--){
//		while(!SCI_IsTransmitRegEmpty(&COMM_SCI));
//		SCI_WriteData(&COMM_SCI, *dat++);
//	}
}

 S32  uartSendBuffEmpty(void)
{
	return (SCI_IsTransmitRegEmpty(&COMM_SCI) == 1);  //新添加
}


 S32 uartTransmitComplete(void)
{
	return (SCI_IsTransmitComplete(&COMM_SCI) == 1);  //新添加
}


void uartSetBaudrate(U32 baudrate)
{
	SCI_SetBaudrate(&COMM_SCI, SYS_CLOCK / 2, baudrate);
}



void uartDisableReceive(void)
{
	__asm("CPSID I");	//TODO....
	SCI_ReadData(&COMM_SCI);

	uart_tag.rx_seek = 0;
	uart_tag.rx_size = 0;
	uart_tag.rx_stat |= RX_BUF_E;   //初始化缓冲区状态为空
	SCI_DisableInterrupt(&COMM_SCI,
				     SCI_INT_TRANSMIT | SCI_INT_TRANSMIT_COMPLETE |
				     SCI_INT_RECEIVE  | SCI_INT_IDLE_LINE);
	__asm("CPSIE I"); //TODO....
}


void uartEnableReceive(void)
{
	SCI_EnableInterrupt(&COMM_SCI,
				    SCI_INT_RECEIVE);
}



