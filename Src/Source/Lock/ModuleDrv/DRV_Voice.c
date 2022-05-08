/***************************************************************************************
**文件名:     DRV_Voice.c
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   
*版本:V1.00     
*修改者:hhx    
*时间:
*修改说明：                    
创建了这个文件
**备注:      
E0	音量0级(最小)	功能指令
E1	音量1级	
E2	音量2级	
E3	音量3级	
E4	音量4级	
E5	音量5级	
E6	音量6级	
E7	音量7级	
E8	音量8级	
E9	音量9级	
EA	音量10级	
EB	音量11级	
EC	音量12级	
ED	音量13级	
EE	音量14级	
EF	音量15级（最大）	
F2	重复播报（F2+语音地址）	
FE	停止播放当前语音	
F0	队列播放（F0 + 地址A1...+地址A35 + F1）	
注	DATA引脚PRA2,BUSY判忙引脚PRA3	
具体使用参考《语音整合评估20200923_05》
****************************************************************************************/
#include "DRV_Voice.h"
#include "pci_drv.h"
#if DRV_VOICE_MODULE_EN
#warning DRV_VOICE模块使能!
#define BELL_LOW 1
/*******************************宏定义***************************/
#define DRV_VOICE_DELAY_US(X) \
    do                        \
    {                         \
        DelayUS(X);           \
    } while (0)
#define DRV_VOICE_DELAY_MS(X) \
    do                        \
    {                         \
        DelayMS(X);           \
    } while (0)	
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/
//static 
//uint8_t datTmp[30] = {0xf0};
/***************************************************************************************
**函数名:       DRV_Voice_SendData
**功能描述:     语音指令函数 
**输入参数:     
uint8_t dat
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Voice_SendData(uint8_t dat)
{
    uint8_t i;
    uint8_t datTmp = 0;
	datTmp = dat & 0x01;
    for (i = 0; i < 8; i++)
    {
        if (1 == datTmp)
        {
            M_VOICE_OUT_1();
			DRV_VOICE_DELAY_US(1200); // 1.2ms
            M_VOICE_OUT_0();
			DRV_VOICE_DELAY_US(400); // 0.4ms
        }
        else
        {
            M_VOICE_OUT_1();
			DRV_VOICE_DELAY_US(400); // 0.4ms
            M_VOICE_OUT_0();
			DRV_VOICE_DELAY_US(1200); // 1.2ms
        }
        dat = dat >> 1;
		datTmp = dat & 0x01;
    }
}
/***************************************************************************************
**函数名:       DRV_Voice_SendDataN
**功能描述:     语音指令发送多个
**输入参数:     
uint8_t dat
**输出参数:     --
**备注:         
****************************************************************************************/
static void DRV_Voice_SendDataN(uint8_t *dat, uint16_t len)
{
    uint16_t i;
	M_VOICE_OUT_PP(); //数据接口配置为输出
    M_VOICE_OUT_0();
	DRV_VOICE_DELAY_MS(5); // 5ms 发送前导码
    for (i = 0; i < len; i++)
    {
        DRV_Voice_SendData(*dat);
        dat++;
    }
	M_VOICE_OUT_1();
}
/*************************公共全局变量*************************/

/*************************局部函数*****************************/

/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_Voice_Init
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_Init(void)
{
	M_WT_POW_OUT_PP();
	M_WT_POW_OUT_1(); //打开语音电源
	M_BELL_OUT_PP();
#if BELL_LOW >0
	M_BELL_OUT_1(); //初始化门铃
#else
	M_BELL_OUT_0(); //初始化门铃
#endif
	M_VOICE_OUT_PP(); 
    M_VOICE_OUT_1(); //初始化语音接口
}
/***************************************************************************************
**函数名:       DRV_Voice_Play
**功能描述:     播放语音
**输入参数:     
uint8_t dat 语音指令
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_Play(uint8_t dat)
{
        uint8_t datTmp[30] = {0xf0};
    uint8_t len = 0;
    uint8_t i = 0;
    datTmp[i++] = 0xf0;
    switch (dat)
    {
    //0-6拼接 8/9/15/51/67/68/72/73拼接
    //一级菜单
    case 0:
        datTmp[i++] = 106;
        datTmp[i++] = 156;
        datTmp[i++] = 147; //第一行播整个菜单
    case 1:
        datTmp[i++] = 102;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        if (dat != 0)
            break;
    case 2:
        datTmp[i++] = 126;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        if (dat != 0)
            break;
    case 3:
        datTmp[i++] = 110;
        datTmp[i++] = 156;
        datTmp[i++] = 150;
        if (dat != 0)
            break;
    case 4:
        datTmp[i++] = 119;
        datTmp[i++] = 156;
        datTmp[i++] = 151;
        if (dat != 0)
            break;
    case 5:
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 152;
        break; //返回请按6

    //增加人脸
    case 6:
        datTmp[i++] = 107;
        datTmp[i++] = 172;
        datTmp[i++] = 156;
        datTmp[i++] = 147; //第一行播整个菜单
    case 7:
        datTmp[i++] = 108;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        if (dat != 6)
            break;
    case 8:
        datTmp[i++] = 109;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        if (dat != 6)
            break;
    case 9:
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 150;
        break; //返回请按4

    //指纹菜单
    case 10:
        datTmp[i++] = 104;
        datTmp[i++] = 156;
        datTmp[i++] = 147; //增加指纹请按一
        datTmp[i++] = 105;
        datTmp[i++] = 156;
        datTmp[i++] = 148; //删除指纹
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        break; //返回请按3
    case 11:
        datTmp[i++] = 104;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        break; //删除指纹

    //电机扭力菜单
    case 18:
        datTmp[i++] = 111;
        datTmp[i++] = 156;
        datTmp[i++] = 147;
        datTmp[i++] = 112;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        break; //返回请按3
    case 19:
        datTmp[i++] = 112;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        break;

        //语音
    case 20:
        datTmp[i++] = 120;
        datTmp[i++] = 156;
        datTmp[i++] = 147; //关闭语音请按1
        datTmp[i++] = 121;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        break; //返回请按3
    case 21:
        datTmp[i++] = 121;
        datTmp[i++] = 156;
        datTmp[i++] = 148;
        break;

    case 12:
        datTmp[i++] = 144;
        datTmp[i++] = 156;
        datTmp[i++] = 149;
        break; //返回请按3

    ////////////////////////////////
    case 13:
        datTmp[i++] = 19;
        break;
    case 14:
        datTmp[i++] = 20;
        break;
    case 15:
        datTmp[i++] = 58;
        break;
    case 16:
        datTmp[i++] = 23;
        break;
    case 17:
        datTmp[i++] = 21;
        break;
    case 23:
        datTmp[i++] = 46;
        break;
    case 24:
        datTmp[i++] = 47;
        break;
    case 25:
        datTmp[i++] = 44;
        break;
    case 26:
        datTmp[i++] = 45;
        break;
    case 27:
        datTmp[i++] = 54;
        break;
    case 28:
        datTmp[i++] = 73;
        break;
    case 29:
        datTmp[i++] = 35;
        break;
    case 30:
        datTmp[i++] = 34;
        break;
    case 31:
        datTmp[i++] = 74;
		datTmp[i++] = 172;
        break;
    case 32:
        datTmp[i++] = 2;
        break;
    case 33:
        datTmp[i++] = 3;
        break;
    case 34:
        datTmp[i++] = 49;
        break;
    case 35:
        datTmp[i++] = 50;
        break;
    case 36:
        datTmp[i++] = 22;
        break;
    case 37:
        #if 0
        datTmp[i++] = 75;
		datTmp[i++] = 172;
        #endif
        #if 1
        datTmp[i++] = 61;
        #endif
        break;
    case 38:
		datTmp[i++] = 174;
        datTmp[i++] = 31;
        break;
    case 39:
		datTmp[i++] = 174;
        datTmp[i++] = 30;
        break;
    case 40:
		datTmp[i++] = 129;
        datTmp[i++] = 36;
		datTmp[i++] = 168;
        break;
    case 41:
        datTmp[i++] = 6;
        break;
    case 42:
        datTmp[i++] = 33;
        break;
    case 43:
        datTmp[i++] = 66;
        break;
    case 44:
        datTmp[i++] = 76;
        break;
    case 45:
        datTmp[i++] = 77;
        break;
    case 46:
        datTmp[i++] = 78;
        break;
    case 47:
        datTmp[i++] = 79;
        break;
    case 48:
        datTmp[i++] = 80;
        break;
    case 49:
        i = 0;
        datTmp[i++] = 0XF2;
        datTmp[i++] = 32;
        break;
    case 50:
        datTmp[i++] = 29;
        break;
    case 51:
        datTmp[i++] = 63;
        break;
    case 52:
        datTmp[i++] = 41;
        break;
    case 53:
        datTmp[i++] = 41;
        break;
    case 55:
        datTmp[i++] = 64;
        break;
    case 56:
        datTmp[i++] = 65;
        break;
    case 57:
        datTmp[i++] = 48;
        break;
    case 60:
        datTmp[i++] = 71;
        break;
    case 61:
        datTmp[i++] = 44;
        break;
    case 62:
        datTmp[i++] = 69;
        break;
    case 63:
        datTmp[i++] = 70;
        break;
    case 64:
        datTmp[i++] = 52;
        break;
    case 65:
        datTmp[i++] = 81;
        break;
    case 66:
        datTmp[i++] = 82;
        break;
    case 67:
        datTmp[i++] = 83;
        break;
    case 68:
        datTmp[i++] = 84;
        break;
    case 69:
        datTmp[i++] = 85;
        break;
    case 70:
        datTmp[i++] = 15;
        break;
    case 71:
		datTmp[i++] = 191;
		datTmp[i++] = 187;
		datTmp[i++] = 189;
        break;
	case 72:
        datTmp[i++] = 191;
		datTmp[i++] = 188;
		datTmp[i++] = 189;
        break;
	case 73:
        datTmp[i++] = 187;
		datTmp[i++] = 189;
		datTmp[i++] = 174;
		datTmp[i++] = 30;
        break;
	case 74:
        datTmp[i++] = 188;
		datTmp[i++] = 189;
		datTmp[i++] = 174;
		datTmp[i++] = 30;
        break;
	case 75:
        datTmp[i++] = 187;
		datTmp[i++] = 189;
		datTmp[i++] = 174;
		datTmp[i++] = 170;
        break;
	case 76:
        datTmp[i++] = 188;
		datTmp[i++] = 189;
		datTmp[i++] = 174;
		datTmp[i++] = 170;
        break;
//    case 71:
//        datTmp[i++] = 2;
//        break;
//    case 72:
//        datTmp[i++] = 3;
//        break;
	case 0xFE:
		i =0;
		datTmp[i++] = 0xfe;
	break;
    case 161:
        datTmp[i++] = 161;
		datTmp[i++] = 163;
		datTmp[i++] = 168;
        break; //呼叫中，请稍候，挂断，请按星号键。
    case 162:
        datTmp[i++] = 162;
        break;
	case 163:
        datTmp[i++] = 57;
        break;
    case 164:
        datTmp[i++] = 37;
        break;
    default:
        if (dat == 0xf2)
            return;
        datTmp[i++] = dat;
        break;
    }
	if(0xf2 != datTmp[0])
	{
		datTmp[i++] = 0xf1;
	}
    len = i;
	if(3 == len) //如果是单条指令不发F0 * F1
	{
		datTmp[0] = datTmp[1];
		len = 1;
	}
    DRV_Voice_SendDataN(datTmp, len);
}

/***************************************************************************************
**函数名:       DRV_Voice_Bell
**功能描述:     控制门铃开关
**输入参数:     
uint8_t dat
0 开启
其他 关闭
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_Bell(uint8_t dat)
{
#if BELL_LOW >0
	if (0 != dat)
	{
		M_BELL_OUT_0(); //关闭
	}
	else
	{
		M_BELL_OUT_1(); //开启
	}
#else
	if (0 != dat)
	{
		M_BELL_OUT_1(); //开启
	}
	else
	{
		M_BELL_OUT_0(); //关闭
	}
#endif
    
    
}	
/***************************************************************************************
**函数名:       DRV_Voice_EnterSleepConfig
**功能描述:     进入低功耗配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_EnterSleepConfig(void)
{
	M_VOICE_OUT_PP(); 
    M_VOICE_OUT_0();
//	M_WT_POW_IN_NOPULL(); //国芯的SDIO
	M_WT_POW_OUT_PP();
#if BELL_LOW >0 
	M_WT_POW_OUT_1();
#else
	M_WT_POW_OUT_0();
#endif
	
}
/***************************************************************************************
**函数名:       DRV_Voice_EnterSleepConfig
**功能描述:     进入低功耗配置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_BellEnterSleepConfig(void)
{
	M_BELL_OUT_PP();
	M_BELL_OUT_0(); 
}
/***************************************************************************************
**函数名:       DRV_Voice_VolumeSet
**功能描述:     语音音量设置
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Voice_VolumeSet(uint8_t dat)
{
	uint8_t tmp;
	switch (dat)
    {
        #if 0
        case 0:
			tmp = 0xE0;
			DRV_Voice_SendDataN(&tmp, 1);
    		break;
        #endif
    	case 1:
			tmp = 0xE4;
			DRV_Voice_SendDataN(&tmp, 1);
    		break;
    	case 2:
			tmp = 0xE9;
			DRV_Voice_SendDataN(&tmp, 1);
    		break;
		case 3:
			tmp = 0xEF;
			DRV_Voice_SendDataN(&tmp, 1);
    		break;
    	default:
    		break;
    }
}
#endif /*DRV_VOICE_MODULE_EN*/
       /************************************Code End******************************************/