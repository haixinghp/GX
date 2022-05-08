/***************************************************************************************
**文件名:     W25QXX.h
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
#ifndef   __W25QXX_H__
#define   __W25QXX_H__
//#include "config.h"
//#include "DRV_SPI.h"
#include "MyConfig.h"
#define W25QXX_MODULE_EN (1) //模块使能
#if W25QXX_MODULE_EN
/*************************专用宏定义*************************/  
#define RW_BUF_SIZE             			(256)
/*************************变量结构声明*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef W25QXX_GLOBALS
#define W25QXX_EXT
#else
#define W25QXX_EXT extern
#endif     /*W25QXX_GLOBALS*/

/*************************公共变量*************************/  
  
/*************************公共函数*************************/
W25QXX_EXT uint8_t W25QXX_WriteCMD(uint8_t *CMD);
W25QXX_EXT void W25QXX_EraseSector(uint8_t Block_Num,uint8_t Sector_Number);
W25QXX_EXT int8_t W25QXX_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum);
W25QXX_EXT int8_t W25QXX_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*W25QXX_MODULE_EN*/

#endif    /*W25QXX_MODULE_EN*/
#endif     /*__W25QXX_H__*/
/************************************Code End******************************************/


