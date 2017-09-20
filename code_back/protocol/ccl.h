#ifndef  __CCL_H__
#define __CCL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl.h
 * @detail  包含接入单元呼叫控制层接口协议、宏定义、结构体、变量
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-23
 */
/*
 *   修改历史
 *   2016-07-23                                           建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
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
 *   类型定义
 *   *************************************************************************/

/******************************************************************************
 *   宏定义
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
 * @brief  逻辑假
 */
#ifndef false
#define false                       0
#endif

/**
 * @def   true
 * @brief 逻辑真
 */
#ifndef true
#define true                        1
#endif

/**
 * @def
 * @brief信令回复结果
 */
#define    ACK_OK                   0x04
#define    ACK_FAIL                 0x05
/**
 * @def
 * @brief PTT  信令状态
 */
#define    CMD_PTT_ON               0x06
#define    CMD_PTT_OFF              0x07
/**
 * @def   INF_SHARE_HEAD_LEN
 * @brief 接入单元与中心交互公用头长度
 */
#define    CENTER_SHARE_HEAD_LEN    28
/**
 * @def   Lc_Header_Len
 * @brief  语音LC 头长度
 */
#define    LC_HEADER_LEN            9
/**
 * @def   TD_LC_Len
 * @brief  语音LC 头长度
 */
#define    TD_LC_LEN                9

/**
 * @def  PTT_ACK_VALID_LEN
 * @brief  CCL层回复PTT命令有效数据长度
 */
#define    PTT_ACK_VALID_LEN                100
/**
 * @def  PTT_CMD_VALID_LEN
 * @brief  CCL层发送PTT命令有效数据长度
 */
#define    PTT_CMD_VALID_LEN                100

/**
 * @def  SMS_ACK_VALID_LEN
 * @brief  CCL回复短信有效数据长度
 */
#define    SMS_ACK_VALID_LEN                12

/**
 * @def   CCL_SMS_DL_HEADLEN
 * @brief  CCL层短消息下行头长度
 */
#define    CCL_SMS_DL_HEADLEN               5


/**
 * @def   DLL_CCL_MSG_HEADLEN
 * @brief  链路层CCL层消息头长度
 */
#define    DLL_CCL_MSG_HEADLEN              20



/**
 * @def  CENTER_VOICE_DL_HEADLEN
 * @brief 中心下行语音帧头长度
 */
#define    CENTER_VOICE_DL_HEADLEN          32//20

/**
 * @def  CENTER_VOICE_DL_PAYLOADLEN
 * @brief 中心下行语音有效负载长度
 */
#define    CENTER_VOICE_DL_PAYLOADLEN       27
/**
 * @def  SILENT_FRAME_DATA_LEN
 * @brief 静音帧有效数据长度
 */
#define    SILENT_FRAME_DATA_LEN            27
/**
 * @def   RTP_DATA_SIZE
 * @brief  RTP  数据包最大数据长度
 */

#define      RTP_MAX_DATA_SIZE              27 //36
/**
 * @def  DLL_VOICE_UL_PAYLOADLEN
 * @brief 链路层语音负载长度
 */
#define    DLL_VOICE_UL_PAYLOADLEN          27
/**
 * @def DB_DATA_SIZE
 * @brief 信令消息有效数据长度
 */

//#define             DB_DATA_SIZE          (24)
/**
 * @def DLL_CCL_SMS_PAYLOADLEN
 * @brief DLL-CCL 层短消息最大有效负载长度
 */

#define    DLL_CCL_SMS_PAYLOADLEN           480

/**
* @def CC_CCL_MSGPSRPT_PAYLODDLEN
* @brief  cc 上拉终端GPS命令有效负载长度
*/

#define    CC_CCL_MSGPSRPT_PAYLODDLEN       4
/**
* @def CC_CCL_SMS_PAYLODDLEN
* @brief  CC 与CCL 层短消息最大有效数据长度
*/

#define    CC_CCL_SMS_PAYLODDLEN            512

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/

/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/


 /******************************************************************************
 *   结构体声明
 *   *************************************************************************/

/**
 * @struct _CC_CVID_SUBNET_DATA
 * @brief  中心呼叫、语音ID、子网段数据
 */
typedef struct  _CC_CVID_SUBNET_DATA
{
    unsigned char SubNet;
    unsigned int  CallId;           // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;          // 业务面ID 标识一次呼叫的业务面ID
    unsigned char srcid[3];
    unsigned char destid[3];
} CC_CVID_SUBNET_DATA;

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
 * @struct  _PTT_CMD
 * @brief  PTT-ON  PTT-OFF  命令结构体
 */
typedef struct _PTT_CMD
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;          // 业务面ID 标识一次呼叫的业务面ID
    unsigned char PttStat;          // PTT信令状态 标识PTT信令状态（请求CMD_ON=0x06、释放CMD_OFF=0x07）
    unsigned char Reserved;         // 字节对齐占位中心无须关心参数
    unsigned char TalkingNum[30];   // 标识当次通话PTT的讲话方号码
    unsigned char CallingNum[30];   // 标识当次呼叫的主叫号码
    unsigned char CalledNum[30];    // 标识当次呼叫的被叫号码（组号、个号）
} PTT_CMD;

/**
 * @struct  _PTT_ON_ACK
 * @brief 响应中心压PTT命令
 */
typedef struct _PTT_ON_ACK
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;          // 业务面ID 标识一次呼叫的业务面ID
    unsigned char Ack;              // 标识PTT回复结果（成功ACK_OK=0x04、失败ACK_FAIL=0x05）
    unsigned char Reserved;         // 字节对齐占位中心无须关心参数
    unsigned char TalkingNum[30];   // 标识当次通话PTT的讲话方号码
    unsigned char CallingNum[30];   // 标识当次呼叫的主叫号码
    unsigned char CalledNum[30];    // 标识当次呼叫的被叫号码（组号、个号）
} PTT_ON_ACK;

/**
 * @struct  _PTT_ON_ACK
 * @brief 响应中心释放PTT命令
 */
typedef struct _PTT_OFF_ACK
{
    CENTER_CMD_SHARE_HEAD SharedHead;
    unsigned int  CallId;           // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;          // 业务面ID 标识一次呼叫的业务面ID
    unsigned char Reserved[2];      // 字节对齐占位中心无须关心参数
    unsigned char TalkingNum[30];   // 标识当次通话PTT的讲话方号码
    unsigned char CallingNum[30];   // 标识当次呼叫的主叫号码
    unsigned char CalledNum[30];    // 标识当次呼叫的被叫号码（组号、个号）
} PTT_OFF_ACK;

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


/**
 * @struct   _SMS_ACK
 * @brief  接入单元向中心发送短消息回复 结构体
 */
typedef struct _SMS_ACK
{
    CENTER_CMD_SHARE_HEAD   SharedHead;
    unsigned int  CallId;           // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;          // 业务面ID 标识一次呼叫的业务面ID
    unsigned char SourceStat;       // 标识发送单元设备状态（空闲、故障、忙等）
    unsigned char Ack;              // 标识短消息回复结果(成功ACK_OK=0x04、失败ACK_FAIL=0x05)
    unsigned char Reserved[2];
}    SMS_ACK;

/**
 * @struct   _SMS_CCL_DLL_EN
 * @brief CLL    发送短消息到DLL
 */
typedef struct _SMS_CCL_DLL_EN
{
    unsigned short Identification;     // Ipc4报头识别值
    unsigned char  DestIpId:4;         // 目的IP地址索引
    unsigned char  SrcIpId:4;          // 源IP地址索引
    unsigned char  SrcPortId:7;        // 源端口索引
    unsigned char  CmpresOpcode1:1;    // 压缩头操作码高有效位
    unsigned char  DestPortId:7;       // 目的端口索引
    unsigned char  CmpresOpcode2:1;    // 压缩头操作码低有效位
   // unsigned  short      ExtHeader1; // 可选扩展头
 //   unsigned  short      ExtHeader2;
    unsigned char     SmsPayLoad [480];
} SMS_CCL_DLL_EN;


/**
 * @struct   _DT_VOICE_LC_HEADER 9bytes
 * @brief  语音LC头, 该结构等同于 NAS_PRE_LC_PDU
 */
typedef struct _DT_VOICE_LC_HEADER
{
    unsigned char CtrOpcode:6;   // uFLCO, 设置成0
    unsigned char Reserved:1;    // uREV, 设置成0
    unsigned char ProtectFlg:1;  // uPF,
    unsigned char FeatureId;     // uFID, 设置成0
    unsigned char ServiceOpt;    // uSO
    unsigned char GroupAddr[3];
    unsigned char SourceAddr[3];
}  DT_VOICE_LC_HEADER;



/**
 * @struct   LC_HEADER_DATA
 * @brief  语音LC 头
 */
typedef struct _LC_HEADER_DATA
{
    unsigned char SourceId[3];          // 源ID
    unsigned char DesId[3];             // 目的ID
}  LC_HEADER_DATA;

/**
 * @struct   _DT_VOICE_LC_TERMINATOR
 * @brief  语音 结束帧
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

/**  语音LC头、终结帧结构
 * @struct DT_VOICE_LC_EN
 * @brief
 */
typedef struct _DT_VOICE_LC_EN
{
    unsigned char Flco:       6;      // 为  0 组呼
    unsigned char Reserved:   1;      // 设置成0
    unsigned char ProtectFlg: 1;      // 设置成0
    unsigned char FeatureId;          // 设置成0
    unsigned char ServiceOpt;         //  0非加密
    unsigned char GroupAddr[3];
    unsigned char SourceAddr[3];
} DT_VOICE_LC_EN;

/**
 * @struct DLL_CCL_UL_T
 * @brief 数据链路上行到CCL结构体
 */
/*typedef struct _DLL_CCL_UL_T
{
    unsigned char MsgType;          ///< 0x01:语音 0x02:数据 0x03:邻点
    unsigned char FrmType;          ///< 见枚举 _FrameType_E
    unsigned char DataType;         ///< 数据类型
    unsigned char SrcId[3];         ///< 源ID
    unsigned char DstId[3];         ///< 目的ID
    unsigned char uRev0[10];        ///< 预留0
    unsigned char DataLen;          ///< 负载长度
    unsigned char PayLoad[500];     ///< 负载数据

} DLL_CCL_UL_T;

*/

/**
 * @struct  _Rtp_Fix_Head
 * @brief RTP 固定头
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
 * @brief RTP 扩展头
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
 * @brief RTP 转发扩展
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
 * @brief 中心语音数据包格式
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
 * @brief 中心语音帧下行结构
 */

typedef  struct  _CENTER_VOICE_DATA_DL
{
    unsigned char  Version:2;
    unsigned char  Padding:1;       // 填充标记
    unsigned char  ExtendHead:1;    // 扩展头标记
    unsigned char  CsrcCount:4;     // 贡献源计数
    unsigned char  Marker:1;        // RTP重要事件标记
    unsigned char  PayloadType:7;   // RTP 负载类型
    unsigned short SequenceNum;     // RTP序列号
    unsigned int   TimeStamp;       // RTP时间戳
    unsigned int   Ssrc;            // RTP同步源
    unsigned short ProfileHead;     // rtp扩展头特性参数
    unsigned short ExheadLen;       // RTP扩展头数据长度
    unsigned int   VoiceId;         // 标识一次呼叫的业务面ID
    unsigned int   Gps;             // GPS时间
    unsigned char  CodecFlg;        // 数据编解码编解码标识
    unsigned char  FrameFlg;        // 数据帧标识
    unsigned char  PathNum;         // 语音通道号码，若不区分通道，则填全F
    unsigned char  PackRate;        // 标识语音包速率
    unsigned char  EncryptFlag;     // 标识是否加密
    unsigned char  Reserve[3];
    unsigned char  VoiceData[507];
    unsigned char  ReserveData[5];
 }CENTER_VOICE_DATA_DL;
/**
 * @struct   _CCL_STAT_INFO_HEAD
 * @brief CCL 状态消息头
 */
typedef struct _CCL_STAT_INFO_HEAD
{
    unsigned short MsgHead;             //标识AC接口信令头标志(0x13ec)
    unsigned short MsgType;             //信令类型    标识AC接口PTT命令信令（0x000a）
    unsigned short Datalength;          //有效数据长度  标识信令有效数据长度（100）
    unsigned short SourceID;            //源单元号            标识AC接口发送单元ID值
    unsigned short DestID;              //目的单元号     标识AC接口接收单元ID值
    unsigned char  SourceType;          //源单元设备类型       标识发送单元设备类型（制式）
    unsigned char  SigVersion;          //信令版本号       标识该信令适合版本号0x02(该信令与0x01版本不兼容)
    unsigned char  PathNum;             //通道号码         标识该信令属于该单元的通道号码，若不区分通道，则填全F
    unsigned char  ChangeMode;          //主辅模式    0x00  主模式   0x01 从模式    0xff 无效
    unsigned char  ConnectMode;         //连接模式    0x00  自动模式    0x01手动模式
    unsigned char  Exhead[9];           //扩展头           扩展头中心无须关心参数
    unsigned int   CheckVal;            // 校验值      标识信令校验结果(01版本 与02版本 目前都填写全F，标示无校验方式)

}CCL_STAT_INFO_HEAD;
/**
 * @struct   _CCL_STAT_INFO
 * @brief CCL 状态消息
 */

typedef  struct _CCL_STAT_INFO
{
    CCL_STAT_INFO_HEAD  StatInfoHead;
    unsigned int  CallId;               //标识一次呼叫的控制面ID
    unsigned int  VoiceId;              //标识一次呼叫的业务面ID
    unsigned char SourceStat;           //标识发送单元设备状态（空闲、故障、忙等）
    unsigned char CallidSn;             //标识发送单元正处理号码在映射表中序号
    unsigned char CallingId;            //标识发送端单元主叫单元号
    unsigned char CalledId;             //标识发送端单元被叫单元号
}CCL_STAT_INFO;



/**
 * @struct   _GPS_DC_INFO
 * @brief 数据链路层到呼叫控制层GPS消息
 */
typedef  struct _GPS_DC_INFO
{
  unsigned char     TimeH[3];           //GPS 时间高三位
  unsigned char     TimeL[3];           //GPS 时间低三位
  unsigned int      N_S:1;              //方向
  unsigned int      Latiude:31;         //纬度
  unsigned int      W_E:1;              //方向
  unsigned int      Longtiude:31;       //经度
  unsigned short    Speed;              //速度
  unsigned short    Direct;             //方向
}GPS_DC_INFO;


/**
* @struct  GPS_DATA_DLL_T
* @brief  GPS NAS   DLL层数据结构
*/
//#pragma pack(1)__attribute__((aligned(1)))
typedef struct _GPS_DATA_DLL_T
{
    UINT8  NS: 1;           //<1bit 纬度指示 0:SOUTH 1:NORTH
    UINT8  NDEG: 7;         //<7bit 纬度    0~89
    UINT8  EW: 1;           //<1bit 经度指示 1:WEST 0:EAST
    UINT8  NMINmm: 7;       //<6bit 纬分的整数部分 0~59
    UINT16 NMINF;           //<14bit 纬分的小数部分 0~9999
    UINT8  EDEG;            //<8bit 经度     0~179
    UINT8  EMINmm;          //<6bit 经分的整数部分 0~59
    UINT16 EMINF;           //<14bit 经分的小数部分 0~9999
    UINT8  YEAR;            ///<年 0～99
    UINT8  MONTH;           ///<月 1～12
    UINT8  DAY;             ///<日 1～31
    UINT8  HOUR;            ///<时 0～23
    UINT8  MIN;             ///<分 0~59
    UINT8  SEC;             ///<秒 0~59
}__attribute__((packed,aligned(1))) GPS_DATA_DLL_T;

/**
* @struct  GPS_DATA_CCL_T
* @brief  GPS NAS   CCL层数据结构
*/

typedef struct _GPS_DATA_CCL_T
{
    //gps
    unsigned char  NS: 1;           //<1bit 纬度指示 0:SOUTH 1:NORTH
    unsigned char  NDEG: 7;         //<7bit 纬度    0~89
    unsigned char  EW: 1;           //<1bit 经度指示 1:WEST 0:EAST
    unsigned char  NMINmm: 7;       //<6bit 纬分的整数部分 0~59
    unsigned short NMINF;           //<14bit 纬分的小数部分 0~9999
    unsigned char  EDEG;            //<8bit 经度     0~179
    unsigned char  EMINmm;          //<6bit 经分的整数部分 0~59
    unsigned short EMINF;           //<14bit 经分的小数部分 0~9999
    //时间
    char  DATE[14];
   /* unsigned char  MONTH[2];           ///<:    月 1～12
    unsigned char  DAY[2];             ///<:    日 1～31
    unsigned char  HOUR[2];            ///<:    时 0～23
    unsigned char  MIN[2];             ///<:    分 0~59
    unsigned char  SEC[2];             ///<:    秒 0~59*/
}__attribute__((packed,aligned(1))) GPS_DATA_CCL_T;

/**
* @struct  GPS_DATA_CC_T
* @brief  GPS NAS   CC层数据结构
*/

typedef struct _GPS_DATA_CC_T
{
    unsigned char   CALLEDID[4];      //被叫设备ID
    GPS_DATA_CCL_T  GPSCCLDATA;
}__attribute__((packed,aligned(1))) GPS_DATA_CC_T;



/**
* @struct   _LP__DC_INFO
* @brief 数据链路层到呼叫控制层LP消息
*/
typedef  struct _LP__DC_INFO
{
    unsigned char MsgHdr;
    unsigned char  OptCode[2];
    struct     GPS_DC_INFO;
}LP_DC_INFO;
/**
* @struct  _GPS__CC_INFO
* @brief 呼叫控制层到中心GPS消息
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
    PTT_CMD  CC_PTT_CMD;        // 中心手台命令
    PTT_OFF_ACK CC_Ptt_Ack;     // 中心PTT命令响应
    SMS_CENTER_CCL_DL CcSmsSig; // 中心短消息命令
} SHARE_CC_DATA_D;



/******************************************************************************
 *   枚举声明
 *   *************************************************************************/
/**
 * @enum    SMS_DATA_CALL_TYPE
 * @brief   数据消息呼叫类型, 如果更改其中任意一项，ccl_dl.c中的 table_sms_type_print 也需随之更改
 */
 typedef enum _SMS_DATA_CALL_TYPE
 {
    //短消息
    MESSAGE_PRIVATE_CALL    =0x00,          //单呼
    MESSAGE_GROUP_CALL      =0x01,          //组呼
    STATUS_PRIVATE_CALL     =0x02,          //状态单呼
    STATUS_GROUP_CALL       =0x03,          //状态组呼

   //命令
    STUN_REQ_MS             =0x07,          //遥晕MS
    STUN_REQ_NAS            =0x08,          //遥晕NAS
    KILL_REQ_NAS            =0x09,          //遥毙NAS
    GPS_REPOTR_MS           =0x0a,          //拉GPS
    GPS_REPORT_NAS          =0x0b,
    REVIVE_REQ_NAS          =0x0c,
    REVIVE_REQ_MS           =0x0d,
    NEGHR_QUERY             =0x0e,          //邻点查询

    //响应
    NEGHR_QUERY_ACK         =0x20,          //邻点查询相应
    GPS_REPROT_MS_ACK       =0x21,          //相应拉MS  GPS
    GPS_REPROT_NAS_ACK      =0x22,          //相应拉NAS  GPS
    STUN_REQ_MS_ACK         =0x23,
    STUN_REQ_NAS_ACK        =0x24,
    KILL_REQ_NAS_ACK        =0X25,
    REVIVE_REQ_NAS_ACK      =0x26,          //激活NAS
    REVIVE_REQ_MS_ACK       =0x27,          //激活MS
    NAS_NEAR_REPORT         =0x28,          //增加邻点上报
    VARIANCE_HRESHOLD       =0x29,          //方差门限（场强值）

    //报告
    STUN_NAS_REPORT         =0x30,
    KILL_NAS_REPORT         =0x31,
    REVIVE_NAS_REPORT       =0x32,

    //告警
    DISCON_ALARM            =0x40,
    MS_ALARM                =0x41,
    DISCON_NAS_ALARM_CLEAR  =0x42,
    MS_ALARM_CLEAR          =0x43

 } SMS_DATA_CALL_TYPE;

/**
* @struct _SMS_FORMAT
* @brief  短消息格式
*/
 typedef  enum  _SMS_FORMAT
 {
    ASCII        =0X00,
    UNICODE      =0X01,
}SMS_FORMAT;

/**
 * @enum     _CENTER _SIG_TYPE_EN
 * @brief  接入单元与中心信令类型
 */
typedef enum _INF_SIG_TYPE_EN
{
    SIG_STATUS_REPORT = 0x0001,  // NAS 状态上报
    SIG_PTT_CMD       = 0X000a,  // PTT 命令 信令
    SIG_PTT_ON_ACK    = 0x000b,  // PTT ON 回复 信令
    SIG_SMS_SEND      = 0x000e,  // 短消息命令
    SIG_SMS_ACK       = 0x0014,  // 短消息回复信令
    SIG_PTT_OFF_ACK   = 0x0015   // PTT OFF 回复 信令
}INF_SIG_TYPE_EN;

/**
 * @enum     _INF_CCL_STATE_EN
 * @brief  呼叫控制层状态
 */
typedef  enum  _INF_CCL_STATE_EN
{
    INF_CCL_IDLE        =0x01,      //呼叫控制层空闲
    CENTER_VOICE_DL     =0x02,      //中心语音下行
    Wait_Stun_Ms_Ack    =0x03,      //等待摇晕MS响应
    Wait_Stun_Nas_Ack   =0x04,      //等待摇晕WLU响应状态
    Wait_Kill_Nas_Ack   =0x05,      //WLU 摇毙
    Wait_Gps_Nas_Ack    =0x06,
    Wait_Gps_Ms_Ack     =0x07,
    Wait_NEGHR_Nas_Ack  =0x08,
    Wait_Reviv_NAS_Ack  =0x09,
    Wait_Reviv_MS_Ack   =0x0b,
    NAS_VOICE_UL        =0x0c,
    NAS_STAT_STUNED     =0x18,      //设备摇晕
    INF_CCL_DISABLE     =0x19,
    SMS_SEND            =0X1a       //短消息发送
}INF_CCL_STATE_EN;


typedef enum _NAS_WORK_STATE_E
{
    NAS_STOP_REKAY              =1,
    NAS_NORMAL_STATE            =5,         ///< 正常态
    NAS_STUNNED_STATE           =6,         ///< 摇晕态
    NAS_KILLED_STATE            =7,         ///< 摇毙态
    NAS_WAIT_STUN_ACK_STATE     =8,         ///< 等待摇晕响应态
    NAS_WAIT_KILL_ACK_STATE     =9,         ///< 等待摇毙响应态

} NAS_WORK_STATE_E;

typedef enum _NEAR_REPORT_TYPE_E
{
    NEAR_REPORT_ACTIVE            =0,       ///<邻点主动上报
    NEAR_REPORT_PASSIVE           =1,       ///< 邻点查询上报
}NEAR_REPORT_TYPE_E;


/******************************************************************************
*   内部函数实现
*   *************************************************************************/

/******************************************************************************
*   可调用函数实现
*   *************************************************************************/
/**
* @brief
*/

/**
* @brief
*/


#endif //UINT_INF_H

