/*
 * Bin library functions interface for user caller
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    10/10/17     For external user call the functions
 */
#include "uart.h"
#include "timer.h"
#include "flash.h"
#include "sensor.h"
#include <stdint.h>
#include "algorithm_bin_interface.h"
#include "SensorSpi.h"
#include "string.h"
#include "fsm.h"

void sensorSpiWriteRead(const void* cpWriteBuf, int nWriteSize, void* pReadBuf, int nReadSize);

/******************************* Call by algorithm bin *************************************/
#define lib_uartSendBuff         uartSendBuff
#define lib_systemTick           systemTickMs
#define lib_sensorRWReg          sensorSpiWriteRead
#define lib_sensorSetMode        (void *)0
#define lib_sensorReadLine       (void *)0
#define lib_sensorReadFrame      (void *)0
#define lib_eflash_program       eflash_program_withkey
#define lib_eflash_bulk_program  eflash_bulk_program_withkey
#define lib_eflash_page_erase    eflash_page_erase_withkey
#define lib_spiflash_read        SPI_Flash_Read 
#define lib_spiflash_write       SPI_Flash_Write_Sectors
#define lib_fsmDebugPrint        fsmSendFrame

//__attribute__((section(".ARM.__at_0x00801080")))
extern void fsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf);

/* Define a function array for init the peripheral device */
void *const bin_funcs_table[]  = {

    lib_uartSendBuff,
    lib_systemTick,
    lib_sensorRWReg,
    lib_sensorSetMode,
    lib_sensorReadLine,
    lib_sensorReadFrame,
	  lib_eflash_program,
	  lib_eflash_bulk_program,
    lib_eflash_page_erase,
    lib_spiflash_read,
    lib_spiflash_write,
	  lib_fsmDebugPrint
};


int32_t mafp_get_chipid(void)
{
	return -1;
}

int32_t mafp_post_enroll(void)
{
	return -1;
}

int32_t mafp_post_authenticate(void)
{
	return -1;
}


int32_t mafp_get_enrollid_status(uint8_t enroll_id)
{
    uint8_t  group_buf[32] = {0x00};
    int32_t res           = 0;
    uint8_t  group_id      = 0;
    uint8_t  bit_index     = 0; 
    uint8_t  group_value   = 0x00;
    uint8_t  enroll_mask   = 0x00;

	res = binlib_api(LIB_READ_ID_LIST,group_buf,32);

    if (res == 0){

        /* Get the enroll mask group and mask  bit index */
        group_id    = enroll_id>>3;
        bit_index   = enroll_id%8;
        group_value = group_buf[group_id];
        enroll_mask = (group_value>>bit_index)&0x01;

        if (enroll_mask)
            return 1;
        else
            return 0;
        
    }else{
        return 0;
    }
    


    

}






int32_t mafp_get_used_enrollids(void)
{
    uint8_t  group_buf[32] = {0x00};
    uint8_t  i             = 0;
    uint8_t  j             = 0;
    uint8_t  totals        = 0;
    int32_t  res           = 0;

	res = binlib_api(LIB_READ_ID_LIST,group_buf,32);

    if (res == 0){
        for (i = 0; i < 32; i++){
            
            if (group_buf[i] == 0){
                continue;
            }

            
            for (j = 0; j < 8; j++){
                if ((group_buf[i]>>j)&0x01){
                    totals++;
                }
            }
        }
        return totals;
    }else{
        return 0;
    }
}


int32_t mafp_remove_enrollid(uint8_t enroll_id)
{
	uint8_t buff[4] = {0x00};

    
    buff[0]=(enroll_id>>8)&0xff;
    buff[1]=(enroll_id&0xff);
    buff[2]=0;
    buff[3]=0x01;

    
    if (binlib_api(LIB_DELETE_ID,buff,4) == 0)
    {
        return 0;
    }else{
        return -1;
    }

}


int32_t mafp_clear_all_enrollids(void)
{
	uint8_t buff[2] = {0x00};
	
	buff[0] = 1;
	if (binlib_api(LIB_INIT_SENSOR, buff, 2))
		return -1;
	else 
		 return 0;

}


int32_t sensor_get_download_buf(uint8_t *name, uint8_t **dst_addr, uint32_t size)
{
	return -1;
}

int32_t sensor_save_download_buf(void)
{
	return -1;
}


int32_t sensor_enroll_image_testing(uint8_t fid)
{
	return -1;
}


int32_t sensor_authenticate_image_testing(void)
{
	return -1;
}


int32_t sensor_get_upload_image_buf(uint32_t idx, uint8_t **dst_addr,uint16_t *width, uint16_t *height)
{
	uint16_t width_x = 0;
	uint16_t height_x = 0;
	uint32_t image_length = 0;
	
	*dst_addr = (uint8_t *)get_image_data(&image_length,&width_x,&height_x);
	*width = width_x;
	*height = height_x;
  
	//image_length = (*width)*(*height);

	return image_length;
}


int32_t sensor_get_upload_template_buf(uint32_t idx, uint8_t **dst_addr)
{
	return -1;
}


int32_t mafp_detect_mode(void)
{
	return -1;
}


int32_t mafp_get_max_subtpl_num(void)
{
	return -1;
}


int32_t mafp_reset_sensor_config(void)
{
	return -1;
}




int32_t sensor_upload_subtpls(void)
{
	return -1;
}


