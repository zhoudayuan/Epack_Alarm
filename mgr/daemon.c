/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    daemon.c
 * @brief   守护进程
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   函数列表
 *   1. main       主函数
 *   2. start_child_task       开启子进程
 *   3. monitor_child_task       守护子进程
 *   4. fill_shm_cfg_default       填充配置区共享内存默认配置参数
 *   5. fill_shm_ipc_default       填充进程间通信区共享内存默认配置参数
 *   6. fill_shm_nm_default       填充网管区共享内存默认配置参数
 *   7. cover_cfg_info       根据配置文件填充前次配置信息
 *
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#include <stdio.h> 
#include <stdlib.h> 	  //system
#include <fcntl.h>		  //open
#include <string.h>
#include <stdarg.h> 	  //让函数能够接收不定量参数
#include <unistd.h> 	  //sleep
#include <sys/stat.h>
#include <sys/wait.h>  
#include <sys/types.h>
#include <sys/sem.h> 
#include <sys/ipc.h>	  //shmget
#include <sys/shm.h>	  //shmget
#include <sys/mman.h>
#include <arpa/inet.h>	  //inet_addr(); inet_ntoa()
#include <pthread.h>
#include "daemon.h"
#include "mgr_public_func.h"


/* 全局变量 */
int semid_cfg;	 
int shmid_cfg;	 
int semid_ipc;	 
int shmid_ipc;	 
int semid_nm;	 
int shmid_nm;	 
SHM_CFG_STRU * shm_cfg_addr = NULL;
SHM_IPC_STRU * shm_ipc_addr = NULL;
SHM_NM_STRU * shm_nm_addr = NULL;
SHM_IPC_STRU * ptIPCShm = NULL;

char src_buf[4096];
/* bit0-配置项变化 bit1-版本变化 bit2-打开配置文件失败 bit3-配置文件不存在 */
unsigned char reload_cfg_flag = 0;

int fd_run_led;
int fd_mcu_pwr_ctrl;
int fd_pwr_key_det;
int fd_wd_en;
int fd_wd_wg;
	
pthread_t tid_run_led;
pthread_t tid_pwr_key_det;
pthread_t tid_wd_wg;


int main()
{	
	CHILD_TASK sub[TASK_NUM] = {
		{0, 0, (char *)"logmain"},
		{0, 0, (char *)"cli"},
		{0, 0, (char *)"mgr_center_agent"},
		{0, 0, (char *)"mgr_ack_receive"},
		{0, 0, (char *)"mgr_local_handle"},
		{0, 0, (char *)"route"},
        {0, 0, (char *)"sap"}
	};

	system("echo 0 > /proc/sys/kernel/printk");

	load_fpga();

	if (-1 == shm_nm_create())
	{
		printf("shm_nm_create err\n");
		return -1;
	}
	
	if (-1 == shm_cfg_create())
	{
		printf("shm_cfg_create err\n");
		return -1;
	}

	if (-1 == shm_ipc_create())
	{
		printf("shm_ipc_create err\n");
		return -1;
	}
	
	fill_shm_cfg_default();
	
	fill_shm_ipc_default();
	
	fill_shm_nm_default();
	
	if (0 == access(NAS_CONFIG_FILE, F_OK))
	{
		cover_cfg_info();
	}
	else
	{
		reload_cfg_flag |= 0x08;
	}
		
	if (0 != reload_cfg_flag)
	{
		printf("wireless_config.ini changed\n");
		if (0 != (reload_cfg_flag & 0x08))
		{
			printf("no wireless_config.ini\n");
		}
		else
		{
			if (0 != remove(NAS_CONFIG_FILE))
			{
				printf("remove wireless_config.ini fail\n");
			}
		}
		write_ini_file();
	}
	else
	{
		printf("wireless_config.ini unchanged\n");
	}

	printf("reload flag: 0x%x\n", reload_cfg_flag);
	
	sleep(2);

	if(daemon(1,1) != 0)
	{
		return -1;
	}
	
	if (-1 == start_child_task(sub))
	{
		printf("start_child_task err\n");
		return -1;
	}

	sleep(1);
	
	if (daemon_pthread_create())
	{
		printf("mgrh_pthread_create err\n");
		return -1;
	}

	sleep(1);
	if (-1 == monitor_child_task(sub))
	{
		printf("monitor_child_task err\n");
		return -1;
	}
    

	return 0;
}


int start_child_task(CHILD_TASK * sub)
{
	pid_t pid;
	int index;

	for (index = 0; index < TASK_NUM; index++)
	{
		switch(pid = vfork())
		{
			case -1:
			{
				printf("vfork err\n");
				return -1;
			}
			case 0:
			{
				printf("start %d %s process\n", getpid(), sub[index].task_name);
				if (execl(sub[index].task_name, sub[index].task_name, NULL) < 0)
				{
					printf("execl err\n");
					return -1;
				}
				break;
			}
			default:
			{
				sub[index].pid = pid;
				
				break;	
			}
		}
	}

	switch(pid = vfork())
	{
		case -1:
		{
			printf("vfork err\n");
			return -1;
		}
		case 0:
		{
			printf("start snmpd process\n");
			if (execl("snmpd", "snmpd", "-L", "-c", "snmpd.conf", NULL) < 0)
			{
				printf("execl snmpd err\n");
				return -1;
			}
			break;
		}
		default:
		{
			break;	
		}
	}

	switch(pid = vfork())
	{
		case -1:
		{
			printf("fork err\n");
			return -1;
		}
		case 0:
		{
			printf("start ProcessModule\n");
			if (execl("ProcessModule", "ProcessModule", NULL) < 0)
			{
				printf("execl ProcessModule err\n");
				return -1;
			}
			break;
		}
		default:
		{
			break;	
		}
	}

	return 0;
}


int monitor_child_task(CHILD_TASK * sub)
{
	int fd;
	char tmp_buf[10];
	int snmpd_cnt = 0;
	pid_t pid;
	int index;

    while(1)
    {
        pid = waitpid(-1, NULL, WNOHANG);
        
		if ((pid != 0)&&(pid != -1))
		{
			for (index = 0; index < TASK_NUM; index++)
			{
				if (pid == sub[index].pid)
				{
					if(sub[index].count == 10)
					{	
						printf("restart 10 times process is %s\n", sub[index].task_name);
						version_back_operation();
					}
					else
					{
						sub[index].count++;
						printf("restart %s process\n", sub[index].task_name);
						switch(pid = vfork())
						{
							case -1:
								{
									printf("vfork err\n");
									return -1;
								}
							case 0:
								{
									sub[index].pid = getpid();
									if (execl(sub[index].task_name, sub[index].task_name, NULL) < 0)
									{
										printf("execl err\n");
										return -1;
									}
									break;
								}
							default:
								{
									sub[index].pid = pid;
									break;	
								}
						}
					}
					break;
				}
			}
		}

        //snmpd守护
		system("ps -ef | grep snmpd | grep -v grep |awk '{print $2}' > /tmp/snmpd_daemon.txt");
	    system("chmod 600 /tmp/snmpd_daemon.txt");	
		fd = open("/tmp/snmpd_daemon.txt", O_RDWR);
		if (fd == -1)
		{
			printf("open /tmp/snmpd_daemon.txt fail\n");
			return -1;
		}
		lseek(fd, 0, SEEK_SET);
		memset(tmp_buf, 0, sizeof(tmp_buf));
		read(fd, tmp_buf, sizeof(tmp_buf));
		close(fd);
		if (snmpd_cnt == 10)
		{
			printf("restart 10 times process is snmpd\n");
			version_back_operation();
		}
		else
		{
			if (0 == strncmp("", tmp_buf, 1))
			{
				snmpd_cnt++;
				printf("restart snmpd process\n");
				switch(pid = vfork())
				{
					case -1:
					{
						printf("vfork err\n");
						return -1;
					}
					case 0:
					{
						printf("start snmpd process\n");
						if (execl("snmpd", "snmpd", "-L", "-c", "snmpd.conf", NULL) < 0)
						{
							printf("execl snmpd err\n");
							return -1;
						}
						break;
					}
					default:
					{
						break;	
					}
				}
			}
		}		
		sleep(3);
	}

	return 0;
}


void fill_shm_cfg_default(void)
{
	sem_cfg_p();
	memset(shm_cfg_addr, 0, sizeof(SHM_CFG_STRU));
	
	shm_cfg_addr->dev_id.id = 0x01;
	memcpy(shm_cfg_addr->dev_id.name, "DEV_ID", 20);
	shm_cfg_addr->dev_id.len = 4;
	shm_cfg_addr->dev_id.val = DEFAULT_DEV_ID;

	shm_cfg_addr->cc.id = 0x02;
	memcpy(shm_cfg_addr->cc.name, "CC", 20);
	shm_cfg_addr->cc.len = 4;
	shm_cfg_addr->cc.val = DEFAULT_CC;

	shm_cfg_addr->locking_time.id = 0x03;
	memcpy(shm_cfg_addr->locking_time.name, "LOCKING_TIME", 20);
	shm_cfg_addr->locking_time.len = 4;
	shm_cfg_addr->locking_time.val = DEFAULT_LOCKING_TIME;

	shm_cfg_addr->freq.id = 0x04;
	memcpy(shm_cfg_addr->freq.name, "FREQ", 20);
	shm_cfg_addr->freq.len = 4;
	shm_cfg_addr->freq.val = DEFAULT_FREQ;

	shm_cfg_addr->power.id = 0x05;
	memcpy(shm_cfg_addr->power.name, "POWER", 20);
	shm_cfg_addr->power.len = 4;
	shm_cfg_addr->power.val = DEFAULT_POWER;

	shm_cfg_addr->open_close_loop.id = 0x06;
	memcpy(shm_cfg_addr->open_close_loop.name, "OPEN_LOOP", 20);
	shm_cfg_addr->open_close_loop.len = 4;
	shm_cfg_addr->open_close_loop.val = DEFAULT_OPEN_CLOSE_LOOP;

	shm_cfg_addr->start_neighbor.id = 0x07;
	memcpy(shm_cfg_addr->start_neighbor.name, "START_NEIGHBOR", 20);
	shm_cfg_addr->start_neighbor.len = 4;
	shm_cfg_addr->start_neighbor.val = DEFAULT_START_NEIGHBOR;

	shm_cfg_addr->half_variance_threshold.id = 0x08;
	memcpy(shm_cfg_addr->half_variance_threshold.name, "VARIANCE_THRES", 20);
	shm_cfg_addr->half_variance_threshold.len = 4;
	shm_cfg_addr->half_variance_threshold.val = DEFAULT_HALF_VARIANCE;

	shm_cfg_addr->neighbor_period.id = 0x09;
	memcpy(shm_cfg_addr->neighbor_period.name, "NEIGHBOR_PERIOD", 20);
	shm_cfg_addr->neighbor_period.len = 4;
	shm_cfg_addr->neighbor_period.val = DEFAULT_NEIGHBOR_PERIOD;

	
	shm_cfg_addr->terminal_cc.id = 0x0A;
	memcpy(shm_cfg_addr->terminal_cc.name, "TERMINAL_CC", 20);
	shm_cfg_addr->terminal_cc.len = 4;
	shm_cfg_addr->terminal_cc.val = DEFAULT_TERMINAL_CC;

	shm_cfg_addr->protocol_mode.id = 0x0B;
	memcpy(shm_cfg_addr->protocol_mode.name, "PROTOCOL_MODE", 20);
	shm_cfg_addr->protocol_mode.len = 4;
	shm_cfg_addr->protocol_mode.val = DEFAULT_PROTOCOL_MODE;

	shm_cfg_addr->s_version.id = 0x0C;
	memcpy(shm_cfg_addr->s_version.name, "S_VERSION", 20);
	shm_cfg_addr->s_version.len = 50;
	memcpy(shm_cfg_addr->s_version.buf, SOFT_VERSION, sizeof(shm_cfg_addr->s_version.buf));

	shm_cfg_addr->air_subnet_id.id= 0x0D;
	memcpy(shm_cfg_addr->air_subnet_id.name, "SUBNET_ID", 20);
	shm_cfg_addr->air_subnet_id.len = 4;
	shm_cfg_addr->air_subnet_id.val=DEFAULT_SUBNET_ID;

	shm_cfg_addr->stun_flag.id= 0x0E;
	memcpy(shm_cfg_addr->stun_flag.name, "STUN_FLAG", 20);
	shm_cfg_addr->stun_flag.len = 4;
	shm_cfg_addr->stun_flag.val=DEFAULT_STUN_FLAG;

	shm_cfg_addr->kill_flag.id= 0x0F;
	memcpy(shm_cfg_addr->kill_flag.name, "KILL_FLAG", 20);
	shm_cfg_addr->kill_flag.len = 4;
	shm_cfg_addr->kill_flag.val=DEFAULT_KILL_FLAG;

	shm_cfg_addr->local_ip.id= 0x10;
	memcpy(shm_cfg_addr->local_ip.name, "LOCAL_IP", 20);
	shm_cfg_addr->local_ip.len = 50;
	memcpy(shm_cfg_addr->local_ip.buf, DEFAULT_LOCAL_IP, sizeof(shm_cfg_addr->local_ip.buf));

	shm_cfg_addr->subnet_mask.id= 0x11;
	memcpy(shm_cfg_addr->subnet_mask.name, "SUBNET_MASK", 20);
	shm_cfg_addr->subnet_mask.len = 50;
	memcpy(shm_cfg_addr->subnet_mask.buf, DEFAULT_SUBNET_MASK, sizeof(shm_cfg_addr->subnet_mask.buf));
	
	shm_cfg_addr->gateway.id= 0x12;
	memcpy(shm_cfg_addr->gateway.name, "GATEWAY", 20);
	shm_cfg_addr->gateway.len = 50;
	memcpy(shm_cfg_addr->gateway.buf, DEFAULT_GATEWAY, sizeof(shm_cfg_addr->gateway.buf));

	shm_cfg_addr->work_time_out.id= 0x13;
	memcpy(shm_cfg_addr->work_time_out.name, "work_time_out", 20);
	shm_cfg_addr->work_time_out.len = 4;
	shm_cfg_addr->work_time_out.val=DEFAULT_WORK_TIME_OUT;

	shm_cfg_addr->gps_time_out.id= 0x14;
	memcpy(shm_cfg_addr->gps_time_out.name, "gps_time_out", 20);
	shm_cfg_addr->gps_time_out.len = 4;
	shm_cfg_addr->gps_time_out.val=DEFAULT_GPS_TIME_OUT;

	shm_cfg_addr->stun_time_out.id= 0x15;
	memcpy(shm_cfg_addr->stun_time_out.name, "stun_time_out", 20);
	shm_cfg_addr->stun_time_out.len = 4;
	shm_cfg_addr->stun_time_out.val=DEFAULT_STUN_TIME_OUT;

	shm_cfg_addr->start_time_out.id= 0x16;
	memcpy(shm_cfg_addr->start_time_out.name, "start_time_out", 20);
	shm_cfg_addr->start_time_out.len = 4;
	shm_cfg_addr->start_time_out.val=DEFAULT_START_TIME_OUT;

	shm_cfg_addr->fpga_version.id = 0x17;
	memcpy(shm_cfg_addr->fpga_version.name, "FPGA_VERSION", 20);
	shm_cfg_addr->fpga_version.len = 50;
	memcpy(shm_cfg_addr->fpga_version.buf, FPGA_VERSION, sizeof(shm_cfg_addr->fpga_version.buf));
	
	shm_cfg_addr->dev_call_timeout.id= 0x18;
	memcpy(shm_cfg_addr->dev_call_timeout.name, "dev_call_timeout", 20);
	shm_cfg_addr->dev_call_timeout.len = 4;
	shm_cfg_addr->dev_call_timeout.val=DEFAULT_DEV_CALL_TIMEOUT;


	shm_cfg_addr->scan_mode.id= 0x19;
	memcpy(shm_cfg_addr->scan_mode.name, "scan_mode", 20);
	shm_cfg_addr->scan_mode.len = 4;
	shm_cfg_addr->scan_mode.val=DEFAULT_SCAN_MODE;

	
	shm_cfg_addr->freq_offset.id= 0x1a;
    memcpy(shm_cfg_addr->freq_offset.name, "freq_offset", 20);
    shm_cfg_addr->freq_offset.len = 4;
	shm_cfg_addr->freq_offset.val=DEFAULT_FREQ_OFFSET;

	
	sem_cfg_v();
}


void fill_shm_ipc_default(void)
{
	sem_ipc_p();
	memset((char *)shm_ipc_addr, 0, sizeof(SHM_IPC_STRU));
	shm_ipc_addr->fpga_debug_sleep = 5;
	sem_ipc_v();
}


void fill_shm_nm_default(void)
{
	sem_nm_p();
	memset((char *)shm_nm_addr, 0, sizeof(SHM_NM_STRU));
	sem_nm_v();
}


void cover_cfg_info(void)
{
	int fd;	
	unsigned int dst_index = 0;
	unsigned int src_index = 0;
	unsigned int dst_item_num = 0;
	unsigned int src_item_num = 0;
	unsigned int same_item_num = 0;
	char * dst_buf = (char *)shm_cfg_addr;
	SHM_CFG_ITEM_HEAD * tmp_src;
	SHM_CFG_ITEM_HEAD * tmp_dst;

    fd = open(NAS_CONFIG_FILE, O_RDWR);
	if (-1 == fd)
	{
		reload_cfg_flag |= 0x04;
		return;
	}

	lseek(fd, 0, SEEK_SET);
	read(fd, src_buf, sizeof(src_buf));
	close(fd);
    /* 计算当前版本软件配置项数目 */
	for (dst_index = 0; *(unsigned short *)(dst_buf + dst_index) != 0 && dst_index < sizeof(SHM_CFG_STRU); \
		dst_index = dst_index + SHM_CFG_ITEM_NAME_SIZE + 4 + *(unsigned short *)(dst_buf + dst_index + SHM_CFG_ITEM_NAME_SIZE + 2))
	{
		dst_item_num++;
	}
	/* 计算配置文件配置项数目 */
	for (src_index = 0; *(unsigned short *)(src_buf + src_index) != 0 && src_index < sizeof(src_buf); \
		src_index = src_index + SHM_CFG_ITEM_NAME_SIZE + 4 + *(unsigned short *)(src_buf + src_index + SHM_CFG_ITEM_NAME_SIZE + 2))
	{
		src_item_num++;
	}
    /* 找出没有变化的配置项并将配置文件中保存值写入共享内存,
           保证新增配置项不被覆盖*/
	for (dst_index = 0; *(unsigned short *)(dst_buf + dst_index) != 0 && dst_index < sizeof(SHM_CFG_STRU); \
		dst_index = dst_index + SHM_CFG_ITEM_NAME_SIZE + 4 + *(unsigned short *)(dst_buf + dst_index + SHM_CFG_ITEM_NAME_SIZE + 2))
	{
		tmp_dst = (SHM_CFG_ITEM_HEAD *)&dst_buf[dst_index];
		
		for (src_index = 0; *(unsigned short *)(src_buf + src_index) != 0 && src_index < sizeof(src_buf); \
			src_index = src_index + SHM_CFG_ITEM_NAME_SIZE + 4 + *(unsigned short *)(src_buf + src_index + SHM_CFG_ITEM_NAME_SIZE + 2))
		{
			tmp_src = (SHM_CFG_ITEM_HEAD *)&src_buf[src_index];
			if ((tmp_src->id == tmp_dst->id) && (0 == strcmp(tmp_src->name, tmp_dst->name)) && (tmp_src->len == tmp_dst->len))
			{
				same_item_num++;
				sem_cfg_p();
				memcpy(&dst_buf[dst_index + SHM_CFG_ITEM_NAME_SIZE + 4], &src_buf[src_index + SHM_CFG_ITEM_NAME_SIZE + 4], tmp_dst->len);
				sem_cfg_v();
				break;
			}
		}
	}
    /* 配置项数目或者名称有变化 */
	if (!((dst_item_num == src_item_num) && (dst_item_num == same_item_num)))
	{
		reload_cfg_flag |= 0x01;
	}

	if (0 != strcmp(shm_cfg_addr->s_version.buf, SOFT_VERSION))
	{
		sem_cfg_p();
		memcpy(shm_cfg_addr->s_version.buf, SOFT_VERSION, sizeof(shm_cfg_addr->s_version.buf));
		sem_cfg_v();
		reload_cfg_flag |= 0x02;
	}
}


int daemon_pthread_create(void)
{
	if (pthread_create(&tid_run_led, NULL, pthread_run_led, NULL)) 
	{
		printf("pthread_create tid_run_led err\n");
		return -1;
	}
	pthread_detach(tid_run_led);

	if (pthread_create(&tid_pwr_key_det, NULL, pthread_pwr_key_det, NULL)) 
	{
		printf("pthread_create tid_pwr_key_det err\n");
		return -1;
	}
	pthread_detach(tid_pwr_key_det);	

	if (pthread_create(&tid_wd_wg, NULL, pthread_wd_wg, NULL)) 
	{
		printf("pthread_create tid_wd_wg err\n");
		return -1;
	}
	pthread_detach(tid_wd_wg);
	
	return 0;
}


void * pthread_run_led(void *arg)
{
	fd_run_led = gpio_init(GPIO_RUN_LED, 1);
	
	while (1)
	{
		usleep(1000000);
		gpio_write(fd_run_led, 1);
		usleep(1000000);
		gpio_write(fd_run_led, 0);
	}
	pthread_exit(NULL);
}


void * pthread_pwr_key_det(void *arg)
{
	int ret = 0;
	int cnt = 0;

	usleep(10000000);
	
	fd_mcu_pwr_ctrl = gpio_init(GPIO_MCU_PWR_CTRL, 1);
	fd_pwr_key_det = gpio_init(GPIO_PWR_KEY_DET, 0);

	gpio_write(fd_mcu_pwr_ctrl, 0);
	
	while (1)
	{
		usleep(500000);

		ret = gpio_read(fd_pwr_key_det);
		if (0 == ret)
		{
			cnt++;
		}
		else
		{
			cnt = 0;
		}
		
		if (cnt == 5)
		{
			printf("start power down!\n");
			gpio_write(fd_mcu_pwr_ctrl, 1);
		}
	}
	pthread_exit(NULL);
}


void * pthread_wd_wg(void *arg)
{	
	fd_wd_wg = gpio_init(GPIO_WD_WG, 1);
	fd_wd_en = gpio_init(GPIO_WD_EN, 1);
	gpio_write(fd_wd_en, 1);
	
	while (1)
	{
		usleep(50000);
		gpio_write(fd_wd_wg, 1);
		usleep(50000);
		gpio_write(fd_wd_wg, 0);
	}
	pthread_exit(NULL);
}


void version_back_operation()
{
    printf("process application version back operation\n");
    unsigned int size;
    pid_t status;

    FILE *fp,*fd;
    char buffer[30]= {0};
    char buffer_active[]="active\n";
    char buffer_error[]="error\n";
	char buffer_back[]="back\n";

    fp=fopen("/loadapp/loadflag","wb");

    if(fp == NULL)
    {
        printf("open /loadapp/loadflag file failed!\n");
		chdir("/loadapp");
		system("./reboot_epack");
        return;
    }

    size=strlen(buffer_error);
    fwrite(buffer_error,size,1,fp);
    fclose(fp);

    fd = popen("/loadapp/mount.sh", "r");

    if(fd==NULL)
    {
        printf("open /loadapp/mount.sh file error!\n");
		chdir("/loadapp");
        system("./reboot_epack");
        return;
    }

    memset(buffer,0x00,sizeof(buffer));

    if(fgets((char*)buffer, sizeof(buffer), fd)==NULL)
    {
        printf("get mount point error!\n");
		chdir("/loadapp");
        system("./reboot_epack");
        return;
    }

    printf("current mount point %s on /loadapp\n",buffer);

    pclose(fd);

    if(strcmp((char*)buffer,"/dev/mtdblock6\n")==0)
    {
        status = system("mount -t jffs2 /dev/mtdblock7 /mnt");
        printf("mount /dev/mtdblock7 on /mnt\n");
    }
    else
    {
        status = system("mount -t jffs2 /dev/mtdblock6 /mnt");
        printf("mount /dev/mtdblock6 on /mnt\n");
    }

    if (-1 == status)
    {
        printf("mount /dev/mtdblockx on /mnt failed!\n");
		chdir("/loadapp");
        status = system("./reboot_epack");
        return;
    }
    else
    {
        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                ;
            }
            else
            {
                printf("mount /dev/mtdblockx on /mnt WEXITSTATUS(status) failed!\n");
				chdir("/loadapp");
                system("./reboot_epack");
                return;
            }
        }
        else
        {
            printf("mount /dev/mtdblockx on /mnt WIFEXITED(status) failed!\n");
			chdir("/loadapp");
            system("./reboot_epack");
            return;
        }
    }

    fp=fopen("/mnt/loadflag","rb");

    if(fp == NULL)
    {
        printf("open /mnt/loadflag file error!\n");
        goto exec;
    }

    memset(buffer,0,sizeof(buffer));
    size=sizeof(buffer);
    fread(buffer,size,1,fp);
    fclose(fp);

    if(0 == strcmp(buffer_back, (char*)buffer))
    {
        printf("this is back application!\n");
        fp=fopen("/mnt/loadflag","wb");

        if(fp == NULL)
        {
            printf("open /mnt/loadflag file error!\n");
            goto exec;

        }

        size=strlen(buffer_active);
        fwrite(buffer_active,size,1,fp);
        fclose(fp);
    }

exec:
    chdir("/loadapp");
	
    system("umount /mnt");

    system("./reboot_epack");

    return;
}


void load_fpga(void)
{
	printf("start load FPGA\n");
	system("echo 0 > /sys/class/fpga-bridge/fpga2hps/enable;echo 0 > /sys/class/fpga-bridge/hps2fpga/enable;echo 0 > /sys/class/fpga-bridge/lwhps2fpga/enable");
	system("dd if=soc_system.rbf of=/dev/fpga0 bs=1M");
	system("echo 1 > /sys/class/fpga-bridge/fpga2hps/enable;echo 1 > /sys/class/fpga-bridge/hps2fpga/enable;echo 1 > /sys/class/fpga-bridge/lwhps2fpga/enable");
}

