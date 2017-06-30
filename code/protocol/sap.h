#ifndef __SAP_H__
#define __SAP_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    sap.h
 * @brief   ����ҵ��ģ�����͡��������ṹ���궨��
 * @author  ������
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   �޸���ʷ
 *   2016-7-20                 ������                     �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   �궨��
 *   *************************************************************************/
#undef TASK_NAME
#define TASK_NAME   "SAP"


/******************************************************************************
 *   ����ͷ�ļ�����
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
 *   ö������
 *   *************************************************************************/
/**
 * @enum CCL_DATA_TYPE_E
 * @brief CCL����֡����
 */
typedef enum _CCL_DATA_TYPE_E
{
    //�տ���������DC_MSG_DATA
    CT_JUNK_DATA            = 0x00,                      ///< ��������Ϣ
    CT_PI_HEADER            = 0x01,                      ///< PIͷ֡��ʶ
    CT_LC_HEADER            = 0x02,                      ///< LCͷ֡��ʶ
    CT_LC_TERMINATOR        = 0x03,                      ///< LC�ս�֡��ʶ
    CT_PACKET_DATA          = 0x04,                      ///< ����Ϣ

    CT_GPS_REPORT_REQ_MS    = 0x11,                      ///< ��̨GPS����
    CT_GPS_REPORT_ACK_MS    = 0x12,                      ///< ��̨GPS����
    CT_DISABLE_REQ_MS       = 0x13,                      ///< ҡ��
    CT_DISABLE_ACK_MS       = 0x14,                      ///< ҡ��
    CT_ENABLE_REQ_MS        = 0x15,                      ///< �ն˼���
    CT_ENABLE_ACK_MS        = 0x16,                      ///< �ն˼���
    CT_ALARM_REQ_MS         = 0x17,                      ///< �ն˽����澯
    CT_ALARM_ACK_MS         = 0x18,                      ///< �ն˽����澯

    //��·����������DC_MSG_WLU
    CT_GPS_REPORT_REQ_NAS   = 0x21,                      ///< GPS����
    CT_GPS_REPORT_ACK_NAS   = 0x22,                      ///< GPS����
    CT_STUN_REQ_NAS         = 0x23,                      ///< ҡ��
    CT_STUN_ACK_NAS         = 0x24,                      ///< ҡ��
    CT_KILL_REQ_NAS         = 0x25,                      ///< ҡ��
    CT_KILL_ACK_NAS         = 0x26,                      ///< ҡ��
    CT_ENABLE_REQ_NAS       = 0x27,                      ///< WLU  ����
    CT_ENABLE_ACK_NAS       = 0x28,                      ///< WLU  ����
    CT_STUN_RPT_NAS         = 0x29,                      ///< WLU  ң���ϱ�
    CT_KILL_RPT_NAS         = 0x2a,                      ///< WLU  ң���ϱ�
    CT_ENABLE_RPT_NAS       = 0x2b,                      ///< WLU  �����ϱ�

    CT_NEGHR_QUERY          = 0x31,                      ///< �ڵ���Ϣ��ѯ
    CT_NEGHR_QUERY_ACK      = 0x32,                      ///< �ڵ���Ϣ��Ӧ
    CT_NEGHR_REPORT         = 0x33,                      ///< �ڵ���Ϣ�ϱ�
    CT_DISCON_ALARM         = 0x34,                      ///< �����澯
    CT_DISCON_ALARM_CLEAR   = 0x35,                      ///< �����澯���

} CCL_DATA_TYPE_E;

/**
 * @struct CCL_DLL_DL_T
 * @brief CCL���е�������·�ṹ��
 */
typedef struct _CCL_DLL_DL_T
{
    UINT8 MsgType;          ///< 0x01:���� 0x02:���� 0x03:�ڵ�
    UINT8 FrmType;          ///< ��ö�� _FrameType_E
    UINT8 DataType;         ///< ��������
    UINT8 SrcId[3];         ///< ԴID
    UINT8 DstId[3];         ///< Ŀ��ID
    UINT8 uRev0[10];        ///< Ԥ��0
    UINT8 DataLen;          ///< ���س���
    UINT8 PayLoad[500];     ///< ��������

} CCL_DLL_DL_T;

/**
 * @struct DLL_CCL_UL_T
 * @brief ������·���е�CCL�ṹ��
 */
typedef struct _DLL_CCL_UL_T
{
    UINT8 MsgType;          ///< 0x01:���� 0x02:���� 0x03:�ڵ�
    UINT8 FrmType;          ///< ��ö�� _FrameType_E
    UINT8 DataType;         ///< ��������
    UINT8 SrcId[3];         ///< ԴID
    UINT8 DstId[3];         ///< Ŀ��ID
    UINT8 Vari[2];          ///< ����FD ������Ч
    UINT8 Rssi[2];          ///< ��ǿֵ
    UINT8 Prohibit;         ///< ֹͣת�����0:��Ч1:ֹͣת��
    UINT8 uRev0[5];         ///< Ԥ��0
    UINT8 DataLen;          ///< ���س���
    UINT8 PayLoad[500];     ///< ��������

} DLL_CCL_UL_T;



/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/

/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/

/******************************************************************************
 *   �ڲ�����ʵ��
 *   *************************************************************************/
/**
 * @brief   ��ȡIPC�����ڴ�
 */
void _IPC_Shm();
/**
 * @brief   ��ȡIPC�ź���
 */
void _IPC_sem();
/**
 * @brief   ��ȡCFG�����ڴ�
 */
void _CFG_Shm();

/**
 * @brief  ��ȡ�������ô�ӡ
 */
int _LocalCfgPrint();


/******************************************************************************
 *   ���ú�������
 *   *************************************************************************/

extern int sem_ipc_p(void);
extern int sem_ipc_v(void);
extern void GetSvnRevision();


#endif

