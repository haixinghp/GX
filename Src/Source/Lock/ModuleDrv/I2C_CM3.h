#ifndef _I2C_H_
#define _I2C_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include "TypeDef.h"

#define I2C_FALSE			0
#define I2C_TRUE			1
extern	void I2C_Delay(uint8 i);
extern  uint8 I2C_Start_I2c(void);
extern  void I2C_Stop_I2c(void);
extern  void I2C_SendByte(uint8 c);
extern  void I2C_Ack_I2c(void);
extern  uint8 I2C_RcvByte(void);
extern  void I2C_Sleep (void);
extern  uint8 I2C_WaitAck_I2c(void);
extern  void I2C_NoAck_I2c(void);
extern  void I2C_ResetI2c (void);

#ifdef __cplusplus
}
#endif
#endif
//.end of the file.
