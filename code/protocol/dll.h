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

//GPS ͬ��ʱ��250*60 ms=15000
#define SYNC_CYC                 250

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
#define DPF_UDT_PACKET           ((UINT8)0)    ///< Unified Data Transport (UDT), UDP����
#define DPF_RESPONSE_PACKET      ((UINT8)1)    ///< Response packet, ��Ӧ����(PDT����ʹ��)
#define DPF_UNCONFIRM_PACKET     ((UINT8)2)    ///< Data packet with unconfirmed delivery  ��ȷ�����ݷ���
#define DPF_CONFIRM_PACKET       ((UINT8)3)    ///< Data packet with confirmed delivery   ��ȷ�����ݷ���
#define DPF_DEFINED_SD_PACKET    ((UINT8)13)   ///< Short Data: Defined  Ԥ���������
#define DPF_RAW_SD_PACKET        ((UINT8)14)   ///< Short Data: Raw or Status/Precoded  ԭʼ���ݻ�Ԥ����״̬����
#define DPF_PROP_DATA_PACKET     ((UINT8)15)   ///< Proprietary Data Packet  ר�����ݷ���
#define DPF_PDT_E2E_ENCRYPT      ((UINT8)4)    ///< PDT end-to-end encrypt  �˵��˼�������ͷ
#define DPF_E2E_NVOC_UNCONFIRM_PACKET   ((UINT8)6)   ///< Proprietary Data Packet  ר�����ݷ���
#define DPF_E2E_NVOC_CONFIRM_PACKET     ((UINT8)7)   ///< Proprietary Data Packet  ר�����ݷ���

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
#define R34_C_DATA_DLEN          ((UINT8)18)//��2�ֽ�ͷУ�鼰��ŵĳ���
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
#define INRANGE         8       //��������3bitһ������ݣ�ÿ���������ݷ�Χ0~7
#define INSTATIC        8       //״̬���Ŀ���״ֵ̬����Χ0~7����һ��״̬Ϊ0״̬����i��״̬Ϊ��һ��������ֵ��
#define INDATA          49      //����144bit,3bitһ�飬��״̬��ת�����48��0~15�������ӵ�49����0��ȷ��״̬��������״̬��
#define INSYMBOL        49      //��������
#define PATH            64      //��i(48>=i>=2)���������ݶ�Ӧ��64����֧������������64��·����
#define SELECTPATH      8       //��֧������С��8��·����ѡ����С��һ��·���������Ϊ�����������
#define BITELEN         9       //
/** @} */

/** @defgroup CSBK������ */
/** @{ */
#define PRE_CSBKO               0x3d  // Ԥ�ز�
#define REQ_CSBKO_DMR           0x24  // Common Signalling Request CSBKO FOR DMR
#define REQ_CSBKO               0x29  // Common Signalling Request CSBKO
#define ACK_CSBKO               0x2c  // Common Signalling Answer Response CSBKO
#define ALARM_CSBKO             0x37  // Digital Alarm Service Request CSBKO
#define ALARM_CSBKO_DMR         0x27  // Digital Alarm Service Request CSBKO For DMR
/** @} */

/** @defgroup ����ҵ�������� */
/** @{ */
#define CALL_ALERT_SSO          0x05  // Call Alert Service
#define EN_RADIO_SSO            0x09  // Radio Enable Service
#define DIS_RADIO_SSO           0x0a  // Radio Disable Service
#define DIGITAL_ALARM_SSO       0x0f  // ��ֵ�ݶ�-Digital Alarm Service
#define DIGITAL_ALARM_SSO_DMR   0x08  // ��ֵ�ݶ�-Digital Alarm Service FOR DMR
#define EN_RADIO_REV_DMR        0x7e  // Radio Enable Service for DMR
#define DIS_RADIO_REV_DMR       0x7f  // Radio Disable Service for DMR
#define EN_RADIO_REV_ACK_DMR    0xfe  // Radio Enable Service for DMR ACK
#define DIS_RADIO_REV_ACK_DMR   0xff  // Radio Disable Service for DMR ACK
/** @} */
#define FLC_GPS_ON      1
#define FLC_GPS_OFF     0

#define EMB_HDR       0
#define EMB_GPS         1

#define PLAN_HDR       0
#define PLAN_GPS         1

#define SND_FLC_ONCE       0   // GPS��Ƕֻ����һ��
#define SND_FLC_MULTI      1   // GPS��Ƕ���ڷ��Ͷ��


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
#define     DISCON_HAPPEN            1     // ��⵽�ڵ�������֣�
#define     DISCON_RECOVER           0     // ��⵽�ڵ�����ѻָ�����ǰΪ������״̬��

#define SIZE_ONE_FRAME		(UINT8)27
#define	SIZE_SUPER_FRAME	(UINT16)(6*SIZE_ONE_FRAME)		//һ����֡
#define BUFFSIZE 			(UINT16)(10*SIZE_SUPER_FRAME)  	//10����֡

#define PRE_CSBK_NONE_FLAG 0
#define PRE_CSBK_GPS_FLAG 1

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
    //----�����ʲ���
    CALL_ERR_VOICE_U = 0x07                      ///< �����ʲ���-�������ж�ʱ

} CALL_STATE_TYPE_E;

/**
 * @enum SAP_TYPE_E
 * @brief ��·��ҵ����������
 */
typedef enum _SAP_TYPE_E
{
    S_LC_HDR            = 0,                      ///< LCͷ
    S_LC_TER            = 1,                      ///< LC�ս�
    S_VOICE_A           = 2,
    S_VOICE_B           = 3,
    S_VOICE_C           = 4,
    S_VOICE_D           = 5,
    S_VOICE_E           = 6,
    S_VOICE_F           = 7,
    S_CSBK_PRE          = 8,                      ///< Ԥ�ز�
    S_C_HDR             = 9,                      ///< ȷ��ͷ
    S_U_HDR             = 10,                     ///< ��ȷ��ͷ
    S_RE_HDR            = 11,                     ///< ��Ӧͷ
    S_SD_HDR            = 12,                     ///< �Զ���ͷ
    S_DATA_1_2          = 13,                     ///< 1/2����
    S_DATA_3_4          = 14,                     ///< 3/4����
    S_TD_LC             = 15,                     ///< �����ս�

    S_NEGR_BST          = 20,                     ///< �ڵ�ͻ��
    S_NEGR_RPT          = 21,                     ///< �ڵ��ϱ�

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

    S_NAS_RELAY         = 60,                     ///< NAS ת��
    S_NAS_PRE_LC        = 61,                     ///< NAS gps����
    
	S_PI_HDR			= 62,					  ///< PIͷ
	S_E_HDR				= 63					  ///< EIͷ

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
    UINT32 NMINF;           ///<: 17;  γ�ֵ�С������ 0~99999
    UINT8  EDEG;            ///<: 8;   ����     0~179
    UINT8  EMINmm;          ///<: 6;   ���ֵ��������� 0��59
    UINT32 EMINF;           ///<: 17;  ���ֵ�С������ 0��99999
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
    UINT8 MsgType;         ///< 0x01:�ڵ��ѯ0x02:GPS��ѯ
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
    UINT8 MsgType;         ///< 0x01:�ڵ��ѯ0x02:GPS��ѯ
    UINT8 AckTag:2;        ///< ��Ӧ���
    UINT8 Opcode:6;        ///< ������
    UINT8 SrcId:4;         ///< ԴID
    UINT8 DstId:4;         ///< Ŀ��ID
    UINT8 PayLoad[8];      ///< ���ݸ���
    UINT8 Crc;             ///< 8CrcУ��

} WLU_DATA_DL_T;

/**
 * @struct NAS_NEGR_BURST_T
 * @brief NAS�ڵ�ͻ��������·�ṹ��
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
    UINT8  auUploadLC[12];             ///< ʱ϶����LC���� 9�ֽ���Ч
    UINT8  auUploadPI[12];             ///< ʱ϶����PI����
    UINT8  auMSCC;                     ///< �ն�ɫ���ʾ
    UINT8  auWLUCC;                    ///< ��·��ɫ���ʾ
    UINT8  auPI;                       ///< ���ܱ�ʾ
    UINT8  auSubNet;                   ///< �տ�����ID
    UINT8  auWorkMode;                 ///< ����ģʽ PDT or DMR
    UINT8  auDownloadLC[12];           ///< ʱ϶����LC����12�ֽ���Ч
    UINT8  auDownloadPI[12];           ///< ʱ϶����PI����
    UINT8  auEmbInfo[16];              ///< ������Ƕ����32bit
    UINT8  auCCflag;                   ///< Զ���������ı�ʾ
    UINT8  auNegrBuSw;                 ///< �ڵ�ͻȻ����1:on,  0:off
    UINT8  auNegrCyc;                  ///< �ڵ�ͻ������(��λ���ӣ���С1)
    UINT8  auNodeId;                   ///< ���ؽڵ�ID
    UINT8  auSoureId[3];               ///< һ���ԴID
    UINT8  auGroupId[3];               ///< һ�����ID
    UINT8  auNasGps[8];                ///< һ���GPS����
    UINT8  auMsGps[8];                 ///< �ն�GPS����
    UINT16 auSevHangTime;              ///< ҵ�����ʱ��
    UINT32 auStunFlag;                 ///< ң�α��
    UINT32 auKillFlag;                 ///< ң�б��
    UINT32 auNegrId1;                  ///< �ڵ���Ϣ1
    UINT32 auNerInfo1[32];             ///< �ڵ�������Ϣ 1
    UINT32 auNegrId2;                  ///< �ڵ���Ϣ2
    UINT32 auNerInfo2[32];             ///< �ڵ�������Ϣ 2
	UINT8  auVoiceType;				   ///< ��������6 AMBE/7 NVOC
	UINT8  auEncryption_Type;		   ///< ������������1.hytera�������� 2.hytera�߼����� 3.dmra ����
	UINT8  ucFlcGpsSwitch;		       ///< NAS ������������ǶGPS����
	UINT8  ucFlcGpsSec;                ///< ��λʱ���·�GPS��Ƕ������Ϊ��λ���60s
    UINT32 ulFlcGpsCnt;                ///< ��ucFlcGpsSecתΪ��ʱ������ֵ
    UINT8  ucFlcGpsSndFlg;             ///< ucFlcGpsSndFlg = 0 ʹ�ܷ���һ�Σ� ucFlcGpsSndFlg = 1�����ڷ���
}DLL_GLB_CFG_T;

/**
 * @struct  NAS_PRE_LC_PDU
 * @brief ����NASԤռLC
 * note:
 * Full Link Control (FULL LC) PDU
 * 96bits
 */
typedef struct _NAS_PRE_LC_PDU
{
    UINT8 uFLCO :6;   // CtrOpcode ������·����������
    UINT8 uREV  :1;   // Reserved
    UINT8 uPF   :1;   // ProtectFlg ������־��Ӧ����Ϊ0
    UINT8 uFID;       // FeatureId: standard:hytera:moto:pdt.
    UINT8 uSO;        // ServiceOpt, ҵ��ѡ��
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 auCRC[3];

} NAS_PRE_LC_PDU;

/**
 * @struct  DMRA_PI_PDU
 * @brief DMRA���ܷ�ʽ��PI�ṹ
 * note: 
 * Full Link Control (FULL LC) PDU
 * 96bits
 */
typedef struct _DMRA_PI_PDU
{
    UINT8 uAlgId :3;   // See Algorithm ID table
    UINT8 uRsv2  :2;   // 0
    UINT8 uGI    :1;   // Group 1;Individual:0
    UINT8 uRsv1  :2;   // 0
    UINT8 uFID;        // Shall be set to 0x10(moto)
    UINT8 uKeyID;      // 1-255
    UINT8 auIV[4];
    UINT8 auTADDR[3];
    UINT8 auCRC[2];

} DMRA_PI_PDU;
/**
 * @struct  HYTERA_PI_PDU_PDT
 * @brief Hytera�߼�����PDT_PI
 * note: 
 * Full Link Control (FULL LC) PDU
 * 96bits
 */
typedef struct _HYTERA_PI_PDU_PDT
{
    UINT8 uFID          ;   // 0x68
    UINT8 uEncryType  :4;   // 2
    UINT8 uRsv1       :4;   // 0
    UINT8 uKeyID        ;   // 1-255
    UINT8 auIV[5]       ;
    UINT8 uRsv2         ;   // 0
    UINT8 uChechSum     ;
    UINT8 auCRC[2];

} HYTERA_PI_PDU_PDT;

/**
 * @struct  HYTERA_PI_PDU_DMR
 * @brief Hytera�߼�����DMR_PI
 * note: 
 * Full Link Control (FULL LC) PDU
 * 96bits
 */
typedef struct _HYTERA_PI_PDU_DMR
{
    UINT8 uEncryType  :4;   // 2
    UINT8 uRsv1       :4;   // 0
    UINT8 uFID          ;   // 0x68
    UINT8 uKeyID        ;   // 1-255
    UINT8 auIV[5]       ;
    UINT8 uRsv2         ;   // 0
    UINT8 uChechSum     ;
    UINT8 auCRC[2];


} HYTERA_PI_PDU_DMR;


/**
 * @brief    ����R12��������������
 */
typedef union _PI_PDU
{
    DMRA_PI_PDU         dmraPiData;
    HYTERA_PI_PDU_PDT   hyteraPiPdtData;

} PI_PDU;

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
    UINT8 uGps :1;//GPSҵ���轫��λ����0
    UINT8 uR12 :1;//����δ��
    UINT8 uREV :4;//Ԥ������
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
    UINT8 uFID;  // Manufacturers Feature ID
    UINT8 uSSO;  // Supplementary Service Options
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
 * @brief PDT �տ�����ȷ������ͷ
 * uFSN: Ƭ��š���Ϣ��Ԫ����ʶ����������Ƭ��Ϣ�ı�ţ�һ�����һ���ϳ���������Ϣ
 *       FSN����߱������ڱ�ʶ���һƬ������ƬӦ��0��
 *       FSN�ĵ�3bitΪÿ��Ƭ�����, ��һƬӦΪ000������һ�ε���, �����ִﵽ111ʱ, ����һ��
 *       ӦΪ001������000. �ɵ���ȷ����Ϣ��ɵ��߼���Ϣ, FSN��ֵΪ1000
 *  uNS: �������
 */
typedef struct _PDT_C_HEAD_PDU_DT
{
    UINT8 uDPF          :4;     // ָʾ��ȷ�Ϻ���ȷ�ϣ�0011:��ȷ�Ϸ�ʽ
    UINT8 uRV1          :1;     // ����
    UINT8 uHC           :1;     // ����
    UINT8 uA            :1;     // 1:��Ҫ������Ӧ����֧�ֻ���
    UINT8 uGI           :1;     // Ŀ���û�Ϊ���˺���
    UINT8 uDEI          :2;     // ָʾ��Ч���ݵĽ��������������ͼ��ʽ
    UINT8 uRV2          :2;     // ����
    UINT8 uSAP          :4;     // ����ָʾ���ݷ����ҵ�����ͣ�1001:˽�з�������;1011:�˵������ݼ���ͷ֡
    UINT8 auTADDR[3];           // Ŀ���û����¼���ǩ,LDS��PDI��MS�տڵ�ַ
    UINT8 auSADDR[3];           // Դ�û����¼���ǩ��LDS��PDI��MS�տڵ�ַ
    UINT8 uBF           :7;     // �����ݰ��к������֡������
    UINT8 uFMF          :1;     // Full Message Flag ���ڱ�ʾ���δ����Ƿ����������ݻ����ش��Ĳ������� 0:�ش����ݰ�; 1:��һ�δ���������
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
    UINT8 uSAP          :4;  // 0011-UDP/ip ѹ��ͷ
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;
    UINT8 uFMF          :1;  // Full Message Flag ���ڱ�ʾ���δ����Ƿ����������ݻ����ش��Ĳ������� 0:�ش����ݰ�; 1:��һ�δ���������
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
    UINT8 uDPF          :4;  // ���ݷ������ 0001-Response, 0010-unconfirmed, 0011-confirmed
    UINT8 uRV1          :2;
    UINT8 uA            :1;  // ��Ӧ�����ʶ 0-No Response, 1-Response
    UINT8 uRV2          :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;  // �������� 0010-TCP/IP header compression, 0011-UDP/IP header compression
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;  // ����֡��
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
    UINT8 uA            :1;  // ��Ӧ�����ʶ 0-No Response, 1-Response
    UINT8 uRV2          :1;
    UINT8 uRV3          :4;
    UINT8 uSAP          :4;  // 10-TCP/IPѹ��ͷ, 11-UDP/IPѹ��ͷ
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uBF           :7;  // ����֡��
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
 * @struct PDT_E_HEAD_PDU_DT
 * @brief PDT �տ����м���˽��ͷ
 */
typedef struct _PDT_E_HEAD_PDU_DT
{
    UINT8 uDPF          :4;  
    UINT8 uSAP          :4;
    UINT8 uFID; 
    UINT8 uEncryType    :4;
    UINT8 uOpcode       :4;
    UINT8 uKeyID; 
    UINT8 auIV[5];
    UINT8 uRsv2;
    UINT8 auCRC[2];

} PDT_E_HEAD_PDU_DT;

/**
 * @struct PDT_E_HEAD_PDU_UT
 * @brief PDT �տ����м���˽��ͷ
 */
typedef struct _PDT_E_HEAD_PDU_UT
{
    UINT8 uDPF          :4;  
    UINT8 uSAP          :4;
    UINT8 uFID; 
    UINT8 uEncryType    :4;
    UINT8 uOpcode       :4;
    UINT8 uKeyID; 
    UINT8 auIV[5];
    UINT8 uRsv2;

} PDT_E_HEAD_PDU_UT;

/**
 * @struct DMR_E_HEAD_PDU_DT
 * @brief DMR �տ����м���˽��ͷ
 */
typedef struct _DMR_E_HEAD_PDU_DT
{
    UINT8 uDPF          :4;  
    UINT8 uSAP          :4;
    UINT8 uFID; 
    UINT8 uOpcode       :4;
    UINT8 uAlgID        :3;
    UINT8 uRsv1         :1;
    UINT8 uKeyID; 
    UINT8 auRsv2[2];
    UINT8 auIV[4];
    UINT8 auCRC[2];

} DMR_E_HEAD_PDU_DT;

/**
 * @struct DMR_E_HEAD_PDU_UT
 * @brief DMR �տ����м���˽��ͷ
 */
typedef struct _DMR_E_HEAD_PDU_UT
{
    UINT8 uDPF          :4;  
    UINT8 uSAP          :4;
    UINT8 uFID; 
    UINT8 uOpcode       :4;
    UINT8 uAlgID        :3;
    UINT8 uRsv1         :1;
    UINT8 uKeyID; 
    UINT8 auRsv2[2];
    UINT8 auIV[4];

} DMR_E_HEAD_PDU_UT;

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
    UINT8 uDBSN                     :7;  // Data Block Serial Number
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
    UINT8 uDBSN                     :7;  // ����֡���, ���ڸ������ڵ�����֡���,0~M-1��ε�����MΪ��������ͷ֡�е�BFֵ
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
    UINT8 uFLCO                     :6;  // ������·������ TD_LC=0X30
    UINT8 uRV                       :1;
    UINT8 uPF                       :1;  // ������ʶ
    UINT8 uFID;                          // ���ܼ�
    UINT8 auTADDR[3];
    UINT8 auSADDR[3];
    UINT8 uNS                       :3;  // ������� N(S) Send sequence Number
    UINT8 uS                        :1;  // ����ͬ����ʶ S(Re-Synchronize flag)
    UINT8 uRV1                      :1;  // ����
    UINT8 uF                        :1;  // ��Ϣ������ʶ FMF(Full Message Flag)
    UINT8 uA                        :1;  // Response Request ��Ӧ�����ʶ
    UINT8 uGI                       :1;
    UINT8 uCRC[3];

} TD_LC_PDU_T;

/**
 * @brief   ���з�������������Ϣ�ṹ��
 */
typedef struct _PDP_UT
{
    UINT8  uDPF; // Data Packet Format, ����ʶ�����ݰ�, 0-UDT,1-Response,2-unconfirmed,3-confirmed ... ...
    UINT8  uBF;  // Blocks to Follow, Number of blocks to follow
    UINT8  uSAP; // Service Access Point
    UINT8  uPOC; //
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
    UINT8  uDPF;           ///< DPF(Data Packet Format)����
    UINT8  uState;         ///< ��ǰ�������ݷ���״̬  @note ����/��һ�η���/�ش�
    UINT8  uSAP;           ///< SAP���ͣ��������� UDT(ͳһ���ݴ���)/TCP IP/UDP IP/IP��������/��ַ����Э��ARP/...
    UINT8  uPOC;
    UINT8  uDEI;           ///< ���ݽ�����ʶ
    UINT8  uNS;
    UINT8  uDataType;      ///< ��������
    UINT8  uHdrNum;        ///< ���з�������ͷ���ܸ���
    UINT8  uBLOCKNum;      ///< ���з����������ݰ����� @note ������CHEAD,P_HEAD��E_HEAD
    UINT8  uPHeadNum;      ///< ���з���������P_Head����
    UINT8  uEHeadNum;      ///< ���з���������E_Head����
    UINT8  s_uDBSNIndex;   ///< ���з����������ݰ�����     @������P_HEAD��E_HEAD
    UINT8  BF;             ///< ֻ��C_HEADͷ֮֡������з�������֡����, ����֡��(BF),�涨�˺������ж��ٸ�����֡,����EHEAD��PHEAD
    UINT8  uTdLcNum;
    UINT8  uTdLcCnt;
    UINT8  uSrcId[3];
    UINT8  uDstId[3];
    UINT8  auResHead[12];  ///< ���з������ݻظ�ȷ��֡ͷ֡
    UINT8  auResData[16];  ///< ���з������ݷ��ͽ��ͼ��         @ÿ1bit����һ��֡��1Ϊ���ͳɹ���0Ϊ����ʧ��
    UINT8  auHead[3][12];  ///< ���з�������ͷ           @����C_HEAD��P_HEAD��E_HEAD
    UINT8  auData[1000];   ///< TSC-CHU ���з�������SDU��@����HEAD��payload
    UINT8  auTdLc[12];     ///<

} PDP_DT;



/******************************************************************************
 *  ����������
 *   *************************************************************************/
typedef union _PDP_HEAD_DU
{
    PDT_NC_HEAD_PDU_DT p_PdpNcHdr;  // �տ�������ȷ������ͷ
    PDT_C_HEAD_PDU_DT  p_Pdp_CHdr;  // �տ�����ȷ������ͷ
    PDT_RE_HEAD_PDU_DT p_PdpReHdr;  // �տ�����Ӧ������ͷ
    PDT_E_HEAD_PDU_DT  p_Pdp_EHdr;  // �տ����м���˽��ͷ
    DMR_NC_HEAD_PDU_DT d_PdpNcHdr;  // �տ�������ȷ������ͷ
    DMR_C_HEAD_PDU_DT  d_Pdp_CHdr;
    DMR_RE_HEAD_PDU_DT d_PdpReHdr;
    DMR_E_HEAD_PDU_DT  d_Pdp_EHdr;  // �տ����м���˽��ͷ
    DMR_P_HEAD_PDU_DT  d_Pdp_PHdr;
    DMR_SP_HEAD_PDU_DT d_PdpSpHdr;
    DMR_RA_HEAD_PDU_DT d_PdpRaHdr;
    DMR_DE_HEAD_PDU_DT d_PdpDeHdr;
    DMR_UD_HEAD_PDU_DT d_PdpUdHdr;

} PDP_HEAD_DU;

typedef union _PDP_HEAD_UU
{
    PDT_NC_HEAD_PDU_UT p_PdpNcHdr;  // �տ�������ȷ������ͷ
    PDT_C_HEAD_PDU_UT  p_Pdp_CHdr;  // �տ�����ȷ������ͷ
    PDT_RE_HEAD_PDU_UT p_PdpReHdr;  // �տ�����Ӧ������ͷ
    PDT_E_HEAD_PDU_UT  p_Pdp_EHdr;  // �տ����м���˽��ͷ
    DMR_NC_HEAD_PDU_UT d_PdpNcHdr;  // �տ�������ȷ������ͷ
    DMR_C_HEAD_PDU_UT  d_Pdp_CHdr;  // �տ�����ȷ������ͷ
    DMR_RE_HEAD_PDU_UT d_PdpReHdr;  // �տ�����Ӧ������ͷ
    DMR_E_HEAD_PDU_UT  d_Pdp_EHdr;  // �տ����м���˽��ͷ
    DMR_P_HEAD_PDU_UT  d_Pdp_PHdr;  // DMR�տ�����˽������ͷ
    DMR_SP_HEAD_PDU_UT d_PdpSpHdr;  // DMR�տ�����״̬��Ԥ��������ͷ
    DMR_RA_HEAD_PDU_UT d_PdpRaHdr;  // DMR�տ�����ԭʼ������ͷ
    DMR_DE_HEAD_PDU_UT d_PdpDeHdr;  // DMR�տ�����Ԥ��������ͷ
    DMR_UD_HEAD_PDU_UT d_PdpUdHdr;  // DMR�տ�����ͳһ���ݴ�������ͷ

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
    R34_NC_DATA_UT  tR34NcData;   // ������ȷ��R34֡����
    R34_NC_LDATA_UT tR34NcLData;  // ������ȷ��R34β֡����
    R34_C_DATA_UT   tR34CData;    // ������ȷ��R34֡����
    R34_C_LDATA_UT  tR34CLData;   // ������ȷ��R34β֡����

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

extern INT8 g_NerbValidFlg;
extern UINT32 g_BurstCnt;
extern void DLL_ClearTimer(void);
extern void DLL_SetTimer(UINT8 CallState, UINT16 CallWait);
extern void CheckLocalNerInfoPrint(void);
extern void CheckOtherNerInfoPrint(unsigned char *pucData);
extern void CheckNerInfo(unsigned char *pucNodeID, unsigned long *pulNerID, const char *Prompt);
extern void CheckNerArray();
extern void CheckNerMatrixPrint();

extern BOOL _DMRA_GetParam(UINT8 *NEXTIV,UINT8 AlgID);


#endif

