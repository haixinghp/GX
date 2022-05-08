#include "fm175xx.h"
#include "spi_drv.h"
#include "pcd_interface.h"
#include "uart_drv.h"
#include "delay.h"
#include "debug.h"

//ODR数据输出锁存 IDR输入引脚值 DDR数据方向 CR1控制1 CR2控制2
//175XX msb优先  上升沿稳定 下降改变  SCK闲为低 NSS低有效 

#define RF_RST_LOW	 do{SPI2->SPIPORT &= 0xFD;}while(0)
#define RF_RST_HIGH	 do{SPI2->SPIPORT |= 0x02;}while(0)
#define RF_NSS_LOW   do{SPI2->SPIPORT &= 0xFE;}while(0)
#define RF_NSS_HIGH  do{SPI2->SPIPORT |= 0x01;}while(0)

/*************************************************************/
//函数名：	    Read_Reg                                 
//功能：	    读寄存器操作                                      
//输入参数：	reg_add，寄存器地址                           
//返回值：	    寄存器数值                                
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
//函数名：	    Read_Reg All
//功能：	    读64个寄存器操作
//输入参数：	无
//输出参数		*reg_value，寄存器数值指针,长度为64字节
//返回值：	    OK
/*************************************************************/
unsigned char Read_Reg_All(unsigned char *reg_value)
{
	unsigned char i;
	for (i = 0; i < 64; i++)
		*(reg_value + i) = Read_Reg(i);
	return OK;
}

/*************************************************************/
//函数名：	    Write_Reg				     			 
//功能：	    写寄存器操作					     	 															 */
//输入参数：	reg_add，寄存器地址；reg_value，寄存器数值	 
//返回值：										 
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
//函数名：	    Read_FIFO									 
//功能：	    读取FIFO  									 															 */
//输入参数：	length，读取FIFO数据长度					 
//输出参数：	*fifo_data，FIFO数据存放指针				 
//返回值：	    											 															 */
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
//函数名：	    Write_FIFO									 
//功能：	    写入FIFO  									 															 */
//输入参数：	length，读取数据长度；						 
//				*fifo_data，数据存放指针					 
//输出参数：												 
//返回值：	    											 															 */
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
//函数名：	    Clear_FIFO									 
//功能：	    清空FIFO  									 															 */
//输入参数：	无											
//返回值：	    OK					
//				ERROR										
/*************************************************************/
unsigned char Clear_FIFO(void)
{
	Set_BitMask(FIFOLevelReg, 0x80);  //清除FIFO缓冲 bit7=1
	if (Read_Reg(FIFOLevelReg) == 0)
		return OK;
	else
		return ERROR;
}

/////////////////////////////////////////////////////////////////////
//开启天线
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void Fm175xxAntennaOn()
{
	Write_Reg(TxControlReg, Read_Reg(TxControlReg) | 0x03); //Tx1RFEn=1 Tx2RFEn=1
}
/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void Fm175xxAntennaOff()
{
	Write_Reg(TxControlReg, Read_Reg(TxControlReg) & (~0x03));
}

/*************************************************************/
//函数名：	    Set_BitMask									 
//功能：	    置位寄存器操作								 
//输入参数：	reg_add，寄存器地址；mask，寄存器置位		
//返回值：	    				 
/*************************************************************/
unsigned char Set_BitMask(unsigned char reg_add, unsigned char mask)
{
	unsigned char result;
	result = Write_Reg(reg_add, Read_Reg(reg_add) | mask);  // set bit mask
	return result;
}

/*************************************************************/
//函数名：	    Clear_BitMask
//功能：	    清除位寄存器操作				
//输入参数：	reg_add，寄存器地址；mask，寄存器清除位
//返回值：	   
/*************************************************************/
unsigned char Clear_BitMask(unsigned char reg_add, unsigned char mask)
{
	unsigned char result;
	result = Write_Reg(reg_add, Read_Reg(reg_add) & ~mask);  // clear bit mask
	return result;
}
/*************************************************************/
//函数名：	    Pcd_Comm
//功能：	    读卡器通信				
//输入参数：	Command，通信操作命令；
//				pInData，发送数据数组；
//				InLenByte，发送数据数组字节长度；
//				pOutData，接收数据数组；
//				pOutLenBit，接收数据的位长度
//返回值：	    OK		ERROR
/*************************************************************/
unsigned char Pcd_Comm(unsigned char Command,
					   unsigned char *pInData,
					   unsigned char InLenByte,
					   unsigned char *pOutData,
					   unsigned int *pOutLenBit)
{
	unsigned char result;
	unsigned char rx_temp = 0; //临时数据字节长度
	unsigned char rx_len = 0; //接收数据字节长度
	unsigned char lastBits = 0; //接收数据位长度
	unsigned char irq;

	Clear_FIFO();
	Write_Reg(CommandReg, Idle);
	Write_Reg(WaterLevelReg, 0x20); //设置FIFOLevel=32字节
	Write_Reg(ComIrqReg, 0x7F);     //清除IRQ标志
	if (Command == MFAuthent)
	{
		Write_FIFO(InLenByte, pInData);   //填入认证密钥
		Set_BitMask(BitFramingReg, 0x80); //启动发送
	}
	Set_BitMask(TModeReg, 0x80); //自动启动定时器,(发射结束即自动开始计数)
	Write_Reg(CommandReg, Command);

	while (1) //循环判断中断标识
	{
		irq = Read_Reg(ComIrqReg); //查询中断标志
		if (irq & 0x01)	//TimerIRq  定时器时间用尽
		{
			result = ERROR;
			break;
		}

		if (Command == MFAuthent)
		{
			if (irq & 0x10)	//IdelIRq  指令执行完成或CommandReg寄存器为Idle空闲
			{
				result = OK;
				break;
			}
		}

		if (Command == Transmit)
		{
			if ((irq & 0x04) && (InLenByte > 0))	//LoAlertIrq+发送字节数大于0
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
				Set_BitMask(BitFramingReg, 0x80);	//启动发送
				Write_Reg(ComIrqReg, 0x04);	//清除LoAlertIrq
			}
			if ((irq & 0x40) && (InLenByte == 0))	//TxIRq
			{
				result = OK;
				break;
			}
		}

		if (Command == Transceive)
		{
			if ((irq & 0x04) && (InLenByte > 0))//LoAlertIrq + 发送字节数大于0  Status!Reg中bit0 LoAlert=FIFOLength<=WaterLevel(0x20)置1，ComIRqReg中bit2 LoAlert为1时LoAlertIrq置1
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
				Set_BitMask(BitFramingReg, 0x80);	//启动发送
				Write_Reg(ComIrqReg, 0x04);	//清除LoAlertIrq
			}
			if (irq & 0x08)	//HiAlertIRq    Status!Reg中bit1 HiAlert=(64-FIFOLength)<=WaterLevel(0x20)置1，ComIRqReg中bit3 HiAlert为1时HiAlertIRq置1
			{
				if ((irq & 0x40) && (InLenByte == 0)
						&& (Read_Reg(FIFOLevelReg) > 32))//TxIRq	+ 待发送长度为0 + FIFO长度大于32
				{
					Read_FIFO(32, pOutData + rx_len); //读出FIFO内容
					rx_len = rx_len + 32;
					Write_Reg(ComIrqReg, 0x08);	//清除 HiAlertIRq
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
		Read_FIFO(rx_temp, pOutData + rx_len); //读出FIFO内容
		rx_len = rx_len + rx_temp; //接收长度累加

		lastBits = Read_Reg(ControlReg) & 0x07;
		if (lastBits)
			*pOutLenBit = (rx_len - 1) * (unsigned int) 8 + lastBits;
		else
			*pOutLenBit = rx_len * (unsigned int) 8;
	}

	Set_BitMask(ControlReg, 0x80);           // TStopNow
	Write_Reg(CommandReg, Idle);
	Clear_BitMask(BitFramingReg, 0x80);      //关闭发送
	return result;
}

/*************************************************************/
//函数名：	    Pcd_SetTimer
//功能：	    设置接收延时
//输入参数：	delaytime，延时时间（单位为毫秒）
//返回值：	    OK
/*************************************************************/
unsigned char Pcd_SetTimer(unsigned long delaytime)           //设定超时时间（ms）
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
//函数名：	    Pcd_ConfigISOType
//功能：	    设置操作协议
//输入参数：	type 0，ISO14443A协议；
//					 1，ISO14443B协议；
//返回值：	    OK
/*************************************************************/
unsigned char Pcd_ConfigISOType(unsigned char type)
{
	Fm175xxAntennaOff();

	if (type == 0)                     //ISO14443_A
	{
		Set_BitMask(ControlReg, 0x10);	//ControlReg 0x0C 设置reader模式
		Set_BitMask(TxAutoReg, 0x40);	//TxASKReg 0x15 设置100%ASK有效
		Write_Reg(TxModeReg, 0x00);	//TxModeReg 0x12 设置TX CRC无效，TxSpeed=106kbit TxFraming =TypeA
		Write_Reg(RxModeReg, 0x00);	//RxModeReg 0x13 设置RX CRC无效，
	}

	if (type == 1)                     //ISO14443_B
	{
		Write_Reg(ControlReg, 0x10);	//ControlReg 0x0C 设置reader模式
		Write_Reg(TxAutoReg, 0x00);	// TxASKReg 0x15 设置100%ASK无效
		Write_Reg(TxModeReg, 0x83);	//TxModeReg 0x12 设置TX CRC有效，TxFraming =TypeB
		Write_Reg(RxModeReg, 0x83); //RxModeReg 0x13 设置RX CRC有效，TxFraming =TypeB
		Write_Reg(GsNReg, 0xF4);	//GsNReg 0x27 设置ON电导
		Write_Reg(GsNOffReg, 0xF4);	//GsNOffReg 0x23 设置OFF电导
		Write_Reg(BitFramingReg, 0x00);
	}

	Fm175xxAntennaOn();
	DelayMS(100);

	return OK;
}

/*************************************************************/
//函数名：FM175XX_SoftReset
//功能：	    软/硬复位操作				
//输入参数：				
//返回值：	    OK			ERROR
/*************************************************************/
unsigned char FM175XX_SoftReset(void)
{
	Write_Reg(CommandReg, SoftReset);
	return Set_BitMask(ControlReg, 0x10);	//17520初始值配置
}

/*************************************************************/
//函数名：FM175XX_HardReset
//功能：	    硬复位操作
//输入参数：
//返回值：
/*************************************************************/
void FM175XX_HardReset(void)
{
	RF_RST_LOW;
	DelayMS(100);
	RF_RST_HIGH;
	DelayMS(100);
}

/*************************************************************/
//函数名：	    FM175XX_SoftPowerdown
//功能：	    软件低功耗操作
//输入参数：	
//返回值：	    OK，进入低功耗模式；
//				ERROR，退出低功耗模式；
/*************************************************************/
unsigned char FM175XX_SoftPowerdown(void)
{
	if (Read_Reg(CommandReg) & 0x10)
	{
		Clear_BitMask(CommandReg, 0x10);	//退出低功耗模式
		return ERROR;
	}
	else
		Set_BitMask(CommandReg, 0x10);	//进入低功耗模式
	return OK;
}

/*************************************************************/
//函数名：	    FM175XX_HardPowerdown
//功能：	    硬件低功耗操作
//输入参数：	
//返回值：	    OK，进入低功耗模式；
//				ERROR，退出低功耗模式；
/*************************************************************/
unsigned char FM175XX_HardPowerdown(void)
{
	//NPD=~NPD;
	//if(NPD==1)
	//  return OK; //进入低功耗模式
	//else
	return ERROR;  //退出低功耗模式
}

/*************************************************************/
//函数名：	    Read_Ext_Reg
//功能：	    读取扩展寄存器
//输入参数：	reg_add，寄存器地址
//返回值：	    寄存器数值
/*************************************************************/
unsigned char Read_Ext_Reg(unsigned char reg_add)
{
	Write_Reg(0x0F, 0x80 + reg_add);
	return Read_Reg(0x0F);
}

/*********************************************/
//函数名：	    Write_Ext_Reg
//功能：	    写入扩展寄存器
//输入参数：	reg_add，寄存器地址；reg_value，寄存器数值
//返回值：	    OK          ERROR
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

    /*写寄存器*/
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
