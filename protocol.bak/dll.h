#ifndef __DLL_H__
#define __DLL_H__

/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    dll.h
 * @brief   ����������·��Э��ȫ�����͡��������ṹ���궨��
 * @author  ������
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   �޸���ʷ
 *   2016-7-20                 ������                     �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "sap.h"


/******************************************************************************
 *   �궨��
 *   *************************************************************************/
/** @defgroup �������Ͷ��� */
/** @{ */
//typedef char           INT8;    /*  1                   1   */
//typedef unsigned char  UINT8;   /*  1                   1   */
//typedef signed short   INT16;   /*  2                   2   */
//typedef unsigned short UINT16;  /*  2                   2   */
//typedef signed long    INT32;   /*  4                   4   */
//typedef unsigned long  UINT32;  /*  4                   4   */

#define _F_                      __FUNCTION__

//GPS ͬ��ʱ��1000*60 ms
#define SYNC_CYC                 1000

/** @defgroup  ���еȴ�ʱ�䶨�� */
/** @{ */
#define CALL_VOICE_WAIT          ((UINT16)0x0006)
#define CALL_DATA_WAIT           ((UINT16)0x0006)
#define CALL_NAS_WAIT            ((UINT16)0x0006)
/** @} */

/** @defgroup  FPGA����״̬���� */
/** @{ */
#define SERVR_IDLE               ((UINT8)0)
#define VOICE_RELY               ((UINT8)1)
#define MESSG_RELY               ((UINT8)2)
#define CALLI_SPON               ((UINT8)4)
/** @} */

/** @defgroup  ҵ��״̬���� */
/** @{ */
#define WORK_START               ((UINT8)0)
#define WORK_STOP                ((UINT8)1)

/** @defgroup  ��ӡ״̬���� */
/** @{ */
#define PRINT_CLOSE               ((UINT8)0)
#define PRINT_DEBUG               ((UINT8)1)
#define PRINT_MEMRY               ((UINT8)2)
#define PRINT_ALL                 ((UINT8)3)
/** @} */

/** @defgroup �տ����ݲ��� */
/** @{ */
// DMR/PDT CRC MASK ����
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

// ����������
#define DPF_UDT_PACKET           ((UINT8)0)    ///< Unified Data Transport (UDT)
#define DPF_RESPONSE_PACKET      ((UINT8)1)    ///< Response packet
#define DPF_UNCONFIRM_PACKET     ((UINT8)2)    ///< Data packet with unconfirmed delivery
#define DPF_CONFIRM_PACKET       ((UINT8)3)    ///< Data packet with confirmed delivery
#define DPF_DEFINED_SD_PACKET    ((UINT8)13)   ///< Short Data: Defined
#define DPF_RAW_SD_PACKET        ((UINT8)14)   ///< Short Data: Raw or Status/Precoded
#define DPF_PROP_DATA_PACKET     ((UINT8)15)   ///< Proprietary Data Packet
#define DPF_PDT_E2E_ENCRYPT      ((UINT8)4)    ///< PDT end-to-end encrypt

// FID����
#define SFID                     ((UINT8)0x00)  //standard
#define HFID                     ((UINT8)0x68)  //hytera
#define MFID                     ((UINT8)0x10)  //moto
#define PFID                     ((UINT8)0x08)  //pdt

// ��������״̬
#define PDP_STATE_IDLE           ((UINT8)0x00)
#define PDP_STATE_OVER           ((UINT8)0x01)
#define PDP_STATE_FIRST          ((UINT8)0x02)
#define PDP_STATE_RETRY          ((UINT8)0x03)

// �������ݳ��ȶ���
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

// ���ݳ���
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

/** @defgroup CSBK������ */
/** @{ */
#define PRE_CSBKO               0x3d  // Ԥ�ز�
#define REQ_CSBKO               0x29  // Common Signalling Request CSBKO
#define ACK_CSBKO               0x2c  // Common Signalling Answer Response CSBKO
#define ALARM_CSBKO             0x37  // Digital Alarm Service Request CSBKO
/** @} */

/** @defgroup ����ҵ�������� */
/** @{ */
#define CALL_ALERT_SSO          0x05  // Call Alert Service
#define EN_RADIO_SSO            0x09  // Radio Enable Service
#define DIS_RADIO_SSO           0x0a  // Radio Disable Service
#define DIGITAL_ALARM_SSO       0x0f  // ��ֵ�ݶ�-Digital Alarm Service
/** @} */

/**
 * @def GPS_FlAG
 * @brief GPS��ʶ��
 */
#define GPS_FlAG                      (116)

/**
 * @def CCL_RECV_MAX
 * @brief ���պ��п���������󳤶�
 */
#define CCL_RECV_MAX                  (600)

/**
 * @def PDP_MAX
 * @brief ���ն���Ϣ������󳤶�
 */
#define PDP_MAX                       (67)

/**
 * @def INF_RECV_MAX
 * @brief ���սӿ�����������󳤶�
 */
#define INF_RECV_MAX                  (128)

/**
 * @def  CCL_HEAD_LEN
 * @brief CCLЭ��ͷ����
 */
#define CCL_HEAD_LEN                  (20)

/**
 * @def  NM_DATA_LEN
 * @brief �������ݳ���
 */
#define NM_DATA_LEN                   (11)

/**
 * @def DB_NODE_MAX
 * @brief �������ڵ���
 */
#define DB_NODE_MAX                   (512)

/**
 * @def DB_SIZE
 * @brief �ڵ����ݰ���󳤶ȶ���
 */
#define DB_SIZE                       (36)

#define GPS_REQ                ((UINT8)0x00)    //GPS����
#define GPS_ACK                ((UINT8)0x01)    //GPSӦ��
#define GPS_OK                 ((UINT8)0x02)    //GPS���
#define GPS_PRE                ((UINT8)0x03)    //GPSԤռ
#define GPS_RLY                ((UINT8)0x04)    //GPS��ת

#define NODE_TYPE_D            ((UINT8)0x00)    //���ݽڵ�
#define NODE_TYPE_V            ((UINT8)0x01)    //�����ڵ�

/**
 * @brief  �ڵ�����澯״̬��־
 */
#define     TURN_ON                  1     // �����ڵ�����澯����[��]
#define     TURN_OFF                 0     // �����ڵ�����澯����[�ر�]
#define     DISCON_DISABLE          -1     // û���ڵ�����澯����
#define     DISCON_HAPPEN            1     // ��⵽�ڵ�������֣�
#define     DISCON_RECOVER           0     // ��⵽�ڵ�����ָ�����ǰ������״̬��


/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/

/******************************************************************************
 *   ö������
 *   *************************************************************************/
/**
 * @enum CALL_STATE_TYPE_E
 * @brief ��·�㵽�ӿڲ���Ϣ����
 */
typedef enum _CALL_STATE_TYPE_E
{
    CALL_IDLE       = 0x00,                      ///< ���п���
    CALL_VOICE_U    = 0x01,                      ///< �������к���
    CALL_VOICE_D    = 0x02,                      ///< �������к���
    CALL_DATA_U     = 0x03,                      ///< �������к���
    CALL_DATA_D     = 0x04,                      ///< �������к���
    CALL_WLU_U      = 0x05,                      ///< ��·�����к���
    CALL_WLU_D      = 0x06,                      ///< ��·�����к���

} CALL_STATE_TYPE_E;

/**
 * @enum SAP_TYPE_E
 * @brief ��·��ҵ����������
 */
typedef enum _SAP_TYPE_E
{
    S_LC_HDR            = 0,                      ///< LCͷ
    S_LC_TER            = 1,                      ///< LC�ս�
    S_VOICE             = 2,                      ///< ����
    S_CSBK_PRE          = 3,                      ///< Ԥ�ز�
    S_C_HDR             = 4,                      ///< ȷ��ͷ
    S_U_HDR             = 5,                      ///< ��ȷ��ͷ
    S_RE_HDR            = 6,                      ///< ��Ӧͷ
    S_SD_HDR            = 7,                      ///< �Զ���ͷ
    S_DATA_1_2          = 8,                      ///< 1/2����
    S_DATA_3_4          = 9,                      ///< 3/4����
    S_TD_LC             = 10,                     ///< �����ս�

    S_NEGR_BST          = 20,                     ///< �ٵ�ͻ��
    S_NEGR_RPT          = 21,                     ///< �ٵ��ϱ�

    S_GPS_REQ           = 30,                     ///< gps����
    S_GPS_ACK           = 31,                     ///< gpsӦ��

    S_CSBK_REQ          = 32,                     ///< CSBK����
    S_CSBK_ACK          = 33,                     ///< CSBKӦ��

    S_KILL_REQ          = 34,                     ///< MS ң������
    S_KILL_ACK          = 35,                     ///< MS ң��Ӧ��

    S_ENABLE_REQ        = 36,                     ///< MS ʹ������
    S_ENABLE_ACK        = 37,                     ///< MS ʹ��Ӧ��

    S_ALARM_REQ         = 38,                     ///< MS �澯����
    S_ALARM_ACK         = 39,                     ///< MS �澯Ӧ��

    S_NAS_GPS_REQ       = 40,                     ///< NAS gps����
    S_NAS_GPS_ACK       = 41,                     ///< NAS gpsӦ��

    S_NAS_KILL_REQ      = 42,                     ///< NAS gps����
    S_NAS_KILL_ACK      = 43,                     ///< NAS gpsӦ��

    S_NAS_STUN_REQ      = 44,                     ///< NAS gps����
    S_NAS_STUN_ACK      = 45,                     ///< NAS gpsӦ��

    S_NAS_ENABLE_REQ    = 46,                     ///< NAS gps����
    S_NAS_ENABLE_ACK    = 47,                     ///< NAS gpsӦ��

    S_NAS_NEGR_REQ      = 48,                     ///< NAS gps����
    S_NAS_NEGR_ACK      = 49,                     ///< NAS gpsӦ��

    S_NAS_ALARM_REQ     = 50,                     ///< NAS �澯����
    S_NAS_ALARN_ACK     = 51,                     ///< NAS �澯Ӧ��

    S_NAS_RELAY         = 60,                     ///< NAS gps����
    S_NAS_PRE_LC        = 61,                     ///< NAS gps����

} SAP_TYPE_E;


/******************************************************************************
 *   �ṹ������
 *   *************************************************************************/
/**
 * @struct DBC_NODE_T
 * @brief  ����ڵ����ݽṹ
 */
typedef struct _DBC_NODE_T
{
    struct _DBC_NODE_T* ptPrev;          ///< ǰһ�ڵ�
    struct _DBC_NODE_T* ptNext;          ///< ��һ�ڵ�
    UINT8               auData[DB_SIZE]; ///< ֡����
    UINT32              u4Node;          ///< �ڵ���
    UINT8               eNodeType;       ///< �ڵ�����
    BOOL                bNdTrafCtrl;     ///< �Ƿ���Ҫ����

} DBC_NODE_T;

/**
 * @struct  GPS_AI_DATA_T
 * @brief  GPS�տ����ݽṹ
 */
typedef struct _GPS_AI_DATA_T
{
    UINT8  CmpsHdr[5];      ///< UDP ѹ��ͷ
    UINT8  MsgHdr;          ///< ��Ϣͷ
    UINT16 Opcode;          ///< ������
    UINT8  RequestID[4];    ///< ����ID
    UINT8  Result[2];       ///< ���
    UINT8  Time[3];         ///< gps time
    UINT8  Data[3];         ///< gps data
    UINT32 Latitude: 31;    ///< γ��
    UINT32 NS: 1;           ///< γ��ָʾ 0:SOUTH 1:NORTH
    UINT32 Longitude: 31;   ///< ����
    UINT32 EW: 1;           ///< ����ָʾ 0:EAST 1:WEST
    UINT8  Speed[2];        ///< �����ٶ�
    UINT8  Direction[2];    ///< �����

} GPS_AI_DATA_T;

/**
 * @struct  GPS_DEV_DATA_T
 * @brief  GPS�豸���ݽṹ
 */
typedef struct _GPS_DEV_DATA_T
{
    UINT8  NS;              ///<: 1;   γ��ָʾ 0:SOUTH 1:NORTH
    UINT8  EW;              ///<: 1;   ����ָʾ 0:EAST  1:WEST
    UINT8  NDEG;            ///<: 7;   γ��    0~89
    UINT8  NMINmm;          ///<: 6;   γ�ֵ��������� 0~59
    UINT32 NMINF;           ///<: 17; γ�ֵ�С������ 0~99999
    UINT8  EDEG;            ///<: 8;   ����     0~179
    UINT8  EMINmm;          ///<: 6;   ���ֵ��������� 0��59
    UINT32 EMINF;           ///<: 17; ���ֵ�С������ 0��99999
    UINT8  YEAR;            ///<: 7;   �� 0��99
    UINT8  MONTH;           ///<: 4;   �� 1��12
    UINT8  DAY;             ///<: 5;   �� 1��31
    UINT8  HOUR;            ///<: 5;   ʱ 0��23
    UINT8  MIN;             ///<: 6;   �� 0~59
    UINT8  SEC;             ///<: 6;   �� 0~59

}__attribute__((packed,aligned(1))) GPS_DEV_DATA_T;

/**
 * @struct  GPS_NAS_DATA_T
 * @brief  GPS NAS���ݽṹ
 */
typedef struct _GPS_NAS_DATA_T
{
    UINT8  NS: 1;           //<1bit γ��ָʾ 0:SOUTH 1:NORTH
    UINT8  NDEG: 7;         //<7bit γ��    0~89
    UINT8  EW: 1;           //<1bit ����ָʾ 0:EAST 1:WEST
    UINT8  NMINmm: 7;       //<6bit γ�ֵ��������� 0~59
    UINT16 NMINF;           //<14bit γ�ֵ�С������ 0~9999
    UINT8  EDEG;            //<8bit ����     0~179
    UINT8  EMINmm;          //<6bit ���ֵ��������� 0~59
    UINT16 EMINF;           //<14bit ���ֵ�С������ 0~9999

}__attribute__((packed,aligned(1))) GPS_NAS_DATA_T;

/**
 * @struct  IPv4_COMPRESS_HEADER_T
 * @brief  IPv4 ѹ��ͷ�ṹ
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
 * @brief  MS GPS �������ݽṹ
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
 * @brief WLU���е�������·�ṹ��
 */
typedef struct _WLU_DATA_UL_T
{
    UINT8 MsgType;         ///< 0x01:�ٵ��ѯ0x02:GPS��ѯ
    UINT8 AckTag:2;        ///< ��Ӧ���
    UINT8 Opcode:6;        ///< ������
    UINT8 SrcId:4;         ///< ԴID
    UINT8 DstId:4;         ///< Ŀ��ID
    UINT8 PayLoad[8];      ///< ���ݸ���
    UINT8 Crc;             ///< 8CrcУ��

}__attribute__((packed,aligned(1))) WLU_DATA_UL_T;

/**
 * @struct WLU_DATA_DL_T
 * @brief WLU���е�������·�ṹ��
 */
typedef struct _WLU_DATA_DL_T
{
    UINT8 MsgType;         ///< 0x01:�ٵ��ѯ0x02:GPS��ѯ
    UINT8 AckTag:2;        ///< ��Ӧ���
    UINT8 Opcode:6;        ///< ������
    UINT8 SrcId:4;         ///< ԴID
    UINT8 DstId:4;         ///< Ŀ��ID
    UINT8 PayLoad[8];      ///< ���ݸ���
    UINT8 Crc;             ///< 8CrcУ��

} WLU_DATA_DL_T;

/**
 * @struct NAS_NEGR_BURST_T
 * @brief NAS�ٵ�ͻ��������·�ṹ��
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
 * @brief ������
 */
typedef struct _DLL_GLB_CFG_T
{
    UINT8  auUploadLC[12];             ///< ʱ϶����LC����
    UINT8  auUploadPI[12];             ///< ʱ϶����PI����
    UINT8  auMSCC;                     ///< �ն�ɫ���ʾ
    UINT8  auWLUCC;                    ///< ��·��ɫ���ʾ
    UINT8  auPI;                       ///< ���ܱ�ʾ
    UINT8  auSubNet;                   ///< �տ�����ID
    UINT8  auWorkMode;                 ///< ����ģʽ PDT or DMR
    UINT8  auDownloadLC[12];           ///< ʱ϶����LC����
    UINT8  auDownloadPI[12];           ///< ʱ϶����PI����
    UINT8  auEmbInfo[16];              ///< ������Ƕ����32bit
    UINT8  auCCflag;                   ///< Զ���������ı�ʾ
    UINT8  auNegrBuSw;                 ///< �ٵ�ͻȻ����1:on,  0:off
    UINT8  auNegrCyc;                  ///< �ٵ�ͻ������(��λ���ӣ���С1)
    UINT8  auNodeId;                   ///< ���ؽڵ�ID
    UINT8  auSoureId[3];               ///< һ���ԴID
    UINT8  auGroupId[3];               ///< һ�����ID
    UINT8  auNasGps[8];                ///< һ���GPS����
    UINT8  auMsGps[8];                 ///< �ն�GPS����
    UINT16 auSevHangTime;              ///< ҵ�����ʱ��
    UINT32 auStunFlag;                 ///< ң�α��
    UINT32 auKillFlag;                 ///< ң�б��
    UINT32 auNegrId1;                  ///< �ٵ���Ϣ1
    UINT32 auNerInfo1[32];             ///< �ٵ�������Ϣ 1
    UINT32 auNegrId2;                  ///< �ٵ���Ϣ2
    UINT32 auNerInfo2[32];             ///< �ٵ�������Ϣ 2

}DLL_GLB_CFG_T;

/**
 * @struct  NAS_PRE_LC_PDU
 * @brief ����NASԤռLC
 */
typedef struct _NAS_PRE_LC_PDU
{
    UINT8 uFLCO :6;
    UINT8 uREV  :1;
    UINT8 uPF   :1;
    UINT8 uFID;
    UINT8 uSO;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 auCRC[3];

} NAS_PRE_LC_PDU;

/**
 * @struct  EMBBED_GPS_PDU
 * @brief ��ǶGPS���ݽṹ
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
 * @brief  ͨ��CSBK
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
 * @brief  Ԥ�ز�CSBK
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
 * @brief  ����ҵ��CSBK
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
 * @brief PDT �տ�������ȷ������ͷ
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
 * @brief PDT �տ�������ȷ������ͷ
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
 * @brief DMR �տ�������ȷ������ͷ
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
 * @brief DMR �տ�������ȷ������ͷ
 */
typedef struct _DMR_NC_HEAD_PDU_UT
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

} DMR_NC_HEAD_PDU_UT;

/**
 * @struct PDT_C_HEAD_PDU_DT
 * @brief PDT �տ�����ȷ������ͷ
 */
typedef struct _PDT_C_HEAD_PDU_DT
{
    UINT8 uDPF          :4;     // 0011:��ȷ�Ϸ�ʽ
    UINT8 uRV1          :1;     // ����
    UINT8 uHC           :1;     // ����
    UINT8 uA            :1;     // 1:��Ҫ������Ӧ����֧�ֻ���
    UINT8 uGI           :1;     // Ŀ���û�Ϊ���˺���
    UINT8 uDEI          :2;     // ���������ͼ��ʽ
    UINT8 uRV2          :2;     // ����
    UINT8 uSAP          :4;     // 1001:˽�з�������;1011:�˵������ݼ���ͷ֡
    UINT8 auTADDR[3];           // Ŀ���û����¼���ǩ,LDS��PDI��MS�տڵ�ַ
    UINT8 auSADDR[3];           // Դ�û����¼���ǩ��LDS��PDI��MS�տڵ�ַ
    UINT8 uBF           :7;     // �����ݰ��к������֡������
    UINT8 uFMF          :1;     // 0:�ش����ݰ�; 1:��һ�δ���������
    UINT8 uFSN          :4;     // ���ݰ��ķ�����ţ������߷���һ�����ݰ�(�������ظ���)���ͼ�1�����ڼ���Ƿ��ظ��ˡ�
    UINT8 uNS           :3;     // 1:���շ�ͬ�����ݰ��ķ������
    UINT8 uS            :1;     // ���ݷ�Ƭ��ţ�֧�ַ�Ƭ���ܲ�ʹ�ã�
    UINT8 auCRC[2];

} PDT_C_HEAD_PDU_DT;

/**
 * @struct PDT_C_HEAD_PDU_UT
 * @brief PDT �տ�����ȷ������ͷ
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
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uFMF          :1;
    UINT8 uFSN          :4;
    UINT8 uNS           :3;
    UINT8 uS            :1;

} PDT_C_HEAD_PDU_UT;

/**
 * @struct DMR_C_HEAD_PDU_DT
 * @brief DMR �տ�����ȷ������ͷ
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
 * @brief DMR �տ�����ȷ������ͷ
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
 * @brief PDT �տ�����Ӧ������ͷ
 */
typedef struct _PDT_RE_HEAD_PDU_DT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :2;
    UINT8 uA            :1;
    UINT8 uRV2          :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uRV4          :1;
    UINT8 uSTAT         :3;
    UINT8 uTYPE         :3;
    UINT8 uCLAS         :2;
    UINT8 auCRC[2];

} PDT_RE_HEAD_PDU_DT;

/**
 * @struct PDT_RE_HEAD_PDU_UT
 * @brief PDT �տ�����Ӧ������ͷ
 */
typedef struct _PDT_RE_HEAD_PDU_UT
{
    UINT8 uDPF          :4;
    UINT8 uRV1          :2;
    UINT8 uA            :1;
    UINT8 uRV2          :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uRV4          :1;
    UINT8 uSTAT         :3;
    UINT8 uTYPE         :3;
    UINT8 uCLAS         :2;

} PDT_RE_HEAD_PDU_UT;

/**
 * @struct DMR_RE_HEAD_PDU_DT
 * @brief DMR �տ�����Ӧ������ͷ
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
 * @brief DMR �տ�����Ӧ������ͷ
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
 * @brief DMR�տ�����˽������ͷ
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
 * @brief DMR�տ�����˽������ͷ
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
 * @brief DMR�տ�����״̬��Ԥ��������ͷ
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
 * @brief DMR�տ�����״̬��Ԥ��������ͷ
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
 * @brief DMR�տ�����ԭʼ������ͷ
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
 * @brief DMR�տ�����ԭʼ������ͷ
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
 * @brief DMR�տ�����Ԥ��������ͷ
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
 * @brief DMR�տ�����Ԥ��������ͷ
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
 * @brief DMR�տ�����ͳһ���ݴ�������ͷ
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
 * @brief DMR�տ�����ͳһ���ݴ�������ͷ
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
 * @brief    ������ȷ��R12֡����
 */
typedef struct _R12_NC_DATA_DT
{
    UINT8 auData[R12_NC_DATA_DLEN];

} R12_NC_DATA_DT;

/**
 * @brief    ������ȷ��R12֡����
 */
typedef struct _R12_NC_DATA_UT
{
    UINT8 auData[R12_NC_DATA_ULEN];

} R12_NC_DATA_UT;

/**
 * @brief    ������ȷ��R12β֡����
 */
typedef struct _R12_NC_LDATA_DT
{
    UINT8 auData[R12_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R12_NC_LDATA_DT;

/**
 * @brief    ������ȷ��R12β֡����
 */
typedef struct _R12_NC_LDATA_UT
{
    UINT8 auData[R12_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R12_NC_LDATA_UT;

/**
 * @brief    ������ȷ��R12֡����
 */
typedef struct _R12_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R12_C_DATA_DLEN-2];

} R12_C_DATA_DT;

/**
 * @brief    ������ȷ��R12֡����
 */
typedef struct _R12_C_DATA_UT
{
    UINT8 auData[R12_C_DATA_ULEN-2];

} R12_C_DATA_UT;

/**
 * @brief    ������ȷ��R12β֡����
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
 * @brief    ������ȷ��R12β֡����
 */
typedef struct _R12_C_LDATA_UT
{
    UINT8 auData[R12_C_DATA_ULEN-6];
    UINT8 auRV[6];

} R12_C_LDATA_UT;

/**
 * @brief    ������ȷ��R34֡����
 */
typedef struct _R34_NC_DATA_DT
{
    UINT8 auData[R34_NC_DATA_DLEN];

} R34_NC_DATA_DT;

/**
 * @brief    ������ȷ��R34֡����
 */
typedef struct _R34_NC_DATA_UT
{
    UINT8 auData[R34_NC_DATA_ULEN];

} R34_NC_DATA_UT;

/**
 * @brief    ������ȷ��R34β֡����
 */
typedef struct _R34_NC_LDATA_DT
{
    UINT8 auData[R34_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R34_NC_LDATA_DT;

/**
 * @brief    ������ȷ��R34β֡����
 */
typedef struct _R34_NC_LDATA_UT
{
    UINT8 auData[R34_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R34_NC_LDATA_UT;

/**
 * @brief    ������ȷ��R34֡����
 */
typedef struct _R34_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_DLEN-2];

} R34_C_DATA_DT;

/**
 * @brief    ������ȷ��R34֡����
 */
typedef struct _R34_C_DATA_UT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R34_C_DATA_ULEN-2];

} R34_C_DATA_UT;

/**
 * @brief    ������ȷ��R34β֡����
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
 * @brief    ������ȷ��R34β֡����
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
 * @brief    ������ȷ��R11֡����
 */
typedef struct _R11_NC_DATA_DT
{
    UINT8 auData[R11_NC_DATA_DLEN];

} R11_NC_DATA_DT;

/**
 * @brief    ������ȷ��R11֡����
 */
typedef struct _R11_NC_DATA_UT
{
    UINT8 auData[R11_NC_DATA_ULEN];

} R11_NC_DATA_UT;

/**
 * @brief    ������ȷ��R11β֡����
 */
typedef struct _R11_NC_LDATA_DT
{
    UINT8 auData[R11_NC_DATA_DLEN-4];
    UINT8 auCRC[4];

} R11_NC_LDATA_DT;

/**
 * @brief    ������ȷ��R11β֡����
 */
typedef struct _R11_NC_LDATA_UT
{
    UINT8 auData[R11_NC_DATA_ULEN-4];
    UINT8 auRV[4];

} R11_NC_LDATA_UT;

/**
 * @brief    ������ȷ��R11֡����
 */
typedef struct _R11_C_DATA_DT
{
    UINT8 uCRC9H                    :1;
    UINT8 uDBSN                     :7;
    UINT8 uCRC9L;
    UINT8 auData[R11_C_DATA_DLEN-2];

} R11_C_DATA_DT;

/**
 * @brief    ������ȷ��R11֡����
 */
typedef struct _R11_C_DATA_UT
{
    UINT8 auData[R11_C_DATA_ULEN-2];

} R11_C_DATA_UT;

/**
 * @brief    ������ȷ��R11β֡����
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
 * @brief    ������ȷ��R11β֡����
 */
typedef struct _R11_C_LDATA_UT
{
    UINT8 auData[R11_C_DATA_ULEN-6];
    UINT8 auRV[6];

} R11_C_LDATA_UT;

/**
 * @brief    ��·������PDU
 */
typedef struct _GEN_LC_PDU_T
{
    UINT8 uFLCO                     :6;
    UINT8 uRV                       :1;
    UINT8 uPF                       :1;
    UINT8 uFID;
    UINT8 uFLCD[7];
    UINT8 uCRC[3];

} GEN_LC_PDU_T;

/**
 * @brief    ������·����PDU
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
 * @brief    ������·�ս�֡
 */
typedef struct _TD_LC_PDU_T
{
    UINT8 uFLCO                     :6;
    UINT8 uRV                       :1;
    UINT8 uPF                       :1;
    UINT8 uFID;
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uNS                       :3;
    UINT8 uS                        :1;
    UINT8 uRV1                      :1;
    UINT8 uF                        :1;
    UINT8 uA                        :1;
    UINT8 uGI                       :1;
    UINT8 uCRC[3];

} TD_LC_PDU_T;

/**
 * @brief   ���з�������������Ϣ�ṹ��
 */
typedef struct _PDP_UT
{
    UINT8  uDPF;
    UINT8  uBF;
    UINT8  uSAP;
    UINT8  uPOC;
    UINT8  uDEI;
    UINT8  uNS;
    UINT8  uDataType;
    UINT8  uState;

    UINT8  uPHeadNum;    // ˽��ͷ����
    UINT8  uPHeadCnt;

    UINT8  uEHeadNum;    // ����ͷ����
    UINT8  uEHeadCnt;

    UINT8  uBLOCKNum;    // ���հ�����
    UINT8  uBLOCKCnt;

    UINT8  uTdLcNum;
    UINT8  uTdLcCnt;

    UINT8  uRetryNum;
    UINT8  uRetryCnt;

    UINT8  uSrcId[3];
    UINT8  uDstId[3];

    UINT8  auResHead[12];
    UINT8  auResData[16];    // 1:���ݳɹ�����

    UINT8  uReBlockNum;      // ���հ�����
    UINT8  uReBlockCnt;

    UINT8  auHead[3][12];
    UINT8  auData[1000];
    UINT8  auTdLc[12];

    UINT8  uBLOCKNum_R;         ///< ���з��������ش�֡֡����@note CHU������������������̨�ش����͵��ش�֡����
    UINT8  uBLOCKCnt_R;         ///< ���з��������ش�֡֡����
    UINT8  ucRetryNum;          ///< CHU ��CRC ERR(DBSN) ��Ҫ��̨�ش�����
    UINT8  ucErrDBSNCnt;        ///< һ�δ����У�������ĸ���

} PDP_UT;

/**
 * @brief   ���з�������������Ϣ�ṹ��
 */
typedef struct _PDP_DT
{
    UINT8  uDPF;                ///< DPF����
    UINT8  uState;              ///< ��ǰ�������ݷ���״̬  @note ����/��һ�η���/�ش�
    UINT8  uSAP;                ///< SAP����
    UINT8  uPOC;
    UINT8  uDEI;
    UINT8  uNS;
    UINT8  uDataType;           ///< ��������

    UINT8  uHdrNum;             ///< ���з�������ͷ���ܸ���
    UINT8  uBLOCKNum;           ///< ���з����������ݰ����� @note ������P_HEAD��E_HEAD

    UINT8  uPHeadNum;           ///< ���з���������P_Head����
    UINT8  uEHeadNum;           ///< ���з���������E_Head����

    UINT8  s_uDBSNIndex;        ///< ���з����������ݰ�����     @������P_HEAD��E_HEAD

    UINT8  BF;                  ///< ��ͷ֮֡������з�������֡����
    UINT8  uTdLcNum;
    UINT8  uTdLcCnt;

    UINT8  uSrcId[3];
    UINT8  uDstId[3];

    UINT8  auResHead[12];       ///< ���з������ݻظ�ȷ��֡ͷ֡
    UINT8  auResData[16];       ///< ���з������ݷ��ͽ��ͼ��         @ÿ1bit����һ��֡��1Ϊ���ͳɹ���0Ϊ����ʧ��

    UINT8  auHead[3][12];       ///< ���з�������ͷ           @����C_HEAD��P_HEAD��E_HEAD
    UINT8  auData[1000];        ///< TSC-CHU ���з�������SDU��@����HEAD��payload
    UINT8  auTdLc[12];          ///<

} PDP_DT;



/******************************************************************************
 *  ����������
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
    PDT_NC_HEAD_PDU_UT p_PdpNcHdr;
    PDT_C_HEAD_PDU_UT  p_Pdp_CHdr;
    PDT_RE_HEAD_PDU_UT p_PdpReHdr;
    DMR_NC_HEAD_PDU_UT d_PdpNcHdr;
    DMR_C_HEAD_PDU_UT  d_Pdp_CHdr;
    DMR_RE_HEAD_PDU_UT d_PdpReHdr;
    DMR_P_HEAD_PDU_UT  d_Pdp_PHdr;
    DMR_SP_HEAD_PDU_UT d_PdpSpHdr;
    DMR_RA_HEAD_PDU_UT d_PdpRaHdr;
    DMR_DE_HEAD_PDU_UT d_PdpDeHdr;
    DMR_UD_HEAD_PDU_UT d_PdpUdHdr;

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
 * @brief    ����R12��������������
 */
typedef union _R12_DU
{
    R12_NC_DATA_DT  tR12NcData;
    R12_NC_LDATA_DT tR12NcLData;
    R12_C_DATA_DT   tR12CData;
    R12_C_LDATA_DT  tR12CLData;

} R12_DU;

/**
 * @brief    ����R12��������������
 */
typedef union _R12_UU
{
    R12_NC_DATA_UT  tR12NcData;
    R12_NC_LDATA_UT tR12NcLData;
    R12_C_DATA_UT   tR12CData;
    R12_C_LDATA_UT  tR12CLData;

} R12_UU;

/**
 * @brief    ����R34��������������
 */
typedef union _R34_DU
{
    R34_NC_DATA_DT  tR34NcData;
    R34_NC_LDATA_DT tR34NcLData;
    R34_C_DATA_DT   tR34CData;
    R34_C_LDATA_DT  tR34CLData;

} R34_DU;

/**
 * @brief    ����R34��������������
 */
typedef union _R34_UU
{
    R34_NC_DATA_UT  tR34NcData;
    R34_NC_LDATA_UT tR34NcLData;
    R34_C_DATA_UT   tR34CData;
    R34_C_LDATA_UT  tR34CLData;

} R34_UU;

/**
 * @brief    ����R11��������������
 */
typedef union _R11_DU
{
    R11_NC_DATA_DT  tR11NcData;
    R11_NC_LDATA_DT tR11NcLData;
    R11_C_DATA_DT   tR11CData;
    R11_C_LDATA_DT  tR11CLData;

} R11_DU;

/**
 * @brief    ����R11��������������
 */
typedef union _R11_UU
{
    R34_NC_DATA_UT  tR11NcData;
    R34_NC_LDATA_UT tR11NcLData;
    R34_C_DATA_UT   tR11CData;
    R34_C_LDATA_UT  tR11CLData;

} R11_UU;

/**
 * @brief    ���з�������֡����������
 */
typedef union _PDP_DATA_DU
{
    R11_DU uR11Data;
    R34_DU uR34Data;
    R12_DU uR12Data;

} PDP_DATA_DU;

/**
 * @brief    ���з�������֡����������
 */
typedef union _PDP_DATA_UU
{
    R11_UU uR11Data;
    R34_UU uR34Data;
    R12_UU uR12Data;

} PDP_DATA_UU;



#endif

