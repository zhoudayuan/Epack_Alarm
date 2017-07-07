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



/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
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


/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
/**
 * @brief  下行向接口适配层发送数据处理
 *
 * @param [in] SAPType      业务数据类型
 * @param [in] SapStr         业务数据打印
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
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



/**
 * @brief  主呼下行向接口层发送数据处理
 *
 * @param [in] MsgT           业务数据类型
 * @param [in] FrmT           业务数据打印
 * @param [in] DataT          业务数据打印
 * @param [in] Len              业务数据打印
 * @param [in] PayLoad      业务数据打印
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

    ptInfData->tDataLink[Frq].PI = g_DllGlobalCfg.auPI;
    ptInfData->tDataLink[Frq].LCSS = 0;
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

    if (tDllPrint->PrintLv == PRINT_DEBUG|| tDllPrint->PrintLv == PRINT_ALL)
    {
        SapDataStr(SAPType, SapStr);
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] TimS:%ld F%dS%d MsgT_%d FrmT_%d CC_%d DataT_%d Len_%d %s",
            _F_,
            pdInfData->TimeStamp,
            (Frq+1),
            (pdInfData->SlotNum+1),
            pdInfData->tDataLink[Frq].MsgType,
            pdInfData->tDataLink[Frq].FrmType,
            pdInfData->tDataLink[Frq].CC,
            pdInfData->tDataLink[Frq].DataType,
            pdInfData->tDataLink[Frq].DataLen,
            SapStr);
    }

    if ((tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL) && pdInfData->tDataLink[Frq].DataLen < 35)
    {
        LOG_PrintM(s_LogMsgId, pdInfData->tDataLink[Frq].PayLoad, pdInfData->tDataLink[Frq].DataLen);
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

    MsGpsReq->IpCmpsHdr.IPv4Idt = htons(0x022a);
    MsGpsReq->IpCmpsHdr.SAID = 1;
    MsGpsReq->IpCmpsHdr.DAID = 0;
    MsGpsReq->IpCmpsHdr.MHCO = 0;
    MsGpsReq->IpCmpsHdr.SPID = 116;
    MsGpsReq->IpCmpsHdr.LHCO = 0;
    MsGpsReq->IpCmpsHdr.DPID = 116;
    
    MsGpsReq->MsgHdr = 0x08;  // 0x11-RRS, 0x08-LP:Location Protocol 
    /*
    ** 0xA0-Standard Location Immediate Service, 0x01-(Request), RequestID[4]+RadioIP[4]
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
    printf("RequestID[0-3]={%#x:%#x:%#x:%#x}\n", RequestID[0],RequestID[1], RequestID[2],RequestID[3]);
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
    printf("RadioIP[0-3]={%d:%d:%d:%d-(%#x:%#x:%#x:%#x)}\n", RadioIP[0],RadioIP[1], RadioIP[2],RadioIP[3], RadioIP[0],RadioIP[1], RadioIP[2],RadioIP[3]);

    Sum = (UINT8 *)&MsGpsReq->OpCode;
    for (UINT8 i = 0; i < 12; i++)
    {
        MsGpsReq->CheckSum += *Sum;
        Sum++;
    }
    MsGpsReq->CheckSum = ~ (MsGpsReq->CheckSum) + 0x33;
    MsGpsReq->MsgEnd = 0x03;
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
void ODP_TerminDataLC(UINT8 *DstId, UINT8 *SrcId, UINT8 GI, UINT8 ResFrq, UINT8 ResSlt)
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
    ptInfData->ResFrqSlot = ResFrq<<4 | ResSlt;

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
    UINT16 auULC[6] = {0};

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

    //ODP_GenNasLcHeader();
    //ODP_GenNasLcHeader();

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
    EmbeddedEnc(g_DllGlobalCfg.auDownloadLC, g_DllGlobalCfg.auEmbInfo);
    memcpy(p_DllFpgaShm->EmbInfo, g_DllGlobalCfg.auEmbInfo, 16);

    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO, DT_LC_HEADER, (FLC_LEN+3), g_DllGlobalCfg.auDownloadLC);

    ODP_SendInfData(ptInfData, S_LC_HDR);

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

    cmp = strncmp((char *)g_DllGlobalCfg.auDownloadLC, (char *)pvCclData->PayLoad, FLC_LEN);
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
        LOG_DEBUG(s_LogMsgId, "[DLL][%s] cmd_code_%#x, op_code_%#x, Seq_[%d], dst_id_%d, src_id_%d", _F_, 
            NasAiData.cmd_code, NasAiData.op_code,
            NasAiData.data[0], NasAiData.dst_id, NasAiData.src_id );
    }


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
    tPreCsbk.uDC = 1;
    tPreCsbk.uGI = GI;
    tPreCsbk.uREV = 0;
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
 */
int ODP_R34CPacketDataFun(UINT8 DataLen, PDP_HEAD_DU *puDPHdr)
{
    UINT8 Poc, Bf, Dei, BlkNum;

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
            Bf = BlkNum+1;
            Dei = 1;

            //设置特征图样
            memset(g_PdpDBuf.auData, 0xff, (Bf*R34_C_DATA_DLEN));
            *(g_PdpDBuf.auData+DataLen) = 0x7f;
        }
        else if (DataLen % 16 == 12)    //==12
        {
            BlkNum = DataLen/16;
            Bf = BlkNum+1;
            Dei = 3;

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
        puDPHdr->p_Pdp_CHdr.uDPF = 3;  // 数据分组格式: 0-UDT数据/1-相应分/2-无确认的数据分组/3-有确认的数据分组/....
        puDPHdr->p_Pdp_CHdr.uRV1 = 0;
        puDPHdr->p_Pdp_CHdr.uHC = 0;  // 压缩头标识， 0-使用完整TCP/IP或UDP/IP头, 1-使用压缩TCP/IP或UDP/IP头
        puDPHdr->p_Pdp_CHdr.uA = 1;  //  相应请求 0-不需求应当相应，1-需求应答相应
        puDPHdr->p_Pdp_CHdr.uGI = 0;  // 0-目的为单呼地址，1-目的为组呼地址
        puDPHdr->p_Pdp_CHdr.uDEI = Dei; 
        puDPHdr->p_Pdp_CHdr.uRV2 = 0;
        puDPHdr->p_Pdp_CHdr.uSAP = 3;
        puDPHdr->p_Pdp_CHdr.uBF = Bf;
        puDPHdr->p_Pdp_CHdr.uFMF = 1;  // 消息完整标识 FMF,  0-后续重传，1-第一次发送
        puDPHdr->p_Pdp_CHdr.uFSN = 8;
        puDPHdr->p_Pdp_CHdr.uNS = uNS%8;
        puDPHdr->p_Pdp_CHdr.uS = 0;

        g_PdpDBuf.uDPF         = puDPHdr->p_Pdp_CHdr.uDPF;
        g_PdpDBuf.uDEI         = Dei;
        g_PdpDBuf.uBLOCKNum    = puDPHdr->p_Pdp_CHdr.uBF;
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

        puDPHdr->d_Pdp_CHdr.uDPF = 3;
        puDPHdr->d_Pdp_CHdr.uPOCH = Poc&0x10;
        puDPHdr->d_Pdp_CHdr.uHC = 0;
        puDPHdr->d_Pdp_CHdr.uA = 1;
        puDPHdr->d_Pdp_CHdr.uGI = 0;
        puDPHdr->d_Pdp_CHdr.uPOCL = Poc&0x0f;
        puDPHdr->d_Pdp_CHdr.uSAP = 3;
        puDPHdr->d_Pdp_CHdr.uBF = Bf;
        puDPHdr->d_Pdp_CHdr.uF = 1;
        puDPHdr->d_Pdp_CHdr.uFSN = 8;
        puDPHdr->d_Pdp_CHdr.uNS = uNS%8;
        puDPHdr->d_Pdp_CHdr.uS = 0;

        g_PdpDBuf.uDPF         = puDPHdr->d_Pdp_CHdr.uDPF;
        g_PdpDBuf.uPOC         = Poc;
        g_PdpDBuf.uBLOCKNum    = puDPHdr->d_Pdp_CHdr.uBF;
        g_PdpDBuf.BF           = puDPHdr->d_Pdp_CHdr.uBF;

        //将分组数据初值设置为0
        memset(g_PdpDBuf.auData, 0, (Bf*R34_C_DATA_DLEN));
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

        p_PdpNcHdr.uDPF = 2;
        p_PdpNcHdr.uRV1 = 0;
        p_PdpNcHdr.uHC = 0;
        p_PdpNcHdr.uA = 0;
        p_PdpNcHdr.uGI = 1;
        p_PdpNcHdr.uDEI = Dei;
        p_PdpNcHdr.uRV2 = 0;
        p_PdpNcHdr.uSAP = 3;
        p_PdpNcHdr.uBF = Bf;
        p_PdpNcHdr.uFMF = 1;
        p_PdpNcHdr.uFSN = 0;
        p_PdpNcHdr.uRV3 = 0;

        memcpy(p_PdpNcHdr.auTADDR, pvCclData->DstId, 3);
        memcpy(p_PdpNcHdr.auSADDR, pvCclData->SrcId, 3);

        g_PdpDBuf.uDPF         = p_PdpNcHdr.uDPF;
        g_PdpDBuf.uState       = PDP_STATE_FIRST;
        g_PdpDBuf.uDataType    = DT_R_1_2_DATA;
        g_PdpDBuf.uDEI         = Dei;
        g_PdpDBuf.uHdrNum      = 1;
        g_PdpDBuf.uBLOCKNum    = p_PdpNcHdr.uBF;
        g_PdpDBuf.BF           = p_PdpNcHdr.uBF;

        memcpy(g_PdpDBuf.auHead[0], &p_PdpNcHdr, DATA_HEADER_LEN);

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

        d_PdpNcHdr.uDPF = 2;
        d_PdpNcHdr.uPOCH = Poc&0x10;
        d_PdpNcHdr.uHC = 0;
        d_PdpNcHdr.uA = 0;
        d_PdpNcHdr.uGI = 1;
        d_PdpNcHdr.uPOCL = Poc&0x0f;
        d_PdpNcHdr.uSAP = 3;
        d_PdpNcHdr.uBF = Bf;
        d_PdpNcHdr.uF = 1;
        d_PdpNcHdr.uFSN = 0;
        d_PdpNcHdr.uRV1 = 0;

        memcpy(d_PdpNcHdr.auTADDR, pvCclData->DstId, 3);
        memcpy(d_PdpNcHdr.auSADDR, pvCclData->SrcId, 3);

        g_PdpDBuf.uDPF         = d_PdpNcHdr.uDPF;
        g_PdpDBuf.uState       = PDP_STATE_FIRST;
        g_PdpDBuf.uDataType    = DT_R_1_2_DATA;
        g_PdpDBuf.uPOC         = Poc;
        g_PdpDBuf.uHdrNum      = 1;
        g_PdpDBuf.uBLOCKNum    = d_PdpNcHdr.uBF;
        g_PdpDBuf.BF           = d_PdpNcHdr.uBF;

        memcpy(g_PdpDBuf.auHead[0], &d_PdpNcHdr, DATA_HEADER_LEN);

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
    memcpy(g_PdpDBuf.auData, pvCclData->PayLoad, pvCclData->DataLen);

    u4CRC = ALG_Crc32((UINT8*)pDPData, R12_NC_DATA_DLEN*Bf - 4);
    // 填充CRC32
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-4] = (UINT8)(u4CRC & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-3] = (UINT8)((u4CRC >>  8) & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-2] = (UINT8)((u4CRC >> 16) & 0xff);
    pDPData->tPdpR12NcBlk.auData[Bf-1][R12_NC_DATA_DLEN-1] = (UINT8)(u4CRC >> 24);

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
    PDP_DATA_DU *puData;
    PDP_DATA_BLOCK_DU* pDPData = (PDP_DATA_BLOCK_DU *)g_PdpDBuf.auData;

    if (pvCclData->DataLen > 8)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] data length out of GPS Req", _F_);
        return ERR;
    }

    memset(&g_PdpDBuf, 0, sizeof(PDP_DT));
    ODP_GenGpsReqData(&pvCclData->PayLoad[0], &pvCclData->PayLoad[4], GpsReq, &GpsLen);
    ODP_R34CPacketDataFun(GpsLen, &puHdr);

    if (tDllPrint->AIDown == 1)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] Ms GpsReq: ", _F_);
        LOG_PrintM(s_LogMsgId, GpsReq, GpsLen);
    }

    memcpy(puHdr.p_Pdp_CHdr.auTADDR, pvCclData->DstId, 3);
    memcpy(puHdr.p_Pdp_CHdr.auSADDR, pvCclData->SrcId, 3);

    memcpy(g_PdpDBuf.uDstId, pvCclData->DstId, 3);
    memcpy(g_PdpDBuf.uSrcId, pvCclData->SrcId, 3);

    Bf = g_PdpDBuf.BF;  // blocks to follow 后续的帧数量
    printf("[%s:%d] g_PdpDBuf.BF=%d\n", __FUNCTION__, __LINE__, g_PdpDBuf.BF);
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
    memcpy(g_PdpDBuf.auData, GpsReq, GpsLen);

    u4CRC = ALG_Crc32((UINT8 *)pDPData, ((R34_NC_DATA_DLEN-2)*Bf - 4));
    // 填充CRC32
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-4] = (UINT8)(u4CRC & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-3] = (UINT8)((u4CRC >> 8) & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-2] = (UINT8)((u4CRC >> 16) & 0xff);
    pDPData->tPdpR34CBlk.auData[Bf-1][R34_C_DATA_DLEN-2-1] = (UINT8)(u4CRC >> 24);

    //发送多个预载波
    ODP_GenPreCSBKFun((Bf+6), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+5), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+4), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+3), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+2), pvCclData->DstId, pvCclData->SrcId, 0);
    ODP_GenPreCSBKFun((Bf+1), pvCclData->DstId, pvCclData->SrcId, 0);

    //下行DataHeader组包
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA, FT_VOICE_NO ,DT_DATA_HEADER, (DATA_HEADER_LEN+2), g_PdpDBuf.auHead[0]);

    ODP_SendInfData(ptInfData, S_C_HDR);

    ptInfData->tDataLink[frq].DataType = DT_R_3_4_DATA;
    ptInfData->tDataLink[frq].DataLen = 33;
    puData = (PDP_DATA_DU *)r34Data;

    for (i = 0; i < Bf; i++)
    {
        memcpy(puData->uR34Data.tR34CData.auData, pDPData->tPdpR34CBlk.auData[i], (R34_NC_DATA_DLEN-2));

        // 填充DBSN
        puData->uR34Data.tR34CData.uDBSN = g_PdpDBuf.s_uDBSNIndex;

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

        ODP_144to196bptc(r34Data, ptInfData->tDataLink[frq].PayLoad);

        ODP_SendInfData(ptInfData, S_DATA_3_4);

        g_PdpDBuf.s_uDBSNIndex++;
    }

    ODP_TerminDataLC(pvCclData->DstId, pvCclData->SrcId, 1, 0xff, 0xff);        //TD_LC 预留0xff

    for (i = 0; i < 5; i++)
    {
        if (1 == p_DllFpgaShm->FollowEn)
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

    tSupsCsbk.uLB = 1;
    tSupsCsbk.uPF = 0;
    tSupsCsbk.uCSBKO = REQ_CSBKO;
    tSupsCsbk.uFID = HFID;
    tSupsCsbk.uSSO = DIS_RADIO_SSO;
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
    ptInfData->ResFrqSlot = tDllPrint->FrqSlt+0x11;         //预留时隙频点和指定频点差1
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_CSBK,(CSBK_LEN+2),(UINT8 *)&tSupsCsbk);
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
    tSupsCsbk.uCSBKO = REQ_CSBKO;
    tSupsCsbk.uFID = HFID;
    tSupsCsbk.uSSO = EN_RADIO_SSO;
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
    tSupsCsbk.uCSBKO = ALARM_CSBKO;
    tSupsCsbk.uFID = HFID;
    tSupsCsbk.uSSO = (SeqSSO++) % DIGITAL_ALARM_SSO; // 从目前的空口包分析，该值是一个小于0x0f的序列。
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
//    ptInfData->ResFrqSlot = tDllPrint->FrqSlt+0x11;   //预留时隙频点和指定频点差1
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
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_DATA,FT_VOICE_NO,DT_CSBK,(CSBK_LEN+2),(UINT8 *)&tSupsCsbk);

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
    NAS_AI_PAYLOAD NasAiData;
    GPS_NAS_DATA_T GpsNasData;
    GPS_DEV_DATA_T *GpsDevData;
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
        IDP_GpsDevToNas(GpsDevData, &GpsNasData);

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
        NasAiData.cmd_code = CMD_CODE_GPS_REPORT;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_GET;
//        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.src_id = pvCclData->SrcId[0];       //test-zhou
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
        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
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
        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
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
        ODP_PrintNasCmdOpLog(&NasAiData);
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
        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_KILL_REQ);
    }

    return NO_ERR;
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
int ODP_NasNeighborFun(CCL_DLL_DL_T * pvCclData)
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

    if (g_DllGlobalCfg.auNodeId == pvCclData->DstId[0])
    {
        ptCclData->MsgType = DI_MSG_WLU;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_NEGHR_QUERY_ACK;

        memcpy(ptCclData->SrcId, pvCclData->SrcId, 3);
        memcpy(ptCclData->DstId, pvCclData->DstId, 3);
        ptCclData->DataLen = NER_LEN+1;
        ptCclData->PayLoad[0] = g_DllGlobalCfg.auNodeId;
        memcpy(&ptCclData->PayLoad[1], (UINT8 *)&g_DllGlobalCfg.auNegrId2, NER_LEN);

        IDP_SendCclData(ptCclData);

    }
    else
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
        ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_NEGR_REQ);
    }

    return NO_ERR;
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
    CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_VOICE,pvCclData->FrmType,pvCclData->DataType,VOICE_LEN,pvCclData->PayLoad);

    switch (pvCclData->FrmType)
    {
        case FT_VOICE_A:
        {
            ODP_SendInfData(ptInfData, S_VOICE);
            break;
        }
        case FT_VOICE_B:
        {
            ptInfData->tDataLink[Frq].LCSS = 1;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[0], 4);
            ODP_SendInfData(ptInfData, S_VOICE);
            break;
        }
        case FT_VOICE_C:
        {
            ptInfData->tDataLink[Frq].LCSS = 3;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[4], 4);
            ODP_SendInfData(ptInfData, S_VOICE);
            break;
        }
        case FT_VOICE_D:
        {
            ptInfData->tDataLink[Frq].LCSS = 3;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[8], 4);
            ODP_SendInfData(ptInfData, S_VOICE);
            break;
        }
        case FT_VOICE_E:
        {
            ptInfData->tDataLink[Frq].LCSS = 2;
            memcpy(ptInfData->tDataLink[Frq].EmbInfo, &g_DllGlobalCfg.auEmbInfo[12], 4);
            ODP_SendInfData(ptInfData, S_VOICE);
            break;
        }
        case FT_VOICE_F:
        {
            ptInfData->tDataLink[Frq].LCSS = 0;
            ODP_SendInfData(ptInfData, S_VOICE);
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
            ODP_LcHeaderFun(pvCclData);
            break;
        }
        case CT_LC_TERMINATOR:
        {
            ODP_LcTerminatorFun(pvCclData);
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

        case CT_ALARM_ACK_MS:
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
        case CT_NEGHR_QUERY:
        {
            ODP_NasNeighborFun(pvCclData);
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
                ODP_CclPrintf(pvCclData);
            }

            if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)
            {
                if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_ENABLE_REQ_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                }
                else if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_STUN_RPT_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                }
                else if (pvCclData->MsgType == DI_MSG_WLU && pvCclData->DataType == CT_KILL_RPT_NAS && pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId)
                {
                }
                else
                {
                    LOG_WARNING(s_LogMsgId, "[DLL][%s] Nas is Stun", _F_);
                    continue;
                }
            }

            if((CCL_HEAD_LEN + pvCclData->DataLen) != RetSize)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] recvfrom size error(Resize:%d)", _F_, RetSize);
                continue;
            }

            if (CALLI_SPON == p_DllFpgaShm->CallingST || SERVR_IDLE == p_DllFpgaShm->CallingST)
            {
            }
            else
            {
                // 当FPGA处于转发状态时, 摇晕摇毙等链路机命令可以使能
                if ((pvCclData->MsgType == DI_MSG_WLU)  && (pvCclData->DstId[0] == g_DllGlobalCfg.auNodeId))
                {

                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] outbound sap busy(CallST:%d) abanbon",_F_,p_DllFpgaShm->CallingST);
                    continue;
                }
            }

            switch (pvCclData->MsgType)
            {
                case DI_MSG_VOICE:
                {
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



