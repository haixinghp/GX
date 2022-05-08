/***************************************************************************************
**文件名:     Maths.c
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
#define MATHS_GLOBALS

#include "Maths.h"

/**************************************************************************//**
 * @brief    Local Function
******************************************************************************/

/**************************************************************************//**
 * @brief    Global Function
******************************************************************************/
 /***************************************************************************************
**函数名:       fn_CompareByteArrar
**功能描述:     
比较两个数组指定长度的内容是否一样
**输入参数: 
**输出参数: 
INT8U
0：不同；1：相同
**备注:         
****************************************************************************************/  
INT8U fn_CompareByteArrar(const INT8U *p_u8_GoalData, INT8U u8_GoalBytes, const INT8U *p_u8_SourceData, INT8U u8_SourceBytes)
{
	if(u8_GoalBytes == 0)
	{
		return M_FALSE;
	}
    if (u8_SourceBytes != u8_GoalBytes)
    {
        return M_FALSE;
    }
    for (; u8_SourceBytes > 0; u8_SourceBytes--, p_u8_SourceData++, p_u8_GoalData++)
    {
        if (*p_u8_SourceData != *p_u8_GoalData)
        {
            return M_FALSE;
        }
    }

    return M_TRUE;
}

/***************************************************************************************
**函数名:       fn_GetSumCheck
**功能描述:     
计算数组的校验和
**输入参数: 
**输出参数: 
INT8U
**备注:         
****************************************************************************************/ 
INT8U fn_GetSumCheck(INT8U *p_u8_Data, INT8U u8_Bytes)
{
    INT8U u8_reValue = 0;

    for (; u8_Bytes > 0; u8_Bytes--, p_u8_Data++)
    {
        u8_reValue += *p_u8_Data;
    }

    return u8_reValue;
}

/***************************************************************************************
**函数名:       fn_GetSumCheck
**功能描述:     
计算数组的校验和
**输入参数: 
**输出参数: 
INT8U
**备注:         
****************************************************************************************/ 
INT16U fn_Get16SumCheck(INT8U *p_u8_Data, INT8U u8_Bytes)
{
    INT16U u16_reValue = 0;

    for (; u8_Bytes > 0; u8_Bytes--, p_u8_Data++)
    {
        u16_reValue += *p_u8_Data;
    }

    return u16_reValue;
}

/***************************************************************************************
**函数名:       fn_GetCrc8Check
**功能描述:     
计算数组CRC8
**输入参数: 	
**输出参数: 
INT8U
**备注:         
****************************************************************************************/ 
INT8U fn_GetCrc8Check(INT8U *p_u8_Source, INT8U u8_Bytes)
{
    INT8U u8_Crc8 = 0;//初始化寄存器
    INT8U u8_CrcTemp;
    INT8U i;
    INT8U j;

    for (i = 0; i < u8_Bytes; i++)
    {
        u8_CrcTemp = p_u8_Source[i];
        for (j = 0; j < 8; j++)
        {
            if (((u8_CrcTemp ^ u8_Crc8) & 0x01) == 0)
            {
                u8_Crc8 >>= 1;
            }
            else
            {
                u8_Crc8 ^= 0x18;
                u8_Crc8 >>= 1;
                u8_Crc8 |= 0x80;
            }
            u8_CrcTemp >>= 1;
        }
    }
    return u8_Crc8;
}


/***************************************************************************************
**函数名:       fn_GetCrc16
**功能描述:     
得到一组数据的CRC16
**输入参数: 
const INT8U *u8_pSource, 
数据数组
INT8U u8_Bytes
数据字节数
**输出参数: 
INT8U
**备注:         
****************************************************************************************/ 
INT16U fn_GetCrc16(const INT8U *u8_pSource, INT8U u8_Bytes)
{
    INT16U u16_CrcValue;
    INT8U a_u8_CrcValue[2];
    INT8U u8_aPloy[2] = {0x01, 0xA0};//初始化多项式
    INT8U u8_i = 0;
    INT8U u8_j = 0;
    INT8U u8_Temp;

    a_u8_CrcValue[0] = 0xFF;//初始化寄存器
    a_u8_CrcValue[1] = 0xFF;//初始化寄存器
    for(u8_i = 0; u8_i < u8_Bytes; u8_i++)
    {
        //与寄存器中数据进行异或操作
        a_u8_CrcValue[0] = a_u8_CrcValue[0] ^ u8_pSource[u8_i];
        //移动数据
        for(u8_j = 0; u8_j < 8; u8_j++)
        {
            //获取数据的最后一位，即被移动出的数据判断是否与多项式异或
            u8_Temp = a_u8_CrcValue[0];
            //右移一位
            u16_CrcValue = (a_u8_CrcValue[1] << 8) + a_u8_CrcValue[0];
            u16_CrcValue = u16_CrcValue >> 1;
            a_u8_CrcValue[0] = (INT8U)u16_CrcValue;
            a_u8_CrcValue[1] = (INT8U)(u16_CrcValue >> 8);
            //如果移出数据为1
            if((u8_Temp & 0x01) == 1)
            {
                //与多项式进行异或操作
                a_u8_CrcValue[0] = a_u8_CrcValue[0] ^ u8_aPloy[0];
                a_u8_CrcValue[1] = a_u8_CrcValue[1] ^ u8_aPloy[1];
            }
        }
    }

    u16_CrcValue = (a_u8_CrcValue[1] << 8) + a_u8_CrcValue[0];

    return u16_CrcValue;
}

/***************************************************************************************
**函数名:       fn_CopyArrar
**功能描述:     
 复制一个数组的内容到另一个数组中,程序员要保证数组不会越界
**输入参数: 
INT8U *p_u8_Dest, const INT8U *p_u8_Src, INT16U u16_Bytes, INT16U u16_MaxBytes
目标数据、源数据、字节数，最大字节数
**输出参数: 
INT8U
完成的字节数
**备注:         
****************************************************************************************/ 
INT8U fn_CopyArrar(INT8U *p_u8_Dest, const INT8U *p_u8_Src, INT16U u16_Bytes, INT16U u16_MaxBytes)
{
    INT16U u16_ReValue = 0;
    for (u16_ReValue = 0; u16_ReValue < u16_Bytes && u16_ReValue < u16_MaxBytes; u16_ReValue++)
    {
        *p_u8_Dest++ = *p_u8_Src++;
    }

    return u16_ReValue;
}

/***************************************************************************************
**函数名:       fn_ContainArrar
**功能描述:     
 验证一个源数组是否包含目标数组的内容,程序员要保证数组不会越界
**输入参数: 
INT8U *p_u8_Dest, const INT8U *p_u8_Src, INT16U u16_Bytes, INT16U u16_MaxBytes
目标数据、源数据、字节数，最大字节数
**输出参数: 
INT8U
0:不含; 1:包含
**备注:         
****************************************************************************************/ 
INT8U fn_ContainArrar(const INT8U *p_u8_Dest, INT8U u8_DestBytes, const INT8U *p_u8_Src, INT8U u8_SrcBytes)
{
    INT8U u8_i = 0;
    INT8U u8_j = 0;
    if(u8_DestBytes<1)
	{
		return M_FALSE;//对比数组不能为空
	}
    if (u8_DestBytes > u8_SrcBytes)
    {
        return M_FALSE;//目标数组大于源数组，一定不包含
    }

    for (u8_i = 0; u8_i <= u8_SrcBytes - u8_DestBytes; u8_i++)
    {
        for (u8_j = 0; u8_j < u8_DestBytes; u8_j++)
        {
            if (p_u8_Dest[u8_j] != p_u8_Src[u8_i+u8_j])
            {
                break;
            }
        }
        if (u8_DestBytes == u8_j)
        {
            return M_TRUE;
        }
    }

    return M_FALSE;
}

/***************************************************************************************
**函数名:       fn_StringToInt8U
**功能描述:     
 将一个字符串转为8位无符号整数
**输入参数: 
const INT8U *p_u8_Src

**输出参数: 
INT8U
返回值，错误返回0
**备注:         
****************************************************************************************/ 
INT8S fn_StringToInt8U(const INT8U *p_u8_Src)
{
    if (p_u8_Src[0] < '0' || p_u8_Src[0] > '9')
    {
        return -1;//参数检查
    }
    if (p_u8_Src[1] < '0' || p_u8_Src[1] > '9')
    {
        return -1;//参数检查
    }
    return (p_u8_Src[0] - '0') * 10 + (p_u8_Src[1] - '0');
}

/***************************************************************************//**
 * @end of file
 ******************************************************************************/

