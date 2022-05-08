/***************************************************************************************
**文件名:     HAL_OnChipFlash.h
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
#ifndef   __HAL_ONCHIP_FLASH_H__
#define   __HAL_ONCHIP_FLASH_H__
#include "MyConfig.h"
#define HAL_ONCHIP_FLASH_MODULE_EN (1) //模块使能
#if HAL_ONCHIP_FLASH_MODULE_EN
/*************************专用宏定义*************************/  

/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef HAL_ONCHIP_FLASH_GLOBALS
#define HAL_ONCHIP_FLASH_EXT
#else
#define HAL_ONCHIP_FLASH_EXT extern
#endif     /*HAL_ONCHIP_FLASH_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
HAL_ONCHIP_FLASH_EXT void HAL_OnChipFlash_Init(void);
HAL_ONCHIP_FLASH_EXT UINT8 HAL_OnChipFlash_EraseBytes(UINT32 addr,UINT32 size);
HAL_ONCHIP_FLASH_EXT UINT8 HAL_OnChipFlash_Write(UINT32 flash_addr, const UINT8* ram_addr, UINT32 len);
HAL_ONCHIP_FLASH_EXT UINT32 HAL_OnChipFlash_CheckSum(UINT32 Addr, UINT32 ByteLength);
HAL_ONCHIP_FLASH_EXT void HAL_OnChipFlash_ReadNWords(UINT32 addr, UINT32 *des, UINT32 len);
HAL_ONCHIP_FLASH_EXT UINT8 HAL_OnChipFlash_WirteNWords(UINT32 addr, UINT32 *des, UINT32 len);
HAL_ONCHIP_FLASH_EXT void HAL_OnChipFlash_ReadBytes(UINT32 addr, UINT8 *des, UINT32 len);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*HAL_ONCHIP_FLASH_MODULE_EN*/
#endif    /*HAL_ONCHIP_FLASH_MODULE_EN*/
#endif     /*__HAL_ONCHIP_FLASH_H__*/
/************************************Code End******************************************/

