
#ifndef _MEAN_FILTER_
#define _MEAN_FILTER_
/*******************************************************************************
* Function Name  : mean_filter
* Description    : һά��ֵ�˲�����
* Input          : indata����������ָ��
*                  outdata: �������ָ��
*                  k: �˲���������ֵ���㴰�ڴ�С��k<n��
*                  n: ���������ַ���Ŀ
*
* Output         : None
* Return         : None
******************************************************************************/
void mean_filter(unsigned char *indata,unsigned char *outdata,int k,int n);
#endif
