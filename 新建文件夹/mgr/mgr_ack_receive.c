/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_ack_receive.c
 * @brief   网管响应接收与处理模块
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   函数列表
 *   1. main       主函数
 *   2. mgra_ack_msg_queue_create       创建消息队列
 *   3. init_ipc_socket       初始化socket
 *   4. mgra_ack_pthread_create       创建线程
 *   5. pthread_mgrh_mgra_recv       本地处理模块消息接收线程函数
 *   6. pthread_adp_mgra_recv       空口适配模块消息接收线程函数
 *   7. pthread_ack_handle      网管响应处理线程函数
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
#include "mgr_ack_receive.h"
#include "mgr_public_func.h"


#undef TASK_NAME
#define TASK_NAME	"MGR_ACK_RECEIVE"

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

struct sockaddr_in sockaddr_mgra_cc;
struct sockaddr_in sockaddr_mgrh_mgra;
struct sockaddr_in sockaddr_adp_mgra;
struct sockaddr_in sockaddr_mgra_adp;
int sock_ipc;
int sock_mgrh_mgra;
int sock_adp_mgra;
int qid_mgra_ack;

pthread_t tid_mgrh_mgra_recv;
pthread_t tid_adp_mgra_recv;
pthread_t tid_ack_handle;
pthread_t tid_clear_lock_slot;


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

//unsigned char mgr_lock_slot;  // 0xFF - 初始态;  0x00 - F1S1;  0x01 - F1S2;  0x02 - F2S1;  0x03 - F2S2
//DLL_FPGA_SHM_T * fpga_mem_config = NULL;
//int fpga_mem_config_fd;



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
	
	if (shm_nm_attch())
	{
		printf("<%s> <%s> <%d> shm_nm_attch err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	init_local_val();
	
	if (init_ipc_socket())
	{
		printf("<%s> <%s> <%d> init_ipc_socket err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	if (mgra_ack_msg_queue_create())
	{
		printf("<%s> <%s> <%d> mgra_ack_msg_queue_create err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	if (mgra_ack_pthread_create())
	{
		printf("<%s> <%s> <%d> mgra_ack_pthread_create err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	while(1)
	{
		sleep(10);
	}
	
	return 0;
}


void init_local_val(void)
{
	sem_cfg_p();
	local_dev_id = shm_cfg_addr->dev_id.val;
	local_cc = shm_cfg_addr->cc.val;
	sem_cfg_v();
}


int mgra_ack_msg_queue_create(void)
{
	key_t key;

	key = ftok(FTOK_F_NAME, FTOK_ID_Q_MGRA_ACK);
	qid_mgra_ack = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
	if (-1 == qid_mgra_ack)
	{
		return -1;
	}

	return 0;
}


int init_ipc_socket(void)
{
	memset(&sockaddr_mgra_cc, 0, sizeof(struct sockaddr_in));
	sockaddr_mgra_cc.sin_family = AF_INET;
	sockaddr_mgra_cc.sin_port = htons(SOCK_PORT_MGRA_CC);
	sockaddr_mgra_cc.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_mgrh_mgra, 0, sizeof(struct sockaddr_in));
	sockaddr_mgrh_mgra.sin_family = AF_INET;
	sockaddr_mgrh_mgra.sin_port = htons(SOCK_PORT_MGRH_MGRA);
	sockaddr_mgrh_mgra.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_adp_mgra, 0, sizeof(struct sockaddr_in));
	sockaddr_adp_mgra.sin_family = AF_INET;
	sockaddr_adp_mgra.sin_port = htons(SOCK_PORT_ADP_MGRA);
	sockaddr_adp_mgra.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_mgra_adp, 0, sizeof(struct sockaddr_in));
	sockaddr_mgra_adp.sin_family = AF_INET;
	sockaddr_mgra_adp.sin_port = htons(SOCK_PORT_MGR_ADP);
	sockaddr_mgra_adp.sin_addr.s_addr = inet_addr("127.0.0.1");

	sock_ipc = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_ipc) 
	{
		printf("sock_ipc socket error\n");
		return -1;
	}

	sock_mgrh_mgra = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_mgrh_mgra) 
	{
		printf("sock_mgrh_mgra socket error\n");
		return -1;
	}
    if (-1 == bind(sock_mgrh_mgra, (struct sockaddr *)(&sockaddr_mgrh_mgra), sizeof(struct sockaddr_in))) 
    {
        printf("sock_mgrh_mgra bind error\n");
        return -1;
    }   

	sock_adp_mgra = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_adp_mgra) 
	{
		printf("sock_adp_mgra socket error\n");
		return -1;
	}
    if (-1 == bind(sock_adp_mgra, (struct sockaddr *)(&sockaddr_adp_mgra), sizeof(struct sockaddr_in))) 
    {
        printf("sock_adp_mgra bind error\n");
        return -1;
    }   

	return 0;
}


int mgra_ack_pthread_create(void)
{
	if (pthread_create(&tid_mgrh_mgra_recv, NULL, pthread_mgrh_mgra_recv, NULL)) 
	{
		printf("pthread_create tid_mgrh_mgra_recv err\n");
		return -1;
	}
	pthread_detach(tid_mgrh_mgra_recv);

	if (pthread_create(&tid_adp_mgra_recv, NULL, pthread_adp_mgra_recv, NULL)) 
	{
		printf("pthread_create tid_adp_mgra_recv err\n");
		return -1;
	}
	pthread_detach(tid_adp_mgra_recv);

	if (pthread_create(&tid_ack_handle, NULL, pthread_ack_handle, NULL)) 
	{
		printf("pthread_create tid_ack_handle err\n");
		return -1;
	}
	pthread_detach(tid_ack_handle);

	return 0;
}


void * pthread_mgrh_mgra_recv(void *arg)
{
	int ret;
	int rd_cnt;
	fd_set fds;
	NM_IPC_MSG ack_msg;
	QUEUE_MSG q_msg;
	
	while (1)
	{
        FD_ZERO(&fds);
        FD_SET(sock_mgrh_mgra, &fds);
		struct timeval tv;    
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        ret = select(sock_mgrh_mgra + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) 
        {
            printf("<%s> <%s> <%d> select error!\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        } 
        else if (0 == ret) 
        {
            continue;       
        }

		if (FD_ISSET(sock_mgrh_mgra, &fds)) 
		{
			rd_cnt = recvfrom(sock_mgrh_mgra, &ack_msg, sizeof(NM_IPC_MSG), 0, NULL, NULL);
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
						ack_msg.nm_type, ack_msg.src_id, ack_msg.dst_id, ack_msg.cmd_code, ack_msg.op_code);
			}
           //////////////////测试////////////////
           
		   

			if (ack_msg.nm_type == NM_TYPE_CENTER)
			{
				sendto(sock_ipc, &ack_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_cc), sizeof(struct sockaddr_in)); 
			}
			else
			{
				q_msg.mtype = MSG_TYPE_ACK;
				memcpy(&q_msg.ipc_msg, &ack_msg, sizeof(NM_IPC_MSG));
				if (-1 == msgsnd(qid_mgra_ack, &q_msg, sizeof(NM_IPC_MSG), 0))
				{
					printf("<%s> <%s> <%d> msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
				}
			}
		}
	}
	
	pthread_exit(NULL);
}


void * pthread_adp_mgra_recv(void *arg)
{
	int ret;
	int rd_cnt;
	fd_set fds;
	NAS_INF_UL_T recv_ai_msg;
	NAS_INF_DL_T send_ai_msg;
	NAS_AI_PAYLOAD nas_ai_payload;
	NM_IPC_MSG ack_msg;
	QUEUE_MSG q_msg;
	
	while (1)
	{
        FD_ZERO(&fds);
        FD_SET(sock_adp_mgra, &fds);
		struct timeval tv;    
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        ret = select(sock_adp_mgra + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) 
        {
            printf("<%s> <%s> <%d> select error!\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        } 
        else if (0 == ret) 
        {
            continue;       
        }

		if (FD_ISSET(sock_adp_mgra, &fds)) 
		{
			rd_cnt = recvfrom(sock_adp_mgra, &recv_ai_msg, sizeof(NAS_INF_UL_T), 0, NULL, NULL);
			if (rd_cnt < 0)
			{
				printf("<%s> <%s> <%d> recvfrom err\n", __FILE__, __FUNCTION__, __LINE__);
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

			if ((nas_ai_payload.op_code == OP_CODE_ALARM) &&(nas_ai_payload.nm_type==NM_TYPE_NM))
			{
				if (nas_ai_payload.src_id !=local_dev_id)
				{
				    ///////发送本地手咪
					if (nas_ai_payload.crc != crc8((unsigned char *)&nas_ai_payload, sizeof(NAS_AI_PAYLOAD) - 1))
					{
						printf("<%s> <%s> <%d> crc8 err\n", __FILE__, __FUNCTION__, __LINE__);
						continue;
					}

					ack_msg.nm_type =NM_TYPE_CENTER;
					ack_msg.src_id = nas_ai_payload.src_id;
					ack_msg.dst_id = nas_ai_payload.dst_id;
					ack_msg.cmd_code = nas_ai_payload.cmd_code;
					ack_msg.op_code = nas_ai_payload.op_code;
					memcpy(ack_msg.payload, nas_ai_payload.data, sizeof(nas_ai_payload.data));
					
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
								ack_msg.nm_type, ack_msg.src_id, ack_msg.dst_id, ack_msg.cmd_code, ack_msg.op_code);
					}
					sendto(sock_ipc, &ack_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_cc), sizeof(struct sockaddr_in)); 

					/////////////////发往空口
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

					if (shm_ipc_addr->mgr_printf[1])
					{
						LOG_DEBUG(s_i4LogMsgId,\
								"\n alarm send ai success\n");
					}
			
					sendto(sock_ipc, &send_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in));
					
				}
			}
			else
			{
				if (nas_ai_payload.dst_id == local_dev_id)
				{
					if (nas_ai_payload.crc != crc8((unsigned char *)&nas_ai_payload, sizeof(NAS_AI_PAYLOAD) - 1))
					{
						printf("<%s> <%s> <%d> crc8 err\n", __FILE__, __FUNCTION__, __LINE__);
						continue;
					}
					
					ack_msg.nm_type = nas_ai_payload.nm_type;
					ack_msg.src_id = nas_ai_payload.src_id;
					ack_msg.dst_id = nas_ai_payload.dst_id;
					ack_msg.cmd_code = nas_ai_payload.cmd_code;
					ack_msg.op_code = nas_ai_payload.op_code;
					memcpy(ack_msg.payload, nas_ai_payload.data, sizeof(nas_ai_payload.data));
					
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
								ack_msg.nm_type, ack_msg.src_id, ack_msg.dst_id, ack_msg.cmd_code, ack_msg.op_code);
					}
					
					if (ack_msg.nm_type == NM_TYPE_CENTER)
					{
						sendto(sock_ipc, &ack_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_cc), sizeof(struct sockaddr_in)); 
					}
					else
					{
						q_msg.mtype = MSG_TYPE_ACK;
						memcpy(&q_msg.ipc_msg, &ack_msg, sizeof(NM_IPC_MSG));
						if (-1 == msgsnd(qid_mgra_ack, &q_msg, sizeof(NM_IPC_MSG), 0))
						{
							printf("<%s> <%s> <%d> msgsnd err, errno = %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
						}
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
			
					sendto(sock_ipc, &send_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in)); 
				}
			}
		}
	}
	
	pthread_exit(NULL);
}


void * pthread_ack_handle(void *arg)
{
	int ret;
	QUEUE_MSG q_msg;
	
	while(1)
	{
		ret = msgrcv(qid_mgra_ack, &q_msg, sizeof(NM_IPC_MSG), 0, 0);
		if (-1 == ret)
		{
			printf("<%s> <%s> <%d> msgrcv err\n", __FILE__, __FUNCTION__, __LINE__);
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
		
		switch (q_msg.ipc_msg.cmd_code)
		{
			case CMD_CODE_DEV_FREQ:
				sem_nm_p();
				memcpy(&shm_nm_addr->dev_freq.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->dev_freq.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEV_POWER:
				sem_nm_p();
				memcpy(&shm_nm_addr->dev_Power.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->dev_Power.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_IQ_DATA:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_iq_data.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_iq_data.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_COMBINED_DATA:
				sem_nm_p();
				memcpy(&shm_nm_addr->combined_data.data, q_msg.ipc_msg.payload, 8);
				shm_nm_addr->combined_data.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_OPEN_CLOSE_LOOP:
				sem_nm_p();
				memcpy(&shm_nm_addr->open_close_loop.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->open_close_loop.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_EMISSIVE_VCO_FREQ:
				sem_nm_p();
				memcpy(&shm_nm_addr->emissive_vco_freq.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->emissive_vco_freq.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_ERROR_RATE_FREQ:
				sem_nm_p();
				memcpy(&shm_nm_addr->error_rate_freq.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->error_rate_freq.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_RECEIVING_VCO_FREQ:
				sem_nm_p();
				memcpy(&shm_nm_addr->receiving_vco_freq.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->receiving_vco_freq.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SECOND_LOCAL_OSCILLATOR:
				sem_nm_p();
				memcpy(&shm_nm_addr->second_local_oscillator.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->second_local_oscillator.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_POWER_AMPLIFIER_SWITCH:
				sem_nm_p();
				memcpy(&shm_nm_addr->power_amplifier_switch.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->power_amplifier_switch.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_FPGA_PARAM:
				sem_nm_p();
				memcpy(&shm_nm_addr->fpga_param.data, q_msg.ipc_msg.payload, sizeof(shm_nm_addr->fpga_param.data));
				shm_nm_addr->fpga_param.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_POWER:
				sem_nm_p();
				memcpy(&shm_nm_addr->power.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->power.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_QUERY_998:
				sem_nm_p();
				memcpy(&shm_nm_addr->query_998.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->query_998.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SET_981:
				sem_nm_p();
				memcpy(&shm_nm_addr->set_981.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->set_981.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SET_998:
				sem_nm_p();
				memcpy(&shm_nm_addr->set_998.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->set_998.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_QUERY_981:
				sem_nm_p();
				memcpy(&shm_nm_addr->query_981.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->query_981.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEV_CALL_TIMEOUT:
				sem_nm_p();
				memcpy(&shm_nm_addr->dev_call_timeout.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->dev_call_timeout.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_START_NEIGHBOR:
				sem_nm_p();
				memcpy(&shm_nm_addr->start_neighbor.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->start_neighbor.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_NEIGHBOR_PERIOD:
				sem_nm_p();
				memcpy(&shm_nm_addr->neighbor_period.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->neighbor_period.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_NEIGHBOR_REPORT_SWITCH:
				sem_nm_p();
				memcpy(&shm_nm_addr->neighbor_report_ai_switch.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->neighbor_report_ai_switch.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEBUG_EMISSIVE_FREQ:
				sem_nm_p();
				memcpy(&shm_nm_addr->debug_emissive_freq.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->debug_emissive_freq.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_AFC:
				sem_nm_p();
				memcpy(&shm_nm_addr->afc.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->afc.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_AFC:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_afc.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_afc.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_IQS:
				sem_nm_p();
				memcpy(&shm_nm_addr->iqs.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->iqs.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_IQS:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_iqs.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_iqs.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_POWER:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_power.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_power.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_OFFSET:
				sem_nm_p();
				memcpy(&shm_nm_addr->offset.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->offset.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_CALIBRATE_OFFSET:
				sem_nm_p();
				memcpy(&shm_nm_addr->calibrate_offset.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->calibrate_offset.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_OFFSET:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_offset.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_offset.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_PHASE:
				sem_nm_p();
				memcpy(&shm_nm_addr->phase.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->phase.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_CALIBRATE_PHASE:
				sem_nm_p();
				memcpy(&shm_nm_addr->calibrate_phase.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->calibrate_phase.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_PHASE:
				sem_nm_p();
				memcpy(&shm_nm_addr->save_phase.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->save_phase.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_ERROR_RATE:
				sem_nm_p();
				memcpy(&shm_nm_addr->error_rate.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->error_rate.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_START_DEBUG:
				sem_nm_p();
				memcpy(&shm_nm_addr->start_debug.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->start_debug.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_START_ERR_RATE:
				sem_nm_p();
				memcpy(&shm_nm_addr->start_error_rate.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->start_error_rate.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SELECT_SIGNAL_SOURCE:
				sem_nm_p();
				memcpy(&shm_nm_addr->select_signal_source.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->select_signal_source.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEBUG_OPEN_CLOSE_LOOP:
				sem_nm_p();
				memcpy(&shm_nm_addr->debug_open_close_loop.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->debug_open_close_loop.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEBUG_POWER:
				sem_nm_p();
				memcpy(&shm_nm_addr->debug_power.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->debug_power.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SLOT_CONTROL:
				sem_nm_p();
				memcpy(&shm_nm_addr->slot_control.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->slot_control.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_VGS:
				sem_nm_p();
				memcpy(&shm_nm_addr->vgs.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->vgs.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_LOCKING_TIME:
				sem_nm_p();
				memcpy(&shm_nm_addr->locking_time.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->locking_time.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_HALF_VARIANCE_THRESHOLD:
				sem_nm_p();
				memcpy(&shm_nm_addr->half_variance_threshold.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->half_variance_threshold.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_DEV_ID:
				sem_nm_p();
				memcpy(&shm_nm_addr->dev_id.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->dev_id.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_WORK_TIME_OUT:
				sem_nm_p();
				memcpy(&shm_nm_addr->work_time_out.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->work_time_out.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_GPS_TIME_OUT:
				sem_nm_p();
				memcpy(&shm_nm_addr->gps_time_out.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->gps_time_out.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_STUN_TIME_OUT:
				sem_nm_p();
				memcpy(&shm_nm_addr->stun_time_out.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->stun_time_out.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_START_TIME_OUT:
				sem_nm_p();
				memcpy(&shm_nm_addr->start_time_out.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->start_time_out.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_LOCAL_CC:
				sem_nm_p();
				memcpy(&shm_nm_addr->local_cc.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->local_cc.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_VERSION:
				sem_nm_p();
				memcpy(shm_nm_addr->version.data, q_msg.ipc_msg.payload, sizeof(q_msg.ipc_msg.payload));
				shm_nm_addr->version.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_TERMINAL_CC:
				sem_nm_p();
				memcpy(&shm_nm_addr->terminal_cc.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->terminal_cc.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_PROTOCOL_MODE:
				sem_nm_p();
				memcpy(&shm_nm_addr->protocol_mode.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->protocol_mode.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_LOCAL_IP:
				sem_nm_p();
				memcpy(shm_nm_addr->local_ip.data, q_msg.ipc_msg.payload, sizeof(q_msg.ipc_msg.payload));
				shm_nm_addr->local_ip.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_LOCAL_MAC:
				sem_nm_p();
				memcpy(shm_nm_addr->local_mac.data, q_msg.ipc_msg.payload, sizeof(q_msg.ipc_msg.payload));
				shm_nm_addr->local_mac.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_FPGA_VERSION:
				sem_nm_p();
				memcpy(shm_nm_addr->fpga_version.data, q_msg.ipc_msg.payload, sizeof(q_msg.ipc_msg.payload));
				shm_nm_addr->fpga_version.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_AIR_SUBNET_ID:
				sem_nm_p();
				memcpy(&shm_nm_addr->air_subnet_id.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->air_subnet_id.ack_flag = ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_FPGA_EEPROM:
				sem_nm_p();
				shm_nm_addr->fpga_eeprom.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SAVE_EEPROM:
				sem_nm_p();
				shm_nm_addr->save_eeprom.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
			case CMD_CODE_SCAN_MODE:
				sem_nm_p();
				memcpy(&shm_nm_addr->scan_mode.data, q_msg.ipc_msg.payload, sizeof(unsigned int)*2);
				shm_nm_addr->scan_mode.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;

			case CMD_CODE_ALARM_SWITCH:
				sem_nm_p();
				memcpy(&shm_nm_addr->alarm_switch_status.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->alarm_switch_status.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
         /*   case CMD_CODE_QUERY_ALARM:
				sem_nm_p();
				memcpy(&shm_nm_addr->alarm_data.data, q_msg.ipc_msg.payload, MGR_ALARM_BYTE_MAX);
				shm_nm_addr->alarm_data.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;*/
            case CMD_CODE_CLOSE_TRAN_THRESHOLD:
				sem_nm_p();
				memcpy(&shm_nm_addr->close_transmit_threshold.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->close_transmit_threshold.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_START_TEMP_ALARM :
				sem_nm_p();
				memcpy(&shm_nm_addr->tempratue_alarm_start_threshold.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->tempratue_alarm_start_threshold.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_CLOSE_TEMP_ALARM :
				sem_nm_p();
				memcpy(&shm_nm_addr->tempratue_alarm_close_threshold.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->tempratue_alarm_close_threshold.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_RESUME_TRAN_THRESHOLD :
				sem_nm_p();
				memcpy(&shm_nm_addr->resume_transmit_threshold.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->resume_transmit_threshold.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_MOTO_SWITCH:
				sem_nm_p();
				memcpy(&shm_nm_addr->moto_mode_switch.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->moto_mode_switch.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
            case CMD_CODE_THRESHOLD_OPT_SWITCH:
				sem_nm_p();
				memcpy(&shm_nm_addr->threshold_opt_switch.data, q_msg.ipc_msg.payload, sizeof(unsigned int));
				shm_nm_addr->threshold_opt_switch.ack_flag= ACK_FLAG_OK;
				sem_nm_v();
				break;
			default:
				printf("<%s> <%s> <%d> unknown cmd_code\n", __FILE__, __FUNCTION__, __LINE__);
				break;
		}
	}
	
	pthread_exit(NULL);
}


