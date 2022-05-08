#ifndef _ISO14443B_H
#define _ISO14443B_H

/////////////////////////////////////////////////////////////////////
//ISO14443B COMMAND
///////////////////////////////////////////////////////////////////// 
#define	ISO14443B_ANTICOLLISION                  0x05
#define	ISO14443B_ATTRIB                         0x1D
#define	ISO14443B_HLTB                           0x50
//AT88RF020 COMMAND
#define AT88RF020_READ                           0x04
#define AT88RF020_WRITE                          0x03
#define AT88RF020_LOCK                           0x02
#define AT88RF020_CHECK_PASSWORD                 0x06
#define AT88RF020_DESELECT                       0x0A
#define AT88RF020_COUNT                          0x0E

/////////////////////////////////////////////////////////////////////
//º¯ÊýÔ­ÐÍ
/////////////////////////////////////////////////////////////////////
char PcdRequestB(unsigned char req_code, 
		      unsigned char AFI, 
		      unsigned char N, 
		      unsigned char *ATQB);
char PcdAttrib(unsigned char *PUPI,unsigned char PARAM3,unsigned char *answer);
char PcdHltb(unsigned char *PUPI);	 
            
char At88rf020Check(unsigned char *password);
char At88rf020Read(unsigned char addr,unsigned char *readdata);
char At88rf020Write(unsigned char addr,unsigned char *writedata);
char At88rf020Lock(unsigned char *lockdata);
char At88rf020Count(unsigned char *signature);
char At88rf020Deselect(void);

char SelectSR(unsigned char *Chip_ID);
char ReadSR176(unsigned char addr, unsigned char *readdata);
char WriteSR176(unsigned char addr,unsigned char *writedata);
char GetProtSR176(unsigned char lockreg);
char ProtectSR176(unsigned char lockreg);
char CompletionSR(void);	  
char ReadSR4K(unsigned char addr,unsigned char *readdata);
char WriteSR4K(unsigned char addr,unsigned char *writedata);
char AuthSR4K(unsigned char *rnd,unsigned char *sig);
char GetUIDSR4K(unsigned char *UID);

char CL_Deselect(unsigned char CID);

#endif
