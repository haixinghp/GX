/***************************************************************************************
**�ļ���:     BasicFunc.h
**�汾:       V1.00
**˵��:       �������ݹ���ģ�飬ȫ�ֿ���
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
****************************************************************************************/
#ifndef   __BASIC_FUNC_H__
#define   __BASIC_FUNC_H__
#include "MyConfig.h"
#define BASIC_FUNC_MODULE_EN (1) //ģ��ʹ��
#if BASIC_FUNC_MODULE_EN
/*************************ר�ú궨��*************************/  

/*************************�����ṹ����*************************/  

#ifdef  __cplusplus
extern "C" {
#endif    /*__cplusplus*/

#ifdef BASIC_FUNC_GLOBALS
#define BASIC_FUNC_EXT
#else
#define BASIC_FUNC_EXT extern
#endif     /*BASIC_FUNC_GLOBALS*/

/*************************��������*************************/  
  
/*************************��������*************************/
BASIC_FUNC_EXT uint16_t BasicFunc_GetSumChk(uint8_t *dat, uint16_t len);

#ifdef  __cplusplus
}
#endif    /*__cplusplus*/
#else /*BASIC_FUNC_MODULE_EN*/
#endif    /*BASIC_FUNC_MODULE_EN*/
#endif     /*__BASIC_FUNC_H__*/
/************************************Code End******************************************/

