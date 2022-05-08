#include "debug.h"	
#include "MyConfig.h"
#include <cm_backtrace.h>
////////////////////////////////////////////////////////////////////////////////// 	 


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//解决HAL库使用时,某些情况可能报错的bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

static UART_TypeDef *UART;

//重定义fputc函数 
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

									 
//pclk:PCLK时钟频率(Mhz)
//bound:波特率 
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













