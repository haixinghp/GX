#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

#define SENSOR_SS_ENABLE (1)
#define SENSOR_SS_DISABLE (0)

extern void sensorSpiInit(void);
extern void sensorSpiWriteRead(const void* cpWriteBuf, int nWriteSize, void* pReadBuf, int nReadSize);
extern int32_t sensor_reset_mode(void);
extern int32_t sensor_fnc_get_chipid(uint8_t *id);


#endif /* SENSOR_H_ */
