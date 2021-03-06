// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : tc_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "type.h"
#include "debug.h"
#include "tc_drv.h"
#include "tc_demo.h"

#define TC_DEMO_INT_EN

void TC_Demo(void)
{
	int i;

//#ifdef TC_DEMO_INT_EN
//	TC_IntInit(TC_WDP_16, 0x08ff, TRUE);
//#else
//	TC_IntInit(TC_WDP_16, 0x08ff, FALSE);
//#endif
	
	TC_ResetInit(TC_WDP_16, 0x08ff, FALSE);

	for(i=0x6000; i>0; i--)
	{
		if( (i%0x400)==0x00 )
		{
			TC_FeedWatchDog();
			printf("-1- TC count: 0x%04x\r\n", TC_GetCount());
		}
		else
		{
			if( (i%250) == 0x00 )
			{
				printf("-2- TC count: 0x%04x\r\n", TC_GetCount());
			}
		}

	}
	printf("NO feed ...\r\n");
	while(1)
	{
		i++;
		if( (i%0x10000) == 0x00 )
		{
			printf("TC count: 0x%04x\r\n", TC_GetCount());
		}
	}
}

