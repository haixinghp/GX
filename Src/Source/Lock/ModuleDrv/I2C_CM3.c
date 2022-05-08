#include "I2C_CM3.h"	
//#include "main.h"
void I2C_SDA_Dir (uint8 dir);
void I2C_SCL_Dir (uint8 dir);

//#define SDA_OUT_0	                M_SDA_FUN_OFF
//#define SDA_OUT_1	                M_SDA_FUN_ON
//#define SCL_OUT_0	                M_SCL_FUN_OFF
//#define SCL_OUT_1	                M_SCL_FUN_ON
//#define READ_SDA_IN	                M_SDA_IN_READ()
//#define READ_SCL_IN	                M_SCL_IN_READ()

//#define SET_SDA_IN	                I2C_SDA_Dir (1)
//#define SET_SDA_OUT	                I2C_SDA_Dir (0)
//#define SET_SCL_IN	                I2C_SCL_Dir (1)
//#define SET_SCL_OUT	                I2C_SCL_Dir (0)

////20190909 ����ĸ���
#define SDA_OUT_0	                M_SDA_OUT_PP();M_SDA_FUN_OFF
#define SDA_OUT_1	                M_SDA_IN_FL()
#define SCL_OUT_0	                M_SCL_OUT_PP();M_SCL_FUN_OFF
#define SCL_OUT_1	                M_SCL_IN_FL()
#define READ_SDA_IN	                M_SDA_IN_READ()
#define READ_SCL_IN	                M_SCL_IN_READ()

#define SET_SDA_IN	                I2C_SDA_Dir (1)
#define SET_SDA_OUT	                I2C_SDA_Dir (0)
#define SET_SCL_IN	                I2C_SCL_Dir (1)
#define SET_SCL_OUT	                I2C_SCL_Dir (0)

void I2C_ResetI2c (void);

/*****************************


*****************************/
void I2C_Sleep (void)
{
    /* Configure the GPIO pin */
    M_SCL_IN_FL();

    /* Enable the GPIO Clock */
    M_SDA_IN_FL();

}
/*****************************


*****************************/
void I2C_SCL_Dir (uint8 dir)
{
    if ( dir == 0 )
    {
        M_SCL_OUT_PP();
    }
    else
    {
        M_SCL_IN_FL();

    }
}
/*****************************


*****************************/
void I2C_SDA_Dir (uint8 dir)
{
    if ( dir == 0 )
    {
        M_SDA_OUT_PP();
    }
    else
    {
        M_SDA_IN_FL();

    }
}
/*****************************


*****************************/
void I2C_Delay(uint8 i)
{
//    uint8 j;

    for(; i > 0; i--)
    {
			__nop();
			__nop();
			//20190909
			__nop();
			__nop();
			__nop();
			__nop();
			__nop();
			__nop();

//        for(j = 27; j > 0; j--)
//        {
//            ;
//        }
    }
}
/*****************************


*****************************/
uint8 I2C_RStart_I2c(void)
{
    SET_SCL_OUT;
    SET_SDA_OUT;
    SDA_OUT_1;
    SCL_OUT_1;
    I2C_Delay(10);
   // if( !READ_SDA_IN ) return I2C_FALSE; //BUSY
    SDA_OUT_0;
    I2C_Delay(10);
  //  if( READ_SDA_IN ) return I2C_FALSE; //ERROR
    SDA_OUT_0;
    I2C_Delay(10);

    return I2C_TRUE;
}
/*****************************


*****************************/
uint8 I2C_Start_I2c(void)
{
	  IIC_EN=1;
    I2C_ResetI2c();

    SET_SCL_OUT;
    SET_SDA_OUT;

    SDA_OUT_1;
    SCL_OUT_1;
		
    I2C_Delay(10);
  //  if( !READ_SDA_IN ) return I2C_FALSE; //BUSY
    SDA_OUT_0;
    I2C_Delay(10);
  //  if( READ_SDA_IN ) return I2C_FALSE; //ERROR
    SDA_OUT_0;
    I2C_Delay(10);
    return I2C_TRUE;
}
/*****************************


*****************************/
void I2C_Stop_I2c(void)
{
    SCL_OUT_0;
    I2C_Delay(10);
    SDA_OUT_0;
    I2C_Delay(10);
    SCL_OUT_1;
    I2C_Delay(10);
    SDA_OUT_1;
    I2C_Delay(10);
	  IIC_EN=0;
}
/*****************************
                 �ֽ����ݴ��ͺ���
����ԭ��: I2C_SendByte(uint8 c)
����:  ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
     ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)
     ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�

*****************************/
void I2C_SendByte(uint8 c)
{
    uint8 i = 8;

    while(i--)
    {
        SCL_OUT_0;
        I2C_Delay(10);
        if( c & 0x80)
            SDA_OUT_1;
        else
            SDA_OUT_0;
        c <<= 1;
        I2C_Delay(10);
        SCL_OUT_1;
        I2C_Delay(10);
    }
    SCL_OUT_0;
}
/*****************************
                 �ֽ����ݴ��ͺ���
����ԭ��: uint8 I2C_RcvByte()
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
     ���������Ӧ������

*****************************/
uint8 I2C_RcvByte(void)
{
    uint8 i = 8;
    uint8 retc = 0;

    SET_SDA_IN;
    SDA_OUT_1;
    while(i--)
    {
        retc <<= 1;
        SCL_OUT_0;
        I2C_Delay(10);
        SCL_OUT_1;
        I2C_Delay(10);
        if( READ_SDA_IN )
        {
            retc |= 0x01;
        }
    }
    SCL_OUT_0;
    SET_SDA_OUT;
    return retc;
}
/*****************************


*****************************/
void I2C_Ack_I2c(void)
{
    SCL_OUT_0;
    I2C_Delay(10);
    SDA_OUT_0;
    I2C_Delay(10);
    SCL_OUT_1;
    I2C_Delay(10);
    SCL_OUT_0;
    I2C_Delay(10);
}
/*****************************


*****************************/
void I2C_NoAck_I2c(void)
{
    SCL_OUT_0;
    I2C_Delay(10);
    SDA_OUT_1;
    I2C_Delay(10);
    SCL_OUT_1;
    I2C_Delay(10);
    SCL_OUT_0;
    I2C_Delay(10);
}
/*****************************


*****************************/
uint8 I2C_WaitAck_I2c(void)
{
    SCL_OUT_0;
    I2C_Delay(10);
    SDA_OUT_1;
    SET_SDA_IN;
    I2C_Delay(10);
    SCL_OUT_1;
    I2C_Delay(10);  
    if( READ_SDA_IN )
    {
        SCL_OUT_0;
        SET_SDA_OUT;
        return I2C_FALSE;
    }
    else
    {
        SCL_OUT_0;
       // SET_SDA_OUT;
			  I2C_Delay(10); 
        return I2C_TRUE;
    }
}
/*****************************


*****************************/
void I2C_ResetI2c (void)
{
	  M_SDA_IN_UP();
    if( READ_SDA_IN == 0)
    {
        I2C_RStart_I2c();
        I2C_SendByte(0xff);
        I2C_NoAck_I2c();
        I2C_Stop_I2c();
    }
}

//.end of the file.
