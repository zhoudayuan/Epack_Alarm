/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file    log.c
 * @brief   
 * @author ������
 * @version v1.0
 * @date    2016-8-30
 */
/*
 *   �����б�
 *   1. 
 *   2. 
 *   3. 
 *   4. 
 *   5. 
 *   6. 
 *   7. 
 *
 *   �޸���ʷ
 *   2016-08-30            �����ļ�                             ������
 *   2017-05-05            �޸������������־ģ��               ������
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>    //inet_addr(); inet_ntoa()
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <errno.h>
#include <arpa/inet.h>
#include "typedef.h"
#include "mgr_common.h"

#include "log.h"

#undef TASK_NAME
#define TASK_NAME	    "LOG"

#define LOG_SEND_PORT   7010


/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/
  /**
 * @var s_i4LogMsgId
 * @brief �������׽���
 */
INT32 s_i4LogMsgId;
 /**
 * @var pLogTxtFd
 * @brief �ļ����
 */
static FILE *pLogTxtFd = NULL;
 /**
 * @var s_i4LogFd
 * @brief 
 */
static INT32 s_i4LogFd = -1;
 /**
 * @var s_tCliInputTask
 * @brief 
 */
//static pthread_t s_tLogTask;
  /**
 * @var s_tLogSendTask
 * @brief 
 */
static pthread_t s_tLogSendTask;
  /**
 * @var s_hsocket
 * @brief �������׽���
 */
//static INT32 s_i4LogSocket;
 /**
 * @var tLogLocalAddr
 * @brief �������׽���
 */
static struct sockaddr_in tLogLocalAddr;
 /**
 * @var tLogMsgRev
 * @brief �������׽���
 */
LOG_MSG_T tLogMsgRev;
/**
 * @var tLogMsgRev
 * @brief �������׽���
 */
 UINT32 u4LogMsgRecvLen;

/**
 * @brief   ����socket����
 *
 * @param [in]  void      
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
/*void LogSetSocketOpt()
{
	int opval = 1;
	int keepAlive = 1; // ����keepalive����
	int keepIdle = 2; // ���������2����û���κ���������,�����̽�� 
	int keepInterval = 1; // ̽��ʱ������ʱ����Ϊ1 ��
	int keepCount = 1; // ̽�Ⳣ�ԵĴ���.�����1��̽������յ���Ӧ��,���2�εĲ��ٷ�.
	int ret;
	
	setsockopt(s_i4LogSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opval, sizeof(int));
	setsockopt(s_i4LogSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&opval, sizeof(int));
	
	ret = setsockopt(s_i4LogSocket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
	if(-1 == ret)
		printf("SO_KEEPALIVE fail\n");
	ret = setsockopt(s_i4LogSocket, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
	if(-1 == ret)
		printf("TCP_KEEPIDLE fail\n");
	
	ret = setsockopt(s_i4LogSocket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
	if(-1 == ret)
		printf("TCP_KEEPINTVL fail\n");
	ret = setsockopt(s_i4LogSocket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
	if(-1 == ret)
		printf("TCP_KEEPCNT fail\n");

	return;

}*/

/**
 * @brief   ��־crc����У��
 *
 * @param [in]  buf         ���ݸ���
 * @param [in]  length      ���ݳ���
 *
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
unsigned short LogCrcSum(char *buf, unsigned short length)
{
    unsigned short crc = 0;
    for(int i = 0; i < length; i++)
    {
        crc+=buf[i];
    }
    return (crc);
}

/**
 * @brief   
 *
 * @param [in]  u4Arc      �̴߳��ݲ�������
 * @param [in]  pvArg      �̴߳��ݲ�������
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
/*static void *LogTask(void* pvArg)
{

	s_i4LogSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( 0 > s_i4LogSocket)
	{
		printf("Failed to create LOG TCP socket");
		exit(1);
	}
	printf("[LogTask]Creat Socket OK!!!\n");
	//fcntl( s_i4LogSocket, F_SETFL, O_NONBLOCK);
	LogSetSocketOpt();

	if(0 != bind(s_i4LogSocket,(struct sockaddr *)&tLogLocalAddr, sizeof(tLogLocalAddr)))
	{
		printf("LogTask Bind local addr and port error\n");
		close(s_i4LogSocket);
		exit(1);
	}
ListenTcp:
	if(0 != listen(s_i4LogSocket,1))
		{
			printf("LogTask listen error\n");
			close(s_i4LogSocket);
			exit(1);
		}
	s_i4LogFd = accept(s_i4LogSocket,NULL,NULL);
	printf("accept ok\n");

	while(1)
	{		
		if(s_i4LogFd < 0)
		{
			printf("Accept error,Reconnect TCP!!!\n");
			close(s_i4LogFd);
			sleep(1);
			goto ListenTcp;
		}	
		else
		{	
			usleep(30000);
		}
	}
	close(s_i4LogSocket);
	close(s_i4LogFd);
	s_i4LogFd = -1;
	pthread_exit(NULL); //�˳��߳�	
}*/

/**
 * @brief   
 *
 * @param [in]  u4Arc      �̴߳��ݲ�������
 * @param [in]  pvArg      �̴߳��ݲ�������
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
/*static void *LogSendTask(void* pvArg)
{
	int i4Ret ;
    
	while(1)
	{
		u4LogMsgRecvLen = msgrcv(s_i4LogMsgId, &tLogMsgRev, sizeof(tLogMsgRev), 0, 0);

		if(u4LogMsgRecvLen  > 0)
		{
	        i4Ret = send(s_i4LogFd, tLogMsgRev.tmsgArray, (u4LogMsgRecvLen-4),0);
			if (i4Ret <= 0)//�ͻ����˳�
			{
				printf("send error \n");
				s_i4LogFd = -1;
			}	
		}
	}
	pthread_exit(NULL); //�˳��߳�	
}*/

/**
 * @brief   
 *
 * @param [in]  log_level      ��־�ȼ�
 * @param [in]  Length         ���ݳ���
 * @param [in]  Payload        ���ݸ����׵�ַ
 * @param [in]  log_print      ��ӡ�ṹ�׵�ַ
 *
 * @return  short         ���ݳ���
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
unsigned short BuildLogStruFun(char log_level, short Length, char *Payload, char *log_print)
{
    unsigned short tmp;
    unsigned short log_len = sizeof(LOG_PRINT_HEAD_T) + 1 + Length;
    WLU_LOG_MSG_T log_tmp;
    memset(&log_tmp, 0, sizeof(log_tmp));
    log_tmp.Head = 0x01ea;
    log_tmp.Type = 0x12;
    log_tmp.DataLen = log_len;
    log_tmp.Data.log_head.head = 0xec13;
    log_tmp.Data.log_head.type = 0x0200;
    log_tmp.Data.log_head.datalen = (1 + Length);
    log_tmp.Data.log_head.src_id = 0;
    log_tmp.Data.log_head.dst_id = 0;
    log_tmp.Data.log_head.src_type = 0xff;
    log_tmp.Data.log_head.sig_ver = 0x02;
    log_tmp.Data.log_head.path_num = 0xff;
    log_tmp.Data.log_head.check_val = 0xffffffff;
    log_tmp.Data.log_level = log_level;
    memcpy(log_tmp.Data.log_data, Payload, Length);

    log_len = log_len+5;
    memcpy(log_print, (char *)&log_tmp, log_len);
    
    tmp = LogCrcSum((log_print+2), log_len);
    *(log_print+log_len+2) = (char)(tmp & 0x00ff);
    *(log_print+log_len+3) = (char)(tmp >> 8 & 0x00ff);
    *(log_print+log_len+4) = 0xed;
    *(log_print+log_len+5) = 0x02;

    log_len = log_len+6;

    return log_len;
}

/**
 * @brief   ��־��������߳�
 *
 * @param [in] p       ���ݸ��߳�start_routine�Ĳ���
 * @author  ������
 * @since   trunk.00001
 * @bug
*/
static void *LOG_NetSendTask(void * p)
{
    char LogPrint[1200];
    unsigned short LogLen;
    socklen_t size_addr = sizeof(struct sockaddr_in);

    int Ret;

    while(1)
    {
		u4LogMsgRecvLen = msgrcv(s_i4LogMsgId, &tLogMsgRev, sizeof(tLogMsgRev), 0, 0);

		if(u4LogMsgRecvLen > 0)
		{
            memset(LogPrint, 0, sizeof(LogPrint));            
            LogLen = BuildLogStruFun(1, (u4LogMsgRecvLen-4), tLogMsgRev.tmsgArray, LogPrint);
            
            Ret = sendto(s_i4LogFd, LogPrint, LogLen, 0, (struct sockaddr *)(&tLogLocalAddr), size_addr);
			if (Ret <= 0)
			{
				printf("[LOG] sendto error\n");
				continue;
			}
		}
        else
        {
            printf("msgrcv data error\n");
        }
    }
	pthread_exit(NULL); //�˳��߳�	

}

int main()
{
	key_t msqKey;
	
	if ((msqKey = ftok("/", LOG_MSG_ID)) == -1)
	{
		printf("[Log_Init] Creat msqKey Error\n");
		exit(1);
	}

	s_i4LogMsgId = msgget(msqKey, IPC_EXCL);  /*�����Ϣ�����Ƿ����*/
	if (0 > s_i4LogMsgId)
	{
		printf("[LOG] Remove Msg!!!\n");
		msgctl(s_i4LogMsgId, IPC_RMID, NULL);//ɾ��
	}
	
	s_i4LogMsgId = msgget(msqKey, IPC_CREAT|0666);/*������Ϣ����*/
	if (0 > s_i4LogMsgId)
	{
		printf("[Log_Init] Failed to create Msq\n");
	    exit(1);
	}

    s_i4LogFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > s_i4LogFd)
    {
        printf("[Log_Init] Failed to create LogSocket!\n");
        exit(1);
    }

    bzero(&tLogLocalAddr, sizeof(struct sockaddr_in));
    tLogLocalAddr.sin_family = AF_INET;
    tLogLocalAddr.sin_port = htons(LOG_SEND_PORT);
    tLogLocalAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//tLogLocalAddr.sin_family   = AF_INET;
	//tLogLocalAddr.sin_port     = htons(SOCK_PORT_LOG_PRINT);
	//tLogLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	pLogTxtFd = fopen("./LOG.txt","r+");
	if(NULL == pLogTxtFd)
	{
		creat("./LOG.txt", S_IRUSR | S_IWUSR);
		pLogTxtFd = fopen("./LOG.txt","r+");
		if(NULL == pLogTxtFd)
		{
			printf("[LOG] fopen Log.txt fail!!!\n");
			exit(1);
		}	
	}
	fseek(pLogTxtFd,0,SEEK_END);
    
    /*
	if (pthread_create(&s_tLogTask, NULL, LogTask, NULL) != 0)
	{
		printf("[CLI][s_tLogTask error \n");
		exit(1);
	}
	pthread_detach(s_tLogTask);
    */
    
	if (pthread_create(&s_tLogSendTask, NULL, LOG_NetSendTask, NULL) != 0)
	{
		printf("[LOG] s_tLogSendTask error \n");
		exit(1);
	}
	pthread_detach(s_tLogSendTask);
    
	while(1)
	{
		sleep(10);
	}
	return 0;
}
