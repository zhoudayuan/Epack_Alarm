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

#include <signal.h>
#include <errno.h>
#include "mgr_common.h"
#include <sys/socket.h>


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
/**
 * @var ptMgrAlarm
 * @brief 告警表指针
 */
ALARM_ITEM *ptMgrAlarm = NULL;

char src_buf[4096];
/* bit0-配置项变化 bit1-版本变化 bit2-打开配置文件失败 bit3-配置文件不存在 */
unsigned char reload_cfg_flag = 0;
unsigned char pwr_key_press_flg=0;

int fd_run_led;
int fd_mcu_pwr_ctrl;
int fd_pwr_key_det;
int fd_wd_en;
int fd_wd_wg;
	
pthread_t tid_run_led;
pthread_t tid_pwr_key_det;
pthread_t tid_wd_wg;


/**
* @var s_tCclSigUlSockfd
* @brief 呼叫控制层信令上行套接字
*/
 static int  s_tToCcSigSockfd ;
/**
* @struct  s_tCclSigUlAddr
* @brief 呼叫控制层信令上行地址
*/
static  struct sockaddr_in  s_tToCcSigAddr;
int main()
{	
	CHILD_TASK sub[TASK_NUM] = {
		{0, 0, (char *)"logmain"},
		{0, 0, (char *)"cli"},
		{0, 0, (char *)"mgr_center_agent"},
		{0, 0, (char *)"mgr_ack_receive"},
		{0, 0, (char *)"mgr_local_handle"},
		{0, 0, (char *)"route"},
        {0, 0, (char *)"sap"},
        {0, 0, (char *)"esn"}
	};

	system("echo 0 > /proc/sys/kernel/printk");

    GetCompileTime();
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

	if (init_alarm_table())
	{
		printf("init_alarm_table err\n");
		return -1;
	}
	
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
	if(write_epack_version_to_file()==-1)
	{
		printf("write epack version to file error\n");
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



int init_alarm_table(void)
{
	int i = 0;
	UINT8 alarm_code;

	sem_ipc_p();

	
	ptMgrAlarm = ptIPCShm->alarm_struct;
	memset(ptMgrAlarm,0x00,MGR_ALARM_MAX * sizeof(ALARM_ITEM));

	for(i = 0;i < MGR_ALARM_MAX ; i++)
	{
		ptMgrAlarm->alm_code = 0xff;
		ptMgrAlarm++;
	}


    ptMgrAlarm = ptIPCShm->alarm_struct;
    alarm_code=MGR_ALARM_CENTER_1;
    for(i = 0;i < (MGR_ALARM_CENTER_NUM+MGR_ALARM_FPGA_NUM+MGR_ALARM_SERVER_NUM);i++)
	{
		ptMgrAlarm->alm_code = alarm_code;
        alarm_code++;
		ptMgrAlarm++;
	}

    sem_ipc_v();
	
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
			printf("vfork err\n");
			return -1;
		}
		case 0:
		{
			printf("start boa process\n");
			if (execl("/loadapp/www/boa", "boa", NULL) < 0)
			{
				printf("execl boa err\n");
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
    int boa_cnt = 0;
	pid_t pid;
	int index;

    while(1)
    {
        if(pwr_key_press_flg)
        {
            printf("[monitor_child_task] break\n");
            break;
        }
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
            close(fd);
			return -1;
		}
		lseek(fd, 0, SEEK_SET);
		memset(tmp_buf, 0, sizeof(tmp_buf));
		if(-1 ==read(fd, tmp_buf, sizeof(tmp_buf)))
        {
            printf("read /tmp/snmpd_daemon.txt fail\n");
            close(fd);
			return -1;
        }      
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


        //boa守护
		system("ps -ef | grep boa | grep -v grep |awk '{print $2}' > /tmp/boa_daemon.txt");
	    system("chmod 600 /tmp/boa_daemon.txt");	
		fd = open("/tmp/boa_daemon.txt", O_RDWR);
		if (fd == -1)
		{
			printf("open /tmp/boa_daemon.txt fail\n");
            close(fd);
			return -1;
		}
		lseek(fd, 0, SEEK_SET);
		memset(tmp_buf, 0, sizeof(tmp_buf));
		read(fd, tmp_buf, sizeof(tmp_buf));
		close(fd);
		if (boa_cnt == 10)
		{
			printf("restart 10 times process is boa\n");
			version_back_operation();
		}
		else
		{
			if (0 == strncmp("", tmp_buf, 1))
			{
				boa_cnt++;
				printf("restart boa process\n");
				switch(pid = vfork())
				{
					case -1:
					{
						printf("vfork err\n");
						return -1;
					}
					case 0:
					{
						printf("start boa process\n");
						if (execl("/loadapp/www/boa", "boa", NULL) < 0)
						{
							printf("execl boa err\n");
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
	memcpy(shm_cfg_addr->work_time_out.name, "work_timeout", 20);
	shm_cfg_addr->work_time_out.len = 4;
	shm_cfg_addr->work_time_out.val=DEFAULT_WORK_TIME_OUT;

	shm_cfg_addr->gps_time_out.id= 0x14;
	memcpy(shm_cfg_addr->gps_time_out.name, "gps_timeout", 20);
	shm_cfg_addr->gps_time_out.len = 4;
	shm_cfg_addr->gps_time_out.val=DEFAULT_GPS_TIME_OUT;

	shm_cfg_addr->stun_time_out.id= 0x15;
	memcpy(shm_cfg_addr->stun_time_out.name, "stun_timeout", 20);
	shm_cfg_addr->stun_time_out.len = 4;
	shm_cfg_addr->stun_time_out.val=DEFAULT_STUN_TIME_OUT;

	shm_cfg_addr->start_time_out.id= 0x16;
	memcpy(shm_cfg_addr->start_time_out.name, "start_timeout", 20);
	shm_cfg_addr->start_time_out.len = 4;
	shm_cfg_addr->start_time_out.val=DEFAULT_START_TIME_OUT;

	shm_cfg_addr->fpga_version.id = 0x17;
	memcpy(shm_cfg_addr->fpga_version.name, "FPGA_VERSION", 20);
	shm_cfg_addr->fpga_version.len = 50;
	memcpy(shm_cfg_addr->fpga_version.buf, FPGA_VERSION, sizeof(shm_cfg_addr->fpga_version.buf));
	
	shm_cfg_addr->dev_call_timeout.id= 0x18;
	memcpy(shm_cfg_addr->dev_call_timeout.name, "call_timeout", 20);
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

	shm_cfg_addr->alarm_switch_status.id= 0x1b;
    memcpy(shm_cfg_addr->alarm_switch_status.name, "alarm_status", 20);
    shm_cfg_addr->alarm_switch_status.len = 4;
	shm_cfg_addr->alarm_switch_status.val=DEFAULT_ALARM_SWITCH_STATUS;

    shm_cfg_addr->close_transmit_threshold.id= 0x1c;
    memcpy(shm_cfg_addr->close_transmit_threshold.name, "close_tran", 20);
    shm_cfg_addr->close_transmit_threshold.len = 4;
	shm_cfg_addr->close_transmit_threshold.val=DEFAULT_CLOSE_TRAN_THRESHOLD;

    shm_cfg_addr->tempratue_alarm_start_threshold.id= 0x1d;
    memcpy(shm_cfg_addr->tempratue_alarm_start_threshold.name, "start_temp", 20);
    shm_cfg_addr->tempratue_alarm_start_threshold.len = 4;
	shm_cfg_addr->tempratue_alarm_start_threshold.val=DEFAULT_START_TEMP_ALARM;

    shm_cfg_addr->tempratue_alarm_close_threshold.id= 0x1e;
    memcpy(shm_cfg_addr->tempratue_alarm_close_threshold.name, "close_temp", 20);
    shm_cfg_addr->tempratue_alarm_close_threshold.len = 4;
	shm_cfg_addr->tempratue_alarm_close_threshold.val=DEFAULT_CLOSE_TEMP_ALARM;

    shm_cfg_addr->resume_transmit_threshold.id= 0x1f;
    memcpy(shm_cfg_addr->resume_transmit_threshold.name, "resume_tran", 20);
    shm_cfg_addr->resume_transmit_threshold.len = 4;
	shm_cfg_addr->resume_transmit_threshold.val=DEFAULT_RESUME_TRAN_THRESHOLD;

    shm_cfg_addr->moto_mode_switch.id= 0x20;
    memcpy(shm_cfg_addr->moto_mode_switch.name, "moto_switch", 20);
    shm_cfg_addr->moto_mode_switch.len = 4;
	shm_cfg_addr->moto_mode_switch.val=DEFAULT_MOTO_MODE_SWITCH;

    shm_cfg_addr->threshold_opt_switch.id= 0x21;
    memcpy(shm_cfg_addr->threshold_opt_switch.name, "threshold_opt", 20);
    shm_cfg_addr->threshold_opt_switch.len = 4;
	shm_cfg_addr->threshold_opt_switch.val=DEFAULT_THRESHOLD_OPT_SWITCH;

    shm_cfg_addr->neighbor_report_ai.id= 0x22;
    memcpy(shm_cfg_addr->neighbor_report_ai.name, "neighbor_report", 20);
    shm_cfg_addr->neighbor_report_ai.len = 4;
	shm_cfg_addr->neighbor_report_ai.val=DEFAULT_NEIGHBOR_REPORT_AI;

    shm_cfg_addr->web_code.id= 0x23;
    memcpy(shm_cfg_addr->web_code.name, "web_code", 20);
    shm_cfg_addr->web_code.len = 50;
	memcpy(shm_cfg_addr->web_code.buf, DEFAULT_WEB_CODE, sizeof(shm_cfg_addr->web_code.buf));

    shm_cfg_addr->web_user.id= 0x24;
    memcpy(shm_cfg_addr->web_user.name, "web_user", 20);
    shm_cfg_addr->web_user.len = 50;
	memcpy(shm_cfg_addr->web_user.buf, DEFAULT_WEB_USER, sizeof(shm_cfg_addr->web_user.buf));
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
	if(-1==read(fd, src_buf, sizeof(src_buf)))
    {
        reload_cfg_flag |= 0x04;
		return;
    }   
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

int check_thread_exist(pthread_t threadid)
{
    int Ret = pthread_kill(threadid,0);
    
    if(Ret == ESRCH)
    {
        printf("the specified thread did not exists or already quit\n");
    }
    else if(Ret == EINVAL)
    {
        printf("signal is invalid\n");
    }
    else
    {
        printf("the specified thread is alive\n");
        return 0;
    }

    return -1;
}
void exit(int signum)
{
    int Ret;
    if(SIGUSR1 == signum)
    {
        Ret=check_thread_exist(tid_run_led );
        if(!Ret)
        {
            printf("[pthread_run_led]exit \n");
            pthread_exit(0);
        }
    }
}

/**
* @brief   设备下电通知手咪信息包
* @param [out] pvCenterData  中心上行数据
* @param [out] Len     包长度
* @author 牛功喜
* @since
* @bug
*/

void IDP_PowerDown_Info(unsigned char *pvCenterData, int *Len)
{
    memset(pvCenterData,0,sizeof(pvCenterData));
    SMS_CENTER_CCL_DL* ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CC_SIG_HRD_LEN;
    ptCenterData->SharedHead.SigType = SIG_SMS_SEND;
    ptCenterData->SmsType =CT_DEVICE_POWER_DOWN;
    ptCenterData->SenderNum[0] = 0xff;
    ptCenterData->SenderNum[1] = 0;
    ptCenterData->SenderNum[2] = 0;
    ptCenterData->ReceiverNum[0] = 0xff;
    ptCenterData->ReceiverNum[1] = 0;
    ptCenterData->ReceiverNum[2] = 0;
    ptCenterData->ValidLength = 0;
    *Len = sizeof(SMS_CENTER_CCL_DL);
}
/**
* @brief CCL 向CC 发送系统下电通知
* @author 牛功喜
* @since
* @bug
*/
 void SentPowerdownSig()
{
    int i4Len;
    unsigned char  centerdata[800];
    int u4Datalen;
    IDP_PowerDown_Info(centerdata,&u4Datalen);
    i4Len=sendto(s_tToCcSigSockfd, centerdata, u4Datalen, 0,(struct sockaddr *)&s_tToCcSigAddr,sizeof(s_tToCcSigAddr));
    if(u4Datalen!=i4Len)
    {
        printf("[SentPowerdownSig] len err :%d\n",i4Len);
        perror("send");
    }
    /*else
    {
        for(i4Len=0;i4Len<u4Datalen;i4Len++)
        {
            printf("%02x",centerdata[i4Len]);
            if(i4Len+1/30==0)
            {
                printf("\n");

            }
        }
    }*/
    printf("[SentPowerdownSig]sendto cc\n");

}


void * pthread_run_led(void *arg)
{ 
	struct sigaction act, oldact;
    act.sa_handler = exit;
    sigaddset(&act.sa_mask, SIGUSR1); 
    act.sa_flags = 0; 
    sigaction(SIGUSR1, &act, &oldact);

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

int check_status(int status)
{
    if (-1 == status) 
    { 
        printf("[check_status]system return failed!\n");
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
			    printf(" [check_status]WEXITSTATUS(status) failed!\n");	   
		   		return -1;
			}
		}
		else  
		{  
		    printf("[check_status]WIFEXITED(status) failed!\n");
			return -1;
		}
	}
}


void stop_srv(void)
{
    int status;
    
    pwr_key_press_flg=1;
    
    SentPowerdownSig();  //中心发送下电通知

    if(0 < fd_wd_en)
    {
        gpio_write(fd_wd_en, 0);    //关闭看门狗
    }
    else
    {
        printf("[stop_srv]fd_wd_en err\n");
    }
    
    pthread_kill(tid_run_led,SIGUSR1);
	gpio_write(fd_run_led, 0);
    usleep(200000);
    
    status=system("pkill sap");
    if(CHECK_STATUS_OK == check_status(status))
    {
        printf("[stop_srv]kill sap  ok\n");
    }
    else
    {
        printf("[stop_srv]kill sap  fail\n");
    }

    printf("check  run_led  thread is exsit\n");
    check_thread_exist(tid_run_led);
    printf("[stop_srv]daemon exit\n");
    usleep(200000);
    exit(0);
}

void * pthread_pwr_key_det(void *arg)
{
	int ret = 0;
	int cnt = 0;

	usleep(10000000);
	
	fd_mcu_pwr_ctrl = gpio_init(GPIO_MCU_PWR_CTRL, 1);
	fd_pwr_key_det = gpio_init(GPIO_PWR_KEY_DET, 0);


    s_tToCcSigSockfd = socket(AF_INET, SOCK_DGRAM, 0);     

		    //呼叫控制信令上行
    if(0 > s_tToCcSigSockfd )
    {
        printf("[pthread_pwr_key_det]creat  CclSigUlSockfd error\n");
        //return -1;
    }

    bzero(&s_tToCcSigAddr, sizeof(s_tToCcSigAddr));
    s_tToCcSigAddr.sin_family = AF_INET;
    s_tToCcSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tToCcSigAddr.sin_port = htons(SOCK_PORT_CCL_CC_S);


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
			//灭运行灯、杀业务线程 关看门狗
			stop_srv();
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
        pclose(fd);
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
    else if(strcmp((char*)buffer,"/dev/mtdblock5\n")==0)
    {
        printf("****************************\n");
        printf("/dev/mtdblock5 error\n");
        printf("****************************\n");
        return;
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
    if(0==fread(buffer,size,1,fp))
    {
        printf("fread /mnt/loadflag file error!\n");
        goto exec;
    }
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

void GetCompileTime(void)
{

    const char year_month[MONTH_PER_YEAR][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char compile_date[YEAR_MONTH_DAY]   = {0};
    char compile_time[HOUR_MINUTES_SEC] = {0};
    char str_month[4] = {0};
    int year, month, day, hour, minutes, seconds;
    int i;

    sprintf(compile_date, "%s", __DATE__);      // "Aug 23 2016"
    sprintf(compile_time, "%s", __TIME__);      // "10:59:19"

    sscanf(compile_date, "%s %d %d", str_month, &day, &year);
    sscanf(compile_time, "%d:%d:%d", &hour, &minutes, &seconds);
    month=0;
    for(i = 0; i < MONTH_PER_YEAR; ++i)
    {
        if (strncmp( str_month, year_month[i], 3) == 0)
        {
            month = i + 1;
            break;
        }
    }
     printf("[%s] Compile time: %d-%d-%d %d:%d:%d\n", TASK_NAME, year, month, day, hour, minutes, seconds); 
}


int write_epack_version_to_file()
{
	char version[20]={0};
	char b[20]={0};    
	int temp[4];    
	int index=0;    
	char *p;    
	int size=0;    
	FILE * fp=NULL;    


	memcpy(version,shm_cfg_addr->s_version.buf,sizeof(version));
	p=strtok(version,".");    
	size=strlen(p);   
	memcpy(b,p+1,size);   
	temp[index]=atoi(b);   
	++index;    
	while((p=strtok(NULL,"."))&&(index<4))   
	{     	
		memset(b,0,sizeof(b));    	
		size=strlen(p);	memcpy(b,p,size);	
		temp[index]=atoi(b);	
		++index;   
	}    
	memset(b,0,sizeof(b));   
	size=sprintf(b,"%03d%03d%03d%03d",temp[0],temp[1],temp[2],temp[3]);  
	chdir("/loadapp");
	fp=fopen("version.txt","w+");   
	if(fp==NULL)  
	{    	
		printf("fopen vertion.txt error\n");	
		//fclose(fp);
		return -1;   
	}    
	fwrite(b,1,size,fp);
	fclose(fp);
	return 0;
}

