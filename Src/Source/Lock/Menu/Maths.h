/***************************************************************************************
**文件名:     Maths.h
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
#ifndef __MATHS_H__
#define __MATHS_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MATHS_GLOBALS
#define MATHS_EXT
#else
#define MATHS_EXT extern
#endif
#include "Myconfig.h"
/**************************************************************************//**
 * @brief    Global Function
******************************************************************************/
MATHS_EXT INT8U fn_CompareByteArrar(const INT8U *p_u8_GoalData, INT8U u8_GoalBytes, const INT8U *p_u8_SourceData, INT8U u8_SourceBytes);
MATHS_EXT INT8U fn_GetSumCheck(INT8U *p_u8_Data, INT8U u8_Bytes);
MATHS_EXT INT16U fn_Get16SumCheck(INT8U *p_u8_Data, INT8U u8_Bytes);
MATHS_EXT INT8U fn_GetCrc8Check(INT8U *p_u8_Source, INT8U u8_Bytes);
MATHS_EXT INT16U fn_GetCrc16(const INT8U *u8_pSource, INT8U u8_Bytes);
MATHS_EXT INT8U fn_CopyArrar(INT8U *p_u8_Dest, const INT8U *p_u8_Src, INT16U u16_Bytes, INT16U u16_MaxBytes);
MATHS_EXT INT8U fn_ContainArrar(const INT8U *p_u8_Dest, INT8U u8_DestBytes, const INT8U *p_u8_Src, INT8U u8_SrcBytes);
MATHS_EXT INT8S fn_StringToInt8U(const INT8U *p_u8_Src);
#ifdef  __cplusplus
}
#endif

#endif
/***************************************************************************//**
 * @end of file
 ******************************************************************************/
