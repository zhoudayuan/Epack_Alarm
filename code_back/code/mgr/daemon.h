/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    daemon.h
 * @brief   �ػ�����ͷ�ļ�
 * @author  �����
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   �޸���ʷ
 *   2016-8-1                �����                     �����ļ�
****************************************************************************/


#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "mgr.h"
#include "gpio_interface.h"

#define TASK_NUM                    8

#define MONTH_PER_YEAR   12   // һ��12��
#define YEAR_MONTH_DAY   20   // �����ջ����С
#define HOUR_MINUTES_SEC 20   // ʱ���뻺���С
#define TASK_NAME   "daemon"

/* ��ʼֵ*/
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








#define GPIO_RUN_LED                 236  //�ɰ���196
#define GPIO_MCU_PWR_CTRL            210  //�ɰ���170
#define GPIO_PWR_KEY_DET             211  //�ɰ���171
#define GPIO_WD_EN                   217
#define GPIO_WD_WG                   218
#define CT_DEVICE_POWER_DOWN         0x36                      ///<  ϵͳ�µ�
#define SIG_SMS_SEND                 0x000e                    // ����Ϣ����
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
 * @brief  ���뵥Ԫ���������������ͷ
 */
typedef struct  _CENTER_CMD_SHARE_HEAD
{
    unsigned short SigHead;         // ��ʶAC�ӿ�����ͷ��־(0x13ec)
    unsigned short SigType;         // ��������-��ʶAC�ӿ�PTT�������0x000a��
    unsigned short Datalength;      // ��Ч���ݳ���-��ʶ������Ч���ݳ��ȣ�100��
    unsigned short SourceID;        // Դ��Ԫ��-��ʶAC�ӿڷ��͵�ԪIDֵ
    unsigned short DestID;          // Ŀ�ĵ�Ԫ��-��ʶAC�ӿڽ��յ�ԪIDֵ
    unsigned char  SourceType;      // Դ��Ԫ�豸����-��ʶ���͵�Ԫ�豸���ͣ���ʽ��
    unsigned char  SigVersion;      // ����汾��-��ʶ�������ʺϰ汾��0x02(��������0x01�汾������)
    unsigned char  PathNum;         // ͨ������-��ʶ���������ڸõ�Ԫ��ͨ�����룬��������ͨ��������ȫF
    unsigned char  Exhead[11];      // ��չͷ-��չͷ����������Ĳ���
    unsigned int   CheckVal;        // У��ֵ-��ʶ����У����(01�汾 ��02�汾 Ŀǰ����дȫF����ʾ��У�鷽ʽ)
} CENTER_CMD_SHARE_HEAD ;

/**
 * @struct SMS_CENTER_CCL_DL
 * @brief  ���ķ��Ͷ���Ϣ��CCL �ӿ�
 */
typedef struct _SMS_CENTER_CCL_DL
{
    CENTER_CMD_SHARE_HEAD  SharedHead;
    unsigned int   CallId;          // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int   VoiceId;         // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char  SourceStat;      // ��ʶ���͵�Ԫ�豸״̬�����С����ϡ�æ�ȣ�
    unsigned char  SmsType ;        // ����Ϣ���ͣ���������������������״̬��Ϣ�͸���״̬��Ϣ
    unsigned short ValidLength;     // ����Ϣ��Ч���ȣ�״̬��ϢΪ2������Ϣ������2�ֽ�״̬��Ϣ���룩
    unsigned char  SmsFormat  ;     // ����Ϣ��ʽ��ASCII=0x00��UNICODE=0x01�ȣ�
    unsigned char  SenderNum[30];   // �����ߺ���
    unsigned char  ReceiverNum[30]; // �����ߺ���
    unsigned char  SmsData[512];    // ����Ϣ����
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
