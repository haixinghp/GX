#include "type.h"
#include "debug.h"
#include "wdt_drv.h"
#include "wdt_demo.h"


void WDT_Demo(void)
{
	int i;

	WDT_Init(0x08ff);		//don't close WDT in startup_ARMCM4.s 

	for(i=0x5000; i>0; i--)
	{
		if( (i%0x400)==0x00 )
		{
			WDT_FeedDog();
			printf("-1- WDT count: 0x%04x\r\n", Get_WDTCount());
		}
		else
		{
			if( (i%150) == 0x00 )
			{
				printf("-2- WDT count: 0x%04x\r\n", Get_WDTCount());
			}
		}

	}
	
//	while(1);

	printf("NO feed ...\r\n");
	while(1)
	{
		i++;
		if( (i%0x1000) == 0x00 )
		{
			printf("WDT count: 0x%04x\r\n", Get_WDTCount());
		}
	}
}
