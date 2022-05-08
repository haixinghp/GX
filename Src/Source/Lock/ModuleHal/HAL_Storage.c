/***************************************************************************************
**�ļ���:     HAL_Storage.c
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   
*�汾:V1.00     
*�޸���:hhx    
*ʱ��:
*�޸�˵����                    


**��ע:      
****************************************************************************************/
#include "HAL_Storage.h"
#include "DRV_Storage.h"
#if HAL_STORAGE_MODULE_EN
#warning HAL_STORAGEģ��ʹ��!
/*******************************�궨��***************************/
#define HAL_STORAGE_RW_BUF_LEN (150)    //��д�����С
#define HAL_STORAGE_DEFAULT_BYTE (0XFF) //�洢���ʵ�Ĭ��ֵ
#define HAL_STORAGE_DELAY_MS(X) (DelayMS(X)) //�ӳ�

/*************************.C�����ṹ����***********************/
typedef enum
{
    eFingerNumModAdd, //����
    eFingerNumModDel, //����
} TYPEe_StorageFingerNumMod;
/*************************˽��ȫ�ֱ���*************************/
static INT8U gRwBuf[HAL_STORAGE_RW_BUF_LEN] = {
    0,
};
/*************************����ȫ�ֱ���*************************/
TYPEs_StorageSave storageSave; //ע�⣡δ��ʼ��
/*************************�ֲ�����*****************************/
/***************************************************************************************
**������:       HAL_StorageRw
**��������:     �洢��д
**�������:     
TYPEe_StorageDire dir, ��д����
INT32U addr, ��д��ַ 
INT8U *data, ��д����
INT16U length ��д����
**�������:     
INT8S
0,ִ�гɹ�
-1,ͨѶʧ��
-2,��������

**��ע:         EEPROMд����Ҫʱ�䣬������ҪԤ��.
****************************************************************************************/
//static INT8S HAL_Storage_Rw(TYPEe_StorageDire dir, INT32U addr, INT8U *data, INT16U length)
INT8S HAL_Storage_Rw(TYPEe_StorageDire dir, INT32U addr, INT8U *data, INT16U length)
{
    if (NULL == data)
    {
        return -2; //��������
    }
    if (0 == length)
    {
        return -2; //��������
    }
    return DRV_Storage_ReadWrite((INT8U)(dir),
                                 addr,
                                 data,
                                 length);
}

/***************************************************************************************
**������:       HAL_Storage_GetAddressWithId
**��������:     ����ID�Ż�ȡ��ַ
**�������:     
INT16U id,//id��
INT32U StartAddress,//��ʼ��ַ
INT32U oneSize//��������ռ�õĴ�С
**�������:     
INT32U ��ʵ�������ַ��
**��ע:         
****************************************************************************************/
static INT32U HAL_Storage_GetAddressWithId(INT16U id, INT32U StartAddress, INT32U oneSize)
{
    return (StartAddress + id * oneSize);
}

/***************************************************************************************
**������:       HAL_Storage_Write16Bit
**��������:     д��16λ������
**�������:     
INT32U addr, ��ַ
INT16U dat Ҫд�������
**�������:     
INT8S 0,�ɹ�
**��ע:         
****************************************************************************************/
static INT8S HAL_Storage_Write16Bit(INT32U addr, INT16U dat)
{
    gRwBuf[0] = (INT8U)(dat >> 8);
    gRwBuf[1] = (INT8U)(dat);
    HAL_Storage_Rw(eStorageDireWrite,
                   addr,
                   gRwBuf,
                   2);
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_WriteOneReg
**��������:     д��һ��ָ����Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageFinger *msg ָ�ƿ��ƽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
�洢�ṹ���£�
ָ����ʼ	��ָ��ռ64�ֽ�	768+38*180=7608	60���û�180��ָ��				
768��0x300��	769	770	771	772	773	774	775
ָ����ЧW	��һ���H	��һ���L	��һ���H	��һ���L	�������˿���	���H	���L
776	777	778	779	780	781	782	783
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	��ǰ���H	��ǰ���L
784	785	786	787	788	789	790	791
�����־	����	����	����	ʱЧ��־	��ʼ��	��	��
792	793	794	795	796	797	798	799
ʱ	��	��	������	��	��	ʱ	��
800	801	802	803	804	805	806	807
��	��	�����־	SOSв�ȱ�־	У��H	У��L		
****************************************************************************************/
static INT8S HAL_Storage_WriteOneReg(INT32U addr, TYPEs_StorageFinger *msg)
{
    INT16U i = 0, j = 0;                                               //ѭ������
    INT16U chkSum = 0;                                                 //У���
    gRwBuf[HAL_STORAGE_VAILD_ADDR] = MSG_FINGERREGMEM_FIG;             //ָ�ƴ��ڱ�־.
    gRwBuf[HAL_STORAGE_UP_ID_ADDR] = (INT8U)(msg->up_pageID >> 8);     //��һ����Ÿ�
    gRwBuf[HAL_STORAGE_UP_ID_ADDR + 1] = (INT8U)(msg->up_pageID);      //��һ����ŵ�
    gRwBuf[HAL_STORAGE_DOWN_ID_ADDR] = (INT8U)(msg->down_pageID >> 8); //��һ����Ÿ�
    gRwBuf[HAL_STORAGE_DOWN_ID_ADDR + 1] = (INT8U)(msg->down_pageID);  //��һ����ŵ�
    gRwBuf[HAL_STORAGE_LIM_ADDR] = msg->Lim;                           //ָ������.
    gRwBuf[HAL_STORAGE_GROUP_ADDR] = (INT8U)(msg->group >> 8);         //����
    gRwBuf[HAL_STORAGE_GROUP_ADDR + 1] = (INT8U)(msg->group);          //����
    i = HAL_STORAGE_NAME_ADDR;
    for (j = 0; j < 6; j++)
    {
        gRwBuf[i++] = msg->name[j]; //�����û���
    }
    gRwBuf[HAL_STORAGE_PAGE_ID_ADDR] = (INT8U)(msg->pageID >> 8); //д�뵱ǰ��ַ��
    gRwBuf[HAL_STORAGE_PAGE_ID_ADDR + 1] = (INT8U)(msg->pageID);  //д�뵱ǰ��ַ��
#ifdef LOCK_FUNCTION_FINGERPWD_ON
    gRwBuf[HAL_STORAGE_PWD_FIG_ADDR] = msg->pwd.fig;
    gRwBuf[HAL_STORAGE_PWD_ADDR] = msg->pwd.pwd[0];
    gRwBuf[HAL_STORAGE_PWD_ADDR + 1] = msg->pwd.pwd[1];
    gRwBuf[HAL_STORAGE_PWD_ADDR + 2] = msg->pwd.pwd[2];
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON //ʱЧ
    gRwBuf[HAL_STORAGE_TM_VAILD_FIG_ADDR] = msg->TimeVaild.fig;
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR] = (INT8U)(msg->TimeVaild.str.year);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 1] = (INT8U)(msg->TimeVaild.str.mon);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 2] = (INT8U)(msg->TimeVaild.str.day);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 3] = (INT8U)(msg->TimeVaild.str.hour);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 4] = (INT8U)(msg->TimeVaild.str.min);
    gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 5] = (INT8U)(msg->TimeVaild.str.sec);

    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR] = (INT8U)(msg->TimeVaild.stp.year);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 1] = (INT8U)(msg->TimeVaild.stp.mon);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 2] = (INT8U)(msg->TimeVaild.stp.day);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 3] = (INT8U)(msg->TimeVaild.stp.hour);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 4] = (INT8U)(msg->TimeVaild.stp.min);
    gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 5] = (INT8U)(msg->TimeVaild.stp.sec);

    gRwBuf[HAL_STORAGE_TM_VAILD_WDAY_ADDR] = (INT8U)(msg->TimeVaild.wday);
#endif

#ifdef LOCK_FUNCTION_KINDRED_ON
    gRwBuf[HAL_STORAGE_KINDRED_ADDR] = msg->kindred;
#endif

#ifdef LOCK_FUNCTION_SOS_ON
    gRwBuf[HAL_STORAGE_SOS_ADDR] = msg->sos;
#endif
    for (j = 0; j < (HAL_STORAGE_ONE_FINGER_SIZE - HAL_STORAGE_CHKSUM_LEN); j++)
    {
        chkSum += gRwBuf[j];
    } //����У���
    gRwBuf[HAL_STORAGE_CHKSUM_ADDR] = (INT8U)(chkSum >> 8);
    gRwBuf[HAL_STORAGE_CHKSUM_ADDR + 1] = (INT8U)(chkSum); //д��У���

    HAL_Storage_Rw(eStorageDireWrite,
                   addr,
                   gRwBuf,
                   HAL_STORAGE_ONE_FINGER_SIZE); //д������
                                                 //	HAL_Storage_Rw(eStorageDireRead,
                                                 //                   addr,
                                                 //                   gRwBuf,
                                                 //                   HAL_STORAGE_ONE_FINGER_SIZE); //�������������Ƿ���ȷ
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ChkSum
**��������:     ����У���
**�������:  
INT8U *dat, //����
INT32U len  //���ݳ���
**�������:     
INT16U У���
**��ע: 
****************************************************************************************/
static INT16U HAL_Storage_ChkSum(INT8U *dat, INT32U len)
{
    INT32U i = 0;
    INT16U sumTmp = 0;
    for (i = 0; i < len; i++)
    {
        sumTmp += *dat;
        dat++;
    }
    return sumTmp;
}

/***************************************************************************************
**������:       HAL_Storage_FingerLeaveNumSet
**��������:     ָ��ʣ���������
**�������:     
TYPEe_StorageFingerNumMod Mod, �� ��
TYPEs_StorageNum *num ����
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
static INT8S HAL_Storage_FingerLeaveNumSet(TYPEe_StorageFingerNumMod Mod, TYPEs_StorageNum *num)
{
    if (eFingerNumModAdd == Mod)
    {
        num->leave += 1;
    }
    else
    {
        num->leave -= 1;
    }
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_LEAVE_ONLINEH, num->leave);
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_WriteOneUserReg
**��������:     д��һ���û���Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
�洢�ṹ���£�
��ЧW	��һ���H	��һ���L	��һ���H	��һ���L	�������˿���	���H	���L
							
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	��ǰ���H	��ǰ���L
							
�����־	����	����	����	ʱЧ��־	��ʼ��	��	��
							
ʱ	��	��	������	��	��	ʱ	��
							
��	��	�����־	SOSв�ȱ�־	ID	ID	ID	ID
							
ID	ID	ID	ID	ID	ID	ID	ID
							
ID	У��H	У��L					
		
****************************************************************************************/
static INT8S HAL_Storage_WriteOneUserReg(INT32U addr, TYPEs_StorageUserDat *msg)
{
    msg->checksum = HAL_Storage_ChkSum((INT8U *)msg, sizeof(TYPEs_StorageUserDat) - sizeof(INT16U)); //����У���
    return HAL_Storage_Rw(eStorageDireWrite,
                          addr,
                          (INT8U *)msg,
                          sizeof(TYPEs_StorageUserDat)); //д������
}
/***************************************************************************************
**������:       HAL_Storage_ReadOneUserReg
**��������:     ��ȡһ���û���Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 					
****************************************************************************************/
static INT8S HAL_Storage_ReadOneUserReg(INT32U addr, TYPEs_StorageUserDat *msg)
{
    return HAL_Storage_Rw(eStorageDireRead,
                          addr,
                          (INT8U *)msg,
                          sizeof(TYPEs_StorageUserDat));
}

/*************************ȫ�ֺ���****************************/

/***************************************************************************************
**������:       HAL_Storage_SearchEmptyFinger
**��������:     ���ҿ�λָ��ͷ
**�������:     --
**�������:     
INT16U ��ȡ���Ŀ���λ��,0XFFFFΪδ��ѯ��
**��ע:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFinger(void)
{
    return HAL_Storage_SearchEmpty(0,
                                   MEM_FINGER_ALL_ONLINE,
                                   MSG_FINGER_REG_MEM_ONLINE_ST,
                                   MSG_FINGER_ONE_SIZE);
}

/***************************************************************************************
**������:       HAL_Storage_GetOneFingerReg
**��������:     ��ȡһ��ָ����Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageFinger *msg ָ�ƿ��ƽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
�洢�ṹ���£�
ָ����ʼ	��ָ��ռ64�ֽ�	768+38*180=7608	60���û�180��ָ��				
768��0x300��	769	770	771	772	773	774	775
ָ����ЧW	��һ���H	��һ���L	��һ���H	��һ���L	�������˿���	���H	���L
776	777	778	779	780	781	782	783
name[i]	name[i]	name[i]	name[i]	name[i]	name[i]	��ǰ���H	��ǰ���L
784	785	786	787	788	789	790	791
�����־	����	����	����	ʱЧ��־	��ʼ��	��	��
792	793	794	795	796	797	798	799
ʱ	��	��	������	��	��	ʱ	��
800	801	802	803	804	805	806	807
��	��	�����־	SOSв�ȱ�־	У��H	У��L		
****************************************************************************************/
INT8S HAL_Storage_GetOneFingerReg(INT32U addr, TYPEs_StorageFinger *msg)
{
    INT16U i = 0, j = 0;
    HAL_Storage_Rw(eStorageDireRead,
                   addr,
                   gRwBuf,
                   HAL_STORAGE_ONE_FINGER_SIZE);                                                       //��ȡ����
    msg->vaild = gRwBuf[HAL_STORAGE_VAILD_ADDR];                                                       //ָ�ƴ��ڱ�־.
    msg->up_pageID = (gRwBuf[HAL_STORAGE_UP_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_UP_ID_ADDR + 1];       //��һ�����
    msg->down_pageID = (gRwBuf[HAL_STORAGE_DOWN_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_DOWN_ID_ADDR + 1]; //��һ�����
    msg->Lim = gRwBuf[HAL_STORAGE_LIM_ADDR];                                                           //ָ������.
    msg->group = (gRwBuf[HAL_STORAGE_GROUP_ADDR] << 8) + gRwBuf[HAL_STORAGE_GROUP_ADDR + 1];           //����
    i = HAL_STORAGE_NAME_ADDR;
    for (j = 0; j < 6; j++)
    {
        msg->name[j] = gRwBuf[i++]; //�����û���
    }
    msg->pageID = (gRwBuf[HAL_STORAGE_PAGE_ID_ADDR] << 8) + gRwBuf[HAL_STORAGE_PAGE_ID_ADDR + 1]; //д���ַ
#ifdef LOCK_FUNCTION_FINGERPWD_ON
    msg->pwd.fig = gRwBuf[HAL_STORAGE_PWD_FIG_ADDR];
    msg->pwd.pwd[0] = gRwBuf[HAL_STORAGE_PWD_ADDR];
    msg->pwd.pwd[1] = gRwBuf[HAL_STORAGE_PWD_ADDR + 1];
    msg->pwd.pwd[2] = gRwBuf[HAL_STORAGE_PWD_ADDR + 2];
#endif
#ifdef LOCK_FUNCTION_TIMEVAILD_ON //ʱЧ
    msg->TimeVaild.fig = gRwBuf[HAL_STORAGE_TM_VAILD_FIG_ADDR];
    msg->TimeVaild.str.year = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR];
    msg->TimeVaild.str.mon = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 1];
    msg->TimeVaild.str.day = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 2];
    msg->TimeVaild.str.hour = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 3];
    msg->TimeVaild.str.min = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 4];
    msg->TimeVaild.str.sec = gRwBuf[HAL_STORAGE_TM_VAILD_STAR_ADDR + 5];

    msg->TimeVaild.stp.year = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR];
    msg->TimeVaild.stp.mon = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 1];
    msg->TimeVaild.stp.day = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 2];
    msg->TimeVaild.stp.hour = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 3];
    msg->TimeVaild.stp.min = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 4];
    msg->TimeVaild.stp.sec = gRwBuf[HAL_STORAGE_TM_VAILD_END_ADDR + 5];

    msg->TimeVaild.wday = gRwBuf[HAL_STORAGE_TM_VAILD_WDAY_ADDR];
#endif

#ifdef LOCK_FUNCTION_KINDRED_ON
    msg->kindred = gRwBuf[HAL_STORAGE_KINDRED_ADDR];
#endif

#ifdef LOCK_FUNCTION_SOS_ON
    msg->sos = gRwBuf[HAL_STORAGE_SOS_ADDR];
#endif
    msg->checksum = (gRwBuf[HAL_STORAGE_CHKSUM_ADDR] << 8) + gRwBuf[HAL_STORAGE_CHKSUM_ADDR + 1]; //У���
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_SaveOneFingerReg
**��������:     ����һ��ָ������
**�������:     
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneFingerReg(TYPEs_StorageSave *msg)
{
    INT32U addrTmp = 0;
    HAL_Storage_FingerLeaveNumSet(eFingerNumModDel, &(msg->fingerNum)); //����ʣ������
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msg->fingerNum.overgroupid,
                                                                      MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                      MSG_FINGER_ONE_SIZE); //��ȡ��һ��ָ�Ƶ�down id��ַ
    HAL_Storage_Write16Bit(addrTmp, msg->finger.pageID);                                    //�޸���һ��ָ�Ƶ�down id
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE);                              //��ȡ�����ַ
    HAL_Storage_WriteOneReg(addrTmp, &(msg->finger));                                         //����һ��ָ����Ϣ
    msg->fingerNum.overpageid = msg->finger.pageID;                                           //ĩβpageID
    HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //д��ĩβpageID
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_DelOneFingerReg
**��������:     ɾ��һ��ָ������
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFingerReg(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE);  //��ȡ�����ַ
    HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //��ȡָ����Ϣ
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //�����Чֵ
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //д����Чֵ
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
                                                                      MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                      MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //�޸���һ��ָ�Ƶ���β
    if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //����Ϊ�յ����
    {
        msg->fingerNum.overpageid = msg->finger.pageID;                                           //ĩβpageID
        HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //д��ĩβpageID
    }
    else
    {
        addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
                                                                        MSG_FINGER_REG_MEM_ONLINE_ST,
                                                                        MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
        HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //�޸���һ��ָ�Ƶ���ͷ
    }
    HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //����ʣ������
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_ReadOneFingerReg
**��������:     ��ȡһ��ָ������
**�������:     
TYPEs_StorageFinger *msg ָ��ͷ�ṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ReadOneFingerReg(TYPEs_StorageFinger *msg)
{
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->pageID,
                                           MSG_FINGER_REG_MEM_ONLINE_ST,
                                           MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    HAL_Storage_GetOneFingerReg(addrTmp, msg);                   //��ȡָ����Ϣ
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ChkFingerNum
**��������:     ���ָ�ƿռ�
**�������:     
TYPEs_StorageSave *msg ָ��ͷ�洢�ṹ
**�������:     
INT8S 
0; //��ָ��
-1; //ָ�ƿ�����
1; //��ָ�ƣ����ǿ⻹û����
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ChkFingerNum(TYPEs_StorageSave *msg)
{
    if (msg->fingerNum.all == msg->fingerNum.leave)
    {
        return 0; //��ָ��
    }
    else if (0 == msg->fingerNum.leave)
    {
        return -1; //ָ�ƿ�����
    }
    else
    {
        return 1; //��ָ�ƣ����ǿ⻹û����
    }
}
/***************************************************************************************
**������:       HAL_Storage_ResetFingerStruct
**��������:     ���ָ�ƽṹ
**�������:     
TYPEs_StorageSave *msg ָ��ͷ�洢�ṹ
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ResetFingerStruct(TYPEs_StorageSave *msg)
{
    memset(&(msg->finger), 0x00, sizeof(msg->finger));
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_EraseAllFingerReg
**��������:     �������е�ָ������
**�������:     --
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_EraseAllFingerReg(void)
{
    INT16U i;
    INT32U addr = 0;
    memset(gRwBuf, 0, HAL_STORAGE_RW_BUF_LEN); //��ջ���
    for (i = 0; i < MEM_FINGER_ALL_ONLINE; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i,
                                            MSG_FINGER_REG_MEM_ONLINE_ST,
                                            MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
        HAL_Storage_Rw(eStorageDireWrite,
                       addr,
                       gRwBuf,
                       HAL_STORAGE_ONE_FINGER_SIZE); //д������
    }

    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_lockRegStatusGet
**��������:     ��ע��״̬��ȡ
**�������:     --
**�������:     
INT8S 
-1 ��������
0 δע��
1 ע����
2 ��ע��
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_lockRegStatusGet(TYPEs_SystemDat *dat)
{
    INT8S reTmp = 0;
    if (NULL == dat)
    {
        return -1; //��������
    }
    switch (dat->lockRegStatus)
    {
    case MSG_FACTMEM_REG_STATUS_NONE:
        reTmp = 0; //δע��
        break;
    case MSG_FACTMEM_REG_STATUS_ING:
        reTmp = 1; //ע����
        break;
    case MSG_FACTMEM_REG_STATUS_OK:
        reTmp = 2; //���ع���Ա��ע��
        break;
    default:
        break;
    }
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Storage_lockRegStatusSet
**��������:     ��ע��״̬����
**�������:     
INT8U status
0 δע��
1 ע����
2 ��ע��
**�������:     
INT8S 
0 �ɹ�
-1 ��������

**��ע: 
****************************************************************************************/
INT8S HAL_Storage_lockRegStatusSet(TYPEs_SystemDat *dat, INT8U status)
{
    if (NULL == dat)
    {
        return -1; //��������
    }
    switch (status)
    {
    case 0:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE;
        break;
    case 1:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_ING;
        break;
    case 2:
        dat->lockRegStatus = MSG_FACTMEM_REG_STATUS_OK;
        break;
    default:
        break;
    }
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_AdminFingerNumChk
**��������:     ����ָ���û�ָ���������
**�������:     --
**�������:     
INT8S 
1 �пռ�
-1 ��������
-2 �������
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_AdminFingerNumChk(TYPEs_SystemDat *dat)
{
    if (NULL == dat)
    {
        return -1; //��������
    }
    if (dat->adminFingerNum >= MEM_FINGER_ADMIN_ALL_LOCAL)
    {
        return -2; //�������
    }
    else
    {
        return 1; //�пռ�
    }
}

/***************************************************************************************
**������:       HAL_Storage_AdminFingerIdChk
**��������:     ����ָ�Ƽ��ID�Ƿ��ǹ���ָ��
**�������:     --
**�������:     
INT8S 
1 �пռ�
-1 ��������
-2 �������
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_AdminFingerIdChk(INT16U id)
{
    if (id < MEM_FINGER_ADMIN_ALL_LOCAL)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
/***************************************************************************************
**������:       HAL_Storage_SaveAdminFingerNum
**��������:     �������Աָ������
**�������:     
INT16U dat ����
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveAdminFingerNum(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_ADMIN_LOCALH,
                           dat);
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_GeneralFingerNumChk
**��������:     ��ָͨ���û�ָ���������
**�������:     --
**�������:     
INT8S 
0 ����Ϊ��
1 �пռ�
-1 ��������
-2 �������
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_GeneralFingerNumChk(TYPEs_SystemDat *dat)
{
    if (NULL == dat)
    {
        return -1; //��������
    }
    if (dat->generalFingerNum >= MEM_FINGER_GUEST_ALL_LOCAL)
    {
        return -2; //�������
    }
    else
    {
        if (dat->generalFingerNum == 0)
        {
            return 0; //����Ϊ��
        }
        else
        {
            return 1; //�пռ�
        }
    }
}
/***************************************************************************************
**������:       HAL_Storage_SaveGeneralFingerNum
**��������:     ��ָͨ������
**�������:     
INT16U dat ����
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveGeneralFingerNum(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_FINGER_GUEST_LOCALH,
                           dat);
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_SaveLockRegStatus
**��������:     ����ϵͳע���־
**�������:     --
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveLockRegStatus(INT16U dat)
{
    HAL_Storage_Write16Bit(MEM_FACTMEM_ADMIN_REGISTER,
                           dat);
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_SystemDataInit
**��������:     ϵͳ���ݳ�ʼ��
**�������:     
TYPEs_StorageSave *pSave, ���ݴ洢�ṹ
TYPEs_SystemDat *pDat ϵͳ����
**�������:     
INT8S 
0 �ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SystemDataInit(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat)
{
    gRwBuf[MEM_FACTMEM_FACTORY_FIG] = MSG_FACTMEM_FACTORY_FIG_OK;                            //д��ɹ���־
    gRwBuf[MEM_FACTMEM_LOCKMODE] = (MSG_FACTMEM_LOCKMODE_FINGER | MSG_FACTMEM_LOCKMODE_BLE); //����ģʽ��ָ��/��������/����Կ�ף�
    gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEH] = MSG_FACTMEM_FINGER_ALL_ONLINEH;                 //ȫ��ָ��H��60*3��
    gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEL] = MSG_FACTMEM_FINGER_ALL_ONLINEL;                 //ȫ��ָ��L��60*3��
    gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEH] = MSG_FACTMEM_FINGER_LEAVE_ONLINEH;             //ʣ�����ָ��H��60*3��
    gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEL] = MSG_FACTMEM_FINGER_LEAVE_ONLINEL;             //ʣ�����ָ��L��60*3��
    gRwBuf[MEM_FACTMEM_PHONE_ALLH] = MSG_FACTMEM_PHONE_ALLH;                                 //ALL��H
    gRwBuf[MEM_FACTMEM_PHONE_ALLL] = MSG_FACTMEM_PHONE_ALLL;                                 //ALL��L
    gRwBuf[MEM_FACTMEM_PHONE_LEAVEH] = MSG_FACTMEM_PHONE_LEAVEH;                             //LEAVE��H
    gRwBuf[MEM_FACTMEM_PHONE_LEAVEL] = MSG_FACTMEM_PHONE_LEAVEL;                             //LEAVE��L
    gRwBuf[MEM_FACTMEM_SMARTKEY_ALLH] = MSG_FACTMEM_SMARTKEY_ALLH;                           //ȫ������Կ������H ��20����
    gRwBuf[MEM_FACTMEM_SMARTKEY_ALLL] = MSG_FACTMEM_SMARTKEY_ALLL;                           //ȫ������Կ������L  ��20����
    gRwBuf[MEM_FACTMEM_SMARTKEY_LEAVEH] = MSG_FACTMEM_SMARTKEY_LEAVEH;                       //ʣ������Կ������H
    gRwBuf[MEM_FACTMEM_SMARTKEY_LEAVEL] = MSG_FACTMEM_SMARTKEY_LEAVEL;                       //ʣ������Կ������L
    gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALH] = 0;                                             //���ع���Աָ������H
    gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALL] = 0;                                             //���ع���Աָ������L
    gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALH] = 0;                                             //�����û�ָ������H
    gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALL] = 0;                                             //�����û�ָ������L
    gRwBuf[MEM_FACTMEM_WIFI_MAIN_SW] = 0;                                                    //WIFI������
    gRwBuf[MEM_FACTMEM_WIFI_LOGUP_SW] = 0;                                                   //WIFI�����ź��ϴ�����
    gRwBuf[MEM_FACTMEM_ADMIN_REGISTER] = (MSG_FACTMEM_REG_STATUS_NONE >> 8) & 0xff;          //���ع���Ա�Ƿ�ע��H
    gRwBuf[MEM_FACTMEM_ADMIN_REGISTER + 1] = (MSG_FACTMEM_REG_STATUS_NONE >> 0) & 0xff;      //���ع���Ա�Ƿ�ע��L
    gRwBuf[MEM_FACTMEM_VOICE] = 0x55;                                                        //��������
    gRwBuf[MEM_DH_G] = 0;                                                                    //dhԭ��dh_key.g_tmp;
    gRwBuf[MEM_DH_P] = 0;                                                                    //DH����dh_key.p_tmp;
    gRwBuf[MEM_FACTMEM_DONE_FIG] = MSG_FACTMEM_FACTORY_FIG_OK;                               //������־  G      ��β��־
    gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH] = MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEH;   //���ָ��ID  H��ע�����ɾ�����ã�����û�
    gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL] = MSG_FACTMEM_REGMSGOVER_PAGEID_ONLINEL;   //���ָ��ID  L��ע�����ɾ�����ã�
    gRwBuf[MEM_FACTMEM_SMARTKEYOVER_PAGEIDH] = MSG_FACTMEM_SMARTKEYOVER_PAGEIDH;             //���һ������Կ�״洢λ��H��ע�����ɾ����
    gRwBuf[MEM_FACTMEM_SMARTKEYOVER_PAGEIDL] = MSG_FACTMEM_SMARTKEYOVER_PAGEIDL;             //���һ������Կ�״洢λ��L��ע�����ɾ����
    gRwBuf[MEM_FACTMEM_CARDOVER_PAGEIDH] = MSG_FACTMEM_PHONEOVER_PAGEIDH;                    //��λ��ID  H��ע�����ɾ�����ã�
    gRwBuf[MEM_FACTMEM_CARDOVER_PAGEIDL] = MSG_FACTMEM_PHONEOVER_PAGEIDL;                    //��λ��ID L��ע�����ɾ�����ã�
    gRwBuf[MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALH] = 0x00;                                    //���һ�����H��ע�����ɾ�����ã������,
    gRwBuf[MEM_FACTMEM_REGMSGOVER_GROUPID_LOCALL] = 0x00;                                    //���һ�����L��ע�����ɾ�����ã������,
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_ST] = MSG_FACTMEM_LOCKLOG_LINK_ST;                       //������¼4�ֽ�
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_2] = MSG_FACTMEM_LOCKLOG_LINK_2;                         //������¼4�ֽ�
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_3] = MSG_FACTMEM_LOCKLOG_LINK_3;                         //������¼4�ֽ�
    gRwBuf[MEM_FACTMEM_LOCKLOG_LINK_OV] = MSG_FACTMEM_LOCKLOG_LINK_OV;                       //������¼4�ֽ�
    gRwBuf[MEM_FACTMEM_LOCKLOG_SN_H] = MSG_FACTMEM_LOCKLOG_SN_H;                             //������¼���к�H
    gRwBuf[MEM_FACTMEM_LOCKLOG_SN_L] = MSG_FACTMEM_LOCKLOG_SN_L;                             //������¼���к�L
    gRwBuf[MEM_FACTMEM_ADMIN_PWD1] = MSG_FACTMEM_ADMIN_PWD1;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD2] = MSG_FACTMEM_ADMIN_PWD2;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD3] = MSG_FACTMEM_ADMIN_PWD3;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD4] = MSG_FACTMEM_ADMIN_PWD4;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD5] = MSG_FACTMEM_ADMIN_PWD5;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD6] = MSG_FACTMEM_ADMIN_PWD6;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD7] = MSG_FACTMEM_ADMIN_PWD7;                                 //����
    gRwBuf[MEM_FACTMEM_ADMIN_PWD8] = MSG_FACTMEM_ADMIN_PWD8;                                 //����
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR,
                   gRwBuf,
                   MEM_FACT_MEM_SIZE); //��������

    // pwd.admin.cur[0] = gRwBuf[MEM_FACTMEM_ADMIN_PWD1];
    // pwd.admin.cur[1] = gRwBuf[MEM_FACTMEM_ADMIN_PWD2];
    // pwd.admin.cur[2] = gRwBuf[MEM_FACTMEM_ADMIN_PWD3];
    // pwd.admin.cur[3] = gRwBuf[MEM_FACTMEM_ADMIN_PWD4];
    // pwd.admin.cur[4] = gRwBuf[MEM_FACTMEM_ADMIN_PWD5];
    // pwd.admin.cur[5] = gRwBuf[MEM_FACTMEM_ADMIN_PWD6];
    // pwd.admin.cur[6] = gRwBuf[MEM_FACTMEM_ADMIN_PWD7];
    // pwd.admin.cur[7] = gRwBuf[MEM_FACTMEM_ADMIN_PWD8];

    pSave->fingerNum.all = ((MSG_FACTMEM_FINGER_ALL_ONLINEH << 8) | MSG_FACTMEM_FINGER_ALL_ONLINEL);       //ָ������
    pSave->fingerNum.leave = ((MSG_FACTMEM_FINGER_LEAVE_ONLINEH << 8) | MSG_FACTMEM_FINGER_LEAVE_ONLINEL); //����ָ��
    pSave->fingerNum.overpageid = ((MSG_FACTMEM_PHONEOVER_PAGEIDH << 8) | MSG_FACTMEM_PHONEOVER_PAGEIDL);  //���һöָ�Ƶ�ID

    //    pDat->adminFingerNum = 0;                          //��տͻ�ָ������
    //    pDat->generalFingerNum = 0;                        //�����ָͨ������
    //    pDat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE; //��ע��

    HAL_Storage_DelAllFingerLocal(pSave, pDat);        //������еı���ָ��
    pDat->lockRegStatus = MSG_FACTMEM_REG_STATUS_NONE; //��ע��
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_SystemDataRead
**��������:     ϵͳ���ݶ�ȡ
**�������:     --
**�������:     
INT8S 
0 �ɹ�
-1 ͨѶʧ��
-2 ��������
-3 ��ȡ����
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SystemDataRead(TYPEs_StorageSave *pSave, TYPEs_SystemDat *pDat)
{
    if (NULL == pSave)
    {
        return -2; //��������
    }
    if (NULL == pDat)
    {
        return -2; //��������
    }
    if (-1 == HAL_Storage_Rw(eStorageDireRead,
                             MEM_FACTMEM_START_ADDR,
                             gRwBuf,
                             MEM_FACT_MEM_SIZE))
    {
        return -1; //ͨѶʧ��
    }
    if ((MSG_FACTMEM_FACTORY_FIG_OK == gRwBuf[MEM_FACTMEM_FACTORY_FIG]) && (MSG_FACTMEM_FACTORY_FIG_OK == gRwBuf[MEM_FACTMEM_DONE_FIG]))
    {

        pSave->fingerNum.all = gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_ALL_ONLINEL];                      //ȫ��ָ��
        pSave->fingerNum.leave = gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_LEAVE_ONLINEL];                //ʣ�����ָ��
        pSave->fingerNum.overpageid = gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH] << 8 | gRwBuf[MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEL]; //���ָ��ID
        pDat->lockRegStatus = gRwBuf[MEM_FACTMEM_ADMIN_REGISTER] << 8 | gRwBuf[MEM_FACTMEM_ADMIN_REGISTER + 1];                           //���ع���Աע��״̬
        pDat->adminFingerNum = gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_ADMIN_LOCALL];                    //���ع���Աָ������
        pDat->generalFingerNum = gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALH] << 8 | gRwBuf[MEM_FACTMEM_FINGER_GUEST_LOCALL];                  //�����û�ָ������
        return 0;                                                                                                                         //�ɹ�                                                                                                                        //�ɹ�
    }
    else
    {
        return -3; //��ȡ����
    }
}
/***************************************************************************************
**������:       HAL_Storage_SearchEmptyFingerLocalAdmin
**��������:     ���ҿ�λָ��ͷ���ع���Ա
**�������:     --
**�������:     
INT16U ��ȡ���Ŀ���λ��,0XFFFFΪδ��ѯ��
**��ע:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFingerLocalAdmin(void)
{
    return HAL_Storage_SearchEmpty(0,
                                   MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MSG_FINGER_REG_MEM_LOCAL_ST,
                                   MSG_FINGER_ONE_SIZE);
}
/***************************************************************************************
**������:       HAL_Storage_SearchEmptyFingerLocalGeneral
**��������:     ���ҿ�λָ��ͷ������ͨ�û�
**�������:     --
**�������:     
INT16U ��ȡ���Ŀ���λ��,0XFFFFΪδ��ѯ��
**��ע:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmptyFingerLocalGeneral(void)
{
    return HAL_Storage_SearchEmpty(MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MEM_FINGER_GUEST_ALL_LOCAL + MEM_FINGER_ADMIN_ALL_LOCAL,
                                   MSG_FINGER_REG_MEM_LOCAL_ST,
                                   MSG_FINGER_ONE_SIZE);
}
/***************************************************************************************
**������:       HAL_Storage_SaveOneFingerRegLocal
**��������:     ����һ��ָ�����ݱ���
**�������:     
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneFingerRegLocal(TYPEs_StorageSave *msg)
{
    INT32U addrTmp = 0;
    //    HAL_Storage_FingerLeaveNumSet(eFingerNumModDel, &(msg->fingerNum)); //����ʣ������
    addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msg->fingerNum.overgroupid,
                                                                      MSG_FINGER_REG_MEM_LOCAL_ST,
                                                                      MSG_FINGER_ONE_SIZE); //��ȡ��һ��ָ�Ƶ�down id��ַ
    HAL_Storage_Write16Bit(addrTmp, msg->finger.pageID);                                    //�޸���һ��ָ�Ƶ�down id
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_LOCAL_ST,
                                           MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    HAL_Storage_WriteOneReg(addrTmp, &(msg->finger));            //����һ��ָ����Ϣ
    msg->fingerNum.overpageid = msg->finger.pageID;              //ĩβpageID

    //   HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //д��ĩβpageID
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_DelOneFingerRegLocal
**��������:     ɾ��һ��ָ�����ݱ���
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFingerRegLocal(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    addrTmp = HAL_Storage_GetAddressWithId(msg->finger.pageID,
                                           MSG_FINGER_REG_MEM_LOCAL_ST,
                                           MSG_FINGER_ONE_SIZE);  //��ȡ�����ַ
    HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //��ȡָ����Ϣ
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //�����Чֵ
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //д����Чֵ
    return 0;
    // addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
    //                                                                   MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                   MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    // HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //�޸���һ��ָ�Ƶ���β
    // if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //����Ϊ�յ����
    // {
    //     msg->fingerNum.overpageid = msg->finger.pageID;                                           //ĩβpageID
    //     HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //д��ĩβpageID
    // }
    // else
    // {
    //     addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
    //                                                                     MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                     MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    //     HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //�޸���һ��ָ�Ƶ���ͷ
    // }
    // HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //����ʣ������
}
/***************************************************************************************
**������:       HAL_Storage_DelAllFingerLocalGeneral
**��������:     ɾ�����еı�����ָͨ��
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocalGeneral(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    INT16U i;
    for (i = MEM_FINGER_ADMIN_ALL_LOCAL;
         i < (MEM_FINGER_ADMIN_ALL_LOCAL + MEM_FINGER_GUEST_ALL_LOCAL);
         i++)
    {
        msg->finger.pageID = i;
        HAL_Storage_DelOneFingerRegLocal(msg);
    }
    pDat->generalFingerNum = 0;
    HAL_Storage_SaveGeneralFingerNum(pDat->generalFingerNum); //������ָͨ������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_DelAllFingerLocalAdmin
**��������:     ɾ�����еı��ع���ָ��
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocalAdmin(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    INT16U i;
    for (i = MEM_FINGER_ADMIN_START_ID;
         i < (MEM_FINGER_ADMIN_START_ID + MEM_FINGER_ADMIN_ALL_LOCAL);
         i++)
    {
        msg->finger.pageID = i;
        HAL_Storage_DelOneFingerRegLocal(msg);
    }
    pDat->adminFingerNum = 0;
    HAL_Storage_SaveGeneralFingerNum(pDat->adminFingerNum); //������ָͨ������
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_DelAllFingerLocal
**��������:     ɾ�����еı���ָ��
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
TYPEs_SystemDat *pDat ϵͳ�ò���
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelAllFingerLocal(TYPEs_StorageSave *msg, TYPEs_SystemDat *pDat)
{
    HAL_Storage_DelAllFingerLocalAdmin(msg, pDat);   //������й���ָ��
    HAL_Storage_DelAllFingerLocalGeneral(msg, pDat); //���������ָͨ��
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_SavePwd
**��������:     �����û�����
**�������:    
const INT8U *dat, ����
**�������:     
INT8S 0,�ɹ�
-1; //��������
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SavePwd(const INT8U *dat)
{
    if (NULL == dat)
    {
        return -1; //��������
    }
    memcpy(gRwBuf, dat, MEM_BOARDPWD_PWDLEN); //��������
    HAL_Storage_Rw(eStorageDireWrite,
                   (MEM_BOARDPWD + MEM_BOARDPWD_PWDLEN),
                   gRwBuf,
                   MEM_BOARDPWD_PWDLEN);
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_ReadPwd
**��������:     �����û�����
**�������:    
const INT8U *dat, ����
**�������:     
INT8S 0,�ɹ�
-1; //��������
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ReadPwd(INT8U *dat)
{
    if (NULL == dat)
    {
        return -1; //��������
    }
    HAL_Storage_Rw(eStorageDireRead,
                   (MEM_BOARDPWD + MEM_BOARDPWD_PWDLEN),
                   gRwBuf,
                   MEM_BOARDPWD_PWDLEN);
    memcpy(dat, gRwBuf, MEM_BOARDPWD_PWDLEN); //��������
    return 0;
}


/***************************************************************************************
**������:       HAL_Storage_DelOneFingerReg
**��������:     ɾ��һ��ָ������
**�������:    
TYPEs_StorageSave *msg ָ��ͷ������ƽṹ
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelOneFaceReg(TYPEs_StorageSave *msg)
{
    TYPEs_StorageFinger msgTmp;
    INT32U addrTmp = 0;
    if (NULL == msg)
    {
        return -1; //��������
    }
    if (msg->sysDat.leaveNumFaceApp >= MEM_FACE_ALL) //û�п���ʣ��
    {
        return -2; //�û�����Ϊ��
    }
    msg->sysDat.leaveNumFaceApp++;
    HAL_Storage_WriteSysData(&msg->sysDat); //��������
    addrTmp = HAL_Storage_GetAddressWithId(msg->UserDat.pageID,
                                           MSG_FACE_REG_MEM_ST,
                                           MSG_FACE_REG_SIZE);    //��ȡ�����ַ
                                                                  //   HAL_Storage_GetOneFingerReg(addrTmp, &msgTmp);                //��ȡָ����Ϣ
    msgTmp.vaild = MSG_FACTMEM_REG_VAILD_NULL;                    //�����Чֵ
    HAL_Storage_Rw(eStorageDireWrite, addrTmp, &msgTmp.vaild, 1); //д����Чֵ

    // addrTmp = HAL_STORAGE_DOWN_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.pageID,
    //                                                                   MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                   MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    // HAL_Storage_Write16Bit(addrTmp, msgTmp.down_pageID);                                    //�޸���һ��ָ�Ƶ���β
    // if (MSG_FACTMEM_REG_PAGEID_NULL == msgTmp.down_pageID)                                  //����Ϊ�յ����
    // {
    //     msg->fingerNum.overpageid = msg->finger.pageID;                                           //ĩβpageID
    //     HAL_Storage_Write16Bit(MEM_FACTMEM_REGMSGOVER_PAGEID_ONLINEH, msg->fingerNum.overpageid); //д��ĩβpageID
    // }
    // else
    // {
    //     addrTmp = HAL_STORAGE_UP_ID_ADDR + HAL_Storage_GetAddressWithId(msgTmp.down_pageID,
    //                                                                     MSG_FINGER_REG_MEM_ONLINE_ST,
    //                                                                     MSG_FINGER_ONE_SIZE); //��ȡ�����ַ
    //     HAL_Storage_Write16Bit(addrTmp, msgTmp.up_pageID);                                    //�޸���һ��ָ�Ƶ���ͷ
    // }
    // HAL_Storage_FingerLeaveNumSet(eFingerNumModAdd, &(msg->fingerNum)); //����ʣ������
    return 0;
}

/***************************************************************************************
**������:       HAL_Storage_WriteSysData
**��������:     д���û�����
**�������:     
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_WriteSysData(TYPEs_StorageSysData *msg)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR,
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageSysData)); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ReadSysData
**��������:     ��ȡ�û�����
**�������:     
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ReadSysData(TYPEs_StorageSysData *msg)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR,
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageSysData)); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ChkSysData
**��������:     ����û�����
**�������:     --
**�������:     
INT8S 
0,�ɹ�
-1; //ʧ��
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ChkSysData(void)
{
    #if 0
    memset((uint8_t *)&storageSave.sysDat, 0XFF, sizeof(storageSave.sysDat));
     return -1; //ʧ�� 
    #endif
    memset((uint8_t *)&storageSave.sysDat, 0, sizeof(storageSave.sysDat));
    HAL_Storage_ReadSysData(&storageSave.sysDat); //��ȡϵͳ����
    if ((MSG_FACTMEM_FACTORY_FIG_OK == storageSave.sysDat.factoryFlag) &&
        (MSG_FACTMEM_FACTORY_FIG_OK == storageSave.sysDat.factoryFlag2))
    {
        return 0; //�ɹ�
    }
    else
    {
        return -1; //ʧ��
    }
}
/***************************************************************************************
**������:       HAL_Storage_ClearSysData
**��������:     ����û�����
**�������:     --
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ClearSysData(void)
{
    memset((uint8_t *)&storageSave.sysDat, 0, sizeof(storageSave.sysDat));            //���ϵͳ����
    memset((uint8_t *)&storageSave.sysDat.pwd, 0x0, sizeof(storageSave.sysDat.pwd)); //����Ĭ������
	storageSave.sysDat.factoryFlag = MSG_FACTMEM_FACTORY_FIG_OK;
	storageSave.sysDat.factoryFlag2 = MSG_FACTMEM_FACTORY_FIG_OK;
	storageSave.sysDat.allFinger = MEM_FINGER_ALL_ONLINE;
	storageSave.sysDat.leaveFinger = MEM_FINGER_ALL_ONLINE;
	storageSave.sysDat.allPhone = MEM_PHONE_ALL; 
	storageSave.sysDat.leavePhone = MEM_PHONE_ALL; 
	storageSave.sysDat.allSmartKey = MEM_SMARTKEY_ALL;
	storageSave.sysDat.leaveSmartKey = MEM_SMARTKEY_ALL;
	storageSave.sysDat.localAdminRegFlag = MSG_FACTMEM_REG_STATUS_NONE;
	storageSave.sysDat.voiceSw = MSG_FACTMEM_VOICE_OPEN;
	storageSave.sysDat.openLog = MSG_FACTMEM_LOCKLOG_ALL;
	storageSave.sysDat.openLogSn = MSG_FACTMEM_LOCKLOG_SN;
	storageSave.sysDat.leaveNumFaceApp = MEM_FACE_ALL_ONLINE;
	storageSave.sysDat.irDef = 0; //���������Ĳ���
	#ifndef LOCK_OPEN_PARM_NO_CLEAR
	storageSave.sysDat.torque[0]=MOTOR_TORQUE_LOW;
	#endif
	HAL_Storage_WriteSysData(&storageSave.sysDat);                                    //����ϵͳ����
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_SaveOneUserReg
**��������:     ����һ���û���Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_SaveOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg)
{
    INT8S reTmp = 0;
    INT32U addr;
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_WriteOneUserReg(addr, msg);
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Storage_GetOneUserReg
**��������:     ��ȡһ���û���Ϣ
**�������:     
INT32U addr, ��ַ
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_GetOneUserReg(INT32U StartAddress, INT32U oneSize, INT16U ID, TYPEs_StorageUserDat *msg)
{
    INT8S reTmp = 0;
    INT32U addr;
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_ReadOneUserReg(addr, msg);
    return reTmp;
}

/***************************************************************************************
**������:       HAL_Storage_SearchEmpty
**��������:     ���ҿ�λ
**�������:     
INT16U StartId, //��ʼ��ַ
INT16U endId //������ַ
**�������:     
INT16U ��ȡ���Ŀ���λ��,0XFFFFΪδ��ѯ��
**��ע:         
****************************************************************************************/
INT16U HAL_Storage_SearchEmpty(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize)
{
    INT16U i;
    for (i = StartId; i < endId; i++)
    {
        HAL_Storage_Rw(eStorageDireRead,
                       HAL_Storage_GetAddressWithId(i, StartAddress, oneSize),
                       gRwBuf,
                       1);
        if (MSG_FINGERREGMEM_FIG != gRwBuf[0]) //�����б�־
        {
            return i;
        }
    }
    return 0XFFFF;
}
/***************************************************************************************
**������:       HAL_Storage_MatchId
**��������:     ƥ���û�
**�������:     
INT16U StartId, ��ʼID
INT16U endId,   ������ַ
INT32U StartAddress, ��ʼ�����ַ
INT32U oneSize, �������ݵ�ռ�ÿռ��С
TYPEs_StorageUserDat *msg //�û�����,ע����������Ҫ����pageID����ƥ������
**�������:     
INT8S 
0; //ƥ��ɹ�
-1; //ƥ��ʧ��
**��ע:         
****************************************************************************************/
INT16S HAL_Storage_MatchId(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize, TYPEs_StorageUserDat *msg,TYPEe_MatchType matchType)
{
    INT16S i,j;
    INT32U addr;
    TYPEs_StorageUserDat msgTmp;
    for (i = StartId; i < endId; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i, StartAddress, oneSize);
        HAL_Storage_Rw(eStorageDireRead,
                       addr,
                       gRwBuf,
                       1);
        if (MSG_FINGERREGMEM_FIG == gRwBuf[0]) //�����б�־
        {
            HAL_Storage_ReadOneUserReg(addr, &msgTmp);
			switch (matchType)
            {
            	case eMatchTypeGroup:
					if (msgTmp.group == msg->group)
					{
						memcpy(msg, &msgTmp, sizeof(msgTmp)); //��������
						return i;                             //ƥ��ɹ�
					}
            		break;
				case eMatchTypeUserID:
					for(j=0;j<sizeof(msgTmp.UserID);j++)
                    {
						if(msgTmp.UserID[j] != msg->UserID[j])
						{
							break;
						}
                    }
					if(sizeof(msgTmp.UserID)  == j)
					{
						memcpy(msg, &msgTmp, sizeof(msgTmp)); //��������
						return i;
					}
            		break;	
            	default:
            		break;
            }
            
        }
    }
    return -1; //ƥ��ʧ��
}

/***************************************************************************************
**������:       HAL_Storage_DelOneUserReg
**��������:     ����һ���û���Ϣ
**�������:     
INT16U ID, �û�ID
INT32U StartAddress, ����ʼ��ַ  
INT32U oneSize ��������ռ�õĿռ�
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_DelOneUserReg(INT16U ID, INT32U StartAddress, INT32U oneSize)
{
    INT8S reTmp = 0;
    INT32U addr;
    TYPEs_StorageUserDat msg;
    memset(&msg, 0, sizeof(msg)); //�������
    addr = HAL_Storage_GetAddressWithId(ID, StartAddress, oneSize);
    reTmp = HAL_Storage_WriteOneUserReg(addr, &msg);
    return reTmp;
}
/***************************************************************************************
**������:       HAL_Storage_DelNUserReg
**��������:     ɾ��N������
**�������:     
INT16U StartId, ��ʼID
INT16U endId,   ������ַ
INT32U StartAddress, ��ʼ�����ַ
INT32U oneSize, �������ݵ�ռ�ÿռ��С
**�������:     
INT8S 
0; //�ɹ�
**��ע:         
****************************************************************************************/
INT8S HAL_Storage_DelNUserReg(INT16U StartId, INT16U endId, INT32U StartAddress, INT32U oneSize)
{
    INT32U addr;
    INT16U i = 0;
    TYPEs_StorageUserDat msg;
    memset(&msg, 0, sizeof(msg)); //�������
    for (i = StartId; i < endId; i++)
    {
        addr = HAL_Storage_GetAddressWithId(i, StartAddress, oneSize);
        HAL_Storage_WriteOneUserReg(addr, &msg);
    }
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_EnterSleepConfig
**��������:     ��������
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Storage_EnterSleepConfig(void)
{
    DRV_Storage_EnterSleepConfig();
}

/***************************************************************************************
**������:       HAL_Storage_WriteSysData
**��������:     д���û�����
**�������:     
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_WriteHardwareData(TYPEs_StorageHardwareData *msg)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MEM_FACTMEM_START_ADDR+sizeof(TYPEs_StorageSysData),
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageHardwareData)); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ReadSysData
**��������:     ��ȡ�û�����
**�������:     
TYPEs_StorageUserDat *msg ���ݽṹ��
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ReadHardwareData(TYPEs_StorageHardwareData *msg)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR+sizeof(TYPEs_StorageSysData),
                   (INT8U *)msg,
                   sizeof(TYPEs_StorageHardwareData)); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_WriteLockLog
**��������:     д���¼���¼
**�������:     
uint8_t *msg,
uint32_t id
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_WriteLockLog(uint8_t *msg,uint32_t id)
{
    HAL_Storage_Rw(eStorageDireWrite,
                   MSG_FACTMEM_LOCKLOG_LINK+id*MSG_LOCKLOGMEM_ONCE,
                   (INT8U *)msg,
                   8); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ReadLockLog
**��������:     ��ȡ�¼���¼
**�������:     
uint8_t *msg,
uint32_t id
**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ReadLockLog(uint8_t *msg,uint32_t id)
{
    HAL_Storage_Rw(eStorageDireRead,
                   MSG_FACTMEM_LOCKLOG_LINK+id*MSG_LOCKLOGMEM_ONCE,
                   (INT8U *)msg,
                   8); //д������
    return 0;
}
/***************************************************************************************
**������:       HAL_Storage_ClearUserData
**��������:     ������е��û�����
**�������:     

**�������:     
INT8S 0,�ɹ�
**��ע: 
****************************************************************************************/
INT8S HAL_Storage_ClearUserData(void)
{
	INT32U i;
	uint8_t test1=0xaa;
	uint32_t addr ;
	HAL_Storage_Rw(eStorageDireRead,
                   MEM_FACTMEM_START_ADDR,
                   &test1,
                   1); //���ﲻ֪��Ϊɶ��Ҫ�ȶ�ȡһ�£����ܲ�������������Ժ��ڲ顣
	for(i=0;i<5;i++)
   {
		DRV_Storage_EraseSector((MEM_FACTMEM_START_ADDR/4096)+i);
    }
	DEBUG_LOG_OUT("HAL_Storage_ClearUserData is:%d\n",test1);
	return 0;
}
/***************************************************************************************
**������:       HAL_Storage_Init
**��������:     ��ʼ��
**�������:     --
**�������:     --
**��ע:         
****************************************************************************************/
void HAL_Storage_Init(void)
{
	DRV_Storage_Init();
	//���洢�豸�ϵ��
}

#endif /*HAL_STORAGE_MODULE_EN*/
/************************************Code End******************************************/
