/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_public_func.c
 * @brief   公共函数
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   函数列表
 *   1. shm_cfg_create       创建配置区共享内存
 *   2. shm_ipc_create       创建通信区共享内存
 *   3. shm_nm_create       创建网管区共享内存
 *   4. shm_cfg_attch       绑定配置区共享内存
 *   5. shm_ipc_attch       绑定通信区共享内存
 *   6. shm_nm_attch       绑定网管区共享内存
 *   7. shm_cfg_detach       删除配置区共享内存
 *   8. shm_ipc_detach       删除通信区共享内存
 *   9. shm_nm_detach       删除网管区共享内存
 *   10. sem_cfg_p       配置区共享内存信号量P操作
 *   11. sem_cfg_v       配置区共享内存信号量V操作
 *   12. sem_ipc_p       通信区共享内存信号量P操作
 *   13. sem_ipc_v       通信区共享内存信号量V操作
 *   14. sem_nm_p      网管区共享内存信号量P操作
 *   15. sem_nm_v       网管区共享内存信号量V操作
 *   16. write_ini_file       创建配置文件
 *   17. save_ini_file       写配置文件
 *   18. crc8       crc校验
 *
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>        //open
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/sem.h> 
#include <sys/ipc.h>      //shmget
#include <sys/shm.h>      //shmget
#include <arpa/inet.h>    //inet_addr(); inet_ntoa()
#include "mgr_public_func.h"
//#include "log.h"


extern int semid_cfg;	 
extern int shmid_cfg;	
extern int semid_ipc;	 
extern int shmid_ipc;	 
extern int semid_nm;	 
extern int shmid_nm;	 
extern SHM_CFG_STRU * shm_cfg_addr;
extern SHM_IPC_STRU * shm_ipc_addr;
extern SHM_NM_STRU * shm_nm_addr;
extern SHM_IPC_STRU * ptIPCShm;


int shm_cfg_create(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_CFG);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_CFG);  

	shmid_cfg = shmget(shmkey, sizeof(SHM_CFG_STRU), 0666 | IPC_CREAT); //创建共享内存 
	if (-1 == shmid_cfg)  
	{
		printf("creat shmid_cfg fail\n");
		return -1;
	}

	shm_cfg_addr = (SHM_CFG_STRU *)shmat(shmid_cfg, NULL, 0);
	if (-1 == (int)(shm_cfg_addr))
	{
		printf("shmat shm_cfg_addr fail\n");
		return -1;
	}

	semid_cfg = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_cfg) 
	{
		printf("semget semid_cfg fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_cfg, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_cfg fail\n");
		return -1;
	}

	return 0;
}


int shm_ipc_create(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_IPC);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_IPC);  

	shmid_ipc = shmget(shmkey, sizeof(SHM_IPC_STRU), 0666 | IPC_CREAT); //创建共享内存 
	if (-1 == shmid_ipc)  
	{
		printf("creat shmid_ipc fail\n");
		return -1;
	}

	shm_ipc_addr = (SHM_IPC_STRU *)shmat(shmid_ipc, NULL, 0);
	if (-1 == (int)(shm_ipc_addr))
	{
		printf("shmat shm_ipc_addr fail\n");
		return -1;
	}

	semid_ipc = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_ipc) 
	{
		printf("semget semid_ipc fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_ipc, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_ipc fail\n");
		return -1;
	}

	return 0;
}


int shm_nm_create(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_NM);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_NM);  

	shmid_nm = shmget(shmkey, sizeof(SHM_NM_STRU), 0666 | IPC_CREAT); //创建共享内存 
	if (-1 == shmid_nm)  
	{
		printf("creat shmid_nm fail\n");
		return -1;
	}

	shm_nm_addr = (SHM_NM_STRU *)shmat(shmid_nm, NULL, 0);
	if (-1 == (int)(shm_nm_addr))
	{
		printf("shmat shm_nm_addr fail\n");
		return -1;
	}

	semid_nm = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_nm) 
	{
		printf("semget semid_nm fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_nm, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_nm fail\n");
		return -1;
	}

	return 0;
}


int shm_cfg_attch(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_CFG);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_CFG);  

	shmid_cfg = shmget(shmkey, 0, 0); //获取共享内存 
	if (-1 == shmid_cfg)  
	{
		printf("attch shmid_cfg fail\n");
		return -1;
	}

	shm_cfg_addr = (SHM_CFG_STRU *)shmat(shmid_cfg, NULL, 0);
	if (-1 == (int)(shm_cfg_addr))
	{
		printf("shmat shm_cfg_addr fail\n");
		return -1;
	}

	semid_cfg = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_cfg) 
	{
		printf("semget semid_cfg fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_cfg, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_cfg fail\n");
		return -1;
	}

	return 0;
}


int shm_ipc_attch(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_IPC);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_IPC);  

	shmid_ipc = shmget(shmkey, 0, 0); //获取共享内存 
	if (-1 == shmid_ipc)  
	{
		printf("attch shmid_ipc fail\n");
		return -1;
	}

	shm_ipc_addr = (SHM_IPC_STRU *)shmat(shmid_ipc, NULL, 0);
	if (-1 == (int)(shm_ipc_addr))
	{
		printf("shmat shm_ipc_addr fail\n");
		return -1;
	}

	semid_ipc = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_ipc) 
	{
		printf("semget semid_ipc fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_ipc, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_ipc fail\n");
		return -1;
	}

	ptIPCShm = shm_ipc_addr;

	return 0;
}


int shm_nm_attch(void)
{
	SEMUN sem_arg;
	key_t semkey;   
	key_t shmkey;  

	semkey = ftok(FTOK_F_NAME, FTOK_ID_SEM_NM);   
	shmkey = ftok(FTOK_F_NAME, FTOK_ID_SHM_NM);  

	shmid_nm = shmget(shmkey, 0, 0); //获取共享内存 
	if (-1 == shmid_nm)  
	{
		printf("attch shmid_nm fail\n");
		return -1;
	}

	shm_nm_addr = (SHM_NM_STRU *)shmat(shmid_nm, NULL, 0);
	if (-1 == (int)(shm_nm_addr))
	{
		printf("shmat shm_nm_addr fail\n");
		return -1;
	}

	semid_nm = semget(semkey, 1, 0666 | IPC_CREAT);   
	if (-1 == semid_nm) 
	{
		printf("semget semid_nm fail\n");
		return -1;
	}

    sem_arg.val = 1;   
    if (-1 == semctl(semid_nm, 0, SETVAL, sem_arg)) 
    {
		printf("semctl semid_nm fail\n");
		return -1;
	}

	return 0;
}


void shm_cfg_detach(void)
{
	if (-1 == shmdt(shm_cfg_addr))
	{
		printf("shm_cfg_detach err\n");
	}
}


void shm_ipc_detach(void)
{
	if (-1 == shmdt(shm_ipc_addr))
	{
		printf("shm_ipc_detach err\n");
	}
}


void shm_nm_detach(void)
{
	if (-1 == shmdt(shm_nm_addr))
	{
		printf("shm_nm_detach err\n");
	}
}


void sem_cfg_p(void)
{
	struct sembuf s_p;
	s_p.sem_num = 0;
	s_p.sem_op = -1;
	s_p.sem_flg = SEM_UNDO;
	if (-1 == semop(semid_cfg, &s_p, 1))
	{
		printf("Sem_cfg_p operation fail\n");
	}
}

 
void sem_cfg_v(void)
{
	struct sembuf s_v;
	s_v.sem_num = 0;
	s_v.sem_op = 1;
	s_v.sem_flg = SEM_UNDO;
	if(-1 == semop(semid_cfg, &s_v, 1))
	{
		printf("Sem_cfg_v operation fail\n");
	}
}


void sem_ipc_p(void)
{
	struct sembuf s_p;
	s_p.sem_num = 0;
	s_p.sem_op = -1;
	s_p.sem_flg = SEM_UNDO;
	if (-1 == semop(semid_ipc, &s_p, 1))
	{
		printf("sem_ipc_p operation fail\n");
	}
}

 
void sem_ipc_v(void)
{
	struct sembuf s_v;
	s_v.sem_num = 0;
	s_v.sem_op = 1;
	s_v.sem_flg = SEM_UNDO;
	if(-1 == semop(semid_ipc, &s_v, 1))
	{
		printf("sem_ipc_v operation fail\n");
	}
}


void sem_nm_p(void)
{
	struct sembuf s_p;
	s_p.sem_num = 0;
	s_p.sem_op = -1;
	s_p.sem_flg = SEM_UNDO;
	if (-1 == semop(semid_nm, &s_p, 1))
	{
		printf("Sem_nm_p operation fail\n");
	}
}

 
void sem_nm_v(void)
{
	struct sembuf s_v;
	s_v.sem_num = 0;
	s_v.sem_op = 1;
	s_v.sem_flg = SEM_UNDO;
	if(-1 == semop(semid_nm, &s_v, 1))
	{
		printf("Sem_nm_v operation fail\n");
	}
}


void write_ini_file(void)
{
	int fd;	

    fd = open(NAS_CONFIG_FILE, O_RDWR | O_CREAT);
	if (-1 == fd)
	{
		printf("open wireless_config.ini fail\n");
		return;
	}

	lseek(fd, 0, SEEK_SET);
	sem_cfg_p();
	write(fd, shm_cfg_addr, sizeof(SHM_CFG_STRU));
	sem_cfg_v();
	close(fd);
	printf("write wireless_config.ini ok\n");
}


void save_ini_file(void)
{
	int fd;	

    fd = open(NAS_CONFIG_FILE, O_RDWR);
	if (-1 == fd)
	{
		return;
	}

	lseek(fd, 0, SEEK_SET);
	sem_cfg_p();
	write(fd, shm_cfg_addr, sizeof(SHM_CFG_STRU));
	sem_cfg_v();
	close(fd);
}


unsigned char crc8(unsigned char *ptr, unsigned char len)
{
    unsigned char crc;
    unsigned char i;
    crc = 0;
    while(len--)
    {
       crc ^= *ptr++;
       for(i = 0;i < 8;i++)
       {
           if(crc & 0x80)
           {
               crc = (crc << 1) ^ 0x07;
           }
           else 
           {
			   crc <<= 1;
		   }
       }
    }
    return crc;
}

