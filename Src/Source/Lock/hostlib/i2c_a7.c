
//#include "i2c_a7.h"
//#include "main.h"

//#define A7_I2C_DELAY  20//((5 * SystemCoreClock / 100000 - 30) / 34)  // 26

//#define A7_I2C_ADDR (0x48 << 1)

////#define A7_SDA_PIN 						M_SDA_GPIO_PIN		// IIC 11脚
////#define A7_SCL_PIN 						M_SCL_GPIO_PIN		// IIC 12脚

////#define A7_SDA_SET() 					M_SDA_FUN_ON
////#define A7_SDA_CLR() 					M_SDA_FUN_OFF	
////#define A7_SDA_GET() 					M_SDA_IN_READ()
////#define A7_SDA_DIRIN() 				M_SDA_IN_UP()
////#define A7_SDA_DIROUT() 			M_SDA_OUT_PP()

////#define A7_SCL_SET() 					M_SCL_FUN_ON
////#define A7_SCL_CLR() 					M_SCL_FUN_OFF 
////#define A7_SCL_GET() 					M_SCL_IN_READ()
////#define A7_SCL_DIRIN() 				M_SCL_IN_UP()
////#define A7_SCL_DIROUT() 			M_SCL_OUT_PP() 

//#define MJ_SDA_PIN 						19		
//#define MJ_SCL_PIN 						20		

//#define M_MJ_SDA_OUT_PP()    //         nrf_gpio_cfg_output(MJ_SDA_PIN)      		
//#define M_MJ_SDA_IN_UP()     //       nrf_gpio_cfg_input(MJ_SDA_PIN,NRF_GPIO_PIN_PULLUP)       		
//#define M_MJ_SDA_IN_FL()     //      nrf_gpio_cfg_input(MJ_SDA_PIN,NRF_GPIO_PIN_NOPULL)   
//#define M_MJ_SDA_FUN_ON      //  nrf_gpio_pin_set(MJ_SDA_PIN)					
//#define M_MJ_SDA_FUN_OFF     //   nrf_gpio_pin_clear(MJ_SDA_PIN)					       
//#define M_MJ_SDA_IN_READ()	//				nrf_gpio_pin_read(MJ_SDA_PIN)	

//#define M_MJ_SCL_OUT_PP()     //        nrf_gpio_cfg_output(MJ_SCL_PIN)          		  
//#define M_MJ_SCL_IN_UP()      //      nrf_gpio_cfg_input(MJ_SCL_PIN,NRF_GPIO_PIN_PULLUP)       
//#define M_MJ_SCL_IN_FL()     //      nrf_gpio_cfg_input(MJ_SCL_PIN,NRF_GPIO_PIN_NOPULL)       	
//#define M_MJ_SCL_FUN_ON      //  nrf_gpio_pin_set(MJ_SCL_PIN)					
//#define M_MJ_SCL_FUN_OFF      //  nrf_gpio_pin_clear(MJ_SCL_PIN)						 
//#define M_MJ_SCL_IN_READ()		//					nrf_gpio_pin_read(MJ_SCL_PIN)	


//#define A7_SDA_SET() 			//		M_MJ_SDA_FUN_ON
//#define A7_SDA_CLR() 			//		M_MJ_SDA_FUN_OFF	
//#define A7_SDA_GET() 			//		M_MJ_SDA_IN_READ()
//#define A7_SDA_DIRIN() 			//	M_MJ_SDA_IN_UP()
//#define A7_SDA_DIROUT() 		//	M_MJ_SDA_OUT_PP()

//#define A7_SCL_SET() 			//		M_MJ_SCL_FUN_ON
//#define A7_SCL_CLR() 			//		M_MJ_SCL_FUN_OFF 
//#define A7_SCL_GET() 			//		M_MJ_SCL_IN_READ()
//#define A7_SCL_DIRIN() 			//	M_MJ_SCL_IN_UP()
//#define A7_SCL_DIROUT() 		//	M_MJ_SCL_OUT_PP() 












//#ifdef TGT_A71CH
//#define I2C_SPEED 400000
//#else
//#define I2C_SPEED 400000
//#endif

//#define BUFSIZE 512

//volatile uint8_t g_bA710XWrAddr = 0;
//volatile uint8_t g_bA710XRdAddr = 0;

//// #define DBG_I2C

////__asm static void a7_i2c_delay(uint32_t cnt)
////{
////L1
////    SUBS R0, R0, #1
////    BNE L1
////    BX LR
////}

//// i2c_start()
//// i2c 起始字节
////
// uint8_t a7_i2c_start(void)
//{
//	M_MJ_SDA_OUT_PP();
//	M_MJ_SCL_OUT_PP();
//	
//  A7_SDA_SET();
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SDA_CLR();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SCL_CLR();
//  return 1;
//}

//// i2c_restart()
//// i2c repeat start
////
//#if 1
// uint8_t a7_i2c_restart(void)
//{
//	A7_SDA_SET();
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SDA_CLR();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SCL_CLR();

//  return 1;
//}
//#endif

////
//// i2c停止字节
////
// void a7_i2c_stop(void)
//{
//  A7_SDA_CLR();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//  A7_SDA_SET();
//  a7_i2c_delay(A7_I2C_DELAY / 2);
//}

////
//// i2c noACK 无应答反馈
////
// void a7_i2c_send_noack(void)
//{
//  A7_SDA_SET();
//  a7_i2c_delay(A7_I2C_DELAY);
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY);
//  A7_SCL_CLR();
//}

// void a7_i2c_send_ack(void)
//{
//  A7_SCL_CLR();
//  a7_i2c_delay(A7_I2C_DELAY);
//  A7_SDA_CLR();
//  a7_i2c_delay(A7_I2C_DELAY);
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY);
//  A7_SCL_CLR();
//  a7_i2c_delay(A7_I2C_DELAY);
//}

////
//// i2c 等待ACK应答
////
// uint8_t a7_i2c_wait_ack(void)
//{
//  uint16_t err = 1;  // 50000;

//  A7_SDA_SET();

//  A7_SDA_DIRIN();
//  A7_SCL_SET();
//  a7_i2c_delay(A7_I2C_DELAY);
//  if (A7_SDA_GET() != 0) {
//    err = 0;
//  }
//  A7_SCL_CLR();
//  A7_SDA_DIROUT();
//  a7_i2c_delay(A7_I2C_DELAY);

//  return err;
//}

////
//// i2c 写一个字节
////
// void a7_i2c_write_byte(uint8_t data)
//{
//  uint8_t i;

//  for (i = 0; i < 8; i++) {
//    if (data & 0x80) {
//      A7_SDA_SET();
//    } else {
//      A7_SDA_CLR();
//    }
//    data <<= 1;
//    a7_i2c_delay(A7_I2C_DELAY);
//    A7_SCL_SET();
//    a7_i2c_delay(A7_I2C_DELAY);
//    A7_SCL_CLR();
//  }
//  a7_i2c_delay(A7_I2C_DELAY);
//}

////
//// i2c 读一个字节
////
// uint8_t a7_i2c_read_byte(void)
//{
//  uint8_t i;
//  uint8_t bReadValue = 0;
//  uint8_t bit;

//  A7_SDA_SET();
//  A7_SDA_DIRIN();
//  a7_i2c_delay(A7_I2C_DELAY);
//  i = 0;
//  for (i = 0; i < 8; i++) {
//    A7_SCL_SET();
//    a7_i2c_delay(A7_I2C_DELAY);
//    if (A7_SDA_GET())
//      bit = 0x01;
//    else
//      bit = 0x00;
//    bReadValue = bReadValue << 1;
//    bReadValue = bReadValue | bit;
//    A7_SCL_CLR();
//    a7_i2c_delay(A7_I2C_DELAY);
//  }
//  //    A7_SDA_SET();
//  A7_SDA_DIROUT();
//  return bReadValue;
//}

//i2c_error_t axI2CWrite(uint8_t bus, uint8_t addr, uint8_t* pTx,
//                       uint16_t txLen)
//{
//  uint32_t i;

//  a7_i2c_start();
//  a7_i2c_write_byte(addr);
//  if (a7_i2c_wait_ack() == 0) {
//    a7_i2c_stop();
//    return I2C_NACK_ON_ADDRESS;
//  }

//  for (i = 0; i < txLen; i++) {
//    a7_i2c_write_byte(*pTx);
//    if (a7_i2c_wait_ack() == 0) {
//      // sm_printf(" --- axI2CWrite 2 Addr 0x%x FAILED, Cnt %d\r\n", addr, i);
//      a7_i2c_stop();
//      return I2C_NACK_ON_DATA;
//    }
//    pTx++;
//  }
//  a7_i2c_stop();

//  return I2C_OK;
//}

//i2c_error_t axI2CWriteRead(uint8_t bus, uint8_t addr, uint8_t* pTx,
//                           uint16_t txLen, uint8_t* pRx, uint16_t* pRxLen)
//{
//  uint16_t i, j;

//  a7_i2c_start();
//  a7_i2c_write_byte(addr);
//  if (a7_i2c_wait_ack() == 0) {
//    a7_i2c_stop();
//		 printf("*************************************************ID2 I2C_NACK_ON_ADDRESS \n");
//    return I2C_NACK_ON_ADDRESS;
//  }

//  for (i = 0; i < txLen; i++) {
//    a7_i2c_write_byte(*pTx);
//    if (a7_i2c_wait_ack() == 0) {
//      a7_i2c_stop();
//      return I2C_NACK_ON_DATA;
//    }
//    pTx++;
//  }

//  a7_i2c_delay(A7_I2C_DELAY * 100);
//  // --------------------------------------------------------------------------------------
//  // restart
//  a7_i2c_start();
//  a7_i2c_write_byte(addr | 0x01);
//  if (a7_i2c_wait_ack() == 0) {
//    a7_i2c_stop();
//    return I2C_NACK_ON_ADDRESS;
//  }

//  // why delay here
//  /* a7_i2c_delay(A7_I2C_DELAY * 100); */

//  j = a7_i2c_read_byte();
//  a7_i2c_send_ack();
//  *pRx = j;
//  pRx++;

//  for (i = 0; i < j; i++) {
//    *pRx = a7_i2c_read_byte();
//    if (i == j - 1) {
//      a7_i2c_send_noack();
//    } else {
//      a7_i2c_send_ack();
//    }
//    pRx++;
//  }

//  a7_i2c_stop();
//  *pRxLen = j + 1;
//  printf("*************************************************ID2 I2C_OK \n");
//  return I2C_OK;
//}

//void ID2_ADDR(void)
//{
//	M_A71_OUT_PP();
//    M_A71_FUN_ON;
//	uint8 cn=0;
//		
//    while(cn<5)
//	{
//		a7_i2c_start();
//		a7_i2c_write_byte(0xE2);
//		if(a7_i2c_wait_ack())
//		{
//				ID2_FLAG = 1;
//				break;
//		}
//		else
//		{
//			cn++;
//			a7_i2c_stop();
//			nrf_delay_ms (5);
//			ID2_FLAG = 0;
//		}

//  }	
//}

//void a7_i2c_Reset(void)
//{
//    M_MJ_SDA_IN_UP();
//    if (M_MJ_SDA_IN_READ() == 0)
//    {
//        a7_i2c_restart();
//        a7_i2c_write_byte(0xff);
//        a7_i2c_send_noack();
//        a7_i2c_stop();
//    }
//}

