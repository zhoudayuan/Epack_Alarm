/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_center_agent.c
 * @brief   ���Ĵ���ģ��
 * @author  �����
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   �����б�
 *   1. main       ������
 *   2. init_ipc_socket       ��ʼ��socket
 *   3. mgra_cc_pthread_create       �����߳�
 *   4. pthread_cc_mgra_recv       ���տ������������̺߳���
 *   5. init_local_val      ��ʼ������ȫ�ֱ���
 *
 *   �޸���ʷ
 *   2016-8-1                �����                     �����ļ�
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
#include "mgr_center_agent.h"
#include "mgr_public_func.h"

#undef TASK_NAME
#define TASK_NAME	"MGR_CENTER_AGENT"

/**
 * @var s_i4LogMsgId
 * @brief �������׽���
 */
static INT32 s_i4LogMsgId;
 /**
 * @var pLogTxtFd
 * @brief �ļ����
 */
static FILE *pLogTxtFd = NULL;

#include "log.h"

SHM_IPC_STRU *ptIPCShm = NULL;

struct sockaddr_in sockaddr_cc_mgra;
struct sockaddr_in sockaddr_mgra_mgrh;
struct sockaddr_in sockaddr_mgra_adp;
int sock_ipc;
int sock_cc_mgra;
pthread_t tid_cc_mgra_recv;

/**
 * @var shmid_cfg
 * @brief �����������ڴ�IPC��ֵ
 */
int shmid_cfg;	 
/**
 * @var semid_cfg
 * @brief �����������ڴ��ź���IPC��ֵ
 */
int semid_cfg;	 
/**
 * @var shm_cfg_addr
 * @brief �����������ڴ��ַ
 */
SHM_CFG_STRU * shm_cfg_addr = NULL;
/**
 * @var shmid_ipc
 * @brief ���̼�ͨ�Ź����ڴ�IPC��ֵ
 */
int shmid_ipc;	 
/**
 * @var semid_ipc
 * @brief ���̼�ͨ�Ź����ڴ��ź���IPC��ֵ
 */
int semid_ipc;	 
/**
 * @var shm_ipc_addr
 * @brief ���̼�ͨ�Ź����ڴ��ַ
 */
SHM_IPC_STRU * shm_ipc_addr = NULL;
/**
 * @var shmid_nm
 * @brief �����������ڴ�IPC��ֵ
 */
int shmid_nm;	 
/**
 * @var semid_nm
 * @brief �����������ڴ��ź���IPC��ֵ
 */
int semid_nm;	 
/**
 * @var shm_nm_addr
 * @brief �����������ڴ��ַ
 */
SHM_NM_STRU * shm_nm_addr = NULL;
/**
 * @var local_cc
 * @brief ����ɫ��ֵ
 */
unsigned int local_cc = 0;
/**
 * @var local_dev_id
 * @brief �����豸ID
 */
unsigned int local_dev_id = 0;




int main(void)
{
    
	printf("<%s> <%s> <%d> mgr_center_agent start!\n", __FILE__, __FUNCTION__, __LINE__);

    
	key_t msqKey;
	//logд�ļ�
	pLogTxtFd = fopen("./LOG.txt","r+");
	if(NULL == pLogTxtFd)
	{
		printf("[ROUTE]fopen Log.txt fail!!!\n");
		return -1;
	}
	fseek(pLogTxtFd,0,SEEK_END);
	//log��־���
	if((msqKey = ftok("/", LOG_MSG_ID)) == -1)
	{
		LOG_WFile(pLogTxtFd,"[Route_Init]Creat msqKey Error\n");
		return -1;
	}
	s_i4LogMsgId = msgget(msqKey, IPC_EXCL);  /*�����Ϣ�����Ƿ����*/
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

	if (mgra_cc_pthread_create())
	{
		printf("<%s> <%s> <%d> mgra_cc_pthread_create err\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	while(1)
	{
		sleep(10);
	}
	
	return 0;
}


int init_ipc_socket(void)
{
	memset(&sockaddr_cc_mgra, 0, sizeof(struct sockaddr_in));
	sockaddr_cc_mgra.sin_family = AF_INET;
	sockaddr_cc_mgra.sin_port = htons(SOCK_PORT_CC_MGRA);
	sockaddr_cc_mgra.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&sockaddr_mgra_mgrh, 0, sizeof(struct sockaddr_in));
	sockaddr_mgra_mgrh.sin_family = AF_INET;
	sockaddr_mgra_mgrh.sin_port = htons(SOCK_PORT_MGRA_MGRH);
	sockaddr_mgra_mgrh.sin_addr.s_addr = inet_addr("127.0.0.1");

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

	sock_cc_mgra = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock_cc_mgra) 
	{
		printf("sock_cc_mgra socket error\n");
		return -1;
	}
    if (-1 == bind(sock_cc_mgra, (struct sockaddr *)(&sockaddr_cc_mgra), sizeof(struct sockaddr_in))) 
    {
        printf("sock_cc_mgra bind error\n");
        return -1;
    }   

	return 0;
}


int mgra_cc_pthread_create(void)
{
	if (pthread_create(&tid_cc_mgra_recv, NULL, pthread_cc_mgra_recv, NULL)) 
	{
		printf("pthread_create tid_cc_mgra_recv err\n");
		return -1;
	}
	pthread_detach(tid_cc_mgra_recv);
	
	return 0;
}


void * pthread_cc_mgra_recv(void *arg)
{
	int ret;
	int rd_cnt;
	fd_set fds;
	NM_IPC_MSG cmd_msg;
	NAS_INF_DL_T nm_ai_msg;
	NAS_AI_PAYLOAD nas_ai_payload;
	
	while (1)
	{
        FD_ZERO(&fds);
        FD_SET(sock_cc_mgra, &fds);
		struct timeval tv;    
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        ret = select(sock_cc_mgra + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) 
        {
            printf("<%s> <%s> <%d> select error!\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        } 
        else if (0 == ret) 
        {
            continue;       
        }

		if (FD_ISSET(sock_cc_mgra, &fds)) 
		{
			rd_cnt = recvfrom(sock_cc_mgra, &cmd_msg, sizeof(NM_IPC_MSG), 0, NULL, NULL);
			if (rd_cnt < 0)
			{
				continue;
			}
            if((cmd_msg.op_code==OP_CODE_GET_ACK)||(cmd_msg.op_code==OP_CODE_SET_ACK))
            {
                switch(cmd_msg.cmd_code)
                	{
                	case CMD_CODE_CENTER_MODULE:
						sem_nm_p();
				        memcpy(shm_nm_addr->center_module.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->center_module.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_GSM_MODULE1:
						sem_nm_p();
				        memcpy(shm_nm_addr->gsm_module1.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->gsm_module1.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_GSM_MODULE2:
						sem_nm_p();
				        memcpy(shm_nm_addr->gsm_module2.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->gsm_module2.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_RADIO_MODULE:
						sem_nm_p();
				        memcpy(shm_nm_addr->radio_module.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->radio_module.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_DSP_MODULE:
						sem_nm_p();
				        memcpy(shm_nm_addr->dsp_module.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->dsp_module.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_MICROPHONE_MODULE1:
						sem_nm_p();
				        memcpy(shm_nm_addr->microphone_module1.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->microphone_module1.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					case CMD_CODE_MICROPHONE_MODULE2:
						sem_nm_p();
				        memcpy(shm_nm_addr->microphone_module2.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->microphone_module2.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;

					case CMD_CODE_CONTROL_MODULE:
						sem_nm_p();
				        memcpy(shm_nm_addr->control_module.data,cmd_msg.payload, sizeof(cmd_msg.payload));
				        shm_nm_addr->control_module.ack_flag= ACK_FLAG_OK;
				        sem_nm_v();
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
							        cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				        }
						break;
					default :break;
                	}
					
            }
			else
		    {
				if (shm_ipc_addr->mgr_printf[1])
				{
					LOG_DEBUG(s_i4LogMsgId,\
							"\n |%s||%s||%d|\n"
							"\r src_id	 : 0x%X\n"
							"\r dst_id	 : 0x%X\n"
							"\r local_dev_id  : 0x%X\n",\
							__FILE__, __FUNCTION__, __LINE__,\
						   cmd_msg.src_id, cmd_msg.dst_id, local_dev_id);
				}
				printf("************************ID*************************\n");
			    if (cmd_msg.dst_id == local_dev_id)
			    {
                    cmd_msg.src_id=local_dev_id;
                    cmd_msg.dst_id=local_dev_id;
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
							   cmd_msg.nm_type, cmd_msg.src_id, cmd_msg.dst_id, cmd_msg.cmd_code, cmd_msg.op_code);
				    }
				    sendto(sock_ipc, &cmd_msg, sizeof(NM_IPC_MSG), 0, (struct sockaddr *)(&sockaddr_mgra_mgrh), sizeof(struct sockaddr_in)); 
			    }
			    else
			    {
			        if(cmd_msg.cmd_code==CMD_CODE_CENTER_QUERY_ALARM )
                    {
                        memset(&nas_ai_payload, 0, sizeof(NAS_AI_PAYLOAD));
	                    memset(&nm_ai_msg, 0, sizeof(NAS_INF_DL_T));

                        nas_ai_payload.cmd_code = CMO_CODE_NAS_PRE;
	                    nas_ai_payload.nm_type = NM_TYPE_CENTER;
	                    nas_ai_payload.op_code =cmd_msg.op_code;
	                    nas_ai_payload.src_id = local_dev_id;
	                    nas_ai_payload.dst_id = cmd_msg.dst_id;
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

	                    sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in));   ///�����Ĵ����ز�
                        sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in)); 
                        sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in)); 
                        sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in)); 

                    //////////������Ч����/////////////////////////////
                        memset(&nas_ai_payload, 0, sizeof(NAS_AI_PAYLOAD));
	                    memset(&nm_ai_msg, 0, sizeof(NAS_INF_DL_T));
                        
                        nas_ai_payload.cmd_code = cmd_msg.cmd_code;
				        nas_ai_payload.nm_type = NM_TYPE_CENTER;
				        nas_ai_payload.op_code = cmd_msg.op_code;
				        nas_ai_payload.src_id = local_dev_id;
				        nas_ai_payload.dst_id = cmd_msg.dst_id;
				        memcpy(nas_ai_payload.data, cmd_msg.payload, sizeof(nas_ai_payload.data));
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
				
				        sendto(sock_ipc, &nm_ai_msg, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&sockaddr_mgra_adp), sizeof(struct sockaddr_in)); 
                    }         
			    }   
			    
		      }
		   }
	}
	
	pthread_exit(NULL);
}


void init_local_val(void)
{
	sem_cfg_p();
	local_dev_id = shm_cfg_addr->dev_id.val;
	local_cc = shm_cfg_addr->cc.val;
	sem_cfg_v();
}


