/***************************************************************************************
**�ļ���:     DRV_OnChipFlash.c
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
#include "DRV_OnChipFlash.h"
#include "libEFlash.h"
#include "eflash_drv.h"
#if  DRV_ONCHIP_FLASH_MODULE_EN
#warning DRV_ONCHIP_FLASHģ��ʹ��!
/*******************************�궨��***************************/  

/*************************.C�����ṹ����***********************/  

/*************************˽��ȫ�ֱ���*************************/  

/*************************����ȫ�ֱ���*************************/  

/*************************�ֲ�����*****************************/  
/*******************************************************************************
* Function Name  : EFLASH_WordsRead
* Description    :  ��EFLASHĿ���ַ��ȡ����Ϊlen��word����
* Input          : - addr: EFLASH destination address
*                  - len:  ��Ҫ��ȡword���ݵĳ���
*
* Output         : - des:  ��ȡ��word���ݻ���
* Return         : None
******************************************************************************/
static void EFLASH_Words_Read(UINT32 addr, UINT32 *des, UINT32 len)
{
	UINT32 i;

	for(i = 0; i < len; i ++)
	{
		des[i] = (*(volatile UINT32 *)(addr+i*4));
	}
}
/***************************************************************************************
**������:       DRV_OnChipFlash_Unlock
**��������:     EFLASH����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_OnChipFlash_Unlock(void)
{
	EFLASH_SetWritePermission();
}
/***************************************************************************************
**������:       DRV_OnChipFlash_Lock
**��������:     EFLASH����
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
static void DRV_OnChipFlash_Lock(void)
{
	EFLASH_ClrWritePermission();
}
/*************************ȫ�ֺ���****************************/  
/***************************************************************************************
**������:       DRV_OnChipFlash_Init
**��������:     Ƭ��FLASH��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_OnChipFlash_Init(void)
{
    UINT32 eflash_clk;
	eflash_clk = g_sys_clk;
	EFLASH_Init(eflash_clk/1000);
}
/***************************************************************************************
**������:       DRV_OnChipFlash_PageErase
**��������:     EFLASHҳ����
**�������:     
addr: Ҫ������EFLASHҳ��ַ
**�������:     
TRUE���ɹ�   
FALSE��ʧ��
**��ע:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_PageErase(UINT32 addr)
{
	 UINT8 reTmp;
	DRV_OnChipFlash_Unlock();
	 reTmp = EFLASH_PageErase(addr);
	DRV_OnChipFlash_Lock();
	return reTmp;
}

/***************************************************************************************
**������:       DRV_OnChipFlash_EraseBytes
**��������:     flash��������
**�������:     
addr  �����������ڵ���ʼ��ַ
size  �����������ڵ����ݴ�С
**�������:     
TRUE���ɹ�   
FALSE��ʧ��
**��ע:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_EraseBytes(UINT32 addr,UINT32 size)
{
	uint32_t	tp4, sp4, i;
	tp4 = size / FLASH_SECTOR_SIZE;
	  sp4 = size % FLASH_SECTOR_SIZE;
		if( sp4 > 0 )
		{
			tp4++;
		}
	    for(i=0; i<tp4; i++)
		{			
			 if(FALSE ==  DRV_OnChipFlash_PageErase( addr ))
			 {
				 return FALSE;
			 }
			 else
			 {
				 addr += FLASH_SECTOR_SIZE; //����������һ������
			 }
		} 
		return 	TRUE;	
}

/***************************************************************************************
**������:       DRV_OnChipFlash_WordsProg
**��������:     word���
**�������:     
addr: Ҫ��̵�EFLASH��ַ
des:  ��̵�word����ָ��
len:  Ҫ��̵�word���ݳ���
**�������:     
TRUE����̳ɹ�   
FALSE�����ʧ��
**��ע:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_WirteNWords(UINT32 addr, UINT32 *des, UINT32 len)
{
	return EFLASH_WordsProg(addr,des,len);
}
/***************************************************************************************
**������:       DRV_OnChipFlash_WordsProg
**��������:     word���
**�������:     
addr: FLASH��ַ
des:  word����ָ��
len:  word���ݳ���
**�������:     
TRUE����̳ɹ�   
FALSE�����ʧ��
**��ע:         
****************************************************************************************/
void DRV_OnChipFlash_ReadNWords(UINT32 addr, UINT32 *des, UINT32 len)
{
	EFLASH_Words_Read(addr,des,len);
}
/***************************************************************************************
**������:       DRV_OnChipFlash_Write
**��������:     EFLASH�����ַд����
**�������:     
addr: FLASH��ַ
des:  word����ָ��
len:  word���ݳ���
**�������:     
TRUE����̳ɹ�   
FALSE�����ʧ��
**��ע:         
****************************************************************************************/
UINT8 DRV_OnChipFlash_Write(UINT32 flash_addr, const UINT8* ram_addr, UINT32 len)
{
	UINT8 reTmp;
	DRV_OnChipFlash_Unlock();
	reTmp  = EFLASH_Write(flash_addr,ram_addr,len);
	DRV_OnChipFlash_Lock();
	return reTmp;
}


/***************************************************************************************
**������:       DRV_OnChipFlash_CheckSum
**��������:     У��
**�������:     
Addr ��ַ
ByteLength �ֽڳ���
**�������:     --
**��ע:         
****************************************************************************************/
UINT32 DRV_OnChipFlash_CheckSum(UINT32 Addr, UINT32 ByteLength)
{
    UINT32 smu =0;
	  UINT32 i;
    
    for (i=0; i<ByteLength; i++)
    {
       smu += *((unsigned char *)(Addr+i));
    }
		return ( smu % ( 2^32 ) );
}
/*******************************************************************************
* Function Name  : DRV_OnChipFlash_ReadBytes
* Description    :  ��EFLASHĿ���ַ��ȡ����Ϊlen��Bytes����
* Input          : - addr: EFLASH destination address
*                  - len:  ��Ҫ��ȡword���ݵĳ���
*
* Output         : - des:  ��ȡ��word���ݻ���
* Return         : None
******************************************************************************/
/***************************************************************************************
**������:       DRV_OnChipFlash_ReadBytes
**��������:     ��EFLASHĿ���ַ��ȡ����Ϊlen��Bytes����
**�������:     
Addr ��ַ
des ���ݻ���
len �ֽڳ���
**�������:     --
**��ע:         
****************************************************************************************/
void DRV_OnChipFlash_ReadBytes(UINT32 addr, UINT8 *des, UINT32 len)
{
	UINT32 i;

	for(i = 0; i < len; i ++)
	{
		des[i] = (*(volatile UINT8 *)(addr+i));
	}
}

#endif  /*DRV_ONCHIP_FLASH_MODULE_EN*/
/************************************Code End******************************************/
