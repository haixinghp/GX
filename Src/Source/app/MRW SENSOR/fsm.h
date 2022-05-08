/*********************************************************************************************************
 ** file:	    fsm.h
 ** description:	ͷ�ļ�
 ** author:      li
 ** date :       2017/05/16
 ***********************************************************************************************************/

#ifndef __FSM_H__
#define __FSM_H__
#include "lib/CCM_Types.h"



//֡����   ֡ͷ(2 Byte)  + ģ���ַ(0xffffffff) +  ����ʶ(1Byte) + ������(2Bytes) + data(n Byte) + У��(1Byte)
#define FRAME_START     0xEF01
#define FRAME_PID       0x06        //��ʶλ
#define FRAME_STA_LEN   9           //


#define FRAME_LEN_HIGH  0x07
#define FRAME_LEN_LOW   0x08
#define FRAME_CRC 1


//��ʶ
#define PID_COMMAND    0x01   //�����
#define PID_DATA       0x02   //���ݰ�
#define PID_LAST_DATA  0x08   //���һ�����ݰ�
#define PID_ACK        0x07   //Ӧ���
#define PID_DATA_ACK   0x09   //����Ӧ���

#define PID_DEBUG_PRINT 0x0a

#define FRAME_MOST_LEN 256




enum
{
	ENROLL =0x01 ,                    //ע��ָ��
	SERACH=0x04,                      //ƥ��ָ��
	UPLOAD_IMAGE =0x0a,               //�ϴ�ͼ��
	DOWN_IMAGE=0x0b,                  //����ͼ��
	Delete_ID=0x0c,                   //ɾ��ָ��id
	EMPTY_ID=0x0d,                    //���ָ�ƿ�
	SET_SYS_PARA=0x0e,                //����ϵͳ����
	GET_SYS_PARA=0x0f,                //��ȡϵͳ����
	SET_PWD=0x12,                     //��������
	VFY_PWD=0x13,                     //��ȡ����
	GET_RANDOM=0x14,                  //��ȡ���������
	SET_ADDR=0x15,                    //���õ�ַ
	Delete_MULTI_ID=0x1c,             //ɾ�����ָ��id
	GET_ID_NUM=0x1d,                  //��ȡָ������
	GET_ID_LIST=0x1f,                 //��ȡָ������
	INIT_SYSTEM=0x21,                 //�ָ���������
	DEVICE_READY=0x22,                //device ��ʼ�����
	HAND_SHAKE=0x23,                  //����
	INIT_SENSOR=0x26,                  //��ʼ��sensor
	SWITCH_PROTOCOL=0x27,              //�л�Э��
	DEBUG_PRINT_EN=0x40,              //ʹ�ܻ��߹ر�fsmDebugPrint
	DEBUG_PRINT_MODE=0x41,            //����fsmDebugPrint
	DEBUG_GET_PARA=0x42,
	DEBUG_UPLOAD=0x43,
	DEBUG_UPDATE=0x44,
	DEBUG_ENROLL_AFTER_DOWNLOAD=0x45,
	DEBUG_MATCH_AFTER_DOWNLOAD=0x46,
    DEBUG_RESET_TEMPLATE_STORAGE=0x47,
    DEBUG_READ_ALL_FLASH=0x48,
	FRAME_MAX
};

//ȷ����
#define PS_OK                   0x00     //OK
#define PS_COMM_ERR             0x01     //ͨ�Ŵ���
#define PS_GET_IMG_ERR          0x03     //��ȡͼ��ʧ��
#define PS_NOT_SEARCHED         0x09     //ƥ��ʧ��
#define PS_ADDRESS_OVER         0x0b     //ָ��IDԽ��
#define PS_UP_CHAR_ERR          0x0d
#define PS_RECV_ERR             0x0e     //����֡���ȴ��������
#define PS_UP_IMG_ERR           0x0f     //�ϴ�ͼ�����
#define PS_DEL_TEMP_ERR         0x10     //ɾ��IDʧ��
#define PS_CLEAR_TEMP_ERR       0x11     //���ָ�ƿ�ʧ��
#define PS_INVALID_PASSWORD     0x13     //�������
#define PS_INVALID_REG          0x1a     //����ļĴ���
#define PS_ENROLL_ERR           0x1e     //ע�����
#define PS_DEVICE_ADDR_ERR      0x20     //������ַ����
#define PS_MUST_VERIFY_PWD      0x21     //������֤����
#define PS_PARAM_ERROR          0x22     //��������
#define PS_END_UPLOAD           0x23
#define PS_DUL_ENROLL_ERROR     0x24     //ָ��ID��ռ��
#define PS_NO_ERROR             0x25     //��������ָ¼��һ�γɹ�
#define PS_TIMEOUT              0x26     //��ʱ
#define PS_DOWN_IMG_ERR         0x27     //����ʧ��
#define PS_COMMAND_ERR          0x28     //�������
#define PS_PID_ERR              0x29     //����ʶ����
#define PS_ENROLL_TIMES_WRONG   0x30    //ע�������ƥ��
#define PS_UPDATE_FAIL          0x31    //����ʧ��
#define PS_SWITCH_PROTOCOL_FAIL 0x32    //�л�Э��ʧ��
#define PS_RESPONSE             0x33    //Ӧ���
#define PS_GET_IMG_OK           0x34




#define SYS_PARA_BAUDRATE	0x04
#define SYS_PARA_SECLVL		0x05
#define SYS_PARA_PACKLEN	0x06




#define FSM_MPU_START (0x004fee00)
#define FSM_MPU_END (0x00500000)
#define FSM_MPU_SIZE (FSM_MPU_END-FSM_MPU_START)


#define FSM_EFLASH_CODE_SIZE  (128*1024)    //128K code


#define FSM_EFLASH1_BASE (EFM_BASE)
#define FSM_EFLASH1_READ_START_ADDR  (FSM_EFLASH1_BASE + FSM_EFLASH_CODE_SIZE )
#define FSM_EFLASH1_READ_SIZE  (FSM_MPU_START -FSM_EFLASH1_BASE-FSM_EFLASH_CODE_SIZE)


#define FSM_EFLASH2_BASE (FSM_MPU_END)
#define FSM_EFLASH2_READ_START_ADDR  (FSM_EFLASH2_BASE)
#define FSM_EFLASH2_READ_SIZE  (EFM_END-FSM_EFLASH2_BASE)


#define FSM_SPIFLASH_READ_START_ADDR  (0x0)
#define FSM_SPIFLASH_READ_SIZE  (2*1024*1024)


#define FSM_SEND_DATA_IMAGE 0x01
#define FSM_SEND_DATA_EFLASH1_DATA 0x02
#define FSM_SEND_DATA_EFLASH2_DATA 0x03
#define FSM_SEND_DATA_SPIFLASH_DATA 0x04

// flag = 0 send image
// flag = 1 send eflash data
// flag = 2 send spiflash data

#define IMAGE_BACKGROUD                 0
#define IMAGE_RMV_BKG                   1
#define IMAGE_REALTIME                  2
#define IMAGE_TEMPORARY                 3
#define IMAGE_ENROLL                    4
#define IMAGE_AUTHEN                    5
#define IMAGE_TESTING                   6


#define IMAGE_DOWN_RAW                  0
#define IMAGE_DOWN_RMV_BKG              1
#define IMAGE_DOWN_TEMPLATE             2
#define IMAGE_DOWN_SENSOR_CFG           3


extern void fsmInit(void);
extern void fsmTask(S32 ch);
extern void deviceReady(void);
extern void fsmDebugPrint(const char *fmt, ...);

U8 SendData(char *name,U8 *buf,U32 len);



#endif
