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

#define FLAG_CLR                 0
#define FLAG_OK                  1
#define FLAG_PARAM               2

#define GPIO_RUN_LED             236  //旧板是196
#define GPIO_MCU_PWR_CTRL        210  //旧板是170
#define GPIO_PWR_KEY_DET         211  //旧板是171

#define KILL_TIMEOUT             600 //s
#define KILL_CHECK_CNT           5
#define ALARM_REPORT_TIME        10  //min

/******************SAVE DATA***********************/
#define DATA_LENGTH             0x4380
#define MAX_VOLUMN_IQ_DATA      60*1024*1024
/******************SAVE DATA***********************/




typedef struct {
	unsigned short  gps_time;
	unsigned short  neighbor_period;
	unsigned short  locking_time;
	unsigned short  variance;
	unsigned int power;
	unsigned int freq;
	unsigned char start_neighbor;
}__attribute__((packed,aligned(1)))CENTER_DATA;

typedef struct {
	unsigned int  value:10;
    unsigned int  powerAmplifier_status:1;
    unsigned int  power_status:1;
    unsigned int other:20;
}__attribute__((packed,aligned(1)))TEMPERATE_STRUCT;

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
void net_config(void);
void * pthread_func_center_print_monitor(void *arg);
void send_cc_get_response_cmd(unsigned short cmd_code, unsigned char * payload);
int init_fpga_mem_iq(void);
int test_code(int status);
void send_iq_close_to_queque(int flag);
void MGR_Alarm_Trigger(ALARM_ITEM *alarm_item);
void MGR_Alarm_Trigger(ALARM_ITEM *alarm_item);
void MGR_Alarm_Set();
void MGR_Local_AlarmTable_Update_Send(ALARM_SEND_ITEM_PAYLOAD *pAlarm,int index,UINT8 uStatus);
//void Center_Nm_Query_AlarmTable_Set();

void MGR_Ai_AlarmTable_Set();
void Send_Local_AlarmTable_Set();
UINT8  MGR_Alarm_Update_Status(UINT32 type,UINT8 uStatus,UINT32 value);
void MGR_Alarm_AI_Build(NAS_INF_DL_T *pAlarm,ALARM_TYPE type);
void MGR_Alarm_Center_Build(NM_IPC_MSG *pAlarm,ALARM_TYPE type,UINT8 num);
void MGR_Alarm_AI_Send(UINT32 u4SocketFd,ALARM_TYPE type);
void MGR_Alarm_Center_Send(ALARM_TYPE type);
void * MGR_Get_Shm(int id, size_t size);
void Center_Query_AlarmTable_Set();
void send_center_query_alarmtable_ack(unsigned char * buffer);
void clear_report_ai_alarm_flag();
int  confirm_mount_point();
int update_loadapp();
int update_dtb();
int update_uboot();
int update_file_system();
int update_rbf();
int update_zimage();
void  send_remote_query_alarm_ack(unsigned short cmd_code, unsigned char nm_type, unsigned char src_id, unsigned char dst_id, unsigned char * data);

void * pthread_func_alarm_send_local(void *arg);
unsigned int db_convert_rssi(int input_db);
void find_db_table(int index,int *convert_db ,unsigned int rssi);
int rssi_convert_db(unsigned int rssi);
int write_vertion_to_eeprom(void);
int read_vertion_from_eeprom(void);
int read_old_fpga_param_from_eeprom(void);


#endif
