#include "debug.h"	
#include "MyConfig.h"
#include <cm_backtrace.h>
////////////////////////////////////////////////////////////////////////////////// 	 


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//���HAL��ʹ��ʱ,ĳЩ������ܱ����bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

static UART_TypeDef *UART;

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
#ifdef DEBUG_LOG_EN
//	UART = SCI1;
	UART = SCI2;
	uint32_t overTime =0;
	UART->SCICR2|= SCICR2_TE_MASK;
	
	overTime =0;
	while( (UART->SCISR1&SCISR1_TDRE_MASK)==0x00 )
	{
		overTime ++;
		if(overTime >=10000)
		{
			break;
		}
		;
	}		
	UART->SCIDRL=(UINT8)ch;
	overTime =0;
	while( (UART->SCISR1&SCISR1_TC_MASK)==0x00 )
	{
		overTime ++;
		if(overTime >=10000)
		{
			break;
		}
		;
	}		
	UART->SCICR2&= ~SCICR2_TE_MASK;
#endif
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

									 
//pclk:PCLKʱ��Ƶ��(Mhz)
//bound:������ 
void UART_Debug_Init(UART_TypeDef *UARTx,UINT32 pclk,UINT32 bound)
{  	 
  UINT32 band_rate=0;
	
	UART = UARTx;
	
	UART->SCIBRDF=(((pclk*8/bound)+1)/2)&0x003f;
	band_rate =(pclk*4/bound)>>6;
	UART->SCIBDH=(UINT8)((band_rate>>8)&0x00ff);
	UART->SCIBDL=(UINT8)(band_rate&0x00ff);
	
	UART->SCICR2=0x00;
	UART->SCICR1=0x00;
}

void assert_failed(UINT8* file, UINT32 line)
{
//	cm_backtrace_assert(cmb_get_sp());
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	while(1);
}













