/*********************************************************************************************************
 ** file:	    fsm.c
 ** description:	协议内容实现
 ** author:      li
 ** date :       2017/05/16
 ***********************************************************************************************************/
#include "fsm.h"
//#include "policy.h"
#include <system.h>
//#include <main.h>
//#include "get_sn_drv.h"
#include "Uart.h"
#include "Timer.h"
#include <string.h>
//#include <mafp_sensor.h>
#include <system.h>
#include <stdio.h>
#include <stdarg.h>
#include "Flash.h"
#include "Sensor.h"
#include "Cache.h"
#include "SHA.h"
#include "stdlib.h"
#include <stdint.h>
#include "ccm_types.h"
#include "system_ccm4202s.h"
#include "algorithm_bin_interface.h"
//#include "synocFsm.h"
//#include "yaleFsm.h"
#include "Debug.h"


typedef enum
{
	STATE_IDLE,                        //空闲状态
	STATE_UPLOAD,                      //上传数据状态
	STATE_DOWNLOAD,                    //下载数据状态
	STATE_ENROLL,                      //注册状态
	STATE_MATCH,                       //匹配状态
	STATE_ENROLL_NOCHECK_FINGER
}_FsmState;  //命令状态机的状态



typedef enum
{
	ENROLL_STATE_IDLE,
	ENROLL_STATE_CAPTURE,
	ENROLL_STATE_COMPUTE,
	ENROLL_STATE_LEAVE,
	ENROLL_STATE_TIMEOUT
}_EnrollState; //注册状态机的状态

typedef enum
{
	MATCH_STATE_IDLE,
	MATCH_STATE_CAPTURE,
	MATCH_STATE_COMPUTE,
	MATCH_STATE_TIMEOUT,
}_MatchState; //匹配状态机的状态


#define ENROLL_PRESS_TIMES_DEFAULT  3   //默认注册按压次数
#define ENROLL_TIMEOUT_DEFAULT      (30 * 1000 * 1000)
#define MATCH_TIMEOUT_DEFAULT		(3 * 1000 * 1000)


//函数申明
static void maFsmTask(S32 ch);
static S32 fsmGetFrame(S32 ch,U8 flag);
static U16 frameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff);
void fsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf);
static void fsmSendFrameTmp(U8 _ack,U16 _data_len,U8 *_data_buf,U32 addr);                  // for setaddr
static U16 frameSetPackTmp(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff,U32 addr);  //for setaddr




//指令函数
//static void fsmVfyPwd(void);
//static void fsmSetPwd(void);
static void fsmSetAddr(void);
static void fsmSetSysPara(void);
static void fsmGetSysPara(void);
static void fsmGetIdList(void);
static void fsmGetIdNum(void);
static void fsmDeleteID(void);
static void fsmEmptyId(void);
static void fsmEnrollTask(void);
static void fsmEnroll(void);
static void fsmMatch(void);
static void fsmMatchTask(void);
static void fsmInitSystem(void);
static void fsmInitSensor(void);
static void fsmHandshake(void);
static void fsmEnDebugPrint(void);
//static void fsmDebugUpdate(void);
static void fsmDebugResetTemplateStorage(void);
//static void fsmDebugReadAllFlash(void);
static void fsmSendDataTask(U8 flag);
static void initEnrollTag(void);
static void initMatchTag(void);
static void fsmDeleteMultiId(void);
static U8 get_enrollid_status(U8 enroll_id);

static void fsmUpLoadTask(void);
static void fsmUploadImage(void);
static void setUpLoadTaskDoneFlag(void);

typedef enum
{
	idle,
	busy,
	done,
	running,
	null
}_Status;



typedef struct
{
	U8 *buff;
	U32 len;
	U8 tmp_buff[256];
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U32 timeout;
	U8 resend_times;
}_SendDataTag;



typedef struct
{
	U8 enroll_id;
	U8 enroll_press_times;
	U32 enroll_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 valid_times;
}_EnrollTag;




typedef struct
{
	U8 match_id;
	U32 match_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
}_MatchTag;


//全局变量
U8  Last_Data_Flag=0;                        //下载数据时，若为最后一帧置1，否则清0
_SendDataTag SendData_Tag={0};
_EnrollTag Enroll_Tag={0};
_MatchTag  Match_Tag={0};


#define RS_BUFF_LEN    512          //接收发送缓冲buff长度
U8 g_recv_buff[RS_BUFF_LEN];         //接收帧缓冲
U8 g_send_buff[RS_BUFF_LEN];         //发送帧缓冲
_FsmState g_fsm_state;
_EnrollState g_enroll_state;
_MatchState g_match_state;
void (*g_frame_func[FRAME_MAX])(void); //命令处理函数表




/*********************************************************************************************************
 ** 函数名称:	fsmInit
 ** 功能描述:	初始化状态机和初始化命令处理函数表
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void fsmInit(void)
{
	//初始化命令处理函数表
	memset(g_frame_func, 0, sizeof(g_frame_func));
	g_frame_func[ENROLL] = fsmEnroll;
	g_frame_func[SERACH] = fsmMatch;
	g_frame_func[Delete_ID] = fsmDeleteID;
	g_frame_func[EMPTY_ID] = fsmEmptyId;
	g_frame_func[SET_SYS_PARA] = fsmSetSysPara;
	g_frame_func[GET_SYS_PARA] = fsmGetSysPara;
	g_frame_func[SET_PWD] = NULL;//fsmSetPwd;
	g_frame_func[VFY_PWD] = NULL;//fsmVfyPwd;
	g_frame_func[GET_RANDOM] = NULL;
	g_frame_func[SET_ADDR] = NULL;//fsmSetAddr;
	g_frame_func[GET_ID_NUM] = fsmGetIdNum;
	g_frame_func[GET_ID_LIST] = fsmGetIdList;
	g_frame_func[DOWN_IMAGE] = NULL;
	g_frame_func[UPLOAD_IMAGE] = fsmUploadImage;


	g_frame_func[INIT_SYSTEM] =fsmInitSystem;
	g_frame_func[DEVICE_READY]=deviceReady;
	g_frame_func[INIT_SENSOR] =fsmInitSensor;
	g_frame_func[HAND_SHAKE]=fsmHandshake;

	g_frame_func[SWITCH_PROTOCOL]=NULL;

	g_frame_func[DEBUG_PRINT_EN]=fsmEnDebugPrint;

	g_frame_func[DEBUG_GET_PARA]=NULL;
	g_frame_func[DEBUG_UPDATE]=NULL;//fsmDebugUpdate;

	g_frame_func[DEBUG_ENROLL_AFTER_DOWNLOAD]=NULL;

	g_frame_func[DEBUG_MATCH_AFTER_DOWNLOAD]= NULL;

	g_frame_func[DEBUG_RESET_TEMPLATE_STORAGE]=fsmDebugResetTemplateStorage;

	g_frame_func[DEBUG_READ_ALL_FLASH]=NULL;//fsmDebugReadAllFlash;

	g_frame_func[Delete_MULTI_ID]=fsmDeleteMultiId;

	//初始化状态机的状态
	g_fsm_state = STATE_IDLE;

	g_enroll_state = ENROLL_STATE_IDLE;

	g_match_state = MATCH_STATE_IDLE;

	initEnrollTag();
	initMatchTag();

}



/*********************************************************************************************************
 ** 函数名称:	fsmTask
 ** 功能描述:	状态机主函数
 **
 ** input: ch-->串口buff中取出的字节
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void fsmTask(S32 ch)
{
	maFsmTask(ch);
}



static void maFsmTask(S32 ch)
{
	U8 command;
	U8 tmp_buf[2]={0};
	if (ch >= 0)
	{
		if (fsmGetFrame(ch,1))
		{
			if(Sys_ParaTag.en_passwd)
			{
				if( (g_recv_buff[9]==VFY_PWD) && (PID_COMMAND ==g_recv_buff[FRAME_PID]))
				{
//					fsmVfyPwd();
					return;
				}

				if((!Gl_Password)&&(g_recv_buff[9]!=INIT_SYSTEM))
				{
					tmp_buf[0]=VFY_PWD;//指令
					tmp_buf[1]=PS_MUST_VERIFY_PWD;
					fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
					return ;
				}
			}
			if(PID_COMMAND ==g_recv_buff[FRAME_PID])
			{
				command = g_recv_buff[9];      //得到命令
				if( (command>0) && (command<FRAME_MAX) &&(g_frame_func[command]) )
				{
					g_frame_func[command]();
				}
				else
				{
					tmp_buf[0]=0;//指令
					tmp_buf[1]=PS_COMMAND_ERR ;
					fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
				}
			}
			else if(PID_DATA_ACK  ==g_recv_buff[FRAME_PID])
			{
				setUpLoadTaskDoneFlag();
			}
		}
	}
	else if(g_fsm_state == STATE_UPLOAD)
	{
		fsmUpLoadTask();
	}
	else if (g_fsm_state == STATE_ENROLL)
	{
		fsmEnrollTask();
	}
	else if (g_fsm_state == STATE_MATCH)
	{
		fsmMatchTask();
	}

}

/*********************************************************************************************************
 ** 函数名称:	fsmCRC
 ** 功能描述:	计算CRC校验
 **
 ** input: dat-->数据buf,len-->数据长度
 ** return:返回CRC校验值
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
U8  fsmCRC(U8 *dat, U32 len)
{
	U8 crc8 = 0;
	while(len--){
		crc8 = CRC8Table[crc8 ^ *dat];
		dat++;
	}
	return crc8;
}


/*********************************************************************************************************
 ** 函数名称:	fsmGetFrame
 ** 功能描述:	取帧
 **
 ** input: ch-->字节
 ** return:0-->没有取到完整帧    >0-->取到完整的帧
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static S32 fsmGetFrame(S32 ch,U8 flag)
{
	U32 device_addr;
	U16 frame_len=0;
	U8 pid=0;
	U8 tmp[2];
	tmp[0]=0;
	tmp[1]=PS_OK;
	static	U16 nold_RecNum;
	static  U32 nrec_TimeOut;
	static U32 recv_len = 0;

	/*判断超时工作*/
	if(recv_len==0)
	{
		nrec_TimeOut=0;
		nold_RecNum=0;
	}

	if(nold_RecNum!=recv_len)
	{
		nrec_TimeOut=0;
		nold_RecNum=recv_len;
	}
	else//判断超时
	{
		nrec_TimeOut++;
		if(nrec_TimeOut>100000)
		{
			recv_len=0;
			return 0;
		}
	}

	if (recv_len == 0)
	{
		if ( ch == (FRAME_START>>8) )
		{
			g_recv_buff[recv_len++] = ch;
		}
	}
	else if(recv_len == 1)
	{
		if ( ch == (FRAME_START&0x00ff) )
		{
			g_recv_buff[recv_len++] = ch;
		}
		else
		{
			recv_len=0;
			return 0;
		}
	}
	else
	{
		g_recv_buff[recv_len++] = ch;
		if(recv_len == 6)
		{
			device_addr=( (g_recv_buff[2]<<8)+g_recv_buff[3]);
			device_addr=( ( device_addr<<16)+(g_recv_buff[4]<<8)+g_recv_buff[5]);
			//debugPrint("%x ",device_addr);
			// 模块地址在flash中存储
			if( (device_addr!=Sys_ParaTag.device_add) && (device_addr!=0))
			{
				recv_len=0;
				tmp[1]=PS_DEVICE_ADDR_ERR;
				if(flag)
					fsmSendFrameTmp(PID_ACK ,0x02,&tmp[0],0);
				return 0;
			}

		}
		else if(recv_len >= FRAME_STA_LEN)
		{

			frame_len=(g_recv_buff[FRAME_LEN_HIGH ]);
			frame_len<<=8;
			frame_len|=(g_recv_buff[FRAME_LEN_LOW ]);

			if(frame_len>FRAME_MOST_LEN)
			{
				recv_len=0;
				tmp[1]=PS_RECV_ERR;
				if(flag)
					fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
				return 0;
			}

			frame_len+=FRAME_STA_LEN;

			if(recv_len >=frame_len)
			{
				recv_len=0;
				//检测是否校验正确
				if( fsmCRC(g_recv_buff,frame_len-1) ==  g_recv_buff[frame_len-1]  )
				{
					//无校验错误
					pid=g_recv_buff[FRAME_PID];   //标识
					if( (pid==PID_COMMAND) ||  (pid == PID_DATA )|| (pid==PID_DATA_ACK ))
					{
						Last_Data_Flag=0;
					}
					else if(pid == PID_LAST_DATA)
					{
						Last_Data_Flag=1;     //表示是最后一帧数据
					}
					else
					{
						tmp[1]=PS_PID_ERR ;
						if(flag)
							fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
						return 0;
					}
#if DEBUG_PRINT
					debugPrint(" frame_len == %d",frame_len);
#endif
					return frame_len;
				}
				else
				{
					//校验错误
					tmp[1]=PS_COMM_ERR;
					if(flag)
						fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
					return 0;

				}
			}
		}
	}
	return 0;
}

/*
 * refresh receive timeout
 */

volatile static  U32 base_TimeOut=0;

static void  refreshRecTimeout(void)
{
	base_TimeOut=systemTickMs();
}


/*********************************************************************************************************
 ** 函数名称:	frameSetPack
 ** 功能描述:	封装发送帧
 **
 ** input:  _data_len-->内容长度,_data_buf-->内容buff,type-->标识,_send_buff 发送buff
 ** return: 0-->失败, >0-->一帧命令或者数据的总长度
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static U16 frameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff)
{

	U8 nwCHK = 0;
	if(_send_buff&&_data_buf)   //参数检测
	{
		/*make packet head*/
		_send_buff[0] = (FRAME_START>>8);
		_send_buff[1] = FRAME_START &0x00ff;
		_send_buff[2] = (Sys_ParaTag.device_add>>24)&0xff;
		_send_buff[3] = (Sys_ParaTag.device_add>>16)&0xff;
		_send_buff[4] = (Sys_ParaTag.device_add>>8)&0xff;
		_send_buff[5] = Sys_ParaTag.device_add&0xff;
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+FRAME_CRC) & 0xff;

		memcpy(&g_send_buff[FRAME_STA_LEN],_data_buf,_data_len);
		nwCHK=fsmCRC(g_send_buff,_data_len+FRAME_STA_LEN);

		g_send_buff[_data_len+FRAME_STA_LEN]=nwCHK;

		return (_data_len+FRAME_CRC+FRAME_STA_LEN);
	}
	else
	{
		return 0;
	}

}


/*********************************************************************************************************
 ** 函数名称:	fsmSendFrame
 ** 功能描述:	发送一帧命令或者数据
 **
 ** input: _data_len-->内容长度,_data_buf-->内容buff,_ack-->标识
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void fsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf)
{
	U16 len;
	if(_data_buf)
	{
		len=frameSetPack(_data_len,_data_buf,_ack,g_send_buff);
		uartSendBuff(g_send_buff,len);
	}
}


static U16 frameSetPackTmp(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff,U32 addr)
{

	U8 nwCHK = 0;
	if(_send_buff&&_data_buf)   //参数检测
	{
		/*make packet head*/
		_send_buff[0] = (FRAME_START>>8);
		_send_buff[1] = FRAME_START &0x00ff;
		_send_buff[2] = (addr>>24)&0xff;
		_send_buff[3] = (addr>>16)&0xff;
		_send_buff[4] = (addr>>8)&0xff;
		_send_buff[5] = addr&0xff;
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+FRAME_CRC) & 0xff;

		memcpy(&g_send_buff[FRAME_STA_LEN],_data_buf,_data_len);
		nwCHK=fsmCRC(g_send_buff,_data_len+FRAME_STA_LEN);

		g_send_buff[_data_len+FRAME_STA_LEN]=nwCHK;

		return (_data_len+FRAME_CRC+FRAME_STA_LEN);
	}
	else
	{
		return 0;
	}

}

static void fsmSendFrameTmp(U8 _ack,U16 _data_len,U8 *_data_buf,U32 addr)
{
	U16 len;
	if(_data_buf)
	{
		len=frameSetPackTmp(_data_len,_data_buf,_ack,g_send_buff,addr);
		uartSendBuff(g_send_buff,len);
	}
}



/*********************************************************************************************************
 ** 函数名称:	fsmSetSysPara
 ** 功能描述: 设置系统参数
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmSetSysPara(void)
{
	U8 type,para,flag=0;
	U8 tmp_buf[2]={0};
	U32 timeout=0xffffff;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];     //指令
	tmp_buf[1]=PS_OK;

	type=g_recv_buff[FRAME_STA_LEN+1];
	para=g_recv_buff[FRAME_STA_LEN+2];


	switch(type)
	{
	case SYS_PARA_BAUDRATE:
		if(para>=1)
		{
			flag|=0x01;
			Sys_ParaTag.baudrate=para;
			WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
		}
		else
		{
			tmp_buf[1]=PS_PARAM_ERROR;
		}
		break;
	case SYS_PARA_SECLVL:
		break;
	case SYS_PARA_PACKLEN:
		if( (para>=0) && (para<=3))
		{
			Sys_ParaTag.pkt_size=para;
			WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
			switch(para)
			{
			case 0: Frame_Data_Size=32; break;
			case 1: Frame_Data_Size=64; break;
			case 2: Frame_Data_Size=128; break;
			case 3: Frame_Data_Size=256; break;
			default:Frame_Data_Size=256;break;
			}

		}
		else
		{
			tmp_buf[1]=PS_PARAM_ERROR;
		}
		break;
	default:
		tmp_buf[1]=PS_INVALID_REG;
		break;
	}

	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
	if(flag&0x01)
	{
		timerDelayMs(20);
		while((!uartSendBuffEmpty())&&(timeout--));
		uartInit(Sys_ParaTag.baudrate*9600);
		timerDelayMs(2);

	}
}

/*********************************************************************************************************
 ** 函数名称:	fsmReadSysPara
 ** 功能描述: 读系统参数
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetSysPara(void)
{
	U8 tmp_buf[18]={0};

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	//版本
	tmp_buf[2]=Sys_ParaTag.version[0];
	tmp_buf[3]=Sys_ParaTag.version[1];
	tmp_buf[4]=Sys_ParaTag.version[2];
	tmp_buf[5]=Sys_ParaTag.version[3];
	tmp_buf[6]=Sys_ParaTag.version[4];
	tmp_buf[7]=Sys_ParaTag.version[5];
	tmp_buf[8]=Sys_ParaTag.version[6];
	tmp_buf[9]=Sys_ParaTag.version[7];

	//指纹库大小
	tmp_buf[10]=(Sys_ParaTag.index>>8)&0xff;
	tmp_buf[11]=Sys_ParaTag.index&0xff;

	tmp_buf[12]=Sys_ParaTag.device_add>>24;
	tmp_buf[13]=Sys_ParaTag.device_add>>16;
	tmp_buf[14]=Sys_ParaTag.device_add>>8;
	tmp_buf[15]=Sys_ParaTag.device_add;

	tmp_buf[16]=Sys_ParaTag.pkt_size;
	tmp_buf[17]=Sys_ParaTag.baudrate;

	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x12,&tmp_buf[0]);
}



/*********************************************************************************************************
 ** 函数名称:	fsmReadConList
 ** 功能描述: 读取指纹ID索引列表
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetIdList(void)
{
	U8 tmp_buf[34]={0};
	U8 page=g_recv_buff[FRAME_STA_LEN+1];
	memset(Index_Tab,0,sizeof(Index_Tab));
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_PARAM_ERROR;

	if( (page>=0) && (page<4))
	{
		if(binlib_api(LIB_READ_ID_LIST,Index_Tab[page]+0,32)==0)
			tmp_buf[1]=PS_OK;
	}
	else
	{
		page=0;
		tmp_buf[1]=PS_PARAM_ERROR;
	}

	memcpy(&tmp_buf[2],Index_Tab[page]+0,sizeof(Index_Tab)/4);
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x22,&tmp_buf[0]);
}



static U16 get_used_enrollids(void)
{
	U16 nums=0,i=0;
	for(i=0;i<MAX_ID;i++)
	{
		if(get_enrollid_status(i)==1)
		{
			nums++;
		}
	}
	return nums;
}

/*********************************************************************************************************
 ** 函数名称:	fsmTemplateNum
 ** 功能描述: 读取录入的指纹数量
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetIdNum(void)
{
	U16 num;
	U8 tmp_buf[4]={0};

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	num=get_used_enrollids();
	tmp_buf[2]=num>>8;
	tmp_buf[3]=num;
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x4,&tmp_buf[0]);
}




/*********************************************************************************************************
 ** 函数名称:	fsmDeleteID
 ** 功能描述: 删除指纹ID
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmDeleteID(void)
{
	U8 tmp_buf[2]={0},buff[4];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];
	tmp_buf[1]=PS_DEL_TEMP_ERR;
	buff[0]=g_recv_buff[FRAME_STA_LEN+1];
	buff[1]=g_recv_buff[FRAME_STA_LEN+2];
	buff[2]=0;
	buff[3]=1;
	if(binlib_api(LIB_DELETE_ID,buff,sizeof(buff))==0)
		tmp_buf[1]=PS_OK;
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


/*********************************************************************************************************
 ** 函数名称:	fsmEmpty
 ** 功能描述: 清空指纹库
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEmptyId(void)
{
	U8 tmp_buf[2]={0},buff[4];
	g_fsm_state = STATE_IDLE;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_CLEAR_TEMP_ERR ;

	buff[0]=0;
	buff[1]=0;
	buff[2]=(MAX_ID>>8)&0xff;
	buff[3]=MAX_ID&0xff;

	if(binlib_api(LIB_DELETE_ID,buff,sizeof(buff))==0)
		tmp_buf[1]=PS_OK ;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}



static void initEnrollTag(void)
{
	Enroll_Tag.enroll_press_times=ENROLL_PRESS_TIMES_DEFAULT;
	Enroll_Tag.enroll_timeout=ENROLL_TIMEOUT_DEFAULT;
	Enroll_Tag.timerRestart=timerRestart;
	Enroll_Tag.timerElapsedUs=timerElapsedUs;
	Enroll_Tag.enroll_id=0;
	Enroll_Tag.valid_times=0;
}




static U8 get_enrollid_status(U8 enroll_id)
{
	U8 buff[32],res=0;

	if(enroll_id<MAX_ID)
	{
		binlib_api(LIB_READ_ID_LIST,buff,sizeof(buff));
		if( (buff[enroll_id/8] )& (1<<(enroll_id%8) ) )
			res=1;
	}
	return res;
}


/*********************************************************************************************************
 ** 函数名称:	fsmEnroll
 ** 功能描述: 注册指纹
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEnroll(void)
{
	U8 tmp[2]={0};
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	g_fsm_state = STATE_IDLE;

//	tmp[1]=PS_RESPONSE;
//	fsmSendFrame(PID_ACK ,0x02,&tmp[0]);          //

	binlib_api(LIB_CLEAR_ENROLL_ALL_STEPS,NULL,0);

	Enroll_Tag.enroll_press_times=ENROLL_PRESS_TIMES_DEFAULT;
	Enroll_Tag.enroll_timeout=ENROLL_TIMEOUT_DEFAULT;
	Enroll_Tag.enroll_id = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];
	Enroll_Tag.valid_times=0;


	if ( (Enroll_Tag.enroll_id >= MAX_ID))
	{
		tmp[1]=PS_ADDRESS_OVER;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else if( (get_enrollid_status(Enroll_Tag.enroll_id))== 1)
	{
		tmp[1]=PS_DUL_ENROLL_ERROR;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else
	{
		g_fsm_state = STATE_ENROLL;
		g_enroll_state = ENROLL_STATE_CAPTURE;
		Enroll_Tag.timerRestart();
	}
}


/*********************************************************************************************************
 ** 函数名称:	fsmEnrollTask
 ** 功能描述: 注册状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEnrollTask(void)
{
	U8 buff[2],tmp[2],r,ret;
	U16 temp_id;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	if (g_fsm_state != STATE_ENROLL)
	{
		g_enroll_state = ENROLL_STATE_IDLE;
	}

	switch(g_enroll_state)
	{
	case ENROLL_STATE_IDLE:
		break;
	case ENROLL_STATE_CAPTURE:
		if (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout)
		{
			g_enroll_state = ENROLL_STATE_TIMEOUT;
			break;
		}

		r = binlib_api(LIB_PRE_ENROLL,NULL,0);
		/*
		 * 0-->ok
		 * !0-->fail
		 * */
		if (r == 0)
		{
			//ok
			g_enroll_state = ENROLL_STATE_COMPUTE;
		}
		else
		{
			g_enroll_state = ENROLL_STATE_CAPTURE;
		}
		break;

	case ENROLL_STATE_COMPUTE:

		temp_id=(Enroll_Tag.enroll_id<<1)&0xfffe;
		buff[0]=(temp_id>>8)&0xff;
		buff[1]=(temp_id&0xff);
		ret=binlib_api(LIB_ENROLL,buff,2);
		/*
		 * 0-->ok
		 * !0-->fail
		 * */
		g_enroll_state = ENROLL_STATE_LEAVE;
		if( ret == 0 )
		{
			//valid times
			Enroll_Tag.valid_times++;
			if(Enroll_Tag.valid_times < Enroll_Tag.enroll_press_times)
			{
				tmp[1] = PS_NO_ERROR;
			}
			else
			{
				temp_id|=0x01;
				buff[0]=(temp_id>>8)&0xff;
				buff[1]=(temp_id&0xff);

				if(binlib_api(LIB_ENROLL,buff,2))
					tmp[1] = PS_ENROLL_ERR;
				else
					tmp[1] =PS_OK;

				g_fsm_state = STATE_IDLE;
				g_enroll_state = ENROLL_STATE_IDLE;
			}
		}
		else
		{
			tmp[1] =PS_ENROLL_ERR;
			g_enroll_state = ENROLL_STATE_IDLE;
			g_fsm_state = STATE_IDLE;
		}

		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
		Enroll_Tag.timerRestart();
		break;
	case ENROLL_STATE_LEAVE:
		if (binlib_api(LIB_IS_FINGERLEAVE,NULL,0) == 0)
		{
			g_enroll_state = ENROLL_STATE_CAPTURE;
		}
		else if (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout)
		{
			g_enroll_state = ENROLL_STATE_TIMEOUT;
		}
		break;
	case ENROLL_STATE_TIMEOUT:
		tmp[1]=PS_TIMEOUT;
		g_enroll_state = ENROLL_STATE_IDLE;
		g_fsm_state = STATE_IDLE;                     //退出注册状态机
		binlib_api(LIB_CLEAR_ENROLL_ALL_STEPS,NULL,0);
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
		break;
	}
}



static void initMatchTag(void)
{
	Match_Tag.match_id=0xff;
	Match_Tag.match_timeout=MATCH_TIMEOUT_DEFAULT;
	Match_Tag.timerElapsedUs=timerElapsedUs;
	Match_Tag.timerRestart=timerRestart;
}


/*********************************************************************************************************
 ** 函数名称:	fsmMatch
 ** 功能描述: 匹配指纹
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmMatch(void)
{
	//暂时不判断参数
//	U8 tmp[4]={0};
//	tmp[0]=g_recv_buff[FRAME_STA_LEN];
//	tmp[1]=PS_RESPONSE;
//	fsmSendFrame(PID_ACK ,0x04,&tmp[0]);

	g_fsm_state = STATE_MATCH;
	g_match_state = MATCH_STATE_CAPTURE;
	Match_Tag.timerRestart();
}



/*********************************************************************************************************
 ** 函数名称:	fsmMatchTask
 ** 功能描述: 匹配状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmMatchTask(void)
{
	U8 tmp[4]={0},ret,buff[2];
	tmp[0]=g_recv_buff[FRAME_STA_LEN];

	if (g_fsm_state != STATE_MATCH){
		g_match_state = MATCH_STATE_IDLE;
	}

	switch(g_match_state)
	{
	case MATCH_STATE_IDLE:
		break;
	case MATCH_STATE_CAPTURE:
		if (Match_Tag.timerElapsedUs() > Match_Tag.match_timeout)
		{
			g_match_state = MATCH_STATE_TIMEOUT;
			break;
		}

		ret= binlib_api(LIB_PRE_MATCH,NULL,0);
		/*
		 * 0-->ok
		 * !0-->fail
		 * */
		if (ret == 0)
		{
			g_match_state = MATCH_STATE_COMPUTE;
		}
		else
		{
			g_match_state = MATCH_STATE_CAPTURE;
		}
		break;
	case MATCH_STATE_COMPUTE:
		ret = binlib_api(LIB_MATCH,buff,2);;
		/*
		 * 0-->ok
		 * !0-->fail
		 * */
		if(ret == 0 )
		{
			tmp[2]=buff[0];
			tmp[3]=buff[1];
			tmp[1]=PS_OK;
		}
		else
		{
			tmp[1]=PS_NOT_SEARCHED;
		}
		g_match_state = MATCH_STATE_IDLE;
		g_fsm_state = STATE_IDLE;
		fsmSendFrame(PID_ACK ,0x04,&tmp[0]);
		break;
	case MATCH_STATE_TIMEOUT:
		tmp[1]=PS_TIMEOUT;
		g_match_state = MATCH_STATE_IDLE;
		g_fsm_state = STATE_IDLE;
		fsmSendFrame(PID_ACK ,0x04,&tmp[0]);
		break;
	}
}




/*********************************************************************************************************
 ** 函数名称:	fsmResetFactory
 ** 功能描述: 恢复出厂设置
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmInitSystem(void)
{
	U32 addr;
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},buff[4];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	ReadParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	addr=Sys_ParaTag.device_add;

	initDeviceErase();
	hardwareInit();

	buff[0]=0;
	buff[1]=0;
	buff[2]=(MAX_ID>>8)&0xff;
	buff[3]=MAX_ID&0xff;
	binlib_api(LIB_DELETE_ID,buff,sizeof(buff));
	initDeviceInfo();

	fsmSendFrameTmp(PID_ACK ,0x02,&tmp_buf[0],addr);
	timerDelayMs(20);
	while(!uartSendBuffEmpty());
	uartInit(Baud_Rate*9600);
	timerDelayMs(2);
}



/*********************************************************************************************************
 ** 函数名称:	fsmResetFactory
 ** 功能描述: 恢复出厂设置
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmInitSensor(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},buff[1];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	buff[0]=0;

	if(binlib_api(LIB_INIT_SENSOR,buff,sizeof(buff))==0)
		tmp_buf[1]=PS_OK;
	else
		tmp_buf[1]=PS_PARAM_ERROR;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);

}




/*********************************************************************************************************
 ** 函数名称:	fsmHandle
 ** 功能描述: 握手指令
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmHandshake(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}

//表示 所有接口初始化完成

void deviceReady(void)
{
	S32 res,bad_blocks;
	U8 tmp_buf[4]={0};
	tmp_buf[0]=DEVICE_READY;

	res=binlib_init(bin_funcs_table);

	if(!res)
	{
		Device_Ready_Flag|=DEVICE_READY_SENSOR_BREAKDOWN;
	}
	else
	{
		Device_Ready_Flag&=~(DEVICE_READY_SENSOR_BREAKDOWN|DEVICE_READY_SENSOR_CRC_FAILE);
	}
	tmp_buf[1]=Device_Ready_Flag;
	tmp_buf[2]=(bad_blocks>>8)&0xff;
	tmp_buf[3]=(bad_blocks&0xff);

	fsmSendFrame(PID_ACK ,0x04,&tmp_buf[0]);

}



//用于调试，必须在 uartInit(Sys_ParaTag.baudrate*9600) 后使用
void fsmDebugPrint(const char *fmt, ...)
{
	if(Sys_ParaTag.en_debug_print)
	{
		timerDelayMs(3);
		//S32 ret;
		U8 tmp_buf[256];
		va_list args;
		va_start(args, fmt);
		/*ret = */vsprintf((char *)&tmp_buf[1], fmt, args);
		va_end(args);
		tmp_buf[0]= DEBUG_PRINT_MODE;
		fsmSendFrame(PID_DEBUG_PRINT ,sizeof(tmp_buf)-2,&tmp_buf[0]);
		timerDelayMs(30);
	}
	return;
}


//用于调试
static void fsmEnDebugPrint(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},flag=0;
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	flag=g_recv_buff[FRAME_STA_LEN+1];
	if(flag)
	{
		Sys_ParaTag.en_debug_print=1;
	}
	else
	{
		Sys_ParaTag.en_debug_print=0;
	}

	WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}







/*********************************************************************************************************
 ** 函数名称:fsmTaskForUpload
 ** 功能描述:	状态机主函数用于upload
 **
 ** input: ch-->串口buff中取出的字节
 ** return:void
 ** output:0--> 收到数据应答包   1-->收到其他命令包则打断上传   2-->等待收到完整一帧
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static U8 fsmTaskForSendData(S32 ch)
{
	if (ch >= 0)
	{
		if (fsmGetFrame(ch,1))
		{
			if(PID_DATA_ACK  ==g_recv_buff[FRAME_PID])
			{
				return 0;
			}
			else if(PID_COMMAND  ==g_recv_buff[FRAME_PID])
			{
				return 1;
			}
		}
	}
	return 2;
}
//////////////////////////////////////////////upload func///////////////////////////////////

typedef struct
{
	U8 * upload_buff;
	U8 upload_cmd;
	S32 upload_len;
	U32 upload_data_offset;
	U32 nums;
	U32 timeout;
	U16 pos;
	U16 off;
	U16 len;
	U16 resend_now_times;
	U16 resend_max_times;
	_Status status;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 tmp_buff[256];
}_UploadTag;

static _UploadTag UpLoad_Tag={0};

/*
 *  upload function tag
 */
static void initUploadTag(void)
{
	UpLoad_Tag.upload_buff=NULL;
	UpLoad_Tag.upload_len=0;
	UpLoad_Tag.upload_data_offset=0;
	UpLoad_Tag.nums=0;
	UpLoad_Tag.timeout=3*1000*1000;
	UpLoad_Tag.off=0;
	UpLoad_Tag.pos=0;
	UpLoad_Tag.resend_max_times=5;
	UpLoad_Tag.resend_now_times=0;
	UpLoad_Tag.timerElapsedUs=timerElapsedUs;
	UpLoad_Tag.timerRestart=timerRestart;
	memset(UpLoad_Tag.tmp_buff,0,sizeof(UpLoad_Tag.tmp_buff));
	UpLoad_Tag.status=idle;
}

/*
 *  upload image cmd
 */
static void fsmUploadImage(void)
{
	uint32_t i = 0;

	U16 width=0,height=0;
	U8 tmp[10]={0},type=0;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	tmp[1]=PS_OK;
	type=g_recv_buff[FRAME_STA_LEN+1];
	initUploadTag();
	g_fsm_state = STATE_IDLE;
	UpLoad_Tag.upload_cmd=UPLOAD_IMAGE;
	switch (type)
	{
	case 1:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_REALTIME, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> Realtime image.  ");
		break;
	case 2:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_BACKGROUD, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> Background.  ");
		break;
	case 3:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_RMV_BKG, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> RMV Background.  ");
		/*for (i = 0; i < UpLoad_Tag.upload_len; i++){
			LOGE("%x ",UpLoad_Tag.upload_buff[i]);
		}*/
		//LOGE("image length:%d ",UpLoad_Tag.upload_len);
		break;
	case 4:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_ENROLL, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> IMAGE ENROLL.  ");
		break;
	case 5:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_AUTHEN, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> IMAGE AUTHEN.  ");
		break;
	case 6:
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(IMAGE_TESTING, &UpLoad_Tag.upload_buff, &width, &height);
		//LOGE("--> IMAGE TESTING.  ");
		break;
	default:
		//LOGE("--> RMV error.  ");
		tmp[1]=PS_PARAM_ERROR;
		break;
	}
	if(UpLoad_Tag.upload_buff && (UpLoad_Tag.upload_len>0) )
	{
		g_fsm_state = STATE_UPLOAD;
		tmp[2]=(UpLoad_Tag.upload_len>>16)&0xff;
		tmp[3]=(UpLoad_Tag.upload_len>>8)&0xff;
		tmp[4]=(UpLoad_Tag.upload_len)&0xff;
		tmp[5]=(width>>8)&0xff;
		tmp[6]=width&0xff;
		tmp[7]=(height>>8)&0xff;
		tmp[8]=height&0xff;
		UpLoad_Tag.len=(Frame_Data_Size-3);
		UpLoad_Tag.pos=UpLoad_Tag.upload_len/UpLoad_Tag.len; //38
		UpLoad_Tag.off=UpLoad_Tag.upload_len%UpLoad_Tag.len; //e8
		UpLoad_Tag.status=running;
/*		
		LOGE("tmp[2] = %x\r\n",tmp[2]);
		LOGE("tmp[3] = %x\r\n",tmp[3]);
		LOGE("tmp[4] = %x\r\n",tmp[4]);
		LOGE("tmp[5] = %x\r\n",tmp[5]);
		LOGE("tmp[6] = %x\r\n",tmp[6]);
		LOGE("tmp[7] = %x\r\n",tmp[7]);
		LOGE("tmp[8] = %x\r\n",tmp[8]);
		LOGE("UpLoad_Tag.len = %x\r\n",UpLoad_Tag.len);
		LOGE("UpLoad_Tag.pos = %x\r\n",UpLoad_Tag.pos);
		LOGE("UpLoad_Tag.off = %x\r\n",UpLoad_Tag.off);
*/		
		
	}
	else
	{
		tmp[1]= PS_UP_IMG_ERR;
	}
	fsmSendFrame(PID_ACK ,0x09,&tmp[0]);
	timerDelayMs(5);

}

/*
 * set done flag of signal data frame in upload task
 */
static void setUpLoadTaskDoneFlag(void)
{
	if(g_fsm_state == STATE_UPLOAD)
	{
		UpLoad_Tag.status=done;
	}
}

/*
 *  upload task
 */
static void fsmUpLoadTask(void)
{

	U8 res;
	U16 nums;
	U8 tmp[6]={0};
	if(g_fsm_state == STATE_UPLOAD)
	{
		switch(UpLoad_Tag.status)
		{
		case idle:
			g_fsm_state = STATE_IDLE;
			break;
		case running:
			if((UpLoad_Tag.upload_buff)&&(UpLoad_Tag.upload_data_offset<=UpLoad_Tag.upload_len)
			   &&(UpLoad_Tag.resend_now_times<UpLoad_Tag.resend_max_times))
			{
				UpLoad_Tag.status=busy;
				UpLoad_Tag.timerRestart();
				UpLoad_Tag.tmp_buff[0]=UpLoad_Tag.nums>>8;
				UpLoad_Tag.tmp_buff[1]=UpLoad_Tag.nums&0xff; //frame tag: (DECIMAL)0-56 
				
				//LOGE("UpLoad_Tag.tmp_buff[0] = %x\r\n",UpLoad_Tag.tmp_buff[0]);
		    //LOGE("UpLoad_Tag.tmp_buff[1] = %x\r\n",UpLoad_Tag.tmp_buff[1]);

				if(UpLoad_Tag.nums<UpLoad_Tag.pos)
				{
					memcpy(&UpLoad_Tag.tmp_buff[2],UpLoad_Tag.upload_buff+UpLoad_Tag.nums*UpLoad_Tag.len,UpLoad_Tag.len);
					UpLoad_Tag.upload_data_offset=UpLoad_Tag.nums*UpLoad_Tag.len+UpLoad_Tag.len;
					if( (UpLoad_Tag.nums==(UpLoad_Tag.pos-1) ) && (UpLoad_Tag.off==0))
					{
						fsmSendFrame(PID_LAST_DATA,UpLoad_Tag.len+2,&UpLoad_Tag.tmp_buff[0]);
					}
					else
					{
						fsmSendFrame(PID_DATA,UpLoad_Tag.len+2,&UpLoad_Tag.tmp_buff[0]);
					}

				}
				else if(UpLoad_Tag.nums==UpLoad_Tag.pos)
				{
					if(UpLoad_Tag.off>0)
					{
						memcpy(&UpLoad_Tag.tmp_buff[2],UpLoad_Tag.upload_buff+UpLoad_Tag.nums*UpLoad_Tag.len,UpLoad_Tag.off);
						UpLoad_Tag.upload_data_offset=UpLoad_Tag.nums*UpLoad_Tag.len+UpLoad_Tag.off;
						fsmSendFrame(PID_LAST_DATA,UpLoad_Tag.off+2,&UpLoad_Tag.tmp_buff[0]);
					}
				}
			}
			break;
		case busy:
			if( UpLoad_Tag.timerElapsedUs()> UpLoad_Tag.timeout)
			{
				UpLoad_Tag.status=running;
				UpLoad_Tag.resend_now_times++;
			}

			if(UpLoad_Tag.resend_now_times>=UpLoad_Tag.resend_max_times)
			{
				g_fsm_state = STATE_IDLE;
				tmp[0]=UpLoad_Tag.upload_cmd;
				tmp[1]=PS_TIMEOUT;
				fsmSendFrame(PID_ACK ,0x05,&tmp[0]);
			}
			break;

		case done:
			nums=g_recv_buff[FRAME_STA_LEN ];
			nums=(nums<<8)+g_recv_buff[FRAME_STA_LEN +1];
			res=g_recv_buff[FRAME_STA_LEN+2];
			if(res==PS_OK )
			{
				UpLoad_Tag.status=running;
				UpLoad_Tag.resend_now_times=0;
				if(UpLoad_Tag.nums == nums)
				{
					UpLoad_Tag.nums++;
					if(UpLoad_Tag.upload_data_offset == UpLoad_Tag.upload_len )
						g_fsm_state = STATE_IDLE;
				}
			}
			else if((res == PS_UP_IMG_ERR)|| (res == PS_UP_CHAR_ERR))
			{
				g_fsm_state = STATE_IDLE;
				tmp[0]=UpLoad_Tag.upload_cmd;
				tmp[1]=res;
				fsmSendFrame(PID_ACK ,0x05,&tmp[0]);
			}
			else
				UpLoad_Tag.status=busy;
			break;
		default:
			g_fsm_state = STATE_IDLE;
			break;
		}
		refreshRecTimeout();
	}
}

/*********************************************************************************************************
 ** 函数名称:	  fsmUpLoadTask
 ** 功能描述:  上传状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
// flag = 0 send image
// flag = 1 send eflash data
// flag = 2 send spiflash data
static void fsmSendDataTask(U8 flag)
{
	S32 ch;
	U32 len=(Frame_Data_Size-3);
	U32 pos=SendData_Tag.len/len;
	U32 off=SendData_Tag.len%len;
	U16 i=0,frameNum=0;
	U8 ret=1,res;
	U8 end=0;

	SendData_Tag.timerElapsedUs=timerElapsedUs;
	SendData_Tag.timerRestart=timerRestart;
	SendData_Tag.timeout=4*1000*1000;
	SendData_Tag.resend_times=10;

#if DEBUG_PRINT
	debugPrint("pos=%d\r\n",pos);
	debugPrint("off=%d\r\n",off);
#endif
	while(1)
	{
#if DEBUG_PRINT
		debugPrint("i=%d ",i);
#endif
//	    timerDelayMs(1);
		SendData_Tag.timerRestart();
		SendData_Tag.tmp_buff[0]=i>>8;
		SendData_Tag.tmp_buff[1]=i&0xff;
		if(i<pos)
		{
			if( (i==(pos-1) ) && (off==0))
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,len);

				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
				{
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),len);
				}
				else if(flag == FSM_SEND_DATA_EFLASH2_DATA)
				{
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH2_READ_START_ADDR+i*len),len);
				}
				else
					SPI_Flash_Read(&SendData_Tag.tmp_buff[2],(FSM_SPIFLASH_READ_START_ADDR+i*len),len);


				fsmSendFrame(PID_LAST_DATA,len+2,&SendData_Tag.tmp_buff[0]);
				end=1;
			}
			else
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,len);

				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),len);

				else if(flag == FSM_SEND_DATA_EFLASH2_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH2_READ_START_ADDR+i*len),len);
				else
					SPI_Flash_Read(&SendData_Tag.tmp_buff[2],(FSM_SPIFLASH_READ_START_ADDR+i*len),len);

				fsmSendFrame(PID_DATA,len+2,&SendData_Tag.tmp_buff[0]);
			}

		}
		if(i==pos)
		{
			if(off>0)
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,off);

				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),off);

				else if(flag == FSM_SEND_DATA_EFLASH2_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH2_READ_START_ADDR+i*len),off);
				else
					SPI_Flash_Read(&SendData_Tag.tmp_buff[2],(FSM_SPIFLASH_READ_START_ADDR+i*len),off);

				fsmSendFrame(PID_LAST_DATA,off+2,&SendData_Tag.tmp_buff[0]);
				end=1;
			}
			else
			{
				break;
			}
		}
		while( SendData_Tag.timerElapsedUs()<SendData_Tag.timeout)
		{
			uartReadByte(&ch);
			ret=fsmTaskForSendData(ch);

			if(ret==0)
			{
				res=g_recv_buff[FRAME_STA_LEN+2];        //获取结果码s
				if(res==PS_OK)
				{
					i++;
				}
				else if(res==PS_END_UPLOAD)
				{
					end=1;
				}
				else
				{
					frameNum=g_recv_buff[FRAME_STA_LEN ];
					frameNum=frameNum<<8;
					frameNum=frameNum|g_recv_buff[FRAME_STA_LEN +1];
					if(frameNum>pos)
					{
						i=0;
					}
					else
					{
						i=frameNum;
					}
					end=0;
				}
				break;
			}
			else if(ret==1)
			{
				end=1;
				break;
			}
		}
		if(SendData_Tag.timerElapsedUs()>=SendData_Tag.timeout)
		{

#if DEBUG_PRINT
			debugPrint("timeout resend_times\r\n",SendData_Tag.resend_times);
#endif
			SendData_Tag.resend_times--;
			if(SendData_Tag.resend_times==0)
				break;
			else
				continue;
		}

		if( end ||(SendData_Tag.resend_times==0))
			break;
	}
}


//成功返回0，失败返回1
U8 SendData(char *name,U8 *buf,U32 len)
{

	timerDelayMs(3);
	U8 tmp[256]={0},flag=0;

	if((len>0)&&(buf)&&(name))
	{
		strcpy((char *)&tmp[4],(char *)name);
		tmp[0]=DEBUG_UPLOAD;
		tmp[1]=(len>>16)&0xff;
		tmp[2]=(len>>8)&0xff;
		tmp[3]=(len)&0xff;
		fsmSendFrame(PID_COMMAND ,0xff,&tmp[0]);

		timerDelayMs(3);
		SendData_Tag.len=len;
		SendData_Tag.buff=buf;
		fsmSendDataTask(FSM_SEND_DATA_IMAGE);
	}
	else
	{
		flag=1;
	}
	timerDelayMs(3);
	return flag;
}




static void fsmDebugResetTemplateStorage(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},buff[1];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	buff[0]=1;
	if(binlib_api(LIB_INIT_SENSOR,buff,sizeof(buff))==0)
		tmp_buf[1]=PS_OK;
	else
		tmp_buf[1]=PS_PARAM_ERROR;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


static void fsmDeleteMultiId(void)
{
	U8 tmp_buf[2]={0},buff[4];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];
	tmp_buf[1]=PS_DEL_TEMP_ERR;
	memcpy((char*)&buff[0],(char*)&g_recv_buff[FRAME_STA_LEN+1],4);
	if(binlib_api(LIB_DELETE_ID,buff,sizeof(buff))==0)
		tmp_buf[1]=PS_OK;

	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}

