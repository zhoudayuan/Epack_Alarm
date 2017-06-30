#ifndef __SAP_H__
#define __SAP_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    sap.h
 * @brief   包含业务模块类型、变量、结构、宏定义
 * @author  陈禹良
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   修改历史
 *   2016-7-20                 陈禹良                     建立文件
 *   *************************************************************************/

/******************************************************************************
 *   宏定义
 *   *************************************************************************/
#undef TASK_NAME
#define TASK_NAME   "SAP"


/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/ipc.h>      //shmget
#include <sys/msg.h>      //msgget
#include <sys/shm.h>      //shmget
#include <arpa/inet.h>    //inet_addr(); inet_ntoa()
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <errno.h>
#include "dll_common.h"
#include "mgr_common.h"
#include "log.h"
#include "cli.h"


/******************************************************************************
 *   枚举声明
 *   *************************************************************************/
/**
 * @enum CCL_DATA_TYPE_E
 * @brief CCL数据帧类型
 */
typedef enum _CCL_DATA_TYPE_E
{
    //空口数据类型DC_MSG_DATA
    CT_JUNK_DATA            = 0x00,                      ///< 非数据消息
    CT_PI_HEADER            = 0x01,                      ///< PI头帧标识
    CT_LC_HEADER            = 0x02,                      ///< LC头帧标识
    CT_LC_TERMINATOR        = 0x03,                      ///< LC终结帧标识
    CT_PACKET_DATA          = 0x04,                      ///< 短消息

    CT_GPS_REPORT_REQ_MS    = 0x11,                      ///< 手台GPS上拉
    CT_GPS_REPORT_ACK_MS    = 0x12,                      ///< 手台GPS上拉
    CT_DISABLE_REQ_MS       = 0x13,                      ///< 摇晕
    CT_DISABLE_ACK_MS       = 0x14,                      ///< 摇晕
    CT_ENABLE_REQ_MS        = 0x15,                      ///< 终端激活
    CT_ENABLE_ACK_MS        = 0x16,                      ///< 终端激活
    CT_ALARM_REQ_MS         = 0x17,                      ///< 终端紧急告警
    CT_ALARM_ACK_MS         = 0x18,                      ///< 终端紧急告警

    //链路机数据类型DC_MSG_WLU
    CT_GPS_REPORT_REQ_NAS   = 0x21,                      ///< GPS上拉
    CT_GPS_REPORT_ACK_NAS   = 0x22,                      ///< GPS上拉
    CT_STUN_REQ_NAS         = 0x23,                      ///< 摇晕
    CT_STUN_ACK_NAS         = 0x24,                      ///< 摇晕
    CT_KILL_REQ_NAS         = 0x25,                      ///< 摇毙
    CT_KILL_ACK_NAS         = 0x26,                      ///< 摇毙
    CT_ENABLE_REQ_NAS       = 0x27,                      ///< WLU  激活
    CT_ENABLE_ACK_NAS       = 0x28,                      ///< WLU  激活
    CT_STUN_RPT_NAS         = 0x29,                      ///< WLU  遥晕上报
    CT_KILL_RPT_NAS         = 0x2a,                      ///< WLU  遥毙上报
    CT_ENABLE_RPT_NAS       = 0x2b,                      ///< WLU  激活上报

    CT_NEGHR_QUERY          = 0x31,                      ///< 邻点信息查询
    CT_NEGHR_QUERY_ACK      = 0x32,                      ///< 邻点信息响应
    CT_NEGHR_REPORT         = 0x33,                      ///< 邻点信息上报
    CT_DISCON_ALARM         = 0x34,                      ///< 断链告警
    CT_DISCON_ALARM_CLEAR   = 0x35,                      ///< 断链告警清除

} CCL_DATA_TYPE_E;

/**
 * @struct CCL_DLL_DL_T
 * @brief CCL下行到数据链路结构体
 */
typedef struct _CCL_DLL_DL_T
{
    UINT8 MsgType;          ///< 0x01:语音 0x02:数据 0x03:邻点
    UINT8 FrmType;          ///< 见枚举 _FrameType_E
    UINT8 DataType;         ///< 数据类型
    UINT8 SrcId[3];         ///< 源ID
    UINT8 DstId[3];         ///< 目的ID
    UINT8 uRev0[10];        ///< 预留0
    UINT8 DataLen;          ///< 负载长度
    UINT8 PayLoad[500];     ///< 负载数据

} CCL_DLL_DL_T;

/**
 * @struct DLL_CCL_UL_T
 * @brief 数据链路上行到CCL结构体
 */
typedef struct _DLL_CCL_UL_T
{
    UINT8 MsgType;          ///< 0x01:语音 0x02:数据 0x03:邻点
    UINT8 FrmType;          ///< 见枚举 _FrameType_E
    UINT8 DataType;         ///< 数据类型
    UINT8 SrcId[3];         ///< 源ID
    UINT8 DstId[3];         ///< 目的ID
    UINT8 Vari[2];          ///< 方差FD 上行有效
    UINT8 Rssi[2];          ///< 场强值
    UINT8 Prohibit;         ///< 停止转发标记0:无效1:停止转发
    UINT8 uRev0[5];         ///< 预留0
    UINT8 DataLen;          ///< 负载长度
    UINT8 PayLoad[500];     ///< 负载数据

} DLL_CCL_UL_T;



/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/

/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/

/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
/**
 * @brief   获取IPC共享内存
 */
void _IPC_Shm();
/**
 * @brief   获取IPC信号量
 */
void _IPC_sem();
/**
 * @brief   获取CFG共享内存
 */
void _CFG_Shm();

/**
 * @brief  获取本地配置打印
 */
int _LocalCfgPrint();


/******************************************************************************
 *   调用函数声明
 *   *************************************************************************/

extern int sem_ipc_p(void);
extern int sem_ipc_v(void);
extern void GetSvnRevision();


#endif

