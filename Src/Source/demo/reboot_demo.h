// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : reboot_demo.h
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef __REBOOT_DEMO_H__
#define __REBOOT_DEMO_H__

#include "i2c_drv.h"

extern void Reboot_Demo(void);
extern void I2C_Config_Gpio_Reboot(I2C_PIN I2C_PINx, UINT8 GPIO_Dir);
extern UINT8 I2C_Read_Gpio_Data_Reboot(I2C_PIN I2C_PINx);

#endif /* __EFLASH_DEMO_H__ */

