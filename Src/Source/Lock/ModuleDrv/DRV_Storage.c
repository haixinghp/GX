/***************************************************************************************
**文件名:     DRV_Storage.c
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
#include "DRV_Storage.h"
#include "ssi_drv.h"
#if DRV_STORAGE_MODULE_EN
#warning DRV_STORAGE模块使能!
/*******************************宏定义***************************/
#define DRV_STORAGE_SPI SSI1
/*************************.C变量结构声明***********************/

/*************************私有全局变量*************************/

/*************************公共全局变量*************************/

/*************************局部函数*****************************/
//读取芯片ID
//返回值如下:
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;
	//	W25QXX_CS=0;
	//	SPI1_ReadWriteByte(0x90);//发送读取ID命令
	//	SPI1_ReadWriteByte(0x00);
	//	SPI1_ReadWriteByte(0x00);
	//	SPI1_ReadWriteByte(0x00);
	//	Temp|=SPI1_ReadWriteByte(0xFF)<<8;
	//	Temp|=SPI1_ReadWriteByte(0xFF);
	//	W25QXX_CS=1;
	return Temp;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SSI_EFlash_QUAD_Read(DRV_STORAGE_SPI,
						 ReadAddr,
						 pBuffer,
						 NumByteToRead);
	// 	u16 i;
	//	W25QXX_CS=0;                            //使能器件
	//    SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令
	//    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址
	//    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));
	//    SPI1_ReadWriteByte((u8)ReadAddr);
	//    for(i=0;i<NumByteToRead;i++)
	//	{
	//        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数
	//    }
	//	W25QXX_CS=1;
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_QUAD_Program(DRV_STORAGE_SPI,
							WriteAddr,
							pBuffer,
							NumByteToWrite);
	// 	u16 i;
	//    W25QXX_Write_Enable();                  //SET WEL
	//	W25QXX_CS=0;                            //使能器件
	//    SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令
	//    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址
	//    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));
	//    SPI1_ReadWriteByte((u8)WriteAddr);
	//    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数
	//	W25QXX_CS=1;                            //取消片选
	//	W25QXX_Wait_Busy();					   //等待写入结束
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //不大于256个字节
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; //写入结束了
		else	   //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //减去已经写入了的字节数
			if (NumByteToWrite > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = NumByteToWrite; //不够256个字节了
		}
	};
}

int8_t W25QXX_Write_WithCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	u8 checkTmp[256]={0x00,};
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //不大于256个字节
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		W25QXX_Read(checkTmp, WriteAddr, pageremain);
		for(uint16_t i =0;i<pageremain;i++)
        {
			if(pBuffer[i] != checkTmp[i])
			{
				return -1; //校验错误
			}
        }
		
		
		if (NumByteToWrite == pageremain)
			break; //写入结束了
		else	   //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //减去已经写入了的字节数
			if (NumByteToWrite > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = NumByteToWrite; //不够256个字节了
		}
	};
	return 0;
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
	Dst_Addr *= 4096;
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_Sector_Erase(DRV_STORAGE_SPI, Dst_Addr);
	//监视falsh擦除情况,测试用
	// 	printf("fe:%x\r\n",Dst_Addr);
	// 	Dst_Addr*=4096;
	//    W25QXX_Write_Enable();                  //SET WEL
	//    W25QXX_Wait_Busy();
	//  	W25QXX_CS=0;                            //使能器件
	//    SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令
	//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址
	//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));
	//    SPI1_ReadWriteByte((u8)Dst_Addr);
	//	W25QXX_CS=1;                            //取消片选
	//    W25QXX_Wait_Busy();   				   //等待擦除完成
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
u8 W25QXX_BUFFER[4096];
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	u16 j;
	u8 *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; //不大于4096个字节
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
		for (i = 0; i < secremain; i++)				  //校验数据
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			W25QXX_Erase_Sector(secpos);	//擦除这个扇区
			for (i = 0; i < secremain; i++) //复制
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			#if 0
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //写入整个扇区
			#else
			for(j =0;j<3;j++)
            {
				if(0 == W25QXX_Write_WithCheck(W25QXX_BUF, secpos * 4096, 4096))
				{
				    DEBUG_LOG_OUT("Flash success！______________________________________\n");
					break;
				}
				else
				{
					W25QXX_Erase_Sector(secpos);	//擦除这个扇区
					for (i = 0; i < secremain; i++) //复制
					{
						W25QXX_BUF[i + secoff] = pBuffer[i];
					}
					DEBUG_LOG_OUT("Flash error！______________________________________\n");
				}
            }
			#endif
		}
		else
		{
			#if 0
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
			#else
			for(j =0;j<3;j++)
            {
				if(0 == W25QXX_Write_WithCheck(pBuffer, WriteAddr, secremain))
				{
					 DEBUG_LOG_OUT("Flash success！______________________________________\n");
					break;
				}
				else
				{
					DEBUG_LOG_OUT("Flash error！______________________________________\n");
				}
            }
			#endif
		}
			
		if (NumByteToWrite == secremain)
			break; //写入结束了
		else	   //写入未结束
		{
			secpos++;	//扇区地址增1
			secoff = 0; //偏移位置为0

			pBuffer += secremain;		 //指针偏移
			WriteAddr += secremain;		 //写地址偏移
			NumByteToWrite -= secremain; //字节数递减
			if (NumByteToWrite > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = NumByteToWrite; //下一个扇区可以写完了
		}
	};
}


/*************************全局函数****************************/
/***************************************************************************************
**函数名:       DRV_Storage_Init
**功能描述:     初始化
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Storage_Init(void)
{
	SSI_Standard_Init(DRV_STORAGE_SPI); //spi flash ????
	//给存储设备上电等
}
/***************************************************************************************
**函数名:       DRV_Storage_ReadWrite
**功能描述:     读写
**输入参数:     
INT8U flag, 写入或者读取 0,写入 1读取
INT32U addr, 操作地址
INT8U *data, 操作指针
INT16U length 操作的数据长度
**输出参数:     
INT8S
0 成功
-1 通讯失败
**备注:         自动分页写入
****************************************************************************************/
INT8S DRV_Storage_ReadWrite(INT8U flag, INT32U addr, INT8U *data, INT16U length)
{
//	INT32U len = 0x100;
//	INT8U tab[0x100] = {0xaa,};
//	INT8U tab1[0x100] = {0x00,};
//	SSI_EFlash_Write_Enable(SSI1);
//	SSI_EFlash_Sector_Erase(SSI1, 0);
//	SSI_EFlash_QUAD_Program(SSI1,
//							0,
//							tab,
//							len);
//	SSI_EFlash_QUAD_Read(SSI1,
//							0,
//							tab1,
//							len);
//	SSI_EFlash_Write_Enable(SSI1);
	if (0 == flag)
	{
		//写入
		W25QXX_Write(data,addr,length);
		DelayMS(10); //稍微等待，让FLASH写入完成
	}
	else
	{
		W25QXX_Read(data,addr,length);
		//读取
	}
	return 0;
}
/***************************************************************************************
**函数名:       DRV_Storage_EnterSleepConfig
**功能描述:     进入休眠
**输入参数:     --
**输出参数:     --
**备注:         
****************************************************************************************/
void DRV_Storage_EnterSleepConfig(void)
{
	M_SPI4_0_OUT_PP();
	M_SPI4_0_OUT_0();
	M_SPI4_1_OUT_PP();
	M_SPI4_1_OUT_0();
	M_SPI4_2_OUT_PP();
	M_SPI4_2_OUT_0();
	M_SPI4_3_OUT_PP();
	M_SPI4_3_OUT_0();
	M_SPI4_SCK_OUT_PP();
	M_SPI4_SCK_OUT_0();
	M_SPI4_SS_OUT_PP();
	M_SPI4_SS_OUT_0();
}
/***************************************************************************************
**函数名:       DRV_Storage_EraseSector
**功能描述:     擦除一个扇区
**输入参数:     
uint32_t Addr 目标地址
**输出参数:     
INT8S
0 成功
-1 通讯失败
**备注:         以4k擦除
****************************************************************************************/
INT8S DRV_Storage_EraseSector(uint32_t Addr)
{
//	SSI_Standard_Init(SSI1); //spi flash ????
	W25QXX_Erase_Sector(Addr);
	return 0;
}
#endif /*DRV_STORAGE_MODULE_EN*/
	   /************************************Code End******************************************/