/*
 * Copyright (C) 2018 Alibaba Group Holding Limited
 */


#include <string.h>
#include "se_driver.h"
#include "main.h"
#include "timer.h"    

#define MJ_SDA_PIN 						19		
#define MJ_SCL_PIN 						20		
#define MJ_POW                          17

#define M_MJ_SDA_OUT_PP()             nrf_gpio_cfg_output(MJ_SDA_PIN)      		
#define M_MJ_SDA_IN_UP()            nrf_gpio_cfg_input(MJ_SDA_PIN,NRF_GPIO_PIN_PULLUP)       		
#define M_MJ_SDA_IN_FL()           nrf_gpio_cfg_input(MJ_SDA_PIN,NRF_GPIO_PIN_NOPULL)   
#define M_MJ_SDA_FUN_ON        nrf_gpio_pin_set(MJ_SDA_PIN)					
#define M_MJ_SDA_FUN_OFF        nrf_gpio_pin_clear(MJ_SDA_PIN)					       
#define M_MJ_SDA_IN_READ()					nrf_gpio_pin_read(MJ_SDA_PIN)	

#define M_MJ_SCL_OUT_PP()             nrf_gpio_cfg_output(MJ_SCL_PIN)          		  
#define M_MJ_SCL_IN_UP()            nrf_gpio_cfg_input(MJ_SCL_PIN,NRF_GPIO_PIN_PULLUP)       
#define M_MJ_SCL_IN_FL()           nrf_gpio_cfg_input(MJ_SCL_PIN,NRF_GPIO_PIN_NOPULL)       	
#define M_MJ_SCL_FUN_ON        nrf_gpio_pin_set(MJ_SCL_PIN)					
#define M_MJ_SCL_FUN_OFF        nrf_gpio_pin_clear(MJ_SCL_PIN)						 
#define M_MJ_SCL_IN_READ()							nrf_gpio_pin_read(MJ_SCL_PIN)	


#define A7_SDA_SET() 					M_MJ_SDA_FUN_ON
#define A7_SDA_CLR() 					M_MJ_SDA_FUN_OFF	
#define A7_SDA_GET() 					M_MJ_SDA_IN_READ()
#define A7_SDA_DIRIN() 				M_MJ_SDA_IN_UP()
#define A7_SDA_DIROUT() 			M_MJ_SDA_OUT_PP()

#define F_SCL_SET() 					M_MJ_SCL_FUN_ON
#define A7_SCL_CLR() 					M_MJ_SCL_FUN_OFF 
#define A7_SCL_GET() 					M_MJ_SCL_IN_READ()
#define A7_SCL_DIRIN() 				M_MJ_SCL_IN_UP()
#define A7_SCL_DIROUT() 			M_MJ_SCL_OUT_PP() 

/***********************I2C inf******************************/
static uint8_t gfm_I2CAddr;
static StSeFunc *pgfm_SeFunc = NULL;
static StSeI2CDriver *pgfm_I2CDrv = NULL;



void user_i2c_dev_power_on(void)
{
    #if 0
	M_A71_OUT_PP();
	M_A71_FUN_ON;
	delayms(I2C_PWR_ON_DELAY);
    #endif
}

void user_i2c_dev_power_off(void)
{
    #if 0
	M_A71_OUT_PP();
	M_A71_FUN_OFF;
	delayms(I2C_PWR_OFF_DELAY);
    #endif
}

void user_i2c_init(void)
{
    #if 0
	a7_i2c_Reset();
	delayms(200);
    #endif
}

void user_i2c_start(void)
{
//	
//	IIC_EN=1;
//   a7_i2c_start();
}

void user_i2c_stop(void)
{
//	a7_i2c_stop();
//	IIC_EN=0;
}

uint8_t user_i2c_send_char(uint8_t ch)
{
//	uint32_t count;

//	count = CHECK_TIMES_I2C;

//	a7_i2c_write_byte(ch);
//	uint8_t ret=a7_i2c_wait_ack();
//	delayus(BYTE_DELAY_I2C);

//	if(ret)
//		return 0;

//	else 
//		return 12;	//timeout err
    
    return 0;
}

uint8_t user_i2c_recv_char(void)
{
//		uint32_t count;

//	count = CHECK_TIMES_I2C;
//	
////	count = CHECK_TIMES_I2C;
////	do{
////		count--;
////		if((I2C2->SR1&0x40)&&(I2C2->SR2&3))
////		{
////			break;
////		}
////		
////	}while(count);
//	uint8_t rcv_byte= a7_i2c_read_byte();
//	a7_i2c_send_ack();
////	I2C_NoAck_I2c();
//	delayus(BYTE_DELAY_I2C);
//        
//    return rcv_byte;
    return 0;
}

uint8_t user_i2c_send_addr(uint8_t ch)
{
//	uint32_t count;

//	
//	count = CHECK_TIMES_I2C;

//	a7_i2c_write_byte(ch);
//	uint8_t ret=a7_i2c_wait_ack();
//	delayus(BYTE_DELAY_I2C);

//	if(ret){
//		return 0;
//	}
//	else {
//		return 12;	//timeout err
//	}
    return 0;
}

StSeI2CDriver gusr_i2c_drv = 
{
	SE_ADDR,
	user_i2c_dev_power_on,
	user_i2c_dev_power_off,
	user_i2c_init,
	user_i2c_start,
	user_i2c_stop,
	user_i2c_send_char,
	user_i2c_recv_char,
	user_i2c_send_addr
};

void fm_i2c_drv_reg(StSeI2CDriver *fm_i2c_drv)
{
	pgfm_I2CDrv = fm_i2c_drv;
}

void fm_i2c_drv_unreg(void)
{
	pgfm_I2CDrv = NULL;
}

void fm_i2c_dev_init(void)
{
	if(pgfm_I2CDrv)
		pgfm_I2CDrv->fm_i2c_init();
}

void fm_i2c_open_device(void)
{
	if(pgfm_I2CDrv)
	{
		gfm_I2CAddr = pgfm_I2CDrv->se_i2c_addr;
		pgfm_I2CDrv->fm_i2c_power_on();
	}
}

void fm_i2c_close_device(void)
{
	if(pgfm_I2CDrv)
	{
		gfm_I2CAddr = 0;
		pgfm_I2CDrv->fm_i2c_power_off();
	}
}

uint8_t fm_i2c_send_frame(uint8_t cmd, uint8_t *sbuf, uint16_t slen)
{
	FM_I2C_HEAD fm_i2c_hd;
	uint16_t i;
	uint8_t bcc;
	uint8_t ret;
	
	fm_i2c_hd.lenlo = slen+3;
	fm_i2c_hd.lenhi = (slen+3)>>8;
	fm_i2c_hd.nad = 0;
	fm_i2c_hd.flag.cmd = cmd;

	if(!pgfm_I2CDrv)
		return 11;
	
	pgfm_I2CDrv->fm_i2c_start();

	//send se addr,write slaver
	ret = pgfm_I2CDrv->fm_i2c_send_addr(gfm_I2CAddr);
	if(ret) goto END;

	//send pack head
	for(i=0; i<4; i++)
	{
		ret = pgfm_I2CDrv->fm_i2c_send_char(*(&fm_i2c_hd.lenlo+i));
		if(ret) goto END;
	}

	//calc bcc
	bcc = fm_i2c_hd.lenlo ^ fm_i2c_hd.lenhi ^ fm_i2c_hd.nad ^ fm_i2c_hd.flag.cmd; 

	//send data
	for(i=0; i<slen; i++)
	{
		ret = pgfm_I2CDrv->fm_i2c_send_char(*(sbuf+i));
		bcc ^= sbuf[i];
		if(ret) goto END;
	}

	//send bcc
	ret = pgfm_I2CDrv->fm_i2c_send_char(bcc);

	pgfm_I2CDrv->fm_i2c_stop();

	//debug
	#ifdef DEBUG_I2C
	//printf("HEAD:%02x,%02x,LRC:%02x\r\n",fm_i2c_hd.lenhi,fm_i2c_hd.lenlo,bcc);
	#endif

//	delayms(FRAME_DELAY_I2C);

END:
 	return ret;
}

uint8_t fm_i2c_recv_frame(uint8_t *rbuf, uint16_t *rlen)
{
	uint16_t i;
	uint16_t recvLen;
	uint8_t bcc;
	uint8_t ret;
	FM_I2C_HEAD fm_i2c_hd;

	*rlen = 0;
	
	if(!pgfm_I2CDrv)
		return 11;
	
	pgfm_I2CDrv->fm_i2c_start();
	
	//send se addr,read slaver
	ret = pgfm_I2CDrv->fm_i2c_send_addr(gfm_I2CAddr+1);
	if(ret)
	{
		return ret;
	}

	//recv length
	fm_i2c_hd.lenlo = pgfm_I2CDrv->fm_i2c_recv_char();
	fm_i2c_hd.lenhi = pgfm_I2CDrv->fm_i2c_recv_char();

	recvLen  = (fm_i2c_hd.lenhi << 8) + fm_i2c_hd.lenlo;
	
	if(recvLen < I2C_MIN_LEN || recvLen > I2C_MAX_LEN)
	{
		//printf("err_recvLen=%04x\r\n",recvLen);
		*rlen = 0;
		return 13;
	}

	//recv nad and sta
	fm_i2c_hd.nad = pgfm_I2CDrv->fm_i2c_recv_char();
	fm_i2c_hd.flag.sta = pgfm_I2CDrv->fm_i2c_recv_char();

	bcc = fm_i2c_hd.lenlo ^ fm_i2c_hd.lenhi ^ fm_i2c_hd.nad ^ fm_i2c_hd.flag.sta;

	//recv data length
	*rlen = recvLen - 3;
	
	//recv data
	for(i=0; i<*rlen; i++)
	{
		rbuf[i] = pgfm_I2CDrv->fm_i2c_recv_char();
		bcc ^= rbuf[i];
	}
	

//	//recv and check bcc
////	bcc ^= pgfm_I2CDrv->fm_i2c_recv_char();
//	bcc ^=a7_i2c_read_byte();
//	a7_i2c_send_noack();
		//stop
	pgfm_I2CDrv->fm_i2c_stop();
	
	if(bcc)
	{
		*rlen = 0;
		return 14;
	}

//	delayms(FRAME_DELAY_I2C);
	
	return fm_i2c_hd.flag.sta;
}
#if 0
uint8_t fm_i2c_get_atr(uint8_t *rbuf, uint16_t *rlen)
{
	uint8_t ret;

	ret = fm_i2c_send_frame(I2C_CMD_GET_ATR, 0, 0);
	if(ret) return ret;

	init_timeout_ms(POLL_TIMEOUT);
	do{
		ret = fm_i2c_recv_frame(rbuf, rlen);
		if(!ret) break;
	}while(!check_timeout_ms());

	return ret;	
}
#endif
#define DEBUG_I2C_SEND
uint8_t fm_i2c_transceive(uint8_t *sbuf, uint16_t slen, uint8_t *rbuf, uint16_t *rlen, 
						uint16_t poll_inv, uint32_t poll_timeout)
{
//	uint8_t ret;
//	uint16_t i;

//	//debug
//	#ifdef DEBUG_I2C_SEND
////	for(i=0; i<slen; i++)
////		printf("%02x,",sbuf[i]);
////	printf("\r\n");
//	#endif

//	*rlen = 0;
//	ret = fm_i2c_send_frame(I2C_CMD_IBLOCK, sbuf, slen);
//	#ifdef DEBUG_I2C
//	printf("fm_i2c_send_frame_ret=%02x\r\n",ret);
//	#endif
//	if(ret) return ret;

//	init_timeout_ms(poll_timeout);
//	do{
//		ret = fm_i2c_recv_frame(rbuf, rlen);

//		if(ret == 12){	//timeout
////			delayms(poll_inv);
//		}else if(ret==13||ret==14){	//bcc err
//			fm_i2c_send_frame(I2C_CMD_NAK, 0, 0);
//			init_timeout_ms(poll_timeout);
//		}else if(ret == 0xF2){	//WTX
//			init_timeout_ms(poll_timeout);
//		}else
//			break;
//	}while(!check_timeout_ms());
//	
//	#ifdef DEBUG_I2C
//	/*for(i=0; i<*rlen; i++)
//		printf("%02x,",rbuf[i]);
//	printf("\r\n");*/
//	#endif
//	
//	return ret;
}

StSeFunc gfm_se_i2c = 
{
//	SE_IF_I2C,
//	fm_i2c_drv_reg,
//	fm_i2c_dev_init,
//	fm_i2c_open_device,
//	fm_i2c_close_device,
//	fm_i2c_get_atr,
//	fm_i2c_transceive,
//	fm_i2c_drv_unreg
};
/*************************I2C inf end**************************************/

StSeFunc *fm_se_register(StSeFunc *fm_se)
{
	StSeFunc *pfm_SeFunc = NULL;
	
	pfm_SeFunc = fm_se;
	if(pfm_SeFunc)
	{
		if(pfm_SeFunc->se_name == SE_IF_I2C){
			pfm_SeFunc->fm_driver_register(&gusr_i2c_drv);
			
		}
//		else if(pfm_SeFunc->se_name == SE_IF_SPI){
//			pfm_SeFunc->fm_driver_register(&gusr_spi_drv);
//			
//		}
	}

	return pfm_SeFunc;
}

irot_result_t se_open_session(void** handle)
{
//	uint8_t result = 0;
//	uint8_t rbuf[32];
//	uint16_t rlen = 0;
//	uint8_t i;

//	#ifdef SE_TYPE_I2C
//	pgfm_SeFunc = fm_se_register(&gfm_se_i2c);
//	#else
//	pgfm_SeFunc = fm_se_register(&gfm_se_spi);
//	#endif
//	
//	if(pgfm_SeFunc == NULL) return IROT_ERROR_GENERIC;

//	*handle = pgfm_SeFunc;
//	
//	pgfm_SeFunc->fm_open_device();

//	pgfm_SeFunc->fm_device_init();

//	result = pgfm_SeFunc->fm_dev_power_on(rbuf, &rlen);

//	#ifdef SE_DEBUG
//	printf("PowerOn:ret=%x,rlen=%d\r\n",result,rlen);
//	for(i=0;i<rlen;i++)
//	{
//		printf("%02x,",rbuf[i]);
//	}
//	printf("\r\n");
//	#endif

//	if(result) return IROT_ERROR_TIMEOUT;
	
    return IROT_SUCCESS;
}

//irot_result_t se_transmit(void* handle, const uint8_t* cmd_apdu, const uint32_t cmd_len, uint8_t* rsp_buf, uint32_t* rsp_len)
//{
//	uint8_t result = 0;
//	StSeFunc *pfm_SeFunc = handle;
//	uint16_t i;
//	
//    memset(rsp_buf, 0x00, *rsp_len);

//	if(handle == NULL) return IROT_ERROR_GENERIC;

//	#ifdef SE_DEBUG
//	printf("Send:slen=%d\r\n",cmd_len);
//	for(i=0;i<cmd_len;i++)
//	{
//		printf("%02x,",cmd_apdu[i]);
//	}
//	printf("\r\n");
//	#endif
//	
//	result = pfm_SeFunc->fm_apdu_transceive((uint8_t*)cmd_apdu, cmd_len, rsp_buf, (uint16_t *)rsp_len, POLL_INTERVAL, POLL_TIMEOUT);

//	#ifdef SE_DEBUG
//	printf("Recv:ret=%x,rlen=%d\r\n",result,*rsp_len);
//	for(i=0;i<*rsp_len;i++)
//	{
//		printf("%02x,",rsp_buf[i]);
//	}
//	printf("\r\n");
//	#endif

//	if(result) return IROT_ERROR_TIMEOUT;
//		
//    return IROT_SUCCESS;
//}

irot_result_t se_close_session(void* handle)
{
//	StSeFunc *pfm_SeFunc = handle;
//	
//	if(pfm_SeFunc == NULL) return IROT_ERROR_GENERIC;
//	
//	pfm_SeFunc->fm_close_device();

//	handle = NULL;

//	#ifdef SE_DEBUG
//	printf("PowerOff!\r\n");
//	#endif
//	
   return IROT_SUCCESS;
}

