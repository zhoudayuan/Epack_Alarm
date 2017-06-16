/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    daemon.h
 * @brief   守护进程头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "mgr.h"
#include "gpio_interface.h"

#define TASK_NUM                     7

#define MONTH_PER_YEAR   12   // 一年12月
#define YEAR_MONTH_DAY   20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20   // 时分秒缓存大小
#define TASK_NAME   "daemon"

/* 初始值*/
#define SOFT_VERSION                 "V1.0.03.004a"
#define FPGA_VERSION                 "V0.0.00.000"
#define DEFAULT_DEV_ID               0x00
#define DEFAULT_CC                   11
#define DEFAULT_LOCKING_TIME         500
#define DEFAULT_FREQ                 358500000
#define DEFAULT_POWER                5
#define DEFAULT_OPEN_CLOSE_LOOP      0
#define DEFAULT_START_NEIGHBOR       1
#define DEFAULT_HALF_VARIANCE        1500
#define DEFAULT_NEIGHBOR_PERIOD      1
#define DEFAULT_TERMINAL_CC          9
#define DEFAULT_PROTOCOL_MODE        0
#define DEFAULT_SUBNET_ID            10
#define DEFAULT_STUN_FLAG            0
#define DEFAULT_KILL_FLAG            0
#define DEFAULT_LOCAL_IP             "192.168.72.31"
#define DEFAULT_SUBNET_MASK          "255.255.255.0"
#define DEFAULT_GATEWAY              "192.168.72.255"
#define DEFAULT_WORK_TIME_OUT        5000
#define DEFAULT_GPS_TIME_OUT         11000
#define DEFAULT_STUN_TIME_OUT        6000
#define DEFAULT_START_TIME_OUT       6000
#define DEFAULT_DEV_CALL_TIMEOUT     60
#define DEFAULT_SCAN_MODE            0
#define DEFAULT_FREQ_OFFSET          500
#define DEFAULT_ALARM_SWITCH_STATUS  0
#define DEFAULT_CLOSE_TRAN_THRESHOLD 600*65536
#define DEFAULT_RESUME_TRAN_THRESHOLD 75*65536
#define DEFAULT_START_TEMP_ALARM     85
#define DEFAULT_CLOSE_TEMP_ALARM     80
#define DEFAULT_MOTO_MODE_SWITCH     1
#define DEFAULT_THRESHOLD_OPT_SWITCH     1
#define DEFAULT_NEIGHBOR_REPORT_AI     0








#define GPIO_RUN_LED                 236  //旧板是196
#define GPIO_MCU_PWR_CTRL            210  //旧板是170
#define GPIO_PWR_KEY_DET             211  //旧板是171
#define GPIO_WD_EN                   217
#define GPIO_WD_WG                   218


typedef struct {
	pid_t pid;
	int	count;
	char * task_name;
}CHILD_TASK;


int start_child_task(CHILD_TASK * sub);
int monitor_child_task(CHILD_TASK * sub);
void fill_shm_cfg_default(void);
void fill_shm_ipc_default(void);
void fill_shm_nm_default(void);
void cover_cfg_info(void);
void version_back_operation();
int daemon_pthread_create(void);
void * pthread_run_led(void *arg);
void * pthread_pwr_key_det(void *arg);
void * pthread_wd_wg(void *arg);
void load_fpga(void);
int init_alarm_table(void);
void GetCompileTime(void);
int write_epack_version_to_file();


#endif
