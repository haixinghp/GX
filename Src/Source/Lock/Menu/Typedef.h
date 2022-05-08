/***************************************************************************************
**�ļ���:     Typedef.h
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
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef  __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief    Type define (Compiler Specific)
******************************************************************************/
typedef unsigned char           	BOOLEAN;            	// ��������
typedef unsigned char           	INT8U;              // �޷���8λ���ͱ���
typedef signed   char           	INT8S;              // �з���8λ���ͱ���
typedef unsigned short          	INT16U;             // �޷���16λ���ͱ���
typedef signed   short          	INT16S;             // �з���16λ���ͱ���
typedef unsigned long           	INT32U;             // �޷���32λ���ͱ���
typedef signed   long           	INT32S;             // �з���32λ���ͱ���
typedef float                   	FP32;               // �����ȸ�������32λ���ȣ�
typedef double                  	FP64;               // ˫���ȸ�������64λ���ȣ�

typedef volatile unsigned char  	VBOOLEAN;           // ��������
typedef volatile unsigned char  	VINT8U;             // �޷���8λ���ͱ���
typedef volatile signed   char  	VINT8S;             // �з���8λ���ͱ���
typedef volatile unsigned short 	VINT16U;            // �޷���16λ���ͱ���
typedef volatile signed   short 	VINT16S;            // �з���16λ���ͱ���
typedef volatile unsigned long  	VINT32U;            // �޷���32λ���ͱ���
typedef volatile signed   long  	VINT32S;            // �з���32λ���ͱ���
typedef volatile float          	VFP32;              // �����ȸ�������32λ���ȣ�
typedef volatile double         	VFP64;              // ˫���ȸ�������64λ���ȣ�

typedef char                      B1;									//

typedef char                			I8;									//
typedef short               			I16; 								//
typedef long               				I32; 								//

typedef char const        				IC8;								//
typedef short const           		IC16; 							//
typedef int const              		IC32;  							//

typedef volatile char        			VI8;								//
typedef volatile short        		VI16; 							//
typedef volatile int    					VI32; 							//

typedef volatile char const       VIC8;								//
typedef volatile short const   		VIC16; 							//
typedef volatile int const    		VIC32;							//

typedef signed   char           	S8;									//
typedef signed   short            S16; 								//
typedef signed   long             S32; 								//
 
typedef signed   char const       SC8;								//
typedef signed   short const     	SC16; 							//
typedef signed   int const        SC32;  							//

typedef volatile signed char       	VS8;							//
typedef volatile signed short      	VS16; 						//
typedef volatile signed int        	VS32; 						//

typedef volatile signed char const 	VSC8;							//
typedef volatile signed short const	VSC16; 						//
typedef volatile signed int const  	VSC32;						//

typedef unsigned char              	U8; 							//
typedef unsigned short             	U16; 							//
typedef unsigned long               U32; 				    	//
typedef unsigned char               u8; 							//
typedef unsigned short              u16; 							//
typedef unsigned long               u32; 				    	//


typedef unsigned char const        	UC8;				    	//
typedef unsigned short const        UC16; 						//
typedef unsigned int const          UC32;  						//

typedef volatile unsigned char      VU8;							//
typedef volatile unsigned short     VU16; 						//
typedef volatile unsigned int       VU32; 						//

typedef volatile unsigned char  const VUC8;				    //
typedef volatile unsigned short const VUC16; 					//
typedef volatile unsigned int   const VUC32;					//

typedef float                         FP32;           //     
typedef double                        FP64;						//

typedef  void (*void_Fun_void)(void);									//�޲����ĺ���ָ��,�޷���ֵ
typedef  void (*void_Fun_U8)(U8);											//��һ�������ĺ���ָ��,�޷���ֵ
typedef  U8 (*U8_Fun_U8)(U8);													//��һ�������ĺ���ָ��
typedef  U8 (*U8_Fun_U8U8)(U8,U8);										//�����������ĺ���ָ��
/**************************************************************************//**
 * @brief    Define (Compiler Specific)
******************************************************************************/
#define M_TRUE          1
#define M_FALSE         0
#define M_NOP()         __nop()
//#define M_INLINE        //__INLINE
#define M_INLINE        __inline //by TEST HX 2018.05.08
#define M_NULL          0

/**************************************************************************//**
 * @brief    Type assert
******************************************************************************/
#define ASSERT(expr) assert_param(expr)

#ifdef  __cplusplus
}
#endif

#endif
/***************************************************************************//**
 * @end of file
 ******************************************************************************/
