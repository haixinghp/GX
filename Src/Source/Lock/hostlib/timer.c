
//#include "bitband.h"

#include <stdint.h>
#if 0
#include "nrf_delay.h"



/******************************************************/
//soft delayus when system clk @72Mhz
void delayus(uint32_t cnt)
{
	uint32_t i,j;

	for(i=cnt; i>0; i--){
//		for(j=14*3; j>0; j--);
		 nrf_delay_us(1);
	}
}

void delayms(uint32_t cnt)
{
//	delayus(cnt*1000);
	nrf_delay_ms(cnt);
}
#include "app_timer.h"
#endif
//extern volatile uint8_t	my_fmse_timer_enable;
//extern volatile uint32_t	my_fmse_timer_cnt;
//extern volatile uint32_t	my_fmse_timer_timeout_val;
//extern void start_se_timer(void) ;

//extern void stop_se_timer(void) ;

//void init_timeout_ms(uint32_t timeout_ms)
//{
//	
//	my_fmse_timer_enable=1;
//	my_fmse_timer_timeout_val=timeout_ms/1000;
//	my_fmse_timer_cnt=0;
//	
//  start_se_timer() ;//Æô¶¯SE¶¨Ê±Æ÷
//	
////	printf("init_timeout_ms timeout_ms=%d\r\n",timeout_ms);
//	
//	
//}

//uint8_t check_timeout_ms(void)
//{
//	if(my_fmse_timer_cnt>=my_fmse_timer_timeout_val)
//	{
//		my_fmse_timer_enable=0;
//		stop_se_timer( ) ;
//		return 1;
//		
//	}else{
//		return 0;
//	}
//	
//}



