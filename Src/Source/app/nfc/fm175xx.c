#include "fm175xx.h"
#include "spi_drv.h"
#include "pcd_interface.h"
#include "uart_drv.h"
#include "delay.h"
#include "debug.h"

//ODR����������� IDR��������ֵ DDR���ݷ��� CR1����1 CR2����2
//175XX msb����  �������ȶ� �½��ı�  SCK��Ϊ�� NSS����Ч 

#define RF_RST_LOW	 do{SPI2->SPIPORT &= 0xFD;}while(0)
#define RF_RST_HIGH	 do{SPI2->SPIPORT |= 0x02;}while(0)
#define RF_NSS_LOW   do{SPI2->SPIPORT &= 0xFE;}while(0)
#define RF_NSS_HIGH  do{SPI2->SPIPORT |= 0x01;}while(0)

/*************************************************************/
//��������	    Read_Reg                                 
//���ܣ�	    ���Ĵ�������                                      
//���������	reg_add���Ĵ�����ַ                           
//����ֵ��	    �Ĵ�����ֵ                                
/*************************************************************/
unsigned char Read_Reg(unsigned char reg_add)
{
	unsigned char ucAddr = ((reg_add << 1) & 0x7E) | 0x80;
	unsigned char ucResult;

	RF_NSS_LOW;
	SPI_MasterSendData(SPI1, &ucAddr, 1);
	SPI_MasterReceiveData(SPI1, &ucResult, 1);
	RF_NSS_HIGH;

	return ucResult;
}

/*************************************************************/
//��������	    Read_Reg All
//���ܣ�	    ��64���Ĵ�������
//���������	��
//�������		*reg_value���Ĵ�����ֵָ��,����Ϊ64�ֽ�
//����ֵ��	    OK
/*************************************************************/
unsigned char Read_Reg_All(unsigned char *reg_value)
{
	unsigned char i;
	for (i = 0; i < 64; i++)
		*(reg_value + i) = Read_Reg(i);
	return OK;
}

/*************************************************************/
//��������	    Write_Reg				     			 
//���ܣ�	    д�Ĵ�������					     	 															 */
//���������	reg_add���Ĵ�����ַ��reg_value���Ĵ�����ֵ	 
//����ֵ��										 
/*************************************************************/
unsigned char Write_Reg(unsigned char reg_add, unsigned char reg_value)
{
	unsigned char ucAddr = ((reg_add << 1) & 0x7E);

	RF_NSS_LOW;
	SPI_MasterSendData(SPI1, &ucAddr, 1);
	SPI_MasterSendData(SPI1, &reg_value, 1);
	RF_NSS_HIGH;

	return OK;
}

/*************************************************************/
//��������	    Read_FIFO									 
//���ܣ�	    ��ȡFIFO  									 															 */
//���������	length����ȡFIFO���ݳ���					 
//���������	*fifo_data��FIFO���ݴ��ָ��				 
//����ֵ��	    											 															 */
/*************************************************************/
void Read_FIFO(unsigned char length, unsigned char *fifo_data)
{
	while (length--)
	{
		*(fifo_data++) = Read_Reg(FIFODataReg);
	}
	return;
}

/*************************************************************/
//��������	    Write_FIFO									 
//���ܣ�	    д��FIFO  									 															 */
//���������	length����ȡ���ݳ��ȣ�						 
//				*fifo_data�����ݴ��ָ��					 
//���������												 
//����ֵ��	    											 															 */
/*************************************************************/
void Write_FIFO(unsigned char length, unsigned char *fifo_data)
{
	while (length--)
	{
		Write_Reg(FIFODataReg, *(fifo_data++));
	}
	return;
}

/*************************************************************/
//��������	    Clear_FIFO									 
//���ܣ�	    ���FIFO  									 															 */
//���������	��											
//����ֵ��	    OK					
//				ERROR										
/*************************************************************/
unsigned char Clear_FIFO(void)
{
	Set_BitMask(FIFOLevelReg, 0x80);  //���FIFO���� bit7=1
	if (Read_Reg(FIFOLevelReg) == 0)
		return OK;
	else
		return ERROR;
}

/////////////////////////////////////////////////////////////////////
//��������
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
/////////////////////////////////////////////////////////////////////
void Fm175xxAntennaOn()
{
	Write_Reg(TxControlReg, Read_Reg(TxControlReg) | 0x03); //Tx1RFEn=1 Tx2RFEn=1
}
/////////////////////////////////////////////////////////////////////
//�ر�����
/////////////////////////////////////////////////////////////////////
void Fm175xxAntennaOff()
{
	Write_Reg(TxControlReg, Read_Reg(TxControlReg) & (~0x03));
}

/*************************************************************/
//��������	    Set_BitMask									 
//���ܣ�	    ��λ�Ĵ�������								 
//���������	reg_add���Ĵ�����ַ��mask���Ĵ�����λ		
//����ֵ��	    				 
/*************************************************************/
unsigned char Set_BitMask(unsigned char reg_add, unsigned char mask)
{
	unsigned char result;
	result = Write_Reg(reg_add, Read_Reg(reg_add) | mask);  // set bit mask
	return result;
}

/*************************************************************/
//��������	    Clear_BitMask
//���ܣ�	    ���λ�Ĵ�������				
//���������	reg_add���Ĵ�����ַ��mask���Ĵ������λ
//����ֵ��	   
/*************************************************************/
unsigned char Clear_BitMask(unsigned char reg_add, unsigned char mask)
{
	unsigned char result;
	result = Write_Reg(reg_add, Read_Reg(reg_add) & ~mask);  // clear bit mask
	return result;
}
/*************************************************************/
//��������	    Pcd_Comm
//���ܣ�	    ������ͨ��				
//���������	Command��ͨ�Ų������
//				pInData�������������飻
//				InLenByte���������������ֽڳ��ȣ�
//				pOutData�������������飻
//				pOutLenBit���������ݵ�λ����
//����ֵ��	    OK		ERROR
/*************************************************************/
unsigned char Pcd_Comm(unsigned char Command,
					   unsigned char *pInData,
					   unsigned char InLenByte,
					   unsigned char *pOutData,
					   unsigned int *pOutLenBit)
{
	unsigned char result;
	unsigned char rx_temp = 0; //��ʱ�����ֽڳ���
	unsigned char rx_len = 0; //���������ֽڳ���
	unsigned char lastBits = 0; //��������λ����
	unsigned char irq;

	Clear_FIFO();
	Write_Reg(CommandReg, Idle);
	Write_Reg(WaterLevelReg, 0x20); //����FIFOLevel=32�ֽ�
	Write_Reg(ComIrqReg, 0x7F);     //���IRQ��־
	if (Command == MFAuthent)
	{
		Write_FIFO(InLenByte, pInData);   //������֤��Կ
		Set_BitMask(BitFramingReg, 0x80); //��������
	}
	Set_BitMask(TModeReg, 0x80); //�Զ�������ʱ��,(����������Զ���ʼ����)
	Write_Reg(CommandReg, Command);

	while (1) //ѭ���ж��жϱ�ʶ
	{
		irq = Read_Reg(ComIrqReg); //��ѯ�жϱ�־
		if (irq & 0x01)	//TimerIRq  ��ʱ��ʱ���þ�
		{
			result = ERROR;
			break;
		}

		if (Command == MFAuthent)
		{
			if (irq & 0x10)	//IdelIRq  ָ��ִ����ɻ�CommandReg�Ĵ���ΪIdle����
			{
				result = OK;
				break;
			}
		}

		if (Command == Transmit)
		{
			if ((irq & 0x04) && (InLenByte > 0))	//LoAlertIrq+�����ֽ�������0
			{
				if (InLenByte < 32)
				{
					Write_FIFO(InLenByte, pInData);
					InLenByte = 0;
				}
				else
				{
					Write_FIFO(32, pInData);
					InLenByte = InLenByte - 32;
					pInData = pInData + 32;
				}
				Set_BitMask(BitFramingReg, 0x80);	//��������
				Write_Reg(ComIrqReg, 0x04);	//���LoAlertIrq
			}
			if ((irq & 0x40) && (InLenByte == 0))	//TxIRq
			{
				result = OK;
				break;
			}
		}

		if (Command == Transceive)
		{
			if ((irq & 0x04) && (InLenByte > 0))//LoAlertIrq + �����ֽ�������0  Status!Reg��bit0 LoAlert=FIFOLength<=WaterLevel(0x20)��1��ComIRqReg��bit2 LoAlertΪ1ʱLoAlertIrq��1
			{
				if (InLenByte > 32)
				{
					Write_FIFO(32, pInData);
					InLenByte = InLenByte - 32;
					pInData = pInData + 32;
				}
				else
				{
					Write_FIFO(InLenByte, pInData);
					InLenByte = 0;
				}
				Set_BitMask(BitFramingReg, 0x80);	//��������
				Write_Reg(ComIrqReg, 0x04);	//���LoAlertIrq
			}
			if (irq & 0x08)	//HiAlertIRq    Status!Reg��bit1 HiAlert=(64-FIFOLength)<=WaterLevel(0x20)��1��ComIRqReg��bit3 HiAlertΪ1ʱHiAlertIRq��1
			{
				if ((irq & 0x40) && (InLenByte == 0)
						&& (Read_Reg(FIFOLevelReg) > 32))//TxIRq	+ �����ͳ���Ϊ0 + FIFO���ȴ���32
				{
					Read_FIFO(32, pOutData + rx_len); //����FIFO����
					rx_len = rx_len + 32;
					Write_Reg(ComIrqReg, 0x08);	//��� HiAlertIRq
				}
			}
			if ((irq & 0x20) && (InLenByte == 0))	//RxIRq=1
			{
				result = OK;
				break;
			}
		}
	}

	if (Command == Transceive)
	{
		rx_temp = Read_Reg(FIFOLevelReg);
		Read_FIFO(rx_temp, pOutData + rx_len); //����FIFO����
		rx_len = rx_len + rx_temp; //���ճ����ۼ�

		lastBits = Read_Reg(ControlReg) & 0x07;
		if (lastBits)
			*pOutLenBit = (rx_len - 1) * (unsigned int) 8 + lastBits;
		else
			*pOutLenBit = rx_len * (unsigned int) 8;
	}

	Set_BitMask(ControlReg, 0x80);           // TStopNow
	Write_Reg(CommandReg, Idle);
	Clear_BitMask(BitFramingReg, 0x80);      //�رշ���
	return result;
}

/*************************************************************/
//��������	    Pcd_SetTimer
//���ܣ�	    ���ý�����ʱ
//���������	delaytime����ʱʱ�䣨��λΪ���룩
//����ֵ��	    OK
/*************************************************************/
unsigned char Pcd_SetTimer(unsigned long delaytime)           //�趨��ʱʱ�䣨ms��
{
	unsigned long TimeReload;
	unsigned int Prescaler;

	Prescaler = 0;
	TimeReload = 0;
	while (Prescaler < 0xfff)
	{
		TimeReload = ((delaytime * (long) 13560) - 1) / (Prescaler * 2 + 1);
		if (TimeReload < 0xffff)
			break;
		Prescaler++;
	}
	TimeReload = TimeReload & 0xFFFF;
	Set_BitMask(TModeReg, Prescaler >> 8);
	Write_Reg(TPrescalerReg, Prescaler & 0xFF);
	Write_Reg(TReloadMSBReg, TimeReload >> 8);
	Write_Reg(TReloadLSBReg, TimeReload & 0xFF);
	return OK;
}

/*************************************************************/
//��������	    Pcd_ConfigISOType
//���ܣ�	    ���ò���Э��
//���������	type 0��ISO14443AЭ�飻
//					 1��ISO14443BЭ�飻
//����ֵ��	    OK
/*************************************************************/
unsigned char Pcd_ConfigISOType(unsigned char type)
{
	Fm175xxAntennaOff();

	if (type == 0)                     //ISO14443_A
	{
		Set_BitMask(ControlReg, 0x10);	//ControlReg 0x0C ����readerģʽ
		Set_BitMask(TxAutoReg, 0x40);	//TxASKReg 0x15 ����100%ASK��Ч
		Write_Reg(TxModeReg, 0x00);	//TxModeReg 0x12 ����TX CRC��Ч��TxSpeed=106kbit TxFraming =TypeA
		Write_Reg(RxModeReg, 0x00);	//RxModeReg 0x13 ����RX CRC��Ч��
	}

	if (type == 1)                     //ISO14443_B
	{
		Write_Reg(ControlReg, 0x10);	//ControlReg 0x0C ����readerģʽ
		Write_Reg(TxAutoReg, 0x00);	// TxASKReg 0x15 ����100%ASK��Ч
		Write_Reg(TxModeReg, 0x83);	//TxModeReg 0x12 ����TX CRC��Ч��TxFraming =TypeB
		Write_Reg(RxModeReg, 0x83); //RxModeReg 0x13 ����RX CRC��Ч��TxFraming =TypeB
		Write_Reg(GsNReg, 0xF4);	//GsNReg 0x27 ����ON�絼
		Write_Reg(GsNOffReg, 0xF4);	//GsNOffReg 0x23 ����OFF�絼
		Write_Reg(BitFramingReg, 0x00);
	}

	Fm175xxAntennaOn();
	DelayMS(100);

	return OK;
}

/*************************************************************/
//��������FM175XX_SoftReset
//���ܣ�	    ��/Ӳ��λ����				
//���������				
//����ֵ��	    OK			ERROR
/*************************************************************/
unsigned char FM175XX_SoftReset(void)
{
	Write_Reg(CommandReg, SoftReset);
	return Set_BitMask(ControlReg, 0x10);	//17520��ʼֵ����
}

/*************************************************************/
//��������FM175XX_HardReset
//���ܣ�	    Ӳ��λ����
//���������
//����ֵ��
/*************************************************************/
void FM175XX_HardReset(void)
{
	RF_RST_LOW;
	DelayMS(100);
	RF_RST_HIGH;
	DelayMS(100);
}

/*************************************************************/
//��������	    FM175XX_SoftPowerdown
//���ܣ�	    ����͹��Ĳ���
//���������	
//����ֵ��	    OK������͹���ģʽ��
//				ERROR���˳��͹���ģʽ��
/*************************************************************/
unsigned char FM175XX_SoftPowerdown(void)
{
	if (Read_Reg(CommandReg) & 0x10)
	{
		Clear_BitMask(CommandReg, 0x10);	//�˳��͹���ģʽ
		return ERROR;
	}
	else
		Set_BitMask(CommandReg, 0x10);	//����͹���ģʽ
	return OK;
}

/*************************************************************/
//��������	    FM175XX_HardPowerdown
//���ܣ�	    Ӳ���͹��Ĳ���
//���������	
//����ֵ��	    OK������͹���ģʽ��
//				ERROR���˳��͹���ģʽ��
/*************************************************************/
unsigned char FM175XX_HardPowerdown(void)
{
	//NPD=~NPD;
	//if(NPD==1)
	//  return OK; //����͹���ģʽ
	//else
	return ERROR;  //�˳��͹���ģʽ
}

/*************************************************************/
//��������	    Read_Ext_Reg
//���ܣ�	    ��ȡ��չ�Ĵ���
//���������	reg_add���Ĵ�����ַ
//����ֵ��	    �Ĵ�����ֵ
/*************************************************************/
unsigned char Read_Ext_Reg(unsigned char reg_add)
{
	Write_Reg(0x0F, 0x80 + reg_add);
	return Read_Reg(0x0F);
}

/*********************************************/
//��������	    Write_Ext_Reg
//���ܣ�	    д����չ�Ĵ���
//���������	reg_add���Ĵ�����ַ��reg_value���Ĵ�����ֵ
//����ֵ��	    OK          ERROR
/*********************************************/
unsigned char Write_Ext_Reg(unsigned char reg_add, unsigned char reg_value)
{
	Write_Reg(0x0F, 0x40 + reg_add);
	Write_Reg(0x0F, 0xC0 + reg_value);
	return OK;
}

void fm17550_test_spi_comm_simple(void)
{
    uint8_t read_res = 0;
//    uint8_t towrite_value = 0;
    uint8_t loop = 0;
    PcdHWReset();
	  PcdSoftReset();
	  DelayMS(100);
    do
    {
        read_res = ReadRawRC(0x01);
			  printf("cmd_reg:%02x\r\n", read_res);
    }
    while((read_res & 0x1f) != 0);

    /*д�Ĵ���*/
    for(loop = 0; loop < 255; loop++)
    {
        Write_Reg(0x02, loop);
        DelayMS(100);
        read_res = Read_Reg(0x02);
        DelayMS(10);
        printf("%02x  ", read_res);
    }

    Write_Reg(0x02, 0x00);
}
