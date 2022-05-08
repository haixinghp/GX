/***************************************************************************************
**文件名:     Typedef.h
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
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef  __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief    Type define (Compiler Specific)
******************************************************************************/
typedef unsigned char           	BOOLEAN;            	// 布尔变量
typedef unsigned char           	INT8U;              // 无符号8位整型变量
typedef signed   char           	INT8S;              // 有符号8位整型变量
typedef unsigned short          	INT16U;             // 无符号16位整型变量
typedef signed   short          	INT16S;             // 有符号16位整型变量
typedef unsigned long           	INT32U;             // 无符号32位整型变量
typedef signed   long           	INT32S;             // 有符号32位整型变量
typedef float                   	FP32;               // 单精度浮点数（32位长度）
typedef double                  	FP64;               // 双精度浮点数（64位长度）

typedef volatile unsigned char  	VBOOLEAN;           // 布尔变量
typedef volatile unsigned char  	VINT8U;             // 无符号8位整型变量
typedef volatile signed   char  	VINT8S;             // 有符号8位整型变量
typedef volatile unsigned short 	VINT16U;            // 无符号16位整型变量
typedef volatile signed   short 	VINT16S;            // 有符号16位整型变量
typedef volatile unsigned long  	VINT32U;            // 无符号32位整型变量
typedef volatile signed   long  	VINT32S;            // 有符号32位整型变量
typedef volatile float          	VFP32;              // 单精度浮点数（32位长度）
typedef volatile double         	VFP64;              // 双精度浮点数（64位长度）

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

typedef  void (*void_Fun_void)(void);									//无参数的函数指针,无返回值
typedef  void (*void_Fun_U8)(U8);											//带一个参数的函数指针,无返回值
typedef  U8 (*U8_Fun_U8)(U8);													//带一个参数的函数指针
typedef  U8 (*U8_Fun_U8U8)(U8,U8);										//带两个参数的函数指针
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
