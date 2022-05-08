
#include "type.h"
#include "pcd_interface.h"
#include "iso14443b.h"
#include <string.h>
#include "emv_layer4.h"

//////////////////////////////////////////////////////////////////////
//REQUEST B
//////////////////////////////////////////////////////////////////////
char PcdRequestB(unsigned char req_code, 
                      unsigned char AFI, 
                      unsigned char N, 
                      unsigned char *ATQB)
{
    char  status = MI_OK;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;

    pi=&MfComData;
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 3;
    MfComData.MfData[0] = ISO14443B_ANTICOLLISION;     	       // APf code
    MfComData.MfData[1] = AFI;                // 
    MfComData.MfData[2] = ((req_code<<3)&0x08) | (N&0x07);  // PARAM
    PcdSetTmo(106);
    status = PcdComTransceive(pi);

    if((status!=MI_OK) && (status!=MI_NOTAGERR))
    {
    	status = (char)MI_COLLERR;
    }
    
    if (MfComData.MfLength != 96)
    {
    	status = (char)MI_COM_ERR;
    }
    
    if (status == MI_OK) 
    {	
    	memcpy(ATQB, &MfComData.MfData[0], 16);
        PcdSetTmo(ATQB[11]>>4); // set FWT 
    } 	

    return status;
}                      

//////////////////////////////////////////////////////////////////////
//SLOT-MARKER
//////////////////////////////////////////////////////////////////////
/*char M531PiccSlotMarker(unsigned char N, unsigned char *ATQB)
{
    char status = MI_OK;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    PcdSetTmo(5);

    if(!N || N>15) status = MI_WRONG_PARAMETER_VALUE;	
    else
    {
       MfComData.MfCommand=PCD_TRANSCEIVE;
       MfComData.MfLength=1;
       MfComData.MfData[0]=0x05|(N<<4); // APn code
        
       status=PcdComTransceive(pi);

       if (status == MI_CRCERR) status = MI_COLLERR; // collision occurs

       if (status == MI_OK) 
       {	
          memcpy(ATQB, &MfComData.MfData[0], 16);
          PcdSetTmo(ATQB[11]>>4); // set FWT 
       } 	
    }
    return status;
}                      
*/

//////////////////////////////////////////////////////////////////////
//ATTRIB
//////////////////////////////////////////////////////////////////////
char PcdAttrib(unsigned char *PUPI,unsigned char PARAM3,unsigned char *answer)
{
    char  status = MI_OK;
//	u8 rate = 0;
//	u8 param = 0;
	
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 9;
    MfComData.MfData[0] = ISO14443B_ATTRIB;
    memcpy(&MfComData.MfData[1], PUPI, 4);
    MfComData.MfData[5] = 0x00;  	    // EOF/SOF required, default TR0/TR1
    /*
#ifdef VERSION_RC531

#else
    SetBitMask(TypeBReg, BIT7 | BIT6); //EOF SOF required
#endif
    */
    MfComData.MfData[6] = FSDI; // Max frame 64 
    MfComData.MfData[7] = PARAM3;  	    // Param3, ISO/IEC 14443-4 compliant?
//    MfComData.MfData[8] = 1;  	    // CID
    MfComData.MfData[8] = 0;  	    // CID XGL

#ifdef ISO14443_SUPPORT_PPS
	if ((g_cPcdPicc&0x04) == 0x04)		//DR=8
    	{
		MfComData.MfData[6] |= 0x30;
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4
	{
		MfComData.MfData[6] |= 0x20;
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2
	{
		MfComData.MfData[6] |= 0x10;
	}
	
	if ((g_cPiccPcd&0x04) == 0x04)		//DS=8
    	{
		MfComData.MfData[6] |= 0xC0;
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4
	{
		MfComData.MfData[6] |= 0x80;
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2
	{
		MfComData.MfData[6] |= 0x40;
	}
	param = MfComData.MfData[6];
#endif

    status  = PcdComTransceive(pi);
    
    *answer = MfComData.MfData[0];
	
#ifdef ISO14443_SUPPORT_PPS
	#ifdef VERSION_RC531
	if ((g_cPcdPicc&0x04) == 0x04)		//DR=8, tx
    	{
		WriteRawRC(RegCoderControl,0x08);
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4
	{
		WriteRawRC(RegCoderControl,0x10);
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2
	{
		WriteRawRC(RegCoderControl,0x18);
	}
	
	if ((g_cPiccPcd&0x04) == 0x04)		//DS=8, rx
    	{
		WriteRawRC(RegRxControl1,0x13);
		WriteRawRC(RegRxThreshold,0x50);
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4
	{
		WriteRawRC(RegRxControl1,0x33);
		WriteRawRC(RegRxThreshold,0x50);
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2
	{
		WriteRawRC(RegRxControl1,0x53);
		WriteRawRC(RegRxThreshold,0x50);
	}
	#else	//VERSION_RC523
	if ((g_cPcdPicc&0x04) == 0x04)		//DR(PCD to PICC)=8,848k, tx
    	{
    		SetBitMask(TxModeReg, BIT5|BIT4);
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4,424k
	{
    		SetBitMask(TxModeReg, BIT5);
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2,212k
	{
    		SetBitMask(TxModeReg, BIT4);
	}
	
	if ((g_cPiccPcd&0x04) == 0x04)		//DS(PICC to PCD)=8,848k, rx
    	{
    		SetBitMask(RxModeReg, BIT5|BIT4);
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4,424k
	{
    		SetBitMask(RxModeReg, BIT5);
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2,212k
	{
    		SetBitMask(RxModeReg, BIT4);
	}
	#endif
#endif

	g_ISO14443_4 = 1;
    return status;
} 


//////////////////////////////////////////////////////////////////////
//ATTRIB
//////////////////////////////////////////////////////////////////////
char PcdAttrib_NoPPS(unsigned char *PUPI,unsigned char PARAM3,unsigned char *answer)
{
    char  status = MI_OK;
//	u8 rate = 0;
//	u8 param = 0;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 9;
    MfComData.MfData[0] = ISO14443B_ATTRIB;
    memcpy(&MfComData.MfData[1], PUPI, 4);
    MfComData.MfData[5] = 0x00;  	    // EOF/SOF required, default TR0/TR1
    /*
#ifdef VERSION_RC531

#else
    SetBitMask(TypeBReg, BIT7 | BIT6); //EOF SOF required
#endif
    */
    MfComData.MfData[6] = FSDI; // Max frame 64 
    MfComData.MfData[7] = PARAM3;  	    // Param3, ISO/IEC 14443-4 compliant?
//    MfComData.MfData[8] = 1;  	    // CID
    MfComData.MfData[8] = 0;  	    // CID XGL

    status  = PcdComTransceive(pi);
    
    *answer = MfComData.MfData[0];
	
	g_ISO14443_4 = 1;
    return status;
} 

//////////////////////////////////////////////////////////////////////
//ATTRIB
//////////////////////////////////////////////////////////////////////
char PcdIdentityCard(unsigned char *senddata, unsigned char sendlen, unsigned char *receivedata, unsigned char *receivelen)
{
    char  status = MI_OK;
//	u8 rate = 0;
//	u8 param = 0;
	
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = sendlen;
    memcpy(MfComData.MfData, senddata, sendlen);
    MfComData.MfData[0] = ISO14443B_ATTRIB;

    status  = PcdComTransceive(pi);

    *receivelen = MfComData.MfLength/8;
    memcpy(receivedata, MfComData.MfData, *receivelen);

    return status;
} 


//////////////////////////////////////////////////////////////////////
//HLTB
//////////////////////////////////////////////////////////////////////
char PcdHltb(unsigned char *PUPI)
{
    char  status = MI_OK;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
  
    PcdSetTmo(g_cFWI);
				                               // disable, ISO/IEC3390 enable	
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 5;
    MfComData.MfData[0] = ISO14443B_HLTB;
    memcpy(&MfComData.MfData[1], PUPI, 4);
    
    status = PcdComTransceive(pi);

    return status;
}                      

/////////////////////////////////////////////////////////////////////
//AT88RF020验证密码
//input:password=8字节密码
/////////////////////////////////////////////////////////////////////
char At88rf020Check(unsigned char *password)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    PcdSetTmo(g_cFWI);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_CHECK_PASSWORD;
    MfComData.MfData[1] = 0;
    memcpy(&MfComData.MfData[2], password, 8);   

    status = PcdComTransceive(pi);
    
    if ((MfComData.MfData[1]&0x01) || (MfComData.MfLength!=0x10))
    {    status = (char)MI_COM_ERR;    }
    return status;
}

/////////////////////////////////////////////////////////////////////
//读AT88RF020一页数据
//input :addr=页地址
//output:readdata=读出的8字节数据
/////////////////////////////////////////////////////////////////////
char At88rf020Read(unsigned char addr, unsigned char *readdata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(g_cFWI);
  //???WriteRawRC(RegTimerControl,0X0);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_READ;
    MfComData.MfData[1] = addr;
    
    status = PcdComTransceive(pi);
    
    if ((status==MI_OK) && (MfComData.MfLength==0x50))
    {   memcpy(readdata, &MfComData.MfData[2], 8);    }
    else
    {   status = (char)MI_COM_ERR;   }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//写AT88RF020一页数据
//input :addr=页地址
//       writedata=要写入的8字节数据
/////////////////////////////////////////////////////////////////////
char At88rf020Write(unsigned char addr,unsigned char *writedata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(g_cFWI);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_WRITE;
    MfComData.MfData[1] = addr;
    memcpy(&MfComData.MfData[2], writedata, 8);    

    status = PcdComTransceive(pi);
    
    if ((MfComData.MfData[1]&0x01) || (MfComData.MfLength!=0x10))
    {    status = (char)MI_COM_ERR;    }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//AT88RF020一LOCK
//input :lockdata=4字节数据
/////////////////////////////////////////////////////////////////////
char At88rf020Lock(unsigned char *lockdata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(g_cFWI);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_LOCK;
    MfComData.MfData[1] = 0;
    memcpy(&MfComData.MfData[2], lockdata, 4);    

    status = PcdComTransceive(pi);
    
    if ((MfComData.MfData[1]&0x01) || (MfComData.MfLength!=0x10))
    {    status = (char)MI_COM_ERR;    }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//AT88RF020计数操作
//input :signature = 6字节签名信息
/////////////////////////////////////////////////////////////////////
char At88rf020Count(unsigned char *signature)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(g_cFWI);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_COUNT;
    MfComData.MfData[1] = 0;
    memcpy(&MfComData.MfData[4], signature, 6);
    
    status = PcdComTransceive(pi);
    
    if((MfComData.MfData[1]&0x01) || (MfComData.MfLength!=0x10))
    {    status = (char)MI_COM_ERR;    }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//AT88RF020进入HALT状态
/////////////////////////////////////////////////////////////////////
char At88rf020Deselect()
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    PcdSetTmo(g_cFWI);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 10;
    MfComData.MfData[0] = AT88RF020_DESELECT;
    
    status = PcdComTransceive(pi);
    if((MfComData.MfData[1]&0x01) || (MfComData.MfLength!=0x10))
    {    status = (char)MI_COM_ERR;    }
    
    return status;
}

//////////////////////////////////////////////////////////////////////
//激活
//////////////////////////////////////////////////////////////////////
char SelectSR(unsigned char *Chip_ID)
{
    char  status = MI_OK;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

#ifdef VERSION_RC531
    ClearBitMask(RegControl,0x08);      // disable crypto 1 unit   
   // SetBitMask(RegTxControl,0x03);      // Tx2RF-En, Tx1RF-En enable
#else	//VERSION_RC523

#endif
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 2;
    MfComData.MfData[0] = 6;     	       
    MfComData.MfData[1] = 0;                
    
    status = PcdComTransceive(pi);

    if (status!=MI_OK && status!=MI_NOTAGERR) 
    {   status = (char)MI_COLLERR;   }          // collision occurs
    
    if(MfComData.MfLength != 8)
    {   status = (char)MI_COM_ERR;   }
    
    if (status == MI_OK)
    {	
         PcdSetTmo(5);
         MfComData.MfCommand = PCD_TRANSCEIVE;
         MfComData.MfLength  = 2;
         MfComData.MfData[1] = MfComData.MfData[0];     	       
         MfComData.MfData[0] = 0x0E;                 
         
         status = PcdComTransceive(pi); 
         
         if (status!=MI_OK && status!=MI_NOTAGERR)  // collision occurs
         {   status = (char)MI_COLLERR;   }               // collision occurs
         if (MfComData.MfLength != 8) 
         {   status = (char)MI_COM_ERR;     }
         if (status == MI_OK)
         {  *Chip_ID = MfComData.MfData[0];  }
    } 	
    
    return status;
}  

//////////////////////////////////////////////////////////////////////
//SR176卡读块
//////////////////////////////////////////////////////////////////////
char ReadSR176(unsigned char addr,unsigned char *readdata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 2;
    MfComData.MfData[0] = 8;
    MfComData.MfData[1] = addr;
  
    status = PcdComTransceive(pi);
  
    if ((status==MI_OK) && (MfComData.MfLength!=16))
    {   status = (char)MI_BITCOUNTERR;    }
    if (status == MI_OK)
    {
        *readdata     = MfComData.MfData[0];
        *(readdata+1) = MfComData.MfData[1];
    }
    return status;  
}  
//////////////////////////////////////////////////////////////////////
//SR176卡写块
//////////////////////////////////////////////////////////////////////
char WriteSR176(unsigned char addr,unsigned char *writedata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;

    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSMIT;
    MfComData.MfLength  = 4;
    MfComData.MfData[0] = 9;
    MfComData.MfData[1] = addr;
    MfComData.MfData[2] = *writedata;
    MfComData.MfData[3] = *(writedata+1);
    status = PcdComTransceive(pi);
    return status;  
}      

//////////////////////////////////////////////////////////////////////
//取SR176卡块锁定状态
//////////////////////////////////////////////////////////////////////                            
char GetProtSR176(unsigned char lockreg)
{
	char status;
	struct TranSciveBuffer MfComData;
	struct TranSciveBuffer *pi;

	status = (char)lockreg;
	pi = &MfComData;
	PcdSetTmo(5);
	MfComData.MfCommand = PCD_TRANSMIT;
	MfComData.MfLength  = 2;
	MfComData.MfData[0] = 8;
	MfComData.MfData[1] = 0x0f;
	status = PcdComTransceive(pi);
	if (status == MI_OK) { lockreg = MfComData.MfData[1];  }
	return status;  
}   	
//////////////////////////////////////////////////////////////////////
//                    SR176卡块锁定
//////////////////////////////////////////////////////////////////////
char ProtectSR176(unsigned char lockreg)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSMIT;
    MfComData.MfLength  = 4;
    MfComData.MfData[0] = 9;
    MfComData.MfData[1] = 0x0F;
    MfComData.MfData[2] = 0;
    MfComData.MfData[3] = lockreg;
    status = PcdComTransceive(pi);
    return status;  
}   

//////////////////////////////////////////////////////////////////////
//COMPLETION ST
//////////////////////////////////////////////////////////////////////
char CompletionSR()
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSMIT;
    MfComData.MfLength  = 1;
    MfComData.MfData[0] = 0x0F;
    status = PcdComTransceive(pi);
    return status;  
}                                          

//////////////////////////////////////////////////////////////////////
//SRIX4K卡读块
//////////////////////////////////////////////////////////////////////
char ReadSR4K(unsigned char addr,unsigned char *readdata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 2;
    MfComData.MfData[0] = 8;
    MfComData.MfData[1] = addr;
    status = PcdComTransceive(pi);
    if (status!=MI_OK) status = (char)MI_COLLERR; // collision occurs
    if (MfComData.MfLength!=32) status = (char)MI_COM_ERR;
    if (status == MI_OK)
    {
        *readdata     = MfComData.MfData[0];
        *(readdata+1) = MfComData.MfData[1];
        *(readdata+2) = MfComData.MfData[2];
        *(readdata+3) = MfComData.MfData[3];
    }
    return status;  
}

//////////////////////////////////////////////////////////////////////
//SR176卡写块
//////////////////////////////////////////////////////////////////////
char WriteSR4K(unsigned char addr,unsigned char *writedata)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSMIT;
    MfComData.MfLength  = 6;
    MfComData.MfData[0] = 9;
    MfComData.MfData[1] = addr;
    MfComData.MfData[2] = *writedata;
    MfComData.MfData[3] = *(writedata+1);
    MfComData.MfData[4] = *(writedata+2);
    MfComData.MfData[5] = *(writedata+3);
    status = PcdComTransceive(pi);
    return status;  
} 

//////////////////////////////////////////////////////////////////////
//SR176卡鉴别
//////////////////////////////////////////////////////////////////////
char AuthSR4K(unsigned char *rnd,unsigned char *sig)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    PcdSetTmo(9);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 7;
    MfComData.MfData[0] = 0x0A;
    memcpy(&MfComData.MfData[1], rnd, 6);
    
    status = PcdComTransceive(pi);
    
    if (status!=MI_OK) status = (char)MI_COLLERR; // collision occurs
    if (MfComData.MfLength!=24) status=(char)MI_COM_ERR;
    if (status == MI_OK)
    {
        * sig    = MfComData.MfData[0];
        *(sig+1) = MfComData.MfData[1];
        *(sig+2) = MfComData.MfData[2];
    }
    return status;  
}  

//////////////////////////////////////////////////////////////////////
//SR176卡读UID
//////////////////////////////////////////////////////////////////////
char GetUIDSR4K(unsigned char *UID)
{
    char status;
    unsigned char i;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(5);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 1;
    MfComData.MfData[0] = 0x0B;
 
    status = PcdComTransceive(pi);

    if (MfComData.MfLength!=64) status = (char)MI_COM_ERR;
    if (status == MI_OK)
    {
        for(i=0;i<8;i++)
        {  *(UID+7-i) = MfComData.MfData[i];  }
    }
    return status;  
}                                         

//////////////////////////////////////////////////////////////////////
//ISO14443 DESELECT
//////////////////////////////////////////////////////////////////////
char CL_Deselect(unsigned char CID)
{
    char status;
    struct TranSciveBuffer MfComData;
    struct TranSciveBuffer *pi;
    pi = &MfComData;
    
    PcdSetTmo(4);
    MfComData.MfCommand = PCD_TRANSCEIVE;
    MfComData.MfLength  = 2;
    MfComData.MfData[0] = 0xca;
    MfComData.MfData[1] = CID;
    status = PcdComTransceive(pi);
    return status;
}

