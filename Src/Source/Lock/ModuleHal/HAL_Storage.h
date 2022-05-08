/***************************************************************************************
**�ļ���:     HAL_Storage.h
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    
����������ļ� 
**��ע:      
****************************************************************************************/
#ifndef __HAL_STORAGE_H__
#define __HAL_STORAGE_H__
#include "MyConfig.h"
#define HAL_STORAGE_MODULE_EN (1) //ģ��ʹ��
#if HAL_STORAGE_MODULE_EN
/*************************ר�ú궨��*************************/
#define HAL_STORAGE_SYS_DATA_PWD_SIZE (8)  //ϵͳ����������ĳ���
#define HAL_STORAGE_SYS_TORQUE_SIZE (3)    //ϵͳ�����е�Ť������
#define HAL_STORAGE_SYS_MODEL_SIZE (8)     //ϵͳ�����е������ͺų���
#define HAL_STORAGE_SYS_FACE_UID_SIZE (20) //ϵͳ�����е�face uid����
#define MOTOR_TORQUE_HIGH (2) //�ߵ��Ť��
#define MOTOR_TORQUE_LOW (1) //�͵��Ť��
#define HAL_STORAGE_SYS_VIOCE_SW_OPEN (3)  //������
#define HAL_STORAGE_SYS_VIOCE_SW_CLOSE (0) //������
#define HAL_STORAGE_SYS_ALL_USER_MEM_SIZE (20) //�����û��Ĵ洢��С,��λΪKB
#define HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE (10)  //��ʱ��������
#define HAL_STORAGE_PWD_LEN (6)  //��ʱ���볤��
/*********************************************************************************
 * ָ�ƽṹ
 * ********************************************************************************/
#define LOCK_FUNCTION_TIMEVAILD_ON                            //ʱЧ���ܿ���
#define LOCK_FUNCTION_KINDRED_ON                              //���鹦�ܿ���
#define LOCK_FUNCTION_SOS_ON                                  //�������ܿ���
#define LOCK_FUNCTION_FINGERPWD_ON                            //���û�����
#define FLM_CONFIG_MEM_CARD                                   //��
#define HAL_STORAGE_VAILD_ADDR (0)                            //ָ����Ч
#define HAL_STORAGE_UP_ID_ADDR (1)                            //��һ���H
#define HAL_STORAGE_DOWN_ID_ADDR (3)                          //��һ���H
#define HAL_STORAGE_LIM_ADDR (5)                              //����
#define HAL_STORAGE_GROUP_ADDR (6)                            //���H
#define HAL_STORAGE_NAME_ADDR (8)                             //���H
#define HAL_STORAGE_PAGE_ID_ADDR (14)                         //��ǰ���H
#define HAL_STORAGE_PWD_FIG_ADDR (16)                         //�����־
#define HAL_STORAGE_PWD_ADDR (17)                             //����
#define HAL_STORAGE_TM_VAILD_FIG_ADDR (20)                    //ʱЧ��־
#define HAL_STORAGE_TM_VAILD_STAR_ADDR (21)                   //ʱЧ��ʼ
#define HAL_STORAGE_TM_VAILD_END_ADDR (27)                    //ʱЧ����
#define HAL_STORAGE_TM_VAILD_WDAY_ADDR (33)                   //����ʱЧ
#define HAL_STORAGE_KINDRED_ADDR (34)                         //�����־
#define HAL_STORAGE_SOS_ADDR (35)                             //SOSв�ȱ�־
#define HAL_STORAGE_CHKSUM_ADDR (36)                          //У��H
#define HAL_STORAGE_CHKSUM_LEN (2)                            //У��͵ĳ���
#define HAL_STORAGE_ONE_FINGER_SIZE (MSG_FINGER_REG_ONE_SIZE) //һ��ָ������ռ�õĿռ�

#define MEM_FINGER_FACE_ADMIN_LOCAL 5                            //5���û�
#define MEM_FINGER_FACE_GUEST_LOCAL 15                           //15���û�
#define MEM_FINGER_ADMIN_START_ID (0)                            //����ָ����ʼid
#define MEM_FINGER_ADMIN_ALL_LOCAL (5)                           //5������ָ���û�
#define MEM_FINGER_GENERAL_START_ID (MEM_FINGER_ADMIN_ALL_LOCAL) //��ָͨ����ʼid
#define MEM_FINGER_GUEST_ALL_LOCAL (20)                          //��ָͨ���û�
#define MEM_FACE_ALL_ONLINE		  (20)
/*********************************************************************************
 * ϵͳ�Ĵ洢�ṹ
 * ********************************************************************************/
#define MEM_FACTMEM_START_ADDR (0x300000)        //�洢��ʼ��ַ
//#define MEM_FACTMEM_START_ADDR (0x000000)        //�洢��ʼ��ַ
#define MEM_FACT_MEM_SIZE (128)                  //�洢����С
#define MEM_FACTMEM_FACTORY_FIG 0                //������־  G
#define MEM_FACTMEM_LOCKMODE 1                   //����ģʽ��ָ��/��������/����Կ�ף�
#define MEM_FACTMEM_FINGER_ALL_ONLINEH 2         //ȫ��ָ��H��60*3��
#define MEM_FACTMEM_FINGER_ALL_ONLINEL 3         //ȫ��ָ��L��60*3��
#define MEM_FACTMEM_FINGER_LEAVE_ONLINEH 4       //ʣ�����ָ��H��60*3��
#define MEM_FACTMEM_FINGER_LEAVE_ONLINEL 5       //ʣ�����ָ��L��60*3��
#define MEM_FACTMEM_PHONE_ALLH 6                 //ALL��H
#define MEM_FACTMEM_PHONE_ALLL 7                 //ALL��L
#define MEM_FACTMEM_PHONE_LEAVEH 8               //LEAVE��H
#define MEM_FACTMEM_PHONE_LEAVEL 9               //LEAVE��L
#define MEM_FACTMEM_SMARTKEY_ALLH 10             //ȫ������Կ������H ��20����
#define MEM_FACTMEM_SMARTKEY_ALLL 11             //ȫ������Կ������L  ��20����
#define MEM_FACTMEM_SMARTKEY_LEAVEH 12           //ʣ������Կ������H
#define MEM_FACTMEM_SMARTKEY_LEAVEL 13           //ʣ������Կ������L
#define MEM_FACTMEM_FINGER_ADMIN_LOCALH 14       //���ع���Աָ������H
#define MEM_FACTMEM_FINGER_ADMIN_LOCALL 15       //���ع���Աָ������L
#define MEM_FACTMEM_FINGER_GUEST_LOCALH 16       //�����û�ָ������H
#define MEM_FACTMEM_FINGER_GUEST_LOCALL 17       //�����û�ָ������L
#define MEM_FACTMEM_WIFI_MAIN_SW 18              //WIFI������
#define MEM_FACTMEM_WIFI_LOGUP_SW 19             //WIFI�����ź��ϴ�����
#define MEM_FACTMEM_ADMIN_REGISTER 20            //���ع���Ա�Ƿ�ע��H \
                                                 //���ع���Ա�Ƿ�ע��L
#define MEM_FACTMEM_VOICE 22                     //��������
#define MEM_DH_G 23                              //dhԭ��
#define MEM_DH_P 24                              //DH����
#define MEM_FACTMEM_DONE_FIG 25                  //������־  G
#define MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH 26 //���ָ��ID  H��ע�����ɾ�����ã�����û���ַ
#define MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL 27 //���ָ��ID  L��ע�����ɾ�����ã�
#define MEM_FACTMEM_SMARTKEYOVER_PAGEIDH 28      //���һ������Կ�״洢λ��H��ע�����ɾ�����ã�
#define MEM_FACTMEM_SMARTKEYOVER_PAGEIDL 29      //���һ������Կ�״洢λ��L��ע�����ɾ�����ã�
#define MEM_FACTMEM_CARDOVER_PAGEIDH 30          //��λ��ID  H��ע�����ɾ�����ã�
#define MEM_FACTMEM_CARDOVER_PAGEIDL 31          //��λ��ID L��ע�����ɾ�����ã�
#define MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALH 32 //���һ�����H��ע�����ɾ�����ã������, ���һ�����.
#define MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALL 33 //���һ�����L��ע�����ɾ�����ã������, ���һ�����.
#define MEM_FACTMEM_LOCKLOG_LINK_ST 34           //������¼4�ֽ�
#define MEM_FACTMEM_LOCKLOG_LINK_2 35            //������¼4�ֽ�
#define MEM_FACTMEM_LOCKLOG_LINK_3 36            //������¼4�ֽ�
#define MEM_FACTMEM_LOCKLOG_LINK_OV 37           //������¼4�ֽ�
#define MEM_FACTMEM_LOCKLOG_SN_H 38              //������¼���к�H
#define MEM_FACTMEM_LOCKLOG_SN_L 39              //������¼���к�L
#define MEM_FACTMEM_ADMIN_PWD1 40                //����
#define MEM_FACTMEM_ADMIN_PWD2 41                //����
#define MEM_FACTMEM_ADMIN_PWD3 42                //����
#define MEM_FACTMEM_ADMIN_PWD4 43                //����
#define MEM_FACTMEM_ADMIN_PWD5 44                //����
#define MEM_FACTMEM_ADMIN_PWD6 45                //����
#define MEM_FACTMEM_ADMIN_PWD7 46                //����
#define MEM_FACTMEM_ADMIN_PWD8 47                //����
#define MEM_EEPROM_TEST_L 48                     //EE����
#define MEM_EEPROM_TEST_H 49                     //EE����
#define MEM_FINGER_AES_EN 51                     //ָ��AESʹ��
#define MEM_FINGER_AES_MANUFACTUER 52            //ָ��AES����
#define MEM_FACTMEM_EEPROM_LOGO 64               //logo 8�ֽ�
#define MEM_FACTMEM_EEPROM_MODEL 72              //�����ͺ� 8�ֽ�
#define MEM_FACTMEM_FINGER_ASEKEY 88             //ָ��ͷAES����Կ16�ֽ�
#define MEM_LOCK_SN_CODE (120)                   //�û�SN 8�ֽ�
#define AUDIO_SWITCH 0x19F                       //��������
#define MEM_BOARDINFO 0x200                      //������Ϣ���򱸷ݵ�ַ δʹ��
#define MSG_BOARDINFO_LEN 50                     //������Ϣ���򱸷ݵ�ַ���� δʹ��
#define MEM_BOARDPWD (0x280)                     //����
#define MEM_BOARDPWD_PWDLEN 8
#define MEM_BOARDPWD_PWDSOSLEN 8
#define MEM_BOARDPWD_PWDTMP_ALL 10
#define MEM_BOARDPWD_PWDTMP_LEN 8
/*********************************************************************************
 * REG2   ��ʼλ��Ϊ768(0x300) Ŀǰ�ķ�Χ��768 ~ (768+64*180 = 0x3000)
 * ********************************************************************************/
#define MSG_FINGER_REG_MEM_ONLINE_ST (MEM_FACTMEM_START_ADDR + 0x0300) //ָ��ע����ʼ��ַ <<ע:���һ���ֽ�Ϊ00>>
#define MSG_FINGER_REG_ONE_SIZE 38                                     //��ָ�������ֽ�����
#define MSG_FINGER_ONE_SIZE 64                                         //��ָ��ʵ�ʿռ�
#define MEM_FINGER_ALL_ONLINE (60 * 3)                                 //180��ָ��. 60���û�
/*********************************************************************************
 * local fig   ��ʼλ��Ϊ12544(0X3100) Ŀǰ�ķ�Χ��0X3100 ~ 12544+64*40 = 0x3B00
 * ********************************************************************************/
#define MSG_FINGER_REG_MEM_LOCAL_ST (((MSG_FINGER_REG_MEM_ONLINE_ST + (MEM_FINGER_ALL_ONLINE)*MSG_FINGER_ONE_SIZE) & 0xFFFFFF00) + 0x0100)
#define MSG_FINGER_REG_ONE_SIZE 38 //��ָ�������ֽ�����
#define FINGER_LOCAL_MULTI 2
#define MEM_FINGER_ALL_LOCAL (20 * FINGER_LOCAL_MULTI) //40��ָ��, 20���û�
/*********************************************************************************
 * REG3 phone ��ʼλ��Ϊ15360(0x3C00)    Ŀǰ�ķ�Χ��0x3C00 ~ 15360+64*150 = 0x6180
 * ********************************************************************************/
#define MSG_CARD_REG_MEM_ST (((MSG_FINGER_REG_MEM_LOCAL_ST + (MEM_FINGER_ALL_LOCAL)*MSG_FINGER_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //��ע����ʼ��ַ <<ע:���һ���ֽ�Ϊ00>>//F00
#define MSG_CARD_REG_ONE_SIZE (38 + 6 + 7)                                                                                       //���������ֽ�����
#define MSG_CARD_REG_SIZE (64)
#define MEM_PHONE_ALL 150 //����ʵ�ʿռ�
/*********************************************************************************
 * REG4 SMARTKEY ��ʼλ��Ϊ25088(0X6200)    Ŀǰ�ķ�Χ��0X6200 ~ 25088+64*20 = 0x6700
 * ********************************************************************************/
#define MSG_SMARTKEY_REG_MEM_ST (((MSG_CARD_REG_MEM_ST + MEM_PHONE_ALL * MSG_CARD_REG_SIZE) & 0xFFFFFF00) + 0x0100) //��ע����ʼ��ַ <<ע:���һ���ֽ�Ϊ00>>
#define MSG_SMARTKEY_REG_ONE_SIZE (38 + 6 + 7)
#define MSG_SMARTKEY_REG_SIZE (64) //����ʵ�ʿռ�
#define MEM_SMARTKEY_ALL 20        //20������Կ��.

/*********************************************************************************
 * REG5FACE ��ʼλ��Ϊ26,624(0X6800)    Ŀǰ�ķ�Χ��0X6800 ~ 26,624+64*20 = 0X6D00
 * ********************************************************************************/
#define MSG_FACE_REG_MEM_ST (((MSG_CARD_REG_MEM_ST + MEM_SMARTKEY_ALL * MSG_SMARTKEY_REG_SIZE) & 0xFFFFFF00) + 0x0100) //��ע����ʼ��ַ <<ע:���һ���ֽ�Ϊ00>>
#define MSG_FACE_REG_ONE_SIZE (38 + 6 + 7)
#define MSG_FACE_REG_SIZE (64) //����ʵ�ʿռ�
#define MEM_FACE_ALL 20        //20������

/*********************************************************************************
 *  LOG ��ʼλ��Ϊ28,160(0X6E00)    Ŀǰ�ķ�Χ��0X6E00 ~ 28,160+17*600 = 0x90D8
 * ********************************************************************************/
#define MSG_FACTMEM_LOCKLOG_LINK (((MSG_FACE_REG_MEM_ST + MEM_FACE_ALL * MSG_FACE_REG_SIZE) & 0xFFFFFF00) + 0x0100) //���� <<ע:���һ���ֽ�Ϊ00>>
#define MSG_LOCKLOGMEM_ONCE 16                                                                                      //������¼ռ�õĿռ�
#define MEM_LOCKLOG_ALL 600                                                                                         //
/*********************************************************************************
 * ID2_TOKEN_ST 
 * ********************************************************************************/
#define MSG_ID2_TOKEN_ST (((MSG_FACTMEM_LOCKLOG_LINK + MEM_LOCKLOG_ALL * MSG_LOCKLOGMEM_ONCE) & 0xFFFFFF00) + 0x0100) //TOKEN
/*********************************************************************************
 * ID2_ONorOFF_ST 
 * ********************************************************************************/
#define MSG_ID2_ONorOFF_ST (MSG_ID2_TOKEN_ST & 0xFFFFFF00) + 0x0100 //���û��߽���ID2ҵ��
/*********************************************************************************
 * MSG_ID2_id_ST 
 * ********************************************************************************/
#define MSG_ID2_id_ST (MSG_ID2_ONorOFF_ST + 1) //����ID2��ID����25���ֽڣ���һ���ֽ��Ǳ�־λ��1��ʾ��Ч, 0 ��ʾ��Ч
/*********************************************************************************
 * FLAG_ST 
 * ********************************************************************************/
#define NFC_FLAG_ST (MSG_ID2_id_ST + 10) //����NFCʹ�ܱ�־
/*********************************************************************************
 * LOCK_ONE_CLICK_DEFENE_MEM_ST 
 * ********************************************************************************/
#define LOCK_ONE_CLICK_DEFENE_MEM_ST (NFC_FLAG_ST + 1) //һ��������׼
/*********************************************************************************
 * ROOT_KEY_MEM_ST   
 * ********************************************************************************/
#define ROOT_KEY_MEM_ST (LOCK_ONE_CLICK_DEFENE_MEM_ST + 10) //����Կ���


// #ifdef M_CARD_FUNCTION
// #ifdef CPU_CARD_ENABLE
// #define MEM_CARD_ALL 30
// #define MSG_CPU_CARD_REG_ONE_SIZE 275 //274//CPU�� ID ��Կ��
// #ifdef T11H
// #define MSG_CARD_REG_ONE_SIZE MSG_CARD_REG_SIZE //MSG_FINGER_REG_ONE_SIZE//�����û������ֽ�����    <ʵ��ʹ������38,������fingerͳһ>
// #else

// #endif
// //MSG_CPU_CARD_REG_MEM_ST (0x9200)
// #define MSG_CPU_CARD_REG_MEM_ST ((ROOT_KEY_MEM_ST & 0xFFFFFF00) + 0x0100)
// #define MSG_CPU_CARD_SE_CUR_KEY_ST (((MSG_CPU_CARD_REG_MEM_ST + MEM_CARD_ALL * MSG_CPU_CARD_REG_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //���� <<ע:���һ���ֽ�Ϊ00>>
// #define MSG_CPU_CARD_NUM_ST (MSG_CPU_CARD_SE_CUR_KEY_ST + 1)                                                                      //��ΪǮ���ſ�����
// #else
// #define MEM_CARD_ALL 100
// #define MSG_CARD_REG_ONE_SIZE MSG_FINGER_REG_ONE_SIZE //�����û������ֽ�����    <ʵ��ʹ������38,������fingerͳһ>
// #endif
// #else
// #define MSG_FACTMEM_LOCKLOG_LINK (((MSG_FINGER_REG_MEM_ST + MEM_FINGER_ALL * MSG_FINGER_REG_ONE_SIZE) & 0xFFFFFF00) + 0x0100) //���� <<ע:���һ���ֽ�Ϊ00>>
// #endif
/*********************************************************************************
 * ����Ϊϵͳ��Ĭ��ֵ  
 * ********************************************************************************/
#define MSG_FINGERREGMEM_FIG ('W')
#define MSG_FINGERREGMEM_PID_MASTER ('M')
#define MSG_FINGERREGMEM_PID_GUEST ('G')
#define MSG_FACTMEM_FACTORY_FIG_OK ('W')
#define MSG_FACTMEM_FACTORY_CLEAR ('N')
#define MSG_FACTMEM_CHANNEL_PWD1 0
#define MSG_FACTMEM_CHANNEL_PWD2 0
#define MSG_FACTMEM_CHANNEL_PWD3 0
#define MSG_FACTMEM_CHANNEL_PWD4 0
#define MSG_FACTMEM_ADMIN_PWD1 0x38
#define MSG_FACTMEM_ADMIN_PWD2 0x38
#define MSG_FACTMEM_ADMIN_PWD3 0x38
#define MSG_FACTMEM_ADMIN_PWD4 0x38
#define MSG_FACTMEM_ADMIN_PWD5 0x38
#define MSG_FACTMEM_ADMIN_PWD6 0x38
#define MSG_FACTMEM_ADMIN_PWD7 0x38
#define MSG_FACTMEM_ADMIN_PWD8 0x38
#define MSG_FACTMEM_DEVICENAME ' '
#define MSG_FACTMEM_FINGER_ALL_ONLINEH (INT8U)(MEM_FINGER_ALL_ONLINE >> 8)
#define MSG_FACTMEM_FINGER_ALL_ONLINEL (INT8U)(MEM_FINGER_ALL_ONLINE)
#define MSG_FACTMEM_FINGER_LEAVE_ONLINEH (INT8U)(MEM_FINGER_ALL_ONLINE >> 8) //Ĭ��ʣ��
#define MSG_FACTMEM_FINGER_LEAVE_ONLINEL (INT8U)(MEM_FINGER_ALL_ONLINE)
#define MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEH 0 //Ĭ�����һ��ע��PAGEIDΪ 0
#define MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEL 0

#define MSG_FACTMEM_FINGER_ALL_LOCALH (INT8U)(MEM_FINGER_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_ALL_LOCALL (INT8U)(MEM_FINGER_ALL_LOCAL)
#define MSG_FACTMEM_FINGER_LEAVE_LOCALH (INT8U)(MEM_FINGER_ALL_LOCAL >> 8) //Ĭ��ʣ��
#define MSG_FACTMEM_FINGER_LEAVE_LOCALL (INT8U)(MEM_FINGER_ALL_LOCAL)

#define MSG_FACTMEM_FINGER_ADMIN_ALL_LOCALH (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_ADMIN_ALL_LOCALL (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 0)

#define MSG_FACTMEM_FINGER_GUEST_ALL_LOCALH (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 8)
#define MSG_FACTMEM_FINGER_GUEST_ALL_LOCALL (INT8U)(MEM_FINGER_ADMIN_ALL_LOCAL >> 0)

#define MSG_FACTMEM_REGMSGOVER_PAGEID_LOCAL_START 0x00B4 //  180 ����ָ����ʼ

#define MSG_FACTMEM_LOCKLOG_LINK_ST 0 //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>24)
#define MSG_FACTMEM_LOCKLOG_LINK_2 0  //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>16)
#define MSG_FACTMEM_LOCKLOG_LINK_3 0  //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK>>8)
#define MSG_FACTMEM_LOCKLOG_LINK_OV 1 //(INT8U)(MSG_FACTMEM_LOCKLOG_LINK) ��¼���,�����ֵ�󲻱�
#define MSG_FACTMEM_LOCKLOG_SN_H 0    //Ĭ�Ͽ�����¼���к�Ϊ0
#define MSG_FACTMEM_LOCKLOG_SN_L 1
#define MSG_FACTMEM_LOCKLOG_SN (INT16U)(1)
#define MSG_FACTMEM_LOCKLOG_ALL (INT32U)(1)
#define MSG_FACTMEM_PHONE_ALLH (INT8U)(MEM_PHONE_ALL >> 8)
#define MSG_FACTMEM_PHONE_ALLL (INT8U)(MEM_PHONE_ALL)
#define MSG_FACTMEM_PHONE_LEAVEH MSG_FACTMEM_PHONE_ALLH
#define MSG_FACTMEM_PHONE_LEAVEL MSG_FACTMEM_PHONE_ALLL
#define MSG_FACTMEM_PHONEOVER_PAGEIDH 0
#define MSG_FACTMEM_PHONEOVER_PAGEIDL 0
#define MSG_FACTMEM_SMARTKEY_ALLH (INT8U)(MEM_SMARTKEY_ALL >> 8)
#define MSG_FACTMEM_SMARTKEY_ALLL (INT8U)(MEM_SMARTKEY_ALL)
#define MSG_FACTMEM_SMARTKEY_LEAVEH MSG_FACTMEM_SMARTKEY_ALLH
#define MSG_FACTMEM_SMARTKEY_LEAVEL MSG_FACTMEM_SMARTKEY_ALLL
#define MSG_FACTMEM_SMARTKEYOVER_PAGEIDH 0
#define MSG_FACTMEM_SMARTKEYOVER_PAGEIDL 0

#define MSG_FACTMEM_REG_PAGEID_NULL (0XFFFF) //ָʾλ��Ϊ��
#define MSG_FACTMEM_REG_VAILD_NULL (0XFF)    //ָʾ�����Чֵ

#define MSG_FACTMEM_REG_STATUS_NONE (0xA5B7) //��ע��
#define MSG_FACTMEM_REG_STATUS_OK (0x0001)   //���ع���Ա��ע��
#define MSG_FACTMEM_REG_STATUS_ING (0x0002)  //ע����
#define MSG_FACTMEM_LIM_ADMIN ('M')          //����ָ��
#define MSG_FACTMEM_LIM_GENERAL ('G')        //��ָͨ��
#define MSG_FACTMEM_LIM_PHONE ('P')        	 //�ֻ�
#if 1
#define MSG_FACTMEM_LOCKMODE_FINGER (0X01)   //ָ�ƿ��ŷ�ʽ
#define MSG_FACTMEM_LOCKMODE_BLE (0X02)      //�������ŷ�ʽ
#define MSG_FACTMEM_LOCKMODE_SMARTKEY (0X04) //����Կ�׿��ŷ�ʽ
#define MSG_FACTMEM_LOCKMODE_PWD (0X08)      //���뿪�ŷ�ʽ
#define MSG_FACTMEM_LOCKMODE_KEY (0X10)      //Կ�׿��ŷ�ʽ
#endif
#define MSG_FACTMEM_VOICE_OPEN (0X03) 		//������ 
/*************************�����ṹ����*************************/
#pragma pack(1) //����ǿ��ʹ��1�ֽڶ���ķ�ʽ
//typedef enum
//{
//    eVoiceSwClose=0, //�ر�
//    eVoiceSwOpen=3,  //����
//} TYPEe_VoiceSw;

typedef struct
{
    INT8U factoryFlag;                            //������־  G
    INT8U openLockMode;                           //����ģʽ��ָ��/��������/����Կ�ף�
    INT16U allFinger;                             //ȫ��ָ��
    INT16U leaveFinger;                           //ʣ�����ָ��
	INT16U allPhone;                               //ȫ���ֻ���
    INT16U leavePhone;                             //ʣ������ֻ���
    INT16U allCard;                               //ȫ��ָ��
    INT16U leaveCard;                             //ʣ�����ָ��
    INT16U allSmartKey;                            //ȫ������Կ��(����Կ��)
    INT16U leaveSmartKey;                          //ʣ������Կ��(����Կ��)
    INT16U localAdminNum;                         //���ع���Ա����
    INT16U localUserNum;                          //�����û�����
    INT8U wifiMainSw;                             //WIFI������
    INT8U wifiOpenUpdataSw;                       //WIFI�����ź��ϴ�����
    INT16U localAdminRegFlag;                     //���ع���Ա�Ƿ�ע��
    INT8U voiceSw;                        		  //��������	
    INT8U factoryFlag2;                           //������־  G
    INT16U lastFingerId;                          //���ָ��ID
    INT16U lastBlueKeyId;                         //���һ������Կ�״洢λ��
    INT16U cradId;                                //��λ��IDע�����ɾ������
    INT16U lasetGroup;                            //���һ����� ע�����ɾ������
    INT32U openLog;                               //������¼
    INT16U openLogSn;                             //������¼���к�
    INT8U pwd[HAL_STORAGE_SYS_DATA_PWD_SIZE];     //����
	INT8U pwdSos[HAL_STORAGE_SYS_DATA_PWD_SIZE];  //SOS����
    INT16U eeTest;                                //EE����
    INT8U keyDef;                                 //һ������
    INT8U irDef;                                  //��������
    INT8U bellDef;                                //�������,��ֹ���ż���
    INT8U speed;                                  //���ٶȿ���
    INT8U autoLockTime;                           //�Զ�����ʱ��
    INT8U pwdSetFlag;                             //�������ñ�־
    INT8U localAdminNumFace;                      //���ع�������
    INT8U localUserNumFace;                       //�����û�����
    INT8U leaveNumFaceApp;                        //ʣ��APP����
	INT8U faceNumApp;							  //APP��������
    INT16U fingerAseKey;                          //AES KEY  
    INT16U IrDistance;							  //�������
	INT8U ShangTangCatEyeSingleDouble; 			  //����è�۵�˫���л�
	INT8U tempPwd[HAL_STORAGE_SYS_DATA_TEMP_PWD_SIZE][HAL_STORAGE_SYS_DATA_PWD_SIZE];							//��ʱ����
	INT8U ID2Flag; //ID2��־
	INT8U ID2Data[24]; //ID2����
} TYPEs_StorageSysData;                           //ϵͳ����
typedef struct
{
	INT8U mac[6];                                 //MAC��ַ
    INT8U version[9];                             //���汾��
    INT8U encryptKey[6];                          //�Ự��Կ
	INT8U channelPwd[4];						  //�ŵ�����
	INT16U dir;                                   //����
    INT8U torque[HAL_STORAGE_SYS_TORQUE_SIZE];    //Ť������
    INT8U model[HAL_STORAGE_SYS_MODEL_SIZE];      //�����ͺ�
    INT8U faceUID[HAL_STORAGE_SYS_FACE_UID_SIZE]; //����UID
	INT8U lockBodyType; //��������
	INT8U dhG;                                    //dhԭ��
    INT8U dhP;                                    //DH����
} TYPEs_StorageHardwareData; //ϵͳӲ������,ֻ����Ӳ�����ʱ�޸�
typedef enum
{
    eStorageDireWrite, //д��
    eStorageDireRead,  //��ȡ
} TYPEe_StorageDire;

typedef struct
{
    INT16U all;         // 180��
    INT16U leave;       // APP����ָ��
    INT8U master;       //������
    INT16U overpageid;  //���һöָ�Ƶ�ID
    INT16U overgroupid; //����ָ�ƵĽ������.	0-65535
} TYPEs_StorageNum;     //ָ������

typedef struct
{
    INT8U year;
    INT8U mon;
    INT8U day;
    INT8U hour;
    INT8U min;
    INT8U sec;
} TYPEs_StorageFingerTime; //ʱ��

typedef struct
{
    INT8U fig;                   //ʹ�ܱ�־
    TYPEs_StorageFingerTime str; //��ʼʱ��
    TYPEs_StorageFingerTime stp; //����ʱ��
    INT8U wday;                  //����ʱЧ BIT1 -BIT 7��ʾ��һ������
} TYPEs_StorageTimeVaild;        //ʱЧ

typedef struct
{
    INT8U fig;      //����ʹ�ܱ�־
    INT8U pwd[3];   //����
} TYPEs_Storagepwd; //����ṹ

typedef struct
{
    INT8U fingerLim;      //����Limite ����,���ǿ���?
    INT8U name[6];        //ָ������
    INT16U down_pageID;   //��һ��ID
    INT16U pageID;        //��ǰID
    INT16U group;         //���.
} TYPEs_StoragFingerName; // ָ����

typedef struct
{
    INT8U vaild; //ָ����Ч
    // INT8U dis_sw;                 //��ʾ����
    INT16U sn;                    //���
    INT16U up_pageID;             //��һ�����
    INT16U down_pageID;           //��һ�����
    INT8U Lim;                    //����Limite ����Limite ����,���ǿ���?
    INT16U group;                 //���
    INT8S name[6];                //����
    INT16U pageID;                //��ǰ���
#ifdef LOCK_FUNCTION_FINGERPWD_ON //���û�����
    TYPEs_Storagepwd pwd;         //����ṹ
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON
    TYPEs_StorageTimeVaild TimeVaild; //ʱЧ
#endif
#ifdef LOCK_FUNCTION_KINDRED_ON
    INT8U kindred;
#endif
#ifdef LOCK_FUNCTION_SOS_ON
    INT8U sos;
#endif
    // #ifdef FLM_CONFIG_MEM_CARD
    //     INT8U cardid[15];
    // #endif
    INT16U checksum;   //У���
} TYPEs_StorageFinger; //ָ������
typedef struct
{
    INT8U vaild; //ָ����Ч
    // INT8U dis_sw;                 //��ʾ����
    INT16U sn;                    //���
    INT16U up_pageID;             //��һ�����
    INT16U down_pageID;           //��һ�����
    INT8U Lim;                    //����Limite ����Limite ����,���ǿ���?
    INT16U group;                 //���
    INT8S name[6];                //����
    INT16U pageID;                //��ǰ���
#ifdef LOCK_FUNCTION_FINGERPWD_ON //���û�����
    TYPEs_Storagepwd pwd;         //����ṹ
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON
    TYPEs_StorageTimeVaild TimeVaild; //ʱЧ
#endif
#ifdef LOCK_FUNCTION_KINDRED_ON
    INT8U kindred;
#endif
#ifdef LOCK_FUNCTION_SOS_ON
    INT8U sos;
#endif
#ifdef FLM_CONFIG_MEM_CARD
    INT8U UserID[13];
#endif
    INT16U checksum;    //У���
} TYPEs_StorageUserDat; //�û���Ϣ
typedef enum
{
    eMatchTypeSn, //���
    eMatchTypeGroup,  //���
	eMatchTypeUserID, //�û�ID
} TYPEe_MatchType;
typedef struct
{
    TYPEs_StorageSysData sysDat;       //ϵͳ����
	TYPEs_StorageHardwareData hardwareData; //Ӳ������
    TYPEs_StorageFinger finger;        //ָ������
    TYPEs_StorageUserDat UserDat;      //�û�����
    TYPEs_StorageNum fingerNum;        //ָ������
    TYPEs_StoragFingerName fingerName; //ָ���û���
} TYPEs_StorageSave;                   //ָ�ƴ洢

typedef struct
{
    INT16U lockRegStatus;    //ע��״̬
    INT16U adminFingerNum;   //����ָ������
    INT16U generalFingerNum; //��ָͨ������
    INT8U timeKey[6];        //ʱ�����
    INT8U macKey[6];         //MAC
    INT8U reservedKey[6];    //Ԥ��
} TYPEs_SystemDat;           //ϵͳ����
#pragma pack()               //ȡ������

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

#ifdef HAL_STORAGE_GLOBALS
#define HAL_STORAGE_EXT
#else
#define HAL_STORAGE_EXT extern
#endif /*HAL_STORAGE_GLOBALS*/

    /*************************��������*************************/
    HAL_STORAGE_EXT TYPEs_StorageSave storageSave;
    /*************************��������*************************/
    // HAL_STORAGE_EXT INT8S HAL_Storage_ChkFingerNum(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFinger(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ReadOneFingerReg(TYPEs_StorageFinger *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneFingerReg(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelOneFingerReg(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ResetFingerStruct(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_EraseAllFingerReg(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_lockRegStatusGet(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_lockRegStatusSet(TYPEs_SystemDat *dat, INT8U status);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveLockRegStatus(INT16U dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerNumChk(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerIdChk(INT16U id);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveAdminFingerNum(INT16U dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_GeneralFingerNumChk(TYPEs_SystemDat *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveGeneralFingerNum(INT16U dat);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFingerLocalAdmin(void);
    // HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmptyFingerLocalGeneral(void);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneFingerRegLocal(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelOneFingerRegLocal(TYPEs_StorageSave *msg);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelAllFingerLocalGeneral(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SystemDataInit(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SystemDataRead(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_DelAllFingerLocal(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_SavePwd(const INT8U *dat);
    // HAL_STORAGE_EXT INT8S HAL_Storage_ReadPwd(INT8U *dat);

    HAL_STORAGE_EXT INT8S HAL_Storage_SaveOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_GetOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_WriteSysData(TYPEs_StorageSysData *msg);
    HAL_STORAGE_EXT INT8S HAL_Storage_ReadSysData(TYPEs_StorageSysData *msg);
    HAL_STORAGE_EXT INT16U HAL_Storage_SearchEmpty(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT16S HAL_Storage_MatchId(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize, TYPEs_StorageUserDat *msg,TYPEe_MatchType matchType);
    HAL_STORAGE_EXT INT8S HAL_Storage_DelOneUserReg(INT16U ID, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT8S HAL_Storage_DelNUserReg(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize);
    HAL_STORAGE_EXT INT8S HAL_Storage_ChkSysData(void);
    HAL_STORAGE_EXT INT8S HAL_Storage_ClearSysData(void);
    HAL_STORAGE_EXT void HAL_Storage_EnterSleepConfig(void);
	HAL_STORAGE_EXT INT8S HAL_Storage_WriteHardwareData(TYPEs_StorageHardwareData *msg);
	HAL_STORAGE_EXT INT8S HAL_Storage_ReadHardwareData(TYPEs_StorageHardwareData *msg);
	HAL_STORAGE_EXT INT8S HAL_Storage_WriteLockLog(uint8_t *msg,uint32_t id);
	HAL_STORAGE_EXT INT8S HAL_Storage_ReadLockLog(uint8_t *msg,uint32_t id);
	HAL_STORAGE_EXT INT8S HAL_Storage_ClearUserData(void);
	HAL_STORAGE_EXT void HAL_Storage_Init(void);
	HAL_STORAGE_EXT INT8S HAL_Storage_AdminFingerIdChk(INT16U id);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#else  /*HAL_STORAGE_MODULE_EN*/

#endif /*HAL_STORAGE_MODULE_EN*/
#endif /*__HAL_STORAGE_H__*/
/************************************Code End******************************************/
