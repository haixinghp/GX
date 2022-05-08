/***************************************************************************************
**�ļ���:     DRV_Storage.c
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
#include "DRV_Storage.h"
#include "ssi_drv.h"
#if DRV_STORAGE_MODULE_EN
#warning DRV_STORAGEģ��ʹ��!
/*******************************�궨��***************************/
#define DRV_STORAGE_SPI SSI1
/*************************.C�����ṹ����***********************/

/*************************˽��ȫ�ֱ���*************************/

/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/
//��ȡоƬID
//����ֵ����:
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;
	//	W25QXX_CS=0;
	//	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����
	//	SPI1_ReadWriteByte(0x00);
	//	SPI1_ReadWriteByte(0x00);
	//	SPI1_ReadWriteByte(0x00);
	//	Temp|=SPI1_ReadWriteByte(0xFF)<<8;
	//	Temp|=SPI1_ReadWriteByte(0xFF);
	//	W25QXX_CS=1;
	return Temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SSI_EFlash_QUAD_Read(DRV_STORAGE_SPI,
						 ReadAddr,
						 pBuffer,
						 NumByteToRead);
	// 	u16 i;
	//	W25QXX_CS=0;                            //ʹ������
	//    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����
	//    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ
	//    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));
	//    SPI1_ReadWriteByte((u8)ReadAddr);
	//    for(i=0;i<NumByteToRead;i++)
	//	{
	//        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������
	//    }
	//	W25QXX_CS=1;
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_QUAD_Program(DRV_STORAGE_SPI,
							WriteAddr,
							pBuffer,
							NumByteToWrite);
	// 	u16 i;
	//    W25QXX_Write_Enable();                  //SET WEL
	//	W25QXX_CS=0;                            //ʹ������
	//    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����
	//    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ
	//    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));
	//    SPI1_ReadWriteByte((u8)WriteAddr);
	//    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��
	//	W25QXX_CS=1;                            //ȡ��Ƭѡ
	//	W25QXX_Wait_Busy();					   //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //������256���ֽ�
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; //д�������
		else	   //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //��ȥ�Ѿ�д���˵��ֽ���
			if (NumByteToWrite > 256)
				pageremain = 256; //һ�ο���д��256���ֽ�
			else
				pageremain = NumByteToWrite; //����256���ֽ���
		}
	};
}

int8_t W25QXX_Write_WithCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	u8 checkTmp[256]={0x00,};
	pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //������256���ֽ�
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		W25QXX_Read(checkTmp, WriteAddr, pageremain);
		for(uint16_t i =0;i<pageremain;i++)
        {
			if(pBuffer[i] != checkTmp[i])
			{
				return -1; //У�����
			}
        }
		
		
		if (NumByteToWrite == pageremain)
			break; //д�������
		else	   //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //��ȥ�Ѿ�д���˵��ֽ���
			if (NumByteToWrite > 256)
				pageremain = 256; //һ�ο���д��256���ֽ�
			else
				pageremain = NumByteToWrite; //����256���ֽ���
		}
	};
	return 0;
}

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
	Dst_Addr *= 4096;
	SSI_EFlash_Write_Enable(SSI1);
	SSI_EFlash_Sector_Erase(DRV_STORAGE_SPI, Dst_Addr);
	//����falsh�������,������
	// 	printf("fe:%x\r\n",Dst_Addr);
	// 	Dst_Addr*=4096;
	//    W25QXX_Write_Enable();                  //SET WEL
	//    W25QXX_Wait_Busy();
	//  	W25QXX_CS=0;                            //ʹ������
	//    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ��
	//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ
	//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));
	//    SPI1_ReadWriteByte((u8)Dst_Addr);
	//	W25QXX_CS=1;                            //ȡ��Ƭѡ
	//    W25QXX_Wait_Busy();   				   //�ȴ��������
}

//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
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
	secpos = WriteAddr / 4096; //������ַ
	secoff = WriteAddr % 4096; //�������ڵ�ƫ��
	secremain = 4096 - secoff; //����ʣ��ռ��С
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; //������4096���ֽ�
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //������������������
		for (i = 0; i < secremain; i++)				  //У������
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break; //��Ҫ����
		}
		if (i < secremain) //��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);	//�����������
			for (i = 0; i < secremain; i++) //����
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			#if 0
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //д����������
			#else
			for(j =0;j<3;j++)
            {
				if(0 == W25QXX_Write_WithCheck(W25QXX_BUF, secpos * 4096, 4096))
				{
				    DEBUG_LOG_OUT("Flash success��______________________________________\n");
					break;
				}
				else
				{
					W25QXX_Erase_Sector(secpos);	//�����������
					for (i = 0; i < secremain; i++) //����
					{
						W25QXX_BUF[i + secoff] = pBuffer[i];
					}
					DEBUG_LOG_OUT("Flash error��______________________________________\n");
				}
            }
			#endif
		}
		else
		{
			#if 0
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
			#else
			for(j =0;j<3;j++)
            {
				if(0 == W25QXX_Write_WithCheck(pBuffer, WriteAddr, secremain))
				{
					 DEBUG_LOG_OUT("Flash success��______________________________________\n");
					break;
				}
				else
				{
					DEBUG_LOG_OUT("Flash error��______________________________________\n");
				}
            }
			#endif
		}
			
		if (NumByteToWrite == secremain)
			break; //д�������
		else	   //д��δ����
		{
			secpos++;	//������ַ��1
			secoff = 0; //ƫ��λ��Ϊ0

			pBuffer += secremain;		 //ָ��ƫ��
			WriteAddr += secremain;		 //д��ַƫ��
			NumByteToWrite -= secremain; //�ֽ����ݼ�
			if (NumByteToWrite > 4096)
				secremain = 4096; //��һ����������д����
			else
				secremain = NumByteToWrite; //��һ����������д����
		}
	};
}


/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       DRV_Storage_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_Storage_Init(void)
{
	SSI_Standard_Init(DRV_STORAGE_SPI); //spi flash ????
	//���洢�豸�ϵ��
}
/***************************************************************************************
**������:       DRV_Storage_ReadWrite
**��������:     ��д
**�������:     
INT8U flag, д����߶�ȡ 0,д�� 1��ȡ
INT32U addr, ������ַ
INT8U *data, ����ָ��
INT16U length ���������ݳ���
**�������:     
INT8S
0 �ɹ�
-1 ͨѶʧ��
**��ע:         �Զ���ҳд��
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
		//д��
		W25QXX_Write(data,addr,length);
		DelayMS(10); //��΢�ȴ�����FLASHд�����
	}
	else
	{
		W25QXX_Read(data,addr,length);
		//��ȡ
	}
	return 0;
}
/***************************************************************************************
**������:       DRV_Storage_EnterSleepConfig
**��������:     ��������
**�������:     --
**�������:     --
**��ע:         
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
**������:       DRV_Storage_EraseSector
**��������:     ����һ������
**�������:     
uint32_t Addr Ŀ���ַ
**�������:     
INT8S
0 �ɹ�
-1 ͨѶʧ��
**��ע:         ��4k����
****************************************************************************************/
INT8S DRV_Storage_EraseSector(uint32_t Addr)
{
//	SSI_Standard_Init(SSI1); //spi flash ????
	W25QXX_Erase_Sector(Addr);
	return 0;
}
#endif /*DRV_STORAGE_MODULE_EN*/
	   /************************************Code End******************************************/