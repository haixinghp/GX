#include "sys.h"
#include "debug.h"
#include "type.h"
#include "uart.h"
#include "gpio.h"
#include "ClockAndPower.h"
#include "Timer.h"
#include "Flash.h"
#include "cache.h"
#include "sensorSPI.h"
#include "algorithm_bin_interface.h"
#include "string.h"
#include "fsm.h"
#include "system.h"


#define MAX_FINGERPRINT_TOTALS (10)


U8 g_gint1_int_flag = 0;
#define SSI_PAGE_SIZE		0x100
#define SSI_SECTOR_SIZE	0x1000
#define SSI_DATA_LEN		SSI_PAGE_SIZE
//volatile unsigned int sector_addr = 0x000000;
//unsigned char __attribute__((aligned (4))) src_data[300] ={0} ;//source bufer addr
//unsigned char __attribute__((aligned (4))) dst_data[300] ={0};//destination bufer addr

void SPI_Flash_Write_Page(U8* pBuffer,U32 WriteAddr,U16 NumByteToWrite);
void SPI_Flash_Erase_Sector(U32 Dst_Addr);





void gpio_test(void)
{
	EPORT_ControlConfig_t config;
	config.TriggerMode = EPORT_HIGH_LEVEL_TRIGGER;
	config.DirectionOutput = 1;
	config.EnablePullup = 0;
	config.OpenDrain = 0;
	EPORT_PinControlConfig(&EPORT, 0, &config); //GINT0¨¦¨¨?a¨º?3?

#if 0
	config.TriggerMode = EPORT_RISING_EDGE_TRIGGER;
	config.DirectionOutput = 0;
	EPORT_PinControlConfig(&EPORT, 4, &config); //GINT4¨¦¨¨?a¨º?¨¨???¦Ì???¡ä£¤¡¤¡é
	EPORT_EnableInterrupt(&EPORT, 4); //?¨ºD¨ªGINT4?D??
	
	NVIC_Init(3, 3, EPORT0_4_IRQn, 2);
#endif	
}

//static uint8_t flash_write_buffer[512] = {0xFF};
//static uint8_t flash_read_buffer[512] = {0xFF};

//void Init_Buf(unsigned char *u8buf,unsigned int buflen,unsigned char inittype,unsigned char initval)
//{
//	unsigned int i;

//	if(inittype == 0)
//	{
//		memset(u8buf,initval,buflen);
//	}
//		else
//	{
//		for(i=0;i<buflen;i++)
//		{
//			u8buf[i] = i;
//		}
//	}
//}

/*
 * Clean all fingerprint
 *
 */
 void clean_all_fingerprint(void)
 {

     U8 res = 0;
     U8 buffer[32] = {0x00};

     buffer[0] = 0;
     buffer[1] = 0;
     buffer[2] = 0;
     buffer[3] = MAX_FINGERPRINT_TOTALS;
 
     res = binlib_api(LIB_DELETE_ID,buffer,4);
     
     if( (res&0x01) == 0)
     {
         printf("empty is ok  id \r\n");
     }
     else
     {
         printf("empty is failed  id\r\n");
     }
 }


 /*
  * regist the indicate fingerprint
  * @fp_id fingerprint ID
  *
  */
 void regist_enroll(U8 fp_id)
 {
	U8 res = 0;
//    U8 enroll_id = fp_id;
    U8 buffer[32] = {0x00};
    buffer[0] = 0x00;
    buffer[1] = (fp_id&0x7F)<<1;

    while(1)
	{
		res = binlib_api(LIB_PRE_ENROLL,buffer,0);
        printf("binlib_api(LIB_PRE_ENROLL,buffer,0);");    
		if( (res&0x01) == 0)
			break;
	}


      

	while(1)
	{
		res=binlib_api(LIB_ENROLL,buffer,2);
        
		if( (res&0x01) == 0)
		{
			printf("single enroll is ok :%d\r\n",fp_id);
		}
		else
		{
			printf("single enroll is failed:%d\r\n",fp_id);
		}
        
		break;
	}

        
	while(1)
	{
		res=binlib_api(LIB_IS_FINGERLEAVE,buffer,0);
		if( (res&0x01) == 0)
			break;
		//else
    	//	printf("error\r\n");
	}
        
    printf("Finger Leave\r\n");

}



/*
 * save the indicate fingerprint
 * @fp_id fingerprint ID
 *
 */

void save_enroll(const U8 fp_id)
{
	U8 res = 0;
    U8 buffer[32] = {0x00};

    buffer[0] = 0x00;
    buffer[1] = (fp_id&0x7F)<<1;
    buffer[1] |= 0x01;
    
    while(1)
    {
        res=binlib_api(LIB_ENROLL,buffer,2);
        if( (res&0x01) == 0)
        {
            printf("***************single post enroll is ok:%d****************\r\n",fp_id);
        }
        else
        {
            printf("single post enroll is failed:%d\r\n",fp_id);
        }
        break;
    }
}


/*
 * Match Fingerprint
 */
U8 match_fingerprint(U8 *fingerprint_index)
{
	U8 res = 0;
    U8 buffer[32] = {0x00};


	while(1)
	{
		res = binlib_api(LIB_PRE_MATCH,buffer,0);
		printf(" binlib_api(LIB_PRE_MATCH,buffer,0);");
		if( (res&0x01) == 0)
			break;
	}

	while(1)
	{
		res = binlib_api(LIB_MATCH,buffer,2);
		if( (res&0x01) == 0)
		{
			printf("########################match is ok id=%d ###################\r\n",buffer[0]*10+buffer[1]);
            
		    
		}
		else
		{
			printf("match is failed!\r\n");
		}

        break;
	}

    *fingerprint_index = (buffer[0]*10+buffer[1]);
    return res;
}

extern void deviceReady(void);
int tmp_buf[79 * 1024 / 4] = {0x00};/* Buffer must >= 79k bytes */

uint8_t * get_image_data(uint32_t *image_length, uint16_t *width,uint16_t *heigth)
{
	uint8_t buff[12] = {0x00};
	uint32_t image_addr = 0x00;
	
	memset(tmp_buf,0,sizeof(tmp_buf));
	binlib_api(0x0E, buff, 12);

	*image_length = (buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|(buff[3]);
	*width = (buff[4]<<8)|buff[5];
	*heigth = (buff[6]<<8)|buff[7];
/*	
	LOGE("buff[0]:%x\r\n",buff[0]);
	LOGE("buff[1]:%x\r\n",buff[1]);
	LOGE("buff[2]:%x\r\n",buff[2]);
	LOGE("buff[3]:%x\r\n",buff[3]);
	 
	LOGE("buff[4]:%x\r\n",buff[4]);
	LOGE("buff[5]:%x\r\n",buff[5]);
	 
	LOGE("buff[6]:%x\r\n",buff[6]);
	LOGE("buff[7]:%x\r\n",buff[7]);
	 
	LOGE("buff[8]:%x\r\n",buff[8]);
	LOGE("buff[9]:%x\r\n",buff[9]);
	LOGE("buff[10]:%x\r\n",buff[10]);
	LOGE("buff[11]:%x\r\n",buff[11]);
*/
	
	image_addr = (buff[8]<<24)|(buff[9]<<16)|(buff[10]<<8)|(buff[11]);
	return (uint8_t *)image_addr;

}
void hardwareInit(void)
{
	//clockInit();
	//gpioInit();
//	debugInit(115200);
	timerInit();
	flashInit();

	
	sensorSpiInit();
}
UINT8 test_spi;
void MRI_Sensor_Demo()
{
	test_spi=1;
	VU32 us, i, j,k;
	S32 ch1;
	
	u8 buff[2] = {0};
	u32 res = 0;
    
    U8 fingerprint_index = 0;
	
	


	hardwareInit();

//	uartInit(115200);
	

  printf("SPI FLASH ID is:%x\r\n",SPI_Flash_ReadID());
	
#if 1	

  printf("0000\r\n");
  binlib_init_data_bss_sections();
	binlib_dbg_en(1);

  printf("1111\r\n");
  tmp_buf[0] = 0x55AAEE44;
	tmp_buf[1] = 0xA5668899;
  binlib_init_buffer(tmp_buf,sizeof(tmp_buf));
	
	res = binlib_init(bin_funcs_table);
 
  printf("binlib res = %d\r\n",res);

	printf("Chip ID is :%x\r\n",binlib_api(0x0D, buff, 2));
/*
  if (!res){
		printf("Init bin library successful\r\n");
	}else{
		printf("Init bin library failed,hang on...\r\n");
		while(1);
	}

	if (!res){
		printf("Init bin library successful\r\n");
	}else{
		buff[0] = 0;
		res = binlib_api(LIB_INIT_SENSOR, buff, 2);
		if (res) {
			printf("Init Sensor Param Failed\r\n");
			while(1);
		}else{
			printf("Init sensor successful...\r\n");
		}

	}
*/	

   if (!res){
		printf("Init bin library successful\r\n");
	}else{
		buff[0] = 0;
		res = binlib_api(LIB_INIT_SENSOR, buff, 2);
		if (res) {
			printf("Init Sensor Param Failed\r\n");
			while(1);
		}else{
			printf("Init sensor successful...\r\n");
		}
   
      buff[0] = 1;
      res = binlib_api(LIB_INIT_SENSOR, buff, 2);
      if (res) {
        printf("Init Flash Failed\r\n");
        while(1);
      }else{
        printf("Init Flash successful...\r\n");
      }
	}
#endif

//  initDeviceInfo();
//	printf("2222\r\n");
//	fsmInit();
//	//printf("1111\r\n");
//	deviceReady();
//	printf("3333\r\n");

//	while(1)
//	{
//		uartReadByte(&ch1);
//		fsmTask(ch1);
//	}
 clean_all_fingerprint();//?????¨´¨®D?????a*/
	test_spi=0;
	
	/*?a¡À?¡Á¡é2¨¢¨¨y??????¡Á?2a¨º?¡ê?¡Á¡é¨°a¡ê???¨°???????¡Á¡é2¨¢¡ä?¨ºy¨¨y¡ä?¡ê?¨¨?¨°a?¨¹??¡Á¡é2¨¢¡ä?¨ºy¡ê??¨ª?¨¤¦Ì¡Â¨®???¡ä?regist_enroll?¡ä?¨¦*/
	for(i=0;i<3;i++)
	{
		

      regist_enroll(i);//2a¨º?¨º¡Ào¨°¨º???¡ã¡ä??o¨®?¨´???a2??¨¢¨ª¨´??¡Á?
			regist_enroll(i);	 
			regist_enroll(i);
			save_enroll(i);
	}
	/*?£¤??2a¨º?¨¢¡Â3¨¬*/
	    while (1)
			{
				if (!(match_fingerprint(&fingerprint_index)&0x01))
				{
					
					//okay++;
					printf("Fingerprint index:%d\r\n",fingerprint_index);
				}
				else
				{
					
					//error++;
				}
			
				
				
			}


#if 0
	if (res) {
		buff[0] = 0;
		res = binlib_api(LIB_INIT_SENSOR, buff, 2);
		if (res) {
			printf("Init Sensor Param Failed\r\n");
		}else{
			printf("Init Sensor Param successful\r\n");
		}
	}
#endif
	

    /*get_fingerprint_totals(&totals, &fingerprint_mask);
    printf("1:totals:%d,mask:%x \r\n",totals,fingerprint_mask);
	clean_all_fingerprint();
    get_fingerprint_totals(&totals, &fingerprint_mask);
    printf("2:totals:%d,mask:%x \r\n",totals,fingerprint_mask);*/


#if 0
    if (mafp_get_used_enrollids()){
		printf("Enroll totals:%d",mafp_get_used_enrollids());
    	while (1)
    	{
            if (!(match_fingerprint(&fingerprint_index)&0x01))
            {
                //okay++;
                
                printf("Fingerprint index:%d\r\n",fingerprint_index);
            }else{
                //error++;
            }
    	}
	
	}else{
		printf("Please enroll %d fingerprints,each fingerprint enroll 3 times",MAX_FINGERPRINT_TOTALS);
	
    	for (i = 0; i < MAX_FINGERPRINT_TOTALS; i++)
    	{
        	regist_enroll(i);
        	regist_enroll(i);    
        	regist_enroll(i);
        	save_enroll(i);
    	}

		
		printf("Enroll all done, and test the match function.\r\n");
		while (1)
		{
			if (!(match_fingerprint(&fingerprint_index)&0x01))
			{
			//okay++;
			
				printf("Fingerprint index:%d\r\n",fingerprint_index);
			}else{
			//error++;
			}
		}
	}

#endif

#if 0
		k = 0;
		
		while (k < 10)
		{
				if (!(match_fingerprint(&fingerprint_index)&0x01))
				{
					//okay++;
					
					printf("Fingerprint index:%d\r\n",fingerprint_index);
				}else{
					//error++;
					
					k++;
				}
		}
	
		printf("1 Enroll totals:%d",mafp_get_used_enrollids());
	
	
		for (i = 0; i < MAX_FINGERPRINT_TOTALS; i++)
		{
			
			regist_enroll(i);
			regist_enroll(i);	 
			regist_enroll(i);
			save_enroll(i);
		}
	
		printf("2 Enroll totals:%d",mafp_get_used_enrollids());
		for (i = 0; i < MAX_FINGERPRINT_TOTALS; i++)
		{
			
			regist_enroll(i);
			regist_enroll(i);	 
			regist_enroll(i);
			save_enroll(i);
		}
		
		printf("3 Enroll totals:%d",mafp_get_used_enrollids());
		
		while (1)
		{
				if (!(match_fingerprint(&fingerprint_index)&0x01))
				{
					//okay++;
					
					printf("Fingerprint index:%d\r\n",fingerprint_index);
				}else{
					//error++;
				}
		}
	
	
	
		
	
#endif





}	

