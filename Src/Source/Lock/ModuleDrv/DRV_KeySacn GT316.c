/***************************************************************************************
**�ļ���:     DRV_KeySacn.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ�
**��ע:      
****************************************************************************************/
#include "DRV_KeySacn.h"
/************����ͷ�ļ�Ϊ��о�ṩ************/
#include "tsi_app.h"
#include "pci_demo.h"
#include "cache_drv.h"
#include "libTSI.h"
#include "i2c_drv.h"
#include "DRV_I2C.h"

#if DRV_KEY_SACN_MODULE_EN
#warning DRV_KEY_SACNģ��ʹ��!
/*******************************�궨��***************************/

/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/
static uint8_t gLastKey = 0xff; //�ϴΰ���ֵ
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       GT316_WriteNByte
**��������:     д��N������
**�������:     
uint8_t addr, ��ַ
uint8_t *data, ����ָ��
uint32_t len ���ݳ���
**�������:     
uint8_t
**��ע:         
****************************************************************************************/
static uint8_t GT316_WriteNByte(uint8_t addr,uint8_t *data,uint32_t len)
{
	return DRV_I2C_WriteNByte(DRV_KEY_SACN_GT316L_I2C_WRITE_ADDR,
					   addr,
						data,
						len);
}
/***************************************************************************************
**������:       GT316_ReadNByte
**��������:     ��ȡN������
**�������:     
uint8_t addr, ��ַ
uint8_t *data, ����ָ��
uint32_t len ���ݳ���
**�������:   
uint8_t
**��ע:         
****************************************************************************************/
static int8_t GT316_ReadNByte(uint8_t addr,uint8_t *data,uint32_t len)
{
	return DRV_I2C_ReadNByte(DRV_KEY_SACN_GT316L_I2C_WRITE_ADDR,
					   addr,
						data,
						len);
}
/***************************************************************************************
**������:       Touch_GetKeyValue
**��������:     ��ȡ����ԭʼֵ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static uint8_t Touch_GetKeyValue(void)
{
    uint32_t keyValue = 0;
    uint32_t channel = 0;
    if (app_tsi_wk_read(&channel) == 1)
    {
        keyValue |= (channel);
    }
    else
    {
        return 0xff;
    }

    for (unsigned char i = 0; i < 16; i++)
    {
        if (keyValue & (1 << i))
        {
            //  printf("key %d\n",i);
            return i;
        }
    }
    return 0xff;
}
/***************************************************************************************
**������:       TouchTask_ReadAction
**��������:     ��ȡ��ť����
**�������:     --
**�������:     
keyValue = N;
�ް��� keyValue = 0xFF
**��ע:         
****************************************************************************************/
static uint8_t TouchTask_ReadAction(void)
{
	uint8_t keyReadTmp[4]={0x00,};
	uint16_t keyAll=0;
	uint8_t keyCnt =0;
	uint8_t curKey = DRV_KEY_SACN_GET_NOKEY;
	uint32_t i=0;
	if(0 != GT316_ReadNByte(0x00,keyReadTmp,4)) 
	{
		memset(keyReadTmp,0,sizeof(keyReadTmp)); //�������
	}
	if((0 != keyReadTmp[2]) || (0 != keyReadTmp[3]))
	{
		keyAll = (keyReadTmp[3]<<8)|(keyReadTmp[2]); //�ϳɼ���ֵ
	}
	else
	{
		keyAll = DRV_KEY_SACN_GET_NOKEY;
	}
	for(i=0;i<16;i++)
    {
		if((keyAll >> i) &0x01)
		{
			curKey = i;
			keyCnt ++;
			if(keyCnt >= 2) //ֻ����������
			{
				return DRV_KEY_SACN_GET_NOKEY;
			}
		}
    }
	curKey += 1;
	#if 0
	DEBUG_LOG_OUT("curKey:%d\n",curKey);
	#endif
	return (curKey);
}

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_KeySacn_Init
**��������:     ������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_KeySacn_Init(void)
{
	uint8_t ret_val = 0;
	uint8_t init_data[16]={0xff,0x3f,0x30,0x0E,0x13,0x00,0x02,0x00,};
//	uint8_t init_data[16]={0xff,0x3f,0x20,0x0E,0x13,0x00,0x02,0x00,};
	uint8_t tmpTab=0;
	tmpTab = 0x80;
	GT316_WriteNByte(0x06,&tmpTab,1); //дһ����λ
	DelayMS(5);
	GT316_WriteNByte(0x04,init_data,8); 
	memset(init_data, 0x10, 16);// 03���3f���
	GT316_WriteNByte(0x10,init_data,16);	
}
/***************************************************************************************
**������:       DRV_KeySacn_Init
**��������:     ������ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_KeySacn_GetValue(void)
{
    uint8_t reTmp = DRV_KEY_SACN_GET_NOKEY;
    switch (TouchTask_ReadAction()) //��ȡ����ֵ
    {
    case DRV_KEY_SACN_ORIGINAL_0:
        reTmp = DRV_KEY_SACN_GET_NOKEY;
        break;
    case DRV_KEY_SACN_ORIGINAL_1:
        reTmp = DRV_KEY_SACN_GET_9;
        break;
    case DRV_KEY_SACN_ORIGINAL_2:
        reTmp = DRV_KEY_SACN_GET_8;
        break;
    case DRV_KEY_SACN_ORIGINAL_3:
        reTmp = DRV_KEY_SACN_GET_6;
        break;
    case DRV_KEY_SACN_ORIGINAL_4:
        reTmp = DRV_KEY_SACN_GET_3;
        break;
    case DRV_KEY_SACN_ORIGINAL_5:
        reTmp = DRV_KEY_SACN_GET_2;
        break;
    case DRV_KEY_SACN_ORIGINAL_6:
        reTmp = DRV_KEY_SACN_GET_5;
        break;
    case DRV_KEY_SACN_ORIGINAL_7:
        reTmp = DRV_KEY_SACN_GET_BELL;
        break;
    case DRV_KEY_SACN_ORIGINAL_8:
        reTmp = DRV_KEY_SACN_GET_1;
        break;
    case DRV_KEY_SACN_ORIGINAL_9:
        reTmp = DRV_KEY_SACN_GET_CLOSE;
        break;
    case DRV_KEY_SACN_ORIGINAL_10:
        reTmp = DRV_KEY_SACN_GET_4;
        break;
    case DRV_KEY_SACN_ORIGINAL_11:
        reTmp = DRV_KEY_SACN_GET_7;
        break;
    case DRV_KEY_SACN_ORIGINAL_12:
        reTmp = DRV_KEY_SACN_GET_ENTER;
        break;
    case DRV_KEY_SACN_ORIGINAL_13:
        reTmp = DRV_KEY_SACN_GET_0;
        break;
    case DRV_KEY_SACN_ORIGINAL_14:
        reTmp = DRV_KEY_SACN_GET_MENU;
        break;
    case DRV_KEY_SACN_ORIGINAL_15:
        reTmp = DRV_KEY_SACN_GET_NOKEY;
        break;
    default:
        break;
    }
    return reTmp;
}

/***************************************************************************************
**������:       DRV_KeySacn_EnterSleepConfig
**��������:     �����������߳�ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_KeySacn_EnterSleepConfig(void)
{
	M_KEY_IRQ_IN_IPU();
	M_KEY_IRQ_EXIT_FALL_ENABLE();
//	EPORT_ConfigGpio(EPORT_PIN34, GPIO_INPUT); 
//    EPORT_PullupConfig(EPORT_PIN34, DISABLE);  
//	EPORT_ConfigGpio(EPORT_PIN38, GPIO_INPUT); 
//    EPORT_PullupConfig(EPORT_PIN38, DISABLE);  
}
/***************************************************************************************
**������:       DRV_KeySacn_PwmOut
**��������:     PWM���
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t DRV_KeySacn_PwmOut(uint8_t dat)
{
    uint8_t led[]={0x1f,0x1f,0x1f};
    switch (dat)
    {
    	case 0:
            led[2] = 0;
    		break;
        case 1:
            led[1] = 0;
    		break;
        case 2:
            led[0] = 0;
    		break;
        case 3:
    		break;
    	case 4:
            memset(led,0,sizeof(led));
    		break;
    	default:
    		break;
    }
    GT316_WriteNByte(0x30,led,3); 
    return 0;
}
#endif /*DRV_KEY_SACN_MODULE_EN*/
       /************************************Code End******************************************/