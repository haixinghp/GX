// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : uart_drv.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef UART_DRV_H_
#define UART_DRV_H_

#include "memmap.h"
#include "def.h"
#include "uart_reg.h"

#define SCI1		        ((UART_TypeDef *)(SCI1_BASE_ADDR))
#define SCI2		        ((UART_TypeDef *)(SCI2_BASE_ADDR))
#define SCI3		        ((UART_TypeDef *)(SCI3_BASE_ADDR))

#define SCI_BT          SCI2


#define IS_UART_ALL_PERIPH(PERIPH) (((PERIPH) == SCI) || \
                                    ((PERIPH) == SCI_BT))
////////////////////////////////////////////////////////////////////////////////////////

#define SCI2_ENABLE_TRANSMITTER      (SCI2->SCICR2 |= SCICR2_TE_MASK)
#define SCI2_DISABLE_TRANSMITTER     (SCI2->SCICR2 &= ~SCICR2_TE_MASK)

#define SCI2_ENABLE_RECEIVER         (SCI2->SCICR2 |= SCICR2_RE_MASK)
#define SCI2_DISABLE_RECEIVER        (SCI2->SCICR2 &= ~SCICR2_RE_MASK)



////////////////////////////////////////////////////////////////////////////////////////
#define		UART_DATA_FRAME_LEN_10BIT	    0				//10bit frame:----StartBit:1bit DataBit:8bit StopBit:1bit
#define		UART_DATA_FRAME_LEN_11BIT	    SCICR1_M_MASK	//11bit frame:----StartBit:1bit DataBit:9bit StopBit:1bit

#define IS_UART_FRAMW_LENGTH(LENGTH) (((LENGTH) == UART_DATA_FRAME_LEN_10BIT) || \
                                      ((LENGTH) == UART_DATA_FRAME_LEN_11BIT))



#define		ParityDIS	                 0			      	//Parity Disable
#define		ParityEN	                 SCICR1_PE_MASK	//Parity Enable

#define		UART_PARITY_EVE		         0			      	//Even Parity
#define		UART_PARITY_ODD		         SCICR1_PT_MASK	//Odd Parity
#define   UART_PARITY_NONE           2              //NONE Parity


#define IS_UART_PARITY(PARITY) (((PARITY) == UART_PARITY_EVE) || \
                                ((PARITY) == UART_PARITY_ODD) || \
                                ((PARITY) == UART_PARITY_NONE))

#define UART_DELAY               20000
#define UART_TIMEOUT(n)          (n*UART_DELAY)
//#define UART_TIMEOUT_1S              20000000  // sys_clk=120MHz, delay 1MS ?? sys_clk/6000
/*******************************************************************************
RXFLSEL                     UART_FIFO_LEN
SCIFCR_RXFLSEL_1_8          0x02
SCIFCR_RXFLSEL_1_4          0x04
SCIFCR_RXFLSEL_1_2          0x08
SCIFCR_RXFLSEL_3_4          0x0C
SCIFCR_RXFLSEL_7_8          0x0E
*******************************************************************************/
#define UART_FIFO_TRIGGER_LEVEL            0x02       //??RXFLSEL????????
#define UART_RECV_MAX_LEN                  0x200

typedef enum
{
	UART_NORMAL_MODE = 0,
	UART_INT_MODE,
	UART_EDMA_MODE,
	UART_DMA_MODE

}UART_MODE;

//????????????????
#define SCI_CPU_EN		0 
#define SCI_DMA_EN		0
#define SCI_INT_EN		1

#define IS_UART_MODE(MODE) (((MODE) == UART_NORMAL_MODE) || \
                            ((MODE) == UART_INT_MODE) || \
                            ((MODE) == UART_EDMA_MODE) || \
                            ((MODE) == UART_DMA_MODE) )

/* UART Init Structure definition */
typedef struct
{

  UINT32 UART_BaudRate;
  UINT8  UART_FrameLength;
  UINT8  UART_StopBits;
  UINT8  UART_Parity;
  UINT8  UART_Mode;
  UINT8  UART_TimeoutCounter;

} UART_InitTypeDef;

typedef enum
{
    UART_RX = 0,
    UART_TX,

}UART_PINx;

extern UART_InitTypeDef UART_BT_InitStruct;
/*******************************************************************************
* Function Name  : UART_Init
* Description    : UART??????
* Input          : - UARTx: where x can be 0 to 1 to select the UART peripheral??????SCI0 or SCI1
*                  - UART_InitStruct??UARTx  initialization parameter structure Poniter
*
* Output         : None
* Return         : None
******************************************************************************/
extern void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);

/*******************************************************************************
* Function Name  : UART_ReceiveByte
* Description    : UART????????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*
* Output         : None
* Return         : ??????????????????
******************************************************************************/
extern UINT8 UART_ReceiveByte(UART_TypeDef *UARTx);
/*******************************************************************************
* Function Name  : UART_ReceiveData
* Description    : UART????????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - recv_buf??????????????
*                  - len????????????????
*
* Output         : None
* Return         : STATUS
******************************************************************************/
extern UINT16 UART_ReceiveData(UART_TypeDef *UARTx,UINT8 *recv_buf, UINT32 len);

/*******************************************************************************
* Function Name  : UART_SendByte
* Description    : UART????????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - SendByte????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern UINT16 UART_SendByte(UART_TypeDef *UARTx, UINT8 outbyte);

/*******************************************************************************
* Function Name  : UART_SendData
* Description    : UART????????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - send_buf??????????????
*                  - len????????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern UINT16 UART_SendData(UART_TypeDef *UARTx, UINT8 *send_buf, UINT32 len);

/*******************************************************************************
* Function Name  : UART_ByteRecieved
* Description    : UART??????????
* Input          : - UARTx:????SCI1?? SCI2
*
* Output         : - None
*
* Return         : - TRUE:??????????
*                  - FALSE:??????????????
******************************************************************************/
extern INT8 UART_ByteRecieved(UART_TypeDef *UARTx);

/*******************************************************************************
* Function Name  : UART_RecvByte
* Description    : UART????????????
* Input          : - UARTx: where x can be 0 to 1 to select the UART peripheral??????SCI0 or SCI1
*
*
* Output         : - dat??????????????
*
* Return         : - TRUE:??????????
*                  - FALSE:??????????????
******************************************************************************/
extern UINT16 UART_RecvByte(UART_TypeDef *UARTx, UINT8 *dat);

/*******************************************************************************
* Function Name  : UART_ConfigGpio
* Description    : UART??????GPIO????
* Input          : - UARTx: where x can be 1, 2  to select the UART peripheral.
*                  - UART_Pin??????PIN????????UART_TX??UART_RX
*                  - UART_Dir??????GPIO????   GPIO_OUTPUT??????  GPIO_INPUT??????
*
* Output         : None
* Return         : None
******************************************************************************/
extern void UART_ConfigGpio(UART_TypeDef *UARTx, UART_PINx UART_Pin, UINT8 UART_Dir);

/*******************************************************************************
* Function Name  : UART_ReadGpioData
* Description    : ????UART_Pin??????????????
* Input          : - UARTx: where x can be 1, 2  to select the UART peripheral.
*                  - UART_Pin??????PIN????????UART_TX??UART_RX
*
* Output         : None
* Return         : Bit_SET:??????  Bit_RESET????????
******************************************************************************/
extern INT8 UART_ReadGpioData(UART_TypeDef *UARTx, UART_PINx UART_Pin);

/*******************************************************************************
* Function Name  : UART_PullUpControl
* Description    : ????UART_Pin??????
* Input          : - - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - UART_Pin??????PIN????????UART_TX??UART_RX
*                  - NewState??????????????DISABLE????????????  ENABLE????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern void UART_PullUpControl(UART_TypeDef *UARTx, UART_PINx UART_Pin, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_PullUpConfig
* Description    : ????UART_Pin????????
* Input          : - - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - UART_Pin??????PIN????????UART_TX??UART_RX
*                  - NewState??????????????DISABLE??????????????????????  ENABLE????????????
*
* Output         : None
* Return         : None
******************************************************************************/							
extern void UART_PullUpConfig(UART_TypeDef *UARTx, UART_PINx UART_Pin, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_WriteGpioData
* Description    : ????UART_Pin??????????????
* Input          : - UARTx: where x can be 1, 2  to select the UART peripheral.
*                  - UART_Pin??????PIN????????UART_TX??UART_RX
*                  - bitVal??????????????Bit_SET??????????????  Bit_RESET??????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern void UART_WriteGpioData(UART_TypeDef *UARTx, UART_PINx UART_Pin, UINT8 bitVal);

/*******************************************************************************
* Function Name  : UART_DMAReceiveData
* Description    : UART DMA????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - channel:DMA????
*                  - dest??????????????
*                  - len??????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern UINT16 UART_DMAReceiveData(UART_TypeDef *UARTx,UINT8 channel,UINT8 *dest,UINT32 len);
    
/*******************************************************************************
* Function Name  : UART_DMASendData
* Description    : UART DMA????????
* Input          : - UARTx: where x can be 1 to 3 to select the UART peripheral??????UART1/UART2/UART3
*                  - channel:DMA????
*                  - dest??????????????
*                  - len??????????????
*
* Output         : None
* Return         : None
******************************************************************************/
extern UINT16 UART_DMASendData(UART_TypeDef *UARTx,UINT8 channel,UINT8 *src,UINT32 len);


#endif /* UART_DRV_H_ */
