// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : uart_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "debug.h"
#include "cpm_drv.h"
#include "iomacros.h"
#include "uart_drv.h"
#include "uart_reg.h"
#include "dmac_drv.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdlib.h"
#include "ioctrl_drv.h"

volatile UINT8 uart_idle_flag = 0;

typedef struct _uart_recv_buf
{
	UINT16 rp;//read pointer
	UINT16 wp;//write pointer
	UINT16 recv_mode;
	UINT16 ret_val;
	UINT8  dat[UART_RECV_MAX_LEN];//buffer
}UartRecvBufStruct;

static UartRecvBufStruct g_UartRecvBufStruct1 = {0, 0, 0, 0};//uartx buffer
static UartRecvBufStruct g_UartRecvBufStruct2 = {0, 0, 0, 0};//uartx buffer
static UartRecvBufStruct g_UartRecvBufStruct3 = {0, 0, 0, 0};//uartx buffer
UART_InitTypeDef  UART_BT_InitStruct;

/*******************************************************************************
* Function Name  : UART_ISR
* Description    : UART�жϴ���
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
static void UART_ISR(UART_TypeDef *UARTx, UartRecvBufStruct *UARTxRecvBufStruct)
{
    UINT32 timeout = UART_TIMEOUT(1000);
    
    UARTxRecvBufStruct->ret_val = STATUS_OK;

	if(UART_DMA_MODE == UARTxRecvBufStruct->recv_mode)
	{
		if(UARTx->SCISR1 & SCISR1_IDLE_MASK)
		{
			while(((UARTx->SCIFSR)&SCIFSR_REMPTY_MASK) == 0) // �ȴ����յ�������DMA����
			{
				if((UARTx->SCIFSR)&SCIFSR_RTOS_MASK)
				{
					UARTx->SCIFSR2 = 0x0F;
					break;
				}

                if(!timeout)
                {
                    UARTxRecvBufStruct->ret_val |= STATUS_UART_TIMEOUT;
                    return;
                }
                timeout --;
			}
			uart_idle_flag = 1;
		}
	}
    
	if(UART_INT_MODE == UARTxRecvBufStruct->recv_mode)
	{
		if((UARTx->SCIFCR&SCIFCR_RFEN) || (UARTx->SCIFCR&SCIFCR_TFEN))
		{
	#if 0  //����������Ҫ��FIFO RFTS�ж϶�Ӧ������������
			if((UARTx->SCIFSR & SCIFSR_RFTS_MASK) == SCIFSR_RFTS_MASK)
			{
				for(UINT8 loop = 0;; loop<UART_FIFO_TRIGGER_LEVEL; loop ++)
				{
					UARTxRecvBufStruct->dat[UARTxRecvBufStruct->wp + loop] = UARTx->SCIDRL;
				}
				UARTxRecvBufStruct->wp += loop;
				if (UARTxRecvBufStruct->wp >= UART_RECV_MAX_LEN)
					UARTxRecvBufStruct->wp = 0;
				UARTxRecvBufStruct->ret_val = STATUS_OK;
			}
	#endif
			
			while((!(UARTx->SCIFSR & SCIFSR_REMPTY_MASK)) && (timeout))
			{
				UARTxRecvBufStruct->dat[UARTxRecvBufStruct->wp] = UARTx->SCIDRL;
				UARTxRecvBufStruct->wp += 1;
				if (UARTxRecvBufStruct->wp >= UART_RECV_MAX_LEN)
					UARTxRecvBufStruct->wp = 0;
				UARTxRecvBufStruct->ret_val = STATUS_OK;
				timeout --;
			}
            		
			if(!timeout)
			{
				UARTxRecvBufStruct->ret_val |= STATUS_UART_TIMEOUT;
			}
		
		}
		else
		{
			//reveice data
			if((UARTx->SCISR1&SCISR1_RDRF_MASK) == SCISR1_RDRF_MASK)
			{
				UARTxRecvBufStruct->dat[UARTxRecvBufStruct->wp] = UARTx->SCIDRL;
				UARTxRecvBufStruct->wp += 1;
				if (UARTxRecvBufStruct->wp >= UART_RECV_MAX_LEN)
					UARTxRecvBufStruct->wp = 0;
				UARTxRecvBufStruct->ret_val = STATUS_OK;
			}
			
		} 
	}

    if((UARTx->SCISR1&SCISR1_OR_MASK) == SCISR1_OR_MASK)
    {
        UARTxRecvBufStruct->ret_val |= STATUS_UART_OVERRUN;
        UINT8 tmp = UARTx->SCIDRL;   //clear error flag
    }

    if(UARTx->SCIFSR2&SCISR2_FOR_MASK)
    {  
        UINT8 tmp = UARTx->SCIDRL;
        UARTx->SCIFSR2 |= SCISR2_FOR_MASK;
        UARTxRecvBufStruct->ret_val |= STATUS_UART_FIFO_OVERRUN;
    }

    //timeout
    if((UARTx->SCIFSR & SCIFSR_RTOS_MASK ) == SCIFSR_RTOS_MASK)
    {
        UINT8 tmp = UARTx->SCIDRL;
        UARTx->SCIFCR2 |= (SCIFCR2_RXFCLR);
        UARTxRecvBufStruct->ret_val |= STATUS_UART_TIMEOUT;
    }
    
}

/*******************************************************************************
* Function Name  : UART3_ISR
* Description    : UART2�жϴ���
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void SCI1_IRQHandler(void)
{
	UART_ISR(SCI1, &g_UartRecvBufStruct1);
}

/*******************************************************************************
* Function Name  : UART4_ISR
* Description    : UART4�жϴ���
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void SCI2_IRQHandler(void)
{
	UART_ISR(SCI2, &g_UartRecvBufStruct2);
}

/*******************************************************************************
* Function Name  : UART2_ISR
* Description    : UART2�жϴ���
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
void SCI3_IRQHandler(void)
{
	UART_ISR(SCI3, &g_UartRecvBufStruct3);
}


/*******************************************************************************
* Function Name  : UART_Init
* Description    : UART��ʼ��
* Input          : - UARTx: ȡֵSCI2 or SCI_BT
*                  - UART_InitStruct��UARTx  initialization parameter structure Poniter
*
* Output         : None
* Return         : None
******************************************************************************/
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
	UINT16 bandrate_I;
	UINT8 bandrate_F;
	UINT8 bandrate_h;
	UINT8 bandrate_l;

	if (SCI1 == UARTx)
	{
		g_UartRecvBufStruct1.wp = 0;
		g_UartRecvBufStruct1.rp = 0; 
		if ((UART_InitStruct->UART_Mode == UART_INT_MODE)||
            (UART_InitStruct->UART_Mode == UART_DMA_MODE))
		{
			NVIC_Init(2, 2, SCI1_IRQn, 2);
		}
	}
	else if (SCI2 == UARTx)
	{
		g_UartRecvBufStruct2.wp = 0;
		g_UartRecvBufStruct2.rp = 0; 
		if ((UART_InitStruct->UART_Mode == UART_INT_MODE)||
            (UART_InitStruct->UART_Mode == UART_DMA_MODE))
		{
			NVIC_Init(3, 3, SCI2_IRQn, 2);
		}
	}
	else
	{
		g_UartRecvBufStruct3.wp = 0;
		g_UartRecvBufStruct3.rp = 0; 
		if ((UART_InitStruct->UART_Mode == UART_INT_MODE)||
            (UART_InitStruct->UART_Mode == UART_DMA_MODE))
		{
			NVIC_Init(3, 3, SCI3_IRQn, 2);
		}
	}

	bandrate_I = ((UINT16)(g_ips_clk*4/UART_InitStruct->UART_BaudRate))>>6;
	bandrate_F = (((UINT16)(g_ips_clk*8/UART_InitStruct->UART_BaudRate)+1)/2)&0x003f;
	bandrate_h = (UINT8)((bandrate_I>>8)&0x00ff);
	bandrate_l = (UINT8)(bandrate_I&0x00ff);

	UARTx->SCIBRDF = bandrate_F;		//Write float before Interger
	UARTx->SCIBDH = bandrate_h;
	UARTx->SCIBDL = bandrate_l;

	UARTx->SCICR1 = 0x00;
	UARTx->SCICR1 |= UART_InitStruct->UART_FrameLength; //11 bit frame
	if (UART_InitStruct->UART_Parity == UART_PARITY_NONE)
	{
		UARTx->SCICR1 |= ParityDIS;	//parity disable
	}
	else
	{
		UARTx->SCICR1 |= ParityEN;	//parity enable
		UARTx->SCICR1 |= UART_InitStruct->UART_Parity;//odd parity
	}
 
	UARTx->SCICR2 = 0;
    UARTx->SCIRXTOCTR = UART_InitStruct->UART_TimeoutCounter;

    if (UART_InitStruct->UART_Mode == UART_INT_MODE)
	{
		if (SCI1 == UARTx)
	{
		g_UartRecvBufStruct1.recv_mode = UART_INT_MODE;
	}
	if (SCI2 == UARTx)
	{
		g_UartRecvBufStruct2.recv_mode = UART_INT_MODE;
	}
	if (SCI3 == UARTx)
	{
		g_UartRecvBufStruct3.recv_mode = UART_INT_MODE;
	}
		//Setup SCI interrupt
		UARTx->SCICR2 |= SCICR2_RIE_MASK|SCICR2_RE_MASK;
		UARTx->SCIFCR2 |= (SCIFCR2_RXFTOE|SCIFCR2_RXFTOIE|SCIFCR2_RXFCLR);
        UARTx->SCIFCR = (SCIFCR_RFEN|SCIFCR_RXFLSEL_1_8);
	} 
    
    if(UART_InitStruct->UART_Mode == UART_DMA_MODE)
    {
				if (SCI1 == UARTx)
	{
		g_UartRecvBufStruct1.recv_mode = UART_DMA_MODE;
	}
	if (SCI2 == UARTx)
	{
		g_UartRecvBufStruct2.recv_mode = UART_DMA_MODE;
	}
	if (SCI3 == UARTx)
	{
		g_UartRecvBufStruct3.recv_mode = UART_DMA_MODE;
	}
	UARTx->SCIFCR = 0;
	UARTx->SCIFCR  |= (SCIFCR_RFEN|SCIFCR_TFEN|SCIFCR_RXFLSEL_1_8|SCIFCR_TXFLSEL_1_8);
	UARTx->SCIFCR2 |= (SCIFCR2_TXFCLR|SCIFCR2_RXFCLR);//enable fifo 
	UARTx->SCICR1  |=  SCICR1_ILT_MASK;
	//  UARTx->SCICR2  |= (SCICR2_RE_MASK|SCICR2_ILIE_MASK);
	UARTx->SCICR2  |= (SCICR2_RE_MASK | SCICR2_TE_MASK | SCICR2_ILIE_MASK);
//		UARTx->SCIFCR = 0;
//		UARTx->SCIFCR  |= (SCIFCR_RFEN|SCIFCR_TFEN); 
//		UARTx->SCIFCR2 |= (SCIFCR2_TXFCLR|SCIFCR2_RXFCLR|SCIFCR_RXFLSEL_1_8|SCIFCR_TXFLSEL_1_8);//enable fifo 
//		UARTx->SCICR1  |=  SCICR1_ILT_MASK;
//		UARTx->SCICR2  |= (SCICR2_RE_MASK|SCICR2_ILIE_MASK);
    }
} 

/*******************************************************************************
* Function Name  : UART_ByteRecieved
* Description    : UART���յ�����
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*
* Output         : - None
*
* Return         : - TRUE:���յ�����
*                  - FALSE:û�н��յ�����
******************************************************************************/
INT8 UART_ByteRecieved(UART_TypeDef *UARTx)
{
	UartRecvBufStruct *UARTxRecvBufStruct;


	if (UARTxRecvBufStruct->wp != UARTxRecvBufStruct->rp)
	{
		return TRUE;
	}
	return FALSE;
}
/*******************************************************************************
* Function Name  : UART_RecvByte
* Description    : UART����һ���ֽ�
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*
*
* Output         : - dat���������ݻ���
*
* Return         : - STATUS_OK:���յ�����
*                  - STATUS_NULL:û�н��յ�����
******************************************************************************/
UINT16 UART_RecvByte(UART_TypeDef *UARTx, UINT8 *dat)
{
	UartRecvBufStruct *UARTxRecvBufStruct;
    UINT16 ret;
if (SCI1 == UARTx)
	{
	UARTxRecvBufStruct = &g_UartRecvBufStruct1;
	}
	if (SCI2 == UARTx)
	{
	UARTxRecvBufStruct = &g_UartRecvBufStruct2;
	}
	if (SCI3 == UARTx)
	{
	UARTxRecvBufStruct = &g_UartRecvBufStruct3;
	}
	
	if (UARTxRecvBufStruct->wp != UARTxRecvBufStruct->rp)
	{
		*dat = UARTxRecvBufStruct->dat[UARTxRecvBufStruct->rp];
		UARTxRecvBufStruct->rp += 1;
		if (UARTxRecvBufStruct->rp >= UART_RECV_MAX_LEN)
			UARTxRecvBufStruct->rp = 0;
		return STATUS_OK;
	}
    
    if(STATUS_OK != UARTxRecvBufStruct->ret_val)
    {
        ret = UARTxRecvBufStruct->ret_val;
        UARTxRecvBufStruct->ret_val = STATUS_OK; 
    }
    else
    {
        ret = STATUS_NULL;
    }
	return ret;
}

/*******************************************************************************
* Function Name  : UART_ReceiveByte
* Description    : UART����һ���ֽ�
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*
* Output         : None
* Return         : ���յ�һ���ֽ�����
******************************************************************************/
UINT8 UART_ReceiveByte(UART_TypeDef *UARTx)
{  
	UINT8 tmp;
	UINT32 timeout = UART_TIMEOUT(1000);

	while(0 == (UARTx->SCISR1 & SCISR1_RDRF_MASK))
	{
		if(!timeout)
		{
				if (SCI1 == UARTx)
	{
            g_UartRecvBufStruct1.ret_val = STATUS_UART_TIMEOUT;
	}
	if (SCI2 == UARTx)
	{
            g_UartRecvBufStruct2.ret_val = STATUS_UART_TIMEOUT;
	}
	if (SCI3 == UARTx)
	{
            g_UartRecvBufStruct3.ret_val = STATUS_UART_TIMEOUT;
	}
			return 0;
		}
		timeout --;
	}

	tmp = UARTx->SCIDRL;

	return tmp;

}
/*******************************************************************************
* Function Name  : UART_ReceiveData
* Description    : UART���ն���ֽ�
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - recv_buf���������ݻ���
*                  - len�����յ����ݳ���
*
* Output         : None
* Return         : STATUS
******************************************************************************/
UINT16 UART_ReceiveData(UART_TypeDef *UARTx, UINT8 *recv_buf, UINT32 len)
{
	UINT32 i;
if (SCI1 == UARTx)
	{
    g_UartRecvBufStruct1.ret_val = STATUS_OK;
	}
	if (SCI2 == UARTx)
	{
    g_UartRecvBufStruct2.ret_val = STATUS_OK;
	}
	if (SCI3 == UARTx)
	{
    g_UartRecvBufStruct3.ret_val = STATUS_OK;
	}
	SCI_ENABLE_RECEIVER(UARTx);
	for(i = 0; i < len; i++)
	{
		recv_buf[i] = UART_ReceiveByte(UARTx);
		if (SCI1 == UARTx)
	{
        if(STATUS_OK != g_UartRecvBufStruct1.ret_val)
        {
            break;
        }
			}
			if (SCI2 == UARTx)
	{
        if(STATUS_OK != g_UartRecvBufStruct2.ret_val)
        {
            break;
        }
			}
			if (SCI3 == UARTx)
	{
        if(STATUS_OK != g_UartRecvBufStruct3.ret_val)
        {
            break;
        }
			}
	}
	SCI_DISABLE_RECEIVER(UARTx);
if (SCI1 == UARTx)
	{
    return g_UartRecvBufStruct1.ret_val;
	}
	if (SCI2 == UARTx)
	{
    return g_UartRecvBufStruct2.ret_val;
	}
	if (SCI3 == UARTx)
	{
    return g_UartRecvBufStruct3.ret_val;
	}
}

/*******************************************************************************
* Function Name  : UART_SendByte
* Description    : UART����һ���ֽ�
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - SendByte�����͵��ֽ�
*
* Output         : None
* Return         : STATUS
******************************************************************************/
UINT16 UART_SendByte(UART_TypeDef *UARTx, UINT8 outbyte)
{
    UINT32 timeout = UART_TIMEOUT(1000);

	while((UARTx->SCISR1 & SCISR1_TC_MASK)==0)
    {
        if(!timeout)
        {
            return STATUS_UART_TIMEOUT;
        }
        timeout --;
    }
	while((UARTx->SCISR1 & SCISR1_TDRE_MASK)==0)
    {
        if(!timeout)
        {
            return STATUS_UART_TIMEOUT;
        }
        timeout --;
    }
	UARTx->SCIDRL = outbyte&0xff;
	while((UARTx->SCISR1 & SCISR1_TC_MASK)==0)
    {
        if(!timeout)
        {
            return STATUS_UART_TIMEOUT;
        }
        timeout --;
    }
    return STATUS_OK;
}

/*******************************************************************************
* Function Name  : UART_SendData
* Description    : UART���Ͷ���ֽ�
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - send_buf���������ݻ���
*                  - len�����͵����ݳ���
*
* Output         : None
* Return         : None
******************************************************************************/
UINT16 UART_SendData(UART_TypeDef *UARTx, UINT8 *send_buf, UINT32 len)
{
	UINT16 ret;
    UINT32 i;

	SCI_ENABLE_TRANSMITTER(UARTx);
	for (i = 0; i < len; i++)
	{
        ret = UART_SendByte(UARTx, send_buf[i]);
		if(STATUS_OK != ret)
        {
            break;
        }
	}
	SCI_DISABLE_TRANSMITTER(UARTx);

    return ret;
}


/*******************************************************************************
* Function Name  : UART_ConfigGpio
* Description    : UART���ó�GPIO��;
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - UART_Pin��Ӧ��PIN�ţ�ȡֵUART_TX��UART_RX
*                  - UART_Dir������GPIO����   GPIO_OUTPUT�����  GPIO_INPUT������
*
* Output         : None
* Return         : None
******************************************************************************/
void UART_ConfigGpio(UART_TypeDef *UARTx, UART_PINx UART_Pin, UINT8 UART_Dir)
{
	UARTx->SCICR1 = 0x80;

	if (UART_Dir == GPIO_OUTPUT)
	{
		UARTx->SCIDDR |= (1<<UART_Pin);//output
	}
	else if (UART_Dir == GPIO_INPUT)
	{
		UARTx->SCIDDR &= (~(1<<UART_Pin));//input
	}

}

/*******************************************************************************
* Function Name  : UART_ReadGpioData
* Description    : ��ȡUART_Pin��Ӧ���ŵĵ�ƽ
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - UART_Pin��Ӧ��PIN�ţ�ȡֵUART_TX��UART_RX
*
* Output         : None
* Return         : Bit_SET:�ߵ�ƽ  Bit_RESET���͵�ƽ
******************************************************************************/
INT8 UART_ReadGpioData(UART_TypeDef *UARTx, UART_PINx UART_Pin)
{
	INT8 bitstatus = 0x00;

	bitstatus = UARTx->SCIPORT;

	if (bitstatus &(Bit_SET<<UART_Pin))
		bitstatus = Bit_SET;
	else
		bitstatus = Bit_RESET;

	return bitstatus;
}

/*******************************************************************************
* Function Name  : UART_WriteGpioData
* Description    : ����UART_Pin��Ӧ���ŵĵ�ƽ
* Input          : - - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - UART_Pin��Ӧ��PIN�ţ�ȡֵUART_TX��UART_RX
*                  - bitVal�����õĵ�ƽ��Bit_SET������Ϊ�ߵ�ƽ  Bit_RESET������Ϊ�͵�ƽ
*
* Output         : None
* Return         : None
******************************************************************************/
void UART_WriteGpioData(UART_TypeDef *UARTx, UART_PINx UART_Pin, UINT8 bitVal)
{
	if (bitVal == Bit_SET)
		UARTx->SCIPORT |= (Bit_SET<<UART_Pin);
	else
		UARTx->SCIPORT &= (~(Bit_SET<<UART_Pin));
}

/*******************************************************************************
* Function Name  : UART_PullUpControl
* Description    : ����UART_Pin������
* Input          : - - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - UART_Pin��Ӧ��PIN�ţ�ȡֵUART_TX��UART_RX
*                  - NewState��������������DISABLE������Ϊ����  ENABLE������Ϊ����
*
* Output         : None
* Return         : None
******************************************************************************/
void UART_PullUpControl(UART_TypeDef *UARTx, UART_PINx UART_Pin, FunctionalState NewState)
{
	if (NewState == DISABLE)//����
		{
			if(UARTx==SCI1)
				{
					if(UART_Pin==UART_RX)
						IOCTRL->SCI_CONTROL_REG &= ~(1<<9);
					else
					IOCTRL->SCI_CONTROL_REG &= ~(1<<8);
				}
				else if(UARTx==SCI2)
					{
						if(UART_Pin==UART_RX)
							IOCTRL->SCI_CONTROL_REG &= ~(1<<17);
						else
						IOCTRL->SCI_CONTROL_REG &= ~(1<<16);
					}
					else
						{
							if(UART_Pin==UART_RX)
								IOCTRL->SCI_CONTROL_REG &= ~(1<<13);
							else
							IOCTRL->SCI_CONTROL_REG &= ~(1<<12);
						}
		}
		else
			{
				if(UARTx==SCI1)
					{
						if(UART_Pin==UART_RX)
							IOCTRL->SCI_CONTROL_REG |= (1<<9);
						else
						IOCTRL->SCI_CONTROL_REG |= (1<<8);
					}
					else if(UARTx==SCI2)
						{
							if(UART_Pin==UART_RX)
								IOCTRL->SCI_CONTROL_REG |= (1<<17);
							else
							IOCTRL->SCI_CONTROL_REG |= (1<<16);
						}
						else
							{
								if(UART_Pin==UART_RX)
									IOCTRL->SCI_CONTROL_REG |= (1<<13);
								else
								IOCTRL->SCI_CONTROL_REG |= (1<<12);
							}
			}
}
			
/*******************************************************************************
* Function Name  : UART_PullUpConfig
* Description    : ����UART_Pin�Ƿ�����
* Input          : - - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - UART_Pin��Ӧ��PIN�ţ�ȡֵUART_TX��UART_RX
*                  - NewState��������������DISABLE�����ý�ֹ������Ϊ����  ENABLE������Ϊ����
*
* Output         : None
* Return         : None
******************************************************************************/							
void UART_PullUpConfig(UART_TypeDef *UARTx, UART_PINx UART_Pin, FunctionalState NewState)
{
	if (NewState == ENABLE)//����ʹ��
	{
		UARTx->SCIPURD |= (1<<0);
	}
	else
	{
		UARTx->SCIPURD &= (~(1<<0));//������ֹ
	}
}
	
/*******************************************************************************
* Function Name  : UART_DMAReceiveData
* Description    : UART DMA��������
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - channel:DMAͨ��
*                  - len���������ݳ���
*
* Output         : - dest���������ݻ���
*                  
* Return         : STATUS
******************************************************************************/
UINT16 UART_DMAReceiveData(UART_TypeDef *UARTx,UINT8 channel,UINT8 *dest,UINT32 len)
{
    UINT16 ret;

	SCI_ENABLE_RECEIVER(UARTx);
    SCI_ENABLE_DMA(UARTx);
	ret = dma_uartRec(UARTx,channel,dest,len);
	SCI_DISABLE_DMA(UARTx);
	SCI_DISABLE_RECEIVER(UARTx);
	SCI_FIFO_CLR(UARTx);

    return ret;
}

/*******************************************************************************
* Function Name  : UART_DMASendData
* Description    : UART DMA��������
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral��ȡֵUART1/UART2/UART3
*                  - channel:DMAͨ��
*                  - dest���������ݻ���
*                  - len���������ݳ���
*
* Output         : None
* Return         : STATUS
******************************************************************************/
UINT16 UART_DMASendData(UART_TypeDef *UARTx,UINT8 channel,UINT8 *src,UINT32 len)
{
    UINT16 ret; 

	SCI_DISABLE_RECEIVER(UARTx);
	SCI_ENABLE_TRANSMITTER(UARTx);
	SCI_ENABLE_DMA(UARTx);
	ret = dma_uartTram(UARTx,channel,src,len);
	SCI_DISABLE_DMA(UARTx);

    return ret;
}

