#include "pcd_interface.h"
#include "ISO14443A.H"
#include "emv_layer4.h"
#include <string.h>
#include "debug.h"

/////////////////////////////////////////////////////////////////////
//寻卡
//input: req_code=寻卡方式
//       req_code=0x52:寻天线区内所有符合14443A标准的卡
//       req_code=0x26:只寻未进入休眠状态的卡
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code, unsigned char *atq)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;

	pi = &MfComData;

	Clear_BitMask(TxModeReg, 0x80);	//关闭TX CRC
	Clear_BitMask(RxModeReg, 0x80);	//关闭RX CRC
	//	Set_BitMask(RxModeReg, 0x08);	    //置1，关闭位接收，忽略接收小于4个bit位
	Clear_BitMask(Status2Reg, 0x08);	//关闭MIFARE Crypto1，执行MFAuthent后置1
	Write_Reg(BitFramingReg, 0x07);	//TxLastBits最后一个字节发送位数，REQ为短帧7bit   PICC低位先回

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 1;
	MfComData.MfData[0] = req_code;
	PcdSetTmo(106);
	status = PcdComTransceive(pi);

	if(0 == status)
	{
		if (MfComData.MfLength != 0x10)
		{
			status = (char)MI_BITCOUNTERR;
		}
		*atq = MfComData.MfData[0];
		*(atq + 1) = MfComData.MfData[1];

		return MI_OK;
	}

	return status;
}

/////////////////////////////////////////////////////////////////////
//防冲撞
//input: g_cSNR=存放序列号(4byte)的内存单元首地址
//output:status=MI_OK:成功
//       得到的序列号放入指定单元
/////////////////////////////////////////////////////////////////////
char PcdAnticoll(unsigned char *snr)
{
	unsigned char i;
	unsigned char snr_check = 0;
	char status = MI_OK;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	ClearBitMask(TxModeReg, BIT7); //不使能发送crc
	ClearBitMask(RxModeReg, BIT7); //不使能接收crc

	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x20;

	status = PcdComTransceive(pi);
	if (!status)
	{
		for (i = 0; i < 4; i++)
		{
			snr_check ^= MfComData.MfData[i];
		}
		if (snr_check != MfComData.MfData[i])
		{
			status = (char)MI_SERNRERR;
		}
		else
		{
			memcpy(snr, &MfComData.MfData[0], 4);
		}
	}
	WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
	return status;
}

/////////////////////////////////////////////////////////////////////
//选定一张卡
//input:g_cSNR=序列号
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *snr, unsigned char *_size)
{
	unsigned char i;
	char status;
	unsigned char snr_check = 0;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC531
	WriteRawRC(RegChannelRedundancy,0x0F);
	ClearBitMask(RegControl,0x08);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	SetBitMask(TxModeReg, BIT7); //使能发送crc
	SetBitMask(RxModeReg, BIT7); //使能接收crc
#endif
	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 7;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x70;
	for (i = 0; i < 4; i++)
	{
		snr_check ^= *(snr + i);
		MfComData.MfData[i + 2] = *(snr + i);
	}
	MfComData.MfData[6] = snr_check;

	status = PcdComTransceive(pi);

	if (status == MI_OK)
	{
		if (MfComData.MfLength != 0x8)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			*_size = MfComData.MfData[0];
		}
	}

	return status;
}

/////////////////////////////////////////////////////////////////////
//将Mifare_One卡密钥转换为RC500接收格式
//input: uncoded=6字节未转换的密钥
//output:coded=12字节转换后的密钥
/////////////////////////////////////////////////////////////////////
char ChangeCodeKey(unsigned char *uncoded, unsigned char *coded)
{
	unsigned char cnt = 0;
	unsigned char ln = 0;
	unsigned char hn = 0;

	for (cnt = 0; cnt < 6; cnt++)
	{
		ln = uncoded[cnt] & 0x0F;
		hn = uncoded[cnt] >> 4;
		coded[cnt * 2 + 1] = (~ln << 4) | ln;
		coded[cnt * 2] = (~hn << 4) | hn;
	}
	return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//将存在RC500的EEPROM中的密钥匙调入RC500的FIFO
//input: startaddr=EEPROM地址
/////////////////////////////////////////////////////////////////////
char PcdLoadKeyE2(unsigned int startaddr)
{
	char status = MI_OK;
//	struct TranSciveBuffer MfComData;
//	struct TranSciveBuffer *pi;
//	pi = &MfComData;

#ifdef VERSION_RC531
	MfComData.MfCommand = PCD_LOADKEYE2;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = startaddr & 0xFF;
	MfComData.MfData[1] = (startaddr >> 8) & 0xFF;

	status = PcdComTransceive(pi);
#endif

	return status;
}

/////////////////////////////////////////////////////////////////////
//功能：将已转换格式后的密钥送到RC500的FIFO中
//input:keys=密钥
/////////////////////////////////////////////////////////////////////
char PcdAuthKey(unsigned char *keys)
{
//	char status = MI_OK;
//	struct TranSciveBuffer MfComData;
//	struct TranSciveBuffer *pi;
//	pi = &MfComData;

#ifdef VERSION_RC531
	PcdSetTmo(106);
	MfComData.MfCommand = PCD_LOADKEY;
	MfComData.MfLength = 12;
	memcpy(&MfComData.MfData[0], keys, 12);

	status = PcdComTransceive(pi);
#endif

	return 0;
}

/////////////////////////////////////////////////////////////////////
//功能：用存放RC500的FIFO中的密钥和卡上的密钥进行验证
//input:auth_mode=验证方式,0x60:验证A密钥,0x61:验证B密钥
//      block=要验证的绝对块号
//      g_cSNR=序列号首地址
/////////////////////////////////////////////////////////////////////
char PcdAuthState(unsigned char auth_mode, unsigned char block,
		unsigned char *g_cSNR)
{
	char status = MI_OK;
//	struct TranSciveBuffer MfComData;
//	struct TranSciveBuffer *pi;
//	pi = &MfComData;

#ifdef VERSION_RC531
	//  WriteRC(RegChannelRedundancy,0x07);
	//       PcdSetTmo(106);
	MfComData.MfCommand = PCD_AUTHENT1;
	MfComData.MfLength = 6;
	MfComData.MfData[0] = auth_mode;
	MfComData.MfData[1] = block;
	memcpy(&MfComData.MfData[2], g_cSNR, 4);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if (ReadRawRC(RegSecondaryStatus) & 0x07)
		{	status = MI_BITCOUNTERR;}
		else
		{
			MfComData.MfCommand = PCD_AUTHENT2;
			MfComData.MfLength = 0;
			status = PcdComTransceive(pi);
			if (status == MI_OK)
			{
				if (ReadRawRC(RegControl) & 0x08)
				{	status = MI_OK;}
				else
				{	status = MI_AUTHERR;}

			}
		}
	}
#endif	
	return status;
}

/**
 ****************************************************************
 * @brief pcd_auth_state() 
 *
 * 功能：用存放在FIFO中的密钥和卡上的密钥进行验证
 *
 * @param: auth_mode=验证方式,0x60:验证A密钥,0x61:验证B密钥
 * @param: block=要验证的绝对块号
 * @param: psnr=序列号首地址
 * @return: status 值为MI_OK:成功
 *
 ****************************************************************
 */
char PcdAuthState_rf(u8 auth_mode, u8 block, u8 *psnr, u8 *pkey)
{
	char status = MI_OK;
//	u8 i;
//	struct TranSciveBuffer mf_com_data;
//	struct TranSciveBuffer *pi;
//	pi = &mf_com_data;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// Tx last bits = 0, rx align = 0

	PcdSetTmo(4);

	mf_com_data.MfCommand= PCD_AUTHENT;
	mf_com_data.MfLength= 12;
	mf_com_data.MfData[0] = auth_mode;
	mf_com_data.MfData[1] = block;
	for (i = 0; i < 6; i++)
	{
		mf_com_data.MfData[2+i] = pkey[i];
	}
	memcpy(&mf_com_data.MfData[8], psnr, 4);

	status = PcdComTransceive(pi);

	if (MI_OK == status)
	{
		if (ReadRawRC(Status2Reg) & BIT3) //MFCrypto1On
		{
			status = MI_OK;
		}
		else
		{
			status = MI_AUTHERR;
		}
	}
#endif	
	return status;
}

/////////////////////////////////////////////////////////////////////
//读mifare_one卡上一块(block)数据(16字节)
//input: addr = 要读的绝对块号
//output:readdata = 读出的数据
/////////////////////////////////////////////////////////////////////
char PcdRead(unsigned char addr, unsigned char *readdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	SetBitMask(RxModeReg, BIT7);//使能接收crc
#endif

	PcdSetTmo(4);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_READ;
	MfComData.MfData[1] = addr;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if (MfComData.MfLength != 0x80)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			memcpy(readdata, &MfComData.MfData[0], 16);
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//写数据到卡上的一块
//input:adde=要写的绝对块号
//      writedata=写入数据
/////////////////////////////////////////////////////////////////////
char PcdWrite(unsigned char addr, unsigned char *writedata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif

	PcdSetTmo(4);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_WRITE;
	MfComData.MfData[1] = addr;

	status = PcdComTransceive(pi);
	if (status != MI_NOTAGERR)
	{
		if (MfComData.MfLength != 4)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_NOTAUTHERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
	}
	if (status == MI_OK)
	{
		PcdSetTmo(15);
		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 16;
		memcpy(&MfComData.MfData[0], writedata, 16);

		status = PcdComTransceive(pi);
		if (status != MI_NOTAGERR)
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_WRITEERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
		PcdSetTmo(106);
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//扣款和充值
//input:dd_mode=命令字,0xC0:扣款,0xC1:充值
//      addr=钱包的绝对块号
//      value=4字节增(减)值首地址,16进制数,低位在前
/////////////////////////////////////////////////////////////////////
char PcdValue(unsigned char dd_mode, unsigned char addr, unsigned char *value)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif

	PcdSetTmo(106);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = dd_mode;
	MfComData.MfData[1] = addr;

	status = PcdComTransceive(pi);
	if (status != MI_NOTAGERR)
	{
		if (MfComData.MfLength != 4)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_NOTAUTHERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			case 0x01:
				status = (char)MI_VALERR;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
	}
	if (status == MI_OK)
	{
		//PcdSetTmo(15);
		PcdSetTmo(106);
		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 4;
		pi = &MfComData;
		memcpy(&MfComData.MfData[0], value, 4);

		status = PcdComTransceive(pi);
		if (status == MI_OK)
		{
			if (MfComData.MfLength != 4)
			{
				status = (char)MI_BITCOUNTERR;
			}
			else
			{
				status = MI_OK;
			}
		}
		else if (status == MI_NOTAGERR)	//增减值成功时M1不返回数据，所以此处返回MI_NOTAGERR
		{
			status = MI_OK;
		}
		else
		{
			status = (char)MI_COM_ERR;
		}
	}

	if (status == MI_OK)
	{
		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 2;
		MfComData.MfData[0] = PICC_TRANSFER;
		MfComData.MfData[1] = addr;

		status = PcdComTransceive(pi);
		if (status != MI_NOTAGERR)
		{
			if (MfComData.MfLength != 4)
			{
				status = (char)MI_BITCOUNTERR;
			}
			else
			{
				MfComData.MfData[0] &= 0x0F;
				switch (MfComData.MfData[0])
				{
				case 0x00:
					status = (char)MI_NOTAUTHERR;
					break;
				case 0x0a:
					status = MI_OK;
					break;
				case 0x01:
					status = (char)MI_VALERR;
					break;
				default:
					status = (char)MI_CODEERR;
					break;
				}
			}
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//将mifare_one卡一块数据调入卡缓冲区
//input:addr=绝对块号
/////////////////////////////////////////////////////////////////////
char PcdRestore(unsigned char addr)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(4);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_RESTORE;
	MfComData.MfData[1] = addr;

	status = PcdComTransceive(pi);
	if (status != MI_NOTAGERR)
	{
		if (MfComData.MfLength != 4)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_NOTAUTHERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			case 0x01:
				status = (char)MI_VALERR;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
	}
	if (status == MI_OK)
	{
		PcdSetTmo(4);
		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 4;
		MfComData.MfData[0] = 0;
		MfComData.MfData[1] = 0;
		MfComData.MfData[2] = 0;
		MfComData.MfData[3] = 0;

		status = PcdComTransceive(pi);
		if (status == MI_NOTAGERR)
		{
			status = MI_OK;
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//将卡缓冲区中数据保存到块
//input:addr=绝对块号
/////////////////////////////////////////////////////////////////////
char PcdTransfer(unsigned char addr)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_TRANSFER;
	MfComData.MfData[1] = addr;

	status = PcdComTransceive(pi);
	if (status != MI_NOTAGERR)
	{
		if (MfComData.MfLength != 4)
		{
			status = (char)MI_BITCOUNTERR;
		}
		else
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_NOTAUTHERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			case 0x01:
				status = (char)MI_VALERR;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//命令卡进入休眠状态
/////////////////////////////////////////////////////////////////////
char PcdHalt()
{
	char status = MI_OK;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;

	pi = &MfComData;
	WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7); //使能发送crc
	ClearBitMask(RxModeReg, BIT7); //不使能接收crc
	PcdSetTmo(2); //according to 14443-3 1ms

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_HALT;
	MfComData.MfData[1] = 0;

	status = PcdComTransceive(pi);
	if (status)
	{
		if (status == MI_NOTAGERR || status == MI_ACCESSTIMEOUT)
			status = MI_OK;
	}

	return status;
}

/////////////////////////////////////////////////////////////////////
//Mifare ultra light防冲撞
//output:snr = 7字节SNR
/////////////////////////////////////////////////////////////////////
char UL_PcdAnticoll(unsigned char *snr)
{
	unsigned char i;
	unsigned char snr_check = 0;
	char status = MI_OK;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC531
	WriteRawRC(RegDecoderControl,0x28);
	ClearBitMask(RegControl,0x08);
	WriteRawRC(RegChannelRedundancy,0x03);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	ClearBitMask(TxModeReg, BIT7); //不使能发送crc
	ClearBitMask(RxModeReg, BIT7); //不使能接收crc
#endif
	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x20;

	status = PcdComTransceive(pi);
	if (!status)
	{
		for (i = 0; i < 4; i++)
		{
			snr_check ^= MfComData.MfData[i];
		}
		if (snr_check != MfComData.MfData[i])
		{
			status = (char)MI_SERNRERR;
		}
		else
		{
			memcpy(snr, &MfComData.MfData[1], 3);
		}

	}
#ifdef VERSION_RC531
	ClearBitMask(RegDecoderControl,0x20);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
#endif

	pi = &MfComData;
	snr_check = 0x88;
#ifdef VERSION_RC531
	WriteRawRC(RegChannelRedundancy,0x0F);
	ClearBitMask(RegControl,0x08);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	SetBitMask(TxModeReg, BIT7); //使能发送crc
	SetBitMask(RxModeReg, BIT7); //使能接收crc
#endif
	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 7;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x70;
	MfComData.MfData[2] = 0x88;
	for (i = 0; i < 3; i++)
	{
		snr_check ^= *(snr + i);
		MfComData.MfData[i + 3] = *(snr + i);
	}
	MfComData.MfData[6] = snr_check;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if (MfComData.MfLength != 0x08)
		{
			status = (char)MI_BITCOUNTERR;
		}
	}

	if (status == MI_OK)
	{
		snr_check = 0;
		pi = &MfComData;
#ifdef VERSION_RC531
		WriteRawRC(RegDecoderControl,0x28);
		ClearBitMask(RegControl,0x08);
		WriteRawRC(RegChannelRedundancy,0x03);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
		ClearBitMask(TxModeReg, BIT7); //不使能发送crc
		ClearBitMask(RxModeReg, BIT7); //不使能接收crc
#endif
		PcdSetTmo(106);

		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 2;
		MfComData.MfData[0] = PICC_ANTICOLL2;
		MfComData.MfData[1] = 0x20;

		status = PcdComTransceive(pi);
		if (!status)
		{
			for (i = 0; i < 4; i++)
			{
				snr_check ^= MfComData.MfData[i];
			}
			if (snr_check != MfComData.MfData[i])
			{
				status = (char)MI_SERNRERR;
			}
			else
			{
				memcpy(snr + 3, &MfComData.MfData[0], 4);
			}
		}
#ifdef VERSION_RC531
		ClearBitMask(RegDecoderControl,0x20);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
#endif

		snr_check = 0;
		pi = &MfComData;
#ifdef VERSION_RC531
		WriteRawRC(RegChannelRedundancy,0x0F);
		ClearBitMask(RegControl,0x08);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
		SetBitMask(TxModeReg, BIT7); //使能发送crc
		SetBitMask(RxModeReg, BIT7); //使能接收crc
#endif
		PcdSetTmo(106);

		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 7;
		MfComData.MfData[0] = PICC_ANTICOLL2;
		MfComData.MfData[1] = 0x70;
		for (i = 0; i < 4; i++)
		{
			snr_check ^= *(snr + 3 + i);
			MfComData.MfData[i + 2] = *(snr + 3 + i);
		}
		MfComData.MfData[6] = snr_check;

		status = PcdComTransceive(pi);
		if (status == MI_OK)
		{
			if (MfComData.MfLength != 0x08)
			{
				status = (char)MI_BITCOUNTERR;
			}
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//Mifare ultra light页写
//input:adde=要写的页地址
//      writedata=写入数据
/////////////////////////////////////////////////////////////////////
char UL_PcdWrite(unsigned char addr, unsigned char *writedata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif

	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 6;
	MfComData.MfData[0] = 0xA2;
	MfComData.MfData[1] = addr;
	memcpy(&MfComData.MfData[2], writedata, 4);

	status = PcdComTransceive(pi);

	if (status != MI_NOTAGERR)
	{
		MfComData.MfData[0] &= 0x0F;
		switch (MfComData.MfData[0])
		{
		case 0x00:
			status = (char)MI_WRITEERR;
			break;
		case 0x0A:
			status = MI_OK;
			break;
		default:
			status = (char)MI_CODEERR;
			break;
		}
	}

	PcdSetTmo(106);
	return status;
}

char UL_PcdAuthenticate1(unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	SetBitMask(RxModeReg, BIT7);//使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = 0x1A;
	MfComData.MfData[1] = 0x00;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		//if (MfComData.MfLength != 0x80)
		//{   status = MI_BITCOUNTERR;   }
		//else
		{
			memcpy(respdata, &MfComData.MfData[0], MfComData.MfLength / 8);
		}
	}

	return status;
}

char UL_PcdAuthenticate2(unsigned char *senddata, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	SetBitMask(TxModeReg, BIT7);//使能发送crc
	SetBitMask(RxModeReg, BIT7);//使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 17;
	MfComData.MfData[0] = 0xAF;
	memcpy(&MfComData.MfData[1], senddata, 16);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		//if (MfComData.MfLength != 0x80)
		//{   status = MI_BITCOUNTERR;   }
		//else
		{
			memcpy(respdata, &MfComData.MfData[0], MfComData.MfLength / 8);
		}
	}

	return status;
}

char PLUS_PcdWritePerso(unsigned short BNr, unsigned char *Data)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(14);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 19;
	MfComData.MfData[0] = 0xA8;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	memcpy(&MfComData.MfData[3], Data, 16);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		return MfComData.MfData[0];
	}

	return status;
}

char PLUS_PcdCommitPerso()
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(14);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 1;
	MfComData.MfData[0] = 0xAA;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		return MfComData.MfData[0];
	}

	return status;
}

char PLUS_PcdGetRndB2(unsigned short BNr, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(14);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 3;
	MfComData.MfData[0] = 0x76;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdGetRndB1(unsigned short BNr, unsigned char *senddata,
		unsigned char *respdata)
{
	char status;
	unsigned char LenCap;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	LenCap = *senddata;
	PcdSetTmo(14);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4 + LenCap;
	MfComData.MfData[0] = 0x70;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = LenCap;
	memcpy(&MfComData.MfData[4], senddata + 1, LenCap);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdGetEncryptRndARotation(unsigned char *senddata,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(14);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 33;
	MfComData.MfData[0] = 0x72;
	memcpy(&MfComData.MfData[1], senddata, 32);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdMultiBlockRead(unsigned short BNr, unsigned char Length,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0x38;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Length;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdMultiBlockWrite(unsigned short BNr, unsigned char Length,
		unsigned char *writedata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
//    PcdSetTmo(4);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0xA8;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Length;

	status = PcdComTransceive(pi);
	if (status != MI_NOTAGERR)
	{
		MfComData.MfData[0] &= 0x0F;
		switch (MfComData.MfData[0])
		{
		case 0x00:
			status = (char)MI_NOTAUTHERR;
			break;
		case 0x0A:
			status = MI_OK;
			break;
		default:
			status = (char)MI_CODEERR;
			break;
		}
	}
	if (status == MI_OK)
	{
		PcdSetTmo(15);
		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = Length;
		memcpy(&MfComData.MfData[0], writedata, Length);

		status = PcdComTransceive(pi);
		if (status != MI_NOTAGERR)
		{
			MfComData.MfData[0] &= 0x0F;
			switch (MfComData.MfData[0])
			{
			case 0x00:
				status = (char)MI_WRITEERR;
				break;
			case 0x0A:
				status = MI_OK;
				break;
			default:
				status = (char)MI_CODEERR;
				break;
			}
		}
		PcdSetTmo(106);
	}
	return status;
}

char PLUS_PcdResetAuthentication()
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 1;
	MfComData.MfData[0] = 0x78;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		return MfComData.MfData[0];
	}

	return status;
}

char PLUS_PcdReadEncryptNoMacRespMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 12;
	MfComData.MfData[0] = 0x30;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read
	memcpy(&MfComData.MfData[4], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadEncryptMacRespMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 12;
	MfComData.MfData[0] = 0x31;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read
	memcpy(&MfComData.MfData[4], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadPlainNoMacRespMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 12;
	MfComData.MfData[0] = 0x32;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read
	memcpy(&MfComData.MfData[4], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadPlainMacRespMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 12;
	MfComData.MfData[0] = 0x33;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read
	memcpy(&MfComData.MfData[4], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadEncryptNoMacRespNoMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0x34;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadEncryptMacRespNoMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0x35;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadPlainNoMacRespNoMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0x36;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdReadPlainMacRespNoMacCmd(unsigned short BNr, unsigned char Ext,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 4;
	MfComData.MfData[0] = 0x37;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	MfComData.MfData[3] = Ext;		//Number of blocks to be read

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdWriteEncryptNoMacRespMacCmd(unsigned short BNr,
		unsigned char *Data, unsigned char DataLen, unsigned char *MAC,
		unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 3 + DataLen + 8;
	MfComData.MfData[0] = 0xA0;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	memcpy(&MfComData.MfData[3], Data, DataLen);
	memcpy(&MfComData.MfData[3 + DataLen], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[0], MfComData.MfLength / 8);
		}
	}

	return status;
}

char PLUS_PcdWriteEncryptMacRespMacCmd(unsigned short BNr, unsigned char *Data,
		unsigned char DataLen, unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 3 + DataLen + 8;
	MfComData.MfData[0] = 0xA1;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	memcpy(&MfComData.MfData[3], Data, DataLen);
	memcpy(&MfComData.MfData[3 + DataLen], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

char PLUS_PcdWritePlainNoMacRespMacCmd(unsigned short BNr, unsigned char *Data,
		unsigned char DataLen, unsigned char *MAC)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 3 + DataLen + 8;
	MfComData.MfData[0] = 0xA2;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	memcpy(&MfComData.MfData[3], Data, DataLen);
	memcpy(&MfComData.MfData[3 + DataLen], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		return MfComData.MfData[0];
	}

	return status;
}

char PLUS_PcdWritePlainMacRespMacCmd(unsigned short BNr, unsigned char *Data,
		unsigned char DataLen, unsigned char *MAC, unsigned char *respdata)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC523
	WriteRawRC(BitFramingReg,0x00);	// // Tx last bits = 0, rx align = 0
	ClearBitMask(TxModeReg, BIT7);//不使能发送crc
	ClearBitMask(RxModeReg, BIT7);//不使能接收crc
#endif
	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 3 + DataLen + 8;
	MfComData.MfData[0] = 0xA3;
	MfComData.MfData[1] = (unsigned char) BNr;		//LSB First
	MfComData.MfData[2] = (unsigned char) (BNr >> 8);
	memcpy(&MfComData.MfData[3], Data, DataLen);
	memcpy(&MfComData.MfData[3 + DataLen], MAC, 8);

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] == 0x0A) || (MfComData.MfData[0] == 0x90))
		{
			memcpy(respdata, &MfComData.MfData[1], MfComData.MfLength / 8 - 1);
		}
		else
		{
			return MfComData.MfData[0];
		}
	}

	return status;
}

/////////////////////////////////////////////////////////////////////
//TypeA防冲撞
//output:snr = xx字节SNR
/////////////////////////////////////////////////////////////////////
char TypeaAnticoll(unsigned char *snr, unsigned char *size)
{
	unsigned char i;
	unsigned char snr_check = 0;
	char status = MI_OK;
	unsigned char tempsn[12];
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

#ifdef VERSION_RC531
	WriteRawRC(RegDecoderControl,0x28);
	ClearBitMask(RegControl,0x08);
	WriteRawRC(RegChannelRedundancy,0x03);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	ClearBitMask(TxModeReg, BIT7); //不使能发送crc
	ClearBitMask(RxModeReg, BIT7); //不使能接收crc
#endif
	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x20;

	status = PcdComTransceive(pi);
	if (!status)
	{
		for (i = 0; i < 4; i++)
		{
			snr_check ^= MfComData.MfData[i];
		}
		if (snr_check != MfComData.MfData[i])
		{
			status = (char)MI_SERNRERR;
		}
		else
		{
			memcpy(tempsn, &MfComData.MfData[0], 4);
		}

	}
#ifdef VERSION_RC531
	ClearBitMask(RegDecoderControl,0x20);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
#endif

	pi = &MfComData;
	snr_check = 0x00;
#ifdef VERSION_RC531
	WriteRawRC(RegChannelRedundancy,0x0F);
	ClearBitMask(RegControl,0x08);
#else	//VERSION_RC523
	WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
	SetBitMask(TxModeReg, BIT7); //使能发送crc
	SetBitMask(RxModeReg, BIT7); //使能接收crc
#endif
	PcdSetTmo(106);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 7;
	MfComData.MfData[0] = PICC_ANTICOLL1;
	MfComData.MfData[1] = 0x70;
	for (i = 0; i < 4; i++)
	{
		snr_check ^= *(tempsn + i);
		MfComData.MfData[i + 2] = *(tempsn + i);
	}
	MfComData.MfData[6] = snr_check;

	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		if (MfComData.MfLength != 0x08)
		{
			status = (char)MI_BITCOUNTERR;
		}
	}

	if (status == MI_OK)
	{
		if ((MfComData.MfData[0] & 0x04) != 0x04)		//UID完整，4字节UID
		{
			*size = 4;
			memcpy(snr, tempsn, *size);
			if ((MfComData.MfData[0] & 0x20) == 0x20)	//兼容ISO14443-4
			{
				g_ISO14443_4 = 1;
			}
			else
			{
				g_ISO14443_4 = 0;
			}
			return status;
		}

		snr_check = 0;
		pi = &MfComData;
#ifdef VERSION_RC531
		WriteRawRC(RegDecoderControl,0x28);
		ClearBitMask(RegControl,0x08);
		WriteRawRC(RegChannelRedundancy,0x03);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
		ClearBitMask(TxModeReg, BIT7); //不使能发送crc
		ClearBitMask(RxModeReg, BIT7); //不使能接收crc
#endif
		PcdSetTmo(106);

		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 2;
		MfComData.MfData[0] = PICC_ANTICOLL2;
		MfComData.MfData[1] = 0x20;

		status = PcdComTransceive(pi);
		if (!status)
		{
			for (i = 0; i < 4; i++)
			{
				snr_check ^= MfComData.MfData[i];
			}
			if (snr_check != MfComData.MfData[i])
			{
				status = (char)MI_SERNRERR;
			}
			else
			{
				memcpy(tempsn + 4, &MfComData.MfData[0], 4);
			}
		}
#ifdef VERSION_RC531
		ClearBitMask(RegDecoderControl,0x20);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// // Tx last bits = 0, rx align = 0
#endif

		snr_check = 0;
		pi = &MfComData;
#ifdef VERSION_RC531
		WriteRawRC(RegChannelRedundancy,0x0F);
		ClearBitMask(RegControl,0x08);
#else	//VERSION_RC523
		WriteRawRC(BitFramingReg, 0x00);	// TxLastBits = 0, RxAlign = 0
		SetBitMask(TxModeReg, BIT7); //使能发送crc
		SetBitMask(RxModeReg, BIT7); //使能接收crc
#endif
		PcdSetTmo(106);

		MfComData.MfCommand = PCD_TRANSCEIVE;
		MfComData.MfLength = 7;
		MfComData.MfData[0] = PICC_ANTICOLL2;
		MfComData.MfData[1] = 0x70;
		for (i = 0; i < 4; i++)
		{
			snr_check ^= *(tempsn + 4 + i);
			MfComData.MfData[i + 2] = *(tempsn + 4 + i);
		}
		MfComData.MfData[6] = snr_check;

		status = PcdComTransceive(pi);
		if (status == MI_OK)
		{
			if (MfComData.MfLength != 0x08)
			{
				status = (char)MI_BITCOUNTERR;
			}
		}

		if (status != MI_OK)
		{
			return status;
		}

		if ((MfComData.MfData[0] & 0x04) != 0x04)		//UID完整，7字节UID
		{
			*size = 7;
			memcpy(snr, tempsn + 1, *size);
			if ((MfComData.MfData[0] & 0x20) == 0x20)	//兼容ISO14443-4
			{
				g_ISO14443_4 = 1;
			}
			else
			{
				g_ISO14443_4 = 0;
			}
			return status;
		}

		//TODO: 10字节UID，待实现
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//Mifare_Pro复位
//input: parameter = PCD BUFERR SIZE
//output:pLen      = 复位信息长度
//       pData     = 复位信息
/////////////////////////////////////////////////////////////////////
char MifareProRst(unsigned char parameter, unsigned char *pLen,
		unsigned char *pData)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	pi = &MfComData;

	PcdSetTmo(15);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = 2;
	MfComData.MfData[0] = PICC_RESET;
	MfComData.MfData[1] = parameter;

	status = PcdComTransceive(pi);
	if ((*pLen = MfComData.MfLength / 8) <= FSD)
	{
		memcpy(pData, &MfComData.MfData[0], *pLen);
	}
	else
	{
		status = (char)MI_COM_ERR;
	}
	return status;
}

/*
 char MifareProPPS(unsigned char parameter,unsigned char *pLen,unsigned char *pData)
 {
 char status;
 struct TranSciveBuffer MfComData;
 struct TranSciveBuffer *pi;
 pi = &MfComData;

 PcdSetTmo(15);

 MfComData.MfCommand = PCD_TRANSCEIVE;
 MfComData.MfLength  = 3;
 MfComData.MfData[0] = PICC_PPS;
 MfComData.MfData[1] = 0x11;		//PPS0
 MfComData.MfData[2] = parameter;	//PPS1

 status = PcdComTransceive(pi);
 if ((*pLen = MfComData.MfLength/8) <= FSD)
 {   memcpy(pData, &MfComData.MfData[0], *pLen);   }
 else
 {   status = MI_COM_ERR;   }
 return status;
 }
 */

char MifareTransmit(unsigned char *request, unsigned int requestLength,
		unsigned char *response, unsigned int *responseLength,
		unsigned char timeout)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;

	PcdSetTmo(timeout);

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = requestLength;
	memcpy(&MfComData.MfData[0], request, requestLength);

	pi = &MfComData;
	status = PcdComTransceive(pi);
	if (status == MI_OK)
	{
		*responseLength = MfComData.MfLength / 8;
		memcpy(response, &MfComData.MfData[0], *responseLength);
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//向ISO14443-4卡发送COS命令
//input:CID_NAD  = 是否包含CID或NAD
//      timeout  = FWI
//      pLen     = 命令长度
//      pCommand = COS命令
//ouput:pLen     = 返回数据长度
//      pCommand = 返回数据
/////////////////////////////////////////////////////////////////////
char MifareProCom(unsigned char CID_NAD, unsigned char timeout,unsigned char *pLen, unsigned char *pCommand)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;
	unsigned char i, CidNadLg, PCB_I, PCB_R;
	unsigned char sendlgok, sendlgnow, sendlgsum, recelgnow, recelgsum;

	recelgnow = 0x00;
	recelgsum = 0x00;
	sendlgnow = 0x00;
	sendlgok = 0x00;
	sendlgsum = *pLen;
	PCB_R = 0xA2;

	PcdSetTmo(timeout);

	switch (CID_NAD)
	{
	case 4:
	case 8:
		CidNadLg = 1;
		break;
	case 0x0C:
		CidNadLg = 2;
		break;
	default:
		CidNadLg = 0;
		break;
	}

	if (sendlgsum > g_cFSC - 1 - CidNadLg)	//数据中不包括CRC，自动填充
	{
		sendlgnow = g_cFSC - 1 - CidNadLg;
		sendlgsum -= sendlgnow;
		PCB_I = 0x12;
	}
	else
	{
		sendlgnow = sendlgsum;
		PCB_I = 0x02;
	}

	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = sendlgnow + CidNadLg + 1;
	MfComData.MfData[0] = 0x02 | CID_NAD | PCB_I | g_cBlockNumber;
	for (i = 0; i < CidNadLg; i++)
	{
		MfComData.MfData[i + 1] = 0;
	}
	memcpy(&MfComData.MfData[CidNadLg + 1], pCommand, sendlgnow);
	sendlgok += sendlgnow;
	pi = &MfComData;

	status = PcdComTransceive(pi);

	//////////////////////////////////////////////
	while (status == MI_OK)
	{
		if (MfComData.MfLength / 8 - 1 - CidNadLg > FSD)
		{
			status = (char)MI_COM_ERR;
			break;
		}
		if ((MfComData.MfData[0] & 0xF0) == 0x00) 		//I block非链接
		//命令通讯结束
		{
			recelgnow = MfComData.MfLength / 8 - 1 - CidNadLg;
			memcpy(pCommand + recelgsum, &MfComData.MfData[CidNadLg + 1],recelgnow);
			recelgsum += recelgnow;
			*pLen = recelgsum;
			g_cBlockNumber ^= 1;
			break;
		}
		if ((MfComData.MfData[0] & 0xF0) == 0xA0)		//ACK
		//发送后续数据
		{
			MfComData.MfCommand = PCD_TRANSCEIVE;
			for (i = 0; i < CidNadLg; i++)
			{
				MfComData.MfData[i + 1] = 0;
			}
			//PCB_I ^= 1;
			g_cBlockNumber ^= 1;
			if (sendlgsum > g_cFSC - 1 - CidNadLg)
			{
				sendlgnow = g_cFSC - 1 - CidNadLg;
				sendlgsum -= sendlgnow;
				PCB_I |= 0x10;
			}
			else
			{
				sendlgnow = sendlgsum;
				PCB_I &= 0xEF;
			}
			MfComData.MfLength = sendlgnow + CidNadLg + 1;
			MfComData.MfData[0] = 0x02 | CID_NAD | PCB_I | g_cBlockNumber;

			memcpy(&MfComData.MfData[CidNadLg + 1], pCommand + sendlgok,sendlgnow);
			sendlgok += sendlgnow;
			pi = &MfComData;
			status = PcdComTransceive(pi);
			continue;
		}
		if ((MfComData.MfData[0] & 0xF0) == 0x10)		//I block链接
		//接收后续数据
		{
			recelgnow = MfComData.MfLength / 8 - 1 - CidNadLg;
			memcpy(pCommand + recelgsum, &MfComData.MfData[CidNadLg + 1],
					recelgnow);
			recelgsum += recelgnow;

			MfComData.MfCommand = PCD_TRANSCEIVE;
			for (i = 0; i < CidNadLg; i++)
			{
				MfComData.MfData[i + 1] = 0;
			}
			MfComData.MfLength = CidNadLg + 1;
			if (MfComData.MfData[0] & 0x01)
			{
				PCB_R &= 0xFE;
			}
			else
			{
				PCB_R |= 0x01;
			}
			MfComData.MfData[0] = PCB_R | (CID_NAD & 0xFB);
			pi = &MfComData;

			status = PcdComTransceive(pi);
			continue;
		}
		if (MfComData.MfData[0] == 0xF2)		//S block WTX request
		{
			recelgnow = MfComData.MfLength / 8 - 1 - CidNadLg;
			memcpy(pCommand + recelgsum, &MfComData.MfData[CidNadLg + 1],recelgnow);
			recelgsum += recelgnow;

			MfComData.MfCommand = PCD_TRANSCEIVE;
			for (i = 0; i < CidNadLg; i++)
			{
				MfComData.MfData[i + 1] = 0;
			}
			MfComData.MfLength = CidNadLg + 1;
			if (MfComData.MfData[0] & 0x01)
			{
				PCB_R &= 0xFE;
			}
			else
			{
				PCB_R |= 0x01;
			}
			MfComData.MfData[0] = PCB_R | (CID_NAD & 0xFB);
			pi = &MfComData;

			status = PcdComTransceive(pi);
			continue;
		}

		status = (char)MI_COM_ERR;
		break;
	}
	return status;
}

char NFCTransmit(unsigned char *pLen, unsigned char *pData)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;

	pi = &MfComData;

	PcdSetTmo(15);
	MfComData.MfCommand = PCD_TRANSCEIVE;
	MfComData.MfLength = *pLen;
	memcpy(MfComData.MfData, pData, *pLen);
	status = PcdComTransceive(pi);
	if(0x0 == status)
	{
		if ((*pLen = MfComData.MfLength / 8) <= FSD)
		{
			memcpy(pData, &MfComData.MfData[0], *pLen);
		}
	}
	else
	{
		status = (char)MI_COM_ERR;
	}

	return status;
}

