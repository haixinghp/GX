#include "memmap.h"

//des
#define DAEDINRH        (*(volatile unsigned long *)(DES_BASE_ADDR))//(0x00c90000))
#define DAEDINRL        (*(volatile unsigned long *)(DES_BASE_ADDR + 0x04))//(0x00c90004))
#define DAEKIN1RH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x08))//(0x00c90008))
#define DAEKIN1RL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x0c))//(0x00c9000c))
#define DAEKIN2RH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x10))//(0x00c90010))
#define DAEKIN2RL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x14))//(0x00c90014))
#define DAEKIN3RH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x18))//(0x00c90018))
#define DAEKIN3RL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x1c))//(0x00c9001c))
#define DAEIVINRH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x20))//(0x00c90020))
#define DAEIVINRL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x24))//(0x00c90024))
#define DAEDOUTRH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x28))//(0x00c90028))
#define DAEDOUTRL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x2c))//(0x00c9002C))
#define DAECSR          (*(volatile unsigned char *)(DES_BASE_ADDR + 0x30))//(0x00c90033))
#define DESSEEDH        (*(volatile unsigned long *)(DES_BASE_ADDR + 0x34))
#define DESSEEDL        (*(volatile unsigned long *)(DES_BASE_ADDR + 0x38))
#define DAEKIN4RH       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x3c))
#define DAEKIN4RL       (*(volatile unsigned long *)(DES_BASE_ADDR + 0x40))

#define DES_DISTURB			0x10 	//3302
#define DES_VDES_EN			0x80
#define DES_FR				0x10	//3273
#define DES_KS_64			0x00
#define DES_KS_TRI128		0x40
#define DES_KS_TRI192		0x60
#define DES_OP_ECB			0x00
#define DES_OP_CBC			0x08
#define DES_ENCRYPT			0x04
#define DES_DECRYPT			0x00
#define DES_ENABLE			0x02
#define DES_BUSY			0x01
