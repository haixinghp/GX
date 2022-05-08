/*
 * Uart.h
 *
 *  Created on: 2016年9月14日
 *      
 */

#ifndef UART_H_
#define UART_H_
#include "CCM_MEM.h"
#include "fsm.h"




#define USART_REC_MODE REC_STOP_MODE
#define REC_COVER_MODE  1
#define REC_STOP_MODE   2

#define RX_MASK						(0x0FFF)
#define USART_REC_LEN  		        (RX_MASK+1)  	     //接收缓冲区的最大值

//接收缓冲区状态标记
#define	RX_BUF_F					0x01   //"接收缓冲区满" 表示缓冲区中全部的数据（RX_MASK+1个）全部没有提取，不能再接收新数据，再接收会覆盖还没有读出的数据
#define RX_BUF_E					0x02   //"接收缓冲区空" 表示接收到缓冲区的数据已经全部读完


typedef struct
{
	U8	rx_buf[USART_REC_LEN];        //接收缓冲区
	VU16	rx_seek;                 //提取数据指针
	VU16	rx_size;                 //接收数据指针
	VU8	rx_stat;                     //接收缓冲区状态
}USARTypDef;

void uartInit(U32 baudrate);
 S32  uartCheckData(void);
 U8   uartReadByte(S32 *ch);
 void uartSendBuff(U8 *dat, U32 len);
 S32  uartSendBuffEmpty(void);
 S32 uartTransmitComplete(void);

void uartSetBaudrate(U32 baudrate);


void uartDisableReceive(void);
void uartEnableReceive(void);

#define LOGE(format, ...) //fsmDebugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )
#define LOGD(format, ...) //fsmDebugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )



#endif /* UART_H_ */
