/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef __SE_DRIVER_H__
#define __SE_DRIVER_H__

#include <stdint.h>
#include "irot_hal.h"

#ifdef __cplusplus
extern "C"
#endif
/******************SE generic define******************************/
#define SE_IF_7816		0
#define SE_IF_I2C		1
#define SE_IF_SPI		2

//interface define SW Error Code
#define IF_ERR_NULL_POINT		0xFFE1
#define IF_ERR_RECV_ACK			0xFFE2
#define IF_ERR_LENGTH			0xFFE3
#define IF_ERR_LRC				0xFFE4
#define IF_ERR_OTHER			0xFFE5
#define SE_ERR_CRC				0xFFE6
#define SE_ERR_INS				0xFFE7

//config MCU Polling SE interval
#define POLL_INTERVAL			0

//config MCU Polling SE time(uint:ms)
//8s
#define POLL_TIMEOUT			8000

//define SE interface type
#define SE_TYPE_I2C

//define debug 
#define SE_DEBUG
#define DEBUG_I2C

/******************I2C define******************************/
#define I2C_NAD					0x00
#define I2C_CMD_IBLOCK			0x02
#define I2C_CMD_GET_ATR			0x30
#define I2C_CMD_NAK				0xBA

#define I2C_MIN_LEN				3
#define I2C_MAX_LEN				1024

#define SE_ADDR					0xE2

#define I2C_PWR_ON_DELAY		10
#define I2C_PWR_OFF_DELAY		10

//check I2C reg timeout
#define CHECK_TIMES_I2C			0xFFFF

#define BYTE_DELAY_I2C			1
#define	FRAME_DELAY_I2C			5

//user config use i2c fast mode
#define I2C_CLK_400K

typedef struct{
	uint8_t lenlo;
	uint8_t lenhi;
	uint8_t nad;
	union{
		uint8_t cmd;
		uint8_t sta;
	}flag;
} FM_I2C_HEAD;

typedef struct{
	uint8_t se_name;
	void (*fm_driver_register)(void *user_drv);
	void (*fm_device_init)(void);
	void (*fm_open_device)(void);
	void (*fm_close_device)(void);
	uint8_t (*fm_dev_power_on)(uint8_t *rbuf, uint16_t *rlen);
	uint8_t (*fm_apdu_transceive)(uint8_t *sbuf, uint16_t slen, uint8_t *rbuf, uint16_t *rlen, 
									uint16_t poll_inv, uint32_t poll_timeout);
	void (*fm_driver_unregister)(void);
} StSeFunc;

typedef struct{
	uint8_t se_i2c_addr;
	void (*fm_i2c_power_on)(void);
	void (*fm_i2c_power_off)(void);
	void (*fm_i2c_init)(void);
	void (*fm_i2c_start)(void);
	void (*fm_i2c_stop)(void);
	uint8_t (*fm_i2c_send_char)(uint8_t ch);
	uint8_t (*fm_i2c_recv_char)(void);
	uint8_t (*fm_i2c_send_addr)(uint8_t ch);
} StSeI2CDriver;

/**
 * @brief open session and connect to SE.
 *
 * @param handle
 *
 * @return @see error code definitions.
 */
irot_result_t se_open_session(void **handle);

/**
 * @brief transmit APDU to SE.
 *
 * @param handle
 * @param cmd_apdu Command APDU(ISO7816-4).
 * @param cmd_len  Command APDU length
 * @param rsp_buf  response APDU buffer.
 * @param rsp_len  input with response APDU buffer length, output with real response APDU length (SW in last two bytes).
 *
 * @return @see error code definitions.
 */
//irot_result_t se_transmit(void *handle, const uint8_t *cmd_apdu, uint32_t cmd_len, uint8_t *rsp_buf, uint32_t *rsp_len);

/**
 * @brief close session and disconnect to SE.
 *
 * @param handle
 *
 * @return @see error code definitions.
 */
irot_result_t se_close_session(void *handle);

#ifdef __cplusplus
}
#endif

#endif  /* __SE_DRIVER_H__ */

