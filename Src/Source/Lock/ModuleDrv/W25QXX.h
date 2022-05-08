/***************************************************************************************
**�ļ���:     W25QXX.h
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
#ifndef   __W25QXX_H__
#define   __W25QXX_H__
//#include "config.h"
//#include "DRV_SPI.h"
#include "MyConfig.h"
#define W25QXX_MODULE_EN (1) //ģ��ʹ��
#if W25QXX_MODULE_EN
/*************************ר�ú궨��*************************/  
#define RW_BUF_SIZE             			(256)
/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef W25QXX_GLOBALS
#define W25QXX_EXT
#else
#define W25QXX_EXT extern
#endif     /*W25QXX_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
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


