/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    mgr_common.h
 * @brief   网管业务相关枚举、结构
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
 *   *************************************************************************/

#ifndef __MGR_COMMON_H__
#define __MGR_COMMON_H__

/******************************************************************************
 *   宏定义
 *   *************************************************************************/
/**
 * @def NAS_CONFIG_FILE
 * @brief 配置文件
 */
#define NAS_CONFIG_FILE 	      "/loadapp/nas_config.ini"
/**
 * @def FTOK_F_NAME
 * @brief 用于获取共享内存和信号量IPC键值的文件名
 */
#define FTOK_F_NAME               "/"
/** * @def SHM_CFG_ITEM_NAME_LEN
 * @brief 共享内存配置结构名称大小
 */
#define SHM_CFG_ITEM_NAME_SIZE    20
/**
 * @def SHM_CFG_ITEM_NAME_LEN
 * @brief 共享内存配置结构字符串类型buff大小
 */
#define SHM_CFG_ITEM_STRBUF_SIZE  50


/******************************************************************************
 *   枚举声明
 *   *************************************************************************/
/**
 * @enum FTOK_ID
 * @brief 获取IPC键值的id
 */
typedef enum {
    FTOK_ID_SHM_CFG     = 1,     ///< 配置区共享内存
    FTOK_ID_SEM_CFG     = 2,     ///< 配置区信号量
    FTOK_ID_SHM_NM      = 3,     ///< 网管区共享内存
    FTOK_ID_SEM_NM      = 4,     ///< 网管区信号量
    FTOK_ID_SHM_IPC     = 5,     ///< 进程间通信共享内存
    FTOK_ID_SEM_IPC     = 6,     ///< 进程间通信信号量
    ROUTE_SEM_ID        = 7,     ///< 配置模块使用
    ROUTE_MSG_SERVER_ID = 8,     ///< 配置模块使用
    LOG_MSG_ID = 9,     ///<
    CLI_MSG_ID = 10,     ///<命令行注册
    FTOK_ID_Q_MGRH      = 101,   ///< 本地网管处理模块消息队列
    FTOK_ID_Q_MGRA_ACK  = 102    ///< 本地网管处理模块消息队列
}FTOK_ID;


/**
 * @enum DEV_ID
 * @brief 设备ID
 */
typedef enum {
    DEV_ID_1  = 0x00,
    DEV_ID_2  = 0x01,
    DEV_ID_3  = 0x02,
    DEV_ID_4  = 0x03,
    DEV_ID_5  = 0x04,
    DEV_ID_6  = 0x05,
    DEV_ID_7  = 0x06,
    DEV_ID_8  = 0x07,
    DEV_ID_9  = 0x08,
    DEV_ID_10 = 0x09,
    DEV_ID_11 = 0x0A,
    DEV_ID_12 = 0x0B,
    DEV_ID_13 = 0x0C,
    DEV_ID_14 = 0x0D,
    DEV_ID_15 = 0x0E,
    DEV_ID_16 = 0x0F,
    DEV_ID_17 = 0x10,
    DEV_ID_18 = 0x11,
    DEV_ID_19 = 0x12,
    DEV_ID_20 = 0x13,
    DEV_ID_21 = 0x14,
    DEV_ID_22 = 0x15,
    DEV_ID_23 = 0x16,
    DEV_ID_24 = 0x17,
    DEV_ID_25 = 0x18,
    DEV_ID_26 = 0x19,
    DEV_ID_27 = 0x1A,
    DEV_ID_28 = 0x1B,
    DEV_ID_29 = 0x1C,
    DEV_ID_30 = 0x1D,
    DEV_ID_31 = 0x1E,
    DEV_ID_32 = 0x1F
}DEV_ID;


/**
 * @enum SOCK_PORT
 * @brief 进程间通信端口号
 */
typedef enum {
    SOCK_PORT_CC_V_SEND  = 10000,
    SOCK_PORT_CC_V_RECV  = 10001,
    SOCK_PORT_CC_S_SEND  = 10002,
    SOCK_PORT_CC_S_RECV  = 10003,

    SOCK_PORT_CCL_CC_V   = 10010,
    SOCK_PORT_CC_CCL_V   = 10011,
    SOCK_PORT_CCL_CC_S   = 10012,
    SOCK_PORT_CC_CCL_S   = 10013,
    SOCK_PORT_MGRA_CC    = 10014,
    SOCK_PORT_CC_MGRA    = 10015,

    SOCK_PORT_CCL_DLL    = 10020,
    SOCK_PORT_DLL_CCL    = 10021,

    SOCK_PORT_DLL_ADP    = 10030,
    SOCK_PORT_ADP_DLL    = 10031,

    SOCK_PORT_MGR_ADP    = 10040,
    SOCK_PORT_ADP_MGRA   = 10041,
    SOCK_PORT_MGRA_MGRH  = 10042,
    SOCK_PORT_MGRH_MGRA  = 10043,
    SOCK_PORT_ADP_MGRH   = 10044,
    SOCK_PORT_CLI_INPUT  =  10050,
    SOCK_PORT_LOG_PRINT  =  10051
}SOCK_PORT;


/**
 * @enum NM_CMD_CODE
 * @brief 网管消息命令字
 */
typedef enum {
    //网管空口命令
    CMD_CODE_DEV_FREQ                = 0x0001,
    CMD_CODE_DEV_POWER               = 0x0002,
    CMD_CODE_START_NEIGHBOR          = 0x0003,
    CMD_CODE_NEIGHBOR_PERIOD         = 0x0004,
    CMD_CODE_COMBINED_DATA           = 0x0005,
    CMD_CODE_SCAN_MODE               = 0x0006,  //以前命令字是0x0006，暂时没用后期开发注意修改
    CMD_CODE_CENTER_QUERY_ALARM      = 0x0007,
    CMD_CODE_BIG_POWER_TRANSMIT      = 0x0008,

    //告警模块命令字
     CMD_CODE_ALARM                  = 0x0060,


    //业务模块命令字C0~FF
    CMD_CODE_GPS_REQUEST             = 0x00C0,
    CMD_CODE_STUN                    = 0x00C1,
    CMD_CODE_KILL                    = 0x00C2,
    CMD_CODE_ENABLE                  = 0x00C3,
    CMD_CODE_NER_QUERY               = 0x00C4,
    CMO_CODE_NER_REPORT              = 0x00C5,
    CMO_CODE_MS_GPS                  = 0x00C6,
    CMO_CODE_MS_DISABlE              = 0x00C7,
    CMO_CODE_MS_ENABLE               = 0x00C8,
    CMO_CODE_NAS_PRE                 = 0x00C9,
    //网管本地命令
    CMD_CODE_OPEN_CLOSE_LOOP         = 0x0100,
    CMD_CODE_LOCKING_TIME            = 0x0101,
    CMD_CODE_HALF_VARIANCE_THRESHOLD = 0x0102,
    CMD_CODE_DEV_ID                  = 0x0103,
    CMD_CODE_LOCAL_CC                = 0x0104,
    CMD_CODE_VERSION                 = 0x0105,
    CMD_CODE_FPGA_VERSION            = 0x0106,
    CMD_CODE_TERMINAL_CC             = 0x0107,
    CMD_CODE_PROTOCOL_MODE           = 0x0108,
    CMD_CODE_IS_CONNET_CC            = 0x0109,
    CMD_CODE_AIR_SUBNET_ID           = 0x010A,
    CMD_CODE_LOCAL_IP                = 0x010B,
    CMD_CODE_LOCAL_MAC               = 0x010C,
    CMD_CODE_WORK_TIME_OUT           = 0x010F,
    CMD_CODE_GPS_TIME_OUT            = 0x0110,
    CMD_CODE_STUN_TIME_OUT           = 0x0111,
    CMD_CODE_START_TIME_OUT          = 0x0112,
    CMD_CODE_FPGA_EEPROM             = 0x0113,
    CMD_CODE_REBOOT                  = 0x0114,
    CMD_CODE_ALARM_SWITCH            = 0x0115,
    CMD_CODE_UPDATE_DTB              = 0x0116,
    CMD_CODE_UPDATE_LOADAPP          = 0x0117,
    CMD_CODE_UPDATE_FILE_SYSTEM      = 0x0118,
    CMD_CODE_UPDATE_UBOOT            = 0x0119,
    CMD_CODE_UPDATE_RBF              = 0x011a,
    CMD_CODE_UPDATE_ZIMAGE           = 0x011b,
    //FPGA调测命令
    CMD_CODE_EMISSIVE_VCO_FREQ       = 0x0A01,
    CMD_CODE_RECEIVING_VCO_FREQ      = 0x0A02,
    CMD_CODE_SECOND_LOCAL_OSCILLATOR = 0x0A03,
    CMD_CODE_POWER_AMPLIFIER_SWITCH  = 0x0A04,
    CMD_CODE_FPGA_PARAM              = 0x0A05,
    CMD_CODE_POWER                   = 0x0A06,
    CMD_CODE_AFC                     = 0x0A07,
    CMD_CODE_SAVE_AFC                = 0x0A08,
    CMD_CODE_IQS                     = 0x0A09,
    CMD_CODE_SAVE_IQS                = 0x0A0A,
    CMD_CODE_OFFSET                  = 0x0A0B,
    CMD_CODE_SAVE_OFFSET             = 0x0A0C,
    CMD_CODE_PHASE                   = 0x0A0D,
    CMD_CODE_SAVE_PHASE              = 0x0A0E,
    CMD_CODE_CALIBRATE_OFFSET        = 0x0A0F,
    CMD_CODE_CALIBRATE_PHASE         = 0x0A10,
    CMD_CODE_DEBUG_EMISSIVE_FREQ     = 0x0A11,
    CMD_CODE_SAVE_POWER              = 0x0A12,
    CMD_CODE_ERROR_RATE              = 0x0A13,
    CMD_CODE_START_DEBUG             = 0x0A14,
    CMD_CODE_START_ERR_RATE          = 0x0A15,
    CMD_CODE_SELECT_SIGNAL_SOURCE    = 0x0A16,
    CMD_CODE_DEBUG_OPEN_CLOSE_LOOP   = 0x0A17,
    CMD_CODE_DEBUG_POWER             = 0x0A18,
    CMD_CODE_SLOT_CONTROL            = 0x0A19,
    CMD_CODE_VGS                     = 0x0A1A,
    CMD_CODE_SAVE_EEPROM             = 0x0A1B,
    CMD_CODE_SET_981                 = 0x0A1C,
    CMD_CODE_QUERY_981               = 0x0A1D,
    CMD_CODE_SET_998                 = 0x0A1E,
    CMD_CODE_QUERY_998               = 0x0A1F,
    CMD_CODE_DEV_CALL_TIMEOUT        = 0x0A20,
    CMD_CODE_ERROR_RATE_FREQ         = 0x0A21,
    CMD_CODE_SAVE_IQ_DATA            = 0x0A22,
    CMD_CODE_CLOSE_TRAN_THRESHOLD    = 0x0A23,
    CMD_CODE_START_TEMP_ALARM        = 0x0A24,
    CMD_CODE_CLOSE_TEMP_ALARM        = 0x0A25,
    CMD_CODE_RESUME_TRAN_THRESHOLD   = 0x0A26,
    CMD_CODE_MOTO_SWITCH             = 0x0A27,
    CMD_CODE_THRESHOLD_OPT_SWITCH    = 0x0A28,
    CMD_CODE_NEIGHBOR_REPORT_SWITCH  = 0x0A29,
    CMD_CODE_FREQ_CHANNEL            = 0x0A2A,
    //手咪命令
    CMD_CODE_CENTER_MODULE           = 0x0B01,
    CMD_CODE_GSM_MODULE1             = 0x0B02,
    CMD_CODE_GSM_MODULE2             = 0x0B03,
    CMD_CODE_RADIO_MODULE            = 0x0B04,
    CMD_CODE_WIRELESS_MODULE         = 0x0B05,
    CMD_CODE_DSP_MODULE              = 0x0B06,
    CMD_CODE_MICROPHONE_MODULE1      = 0x0B07,
    CMD_CODE_MICROPHONE_MODULE2      = 0x0B08,
    CMD_CODE_CONTROL_MODULE          = 0x0B09,
    CMD_CODE_CENTER_PRINT            = 0x0B0A,
    CMD_CODE_CENTER_INFO             = 0x0B0B
}NM_CMD_CODE;



/**
 * @enum NM_OP_CODE
 * @brief 网管消息操作码
 */
typedef enum {
    OP_CODE_GET     = 0x00,  ///< 读取操作
    OP_CODE_GET_ACK = 0x01,  ///< 读取响应
    OP_CODE_SET     = 0x02,  ///< 设置操作
    OP_CODE_SET_ACK = 0x03,  ///< 设置响应
    OP_CODE_ALARM   = 0x04,  ///< 普通告警
    OP_CODE_E_ALARM = 0x05   ///< 紧急告警
}NM_OP_CODE;


/**
 * @enum NM_TYPE
 * @brief 网管类型
 */
typedef enum {
    NM_TYPE_NM     = 0x00,
    NM_TYPE_CENTER = 0x01
}NM_TYPE;


/**
 * @union SEMUN
 * @brief 信号量联合体
 */
typedef union {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}SEMUN;


/**
 * @struct SHM_CFG_ITEM_HEAD
 * @brief 配置区共享内存配置项结构头
 */
typedef struct {
    unsigned short id;
    char name[SHM_CFG_ITEM_NAME_SIZE];
    unsigned short len;
}__attribute__((packed,aligned(1)))SHM_CFG_ITEM_HEAD;


/**
 * @struct SHM_CFG_IP_ITEM
 * @brief 配置区共享内存IP类型配置项结构
 */
typedef struct {
    unsigned short id;
    char name[SHM_CFG_ITEM_NAME_SIZE];
    unsigned short len;
    struct in_addr addr;
}__attribute__((packed,aligned(1)))SHM_CFG_IP_ITEM;


/**
 * @struct SHM_CFG_UINT_ITEM
 * @brief 配置区共享内存无符号整型配置项结构
 */
typedef struct {
    unsigned short id;
    char name[SHM_CFG_ITEM_NAME_SIZE];
    unsigned short len;
    unsigned int val;
}__attribute__((packed,aligned(1)))SHM_CFG_UINT_ITEM;


/**
 * @struct SHM_CFG_STRING_ITEM
 * @brief 配置区共享内存字符串型配置项结构
 */
typedef struct {
    unsigned short id;
    char name[SHM_CFG_ITEM_NAME_SIZE];
    unsigned short len;
    char buf[SHM_CFG_ITEM_STRBUF_SIZE];
}__attribute__((packed,aligned(1)))SHM_CFG_STRING_ITEM;


/**
 * @struct SHM_CFG_STRU
 * @brief 配置区共享内存整体结构
 */
typedef struct {
    SHM_CFG_STRING_ITEM s_version;    //EPACK版本号
    SHM_CFG_STRING_ITEM fpga_version; //FPGA版本号
    SHM_CFG_UINT_ITEM dev_id;         //链路机ID
    SHM_CFG_UINT_ITEM cc;             //链路机色码
    SHM_CFG_UINT_ITEM locking_time;   //业务挂起时间
    SHM_CFG_UINT_ITEM freq;           //频点
    SHM_CFG_UINT_ITEM power;          //功率
    SHM_CFG_UINT_ITEM open_close_loop;            //锁相环
    SHM_CFG_UINT_ITEM start_neighbor;             //临点开关
    SHM_CFG_UINT_ITEM half_variance_threshold;    //半帧方差门限
    SHM_CFG_UINT_ITEM neighbor_period;            //临点周期
    SHM_CFG_UINT_ITEM neighbor_report_ai;            //邻点上报开关
    SHM_CFG_UINT_ITEM terminal_cc;                //终端色码
    SHM_CFG_UINT_ITEM protocol_mode;              //工作模式
    SHM_CFG_UINT_ITEM air_subnet_id;              //空口子网ID
    SHM_CFG_STRING_ITEM local_ip;                   //链路机IP
    SHM_CFG_UINT_ITEM stun_flag;                  //遥晕标记
    SHM_CFG_UINT_ITEM kill_flag;                  //遥毙标记
    SHM_CFG_STRING_ITEM subnet_mask;              //子网掩码
    SHM_CFG_STRING_ITEM gateway;                  //默认网关
    SHM_CFG_UINT_ITEM work_time_out;              //设备业务超时时间
    SHM_CFG_UINT_ITEM gps_time_out;              //终端GPS超时时间
    SHM_CFG_UINT_ITEM stun_time_out;              //终端摇晕超时时间
    SHM_CFG_UINT_ITEM start_time_out;              //终端激活超时时间
    SHM_CFG_UINT_ITEM dev_call_timeout;            //设备通话超时时间
    SHM_CFG_UINT_ITEM scan_mode;                  //扫描模式开关
    SHM_CFG_UINT_ITEM freq_offset;               //扫描模式频点偏移
    SHM_CFG_UINT_ITEM alarm_switch_status;       //告警上报空口开关
    SHM_CFG_UINT_ITEM close_transmit_threshold;              //关闭发射门限
    SHM_CFG_UINT_ITEM resume_transmit_threshold;              //恢复发射门限
    SHM_CFG_UINT_ITEM tempratue_alarm_start_threshold;       //温度告警触发门限
    SHM_CFG_UINT_ITEM tempratue_alarm_close_threshold;       //温度告警退出门限
    SHM_CFG_UINT_ITEM moto_mode_switch;                     //摩托模式和正常模式切换开关
    SHM_CFG_UINT_ITEM threshold_opt_switch;                //方差优化开关
    SHM_CFG_UINT_ITEM stop_tans;                //大功率禁止转发开关
    SHM_CFG_UINT_ITEM boot_mode;                //开机方式
	SHM_CFG_UINT_ITEM reboot_strategy;                //复位策略
    SHM_CFG_STRING_ITEM web_code;                   //web密码
    SHM_CFG_STRING_ITEM web_user;                   //web用户名
}__attribute__((packed,aligned(1)))SHM_CFG_STRU;

///////////////////告警相关////////////////////
/**
 * @enum ALARM_TYPE
 * @brief 网管消息操作码
 */
typedef enum _ALARM_TYPE{
    ALARM_TYPE_NORMAL     = 0x00,  ///< 读取操作
    ALARM_TYPE_E    = 0x01///< 读取操作
}ALARM_TYPE;
/**
 * @struct ALARM_ITEM
 * @brief 告警表项
 */

typedef struct alarm_item_data
{
    unsigned char  uStatus;
    unsigned char alm_cnt;
    unsigned char alm_exist;
    unsigned char alm_code;
    unsigned char alm_clear;
    unsigned char alm_sended_center_flag;
    unsigned char alm_reset_send_center_num;
    unsigned char alm_sended_ai_flag;
    unsigned char alm_reset_send_ai_num;
    unsigned char alm_eflag;
    unsigned int    value;
} __attribute__((packed,aligned(1)))ALARM_ITEM;

/**
* @brief    告警状态无告警
*/
#define DEF_ALM_NORMAL  0   //正常
/**
* @brief    告警状态有告警
*/
#define DEF_ALM_OCCUR               1   //告警发生

 /**
 * @def MGR_ALARM_MAX
 * @brief告警最多字节数(8*8-4)
 */
#define MGR_ALARM_BYTE_MAX        8

/**
 * @def MGR_ALARM_MAX
 * @brief告警数(8*8)
 */
#define MGR_ALARM_MAX         60

/** *
@def MGR_ALARM_CENTER_NUM
* @brief 中心告警个数
*/
#define MGR_ALARM_CENTER_NUM          24

/** * @def MGR_ALARM_FPGA_NUM
* @brief 中心告警个数
*/
#define MGR_ALARM_FPGA_NUM       5
/** *
@def MGR_ALARM_SERVER_NUM
* @brief 中心告警个数
*/
#define MGR_ALARM_SERVER_NUM          1

/** * @struct MGR_ALARM_CENTER_TYPE_E
 * @brief 告警类型
 */
typedef enum _MGR_ALARM_TYPE
{
    MGR_ALARM_CENTER_1                     = 0x0001,//电池电量告警
    MGR_ALARM_CENTER_2                     = 0x0002,//电池通信异常
    MGR_ALARM_CENTER_3                     = 0x0003,//电池模块预留
    MGR_ALARM_CENTER_4                     = 0x0004,//电池模块预留
    MGR_ALARM_CENTER_5                     = 0x0005, //GSM模块告警（SIM卡未插入）
    MGR_ALARM_CENTER_6                     = 0x0006,//GSM模块预留
    MGR_ALARM_CENTER_7                     = 0x0007,//GSM模块预留
    MGR_ALARM_CENTER_8                     = 0x0008,//GSM模块预留
    MGR_ALARM_CENTER_9                     = 0x0009,//DSP故障告警
    MGR_ALARM_CENTER_10                    = 0x000a,//dSP预留
    MGR_ALARM_CENTER_11                    = 0x000b,//模拟信道机未连接告警
    MGR_ALARM_CENTER_12                    = 0x000c,//模拟信道机预留
    MGR_ALARM_CENTER_13                    = 0x000d,//无线链路预留
    MGR_ALARM_CENTER_14                    = 0x000e,//无线链路预留
    MGR_ALARM_CENTER_15                    = 0x000f,  //网管预留
    MGR_ALARM_CENTER_16                    = 0x0010,//网管预留
    MGR_ALARM_CENTER_17                    = 0x0011,//互联交互预留
    MGR_ALARM_CENTER_18                    = 0x0012,//互联交互预留
    MGR_ALARM_CENTER_19                    = 0x0013,//手咪预留
    MGR_ALARM_CENTER_20                    = 0x0014, //手咪预留
    MGR_ALARM_CENTER_21                    = 0x0015,//CODEC预留
    MGR_ALARM_CENTER_22                    = 0x0016,//CODEC预留
    MGR_ALARM_CENTER_23                    = 0x0017,//主控预留
    MGR_ALARM_CENTER_24                    = 0x0018,//守护进程预留

    MGR_ALARM_FPGA_1                       = 0x0019,//GPS失锁
    MGR_ALARM_FPGA_2                       = 0x001a,//功放过热
    MGR_ALARM_FPGA_3                       = 0x001b,//笛卡尔环甭快
    MGR_ALARM_FPGA_4                       = 0x001c,///读DSP标志位异常
    MGR_ALARM_FPGA_5                       = 0x001d,//DSP上行数据校验失败
    MGR_ALARM_SERVER_1                     = 0x001e//链路断链
} MGR_ALARM_TYPE;

/**
 * @struct SHM_IPC_STRU
 * @brief 进程间通信共享内存整体结构
 */
typedef struct {
    unsigned int print_mark;
    unsigned int is_connect_cc;
    ALARM_ITEM alarm_struct[MGR_ALARM_MAX];//
    unsigned char ccl_printf[8];
    unsigned char dll_printf[16];
    unsigned char Err_printf[4];
    unsigned char route_printf[7];
    unsigned char printf_select;//默认0为工具输出，1为本地输出
    unsigned char mgr_printf[8];
    unsigned char fpga_debug_sleep;
    unsigned char update_dtb_result;
    unsigned char update_rbf_result;
    unsigned char update_boot_result;
    unsigned char update_ram_result;
    unsigned char update_zimage_result;
    unsigned int  freq_channel;
    unsigned char modify_web_info_flag;
    unsigned char web_old_user[30];
    unsigned char web_old_code[30];
    unsigned char clear_update_status;
}__attribute__((packed,aligned(1)))SHM_IPC_STRU;





/**
 * @struct NAS_AI_PAYLOAD
 * @brief NAS空口负载结构
 */
typedef struct{
    unsigned char cmd_code;   ///< 命令字
    unsigned char src_id:5;   ///< 源ID
    unsigned char nm_type:3;  ///< 网管类型
    unsigned char dst_id:5;   ///< 目的ID
    unsigned char op_code:3;  ///< 操作码
    unsigned char data[8];    ///< 数据
    unsigned char crc;        ///< 校验
}NAS_AI_PAYLOAD;


/**
 * @struct ALARM_SEND_ITEM_PAYLOAD
 * @brief 告警数据包单字节结构
 */
typedef struct{
    unsigned char bit_0:1;
    unsigned char bit_1:1;
    unsigned char bit_2:1;
    unsigned char bit_3:1;
    unsigned char bit_4:1;
    unsigned char bit_5:1;
    unsigned char bit_6:1;
    unsigned char bit_7:1;
}ALARM_SEND_ITEM_PAYLOAD;


#endif
