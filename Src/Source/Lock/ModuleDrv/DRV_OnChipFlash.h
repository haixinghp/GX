/***************************************************************************************
**�ļ���:     DRV_OnChipFlash.h
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
#ifndef   __DRV_ONCHIP_FLASH_H__
#define   __DRV_ONCHIP_FLASH_H__
#include "MyConfig.h"
#define DRV_ONCHIP_FLASH_MODULE_EN (1) //ģ��ʹ��
#if DRV_ONCHIP_FLASH_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef DRV_ONCHIP_FLASH_GLOBALS
#define DRV_ONCHIP_FLASH_EXT
#else
#define DRV_ONCHIP_FLASH_EXT extern
#endif     /*DRV_ONCHIP_FLASH_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
DRV_ONCHIP_FLASH_EXT void DRV_OnChipFlash_Init(void);
DRV_ONCHIP_FLASH_EXT UINT8 DRV_OnChipFlash_PageErase(UINT32 addr);
DRV_ONCHIP_FLASH_EXT UINT8 DRV_OnChipFlash_WirteNWords(UINT32 addr, UINT32 *des, UINT32 len);
DRV_ONCHIP_FLASH_EXT void DRV_OnChipFlash_ReadNWords(UINT32 addr, UINT32 *des, UINT32 len);
DRV_ONCHIP_FLASH_EXT UINT8 DRV_OnChipFlash_Write(UINT32 flash_addr, const UINT8* ram_addr, UINT32 len);
DRV_ONCHIP_FLASH_EXT UINT32 DRV_OnChipFlash_CheckSum(UINT32 Addr, UINT32 ByteLength);
DRV_ONCHIP_FLASH_EXT UINT8 DRV_OnChipFlash_EraseBytes(UINT32 addr,UINT32 size);
DRV_ONCHIP_FLASH_EXT void DRV_OnChipFlash_ReadBytes(UINT32 addr, UINT8 *des, UINT32 len);
#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*DRV_ONCHIP_FLASH_MODULE_EN*/
#endif    /*DRV_ONCHIP_FLASH_MODULE_EN*/
#endif     /*__DRV_ONCHIP_FLASH_H__*/
/************************************Code End******************************************/

