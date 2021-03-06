#ifndef _ISO_14443A_H_
#define _ISO_14443A_H_

/////////////////////////////////////////////////////////////////////
//ISO14443A COMMAND
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠
#define PICC_RESET            0xE0               //复位
#define PICC_PPS            0xD0               //PPS


/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *atq);                     
char PcdAnticoll(unsigned char *snr);                                   
char PcdSelect(unsigned char *snr,unsigned char *_size);                        
char PcdHalt(void);                                            

char MifareTransmit(unsigned char *request, unsigned int requestLength,
		unsigned char *response, unsigned int *responseLength,
		unsigned char timeout);

char ChangeCodeKey(unsigned char *uncoded,unsigned char *coded);                
char PcdAuthKey(unsigned char *coded);                                  
char PcdLoadKeyE2(unsigned int startaddr);
char PcdAuthState(unsigned char auth_mode,unsigned char block,unsigned char *snr);      
char PcdRead(unsigned char addr,unsigned char *readdata);                       
char PcdWrite(unsigned char addr,unsigned char *writedata);                     
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *value);           
char PcdRestore(unsigned char addr);                                    
char PcdTransfer(unsigned char addr);                                  
char TypeaAnticoll(unsigned char *snr, unsigned char *size);

char UL_PcdAnticoll(unsigned char *snr); 
char UL_PcdWrite(unsigned char addr,unsigned char *writedata);

char MifareProRst(unsigned char parameter,unsigned char *pLen,unsigned char *pData);
char MifareProCom(unsigned char CID_NAD,unsigned char timeout,unsigned char *pLen,unsigned char *pCommand);

char SHC1102Auth(unsigned char *password);
char SHC1102Read(unsigned char addr, unsigned char *pData);
char SHC1102Write(unsigned char addr,unsigned char *pData);

#endif

