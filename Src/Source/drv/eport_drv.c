// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : eport_drv.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "debug.h"
//#include "iccemv.h"
#include "eport_drv.h"
#include "ccm_drv.h"
#include "ioctrl_drv.h"
#include "DRV_Alarm.h"
#include "DRV_Finger.h"
#include "DRV_Power.h"
unsigned char eport_isr_mark = 0;
/*******************************************************************************
* Function Name  : EPORT0_x_IRQHandler
* Description    : EPORT0_x?жϴ???
* Input          : None
*
* Output         : None
* Return         : None
******************************************************************************/
static void EPORT_IRQHandler(EPORT_TypeDef *EPORTx,EPORT_PINx PINx)
{
	UINT8 status;
	status = EPORTx->EPFR;
	
	printf("enter eport interrupt.current status = %02x\r\n",status);
	
	if(status&(0x01<< PINx))
	{
		EPORTx->EPFR |= 0x01<< PINx;			//clear flag by writing 1 to it.
		if(EPORTx == EPORT)
		{
			printf("Enter EPORT%d interrupt.PIN = %d\r\n",0,PINx);
			DRV_Alarm_InterrptService(PINx);
			DRV_Power_ReuseInterrptService(PINx);
		}
		if(EPORTx == EPORT1)
		{
			printf("Enter EPORT%d interrupt.PIN = %d\r\n",1,PINx);
		}
		if(EPORTx == EPORT2)
		{
			printf("Enter EPORT%d interrupt.PIN = %d\r\n",2,PINx);
		}
		if(EPORTx == EPORT3)
		{
			printf("Enter EPORT%d interrupt.PIN = %d\r\n",3,PINx);
		}
		if(EPORTx == EPORT4)
		{
			printf("Enter EPORT%d interrupt.PIN = %d\r\n",4,PINx);
		}
	}
		
}

void EPORT0_0_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN0);
	EPORT_IRQHandler(EPORT2,EPORT_PIN0);
}

void EPORT0_1_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN1);
	EPORT_IRQHandler(EPORT2,EPORT_PIN1);
}

void EPORT0_2_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN2);
	EPORT_IRQHandler(EPORT2,EPORT_PIN2);
}

void EPORT0_3_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN3);
	EPORT_IRQHandler(EPORT2,EPORT_PIN3);

}

void EPORT0_4_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN4);
	EPORT_IRQHandler(EPORT2,EPORT_PIN4);

}

void EPORT0_5_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN5);
	EPORT_IRQHandler(EPORT2,EPORT_PIN5);
}

void EPORT0_6_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN6);
	EPORT_IRQHandler(EPORT2,EPORT_PIN6);
}

void EPORT0_7_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT,EPORT_PIN7);
	EPORT_IRQHandler(EPORT2,EPORT_PIN7);
}

void EPORT1_0_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN0);
	EPORT_IRQHandler(EPORT3,EPORT_PIN0);
	EPORT_IRQHandler(EPORT4,EPORT_PIN0);
}

void EPORT1_1_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN1);
	EPORT_IRQHandler(EPORT3,EPORT_PIN1);
	EPORT_IRQHandler(EPORT4,EPORT_PIN1);
}

void EPORT1_2_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN2);
	EPORT_IRQHandler(EPORT3,EPORT_PIN2);
	EPORT_IRQHandler(EPORT4,EPORT_PIN2);
}

void EPORT1_3_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN3);
	EPORT_IRQHandler(EPORT3,EPORT_PIN3);
	EPORT_IRQHandler(EPORT4,EPORT_PIN3);
}

void EPORT1_4_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN4);
	EPORT_IRQHandler(EPORT3,EPORT_PIN4);
	EPORT_IRQHandler(EPORT4,EPORT_PIN4);
}

void EPORT1_5_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN5);
	EPORT_IRQHandler(EPORT3,EPORT_PIN5);
	EPORT_IRQHandler(EPORT4,EPORT_PIN5);
}

void EPORT1_6_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN6);
	EPORT_IRQHandler(EPORT3,EPORT_PIN6);
	EPORT_IRQHandler(EPORT4,EPORT_PIN6);
}

void EPORT1_7_IRQHandler(void)
{
	EPORT_IRQHandler(EPORT1,EPORT_PIN7);
	EPORT_IRQHandler(EPORT3,EPORT_PIN7);
	EPORT_IRQHandler(EPORT4,EPORT_PIN7);
}

/*******************************************************************************
* Function Name  : EPORT_ConfigGpio
* Description    : EPORT???ó?GPIO??;
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - GpioDir??????GPIO????   GPIO_OUTPUT??????  GPIO_INPUT??????
*
* Output         : None
* Return         : None
******************************************************************************/
INT8 EPORT_ConfigGpio(EPORT_PINx GpioNo, UINT8 GpioDir)
{
	//UINT32 temp = 0x01;
	assert_param(IS_EPORT_PINx(GpioNo));
	assert_param(IS_GPIO_DIR_BIT(GpioDir));

	if (GpioNo > EPORT_PIN39)
	{
		return -1;
	}
	
	if ((GpioNo >= EPORT_PIN9) && (GpioNo <= EPORT_PIN11))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 0);
	}

	if ((GpioNo >= EPORT_PIN14) && (GpioNo <= EPORT_PIN15))
	{
		*(volatile unsigned int *)(0x4000001c) &= ~(1 << 1);
	}
	
	if ((GpioNo >= EPORT_PIN38) && (GpioNo <= EPORT_PIN39))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 2);
	}
	
	if ((GpioNo >= EPORT_PIN6) && (GpioNo <= EPORT_PIN7))
	{
		*(volatile unsigned int *)(0x4000001c) &= ~(1 << 4);
	}
	
	if ((GpioNo >= EPORT_PIN32) && (GpioNo <= EPORT_PIN37))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 7);
	}

	if ((GpioNo >= EPORT_PIN26) && (GpioNo <= EPORT_PIN31))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 12);
	}
	
	if ((GpioNo >= EPORT_PIN22) && (GpioNo <= EPORT_PIN25))
	{
		*(volatile unsigned int *)(0x40000044) &= ~((UINT32)0xf << 28);
		*(volatile unsigned int *)(0x4000001c) |= (1 << 13);
	}
	
	if ((GpioNo >= EPORT_PIN18) && (GpioNo <= EPORT_PIN21))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 14);
	}
	
	if ((GpioNo >= EPORT_PIN16) && (GpioNo <= EPORT_PIN17))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 15);
	}
	
	if (GpioDir == GPIO_INPUT)//EPORT_EPDDR
	{
		if((GpioNo >>3) == 0)
		{
			EPORT->EPDDR &= ~(1<<(GpioNo - EPORT_PIN0));
		}
		else if((GpioNo >>3) == 1)
		{
			EPORT1->EPDDR &= ~(1<<(GpioNo - EPORT_PIN8));
		}
		else if((GpioNo >>3) == 2)
		{
			EPORT2->EPDDR &= ~(1<<(GpioNo - EPORT_PIN16));
		}
		else if((GpioNo >>3) == 3)
		{
			EPORT3->EPDDR &= ~(1<<(GpioNo - EPORT_PIN24));
		}
		else
		{
			EPORT4->EPDDR &= ~(1<<(GpioNo - EPORT_PIN32));
		}
	}
	else
	{
		if((GpioNo >>3) == 0)
		{
			EPORT->EPDDR |= (1<<(GpioNo- EPORT_PIN0));
		}
		else if((GpioNo >>3) == 1)
		{
			EPORT1->EPDDR |= (1<<(GpioNo - EPORT_PIN8));
		}
		else if((GpioNo >>3) == 2)
		{
			EPORT2->EPDDR |= (1<<(GpioNo - EPORT_PIN16));
		}
		else if((GpioNo >>3) == 3)
		{
			EPORT3->EPDDR |= (1<<(GpioNo - EPORT_PIN24));
		}
		else
		{
			EPORT4->EPDDR |= (1<<(GpioNo - EPORT_PIN32));
		}
	}
	return 0;
}

/*******************************************************************************
* Function Name  : EPORT_WriteGpioData
* Description    : ????EPORT_PINx??Ӧ???ŵĵ?ƽ
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - bitVal?????õĵ?ƽ??Bit_SET??????Ϊ?ߵ?ƽ  Bit_RESET??????Ϊ?͵?ƽ
*
* Output         : None
* Return         : 0: ???óɹ?    other??????ʧ??
******************************************************************************/
void EPORT_WriteGpioData(EPORT_PINx GpioNo, UINT8 bitVal)
{

	assert_param(IS_EPORT_PINx(GpioNo));
	assert_param(IS_GPIO_BIT_ACTION(bitVal));

	if (GpioNo > EPORT_PIN39)
		return;
	if((GpioNo >>3) == 0)
	{
		EPORT->EPDDR |= (1<<(GpioNo - EPORT_PIN0));
		if (bitVal == Bit_SET)
		{
			EPORT->EPDR |= (Bit_SET<<(GpioNo - EPORT_PIN0));
		}
		else
		{
			EPORT->EPDR &= ~(Bit_SET<<(GpioNo - EPORT_PIN0));
		}
	}
	else if((GpioNo >>3) == 1)
	{
		EPORT1->EPDDR |= (1<<(GpioNo - EPORT_PIN8));
		if (bitVal == Bit_SET)
		{
				EPORT1->EPDR |= (Bit_SET<<(GpioNo - EPORT_PIN8));
		}
		else
		{
			EPORT1->EPDR &= ~(Bit_SET<<(GpioNo - EPORT_PIN8));
		}
	}
	else if((GpioNo >>3) == 2)
	{
		EPORT2->EPDDR |= (1<<(GpioNo - EPORT_PIN16));
		if (bitVal == Bit_SET)
		{
				EPORT2->EPDR |= (Bit_SET<<(GpioNo - EPORT_PIN16));
		}
		else
		{
			EPORT2->EPDR &= ~(Bit_SET<<(GpioNo - EPORT_PIN16));
		}
	}
	else if((GpioNo >>3) == 3)
	{
		EPORT3->EPDDR |= (1<<(GpioNo - EPORT_PIN24));
		if (bitVal == Bit_SET)
		{
				EPORT3->EPDR |= (Bit_SET<<(GpioNo - EPORT_PIN24));
		}
		else
		{
			EPORT3->EPDR &= ~(Bit_SET<<(GpioNo - EPORT_PIN24));
		}
	}
	else
	{
		EPORT4->EPDDR |= (1<<(GpioNo - EPORT_PIN32));
		if (bitVal == Bit_SET)
		{
				EPORT4->EPDR |= (Bit_SET<<(GpioNo - EPORT_PIN32));
		}
		else
		{
			EPORT4->EPDR &= ~(Bit_SET<<(GpioNo - EPORT_PIN32));
		}
	}

}

/*******************************************************************************
* Function Name  : EPORT_ReadGpioData
* Description    : ??ȡEPORT_PINx??Ӧ???ŵĵ?ƽ
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*
* Output         : None
* Return         : Bit_SET:?ߵ?ƽ  Bit_RESET???͵?ƽ 
******************************************************************************/
INT8 EPORT_ReadGpioData(EPORT_PINx GpioNo)
{
	INT8  bitstatus = 0x00;

	assert_param(IS_EPORT_PINx(GpioNo));

	if (GpioNo > EPORT_PIN39)
		return -1;
	if((GpioNo >>3) == 0)
	{
		//		EPORTx->EPDDR &= ~(1<<(GpioNo - EPORT_PIN0));//xukai20181209ʵ??Ӧ???У???????Ϊ????ʱ??Ҳ?п??ܶ?ȡ??ƽ״̬?????????ﲻ??????Ϊ???롣
		bitstatus = EPORT->EPPDR;
		if (bitstatus&(Bit_SET<<(GpioNo - EPORT_PIN0)))
		{
			return Bit_SET;
		}
		else
		{
			return Bit_RESET;
		}

	}
	else if((GpioNo >>3) == 1)
	{
		//EPORT1->EPDDR &= ~(1<<(GpioNo - EPORT_PIN8));//xukai20181209ʵ??Ӧ???У???????Ϊ????ʱ??Ҳ?п??ܶ?ȡ??ƽ״̬?????????ﲻ??????Ϊ???롣
		bitstatus = EPORT1->EPPDR;
		if (bitstatus&(Bit_SET<<(GpioNo - EPORT_PIN8)))
		{
			return Bit_SET;
		}
		else
		{
			return Bit_RESET;
		}
	}
	else if((GpioNo >>3) == 2)
	{
		//EPORT2->EPDDR &= ~(1<<(GpioNo - EPORT_PIN16));//xukai20181209ʵ??Ӧ???У???????Ϊ????ʱ??Ҳ?п??ܶ?ȡ??ƽ״̬?????????ﲻ??????Ϊ???롣
		bitstatus = EPORT2->EPPDR;
		if (bitstatus&(Bit_SET<<(GpioNo - EPORT_PIN16)))
		{
			return Bit_SET;
		}
		else
		{
			return Bit_RESET;
		}
	}
	else if((GpioNo >>3) == 3)
	{
		//EPORT3->EPDDR &= ~(1<<(GpioNo - EPORT_PIN24));//xukai20181209ʵ??Ӧ???У???????Ϊ????ʱ??Ҳ?п??ܶ?ȡ??ƽ״̬?????????ﲻ??????Ϊ???롣
		bitstatus = EPORT3->EPPDR;
		if (bitstatus&(Bit_SET<<(GpioNo - EPORT_PIN24)))
		{
			return Bit_SET;
		}
		else
		{
			return Bit_RESET;
		}
	}
	else
	{
		//EPORT4->EPDDR &= ~(1<<(GpioNo - EPORT_PIN32));//xukai20181209ʵ??Ӧ???У???????Ϊ????ʱ??Ҳ?п??ܶ?ȡ??ƽ״̬?????????ﲻ??????Ϊ???롣
		bitstatus = EPORT4->EPPDR;
		if (bitstatus&(Bit_SET<<(GpioNo - EPORT_PIN32)))
		{
			return Bit_SET;
		}
		else
		{
			return Bit_RESET;
		}
	}

}


/*******************************************************************************
* Function Name  : EPORT_ITTypeConfig
* Description    : ????EPORT_PINx?ŵ??жϴ?????ʽ????ʹ???ж?
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - IntMode:?жϴ?????ʽ
*                            LOW_LEVEL_INT: ?͵?ƽ????
*                            HIGH_LEVEL_INT: ?ߵ?ƽ????
*                            RISING_EDGE_INT:?????ش???
*                            FALLING_EDGE_INT:?½??ش???
*                            RISING_FALLING_EDGE_INT:?????ػ??½??ش???
*
* Output         : None
* Return         : None
******************************************************************************/
void EPORT_ITTypeConfig( EPORT_PINx IntNo, EPORT_INT_MODE IntMode)
{
	assert_param(IS_EPORT_PINx(IntNo));
	assert_param(IS_EPORT_INT_MODE(IntMode));

	if (IntNo > EPORT_PIN39)
		return;
	
	if ((IntNo >= EPORT_PIN9) && (IntNo <= EPORT_PIN11))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 0);
	}

	if ((IntNo >= EPORT_PIN14) && (IntNo <= EPORT_PIN15))
	{
		*(volatile unsigned int *)(0x4000001c) &= ~(1 << 1);
	}
	
	if ((IntNo >= EPORT_PIN38) && (IntNo <= EPORT_PIN39))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 2);
	}
	
	if ((IntNo >= EPORT_PIN6) && (IntNo <= EPORT_PIN7))
	{
		*(volatile unsigned int *)(0x4000001c) &= ~(1 << 4);
	}
	
	if ((IntNo >= EPORT_PIN32) && (IntNo <= EPORT_PIN37))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 7);
	}

	if ((IntNo >= EPORT_PIN26) && (IntNo <= EPORT_PIN31))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 12);
	}
	
	if ((IntNo >= EPORT_PIN22) && (IntNo <= EPORT_PIN25))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 13);
	}
	
	if ((IntNo >= EPORT_PIN18) && (IntNo <= EPORT_PIN21))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 14);
	}
	
	if ((IntNo >= EPORT_PIN16) && (IntNo <= EPORT_PIN17))
	{
		*(volatile unsigned int *)(0x4000001c) |= (1 << 15);
	}

	if((IntNo >>3) == 0)
	{
		EPORT->EPDDR &= ~(0x01<<(IntNo - EPORT_PIN0));		//????
	}
	else if((IntNo >>3) == 1)
	{
		EPORT1->EPDDR &= ~(0x01<<(IntNo - EPORT_PIN8));		//????
	}
	else if((IntNo >>3) == 2)
	{
		EPORT2->EPDDR &= ~(0x01<<(IntNo - EPORT_PIN16));		//????
	}
	else if((IntNo >>3) == 3)
	{
		EPORT3->EPDDR &= ~(0x01<<(IntNo - EPORT_PIN24));		//????
	}
	else
	{
		EPORT4->EPDDR &= ~(0x01<<(IntNo - EPORT_PIN32));		//????
	}

	switch (IntMode)
	{
		case LOW_LEVEL_INT:			//?͵?ƽ????
		{
			if((IntNo >>3) == 0)
			{
				EPORT->EPPAR &= ~(0x0003<<((IntNo-EPORT_PIN0)*2));
			}
			else if((IntNo >>3) == 1)
			{
				EPORT1->EPPAR &= ~(0x0003<<((IntNo-EPORT_PIN8)*2));
			}
			else if((IntNo >>3) == 2)
			{
				EPORT2->EPPAR &= ~(0x0003<<((IntNo-EPORT_PIN16)*2));
			}
			else if((IntNo >>3) == 3)
			{
				EPORT3->EPPAR &= ~(0x0003<<((IntNo-EPORT_PIN24)*2));
			}
			else
			{
				EPORT4->EPPAR &= ~(0x0003<<((IntNo-EPORT_PIN32)*2));
			}
			break;
		}
		case HIGH_LEVEL_INT:			//?ߵ?ƽ????
		{
			if((IntNo >>3) == 0)
			{
				EPORT->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN0)*2));
				EPORT->EPLPR |= (0x01<<(IntNo - EPORT_PIN0));
			}
			else if((IntNo >>3) == 1)
			{
				EPORT1->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN8)*2));
				EPORT1->EPLPR |= (0x01<<(IntNo - EPORT_PIN8));
			}
			else if((IntNo >>3) == 2)
			{
				EPORT2->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN16)*2));
				EPORT2->EPLPR |= (0x01<<(IntNo - EPORT_PIN16));
			}
			else if((IntNo >>3) == 3)
			{
				EPORT3->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN24)*2));
				EPORT3->EPLPR |= (0x01<<(IntNo - EPORT_PIN24));
			}
			else
			{
				EPORT4->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN32)*2));
				EPORT4->EPLPR |= (0x01<<(IntNo - EPORT_PIN32));
			}
			break;
		}
		case RISING_EDGE_INT:			//?????ش???
		{
			if((IntNo >>3) == 0)
			{
				EPORT->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN0)*2));
				EPORT->EPPAR |= (0x0001<<((IntNo - EPORT_PIN0)*2));
			}
			else if((IntNo >>3) == 1)
			{
				EPORT1->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN8)*2));
				EPORT1->EPPAR |= (0x0001<<((IntNo - EPORT_PIN8)*2));
			}
			else if((IntNo >>3) == 2)
			{
				EPORT2->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN16)*2));
				EPORT2->EPPAR |= (0x0001<<((IntNo - EPORT_PIN16)*2));
			}
			else if((IntNo >>3) == 3)
			{
				EPORT3->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN24)*2));
				EPORT3->EPPAR |= (0x0001<<((IntNo - EPORT_PIN24)*2));
			}
			else
			{
				EPORT4->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN32)*2));
				EPORT4->EPPAR |= (0x0001<<((IntNo - EPORT_PIN32)*2));
			}
			break;
		}
		case FALLING_EDGE_INT:			//?½??ش???
		{
			if((IntNo >>3) == 0)
			{
				EPORT->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN0)*2));
				EPORT->EPPAR |= (0x0002<<((IntNo - EPORT_PIN0)*2));
			}
			else if((IntNo >>3) == 1)
			{
				EPORT1->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN8)*2));
				EPORT1->EPPAR |= (0x0002<<((IntNo - EPORT_PIN8)*2));
			}
			else if((IntNo >>3) == 2)
			{
				EPORT2->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN16)*2));
				EPORT2->EPPAR |= (0x0002<<((IntNo - EPORT_PIN16)*2));
			}
			else if((IntNo >>3) == 3)
			{
				EPORT3->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN24)*2));
				EPORT3->EPPAR |= (0x0002<<((IntNo - EPORT_PIN24)*2));
			}
			else
			{
				EPORT4->EPPAR &= ~(0x0003<<((IntNo - EPORT_PIN32)*2));
				EPORT4->EPPAR |= (0x0002<<((IntNo - EPORT_PIN32)*2));
			}
			break;
		}
		case RISING_FALLING_EDGE_INT:			//?????غ??½??ض?????
		{
			if((IntNo >>3) == 0)
			{
				EPORT->EPPAR |= (0x0003<<((IntNo - EPORT_PIN0)*2));
			}
			else if((IntNo >>3) == 1)
			{
				EPORT1->EPPAR |= (0x0003<<((IntNo - EPORT_PIN8)*2));
			}
			else if((IntNo >>3) == 2)
			{
				EPORT2->EPPAR |= (0x0003<<((IntNo - EPORT_PIN16)*2));
			}
			else if((IntNo >>3) == 3)
			{
				EPORT3->EPPAR |= (0x0003<<((IntNo - EPORT_PIN24)*2));
			}
			else
			{
				EPORT4->EPPAR |= (0x0003<<((IntNo - EPORT_PIN32)*2));
			}
			break;
		}
		default:
		{
			break;
		}
	}
	//ʹ???ж?
	if((IntNo >>3) == 0)
	{
		EPORT->EPIER |= (0x01<<(IntNo - EPORT_PIN0));
	}
	else if((IntNo >>3) == 1)
	{
		EPORT1->EPIER |= (0x01<<(IntNo - EPORT_PIN8));
	}
	else if((IntNo >>3) == 2)
	{
		EPORT2->EPIER |= (0x01<<(IntNo - EPORT_PIN16));
	}
	else if((IntNo >>3) == 3)
	{
		EPORT3->EPIER |= (0x01<<(IntNo - EPORT_PIN24));
	}
	else
	{
		EPORT4->EPIER |= (0x01<<(IntNo - EPORT_PIN32));
	}
}


/*******************************************************************************
* Function Name  : EPORT_ITConfig
* Description    : EPORT?ж?ʹ?ܿ???
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - NewState??new state of the specified EPORT interrupts.
*                              This parameter can be: ENABLE or DISABLE.
*
* Output         : None
* Return         : None
******************************************************************************/
void EPORT_ITConfig(EPORT_PINx IntNo, FunctionalState NewState)
{
	assert_param(IS_EPORT_PINx(IntNo));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (IntNo > EPORT_PIN39)
		return;
	
	if (NewState != DISABLE)
	{
		if((IntNo >>3) == 0)
		{
			EPORT->EPIER |= (0x01<<(IntNo - EPORT_PIN0));		//ʹ???ж?
		}
		else if((IntNo >>3) == 1)
		{
			EPORT1->EPIER |= (0x01<<(IntNo - EPORT_PIN8));		//ʹ???ж?
		}
		else if((IntNo >>3) == 2)
		{
			EPORT2->EPIER |= (0x01<<(IntNo - EPORT_PIN16));		//ʹ???ж?
		}
		else if((IntNo >>3) == 3)
		{
			EPORT3->EPIER |= (0x01<<(IntNo - EPORT_PIN24));		//ʹ???ж?
		}
		else
		{
			EPORT4->EPIER |= (0x01<<(IntNo - EPORT_PIN32));		//ʹ???ж?
		}
	}
	else
	{
		if((IntNo >>3) == 0)
		{
			EPORT->EPIER &= ~(0x01<<(IntNo - EPORT_PIN0));		//disable?ж?
		}
		else if((IntNo >>3) == 1)
		{
			EPORT1->EPIER &= ~(0x01<<(IntNo - EPORT_PIN8));		//disable?ж?
		}
		else if((IntNo >>3) == 2)
		{
			EPORT2->EPIER &= ~(0x01<<(IntNo - EPORT_PIN16));		//disable?ж?
		}
		else if((IntNo >>3) == 3)
		{
			EPORT3->EPIER &= ~(0x01<<(IntNo - EPORT_PIN24));		//disable?ж?
		}
		else
		{
			EPORT4->EPIER &= ~(0x01<<(IntNo - EPORT_PIN32));		//disable?ж?
		}
	}
}

/*******************************************************************************
* Function Name  : EPORT_PullupConfig
* Description    : EPORT????ʹ?ܿ???
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - NewState??new state of the specified EPORT Pullup.
*                              This parameter can be: ENABLE or DISABLE.
*
* Output         : None
* Return         : None
******************************************************************************/
void EPORT_PullupConfig(EPORT_PINx IntNo, FunctionalState NewState)
{
	assert_param(IS_EPORT_PINx(IntNo));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{
		if((IntNo >>3) == 0)
		{
			EPORT->EPPUER |= (0x01<<(IntNo - EPORT_PIN0));
		}
		else if((IntNo >>3) == 1)
		{
			EPORT1->EPPUER |= (0x01<<(IntNo - EPORT_PIN8));
		}
		else if((IntNo >>3) == 2)
		{
			EPORT2->EPPUER |= (0x01<<(IntNo - EPORT_PIN16));
		}
		else if((IntNo >>3) == 3)
		{
			EPORT3->EPPUER |= (0x01<<(IntNo - EPORT_PIN24));
		}
		else
		{
			EPORT4->EPPUER |= (0x01<<(IntNo - EPORT_PIN32));
		}
	}
	else
	{
		if((IntNo >>3) == 0)
		{
			EPORT->EPPUER &= ~(0x01<<(IntNo - EPORT_PIN0));
		}
		else if((IntNo >>3) == 1)
		{
			EPORT1->EPPUER &= ~(0x01<<(IntNo - EPORT_PIN8));
		}
		else if((IntNo >>3) == 2)
		{
			EPORT2->EPPUER &= ~(0x01<<(IntNo - EPORT_PIN16));
		}
		else if((IntNo >>3) == 3)
		{
			EPORT3->EPPUER &= ~(0x01<<(IntNo - EPORT_PIN24));
		}
		else
		{
			EPORT4->EPPUER &= ~(0x01<<(IntNo - EPORT_PIN32));
		}
	}
}
/*******************************************************************************
* Function Name  : EPORT_PullControl
* Description    : EPORT??????????
* Input          : - EPORT_PINx: EPORT Pin??where x can be 0~15 to select the EPORT peripheral.
*                  - NewState??new state of the specified EPORT Pullup.
*                              This parameter can be: ENABLE or DISABLE.
*
* Output         : None
* Return         : None
******************************************************************************/
void EPORT_PullUPControl(EPORT_PINx IntNo, FunctionalState NewState)
{
	if (NewState == DISABLE)//????
		{
			if((IntNo >>3) == 0)  //gint0~gint7
				{
					IOCTRL->GINTL_CONTROL_REG &= ~(0x01<<(IntNo - EPORT_PIN0)+24);
				}
				else if((IntNo >>3) == 1) //gint8~gint15
					{
						IOCTRL->GINTH_CONTROL_REG &= ~(0x01<<(IntNo - EPORT_PIN8)+24);
					}
					else if((IntNo >>3) == 2)//gint16~gint23
					{
						if(IntNo>=EPORT_PIN22&&IntNo<=EPORT_PIN23)
						IOCTRL->GINT_29_22_CONTROL_REG &= ~(0x01<<(IntNo - EPORT_PIN22)+8);
					}
					else if((IntNo >>3) == 2)//gint24~gint31
					{
						if(IntNo>=EPORT_PIN24&&IntNo<=EPORT_PIN29)
						IOCTRL->GINT_29_22_CONTROL_REG &= ~(0x01<<(IntNo - EPORT_PIN22)+8);
					}
		}
		else//????
		{
			if((IntNo >>3) == 0)  //gint0~gint7
				{
					IOCTRL->GINTL_CONTROL_REG |= (0x01<<(IntNo - EPORT_PIN0)+24);
				}
				else if((IntNo >>3) == 1) //gint8~gint15
					{
						IOCTRL->GINTH_CONTROL_REG |= (0x01<<(IntNo - EPORT_PIN8)+24);
					}
					else if((IntNo >>3) == 2)//gint16~gint23
					{
						if(IntNo>=EPORT_PIN22&&IntNo<=EPORT_PIN23)
						IOCTRL->GINT_29_22_CONTROL_REG |= (0x01<<(IntNo - EPORT_PIN22)+8);
					}
					else if((IntNo >>3) == 2)//gint24~gint31
					{
						if(IntNo>=EPORT_PIN24&&IntNo<=EPORT_PIN29)
						IOCTRL->GINT_29_22_CONTROL_REG |= (0x01<<(IntNo - EPORT_PIN22)+8);
					}
		}
					
	
}
/*******************************************************************************
* Function Name  : EPORT_Init
* Description    : EPORT??ʼ??
* Input          : -PINx   ?ж????ź?
*                  -IntMode ?ж?ģʽ
*
* Output         : None
* Return         : None
******************************************************************************/
extern void EPORT_Init(EPORT_PINx PINx,EPORT_INT_MODE IntMode)
{
	EPORT_ITConfig(PINx, DISABLE);
	if(((PINx>>3) == 0) ||((PINx>>3) == 2)) //EPORT0 ?? EPORT2
	{
		NVIC_Init(3, 3, (EPORT0_0_IRQn) + (PINx&7), 2);
	}
	else
	{
		NVIC_Init(3, 3, (EPORT1_0_IRQn) + (PINx&7), 2);
	}
	
	EPORT_ITTypeConfig(PINx, IntMode);
	EPORT_ITConfig(PINx, ENABLE);
}

