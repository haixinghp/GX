#ifndef	__SM_H__
#define	__SM_H__

#include	"type.h"

#define		RETRY_MAX	0xFFFF		//����������
#define		RETRY_15	0xFFFE		//0
#define		RETRY_14	0xFFFC		//1
#define		RETRY_13	0xFFF8		//2
#define		RETRY_12	0xFFF0		//3
#define		RETRY_11	0xFFE0		//4
#define		RETRY_10	0xFFC0		//5
#define		RETRY_9		0xFF80		//6
#define		RETRY_8		0xFF00		//7
#define		RETRY_7		0xFE00		//8
#define		RETRY_6		0xFC00		//9
#define		RETRY_5		0xF800		//10
#define		RETRY_4		0xF000		//11
#define		RETRY_3		0xE000		//12
#define		RETRY_2		0xC000		//13
#define		RETRY_1		0x8000		//14
#define		RETRY_0		0x0000		//15

//����pin��ȷ���pin����Ϊ0ʱ����
//��λ��1��ʾ17��0xff��ʾ��������
#define		TOTAL_RETRY_LOW_0	0x0000		//0
#define		TOTAL_RETRY_LOW_1	0x8000		//1
#define		TOTAL_RETRY_LOW_2	0xC000		//2
#define		TOTAL_RETRY_LOW_3	0xE000		//3
#define		TOTAL_RETRY_LOW_4	0xF000		//4
#define		TOTAL_RETRY_LOW_5	0xF800		//5
#define		TOTAL_RETRY_LOW_6	0xFC00		//6
#define		TOTAL_RETRY_LOW_7	0xFE00		//7
#define		TOTAL_RETRY_LOW_8	0xFF00		//8
#define		TOTAL_RETRY_LOW_9	0xFF80		//9
#define		TOTAL_RETRY_LOW_10	0xFFC0		//10
#define		TOTAL_RETRY_LOW_11	0xFFE0		//11
#define		TOTAL_RETRY_LOW_12	0xFFF0		//12
#define		TOTAL_RETRY_LOW_13	0xFFF8		//13
#define		TOTAL_RETRY_LOW_14	0xFFFC		//14
#define		TOTAL_RETRY_LOW_15	0xFFFE		//15
#define		TOTAL_RETRY_LOW_16	0xFFFF		//16

typedef	struct{
	U8 type;//��Կ����,�����ڵĲ���
	U8 ver;//��Կ�汾
	U8 size;//��Կ�ĳ���
	U8 reserve;
}KEY_PARA;

typedef	struct{//12bytes
	U8 reserve1;//     // 2017-2-23 17:45:35	ȥ��ID
	U8 ver;//��Կ�汾
	U8 algo;//֧�ֵ��㷨algorithm
	U8 type;//��Կ����,�����ڵĲ���
	U8 uac;//��Կʹ��Ȩ��
	U8 wac;//��Կ�޸�Ȩ��
	U8 follow;//������״̬
	U8 cnt;//���������ܴ���
	U8 size;//��Կ�ĳ���
	U8 changed;//��Կ�Ƿ��޸Ĺ�,0xff-û�޸ģ�����-�޸Ĺ�
	U16 retry;//��ǰ���������ԵĴ���

}KEY_STRUCT;


typedef struct{
	U16 fd_id;
	U8	sm_state;
	U8	reserve;
}SM_STATE_STRUCT;


#define		SEARCH_KEYID	0
#define		SEARCH_KEYTYPE	1
#define		SEARCH_KEY2	2//id and type
#define		SEARCH_KEY3	3//algo and type

//key algorithm
#define		SM_ALGO_DES		0x11
#define		SM_ALGO_3DES	0x21   //lsun
#define		SM_ALGO_SM1		0x12
#define		SM_ALGO_AES		0x13
#define		SM_ALGO_SM4		0x14
#define		SM_ALGO_SSF33	0x15
#define		SM_ALGO_SM6		0x16

#define		SM_ALGO_SM2		0x17
#define		SM_ALGO_RSA		0x18




//KEY TYPE
#define		SM_KEYTYPE_CTRL			0x00//������Կ
#define		SM_KEYTYPE_PIN			0x01//�û�����
#define		SM_KEYTYPE_EXAUTH		0x02//�ⲿ��֤��Կ
#define		SM_KEYTYPE_INANTH		0x03//�ڲ���֤��Կ
#define		SM_KEYTYPE_SUPPIN		0x04//�����������������ڽ����û�����
#define		SM_KEYTYPE_MAINTAIN		0x05//Ӧ��ά����Կ����������Կ��Ӧ��id���ܻ�ã���������mac
#define		SM_KEYTYPE_PINUNLOCK	0x06//���������Կ�����ڽ����û�����ʱ����mac
#define		SM_KEYTYPE_PINRELOAD	0x07//������װ��Կ���������°�װ�û�����ʱ����mac
#define		SM_KEYTYPE_CRYPT		0x08//���ݼӽ�����Կ�����ڰ�ȫ�����������Կ��װ�޸�����Ա��Ľ��мӽ���
#define		SM_KEYTYPE_KEK			0x09//��Կ������Կ�����ڱ����Ự��Կ

#define	TMPKEY_LEN	16//��ʱ��Կ����������

//key len
//#define		SM_KEYLEN_PIN	8
#define		SM_KEYLEN_MAXPIN	16
#define		SM_KEYLEN_DES	8	
#define		SM_KEYLEN_3DES	16
#define		SM_KEYLEN_SM1	16
#define		SM_KEYLEN_AES	16//CRT(CBC)ģʽΪ32
#define		SM_KEYLEN_SSF33	16
#define		SM_KEYLEN_SMS4	16

#define		SM_KEY_UNCHANGE	0xFF
#define		SM_KEY_CHANGED	0xFE

#define		SM_IVLEN_DES	8//DES,3DES
#define		SM_IVLEN_SM1	16//SM1

#define		SM_DATALEN_DES		8
#define		SM_DATALEN_SM1		16
#define		SM_DATALEN_AES		16
#define		SM_DATALEN_SSF33	16
#define		SM_DATALEN_SMS4		16
#define		OP_ENC	0
#define		OP_DEC	1

#define		MODE_ECB	0
#define		MODE_CBC	1

//#define		SM_ISF_ID		0x0000

//write key p1
#define		SM_KEY_UPDATE	0x01//������Կ
#define		SM_KEY_APPEND	0x02//װ����Կ
//write key p2
#define		SM_APPEND_EXAUTH	1//װ��������Կ����Ҫ�ڸ�df�ļ��е�������Կ�����½���
#define		SM_APPEND_OTHER		0//װ��������Կ

#define		SM_OP_READ			0//read bin, read record
#define		SM_OP_WRITE			1//update bin, update record, write key, pin unblock
#define		SM_OP_MAINTAIN		2//reload pin, app block, app unblock, card block,

#define		SM_MAC_LEN			4
#define		SM_MAC_FILLIN		0x80

#define		SM_FLAG_SUPPIN	1
#define		SM_FLAG_USERPIN	0

#define		ALGO_DES			0x00000001
#define		ALGO_AES			0x00000002
#define		ALGO_AES_CTR		0x00000004
#define		ALGO_SM1			0x00000008
#define		ALGO_SMS4			0x00000010
#define		ALGO_SSF33			0x00000020
#define		ALGO_SM6			0x00000040

#define		ALGO_RSA_1024		0x00001000
#define		ALGO_RSA_2048		0x00002000
#define		ALGO_SM2			0x00004000

#define		ALGO_SM3			0x00100000
#define		ALGO_SHA1			0x00200000
#define		ALGO_SHA224			0x00400000
#define		ALGO_SHA256			0x00800000
#define		ALGO_SHA384			0x01000000
#define		ALGO_SHA512			0x02000000

#define		ALGO_SUPPORT\
				ALGO_DES|ALGO_AES|ALGO_AES_CTR|ALGO_SM1|ALGO_SMS4|ALGO_SSF33|ALGO_SM6\
				|ALGO_RSA_1024|ALGO_RSA_2048|ALGO_SM2\
				|ALGO_SM3|ALGO_SHA1|ALGO_SHA224|ALGO_SHA256




///  ԭ����apdu_command.h ��
//for manage pin
#define	P1_RELOAD_PIN				0
#define	P1_CHANGE_PIN				1
//for app block
#define	P2_BLOCK_TEMPORARY			0//Ӧ����ʱ���������ͨ��UNBLOCK����
#define	P2_BLOCK_FOREVER			1//Ӧ����������

#define	SESSIONKEY_CNT		100
#define	SESSIONKEY_MAXLEN	16

#define	DES_CODE

extern U16 sm_manage(U8 *pdatain, U8 **pdataout, U32 *datalen);
extern U16 ID2_key_encode(U32 *pdata, U32 *pdecode, U16 datalen, U32 *pkeybuf,U32 *iv);
extern U16 ID2_key_decode(U32 *pdata, U32 *pdecode, U16 datalen, U32 *pkeybuf,U32 *iv);

#endif
