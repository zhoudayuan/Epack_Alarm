/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_local_handle.h
 * @brief   本地网管处理模块头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#ifndef __MGR_LOCAL_HANDLE_H__
#define __MGR_LOCAL_HANDLE_H__

#include "mgr.h"
#include "gpio_interface.h"
#include "mgr_i2c_eeprom.h"


#define FPGA_MEM_BASE_ADDR              0xc0010000
#define FPGA_MEM_LENGTH                 0x1000
#define DATA_LENGTH             0x4380
#define MAX_VOLUMN_IQ_DATA      60*1024*1024

#define FLAG_CLR                 0
#define FLAG_OK                  1
#define FLAG_PARAM               2

#define GPIO_RUN_LED             236  //旧板是196
#define GPIO_MCU_PWR_CTRL        210  //旧板是170
#define GPIO_PWR_KEY_DET         211  //旧板是171

#define KILL_TIMEOUT             600 //s
#define KILL_CHECK_CNT           5

typedef struct {
	unsigned short  gps_time;
	unsigned short  neighbor_period;
	unsigned short  locking_time;
	unsigned short  variance;
	unsigned int power;
	unsigned int freq;
	unsigned char start_neighbor;
}__attribute__((packed,aligned(1)))CENTER_DATA;


void  read_fpga_version(void);
int init_fpga_mem_nm(void);
int init_ipc_socket(void);
int mgrh_msg_queue_create(void);
int mgrh_pthread_create(void);
void * pthread_mgra_mgrh_recv(void *arg);
void * pthread_adp_mgrh_recv(void *arg);
void * pthread_msg_handle(void *arg);
void * pthread_run_led(void *arg);
void * pthread_pwr_key_det(void *arg);
void * pthread_func_alarm_handle(void *arg);
void * pthread_func_alarm_status(void *arg);
void * pthread_func_alarm_send(void *arg);
void * pthread_fpga_debug(void *arg);
void * pthread_func_kill_monitor(void *arg);
void * pthread_func_stun_monitor(void *arg);
void send_remote_nm_get_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char src_id, unsigned char dst_id, unsigned char * data);
void send_remote_nm_set_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char src_id, unsigned char dst_id);
void send_local_nm_get_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char * data);
void send_local_nm_set_ack(unsigned short cmd_code, unsigned char nm_type);
void init_local_val(void);
void notify_fpga(void);
int init_fpga_param(void);
int write_fpga_param_to_eeprom(void);
int read_fpga_param_from_eeprom(void);
int write_esn_param_to_eeprom(void);
int read_esn_param_from_eeprom(void);
int init_eeprom(void);
void send_cc_set_cmd(unsigned short cmd_code, unsigned char * payload);
int init_alarm_table(void);
void net_config(void);
void * pthread_func_center_print_monitor(void *arg);
void send_cc_get_response_cmd(unsigned short cmd_code, unsigned char * payload);
int test_code(int status);
void send_iq_close_to_queque(int flag);
int init_fpga_mem_iq(void);

#endif
