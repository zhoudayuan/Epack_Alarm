/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    mgr.h
 * @brief   网管模块头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
 *   *************************************************************************/


#ifndef __MGR_H__
#define __MGR_H__

#include "dll_common.h"
#include "mgr_common.h"


#define NM_IPC_MSG_PAYLOAD_SIZE  512
#define NM_TIMEOUT_TIME          5000000 //us
#define NM_DEBUG_TIMEOUT_TIME    10000000 //us
#define POLLING_INTERVA          20000   //us
#define CALIBRATE_PHASE_DELAY    450000000
#define CALIBRATE_DELAY          20000000

#define ACK_FLAG_OK              1
#define ACK_FLAG_CLR             0

#define TIME_SLOT_F1S1           0x00
#define TIME_SLOT_F1S2           0x01
#define TIME_SLOT_F2S1           0x02
#define TIME_SLOT_F2S2           0x03
#define TIME_SLOT_IDLE           0xFF


/**
 * @单机网管配置项值范围限制
 */
#define MIN_FREQ   350000000
#define MAX_FREQ   400000000
#define MIN_CC     0
#define MAX_CC     15
#define MIN_VARIANCE  50
#define MAX_VARIANCE  5000
#define MIN_DEV_ID  0
#define MAX_DEV_ID  31
#define MIN_LOCK_TIME  500
#define MAX_LOCK_TIME  3000
#define MIN_NETMASK_ID  0
#define MAX_NETMASK_ID  255
#define MIN_TIMEOUT  0
#define MAX_TIMEOUT  65535
#define MIN_PERIOD  2
#define MAX_PERIOD  20


typedef enum {
	MSG_TYPE_CMD = 1,
	MSG_TYPE_ACK = 2
}QUEUE_MSG_TYPE;


typedef struct {
	unsigned char ack_flag;
	unsigned int data;
}SHM_NM_UINT_ITEM;


typedef struct {
	unsigned char ack_flag;
	unsigned char data[NM_IPC_MSG_PAYLOAD_SIZE];
}SHM_NM_OPAQUE_ITEM;


typedef struct {
	//配置
	SHM_NM_UINT_ITEM dev_freq;
	SHM_NM_UINT_ITEM dev_Power;
	SHM_NM_UINT_ITEM open_close_loop;
	SHM_NM_UINT_ITEM locking_time;
	SHM_NM_UINT_ITEM half_variance_threshold;
	SHM_NM_UINT_ITEM start_neighbor;
	SHM_NM_UINT_ITEM neighbor_period;
	SHM_NM_UINT_ITEM dev_id;
	SHM_NM_UINT_ITEM local_cc;
	SHM_NM_UINT_ITEM terminal_cc;
	SHM_NM_UINT_ITEM protocol_mode;
	SHM_NM_UINT_ITEM air_subnet_id;
	SHM_NM_UINT_ITEM work_time_out;
	SHM_NM_UINT_ITEM gps_time_out;
	SHM_NM_UINT_ITEM stun_time_out;
	SHM_NM_UINT_ITEM start_time_out;
	SHM_NM_OPAQUE_ITEM local_ip;
	SHM_NM_OPAQUE_ITEM local_mac;
	SHM_NM_OPAQUE_ITEM version;
	SHM_NM_OPAQUE_ITEM fpga_version;
	SHM_NM_OPAQUE_ITEM combined_data;  //中心网管数据包
	//校准
	SHM_NM_UINT_ITEM emissive_vco_freq;
	SHM_NM_UINT_ITEM receiving_vco_freq;
	SHM_NM_UINT_ITEM second_local_oscillator;
	SHM_NM_UINT_ITEM power_amplifier_switch;
	SHM_NM_UINT_ITEM power;
	SHM_NM_UINT_ITEM debug_emissive_freq;
	SHM_NM_UINT_ITEM offset;
	SHM_NM_UINT_ITEM calibrate_offset;
	SHM_NM_UINT_ITEM phase;
	SHM_NM_UINT_ITEM calibrate_phase;
	SHM_NM_UINT_ITEM afc;
	SHM_NM_UINT_ITEM save_afc;
	SHM_NM_UINT_ITEM iqs;
	SHM_NM_UINT_ITEM error_rate;
	SHM_NM_UINT_ITEM start_debug;
	SHM_NM_UINT_ITEM debug_open_close_loop;
	SHM_NM_UINT_ITEM debug_power;
	SHM_NM_UINT_ITEM start_error_rate;
	SHM_NM_UINT_ITEM select_signal_source;
	SHM_NM_UINT_ITEM slot_control;
	SHM_NM_UINT_ITEM vgs;
	SHM_NM_UINT_ITEM save_eeprom;
	SHM_NM_UINT_ITEM set_981;
	SHM_NM_UINT_ITEM query_981;
	SHM_NM_UINT_ITEM set_998;
	SHM_NM_UINT_ITEM query_998;
	SHM_NM_UINT_ITEM dev_call_timeout;
	SHM_NM_UINT_ITEM error_rate_freq;  
	SHM_NM_UINT_ITEM save_iq_data;
	SHM_NM_OPAQUE_ITEM save_iqs;
	SHM_NM_OPAQUE_ITEM save_power;
	SHM_NM_OPAQUE_ITEM save_offset;
	SHM_NM_OPAQUE_ITEM save_phase;
	SHM_NM_OPAQUE_ITEM fpga_param;
	SHM_NM_OPAQUE_ITEM fpga_eeprom;
	SHM_NM_OPAQUE_ITEM scan_mode;
	//手咪
    SHM_NM_OPAQUE_ITEM center_module;
	SHM_NM_OPAQUE_ITEM gsm_module1;
	SHM_NM_OPAQUE_ITEM gsm_module2;
	SHM_NM_OPAQUE_ITEM radio_module;
	SHM_NM_OPAQUE_ITEM wireless_module;
	SHM_NM_OPAQUE_ITEM dsp_module;
	SHM_NM_OPAQUE_ITEM microphone_module1;
	SHM_NM_OPAQUE_ITEM microphone_module2;
	SHM_NM_OPAQUE_ITEM control_module;
}SHM_NM_STRU;


typedef struct{
    unsigned char nm_type;
	unsigned char src_id;
	unsigned char dst_id;
	unsigned short cmd_code;
	unsigned char op_code;
	unsigned char payload[NM_IPC_MSG_PAYLOAD_SIZE];
}__attribute__((packed,aligned(1)))NM_IPC_MSG;


typedef struct{
	unsigned int flag;
	unsigned int cmd_code;
	unsigned int op_code;
	unsigned char payload[NM_IPC_MSG_PAYLOAD_SIZE];
}MGRH_FPGA_MSG;


typedef struct { 
	long mtype;  
	NM_IPC_MSG ipc_msg;  
}QUEUE_MSG;


typedef struct{
	unsigned int calibration_power[60];
	unsigned int calibration_offset[30];
	unsigned int calibration_phase[51];
	unsigned int calibration_afc;
}EEPROM_FPGA_PARAM;


typedef struct{
	unsigned char mac_addr[6];
}EEPROM_ESN_PARAM;


typedef struct{
	EEPROM_FPGA_PARAM calibration_param;
	unsigned int scan_mode;
	unsigned int freq_offset;
	unsigned int freq;
	unsigned int power;
	unsigned int open_close_loop;
	unsigned int locking_time;
	unsigned int half_variance_threshold;
	unsigned int dev_call_timeout;
}INIT_FPGA_PARAM;


typedef struct{
    unsigned int freq;    //频率
    unsigned char power;   //功率
    unsigned char start_neighbor;   //开启临点
    unsigned short neighbor_period;  //临点周期
}__attribute__((packed,aligned(1)))NM_COMBINED_DATA;


typedef struct{
	unsigned short f1s1_half;
	unsigned short f1s2_half;
	unsigned short f2s1_half;
	unsigned short f2s2_half;
    unsigned int  u_half_rssi;
	unsigned char prohibit_transmit_flag;
	unsigned short data1;
	unsigned short data2;
	unsigned short data3;
	unsigned short data4;
	unsigned short data5;
	unsigned short data6;
    unsigned char reserve1[75];
	
	unsigned int vari_threshold;
	unsigned char lock_flag;
	unsigned char f1s1;
	unsigned char f1s2;
	unsigned char f2s1;
	unsigned char f2s2;
	unsigned char arm_downlink_lock;
	unsigned char f1s1_crc_flag;
	unsigned char f1s2_crc_flag;
	unsigned char f2s1_crc_flag;
	unsigned char f2s2_crc_flag;
	unsigned char reserve2[86];

    unsigned int lat_m_pout;
	unsigned int long_m_pout;
	unsigned char gps_lock;
	unsigned char gps_pps_out;
	unsigned char pos_e_w;
	unsigned char pos_n_s;
	unsigned char utc_en_out;
	unsigned char day_out;
	unsigned char hour_out;
	unsigned char lat_d_out;
	unsigned char lat_m_out;
	unsigned char long_d_out;
	unsigned char long_m_out;
	unsigned char minute_out;
	unsigned char second_out;
	unsigned char year_out;
	unsigned char reserve3[78];

	unsigned char downlink_dout;
	unsigned char freq_type_i;
	unsigned char frame_type;
	unsigned char trans_recv_sel; 
	unsigned char tx_on; 
	unsigned char rx_on;
	unsigned char lna_on;
	unsigned char pa_pwr_on; 
	unsigned char pa_ramp_en;
	unsigned char pa_tx_on;
	unsigned char tx_freq_cfg_en; 
	unsigned char freq_type_o; 
	unsigned char fpga_en_sync;
	unsigned char fpga_send_data_vld; 
	unsigned char fpga_tx_pwr_ctrl_vld;
	unsigned char arm_send_data_vld;
}FPGA_DEBUG_PRINT;



#endif
