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

#define TASK_NUM                    8

#define MONTH_PER_YEAR   12   // 一年12月
#define YEAR_MONTH_DAY   20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20   // 时分秒缓存大小
#define TASK_NAME   "daemon"

/* 初始值*/
#define SOFT_VERSION                 "V1.0.03.005"
#define FPGA_VERSION                 "V0.0.00.000"
#define DEFAULT_DEV_ID               0x00
#define DEFAULT_CC                   1
#define DEFAULT_LOCKING_TIME         500
#define DEFAULT_FREQ                 358500000
#define DEFAULT_POWER                5
#define DEFAULT_OPEN_CLOSE_LOOP      0
#define DEFAULT_START_NEIGHBOR       1
#define DEFAULT_HALF_VARIANCE        1500
#define DEFAULT_NEIGHBOR_PERIOD      1
#define DEFAULT_TERMINAL_CC          5
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
#define DEFAULT_CLOSE_TRAN_THRESHOLD 60
#define DEFAULT_RESUME_TRAN_THRESHOLD 70
#define DEFAULT_START_TEMP_ALARM     85
#define DEFAULT_CLOSE_TEMP_ALARM     80
#define DEFAULT_MOTO_MODE_SWITCH     1
#define DEFAULT_THRESHOLD_OPT_SWITCH     0
#define DEFAULT_NEIGHBOR_REPORT_AI     0
#define DEFAULT_WEB_CODE      "admin"
#define DEFAULT_WEB_USER      "admin"








#define GPIO_RUN_LED                 236  //旧板是196
#define GPIO_MCU_PWR_CTRL            210  //旧板是170
#define GPIO_PWR_KEY_DET             211  //旧板是171
#define GPIO_WD_EN                   217
#define GPIO_WD_WG                   218
#define CT_DEVICE_POWER_DOWN         0x36                      ///<  系统下电
#define SIG_SMS_SEND                 0x000e                    // 短消息命令
#define CC_SIG_HRD_LEN               28

#define CHECK_STATUS_OK                0
#define CHECK_STATUS_ERR               -1


typedef struct {
	pid_t pid;
	int	count;
	char * task_name;
}CHILD_TASK;

/**
 * @struct _CENTER_CMD_SHAREHEADER
 * @brief  接入单元与中心命令交互共用头
 */
typedef struct  _CENTER_CMD_SHARE_HEAD
{
    unsigned short SigHead;         // 标识AC接口信令头标志(0x13ec)
    unsigned short SigType;         // 信令类型-标识AC接口PTT命令信令（0x000a）
    unsigned short Datalength;      // 有效数据长度-标识信令有效数据长度（100）
    unsigned short SourceID;        // 源单元号-标识AC接口发送单元ID值
    unsigned short DestID;          // 目的单元号-标识AC接口接收单元ID值
    unsigned char  SourceType;      // 源单元设备类型-标识发送单元设备类型（制式）
    unsigned char  SigVersion;      // 信令版本号-标识该信令适合版本号0x02(该信令与0x01版本不兼容)
    unsigned char  PathNum;         // 通道号码-标识该信令属于该单元的通道号码，若不区分通道，则填全F
    unsigned char  Exhead[11];      // 扩展头-扩展头中心无须关心参数
    unsigned int   CheckVal;        // 校验值-标识信令校验结果(01版本 与02版本 目前都填写全F，标示无校验方式)
} CENTER_CMD_SHARE_HEAD ;

/**
 * @struct SMS_CENTER_CCL_DL
 * @brief  中心发送短消息到CCL 接口
 */
typedef struct _SMS_CENTER_CCL_DL
{
    CENTER_CMD_SHARE_HEAD  SharedHead;
    unsigned int   CallId;          // 控制面ID 标识一次呼叫的控制面ID
    unsigned int   VoiceId;         // 业务面ID 标识一次呼叫的业务面ID
    unsigned char  SourceStat;      // 标识发送单元设备状态（空闲、故障、忙等）
    unsigned char  SmsType ;        // 短消息类型（组呼、个呼），增加组呼状态消息和个呼状态消息
    unsigned short ValidLength;     // 短消息有效长度（状态消息为2，短消息内容填2字节状态消息号码）
    unsigned char  SmsFormat  ;     // 短消息格式（ASCII=0x00、UNICODE=0x01等）
    unsigned char  SenderNum[30];   // 发送者号码
    unsigned char  ReceiverNum[30]; // 接收者号码
    unsigned char  SmsData[512];    // 短消息内容
    unsigned char  Reserved[3];
} SMS_CENTER_CCL_DL;


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
