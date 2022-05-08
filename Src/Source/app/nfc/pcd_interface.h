/*
 * pcd_interface.h
 *
 *  Created on: 2018年8月28日
 *      Author: CCORE YY
 */

#ifndef PCD_INTERFACE_H_
#define PCD_INTERFACE_H_

#include "fm175xx.h"

#define MI_OK 0
//#define
#define MI_NOTAGERR                    (char)(-1)
#define MI_CHK_FAILED                  (char)(-1)
#define MI_CRCERR                      (char)(-2)
#define MI_CHK_COMPERR                 (char)(-2)
#define MI_EMPTY                       (char)(-3)
#define MI_AUTHERR                     (char)(-4)
#define MI_PARITYERR                   (char)(-5)
#define MI_CODEERR                     (char)(-6)
#define MI_SERNRERR                    (char)(-8)
#define MI_KEYERR                      (char)(-9)
#define MI_NOTAUTHERR                  (char)(-10)
#define MI_BITCOUNTERR                 (char)(-11)
#define MI_BYTECOUNTERR                (char)(-12)
#define MI_IDLE                        (char)(-13)
#define MI_TRANSERR                    (char)(-14)
#define MI_WRITEERR                    (char)(-15)
#define MI_INCRERR                     (char)(-16)
#define MI_DECRERR                     (char)(-17)
#define MI_READERR                     (char)(-18)
#define MI_OVFLERR                     (char)(-19)
#define MI_POLLING                     (char)(-20)
#define MI_FRAMINGERR                  (char)(-21)
#define MI_ACCESSERR                   (char)(-22)
#define MI_UNKNOWN_COMMAND             (char)(-23)
#define MI_COLLERR                     (char)(-24)
#define MI_RESETERR                    (char)(-25)
#define MI_INITERR                     (char)(-25)
#define MI_INTERFACEERR                (char)(-26)
#define MI_ACCESSTIMEOUT               (char)(-27)
#define MI_NOBITWISEANTICOLL           (char)(-28)
#define MI_QUIT                        (char)(-30)
#define MI_RECBUF_OVERFLOW             (char)(-50)
#define MI_SENDBYTENR                  (char)(-51)
#define MI_SENDBUF_OVERFLOW            (char)(-53)
#define MI_BAUDRATE_NOT_SUPPORTED      (char)(-54)
#define MI_SAME_BAUDRATE_REQUIRED      (char)(-55)
#define MI_WRONG_PARAMETER_VALUE       (char)(-60)
#define MI_BREAK                       (char)(-99)
#define MI_NY_IMPLEMENTED              (char)(-100)
#define MI_NO_MFRC                     (char)(-101)
#define MI_MFRC_NOTAUTH                (char)(-102)
#define MI_WRONG_DES_MODE              (char)(-103)
#define MI_HOST_AUTH_FAILED            (char)(-104)
#define MI_WRONG_LOAD_MODE             (char)(-106)
#define MI_WRONG_DESKEY                (char)(-107)
#define MI_MKLOAD_FAILED               (char)(-108)
#define MI_FIFOERR                     (char)(-109)
#define MI_WRONG_ADDR                  (char)(-110)
#define MI_DESKEYLOAD_FAILED           (char)(-111)
#define MI_WRONG_SEL_CNT               (char)(-114)
#define MI_WRONG_TEST_MODE             (char)(-117)
#define MI_TEST_FAILED                 (char)(-118)
#define MI_TOC_ERROR                   (char)(-119)
#define MI_COMM_ABORT                  (char)(-120)
#define MI_INVALID_BASE                (char)(-121)
#define MI_MFRC_RESET                  (char)(-122)
#define MI_WRONG_VALUE                 (char)(-123)
#define MI_VALERR                      (char)(-124)
#define MI_COM_ERR                     (char)(-125)

#define PCD_TRANSCEIVE Transceive
#define PCD_AUTHENT	   MFAuthent

#define PCD_IDLE              0x00               //取消当前命令
#define PCD_WRITEE2           0x01               //写EEPROM
#define PCD_READE2            0x03               //读EEPROM
#define PCD_LOADCONFIG        0x07               //调EEPROM中保存的寄存器设置
#define PCD_LOADKEYE2         0x0B               //将EEPROM中保存的密钥调入缓存
#define PCD_AUTHENT1          0x0C               //验证密钥第一步
#define PCD_AUTHENT2          0x14               //验证密钥第二步
#define PCD_RECEIVE           0x16               //接收数据
#define PCD_LOADKEY           0x19               //传送密钥
#define PCD_TRANSMIT          0x1A               //发送数据
//#define PCD_TRANSCEIVE        0x1E               //发送并接收数据
#define PCD_RESETPHASE        0x3F               //复位
#define PCD_CALCCRC           0x12               //CRC计算

struct TranSciveBuffer
{
	unsigned char MfCommand;
    unsigned int  MfLength;
    unsigned char MfData[64];
};

extern unsigned char ReadRawRC(unsigned char Address);
extern void WriteRawRC(unsigned char Address, unsigned char value);
extern void SetBitMask( unsigned char reg, unsigned char mask );
extern void ClearBitMask( unsigned char reg, unsigned char mask );
extern void PcdSetTmo(unsigned char fwi);
extern void PcdHWReset(void);
extern void PcdSoftReset(void);
extern void PcdAntennaOn(void);
extern void PcdAntennaOff(void);
extern char PcdConfigISOType(unsigned char type);
extern char PcdComTransceive(struct TranSciveBuffer *pi);

#endif /* PCD_INTERFACE_H_ */
