/***************************************************************************************
**文件名:     DRV_KeySacn.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
****************************************************************************************/
#include "DRV_KeySacn.h"
/************以下头文件为国芯提供************/
#include "tsi_app.h"
#include "pci_demo.h"
#include "cache_drv.h"
#include "libTSI.h"
#include "i2c_drv.h"
#include "DRV_I2C.h"

#if DRV_KEY_SACN_MODULE_EN
#warning DRV_KEY_SACN模块使能!
/*******************************宏定义***************************/

/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/
static uint8_t gLastKey = 0xff; //上次按键值
/*************************公共全局变量*************************/

/*************************局部函数*****************************/
/***************************************************************************************
**函数名:       GT316_WriteNByte
**功能描述:     写入N个数据
**输入参数:     
uint8_t addr, 地址
uint8_t *data, 数据指针
uint32_t len 数据长度
**输出参数:     
uint8_t
**备注:         
****************************************************************************************/
static uint8_t GT316_WriteNByte(uint8_t addr,uint8_t *data,uint32_t len)
{
	return DRV_I2C_WriteNByte(DRV_KEY_SACN_GT316L_I2C_WRITE_ADDR,
					   addr,
						data,
						len);
}
/***************************************************************************************
**函数名:       GT316_ReadNByte
**功能描述:     读取N个数据
**输入参数:     
uint8_t addr, 地址
uint8_t *data, 数据指针
uint32_t len 数据长度
**输出参数:   
uint8_t
**备注:         
****************************************************************************************/
static int8_t GT316_ReadNByte(uint8_t addr,uint8_t *data,uint32_t len)
{
	return DRV_I2C_ReadNByte(DRV_KEY_SACN_GT316L_I2C_WRITE_ADDR,
					   addr,
						data,
						len);
}
/***************************************************************************************
**函数名:       Touch_GetKeyValue
**功能描述:     读取按键原始值
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       TouchTask_ReadAction
**功能描述:     读取按钮动作
**输入参数:     --
**输出参数:     
keyValue = N;
无按键 keyValue = 0xFF
**备注:         
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
		memset(keyReadTmp,0,sizeof(keyReadTmp)); //清空数据
	}
	if((0 != keyReadTmp[2]) || (0 != keyReadTmp[3]))
	{
		keyAll = (keyReadTmp[3]<<8)|(keyReadTmp[2]); //合成键盘值
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
			if(keyCnt >= 2) //只允许单键操作
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

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_KeySacn_Init
**功能描述:     按键初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_KeySacn_Init(void)
{
	uint8_t ret_val = 0;
	uint8_t init_data[16]={0xff,0x3f,0x30,0x0E,0x13,0x00,0x02,0x00,};
//	uint8_t init_data[16]={0xff,0x3f,0x20,0x0E,0x13,0x00,0x02,0x00,};
	uint8_t tmpTab=0;
	tmpTab = 0x80;
	GT316_WriteNByte(0x06,&tmpTab,1); //写一次软复位
	DelayMS(5);
	GT316_WriteNByte(0x04,init_data,8); 
	memset(init_data, 0x10, 16);// 03最高3f最低
	GT316_WriteNByte(0x10,init_data,16);	
}
/***************************************************************************************
**函数名:       DRV_KeySacn_Init
**功能描述:     按键初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
uint8_t DRV_KeySacn_GetValue(void)
{
    uint8_t reTmp = DRV_KEY_SACN_GET_NOKEY;
    switch (TouchTask_ReadAction()) //获取按键值
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
**函数名:       DRV_KeySacn_EnterSleepConfig
**功能描述:     按键进入休眠初始化
**输入参数:     --
**输出参数:     --
**备注:         
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
**函数名:       DRV_KeySacn_PwmOut
**功能描述:     PWM输出
**输入参数:     --
**输出参数:     --
**备注:         
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