/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    DLLodp.c
 * @brief   实现下行数据处理模块的入口和出口函数
 * @author  陈禹良
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   函数列表
 *   1. init_rtp                    创建8路消息队列和数据处理线程
 *   2. msg_queue_init              创建消息队列实现函数
 *
 *
 *   修改历史
 *   2016-7-20                      陈禹良                     建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "dll_fun.h"
#include "print_debug.h"
#include "EncDec.h"



/******************************************************************************
 *   宏定义
 *   *************************************************************************/
#define FLC_GPS_ON      1  // GPS内嵌是开
#define FLC_GPS_OFF     0  // GPS内嵌是关
#define EMB_HDR         0  // 当前语音内嵌为 HDR
#define EMB_GPS         1  // 当前语音内嵌为 GPS
#define SND_FLC_START   1  //单次发GPS内嵌的初始状态
#define SND_FLC_OVER    0   //发完一次变为此状态

typedef struct _T_VOICE_FLC {
    UINT8 ucFlcType;
    UINT8 aucFLC[FLC_LEN];
}  T_VOICE_FLC;

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
const char *apcFlcTypeStr[] = {"HDR", "GPS"};
static UINT32 s_ulSuperFrmCnt = 0;              // 超帧计数器
UINT8 g_ucSndFlcOnceState = SND_FLC_START;
/**
 * @brief 下行分组数据缓存
 */
PDP_DT g_PdpDBuf = {0};

/**
 * @brief 下行分组发送序号
 */
UINT32 uNS;

/** @defgroup 上下行socket变量 */
/** @{ */
extern int CCLSocket;
extern int INFSocket;
extern struct sockaddr_in DllOdpAddr;
extern struct sockaddr_in DllIdpAddr;
extern struct sockaddr_in RecvAddr;
extern struct sockaddr_in CclAddr;
extern struct sockaddr_in InfAddr;
extern socklen_t AddrSize;
/** @} */

/**
 * @var  g_PdpUBuf
 * @brief 上行分组数据缓存
 */
extern PDP_UT g_PdpUBuf;

/**
 * @var uLockFreq
 * @brief  业务锁定频率
 */
extern UINT8 uLockFreq;

/**
 * @var uLockSlot
 * @brief  业务锁定时隙
 */
extern UINT8 uLockSlot;

/**
 * @var uGpsFlag
 * @brief  GPS业务标记
 */
extern UINT8 uGpsFlag;

/**
 * @var uRelayFlag
 * @brief  转发标记
 */
extern UINT8 uRelayFlag;

/**
 * @var  s_i4LogMsgId
 * @brief 命令行套接字
 */
extern INT32 s_LogMsgId;

/**
 * @var  pLogTxtFd
 * @brief 文件句柄
 */
extern FILE *pLogFd;

/**
 * @var tDllPrint
 * @brief DLL打印开关
 */
extern DLL_PRINT_T *tDllPrint;

/**
 * @var ptIPCShm
 * @brief 共享内存系统信息
 */
extern SHM_IPC_STRU *ptIPCShm;

/**
 * @var ptCclData
 * @brief 上行呼叫控制数据指针
 */
extern DLL_CCL_UL_T *ptCclData;

/**
 * @var ptInfData
 * @brief 下行接口适配数据指针
 */
extern NAS_INF_DL_T *ptInfData;

/**
 * @var uCallState
 * @brief  业务状态标示
 */
extern UINT8 uCallState;

/**
 * @var uCallWait
 * @brief  业务等待时间
 */
extern UINT16 uCallWait;

/**
 * @var g_DllGlobalCfg
 * @brief 全局配置项
 */
extern DLL_GLB_CFG_T g_DllGlobalCfg;

/**
 * @var p_DllFpgaShm
 * @brief 链接层和FPGA共享内存
 */
extern DLL_FPGA_SHM_T *p_DllFpgaShm;

/**
 * @var ptCFGShm
 * @brief 共享内存系统信息
 */
extern SHM_CFG_STRU *ptCFGShm;


/******************************************************************************
  *   静态变量定义
  *   *************************************************************************/
//UINT16 s_au2VoiceSource[3][6][27] =
UINT8 s_au2VoiceSource[3][6][27] =
{
    {
        // 1031信源测试信源
        {   // 1031信源VoiceA
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        },
        {   // 1031信源VoiceB
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce ,0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        },
        {   // 1031信源VoiceC
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce ,0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        },
        {   // 1031信源VoiceD
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce ,0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        },
        {   // 1031信源VoiceE
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce ,0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        },
        {   // 1031信源VoiceF
            0xce, 0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a, 0xce, 0xa8, 0xfe, 0x83, 0xac,
            0xc4, 0x58, 0x20, 0x0a, 0xce ,0xa8, 0xfe, 0x83, 0xac, 0xc4, 0x58, 0x20, 0x0a
        }
    },
    {
        // Silence信源测试信源
        {   // Silence信源VoiceA
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        },
        {   // Silence信源VoiceB
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        },
        {   // Silence信源VoiceC
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        },
        {   // Silence信源VoiceD
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        },
        {   // Silence信源VoiceE
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        },
        {   // Silence信源VoiceF
            0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61,
            0x73, 0x00, 0x2a, 0x6b, 0xb9, 0xe8, 0x81, 0x52, 0x61, 0x73, 0x00, 0x2a, 0x6b
        }
    },
    {
        // 153信源测试信源
        {   // 153信源VoiceA
            0xff, 0x83, 0xdf, 0x17, 0x32, 0x09, 0x4e, 0xd1, 0xe7, 0xcd, 0x8a, 0x91, 0xc6, 0xd5,
            0xc4, 0xc4, 0x40, 0x21, 0x18, 0x4e, 0x55, 0x86, 0xf4, 0xdc, 0x8a, 0x15, 0xa7
        },
        {   // 153信源VoiceB
            //0xE322, 0x2193, 0x5348, 0x121A, 0x34BF, 0xA2C7, 0x5967, 0x8FBA, 0x0D6D, 0xD82D, 0x7D54,
            //0x0A57, 0x9310, 0x3900
            // source
            0xec, 0x92, 0xdf, 0x93, 0x53, 0x30, 0x18, 0xca, 0x34, 0xbf, 0xa2, 0xc7, 0x59, 0x67,
            0x8f, 0xba, 0x0d, 0x6d, 0xd8, 0x2d, 0x7d, 0x54, 0x0a, 0x57, 0x97, 0x70, 0x39
        },
        {   // 153信源VoiceC
            0xd2, 0x7a, 0xea, 0x24, 0x33, 0x85, 0xed, 0x9a, 0x1d, 0xe1, 0xff, 0x07, 0xbe, 0x2e,
            0x64, 0x12, 0x9d, 0xa3, 0xcf, 0x9b, 0x15, 0x23, 0x8d, 0xab, 0x89, 0x88, 0x80
        },
        {   // 153信源VoiceD
            0x42, 0x30, 0x9c, 0xab, 0x0d, 0xe9, 0xb9, 0x14, 0x2b, 0x4f, 0xd9, 0x25, 0xbf, 0x26,
            0xa6, 0x60, 0x31, 0x94, 0x69, 0x7f, 0x45, 0x8e, 0xb2, 0xcf, 0x1f, 0x74, 0x1a
        },
        {   // 153信源VoiceE
            0xdb, 0xb0, 0x5a, 0xfa, 0xa8, 0x14, 0xaf, 0x2e, 0xe0, 0x73, 0xa4, 0xf5, 0xd4, 0x48,
            0x67, 0x0b, 0xdb, 0x34, 0x3b, 0xc3, 0xfe, 0x0f, 0x7c, 0x5c, 0xc8, 0x25, 0x3b
        },
        {   // 153信源VoiceF
            0x47, 0x9f, 0x36, 0x2a, 0x47, 0x1b, 0x57, 0x13, 0x11, 0x00, 0x84, 0x61, 0x39, 0x56,
            0x1b, 0xd3, 0x72, 0x28, 0x56, 0x9f, 0xb2, 0x4b, 0x7e, 0x4d, 0x4c, 0xc0, 0x63
        }
    }
};

/**
 * @brief 查表
 * by zhoudayuan
 */
typedef struct _T_SapPrint {
    unsigned char SapType;
    const char *SapStr;
} T_SapPrint;

T_SapPrint atSapPrint[] = {
    {S_LC_HDR,          "LC_HDR"},
    {S_PI_HDR,          "PI_HDR"},
    {S_LC_TER,          "LC_TER"},
    {S_VOICE_A,         "V_A"},
    {S_VOICE_B,         "V_B"},
    {S_VOICE_C,         "V_C"},
    {S_VOICE_D,         "V_D"},
    {S_VOICE_E,         "V_E"},
    {S_VOICE_F,         "V_F"},
    {S_CSBK_PRE,        "CSBK_PRE"},
    {S_C_HDR,           "C_HDR"},
    {S_E_HDR,           "E_HDR"},
    {S_U_HDR,           "U_HDR"},
    {S_RE_HDR,          "RE_HDR"},
    {S_SD_HDR,          "SD_HDR"},
    {S_DATA_1_2,        "D_12"},
    {S_DATA_3_4,        "D_34"},
    {S_TD_LC,           "TD_LC"},
    {S_NEGR_BST,        "NEGR_BST"},
    {S_NEGR_RPT,        "NEGR_RPT"},
    {S_GPS_REQ,         "GPS_REQ"},
    {S_GPS_ACK,         "GPS_ACK"},
    {S_CSBK_REQ,        "CSBK_REQ"},
    {S_CSBK_ACK,        "CSBK_ACK"},
    {S_KILL_REQ,        "KILL_REQ"},
    {S_KILL_ACK,        "KILL_ACK"},
    {S_ALARM_REQ,       "ALARM_REQ"},
    {S_ALARM_ACK,       "ALARM_ACK"},
    {S_ENABLE_REQ,      "ENABLE_REQ"},
    {S_ENABLE_ACK,      "ENABLE_ACK"},
    {S_NAS_GPS_REQ,     "NAS_GPS_REQ"},
    {S_NAS_GPS_ACK,     "NAS_GPS_ACK"},
    {S_NAS_KILL_REQ,    "NAS_KILL_REQ"},
    {S_NAS_KILL_ACK,    "NAS_KILL_ACK"},
    {S_NAS_STUN_REQ,    "NAS_STUN_REQ"},
    {S_NAS_STUN_ACK,    "NAS_STUN_ACK"},
    {S_NAS_ENABLE_REQ,  "NAS_ENABLE_REQ"},
    {S_NAS_ENABLE_ACK,  "NAS_ENABLE_ACK"},
    {S_NAS_NEGR_REQ,    "NAS_NEGR_REQ"},
    {S_NAS_NEGR_ACK,    "NAS_NEGR_ACK"},
    {S_NAS_ALARM_REQ,   "NAS_ALARM_REQ"},
    {S_NAS_ALARN_ACK,   "NAS_ALARM_ACK"},
    {S_NAS_RELAY,       "NAS_RELAY"},
    {S_NAS_PRE_LC,      "NAS_PRE_LC"},
};
T_SapPrint *ptSapPrint = atSapPrint;
int SapPrintLen = sizeof(atSapPrint)/sizeof(atSapPrint[0]);
//DMRA加密方式下的编内嵌用的ID
UINT8 Down_Alg_ID=0;
UINT8 Down_Feature_ID=0;
UINT8 Down_PI_FLAG=0;
//UINT32 Down_Voice_Cnt=0;
UINT8 s_plan_gps_Flag=0;

/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
/**
 * @brief  下行向接口适配层发送数据处理
 *
 * @param [in] SAPType      业务数据类型
 * @param [in] SapStr         业务数据打印
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
void SapDataStr(UINT8 SAPType, char *SapStr)
{
    int i;
    for (i = 0; i < SapPrintLen; i++)
    {
        if (ptSapPrint[i].SapType == SAPType)
        {
            strcpy(SapStr, ptSapPrint[i].SapStr);
            return;
        }
    }
    strcpy(SapStr, "default type");
}
#if 0
void SapDataStr(UINT8 SAPType, char *SapStr)
{
    switch(SAPType)
    {
        case S_LC_HDR:
            strcpy(SapStr, "LC_HDR");
            break;
        case S_LC_TER:
            strcpy(SapStr, "LC_TER");
            break;
        case S_VOICE:
            strcpy(SapStr, "VOICE");
            break;
        case S_CSBK_PRE:
            strcpy(SapStr, "CSBK_PRE");
            break;
        case S_C_HDR:
            strcpy(SapStr, "C_HDR");
            break;
        case S_U_HDR:
            strcpy(SapStr, "U_HDR");
            break;
        case S_RE_HDR:
            strcpy(SapStr, "RE_HDR");
            break;
        case S_SD_HDR:
            strcpy(SapStr, "SD_HDR");
            break;
        case S_DATA_1_2:
            strcpy(SapStr, "DATA_1_2");
            break;
        case S_DATA_3_4:
            strcpy(SapStr, "DATA_3_4");
            break;
        case S_TD_LC:
            strcpy(SapStr, "TD_LC");
            break;
        case S_NEGR_BST:
            strcpy(SapStr, "NEGR_BST");
            break;
        case S_NEGR_RPT:
            strcpy(SapStr, "NEGR_RPT");
            break;
        case S_GPS_REQ:
            strcpy(SapStr, "GPS_REQ");
            break;
        case S_GPS_ACK:
            strcpy(SapStr, "GPS_ACK");
            break;
        case S_CSBK_REQ:
            strcpy(SapStr, "CSBK_REQ");
            break;
        case S_CSBK_ACK:
            strcpy(SapStr, "CSBK_ACK");
            break;
        case S_KILL_REQ:
            strcpy(SapStr, "KILL_REQ");
            break;
        case S_KILL_ACK:
            strcpy(SapStr, "KILL_ACK");
            break;
        case S_ALARM_REQ:
            strcpy(SapStr, "ALARM_REQ");
            break;
        case S_ALARM_ACK:
            strcpy(SapStr, "ALARM_ACK");
            break;
        case S_ENABLE_REQ:
            strcpy(SapStr, "ENABLE_REQ");
            break;
        case S_ENABLE_ACK:
            strcpy(SapStr, "ENABLE_ACK");
            break;
        case S_NAS_GPS_REQ:
            strcpy(SapStr, "NAS_GPS_REQ");
            break;
        case S_NAS_GPS_ACK:
            strcpy(SapStr, "NAS_GPS_ACK");
            break;
        case S_NAS_KILL_REQ:
            strcpy(SapStr, "NAS_KILL_REQ");
            break;
        case S_NAS_KILL_ACK:
            strcpy(SapStr, "NAS_KILL_ACK");
            break;
        case S_NAS_STUN_REQ:
            strcpy(SapStr, "NAS_STUN_REQ");
            break;
        case S_NAS_STUN_ACK:
            strcpy(SapStr, "NAS_STUN_ACK");
            break;
        case S_NAS_ENABLE_REQ:
            strcpy(SapStr, "NAS_ENABLE_REQ");
            break;
        case S_NAS_ENABLE_ACK:
            strcpy(SapStr, "NAS_ENABLE_ACK");
            break;
        case S_NAS_NEGR_REQ:
            strcpy(SapStr, "NAS_NEGR_REQ");
            break;
        case S_NAS_NEGR_ACK:
            strcpy(SapStr, "NAS_NEGR_ACK");
            break;
        case S_NAS_ALARM_REQ:
            strcpy(SapStr, "NAS_ALARM_REQ");
            break;
        case S_NAS_ALARN_ACK:
            strcpy(SapStr, "NAS_ALARM_ACK");
            break;
        case S_NAS_RELAY:
            strcpy(SapStr, "NAS_RELAY");
            break;
        case S_NAS_PRE_LC:
            strcpy(SapStr, "NAS_PRE_LC");
            break;

        default:
            strcpy(SapStr, "default type");
            break;

    }
}
#endif


/**
 * @brief  主呼下行向接口层发送数据处理
 *
 * @param [in] MsgT           业务数据类型
 * @param [in] FrmT           业务数据打印
 * @param [in] DataT          业务数据打印
 * @param [in] Len            业务数据打印
 * @param [in] PayLoad        业务数据打印
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void CallingShootData(UINT32 TimeS, UINT8 FrqSlt, UINT8 MsgT, UINT8 FrmT, UINT8 DataT, UINT8 Len, UINT8 *PayLoad)
{
    UINT8 Frq = (FrqSlt >> 4) & 0x0f;
    UINT8 slt = FrqSlt & 0x0f;

    ptInfData->TimeStamp = TimeS;
    ptInfData->SlotNum = slt;

    if (Frq == 0)
    {
        ptInfData->TxFreq1 = 1;
    }
    else
    {
        ptInfData->TxFreq2 = 1;
    }
    ptInfData->tDataLink[Frq].MsgType = MsgT;
    ptInfData->tDataLink[Frq].FrmType = FrmT;

    if (MsgT == DI_MSG_DATA || MsgT == DI_MSG_VOICE)
    {
        ptInfData->tDataLink[Frq].CC = g_DllGlobalCfg.auMSCC;
    }
    else
    {
        ptInfData->tDataLink[Frq].CC = g_DllGlobalCfg.auWLUCC;
    }
    if((FrmT>=FT_VOICE_B) && (FrmT<=FT_VOICE_E) && (g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_ENHA)&&(s_plan_gps_Flag==PLAN_GPS))//只有Hytera高级加密内嵌才需要传PI,下一帧计划传GPS本超帧内嵌才传PI=1
    {
        ptInfData->tDataLink[Frq].PI = Down_PI_FLAG;
    }
    else
    {
        ptInfData->tDataLink[Frq].PI = 0;
    }
    ptInfData->tDataLink[Frq].LCSS = 0;  // 00-单片LC, 01-LC信令的第一片, 10-LC的最后片, 11-LC的中间片
    ptInfData->tDataLink[Frq].DataType = DataT;
    ptInfData->tDataLink[Frq].DataLen = Len;
    memcpy(ptInfData->tDataLink[Frq].PayLoad, PayLoad, Len);
}


/**
 * @brief  下行向接口适配层发送数据处理
 *
 * @param [in] ptInfData      下行发送数据
 * @param [in] DataType      发送数据类型
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_SendInfData(NAS_INF_DL_T *pdInfData, UINT8 SAPType)
{
    if (tDllPrint->AIDown == 1)
    {
        ODP_AiPrintf(pdInfData, SAPType);
    }
    sendto(INFSocket, pdInfData, sizeof(NAS_INF_DL_T), 0, (struct sockaddr *)(&InfAddr), AddrSize);
    return;
}


/**
 * @brief  打印下行控制层数据
 *
 * @param [in] pvCclData      下行发送数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_CclPrintf(CCL_DLL_DL_T * pvCclData)
{
    if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] MsgT_%d FrmT_%d DataT_%#x Len_%d",
                  _F_,
                  pvCclData->MsgType,
                  pvCclData->FrmType,
                  pvCclData->DataType,
                  pvCclData->DataLen);
    }

    if (tDllPrint->PrintLv == PRINT_MEMRY|| tDllPrint->PrintLv == PRINT_ALL)
    {
        LOG_PrintM(s_LogMsgId,pvCclData->PayLoad,pvCclData->DataLen);
    }
}


/**
 * @brief  打印下行接口层数据
 *
 * @param [in] ptInfData      下行发送数据
 * @param [in] SAPType       业务数据类型
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_AiPrintf(NAS_INF_DL_T * pdInfData, UINT8 SAPType)
{
    UINT8 Frq;
    char SapStr[20] = {0};
    if(1 == pdInfData->TxFreq1)
    {
        Frq = 0;
    }
    else
    {
        Frq = 1;
    }

    if ((tDllPrint->PrintLv == PRINT_DEBUG) || (tDllPrint->PrintLv == PRINT_ALL))
    {
        SapDataStr(SAPType, SapStr);
        LOG_DEBUG(s_LogMsgId,"[DLL][ODP_AI] TimS:%ld F%dS%d MsgT_%d FrmT_%d CC_%d DataT_%d PI_%d Len_%d %s",
            pdInfData->TimeStamp,
            (Frq+1),
            (pdInfData->SlotNum+1),
            pdInfData->tDataLink[Frq].MsgType,
            pdInfData->tDataLink[Frq].FrmType,
            pdInfData->tDataLink[Frq].CC,
            pdInfData->tDataLink[Frq].DataType,
            pdInfData->tDataLink[Frq].PI,
            pdInfData->tDataLink[Frq].DataLen,
            SapStr);
    }

    if ((tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL) && (pdInfData->tDataLink[Frq].DataLen < 35))
    {
        LOG_PrintM(s_LogMsgId, pdInfData->tDataLink[Frq].PayLoad, pdInfData->tDataLink[Frq].DataLen);
        LOG_PrintM(s_LogMsgId, pdInfData->tDataLink[Frq].EmbInfo, 4);
    }
}


/**
 * @brief 下行(196,144)BPTC 编码函数
 *
 * @param [in]    pData         下行3/4原始PDU数据
 * @param [out]  pAiData      下行空口发送数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_144to196bptc(UINT8 *pData, UINT8 *pAiData)
{
    UINT8 i = 0;
    UINT16 u16PduData[9];
    UINT16 u16AiData[18];

    memcpy(u16PduData, pData, 18);

    for (i = 0; i < 9; i++)
    {
        *(u16PduData+i) = (*(u16PduData+i) << 8) | (*(u16PduData+i) >> 8);
    }

    bptc_144_196_enc(u16PduData, u16AiData);        // 大端模式

    pAiData[0] = u16AiData[1] & 0xff;
    pAiData[1] = (u16AiData[2] >> 8) & 0xff;
    pAiData[2] = u16AiData[2] & 0xff;
    pAiData[3] = (u16AiData[3] >> 8) & 0xff;
    pAiData[4] = u16AiData[3] & 0xff;
    pAiData[5] = (u16AiData[4] >> 8) & 0xff;
    pAiData[6] = u16AiData[4] & 0xff;
    pAiData[7] = (u16AiData[5] >> 8) & 0xff;
    pAiData[8] = u16AiData[5] & 0xff;
    pAiData[9] = (u16AiData[6] >> 8) & 0xff;
    pAiData[10] = u16AiData[6] & 0xff;
    pAiData[11] = (u16AiData[7] >> 8) & 0xff;
    pAiData[12] = u16AiData[7] & 0xff;
    pAiData[13] = (u16AiData[8] >> 8) & 0xff;
    pAiData[14] = u16AiData[8] & 0xff;
    pAiData[15] = (u16AiData[9] >> 8) & 0xff;
    pAiData[16] = u16AiData[9] & 0xff;
    pAiData[17] = (u16AiData[10] >> 8) & 0xff;
    pAiData[18] = u16AiData[10] & 0xff;
    pAiData[19] = (u16AiData[11] >> 8) & 0xff;
    pAiData[20] = u16AiData[11] & 0xff;
    pAiData[21] = (u16AiData[12] >> 8) & 0xff;
    pAiData[22] = u16AiData[12] & 0xff;
    pAiData[23] = (u16AiData[13] >> 8) & 0xff;
    pAiData[24] = u16AiData[13] & 0xff;
    pAiData[25] = (u16AiData[14] >> 8) & 0xff;
    pAiData[26] = u16AiData[14] & 0xff;
    pAiData[27] = (u16AiData[15] >> 8) & 0xff;
    pAiData[28] = u16AiData[15] & 0xff;
    pAiData[29] = (u16AiData[16] >> 8) & 0xff;
    pAiData[30] = u16AiData[16] & 0xff;
    pAiData[31] = (u16AiData[17] >> 8) & 0xff;
    pAiData[32] = u16AiData[17] & 0xff;

}


/**
 * @brief   下行GPS下拉请求组包
 *
 * @param [in]  RequestID    GPS上拉请求ID
 * @param [in]  RadioIP      GPS上拉终端IP
 * @param [out] GpsData      生成数据
 * @param [out] Length       生成数据长度
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_GenGpsReqData(UINT8 *RequestID, UINT8 *RadioIP, UINT8 *GpsData, UINT8 *Length)
{
    UINT8 *Sum = 0;
    MS_GPS_REQ_T *MsGpsReq;
    MsGpsReq = (MS_GPS_REQ_T *)GpsData;
    memset(GpsData, 0, sizeof(MS_GPS_REQ_T));

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if(g_DllGlobalCfg.auPI==1)
            MsGpsReq->IpCmpsHdr.IPv4Idt = htons(0x0098);
        else
            MsGpsReq->IpCmpsHdr.IPv4Idt = htons(0x022a);
    }
    else
    {
        MsGpsReq->IpCmpsHdr.IPv4Idt = htons(0x009c);
    }
    MsGpsReq->IpCmpsHdr.SAID = 1;
    MsGpsReq->IpCmpsHdr.DAID = 0;
    MsGpsReq->IpCmpsHdr.MHCO = 0;
    MsGpsReq->IpCmpsHdr.SPID = 116;
    MsGpsReq->IpCmpsHdr.LHCO = 0;
    MsGpsReq->IpCmpsHdr.DPID = 116;

    MsGpsReq->MsgHdr = 0x08;  // 0x11-RRS, 0x08-LP:Location Protocol
    /*
    ** 0xA0-Standard Location Immediate Service,
    ** 0x01-(Request), RequestID[4]+RadioIP[4]
    */
    MsGpsReq->OpCode = htons(0xa001);
    /*
    ** Unique identifier of the request, usually,
    ** it’s the request sent counter in hexadecimal,
    ** this value will increase by 1 in each sent, the initial value is 1.
    ** Example: 0x1, this request is first sent.
    */
    MsGpsReq->ByteLen = htons(0x0008);
    MsGpsReq->RequestID[0] = RequestID[0];
    MsGpsReq->RequestID[1] = RequestID[1];
    MsGpsReq->RequestID[2] = RequestID[2];
    MsGpsReq->RequestID[3] = RequestID[3];
    /*
    ** A unique identity of the terminal radio in IPV4 format.
    ** Example:
    ** 0x0a000001, this value means the subnet is 10.0.0.0, the RadioID is 1 when fresh from factory.
    ** 0x0a000050, the RadioIP is 10.0.0.80, indicating that the subnet is 10.x.x.x, and the Radio ID is 80.
    */
    MsGpsReq->RadioIP[0] = RadioIP[0];
    MsGpsReq->RadioIP[1] = RadioIP[1];
    MsGpsReq->RadioIP[2] = RadioIP[2];
    MsGpsReq->RadioIP[3] = RadioIP[3];

    Sum = (UINT8 *)&MsGpsReq->OpCode;
    for (UINT8 i = 0; i < 12; i++)
    {
        MsGpsReq->CheckSum += *Sum;
        Sum++;
    }
    MsGpsReq->CheckSum = ~ (MsGpsReq->CheckSum) + 0x33;
    MsGpsReq->MsgEnd = 0x03;  // As the end of one frame, "MsgEnd" filed occupies 1 byte, the value is always 0x03.
    *Length = sizeof(MS_GPS_REQ_T);
    return;
}


/**
 * @brief 下行发送MS应答数据头函数
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_GenReponHeader(UINT8 *DstId, UINT8 *SrcId, PDT_RE_HEAD_PDU_DT *p_ReHdrPdu)
{
    UINT16 u2CRC = 0;
    p_ReHdrPdu->uDPF = 1;  // Response
    p_ReHdrPdu->uRV1 = 0;
    p_ReHdrPdu->uA = 0;  // No Response
    p_ReHdrPdu->uRV2 = 0;
    p_ReHdrPdu->uRV3 = 0;
    p_ReHdrPdu->uSAP = 3; // UDP/IP header compression
    p_ReHdrPdu->uBF = 0;  // 无
    p_ReHdrPdu->uRV4 = 0;
    p_ReHdrPdu->uCLAS = 0;
    p_ReHdrPdu->uTYPE = 1;
    p_ReHdrPdu->uSTAT = g_PdpUBuf.uBLOCKCnt;

    memcpy(p_ReHdrPdu->auSADDR, SrcId, 3);
    memcpy(p_ReHdrPdu->auTADDR, DstId, 3);

    u2CRC = ALG_Crc16((UINT8 *)p_ReHdrPdu, DATA_HEADER_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_DATA;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_DATA;
    }

    p_ReHdrPdu->auCRC[0] = (UINT8)(u2CRC >> 8);
    p_ReHdrPdu->auCRC[1] = (UINT8)(u2CRC & 0xff);

}


/**
 * @brief 下行发送NAS预占lc_Header函数
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_GenNasLcHeader(void)
{
    UINT16 auULC[6] = {0};
    NAS_PRE_LC_PDU s_NasPreLc;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));

    //NAS预占LC
    s_NasPreLc.uFLCO = 0x30;
    s_NasPreLc.uREV = 0;
    s_NasPreLc.uPF = 1;
    s_NasPreLc.uFID = 0;
    s_NasPreLc.uSO = 0;
    memset(s_NasPreLc.auTADDR, 0, 3);
    memcpy(s_NasPreLc.auSADDR, g_DllGlobalCfg.auSoureId, 3);
    memcpy(auULC, &s_NasPreLc, FLC_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_head_fulllc_enc(auULC);
    }
    else
    {
        dmr_head_fulllc_enc(auULC);
    }

    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_LC_HEADER,(FLC_LEN+3),(UINT8 *)auULC);
    ODP_SendInfData(ptInfData, S_NAS_PRE_LC);
    return;
}


/**
 * @brief 下行发送数据链路终结帧函数
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void ODP_TerDataLC(UINT8 *DstId, UINT8 *SrcId, UINT8 GI, UINT8 ResFrq, UINT8 ResSlt)
{
    UINT16 auULC[6];
    TD_LC_PDU_T s_Tdlc;

    s_Tdlc.uFLCO = 48;      // 110000
    s_Tdlc.uRV = 0;
    s_Tdlc.uPF = 0;
    s_Tdlc.uFID = 0;
    s_Tdlc.uNS = uNS%8;
    s_Tdlc.uS = 0;
    s_Tdlc.uRV1 = 0;
    s_Tdlc.uF = 1;

    if (0 == GI)
    {
        s_Tdlc.uA = 1;
    }
    else
    {
        s_Tdlc.uA = 0;
    }
    s_Tdlc.uGI = GI;

    memcpy(s_Tdlc.auTADDR, DstId, 3);
    memcpy(s_Tdlc.auSADDR, SrcId, 3);

    memset(auULC, 0, 12);
    memcpy(auULC, &s_Tdlc, FLC_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_term_fulllc_enc(auULC);
    }
    else
    {
        dmr_term_fulllc_enc(auULC);
    }

    //下行TD_LC组包
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    ptInfData->ResFrqSlot = (ResFrq<<4) | ResSlt;
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_LC_TERMINATOR, (DATA_HEADER_LEN+2), (UINT8 *)auULC);
    if (tDllPrint->AIDown == 1)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] TD_LC ResFrq: %d ResSlot: %d", _F_, ResFrq, ResSlt); // why
    }
    ODP_SendInfData(ptInfData, S_TD_LC);
    uNS++;

    return;
}


/**
 * @brief 设置定时器定时值

 * @param
 * @return
 * @author  周大元
 * @since
 * @bug
 */
void SetTimerSec2Cnt(UINT8 *pucSec, UINT32 *pulCnt, UINT16 usTimerBase)
{
    UINT32 ulSec = (UINT32 )(*pucSec);
    *pulCnt = (ulSec * 1000)/usTimerBase;
}

void VoiceHdrEmbInit(CCL_DLL_DL_T *pvCclData)
{
    UINT16 auULC[6] = {0};
    memcpy(auULC, pvCclData->PayLoad, FLC_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_head_fulllc_enc(auULC);
    }
    else
    {
        dmr_head_fulllc_enc(auULC);
    }

    memcpy(g_DllGlobalCfg.auDownloadLC, auULC, (FLC_LEN+3));
    EmbeddedEnc(g_DllGlobalCfg.auDownloadLC, g_DllGlobalCfg.auEmbInfo); // auDownloadLC(src) ==> auEmbInfo(dst)
    memcpy(p_DllFpgaShm->EmbInfo, g_DllGlobalCfg.auEmbInfo, 16);
}





void VoiceGpsEmbInit()
{
#if EMB_GPS_DEBUG
    // 下列各项或者由网管配置，或者自动生成
    // p_DllFpgaShm->GpsFlag = 0x01;
    p_DllFpgaShm->GpsFlag= 0x01;
    g_DllGlobalCfg.ucFlcGpsSwitch = FLC_GPS_ON; // 开关打开
    g_DllGlobalCfg.ucFlcGpsSec = 5;             //15s
#endif
    LOG_DEBUG(s_LogMsgId, "[DLL][%s] GPS: Switch=%u Lock=%u, Exist=%u", _F_, g_DllGlobalCfg.ucFlcGpsSwitch, p_DllFpgaShm->GpsFlag & (1<<0), (p_DllFpgaShm->GpsFlag & (1<<1))>>1);

    if (g_DllGlobalCfg.ucFlcGpsSwitch == FLC_GPS_ON)  // 当选中有GPS内嵌时，每次发语音帧之前需要进行初始化
    {
        s_ulSuperFrmCnt = 0;  // 超帧计数器
        g_ucSndFlcOnceState = SND_FLC_START;
        SetTimerSec2Cnt(&g_DllGlobalCfg.ucFlcGpsSec, &g_DllGlobalCfg.ulFlcGpsCnt, TIMER_BASE);  // ucFlcGpsSec ==> ulFlcGpsCnt, 时基: 60ms
        SetTimerOff(FLC_GPS_ODP);  // 关闭当前定时器， SetTimerOn启动
        RegisterTimerFun(FLC_GPS_ODP, SetTimerOff);
    }
}

/**
 * @brief 下行LC_header处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_LcHeaderFun(CCL_DLL_DL_T * pvCclData)
{
    UINT16 u2CRC    = 0;

    if (FLC_LEN != pvCclData->DataLen)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Header data length error", _F_);
        return ERR;
    }

#if 0
    DLL_SyncFpgaCfg();
    if (1 == p_DllFpgaShm->FollowEn)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] service is going on", _F_);
        return NO_ERR;
    }
    else
    {
        uCallState = CALL_VOICE_D;
    }
#endif
    VoiceHdrEmbInit(pvCclData);
    VoiceGpsEmbInit();
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_LC_HEADER, (FLC_LEN+3), g_DllGlobalCfg.auDownloadLC);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    ODP_SendInfData(ptInfData, S_LC_HDR);
    if(g_DllGlobalCfg.auPI==1)
    {
            DLL_ENCLIB_VOICE Encryp_str;
            PI_PDU *uPi_Struct=(PI_PDU*)Encryp_str.PIInfo;
            memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
            memcpy(Encryp_str.LCInfo,pvCclData->PayLoad,FLC_LEN);
            Encryp_str.FrameNum = VOICE_LC_FRAME;
            Encryp_str.ValidFlag = VALID;
            EncryptFunc_Voice(&Encryp_str);

            //LOG_WARNING(s_LogMsgId,"[DLL][%s] PI Data:", _F_);
            //LOG_PrintM(s_LogMsgId, Encryp_str.PIInfo,10);

            if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_BASE)//基本加密
            {
                //只做初始化，不产生PI
            }
            else//高级加密
            {
                memset(g_DllGlobalCfg.auDownloadPI, 0, PI_LEN+2);
                if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA)//DMRA
                {
                    Down_Alg_ID = uPi_Struct->dmraPiData.uAlgId;
                    Down_Feature_ID = uPi_Struct->dmraPiData.uKeyID;
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] Down_Alg_ID=%d,Down_Feature_ID=%d", _F_,Down_Alg_ID,Down_Feature_ID);
                }
                memcpy(g_DllGlobalCfg.auDownloadPI,Encryp_str.PIInfo,PI_LEN);
                u2CRC = ALG_Crc16(g_DllGlobalCfg.auDownloadPI, PI_LEN);
                if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
                {
                    u2CRC = u2CRC ^ PDT_CRC_MASK_PI;
                }
                else
                {
                    u2CRC = u2CRC ^ DMR_CRC_MASK_PI;
                }
                g_DllGlobalCfg.auDownloadPI[PI_LEN]   = (UINT8)(u2CRC >> 8);
                g_DllGlobalCfg.auDownloadPI[PI_LEN + 1] = (UINT8)(u2CRC & 0xff);
                //发送至空口
                CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_PI_HEADER, (PI_LEN+2), g_DllGlobalCfg.auDownloadPI);
                ODP_SendInfData(ptInfData, S_PI_HDR);
            }
    }
    return NO_ERR;
}


/**
 * @brief 下行LC_Terminator处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_LcTerminatorFun(CCL_DLL_DL_T * pvCclData)
{
    int cmp;
    UINT16 auULC[6];
    memcpy(auULC, pvCclData->PayLoad, (FLC_LEN+3));

    if (FLC_LEN != pvCclData->DataLen)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Termin data length error", _F_);
        return ERR;
    }

    cmp = strncmp((char *)g_DllGlobalCfg.auDownloadLC, (char *)pvCclData->PayLoad, FLC_LEN);//应与之前下行LC_H 一致
    if (0 != cmp)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Termin data error", _F_);
        return ERR;
    }
#if 0
    if (CALL_VOICE_D != uCallState)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] recv ccl voice, callstate: %d abandon", _F_, uCallState);
        return NO_ERR;
    }
    else
    {
        DLL_SetTimer(CALL_VOICE_D, 7);
    }
#endif
    if(g_DllGlobalCfg.auPI==1)
    {
        DLL_ENCLIB_VOICE Encryp_str;
        memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
        memcpy(Encryp_str.LCInfo,pvCclData->PayLoad,FLC_LEN);
        Encryp_str.FrameNum = VOICE_TERM_FRAME;
        Encryp_str.ValidFlag = VALID;
        EncryptFunc_Voice(&Encryp_str);
    }
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_term_fulllc_enc(auULC);
    }
    else
    {
        dmr_term_fulllc_enc(auULC);
    }

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_LC_TERMINATOR, (FLC_LEN+3), (UINT8 *)auULC);
    ODP_SendInfData(ptInfData, S_LC_TER);
    return NO_ERR;
}



/**
 * @brief 下行链路机发送预载波数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
int ODP_GenNasPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI)
{
    NAS_AI_PAYLOAD NasAiData;
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));
    NasAiData.cmd_code = CMO_CODE_NAS_PRE;
    NasAiData.src_id = (*Src&0x1f);
    NasAiData.nm_type = (unsigned char )NM_TYPE_CENTER;
    NasAiData.dst_id = (*Dst&0x1f);
    NasAiData.op_code = OP_CODE_GET;
    NasAiData.data[0] = cbf;
    NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

    if (tDllPrint->AIDown == 1)
    {
        if (GetTimerCnt(NAS_PRE_ODP) == 0)
        {
            SetTimerOn(NAS_PRE_ODP, 5, SetTimerOff);  // 300ms
            LOG_DEBUG(s_LogMsgId, "[DLL][%s] dst_id_%d, src_id_%d", _F_, NasAiData.dst_id, NasAiData.src_id );
        }
    }

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
    ODP_SendInfData(ptInfData, S_CSBK_PRE);
    return NO_ERR;
}

/**
 * @brief 下行发送预载波数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_GenPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI)
{
    UINT16 u2CRC    = 0;
    PRE_CSBK_PDU tPreCsbk;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));

    tPreCsbk.uLB = 1;
    tPreCsbk.uPF = 0;
    tPreCsbk.uCSBKO = PRE_CSBKO;
    tPreCsbk.uFID = 0;
    tPreCsbk.uDC = 1;//此标志位MS_GPS上拉必须置一(无论PDT/DMR).遥晕遥毙抓包此标志位不同，但功能无影响
    tPreCsbk.uGI = GI;
    tPreCsbk.uREV = 0;//字段预留4bit
    tPreCsbk.uR12 = 0;
    tPreCsbk.uGps= 0;//MS  GPS业务必须将此位置置0.
    tPreCsbk.uCBF = cbf;

    memcpy(tPreCsbk.auTADDR, Dst, 3);
    memcpy(tPreCsbk.auSADDR, Src, 3);

    u2CRC = ALG_Crc16((UINT8 *)&tPreCsbk, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }
    tPreCsbk.auCRC[0] = (UINT8)(u2CRC >> 8);
    tPreCsbk.auCRC[1] = (UINT8)(u2CRC & 0xff);
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_CSBK, (CSBK_LEN+2), (UINT8 *)&tPreCsbk);
    ODP_SendInfData(ptInfData, S_CSBK_PRE);
    return NO_ERR;
}


/**
 * @brief   下行R34带确认数据组包
 *
 * @param [in]  DataLen       下行数据长度
 * @param [out]  puDPHdr       生成数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 * @explain:
 * 结束特征图样: CRC校验之前，填满数据帧的剩余空间。
 *     DEI-数据结束指示。结束特征图样用来指示有效数据结束。
 *         00 最后帧数据全空，倒数第二帧以特征图样结束
 *         01 最后帧数据未满，以特征图样结束
 *         10 倒数第二帧数据全满， 且数据在倒数第二帧结束，最后帧全填0
 *         11 最后帧数据全满
 *     SAP-用来指示数据分组的业务类型，例如IP业务其值为4。
 *     DPF-用来区分有无确认传输方式。
 *     FSN-片序号，用于片重组。一共4bit, 最低3bit用于片序号，最高bit用于指示是否为最终片.
 *     12 16 18是什么意思 ?
 */
int ODP_R34CPacketDataFun(UINT8 DataLen, PDP_HEAD_DU *puDPHdr,PDP_HEAD_DU *puDPEHdr)
{
    UINT8 Poc, Bf, Dei, BlkNum;
    DLL_ENCLIB_DATA Encry_Data;
    memset(&Encry_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    // 基本信息记录
    g_PdpDBuf.uState  = PDP_STATE_FIRST;
    g_PdpDBuf.s_uDBSNIndex = 0;
    g_PdpDBuf.uDataType    = DT_R_3_4_DATA;
    g_PdpDBuf.uHdrNum      = 1;

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if (DataLen % 16 < 12)          //0~12
        {
            BlkNum = DataLen/16;
            Bf = BlkNum+1;  // 后续帧数
            Dei = 1;

            //设置特征图样
            memset(g_PdpDBuf.auData, 0xff, (Bf*R34_C_DATA_DLEN));
            *(g_PdpDBuf.auData+DataLen) = 0x7f;
        }
        else if (DataLen % 16 == 12)    //==12
        {
            BlkNum = DataLen/16;
            Bf = BlkNum+1;
            Dei = 3; // 最后帧数据全满

            //设置特征图样
            memset(g_PdpDBuf.auData, 0, (Bf*R34_C_DATA_DLEN));
        }
        else if (DataLen % 16 == 0)     //==16
        {
            BlkNum = DataLen/16;
            Bf = BlkNum+1;
            Dei = 2;  // 倒数第二帧数据全满，且数据在倒数第二帧结束，最后帧填充全0

            //设置特征图样
            memset(g_PdpDBuf.auData, 0, (Bf*R34_C_DATA_DLEN));
        }
        else                                       //12~16
        {
            BlkNum = DataLen/16;
            Bf = BlkNum+2;
            Dei = 0;  // 最后帧数据全空， 倒数第二帧以特征图样结束

            //设置特征图样
            memset(g_PdpDBuf.auData, 0xff, (Bf*R34_C_DATA_DLEN));
            *(g_PdpDBuf.auData+DataLen) = 0x7f;
        }
        puDPHdr->p_Pdp_CHdr.uDPF = DPF_CONFIRM_PACKET;  // 数据分组格式: 00-UDT数据/01-相应分/10-无确认的数据分组/11-有确认的数据分组/....
        puDPHdr->p_Pdp_CHdr.uRV1 = 0;
        puDPHdr->p_Pdp_CHdr.uHC = 0;  // 压缩头标识， 0-使用完整TCP/IP或UDP/IP头, 1-使用压缩TCP/IP或UDP/IP头
        puDPHdr->p_Pdp_CHdr.uA = 1;  //  响应请求 0-不需求应当相应，1-需求应答相应
        puDPHdr->p_Pdp_CHdr.uGI = 0;  // 0-目的为单呼地址，1-目的为组呼地址
        puDPHdr->p_Pdp_CHdr.uDEI = Dei;  // 数据结束指示 DEI
        puDPHdr->p_Pdp_CHdr.uRV2 = 0;
        //puDPHdr->p_Pdp_CHdr.uSAP = 3;  // UDP/IP 压缩头
        //puDPHdr->p_Pdp_CHdr.uBF = Bf;  //
        puDPHdr->p_Pdp_CHdr.uFMF = 1;  // 消息完整标识 FMF,  0-后续重传，1-第一次发送
        puDPHdr->p_Pdp_CHdr.uFSN = 8;  // 由单个确认消息(或数据分组)组成的逻辑消息, 其FSN为8
        puDPHdr->p_Pdp_CHdr.uNS = uNS%8;
        puDPHdr->p_Pdp_CHdr.uS = 0;
        if(g_DllGlobalCfg.auPI==1)
        {
            puDPHdr->p_Pdp_CHdr.uSAP = 11;
            g_PdpDBuf.uEHeadNum= 1;
        }
        else
        {
            puDPHdr->p_Pdp_CHdr.uSAP = 3;
            g_PdpDBuf.uEHeadNum= 0;
        }
        puDPHdr->p_Pdp_CHdr.uBF = Bf+g_PdpDBuf.uEHeadNum;//包含EHEAD

        g_PdpDBuf.uDPF         = puDPHdr->p_Pdp_CHdr.uDPF;
        g_PdpDBuf.uDEI         = Dei;  // 数据结束指示
        g_PdpDBuf.uBLOCKNum    = Bf;
        g_PdpDBuf.BF           = puDPHdr->p_Pdp_CHdr.uBF;

    }
    else
    {
        if (DataLen % 16 <= 12)
        {
            BlkNum = DataLen/16;
            Poc = 12-(DataLen%16);
            Bf = BlkNum+1;
        }
        else
        {
            BlkNum = DataLen/16;
            Poc = 16-(DataLen%16)+12;
            Bf = BlkNum+2;
        }

        puDPHdr->d_Pdp_CHdr.uDPF = DPF_CONFIRM_PACKET;
        puDPHdr->d_Pdp_CHdr.uPOCH = Poc&0x10;
        puDPHdr->d_Pdp_CHdr.uHC = 0;
        puDPHdr->d_Pdp_CHdr.uA = 1;
        puDPHdr->d_Pdp_CHdr.uGI = 0;
        puDPHdr->d_Pdp_CHdr.uPOCL = Poc&0x0f;
        //puDPHdr->d_Pdp_CHdr.uSAP = 3;
        //puDPHdr->d_Pdp_CHdr.uBF = Bf;
        puDPHdr->d_Pdp_CHdr.uF = 1;
        puDPHdr->d_Pdp_CHdr.uFSN = 8;
        puDPHdr->d_Pdp_CHdr.uNS = uNS%8;
        puDPHdr->d_Pdp_CHdr.uS = 0;
        if(g_DllGlobalCfg.auPI==1)
        {
            puDPHdr->d_Pdp_CHdr.uSAP = 9;
            g_PdpDBuf.uEHeadNum= 1;
        }
        else
        {
            puDPHdr->d_Pdp_CHdr.uSAP = 3;
            g_PdpDBuf.uEHeadNum= 0;
        }
        puDPHdr->d_Pdp_CHdr.uBF = Bf+g_PdpDBuf.uEHeadNum;//包含EHEAD

        g_PdpDBuf.uDPF         = puDPHdr->d_Pdp_CHdr.uDPF;
        g_PdpDBuf.uPOC         = Poc;
        g_PdpDBuf.uBLOCKNum    = Bf;
        g_PdpDBuf.BF           = puDPHdr->d_Pdp_CHdr.uBF;

        //将分组数据初值设置为0
        memset(g_PdpDBuf.auData, 0, (Bf*R34_C_DATA_DLEN));
    }
    if(g_DllGlobalCfg.auPI==1)//EHEAD放入g_PdpDBuf.auHead[1]
    {
        memcpy(Encry_Data.EHead,(UINT8 *)puDPHdr,DATA_HEADER_LEN);
        Encry_Data.Type= TYPE_EHEAD;
        int ret=EncryptFunc_Data(&Encry_Data);
        if(ret==0)
        {
            memcpy((UINT8 *)puDPEHdr,Encry_Data.EHead,DATA_HEADER_LEN);
            LOG_WARNING(s_LogMsgId,"[DLL][%s][34][EI DATA:]", _F_);
            LOG_PrintM(s_LogMsgId,(UINT8 *)puDPEHdr,DATA_HEADER_LEN);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s][34] Data Encryption E_HEAD Fail", _F_);
            return ERR;
        }
    }

    return NO_ERR;
}


/**
 * @brief  下行短消息数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_R12UPacketDataFun(CCL_DLL_DL_T * pvCclData)
{
    UINT8 Poc, Bf, Dei, BlkNum, i;
    UINT16 u2CRC = 0;
    UINT32 u4CRC = 0;
    PDT_NC_HEAD_PDU_DT p_PdpNcHdr;
    DMR_NC_HEAD_PDU_DT d_PdpNcHdr;
    //PDT_E_HEAD_PDU_DT  p_Pdp_EHdr;  // 空口下行加密私有头
    //DMR_E_HEAD_PDU_DT  d_Pdp_EHdr;  // 空口下行加密私有头
    DLL_ENCLIB_DATA Encry_Data;
    memset(&Encry_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    PDP_DATA_BLOCK_DU* pDPData = (PDP_DATA_BLOCK_DU*)g_PdpDBuf.auData;

    if (pvCclData->DataLen > PDP_MAX)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] data length out of range", _F_);
        return ERR;
    }

    memset(&g_PdpDBuf, 0, sizeof(PDP_DT));

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if (pvCclData->DataLen%R12_NC_DATA_DLEN < 8)          //0~8
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Bf = BlkNum+1;
            Dei = 1;

            //设置特征图样
            memset((UINT8*)pDPData, 0xff, (Bf*R12_NC_DATA_DLEN));
            *((UINT8*)pDPData+pvCclData->DataLen) = 0x7f;
        }
        else if (pvCclData->DataLen%R12_NC_DATA_DLEN == 8)    //==8
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Bf = BlkNum+1;
            Dei = 3;

            //设置特征图样
            memset((UINT8*)pDPData, 0, (Bf*R12_NC_DATA_DLEN));
        }
        else if (pvCclData->DataLen%R12_NC_DATA_DLEN == 0)    //==12
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Bf = BlkNum+1;
            Dei = 2;

            //设置特征图样
            memset((UINT8*)pDPData, 0, (Bf*R12_NC_DATA_DLEN));
        }
        else                                                //8~12
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Bf = BlkNum+2;
            Dei = 0;

            //设置特征图样
            memset((UINT8*)pDPData, 0xff, (Bf*R12_NC_DATA_DLEN));
            *((UINT8*)pDPData+pvCclData->DataLen) = 0x7f;
        }

        p_PdpNcHdr.uDPF = DPF_UNCONFIRM_PACKET;
        p_PdpNcHdr.uRV1 = 0;
        p_PdpNcHdr.uHC = 0;
        p_PdpNcHdr.uA = 0;
        p_PdpNcHdr.uGI = 1;
        p_PdpNcHdr.uDEI = Dei;
        p_PdpNcHdr.uRV2 = 0;
        p_PdpNcHdr.uFMF = 1;
        p_PdpNcHdr.uFSN = 0;
        p_PdpNcHdr.uRV3 = 0;
        if(g_DllGlobalCfg.auPI==1)
        {
            p_PdpNcHdr.uSAP = 11;
            g_PdpDBuf.uEHeadNum= 1;
        }
        else
        {
            p_PdpNcHdr.uSAP = 3;
            g_PdpDBuf.uEHeadNum= 0;
        }
        p_PdpNcHdr.uBF = Bf+g_PdpDBuf.uEHeadNum;//包含EHEAD
        memcpy(p_PdpNcHdr.auTADDR, pvCclData->DstId, 3);
        memcpy(p_PdpNcHdr.auSADDR, pvCclData->SrcId, 3);

        g_PdpDBuf.uDPF         = p_PdpNcHdr.uDPF;
        g_PdpDBuf.uState       = PDP_STATE_FIRST;
        g_PdpDBuf.uDataType    = DT_R_1_2_DATA;
        g_PdpDBuf.uDEI         = Dei;
        g_PdpDBuf.uHdrNum      = 1+g_PdpDBuf.uEHeadNum;
        g_PdpDBuf.uBLOCKNum    = Bf;
        g_PdpDBuf.BF           = p_PdpNcHdr.uBF;

        memcpy(g_PdpDBuf.auHead[0], &p_PdpNcHdr, DATA_HEADER_LEN);

        if(g_DllGlobalCfg.auPI==1)//EHEAD放入g_PdpDBuf.auHead[1]
        {
            memcpy(Encry_Data.EHead,&p_PdpNcHdr,DATA_HEADER_LEN);
            Encry_Data.Type= TYPE_EHEAD;
            int ret=EncryptFunc_Data(&Encry_Data);
            if(ret==0)
            {
                memcpy(g_PdpDBuf.auHead[1],Encry_Data.EHead,DATA_HEADER_LEN);
                LOG_WARNING(s_LogMsgId,"[DLL][%s][PDT][EI DATA:]", _F_);
                LOG_PrintM(s_LogMsgId,Encry_Data.EHead,DATA_HEADER_LEN);
            }
            else
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s][PDT] Data Encryption E_HEAD Fail", _F_);
                return ERR;
            }
        }
    }
    else
    {
        if (pvCclData->DataLen%R12_NC_DATA_DLEN <= 8)
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Poc = 8-(pvCclData->DataLen%R12_NC_DATA_DLEN);
            Bf = BlkNum+1;
        }
        else
        {
            BlkNum = pvCclData->DataLen/R12_NC_DATA_DLEN;
            Poc = 12-(pvCclData->DataLen%R12_NC_DATA_DLEN)+8;
            Bf = BlkNum+2;
        }

        d_PdpNcHdr.uDPF = DPF_UNCONFIRM_PACKET;
        d_PdpNcHdr.uPOCH = Poc&0x10;
        d_PdpNcHdr.uHC = 0;
        d_PdpNcHdr.uA = 0;
        d_PdpNcHdr.uGI = 1;
        d_PdpNcHdr.uPOCL = Poc&0x0f;
        //d_PdpNcHdr.uSAP = 3;
        //d_PdpNcHdr.uBF = Bf;
        d_PdpNcHdr.uF = 1;
        d_PdpNcHdr.uFSN = 0;
        d_PdpNcHdr.uRV1 = 0;
        if(g_DllGlobalCfg.auPI==1)
        {
            d_PdpNcHdr.uSAP = 9;
            g_PdpDBuf.uEHeadNum= 1;
        }
        else
        {
            d_PdpNcHdr.uSAP = 3;
            g_PdpDBuf.uEHeadNum= 0;
        }
        d_PdpNcHdr.uBF = Bf+g_PdpDBuf.uEHeadNum;//包含EHEAD
        memcpy(d_PdpNcHdr.auTADDR, pvCclData->DstId, 3);
        memcpy(d_PdpNcHdr.auSADDR, pvCclData->SrcId, 3);

        g_PdpDBuf.uDPF         = d_PdpNcHdr.uDPF;
        g_PdpDBuf.uState       = PDP_STATE_FIRST;
        g_PdpDBuf.uDataType    = DT_R_1_2_DATA;
        g_PdpDBuf.uPOC         = Poc;
        g_PdpDBuf.uHdrNum      = 1+g_PdpDBuf.uEHeadNum;
        g_PdpDBuf.uBLOCKNum    = Bf;
        g_PdpDBuf.BF           = d_PdpNcHdr.uBF;

        memcpy(g_PdpDBuf.auHead[0], &d_PdpNcHdr, DATA_HEADER_LEN);
        if(g_DllGlobalCfg.auPI==1)//EHEAD放入g_PdpDBuf.auHead[1]
        {
            memcpy(Encry_Data.EHead,&d_PdpNcHdr,DATA_HEADER_LEN);
            Encry_Data.Type= TYPE_EHEAD;
            int ret=EncryptFunc_Data(&Encry_Data);
            if(ret==0)
            {
                memcpy(g_PdpDBuf.auHead[1],Encry_Data.EHead,DATA_HEADER_LEN);
                LOG_WARNING(s_LogMsgId,"[DLL][%s][DMR][EI DATA:]", _F_);
                LOG_PrintM(s_LogMsgId,Encry_Data.EHead,DATA_HEADER_LEN);
            }
            else
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s][DMR] Data Encryption E_HEAD Fail", _F_);
                return ERR;
            }
        }

        //将分组数据初值设置为0
        memset((UINT8*)pDPData, 0, (Bf*R12_NC_DATA_DLEN));
    }


    u2CRC = ALG_Crc16(g_PdpDBuf.auHead[0], DATA_HEADER_LEN);
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_DATA;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_DATA;
    }

    g_PdpDBuf.auHead[0][DATA_HEADER_LEN]     = (UINT8)(u2CRC >> 8);
    g_PdpDBuf.auHead[0][DATA_HEADER_LEN + 1] = (UINT8)(u2CRC & 0xff);
    if(g_DllGlobalCfg.auPI==1)
    {
        u2CRC = ALG_Crc16(g_PdpDBuf.auHead[1], DATA_HEADER_LEN);
        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            u2CRC = u2CRC ^ PDT_CRC_MASK_DATA;
        }
        else
        {
            u2CRC = u2CRC ^ DMR_CRC_MASK_DATA;
        }
        g_PdpDBuf.auHead[1][DATA_HEADER_LEN]     = (UINT8)(u2CRC >> 8);
        g_PdpDBuf.auHead[1][DATA_HEADER_LEN + 1] = (UINT8)(u2CRC & 0xff);
    }
    memcpy(g_PdpDBuf.auData, pvCclData->PayLoad, pvCclData->DataLen);

    LOG_WARNING(s_LogMsgId,"[DLL][%s][Before][ADMIN DATA:]", _F_);
    LOG_PrintM(s_LogMsgId,g_PdpDBuf.auData,pvCclData->DataLen);
    //数据加密
    if(g_DllGlobalCfg.auPI==1)
    {
        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            Encry_Data.Data=g_PdpDBuf.auData+5;
            Encry_Data.DataLen=pvCclData->DataLen-5;
        }
        else
        {
            Encry_Data.Data=g_PdpDBuf.auData;
            Encry_Data.DataLen=pvCclData->DataLen;
        }
        Encry_Data.Type= TYPE_DATA;
        int ret=EncryptFunc_Data(&Encry_Data);
        if(ret==0)
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s][After][ADMIN DATA:]", _F_);
            LOG_PrintM(s_LogMsgId,g_PdpDBuf.auData,pvCclData->DataLen);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s]Data Encryption ADMIN DATA Fail", _F_);
            return ERR;
        }
    }
    //数据加密结束
    u4CRC = ALG_Crc32((UINT8*)pDPData, R12_NC_DATA_DLEN*Bf - 4);
    // 填充CRC32
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-4] = (UINT8)(u4CRC & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-3] = (UINT8)((u4CRC >>  8) & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-2] = (UINT8)((u4CRC >> 16) & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-1] = (UINT8)(u4CRC >> 24);

    //每包只有12字节，无包序号和头校验，只有尾帧的4字节校验，因为1/2本身冗余多，纠错能力强
    //发送多个预载波
    ODP_GenPreCSBKFun((Bf+6), pvCclData->DstId, pvCclData->SrcId, 1);
    ODP_GenPreCSBKFun((Bf+5), pvCclData->DstId, pvCclData->SrcId, 1);
    ODP_GenPreCSBKFun((Bf+4), pvCclData->DstId, pvCclData->SrcId, 1);
    ODP_GenPreCSBKFun((Bf+3), pvCclData->DstId, pvCclData->SrcId, 1);
    ODP_GenPreCSBKFun((Bf+2), pvCclData->DstId, pvCclData->SrcId, 1);
    ODP_GenPreCSBKFun((Bf+1), pvCclData->DstId, pvCclData->SrcId, 1);

    //下行DataHeader组包
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_DATA_HEADER,(DATA_HEADER_LEN+2),g_PdpDBuf.auHead[0]);
    ODP_SendInfData(ptInfData, S_U_HDR);
    //下行EHEAD组包

    if(g_DllGlobalCfg.auPI==1)
    {
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_DATA_HEADER,(DATA_HEADER_LEN+2),g_PdpDBuf.auHead[1]);
        ODP_SendInfData(ptInfData, S_E_HDR);
    }
    //下行R1/2 Data组包
    for (i = 0; i < Bf; i++)
    {
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_R_1_2_DATA,R12_NC_DATA_DLEN,pDPData->tPdpR12NcBlk.auData[i]);
        ODP_SendInfData(ptInfData, S_DATA_1_2);
    }

    return NO_ERR;
}


/**
 * @brief 下行MS GPS上拉数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_MsGpsRequestFun(CCL_DLL_DL_T * pvCclData)
{
    UINT8 Bf, i;
    UINT8 GpsReq[28];
    UINT8 r34Data[18];
    UINT8 GpsLen;
    UINT16 u2CRC = 0;
    UINT32 u4CRC = 0;
    UINT8 frq = (tDllPrint->FrqSlt >> 4) & 0x0f;
    PDP_HEAD_DU puHdr;
    PDP_HEAD_DU puEHdr;
    PDP_DATA_DU *puData;
    DLL_ENCLIB_DATA Encry_Data;
    PDP_DATA_BLOCK_DU* pDPData = (PDP_DATA_BLOCK_DU *)g_PdpDBuf.auData;  //     pDPData 作为临时的真实数据存储区

    if (pvCclData->DataLen > 8)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] data length out of GPS Req", _F_);
        return ERR;
    }

    memset(&g_PdpDBuf, 0, sizeof(PDP_DT));
    /*
        ** PayLoad[0]-CallId  (CCL赋值)
        ** PayLoad[4]-10.0.0.143 (假设值)
        */
    ODP_GenGpsReqData(&pvCclData->PayLoad[0], &pvCclData->PayLoad[4], GpsReq, &GpsLen);  // 产生GPS数据GpsReq 及数据长度
    ODP_R34CPacketDataFun(GpsLen, &puHdr,&puEHdr);  // 下行R34带确认头数据组包，全局数据更新，内部设置图样

    if (tDllPrint->AIDown == 1)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s][%d] Ms GpsReq: ", _F_, __LINE__);
        LOG_PrintM(s_LogMsgId, GpsReq, GpsLen);
    }

    memcpy(puHdr.p_Pdp_CHdr.auTADDR, pvCclData->DstId, 3);
    memcpy(puHdr.p_Pdp_CHdr.auSADDR, pvCclData->SrcId, 3);
    memcpy(g_PdpDBuf.uDstId, pvCclData->DstId, 3);
    memcpy(g_PdpDBuf.uSrcId, pvCclData->SrcId, 3);

    Bf = g_PdpDBuf.BF-g_PdpDBuf.uEHeadNum;  // blocks to follow 后续的帧数量
    memcpy(g_PdpDBuf.auHead[0], &puHdr, DATA_HEADER_LEN);
    u2CRC = ALG_Crc16(g_PdpDBuf.auHead[0], DATA_HEADER_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_DATA;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_DATA;
    }

    g_PdpDBuf.auHead[0][DATA_HEADER_LEN]     = (UINT8)(u2CRC >> 8);
    g_PdpDBuf.auHead[0][DATA_HEADER_LEN + 1] = (UINT8)(u2CRC & 0xff);
    if(g_DllGlobalCfg.auPI==1)//EHEAD放入g_PdpDBuf.auHead[1]
    {
        memcpy(g_PdpDBuf.auHead[1], &puEHdr, DATA_HEADER_LEN);
        u2CRC = ALG_Crc16(g_PdpDBuf.auHead[1], DATA_HEADER_LEN);
        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            u2CRC = u2CRC ^ PDT_CRC_MASK_DATA;
        }
        else
        {
            u2CRC = u2CRC ^ DMR_CRC_MASK_DATA;
        }
        g_PdpDBuf.auHead[1][DATA_HEADER_LEN]     = (UINT8)(u2CRC >> 8);
        g_PdpDBuf.auHead[1][DATA_HEADER_LEN + 1] = (UINT8)(u2CRC & 0xff);
    }
    memcpy(g_PdpDBuf.auData, GpsReq, GpsLen);  // GPS 拷贝,此处是带图样不带单包校验的纯数据
    LOG_WARNING(s_LogMsgId,"[DLL][%s][Before][ADMIN DATA:]", _F_);
    LOG_PrintM(s_LogMsgId,g_PdpDBuf.auData,GpsLen);
    //数据加密
    if(g_DllGlobalCfg.auPI==1)
    {
        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            Encry_Data.Data=g_PdpDBuf.auData+sizeof(IPv4_COMPRESS_HEADER_T);
            Encry_Data.DataLen=GpsLen-sizeof(IPv4_COMPRESS_HEADER_T);
        }
        else
        {
            Encry_Data.Data=g_PdpDBuf.auData;
            Encry_Data.DataLen=GpsLen;
        }
        Encry_Data.Type= TYPE_DATA;
        int ret=EncryptFunc_Data(&Encry_Data);
        //UINT8 TempGps[15]={0xbc,0xfc,0xc8,0x36,0xfa,0x6b,0x2c,0x22,0x94,0xa3,0x92,0x0d,0x27,0x0d,0xc6};
        if(ret==0)
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s][After][Len=%d][ADMIN DATA:]", _F_,Encry_Data.DataLen);
            //memcpy(g_PdpDBuf.auData+5,TempGps,GpsLen-5);
            LOG_PrintM(s_LogMsgId,g_PdpDBuf.auData,GpsLen);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Data Encryption ADMIN DATA Fail", _F_);
            return ERR;
        }
    }

    //数据加密结束

    u4CRC = ALG_Crc32((UINT8 *)pDPData, ((R34_NC_DATA_DLEN-2)*Bf - 4));//带错误图样不带校验进行CRC32
    // 填充CRC32
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-4] = (UINT8)(u4CRC & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-3] = (UINT8)((u4CRC >> 8) & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-2] = (UINT8)((u4CRC >> 16) & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-1] = (UINT8)(u4CRC >> 24);

    /*
    ** 发送多个预载波
    */
    ODP_GenPreCSBKFun((Bf+6), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+5), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+4), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+3), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+2), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+1), pvCclData->DstId, pvCclData->SrcId, 0);

    /*
    ** 下行DataHeader组包
    */
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO ,DT_DATA_HEADER, (DATA_HEADER_LEN+2), g_PdpDBuf.auHead[0]);
    ODP_SendInfData(ptInfData, S_C_HDR);

    //下行EHEAD组包
    if(g_DllGlobalCfg.auPI==1)
    {
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_DATA_HEADER,(DATA_HEADER_LEN+2),g_PdpDBuf.auHead[1]);
        ODP_SendInfData(ptInfData, S_E_HDR);
    }

    /*
    ** 发3/4数据
    */
    ptInfData->tDataLink[frq].DataType = DT_R_3_4_DATA;
    ptInfData->tDataLink[frq].DataLen = 33;
    puData = (PDP_DATA_DU *)r34Data;
    for (i = 0; i < Bf; i++)
    {
        memcpy(puData->uR34Data.tR34CData.auData, pDPData->tPdpR34CBlk.auData[i], (R34_NC_DATA_DLEN-2));  // GPS 拷贝
        // 填充DBSN
        puData->uR34Data.tR34CData.uDBSN = g_PdpDBuf.s_uDBSNIndex;  // DBSN(Data Block Serial Number)

        // 填充CRC9
        u2CRC = ALG_Crc9((UINT8*)puData, R34_C_DATA_DLEN);

        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            u2CRC = u2CRC ^ PDT_CRC_MASK_R34;
        }
        else
        {
            u2CRC = u2CRC ^ DMR_CRC_MASK_R34;
        }
        puData->uR34Data.tR34CData.uCRC9H = (u2CRC >> 8) & 0x01;
        puData->uR34Data.tR34CData.uCRC9L = u2CRC & 0xff;

        if (tDllPrint->AIDown == 1)
        {
            if (tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL)
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s] R34_C_DATA", _F_);
                LOG_PrintM(s_LogMsgId, r34Data, 18);
            }
        }

        ODP_144to196bptc(r34Data, ptInfData->tDataLink[frq].PayLoad); // 下行(196,144)BPTC 编码函数
        //数据头已组包完毕，不用再次系统组包，只需要更改相应变化量即可
        ODP_SendInfData(ptInfData, S_DATA_3_4);
        g_PdpDBuf.s_uDBSNIndex++;
    }

    /*
    ** 发 TD_LC
    */
    if((PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)&&(g_DllGlobalCfg.auPI==0))
        ODP_TerDataLC(pvCclData->DstId, pvCclData->SrcId, 1, 0xff, 0xff);        //TD_LC 预留0xff
    else
        ODP_TerDataLC(pvCclData->DstId, pvCclData->SrcId, 0, 0xff, 0xff);       //TD_LC 预留0xff

    for (i = 0; i < 5; i++)
    {
        if (1 == p_DllFpgaShm->FollowEn)//什么情况会拉高  下行时隙锁定还是上行锁定    还是等待FPGA发完
        {
            uGpsFlag = GPS_REQ;
            break;
        }
        else
        {
            usleep(60000);  // 60ms
        }
    }
    DLL_SetTimer(CALL_DATA_D, 4000);        //定时4s
    //LOG_DEBUG(s_LogMsgId,"[DLL][%s] Down TD_LC OK", _F_);

    return NO_ERR;
}


/**
 * @brief 下行MS 摇晕数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_MsDisableFun(CCL_DLL_DL_T * pvCclData)
{
    UINT16 u2CRC    = 0;
    SUPS_CSBK_PDU tSupsCsbk;
    tSupsCsbk.uLB = 1;  // This bit shall be set to 1
    tSupsCsbk.uPF = 0;  // Default value is 0
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        tSupsCsbk.uCSBKO = REQ_CSBKO;  //  Shall be set to 101001(0x29)
        tSupsCsbk.uFID = HFID;   // hytera
        tSupsCsbk.uSSO = DIS_RADIO_SSO;   // Supplementary Service Options = Radio Disable Service
        tSupsCsbk.uREV = 0;

        memcpy(tSupsCsbk.auTADDR, pvCclData->DstId, 3);
        memcpy(tSupsCsbk.auSADDR, pvCclData->SrcId, 3);
    }
    else
    {
        tSupsCsbk.uCSBKO = REQ_CSBKO_DMR;
        tSupsCsbk.uFID = MFID;
        tSupsCsbk.uSSO = 0;
        tSupsCsbk.uREV = DIS_RADIO_REV_DMR;
        memcpy(tSupsCsbk.auTADDR, pvCclData->SrcId, 3);
        memcpy(tSupsCsbk.auSADDR, pvCclData->DstId, 3);
    }

    u2CRC = ALG_Crc16((UINT8 *)&tSupsCsbk, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }
    tSupsCsbk.auCRC[0] = (UINT8)(u2CRC >> 8);
    tSupsCsbk.auCRC[1] = (UINT8)(u2CRC & 0xff);
    //发送多个预载波
    ODP_GenPreCSBKFun(6, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(5, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(4, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(3, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(2, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(1, pvCclData->DstId, pvCclData->SrcId, 0);
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    ptInfData->ResFrqSlot = tDllPrint->FrqSlt + 0x11; // 预留时隙频点和指定频点差1，同频同时隙上行开放，+0x11是为了适应FPGA接口
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_CSBK, (CSBK_LEN+2), (UINT8 *)&tSupsCsbk);
    ODP_SendInfData(ptInfData, S_KILL_REQ);
    return NO_ERR;
}


/**
 * @brief 下行MS 激活数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_MsEnableFun(CCL_DLL_DL_T * pvCclData)
{
    UINT16 u2CRC    = 0;
    SUPS_CSBK_PDU tSupsCsbk;

    tSupsCsbk.uLB = 1;
    tSupsCsbk.uPF = 0;
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        tSupsCsbk.uCSBKO = REQ_CSBKO;
        tSupsCsbk.uFID = HFID;
        tSupsCsbk.uSSO = EN_RADIO_SSO;
        tSupsCsbk.uREV = 0;

        memcpy(tSupsCsbk.auTADDR, pvCclData->DstId, 3);
        memcpy(tSupsCsbk.auSADDR, pvCclData->SrcId, 3);
    }
    else
    {
        tSupsCsbk.uCSBKO = REQ_CSBKO_DMR;
        tSupsCsbk.uFID = MFID;
        tSupsCsbk.uSSO = 0;
        tSupsCsbk.uREV = EN_RADIO_REV_DMR;

        memcpy(tSupsCsbk.auTADDR, pvCclData->SrcId, 3);
        memcpy(tSupsCsbk.auSADDR, pvCclData->DstId, 3);
    }

    u2CRC = ALG_Crc16((UINT8 *)&tSupsCsbk, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }
    tSupsCsbk.auCRC[0] = (UINT8)(u2CRC >> 8);
    tSupsCsbk.auCRC[1] = (UINT8)(u2CRC & 0xff);

    //发送多个预载波
    ODP_GenPreCSBKFun(6, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(5, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(4, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(3, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(2, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(1, pvCclData->DstId, pvCclData->SrcId, 0);

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    ptInfData->ResFrqSlot = tDllPrint->FrqSlt+0x11;         //预留时隙频点和指定频点差1
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_CSBK, (CSBK_LEN+2), (UINT8 *)&tSupsCsbk);

    ODP_SendInfData(ptInfData, S_ENABLE_REQ);

    return NO_ERR;
}



/**
 * @brief 下行MS 告警 NAS和MS通用
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
int ODP_MsNasAlarmReqFun(CCL_DLL_DL_T * pvCclData)
{
    UINT16 u2CRC    = 0;
    SUPS_CSBK_PDU tSupsCsbk;
    static UINT32 SeqSSO = 0;

    tSupsCsbk.uLB = 1;  // CSBK or MBC Last Block
    tSupsCsbk.uPF = 0;  // Reserved for future use
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        tSupsCsbk.uCSBKO = ALARM_CSBKO;
        tSupsCsbk.uFID = HFID;
        tSupsCsbk.uSSO = (SeqSSO++) % DIGITAL_ALARM_SSO; // 从目前的空口包分析，该值是一个小于0x0f的序列。
    }
    else
    {
        tSupsCsbk.uCSBKO = ALARM_CSBKO_DMR;
        tSupsCsbk.uFID = MFID;
        tSupsCsbk.uSSO = (SeqSSO++) % DIGITAL_ALARM_SSO_DMR; // 从目前的空口包分析，该值是一个小于0x08的序列。
    }
    tSupsCsbk.uREV = 0;

    memcpy(tSupsCsbk.auTADDR, pvCclData->DstId, 3);
    memcpy(tSupsCsbk.auSADDR, pvCclData->SrcId, 3);

    u2CRC = ALG_Crc16((UINT8 *)&tSupsCsbk, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }
    tSupsCsbk.auCRC[0] = (UINT8)(u2CRC >> 8);
    tSupsCsbk.auCRC[1] = (UINT8)(u2CRC & 0xff);

    //发送多个预载波
    ODP_GenPreCSBKFun(6, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(5, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(4, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(3, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(2, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(1, pvCclData->DstId, pvCclData->SrcId, 0);

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
//    ptInfData->ResFrqSlot = tDllPrint->FrqSlt+0x11;   //预留时隙频点和指定频点差1，告警不需要ACK
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_CSBK, (CSBK_LEN+2), (UINT8 *)&tSupsCsbk);
    ODP_SendInfData(ptInfData, S_ALARM_REQ);
    return NO_ERR;
}


/**
 * @brief 下行MS 告警 应答
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */

int ODP_MsAlarmAckFun(CCL_DLL_DL_T * pvCclData)
{
    UINT16 u2CRC    = 0;
    SUPS_CSBK_PDU tSupsCsbk;

    tSupsCsbk.uLB = 1;
    tSupsCsbk.uPF = 0;
    tSupsCsbk.uCSBKO = ALARM_CSBKO;
    tSupsCsbk.uFID = HFID;
    tSupsCsbk.uSSO = DIGITAL_ALARM_SSO;
    tSupsCsbk.uREV = 0;

    memcpy(tSupsCsbk.auTADDR, pvCclData->DstId, 3);
    memcpy(tSupsCsbk.auSADDR, pvCclData->SrcId, 3);

    u2CRC = ALG_Crc16((UINT8 *)&tSupsCsbk, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }
    tSupsCsbk.auCRC[0] = (UINT8)(u2CRC >> 8);
    tSupsCsbk.auCRC[1] = (UINT8)(u2CRC & 0xff);

    //发送多个预载波
    ODP_GenPreCSBKFun(6, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(5, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(4, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(3, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(2, pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun(1, pvCclData->DstId, pvCclData->SrcId, 0);

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
//    ptInfData->ResFrqSlot = tDllPrint->FrqSlt+0x11;         //预留时隙频点和指定频点差1
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_CSBK, (CSBK_LEN+2), (UINT8 *)&tSupsCsbk);

    ODP_SendInfData(ptInfData, S_ALARM_ACK);

    return NO_ERR;
}


/**
 * @brief 下行NAS GPS上拉数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_NasGpsReportFun(CCL_DLL_DL_T * pvCclData)
{
    NAS_AI_PAYLOAD NasAiData;//12字节的空口数据结构
    GPS_NAS_DATA_T GpsNasData;//8字节的节点数据结构
    GPS_DEV_DATA_T *GpsDevData;//FPGA共享内存的GPS数据结构
    unsigned char DstId;
    unsigned char SrcId;
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (DEV_ID_32 < pvCclData->DstId[0])
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas GPS Report DstId over range: %d", _F_, pvCclData->DstId[0]);
        return ERR;
    }

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])   // 本地节点查询
    {
        GpsDevData = (GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData);
        IDP_GpsDevToNas(GpsDevData, &GpsNasData);//转换为8字节的节点数据结构

        if (tDllPrint->AIDown == 1)
        {
            LOG_DEBUG(s_LogMsgId,"[DLL][%s] NS_%d, EW_%d, NDEG_%d, EDEG_%d NMINmm_%d EMINmm_%d NMINF_%ld EMINF_%ld",
                    _F_,
                    GpsDevData->NS,
                    GpsDevData->EW,
                    GpsDevData->NDEG,
                    GpsDevData->EDEG,
                    GpsDevData->NMINmm,
                    GpsDevData->EMINmm,
                    GpsDevData->NMINF,
                    GpsDevData->EMINF);
            LOG_DEBUG(s_LogMsgId,"[DLL][%s] Nas GPS Report query local %d", _F_, sizeof(GPS_DEV_DATA_T));
            LOG_PrintM(s_LogMsgId,p_DllFpgaShm->GpsData,20);
        }

        ptCclData->MsgType = DI_MSG_WLU;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_GPS_REPORT_ACK_NAS;

        memcpy(ptCclData->SrcId, pvCclData->DstId, 3);
        memcpy(ptCclData->DstId, pvCclData->SrcId, 3);
        ptCclData->DataLen = GPS_LEN;
        memcpy(ptCclData->PayLoad, &GpsNasData, GPS_LEN);

        IDP_SendCclData(ptCclData);

    }
    else   // 其他节点查询
    {
        NasAiData.cmd_code = CMD_CODE_GPS_REQUEST;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_GET;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvCclData->DstId[0];
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

        //发送多个预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_GPS_REQ);
    }

    return NO_ERR;
}


/**
 * @brief 下行NAS 激活数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_NasEnableFun(CCL_DLL_DL_T * pvCclData)
{
    NAS_AI_PAYLOAD NasAiData;
    unsigned char DstId;
    unsigned char SrcId;
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (DEV_ID_32 < pvCclData->DstId[0])
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas Enable DstId over range: %d", _F_, pvCclData->DstId[0]);
        return ERR;
    }

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])
    {
        g_DllGlobalCfg.auStunFlag = 0;
        g_DllGlobalCfg.auKillFlag = 0;
        ptCFGShm->kill_flag.val   = 0;
        ptCFGShm->stun_flag.val   = 0;
        p_DllFpgaShm->StunFlag[0] = 0;
        p_DllFpgaShm->StunFlag[1] = 0;
        p_DllFpgaShm->StunFlag[2] = 0;
        p_DllFpgaShm->StunFlag[3] = 0;
        ptCclData->MsgType  = DI_MSG_WLU;
        ptCclData->FrmType  = FT_VOICE_NO;
        ptCclData->DataType = CT_ENABLE_ACK_NAS;
        memcpy(ptCclData->SrcId, pvCclData->SrcId, 3);
        memcpy(ptCclData->DstId, pvCclData->DstId, 3);
        ptCclData->DataLen = 0;
        IDP_SendCclData(ptCclData);
    }
    else
    {
        NasAiData.cmd_code = CMD_CODE_ENABLE;
        NasAiData.nm_type  = NM_TYPE_CENTER;
        NasAiData.op_code  = OP_CODE_SET;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvCclData->DstId[0];
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
        //发送多个预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_ENABLE_REQ);
    }

    return NO_ERR;
}


/**
 * @brief 下行NAS 摇晕数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_NasStunFun(CCL_DLL_DL_T * pvCclData)
{
    NAS_AI_PAYLOAD NasAiData;
    unsigned char DstId;
    unsigned char SrcId;
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (DEV_ID_32 < pvCclData->DstId[0])
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas Stun DstId over range: %d", _F_, pvCclData->DstId[0]);
        return ERR;
    }

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])
    {
        ptCFGShm->stun_flag.val   = NAS_KILL_FLAG;
        g_DllGlobalCfg.auStunFlag = NAS_KILL_FLAG;
        p_DllFpgaShm->StunFlag[0] = NAS_KILL_FLAG      &0xff;
        p_DllFpgaShm->StunFlag[1] = (NAS_KILL_FLAG>>8) &0xff;
        p_DllFpgaShm->StunFlag[2] = (NAS_KILL_FLAG>>16)&0xff;
        p_DllFpgaShm->StunFlag[3] = (NAS_KILL_FLAG>>24)&0xff;

        ptCclData->MsgType = DI_MSG_WLU;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_STUN_ACK_NAS;

        memcpy(ptCclData->SrcId, pvCclData->SrcId, 3);
        memcpy(ptCclData->DstId, pvCclData->DstId, 3);
        ptCclData->DataLen = 0;

        IDP_SendCclData(ptCclData);

    }
    else
    {
        NasAiData.cmd_code = CMD_CODE_STUN;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_SET;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvCclData->DstId[0];
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

        //发送多个预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU,(NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);
        ODP_SendInfData(ptInfData, S_NAS_STUN_REQ);
    }

    return NO_ERR;
}


/**
 * @brief 下行NAS 摇毙数据处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_NasKillFun(CCL_DLL_DL_T * pvCclData)
{
    NAS_AI_PAYLOAD NasAiData;
    unsigned char DstId;
    unsigned char SrcId;

    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (DEV_ID_32 < pvCclData->DstId[0])
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas Kill DstId over range: %d", _F_, pvCclData->DstId[0]);
        return ERR;
    }

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])
    {
        g_DllGlobalCfg.auKillFlag = NAS_KILL_FLAG;
        ptCFGShm->kill_flag.val = NAS_KILL_FLAG;
        p_DllFpgaShm->StunFlag[0] = NAS_KILL_FLAG      &0xff;
        p_DllFpgaShm->StunFlag[1] = (NAS_KILL_FLAG>>8) &0xff;
        p_DllFpgaShm->StunFlag[2] = (NAS_KILL_FLAG>>16)&0xff;
        p_DllFpgaShm->StunFlag[3] = (NAS_KILL_FLAG>>24)&0xff;

        ptCclData->MsgType = DI_MSG_WLU;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_KILL_ACK_NAS;

        memcpy(ptCclData->SrcId, pvCclData->SrcId, 3);
        memcpy(ptCclData->DstId, pvCclData->DstId, 3);
        ptCclData->DataLen = 0;

        IDP_SendCclData(ptCclData);

    }
    else
    {
        NasAiData.cmd_code = CMD_CODE_KILL;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_SET;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvCclData->DstId[0];
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

        //发送多个预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU,FT_VOICE_NO,DI_MSG_WLU,(NM_DATA_LEN+1),(UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_KILL_REQ);
    }

    return NO_ERR;
}


void SaveNerCheckLog()
{
#if NER_CHECK_DEBUG
#if  1
    int Len;
    int ret;
    time_t timep;
    struct timeval tSndTime;
    static unsigned long index = 0;
    char Buf[100];
    printf("~~~~~~~~~~~~~~~~~~~~~~~[%s:%d]\n", __FUNCTION__, __LINE__);
    Len = snprintf(Buf, sizeof(Buf), "%.2d:%.2d:%.2d:%.3d>-%#.8lx-%lu\n",(int)localtime(&timep)->tm_hour, (int)localtime(&timep)->tm_min, (int)localtime(&timep)->tm_sec,(int)tSndTime.tv_usec/1000, g_DllGlobalCfg.auNegrId2, index++);
    printf("~[%s:%d]@2Buf=%s, Len=%d\n", __FUNCTION__, __LINE__, Buf, Len);
    ret = fwrite(Buf, 1, Len, g_fpNerCheckLog);
#else
    printf("[%s:%d]!~~~~~~~~~~~\n", __FUNCTION__, __LINE__);
    static unsigned long index = 0;
    fprintf(g_fpNerCheckLog, "%#x-%u\n",  g_DllGlobalCfg.auNegrId2, index++);
#endif
#endif
}
/**
 * @brief 下行NAS 查询邻点信息处理函数
 *
 * @param [in]  pvCclData       下行控制层数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int ODP_NegrQueryFun(CCL_DLL_DL_T * pvCclData)
{
    NAS_AI_PAYLOAD NasAiData;
    unsigned char DstId;
    unsigned char SrcId;
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (DEV_ID_32 < pvCclData->DstId[0])
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas neighbor data DstId over range: %d", _F_, pvCclData->DstId[0]);
        return ERR;
    }

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])  // 邻点查询-本地
    {
        ptCclData->MsgType = DI_MSG_WLU;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_NEGHR_QUERY_ACK;
        memcpy(ptCclData->SrcId, pvCclData->SrcId, 3);
        memcpy(ptCclData->DstId, pvCclData->DstId, 3);
        ptCclData->DataLen = NER_LEN+1;
        ptCclData->PayLoad[0] = g_DllGlobalCfg.auNodeId;
        memcpy(&ptCclData->PayLoad[1], (UINT8 *)&g_DllGlobalCfg.auNegrId2, NER_LEN);

        if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
        {
            CheckNerArray();
            CheckNerInfo(&g_DllGlobalCfg.auNodeId, &g_DllGlobalCfg.auNegrId2, "Local->CC");
            CheckNerMatrixPrint();
            SaveNerCheckLog();
        }
        IDP_SendCclData(ptCclData);
    }
    else  // 邻点查询-非本地
    {
        NasAiData.cmd_code = CMD_CODE_NER_QUERY;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_GET;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvCclData->DstId[0];
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

        //发送多个预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
        ODP_SendInfData(ptInfData, S_NAS_NEGR_REQ);
    }

    return NO_ERR;
}
void GPS_GetLonLat(GPS_NAS_DATA_T *ptGpsData)
{
    ptGpsData->EW = 0; //EAST;
    ptGpsData->EDEG = 126;   //<8bit 经度     0~179
    ptGpsData->EMINmm = 31;  //<6bit经分的整数部分 0~59
    ptGpsData->EMINF = 4216; //<14bit经分的小数部分 0~9999
    ptGpsData->NS = 1;  //NORTH;
    ptGpsData->NDEG = 45;    //度
    ptGpsData->NMINmm = 5;   //0~59
    ptGpsData->NMINF = 6674; //0~9999
}

/**
 * @brief   获取纬度
 *
 * @param [in]
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
void GPS_GenEmbLat(GPS_NAS_DATA_T *ptGpsData, unsigned char *pucLat, unsigned char *pucFlg)
{
    int i;
    double dTmp;
    unsigned long  ulBitNum;

    // 计算纬度
    dTmp = ((double)ptGpsData->NDEG + (double)(ptGpsData->NMINmm/60.0) + (double)(ptGpsData->NMINF)/(60*10000.0));

    ulBitNum = (unsigned long)(dTmp * LAT_RATIO);
    ulBitNum |= ptGpsData->NS << LAT_BIT;
    *pucFlg = ptGpsData->NS;

    for (i = 0; i < LON_LAT_LEN; i++)
    {
        pucLat[LON_LAT_LEN-i-1] = (ulBitNum >> i*8) & 0xff;
    }
}



/**
 * @brief   获取经度
 *
 * @param [in]
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
void GPS_GenEmbLon(GPS_NAS_DATA_T *ptGpsData, unsigned char *pucLon, unsigned char *pucFlg)
{
    int i;
    double dTmp;
    unsigned long ulBitNum;
    // 计算精度

    dTmp = ((double)ptGpsData->EDEG + (double)(ptGpsData->EMINmm/60.0) + (double)(ptGpsData->EMINF)/(60*10000.0));
    ulBitNum = (unsigned long)(dTmp * LON_RATIO);
    *pucFlg = ptGpsData->EW;

    for (i = 0; i < LON_LAT_LEN; i++)
    {
        pucLon[LON_LAT_LEN-i-1] = (ulBitNum >> i*8) & 0xff;
    }
}


/**
 * @brief   获取当前内嵌LC
 *
 * @param [in]
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
void GPS_GenFlcEmb(GPS_NAS_DATA_T *ptGpsData, EMBBED_GPS_PDU *ptGpsPdu)
{
    unsigned char ucLonFlg;
    unsigned char ucLatFlg;
    GPS_GenEmbLon(ptGpsData, ptGpsPdu->uLongitude, &ucLonFlg);
    GPS_GenEmbLat(ptGpsData, ptGpsPdu->uLatitude, &ucLatFlg);
//  空口抓到的EmbData->uErrCode=0x7, EmbData->uEW=0, EmbData->uFID=0, EmbData->uPF=0, EmbData->uREV=0, EmbData->uREV1=0
    ptGpsPdu->uFLCO = 0x08;     //:6;
    ptGpsPdu->uREV  = 0x00;     //:1;
    ptGpsPdu->uPF = 0;          //:1;
    ptGpsPdu->uFID = 0;
    ptGpsPdu->uErrCode = 0x7;   //:3;
    ptGpsPdu->uEW = ucLonFlg;
    ptGpsPdu->uREV1 = 0;        //:4;
}


/**
 * @brief   获取当前内嵌LC
 *
 * @param [in]
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
int GetEmbLcType(UINT8 ucGpsGetFlg, T_VOICE_FLC *ptVoiceFlc)
{
    GPS_NAS_DATA_T tGpsNas;

    ptVoiceFlc->ucFlcType = EMB_HDR;
    /*
    **  ucGpsGetFlg 是否为获取GPS内嵌, GpsSwitch 是否打开， GpsFlag是否锁定
    */
    if ((ucGpsGetFlg == PLAN_GPS) && (g_DllGlobalCfg.ucFlcGpsSwitch == FLC_GPS_ON) && ((p_DllFpgaShm->GpsFlag & 0x01) == 1) )
    //if ((ucGpsGetFlg == PLAN_GPS) && (g_DllGlobalCfg.ucFlcGpsSwitch == FLC_GPS_ON)  )
    {
        if (g_DllGlobalCfg.ucFlcGpsSndFlg == SND_FLC_MULTI)  // 周期发GPS内嵌
        {
            if (GetTimerCnt(FLC_GPS_ODP) == 0)    // 获取当前定时时间
            {
                SetTimerOn(FLC_GPS_ODP, g_DllGlobalCfg.ulFlcGpsCnt, SetTimerOff);  // 启动定时器
                ptVoiceFlc->ucFlcType = EMB_GPS;
            }
        }
        else if (g_DllGlobalCfg.ucFlcGpsSndFlg == SND_FLC_ONCE)  // 单次发GPS内嵌
        {
            if (g_ucSndFlcOnceState == SND_FLC_START)
            {
                g_ucSndFlcOnceState = SND_FLC_OVER;
                ptVoiceFlc->ucFlcType = EMB_GPS;
            }
        }
    }
    //LOG_WARNING(s_LogMsgId,"[DLL][%s][FRAME_A][ucGpsGetFlg=%d][ptVoiceFlc->ucFlcType=%d] ", _F_,ucGpsGetFlg,ptVoiceFlc->ucFlcType);

    /*
    **  选中当前所需内嵌
    */
    if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_ENHA)//HYT高级加密
    {
        if (ptVoiceFlc->ucFlcType == EMB_HDR)//发送话权LC/PI
        {
            if(Down_PI_FLAG==1)
            {
                memcpy(ptVoiceFlc->aucFLC, g_DllGlobalCfg.auDownloadPI, FLC_LEN);
                s_plan_gps_Flag=PLAN_GPS;

                //LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_ENHA][PI] ", _F_);
                //LOG_PrintM(s_LogMsgId,ptVoiceFlc->aucFLC,9);
            }
            else
            {
                memcpy(ptVoiceFlc->aucFLC, g_DllGlobalCfg.auDownloadLC, FLC_LEN);
                s_plan_gps_Flag=PLAN_HDR;

                //LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_ENHA][LC] ", _F_);
                //LOG_PrintM(s_LogMsgId,ptVoiceFlc->aucFLC,9);
            }
        }
        else//发送GPS
        {
            IDP_GpsDevToNas((GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData), &tGpsNas);    // GpsData ==> tGpsNas
            GPS_GenFlcEmb(&tGpsNas, (EMBBED_GPS_PDU *)(ptVoiceFlc->aucFLC));  // tGpsNas ==> aucFLC

            //LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_ENHA][GPS] ", _F_);
            //LOG_PrintM(s_LogMsgId,ptVoiceFlc->aucFLC,9);
            Down_PI_FLAG = Down_PI_FLAG ^ 0x01;
            s_plan_gps_Flag=PLAN_HDR;
        }
    }
    else
    {
        if (ptVoiceFlc->ucFlcType == EMB_HDR)//发送话权LC
        {
            memcpy(ptVoiceFlc->aucFLC, g_DllGlobalCfg.auDownloadLC, FLC_LEN);

            //LOG_WARNING(s_LogMsgId,"[DLL][%s][LC] ", _F_);
            //LOG_PrintM(s_LogMsgId,ptVoiceFlc->aucFLC,9);
        }
        else//发送GPS
        {
            IDP_GpsDevToNas((GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData), &tGpsNas);    // GpsData ==> tGpsNas
            GPS_GenFlcEmb(&tGpsNas, (EMBBED_GPS_PDU *)(ptVoiceFlc->aucFLC));  // tGpsNas ==> aucFLC

            //LOG_WARNING(s_LogMsgId,"[DLL][%s][GPS] ", _F_);
            //LOG_PrintM(s_LogMsgId,ptVoiceFlc->aucFLC,9);
        }
        s_plan_gps_Flag^=0x01;
    }
    return  ptVoiceFlc->ucFlcType;
}

/**
 * @brief   呼叫控制层下行语音处理
 *
 * @param [in]  pvCclData       CCL下行数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void CCLVoiceProcess(CCL_DLL_DL_T *pvCclData)
{
    UINT8 Frq = (tDllPrint->FrqSlt >> 4) & 0x0f;
    //UINT8 uGps_Info[9]={0};
    UINT16 uF_Emb=0;
    UINT16 uF_Emb_Out[2]={0};
    UINT8  CheckId[4] = {0};


    if (Frq > 1)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] voice outbound Frq: %d error", _F_, Frq);
        return;
    }

    if (pvCclData->DataLen != VOICE_LEN)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] voice datalen: %d error", _F_, pvCclData->DataLen);
        return;
    }

#if 0
    if (CALL_VOICE_D != uCallState)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] recv ccl voice, callstate: %d abandon", _F_, uCallState);
        return NO_ERR;
    }
#endif

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));

    if(g_DllGlobalCfg.auPI==1)
    {
        DLL_ENCLIB_VOICE Encryp_str;
        memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
        memcpy(Encryp_str.Payload,pvCclData->PayLoad,pvCclData->DataLen);
        Encryp_str.FrameNum = pvCclData->FrmType;
        if(pvCclData->SilenceFrame==1)
        {
            Encryp_str.ValidFlag = VALID;//空口无法标识此帧是静音帧，静音帧也需要按照加解密处理
        }
        else
        {
            Encryp_str.ValidFlag = VALID;
        }
        EncryptFunc_Voice(&Encryp_str);

        //LOG_WARNING(s_LogMsgId,"[DLL][%s] Frame_type=%d, Valid_Flag=%d,Payload:", _F_,pvCclData->FrmType,Encryp_str.ValidFlag);
        //LOG_PrintM(s_LogMsgId,Encryp_str.Payload,27);

        memcpy(pvCclData->PayLoad,Encryp_str.Payload,pvCclData->DataLen);

        if((pvCclData->FrmType==FT_VOICE_A)&&(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_ENHA))//高级加密，需要内嵌PI,且为A帧返回PI
        {
            memcpy(g_DllGlobalCfg.auDownloadPI,Encryp_str.PIInfo,PI_LEN);

            //LOG_WARNING(s_LogMsgId,"[DLL][%s] Frame_type=%d, Valid_Flag=%d,EMB_PI_Payload:", _F_,pvCclData->FrmType,Encryp_str.ValidFlag);
            //LOG_PrintM(s_LogMsgId,Encryp_str.PIInfo,PI_LEN);
        }
    }

    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_VOICE,pvCclData->FrmType,pvCclData->DataType,VOICE_LEN,pvCclData->PayLoad);

    /*上行丢超帧测试*/
#if 0
    LOG_WARNING(s_LogMsgId,"[DLL][%s] Down_Voice_Cnt=%lu,FrmType=%d", _F_,Down_Voice_Cnt,pvCclData->FrmType);

    if((Down_Voice_Cnt/6==4)||(Down_Voice_Cnt/6==5))
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Down_Voice_Cnt=%lu,FrmType=%d,Throw", _F_,Down_Voice_Cnt,pvCclData->FrmType);
        if(pvCclData->FrmType==FT_VOICE_F)
        {
            Down_PI_FLAG = Down_PI_FLAG ^ 0x01;
        }

        Down_Voice_Cnt++;
        return;
    }
    Down_Voice_Cnt++;
#endif
    /*上行丢超帧测试完毕*/

    switch (pvCclData->FrmType)
    {
        case FT_VOICE_A:
        {
            T_VOICE_FLC tVoiceFLC;
            GetEmbLcType(s_plan_gps_Flag, &tVoiceFLC);//获取语音内嵌类型及9字节数据
            EmbeddedEnc(tVoiceFLC.aucFLC, g_DllGlobalCfg.auEmbInfo);  // aucFLC ==> auEmbInfo
            memcpy(p_DllFpgaShm->EmbInfo, g_DllGlobalCfg.auEmbInfo, 16);
            ODP_SendInfData(ptInfData, S_VOICE_A);
            break;
        }
        case FT_VOICE_B:
        {
            ptInfData->tDataLink[Frq].LCSS = 1;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[0], 4);
            ODP_SendInfData(ptInfData, S_VOICE_B);
            break;
        }
        case FT_VOICE_C:
        {
            ptInfData->tDataLink[Frq].LCSS = 3;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[4], 4);
            ODP_SendInfData(ptInfData, S_VOICE_C);
            break;
        }
        case FT_VOICE_D:
        {
            ptInfData->tDataLink[Frq].LCSS = 3;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[8], 4);
            ODP_SendInfData(ptInfData, S_VOICE_D);
            break;
        }
        case FT_VOICE_E:
        {
            ptInfData->tDataLink[Frq].LCSS = 2;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[12], 4);
            ODP_SendInfData(ptInfData, S_VOICE_E);
            break;
        }
        case FT_VOICE_F:
        {
            ptInfData->tDataLink[Frq].LCSS = 0;

            if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA)
            {
                //编32bit内嵌
                uF_Emb=((((UINT16)Down_Feature_ID)<<3)& 0x07f8)|(((UINT16)Down_Alg_ID)& 0x0007);
                rc_enc(&uF_Emb,uF_Emb_Out);
                //LOG_WARNING(s_LogMsgId,"[DLL][%s] Frame_type=%d, uF_Emb=%x,uF_Emb_Out:", _F_,pvCclData->FrmType,uF_Emb);
                //LOG_PrintM(s_LogMsgId,uF_Emb_Out,4);
                memcpy(ptInfData->tDataLink[Frq].EmbInfo, uF_Emb_Out, 4);
            }
            else
            {
                memcpy(ptInfData->tDataLink[Frq].EmbInfo, CheckId, 4);//内嵌填0
            }
            ODP_SendInfData(ptInfData, S_VOICE_F);
            Down_PI_FLAG = Down_PI_FLAG ^ 0x01;
            break;
        }
        default:
        {
            break;
        }

    }

}


/**
 * @brief   呼叫控制层下行数据处理
 *
 * @param [in]  pvCclData       CCL下行数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void CCLDataProcess(CCL_DLL_DL_T * pvCclData)
{

    switch (pvCclData->DataType)
    {
        case CT_PI_HEADER:
        {
            break;
        }
        case CT_LC_HEADER:
        {
            g_BurstCnt = 0;
            Down_PI_FLAG=0;
            s_plan_gps_Flag=0;
            ODP_LcHeaderFun(pvCclData);
            //Down_Voice_Cnt=0;
            break;
        }
        case CT_LC_TERMINATOR:
        {
            g_BurstCnt = 0;
            ODP_LcTerminatorFun(pvCclData);
            memset(g_DllGlobalCfg.auDownloadLC, 0xff, (FLC_LEN+3));
            break;
        }
        case CT_PACKET_DATA:
        {
            ODP_R12UPacketDataFun(pvCclData);
            break;
        }
        case CT_GPS_REPORT_REQ_MS:
        {
            ODP_MsGpsRequestFun(pvCclData);
            break;
        }
        case CT_DISABLE_REQ_MS:
        {
            ODP_MsDisableFun(pvCclData);
            break;
        }
        case CT_ENABLE_REQ_MS:
        {
            ODP_MsEnableFun(pvCclData);
            break;
        }

        case CT_ALARM_REQ_MS: // MS和NAS
        {
            ODP_MsNasAlarmReqFun(pvCclData);
            break;
        }

        case CT_ALARM_ACK_MS: // 没有这项
        {
            ODP_MsAlarmAckFun(pvCclData);
            break;
        }

        default:
        {
            break;
        }
    }
}


/**
 * @brief   呼叫控制层下行邻点处理
 *
 * @param [in]  pvCclData       CCL下行数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void CCLNearDataProcess(CCL_DLL_DL_T * pvCclData)
{
}


/**
 * @brief   呼叫控制层下行链路数据处理
 *
 * @param [in]  pvCclData       CCL下行数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void CCLNasDataProcess(CCL_DLL_DL_T * pvCclData)
{
    switch (pvCclData->DataType)
    {
        case CT_GPS_REPORT_REQ_NAS:
        {
            ODP_NasGpsReportFun(pvCclData);
            break;
        }
        case CT_STUN_REQ_NAS:
        {
            ODP_NasStunFun(pvCclData);
            break;
        }
        case CT_KILL_REQ_NAS:
        {
            ODP_NasKillFun(pvCclData);
            break;
        }
        case CT_ENABLE_REQ_NAS:
        {
            ODP_NasEnableFun(pvCclData);
            break;
        }
        case CT_NEGHR_QUERY:  // 邻点查询
        {
            ODP_NegrQueryFun(pvCclData);
            break;
        }
        default:
        {
            break;
        }
    }
}


/**
 * @brief   呼叫控制层下行线程
 *
 * @param [in] p       传递给线程start_routine的参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void *CCLDownloadTask(void * p)
{
    int RetSize;
    int RecvBuf[CCL_RECV_MAX];
    int ret;
    CCL_DLL_DL_T *pvCclData;
    uNS = 0;

    ptInfData = new NAS_INF_DL_T;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));

    fd_set fdsr;
    struct timeval tv;

    while(1)
    {
        FD_ZERO(&fdsr);
        FD_SET(CCLSocket, &fdsr);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select((CCLSocket+1), &fdsr, NULL, NULL, &tv);

        if (ret < 0)
        {
            LOG_ERROR(s_LogMsgId,"[DLL][%s] select error", _F_);
            continue;
        }
        else if (ret == 0)
        {
            continue;
        }

        if (FD_ISSET(CCLSocket, &fdsr))
        {
            RetSize = recvfrom(CCLSocket, RecvBuf, CCL_RECV_MAX, 0, (struct sockaddr *)(&RecvAddr), &AddrSize);
            if (RetSize < 0)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] recvfrom <0 fail", _F_);
                continue;
            }

            pvCclData = (CCL_DLL_DL_T *)RecvBuf;
            if (tDllPrint->CCDown == 1)
            {
                ODP_CclPrintf(pvCclData);            }

            if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)//处于遥晕或遥毙状态
            {
                if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_ENABLE_REQ_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                    //只有激活信令可以处理
                }
                else if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_STUN_RPT_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                    //无此下行分支
                }
                else if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_KILL_RPT_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                    //无此下行分支
                }
                else
                {
                    LOG_WARNING(s_LogMsgId, "[DLL][%s] Nas is Stuned or Killed", _F_);
                    continue;
                }
            }

            if((CCL_HEAD_LEN + pvCclData->DataLen) != RetSize)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] Recvfrom size Error(Resize:%d)", _F_, RetSize);
                continue;
            }

            if (CALLI_SPON == p_DllFpgaShm->CallingST || SERVR_IDLE == p_DllFpgaShm->CallingST)
            {
                //FPGA空闲
            }
            else
            {
                if ((pvCclData->MsgType == DI_MSG_WLU)  && (pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId))
                {
                    // 当FPGA处于转发状态时, 本节点的摇晕摇毙激活GPS上拉邻点查询链路机命令可以正常处理
                    //因为这些操作不经过FPGA，DLL层直接返回ACK
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] Outbound sap busy(FPGA_CallST:%d) abanbon",_F_,p_DllFpgaShm->CallingST);
                    continue;
                }
            }

            switch (pvCclData->MsgType)
            {
                case DI_MSG_VOICE:
                {
                    g_BurstCnt = 0;
                    CCLVoiceProcess(pvCclData);
                    break;
                }
                case DI_MSG_DATA:
                {
                    CCLDataProcess(pvCclData);
                    break;
                }
                case DI_MSG_NEAR:
                {
                    CCLNearDataProcess(pvCclData);
                    break;
                }
                case DI_MSG_WLU:
                {
                    CCLNasDataProcess(pvCclData);
                    break;
                }
                default:
                {
                    LOG_ERROR(s_LogMsgId,"[DLL][%s] MsgType error", _F_);
                }
            }
        }
    }

    delete(ptInfData);
    pthread_exit(NULL);
}
