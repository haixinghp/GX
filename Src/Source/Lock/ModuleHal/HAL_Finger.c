/***************************************************************************************
**�ļ���:     HAL_Finger.c
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
#include "HAL_Finger.h"
#include "DRV_Finger.h"
#if HAL_FINGER_MODULE_EN
#warning HAL_FINGERģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_FINGER_REG_REPEAT_MAX (6) //ע��ʱ��ѹָ��ͷ�Ĵ���
/*************************.C�����ṹ����***********************/
typedef enum
{
    eOptionStatusNone,        //����
    eOptionStatusStart,       //��ʼ
    eOptionStatusPowerUp,     //�ϵ�
    eOptionStatusWaitPowerOk, //�ȴ��ϵ����
    eOptionStatusStandby,     //������
    eOptionStatusReg,         //ָ��ע��
    eOptionStatusSerach,      //ָ������
    eOptionStatusGenImg,      //¼ָ��ͼ��
    eOptionStatusImg2Tz,      //�׶�ͼ��ת����
    eOptionStatusRegModel,    //�׶κϲ�ģ��
    eOptionStatusStoreChar,   //�׶δ���ģ��
    eOptionStatusDel,         //ɾ��ָ��
} TYPEe_OptionStatus;         //ָ�ƹ���״̬
typedef struct
{
    TYPEe_OptionStatus optionStatus;
    uint8_t regSaveBuf; //�洢������
    uint8_t *pAck;      //�ظ����ݵ�ָ��

} TYPEs_FingerOption;
/*************************˽��ȫ�ֱ���*************************/
static TYPEs_FingerOption fingerOption =
    {
        .optionStatus = eOptionStatusNone,
};
/*************************����ȫ�ֱ���*************************/

/*************************�ֲ�����*****************************/

/*************************ȫ�ֺ���****************************/
/***************************************************************************************
**������:       HAL_Finger_Init
**��������:     ָ��ͷ��ʼ��
**�������:     --
**�������:     
int8_t
0 �ɹ�
-1 ʧ��
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_Init(void)
{
    fingerOption.pAck = DRV_Finger_GetAckData(); //��ȡ�������ݵĵ�ַ��
	return DRV_Finger_Init();
}

/***************************************************************************************
**������:       HAL_Finger_SaveBufSet
**��������:     ָ��ͷ������������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Finger_SaveBufSet(uint8_t dat)
{
    fingerOption.regSaveBuf = dat;
}
/***************************************************************************************
**������:       HAL_Finger_SaveBufGet
**��������:     ָ��ͷ���������ȡ
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
uint8_t HAL_Finger_SaveBufGet(void)
{
    return fingerOption.regSaveBuf;
}
/***************************************************************************************
**������:       HAL_Finger_RegStart
**��������:     ��ʼע��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_RegStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    fingerOption.regSaveBuf = 1;
    return 0;
}
/***************************************************************************************
**������:       HAL_Finger_RegReStart
**��������:     ����ע��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_RegReStart(void)
{
    fingerOption.optionStatus = eOptionStatusGenImg;
    return 0;
}
/***************************************************************************************
**������:       HAL_Finger_Reg
**��������:     ָ��ע������
**�������:     uint16_t model ע���ģ���ַ
**�������:     
int8_t
0,ִ���У��ȴ����
1����ʼ¼�����
2����⵽��Ч��ָ
3��ͼ��ת�����ɹ�
-3��ͼ��ת����ʧ�ܡ�
4��ת�����ϳ�
5�������ϳɳɹ�
-5������,�����ϳ�ʧ��
6������ɹ�
-6������,��������ʧ��
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_Reg(uint16_t model)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.regSaveBuf = 1; //����,�洢������
        fingerOption.optionStatus = eOptionStatusGenImg;
        DRV_Finger_ResetCmd();
        reTmp = 1; //��ʼ
        break;
    case eOptionStatusGenImg:
        switch (DRV_Finger_GenImg(model,0))
        {
        case 0: //������ȷ
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusImg2Tz;
                reTmp = 2; //��⵽��Ч��ָ
                DEBUG_LOG_OUT("Have Finger\n");
                break;
            default:
                //                DEBUG_LOG_OUT("No Finger\n");
                fingerOption.optionStatus = eOptionStatusGenImg; //����
                break;
            }
            break;
        case -2: //���ճ�ʱ
            DEBUG_LOG_OUT("GenImg Timer Over\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        case -3: //��������
            DEBUG_LOG_OUT("GenImg parm error\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        }
        break;
    case eOptionStatusImg2Tz:
        switch (DRV_Finger_Img2Tz(fingerOption.regSaveBuf))
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                DEBUG_LOG_OUT("Img2Tz OK\n");
                DEBUG_LOG_OUT("fingerOption.regSaveBuf is %d\n", fingerOption.regSaveBuf);
                fingerOption.regSaveBuf++;                               //׼��������һ��buffer
                if (fingerOption.regSaveBuf > HAL_FINGER_REG_REPEAT_MAX) //�ظ�����ָ��ͷ����
                {
                    fingerOption.optionStatus = eOptionStatusRegModel; //�ϲ�ģ��
                    reTmp = 4;                                         //ͼ�������ϳ�
                }
                else
                {
                    fingerOption.optionStatus = eOptionStatusNone; //��ʾ�û��󣬼�������
                    reTmp = 3;                                     //ͼ��ת�����ɹ���
                }
                break;
            default:
                reTmp = -3;                                      //ͼ��ת����ʧ�ܡ�
                fingerOption.optionStatus = eOptionStatusGenImg; //����
                break;
            }
            break;
        case -1: //������
            break;
        case -2:                                             //���ճ�ʱ
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        case -3:                                             //��������
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        }
        break;
    case eOptionStatusRegModel:
		DEBUG_LOG_OUT("eOptionStatusRegModel\n");
        switch (DRV_Finger_RegModel())
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusStoreChar;
                reTmp = 5; //�����ϳɳɹ�
                break;
            default:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = -5; //����,�����ϳ�ʧ��
                break;
            }
            break;
        case -1: //������
            break;
        case -2: //���ճ�ʱ
        case -3: //��������
        default:
			DEBUG_LOG_OUT("eOptionStatusRegModel -4\n");
            fingerOption.optionStatus = eOptionStatusNone;
            reTmp = -4; //����,�����ϳ�ʧ��
            break;
        }
        break;
    case eOptionStatusStoreChar:
        switch (DRV_Finger_Store(1, model))
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = 6; //����ɹ�
                break;
            default:
                fingerOption.optionStatus = eOptionStatusNone;
                reTmp = -6; //����,��������ʧ��
                break;
            }
            break;
        case -1: //������
            break;
        case -2: //���ճ�ʱ
        case -3: //��������
        default:
            fingerOption.optionStatus = eOptionStatusNone;
            reTmp = -5; //����,��������ʧ��
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Finger_VerifyStart
**��������:     ����ָ��У������
**�������:     --
**�������:     
int8_t
0,�ɹ�
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_VerifyStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}
/***************************************************************************************
**������:       HAL_Finger_Verify
**��������:     ָ��У������
**�������:     uint16_t id ��ȡ����ID��
**�������:     
int8_t
0,ִ���У��ȴ����
1����ʼ¼�����
2����⵽��Ч��ָ
3��ͼ��ת�����ɹ�
-3��ͼ��ת����ʧ�ܡ�
4��ָ�������ɹ�
-4��ָ������ʧ��
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_Verify(uint16_t *id)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusGenImg;
        DRV_Finger_ResetCmd();
        reTmp = 1; //��ʼ
        break;
    case eOptionStatusGenImg:
        switch (DRV_Finger_GenImg(*id,1))
        {
        case 0: //������ȷ
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                fingerOption.optionStatus = eOptionStatusImg2Tz;
                DRV_Finger_ResetCmd();
                reTmp = 2; //��⵽��Ч��ָ
                DEBUG_LOG_OUT("Have Finger\n");
                break;
            default:
                //				DEBUG_LOG_OUT("No Finger\n");
                fingerOption.optionStatus = eOptionStatusGenImg; //����
                break;
            }
            break;
        case -2: //���ճ�ʱ
            DEBUG_LOG_OUT("GenImg Timer Over\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        case -3: //��������
            DEBUG_LOG_OUT("GenImg parm error\n");
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        }
        break;
    case eOptionStatusImg2Tz:
        switch (DRV_Finger_Img2Tz(1)) //��������,��������01
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                DEBUG_LOG_OUT("Img2Tz OK\n");
                DEBUG_LOG_OUT("fingerOption.regSaveBuf is %d\n", fingerOption.regSaveBuf);
                fingerOption.optionStatus = eOptionStatusSerach; //����ָ��
                reTmp = 3;                                       //ͼ��ת�����ɹ���
                break;
            default:
                reTmp = -3;                                      //ͼ��ת����ʧ�ܡ�
                fingerOption.optionStatus = eOptionStatusGenImg; //����
                break;
            }
            break;
        case -1: //������
            break;
        case -2:                                             //���ճ�ʱ
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        case -3:                                             //��������
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg; //����
            break;
        }
        break;
    case eOptionStatusSerach:
        switch (DRV_Finger_Serach(1, 0, 100)) //
        {
        case 0:
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                *id = (uint16_t)((fingerOption.pAck[1] << 8) + fingerOption.pAck[2]); //��ȡָ��ID
                reTmp = 4;                                                            //ָ�������ɹ�
                fingerOption.optionStatus = eOptionStatusGenImg;
                break;
            default:
                reTmp = -4; //ָ������ʧ��
                fingerOption.optionStatus = eOptionStatusGenImg;
                break;
            }
            break;
        case -1: //������
            break;
        case -2: //���ճ�ʱ
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        case -3: //��������
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        default:
            fingerOption.optionStatus = eOptionStatusGenImg;
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Finger_DelStart
**��������:     ɾ��ָ�ƿ���
**�������:     --
**�������:     
int8_t
0,�ɹ�
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_DelStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}

/***************************************************************************************
**������:       HAL_Finger_Del
**��������:     ɾ��ָ��
**�������:     
uint16_t model			ģ���
uint16_t num 			ɾ������
**�������:     
int8_t
0,ִ���У��ȴ����
1�������ɹ�
-1����ʱ
-2����������
-3��ɾ��ʧ��
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_Del(uint16_t model, uint16_t num)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusDel;
        DRV_Finger_ResetCmd();
        break;
    case eOptionStatusDel:
        switch (DRV_Finger_DeletChar(model, num))
        {
        case 0: //�����ɹ�
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                reTmp = 1; //�����ɹ�
                break;
            default:
                reTmp = -3; //ɾ��ʧ��
                break;
            }
            //            reTmp = 1;
            break;
        case -2: //��ʱ
            reTmp = -1;
            break;
        case -3: //��������
            reTmp = -2;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Finger_EmptyStart
**��������:     ���ָ�ƿ���
**�������:     --
**�������:     
int8_t
0,�ɹ�
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_EmptyStart(void)
{
    fingerOption.optionStatus = eOptionStatusStart;
    return 0;
}

/***************************************************************************************
**������:       HAL_Finger_Empty
**��������:     ���ָ��
**�������:     --
**�������:     
int8_t
0,ִ���У��ȴ����
1�������ɹ�
-1����ʱ
-2����������
-3�����ʧ��
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_Empty(void)
{
    int8_t reTmp = 0;
    switch (fingerOption.optionStatus)
    {
    case eOptionStatusNone:
        break;
    case eOptionStatusStart:
        fingerOption.optionStatus = eOptionStatusDel;
        DRV_Finger_ResetCmd();
        break;
    case eOptionStatusDel:
        switch (DRV_Finger_Empty())
        {
        case 0: //�����ɹ�
            switch (fingerOption.pAck[0])
            {
            case DRV_FINGER_R_Ok:
                reTmp = 1; //�����ɹ�
                break;
            default:
                reTmp = -3; //���ʧ��
                break;
            }
            break;
        case -2: //��ʱ
            reTmp = -1;
            break;
        case -3: //��������
            reTmp = -2;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Finger_EnterSleepConfig
**��������:     ��������״̬
**�������:     --
**�������:     
**��ע:         
****************************************************************************************/
void HAL_Finger_EnterSleepConfig(void)
{
	DRV_Finger_EnterSleepConfig();
}
/***************************************************************************************
**������:       HAL_Finger_HavePress
**��������:     �а�����ѹ
**�������:     --
**�������:     
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_HavePress(void)
{
	if (1 == M_FING_IRQ_READIN())
	{
		return 0; //����ָ
	}
	else
	{
		return -1; //û����ָ
	}
}
/***************************************************************************************
**������:       HAL_Finger_ChkFingerLeave
**��������:     ���ָ���Ƿ��뿪
**�������:     --
**�������:     
int8_t
0;		//�����ɹ�
-1;		//������
-2;		//��ʱ
-3;		//��������
**��ע:         
****************************************************************************************/
int8_t HAL_Finger_ChkFingerLeave(void)
{
    return DRV_Finger_ChkFingerLeave();
}
#endif /*HAL_FINGER_MODULE_EN*/
/************************************Code End******************************************/
