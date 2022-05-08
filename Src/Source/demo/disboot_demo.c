// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : disboot_demo.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "eflash_drv.h"
#include "disboot_demo.h"
#include "type.h"
#include "debug.h"
#include "libEFlash.h"

void Disboot_Demo(void)
{
	UINT32 eflash_clk;
	
	eflash_clk = g_sys_clk;
	
	EFM_Init(eflash_clk);
	EFlash_Set_Main_Permission(EFLASH_WRITEABLE, EFLASH_WRITEABLE);
	
	EFlash_Disboot();
	 
	while(1);
}

