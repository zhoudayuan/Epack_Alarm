/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_local_handle.c
 * @brief   本地网管处理模块
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   函数列表
 *   1. main       主函数
 *   2. init_fpga_mem_nm       映射FPGA内存
 *   3. init_ipc_socket       初始化socket
 *   4. mgrh_msg_queue_create      创建消息队列
 *   5. mgrh_pthread_create     创建线程
 *   6. pthread_mgra_mgrh_recv     网管代理模块消息接收线程函数
 *   7. pthread_adp_mgrh_recv     空口适配模块消息接收线程函数
 *   8. pthread_msg_handle     消息处理线程函数
 *   9. send_remote_nm_get_ack     发送远端网管读取响应
 *   10. send_remote_nm_set_ack     发送远端网管设置响应
 *   11. send_local_nm_get_ack     发送本地网管读取响应
 *   12. send_local_nm_set_ack     发送本地网管设置响应
 *   13. init_local_val     初始化本地全局变量
 *   14. notify_fpga     通知FPGA读取数据
 *
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>        //open
#include <sys/types.h>
#include <sys/ipc.h>	  //shmget
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/sem.h> 
#include <sys/shm.h>	  //shmget
#include <arpa/inet.h>	  //inet_addr(); inet_ntoa()
#include <sys/socket.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <time.h>
#include <memory.h>
#include <getopt.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "mgr_local_handle.h"
#include "mgr_public_func.h"

#undef TASK_NAME
#define TASK_NAME	"MGR_LOCAL_HANDLE"

struct sigaction action_axi;
void *iq_start_addr=NULL;
// 与读写Iqdata相关
static int s_SaveIqCnt     = 0;         // 写入SD的次数
static int s_SaveIqDataNum = 0;         // 写入SD的字节总数, 以兆为单位
pthread_t IqdataPid;                    // 读Iqdata数据线程
FILE * mem_fd_fp = NULL;                // SD卡/mnt/iqdata.txt 文件指针
unsigned int iq_switch_open_count = 0;  // 记录网管工具 [接收数据采集] 开启按钮 的点击次数
int iq_excess_volumn_flag = 0;          // 异常标识:     0-正常; 非0-异常(包括挂载\打开文件\写入SD超过60M失败)
unsigned int iq_switch_status = 0;      // 标识FPGA状态: 1-打开 0-关闭
unsigned int iq_mount_sd_flag = 0;      // sd卡挂载标志: 1-挂载 0-卸载
unsigned int OpenIqfd;
#define BUF_SIZE                DATA_LENGTH
#define READ_IQDATA_FILE        "/dev/axiram-iqdata"  // FPGA通知驱动后数据写入的文件
#define WRITE_IQDATA_FILE       "/mnt/iqdata.txt"     // /dev/mmcblk0p1-挂载后的文件，即最终的写入位置
#define IQ_SWITCH_ON            1
#define IQ_SWITCH_OFF           0
#define YES                     0
#define NO                     -1
#define TEST_YES                0
#define TEST_ERR               -1
#define SD_MOUNT                1
#define SD_UMOUNT               0
#define ERR_SD_OPEN            -3
#define ERR_MOERE_60M          -2
#define ERR_SD_MOUNT           -1
// 打印全局变量
#define PRINT_GLOBAL() do {\
printf("[%s:%s:%d]\
\niq_switch_status=(%d), 1-on 0-off\
\niq_mount_sd_flag=(%d), 0-umount 1-mount\
\niq_excess_volumn_flag=(%d), 0-ok,1-ERR\
\niq_switch_open_count=(%d)\
\nmem_fd_fp=(%s)\
\n\n", __FILE__,__FUNCTION__, __LINE__,\
iq_switch_status,\
iq_mount_sd_flag,\
iq_excess_volumn_flag,\
iq_switch_open_count,\
mem_fd_fp);\
}while(0)

/**
 * @var s_i4LogMsgId
 * @brief 命令行套接字
 */
static INT32 s_i4LogMsgId;
 /**
 * @var pLogTxtFd
 * @brief 文件句柄
 */
static FILE *pLogTxtFd = NULL;

#include "log.h"

SHM_IPC_STRU *ptIPCShm = NULL;

int qid_mgrh;
pthread_t tid_mgra_mgrh_recv;
pthread_t tid_adp_mgrh_recv;
pthread_t tid_msg_handle;
pthread_t tid_clear_lock_slot;
pthread_t tid_fpga_debug;
pthread_t tid_alarm_handle;
pthread_t tid_alarm_status;
pthread_t tid_alarm_send;
pthread_t tid_kill_monitor;
pthread_t tid_stun_monitor;
pthread_t tid_center_print_monitor;


struct sockaddr_in sockaddr_mgra_mgrh;
struct sockaddr_in sockaddr_mgrh_mgra;
struct sockaddr_in sockaddr_adp_mgrh;
struct sockaddr_in sockaddr_mgrh_adp;
/*手咪*/

/**
 * @var sockaddr_mgra_cc
 * @brief 网管代理向控制中心处理模块发送数据的套接字地址
 */
struct sockaddr_in sockaddr_mgra_cc;

int sock_ipc;
int sock_mgra_mgrh;
int sock_adp_mgrh;

/**
 * @var shmid_cfg
 * @brief 配置区共享内存IPC键值
 */
int shmid_cfg;	 
/**
 * @var semid_cfg
 * @brief 配置区共享内存信号量IPC键值
 */
int semid_cfg;	 
/**
 * @var shm_cfg_addr
 * @brief 配置区共享内存地址
 */
SHM_CFG_STRU * shm_cfg_addr = NULL;
/**
 * @var shmid_ipc
 * @brief 进程间通信共享内存IPC键值
 */
int shmid_ipc;	 
/**
 * @var semid_ipc
 * @brief 进程间通信共享内存信号量IPC键值
 */
int semid_ipc;	 
/**
 * @var shm_ipc_addr
 * @brief 进程间通信共享内存地址
 */
SHM_IPC_STRU * shm_ipc_addr = NULL;
/**
 * @var shmid_nm
 * @brief 网管区共享内存IPC键值
 */
int shmid_nm;	 
/**
 * @var semid_nm
 * @brief 网管区共享内存信号量IPC键值
 */
int semid_nm;	 
/**
 * @var shm_nm_addr
 * @brief 网管区共享内存地址
 */
SHM_NM_STRU * shm_nm_addr = NULL;
/**
 * @var local_cc
 * @brief 本地色码值
 */
unsigned int local_cc = 0;
/**
 * @var local_dev_id
 * @brief 本地设备ID
 */
unsigned int local_dev_id = 0;
unsigned int local_stun_flag = 0;
unsigned int local_kill_flag = 0;


int fpga_mem_nm_fd;
int eeprom_fd;
void * fpga_mem_nm_start = NULL;
MGRH_FPGA_MSG * fpga_mem_nm_w = NULL;
MGRH_FPGA_MSG * fpga_mem_nm_r = NULL;
INIT_FPGA_PARAM * fpga_mem_nm_param = NULL;
unsigned char * fpga_mem_nm_share = NULL;
unsigned char * fpga_mem_nm_alarm = NULL;
FPGA_DEBUG_PRINT * fpga_mem_nm_debug = NULL;

/********
 *中心打印开关
*/
unsigned int center_print_flag=0;
/**
 * @var timeout_cnt
 * @brief 网管命令的超时计数
 */
unsigned int timeout_cnt = 0;
unsigned int debug_timeout_cnt = 0;

EEPROM_FPGA_PARAM eeprom_fpga_param;
EEPROM_ESN_PARAM eeprom_esn_param;


/**
 * @var ALARM_PACKET_MAX
 * @brief 告警包分包最多包数
 */
#define ALARM_PACKET_MAX  2
/**
 * @var g_tMgrAlarm
 * @brief 告警表
 */
static ALARM_ITEM_SEND g_tMgrAlarm[MGR_ALARM_MAX * 3];
/**
 * @var g_auAlarmSendNum
 * @brief 一个告警包中的告警个数
 */
static UINT8 g_auAlarmSendNum[ALARM_PACKET_MAX] = {0,0};
/**
 * @var ALARM_SEND_NUM
 * @brief 普通告警个数
 */
UINT8 ALARM_SEND_NUM = 0;
/**
 * @var EALARM_SEND_NUM
 * @brief 紧急告警个数
 */
UINT8 EALARM_SEND_NUM = 0;
/**
 * @var ptMgrAlarm
 * @brief 告警表指针
 */
ALARM_ITEM *ptMgrAlarm = NULL;



// 打印IQ访问时的异常
void print_iq_excess_err()
{
    if (iq_excess_volumn_flag == ERR_SD_MOUNT)
    {
        LOG_DEBUG(s_i4LogMsgId, "\n\tFlag: SD mount\n");
//      printf("\n\tFlag: SD mount\n");
    }
    else if (iq_excess_volumn_flag == ERR_MOERE_60M)
    {
        LOG_DEBUG(s_i4LogMsgId, "\n\tFlag: OVER 60M\n");
//      printf("\n\tFlag: OVER 60M\n");
    }
    else if (iq_excess_volumn_flag == ERR_SD_OPEN)
    {
        LOG_DEBUG(s_i4LogMsgId, "\n\tFlag: OPEN /mnt/xxx\n");
//      printf("\n\tFlag: OPEN /mnt/xxx\n");

    }
    else if (iq_excess_volumn_flag == 0)
    {
        LOG_DEBUG(s_i4LogMsgId, "\n\tFlag: CORRECT\n");
//      printf("\n\tFlag: CORRECT\n");

    }
    else
    {
        LOG_DEBUG(s_i4LogMsgId, "\n\tFlag: CORRECT\n");
//      printf("\n\tFlag: CORRECT\n");
    }
}

/**
* @brief   告警表状态置位
*
* @param[in]		alarm_item 告警表位置
*
* @return		void
* @author		ztt
* @bug
*/
void MGR_Alarm_Trigger(ALARM_ITEM *alarm_item)
{
	if (alarm_item->alm_cnt < 3)
		alarm_item->alm_cnt++;
	else
	{
		alarm_item->alm_exist = 1;
		alarm_item->alm_clear = 0;
	}
	return;
}
/**
* @brief   告警表状态清除
*
* @param[in]		alarm_item 告警表位置
*
* @return		void
* @author		ztt
* @bug
*/
void MGR_Alarm_Clear(ALARM_ITEM *alarm_item)
{
	if (alarm_item->alm_cnt > 0)
		alarm_item->alm_cnt--;
	else if (1 == alarm_item->alm_exist)
	{
		alarm_item->alm_exist = 0;
		alarm_item->alm_clear = 1;
	}
	else if((alarm_item->alm_clear > 0) && (alarm_item->alm_clear < 4))//更改发送频率  故改为8
		alarm_item->alm_clear++;
	return;
}
/**
* @brief   更新告警状态
*
* @param[in]		
*
* @return		void
* @author		ztt
* @bug
*/
void MGR_Alarm_Set()
{
	int i = 0;
	ALARM_ITEM *ptAlarm = ptIPCShm->alarm_center;
	for(i=0; i<MGR_ALARM_MAX*2; i++,ptAlarm++)
	{		
		if(ptAlarm->uStatus ==1)
			MGR_Alarm_Trigger(ptAlarm);
		else
			MGR_Alarm_Clear(ptAlarm);
	}
	return;
}
/**
* @brief   生成发送告警表
*
* @param[in]		
*
* @return		void
* @author		ztt
* @bug
*/
void MGR_AlarmTable_Set()
{
	int i ,num = 0;
	ALARM_ITEM *ptAlarm = ptIPCShm->alarm_center;
	
	for(i=0; i<MGR_ALARM_MAX*2; i++,ptAlarm++)
	{
		
		if(DEF_ALM_OCCUR == ptAlarm->alm_exist)
		{
			g_tMgrAlarm[num].alm_code = ptAlarm->alm_code;
			g_tMgrAlarm[num].alm_status = DEF_ALM_OCCUR;
			num++;
		}
		else if((ptAlarm->alm_clear > 0) &&(ptAlarm->alm_clear < 4))
		{
			g_tMgrAlarm[num].alm_code = ptAlarm->alm_code;
			g_tMgrAlarm[num].alm_status = DEF_ALM_RESUME;
			num++;
		}
		else
			{}
			
	}
	ALARM_SEND_NUM = num;

	if(ptIPCShm->mgr_printf[0])
	{
		LOG_DEBUG(s_i4LogMsgId,"Alarm Num = %d",ALARM_SEND_NUM);
		ptAlarm = ptIPCShm->alarm_center;
		for(i=0; i<MGR_ALARM_MAX*2; i++,ptAlarm++)
	       {
			if(g_tMgrAlarm[i].alm_status == DEF_ALM_OCCUR)
				LOG_DEBUG(s_i4LogMsgId,"Alarm Code = %d,DEF_ALM_OCCUR",g_tMgrAlarm[i].alm_code);
			
			if(g_tMgrAlarm[i].alm_status == DEF_ALM_RESUME)
				LOG_DEBUG(s_i4LogMsgId,"Alarm Code = %d,DEF_ALM_RESUME",g_tMgrAlarm[i].alm_code);
			
		}
	}
	
	return;
}
void MGR_AlarmTable_Clear()
{
	memset((UINT8 *)g_tMgrAlarm,0x00,sizeof(g_tMgrAlarm));
	ALARM_SEND_NUM = 0;
	g_auAlarmSendNum[0] = 0;
	g_auAlarmSendNum[1] = 0;
	return;
}
/**
* @brief   告警模块提供给外部使用接口
*
* @param[in]		type		告警类型
* @param[in]		uStatus	状态
*
* @return		UINT8     0失败 1 成功
* @author		ztt
* @bug
*/
UINT8  MGR_Alarm_Update_Status(UINT32 type,UINT8 uStatus)
{
	UINT32 i;
	ALARM_ITEM *ptAlarm = ptIPCShm->alarm_center;
	
	if((2 <=  uStatus)
		||(NULL == ptAlarm))
		return 0;
	
	for(i = 0;i< MGR_ALARM_MAX*2;i++,ptAlarm++)
	{
		if(type == ptAlarm->alm_code)
		{
			ptAlarm->uStatus = uStatus;
			return 1;
		}
	}

	return 0;
}
/**
* @brief   紧急告警模块提供给外部使用接口
*
* @param[in]		type		告警类型
* @param[in]		uStatus	状态
*
* @return		UINT8     0失败 1 成功
* @author		ztt
* @bug立刻产生告警
*/
UINT8  MGR_E_Alarm_Set(UINT32 type,UINT8 uStatus)
{
	int i;
	ALARM_ITEM *ptAlarm = ptIPCShm->alarm_center;

	if(1 != uStatus)
		return 0;
	
	if(type)//是紧急告警
	{
		for(i = 0;i< MGR_ALARM_MAX*2;i++,ptAlarm++)
		{
			if(type == ptAlarm->alm_code)
			{
				ptAlarm->alm_exist  = uStatus;
				ptAlarm->uStatus = 0;
				ptAlarm->alm_cnt = 0;
				ptAlarm->alm_clear = 0;
				return 1;
			}
		}
	}
	return 0;
}
/**
 * @brief    告警组包
 *
 * @param[in]		pAlarm		告警包
 * @param[in]		type		见ALARM_TYPE
 *
 * @return		void
 * @author		ztt
 * @bug
 */
void MGR_Alarm_AI_Build(NAS_INF_DL_T *pAlarm,ALARM_ITEM_SEND *ptAlarmSrc,ALARM_TYPE type,UINT8 num)
{
	int i;
	DATA_LINK_T *ptLink = NULL;
	NAS_AI_PAYLOAD *ptPayload = NULL;
	
	if((NULL == pAlarm)
		||(NULL == ptAlarmSrc)
		||(num > 8))
	{
		//打印
		return;
	}
	

	ptLink = (DATA_LINK_T *)&(pAlarm->tDataLink[0]);
	ptPayload = (NAS_AI_PAYLOAD *)(ptLink->PayLoad);
	
	ptPayload->cmd_code = 0;//需要改
	ptPayload->src_id = shm_cfg_addr->dev_id.val;
	ptPayload->dst_id = 0x1f;
	ptPayload->op_code = OP_CODE_ALARM;
	ptPayload->nm_type = NM_TYPE_NM;
	
	for(i = 0;i < num;i++)
	{
		ptPayload->data[i] = (ptAlarmSrc->alm_code << 2) |(ptAlarmSrc->alm_status);//需要测试
	}
	ptPayload->crc = crc8((unsigned char *)ptPayload, sizeof(NAS_AI_PAYLOAD) - 1);

	pAlarm->TimeStamp = 0xffffffff;
	pAlarm->SlotNum = S_1;
	pAlarm->TxFreq1 = 1;
	pAlarm->TxFreq2 = 0;
	pAlarm->ResFrqSlot = 0;
	
	ptLink->MsgType = DI_MSG_NM;
	ptLink->CC = local_cc;
	ptLink->DataType = DI_MSG_NM;
	ptLink->DataLen = sizeof(NAS_AI_PAYLOAD);

	return;
}
/**
 * @brief   告警组包
 *
 * @param[in]		u4SocketFd		发送句柄
 *
 * @return		void
 * @author		ztt
 * @bug
 */
void MGR_Alarm_Center_Build(NM_IPC_MSG *pAlarm,ALARM_ITEM_SEND *ptAlarmSrc,ALARM_TYPE type)
{
	int i,u4SendNum = NM_IPC_MSG_PAYLOAD_SIZE + 1;
	
	if((NULL == pAlarm) ||(NULL == ptAlarmSrc))
	{
		//打印
		return;
	}

	u4SendNum = ALARM_SEND_NUM;
	pAlarm->op_code = OP_CODE_ALARM;
	
	if(u4SendNum > NM_IPC_MSG_PAYLOAD_SIZE)
	{
		//打印
		return;
	}
		
	pAlarm->cmd_code = 0;//需要改
	pAlarm->src_id = shm_cfg_addr->dev_id.val;
	pAlarm->dst_id = 0xff;
	pAlarm->nm_type = NM_TYPE_CENTER;
	
	for(i = 0;i < u4SendNum;i++,ptAlarmSrc++)
	{
		pAlarm->payload[i] = (ptAlarmSrc->alm_code << 2) |(ptAlarmSrc->alm_status);//需要测试
	}

	if(ptIPCShm->mgr_printf[0])
	{	
		LOG_DEBUG(s_i4LogMsgId,"[MGR_Alarm_Center_Send][Alarm Num=%d]",ALARM_SEND_NUM);
	}
	
	return;
}
/**
 * @brief   获取普通告警分包个数
 *
 * @param[in]		void		
 *
 * @return		void
 * @author		ztt
 * @bug普通告警消息，可能出现分包情况
 */
void  MGR_Alarm_Packet_Num(UINT8 *pPacketNum,ALARM_TYPE type)
{
	if(type == ALARM_TYPE_NORMAL)
	{
		*pPacketNum = ALARM_SEND_NUM/8;
		if(ALARM_SEND_NUM%8)
			*pPacketNum = (*pPacketNum ) +1;

	       if(1 == *pPacketNum)
		{
			g_auAlarmSendNum[0] = ALARM_SEND_NUM;
		}
		else if(2 == *pPacketNum)
		{
			g_auAlarmSendNum[0] = 8;
			g_auAlarmSendNum[1] = (ALARM_SEND_NUM%8 == 0) ? 8 :  ALARM_SEND_NUM%8;
		}
		else
		{
			*pPacketNum = 0;
		}
	}
	else
	{
		
	}

	return ;
}
/**
 * @brief   告警状态空口发送
 *
 * @param[in]		u4SocketFd		发送句柄
 * @param[in]		type		见ALARM_TYPE
 * @param[in]		ptAlarmSrc		告警表数据源
 *
 * @return		void
 * @author		ztt
 * @bug
 */
void MGR_Alarm_AI_Send(UINT32 u4SocketFd,ALARM_TYPE type,ALARM_ITEM_SEND *ptAlarmSrc)
{
	
	NAS_INF_DL_T tMGRAlarm;
	UINT8 i,u4Num ;
	
	MGR_Alarm_Packet_Num(&u4Num,type);

	if(ptIPCShm->mgr_printf[0])
	{
		LOG_DEBUG(s_i4LogMsgId,"[MGR_Alarm_AI_Send][%d Packet will be send AlarmNum1=%d AlarmNum2=%d]",u4Num,g_auAlarmSendNum[0],g_auAlarmSendNum[1]);
	}
	
	for(i = 0;i<u4Num;i++)
	{
		memset(&tMGRAlarm,0x00,sizeof(NAS_INF_DL_T));
		MGR_Alarm_AI_Build(&tMGRAlarm,&ptAlarmSrc[i*8],type,g_auAlarmSendNum[i]);//测试
		sendto(u4SocketFd, &tMGRAlarm, sizeof(tMGRAlarm), 0, (struct sockaddr *)&sockaddr_mgrh_adp, sizeof(sockaddr_mgrh_adp));
	}
	
	return;
}
/**
 * @brief   告警状态中心发送
 *
 * @param[in]		void
 *
 * @return		void
 * @author		ztt
 * @bug给中心的告警可以一包处理
 */
void MGR_Alarm_Center_Send(UINT32 u4SocketFd,ALARM_TYPE type,ALARM_ITEM_SEND *ptAlarmSrc)
{
	if(ALARM_SEND_NUM == 0)
	{
		if(ptIPCShm->mgr_printf[0])
		{
			LOG_DEBUG(s_i4LogMsgId,"[MGR_Alarm_Center_Send][Alarm Num=0]");
		}
		return;
	}
	
	NM_IPC_MSG tMGRAlarm;
	memset(&tMGRAlarm,0x00,sizeof(NM_IPC_MSG));
	MGR_Alarm_Center_Build(&tMGRAlarm,ptAlarmSrc,type);
	sendto(u4SocketFd, &tMGRAlarm, sizeof(tMGRAlarm), 0, (struct sockaddr *)&sockaddr_mgrh_mgra, sizeof(sockaddr_mgrh_mgra));
	return;
}
/**
 * @brief   告警状态发送
 *
 * @param[in]		u4SocketFd		发送句柄
 *
 * @return		void
 * @author		ztt
 * @bug
 */
void MGR_Alarm_Send(UINT32 u4SocketFd)
{
	
	if(ptIPCShm->is_connect_cc)//中心
	{
		//网管包
		MGR_Alarm_Center_Send(u4SocketFd,ALARM_TYPE_NORMAL,g_tMgrAlarm);
	}
	else
	{
		//空口数据包	
		MGR_Alarm_AI_Send(u4SocketFd,ALARM_TYPE_NORMAL,g_tMgrAlarm);
	}
			
	return;
}
/**
 * @brief   获取共享内存首地址
 *
 * @param[in]		u4SocketFd		发送句柄
 *
 * @return		void
 * @author		ztt
 * @bug
 */
void * MGR_Get_Shm(int id, size_t size)
{
 int i4ShmId;
 void *shmaddr = NULL;
 key_t key;

 key = ftok(FTOK_F_NAME, id);
 if (key < 0)
 {
    printf("key error!!\n");
  return NULL;
 }

  i4ShmId = shmget(key, 0, 0);
  if (-1 == i4ShmId)  
  {
  	 printf("shmget error!!\n");
    	return NULL;
  }
   
 shmaddr = shmat(i4ShmId, NULL, 0);
  if (-1 == (int)(shmaddr))
  {
    printf("shmat error!!\n");
    return NULL;
  }
 return shmaddr;
}

int main(void)
{
	key_t msqKey;
	//log写文件
	pLogTxtFd = fopen("./LOG.txt","r+");
	if(NULL == pLogTxtFd)
	{
		printf("[ROUTE]fopen Log.txt fail!!!\n");
		return -1;
	}
	fseek(pLogTxtFd,0,SEEK_END);                
	//log日志输出
	if((msqKey = ftok("/", LOG_MSG_ID)) == -1)
	{
		LOG_WFile(pLogTxtFd,"[Route_Init]Creat msqKey Error\n");
		return -1;
	}
	s_i4LogMsgId = msgget(msqKey, IPC_EXCL);  /*检查消息队列是否存在*/
	if(s_i4LogMsgId < 0)
	{
		LOG_WFile(pLogTxtFd,"[Route_Init]failed to get msq\n");
	    return -1;
	}
	
	timeout_cnt = NM_TIMEOUT_TIME / POLLING_INTERVA;
	debug_timeout_cnt = NM_DEBUG_TIMEOUT_TIME / POLLING_INTERVA;
	
	if (shm_cfg_attch())
	{
		printf("<%s> <%s> <%d> shm_cfg_attch err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	if (shm_ipc_attch())
	{
		printf("<%s> <%s> <%d> shm_ipc_attch err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	
	init_local_val();
/*
	if (init_eeprom())
	{
		printf("<%s> <%s> <%d> init_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
*/ 
	if (init_fpga_mem_nm())
	{
		printf("<%s> <%s> <%d> init_fpga_mem_nm err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
    
    if (init_fpga_mem_iq())
    {
        printf("<%s> <%s> <%d> init_fpga_mem_iq err\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
	if (init_fpga_param())
	{
		printf("<%s> <%s> <%d> init_fpga_param err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	
	if (init_ipc_socket())
	{
		printf("<%s> <%s> <%d> init_ipc_socket err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	
	if (mgrh_msg_queue_create())
	{
		printf("<%s> <%s> <%d> mgrh_msg_queue_create err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	
	if (init_alarm_table())
	{
		printf("init_alarm_table err\n");
		return -1;
	}

	if (mgrh_pthread_create())
	{
		printf("<%s> <%s> <%d> mgrh_pthread_create err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	net_config();
	read_fpga_version();
	while(1)
	{
		sleep(10);
	}
		
	return 0;
}

void read_fpga_version(void)
{
    sem_cfg_p();
	memset(shm_cfg_addr->fpga_version.buf,0,sizeof(shm_cfg_addr->fpga_version.buf));
	snprintf(shm_cfg_addr->fpga_version.buf,sizeof(shm_cfg_addr->fpga_version.buf),"V%d.%d.%02d.%03d",fpga_mem_nm_share[0],fpga_mem_nm_share[1],fpga_mem_nm_share[2],fpga_mem_nm_share[3]);
	sem_cfg_v();
	sleep(1);
	printf("**************** VERSION INFO ****************\n");
	printf("E-PACK VERSION : %s\n", shm_cfg_addr->s_version.buf);
	printf("FPGA VERSION   : %s\n", shm_cfg_addr->fpga_version.buf);
	printf("**********************************************\n");
	
}
int init_eeprom(void)
{
 	eeprom_fd = open("/dev/i2c-0",O_RDWR);
	if (eeprom_fd < 0)
	{
		printf("Open axiram-nm error!\n");
        return -1;
	}
	return 0;
}


/**
 * @brief   获取文件大小
 *
 * @author  周大元
 * @since
 * @bug
*/
int get_file_size(const char *path)
{
    struct stat statbuff;
    if (path == NULL)
    {
        printf("path is NULL\n");
        exit(0);
    }
    return (stat(path, &statbuff) < 0) ? -1: statbuff.st_size;
}


/**
 * @brief   判断文件是否存在
 *
 * @author  周大元
 * @since
 * @bug
*/
int is_file_exist(const char *path)
{
    if (path == NULL)
    {
        printf("path is NULL\n");
        exit(0);
    }

    if (access(path, F_OK) == 0)
    {
        return 0;
    }
    else
    {
        perror(WRITE_IQDATA_FILE">");
        return -1;
    }
}



/**
 * @brief   检查SD当前的挂载状态，如果没有挂载则挂载
 *          只有没插卡才返回挂载失败。
 * @author  周大元
 * @since
 * @bug
*/
int is_sd_mount_ok()
{
    int status;

    if (is_file_exist(WRITE_IQDATA_FILE) == 0)  // 确定SD文件是否存在
    {
        // SD卡中文件存在
        printf("[%s:%d] the SD file (%s) is exist \n", __FUNCTION__, __LINE__, WRITE_IQDATA_FILE);
        return  SD_MOUNT;  // 如果存在说明 mount 成功。
    }
    else
    {
        // SD卡中文件不存在
        printf("[%s:%d] the SD file (%s) not exist, mount now... \n", __FUNCTION__, __LINE__, WRITE_IQDATA_FILE);
        status = system("mount /dev/mmcblk0p1 /mnt");
        if (test_code(status) == TEST_YES)
        {
            iq_mount_sd_flag = SD_MOUNT;  // mount成功
        }
        else
        {
            iq_mount_sd_flag = SD_UMOUNT;  // mount失败，说明没插卡
            send_iq_close_to_queque(ERR_SD_MOUNT);     // 通知FPGA设置当前状态
        }
        return  iq_mount_sd_flag;
    }
}


/**
 * @brief   通过检查各种状态，判断是否可以往SD卡中写数据
 *
 * @author  周大元
 * @since
 * @bug
*/
int is_iqdata_write_ready()
{
    if (iq_switch_open_count == 0)  // 开启按钮点击次多，过滤多余次数
    {
        iq_switch_open_count++;
        s_SaveIqDataNum = 0;

        // 判断文件是否挂载成功
        if (is_sd_mount_ok() != SD_MOUNT)
        {
            return NO;
        }

        // 以清零的方式打开SD中的准写入文件
        if ((mem_fd_fp = fopen(WRITE_IQDATA_FILE, "wb")) == NULL)
        {
            printf("open /mnt/iqdata.txt ERR\n");
            fclose(mem_fd_fp);                // 先关闭
            system("umount /mnt");            // 后卸载
            iq_mount_sd_flag = SD_UMOUNT;     // 然后再设置标志
            send_iq_close_to_queque(ERR_SD_OPEN);        // 通知FPGA异常
            return NO;
        }
        printf("open /mnt/iqdata.txt OK\n");
    }
    return YES;
}



/**
 * @brief   从底层读出Iqdata写入SD卡中
 *
 * @author  周大元
 * @since
 * @bug
*/
void *get_iqdata_sd_task(void *p)
{
    char buf[BUF_SIZE];
    int len;
    int  ReadNum;
    int  WriteNum;
    int  cnt = 4;

    while (1)
    {
        // 读iqdate, 从/dev/axiram-iqdata，一次性读出BUF_SIZE字节存入buf。
        if ((ReadNum = read(OpenIqfd, buf, sizeof(buf))) !=  BUF_SIZE)
        {
            LOG_ERROR(s_i4LogMsgId, "[%s:%s:%d]\n\t ReadNum=%d\n", __FILE__, __FUNCTION__, __LINE__, ReadNum);
            perror("read fd>");
            if (0 == cnt--)
            {
                break;
            }
            else
            {
                sleep(1);
            }
            continue;
        }

        // 开关必须打开才存数据， 过滤掉无效数据
        if (iq_switch_status != IQ_SWITCH_ON)
        {
            continue;
        }

        // SD是否准备好被写入Iqdata数据
        if (is_iqdata_write_ready() != YES)
        {
            LOG_WARNING(s_i4LogMsgId, "[%s:%s:%d]\n\t IQDATA not READY\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        }

        // 只有当SD挂载成功, 程序才能执行到此
        if (mem_fd_fp != NULL)  // 确定SD卡中文件已经打开
        {
            len = ftell(mem_fd_fp); // 读取SD中iqdata.txt文件大小
            if ((len + DATA_LENGTH) > MAX_VOLUMN_IQ_DATA) // 大于指定数据大小不写入
            {

                printf("save iq data excess 60M \n");
                if (mem_fd_fp != NULL)
                {
                    fclose(mem_fd_fp);
                    system("umount /mnt");
                    iq_mount_sd_flag = SD_UMOUNT;
                    mem_fd_fp = NULL;
                }
                send_iq_close_to_queque(ERR_MOERE_60M);
            }
            else
            {
                // 写iqdate, 将buf中的数据一次性写BUF_SIZE个字节到/mnt/iqdata.txt中
                if ((WriteNum = fwrite(buf, ReadNum, 1, mem_fd_fp)) == 1)
                {
                    if (s_SaveIqCnt++ > 59)  // 每写入59次对应写入1M数据到SD卡
                    {
                        s_SaveIqCnt = 0;
                        printf("save iq data %dM\n", (++s_SaveIqDataNum));
                    }
                }
                else
                {
                    perror("fwrite sd>");
                    printf("save iq data err\n");
                }
            }
        }
    }
    pthread_exit(NULL);
}

/**
 * @brief   创建获取IQdata线程
 *
 * @author  周大元
 * @since
 * @bug
*/
void creat_get_iqdata_task()
{
    int OpenFlags = O_RDWR;

    // 打开 /dev/axiram-iqdata, 驱动会往这个文件写数据
    if ((OpenIqfd = open(READ_IQDATA_FILE, OpenFlags)) < 0)
    {
        printf("Open (%s) error!\n", READ_IQDATA_FILE);
        LOG_ERROR(s_i4LogMsgId, "[%s:%s:%d]\n\t open(%s) ERR\n", __FILE__, __FUNCTION__, __LINE__, READ_IQDATA_FILE);
        perror("open>");
        exit(0);
    }

    // 创建读写IQdata线程
    if (pthread_create(&IqdataPid, NULL, get_iqdata_sd_task, NULL) != 0)
    {
        LOG_ERROR(s_i4LogMsgId, "[%s:%s:%d]\n\t pthread create ERR\n", __FILE__, __FUNCTION__, __LINE__);
        perror("create>");
        exit(0);
    }

    pthread_detach(IqdataPid);
}



int init_fpga_mem_iq(void)
{
    creat_get_iqdata_task();  // 正式程序
    return 0;
}




int init_fpga_mem_nm(void)
{
	if((fpga_mem_nm_fd = open("/dev/axiram-nm",O_RDWR)) < 0)
	{
		printf("Open axiram-nm error!\n");
        return -1;
	}

    fpga_mem_nm_start = mmap(NULL, FPGA_MEM_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fpga_mem_nm_fd, FPGA_MEM_BASE_ADDR);
    
    if(fpga_mem_nm_start == NULL) 
    {
        printf("mmap err, fpga_mem_nm_start is NULL!\n"); 
        close(fpga_mem_nm_fd);
        return -1; 
    }

    if(fpga_mem_nm_start == (void *)-1)
    {
        printf("mmap err, return value is -1!\n"); 
        close(fpga_mem_nm_fd);
        return -1; 
    } 

	fpga_mem_nm_w = (MGRH_FPGA_MSG *)fpga_mem_nm_start;
	fpga_mem_nm_r = (MGRH_FPGA_MSG *)((unsigned char *)fpga_mem_nm_start + 512);
	fpga_mem_nm_param = (INIT_FPGA_PARAM *)((unsigned char *)fpga_mem_nm_start + 1024);
	fpga_mem_nm_share = (unsigned char *)fpga_mem_nm_start + 2048;
	fpga_mem_nm_alarm = (unsigned char *)fpga_mem_nm_start + 2048 + 256;
	fpga_mem_nm_debug = (FPGA_DEBUG_PRINT *)((unsigned char *)fpga_mem_nm_start + 3072);

	/*printf("\nfpga_mem_nm_w = %p\n", fpga_mem_nm_w);
	printf("fpga_mem_nm_r = %p\n", fpga_mem_nm_r);
	printf("fpga_mem_nm_param = %p\n", fpga_mem_nm_param);
	printf("fpga_mem_nm_share = %p\n", fpga_mem_nm_share);
	printf("fpga_mem_nm_alarm = %p\n", fpga_mem_nm_alarm);
	printf("fpga_mem_nm_debug = %p\n", fpga_mem_nm_debug);*/

	return 0;
}


int init_ipc_socket(void)
{
	memset(&sockaddr_mgra_mgrh, 0, sizeof(struct sockaddr_in));
	sockaddr_mgra_mgrh.sin_family = AF_INET;
	sockaddr_mgra_mgrh.sin_port = htons(SOCK_PORT_MGRA_MGRH);
	sockaddr_mgra_mgrh.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_mgrh_mgra, 0, sizeof(struct sockaddr_in));
	sockaddr_mgrh_mgra.sin_family = AF_INET;
	sockaddr_mgrh_mgra.sin_port = htons(SOCK_PORT_MGRH_MGRA);
	sockaddr_mgrh_mgra.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_adp_mgrh, 0, sizeof(struct sockaddr_in));
	sockaddr_adp_mgrh.sin_family = AF_INET;
	sockaddr_adp_mgrh.sin_port = htons(SOCK_PORT_ADP_MGRH);
	sockaddr_adp_mgrh.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_mgrh_adp, 0, sizeof(struct sockaddr_in));
	sockaddr_mgrh_adp.sin_family = AF_INET;
	sockaddr_mgrh_adp.sin_port = htons(SOCK_PORT_MGR_ADP);
	sockaddr_mgrh_adp.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	memset(&sockaddr_mgra_cc, 0, sizeof(struct sockaddr_in));
	sockaddr_mgra_cc.sin_family = AF_INET;
	sockaddr_mgra_cc.sin_port = htons(SOCK_PORT_MGRA_CC);
	sockaddr_mgra_cc.sin_addr.s_addr = inet_addr("127.0.0.1");

	sock_ipc = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_ipc) 
	{
		printf("sock_ipc socket error\n");
		return -1;
	}

	sock_mgra_mgrh = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_mgra_mgrh) 
	{
		printf("sock_mgra_mgrh socket error\n");
		return -1;
	}
    if (-1 == bind(sock_mgra_mgrh, (struct sockaddr *)(&sockaddr_mgra_mgrh), sizeof(struct sockaddr_in))) 
    {
        printf("sock_mgra_mgrh bind error\n");
        return -1;
    }   

	sock_adp_mgrh = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_adp_mgrh) 
	{
		printf("sock_adp_mgrh socket error\n");
		return -1;
	}
    if (-1 == bind(sock_adp_mgrh, (struct sockaddr *)(&sockaddr_adp_mgrh), sizeof(struct sockaddr_in))) 
    {
        printf("sock_adp_mgrh bind error\n");
        return -1;
    }   

	return 0;
}


int mgrh_msg_queue_create(void)
{
	key_t key;

	key = ftok(FTOK_F_NAME, FTOK_ID_Q_MGRH);
	qid_mgrh = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
	if (-1 == qid_mgrh)
	{
		return -1;
	}

	return 0;
}


int mgrh_pthread_create(void)
{
	if (pthread_create(&tid_mgra_mgrh_recv, NULL, pthread_mgra_mgrh_recv, NULL)) 
	{
		printf("pthread_create tid_mgra_mgrh_recv err\n");
		return -1;
	}
	pthread_detach(tid_mgra_mgrh_recv);

	if (pthread_create(&tid_adp_mgrh_recv, NULL, pthread_adp_mgrh_recv, NULL)) 
	{
		printf("pthread_create tid_adp_mgrh_recv err\n");
		return -1;
	}
	pthread_detach(tid_adp_mgrh_recv);

	if (pthread_create(&tid_msg_handle, NULL, pthread_msg_handle, NULL)) 
	{
		printf("pthread_create tid_msg_handle err\n");
		return -1;
	}
	pthread_detach(tid_msg_handle);
	
	if (pthread_create(&tid_alarm_handle, NULL, pthread_func_alarm_handle, NULL)) 
	{
		printf("pthread_create pthread_func_alarm_handle err\n");
		return -1;
	}
	pthread_detach(tid_alarm_handle);

	if (pthread_create(&tid_alarm_status, NULL, pthread_func_alarm_status, NULL)) 
	{
		printf("pthread_create pthread_func_alarm_status err\n");
		return -1;
	}
	pthread_detach(tid_alarm_status);

    if (pthread_create(&tid_alarm_send, NULL, pthread_func_alarm_send, NULL)) 
	{
		printf("pthread_create pthread_func_alarm_send err\n");
		return -1;
	}
	pthread_detach(tid_alarm_send);

	if (pthread_create(&tid_fpga_debug, NULL, pthread_fpga_debug, NULL)) 
	{
		printf("pthread_create tid_fpga_debug err\n");
		return -1;
	}
	pthread_detach(tid_fpga_debug);	

    if (pthread_create(&tid_kill_monitor, NULL, pthread_func_kill_monitor, NULL)) 
	{
		printf("pthread_create pthread_func_kill_monitor err\n");
		return -1;
	}
	pthread_detach(tid_kill_monitor);

    if (pthread_create(&tid_stun_monitor, NULL, pthread_func_stun_monitor, NULL)) 
	{
		printf("pthread_create pthread_func_stun_monitor err\n");
		return -1;
	}
	pthread_detach(tid_stun_monitor);

	if (pthread_create(&tid_center_print_monitor, NULL, pthread_func_center_print_monitor, NULL)) 
	{
		printf("pthread_create pthread_func_center_print_monitor err\n");
		return -1;
	}
	pthread_detach(tid_center_print_monitor);

	return 0;
}


void * pthread_mgra_mgrh_recv(void *arg)
{
	int ret;
	int rd_cnt;
	fd_set fds;
	QUEUE_MSG q_msg;
	
	while (1)
	{
        FD_ZERO(&fds);
        FD_SET(sock_mgra_mgrh, &fds);
		struct timeval tv;    
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        ret = select(sock_mgra_mgrh + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) 
        {
            printf("<%s> <%s> <%d> select error!\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        } 
        else if (0 == ret) 
        {
            continue;       
        }

		if (FD_ISSET(sock_mgra_mgrh, &fds)) 
		{
			q_msg.mtype = MSG_TYPE_CMD;
			rd_cnt = recvfrom(sock_mgra_mgrh, &q_msg.ipc_msg, sizeof(NM_IPC_MSG), 0, NULL, NULL);
			if (rd_cnt < 0)
			{
				continue;
			}

			if (shm_ipc_addr->mgr_printf[1])
			{
				LOG_DEBUG(s_i4LogMsgId,\
					    "\n |%s||%s||%d|\n"
						"\r nm_type  : 0x%X\n"
						"\r src_id   : 0x%X\n"
						"\r dst_id   : 0x%X\n"
						"\r cmd_code : 0x%X\n"
						"\r op_code  : 0x%X\n",\
						__FILE__, __FUNCTION__, __LINE__,\
						q_msg.ipc_msg.nm_type, q_msg.ipc_msg.src_id, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.op_code);
			}
			
			if (-1 == msgsnd(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0))
			{
				printf("<%s> <%s> <%d> qid_mgrh msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
			}
		}
	}
	
	pthread_exit(NULL);
}


void * pthread_adp_mgrh_recv(void *arg)
{
	int ret;
	int rd_cnt;
	fd_set fds;
	NAS_INF_UL_T recv_ai_msg;
	NAS_INF_DL_T send_ai_msg;
	NAS_AI_PAYLOAD nas_ai_payload;
	QUEUE_MSG q_msg;
	
	while (1)
	{
        FD_ZERO(&fds);
        FD_SET(sock_adp_mgrh, &fds);
		struct timeval tv;    
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        ret = select(sock_adp_mgrh + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) 
        {
            printf("<%s> <%s> <%d> select error!\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        } 
        else if (0 == ret) 
        {
            continue;       
        }

		if (FD_ISSET(sock_adp_mgrh, &fds)) 
		{
			rd_cnt = recvfrom(sock_adp_mgrh, &recv_ai_msg, sizeof(NAS_INF_UL_T), 0, NULL, NULL);
			if (rd_cnt < 0)
			{
				printf("<%s> <%s> <%d> sock_adp_mgrh recvfrom err\n", __FILE__, __FUNCTION__, __LINE__);
				continue;
			}

			if (recv_ai_msg.RxFreq1 == 0x01)
			{
				memcpy(&nas_ai_payload, recv_ai_msg.tDataLink[0].PayLoad, sizeof(NAS_AI_PAYLOAD));
			}
			else
			{
				memcpy(&nas_ai_payload, recv_ai_msg.tDataLink[1].PayLoad, sizeof(NAS_AI_PAYLOAD));
			}

			if (nas_ai_payload.dst_id == local_dev_id)
			{
				if (nas_ai_payload.crc != crc8((unsigned char *)&nas_ai_payload, sizeof(NAS_AI_PAYLOAD) - 1))
				{
					printf("<%s> <%s> <%d> crc8 err\n", __FILE__, __FUNCTION__, __LINE__);
					continue;
				}
				
				q_msg.mtype = MSG_TYPE_CMD;
				q_msg.ipc_msg.nm_type = nas_ai_payload.nm_type;
				q_msg.ipc_msg.src_id = nas_ai_payload.src_id;
				q_msg.ipc_msg.dst_id = nas_ai_payload.dst_id;
				q_msg.ipc_msg.cmd_code = nas_ai_payload.cmd_code;
				q_msg.ipc_msg.op_code = nas_ai_payload.op_code;
				memcpy(q_msg.ipc_msg.payload, nas_ai_payload.data, sizeof(nas_ai_payload.data));
				
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId,\
							"\n |%s||%s||%d|\n"
							"\r nm_type  : 0x%X\n"
							"\r src_id	 : 0x%X\n"
							"\r dst_id	 : 0x%X\n"
							"\r cmd_code : 0x%X\n"
							"\r op_code  : 0x%X\n",\
							__FILE__, __FUNCTION__, __LINE__,\
							q_msg.ipc_msg.nm_type, q_msg.ipc_msg.src_id, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.op_code);
				}

				usleep(shm_cfg_addr->locking_time.val * 1000 + 120000); //强制延时大于保护时间后再执行网管命令，避开当前业务保护时间
				
				if (-1 == msgsnd(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0))
				{
					printf("<%s> <%s> <%d> qid_mgrh msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
				}
			}
			else
			{
				if (recv_ai_msg.RxFreq1 == 0x01)
				{
					if (recv_ai_msg.SlotNum == 0x00)
					{
						send_ai_msg.TxFreq1 = 0x01;
						send_ai_msg.TxFreq2 = 0x00;
						send_ai_msg.SlotNum = 0x01;
						memcpy(&send_ai_msg.tDataLink[0], &recv_ai_msg.tDataLink[0], sizeof(DATA_LINK_T));
					}
					else
					{
						send_ai_msg.TxFreq1 = 0x00;
						send_ai_msg.TxFreq2 = 0x01;
						send_ai_msg.SlotNum = 0x00;
						memcpy(&send_ai_msg.tDataLink[1], &recv_ai_msg.tDataLink[0], sizeof(DATA_LINK_T));
					}
				}
				else
				{
					if (recv_ai_msg.SlotNum == 0x00)
					{
						send_ai_msg.TxFreq1 = 0x00;
						send_ai_msg.TxFreq2 = 0x01;
						send_ai_msg.SlotNum = 0x01;
						memcpy(&send_ai_msg.tDataLink[1], &recv_ai_msg.tDataLink[1], sizeof(DATA_LINK_T));
					}
					else
					{
						send_ai_msg.TxFreq1 = 0x01;
						send_ai_msg.TxFreq2 = 0x00;
						send_ai_msg.SlotNum = 0x00;
						memcpy(&send_ai_msg.tDataLink[0], &recv_ai_msg.tDataLink[1], sizeof(DATA_LINK_T));
					}
				}

				send_ai_msg.TimeStamp += 2;
				send_ai_msg.ResFrqSlot = 0;

				sendto(sock_ipc, &send_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgrh_adp), sizeof(struct sockaddr_in)); 
			}
		}
	}
	
	pthread_exit(NULL);
}


void * pthread_msg_handle(void *arg)
{
	int ret;
	unsigned int i = 0;
	QUEUE_MSG q_msg;
	struct timeval delay; 
	unsigned int save_id = 0;
	unsigned int addr_981 = 0;
	unsigned int addr_998 = 0;
	unsigned int save_val = 0;
	unsigned int tmp_val = 0;
	unsigned int tmp_freq = 0;
	unsigned int tmp_locking_time = 0;
	unsigned int call_time_out = 0;
	unsigned int tmp_neighbor_period=0;
	unsigned int fpga_param[61];
	unsigned int ip_len=0;
	unsigned int mask_len=0;
	unsigned int gateway_len=0;
	char fpga_version[50];
    char tmp_mac[50];
	NM_COMBINED_DATA combined_data;
	CENTER_DATA  center_data;
	char tmp_ip[50];
	unsigned int scan_mode=0;
	unsigned int freq_offset=0;
	unsigned char scan[8]={0};
	unsigned char scan_tran[2]={0};
	unsigned int scan_flag=0; 
    unsigned int save_data_switch=0;
	FILE * fd;
	while(1)
	{
		ret = msgrcv(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0, 0);
		if (-1 == ret)
		{
			printf("<%s> <%s> <%d> qid_mgrh msgrcv err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
			continue;
		}

		if (shm_ipc_addr->mgr_printf[1])
		{
			LOG_DEBUG(s_i4LogMsgId,\
					"\n |%s||%s||%d|\n"
					"\r nm_type  : 0x%X\n"
					"\r src_id	 : 0x%X\n"
					"\r dst_id	 : 0x%X\n"
					"\r cmd_code : 0x%X\n"
					"\r op_code  : 0x%X\n",\
					__FILE__, __FUNCTION__, __LINE__,\
					q_msg.ipc_msg.nm_type, q_msg.ipc_msg.src_id, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.op_code);
		}
		
		switch (q_msg.ipc_msg.cmd_code)
		{
			case CMD_CODE_DEV_FREQ:

				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center freq=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->freq.val);
					}
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(shm_cfg_addr->freq.val));
				}
				else
				{
					fpga_mem_nm_r->flag = FLAG_CLR;
					fpga_mem_nm_w->flag = FLAG_OK;
					fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
					fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
					if (q_msg.ipc_msg.op_code == OP_CODE_SET)
					{
						memcpy((unsigned char *)&tmp_freq, q_msg.ipc_msg.payload, sizeof(unsigned int));
						tmp_val = tmp_freq * 2;
						memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
						if(1==shm_cfg_addr->scan_mode.val)
						{
							tmp_val = tmp_freq + 73336250 + 12500;
						}
						else
						{
							tmp_val = tmp_freq + 73336250 + 25000;
						}

						memcpy(fpga_mem_nm_w->payload + sizeof(unsigned int), (unsigned char *)&tmp_val, sizeof(unsigned int));
					}
					notify_fpga();
					
					for (i = 0; i < timeout_cnt; i++)
					{
						delay.tv_sec = 0;
						delay.tv_usec = POLLING_INTERVA;
						select(0, NULL, NULL, NULL, &delay);
						if (FLAG_OK == fpga_mem_nm_r->flag)
						{
							if (shm_ipc_addr->mgr_printf[1])
							{
								LOG_DEBUG(s_i4LogMsgId,\
										"\n |%s||%s||%d|\n"
										"\r read fpga - flag	 : 0x%X\n"
										"\r read fpga - cmd_code : 0x%X\n"
										"\r read fpga - op_code  : 0x%X\n",\
										__FILE__, __FUNCTION__, __LINE__,\
										fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
							}
							if (q_msg.ipc_msg.src_id == local_dev_id)
							{
								if (q_msg.ipc_msg.op_code == OP_CODE_GET)
								{
									memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
									tmp_val = tmp_val / 2;
									send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&tmp_val);
								}
								else
								{
									if (shm_cfg_addr->freq.val != tmp_freq)
									{
										sem_cfg_p();
										shm_cfg_addr->freq.val = tmp_freq;
										sem_cfg_v();
										save_ini_file();
										
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save freq = %u\n", __FILE__, __FUNCTION__, __LINE__, tmp_freq);
										}
									}
									else
									{
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
										}
									}
									send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
								}
							}
							else
							{
								if (q_msg.ipc_msg.op_code == OP_CODE_GET)
								{
									memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
									tmp_val = tmp_val / 2;
									send_remote_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id, (unsigned char *)&tmp_val);
								}
								else
								{
									if (shm_cfg_addr->freq.val != tmp_freq)
									{
										sem_cfg_p();
										shm_cfg_addr->freq.val = tmp_freq;
										sem_cfg_v();
										save_ini_file();
										
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save freq = %u\n", __FILE__, __FUNCTION__, __LINE__, tmp_freq);
										}
									}
									else
									{
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
										}
									}
									send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
								}
							}
							if((q_msg.ipc_msg.op_code==OP_CODE_SET)&&
								(q_msg.ipc_msg.nm_type==NM_TYPE_NM))
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center freq=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->freq.val);
								}
								send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
							}
							break;
						}
					}
					if (i == timeout_cnt)
					{
						printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
					}
				}
				break;
			case CMD_CODE_DEV_POWER:

				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center power=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->power.val);
					}
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(shm_cfg_addr->power.val));
				}
				else
				{
				    
					fpga_mem_nm_r->flag = FLAG_CLR;
					fpga_mem_nm_w->flag = FLAG_OK;
					fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
					fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
					if (q_msg.ipc_msg.op_code == OP_CODE_SET)
					{
						memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
					}
					notify_fpga();
					
					for (i = 0; i < timeout_cnt; i++)
					{
						delay.tv_sec = 0;
						delay.tv_usec = POLLING_INTERVA;
						select(0, NULL, NULL, NULL, &delay);
						if (FLAG_OK == fpga_mem_nm_r->flag)
						{
							if (shm_ipc_addr->mgr_printf[1])
							{
								LOG_DEBUG(s_i4LogMsgId,\
										"\n |%s||%s||%d|\n"
										"\r read fpga - flag	 : 0x%X\n"
										"\r read fpga - cmd_code : 0x%X\n"
										"\r read fpga - op_code  : 0x%X\n",\
										__FILE__, __FUNCTION__, __LINE__,\
										fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
							}
							if (q_msg.ipc_msg.src_id == local_dev_id)
							{
								if (q_msg.ipc_msg.op_code == OP_CODE_GET)
								{
									send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
								}
								else
								{
									memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
									if (shm_cfg_addr->power.val != tmp_val)
									{
										sem_cfg_p();
										shm_cfg_addr->power.val = tmp_val;
										sem_cfg_v();
										save_ini_file();
										
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save power = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->power.val);
										}
									}
									else
									{
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
										}
									}
									send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
								}
							}
							else
							{
								if (q_msg.ipc_msg.op_code == OP_CODE_GET)
								{
									send_remote_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id, fpga_mem_nm_r->payload);
								}
								else
								{
									memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
									if (shm_cfg_addr->power.val != tmp_val)
									{
										sem_cfg_p();
										shm_cfg_addr->power.val = tmp_val;
										sem_cfg_v();
										save_ini_file();
										
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save power = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->power.val);
										}
									}
									else
									{
										if (shm_ipc_addr->mgr_printf[1])
										{
											LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
										}
									}
									send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
								}
							}
							if((q_msg.ipc_msg.op_code==OP_CODE_SET)&&
								(q_msg.ipc_msg.nm_type==NM_TYPE_NM))
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center power=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->power.val);
								}
								send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
							}
							break;
						}
					}
					if (i == timeout_cnt)
					{
						printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
					}
				}
				break;
			case CMD_CODE_START_NEIGHBOR:

				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center start_neighbor=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->start_neighbor.val);
					}
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(shm_cfg_addr->start_neighbor.val));
				}
				else
				{
				    if (q_msg.ipc_msg.src_id == local_dev_id)
				   {
					    if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
						    send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->start_neighbor.val);
						}
						else
						{
							memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
							if (shm_cfg_addr->start_neighbor.val != tmp_val)
							{
								sem_cfg_p();
								shm_cfg_addr->start_neighbor.val = tmp_val;
								sem_cfg_v();
								save_ini_file();
								
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save start_neighbor = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->start_neighbor.val);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
						    send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
					}
					else
					{
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							send_remote_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id, (unsigned char *)&shm_cfg_addr->start_neighbor.val);
						}
						else
						{
							memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
							if (shm_cfg_addr->start_neighbor.val != tmp_val)
							{
								sem_cfg_p();
								shm_cfg_addr->start_neighbor.val = tmp_val;
								sem_cfg_v();
								save_ini_file();
								
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save start_neighbor = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->start_neighbor.val);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
						}
					}
					if((q_msg.ipc_msg.op_code==OP_CODE_SET)&&
					   (q_msg.ipc_msg.nm_type==NM_TYPE_NM))
					{
					    if (shm_ipc_addr->mgr_printf[1])
					    {
						    LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center start_neighbor=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->start_neighbor.val);
					    }
					    send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
					}
				}
				break;
						
			case CMD_CODE_NEIGHBOR_PERIOD:

				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center neighbor_period=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->neighbor_period.val*2);
					}
					tmp_neighbor_period=shm_cfg_addr->neighbor_period.val*2;
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(tmp_neighbor_period));
				}
				else
				{
					if (q_msg.ipc_msg.src_id == local_dev_id)
					{
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
						    tmp_neighbor_period=shm_cfg_addr->neighbor_period.val*2;
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&tmp_neighbor_period);
						}
						else
						{
							memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
							tmp_neighbor_period=shm_cfg_addr->neighbor_period.val*2;
							if (tmp_neighbor_period != tmp_val)
							{
								sem_cfg_p();
								shm_cfg_addr->neighbor_period.val = tmp_val/2;
								sem_cfg_v();
								save_ini_file();
								
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save neighbor_period = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->neighbor_period.val);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
					}
					else
					{
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
						    tmp_neighbor_period=shm_cfg_addr->neighbor_period.val*2;
							send_remote_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id, (unsigned char *)&tmp_neighbor_period);
						}
						else
						{
							memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
							tmp_neighbor_period=shm_cfg_addr->neighbor_period.val*2;
							if (tmp_neighbor_period != tmp_val)
							{
								sem_cfg_p();
								shm_cfg_addr->neighbor_period.val = tmp_val / 2;
								sem_cfg_v();
								save_ini_file();
								
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save neighbor_period = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->neighbor_period.val);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
						}
					}
					if((q_msg.ipc_msg.nm_type==NM_TYPE_NM)&&
						(q_msg.ipc_msg.op_code== OP_CODE_SET))
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center neighbor_period=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->neighbor_period.val*2);
						}
						send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
					}
                        
				}
				break;

			case CMD_CODE_COMBINED_DATA:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					memset((unsigned char *)&combined_data, 0, sizeof(NM_COMBINED_DATA));
					fpga_mem_nm_r->flag = FLAG_CLR;
					fpga_mem_nm_w->flag = FLAG_OK;
					fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
					fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code; 
					notify_fpga();
					for (i = 0; i < timeout_cnt; i++)
				   	{
				   	    delay.tv_sec = 0;
					    delay.tv_usec = POLLING_INTERVA;
					    select(0, NULL, NULL, NULL, &delay);
						if (FLAG_OK == fpga_mem_nm_r->flag)
						{   
						    if (shm_ipc_addr->mgr_printf[1])
						    {
							    LOG_DEBUG(s_i4LogMsgId,\
									     "\n |%s||%s||%d|\n"
									     "\r read fpga - flag     : 0x%X\n"
									     "\r read fpga - cmd_code : 0x%X\n"
									     "\r read fpga - op_code  : 0x%X\n",\
									     __FILE__, __FUNCTION__, __LINE__,\
									    fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						    }
							memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
							combined_data.freq = tmp_val / 2;
							memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload + sizeof(unsigned int), sizeof(unsigned int));
							combined_data.power = (unsigned char)tmp_val;

							combined_data.start_neighbor = (unsigned char)shm_cfg_addr->start_neighbor.val;
							combined_data.neighbor_period = (unsigned short)(shm_cfg_addr->neighbor_period.val*2);

						    if (shm_ipc_addr->mgr_printf[1])
						    {
							    LOG_DEBUG(s_i4LogMsgId,\
									     "\n |%s||%s||%d|\n"
									     "\r read freq            : 0x%X\n"
									     "\r read power           : 0x%X\n"
									     "\r read start_neighbor  : 0x%X\n"
									     "\r read neighbor_period : 0x%X\n",\
									     __FILE__, __FUNCTION__, __LINE__,\
									    combined_data.freq, combined_data.power, combined_data.start_neighbor, combined_data.neighbor_period);
						    }
							
							if (q_msg.ipc_msg.src_id == local_dev_id)
							{
                                send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&combined_data);
							}
							else
							{
							    send_remote_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id, (unsigned char *)&combined_data); 
							}
							break;
						}
				   	}
					if (i == timeout_cnt)
					{
						printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
					}
				}
				else
				{    
					memcpy((unsigned char *)&combined_data, q_msg.ipc_msg.payload, sizeof(NM_COMBINED_DATA));
				    if((combined_data.freq != 0xFFFFFFFF)||(combined_data.power != 0xFF))
				    {
				        fpga_mem_nm_r->flag = FLAG_CLR;
				        fpga_mem_nm_w->flag = FLAG_OK;
				        fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				        fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code; 
						tmp_val = combined_data.freq * 2; 
						memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
						if(shm_cfg_addr->scan_mode.val==1)
						{
							tmp_val = combined_data.freq + 73336250 + 12500;
						}
						else
						{
							tmp_val = combined_data.freq + 73336250 + 25000;
						}
						memcpy(fpga_mem_nm_w->payload + sizeof(unsigned int), (unsigned char *)&tmp_val, sizeof(unsigned int));
						tmp_val = combined_data.power;
						memcpy(fpga_mem_nm_w->payload + 2 * sizeof(unsigned int), (unsigned char *)&tmp_val, sizeof(unsigned int));
						notify_fpga();
						for (i = 0; i < timeout_cnt; i++)
						{
						    delay.tv_sec = 0;
					        delay.tv_usec = POLLING_INTERVA;
					        select(0, NULL, NULL, NULL, &delay);
							if (FLAG_OK == fpga_mem_nm_r->flag)
							{
                                if (shm_ipc_addr->mgr_printf[1])
						        {
									LOG_DEBUG(s_i4LogMsgId,\
										   "\n |%s||%s||%d|\n"
									       "\r read fpga - flag     : 0x%X\n"
									       "\r read fpga - cmd_code : 0x%X\n"
									       "\r read fpga - op_code  : 0x%X\n",\
									       __FILE__, __FUNCTION__, __LINE__,\
									       fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						        } 
                                if(combined_data.freq != 0xFFFFFFFF)
                                {
                                   sem_cfg_p();
							       shm_cfg_addr->freq.val = combined_data.freq;
							       sem_cfg_v();  
							    }
                                if(combined_data.power != 0xFF)
                                {
                                   sem_cfg_p();
							       shm_cfg_addr->power.val = combined_data.power;
							       sem_cfg_v();  
							    }
                                if(combined_data.start_neighbor!= 0xFF)
                                {
                                   sem_cfg_p();
							       shm_cfg_addr->start_neighbor.val = combined_data.start_neighbor;
							       sem_cfg_v();  
							    }
                                if(combined_data.neighbor_period!= 0xFFFF)
                                {
                                   sem_cfg_p();
							       shm_cfg_addr->neighbor_period.val = combined_data.neighbor_period / 2;
							       sem_cfg_v();  
							    }
								save_ini_file();

								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId,\
											 "\n |%s||%s||%d|\n"
											 "\r save freq			  : 0x%X\n"
											 "\r save power 		  : 0x%X\n"
											 "\r save start_neighbor  : 0x%X\n"
											 "\r save neighbor_period : 0x%X\n",\
											 __FILE__, __FUNCTION__, __LINE__,\
											combined_data.freq, combined_data.power, combined_data.start_neighbor, combined_data.neighbor_period / 2);
								}

								if (q_msg.ipc_msg.src_id == local_dev_id)
								{
	                                send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
								}
								else
								{
								    send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
								}
							    if(q_msg.ipc_msg.nm_type==NM_TYPE_NM)
						        {
									if (shm_ipc_addr->mgr_printf[1])
									{
										LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center combined data\n", __FILE__, __FUNCTION__, __LINE__);
									}
						            send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
						        }
								break;
							}
						}
						if (i == timeout_cnt)
						{
							printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
						}
				    }
					else
					{
						if(combined_data.start_neighbor!= 0xFF)
						{
						   sem_cfg_p();
						   shm_cfg_addr->start_neighbor.val = combined_data.start_neighbor;
						   sem_cfg_v();  
						}
						if(combined_data.neighbor_period!= 0xFFFF)
						{
						   sem_cfg_p();
						   shm_cfg_addr->neighbor_period.val = combined_data.neighbor_period / 2;
						   sem_cfg_v();  
						}
						save_ini_file();

						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									 "\n |%s||%s||%d|\n"
									 "\r save start_neighbor  : 0x%X\n"
									 "\r save neighbor_period : 0x%X\n",\
									 __FILE__, __FUNCTION__, __LINE__,\
									combined_data.start_neighbor, combined_data.neighbor_period /2);
						}

						if (q_msg.ipc_msg.src_id == local_dev_id)
						{
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						else
						{
							send_remote_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, q_msg.ipc_msg.dst_id, q_msg.ipc_msg.src_id);
						}

						if(q_msg.ipc_msg.nm_type==NM_TYPE_NM)
						{
							if (shm_ipc_addr->mgr_printf[1])
							{
								LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center combined data\n", __FILE__, __FUNCTION__, __LINE__);
							}
						    send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
						}
					}					
				}
				break;
				
			case CMD_CODE_OPEN_CLOSE_LOOP:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
					memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
				}
				notify_fpga();
			
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						}
						else
						{
							memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
							if (shm_cfg_addr->open_close_loop.val != tmp_val)
							{
								sem_cfg_p();
								shm_cfg_addr->open_close_loop.val = tmp_val;
								sem_cfg_v();
								save_ini_file();
								
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save open_close_loop = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->open_close_loop.val);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_LOCKING_TIME: //ms
			
				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center locking_time=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->locking_time.val);
					}
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(shm_cfg_addr->locking_time.val));
				}
				else
				{
					fpga_mem_nm_r->flag = FLAG_CLR;
					fpga_mem_nm_w->flag = FLAG_OK;
					fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
					fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
					if (q_msg.ipc_msg.op_code == OP_CODE_SET)
					{
						memcpy((unsigned char *)&tmp_locking_time, q_msg.ipc_msg.payload, sizeof(unsigned int));
						tmp_val = tmp_locking_time * 92160;
						memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
					}
					notify_fpga();
					
					for (i = 0; i < timeout_cnt; i++)
					{
						delay.tv_sec = 0;
						delay.tv_usec = POLLING_INTERVA;
						select(0, NULL, NULL, NULL, &delay);
						if (FLAG_OK == fpga_mem_nm_r->flag)
						{
							if (shm_ipc_addr->mgr_printf[1])
							{
								LOG_DEBUG(s_i4LogMsgId,\
										"\n |%s||%s||%d|\n"
										"\r read fpga - flag	 : 0x%X\n"
										"\r read fpga - cmd_code : 0x%X\n"
										"\r read fpga - op_code  : 0x%X\n",\
										__FILE__, __FUNCTION__, __LINE__,\
										fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
							}
							if (q_msg.ipc_msg.op_code == OP_CODE_GET)
							{
								memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
								tmp_val = tmp_val / 92160;
								send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&tmp_val);
							}
							else
							{
								if (shm_cfg_addr->locking_time.val != tmp_locking_time)
								{
									sem_cfg_p();
									shm_cfg_addr->locking_time.val = tmp_locking_time;
									sem_cfg_v();
									save_ini_file();
									
									if (shm_ipc_addr->mgr_printf[1])
									{
										LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save locking_time = %u\n", __FILE__, __FUNCTION__, __LINE__, tmp_locking_time);
									}
								}
								else
								{
									if (shm_ipc_addr->mgr_printf[1])
									{
										LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
									}
								}
								send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
					
							   if(q_msg.ipc_msg.nm_type==NM_TYPE_NM)
							   {
								   if (shm_ipc_addr->mgr_printf[1])
								   {
									   LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center locking_time=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->locking_time.val);
								   }
								   send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
							   }
							}
							break;
						}
					}
					if (i == timeout_cnt)
					{
						printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
					}
				}
				break;
			case CMD_CODE_HALF_VARIANCE_THRESHOLD:
				
				if((q_msg.ipc_msg.nm_type==NM_TYPE_CENTER)&&
					(q_msg.ipc_msg.op_code== OP_CODE_GET))
				{
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center half_variance_threshold=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->half_variance_threshold.val);
					}
					send_cc_get_response_cmd(q_msg.ipc_msg.cmd_code,(unsigned char *)&(shm_cfg_addr->half_variance_threshold.val));
				}
				else
				{
					fpga_mem_nm_r->flag = FLAG_CLR;
					fpga_mem_nm_w->flag = FLAG_OK;
					fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
					fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
					if (q_msg.ipc_msg.op_code == OP_CODE_SET)
					{
						memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
					}
					notify_fpga();
					
					for (i = 0; i < timeout_cnt; i++)
					{
						delay.tv_sec = 0;
						delay.tv_usec = POLLING_INTERVA;
						select(0, NULL, NULL, NULL, &delay);
						if (FLAG_OK == fpga_mem_nm_r->flag)
						{
							if (shm_ipc_addr->mgr_printf[1])
							{
								LOG_DEBUG(s_i4LogMsgId,\
										"\n |%s||%s||%d|\n"
										"\r read fpga - flag	 : 0x%X\n"
										"\r read fpga - cmd_code : 0x%X\n"
										"\r read fpga - op_code  : 0x%X\n",\
										__FILE__, __FUNCTION__, __LINE__,\
										fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
							}
							if (q_msg.ipc_msg.op_code == OP_CODE_GET)
							{
								send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
							}
							else
							{
								memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
								if (shm_cfg_addr->half_variance_threshold.val != tmp_val)
								{
									sem_cfg_p();
									shm_cfg_addr->half_variance_threshold.val = tmp_val;
									sem_cfg_v();
									save_ini_file();
									
									if (shm_ipc_addr->mgr_printf[1])
									{
										LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save half_variance_threshold = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->half_variance_threshold.val);
									}
								}
								else
								{
									if (shm_ipc_addr->mgr_printf[1])
									{
										LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
									}
								}
								send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
								
							   if(q_msg.ipc_msg.nm_type==NM_TYPE_NM)
							   {
								   if (shm_ipc_addr->mgr_printf[1])
								   {
									   LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| send center half_variance_threshold=%d\n", __FILE__, __FUNCTION__, __LINE__,shm_cfg_addr->half_variance_threshold.val);
								   }
								   send_cc_set_cmd(q_msg.ipc_msg.cmd_code,q_msg.ipc_msg.payload);
							   }
							}
							break;
						}
					}
					if (i == timeout_cnt)
					{
						printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
					}
				}
				break;
				
			case CMD_CODE_DEV_ID:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->dev_id.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->dev_id.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->dev_id.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save dev_id = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->dev_id.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_LOCAL_CC:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->cc.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->cc.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->cc.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save cc = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->cc.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
				
			case CMD_CODE_SET_981:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				memcpy((unsigned char *)&addr_981, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));

				notify_fpga();
				
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}

								
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set 981addr = %u\n", __FILE__, __FUNCTION__, __LINE__, addr_981);
						}
						send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_SET_998:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				memcpy((unsigned char *)&addr_998, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));

				notify_fpga();
				
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}

								
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set 998addr = %u\n", __FILE__, __FUNCTION__, __LINE__, addr_998);
						}
						send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
            
            case CMD_CODE_SAVE_IQ_DATA:
            {
                if (iq_excess_volumn_flag != 0)  // 标志指示异常，各种异常  满60M 设置1
                {
                    print_iq_excess_err();  // 查看是什么异常
                    iq_excess_volumn_flag = 0;
                    // 固定过程
                    fpga_mem_nm_r->flag = FLAG_CLR;
                    fpga_mem_nm_w->flag = FLAG_OK;
                    fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
                    fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
                    memcpy((unsigned char *)&save_data_switch, q_msg.ipc_msg.payload, sizeof(unsigned int));
                    memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
                    notify_fpga();

                    for (i = 0; i < timeout_cnt; i++)
                    {
                        delay.tv_sec = 0;
                        delay.tv_usec = POLLING_INTERVA;
                        select(0, NULL, NULL, NULL, &delay);
                        if (FLAG_OK == fpga_mem_nm_r->flag)
                        {
                            if (shm_ipc_addr->mgr_printf[1])
                            {
                                LOG_DEBUG(s_i4LogMsgId,\
                                          "\n |%s||%s||%d|\n"
                                          "\r read fpga - flag   : 0x%X\n"
                                          "\r read fpga - cmd_code : 0x%X\n"
                                          "\r read fpga - op_code  : 0x%X\n",\
                                          __FILE__, __FUNCTION__, __LINE__,\
                                          fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
                            }


                            if (shm_ipc_addr->mgr_printf[1])
                            {
                                LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set data_switch = %u\n", __FILE__, __FUNCTION__, __LINE__, save_data_switch);
                            }


                            iq_switch_status=0;
                            iq_switch_open_count=0;
                            break;
                        }
                    }

                    if (i == timeout_cnt)
                    {
                        printf("[%s:%s:%d] timeout\n", __FILE__, __FUNCTION__, __LINE__);
                    }
                }
                else if (iq_excess_volumn_flag == 0)  // 正常情况
                {
                    fpga_mem_nm_r->flag = FLAG_CLR;
                    fpga_mem_nm_w->flag = FLAG_OK;
                    fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
                    fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
                    memcpy((unsigned char *)&save_data_switch, q_msg.ipc_msg.payload, sizeof(unsigned int));
                    memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
                    notify_fpga();

                    for (i = 0; i < timeout_cnt; i++)
                    {
                        delay.tv_sec = 0;
                        delay.tv_usec = POLLING_INTERVA;
                        select(0, NULL, NULL, NULL, &delay);
                        if (FLAG_OK == fpga_mem_nm_r->flag)
                        {
                            if (shm_ipc_addr->mgr_printf[1])
                            {
                                LOG_DEBUG(s_i4LogMsgId,\
                                        "\n |%s||%s||%d|\n"
                                        "\r read fpga - flag     : 0x%X\n"
                                        "\r read fpga - cmd_code : 0x%X\n"
                                        "\r read fpga - op_code  : 0x%X\n",\
                                        __FILE__, __FUNCTION__, __LINE__,\
                                        fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
                            }


                            if (shm_ipc_addr->mgr_printf[1])
                            {
                                LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set data_switch = %u\n", __FILE__, __FUNCTION__, __LINE__, save_data_switch);
                            }

                            if (save_data_switch == 0)  // 关闭
                            {
                                iq_switch_status = 0;
                                iq_switch_open_count = 0;

                                // 一定要先close文件, 后卸载SD卡，顺序不能错
                                // 1-close /mnt/iqdata
                                if(mem_fd_fp != NULL)
                                {
                                    fclose(mem_fd_fp);
                                    mem_fd_fp = NULL;
                                }

                                // 2-umount /mnt
                                if (1 == iq_mount_sd_flag)
                                {
                                    iq_mount_sd_flag = 0;
                                    printf("[%s:%d] umount /mnt\n", __FUNCTION__, __LINE__);
                                    system("umount /mnt");
                                }
                            }
                            else if (save_data_switch == 1)  // 打开
                            {
                                iq_switch_status = 1;
                            }

                            send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
                            break;
                        }
                    }
                    if (i == timeout_cnt)
                    {
                        printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
                    }
                }
                else
                {
                    printf("[%s:%s:%d] timeout\n", __FILE__, __FUNCTION__, __LINE__);
                }
                break;
            }
			case CMD_CODE_QUERY_981:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;

				notify_fpga();
				
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}

					    send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;

			case CMD_CODE_QUERY_998:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;

				notify_fpga();
				
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}

					    send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;

			case CMD_CODE_WORK_TIME_OUT:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->work_time_out.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->work_time_out.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->work_time_out.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save work_time_out = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->work_time_out.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_GPS_TIME_OUT:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->gps_time_out.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->gps_time_out.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->gps_time_out.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save gps_time_out = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->gps_time_out.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_STUN_TIME_OUT:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->stun_time_out.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->stun_time_out.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->stun_time_out.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save stun_time_out = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->stun_time_out.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_START_TIME_OUT:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->start_time_out.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->start_time_out.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->start_time_out.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save start_time_out = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->start_time_out.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_VERSION:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)shm_cfg_addr->s_version.buf);
				}
				break;
			
			case CMD_CODE_FPGA_VERSION:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					snprintf(fpga_version,sizeof(fpga_version),"V%d.%d.%02d.%03d",fpga_mem_nm_share[0],fpga_mem_nm_share[1],fpga_mem_nm_share[2],fpga_mem_nm_share[3]);
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| fpga_version : %s\n", __FILE__, __FUNCTION__, __LINE__, fpga_version);
					}
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)fpga_version);
				}
	        	break;
			case CMD_CODE_TERMINAL_CC:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->terminal_cc.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->terminal_cc.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->terminal_cc.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save terminal_cc = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->terminal_cc.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_PROTOCOL_MODE:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->protocol_mode.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->protocol_mode.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->protocol_mode.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save protocol_mode = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->protocol_mode.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_AIR_SUBNET_ID:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)&shm_cfg_addr->air_subnet_id.val);
				}
				else
				{
					memcpy((unsigned char *)&tmp_val, q_msg.ipc_msg.payload, sizeof(unsigned int));
					if (shm_cfg_addr->air_subnet_id.val != tmp_val)
					{
						sem_cfg_p();
						shm_cfg_addr->air_subnet_id.val = tmp_val;
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save air_subnet_id = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->air_subnet_id.val);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;

			case CMD_CODE_LOCAL_IP:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
				    ip_len=strlen(shm_cfg_addr->local_ip.buf);
					mask_len=strlen(shm_cfg_addr->subnet_mask.buf);
					gateway_len=strlen(shm_cfg_addr->gateway.buf);
					memset((unsigned char *)tmp_ip,0,sizeof(tmp_ip));
					memcpy((unsigned char *)(tmp_ip+3), shm_cfg_addr->local_ip.buf, ip_len);
					memcpy((unsigned char *)(tmp_ip+3+ip_len), shm_cfg_addr->subnet_mask.buf, mask_len);
					memcpy((unsigned char *)(tmp_ip+3+ip_len+mask_len), shm_cfg_addr->gateway.buf, gateway_len);
					tmp_ip[0]=ip_len;
					tmp_ip[1]=mask_len;
					tmp_ip[2]=gateway_len;
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,(unsigned char *)tmp_ip);
				}
				else
				{
				    memset((unsigned char *)tmp_ip,0,sizeof(tmp_ip));
					memcpy((unsigned char *)tmp_ip, q_msg.ipc_msg.payload+3, q_msg.ipc_msg.payload[0]);
					if (strcmp(shm_cfg_addr->local_ip.buf, tmp_ip))
					{
						sem_cfg_p();
						memcpy((unsigned char *)shm_cfg_addr->local_ip.buf, tmp_ip, sizeof(tmp_ip));
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save local_ip = %s\n", __FILE__, __FUNCTION__, __LINE__, (unsigned char *)shm_cfg_addr->local_ip.buf);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
				    memset((unsigned char *)tmp_ip,0,sizeof(tmp_ip));
					memcpy((unsigned char *)tmp_ip, q_msg.ipc_msg.payload+3+q_msg.ipc_msg.payload[0], q_msg.ipc_msg.payload[1]);
					if (strcmp(shm_cfg_addr->subnet_mask.buf, tmp_ip))
					{
						sem_cfg_p();
						memcpy((unsigned char *)shm_cfg_addr->subnet_mask.buf, tmp_ip, sizeof(tmp_ip));
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save subnet_mask = %s\n", __FILE__, __FUNCTION__, __LINE__, (unsigned char *)shm_cfg_addr->subnet_mask.buf);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
				    memset((unsigned char *)tmp_ip,0,sizeof(tmp_ip));
					memcpy((unsigned char *)tmp_ip, q_msg.ipc_msg.payload+3+q_msg.ipc_msg.payload[0]+q_msg.ipc_msg.payload[1], q_msg.ipc_msg.payload[2]);
					if (strcmp(shm_cfg_addr->gateway.buf, tmp_ip))
					{
						sem_cfg_p();
						memcpy((unsigned char *)shm_cfg_addr->gateway.buf, tmp_ip, sizeof(tmp_ip));
						sem_cfg_v();
						save_ini_file();
						
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save gateway = %s\n", __FILE__, __FUNCTION__, __LINE__, (unsigned char *)shm_cfg_addr->gateway.buf);
						}
					}
					else
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
						}
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				break;
			case CMD_CODE_LOCAL_MAC:
				memset((unsigned char *)&eeprom_esn_param,0,sizeof(EEPROM_ESN_PARAM));
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
					if (read_esn_param_from_eeprom())
				   {
					    printf("<%s> <%s> <%d> read_esn_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					    break;
				   }
				   send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type,eeprom_esn_param.mac_addr);
				}
				else
				{
				    if (read_esn_param_from_eeprom())
				    {
					    printf("<%s> <%s> <%d> read_esn_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
				 	    break;
				    }
					memcpy(eeprom_esn_param.mac_addr,q_msg.ipc_msg.payload,sizeof(eeprom_esn_param.mac_addr)); 
					if(shm_ipc_addr->mgr_printf[1])
					{
					    sprintf(tmp_mac,"%x:%x:%x:%x:%x:%x",eeprom_esn_param.mac_addr[0],eeprom_esn_param.mac_addr[1],eeprom_esn_param.mac_addr[2],\
							                                eeprom_esn_param.mac_addr[3],eeprom_esn_param.mac_addr[4],eeprom_esn_param.mac_addr[5]);
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set local_mac = %s\n", __FILE__, __FUNCTION__, __LINE__, tmp_mac);
						
					}
					if (write_esn_param_to_eeprom())
				    {
					    printf("<%s> <%s> <%d> write_esn_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
				 	    break;
				    }
                    send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}	 
				break;
			case CMD_CODE_IS_CONNET_CC:
					sem_ipc_p();
					memcpy((unsigned char *)&shm_ipc_addr->is_connect_cc, q_msg.ipc_msg.payload, sizeof(unsigned int));
					sem_ipc_v();
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| set is_connet_cc = %u\n", __FILE__, __FUNCTION__, __LINE__, shm_ipc_addr->is_connect_cc);
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				break;
			case CMD_CODE_EMISSIVE_VCO_FREQ:
			case CMD_CODE_DEBUG_EMISSIVE_FREQ:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
					memcpy((unsigned char *)&tmp_freq, q_msg.ipc_msg.payload, sizeof(unsigned int));
					tmp_val = tmp_freq * 2;
					memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
					if(shm_cfg_addr->scan_mode.val==1)
					{
						tmp_val = tmp_freq + 73336250 + 12500;
					}
					else
					{
						tmp_val = tmp_freq + 73336250 + 25000;
					}
					memcpy(fpga_mem_nm_w->payload + sizeof(unsigned int), (unsigned char *)&tmp_val, sizeof(unsigned int));
				}
				notify_fpga();
				
				for (i = 0; i < debug_timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
								    "\n |%s||%s||%d|\n"
									"\r read fpga - flag     : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
							tmp_val = tmp_val / 2;
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&tmp_val);
						}
						else
						{
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == debug_timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_RECEIVING_VCO_FREQ:
			case CMD_CODE_SECOND_LOCAL_OSCILLATOR:
			case CMD_CODE_POWER:
			case CMD_CODE_OFFSET:
			case CMD_CODE_POWER_AMPLIFIER_SWITCH:
			case CMD_CODE_PHASE:
			case CMD_CODE_AFC:
			case CMD_CODE_IQS:
			case CMD_CODE_ERROR_RATE:
			case CMD_CODE_START_DEBUG:
			case CMD_CODE_START_ERR_RATE:
			case CMD_CODE_SELECT_SIGNAL_SOURCE:
			case CMD_CODE_DEBUG_OPEN_CLOSE_LOOP:
			case CMD_CODE_DEBUG_POWER:
            case CMD_CODE_SLOT_CONTROL:
			case CMD_CODE_VGS:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
					memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
				}
				notify_fpga();
				
				for (i = 0; i < debug_timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
								    "\n |%s||%s||%d|\n"
									"\r read fpga - flag     : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						}
						else
						{
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == debug_timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_CALIBRATE_OFFSET:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
					memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
				}
				notify_fpga();
				
				for (i = 0; i < (CALIBRATE_DELAY / POLLING_INTERVA); i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
								    "\n |%s||%s||%d|\n"
									"\r read fpga - flag     : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						}
						else
						{
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == (CALIBRATE_DELAY / POLLING_INTERVA))
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_FPGA_PARAM:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				fpga_param[0] = eeprom_fpga_param.calibration_afc;
				memcpy((unsigned char *)&fpga_param[1], (unsigned char *)eeprom_fpga_param.calibration_power, sizeof(eeprom_fpga_param.calibration_power));
				send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)fpga_param);
				break;
			case CMD_CODE_SAVE_AFC:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				memcpy((unsigned char *)&eeprom_fpga_param.calibration_afc, q_msg.ipc_msg.payload, sizeof(unsigned int));
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save afc = %u\n", __FILE__, __FUNCTION__, __LINE__, eeprom_fpga_param.calibration_afc);
				}
				if (write_fpga_param_to_eeprom())
				{
					printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				break;
			case CMD_CODE_SAVE_IQS:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				memcpy((unsigned char *)&save_id, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy((unsigned char *)&save_val, q_msg.ipc_msg.payload + sizeof(unsigned int), sizeof(unsigned int));
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save_id = %u, save_val = %u\n",\
						__FILE__, __FUNCTION__, __LINE__, save_id, save_val);
				}
				if ((save_id >= 1) && (save_id <= 60))
				{
					eeprom_fpga_param.calibration_power[save_id - 1] = (eeprom_fpga_param.calibration_power[save_id - 1] & 0xFFFF0000) | (save_val & 0x0000FFFF);
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save power = %u\n", __FILE__, __FUNCTION__, __LINE__, eeprom_fpga_param.calibration_power[save_id - 1]);
					}
					if (write_fpga_param_to_eeprom())
					{
						printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
						break;
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				else
				{
					printf("<%s> <%s> <%d> err save_id = %u\n", __FILE__, __FUNCTION__, __LINE__, save_id);
				}
				break;
			case CMD_CODE_SAVE_POWER:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				memcpy((unsigned char *)&save_id, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy((unsigned char *)&save_val, q_msg.ipc_msg.payload + sizeof(unsigned int), sizeof(unsigned int));
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save_id = %u, save_val = %u\n",\
						__FILE__, __FUNCTION__, __LINE__, save_id, save_val);
				}
				if ((save_id >= 1) && (save_id <= 60))
				{
					eeprom_fpga_param.calibration_power[save_id - 1] = save_val;
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save power = %u\n", __FILE__, __FUNCTION__, __LINE__, eeprom_fpga_param.calibration_power[save_id - 1]);
					}
					if (write_fpga_param_to_eeprom())
					{
						printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
						break;
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				else
				{
					printf("<%s> <%s> <%d> err save_id = %u\n", __FILE__, __FUNCTION__, __LINE__, save_id);
				}
				break;
			case CMD_CODE_SAVE_OFFSET:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				memcpy((unsigned char *)&save_id, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy((unsigned char *)&save_val, q_msg.ipc_msg.payload + sizeof(unsigned int), sizeof(unsigned int));
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save_id = %u, save_val = %u\n",\
						__FILE__, __FUNCTION__, __LINE__, save_id, save_val);
				}
				if ((save_id >= 1) && (save_id <= 30))
				{
					eeprom_fpga_param.calibration_offset[save_id - 1] = save_val;
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save offset = %u\n", __FILE__, __FUNCTION__, __LINE__, eeprom_fpga_param.calibration_offset[save_id - 1]);
					}
					if (write_fpga_param_to_eeprom())
					{
						printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
						break;
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				else
				{
					printf("<%s> <%s> <%d> err save_id = %u\n", __FILE__, __FUNCTION__, __LINE__, save_id);
				}
				break;
			case CMD_CODE_SAVE_PHASE:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
					printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					break;
				}
				memcpy((unsigned char *)&save_id, q_msg.ipc_msg.payload, sizeof(unsigned int));
				memcpy((unsigned char *)&save_val, q_msg.ipc_msg.payload + sizeof(unsigned int), sizeof(unsigned int));
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save_id = %u, save_val = %u\n",\
						__FILE__, __FUNCTION__, __LINE__, save_id, save_val);
				}
				if ((save_id >= 1) && (save_id <= 51))
				{
					eeprom_fpga_param.calibration_phase[save_id - 1] = save_val;
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save phase = %u\n", __FILE__, __FUNCTION__, __LINE__, eeprom_fpga_param.calibration_phase[save_id - 1]);
					}
					if (write_fpga_param_to_eeprom())
					{
						printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
						break;
					}
					send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				}
				else
				{
					printf("<%s> <%s> <%d> err save_id = %u\n", __FILE__, __FUNCTION__, __LINE__, save_id);
				}
				break;
			case CMD_CODE_CALIBRATE_PHASE:
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
					memcpy(fpga_mem_nm_w->payload, q_msg.ipc_msg.payload, sizeof(unsigned int));
				}
				notify_fpga();
				
				for (i = 0; i < CALIBRATE_PHASE_DELAY / POLLING_INTERVA; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
								    "\n |%s||%s||%d|\n"
									"\r read fpga - flag     : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}

						memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
						if (read_fpga_param_from_eeprom())
						{
							printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
							break;
						}
						memcpy((unsigned char *)eeprom_fpga_param.calibration_phase, fpga_mem_nm_r->payload, sizeof(eeprom_fpga_param.calibration_phase));
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save phase ok\n", __FILE__, __FUNCTION__, __LINE__);
						}
						if (write_fpga_param_to_eeprom())
						{
							printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
							break;
						}
						
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, fpga_mem_nm_r->payload);
						}
						else
						{
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == (CALIBRATE_PHASE_DELAY / POLLING_INTERVA))
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			case CMD_CODE_WIRELESS_MODULE:
				if (q_msg.ipc_msg.op_code == OP_CODE_GET)
				{
				    memset((unsigned char *)&center_data,0,sizeof(center_data));
					center_data.gps_time=10;
					center_data.freq=(shm_cfg_addr->freq.val);
					center_data.locking_time=(unsigned short)(shm_cfg_addr->locking_time.val);
					center_data.neighbor_period=(unsigned short)(shm_cfg_addr->neighbor_period.val * 2);
					center_data.power=shm_cfg_addr->power.val;
					center_data.start_neighbor=(unsigned short)(shm_cfg_addr->start_neighbor.val);
					center_data.variance=(unsigned short)(shm_cfg_addr->half_variance_threshold.val);
					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| center get data_packet\n", __FILE__, __FUNCTION__, __LINE__);
					}
					send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&center_data);
					
				}
				break;
			case CMD_CODE_FPGA_EEPROM:
				chdir("/home");
				fd=fopen("fpga_param","rb");
				if(fd == NULL)
				{
				    printf("open fpga_param  file error!\n");
				}
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (sizeof(EEPROM_FPGA_PARAM) != fread((unsigned char *)&eeprom_fpga_param, sizeof(char),sizeof(EEPROM_FPGA_PARAM),fd))
				{
				    printf("fread err\n");
					fclose(fd);
				}
				if (write_fpga_param_to_eeprom())
				{
				    printf("<%s> <%s> <%d> write_fpga_param_to_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
					fclose(fd);
				}
				fclose(fd);
				send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				break;
			case CMD_CODE_SAVE_EEPROM:
				memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
				if (read_fpga_param_from_eeprom())
				{
			        printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
				}
				fd=fopen("/loadapp/fpga_param","wb");
				if(fd == NULL)
				{
				    printf("open fpga_param  file error!\n");
				}
				if (sizeof(EEPROM_FPGA_PARAM) != fwrite((unsigned char *)&eeprom_fpga_param, sizeof(char), sizeof(EEPROM_FPGA_PARAM), fd))
				{
				    printf("fwrite err\n");
					fclose(fd);
				}
				fclose(fd);
				printf("<%s> <%s> <%d> save fpga_param to file\n", __FILE__, __FUNCTION__, __LINE__);
				send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
				break;
			case CMD_CODE_REBOOT:
				chdir("/loadapp");
				if (shm_ipc_addr->mgr_printf[1])
				{
				    LOG_DEBUG(s_i4LogMsgId, \
						     "start reboot \n");
				}
				system("./reboot_epack");
				break;

			case CMD_CODE_DEV_CALL_TIMEOUT: //S
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
						memcpy((unsigned char *)&call_time_out, q_msg.ipc_msg.payload, sizeof(unsigned int));
						tmp_val = call_time_out* 5760000;
						memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
				}
				notify_fpga();
					
				for (i = 0; i < timeout_cnt; i++)
				{
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							memcpy((unsigned char *)&tmp_val, fpga_mem_nm_r->payload, sizeof(unsigned int));
							tmp_val = tmp_val / 5760000;
							send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, (unsigned char *)&tmp_val);
						}
						else
						{
							if (shm_cfg_addr->dev_call_timeout.val != call_time_out)
							{
								sem_cfg_p();
								shm_cfg_addr->dev_call_timeout.val = call_time_out;
								sem_cfg_v();
								save_ini_file();
									
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save dev_call_timeout = %u\n", __FILE__, __FUNCTION__, __LINE__, call_time_out);
								}
							}
							else
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
									LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
					
						}
							break;
					}
				}
				if (i == timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
/*		   case CMD_CODE_SCAN_MODE:
					
			    fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
				    scan_mode=q_msg.ipc_msg.payload[0];
					freq_offset=q_msg.ipc_msg.payload[1];
					
				    switch(freq_offset)
				    {
				        case 1:
							freq_offset=1000000;
							break;
						case 2:
							freq_offset=1500000;
							break;
						case 3:
							freq_offset=2000000;
							break;
						case 4:
							freq_offset=5000000;
							break;
						case 0:
							freq_offset=500000;
							break;
						default:
							scan_flag=1;
							break;
				    }
					if(scan_flag==1)
					{
					    scan_flag=0;
					    printf("freq_offset =%d set error\n",freq_offset);
						break;
					}
					else
					{
						memcpy(scan, (unsigned char *)&scan_mode, sizeof(unsigned int));
						memcpy(scan+sizeof(unsigned int), (unsigned char *)&freq_offset, sizeof(unsigned int));
						memcpy(fpga_mem_nm_w->payload, scan, sizeof(scan));
					}

				}
				notify_fpga();
						
				for (i = 0; i < timeout_cnt; i++)
				{
				    delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
					    if (shm_ipc_addr->mgr_printf[1])
						{
						    LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
						if (q_msg.ipc_msg.op_code == OP_CODE_GET)
						{
							memcpy((unsigned char *)&scan_mode, fpga_mem_nm_r->payload, sizeof(unsigned int));
							memcpy((unsigned char *)&freq_offset, fpga_mem_nm_r->payload+sizeof(unsigned int), sizeof(unsigned int));
							switch(freq_offset)
							{
							    case 500000:
									scan_tran[1]=0;
									break;
								case 1000000:
								    scan_tran[1]=1;
									break;
								case 1500000:
									scan_tran[1]=2;
									break;
								case 2000000:
									scan_tran[1]=3;
								    break;
								case 5000000:
								    scan_tran[1]=4;
									break;
								default:
									scan_flag=1;
									break;
							}
							if(scan_flag==1)
							{
							    printf("freq_offset =%d get error\n",freq_offset);
								scan_flag=0;
								break;
							}
							else
							{
							    scan_tran[0]=scan_mode;
							}
						    send_local_nm_get_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type, scan_tran);
						}
						else
						{
							if((shm_cfg_addr->scan_mode.val== scan_mode)&&(shm_cfg_addr->freq_offset.val==(freq_offset/1000)))
							{
								if (shm_ipc_addr->mgr_printf[1])
								{
								    LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| the same value in config file, don't need save\n", __FILE__, __FUNCTION__, __LINE__);
								}
							}
							else
							{
								sem_cfg_p();
								shm_cfg_addr->scan_mode.val = scan_mode;
								shm_cfg_addr->freq_offset.val=freq_offset/1000;
								sem_cfg_v();
								save_ini_file();
																		
								if (shm_ipc_addr->mgr_printf[1])
								{
								    LOG_DEBUG(s_i4LogMsgId, "|%s||%s||%d| save scan_mode = %u,freq_offset=%u\n", __FILE__, __FUNCTION__, __LINE__, shm_cfg_addr->scan_mode.val,shm_cfg_addr->freq_offset.val);
								}
							}
						    send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						}
						break;
					}
				}
				if (i == timeout_cnt)
				{
				    printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
*/
			case CMD_CODE_CENTER_PRINT:
				
				if (q_msg.ipc_msg.op_code == OP_CODE_SET)
				{
				    if(q_msg.ipc_msg.payload[0]==1)
				    {
				        center_print_flag=1;
				    }
					else
					{
					    center_print_flag=0;
					}
				}
				else
				{
				    printf("center_print op_code = %d error\n",q_msg.ipc_msg.op_code);
				}
				break;
			case CMD_CODE_ERROR_RATE_FREQ:
				
				fpga_mem_nm_r->flag = FLAG_CLR;
				fpga_mem_nm_w->flag = FLAG_OK;
				fpga_mem_nm_w->cmd_code = q_msg.ipc_msg.cmd_code;
				fpga_mem_nm_w->op_code = q_msg.ipc_msg.op_code;
								
			    memcpy((unsigned char *)&tmp_freq, q_msg.ipc_msg.payload, sizeof(unsigned int));
				tmp_val = tmp_freq * 2;
				memcpy(fpga_mem_nm_w->payload, (unsigned char *)&tmp_val, sizeof(unsigned int));
				tmp_val = tmp_freq + 73336250 + 12500;
				memcpy(fpga_mem_nm_w->payload + sizeof(unsigned int), (unsigned char *)&tmp_val, sizeof(unsigned int));
				notify_fpga();
								
				for (i = 0; i < debug_timeout_cnt; i++)
			    {
					delay.tv_sec = 0;
					delay.tv_usec = POLLING_INTERVA;
					select(0, NULL, NULL, NULL, &delay);
					if (FLAG_OK == fpga_mem_nm_r->flag)
					{
						if (shm_ipc_addr->mgr_printf[1])
						{
							LOG_DEBUG(s_i4LogMsgId,\
									"\n |%s||%s||%d|\n"
									"\r read fpga - flag	 : 0x%X\n"
									"\r read fpga - cmd_code : 0x%X\n"
									"\r read fpga - op_code  : 0x%X\n",\
									__FILE__, __FUNCTION__, __LINE__,\
									fpga_mem_nm_r->flag, fpga_mem_nm_r->cmd_code, fpga_mem_nm_r->op_code);
						}
								
						send_local_nm_set_ack(q_msg.ipc_msg.cmd_code, q_msg.ipc_msg.nm_type);
						break;
					}
				}
				if (i == debug_timeout_cnt)
				{
					printf("<%s> <%s> <%d> timeout\n", __FILE__, __FUNCTION__, __LINE__);
				}
				break;
			default:
				printf("<%s> <%s> <%d> unknown cmd_code\n", __FILE__, __FUNCTION__, __LINE__);
				break;
		}
	}
	
	pthread_exit(NULL);
}



void * pthread_func_alarm_handle(void *arg)
{
	while(1)
	{
		sleep(3);
		MGR_Alarm_Set();
	}
	pthread_exit(NULL);
}


void * pthread_func_alarm_status(void *arg)
{
	while(1)
	{
		sleep(2);

		if (shm_ipc_addr->mgr_printf[1] > 0)
		{
			if (shm_ipc_addr->mgr_printf[1] < 100)
			{
				MGR_Alarm_Update_Status(MGR_ALARM_FPGA_1, 0);
			}
			else
			{
				MGR_Alarm_Update_Status(MGR_ALARM_FPGA_1, 1);
			}
		}
	}
	
	pthread_exit(NULL);
}
void * pthread_func_alarm_send(void *arg)
{
	int i;

	UINT32 u4SocketFd;//空口
	
	u4SocketFd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > u4SocketFd)
	{
		printf("[pthread_func_alarm_send][socketfd error!!!]\n");
		return NULL;
	}
	
	while(1)
	{
		//获取随机事件
		i = rand()%10;//获取休眠时间单位分钟

		//sleep(i*60);

		sleep(3);
		

		if(ptIPCShm->mgr_printf[0])
		{
			LOG_DEBUG(s_i4LogMsgId,"[pthread_func_alarm_send]time=%d min",i);
		}
		MGR_AlarmTable_Set();
		MGR_Alarm_Send(u4SocketFd);
		MGR_AlarmTable_Clear();
	}
	
	pthread_exit(NULL);
}


void * pthread_func_kill_monitor(void *arg)
{
	unsigned int kill_start = 0;
	unsigned int cnt = 0;
	unsigned int kill_cnt = 0;
	unsigned int timeout = KILL_TIMEOUT;
	
	while (1)
	{
		sleep(1);

		if (local_kill_flag != shm_cfg_addr->kill_flag.val)
		{
			local_kill_flag = shm_cfg_addr->kill_flag.val;
			printf("save kill_flag:0x%X\n", shm_cfg_addr->kill_flag.val);
			save_ini_file();

			if (shm_cfg_addr->kill_flag.val == NAS_KILL_FLAG)
			{
				printf("Start kill process!!!\n");
			}
			else
			{
				kill_start = 0;
				cnt = 0;
				kill_cnt = 0;
				timeout = KILL_TIMEOUT;
				printf("Cancel kill process!!!\n");
			}
		}

		if (kill_start == 0)
		{
			if (shm_cfg_addr->kill_flag.val == NAS_KILL_FLAG)
			{
				if (cnt < KILL_CHECK_CNT)
				{
					cnt++;
				}
				if (cnt == KILL_CHECK_CNT)
				{
					kill_start = 1;
					printf("About 10 minutes later will kill process!!!\n");
				}
			}
		}
		else
		{
			if (timeout > 0)
			{
				timeout--;
			}
			else
			{
				if (shm_cfg_addr->kill_flag.val == NAS_KILL_FLAG)
				{
					if (kill_cnt < KILL_CHECK_CNT)
					{
						kill_cnt++;
					}
					if (kill_cnt == KILL_CHECK_CNT)
					{
						printf("killing process...\n");
						system("flash_eraseall /dev/mtd1");
						system("reboot");
					}
				}
			}
		}
	}
	pthread_exit(NULL);
}


void * pthread_func_stun_monitor(void *arg)
{
	while (1)
	{
		sleep(1);
		if (local_stun_flag != shm_cfg_addr->stun_flag.val)
		{
			local_stun_flag = shm_cfg_addr->stun_flag.val;
			printf("save stun_flag:0x%X\n", shm_cfg_addr->stun_flag.val);
			save_ini_file();
		}
	}
	pthread_exit(NULL);
}



void * pthread_func_center_print_monitor(void *arg)
{
    unsigned char freq_slot=0;
	unsigned short half=0;
	unsigned char info[3]={0};
	while (1)
	{	
		if (center_print_flag == 1)
		{
		    if(fpga_mem_nm_debug->lock_flag ==1)
		    {
		        if(fpga_mem_nm_debug->f1s1 ==1)
		        {
		            freq_slot=1;
					half=fpga_mem_nm_debug->f1s1_half;
		        }
				else if(fpga_mem_nm_debug->f1s2 ==1)
				{
		            freq_slot=2;
					half=fpga_mem_nm_debug->f1s2_half;
				}
				else if(fpga_mem_nm_debug->f2s1 ==1)
				{
		            freq_slot=3;
					half=fpga_mem_nm_debug->f2s1_half;
				}
				else if(fpga_mem_nm_debug->f2s2 ==1)
				{
		            freq_slot=4;
					half=fpga_mem_nm_debug->f2s2_half;
				}
		    }
			else
			{
			    freq_slot=0;
				half=0;
			}
			info[0]=freq_slot;
			memcpy(info+1,(unsigned char *)&half,sizeof(short));
			send_cc_set_cmd(CMD_CODE_CENTER_INFO,info);
		}
		sleep(2);
	}
	pthread_exit(NULL);
}


void * pthread_fpga_debug(void *arg)
{	
	while (1)
	{
		sleep(shm_ipc_addr->fpga_debug_sleep);
		if (shm_ipc_addr->mgr_printf[2] == 2)
		{
			LOG_DEBUG(s_i4LogMsgId,\
				"\n *****************************************************************************\n"
				"\r LockFlag:0x%x   ProhibitTranFlag:0x%x     HalfRssi:%u\n"
				"\r F1S1:0x%X     F1S2:0x%X     F2S1:0x%X     F2S2:0x%X\n"
				"\r HalfFd:%u     HalfFd:%u     HalfFd:%u     HalfFd:%u\n"
				"\r *****************************************************************************\n",\
				fpga_mem_nm_debug->lock_flag,fpga_mem_nm_debug->prohibit_transmit_flag,fpga_mem_nm_debug->u_half_rssi,\
				fpga_mem_nm_debug->f1s1, fpga_mem_nm_debug->f1s2, fpga_mem_nm_debug->f2s1, fpga_mem_nm_debug->f2s2,\
				fpga_mem_nm_debug->f1s1_half, fpga_mem_nm_debug->f1s2_half, fpga_mem_nm_debug->f2s1_half, fpga_mem_nm_debug->f2s2_half);
		}
		else if (shm_ipc_addr->mgr_printf[2] == 1)
		{
			LOG_DEBUG(s_i4LogMsgId,\
				"\n ***************************************\n"
				"\r f1s1_half : 0x%X\n"
				"\r f1s2_half : 0x%x\n"
				"\r f2s1_half : 0x%X\n"
				"\r f2s2_half : 0x%X\n"
				"\r ***************************************\n",\
				fpga_mem_nm_debug->f1s1_half,\
				fpga_mem_nm_debug->f1s2_half,\
				fpga_mem_nm_debug->f2s1_half,\
				fpga_mem_nm_debug->f2s2_half);

			LOG_DEBUG(s_i4LogMsgId,\
				"\n ***************************************\n"
				"\r u_half_rssi               : %u\n"
				"\r prohibit_transmit_flag    : 0x%X\n"
				"\r data1                     : 0x%X\n"
				"\r data2                     : 0x%X\n"
				"\r data3                     : 0x%X\n"
				"\r data4                     : 0x%X\n"
				"\r data5                     : 0x%X\n"
				"\r data6                     : 0x%X\n"
				"\r vari_threshold    : 0x%X\n"
				"\r lock_flag         : 0x%x\n"
				"\r f1s1              : 0x%X\n"
				"\r f1s2              : 0x%X\n"
				"\r f2s1              : 0x%X\n"
				"\r f2s2              : 0x%x\n"
				"\r arm_downlink_lock : 0x%X\n"
				"\r f1s1_crc_flag     : 0x%X\n"
				"\r f1s2_crc_flag     : 0x%X\n"
				"\r f2s1_crc_flag     : 0x%X\n"
				"\r f2s2_crc_flag     : 0x%X\n"
				"\r ***************************************\n",\
				fpga_mem_nm_debug->u_half_rssi,\
				fpga_mem_nm_debug->prohibit_transmit_flag,\
				fpga_mem_nm_debug->data1,\
				fpga_mem_nm_debug->data2,\
				fpga_mem_nm_debug->data3,\
				fpga_mem_nm_debug->data4,\
				fpga_mem_nm_debug->data5,\
				fpga_mem_nm_debug->data6,\
				fpga_mem_nm_debug->vari_threshold,\
				fpga_mem_nm_debug->lock_flag,\
				fpga_mem_nm_debug->f1s1,\
				fpga_mem_nm_debug->f1s2,\
				fpga_mem_nm_debug->f2s1,\
				fpga_mem_nm_debug->f2s2,\
				fpga_mem_nm_debug->arm_downlink_lock,\
				fpga_mem_nm_debug->f1s1_crc_flag,\
				fpga_mem_nm_debug->f1s2_crc_flag,\
				fpga_mem_nm_debug->f2s1_crc_flag,\
				fpga_mem_nm_debug->f2s2_crc_flag);

			LOG_DEBUG(s_i4LogMsgId,\
				"\n ***************************************\n"
				"\r lat_m_pout  : 0x%X\n"
				"\r long_m_pout : 0x%x\n"
				"\r gps_lock    : 0x%X\n"
				"\r gps_pps_out : 0x%X\n"
				"\r pos_e_w     : 0x%X\n"
				"\r pos_n_s     : 0x%x\n"
				"\r utc_en_out  : 0x%X\n"
				"\r day_out     : 0x%X\n"
				"\r hour_out    : 0x%X\n"
				"\r lat_d_out   : 0x%X\n"
				"\r lat_m_out   : 0x%x\n"
				"\r long_d_out  : 0x%X\n"
				"\r long_m_out  : 0x%X\n"
				"\r minute_out  : 0x%X\n"
				"\r second_out  : 0x%x\n"
				"\r year_out    : 0x%X\n"
				"\r ***************************************\n",\
				fpga_mem_nm_debug->lat_m_pout,\
				fpga_mem_nm_debug->long_m_pout,\
				fpga_mem_nm_debug->gps_lock,\
				fpga_mem_nm_debug->gps_pps_out,\
				fpga_mem_nm_debug->pos_e_w,\
				fpga_mem_nm_debug->pos_n_s,\
				fpga_mem_nm_debug->utc_en_out,\
				fpga_mem_nm_debug->day_out,\
				fpga_mem_nm_debug->hour_out,\
				fpga_mem_nm_debug->lat_d_out,\
				fpga_mem_nm_debug->lat_m_out,\
				fpga_mem_nm_debug->long_d_out,\
				fpga_mem_nm_debug->long_m_out,\
				fpga_mem_nm_debug->minute_out,\
				fpga_mem_nm_debug->second_out,\
				fpga_mem_nm_debug->year_out);

			LOG_DEBUG(s_i4LogMsgId,\
				"\n ***************************************\n"
				"\r downlink_dout        : 0x%X\n"
				"\r freq_type_i          : 0x%x\n"
				"\r frame_type           : 0x%X\n"
				"\r trans_recv_sel       : 0x%X\n"
				"\r tx_on                : 0x%X\n"
				"\r rx_on                : 0x%x\n"
				"\r lna_on               : 0x%X\n"
				"\r pa_pwr_on            : 0x%X\n"
				"\r pa_ramp_en           : 0x%X\n"
				"\r pa_tx_on             : 0x%X\n"
				"\r tx_freq_cfg_en       : 0x%x\n"
				"\r freq_type_o          : 0x%X\n"
				"\r fpga_en_sync         : 0x%X\n"
				"\r fpga_send_data_vld   : 0x%X\n"
				"\r fpga_tx_pwr_ctrl_vld : 0x%x\n"
				"\r arm_send_data_vld    : 0x%X\n"
				"\r ***************************************\n",\
				fpga_mem_nm_debug->downlink_dout,\
				fpga_mem_nm_debug->freq_type_i,\
				fpga_mem_nm_debug->frame_type,\
				fpga_mem_nm_debug->trans_recv_sel,\
				fpga_mem_nm_debug->tx_on,\
				fpga_mem_nm_debug->rx_on,\
				fpga_mem_nm_debug->lna_on,\
				fpga_mem_nm_debug->pa_pwr_on,\
				fpga_mem_nm_debug->pa_ramp_en,\
				fpga_mem_nm_debug->pa_tx_on,\
				fpga_mem_nm_debug->tx_freq_cfg_en,\
				fpga_mem_nm_debug->freq_type_o,\
				fpga_mem_nm_debug->fpga_en_sync,\
				fpga_mem_nm_debug->fpga_send_data_vld,\
				fpga_mem_nm_debug->fpga_tx_pwr_ctrl_vld,\
				fpga_mem_nm_debug->arm_send_data_vld);
		}
	}
	pthread_exit(NULL);
}


void send_remote_nm_get_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char src_id, unsigned char dst_id, unsigned char * data)
{
	NAS_AI_PAYLOAD nas_ai_payload;
	NAS_INF_DL_T nm_ai_msg;
	memset(&nas_ai_payload, 0, sizeof(NAS_AI_PAYLOAD));
	memset(&nm_ai_msg, 0, sizeof(NAS_INF_DL_T));
	
	nas_ai_payload.cmd_code = (unsigned char)cmd_code;
	nas_ai_payload.nm_type = nm_type;
	nas_ai_payload.op_code = OP_CODE_GET_ACK;
	nas_ai_payload.src_id = src_id;
	nas_ai_payload.dst_id = dst_id;
	memcpy(nas_ai_payload.data, data, sizeof(nas_ai_payload.data));
	nas_ai_payload.crc = crc8((unsigned char *)&nas_ai_payload, sizeof(NAS_AI_PAYLOAD) - 1);

	nm_ai_msg.TimeStamp = 0xFFFFFFFF;
	nm_ai_msg.SlotNum = 0x00;
	nm_ai_msg.TxFreq1 = 0x01;
	nm_ai_msg.TxFreq2 = 0x00;
	nm_ai_msg.ResFrqSlot = 0;
	nm_ai_msg.tDataLink[0].MsgType = DI_MSG_NM;
	nm_ai_msg.tDataLink[0].CC = local_cc;
	nm_ai_msg.tDataLink[0].DataType = DI_MSG_NM;
	nm_ai_msg.tDataLink[0].DataLen = sizeof(NAS_AI_PAYLOAD);
	memcpy(nm_ai_msg.tDataLink[0].PayLoad, &nas_ai_payload, sizeof(NAS_AI_PAYLOAD));

	
    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			"\n |%s||%s||%d|\n"
			"\r cmd_code  : 0x%X\n"
			"\r nm_type   : 0x%x\n"
			"\r op_code   : 0x%X\n"
			"\r src_id	  : 0x%X\n"
			"\r dst_id	  : 0x%X\n"
			"\r crc 	  : 0x%X\n"
			"\r TimeStamp : 0x%lX\n"
			"\r SlotNum   : 0x%X\n"
			"\r TxFreq1   : 0x%X\n"
			"\r TxFreq2   : 0x%X\n"
			"\r ResFrqSlot: 0x%X\n"
			"\r MsgType   : 0x%X\n"
			"\r CC        : 0x%X\n"
			"\r DataType  : 0x%X\n"
			"\r DataLen   : 0x%X\n",\
			__FILE__, __FUNCTION__, __LINE__,\
			nas_ai_payload.cmd_code, nas_ai_payload.nm_type, nas_ai_payload.op_code,\
			nas_ai_payload.src_id, nas_ai_payload.dst_id, nas_ai_payload.crc,\
			nm_ai_msg.TimeStamp, nm_ai_msg.SlotNum, nm_ai_msg.TxFreq1, nm_ai_msg.TxFreq2, nm_ai_msg.ResFrqSlot,\
			nm_ai_msg.tDataLink[0].MsgType, nm_ai_msg.tDataLink[0].CC, nm_ai_msg.tDataLink[0].DataType, nm_ai_msg.tDataLink[0].DataLen);
	}
	
	sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgrh_adp), sizeof(struct sockaddr_in)); 
}


void send_remote_nm_set_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char src_id, unsigned char dst_id)
{
	NAS_AI_PAYLOAD nas_ai_payload;
	NAS_INF_DL_T nm_ai_msg;
	memset(&nas_ai_payload, 0, sizeof(NAS_AI_PAYLOAD));
	memset(&nm_ai_msg, 0, sizeof(NAS_INF_DL_T));
	
	nas_ai_payload.cmd_code = (unsigned char)cmd_code;
	nas_ai_payload.nm_type = nm_type;
	nas_ai_payload.op_code = OP_CODE_SET_ACK;
	nas_ai_payload.src_id = src_id;
	nas_ai_payload.dst_id = dst_id;
	nas_ai_payload.crc = crc8((unsigned char *)&nas_ai_payload, sizeof(NAS_AI_PAYLOAD) - 1);

	nm_ai_msg.TimeStamp = 0xFFFFFFFF;
	nm_ai_msg.SlotNum = 0x00;
	nm_ai_msg.TxFreq1 = 0x01;
	nm_ai_msg.TxFreq2 = 0x00;
	nm_ai_msg.ResFrqSlot = 0;
	nm_ai_msg.tDataLink[0].MsgType = DI_MSG_NM;
	nm_ai_msg.tDataLink[0].CC = local_cc;
	nm_ai_msg.tDataLink[0].DataType = DI_MSG_NM;
	nm_ai_msg.tDataLink[0].DataLen = sizeof(NAS_AI_PAYLOAD);
	memcpy(nm_ai_msg.tDataLink[0].PayLoad, &nas_ai_payload, sizeof(NAS_AI_PAYLOAD));

	
    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			"\n |%s||%s||%d|\n"
			"\r cmd_code  : 0x%X\n"
			"\r nm_type   : 0x%x\n"
			"\r op_code   : 0x%X\n"
			"\r src_id	  : 0x%X\n"
			"\r dst_id	  : 0x%X\n"
			"\r crc 	  : 0x%X\n"
			"\r TimeStamp : 0x%lX\n"
			"\r SlotNum   : 0x%X\n"
			"\r TxFreq1   : 0x%X\n"
			"\r TxFreq2   : 0x%X\n"
			"\r ResFrqSlot: 0x%X\n"
			"\r MsgType   : 0x%X\n"
			"\r CC		  : 0x%X\n"
			"\r DataType  : 0x%X\n"
			"\r DataLen   : 0x%X\n",\
			__FILE__, __FUNCTION__, __LINE__,\
			nas_ai_payload.cmd_code, nas_ai_payload.nm_type, nas_ai_payload.op_code,\
			nas_ai_payload.src_id, nas_ai_payload.dst_id, nas_ai_payload.crc,\
			nm_ai_msg.TimeStamp, nm_ai_msg.SlotNum, nm_ai_msg.TxFreq1, nm_ai_msg.TxFreq2, nm_ai_msg.ResFrqSlot,\
			nm_ai_msg.tDataLink[0].MsgType, nm_ai_msg.tDataLink[0].CC, nm_ai_msg.tDataLink[0].DataType, nm_ai_msg.tDataLink[0].DataLen);			
	}
	
	sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgrh_adp), sizeof(struct sockaddr_in)); 
}


void send_local_nm_get_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char * data)
{
	NM_IPC_MSG nm_ipc_msg;
	memset((unsigned char *)&nm_ipc_msg, 0 ,sizeof(NM_IPC_MSG));
	nm_ipc_msg.nm_type = nm_type;
	nm_ipc_msg.src_id = local_dev_id;
	nm_ipc_msg.dst_id = local_dev_id;
	nm_ipc_msg.cmd_code = cmd_code;
	nm_ipc_msg.op_code = OP_CODE_GET_ACK;
	memcpy(nm_ipc_msg.payload, data, sizeof(nm_ipc_msg.payload));

    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			    "\n |%s||%s||%d|\n"
			    "\r nm_type  : 0x%X\n"
			    "\r src_id   : 0x%X\n"
			    "\r dst_id   : 0x%X\n"
			    "\r cmd_code : 0x%X\n"
			    "\r op_code  : 0x%X\n",\
			    __FILE__, __FUNCTION__, __LINE__,\
			    nm_ipc_msg.nm_type, nm_ipc_msg.src_id, nm_ipc_msg.dst_id, nm_ipc_msg.cmd_code, nm_ipc_msg.op_code);
	}

	sendto(sock_ipc, &nm_ipc_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgrh_mgra), sizeof(struct sockaddr_in)); 
}


void send_local_nm_set_ack(unsigned short cmd_code, unsigned char nm_type)
{
	NM_IPC_MSG nm_ipc_msg;
	memset((unsigned char *)&nm_ipc_msg, 0 ,sizeof(NM_IPC_MSG));
	nm_ipc_msg.nm_type = nm_type;
	nm_ipc_msg.src_id = local_dev_id;
	nm_ipc_msg.dst_id = local_dev_id;
	nm_ipc_msg.cmd_code = cmd_code;
	nm_ipc_msg.op_code = OP_CODE_SET_ACK;

    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			    "\n |%s||%s||%d|\n"
			    "\r nm_type  : 0x%X\n"
			    "\r src_id   : 0x%X\n"
			    "\r dst_id   : 0x%X\n"
			    "\r cmd_code : 0x%X\n"
			    "\r op_code  : 0x%X\n",\
			    __FILE__, __FUNCTION__, __LINE__,\
			    nm_ipc_msg.nm_type, nm_ipc_msg.src_id, nm_ipc_msg.dst_id, nm_ipc_msg.cmd_code, nm_ipc_msg.op_code);
	}

	sendto(sock_ipc, &nm_ipc_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgrh_mgra), sizeof(struct sockaddr_in)); 
}

void send_cc_set_cmd(unsigned short cmd_code, unsigned char * payload)
{
	NM_IPC_MSG nm_ipc_msg;
	memset((unsigned char *)&nm_ipc_msg, 0 ,sizeof(NM_IPC_MSG));
	nm_ipc_msg.nm_type = NM_TYPE_CENTER;
	nm_ipc_msg.src_id = local_dev_id;
	nm_ipc_msg.dst_id = local_dev_id;
	nm_ipc_msg.cmd_code = cmd_code;
	nm_ipc_msg.op_code = OP_CODE_SET;
	memcpy(nm_ipc_msg.payload, payload, sizeof(nm_ipc_msg.payload));
	
    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			    "\n |%s||%s||%d|\n"
			    "\r nm_type  : 0x%X\n"
			    "\r src_id   : 0x%X\n"
			    "\r dst_id   : 0x%X\n"
			    "\r cmd_code : 0x%X\n"
			    "\r op_code  : 0x%X\n",\
			    __FILE__, __FUNCTION__, __LINE__,\
			    nm_ipc_msg.nm_type, nm_ipc_msg.src_id, nm_ipc_msg.dst_id, nm_ipc_msg.cmd_code, nm_ipc_msg.op_code);
	}
	
	sendto(sock_ipc, &nm_ipc_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_cc), sizeof(struct sockaddr_in)); 
}


void send_cc_get_response_cmd(unsigned short cmd_code, unsigned char * payload)
{
	NM_IPC_MSG nm_ipc_msg;
	memset((unsigned char *)&nm_ipc_msg, 0 ,sizeof(NM_IPC_MSG));
	nm_ipc_msg.nm_type = NM_TYPE_CENTER;
	nm_ipc_msg.src_id = local_dev_id;
	nm_ipc_msg.dst_id = local_dev_id;
	nm_ipc_msg.cmd_code = cmd_code;
	nm_ipc_msg.op_code = OP_CODE_GET_ACK;
	memcpy(nm_ipc_msg.payload, payload, sizeof(nm_ipc_msg.payload));
	
    if (shm_ipc_addr->mgr_printf[1])
    {
		LOG_DEBUG(s_i4LogMsgId,\
			    "\n |%s||%s||%d|\n"
			    "\r nm_type  : 0x%X\n"
			    "\r src_id   : 0x%X\n"
			    "\r dst_id   : 0x%X\n"
			    "\r cmd_code : 0x%X\n"
			    "\r op_code  : 0x%X\n",\
			    __FILE__, __FUNCTION__, __LINE__,\
			    nm_ipc_msg.nm_type, nm_ipc_msg.src_id, nm_ipc_msg.dst_id, nm_ipc_msg.cmd_code, nm_ipc_msg.op_code);
	}
	
	sendto(sock_ipc, &nm_ipc_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_cc), sizeof(struct sockaddr_in)); 
}

void init_local_val(void)
{
	sem_cfg_p();
	local_dev_id = shm_cfg_addr->dev_id.val;
	local_cc = shm_cfg_addr->cc.val;
	local_stun_flag = shm_cfg_addr->stun_flag.val;
	local_kill_flag = shm_cfg_addr->kill_flag.val;
	sem_cfg_v();
}

int init_alarm_table(void)
{
	int i = 0;
	UINT8 alarm_code;
	
	ptMgrAlarm = ptIPCShm->alarm_center;
	memset(ptMgrAlarm,0x00,MGR_ALARM_MAX * 3 * sizeof(ALARM_ITEM));

	for(i = 0;i < MGR_ALARM_MAX * 3; i++)
	{
		ptMgrAlarm->alm_code = 0xff;
		ptMgrAlarm++;
	}

	ptMgrAlarm = ptIPCShm->alarm_center;
	alarm_code = MGR_ALARM_CENTER_1;
	for(i = 0;i < MGR_ALARM_CENTER_NUM; i++,ptMgrAlarm++)
	{
		ptMgrAlarm->alm_code = alarm_code++;
	}

	ptMgrAlarm = ptIPCShm->alarm_fpga;
	alarm_code = MGR_ALARM_FPGA_1;
	for(i = 0;i < MGR_ALARM_FPGA_NUM; i++,ptMgrAlarm++)
	{
		ptMgrAlarm->alm_code = alarm_code++;
	}

	memset(g_tMgrAlarm,0x00,sizeof(g_tMgrAlarm));
	return 0;
}
void notify_fpga(void)
{
    unsigned char gpio_value = 0;
	usleep(10000);
    write(fpga_mem_nm_fd, &gpio_value, 1);
	usleep(100);
	gpio_value = 1;
	write(fpga_mem_nm_fd, &gpio_value, 1);
}


int init_fpga_param(void)
{
    unsigned int count=10;
	int i=0;
    
	memset((unsigned char *)&eeprom_fpga_param, 0, sizeof(EEPROM_FPGA_PARAM));
	if (read_fpga_param_from_eeprom())
	{
		printf("<%s> <%s> <%d> read_fpga_param_from_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
	}
	memcpy(&fpga_mem_nm_param->calibration_param, &eeprom_fpga_param, sizeof(EEPROM_FPGA_PARAM));

	printf("nm_memory_scan_mode = %u\n",  shm_cfg_addr->scan_mode.val);
	printf("nm_memory_freq_offset = %u\n",  shm_cfg_addr->freq_offset.val);
	printf("nm_memory_freq = %u\n",  shm_cfg_addr->freq.val);
	printf("nm_memory_power = %u\n", shm_cfg_addr->power.val);
	printf("nm_memory_open_close_loop = %u\n",shm_cfg_addr->open_close_loop.val);
	printf("nm_memory_locking_time = %u\n", shm_cfg_addr->locking_time.val);
	printf("nm_memory_half_variance_threshold = %u\n", shm_cfg_addr->half_variance_threshold.val);
	printf("nm_memory_dev_call_timeout = %u\n", shm_cfg_addr->dev_call_timeout.val);



	fpga_mem_nm_param->scan_mode= shm_cfg_addr->scan_mode.val;
	fpga_mem_nm_param->freq_offset= shm_cfg_addr->freq_offset.val*1000;
	fpga_mem_nm_param->freq = shm_cfg_addr->freq.val * 2;
	fpga_mem_nm_param->power = shm_cfg_addr->power.val;
	fpga_mem_nm_param->open_close_loop = shm_cfg_addr->open_close_loop.val;
	fpga_mem_nm_param->locking_time = shm_cfg_addr->locking_time.val * 92160;
	fpga_mem_nm_param->half_variance_threshold = shm_cfg_addr->half_variance_threshold.val;
	if(shm_cfg_addr->dev_call_timeout.val < 20)
	{

	   sem_cfg_p();
	   shm_cfg_addr->dev_call_timeout.val=20; 
	   sem_cfg_v();
	   save_ini_file();									
	}
	fpga_mem_nm_param->dev_call_timeout= shm_cfg_addr->dev_call_timeout.val*5760000;
	fpga_mem_nm_w->flag = FLAG_PARAM;
    
	usleep(500000);
	notify_fpga();
    while(--count)
	{
		sleep(1);
		if(fpga_mem_nm_r->flag==FLAG_OK)
		{
		    printf("/****************************************/\n");
			printf("count =%d\n",i);
            printf("/****************************************/\n");
			break;
		}
		else
		{
			notify_fpga();
			i++;
		}
		
	}
	if(count==0)
	{	
	    printf("/****************************************/\n");
		printf("notify fpga  fail\n");
        printf("/****************************************/\n");
	}

    
    printf("scan_mode = %u\n", fpga_mem_nm_param->scan_mode);
	printf("freq_offset = %u\n", fpga_mem_nm_param->freq_offset);
	printf("freq = %u\n", fpga_mem_nm_param->freq);
	printf("power = %u\n", fpga_mem_nm_param->power);
	printf("open_close_loop = %u\n", fpga_mem_nm_param->open_close_loop);
	printf("locking_time = %u\n", fpga_mem_nm_param->locking_time);
	printf("half_variance_threshold = %u\n", fpga_mem_nm_param->half_variance_threshold);
	printf("dev_call_timeout = %u\n", fpga_mem_nm_param->dev_call_timeout);
	return 0;
}


int write_fpga_param_to_eeprom(void)
{
	if (init_eeprom())
	{
		printf("<%s> <%s> <%d> init_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	if (eeprom_write(eeprom_fd, 0x0, (unsigned char *)&eeprom_fpga_param, sizeof(EEPROM_FPGA_PARAM)) == -1)
	{
		printf("eeprom_write fpga param err\n");
		close(eeprom_fd);
		return -1;
	}
	printf("eeprom_write fpga param ok\n");
	close(eeprom_fd);
	return 0;
}


int read_fpga_param_from_eeprom(void)
{
    if (init_eeprom())
	{
		printf("<%s> <%s> <%d> init_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	if (eeprom_read(eeprom_fd, 0x0, (unsigned char *)&eeprom_fpga_param, sizeof(EEPROM_FPGA_PARAM)) == -1)
	{
		printf("eeprom_read fpga param err\n");
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	printf("eeprom_read fpga param ok\n");
	return 0;
}


int write_esn_param_to_eeprom(void)
{
	if (init_eeprom())
	{
		printf("<%s> <%s> <%d> init_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	if (eeprom_write(eeprom_fd, 0x400, (unsigned char *)&eeprom_esn_param, sizeof(EEPROM_ESN_PARAM)) == -1)
	{
		printf("eeprom_write esn param err\n");
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	printf("eeprom_write esn param ok\n");
	return 0;
}


int read_esn_param_from_eeprom(void)
{
	if (init_eeprom())
	{
		printf("<%s> <%s> <%d> init_eeprom err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	if (eeprom_read(eeprom_fd, 0x400, (unsigned char *)&eeprom_esn_param, sizeof(EEPROM_ESN_PARAM)) == -1)
	{
		printf("eeprom_read esn param err\n");
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	printf("eeprom_read esn param ok\n");
	return 0;
}


/**
 * @brief   配置系统 ip、网管、掩码等
 * @author  王存刚
 * @since   trunk.0001
 * @bug
 */
void net_config(void)
{
    char cmd_ip_buf[128] = {0};
    char cmd_mac_buf[128] = {0};
    char cmd_route_buf[128] = {0};

	if (read_esn_param_from_eeprom())
	{
		printf("read_esn_param_from_eeprom err\n");
		return;
	}

    system("ifdown eth0");
    sleep(1);
	if ((eeprom_esn_param.mac_addr[0] == 0xFF) && (eeprom_esn_param.mac_addr[1] == 0xFF) && (eeprom_esn_param.mac_addr[2] == 0xFF)\
		&& (eeprom_esn_param.mac_addr[3] == 0xFF) && (eeprom_esn_param.mac_addr[4] == 0xFF) && (eeprom_esn_param.mac_addr[5] == 0xFF))
	{
		printf("no mac_addr in eeprom\n");
	}
	else
	{
		sprintf(cmd_mac_buf, "ifconfig eth0 hw ether %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",\
			eeprom_esn_param.mac_addr[0], eeprom_esn_param.mac_addr[1], eeprom_esn_param.mac_addr[2],\
			eeprom_esn_param.mac_addr[3], eeprom_esn_param.mac_addr[4], eeprom_esn_param.mac_addr[5]);
		system(cmd_mac_buf);
	}
    sleep(1);
	system("ifup eth0");
    sprintf(cmd_ip_buf, "ifconfig eth0 %s netmask %s", shm_cfg_addr->local_ip.buf, shm_cfg_addr->subnet_mask.buf);
    system(cmd_ip_buf);
    sleep(1);
    sprintf(cmd_route_buf, "route add default gw %s", shm_cfg_addr->gateway.buf);
    system(cmd_route_buf);
    
    return;
}

int test_code(int status)
{
    if (-1 == status)
    {
        printf("system return failed!\n");
        return -1;
    }
    else
    {
        if (WIFEXITED(status))
        {
            if(0 == WEXITSTATUS(status))
            {
                return 0;
            }
            else
            {
                printf(" WEXITSTATUS(status) failed!\n");
                return -1;
            }
        }
        else
        {
            printf("WIFEXITED(status) failed!\n");
            return -1;
        }
    }
}


#if 0
void signal_handler(int num)
{
    unsigned int area_flag = 0;
    int status=0;

    if (iq_switch_status == 1)  // 打开-FPGA
    {
        if(0 == iq_switch_open_count)
        {
            status=system("mount /dev/mmcblk0p1 /mnt");
            if(-1==test_code(status))
            {
                printf("mount /dev/mmcblk0p1 fail\n");
                iq_mount_sd_flag=0;
                send_iq_close_to_queque();
                return ;
            }
            else
            {
                mem_fd_fp= fopen("/mnt/iqdata.txt","wb");
                iq_mount_sd_flag=1;
                if(mem_fd_fp == NULL)
                {
                    printf("File /mnt/iqdata.txt open error\n");
                    system("umount /mnt");
                    iq_mount_sd_flag=0;
                    fclose(mem_fd_fp);
                    send_iq_close_to_queque();
                    return;
                }
            }
            ++iq_switch_open_count;
        }


        if ((ftell(mem_fd_fp)+DATA_LENGTH) > MAX_VOLUMN_IQ_DATA)
        {
            printf("save iq data excess 60M \n");
            if(mem_fd_fp != NULL)
            {
                fclose(mem_fd_fp);
                system("umount /mnt");
                iq_mount_sd_flag = 0;
                mem_fd_fp=NULL;
            }
            send_iq_close_to_queque();
            return;
        }
        else
        {


            area_flag = ioctl(mem_fd, GET_DATA_AREA_CMD, 0);

            if(area_flag == UP_AREA_FLAG)
            {
                printf("first area write\n");
                fwrite(iq_start_addr, DATA_LENGTH, 1, mem_fd_fp);
            }
            else if(area_flag == DOWN_AREA_FLAG)
            {

                printf("second area write\n");
                fwrite(iq_start_addr+MEM_LENGTH/2,DATA_LENGTH,1,mem_fd_fp);
            }
            else
            {
                printf("app ioctl error!\n");
            }
        }
    }

    return;
}
#endif

#if 0
// 通知FPGA关闭
void send_iq_close_to_queque(void)
{
    QUEUE_MSG q_msg;

    int close = 0x00;
    iq_excess_volumn_flag = 0;  // 设置正常情况，通知FPGA关闭

    q_msg.mtype = MSG_TYPE_CMD;

    q_msg.ipc_msg.dst_id=local_dev_id;
    q_msg.ipc_msg.src_id=local_dev_id;
    q_msg.ipc_msg.nm_type=NM_TYPE_NM;
    q_msg.ipc_msg.cmd_code=CMD_CODE_SAVE_IQ_DATA;
    q_msg.ipc_msg.op_code=OP_CODE_SET;

    memcpy(q_msg.ipc_msg.payload, &close, sizeof(int));  //通知FPGA关闭数据保存

    if (-1 == msgsnd(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0))
    {
        // 没通知成功
        printf("<%s> <%s> <%d> qid_mgrh msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);

    }
    else
    {
       iq_excess_volumn_flag=1;  // 设置为异常  (通知成功)
    }
}
#endif

// 通知FPGA关闭
void send_iq_close_to_queque(int flag)
{
    QUEUE_MSG q_msg;

    int close = 0x00;
    iq_excess_volumn_flag = 0;  // 设置正常情况，通知FPGA关闭

    q_msg.mtype = MSG_TYPE_CMD;

    q_msg.ipc_msg.dst_id=local_dev_id;
    q_msg.ipc_msg.src_id=local_dev_id;
    q_msg.ipc_msg.nm_type=NM_TYPE_NM;
    q_msg.ipc_msg.cmd_code=CMD_CODE_SAVE_IQ_DATA;
    q_msg.ipc_msg.op_code=OP_CODE_SET;

    memcpy(q_msg.ipc_msg.payload, &close, sizeof(int));  //通知FPGA关闭数据保存

    if (-1 == msgsnd(qid_mgrh, &q_msg, sizeof(NM_IPC_MSG), 0))
    {
        // 没通知成功
        printf("<%s> <%s> <%d> qid_mgrh msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);

    }
    else
    {
       iq_excess_volumn_flag = flag;  // 设置为异常  (通知成功)
    }
}



