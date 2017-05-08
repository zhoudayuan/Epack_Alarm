/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    sap.c
 * @brief  ҵ��ģ��ĳ�ʼ������Դ����
 * @author  ������
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   �����б�
 *   1. DLL_Init                          ����������·�㴦���߳�
 *   2. CCL_Init                          �������п��Ʋ㴦���߳�
 *
 *
 *   �޸���ʷ
 *   2016-7-20                      ������                     �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "sap.h"
#include "ccl.h"
#include "dll_fun.h"

/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/
/**
 * @var  s_i4LogMsgId
 * @brief �������׽���
 */
INT32 s_LogMsgId;

/**
 * @var  s_semid
 * @brief �ź���ID
 */
static int s_semid;

/**
 * @var  pLogTxtFd
 * @brief �ļ����
 */
FILE *pLogFd = NULL;

/**
 * @var ptIPCShm
 * @brief �����ڴ�ϵͳ��Ϣ
 */
SHM_IPC_STRU *ptIPCShm = NULL;

/**
 * @var ptCFGShm
 * @brief �����ڴ�ϵͳ��Ϣ
 */
SHM_CFG_STRU *ptCFGShm = NULL;




#include "log.h"

/******************************************************************************
 *   �ڲ�����ʵ��
 *   *************************************************************************/
/**
 * @brief   ������
 *
 * @return  int         0:�ɹ�;-1:ʧ��
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
int main(void)
{
    int ret;
    key_t msqKey;

    printf("\n");
    //logд�ļ�
    pLogFd = fopen("./LOG.txt", "r+");
    if(NULL == pLogFd)
    {
        printf("[SAP]fopen Log.txt fail!!!\n");
        exit(1);
    }
    fseek(pLogFd, 0, SEEK_END);

    //log��־���
    if((msqKey = ftok("/", LOG_MSG_ID)) == -1)
    {
        LOG_WFile(pLogFd, "[SAP]Creat msqKey Error");
        exit(1);
    }
    s_LogMsgId = msgget(msqKey, IPC_EXCL);  /*�����Ϣ�����Ƿ����*/
    if(s_LogMsgId < 0)
    {
        LOG_WFile(pLogFd,"[SAP]failed to get msq");
        exit(1);
    }
    printf("[SAP] log print ok!\n");

    _CFG_Shm();
    printf("[SAP] cfg shm ok!\n");

    _IPC_sem();
    printf("[SAP] ipc sem ok!\n");

    _IPC_Shm();
    printf("[SAP] ipc shm ok!\n");

    ret = _LocalCfgPrint();
    if(ret == -1)
    {
        exit(1);
    }
    printf("[SAP] local cfg print ok!\n");

    DLL_Init();
    printf("[SAP] dll init\n");

    CCL_init();
    printf("[SAP] ccl init\n");

    printf("[SAP] INIT......ok\n");
    LOG_DEBUG(s_LogMsgId,"[SAP] INIT......ok");

    while(1)
    {
        sleep(10);
    }

    return 0;
}


/**
 * @brief   ��ȡIPC�����ڴ��ַ
 *
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
void _IPC_Shm()
{
    int shmid;
    key_t shmkey;

    shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_IPC);
    if (shmkey < 0)
    {
        LOG_WFile(pLogFd,"Get IPC Shm key error!!!");
        LOG_ERROR(s_LogMsgId,"..._Get IPC Shm key error!!!, exit!!!");
        exit(1);
    }

    /* ���������ڴ�*/
    shmid = shmget(shmkey, 0, 0);   //���������ڴ�
    if (-1 == shmid)
    {
        shmid = shmget(shmkey, sizeof(SHM_IPC_STRU), 0666 | IPC_CREAT); //���������ڴ�
        if(shmid < 0)
        {
            LOG_WFile(pLogFd,"Get IPC Shm error!!!");
            LOG_ERROR(s_LogMsgId,"..._Get IPC Shm error!!!, exit!!!");
            exit(1);
        }
    }

    /* �������ڴ�ӳ�䵽��ǰ���̵ĵ�ַ�� */
    ptIPCShm = (SHM_IPC_STRU *)shmat(shmid, NULL, 0);
    if (0 > ptIPCShm)
    {
        LOG_WFile(pLogFd,"Get IPC Shm MAT error!!!");
        LOG_ERROR(s_LogMsgId,"..._Get IPC Shm MAT error!!!, exit!!!");
        exit(1);
    }

    return;
}


/**
 * @brief   ��ȡ���ù����ڴ��ַ
 *
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
void _CFG_Shm()
{
    int shmid;
    key_t shmkey;

    shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_CFG);
    if (shmkey < 0)
    {
        LOG_WFile(pLogFd,"Get CFG Shm key error!!!");
        printf("..._Get CFG Shm key error!!!, exit!!!");
        exit(1);
    }

    /* ���������ڴ�*/
    shmid = shmget(shmkey, 0, 0);   //���������ڴ�
    if (-1 == shmid)
    {
        LOG_WFile(pLogFd,"Get CFG Shm error!!!");
        printf("..._Get CFG Shm error!!!, exit!!!");
        exit(1);
    }

    /* �������ڴ�ӳ�䵽��ǰ���̵ĵ�ַ�� */
    ptCFGShm = (SHM_CFG_STRU *)shmat(shmid, NULL, 0);
    if (0 > ptCFGShm)
    {
        LOG_WFile(pLogFd,"Get CFG Shm MAT error!!!");
        printf("..._Get CFG Shm MAT error!!!, exit!!!");
        exit(1);
    }

    return;
}




/* changes the semaphore by -1 (waiting). */
int sem_ipc_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(s_semid, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}


int sem_ipc_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(s_semid, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}


static int set_semvalue(void)
{
    SEMUN sem_union;
    sem_union.val = 1;
    if (semctl(s_semid, 0, SETVAL, sem_union) == -1)
    {
        return(0);
    }
    return(1);
}


/**
 * @brief   ��ʼ���ź���
 *
 * @author  �ܴ�Ԫ
 * @since   trunk.00001
 * @bug
 */
void _IPC_sem()
{
    key_t semkey;

    semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_IPC);
    if (semkey < 0)
    {
        LOG_WFile(pLogFd, "Get IPC Sem key error!!!");
        LOG_ERROR(s_LogMsgId, "..._Get IPC Sem key error!!!, exit!!!");
        exit(1);
    }

    s_semid = semget(semkey, 1, (0666 | IPC_CREAT));
    if (s_semid == -1)
    {
        LOG_WFile(pLogFd, "Get IPC sem error!!!");
        LOG_ERROR(s_LogMsgId, "..._Get IPC sem error!!!, exit!!!");
        exit(1);
    }
    else if (s_semid > 1)
    {
        if (!set_semvalue())
        {
            LOG_WFile(pLogFd, "Failed to initialize semaphore");
            LOG_ERROR(s_LogMsgId, "..._Failed to initialize semaphore!!!, exit!!!");
            exit(1);
        }
    }
    else
    {
        perror("s_semid>");
        exit(1);
    }
}
/**
 * @brief   ��ȡ�������ô�ӡ
 *
 * @author  ������
 * @since   trunk.00001
 * @bug
 */
int _LocalCfgPrint()
{
    LOG_DEBUG(s_LogMsgId,"Version:          %s", ptCFGShm->s_version.buf);
    LOG_DEBUG(s_LogMsgId,"Device ID:        %d", ptCFGShm->dev_id.val);
    LOG_DEBUG(s_LogMsgId,"WLU CC:           %d", ptCFGShm->cc.val);
    LOG_DEBUG(s_LogMsgId,"Lock Time:        %d", ptCFGShm->locking_time.val);
    LOG_DEBUG(s_LogMsgId,"Frequency:        %d", ptCFGShm->freq.val);
    LOG_DEBUG(s_LogMsgId,"Power:            %d", ptCFGShm->power.val);
    LOG_DEBUG(s_LogMsgId,"Negr switch:      %d", ptCFGShm->start_neighbor.val);
    LOG_DEBUG(s_LogMsgId,"Negr cycle:       %d", ptCFGShm->neighbor_period.val);
    LOG_DEBUG(s_LogMsgId,"MS CC:            %d", ptCFGShm->terminal_cc.val);
    LOG_DEBUG(s_LogMsgId,"Work Mode:        %d", ptCFGShm->protocol_mode.val);
    LOG_DEBUG(s_LogMsgId,"Subnet ID:        %d", ptCFGShm->air_subnet_id.val);
    LOG_DEBUG(s_LogMsgId,"WLU IP:           %s", ptCFGShm->local_ip.buf);
    LOG_DEBUG(s_LogMsgId,"Stun flag:        %d", ptCFGShm->stun_flag.val);
    LOG_DEBUG(s_LogMsgId,"Kill flag:        %d", ptCFGShm->kill_flag.val);

    if(ptCFGShm->dev_id.val > 31)
    {
        printf("[SAP] Device ID error: %d\n", ptCFGShm->dev_id.val);
        LOG_ERROR(s_LogMsgId,"Device ID error: %d", ptCFGShm->dev_id.val);
        return -1;
    }
    else if(ptCFGShm->cc.val > 15)
    {
        printf("[SAP] WLU CC error: %d\n", ptCFGShm->cc.val);
        LOG_ERROR(s_LogMsgId,"WLU CC error: %d", ptCFGShm->cc.val);
        return -1;
    }
    else if(ptCFGShm->locking_time.val < 360)
    {
        printf("[SAP] Lock Time error: %d\n", ptCFGShm->locking_time.val);
        LOG_ERROR(s_LogMsgId,"Lock Time error: %d", ptCFGShm->locking_time.val);
        return -1;
    }
    else if(ptCFGShm->start_neighbor.val > 1)
    {
        printf("[SAP] Negr switch error: %d\n", ptCFGShm->start_neighbor.val);
        LOG_ERROR(s_LogMsgId,"Negr switch error: %d", ptCFGShm->start_neighbor.val);
        return -1;
    }
    else if(ptCFGShm->neighbor_period.val < 1)
    {
        printf("[SAP] Negr cycle error: %d\n", ptCFGShm->neighbor_period.val);
        LOG_ERROR(s_LogMsgId,"Negr cycle error: %d", ptCFGShm->neighbor_period.val);
        return -1;
    }
    else if(ptCFGShm->terminal_cc.val > 15)
    {
        printf("[SAP] MS CC error: %d\n", ptCFGShm->terminal_cc.val);
        LOG_ERROR(s_LogMsgId,"MS CC error: %d", ptCFGShm->terminal_cc.val);
        return -1;
    }
    else if(ptCFGShm->protocol_mode.val > 1)
    {
        printf("[SAP] Work Mode error: %d\n", ptCFGShm->protocol_mode.val);
        LOG_ERROR(s_LogMsgId,"Work Mode error: %d", ptCFGShm->protocol_mode.val);
        return -1;
    }
    else if(ptCFGShm->air_subnet_id.val > 255)
    {
        printf("[SAP] Subnet ID error: %d\n", ptCFGShm->air_subnet_id.val);
        LOG_ERROR(s_LogMsgId,"Subnet ID error: %d", ptCFGShm->air_subnet_id.val);
        return -1;
    }

    return 0;
}


