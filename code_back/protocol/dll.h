#ifndef __DLL_H__
#define __DLL_H__

/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    dll.h
 * @brief   包含数据链路层协议全局类型、变量、结构、宏定义
 * @author  陈禹良
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   修改历史
 *   2016-7-20                 陈禹良                     建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "sap.h"


/******************************************************************************
 *   宏定义
 *   *************************************************************************/
/** @defgroup 变量类型定义 */
/** @{ */
//typedef char           INT8;    /*  1                   1   */
//typedef unsigned char  UINT8;   /*  1                   1   */
//typedef signed short   INT16;   /*  2                   2   */
//typedef unsigned short UINT16;  /*  2                   2   */
//typedef signed long    INT32;   /*  4                   4   */
//typedef unsigned long  UINT32;  /*  4                   4   */

#define _F_                      __FUNCTION__

//GPS 同步时间250*60 ms=15000
#define SYNC_CYC                 250

/** @defgroup  呼叫等待时间定义 */
/** @{ */
#define CALL_VOICE_WAIT          ((UINT16)0x0006)
#define CALL_DATA_WAIT           ((UINT16)0x0006)
#define CALL_NAS_WAIT            ((UINT16)0x0006)
/** @} */

/** @defgroup  FPGA呼叫状态定义 */
/** @{ */
#define SERVR_IDLE               ((UINT8)0)
#define VOICE_RELY               ((UINT8)1)
#define MESSG_RELY               ((UINT8)2)
#define CALLI_SPON               ((UINT8)4)
/** @} */

/** @defgroup  业务状态定义 */
/** @{ */
#define WORK_START               ((UINT8)0)
#define WORK_STOP                ((UINT8)1)

/** @defgroup  打印状态定义 */
/** @{ */
#define PRINT_CLOSE               ((UINT8)0)
#define PRINT_DEBUG               ((UINT8)1)
#define PRINT_MEMRY               ((UINT8)2)
#define PRINT_ALL                 ((UINT8)3)
/** @} */

/** @defgroup 空口数据参数 */
/** @{ */
// DMR/PDT CRC MASK 掩码
#define DMR_CRC_MASK_PI          ((UINT16)0x6969)
#define DMR_CRC_MASK_CSBK        ((UINT16)0xA5A5)
#define DMR_CRC_MASK_MBC         ((UINT16)0xAAAA)
#define DMR_CRC_MASK_DATA        ((UINT16)0xCCCC)
#define DMR_CRC_MASK_R12         ((UINT16)0x00F0)
#define DMR_CRC_MASK_R34         ((UINT16)0x01FF)
#define DMR_CRC_MASK_R11         ((UINT16)0x010F)

#define PDT_CRC_MASK_PI          ((UINT16)0x89AA)
#define PDT_CRC_MASK_CSBK        ((UINT16)0xF150)
#define PDT_CRC_MASK_MBC         ((UINT16)0xD8ED)
#define PDT_CRC_MASK_DATA        ((UINT16)0x785E)
#define PDT_CRC_MASK_R12         ((UINT16)0x0179)
#define PDT_CRC_MASK_R34         ((UINT16)0x0075)
#define PDT_CRC_MASK_R11         ((UINT16)0x018E)

// 短数据类型
#define DPF_UDT_PACKET           ((UINT8)0)    ///< Unified Data Transport (UDT), UDP数据
#define DPF_RESPONSE_PACKET      ((UINT8)1)    ///< Response packet, 相应分组(PDT常规使用)
#define DPF_UNCONFIRM_PACKET     ((UINT8)2)    ///< Data packet with unconfirmed delivery  无确认数据分组
#define DPF_CONFIRM_PACKET       ((UINT8)3)    ///< Data packet with confirmed delivery   有确认数据分组
#define DPF_DEFINED_SD_PACKET    ((UINT8)13)   ///< Short Data: Defined  预定义短数据
#define DPF_RAW_SD_PACKET        ((UINT8)14)   ///< Short Data: Raw or Status/Precoded  原始数据或预定义状态数据
#define DPF_PROP_DATA_PACKET     ((UINT8)15)   ///< Proprietary Data Packet  专有数据分组
#define DPF_PDT_E2E_ENCRYPT      ((UINT8)4)    ///< PDT end-to-end encrypt  端到端加密数据头

// FID类型
#define SFID                     ((UINT8)0x00)  //standard
#define HFID                     ((UINT8)0x68)  //hytera
#define MFID                     ((UINT8)0x10)  //moto
#define PFID                     ((UINT8)0x08)  //pdt

// 分组数据状态
#define PDP_STATE_IDLE           ((UINT8)0x00)
#define PDP_STATE_OVER           ((UINT8)0x01)
#define PDP_STATE_FIRST          ((UINT8)0x02)
#define PDP_STATE_RETRY          ((UINT8)0x03)

// 分组数据长度定义
#define R12_NC_DATA_DLEN         ((UINT8)12)
#define R12_NC_DATA_ULEN         ((UINT8)12)
#define R34_NC_DATA_DLEN         ((UINT8)18)
#define R34_NC_DATA_ULEN         ((UINT8)18)
#define R11_NC_DATA_DLEN         ((UINT8)24)
#define R11_NC_DATA_ULEN         ((UINT8)24)
#define R12_C_DATA_DLEN          ((UINT8)12)
#define R12_C_DATA_ULEN          ((UINT8)12)
#define R34_C_DATA_DLEN          ((UINT8)18)
#define R34_C_DATA_ULEN          ((UINT8)18)
#define R11_C_DATA_DLEN          ((UINT8)24)
#define R11_C_DATA_ULEN          ((UINT8)24)
#define R12_NC_BLOCK_MAX         ((UINT8)127)
#define R34_NC_BLOCK_MAX         ((UINT8)100)
#define R11_NC_BLOCK_MAX         ((UINT8)75)
#define R12_C_BLOCK_MAX          ((UINT8)127)
#define R34_C_BLOCK_MAX          ((UINT8)100)
#define R11_C_BLOCK_MAX          ((UINT8)75)
#define DATA_BLOCK_MAX           ((UINT8)127)
#define UDT_BLACK_MAX            ((UINT8)5)
#define UDT_DATA_DLEN            ((UINT8)12)
#define UDT_DATA_ULEN            ((UINT8)12)
#define MBC_BLACK_MAX            ((UINT8)4)
#define MBC_DATA_DLEN            ((UINT8)12)
#define MBC_DATA_ULEN            ((UINT8)12)

// 数据长度
#define RC_LEN                   ((UINT8)2)
#define NER_LEN                  ((UINT8)4)
#define GPS_LEN                  ((UINT8)8)
#define FLC_LEN                  ((UINT8)9)
#define SLC_LEN                  ((UINT8)4)
#define EMB_LEN                  ((UINT8)6)
#define PI_LEN                   ((UINT8)10)
#define CSBK_LEN                 ((UINT8)10)
#define MBC_HEADER_LEN           ((UINT8)10)
#define MBC_CONTINUATION_LEN     ((UINT8)12)
#define MBC_TAIL_LEN             ((UINT8)10)
#define MBC_2_BLOCKS_LEN         ((UINT8)20)
#define MBC_3_BLOCKS_LEN         ((UINT8)32)
#define MBC_4_BLOCKS_LEN         ((UINT8)44)
#define DATA_HEADER_LEN          ((UINT8)10)
#define DATA_TAIL_LEN            ((UINT8)10)
#define DATA_BODY_LEN            ((UINT8)12)
#define UDT_2_BLOCKS_LEN         ((UINT8)20)
#define UDT_3_BLOCKS_LEN         ((UINT8)32)
#define UDT_4_BLOCKS_LEN         ((UINT8)44)
#define UDT_5_BLOCKS_LEN         ((UINT8)56)
#define VOICE_LEN                ((UINT8)27)
/** @} */

/** @defgroup CSBK命令码 */
/** @{ */
#define PRE_CSBKO               0x3d  // 预载波
#define REQ_CSBKO               0x29  // Common Signalling Request CSBKO
#define ACK_CSBKO               0x2c  // Common Signalling Answer Response CSBKO
#define ALARM_CSBKO             0x37  // Digital Alarm Service Request CSBKO
/** @} */

/** @defgroup 补充业务命令码 */
/** @{ */
#define CALL_ALERT_SSO          0x05  // Call Alert Service
#define EN_RADIO_SSO            0x09  // Radio Enable Service
#define DIS_RADIO_SSO           0x0a  // Radio Disable Service
#define DIGITAL_ALARM_SSO       0x0f  // 该值暂定-Digital Alarm Service
/** @} */

/**
 * @def GPS_FlAG
 * @brief GPS标识符
 */
#define GPS_FlAG                      (116)

/**
 * @def CCL_RECV_MAX
 * @brief 接收呼叫控制数据最大长度
 */
#define CCL_RECV_MAX                  (600)

/**
 * @def PDP_MAX
 * @brief 接收短消息数据最大长度
 */
#define PDP_MAX                       (67)

/**
 * @def INF_RECV_MAX
 * @brief 接收接口适配数据最大长度
 */
#define INF_RECV_MAX                  (128)

/**
 * @def  CCL_HEAD_LEN
 * @brief CCL协议头长度
 */
#define CCL_HEAD_LEN                  (20)

/**
 * @def  NM_DATA_LEN
 * @brief 网管数据长度
 */
#define NM_DATA_LEN                   (11)

/**
 * @def DB_NODE_MAX
 * @brief 链表最大节点数
 */
#define DB_NODE_MAX                   (512)

/**
 * @def DB_SIZE
 * @brief 节点数据包最大长度定义
 */
#define DB_SIZE                       (36)

#define GPS_REQ                ((UINT8)0x00)    //GPS请求
#define GPS_ACK                ((UINT8)0x01)    //GPS应答
#define GPS_OK                 ((UINT8)0x02)    //GPS完成
#define GPS_PRE                ((UINT8)0x03)    //GPS预占
#define GPS_RLY                ((UINT8)0x04)    //GPS中转

#define NODE_TYPE_D            ((UINT8)0x00)    //数据节点
#define NODE_TYPE_V            ((UINT8)0x01)    //语音节点

/**
 * @brief  邻点锻炼告警状态标志
 */
#define     TURN_ON                  1     // 设置邻点断链告警开关[打开]
#define     TURN_OFF                 0     // 设置邻点断链告警开关[关闭]
#define     DISCON_HAPPEN            1     // 检测到邻点断链出现，
#define     DISCON_RECOVER           0     // 检测到邻点断链已恢复，当前为不断链状态，


/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/

/******************************************************************************
 *   枚举声明
 *   *************************************************************************/
/**
 * @enum CALL_STATE_TYPE_E
 * @brief 链路层到接口层消息类型
 */
typedef enum _CALL_STATE_TYPE_E
{
    CALL_IDLE       = 0x00,                      ///< 呼叫空闲
    CALL_VOICE_U    = 0x01,                      ///< 语音上行呼叫
    CALL_VOICE_D    = 0x02,                      ///< 语音下行呼叫
    CALL_DATA_U     = 0x03,                      ///< 数据上行呼叫
    CALL_DATA_D     = 0x04,                      ///< 数据下行呼叫
    CALL_WLU_U      = 0x05,                      ///< 链路机上行呼叫
    CALL_WLU_D      = 0x06,                      ///< 链路机下行呼叫
	//----误码率测试
	CALL_ERR_VOICE_U = 0x07

} CALL_STATE_TYPE_E;

/**
 * @enum SAP_TYPE_E
 * @brief 链路层业务数据类型
 */
typedef enum _SAP_TYPE_E
{
    S_LC_HDR            = 0,                      ///< LC头
    S_LC_TER            = 1,                      ///< LC终结
    S_VOICE             = 2,                      ///< 语音
    S_CSBK_PRE          = 3,                      ///< 预载波
    S_C_HDR             = 4,                      ///< 确认头
    S_U_HDR             = 5,                      ///< 非确认头
    S_RE_HDR            = 6,                      ///< 响应头
    S_SD_HDR            = 7,                      ///< 自定义头
    S_DATA_1_2          = 8,                      ///< 1/2数据
    S_DATA_3_4          = 9,                      ///< 3/4数据
    S_TD_LC             = 10,                     ///< 数据终结

    S_NEGR_BST          = 20,                     ///< 邻点突发
    S_NEGR_RPT          = 21,                     ///< 邻点上报

    S_GPS_REQ           = 30,                     ///< gps请求
    S_GPS_ACK           = 31,                     ///< gps应答

    S_CSBK_REQ          = 32,                     ///< CSBK请求
    S_CSBK_ACK          = 33,                     ///< CSBK应答

    S_KILL_REQ          = 34,                     ///< MS 遥毙请求
    S_KILL_ACK          = 35,                     ///< MS 遥毙应答

    S_ENABLE_REQ        = 36,                     ///< MS 使能请求
    S_ENABLE_ACK        = 37,                     ///< MS 使能应答

    S_ALARM_REQ         = 38,                     ///< MS 告警请求
    S_ALARM_ACK         = 39,                     ///< MS 告警应答

    S_NAS_GPS_REQ       = 40,                     ///< NAS gps请求
    S_NAS_GPS_ACK       = 41,                     ///< NAS gps应答

    S_NAS_KILL_REQ      = 42,                     ///< NAS gps请求
    S_NAS_KILL_ACK      = 43,                     ///< NAS gps应答

    S_NAS_STUN_REQ      = 44,                     ///< NAS gps请求
    S_NAS_STUN_ACK      = 45,                     ///< NAS gps应答

    S_NAS_ENABLE_REQ    = 46,                     ///< NAS gps请求
    S_NAS_ENABLE_ACK    = 47,                     ///< NAS gps应答

    S_NAS_NEGR_REQ      = 48,                     ///< NAS gps请求
    S_NAS_NEGR_ACK      = 49,                     ///< NAS gps应答

    S_NAS_ALARM_REQ     = 50,                     ///< NAS 告警请求
    S_NAS_ALARN_ACK     = 51,                     ///< NAS 告警应答

    S_NAS_RELAY         = 60,                     ///< NAS gps请求
    S_NAS_PRE_LC        = 61,                     ///< NAS gps请求

} SAP_TYPE_E;


/******************************************************************************
 *   结构体声明
 *   *************************************************************************/
/**
 * @struct DBC_NODE_T
 * @brief  链表节点数据结构
 */
typedef struct _DBC_NODE_T
{
    struct _DBC_NODE_T* ptPrev;          ///< 前一节点
    struct _DBC_NODE_T* ptNext;          ///< 后一节点
    UINT8               auData[DB_SIZE]; ///< 帧数据
    UINT32              u4Node;          ///< 节点句柄
    UINT8               eNodeType;       ///< 节点类型
    BOOL                bNdTrafCtrl;     ///< 是否需要流控

} DBC_NODE_T;

/**
 * @struct  GPS_AI_DATA_T
 * @brief  GPS空口数据结构
 */
typedef struct _GPS_AI_DATA_T
{
    UINT8  CmpsHdr[5];      ///< UDP 压缩头
    UINT8  MsgHdr;          ///< 消息头
    UINT16 Opcode;          ///< 操作码
    UINT8  RequestID[4];    ///< 请求ID
    UINT8  Result[2];       ///< 结果
    UINT8  Time[3];         ///< gps time
    UINT8  Data[3];         ///< gps data
    UINT32 Latitude: 31;    ///< 纬度
    UINT32 NS: 1;           ///< 纬度指示 0:SOUTH 1:NORTH
    UINT32 Longitude: 31;   ///< 经度
    UINT32 EW: 1;           ///< 经度指示 0:EAST 1:WEST
    UINT8  Speed[2];        ///< 地面速度
    UINT8  Direction[2];    ///< 方向角

} GPS_AI_DATA_T;

/**
 * @struct  GPS_DEV_DATA_T
 * @brief  GPS设备数据结构
 */
typedef struct _GPS_DEV_DATA_T
{
    UINT8  NS;              ///<: 1;   纬度指示 0:SOUTH 1:NORTH
    UINT8  EW;              ///<: 1;   经度指示 0:EAST  1:WEST
    UINT8  NDEG;            ///<: 7;   纬度    0~89
    UINT8  NMINmm;          ///<: 6;   纬分的整数部分 0~59
    UINT32 NMINF;           ///<: 17; 纬分的小数部分 0~99999
    UINT8  EDEG;            ///<: 8;   经度     0~179
    UINT8  EMINmm;          ///<: 6;   经分的整数部分 0～59
    UINT32 EMINF;           ///<: 17; 经分的小数部分 0～99999
    UINT8  YEAR;            ///<: 7;   年 0～99
    UINT8  MONTH;           ///<: 4;   月 1～12
    UINT8  DAY;             ///<: 5;   日 1～31
    UINT8  HOUR;            ///<: 5;   时 0～23
    UINT8  MIN;             ///<: 6;   分 0~59
    UINT8  SEC;             ///<: 6;   秒 0~59

}__attribute__((packed,aligned(1))) GPS_DEV_DATA_T;

/**
 * @struct  GPS_NAS_DATA_T
 * @brief  GPS NAS数据结构
 */
typedef struct _GPS_NAS_DATA_T
{
    UINT8  NS: 1;           //<1bit 纬度指示 0:SOUTH 1:NORTH
    UINT8  NDEG: 7;         //<7bit 纬度    0~89
    UINT8  EW: 1;           //<1bit 经度指示 0:EAST 1:WEST
    UINT8  NMINmm: 7;       //<6bit 纬分的整数部分 0~59
    UINT16 NMINF;           //<14bit 纬分的小数部分 0~9999
    UINT8  EDEG;            //<8bit 经度     0~179
    UINT8  EMINmm;          //<6bit 经分的整数部分 0~59
    UINT16 EMINF;           //<14bit 经分的小数部分 0~9999

}__attribute__((packed,aligned(1))) GPS_NAS_DATA_T;

/**
 * @struct  IPv4_COMPRESS_HEADER_T
 * @brief  IPv4 压缩头结构
 */
typedef struct _IPv4_COMPRESS_HEADER_T
{
    UINT16 IPv4Idt;
    UINT8 DAID:4;
    UINT8 SAID:4;
    UINT8 SPID:7;
    UINT8 MHCO:1;
    UINT8 DPID:7;
    UINT8 LHCO:1;

}__attribute__((packed,aligned(1))) IPv4_COMPRESS_HEADER_T;

/**
 * @struct  MS_GPS_REQ_T
 * @brief  MS GPS 请求数据结构
 */
typedef struct _MS_GPS_REQ_T
{
    IPv4_COMPRESS_HEADER_T IpCmpsHdr;
    UINT8 MsgHdr;
    UINT16 OpCode;
    UINT16 ByteLen;
    UINT8 RequestID[4];
    UINT8 RadioIP[4];
    UINT8 CheckSum;
    UINT8 MsgEnd;

}__attribute__((packed,aligned(1))) MS_GPS_REQ_T;

/**
 * @struct WLU_DATA_UL_T
 * @brief WLU上行到数据链路结构体
 */
typedef struct _WLU_DATA_UL_T
{
    UINT8 MsgType;         ///< 0x01:邻点查询0x02:GPS查询
    UINT8 AckTag:2;        ///< 响应标记
    UINT8 Opcode:6;        ///< 操作码
    UINT8 SrcId:4;         ///< 源ID
    UINT8 DstId:4;         ///< 目的ID
    UINT8 PayLoad[8];      ///< 数据负载
    UINT8 Crc;             ///< 8Crc校验

}__attribute__((packed,aligned(1))) WLU_DATA_UL_T;

/**
 * @struct WLU_DATA_DL_T
 * @brief WLU下行到数据链路结构体
 */
typedef struct _WLU_DATA_DL_T
{
    UINT8 MsgType;         ///< 0x01:邻点查询0x02:GPS查询
    UINT8 AckTag:2;        ///< 响应标记
    UINT8 Opcode:6;        ///< 操作码
    UINT8 SrcId:4;         ///< 源ID
    UINT8 DstId:4;         ///< 目的ID
    UINT8 PayLoad[8];      ///< 数据负载
    UINT8 Crc;             ///< 8Crc校验

} WLU_DATA_DL_T;

/**
 * @struct NAS_NEGR_BURST_T
 * @brief NAS邻点突发数据链路结构体
 */
typedef struct _NAS_NEGR_BURST_T
{
    UINT8 NodeId;
    UINT32 NegrId;
    UINT8 Rsv[5];
    UINT8 Crc[2];

}__attribute__((packed,aligned(1))) NAS_NEGR_BURST_T;

/**
 * @struct DLL_GLB_CFG_T
 * @brief 配置项
 */
typedef struct _DLL_GLB_CFG_T
{
    UINT8  auUploadLC[12];             ///< 时隙上行LC信令
    UINT8  auUploadPI[12];             ///< 时隙上行PI信令
    UINT8  auMSCC;                     ///< 终端色码标示
    UINT8  auWLUCC;                    ///< 链路机色码标示
    UINT8  auPI;                       ///< 加密标示
    UINT8  auSubNet;                   ///< 空口子网ID
    UINT8  auWorkMode;                 ///< 工作模式 PDT or DMR
    UINT8  auDownloadLC[12];           ///< 时隙下行LC信令
    UINT8  auDownloadPI[12];           ///< 时隙下行PI信令
    UINT8  auEmbInfo[16];              ///< 语音内嵌信令32bit
    UINT8  auCCflag;                   ///< 远程连接中心标示
    UINT8  auNegrBuSw;                 ///< 邻点突然开关1:on,  0:off
    UINT8  auNegrCyc;                  ///< 邻点突发周期(单位分钟，最小1)
    UINT8  auNodeId;                   ///< 本地节点ID
    UINT8  auSoureId[3];               ///< 一体机源ID
    UINT8  auGroupId[3];               ///< 一体机组ID
    UINT8  auNasGps[8];                ///< 一体机GPS数据
    UINT8  auMsGps[8];                 ///< 终端GPS数据
    UINT16 auSevHangTime;              ///< 业务挂起时间
    UINT32 auStunFlag;                 ///< 遥晕标记
    UINT32 auKillFlag;                 ///< 遥毙标记
    UINT32 auNegrId1;                  ///< 邻点信息1
    UINT32 auNerInfo1[32];             ///< 邻点拓扑信息 1
    UINT32 auNegrId2;                  ///< 邻点信息2
    UINT32 auNerInfo2[32];             ///< 邻点拓扑信息 2

}DLL_GLB_CFG_T;

/**
 * @struct  NAS_PRE_LC_PDU
 * @brief 下行NAS预占LC
 */
typedef struct _NAS_PRE_LC_PDU
{
    UINT8 uFLCO :6;   // CtrOpcode 完整链路控制命令码
    UINT8 uREV  :1;   // Reserved
    UINT8 uPF   :1;   // ProtectFlg 保护标志，应设置为0
    UINT8 uFID;       // FeatureId: standard:hytera:moto:pdt.
    UINT8 uSO;        // ServiceOpt, 业务选项
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 auCRC[3];

} NAS_PRE_LC_PDU;

/**
 * @struct  EMBBED_GPS_PDU
 * @brief 内嵌GPS数据结构
 */
typedef struct _EMBBED_GPS_PDU
{
    UINT8 uFLCO :6;
    UINT8 uREV  :1;
    UINT8 uPF   :1;
    UINT8 uFID;
    UINT8 uEW   :1;
    UINT8 uErrCode:3;
    UINT8 uREV1   :4;
    UINT8 uLongitude[3];
    UINT8 uLatitude[3];

} EMBBED_GPS_PDU;

/**
 * @struct  GEN_CSBK_PDU
 * @brief  通用CSBK
 */
typedef struct _GEN_CSBK_PDU
{
    UINT8 uCSBKO:6;
    UINT8 uPF  :1;
    UINT8 uLB  :1;
    UINT8 uFID;
    UINT8 uData[8];
    UINT8 auCRC[2];

} GEN_CSBK_PDU;

/**
 * @struct  PRE_CSBK_PDU
 * @brief  预载波CSBK
 */
typedef struct _PRE_CSBK_PDU
{
    UINT8 uCSBKO:6;
    UINT8 uPF  :1;
    UINT8 uLB  :1;
    UINT8 uFID;
    UINT8 uREV :6;
    UINT8 uGI  :1;
    UINT8 uDC  :1;
    UINT8 uCBF;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 auCRC[2];

} PRE_CSBK_PDU;

/**
 * @struct  SUPS_CSBK_PDU
 * @brief  补充业务CSBK
 */
typedef struct _SUPS_CSBK_PDU
{
    UINT8 uCSBKO:6;
    UINT8 uPF  :1;  // Protect Flag
    UINT8 uLB  :1;  // Last Block, 0-MBC Header or Continuation Block,1-CSBK or MBC Last Block
    UINT8 uFID;
    UINT8 uSSO;
    UINT8 uREV;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 auCRC[2];

} SUPS_CSBK_PDU;

/**
 * @struct PDT_NC_HEAD_PDU_DT
 * @brief PDT 空口下行无确认数据头
 */
typedef struct _PDT_NC_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uDEI          :2;
    UINT8 uRV2          :2;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uFMF          :1;
    UINT8 uFSN          :4;
    UINT8 uRV3          :4;
    UINT8 auCRC[2];

} PDT_NC_HEAD_PDU_DT;

/**
 * @struct PDT_NC_HEAD_PDU_UT
 * @brief PDT 空口上行无确认数据头
 */
typedef struct _PDT_NC_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uDEI          :2;
    UINT8 uRV2          :2;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uFMF          :1;
    UINT8 uFSN          :4;
    UINT8 uRV3          :4;

} PDT_NC_HEAD_PDU_UT;

/**
 * @struct DMR_NC_HEAD_PDU_DT
 * @brief DMR 空口下行无确认数据头
 */
typedef struct _DMR_NC_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uPOCH         :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uPOCL         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uF            :1;
    UINT8 uFSN          :4;
    UINT8 uRV1          :4;
    UINT8 auCRC[2];

} DMR_NC_HEAD_PDU_DT;

/**
 * @struct DMR_NC_HEAD_PDU_UT
 * @brief DMR 空口上行无确认数据头
 */
typedef struct _DMR_NC_HEAD_PDU_UT
{
    UINT8 uDPF          :4;  // Data Packet Format
    UINT8 uPOCH         :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uPOCL         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uF            :1;
    UINT8 uFSN          :4;
    UINT8 uRV1          :4;

} DMR_NC_HEAD_PDU_UT;

/**
 * @struct PDT_C_HEAD_PDU_DT
 * @brief PDT 空口下行确认数据头
 * uFSN: 片序号。信息单元用于识别连续数据片消息的编号，一起组成一个较长的数据消息
 *       FSN的最高比特用于标识最后一片，其余片应置0，
 *       FSN的低3bit为每个片的序号, 第一片应为000，并且一次递增, 当数字达到111时, 则下一个
 *       应为001而不是000. 由单个确认消息组成的逻辑消息, FSN的值为1000
 *  uNS: 发送序号      
 */
typedef struct _PDT_C_HEAD_PDU_DT
{
    UINT8 uDPF          :4;     // 指示有确认和无确认，0011:有确认方式
    UINT8 uRV1          :1;     // 保留
    UINT8 uHC           :1;     // 保留
    UINT8 uA            :1;     // 1:需要立即响应，不支持滑窗
    UINT8 uGI           :1;     // 目的用户为个人号码
    UINT8 uDEI          :2;     // 指示有效数据的结束，数据填充样图格式
    UINT8 uRV2          :2;     // 保留
    UINT8 uSAP          :4;     // 用来指示数据分组的业务类型，1001:私有分组数据;1011:端到端数据加密头帧
    UINT8 auTADDR[3];           // 目的用户的事件标签,LDS或PDI或MS空口地址
    UINT8 auSADDR[3];           // 源用户的事件标签，LDS或PDI或MS空口地址
    UINT8 uBF           :7;     // 本数据包中后面跟的帧的数据
    UINT8 uFMF          :1;     // Full Message Flag 用于表示本次传输是否完整的数据或者重传的部分数据 0:重传数据包; 1:第一次传输完整包
    UINT8 uFSN          :4;     // 数据包的发送序号，发送者发送一个数据包(不包括重复的)，就加1；用于检测是否发重复了。
    UINT8 uNS           :3;     // 1:接收方同步数据包的发送序号
    UINT8 uS            :1;     // 数据分片序号（支持分片功能才使用）
    UINT8 auCRC[2];

} PDT_C_HEAD_PDU_DT;

/**
 * @struct PDT_C_HEAD_PDU_UT
 * @brief PDT 空口上行确认数据头
 */
typedef struct _PDT_C_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uDEI          :2;
    UINT8 uRV2          :2;
    UINT8 uSAP          :4;  // 0011-UDP/ip 压缩头
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uFMF          :1;  // Full Message Flag 用于表示本次传输是否完整的数据或者重传的部分数据 0:重传数据包; 1:第一次传输完整包
    UINT8 uFSN          :4;
    UINT8 uNS           :3;
    UINT8 uS            :1;

} PDT_C_HEAD_PDU_UT;

/**
 * @struct DMR_C_HEAD_PDU_DT
 * @brief DMR 空口下行确认数据头
 */
typedef struct _DMR_C_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uPOCH         :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uPOCL         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uF            :1;
    UINT8 uFSN          :4;
    UINT8 uNS           :3;
    UINT8 uS            :1;
    UINT8 auCRC[2];

} DMR_C_HEAD_PDU_DT;

/**
 * @struct DMR_C_HEAD_PDU_UT
 * @brief DMR 空口上行确认数据头
 */
typedef struct _DMR_C_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uPOCH         :1;
    UINT8 uHC           :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uPOCL         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uF            :1;
    UINT8 uFSN          :4;
    UINT8 uNS           :3;
    UINT8 uS            :1;

} DMR_C_HEAD_PDU_UT;

/**
 * @struct PDT_RE_HEAD_PDU_DT
 * @brief PDT 空口下行应答数据头
 */
typedef struct _PDT_RE_HEAD_PDU_DT
{
    UINT8 uDPF          :4;  // 数据分组合适 0001-Response, 0010-unconfirmed, 0011-confirmed
    UINT8 uRV1          :2;
    UINT8 uA            :1;  // 相应请求标识 0-No Response, 1-Response
    UINT8 uRV2          :1;  
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;  // 服务接入点 0010-TCP/IP header compression, 0011-UDP/IP header compression
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;  // 后续帧数
    UINT8 uRV4          :1;
    UINT8 uSTAT         :3;   
    UINT8 uTYPE         :3;
    UINT8 uCLAS         :2;
    UINT8 auCRC[2];

} PDT_RE_HEAD_PDU_DT;

/**
 * @struct PDT_RE_HEAD_PDU_UT
 * @brief PDT 空口上行应答数据头
 */
typedef struct _PDT_RE_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :2;
    UINT8 uA            :1;  // 相应请求标识 0-No Response, 1-Response
    UINT8 uRV2          :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;  // 10-TCP/IP压缩头, 11-UDP/IP压缩头
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;  // 后续帧数
    UINT8 uRV4          :1;
    UINT8 uSTAT         :3;
    UINT8 uTYPE         :3;
    UINT8 uCLAS         :2;

} PDT_RE_HEAD_PDU_UT;

/**
 * @struct DMR_RE_HEAD_PDU_DT
 * @brief DMR 空口下行应答数据头
 */
typedef struct _DMR_RE_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uDEI          :2;
    UINT8 uRV2          :2;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uRV3          :1;
    UINT8 uSTATUS       :3;
    UINT8 uTYPE         :3;
    UINT8 uCLASS        :2;
    UINT8 auCRC[2];

} DMR_RE_HEAD_PDU_DT;

/**
 * @struct DMR_RE_HEAD_PDU_UT
 * @brief DMR 空口上行应答数据头
 */
typedef struct _DMR_RE_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uRV2          :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uRV4          :1;
    UINT8 uSTATUS       :3;
    UINT8 uTYPE         :3;
    UINT8 uCLASS        :2;

} DMR_RE_HEAD_PDU_UT;

/**
 * @struct DMR_P_HEAD_PDU_DT
 * @brief DMR空口下行私有数据头
 */
typedef struct _DMR_P_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uSAP          :4;
    UINT8 uMFID;
    UINT8 auTADDR[8];
    UINT8 auCRC[2];

} DMR_P_HEAD_PDU_DT;

/**
 * @struct DMR_P_HEAD_PDU_UT
 * @brief DMR空口上行私有数据头
 */
typedef struct _DMR_P_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uSAP          :4;
    UINT8 uMFID;
    UINT8 auTADDR[8];

} DMR_P_HEAD_PDU_UT;

/**
 * @struct DMR_SP_HEAD_PDU_DT
 * @brief DMR空口下行状态或预编码数据头
 */
typedef struct _DMR_SP_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uS_PH         :2;
    UINT8 uDP           :3;
    UINT8 uSP           :3;
    UINT8 uS_PL;
    UINT8 auCRC[2];

} DMR_SP_HEAD_PDU_DT;

/**
 * @struct DMR_SP_HEAD_PDU_UT
 * @brief DMR空口上行状态或预编码数据头
 */
typedef struct _DMR_SP_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uS_PH         :2;
    UINT8 uDP           :3;
    UINT8 uSP           :3;
    UINT8 uS_PL;

} DMR_SP_HEAD_PDU_UT;

/**
 * @struct DMR_RA_HEAD_PDU_DT
 * @brief DMR空口下行原始短数据头
 */
typedef struct _DMR_RA_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uF            :1;
    UINT8 uS            :1;
    UINT8 uDP           :3;
    UINT8 uSP           :3;
    UINT8 uBitPad;
    UINT8 auCRC[2];

} DMR_RA_HEAD_PDU_DT;

/**
 * @struct DMR_RA_HEAD_PDU_UT
 * @brief DMR空口上行原始短数据头
 */
typedef struct _DMR_RA_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uF            :1;
    UINT8 uS            :1;
    UINT8 uDP           :3;
    UINT8 uSP           :3;
    UINT8 uBitPad;

} DMR_RA_HEAD_PDU_UT;

/**
 * @struct DMR_DE_HEAD_PDU_DT
 * @brief DMR空口下行预定义数据头
 */
typedef struct _DMR_DE_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uF            :1;
    UINT8 uS            :1;
    UINT8 uDDF          :6;
    UINT8 uBitPad;
    UINT8 auCRC[2];

} DMR_DE_HEAD_PDU_DT;

/**
 * @struct DMR_DE_HEAD_PDU_UT
 * @brief DMR空口上行预定义数据头
 */
typedef struct _DMR_DE_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uABH          :2;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uABL          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uF            :1;
    UINT8 uS            :1;
    UINT8 uDDF          :6;
    UINT8 uBitPad;

} DMR_DE_HEAD_PDU_UT;

/**
 * @struct DMR_UD_HEAD_PDU_DT
 * @brief DMR空口下行统一数据传输数据头
 */
typedef struct _DMR_UD_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uRV2          :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uUDTF         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uUAB          :2;
    UINT8 uRV3          :1;
    UINT8 uPADN         :5;
    UINT8 uUDTO         :6;
    UINT8 uPF           :1;
    UINT8 uSF           :1;
    UINT8 auCRC[2];

} DMR_UD_HEAD_PDU_DT;

/**
 * @struct DMR_UD_HEAD_PDU_UT
 * @brief DMR空口上行统一数据传输数据头
 */
typedef struct _DMR_UD_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :1;
    UINT8 uRV2          :1;
    UINT8 uA            :1;
    UINT8 uGI           :1;
    UINT8 uUDTF         :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uUAB          :2;
    UINT8 uRV3          :1;
    UINT8 uPADN         :5;
    UINT8 uUDTO         :6;
    UINT8 uPF           :1;
    UINT8 uSF           :1;

} DMR_UD_HEAD_PDU_UT;

typedef struct _R12_NC_BLOCK_DT
{
    UINT8 auData[R12_NC_BLOCK_MAX][R12_NC_DATA_DLEN];

} R12_NC_BLOCK_DT;

typedef struct _R12_NC_BLOCK_UT
{
    UINT8 auData[R12_NC_BLOCK_MAX][R12_NC_DATA_ULEN];

} R12_NC_BLOCK_UT;

typedef struct _R34_NC_BLOCK_DT
{
    UINT8 auData[R34_NC_BLOCK_MAX][R34_NC_DATA_DLEN];

} R34_NC_BLOCK_DT;

typedef struct _R34_NC_BLOCK_UT
{
    UINT8 auData[R34_NC_BLOCK_MAX][R34_NC_DATA_ULEN];

} R34_NC_BLOCK_UT;

typedef struct _R11_NC_BLOCK_DT
{
    UINT8 auData[R11_NC_BLOCK_MAX][R11_NC_DATA_DLEN];

} R11_NC_BLOCK_DT;

typedef struct _R11_NC_BLOCK_UT
{
    UINT8 auData[R11_NC_BLOCK_MAX][R11_NC_DATA_ULEN];

} R11_NC_BLOCK_UT;

typedef struct _R12_C_BLOCK_DT
{
    UINT8 auData[R12_C_BLOCK_MAX][R12_C_DATA_DLEN-2];

} R12_C_BLOCK_DT;

typedef struct _R12_C_BLOCK_UT
{
    UINT8 auData[R12_C_BLOCK_MAX][R12_C_DATA_ULEN-2];

} R12_C_BLOCK_UT;

typedef struct _R34_C_BLOCK_DT
{
    UINT8 auData[R34_C_BLOCK_MAX][R34_C_DATA_DLEN-2];

} R34_C_BLOCK_DT;

typedef struct _R34_C_BLOCK_UT
{
    UINT8 auData[R34_C_BLOCK_MAX][R34_C_DATA_ULEN-2];

} R34_C_BLOCK_UT;

typedef struct _R11_C_BLOCK_DT
{
    UINT8 auData[R11_C_BLOCK_MAX][R11_C_DATA_DLEN-2];

} R11_C_BLOCK_DT;

typedef struct _R11_C_BLOCK_UT
{
    UINT8 auData[R11_C_BLOCK_MAX][R11_C_DATA_ULEN-2];

} R11_C_BLOCK_UT;

/**
 * @brief    下行无确认R12帧数据
 */
typedef struct _R12_NC_DATA_DT
{
    UINT8 auData[R12_NC_DATA_DLEN];

} R12_NC_DATA_DT;

/**
 * @brief    上行无确认R12帧数据
 */
typedef struct _R12_NC_DATA_UT
{
    UINT8 auData[R12_NC_DATA_ULEN];

} R12_NC_DATA_UT;

/**
 * @brief    下行无确认R12尾帧数据
 */
typedef struct _R12_NC_LDATA_DT
{
    UINT8 auData[R12_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R12_NC_LDATA_DT;

/**
 * @brief    上行无确认R12尾帧数据
 */
typedef struct _R12_NC_LDATA_UT
{
    UINT8 auData[R12_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R12_NC_LDATA_UT;

/**
 * @brief    下行有确认R12帧数据
 */
typedef struct _R12_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R12_C_DATA_DLEN-2];

} R12_C_DATA_DT;

/**
 * @brief    上行有确认R12帧数据
 */
typedef struct _R12_C_DATA_UT
{
    UINT8 auData[R12_C_DATA_ULEN-2];

} R12_C_DATA_UT;

/**
 * @brief    下行有确认R12尾帧数据
 */
typedef struct _R12_C_LDATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R12_C_DATA_DLEN-6];
    UINT8 auCRC[4];

} R12_C_LDATA_DT;

/**
 * @brief    上行有确认R12尾帧数据
 */
typedef struct _R12_C_LDATA_UT
{
    UINT8 auData[R12_C_DATA_ULEN-6];
    UINT8 auRV[6];

} R12_C_LDATA_UT;

/**
 * @brief    下行无确认R34帧数据
 */
typedef struct _R34_NC_DATA_DT
{
    UINT8 auData[R34_NC_DATA_DLEN];

} R34_NC_DATA_DT;

/**
 * @brief    上行无确认R34帧数据
 */
typedef struct _R34_NC_DATA_UT
{
    UINT8 auData[R34_NC_DATA_ULEN];

} R34_NC_DATA_UT;

/**
 * @brief    下行无确认R34尾帧数据
 */
typedef struct _R34_NC_LDATA_DT
{
    UINT8 auData[R34_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R34_NC_LDATA_DT;

/**
 * @brief    上行无确认R34尾帧数据
 */
typedef struct _R34_NC_LDATA_UT
{
    UINT8 auData[R34_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R34_NC_LDATA_UT;

/**
 * @brief    下行有确认R34帧数据
 */
typedef struct _R34_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_DLEN-2];

} R34_C_DATA_DT;

/**
 * @brief    上行有确认R34帧数据
 */
typedef struct _R34_C_DATA_UT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;  // Data Block Serial Number
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_ULEN-2];

} R34_C_DATA_UT;

/**
 * @brief    下行有确认R34尾帧数据
 */
typedef struct _R34_C_LDATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_DLEN-6];
    UINT8 auCRC[4];

} R34_C_LDATA_DT;

/**
 * @brief    上行有确认R34尾帧数据
 */
typedef struct _R34_C_LDATA_UT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_DLEN-6];
    UINT8 auCRC[4];

} R34_C_LDATA_UT;

/**
 * @brief    下行无确认R11帧数据
 */
typedef struct _R11_NC_DATA_DT
{
    UINT8 auData[R11_NC_DATA_DLEN];

} R11_NC_DATA_DT;

/**
 * @brief    上行无确认R11帧数据
 */
typedef struct _R11_NC_DATA_UT
{
    UINT8 auData[R11_NC_DATA_ULEN];

} R11_NC_DATA_UT;

/**
 * @brief    下行无确认R11尾帧数据
 */
typedef struct _R11_NC_LDATA_DT
{
    UINT8 auData[R11_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R11_NC_LDATA_DT;

/**
 * @brief    上行无确认R11尾帧数据
 */
typedef struct _R11_NC_LDATA_UT
{
    UINT8 auData[R11_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R11_NC_LDATA_UT;

/**
 * @brief    下行有确认R11帧数据
 */
typedef struct _R11_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;  // 数据帧序号, 用于给分组内的数据帧编号,0~M-1逐次递增，M为分组数据头帧中的BF值
    UINT8 uCRC9L;
    UINT8 auData[R11_C_DATA_DLEN-2];

} R11_C_DATA_DT;

/**
 * @brief    上行有确认R11帧数据
 */
typedef struct _R11_C_DATA_UT
{
    UINT8 auData[R11_C_DATA_ULEN-2];

} R11_C_DATA_UT;

/**
 * @brief    下行有确认R11尾帧数据
 */
typedef struct _R11_C_LDATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R11_C_DATA_DLEN-6];
    UINT8 auCRC[4];

} R11_C_LDATA_DT;

/**
 * @brief    上行有确认R11尾帧数据
 */
typedef struct _R11_C_LDATA_UT
{
    UINT8 auData[R11_C_DATA_ULEN-6];
    UINT8 auRV[6];

} R11_C_LDATA_UT;

/**
 * @brief    链路控制总PDU
 * Full Link Control (FULL LC) PDU
 * len:96bit
 * The FULL LC PDU has a length of either 96 bits for header and terminator bursts 
 *     or 77 bits for embedded signalling.
 */
typedef struct _GEN_LC_PDU_T
{
    UINT8 uFLCO :6;
    UINT8 uRV   :1;
    UINT8 uPF   :1;
    UINT8 uFID;
    UINT8 uFLCD[7];  // Full LC Data
    UINT8 uCRC[3];

} GEN_LC_PDU_T;

/**
 * @brief    语音链路控制PDU
 */
typedef struct _VOICE_LC_PDU_T
{
    UINT8 uFLCO                     :6;
    UINT8 uRV                       :1;
    UINT8 uPF                       :1;
    UINT8 uFID;
    UINT8 uSO;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uCRC[3];

} VOICE_LC_PDU_T;

/**
 * @brief    数据链路终结帧
 */
typedef struct _TD_LC_PDU_T
{
    UINT8 uFLCO                     :6;  // 完整链路控制码 TD_LC=0X30
    UINT8 uRV                       :1;
    UINT8 uPF                       :1;  // 保护标识
    UINT8 uFID;                          // 功能集
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uNS                       :3;  // 发送序号 N(S) Send sequence Number
    UINT8 uS                        :1;  // 重新同步标识 S(Re-Synchronize flag)
    UINT8 uRV1                      :1;  // 保留
    UINT8 uF                        :1;  // 消息完整标识 FMF(Full Message Flag)
    UINT8 uA                        :1;  // Response Request 响应请求标识
    UINT8 uGI                       :1;
    UINT8 uCRC[3];

} TD_LC_PDU_T;

/**
 * @brief   上行分组数据配置信息结构体
 */
typedef struct _PDP_UT
{
    UINT8  uDPF; // Data Packet Format, 用来识别数据包, 0-UDT,1-Response,2-unconfirmed,3-confirmed ... ...
    UINT8  uBF;  // Blocks to Follow, Number of blocks to follow
    UINT8  uSAP; // Service Access Point
    UINT8  uPOC; //
    UINT8  uDEI;
    UINT8  uNS;
    UINT8  uDataType;
    UINT8  uState;

    UINT8  uPHeadNum;    // 私有头个数
    UINT8  uPHeadCnt;

    UINT8  uEHeadNum;    // 加密头个数
    UINT8  uEHeadCnt;

    UINT8  uBLOCKNum;    // 接收包计数
    UINT8  uBLOCKCnt;

    UINT8  uTdLcNum;
    UINT8  uTdLcCnt;

    UINT8  uRetryNum;
    UINT8  uRetryCnt;

    UINT8  uSrcId[3];
    UINT8  uDstId[3];

    UINT8  auResHead[12];
    UINT8  auResData[16];    // 1:数据成功接收

    UINT8  uReBlockNum;      // 接收包计数
    UINT8  uReBlockCnt;

    UINT8  auHead[3][12];
    UINT8  auData[1000];
    UINT8  auTdLc[12];

    UINT8  uBLOCKNum_R;         ///< 上行分组数据重传帧帧个数@note CHU接收上行数据有误，手台重传发送的重传帧个数
    UINT8  uBLOCKCnt_R;         ///< 上行分组数据重传帧帧计数
    UINT8  ucRetryNum;          ///< CHU 因CRC ERR(DBSN) 需要手台重传次数
    UINT8  ucErrDBSNCnt;        ///< 一次传输中，错误包的个数

} PDP_UT;

/**
 * @brief   下行分组数据配置信息结构体
 */
typedef struct _PDP_DT
{
    UINT8  uDPF;           ///< DPF(Data Packet Format)类型
    UINT8  uState;         ///< 当前分组数据发送状态  @note 空闲/第一次发送/重传
    UINT8  uSAP;           ///< SAP类型，服务接入点 UDT(统一数据传输)/TCP IP/UDP IP/IP分组数据/地址解析协议ARP/...
    UINT8  uPOC;
    UINT8  uDEI;           ///< 数据结束标识
    UINT8  uNS;
    UINT8  uDataType;      ///< 数据类型
    UINT8  uHdrNum;        ///< 下行分组数据头的总个数
    UINT8  uBLOCKNum;      ///< 下行分组数据数据包个数 @note 不包含P_HEAD和E_HEAD
    UINT8  uPHeadNum;      ///< 下行分组数据中P_Head个数
    UINT8  uEHeadNum;      ///< 下行分组数据中E_Head个数
    UINT8  s_uDBSNIndex;   ///< 下行分组数据数据包索引     @不包含P_HEAD和E_HEAD
    UINT8  BF;             ///< 除头帧之外的下行分组数据帧个数, 后续帧数(BF),规定了后续还有多少个数据帧
    UINT8  uTdLcNum;
    UINT8  uTdLcCnt;
    UINT8  uSrcId[3];
    UINT8  uDstId[3];
    UINT8  auResHead[12];  ///< 下行分组数据回复确认帧头帧
    UINT8  auResData[16];  ///< 下行分组数据发送结果图样         @每1bit代表一个帧，1为发送成功，0为发送失败
    UINT8  auHead[3][12];  ///< 下行分组数据头           @包括C_HEAD、P_HEAD、E_HEAD
    UINT8  auData[1000];   ///< TSC-CHU 下行分组数据SDU，@包括HEAD和payload
    UINT8  auTdLc[12];     ///<

} PDP_DT;



/******************************************************************************
 *  联合体声明
 *   *************************************************************************/
typedef union _PDP_HEAD_DU
{
    PDT_NC_HEAD_PDU_DT p_PdpNcHdr;
    PDT_C_HEAD_PDU_DT  p_Pdp_CHdr;
    PDT_RE_HEAD_PDU_DT p_PdpReHdr;
    DMR_NC_HEAD_PDU_DT d_PdpNcHdr;
    DMR_C_HEAD_PDU_DT  d_Pdp_CHdr;
    DMR_RE_HEAD_PDU_DT d_PdpReHdr;
    DMR_P_HEAD_PDU_DT  d_Pdp_PHdr;
    DMR_SP_HEAD_PDU_DT d_PdpSpHdr;
    DMR_RA_HEAD_PDU_DT d_PdpRaHdr;
    DMR_DE_HEAD_PDU_DT d_PdpDeHdr;
    DMR_UD_HEAD_PDU_DT d_PdpUdHdr;

} PDP_HEAD_DU;

typedef union _PDP_HEAD_UU
{
    PDT_NC_HEAD_PDU_UT p_PdpNcHdr;  // 空口上行无确认数据头
    PDT_C_HEAD_PDU_UT  p_Pdp_CHdr;  // 空口上行确认数据头
    PDT_RE_HEAD_PDU_UT p_PdpReHdr;  // 空口上行应答数据头
    DMR_NC_HEAD_PDU_UT d_PdpNcHdr;  // 空口上行无确认数据头
    DMR_C_HEAD_PDU_UT  d_Pdp_CHdr;  // 空口上行确认数据头
    DMR_RE_HEAD_PDU_UT d_PdpReHdr;  // 空口上行应答数据头
    DMR_P_HEAD_PDU_UT  d_Pdp_PHdr;  // DMR空口上行私有数据头
    DMR_SP_HEAD_PDU_UT d_PdpSpHdr;  // DMR空口上行状态或预编码数据头
    DMR_RA_HEAD_PDU_UT d_PdpRaHdr;  // DMR空口上行原始短数据头
    DMR_DE_HEAD_PDU_UT d_PdpDeHdr;  // DMR空口上行预定义数据头
    DMR_UD_HEAD_PDU_UT d_PdpUdHdr;  // DMR空口上行统一数据传输数据头

} PDP_HEAD_UU;

typedef union _PDP_DATA_BLOCK_DU
{
    R12_NC_BLOCK_DT tPdpR12NcBlk;
    R34_NC_BLOCK_DT tPdpR34NcBlk;
    R11_NC_BLOCK_DT tPdpR11NcBlk;
    R12_C_BLOCK_DT  tPdpR12CBlk;
    R34_C_BLOCK_DT  tPdpR34CBlk;
    R11_C_BLOCK_DT  tPdpR11CBlk;

} PDP_DATA_BLOCK_DU;

typedef union _PDP_DATA_BLOCK_UU
{
    R12_NC_BLOCK_UT tPdpR12NcBlk;
    R34_NC_BLOCK_UT tPdpR34NcBlk;
    R11_NC_BLOCK_UT tPdpR11NcBlk;
    R12_C_BLOCK_UT  tPdpR12CBlk;
    R34_C_BLOCK_UT  tPdpR34CBlk;
    R11_C_BLOCK_UT  tPdpR11CBlk;

} PDP_DATA_BLOCK_UU;

/**
 * @brief    下行R12分组数据联合体
 */
typedef union _R12_DU
{
    R12_NC_DATA_DT  tR12NcData;
    R12_NC_LDATA_DT tR12NcLData;
    R12_C_DATA_DT   tR12CData;
    R12_C_LDATA_DT  tR12CLData;

} R12_DU;

/**
 * @brief    上行R12分组数据联合体
 */
typedef union _R12_UU
{
    R12_NC_DATA_UT  tR12NcData;
    R12_NC_LDATA_UT tR12NcLData;
    R12_C_DATA_UT   tR12CData;
    R12_C_LDATA_UT  tR12CLData;

} R12_UU;

/**
 * @brief    下行R34分组数据联合体
 */
typedef union _R34_DU
{
    R34_NC_DATA_DT  tR34NcData;
    R34_NC_LDATA_DT tR34NcLData;
    R34_C_DATA_DT   tR34CData;
    R34_C_LDATA_DT  tR34CLData;

} R34_DU;

/**
 * @brief    上行R34分组数据联合体
 */
typedef union _R34_UU
{
    R34_NC_DATA_UT  tR34NcData;   // 上行无确认R34帧数据
    R34_NC_LDATA_UT tR34NcLData;  // 上行无确认R34尾帧数据
    R34_C_DATA_UT   tR34CData;    // 上行有确认R34帧数据
    R34_C_LDATA_UT  tR34CLData;   // 上行有确认R34尾帧数据

} R34_UU;

/**
 * @brief    下行R11分组数据联合体
 */
typedef union _R11_DU
{
    R11_NC_DATA_DT  tR11NcData;
    R11_NC_LDATA_DT tR11NcLData;
    R11_C_DATA_DT   tR11CData;
    R11_C_LDATA_DT  tR11CLData;

} R11_DU;

/**
 * @brief    上行R11分组数据联合体
 */
typedef union _R11_UU
{
    R34_NC_DATA_UT  tR11NcData;
    R34_NC_LDATA_UT tR11NcLData;
    R34_C_DATA_UT   tR11CData;
    R34_C_LDATA_UT  tR11CLData;

} R11_UU;

/**
 * @brief    下行分组数据帧数据联合体
 */
typedef union _PDP_DATA_DU
{
    R11_DU uR11Data;
    R34_DU uR34Data;
    R12_DU uR12Data;

} PDP_DATA_DU;

/**
 * @brief    上行分组数据帧数据联合体
 */
typedef union _PDP_DATA_UU
{
    R11_UU uR11Data;
    R34_UU uR34Data;
    R12_UU uR12Data;

} PDP_DATA_UU;

extern INT8 g_NerbValidFlg;
extern UINT32 g_BurstCnt;
extern UINT8 g_BurstInTalkingCnt;
#endif

