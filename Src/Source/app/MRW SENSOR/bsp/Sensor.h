/*
 * Sensor.h
 *
 *  Created on: 2016Äê9ÔÂ18ÈÕ
 *      
 */

#ifndef SENSOR_H_
#define SENSOR_H_
#include "lib/CCM_MEM.h"

extern U8 Spi_Mode;

#define SPI_SWITCH_ON 0


#define SPI_SENSOR_MODE		1
#define SPI_FLASH_MODE		2


#define SENSOR_POWERDOWN_MODE     0x80
#define SENSOR_DETECT_MODE        0x84
#define SENSOR_CAPTURE_MODE       0x88
#define SENSOR_RESET_MODE         0x8C


void sensorInit(void);
void sensorSetMode(U8);
void sensorRWReg(U8 addr, U8 w_data, U8 *old_data, U8 *new_data);
void sensorReadLine(U8 *buff, U32 len);
void sensorReadFrame(U8 *buff, U32 len);
void sensorSetLine(U8 *buff, U32 len);

#endif /* SENSOR_H_ */
