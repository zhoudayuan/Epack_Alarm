#ifndef  __CCL_H__
#define __CCL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl.h
 * @detail  �������뵥Ԫ���п��Ʋ�ӿ�Э�顢�궨�塢�ṹ�塢����
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-23
 */
/*
 *   �޸���ʷ
 *   2016-07-23                                           �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/

#include <signal.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include "sap.h"
#include "ccl_thread.h"



/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/

/******************************************************************************
 *   �궨��
 *   *************************************************************************/

#define LONGTITUDE                  1
#define LATITUDE                    0
#define GPS_LATITUDE_LEN            10
#define GPS_LONGITUEDE_LEN          9
#define GPS_SPEED_LEN               3
#define GPS_DIRECTION_LEN           3
#define GPS_DATE_LEN                6
#define GPS_TIME_LEN                6


/**
 * @def  false
 * @brief  �߼���
 */
#ifndef false
#define false                       0
#endif

/**
 * @def   true
 * @brief �߼���
 */
#ifndef true
#define true                        1
#endif

/**
 * @def
 * @brief����ظ����
 */
#define    ACK_OK                   0x04
#define    ACK_FAIL                 0x05
/**
 * @def
 * @brief PTT  ����״̬
 */
#define    CMD_PTT_ON               0x06
#define    CMD_PTT_OFF              0x07
/**
 * @def   INF_SHARE_HEAD_LEN
 * @brief ���뵥Ԫ�����Ľ�������ͷ����
 */
#define    CENTER_SHARE_HEAD_LEN    28
/**
 * @def   Lc_Header_Len
 * @brief  ����LC ͷ����
 */
#define    LC_HEADER_LEN            9
/**
 * @def   TD_LC_Len
 * @brief  ����LC ͷ����
 */
#define    TD_LC_LEN                9

/**
 * @def  PTT_ACK_VALID_LEN
 * @brief  CCL��ظ�PTT������Ч���ݳ���
 */
#define    PTT_ACK_VALID_LEN                100
/**
 * @def  PTT_CMD_VALID_LEN
 * @brief  CCL�㷢��PTT������Ч���ݳ���
 */
#define    PTT_CMD_VALID_LEN                100

/**
 * @def  SMS_ACK_VALID_LEN
 * @brief  CCL�ظ�������Ч���ݳ���
 */
#define    SMS_ACK_VALID_LEN                12

/**
 * @def   CCL_SMS_DL_HEADLEN
 * @brief  CCL�����Ϣ����ͷ����
 */
#define    CCL_SMS_DL_HEADLEN               5


/**
 * @def   DLL_CCL_MSG_HEADLEN
 * @brief  ��·��CCL����Ϣͷ����
 */
#define    DLL_CCL_MSG_HEADLEN              20



/**
 * @def  CENTER_VOICE_DL_HEADLEN
 * @brief ������������֡ͷ����
 */
#define    CENTER_VOICE_DL_HEADLEN          32//20

/**
 * @def  CENTER_VOICE_DL_PAYLOADLEN
 * @brief ��������������Ч���س���
 */
#define    CENTER_VOICE_DL_PAYLOADLEN       27
/**
 * @def  SILENT_FRAME_DATA_LEN
 * @brief ����֡��Ч���ݳ���
 */
#define    SILENT_FRAME_DATA_LEN            27
/**
 * @def   RTP_DATA_SIZE
 * @brief  RTP  ���ݰ�������ݳ���
 */

#define      RTP_MAX_DATA_SIZE              27 //36
/**
 * @def  DLL_VOICE_UL_PAYLOADLEN
 * @brief ��·���������س���
 */
#define    DLL_VOICE_UL_PAYLOADLEN          27
/**
 * @def DB_DATA_SIZE
 * @brief ������Ϣ��Ч���ݳ���
 */

//#define             DB_DATA_SIZE          (24)
/**
 * @def DLL_CCL_SMS_PAYLOADLEN
 * @brief DLL-CCL �����Ϣ�����Ч���س���
 */

#define    DLL_CCL_SMS_PAYLOADLEN           480

/**
* @def CC_CCL_MSGPSRPT_PAYLODDLEN
* @brief  cc �����ն�GPS������Ч���س���
*/

#define    CC_CCL_MSGPSRPT_PAYLODDLEN       4
/**
* @def CC_CCL_SMS_PAYLODDLEN
* @brief  CC ��CCL �����Ϣ�����Ч���ݳ���
*/

#define    CC_CCL_SMS_PAYLODDLEN            512

/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/

/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/


 /******************************************************************************
 *   �ṹ������
 *   *************************************************************************/

/**
 * @struct _CC_CVID_SUBNET_DATA
 * @brief  ���ĺ��С�����ID������������
 */
typedef struct  _CC_CVID_SUBNET_DATA
{
    unsigned char SubNet;
    unsigned int  CallId;           // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;          // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char srcid[3];
    unsigned char destid[3];
} CC_CVID_SUBNET_DATA;

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
 * @struct  _PTT_CMD
 * @brief  PTT-ON  PTT-OFF  ����ṹ��
 */
typedef struct _PTT_CMD
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;          // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char PttStat;          // PTT����״̬ ��ʶPTT����״̬������CMD_ON=0x06���ͷ�CMD_OFF=0x07��
    unsigned char Reserved;         // �ֽڶ���ռλ����������Ĳ���
    unsigned char TalkingNum[30];   // ��ʶ����ͨ��PTT�Ľ���������
    unsigned char CallingNum[30];   // ��ʶ���κ��е����к���
    unsigned char CalledNum[30];    // ��ʶ���κ��еı��к��루��š����ţ�
} PTT_CMD;

/**
 * @struct  _PTT_ON_ACK
 * @brief ��Ӧ����ѹPTT����
 */
typedef struct _PTT_ON_ACK
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;          // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char Ack;              // ��ʶPTT�ظ�������ɹ�ACK_OK=0x04��ʧ��ACK_FAIL=0x05��
    unsigned char Reserved;         // �ֽڶ���ռλ����������Ĳ���
    unsigned char TalkingNum[30];   // ��ʶ����ͨ��PTT�Ľ���������
    unsigned char CallingNum[30];   // ��ʶ���κ��е����к���
    unsigned char CalledNum[30];    // ��ʶ���κ��еı��к��루��š����ţ�
} PTT_ON_ACK;

/**
 * @struct  _PTT_ON_ACK
 * @brief ��Ӧ�����ͷ�PTT����
 */
typedef struct _PTT_OFF_ACK
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;          // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char Reserved[2];      // �ֽڶ���ռλ����������Ĳ���
    unsigned char TalkingNum[30];   // ��ʶ����ͨ��PTT�Ľ���������
    unsigned char CallingNum[30];   // ��ʶ���κ��е����к���
    unsigned char CalledNum[30];    // ��ʶ���κ��еı��к��루��š����ţ�
} PTT_OFF_ACK;

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


/**
 * @struct   _SMS_ACK
 * @brief  ���뵥Ԫ�����ķ��Ͷ���Ϣ�ظ� �ṹ��
 */
typedef struct _SMS_ACK
{
    CENTER_CMD_SHARE_HEAD   SharedHead;
    unsigned int  CallId;           // ������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;          // ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char SourceStat;       // ��ʶ���͵�Ԫ�豸״̬�����С����ϡ�æ�ȣ�
    unsigned char Ack;              // ��ʶ����Ϣ�ظ����(�ɹ�ACK_OK=0x04��ʧ��ACK_FAIL=0x05)
    unsigned char Reserved[2];
}    SMS_ACK;

/**
 * @struct   _SMS_CCL_DLL_EN
 * @brief CLL    ���Ͷ���Ϣ��DLL
 */
typedef struct _SMS_CCL_DLL_EN
{
    unsigned short Identification;     // Ipc4��ͷʶ��ֵ
    unsigned char  DestIpId:4;         // Ŀ��IP��ַ����
    unsigned char  SrcIpId:4;          // ԴIP��ַ����
    unsigned char  SrcPortId:7;        // Դ�˿�����
    unsigned char  CmpresOpcode1:1;    // ѹ��ͷ���������Чλ
    unsigned char  DestPortId:7;       // Ŀ�Ķ˿�����
    unsigned char  CmpresOpcode2:1;    // ѹ��ͷ���������Чλ
   // unsigned  short      ExtHeader1; // ��ѡ��չͷ
 //   unsigned  short      ExtHeader2;
    unsigned char     SmsPayLoad [480];
} SMS_CCL_DLL_EN;


/**
 * @struct   _DT_VOICE_LC_HEADER 9bytes
 * @brief  ����LCͷ, �ýṹ��ͬ�� NAS_PRE_LC_PDU
 */
typedef struct _DT_VOICE_LC_HEADER
{
    unsigned char CtrOpcode:6;   // uFLCO, ���ó�0
    unsigned char Reserved:1;    // uREV, ���ó�0
    unsigned char ProtectFlg:1;  // uPF,
    unsigned char FeatureId;     // uFID, ���ó�0
    unsigned char ServiceOpt;    // uSO
    unsigned char GroupAddr[3];
    unsigned char SourceAddr[3];
}  DT_VOICE_LC_HEADER;



/**
 * @struct   LC_HEADER_DATA
 * @brief  ����LC ͷ
 */
typedef struct _LC_HEADER_DATA
{
    unsigned char SourceId[3];          // ԴID
    unsigned char DesId[3];             // Ŀ��ID
}  LC_HEADER_DATA;

/**
 * @struct   _DT_VOICE_LC_TERMINATOR
 * @brief  ���� ����֡
 */
typedef struct _DT_VOICE_LC_TERMINATOR
{
    unsigned char ProtectFlg:1;
    unsigned char Reserved1:1;
    unsigned char CtrOpcode:6;
    unsigned char FeatureId;
    unsigned char DestLinkId[3];
    unsigned char SrcLinkId[3];
    unsigned char CallType:1;           // Group or Individual
    unsigned char ResponseReq:1;
    unsigned char FullMsgFlg:1;
    unsigned char Reserved2:1;
    unsigned char ReSyncFlg:1;
    unsigned char SendSeqNum:3;
}  DT_VOICE_LC_TERMINATOR;

/**  ����LCͷ���ս�֡�ṹ
 * @struct DT_VOICE_LC_EN
 * @brief
 */
typedef struct _DT_VOICE_LC_EN
{
    unsigned char Flco:       6;      // Ϊ  0 ���
    unsigned char Reserved:   1;      // ���ó�0
    unsigned char ProtectFlg: 1;      // ���ó�0
    unsigned char FeatureId;          // ���ó�0
    unsigned char ServiceOpt;         //  0�Ǽ���
    unsigned char GroupAddr[3];
    unsigned char SourceAddr[3];
} DT_VOICE_LC_EN;

/**
 * @struct DLL_CCL_UL_T
 * @brief ������·���е�CCL�ṹ��
 */
/*typedef struct _DLL_CCL_UL_T
{
    unsigned char MsgType;          ///< 0x01:���� 0x02:���� 0x03:�ڵ�
    unsigned char FrmType;          ///< ��ö�� _FrameType_E
    unsigned char DataType;         ///< ��������
    unsigned char SrcId[3];         ///< ԴID
    unsigned char DstId[3];         ///< Ŀ��ID
    unsigned char uRev0[10];        ///< Ԥ��0
    unsigned char DataLen;          ///< ���س���
    unsigned char PayLoad[500];     ///< ��������

} DLL_CCL_UL_T;

*/

/**
 * @struct  _Rtp_Fix_Head
 * @brief RTP �̶�ͷ
 */

typedef struct   _Rtp_Fix_Head
{
    unsigned char  version:2;
    unsigned char  padding_flag:1;
    unsigned char  ex_head_flag:1;
    unsigned char  csrc_count:4;
    unsigned char  marker:1;
    unsigned char  payload_type:7;
    unsigned short sequence_num;
    unsigned int   time_stamp;
    unsigned int   ssrc;
} Rtp_Fix_Head;
/**
 * @struct  _Rtp_Ex_Head
 * @brief RTP ��չͷ
 */


typedef struct _Rtp_Ex_Head
{
    unsigned short profile_head;
    unsigned short exhead_length;      // 32bit number == 4 bytes number
    unsigned int  voice_id;
    unsigned int  gps;
    unsigned char codec_flag;
    unsigned char frame_flag;
    unsigned char path_num;             // show which path voice 0xff indicates invalid value, "0" indicates the first path
    unsigned char pack_rate;           // packet rate "20" or "60" milliseconds
    unsigned char encrypt_flag;
    unsigned char encode_flag;
    unsigned char encode_frame;
    unsigned char enconde_rate;
} Rtp_Ex_Head;

/**
 * @struct   _Rtp_Repeater_Ex_Head
 * @brief RTP ת����չ
 */
typedef struct  _Rtp_Repeater_Ex_Head
{
    unsigned short   profile_head;
    unsigned short   exhead_length;      /* 32bit number == 4 bytes number*/
    unsigned int     voice_id;
    unsigned int     gps;
    unsigned char    codec_flag;
    unsigned char    frame_flag;
    unsigned short   reserve;
} Rtp_Repeater_Ex_Head;


/**
 * @struct   _CENTER_VOICE_DATA
 * @brief �����������ݰ���ʽ
 */
typedef struct _CENTER_VOICE_DATA
{
    Rtp_Fix_Head    Rtp_Head;
    Rtp_Ex_Head     Rtp_Ex;
    unsigned char   Payload[ RTP_MAX_DATA_SIZE];
    unsigned char   Reserve_Data[5];
} CENTER_VOICE_DATA;


/**
 * @struct   _CENTER_VOICE_DATA_DL
 * @brief ��������֡���нṹ
 */

typedef  struct  _CENTER_VOICE_DATA_DL
{
    unsigned char  Version:2;
    unsigned char  Padding:1;       // �����
    unsigned char  ExtendHead:1;    // ��չͷ���
    unsigned char  CsrcCount:4;     // ����Դ����
    unsigned char  Marker:1;        // RTP��Ҫ�¼����
    unsigned char  PayloadType:7;   // RTP ��������
    unsigned short SequenceNum;     // RTP���к�
    unsigned int   TimeStamp;       // RTPʱ���
    unsigned int   Ssrc;            // RTPͬ��Դ
    unsigned short ProfileHead;     // rtp��չͷ���Բ���
    unsigned short ExheadLen;       // RTP��չͷ���ݳ���
    unsigned int   VoiceId;         // ��ʶһ�κ��е�ҵ����ID
    unsigned int   Gps;             // GPSʱ��
    unsigned char  CodecFlg;        // ���ݱ���������ʶ
    unsigned char  FrameFlg;        // ����֡��ʶ
    unsigned char  PathNum;         // ����ͨ�����룬��������ͨ��������ȫF
    unsigned char  PackRate;        // ��ʶ����������
    unsigned char  EncryptFlag;     // ��ʶ�Ƿ����
    unsigned char  Reserve[3];
    unsigned char  VoiceData[507];
    unsigned char  ReserveData[5];
 }CENTER_VOICE_DATA_DL;
/**
 * @struct   _CCL_STAT_INFO_HEAD
 * @brief CCL ״̬��Ϣͷ
 */
typedef struct _CCL_STAT_INFO_HEAD
{
    unsigned short MsgHead;             //��ʶAC�ӿ�����ͷ��־(0x13ec)
    unsigned short MsgType;             //��������    ��ʶAC�ӿ�PTT�������0x000a��
    unsigned short Datalength;          //��Ч���ݳ���  ��ʶ������Ч���ݳ��ȣ�100��
    unsigned short SourceID;            //Դ��Ԫ��            ��ʶAC�ӿڷ��͵�ԪIDֵ
    unsigned short DestID;              //Ŀ�ĵ�Ԫ��     ��ʶAC�ӿڽ��յ�ԪIDֵ
    unsigned char  SourceType;          //Դ��Ԫ�豸����       ��ʶ���͵�Ԫ�豸���ͣ���ʽ��
    unsigned char  SigVersion;          //����汾��       ��ʶ�������ʺϰ汾��0x02(��������0x01�汾������)
    unsigned char  PathNum;             //ͨ������         ��ʶ���������ڸõ�Ԫ��ͨ�����룬��������ͨ��������ȫF
    unsigned char  ChangeMode;          //����ģʽ    0x00  ��ģʽ   0x01 ��ģʽ    0xff ��Ч
    unsigned char  ConnectMode;         //����ģʽ    0x00  �Զ�ģʽ    0x01�ֶ�ģʽ
    unsigned char  Exhead[9];           //��չͷ           ��չͷ����������Ĳ���
    unsigned int   CheckVal;            // У��ֵ      ��ʶ����У����(01�汾 ��02�汾 Ŀǰ����дȫF����ʾ��У�鷽ʽ)

}CCL_STAT_INFO_HEAD;
/**
 * @struct   _CCL_STAT_INFO
 * @brief CCL ״̬��Ϣ
 */

typedef  struct _CCL_STAT_INFO
{
    CCL_STAT_INFO_HEAD  StatInfoHead;
    unsigned int  CallId;               //��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;              //��ʶһ�κ��е�ҵ����ID
    unsigned char SourceStat;           //��ʶ���͵�Ԫ�豸״̬�����С����ϡ�æ�ȣ�
    unsigned char CallidSn;             //��ʶ���͵�Ԫ�����������ӳ��������
    unsigned char CallingId;            //��ʶ���Ͷ˵�Ԫ���е�Ԫ��
    unsigned char CalledId;             //��ʶ���Ͷ˵�Ԫ���е�Ԫ��
}CCL_STAT_INFO;



/**
 * @struct   _GPS_DC_INFO
 * @brief ������·�㵽���п��Ʋ�GPS��Ϣ
 */
typedef  struct _GPS_DC_INFO
{
  unsigned char     TimeH[3];           //GPS ʱ�����λ
  unsigned char     TimeL[3];           //GPS ʱ�����λ
  unsigned int      N_S:1;              //����
  unsigned int      Latiude:31;         //γ��
  unsigned int      W_E:1;              //����
  unsigned int      Longtiude:31;       //����
  unsigned short    Speed;              //�ٶ�
  unsigned short    Direct;             //����
}GPS_DC_INFO;


/**
* @struct  GPS_DATA_DLL_T
* @brief  GPS NAS   DLL�����ݽṹ
*/
//#pragma pack(1)__attribute__((aligned(1)))
typedef struct _GPS_DATA_DLL_T
{
    UINT8  NS: 1;           //<1bit γ��ָʾ 0:SOUTH 1:NORTH
    UINT8  NDEG: 7;         //<7bit γ��    0~89
    UINT8  EW: 1;           //<1bit ����ָʾ 1:WEST 0:EAST
    UINT8  NMINmm: 7;       //<6bit γ�ֵ��������� 0~59
    UINT16 NMINF;           //<14bit γ�ֵ�С������ 0~9999
    UINT8  EDEG;            //<8bit ����     0~179
    UINT8  EMINmm;          //<6bit ���ֵ��������� 0~59
    UINT16 EMINF;           //<14bit ���ֵ�С������ 0~9999
    UINT8  YEAR;            ///<�� 0��99
    UINT8  MONTH;           ///<�� 1��12
    UINT8  DAY;             ///<�� 1��31
    UINT8  HOUR;            ///<ʱ 0��23
    UINT8  MIN;             ///<�� 0~59
    UINT8  SEC;             ///<�� 0~59
}__attribute__((packed,aligned(1))) GPS_DATA_DLL_T;

/**
* @struct  GPS_DATA_CCL_T
* @brief  GPS NAS   CCL�����ݽṹ
*/

typedef struct _GPS_DATA_CCL_T
{
    //gps
    unsigned char  NS: 1;           //<1bit γ��ָʾ 0:SOUTH 1:NORTH
    unsigned char  NDEG: 7;         //<7bit γ��    0~89
    unsigned char  EW: 1;           //<1bit ����ָʾ 1:WEST 0:EAST
    unsigned char  NMINmm: 7;       //<6bit γ�ֵ��������� 0~59
    unsigned short NMINF;           //<14bit γ�ֵ�С������ 0~9999
    unsigned char  EDEG;            //<8bit ����     0~179
    unsigned char  EMINmm;          //<6bit ���ֵ��������� 0~59
    unsigned short EMINF;           //<14bit ���ֵ�С������ 0~9999
    //ʱ��
    char  DATE[14];
   /* unsigned char  MONTH[2];           ///<:    �� 1��12
    unsigned char  DAY[2];             ///<:    �� 1��31
    unsigned char  HOUR[2];            ///<:    ʱ 0��23
    unsigned char  MIN[2];             ///<:    �� 0~59
    unsigned char  SEC[2];             ///<:    �� 0~59*/
}__attribute__((packed,aligned(1))) GPS_DATA_CCL_T;

/**
* @struct  GPS_DATA_CC_T
* @brief  GPS NAS   CC�����ݽṹ
*/

typedef struct _GPS_DATA_CC_T
{
    unsigned char   CALLEDID[4];      //�����豸ID
    GPS_DATA_CCL_T  GPSCCLDATA;
}__attribute__((packed,aligned(1))) GPS_DATA_CC_T;



/**
* @struct   _LP__DC_INFO
* @brief ������·�㵽���п��Ʋ�LP��Ϣ
*/
typedef  struct _LP__DC_INFO
{
    unsigned char MsgHdr;
    unsigned char  OptCode[2];
    struct     GPS_DC_INFO;
}LP_DC_INFO;
/**
* @struct  _GPS__CC_INFO
* @brief ���п��Ʋ㵽����GPS��Ϣ
*/
typedef  struct _GPS__CC_INFO
{
    unsigned char Status;
    unsigned char Time[GPS_TIME_LEN];
    unsigned char Data[GPS_TIME_LEN];
    unsigned char N_S;
    unsigned char Latitude[GPS_LATITUDE_LEN];
    unsigned char W_E;
    unsigned char Longtitude[GPS_LONGITUEDE_LEN];
    unsigned char Speed[GPS_SPEED_LEN];
    unsigned char Direct[GPS_DIRECTION_LEN];
}GPS__CC_INFO;



typedef union _SHARE_CC_DATA_D
{
    PTT_CMD  CC_PTT_CMD;        // ������̨����
    PTT_OFF_ACK CC_Ptt_Ack;     // ����PTT������Ӧ
    SMS_CENTER_CCL_DL CcSmsSig; // ���Ķ���Ϣ����
} SHARE_CC_DATA_D;



/******************************************************************************
 *   ö������
 *   *************************************************************************/
/**
 * @enum    SMS_DATA_CALL_TYPE
 * @brief   ������Ϣ��������, ���������������һ�ccl_dl.c�е� table_sms_type_print Ҳ����֮����
 */
 typedef enum _SMS_DATA_CALL_TYPE
 {
    //����Ϣ
    MESSAGE_PRIVATE_CALL    =0x00,          //����
    MESSAGE_GROUP_CALL      =0x01,          //���
    STATUS_PRIVATE_CALL     =0x02,          //״̬����
    STATUS_GROUP_CALL       =0x03,          //״̬���

   //����
    STUN_REQ_MS             =0x07,          //ң��MS
    STUN_REQ_NAS            =0x08,          //ң��NAS
    KILL_REQ_NAS            =0x09,          //ң��NAS
    GPS_REPOTR_MS           =0x0a,          //��GPS
    GPS_REPORT_NAS          =0x0b,
    REVIVE_REQ_NAS          =0x0c,
    REVIVE_REQ_MS           =0x0d,
    NEGHR_QUERY             =0x0e,          //�ڵ��ѯ

    //��Ӧ
    NEGHR_QUERY_ACK         =0x20,          //�ڵ��ѯ��Ӧ
    GPS_REPROT_MS_ACK       =0x21,          //��Ӧ��MS  GPS
    GPS_REPROT_NAS_ACK      =0x22,          //��Ӧ��NAS  GPS
    STUN_REQ_MS_ACK         =0x23,
    STUN_REQ_NAS_ACK        =0x24,
    KILL_REQ_NAS_ACK        =0X25,
    REVIVE_REQ_NAS_ACK      =0x26,          //����NAS
    REVIVE_REQ_MS_ACK       =0x27,          //����MS
    NAS_NEAR_REPORT         =0x28,          //�����ڵ��ϱ�
    VARIANCE_HRESHOLD       =0x29,          //�������ޣ���ǿֵ��

    //����
    STUN_NAS_REPORT         =0x30,
    KILL_NAS_REPORT         =0x31,
    REVIVE_NAS_REPORT       =0x32,

    //�澯
    DISCON_ALARM            =0x40,
    MS_ALARM                =0x41,
    DISCON_NAS_ALARM_CLEAR  =0x42,
    MS_ALARM_CLEAR          =0x43

 } SMS_DATA_CALL_TYPE;

/**
* @struct _SMS_FORMAT
* @brief  ����Ϣ��ʽ
*/
 typedef  enum  _SMS_FORMAT
 {
    ASCII        =0X00,
    UNICODE      =0X01,
}SMS_FORMAT;

/**
 * @enum     _CENTER _SIG_TYPE_EN
 * @brief  ���뵥Ԫ��������������
 */
typedef enum _INF_SIG_TYPE_EN
{
    SIG_STATUS_REPORT = 0x0001,  // NAS ״̬�ϱ�
    SIG_PTT_CMD       = 0X000a,  // PTT ���� ����
    SIG_PTT_ON_ACK    = 0x000b,  // PTT ON �ظ� ����
    SIG_SMS_SEND      = 0x000e,  // ����Ϣ����
    SIG_SMS_ACK       = 0x0014,  // ����Ϣ�ظ�����
    SIG_PTT_OFF_ACK   = 0x0015   // PTT OFF �ظ� ����
}INF_SIG_TYPE_EN;

/**
 * @enum     _INF_CCL_STATE_EN
 * @brief  ���п��Ʋ�״̬
 */
typedef  enum  _INF_CCL_STATE_EN
{
    INF_CCL_IDLE        =0x01,      //���п��Ʋ����
    CENTER_VOICE_DL     =0x02,      //������������
    Wait_Stun_Ms_Ack    =0x03,      //�ȴ�ҡ��MS��Ӧ
    Wait_Stun_Nas_Ack   =0x04,      //�ȴ�ҡ��WLU��Ӧ״̬
    Wait_Kill_Nas_Ack   =0x05,      //WLU ҡ��
    Wait_Gps_Nas_Ack    =0x06,
    Wait_Gps_Ms_Ack     =0x07,
    Wait_NEGHR_Nas_Ack  =0x08,
    Wait_Reviv_NAS_Ack  =0x09,
    Wait_Reviv_MS_Ack   =0x0b,
    NAS_VOICE_UL        =0x0c,
    NAS_STAT_STUNED     =0x18,      //�豸ҡ��
    INF_CCL_DISABLE     =0x19,
    SMS_SEND            =0X1a       //����Ϣ����
}INF_CCL_STATE_EN;


typedef enum _NAS_WORK_STATE_E
{
    NAS_STOP_REKAY              =1,
    NAS_NORMAL_STATE            =5,         ///< ����̬
    NAS_STUNNED_STATE           =6,         ///< ҡ��̬
    NAS_KILLED_STATE            =7,         ///< ҡ��̬
    NAS_WAIT_STUN_ACK_STATE     =8,         ///< �ȴ�ҡ����Ӧ̬
    NAS_WAIT_KILL_ACK_STATE     =9,         ///< �ȴ�ҡ����Ӧ̬

} NAS_WORK_STATE_E;

typedef enum _NEAR_REPORT_TYPE_E
{
    NEAR_REPORT_ACTIVE            =0,       ///<�ڵ������ϱ�
    NEAR_REPORT_PASSIVE           =1,       ///< �ڵ��ѯ�ϱ�
}NEAR_REPORT_TYPE_E;


/******************************************************************************
*   �ڲ�����ʵ��
*   *************************************************************************/

/******************************************************************************
*   �ɵ��ú���ʵ��
*   *************************************************************************/
/**
* @brief
*/

/**
* @brief
*/


#endif //UINT_INF_H

