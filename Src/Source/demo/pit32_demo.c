
#include "debug.h"
#include "pit32_drv.h"
#include "pit32_demo.h"
#include "delay.h"

void PIT32_Demo(void)
{
//	UINT32 i;
	
//	PIT32_Init(PIT32_CLK_DIV_64, 0xfffff,TRUE);
	
	while(1)
	{
		DelayMS(10);
	//	printf("pit32 count: 0x%05x\r\n", PIT32_ReadCNTR());
	}
}

