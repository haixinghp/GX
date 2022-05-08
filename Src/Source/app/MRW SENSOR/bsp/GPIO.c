#include "GPIO.h"
#include "Debug.h"
#include "ccm4202s.h"
#include "sys.h"

void gpioInit(void)
{
	EPORT_ControlConfig_t config;
	config.TriggerMode = EPORT_HIGH_LEVEL_TRIGGER;
	config.DirectionOutput = 1;
	config.EnablePullup = 0;
	config.OpenDrain = 0;
	EPORT_PinControlConfig(&EPORT, 0, &config); //GINT0设为输出

	config.TriggerMode = EPORT_RISING_EDGE_TRIGGER;
	config.DirectionOutput = 0;
	EPORT_PinControlConfig(&EPORT, 4, &config); //GINT4设为输入高电平触发
	EPORT_EnableInterrupt(&EPORT, 4); //允许GINT4中断
	
	NVIC_Init(3, 3, EPORT0_4_IRQn, 2);
	
}


extern U8 g_gint1_int_flag;
void gpioISR(void)
{
	EPORT_ClearInterrupt(&EPORT, 4);
//	g_gint1_int_flag = 1;
}

void gpioSet(U8 level)
{
	EPORT_WritePin(&EPORT, 0, level);
}
