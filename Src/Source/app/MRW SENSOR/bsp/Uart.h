/*
 * Uart.h
 *
 *  Created on: 2016��9��14��
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
#define USART_REC_LEN  		        (RX_MASK+1)  	     //���ջ����������ֵ

//���ջ�����״̬���
#define	RX_BUF_F					0x01   //"���ջ�������" ��ʾ��������ȫ�������ݣ�RX_MASK+1����ȫ��û����ȡ�������ٽ��������ݣ��ٽ��ջḲ�ǻ�û�ж���������
#define RX_BUF_E					0x02   //"���ջ�������" ��ʾ���յ��������������Ѿ�ȫ������


typedef struct
{
	U8	rx_buf[USART_REC_LEN];        //���ջ�����
	VU16	rx_seek;                 //��ȡ����ָ��
	VU16	rx_size;                 //��������ָ��
	VU8	rx_stat;                     //���ջ�����״̬
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
