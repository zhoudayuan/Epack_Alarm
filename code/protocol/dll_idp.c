/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    DLLidp.c
 * @brief   实现上行数据处理模块的入口和出口函数
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
#include "string.h"
#include "EncDec.h"

//---------类型定义--------------------
typedef struct _T_HdrID {
    unsigned char ucEnableFlg;  // 本次通话是否接收到Header, 如果接收到为1， 未接收到为0
    unsigned char aucCallingID[3];  // 主叫ID, 手台或链路机ID
    unsigned char aucCalledID[3];  // 被叫ID, 正确情景为组号
} T_HdrID;

T_HdrID tHdrID;
T_HdrID *ptHdrID = &tHdrID;

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
/**
 * @var  auLastLcss
 * @brief 上一次LCSS标记
 */
UINT8 auLastLcss;
/**
 * @var  auPiFlag
 * @brief 加密的迟入标记 值为1说明已上行PI,并初始过加密向量
 */
UINT8 auPiFlag=0;
/**
 * @var  auPiFlag
 * @brief 解决Hytera高级加密E帧上报错误数据问题
 */
UINT8 auLastPiFlag=0;
/**
 * @var  auGpsRetryCnt
 * @brief GPS重传次数 最多一次
 */
UINT8 auGpsRetryCnt=0;

/**
 * @var  auSndFrq[]
 * @brief 下行转发频率
 */
UINT8 uSndFrq[2];

/**
 * @var  auSndSlot[]
 * @brief 下行转发时隙
 */
UINT8 uSndSlot[2];

/**
 * @var  g_PdpUBuf
 * @brief 上行分组数据缓存
 */
PDP_UT g_PdpUBuf = {0};

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
 * @brief 下行分组数据缓存
 */
extern PDP_DT g_PdpDBuf;

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

/**
 * @var g_discon_state
 * @brief 检查当是否前断链
 */
extern UINT8 g_discon_state;

/**
 * @var g_DisconCnt
 * @brief 断链计数器
 */
extern UINT8 g_DisconCnt;

extern UINT8 g_DisconRecoverPrintFlg;
/**
 * @fun SetDisconAlarm
 * @brief 设置当前邻点锻炼告警开关状态
 */
extern void SetDisconAlarm(int AlarmSwitch);

/**
 * @funr get_alarm_discon
 * @brief 获取当前邻点锻炼告警开关状态
 */
extern int get_alarm_discon();

extern UINT32 uWaitVoiceTimes;

UINT8 dmra_voice_tmp[BUFFSIZE]={0};

/**
 * @funr Later_PI_temp
 * @brief DMRA加密模式下迟入存储的E帧PI信息
 */
DMRA_PI_PDU Later_PI_temp;
/**
 * @funr g_bCcRightFrmRcvFlg
 * @brief CC 正确性标识,
 * 容错机制,  允许出现误码，允许出现N个超帧
 * 当 g_bCcRightFrmRcvFlg = 1, 说明收到正确语音帧
 * 当 g_bCcRightFrmRcvFlg = 0, 说明尚未收到正确语音帧
 */
int g_bCcRightFrmRcvFlg = 0;
/**
 * @funr g_iCcErrFrmCnt
 * @brief CC 错误次数
 */
int g_iCcErrFrmCnt = 0;

/**
 * @funr g_iCcErrSuperFrmCnt
 * @brief CC超帧 错误次数
 */
int g_iCcErrSuperFrmCnt = 0;
/**
 * @funr CCERR_FRM_MAX
 * @brief CC 错误帧最大次数
 */
int CCERR_SUPERFRM_MAX = 3;


const char *g_apcLatStr[2] = {"NORTH", "SOUTH"};
const char *g_apcLonStr[2] = {"EAST",  "WEST"};


/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
UINT8 _PI_CheckSum(DLL_ENCLIB_VOICE* pInput)
{
    UINT16 uCheckSum = 0;

    uCheckSum = pInput->PIInfo[0] + pInput->PIInfo[1] +
                pInput->PIInfo[2] + pInput->PIInfo[3] +
                pInput->PIInfo[4] + pInput->PIInfo[5] +
                pInput->PIInfo[6] + pInput->PIInfo[7] +
                pInput->PIInfo[8];
    uCheckSum &= 0xFF;
    uCheckSum = ~uCheckSum + 1;
    return (UINT8)(uCheckSum & 0x00FF);
}
/**
 * @brief  等待FPGA跟随标记延迟处理
 *
 * @param [in] WaitMaxTime           最大等待时间(毫秒)
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void WaitFollowEnable(UINT16 WaitMaxTime)
{
    UINT16 uCnt;
    UINT16 uMax = WaitMaxTime/30;           //以30毫米为周期

    if (tDllPrint->AIUp == 1)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] wait fpga FollowEn flag......", _F_);
    }

    for (uCnt = 0; uCnt < uMax; uCnt++)
    {
        if (0 == p_DllFpgaShm->FollowEn)
        {
            break;
        }
        usleep(30000);
    }

    return;
}


/**
 * @brief  上行3/4速率数据处理
 *
 * @param [in] r34Data      3/4速率数据
 * @param [in] DataLen      数据长度
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void R34DataAnalyze(UINT8 *r34Data, UINT8 DataLen)
{
    GPS_AI_DATA_T *pGpsData = (GPS_AI_DATA_T *)r34Data;//32字节结构体
    GPS_AI_DATA_T GpsAiData;
    GPS_NAS_DATA_T GpsNasData;

    DLL_ENCLIB_DATA Decry_Data;
    memset(&Decry_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    //解密处理
    LOG_WARNING(s_LogMsgId,"[DLL][%s][Before][Admin Data:]", _F_);
    LOG_PrintM(s_LogMsgId,g_PdpUBuf.auData,(UINT8)(sizeof(GPS_AI_DATA_T)));

    if(g_DllGlobalCfg.auPI==1)// ip压缩头5字节不加密
    {
        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            if (pGpsData->CmpsHdr[0]== 0x00 && pGpsData->CmpsHdr[1]== 0x98)//下行的3/4分组数据
            {
                Decry_Data.Data=r34Data+sizeof(IPv4_COMPRESS_HEADER_T);
                Decry_Data.DataLen=sizeof(MS_GPS_REQ_T)-sizeof(IPv4_COMPRESS_HEADER_T);
            }
            else //MS上行的GPS数据
            {
                Decry_Data.Data=r34Data+sizeof(IPv4_COMPRESS_HEADER_T);
                Decry_Data.DataLen=sizeof(GPS_AI_DATA_T)-sizeof(IPv4_COMPRESS_HEADER_T);
            }
        }
        else
        {
            Decry_Data.Data=r34Data;
            Decry_Data.DataLen=sizeof(GPS_AI_DATA_T);
        }
        Decry_Data.Type= TYPE_DATA;
        int ret=DecryptFunc_Data(&Decry_Data);
        if(ret==0)
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s][After][Len=%d][Admin Data:]", _F_,Decry_Data.DataLen);
            LOG_PrintM(s_LogMsgId,g_PdpUBuf.auData,(UINT8)(sizeof(GPS_AI_DATA_T)));
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s]Data Decryption Fail", _F_);
            return ;
        }
    }

    if (pGpsData->MsgHdr == 0x08 && pGpsData->Opcode == ntohs(0xa001))//下行的3/4分组数据
    {
        g_PdpDBuf.uDPF         =  g_PdpUBuf.uDPF;
        g_PdpDBuf.uSAP         =  g_PdpUBuf.uSAP;
        g_PdpDBuf.uBLOCKNum    =  g_PdpUBuf.uBLOCKNum;
        g_PdpDBuf.uState       =  g_PdpUBuf.uState;
        g_PdpDBuf.uDEI         =  g_PdpUBuf.uDEI;
        g_PdpDBuf.uNS          =  g_PdpUBuf.uNS;
        g_PdpDBuf.uTdLcCnt     =  g_PdpUBuf.uTdLcCnt;
        g_PdpDBuf.uEHeadNum    =  g_PdpUBuf.uEHeadNum;

        memcpy(g_PdpDBuf.uSrcId, g_PdpUBuf.uSrcId, 3);
        memcpy(g_PdpDBuf.uDstId, g_PdpUBuf.uDstId, 3);
        memcpy(g_PdpDBuf.auHead[0], g_PdpUBuf.auHead[0], (DATA_HEADER_LEN + 2));

        uGpsFlag = GPS_RLY;
    }
    else if (pGpsData->MsgHdr == 0x01 && pGpsData->Opcode == ntohs(0xa002))//MS上行的GPS数据
    {
        if (DataLen != sizeof(GPS_AI_DATA_T))
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Gps data len err[%d:%d]", _F_, DataLen, sizeof(GPS_AI_DATA_T));
            return;
        }
        memcpy(&GpsAiData, g_PdpUBuf.auData, sizeof(GPS_AI_DATA_T));
        IDP_GpsAiToNas(&GpsAiData, &GpsNasData);
        if (tDllPrint->AIUp == 1)
        {
            LOG_DEBUG(s_LogMsgId,"[DLL][%s] Ms Gps %x %x %x",_F_,g_PdpUBuf.uSrcId[0],g_PdpUBuf.uSrcId[1],g_PdpUBuf.uSrcId[2]);

            LOG_DEBUG(s_LogMsgId,"[DLL][%s] Ms Gps NS %ld LAT %ld EW %ld LOG %ld", _F_,
                                                                        GpsAiData.NS,
                                                                        GpsAiData.Latitude,
                                                                        GpsAiData.EW,
                                                                        GpsAiData.Longitude);
        }
        memcpy(g_DllGlobalCfg.auMsGps, &GpsNasData, 8);
        uGpsFlag = GPS_OK;
    }
    else
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] R34Data err", _F_);
    }

}



void GpsParseLon(GPS_NAS_DATA_T *ptGpsData, UINT32 GpsEmb)
{
    double tmp;
    double dGpsEmbPrint;
    int iDegree;
    int iMinLarge;
    int iMinLittle;

    // D.D -> DDFF.FFFF => [度.度 -> 度.分分(0~59).分分分分(0~9999)]
    tmp = (GpsEmb & ~(1<<24)) /LON_RATIO;  // 去掉符号位计算
    dGpsEmbPrint = tmp;  //  打印暂存
    iDegree = (int)tmp;
    tmp = (tmp - iDegree)*60.0;
    iMinLarge = (int)tmp;
    tmp = (tmp - iMinLarge)*10000.0;
    iMinLittle = (int)tmp;

    ptGpsData->EW = ((GpsEmb & (1<<24)) == 0)? EAST: WEST;  //<1bit 经度指示 0:EAST 1:WEST
    ptGpsData->EDEG = iDegree;        //<8bit 经度     0~179
    ptGpsData->EMINmm = iMinLarge;    //<6bit 经分的整数部分 0~59
    ptGpsData->EMINF = iMinLittle;    //<14bit 经分的小数部分 0~9999
//    LOG_DEBUG(s_LogMsgId,"[DLL][%s] Lon(%lu-%f): EW=%d, EDEG=%d, EMINmm=%d, EMINF=%d", _F_, GpsEmb, dGpsEmbPrint, ptGpsData->EW, ptGpsData->EDEG, ptGpsData->EMINmm, ptGpsData->EMINF);
    printf("[DLL][%s] Lon(%lu-%f): EW=%d, EDEG=%d, EMINmm=%d, EMINF=%d\n", _F_, GpsEmb, dGpsEmbPrint, ptGpsData->EW, ptGpsData->EDEG, ptGpsData->EMINmm, ptGpsData->EMINF);
}








void GpsParseLat(GPS_NAS_DATA_T *ptGpsData, UINT32 GpsEmb)
{
    double tmp;
    double dGpsEmbPrint;
    int iDegree;
    int iMinLarge;
    int iMinLittle;

    // D.D -> DDFF.FFFF => [度.度 -> 度.分分(0~59).分分分分(0~9999)]
    tmp = (GpsEmb & ~(1<<23)) /LAT_RATIO;  // 去掉符号位计算
    dGpsEmbPrint = tmp;
    iDegree = (int)tmp;
    tmp = (tmp - iDegree)*60.0;
    iMinLarge = (int)tmp;
    tmp = (tmp - iMinLarge)*10000.0;
    iMinLittle = (int)tmp;

    ptGpsData->NS = ((GpsEmb & (1<<23)) == 0)? SOUTH: NORTH;
    ptGpsData->NDEG = iDegree;        // 度
    ptGpsData->NMINmm = iMinLarge;    // 0~59
    ptGpsData->NMINF = iMinLittle;    // 0~9999
//    LOG_DEBUG(s_LogMsgId,"[DLL][%s] Lat(%lu-%f): NS=%d, NDEG=%d, NMINmm=%d, NMINF=%d", _F_, GpsEmb, dGpsEmbPrint, ptGpsData->NS, ptGpsData->NDEG, ptGpsData->NMINmm, ptGpsData->NMINF);
    printf("[DLL][%s] Lat(%lu-%f): NS=%d, NDEG=%d, NMINmm=%d, NMINF=%d\n", _F_, GpsEmb, dGpsEmbPrint, ptGpsData->NS, ptGpsData->NDEG, ptGpsData->NMINmm, ptGpsData->NMINF);
}


/**
 * @brief  将压缩经度转成整型非压缩数据
 *
 * @param [in] ptGpsData      GPS数据指针
 * @param [in] code            压缩经度
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void LongitudeToInt(GPS_NAS_DATA_T *ptGpsData, UINT32 *code)
{
    double temp1;
    int temp2;
    int temp_long;
    char EDEG[5] = {0};
    char EMINmm[5] = {0};
    char EMINF[5] = {0};

    temp_long = *code;
    //if the value is negative,we need to make sure the new value is negative too
    if((temp_long & 0x01000000) != 0)
    {
        temp_long |= 0xFF000000;  // 问题-1  为什么要FF           ptGpsData->EW = 0;  为什么不放在这里?
    }
    //the 93206.76 is from 1/(360/(2^25))
    temp1 = temp_long / 93206.76;
    LOG_DEBUG(s_LogMsgId,"[DLL][%s] temp1=%f", _F_, temp1);
    if (temp1 >= 0)
    {
        ptGpsData->EW = 0;
    }
    else
    {
        temp1 *= -1;
        ptGpsData->EW = 1;
    }
    // snprintf 不可以吗
    temp2 = (int)(temp1);
    LOG_DEBUG(s_LogMsgId,"[DLL][%s] temp2=%d", _F_, temp2);
    EDEG[0] = temp2/100 + '0';
    EDEG[1] = (temp2%100)/10 + '0';
    EDEG[2] = temp2%10 + '0';
    temp2 = (int)((temp1 - temp2)*60*10000);  // *60*10000
    EMINmm[0] = temp2/100000 + '0';
    EMINmm[1] = (temp2%100000)/10000 + '0';
    EMINF[0] = (temp2%10000)/1000 + '0';
    EMINF[1] = (temp2%1000)/100 + '0';
    EMINF[2] = (temp2%100)/10 + '0';
    EMINF[3] = (temp2%10) + '0';

    ptGpsData->EDEG = atoi(EDEG);
    ptGpsData->EMINmm = atoi(EMINmm);
    ptGpsData->EMINF = atoi(EMINF);

    LOG_DEBUG(s_LogMsgId,"[DLL][%s] ptGpsData->EW=%d, EDEG=%d, EMINmm=%d, EMINF=%d ", _F_,
    ptGpsData->EW,  ptGpsData->EDEG , ptGpsData->EMINmm, ptGpsData->EMINF);


}


/**
 * @brief  将压缩纬度转成整型非压缩数据
 *
 * @param [in] ptGpsData      GPS数据指针
 * @param [in] code            压缩纬度
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void LatitudeToInt(GPS_NAS_DATA_T *ptGpsData, UINT32 *code)
{
    double temp1;
    int temp2;
    int temp_long;
    char NDEG[5] = {0};
    char NMINmm[5] = {0};
    char NMINF[5] = {0};

    temp_long = *code;
    //if the value is negative,we need to make sure the new value is negative too
    if((temp_long & 0x00800000) != 0)
    {
        temp_long |= 0xFF000000;
    }
    //the 46603 is from 1/(360/(2^24))
    temp1 = temp_long / 46603;
    if(temp1 >= 0)
    {
        ptGpsData->NS = 1;
    }
    else
    {
        temp1 *= -1;
        ptGpsData->EW = 0;
    }

    temp2 = (int)(temp1);
    NDEG[0] = temp2/10 + '0';
    NDEG[1] = temp2%10 + '0';
    temp2 = (int)((temp1 - temp2)*60*10000);
    NMINmm[0] = temp2/100000 + '0';
    NMINmm[1] = (temp2%100000)/10000 + '0';
    NMINF[0] = (temp2%10000)/1000 + '0';
    NMINF[1] = (temp2%1000)/100 + '0';
    NMINF[2] = (temp2%100)/10 + '0';
    NMINF[3] = (temp2%10) + '0';

    ptGpsData->EDEG = atoi(NDEG);
    ptGpsData->EMINmm = atoi(NMINmm);
    ptGpsData->EMINF = atoi(NMINF);

}


/**
 * @brief  上行链路机使能数据检索
 *
 * @param [in] pvInfData      上行接收数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int EnableNasGrep(NAS_INF_UL_T *pvInfData)
{
    UINT8 Frq;
    NAS_AI_PAYLOAD *pNasAiData;
    if (1 == pvInfData->RxFreq1)
    {
        Frq = F_1;
    }
    else
    {
        Frq = F_2;
    }

    if (pvInfData->tDataLink[Frq].MsgType == DI_MSG_WLU && pvInfData->tDataLink[Frq].CC == g_DllGlobalCfg.auWLUCC)
    {
        pNasAiData = (NAS_AI_PAYLOAD *)(pvInfData->tDataLink[Frq].PayLoad);//链路机数据业务结构

        if (pNasAiData->cmd_code == CMD_CODE_ENABLE || pNasAiData->cmd_code == CMD_CODE_STUN || pNasAiData->cmd_code == CMD_CODE_KILL)
        {
            return 0;
        }

    }

    return -1;
}


/**
 * @brief  检索结束特征图样
 *
 * @param [in] pvInfData      上行接收数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int DetectEndPattern(PDP_UT *PdpBuf, UINT8 *Len)
{
    UINT8 i, k;
    PDP_DATA_BLOCK_UU *puUPData = (PDP_DATA_BLOCK_UU *)PdpBuf->auData;
    R12_UU *puD12Data;

    if (PdpBuf->uDEI == 0)
    {
        puD12Data = (R12_UU *)puUPData->tPdpR12NcBlk.auData[PdpBuf->uBLOCKNum-2];

        for (i = 0; i < R12_NC_DATA_ULEN; i++)
        {
            if (puD12Data->tR12NcData.auData[i] == 0x7f)
            {
                if (i == (R12_NC_DATA_ULEN-1))      //特征图样检索完毕
                {
                    break;
                }
                else
                {
                    for (k = 1; k < (R12_NC_DATA_ULEN-i); k++)
                    {
                        if (puD12Data->tR12NcData.auData[i+k] != 0xff)
                        {
                            break;
                        }
                    }

                    if (k == (R12_NC_DATA_ULEN-i))      //特征图样检索完毕
                    {
                        break;
                    }
                }
            }
        }

        if (i != R12_NC_DATA_ULEN)
        {
            *Len = PdpBuf->uBLOCKNum * R12_NC_DATA_ULEN -4 -(R12_NC_DATA_ULEN-i+8);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] End parrten detect error ", _F_);
            return ERR;
        }
    }
    else if (PdpBuf->uDEI == 1)
    {
        puD12Data = (R12_UU *)puUPData->tPdpR12NcBlk.auData[PdpBuf->uBLOCKNum-1];

        for (i = 0; i < R12_NC_DATA_ULEN; i++)
        {
            if (puD12Data->tR12NcLData.auData[i] == 0x7f)
            {
                if (i == (R12_NC_DATA_ULEN-4-1))      //特征图样检索完毕
                {
                    break;
                }
                else
                {
                    for (k = 1; k < (R12_NC_DATA_ULEN-4-i); k++)
                    {
                        if (puD12Data->tR12NcData.auData[i+k] != 0xff)
                        {
                            break;
                        }
                    }

                    if (k == (R12_NC_DATA_ULEN-4-i))      //特征图样检索完毕
                    {
                        break;
                    }
                }
            }
        }

        if (i != R12_NC_DATA_ULEN)
        {
            *Len = PdpBuf->uBLOCKNum * R12_NC_DATA_ULEN -4 -(8-i);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] End parrten detect error ", _F_);
            return ERR;
        }

    }
    else if (PdpBuf->uDEI == 2)
    {
        *Len = PdpBuf->uBLOCKNum * R12_NC_DATA_ULEN -4 -8;
    }
    else if (PdpBuf->uDEI == 3)
    {
        *Len = PdpBuf->uBLOCKNum * R12_NC_DATA_ULEN -4;
    }

    return NO_ERR;
}


/**
 * @brief  生成上行控制层语音数据
 *
 * @param [in] pData          上行控制层数据结构
 * @param [in] pDataLink    上行接口层数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int GenVoiceCclData(DLL_CCL_UL_T *pData, DATA_LINK_T *pDataLink)
{
    int ret;

    memset(pData, 0, sizeof(DLL_CCL_UL_T));
    pData->MsgType = pDataLink->MsgType;
    pData->FrmType = pDataLink->FrmType;
    pData->DataType = pDataLink->DataType;
    pData->DataLen = pDataLink->DataLen;
    pData->Vari[0] = pDataLink->Vari[0];
    pData->Vari[1] = pDataLink->Vari[1];
    pData->Rssi[0] = pDataLink->Rssi[0];
    pData->Rssi[1] = pDataLink->Rssi[1];
    pData->Prohibit = pDataLink->Prohibit;
    memcpy(pData->DstId, &g_DllGlobalCfg.auUploadLC[3], 3);
    memcpy(pData->SrcId, &g_DllGlobalCfg.auUploadLC[6], 3);

    if(g_DllGlobalCfg.auPI==1)//加密
    {
        if(((auPiFlag==1)&&(g_DllGlobalCfg.auEncryption_Type != ENCRYPT_HYT_BASE))||(g_DllGlobalCfg.auEncryption_Type == ENCRYPT_HYT_BASE))
        {
            //加密结构初始化
            DLL_ENCLIB_VOICE Encryp_str;
            memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
            memcpy(Encryp_str.Payload,pDataLink->PayLoad,pDataLink->DataLen);
            Encryp_str.FrameNum = pData->FrmType;
            Encryp_str.ValidFlag = VALID;

            ret=DecryptFunc_Voice(&Encryp_str);//语音解密

            //LOG_DEBUG(s_LogMsgId,"[DLL][%s] DeEncryption Result: FrmType=%d", _F_,pData->FrmType);
            //LOG_PrintM(s_LogMsgId, Encryp_str.Payload, pDataLink->DataLen);

            if(ret==0)//解密成功
            {
                memcpy(pData->PayLoad, Encryp_str.Payload, pDataLink->DataLen);
                if((g_DllGlobalCfg.auEncryption_Type == ENCRYPT_HYT_ENHA)&&(auLastPiFlag==0)&&(pData->FrmType==FT_VOICE_E))//Hytera高级加密的第一个E帧丢弃，不上到本地中心
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] [Hytera hight Encryption][Enter Later][Throw First Frame_E]", _F_);
                    return ERR;
                }
            }
            else//解密失败
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] DeEncryption error FrmType=%d,Encryption_Type=%d", _F_,pData->FrmType,g_DllGlobalCfg.auEncryption_Type);
                return ERR;
            }
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Hight Level Encryption Vector Uninit. FrmType=%d,Encryption_Type=%d,auPiFlag=%d", _F_,pData->FrmType,g_DllGlobalCfg.auEncryption_Type,auPiFlag);
            return ERR;
        }
    }
    else
    {
        memcpy(pData->PayLoad, pDataLink->PayLoad, pDataLink->DataLen);
    }
    return NO_ERR;

}


/**
 * @brief  生成上行控制层链路机消息数据
 *
 * @param [in] pData          上行控制层数据结构
 * @param [in] pDataLink    上行接口层数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void GenNasCclData(DLL_CCL_UL_T *pData, UINT8 DT, UINT8 Dst, UINT8 Src, UINT8 Len, UINT8 *PayL)
{
    memset(pData, 0, sizeof(DLL_CCL_UL_T));
    pData->MsgType = DI_MSG_WLU;
    pData->FrmType = FT_VOICE_NO;
    pData->DataType = DT;
    pData->DataLen = Len;
    pData->DstId[0] = Dst;
    pData->SrcId[0] = Src;

    if (Len != 0)
    {
        memcpy(pData->PayLoad, PayL, Len);
    }
}


/**
 * @brief  上行向呼叫控制层发送数据处理
 *
 * @param [in] ptCclData       上行发送数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_SendCclData(DLL_CCL_UL_T * ptCclData)
{
    if (tDllPrint->CCUp == 1)
    {
        IDP_CclPrintf(ptCclData);
    }

    sendto(CCLSocket, ptCclData, sizeof(DLL_CCL_UL_T), 0, (struct sockaddr *)(&CclAddr), AddrSize);
    return;
}


/**
 * @brief  上行数据打印
 *
 * @param [in] pvInfData       上行接口层数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_AiPrintf(NAS_INF_UL_T * pvInfData)
{
    char acAiItemBuf[20];
    UINT8 Frq;
    if (1 == pvInfData->RxFreq1)
    {
        Frq = 0;
        if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
        {
            GetAiItemStr(&pvInfData->tDataLink[Frq], acAiItemBuf, sizeof(acAiItemBuf));
            LOG_DEBUG(s_LogMsgId,"[DLL][IDP] TimS:%ld F1S%d MsgT_%d FrmT_%d CC_%d DataT_%d PI_%d Vari_%d Rssi_%d StopFg_%d[%s]",
                    pvInfData->TimeStamp,
                    (pvInfData->SlotNum+1),
                    pvInfData->tDataLink[Frq].MsgType,
                    pvInfData->tDataLink[Frq].FrmType,
                    pvInfData->tDataLink[Frq].CC,
                    pvInfData->tDataLink[Frq].DataType,
                    pvInfData->tDataLink[Frq].PI,
                    (UINT16)(pvInfData->tDataLink[Frq].Vari[0] << 8) | pvInfData->tDataLink[Frq].Vari[1],
                    (UINT16)(pvInfData->tDataLink[Frq].Rssi[0] << 8) | pvInfData->tDataLink[Frq].Rssi[1],
                    pvInfData->tDataLink[Frq].Prohibit, acAiItemBuf);
        }

        if ((tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL) && pvInfData->tDataLink[Frq].DataLen < 35)
        {
            LOG_PrintM(s_LogMsgId, pvInfData->tDataLink[Frq].PayLoad, pvInfData->tDataLink[Frq].DataLen);
            LOG_PrintM(s_LogMsgId, pvInfData->tDataLink[Frq].EmbInfo,4);
        }
    }

    if (1 == pvInfData->RxFreq2)
    {
        Frq = 1;

        if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
        {
            GetAiItemStr(&pvInfData->tDataLink[Frq], acAiItemBuf, sizeof(acAiItemBuf));
            LOG_DEBUG(s_LogMsgId,"[DLL][IDP] TimS_%ld F2S%d MsgT_%d FrmT_%d CC_%d DataT_%d PI_%d Vari_%d Rssi_%d StopFg_%d[%s]",
                    pvInfData->TimeStamp,
                    (pvInfData->SlotNum+1),
                    pvInfData->tDataLink[Frq].MsgType,
                    pvInfData->tDataLink[Frq].FrmType,
                    pvInfData->tDataLink[Frq].CC,
                    pvInfData->tDataLink[Frq].DataType,
                    pvInfData->tDataLink[Frq].PI,
                    (UINT16)(pvInfData->tDataLink[Frq].Vari[0] << 8) | pvInfData->tDataLink[Frq].Vari[1],
                    (UINT16)(pvInfData->tDataLink[Frq].Rssi[0] << 8) | pvInfData->tDataLink[Frq].Rssi[1],
                    pvInfData->tDataLink[Frq].Prohibit, acAiItemBuf);
        }

        if ((tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL) && pvInfData->tDataLink[Frq].DataLen < 35)
        {
            LOG_PrintM(s_LogMsgId, pvInfData->tDataLink[Frq].PayLoad, pvInfData->tDataLink[Frq].DataLen);
            LOG_PrintM(s_LogMsgId, pvInfData->tDataLink[Frq].EmbInfo,4);
        }
    }
}


/**
 * @brief  上行数据打印
 *
 * @param [in] ptCclData       上行控制层数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_CclPrintf(DLL_CCL_UL_T * ptCclData)
{
    if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] MsgT_%d, FrmT_%d, DataT_%#x, DataLen_%d Stopfg_%d",
                _F_,
                ptCclData->MsgType,
                ptCclData->FrmType,
                ptCclData->DataType,
                ptCclData->DataLen,
                ptCclData->Prohibit);
    }
    if (tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL)
    {
        LOG_PrintM(s_LogMsgId, ptCclData->PayLoad, ptCclData->DataLen);
    }
}


/**
 * @brief 上行(196,144)BPTC 解码函数
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_196to144bptc(UINT8 *pAiData, UINT8 *pData)
{
    UINT8 i = 0;
    UINT16 u16PduData[9]={0};
    UINT16 u16AiData[18];

    memcpy(u16AiData, pAiData, 36);

    for (i = 0; i < 18; i++)
    {
        *(u16AiData+i) = (*(u16AiData+i) << 8) | (*(u16AiData+i) >> 8);
    }

    bptc_144_196_dec(u16AiData, u16PduData);

    pData[0] = (u16PduData[0] >> 8) & 0xff;
    pData[1] = u16PduData[0] & 0xff;
    pData[2] = (u16PduData[1] >> 8) & 0xff;
    pData[3] = u16PduData[1] & 0xff;
    pData[4] = (u16PduData[2] >> 8) & 0xff;
    pData[5] = u16PduData[2] & 0xff;
    pData[6] = (u16PduData[3] >> 8) & 0xff;
    pData[7] = u16PduData[3] & 0xff;
    pData[8] = (u16PduData[4] >> 8) & 0xff;
    pData[9] = u16PduData[4] & 0xff;
    pData[10] = (u16PduData[5] >> 8) & 0xff;
    pData[11] = u16PduData[5] & 0xff;
    pData[12] = (u16PduData[6] >> 8) & 0xff;
    pData[13] = u16PduData[6] & 0xff;
    pData[14] = (u16PduData[7] >> 8) & 0xff;
    pData[15] = u16PduData[7] & 0xff;
    pData[16] = (u16PduData[8] >> 8) & 0xff;
    pData[17] = u16PduData[8] & 0xff;

}


/**
 * @brief  上行转发时隙频率
 *
 * @param [in] RevSlot       上行接收时隙
 * @param [in] RevFrq        上行接收频率
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_ReptFreqSlot(UINT8 RevSlot, UINT8 RevFrq)
{
    if(F_2 == RevFrq && S_2 == RevSlot)
    {
        uSndFrq[F_2] = F_1;
        uSndSlot[F_2] = S_1;
    }
    else if(F_1 == RevFrq && S_1 == RevSlot)
    {
        uSndFrq[F_1] = F_1;
        uSndSlot[F_1] = S_2;
    }
    else if(F_1 == RevFrq && S_2 == RevSlot)
    {
        uSndFrq[F_1] = F_2;
        uSndSlot[F_1] = S_1;
    }
    else if(F_2 == RevFrq && S_1 == RevSlot)
    {
        uSndFrq[F_2] = F_2;
        uSndSlot[F_2] = S_2;
    }
}


/**
 * @brief  上行锁定时隙频率
 *
 * @param [in] RevSlot       上行接收时隙
 * @param [in] RevFrq        上行接收频率
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_LockFreqSlot(UINT8 RevSlot, UINT8 RevFrq1, UINT8 RevFrq2)
{
    UINT8 freq = 0;

    if (0 == RevFrq1 && 0 == RevFrq2)
    {
        return ERR;
    }
    else if (1 == RevFrq1 && 0 == RevFrq2)
    {
        freq = 0;
    }
    else if (0 == RevFrq1 && 1 == RevFrq2)
    {
        freq = 1;
    }
    else
    {
        return ERR;
    }

    if (uLockSlot == 0xff || uLockFreq == 0xff)
    {
        uLockSlot = RevSlot;
        uLockFreq = freq;
        return NO_ERR;
    }
    else
    {
        if (uLockSlot == RevSlot && uLockFreq == freq)
        {
            return NO_ERR;
        }
        else if (uSndFrq[uLockFreq] == freq && uSndSlot[uLockSlot] == RevSlot)
        {
            return 2;
        }
        else
        {
            return ERR;
        }
    }
}


/**
 * @brief  上行GPS空口数据转换NAS数据
 *
 * @param [in] pvAiData             上行GPS空口数据
 * @param [out] pvNasData       上行GPS NAS 数据
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_GpsAiToNas(GPS_AI_DATA_T *pvAiGps, GPS_NAS_DATA_T *pvNasGps)
{
    char NDEG[5];
    char NMINmm[5];
    char NMINF[5];
    char EDEG[5];
    char EMINmm[5];
    char EMINF[5];
    char latitude[10];
    char longitude[10];

    memset(NDEG, 0, 5);
    memset(NMINmm, 0, 5);
    memset(NMINF, 0, 5);
    memset(EDEG, 0, 5);
    memset(EMINmm, 0, 5);
    memset(EMINF, 0, 5);
    memset(latitude, 0, 10);
    memset(longitude, 0, 10);

    sprintf(latitude, "%ld", pvAiGps->Latitude);
    sprintf(longitude, "%ld", pvAiGps->Longitude);

    memcpy(NDEG, latitude, 2);
    memcpy(NMINmm, &latitude[2], 2);
    memcpy(NMINF, &latitude[4], 4);

    memcpy(EDEG, longitude, 3);
    memcpy(EMINmm, &longitude[3], 2);
    memcpy(EMINF, &longitude[5], 4);

    pvNasGps->NS = pvAiGps->NS;
    pvNasGps->EW = pvAiGps->EW;
    pvNasGps->NDEG = atoi(NDEG);
    pvNasGps->NMINmm = atoi(NMINmm);
    pvNasGps->NMINF = atoi(NMINF);
    pvNasGps->EDEG = atoi(EDEG);
    pvNasGps->EMINmm = atoi(EMINmm);
    pvNasGps->EMINF = atoi(EMINF);

    return;
}


/**
 * @brief  上行GPS 设备数据转换NAS数据
 *
 * @param [in] pvAiData             上行GPS空口数据
 * @param [out] pvNasData       上行GPS NAS 数据
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_GpsDevToNas(GPS_DEV_DATA_T *pvDevGps, GPS_NAS_DATA_T *pvNasGps)
{
    pvNasGps->NS = pvDevGps->NS & 0x01;
    pvNasGps->EW = pvDevGps->EW & 0x01;
    pvNasGps->NDEG = pvDevGps->NDEG & 0x7f;
    pvNasGps->NMINmm = pvDevGps->NMINmm & 0x3f;
    pvNasGps->NMINF = (UINT16)(pvDevGps->NMINF/10);
    pvNasGps->EDEG = pvDevGps->EDEG & 0xff;
    pvNasGps->EMINmm = pvDevGps->EMINmm & 0x3f;
    pvNasGps->EMINF = (UINT16)(pvDevGps->EMINF/10);
    return;
}


/**
 * @brief  上行TD_LC数据处理
 *
 * @param [in] pvInfData     接口层数据
 * @param [in] RevFrqNo     接收频率号
 * @param [in] pTdLcPdu      Tdlc数据
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_TD_LCFun(NAS_INF_UL_T *pvInfData, UINT8 RevFrqNo, TD_LC_PDU_T *pTdLcPdu)
{
    UINT8 FrqSlt = 0;
    UINT8 SndFrq = uSndFrq[RevFrqNo];
    PDT_RE_HEAD_PDU_DT p_ReHdrPdu;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);

    if (GPS_RLY == uGpsFlag)//下行GPS的TD_LC
    {
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        ptInfData->ResFrqSlot = 0xff;
        ptInfData->TimeStamp = pvInfData->TimeStamp%2880000 + 2;
        ptInfData->SlotNum = uSndSlot[RevFrqNo];

        if (F_1 == SndFrq)
        {
            ptInfData->TxFreq1 = 1;
        }
        else
        {
            ptInfData->TxFreq2 = 1;
        }

        memcpy(&(ptInfData->tDataLink[SndFrq]), &(pvInfData->tDataLink[RevFrqNo]), sizeof(DATA_LINK_T));

        ODP_SendInfData(ptInfData, S_TD_LC);

        uGpsFlag = GPS_REQ;

        DLL_SetTimer(CALL_DATA_D, 4000);        //定时4s

    }
    else if (GPS_OK == uGpsFlag)//上行终端的TD_LC
    {
        if (1 == pTdLcPdu->uA)  // 响应请求标识
        {
            ODP_GenReponHeader(pTdLcPdu->auSADDR, pTdLcPdu->auTADDR, &p_ReHdrPdu);

            memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
            ptInfData->TimeStamp = pvInfData->TimeStamp+2;
            ptInfData->SlotNum = pvInfData->SlotNum;
            ptInfData->TxFreq1 = pvInfData->RxFreq1;//RE HEAD 响应不交换频点时隙，不需要中转，只递增时间戳
            ptInfData->TxFreq2 = pvInfData->RxFreq2;
            ptInfData->tDataLink[RevFrqNo].MsgType = DI_MSG_DATA;
            ptInfData->tDataLink[RevFrqNo].FrmType = FT_VOICE_NO;
            ptInfData->tDataLink[RevFrqNo].CC = g_DllGlobalCfg.auMSCC;
            ptInfData->tDataLink[RevFrqNo].PI = g_DllGlobalCfg.auPI;
            ptInfData->tDataLink[RevFrqNo].DataType = DT_DATA_HEADER;
            ptInfData->tDataLink[RevFrqNo].DataLen = DATA_HEADER_LEN+2;
            memcpy(ptInfData->tDataLink[RevFrqNo].PayLoad, &p_ReHdrPdu, (DATA_HEADER_LEN+2));
            ODP_SendInfData(ptInfData, S_RE_HDR);         //send response header
            ptInfData->TimeStamp = pvInfData->TimeStamp+4;
            ODP_SendInfData(ptInfData, S_RE_HDR);         //send response header
        }

        //GPS信息上报本地中心
        memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
        ptCclData->MsgType = pDataLink->MsgType;
        ptCclData->FrmType = pDataLink->FrmType;
        ptCclData->DataType = CT_GPS_REPORT_ACK_MS;
        ptCclData->DataLen = GPS_LEN;
        ptCclData->Vari[0] = pDataLink->Vari[0];
        ptCclData->Vari[1] = pDataLink->Vari[1];
        memcpy(ptCclData->DstId, g_PdpUBuf.uDstId, 3);
        memcpy(ptCclData->SrcId, g_PdpUBuf.uSrcId, 3);
        memcpy(ptCclData->PayLoad, g_DllGlobalCfg.auMsGps, ptCclData->DataLen);
        IDP_SendCclData(ptCclData);

        //GPS信息链路中转
        NAS_AI_PAYLOAD NasAiData;
        NasAiData.cmd_code = CMO_CODE_MS_GPS;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_GET_ACK;
        NasAiData.src_id = 31;      //bit0-bit4: 1
        NasAiData.dst_id = 31;      //bit0-bit4: 1
        memcpy(NasAiData.data, g_DllGlobalCfg.auMsGps, GPS_LEN);
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

//        WaitFollowEnable(3000);
#if 0
        // 预载波
        DstId = NasAiData.dst_id;
        SrcId = NasAiData.src_id;
        ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
        ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
#endif

        if (1 == pvInfData->RxFreq1)
        {
            FrqSlt = (0x00 << 4) | (pvInfData->SlotNum & 0x0f);
        }
        else
        {
            FrqSlt = (0x01 << 4) | (pvInfData->SlotNum & 0x0f);//自己生成的ACK与上行的TD_LC频点时隙相同，时间戳增大
        }

        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData((pvInfData->TimeStamp+18), FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (CSBK_LEN+2), (UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);  // by zhoudayuan
        ODP_SendInfData(ptInfData, S_GPS_ACK);    // send Ms Gps Ack
        CallingShootData((pvInfData->TimeStamp+20), FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (CSBK_LEN+2), (UINT8 *)&NasAiData);
        ODP_SendInfData(ptInfData, S_GPS_ACK);    // send Ms Gps Ack
        CallingShootData((pvInfData->TimeStamp+22), FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (CSBK_LEN+2), (UINT8 *)&NasAiData);
        ODP_SendInfData(ptInfData, S_GPS_ACK);    // send Ms Gps Ack
        CallingShootData((pvInfData->TimeStamp+24), FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (CSBK_LEN+2), (UINT8 *)&NasAiData);
        ODP_SendInfData(ptInfData, S_GPS_ACK);    // send Ms Gps Ack
        DLL_ClearTimer();
    }
    else
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Gps data Err(GpsFlag:%x)", _F_, uGpsFlag);
    }
}


/**
 * @brief  上行LC_header处理函数
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_LcHeaderFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT16 ReStatus = NO_ERR;
    UINT16 auULC[6] = {0};
    UINT8 CheckId[3] = {0};
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    NAS_PRE_LC_PDU *t_NasPreLc = (NAS_PRE_LC_PDU *)(pDataLink->PayLoad);

    memcpy(auULC, pDataLink->PayLoad, (FLC_LEN+3));

    // 区分PDT/DMR
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_head_fulllc_dec(auULC, &ReStatus);
    }
    else
    {
        dmr_head_fulllc_dec(auULC, &ReStatus);
    }
    if (NO_ERR != ReStatus)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Header fulllc_dec error", _F_);
        return ERR;
    }

    // ID不为0都接收
    if (memcmp(t_NasPreLc->auTADDR, CheckId, 3) == 0)  // 目的地址不为0
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Header error(TADDR:0)", _F_);
        return ERR;
    }

    // FLCO_TDLC即为结束帧
    if (t_NasPreLc->uFLCO == FLCO_TDLC || t_NasPreLc->uPF == 1)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Header error(FLCO:%d,PF:%d)", _F_, t_NasPreLc->uFLCO, t_NasPreLc->uPF);
        return ERR;
    }

    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    ptCclData->MsgType  = DI_MSG_DATA;
    ptCclData->FrmType  = FT_VOICE_NO;
    ptCclData->DataType = CT_LC_HEADER;
    ptCclData->DataLen  = FLC_LEN;
    ptCclData->Vari[0] = pDataLink->Vari[0];
    ptCclData->Vari[1] = pDataLink->Vari[1];
    ptCclData->Rssi[0] = pDataLink->Rssi[0];
    ptCclData->Rssi[1] = pDataLink->Rssi[1];
    ptCclData->Prohibit = pDataLink->Prohibit;
    memcpy(ptCclData->SrcId, t_NasPreLc->auSADDR, 3);
    memcpy(ptCclData->DstId, t_NasPreLc->auTADDR, 3);
    /*
    ** GPS语音内嵌，保存本次通话ID
    */
    memcpy(ptHdrID->aucCallingID, t_NasPreLc->auSADDR, sizeof(t_NasPreLc->auSADDR));
    memcpy(ptHdrID->aucCalledID, t_NasPreLc->auTADDR, sizeof(t_NasPreLc->auTADDR));
    ptHdrID->ucEnableFlg = 1;
    memcpy(ptCclData->PayLoad, pDataLink->PayLoad, FLC_LEN);
    memcpy(g_DllGlobalCfg.auUploadLC, pDataLink->PayLoad, FLC_LEN);
    EmbeddedEnc(g_DllGlobalCfg.auUploadLC, g_DllGlobalCfg.auEmbInfo);  // auUploadLC ==> auEmbInfo
    memcpy(p_DllFpgaShm->EmbInfo, g_DllGlobalCfg.auEmbInfo, 16);
    IDP_SendCclData(ptCclData);
    return NO_ERR;
}


/**
 * @brief  上行PI_header处理函数 高级加密才进入此流程
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  whl
 * @since   trunk.00001
 * @bug
 */
int IDP_PiHeaderFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    //UINT16 ReStatus = NO_ERR;
    //UINT16 auUPI[6] = {0};
    UINT16 u2CRC = 0;
    UINT16 AirCRC = 0;

    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    PI_PDU *pSupsCsbk = (PI_PDU *)(pDataLink->PayLoad);

    u2CRC = ALG_Crc16(pDataLink->PayLoad, PI_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_PI;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_PI;
    }

    AirCRC = ((UINT16)(pSupsCsbk->dmraPiData.auCRC[0]) << 8) | pSupsCsbk->dmraPiData.auCRC[1];

    if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] ALG_ID=%d,KEYID=%d", _F_,pSupsCsbk->dmraPiData.uAlgId,pSupsCsbk->dmraPiData.uKeyID);
    }

    if (u2CRC != AirCRC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Pi_Header fulllc_dec error", _F_);
        return ERR;
    }

    memcpy(g_DllGlobalCfg.auUploadPI, pDataLink->PayLoad, PI_LEN);

    DLL_ENCLIB_VOICE Encryp_str;
    memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
    memcpy(Encryp_str.PIInfo,&(pSupsCsbk->dmraPiData),PI_LEN);
    Encryp_str.FrameNum = VOICE_PI_FRAME;
    Encryp_str.ValidFlag = VALID;
    if(auPiFlag==0)//防止多个PI重复初始化向量函数
    {
        DecryptFunc_Voice(&Encryp_str);//PI调用解码初始化向量函数
        auPiFlag=1;
        //LOG_DEBUG(s_LogMsgId,"[DLL][%s] Rcv PI_Head,Init Encryption",_F_);
    }

    DLL_SetTimer(CALL_VOICE_U,360);//最后一个PI起定时

    return NO_ERR;
}

/**
 * @brief  上行LC_Terminator处理函数
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_LcTerminatorFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    int cmp;
    UINT16 ReStatus = NO_ERR;
    UINT16 auULC[6];
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);

    memcpy(auULC, pDataLink->PayLoad, (FLC_LEN+3));
    if ((tDllPrint->PrintLv == PRINT_MEMRY) || (tDllPrint->PrintLv == PRINT_ALL))
    {
        LOG_PrintM(s_LogMsgId, pDataLink->PayLoad, 12);
    }
    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        pdt_term_fulllc_dec(auULC, &ReStatus);
    }
    else
    {
        dmr_term_fulllc_dec(auULC, &ReStatus);
    }

    if (NO_ERR != ReStatus)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Terminator fulllc_dec error", _F_);
        return ERR;
    }

    GEN_LC_PDU_T *pGenLcPdu = (GEN_LC_PDU_T *)auULC;

    if (FLCO_TER == pGenLcPdu->uFLCO)  // LC_Terminiator
    {
        g_BurstCnt = 0;
        g_bCcRightFrmRcvFlg = 0;
        g_iCcErrFrmCnt = 0;
        g_iCcErrSuperFrmCnt = 0;


        //调用解密函数 清除秘钥及相关信息
        if(g_DllGlobalCfg.auPI==1)
        {
            DLL_ENCLIB_VOICE Dencryp_str;
            memset(&Dencryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
            memcpy(Dencryp_str.LCInfo,pDataLink->PayLoad,FLC_LEN);
            Dencryp_str.FrameNum = VOICE_TERM_FRAME;
            Dencryp_str.ValidFlag = VALID;
            DecryptFunc_Voice(&Dencryp_str);//LC_Ter清除相关加密信息
        }
        cmp = memcmp((char *)g_DllGlobalCfg.auUploadLC, (char *)pDataLink->PayLoad, FLC_LEN);
        if (0 != cmp)
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc_Termin data error", _F_);
            LOG_PrintM(s_LogMsgId, (char *)g_DllGlobalCfg.auUploadLC, 12);
            LOG_PrintM(s_LogMsgId, (char *)pDataLink->PayLoad, 12);
            return ERR;
        }
        memset(g_DllGlobalCfg.auUploadLC, 0xff, (FLC_LEN+3));

        p_DllFpgaShm->WorkOver = WORK_STOP;

        memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
        ptCclData->MsgType = pDataLink->MsgType;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_LC_TERMINATOR;
        ptCclData->DataLen = FLC_LEN;
        ptCclData->Vari[0] = pDataLink->Vari[0];
        ptCclData->Vari[1] = pDataLink->Vari[1];
        ptCclData->Rssi[0] = pDataLink->Rssi[0];
        ptCclData->Rssi[1] = pDataLink->Rssi[1];
        ptCclData->Prohibit = pDataLink->Prohibit;
        memcpy(ptCclData->DstId, &pDataLink->PayLoad[3], 3);
        memcpy(ptCclData->SrcId, &pDataLink->PayLoad[6], 3);
        memcpy(ptCclData->PayLoad, pDataLink->PayLoad, FLC_LEN);
        memset(ptHdrID, 0, sizeof(ptHdrID[0]));  // 清除本次通话ID, 保存ID在header处
        if (ptErrPrint->Voice == 1)
        {
            ErrRateVoiceTest(NULL, SET_ERR_TER);
        }
        IDP_SendCclData(ptCclData);
        uCallState = CALL_IDLE;
    }
    else if (FLCO_TDLC == pGenLcPdu->uFLCO)            //TD_LC (DEC:48==HEX:0X30)
    {
        TD_LC_PDU_T *pTdLcPdu = (TD_LC_PDU_T *)auULC;

        if (pTdLcPdu->auSADDR[0] == g_PdpDBuf.uSrcId[0] \
         && pTdLcPdu->auSADDR[1] == g_PdpDBuf.uSrcId[1] \
         && pTdLcPdu->auSADDR[2] == g_PdpDBuf.uSrcId[2] && GPS_REQ== uGpsFlag)//自己发出去的TD_LC?
        {
            if (tDllPrint->AIUp == 1)
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s] Td_Lc from %#x %#x %#x GpsFlag %x abandon", _F_,
                    pTdLcPdu->auSADDR[0],
                    pTdLcPdu->auSADDR[1],
                    pTdLcPdu->auSADDR[2],
                    uGpsFlag);
            }
            return NO_ERR;
        }

        IDP_TD_LCFun(pvInfData, RevFrqNo, pTdLcPdu);

    }

    return NO_ERR;
}


/**
 * @brief  上行MS 信令数据处理函数
 *
 * @param [in] pvInfData  上行接口层数据
 * @param [in] RevFrqNo   接收频率号
 *
 * @return  int           0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_MsCSBKFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT8 SndFrq  = uSndFrq[RevFrqNo];
    UINT8 SndSlot = uSndSlot[RevFrqNo];
    UINT16 u2CRC = 0;
    UINT16 AirCRC = 0;
    NAS_AI_PAYLOAD NasAiData;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    SUPS_CSBK_PDU *pSupsCsbk = (SUPS_CSBK_PDU *)(pDataLink->PayLoad);

    u2CRC = ALG_Crc16(pDataLink->PayLoad, CSBK_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    }
    else
    {
        u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
    }

    AirCRC = ((UINT16)(pSupsCsbk->auCRC[0]) << 8) | pSupsCsbk->auCRC[1];
    if (u2CRC != AirCRC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] CSBK CRC Err", _F_);
        return ERR;
    }

    // standard
    if (SFID == pSupsCsbk->uFID && PRE_CSBKO == pSupsCsbk->uCSBKO)//终端相关业务的预载波(上下行预载波)
    {
        if (0xff == uGpsFlag)
        {
            IDP_RelayLinkData(pvInfData, RevFrqNo, S_CSBK_PRE);//上行终端GPS预载玻不进行转发，其余情况转发,预载波按照规则翻转频点时隙
        }
        else
        {
            if (tDllPrint->AIUp == 1)
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s][%d][Gps Ms Upload] PRE CSBK from [%#x:%#x:%#x] abandon", _F_, __LINE__, pSupsCsbk->auSADDR[0], pSupsCsbk->auSADDR[1], pSupsCsbk->auSADDR[2]);
            }
            return NO_ERR;
        }
        return NO_ERR;
    }

    // hytera
    if (HFID == pSupsCsbk->uFID)//PDT
    {
        if (REQ_CSBKO == pSupsCsbk->uCSBKO)
        {
            memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
            ptInfData->ResFrqSlot = (SndFrq+1)<<4 | (SndSlot+1);
            ptInfData->TimeStamp = pvInfData->TimeStamp%2880000 + 2;
            ptInfData->SlotNum = uSndSlot[RevFrqNo];

            if (F_1 == SndFrq)
            {
                ptInfData->TxFreq1 = 1;
            }
            else
            {
                ptInfData->TxFreq2 = 1;
            }

            memcpy(&(ptInfData->tDataLink[SndFrq]), &(pvInfData->tDataLink[RevFrqNo]), sizeof(DATA_LINK_T));//请求按照规则翻转频率时隙

            if (tDllPrint->AIUp == 1)
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s][%d] REQ CSBK relay: ResFrq %d ResSlot %d", _F_, __LINE__, (SndFrq+1), (SndSlot+1));
            }

            ODP_SendInfData(ptInfData, S_CSBK_REQ);
            return NO_ERR;
        }

        if (ACK_CSBKO == pSupsCsbk->uCSBKO)
        {
            memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
            ptCclData->MsgType = DI_MSG_DATA;
            ptCclData->FrmType = FT_VOICE_NO;
            memcpy(ptCclData->DstId, pSupsCsbk->auTADDR, 3);
            memcpy(ptCclData->SrcId, pSupsCsbk->auSADDR, 3);

            if (EN_RADIO_SSO == pSupsCsbk->uSSO)
            {
                ptCclData->DataType = CT_ENABLE_ACK_MS;
                ptCclData->DataLen = 1;
                ptCclData->PayLoad[0] = NO_ERR;
                IDP_SendCclData(ptCclData);

                //遥晕激活链路中转
                memset(&NasAiData, 0, sizeof(NasAiData));
                NasAiData.cmd_code = CMO_CODE_MS_ENABLE;
                NasAiData.nm_type = NM_TYPE_CENTER;
                NasAiData.op_code = OP_CODE_SET_ACK;
                NasAiData.src_id = 31;          //11111
                NasAiData.dst_id = 31;          //11111
                memcpy(NasAiData.data, pSupsCsbk->auSADDR, 3);
                memcpy(&NasAiData.data[3], pSupsCsbk->auTADDR, 3);//用于上行CCL解析具体哪个终端
                NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
//                WaitFollowEnable(3000);
#if 0
                DstId = NasAiData.dst_id;
                SrcId = NasAiData.src_id;
                ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
#endif
                memset(ptInfData, 0, sizeof(NAS_INF_DL_T));//主动生成的NAS格式的 MS_ACK不进行时隙频率翻转
                CallingShootData((pvInfData->TimeStamp+18), tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//                ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
                // by zhoudayuan
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
            }
            else if (DIS_RADIO_SSO == pSupsCsbk->uSSO)
            {
                //遥晕激活应答上报本地中心
                ptCclData->DataType = CT_DISABLE_ACK_MS;
                ptCclData->DataLen = 1;
                ptCclData->PayLoad[0] = NO_ERR;
                IDP_SendCclData(ptCclData);

                //遥晕激活链路中转
                memset(&NasAiData, 0, sizeof(NasAiData));
                NasAiData.cmd_code = CMO_CODE_MS_DISABlE;
                NasAiData.nm_type = NM_TYPE_CENTER;
                NasAiData.op_code = OP_CODE_SET_ACK;
                NasAiData.src_id = 31;          //11111
                NasAiData.dst_id = 31;          //11111
                memcpy(NasAiData.data, pSupsCsbk->auSADDR, 3);
                memcpy(&NasAiData.data[3], pSupsCsbk->auTADDR, 3);
                NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

//                WaitFollowEnable(3000);
#if 0
                DstId = NasAiData.dst_id;
                SrcId = NasAiData.src_id;
                ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);
#endif
                memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
                CallingShootData((pvInfData->TimeStamp+18), tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//                ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
                // by zhoudayuan
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
            }

            return NO_ERR;
        }

        if (ALARM_CSBKO == pSupsCsbk->uCSBKO)
        {
            if (0x0f > pSupsCsbk->uSSO)         //req  过滤ALARM_ACK
            {
                if (tDllPrint->AIUp == 1)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][%d] PRE CSBK(ALARM) relay", _F_, __LINE__);
                }
                IDP_RelayLinkData(pvInfData, RevFrqNo, S_ALARM_REQ);

                //紧急告警请求上报本地中心，  目的让EPACK进行告警
                memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                ptCclData->MsgType = DI_MSG_DATA;
                ptCclData->FrmType = FT_VOICE_NO;
                memcpy(ptCclData->DstId, pSupsCsbk->auTADDR, sizeof(pSupsCsbk->auTADDR));
                memcpy(ptCclData->SrcId, pSupsCsbk->auSADDR, sizeof(pSupsCsbk->auSADDR));
                ptCclData->DataType = CT_ALARM_REQ_MS;
                ptCclData->DataLen = CSBK_LEN;
                IDP_SendCclData(ptCclData);
            }
            else        //ack
            {
            }
        }
    }
    else if (MFID == pSupsCsbk->uFID) //DMR
    {
        if (REQ_CSBKO_DMR == pSupsCsbk->uCSBKO)//DMR遥毙/激活请求/响应
        {
            if((DIS_RADIO_REV_DMR == pSupsCsbk->uREV)||(EN_RADIO_REV_DMR == pSupsCsbk->uREV))//DMR遥毙请求/激活请求
            {
                memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
                ptInfData->ResFrqSlot = (SndFrq+1)<<4 | (SndSlot+1);
                ptInfData->TimeStamp = pvInfData->TimeStamp%2880000 +2;
                ptInfData->SlotNum = uSndSlot[RevFrqNo];

                if (F_1 == SndFrq)
                {
                    ptInfData->TxFreq1 = 1;
                }
                else
                {
                    ptInfData->TxFreq2 = 1;
                }

                memcpy(&(ptInfData->tDataLink[SndFrq]), &(pvInfData->tDataLink[RevFrqNo]), sizeof(DATA_LINK_T));

                if (tDllPrint->AIUp == 1)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][DMR][%s][%d] REQ CSBK relay: ResFrq %d ResSlot %d", _F_, __LINE__, (SndFrq+1), (SndSlot+1));
                }

                ODP_SendInfData(ptInfData, S_CSBK_REQ);
            }
            else if(EN_RADIO_REV_ACK_DMR == pSupsCsbk->uREV)//激活响应
            {
                memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                ptCclData->MsgType = DI_MSG_DATA;
                ptCclData->FrmType = FT_VOICE_NO;
                memcpy(ptCclData->DstId, pSupsCsbk->auTADDR, 3);
                memcpy(ptCclData->SrcId, pSupsCsbk->auSADDR, 3);

                ptCclData->DataType = CT_ENABLE_ACK_MS;
                ptCclData->DataLen = 1;
                ptCclData->PayLoad[0] = NO_ERR;
                IDP_SendCclData(ptCclData);

                //遥晕激活链路中转
                memset(&NasAiData, 0, sizeof(NasAiData));
                NasAiData.cmd_code = CMO_CODE_MS_ENABLE;
                NasAiData.nm_type = NM_TYPE_CENTER;
                NasAiData.op_code = OP_CODE_SET_ACK;
                NasAiData.src_id = 31;          //11111
                NasAiData.dst_id = 31;          //11111
                memcpy(NasAiData.data, pSupsCsbk->auSADDR, 3);
                memcpy(&NasAiData.data[3], pSupsCsbk->auTADDR, 3);
                NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

                memset(ptInfData, 0, sizeof(NAS_INF_DL_T));//主动发的NAS     终端ACK 不翻转频点时隙
                CallingShootData((pvInfData->TimeStamp+18), tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
                ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
                // by zhoudayuan
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
            }
            else if(DIS_RADIO_REV_ACK_DMR == pSupsCsbk->uREV)//遥毙响应
            {
                memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                ptCclData->MsgType = DI_MSG_DATA;
                ptCclData->FrmType = FT_VOICE_NO;
                memcpy(ptCclData->DstId, pSupsCsbk->auTADDR, 3);
                memcpy(ptCclData->SrcId, pSupsCsbk->auSADDR, 3);

                //遥晕激活应答上报本地中心
                ptCclData->DataType = CT_DISABLE_ACK_MS;
                ptCclData->DataLen = 1;
                ptCclData->PayLoad[0] = NO_ERR;
                IDP_SendCclData(ptCclData);

                //遥晕激活链路中转
                memset(&NasAiData, 0, sizeof(NasAiData));
                NasAiData.cmd_code = CMO_CODE_MS_DISABlE;
                NasAiData.nm_type = NM_TYPE_CENTER;
                NasAiData.op_code = OP_CODE_SET_ACK;
                NasAiData.src_id = 31;          //11111
                NasAiData.dst_id = 31;          //11111
                memcpy(NasAiData.data, pSupsCsbk->auSADDR, 3);
                memcpy(&NasAiData.data[3], pSupsCsbk->auTADDR, 3);
                NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

                memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
                CallingShootData((pvInfData->TimeStamp+18), tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
                ODP_PrintNasCmdOpLog(&NasAiData);   //  打印Nas,cmd和op by zhoudayuan
                // by zhoudayuan
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
                ODP_SendInfData(ptInfData, S_CSBK_ACK);
            }
        }
        else if (ALARM_CSBKO_DMR == pSupsCsbk->uCSBKO)
        {
            if (DIGITAL_ALARM_SSO_DMR > pSupsCsbk->uSSO)         //req
            {
                if (tDllPrint->AIUp == 1)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][DMR][%s][%d] PRE CSBK(ALARM) relay", _F_, __LINE__);
                }
                IDP_RelayLinkData(pvInfData, RevFrqNo, S_ALARM_REQ);

                //紧急告警请求上报本地中心
                memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                ptCclData->MsgType = DI_MSG_DATA;
                ptCclData->FrmType = FT_VOICE_NO;
                memcpy(ptCclData->DstId, pSupsCsbk->auTADDR, sizeof(pSupsCsbk->auTADDR));
                memcpy(ptCclData->SrcId, pSupsCsbk->auSADDR, sizeof(pSupsCsbk->auSADDR));
                ptCclData->DataType = CT_ALARM_REQ_MS;
                ptCclData->DataLen = CSBK_LEN;
                IDP_SendCclData(ptCclData);
            }
        }
    }
    return NO_ERR;
}


/**
 * @brief   处理上行无确认分组数据头
 *
 * @param [in] puData      待处理数据存储区指针
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_UCHeaderFun(UINT8 *puData)
{
    PDP_HEAD_UU* puUPHdr = (PDP_HEAD_UU*)puData;
    DLL_ENCLIB_DATA Decry_Data;
    memset(&Decry_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if (0x01 == puUPHdr->p_PdpNcHdr.uFMF)
        {
            g_PdpUBuf.uDPF      = puUPHdr->p_PdpNcHdr.uDPF;
            g_PdpUBuf.uBF       = puUPHdr->p_PdpNcHdr.uBF;
            g_PdpUBuf.uSAP      = puUPHdr->p_PdpNcHdr.uSAP;
            g_PdpUBuf.uBLOCKNum = puUPHdr->p_PdpNcHdr.uBF-g_DllGlobalCfg.auPI;
            g_PdpUBuf.uState    = PDP_STATE_FIRST;
            g_PdpUBuf.uDEI      = puUPHdr->p_PdpNcHdr.uDEI;
            g_PdpUBuf.uBLOCKCnt = 0;
            g_PdpUBuf.uTdLcCnt  = 0;

            memcpy(g_PdpUBuf.uDstId, puUPHdr->p_PdpNcHdr.auTADDR, 3);
            memcpy(g_PdpUBuf.uSrcId, puUPHdr->p_PdpNcHdr.auSADDR, 3);
            memcpy(g_PdpUBuf.auHead[0], puData, DATA_HEADER_LEN + 2);

            IDP_InitFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum-1,0);
            IDP_InitFlag(g_PdpUBuf.auResData, g_PdpUBuf.uBLOCKNum, 127,1);
        }
    }
    else
    {
        if (0x01 == puUPHdr->d_PdpNcHdr.uF)
        {
            g_PdpUBuf.uDPF      = puUPHdr->d_PdpNcHdr.uDPF;
            g_PdpUBuf.uBF       = puUPHdr->d_PdpNcHdr.uBF;
            g_PdpUBuf.uSAP      = puUPHdr->d_PdpNcHdr.uSAP;
            g_PdpUBuf.uBLOCKNum = puUPHdr->d_PdpNcHdr.uBF-g_DllGlobalCfg.auPI;
            g_PdpUBuf.uState    = PDP_STATE_FIRST;
            g_PdpUBuf.uPOC      = puUPHdr->d_PdpNcHdr.uPOCH<<4 | puUPHdr->d_PdpNcHdr.uPOCL;
            g_PdpUBuf.uBLOCKCnt = 0;
            g_PdpUBuf.uTdLcCnt  = 0;

            memcpy(g_PdpUBuf.uDstId, puUPHdr->p_PdpNcHdr.auTADDR, 3);
            memcpy(g_PdpUBuf.uSrcId, puUPHdr->p_PdpNcHdr.auSADDR, 3);
            memcpy(g_PdpUBuf.auHead[0], puData, DATA_HEADER_LEN + 2);

            IDP_InitFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum-1,0);
            IDP_InitFlag(g_PdpUBuf.auResData, g_PdpUBuf.uBLOCKNum, 127,1);
        }

    }
    if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_BASE)//基本加密需要在(U)C_HEAD初始化加密库，高级加密需要在EHEAD初始化加密库
    {
        memcpy(Decry_Data.EHead,puData,DATA_HEADER_LEN);
        Decry_Data.Type= TYPE_EHEAD;
        int ret=DecryptFunc_Data(&Decry_Data);
        if(ret==0)
        {
            memcpy(g_PdpUBuf.auHead[1],Decry_Data.EHead,DATA_HEADER_LEN);
            LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_BASE][Decry_Data Init OK]", _F_);
            LOG_PrintM(s_LogMsgId,Decry_Data.EHead,DATA_HEADER_LEN);
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_BASE][Decry_Data Init Fail]", _F_);
            return ERR;
        }
    }
    return NO_ERR;
}


/**
 * @brief   处理上行带确认分组数据头
 *
 * @param [in] puData      待处理数据存储区指针
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_CHeaderFun(UINT8 *puData)
{
    PDP_HEAD_UU* puUPHdr = (PDP_HEAD_UU*)puData;

    // 只有C_HEAD
    if (0 == puUPHdr->p_Pdp_CHdr.uBF)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Data Just a single C_HEAD", _F_);
        return NO_ERR;
    }

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if (0x01 == puUPHdr->p_Pdp_CHdr.uFMF && ((0x03 == puUPHdr->p_Pdp_CHdr.uSAP)||(0x0b == puUPHdr->p_Pdp_CHdr.uSAP)))
        {
            g_PdpUBuf.uDPF      = puUPHdr->p_Pdp_CHdr.uDPF;
            g_PdpUBuf.uBF       = puUPHdr->p_Pdp_CHdr.uBF;
            g_PdpUBuf.uSAP      = puUPHdr->p_Pdp_CHdr.uSAP;
            g_PdpUBuf.uBLOCKNum = puUPHdr->p_Pdp_CHdr.uBF-g_DllGlobalCfg.auPI;
            g_PdpUBuf.uState    = PDP_STATE_FIRST;
            g_PdpUBuf.uDEI      = puUPHdr->p_Pdp_CHdr.uDEI;
            g_PdpUBuf.uNS       = puUPHdr->p_Pdp_CHdr.uNS;
            g_PdpUBuf.uBLOCKCnt = 0;
            g_PdpUBuf.uTdLcCnt  = 0;
            g_PdpUBuf.ucRetryNum = 0;
            g_PdpUBuf.ucErrDBSNCnt = 0;

            memcpy(g_PdpUBuf.uSrcId, puUPHdr->p_Pdp_CHdr.auSADDR, 3);
            memcpy(g_PdpUBuf.uDstId, puUPHdr->p_Pdp_CHdr.auTADDR, 3);
            memcpy(g_PdpUBuf.auHead[0], puData, DATA_HEADER_LEN + 2);

            IDP_InitFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum-1,0);
            IDP_InitFlag(g_PdpUBuf.auResData, g_PdpUBuf.uBLOCKNum, 127,1);
        }
    }
    else
    {
        if (0x01 == puUPHdr->d_PdpNcHdr.uF && ((0x03 == puUPHdr->d_Pdp_CHdr.uSAP)||(0x09 == puUPHdr->d_Pdp_CHdr.uSAP)))
        {
            g_PdpUBuf.uDPF      = puUPHdr->d_Pdp_CHdr.uDPF;
            g_PdpUBuf.uBF       = puUPHdr->d_Pdp_CHdr.uBF;
            g_PdpUBuf.uSAP      = puUPHdr->d_Pdp_CHdr.uSAP;
            g_PdpUBuf.uBLOCKNum = puUPHdr->d_Pdp_CHdr.uBF-g_DllGlobalCfg.auPI;
            g_PdpUBuf.uState    = PDP_STATE_FIRST;
            g_PdpUBuf.uPOC      = puUPHdr->d_Pdp_CHdr.uPOCH<<4 | puUPHdr->d_Pdp_CHdr.uPOCL;
            g_PdpUBuf.uNS       = puUPHdr->d_Pdp_CHdr.uNS;
            g_PdpUBuf.uBLOCKCnt = 0;
            g_PdpUBuf.uTdLcCnt  = 0;
            g_PdpUBuf.ucRetryNum = 0;
            g_PdpUBuf.ucErrDBSNCnt = 0;

            memcpy(g_PdpUBuf.uSrcId, puUPHdr->p_Pdp_CHdr.auSADDR, 3);
            memcpy(g_PdpUBuf.uDstId, puUPHdr->p_Pdp_CHdr.auTADDR, 3);
            memcpy(g_PdpUBuf.auHead[0], puData, DATA_HEADER_LEN + 2);

            IDP_InitFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum-1,0);
            IDP_InitFlag(g_PdpUBuf.auResData, g_PdpUBuf.uBLOCKNum, 127,1);
        }

    }

    return NO_ERR;
}


/**
 * @brief   处理上行预定义分组数据头
 *
 * @param [in] puData      待处理数据存储区指针
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_DSDHeaderFun(UINT8 *puData)
{
    return NO_ERR;
}


/**
 * @brief   处理上行响应分组数据头
 *
 * @param [in] puData      待处理数据存储区指针
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_REHeaderFun(UINT8 *puData)
{
    PDP_HEAD_UU* puUPHdr = (PDP_HEAD_UU*)puData;

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        if (0 == puUPHdr->p_PdpReHdr.uA && ((3 == puUPHdr->p_PdpReHdr.uSAP)||(11 == puUPHdr->p_PdpReHdr.uSAP)) && 0 == puUPHdr->p_PdpReHdr.uBF)
        {
            if (0 == puUPHdr->p_PdpReHdr.uCLAS && 1 == puUPHdr->p_PdpReHdr.uTYPE)  // NS 的所有数据帧全部接收
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s] Response header", _F_);
            }
            else
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] Response header Err(CLASS:%d,TYPE:%d)",
                            _F_,
                            puUPHdr->p_PdpReHdr.uCLAS,
                            puUPHdr->p_PdpReHdr.uTYPE);
            }
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Response header Err(A:%d,SAP:%d,BF:%d)",
                        _F_,
                        puUPHdr->p_PdpReHdr.uA,
                        puUPHdr->p_PdpReHdr.uSAP,
                        puUPHdr->p_PdpReHdr.uBF);
        }
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s][DMR] Response header", _F_);
    }

    return NO_ERR;
}


/**
 * @brief   处理上行Data Header
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_DataHeaderFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT16 SrcChecksum  = 0;
    UINT16 DstChecksum  = 0;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    UINT8 *puData = pDataLink->PayLoad;
    PDP_HEAD_UU *puUPHdr = (PDP_HEAD_UU *)puData;
    DLL_ENCLIB_DATA Detrypt_Data;
    memset(&Detrypt_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    DstChecksum = ALG_Crc16(puData, DATA_HEADER_LEN);

    if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
    {
        DstChecksum ^= PDT_CRC_MASK_DATA;
    }
    else
    {
        DstChecksum ^= DMR_CRC_MASK_DATA;
    }

    SrcChecksum = ((UINT16)*(puData + 10) << 8) | *(puData + 11);

    if (DstChecksum != SrcChecksum)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Dataheader CRC Err(DPF:%d)", _F_, puUPHdr->d_PdpNcHdr.uDPF);
        return ERR;
    }
    if((puUPHdr->d_PdpNcHdr.uDPF != DPF_PDT_E2E_ENCRYPT)&&(puUPHdr->d_PdpNcHdr.uDPF != DPF_PROP_DATA_PACKET))
    {
        //LOG_WARNING(s_LogMsgId,"[DLL][%s] Clear g_PdpUBuf,puUPHdr->d_PdpNcHdr.uDPF=%d", _F_,puUPHdr->d_PdpNcHdr.uDPF);
        memset(&g_PdpUBuf, 0, sizeof(PDP_UT));
    }
    switch (puUPHdr->d_PdpNcHdr.uDPF)
    {
        case DPF_UNCONFIRM_PACKET:  // 无确认数据分组
        {
            // 只有U_HEAD
            if (0 == puUPHdr->d_PdpNcHdr.uBF)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] Data Just a single U_HEAD", _F_);
                return NO_ERR;
            }
            else
            {
                if ((6+g_DllGlobalCfg.auPI) < puUPHdr->d_PdpNcHdr.uBF)
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] U_Data length = %d over range", _F_,puUPHdr->d_PdpNcHdr.uBF);
                    return NO_ERR;
                }
            }

            IDP_RelayLinkData(pvInfData, RevFrqNo, S_U_HDR);//组包+发送
            IDP_UCHeaderFun(puData);
            break;
        }
        case DPF_CONFIRM_PACKET:  // confirm, 有确认数据分组
        {
            if ((2+g_DllGlobalCfg.auPI) == puUPHdr->p_Pdp_CHdr.uBF && 0xff == uGpsFlag)//下行分组数据头转发
            {
                uGpsFlag = GPS_PRE;
                IDP_CHeaderFun(puData);
                IDP_RelayLinkData(pvInfData, RevFrqNo, S_C_HDR);

                DLL_SetTimer(CALL_DATA_D, 1000);        //定时1s
                //return NO_ERR;
            }
            else if ((3+g_DllGlobalCfg.auPI) == puUPHdr->p_Pdp_CHdr.uBF && GPS_REQ == uGpsFlag)//上行分组数据头不进行转发
            {
                uGpsFlag = GPS_ACK;
                IDP_CHeaderFun(puData);
            }
            else
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] DataHdr GpsFlag %x", _F_, uGpsFlag);
                break;
            }

            if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_BASE)
            {
                memcpy(Detrypt_Data.EHead,puData,DATA_HEADER_LEN);
                Detrypt_Data.Type= TYPE_EHEAD;
                int ret=DecryptFunc_Data(&Detrypt_Data);
                if(ret==0)
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_BASE][Decryption Init OK][C_HEAD]", _F_);
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s][ENCRYPT_HYT_BASE][Decryption Init Fail]", _F_);
                    return ERR;
                }
            }
            break;
        }
        case DPF_DEFINED_SD_PACKET:  // 预定义短数据
        {
            IDP_DSDHeaderFun(puData);
            break;
        }
        case DPF_RESPONSE_PACKET:    // Response, 响应头
        {
            IDP_REHeaderFun(puData);
            break;
        }
        case DPF_PDT_E2E_ENCRYPT:
        case DPF_PROP_DATA_PACKET:
        //case DPF_E2E_NVOC_UNCONFIRM_PACKET:
        //case DPF_E2E_NVOC_CONFIRM_PACKET://处理加密数据头EHEAD
        {
            if((uGpsFlag==0xff)||(uGpsFlag==GPS_PRE)) //1/2分组数据EHEAD或3/4下发的EHEAD请求
            {
                IDP_RelayLinkData(pvInfData, RevFrqNo, S_E_HDR);
            }
            else if(uGpsFlag==GPS_ACK)//终端上行的EHEAD
            {
                LOG_DEBUG(s_LogMsgId,"[DLL][%s] Rcv Ehead . uGpsFlag=%d", _F_,uGpsFlag);
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[DLL][%s] Rcv Ehead Error. uGpsFlag=%d", _F_,uGpsFlag);
                break;
            }

            if((g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_ENHA)||(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA))
            {
                memcpy(Detrypt_Data.EHead,puData,DATA_HEADER_LEN);
                Detrypt_Data.Type= TYPE_EHEAD;
                int ret=DecryptFunc_Data(&Detrypt_Data);
                if(ret==0)
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s][High Encryption][E_HEAD][Decry Init OK]", _F_);
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s][High Encryption][E_HEAD][Decry Init Fail]", _F_);
                    return ERR;
                }
            }
            break;
        }
        case DPF_UDT_PACKET:
        case DPF_RAW_SD_PACKET:
        {
            break;
        }
        default:    // 未定义
        {
            LOG_ERROR(s_LogMsgId,"[DLL][%s] Dataheader Undefined DPF type", _F_);
            break;
        }
    }


    return NO_ERR;
}


/**
 * @brief   处理上行R12数据
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_R12PackDataFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT8 Restat = NO_ERR;
    UINT32 u4CRCA  = 0;
    UINT32 u4CRCB  = 0;

    PDP_DATA_BLOCK_UU* puUPData = (PDP_DATA_BLOCK_UU*)g_PdpUBuf.auData;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    R12_UU *puD12Data = (R12_UU *)(pDataLink->PayLoad);
    DLL_ENCLIB_DATA Decry_Data;
    memset(&Decry_Data, 0x00, sizeof(DLL_ENCLIB_DATA));

    if (DPF_UNCONFIRM_PACKET == g_PdpUBuf.uDPF)
    {
        IDP_RelayLinkData(pvInfData, RevFrqNo, S_DATA_1_2);

        memcpy(puUPData->tPdpR12NcBlk.auData[g_PdpUBuf.uBLOCKCnt], puD12Data->tR12NcData.auData, R12_NC_DATA_ULEN);//攒包做整体校验
        IDP_WriteFlag(g_PdpUBuf.auResData, g_PdpUBuf.uBLOCKCnt, 1);

        g_PdpUBuf.uDataType = DT_R_1_2_DATA;
        g_PdpUBuf.uBLOCKCnt++;

        if (g_PdpUBuf.uBLOCKNum <= g_PdpUBuf.uBLOCKCnt)
        {
            puD12Data = (R12_UU *)puUPData->tPdpR12NcBlk.auData[g_PdpUBuf.uBLOCKNum-1];
            u4CRCA = ALG_Crc32((UINT8 *)puUPData, R12_NC_DATA_ULEN*g_PdpUBuf.uBLOCKNum - 4);
            u4CRCB = (puD12Data->tR12NcLData.auRV[3] << 24) |
                     (puD12Data->tR12NcLData.auRV[2] << 16) |
                     (puD12Data->tR12NcLData.auRV[1] << 8)  |
                     (puD12Data->tR12NcLData.auRV[0]);

            if (IDP_CheckFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum - 1))//中间丢包不作任何处理
            {
                if (u4CRCA == u4CRCB)
                {
                    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                    ptCclData->MsgType = pDataLink->MsgType;
                    ptCclData->FrmType = pDataLink->FrmType;
                    ptCclData->DataType = CT_PACKET_DATA;
                    //计算实际的数据长度
                    if (PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
                    {
                        Restat = DetectEndPattern(&g_PdpUBuf, &(ptCclData->DataLen));
                        if (Restat == ERR)
                        {
                            return ERR;
                        }
                    }
                    else
                    {
                        ptCclData->DataLen = g_PdpUBuf.uBLOCKNum * R12_NC_DATA_ULEN -4 -g_PdpUBuf.uPOC;
                    }

                    memcpy(ptCclData->DstId, g_PdpUBuf.uDstId, 3);
                    memcpy(ptCclData->SrcId, g_PdpUBuf.uSrcId, 3);

                    LOG_WARNING(s_LogMsgId,"[DLL][%s][Before][Admin Data:]", _F_);
                    LOG_PrintM(s_LogMsgId,g_PdpUBuf.auData,ptCclData->DataLen);

                    if(g_DllGlobalCfg.auPI==1)
                    {
                        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
                        {
                            Decry_Data.Data=g_PdpUBuf.auData+5;
                            Decry_Data.DataLen=ptCclData->DataLen-5;
                        }
                        else
                        {
                            Decry_Data.Data=g_PdpUBuf.auData;
                            Decry_Data.DataLen=ptCclData->DataLen;
                        }
                        Decry_Data.Type= TYPE_DATA;
                        int ret=DecryptFunc_Data(&Decry_Data);
                        if(ret==0)
                        {
                            LOG_WARNING(s_LogMsgId,"[DLL][%s][After][Admin Data:]", _F_);
                            LOG_PrintM(s_LogMsgId,g_PdpUBuf.auData,ptCclData->DataLen);
                        }
                        else
                        {
                            LOG_WARNING(s_LogMsgId,"[DLL][%s] Data Decryption Fail", _F_);
                            return ERR;
                        }
                    }

                    memcpy(ptCclData->PayLoad, g_PdpUBuf.auData, ptCclData->DataLen);
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] ShortMessage: SrcId:%2x %2x %2x DstId:%2x %2x %2x datalen:%d",
                            _F_,
                            ptCclData->SrcId[0],
                            ptCclData->SrcId[1],
                            ptCclData->SrcId[2],
                            ptCclData->DstId[0],
                            ptCclData->DstId[1],
                            ptCclData->DstId[2],
                            ptCclData->DataLen);

                    LOG_PrintM(s_LogMsgId,  ptCclData->PayLoad, ptCclData->DataLen);

                    IDP_SendCclData(ptCclData);
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] R12Data Crc Check Err", _F_);
                }
            }

        }
    }
    else if (DPF_DEFINED_SD_PACKET == g_PdpUBuf.uDPF)
    {

    }
    else
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] R12Data Undefined DPF type %d", _F_,g_PdpUBuf.uDPF);
    }

    return NO_ERR;
}


/**
 * @brief   处理上行R34数据
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_R34PackDataFun(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT8 i;
    UINT8 *Index = NULL;
    UINT16 DataLen = 0;
    UINT16 u2CRCA = 0;
    UINT16 u2CRCB = 0;
    UINT32 u4CRCA = 0;
    UINT32 u4CRCB = 0;
    UINT8 r34Data[18];

    PDP_DATA_BLOCK_UU *puUPData = (PDP_DATA_BLOCK_UU *)g_PdpUBuf.auData;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    R34_UU *pu34Data = NULL;

    if (GPS_REQ == uGpsFlag || GPS_PRE == uGpsFlag)
    {
    }
    else if (GPS_ACK == uGpsFlag)
    {
    }
    else
    {
        if (tDllPrint->AIUp == 1)
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] R34Data GpsFlag %x", _F_, uGpsFlag);
        }
        return ERR;
    }

    IDP_196to144bptc(pDataLink->PayLoad, r34Data);
    pu34Data = (R34_UU *)r34Data;

    if (tDllPrint->AIUp == 1)
    {
        if (tDllPrint->PrintLv == PRINT_MEMRY || tDllPrint->PrintLv == PRINT_ALL)
        {
            LOG_DEBUG(s_LogMsgId,"[DLL][%s] R34_C_DATA", _F_);
            LOG_PrintM(s_LogMsgId, r34Data, 18);
        }
    }

    if (DPF_CONFIRM_PACKET == g_PdpUBuf.uDPF)  // 有确认数据分组
    {
        u2CRCA = ALG_Crc9(r34Data, R34_C_DATA_DLEN);
        u2CRCB = (UINT16)((pu34Data->tR34CData.uCRC9H << 8) | pu34Data->tR34CData.uCRC9L);

        if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
        {
            u2CRCA = u2CRCA ^ PDT_CRC_MASK_R34;
        }
        else
        {
            u2CRCA = u2CRCA ^ DMR_CRC_MASK_R34;
        }

        if (u2CRCB == u2CRCA)
        {
            memcpy(puUPData->tPdpR34CBlk.auData[pu34Data->tR34CData.uDBSN], pu34Data->tR34CData.auData, R34_C_DATA_DLEN-2);
            IDP_WriteFlag(g_PdpUBuf.auResData, pu34Data->tR34CData.uDBSN, 1);
            if (GPS_PRE == uGpsFlag)//解析是下行的3/4数据才转发，上行不进行转发
            {
                ODP_144to196bptc(r34Data, pvInfData->tDataLink[RevFrqNo].PayLoad);
                IDP_RelayLinkData(pvInfData, RevFrqNo, S_DATA_3_4);
            }
        }
        else
        {
            g_PdpUBuf.ucErrDBSNCnt ++;
            LOG_WARNING(s_LogMsgId,"[DLL][%s] R34_C_DATA CRC ERR(DBSN:%d)", _F_, pu34Data->tR34CData.uDBSN);
            //return ERR;
        }

        g_PdpUBuf.uDataType = DT_R_3_4_DATA;

        if (PDP_STATE_FIRST == g_PdpUBuf.uState)
        {
            g_PdpUBuf.uBLOCKCnt++;
            //包个数够了或者索引够了(中间丢包，体现在错误图样上)
            if ((g_PdpUBuf.uBLOCKNum <= g_PdpUBuf.uBLOCKCnt) || ((g_PdpUBuf.uBLOCKNum -1) == pu34Data->tR34CData.uDBSN))
            {
                //修复重传机制，失败清除原状态
                //需要之前的C_HEAD数据包个数来区分是否NAS的数据还是MS的数据，进而进行分别处理
                //            g_PdpUBuf.uBF  依据此值进行修改
                if(g_PdpUBuf.uBLOCKNum==3)//说明之前的C_HEAD是终端上行的,同时随之而来的3/4数据也是终端上行的,考虑改成blocknum
                {
                    if(g_PdpUBuf.ucErrDBSNCnt>0)//本次失败
                    {
                        if(auGpsRetryCnt==1)//两次都失败
                        {
                            g_PdpUBuf.ucErrDBSNCnt=0;
                            auGpsRetryCnt=0;

                            LOG_WARNING(s_LogMsgId,"[DLL][Ms_Gps Upload] R34_C_DATA Two Times Error [Final Fail]");
                        }
                        else //再给予一次机会，清除之前状态,回到等待终端相应并上报数据状态
                        {
                            auGpsRetryCnt=1;
                            uGpsFlag = GPS_REQ;
                            DLL_SetTimer(CALL_DATA_D, 4000);        //定时4s

                            LOG_WARNING(s_LogMsgId,"[DLL][Ms_Gps Upload] R34_C_DATA %d th Times Error [Retry]",auGpsRetryCnt);
                            return ERR;
                        }
                    }
                    else//成功，单包校验通过
                    {
                        LOG_WARNING(s_LogMsgId,"[DLL][Ms_Gps Upload] R34_C_DATA Success Times:%d th",1+auGpsRetryCnt);
                        g_PdpUBuf.ucErrDBSNCnt=0;
                        auGpsRetryCnt=0;
                    }
                }
                else
                {
                    if(g_PdpUBuf.ucErrDBSNCnt>0)//Req失败
                        LOG_WARNING(s_LogMsgId,"[DLL][%s] Ms_Gps Upload Req CRC Error for Every Bag, Error_Cnt=%d",_F_,g_PdpUBuf.ucErrDBSNCnt);
                    else
                        LOG_WARNING(s_LogMsgId,"[DLL][%s] Ms_Gps Upload Req CRC OK for Every Bag,Not for Whole Bag",_F_);
                    g_PdpUBuf.ucErrDBSNCnt=0;
                    auGpsRetryCnt=0;
                }
                memcpy(pu34Data->tR34CLData.auData, puUPData->tPdpR34CBlk.auData[g_PdpUBuf.uBLOCKNum-1], (R34_C_DATA_DLEN-2));
                u4CRCA = ALG_Crc32((UINT8 *)puUPData, (R34_C_DATA_DLEN-2)*g_PdpUBuf.uBLOCKNum - 4);
                u4CRCB = (pu34Data->tR34CLData.auCRC[3] << 24) |
                         (pu34Data->tR34CLData.auCRC[2] << 16) |
                         (pu34Data->tR34CLData.auCRC[1] << 8)  |
                         (pu34Data->tR34CLData.auCRC[0]);

                g_PdpUBuf.uState = PDP_STATE_IDLE;
                if (IDP_CheckFlag(g_PdpUBuf.auResData, 0, g_PdpUBuf.uBLOCKNum - 1))
                {
                    if (u4CRCA == u4CRCB)
                    {
                        if (PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
                        {
                            if (g_PdpUBuf.uDEI == 0)
                            {
                                Index = puUPData->tPdpR34CBlk.auData[g_PdpUBuf.uBLOCKNum-2];
                                for (i = 0; i < (R34_C_DATA_DLEN-2); i++)
                                {
                                    if (*(Index+i) == 0x7f)
                                    {
                                        UINT8 total=0xff;
                                        for(int j=i+1;j<(R34_C_DATA_DLEN-2);j++)
                                        {
                                            total=total & (*(Index+j));
                                        }
                                        if(total==0xff)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            continue;
                                        }
                                    }
                                }
                                DataLen = g_PdpUBuf.uBLOCKNum * (R34_C_DATA_DLEN-2) -4 -(R34_C_DATA_DLEN-2-i+12);
                            }
                            else if (g_PdpUBuf.uDEI == 1)
                            {
                                Index = puUPData->tPdpR34CBlk.auData[g_PdpUBuf.uBLOCKNum-1];
                                for (i = 0; i < (R34_C_DATA_DLEN-2); i++)
                                {
                                    if (*(Index+i) == 0x7f)
                                    {
                                        UINT8 total=0xff;
                                        for(int j=i+1;j<(R34_C_DATA_DLEN-2);j++)
                                        {
                                            total=total & (*(Index+j));
                                        }
                                        if(total==0xff)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            continue;
                                        }
                                    }
                                }
                                DataLen = g_PdpUBuf.uBLOCKNum * (R34_C_DATA_DLEN-2) -4 -(12-i);
                            }
                            else if (g_PdpUBuf.uDEI == 2)
                            {
                                DataLen = g_PdpUBuf.uBLOCKNum * (R34_C_DATA_DLEN-2) -4 -12;
                            }
                            else if (g_PdpUBuf.uDEI == 3)
                            {
                                DataLen = g_PdpUBuf.uBLOCKNum * (R34_C_DATA_DLEN-2) -4;
                            }
                            LOG_WARNING(s_LogMsgId,"[DLL][%s][PDT] g_PdpUBuf.uDEI=%d,DataLen=%d",_F_,g_PdpUBuf.uDEI,DataLen);
                        }
                        else
                        {
                            DataLen = g_PdpUBuf.uBLOCKNum * (R34_C_DATA_DLEN-2) -4 -g_PdpUBuf.uPOC;
                            LOG_WARNING(s_LogMsgId,"[DLL][%s][DMR] uPOC=%d,DataLen=%d",_F_,g_PdpUBuf.uPOC,DataLen);
                        }
                        //图样处理完毕，得出数据长度和数据内容
                        R34DataAnalyze(g_PdpUBuf.auData, DataLen);

                    }
                    else
                    {
                        LOG_WARNING(s_LogMsgId,"[DLL][%s] R34Data Crc Check Err",_F_);
                    }
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] R34Data recv lose",_F_);
                }
            }
            else
            {
                return ERR;
            }
        }
        else if (PDP_STATE_RETRY == g_PdpUBuf.uState)
        {
            g_PdpUBuf.uBLOCKCnt_R++;
        }
        else
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] unexpected R34_C_DATA",_F_);
            return NO_ERR;
        }
    }

    return NO_ERR;
}


/**
 * @brief   初始化分组数据正确性图样
 *
 * @param [in]  *puData   待处理数据存储区指针
 * @param [in]  IndexA     图样起始bit位置
 * @param [in]  IndexB     图样结束bit位置
 * @param [in]  uValue     每个bit的值
 *
 * @return  STATUS         见@link returncodes 返回值定义@endlink
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void IDP_InitFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB, UINT8 uValue)
{
    UINT8 uCntA  = 0;
    UINT8 uTempA = 0;
    UINT8 uTempB = 0;

    IndexA &= 0x7F;
    IndexB &= 0x7F;

    uTempA = 0xFF << (IndexA%8);
    uTempB = 0xFF >> (7 -(IndexB%8));

    if (0 < uValue)
    {
        if (IndexA/8 != IndexB/8)
        {
            puData[IndexA/8] |= uTempA;
            puData[IndexB/8] |= uTempB;

            for (uCntA = (IndexA/8 + 1); uCntA < IndexB/8; uCntA++)
            {
                puData[uCntA] = 0xFF;
            }
        }
        else
        {
            puData[IndexA/8] |= uTempA & uTempB;
        }
    }
    else
    {
        if (IndexA/8 != IndexB/8)
        {
            puData[IndexA/8] &= (~uTempA) & 0xFF;
            puData[IndexB/8] &= (~uTempB) & 0xFF;

            for (uCntA = (IndexA/8 + 1); uCntA < IndexB/8; uCntA++)
            {
                puData[uCntA] = 0x00;
            }
        }
        else
        {
            puData[IndexA/8] &= (~(uTempA & uTempB)) & 0xFF;
        }
    }

    return;
}


/**
 * @brief   检查分组数据图样中所有bit是否全为1
 *
 * @param [in]  *puData   待处理数据存储区指针
 * @param [in]  IndexA     图样起始bit位置
 * @param [in]  IndexB     图样结束bit位置
 *
 * @return  BOOL         全1返回TRUE:数据全部接受或者发送成功
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
BOOL IDP_CheckFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB)
{
    UINT8 uCntA  = 0;
    UINT8 uTempA = 0;
    UINT8 uTempB = 0;

    IndexA &= 0x7F;
    IndexB &= 0x7F;

    uTempA = 0xFF << (IndexA%8);
    uTempB = 0xFF >> (7 -(IndexB%8));

    if (IndexA/8 != IndexB/8) //待检查图样大小超过1个字节
    {
        if (((puData[IndexA/8] & uTempA) != uTempA) ||
            ((puData[IndexB/8] & uTempB) != uTempB))  //开始字节图样或结尾字节图样出现为0的bit
        {
            return FALSE;
        }
        for (uCntA = (IndexA/8 + 1); uCntA < IndexB/8; uCntA++) //中间字节图样中出现为0的bit
        {
            if (0xFF != puData[uCntA])
            {
                return FALSE;
            }
        }
    }
    else //待检查图样为1个字节
    {
        if ((uTempA & uTempB) != (puData[IndexA/8] & (uTempA & uTempB)))
        {
            return FALSE;
        }
    }

    return TRUE;
}


/**
 * @brief   为图样中的某一bit位赋值
 *
 * @param [in]  *puData    待处理数据存储区指针
 * @param [in]  IndexA     待赋值的位置
 * @param [in]  IndexB     带赋值的数 0或1
 *
 * @return  无
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void IDP_WriteFlag(UINT8 *puData, UINT8 Index, UINT8 uValue)
{
    Index &= 0x7F;

    if (0 < uValue)
    {
        puData[Index/8] |= (0x01 << (Index%8));
    }
    else
    {
        puData[Index/8] &= ~(0x01 << (Index%8));
    }

    return;
}


/**
 * @brief   读取图样中某一bit位的值
 *
 * @param [in]  *puData    待处理数据存储区指针
 * @param [in]  IndexA     待赋值的位置
 * @param [in]  IndexB     带赋值的数 0或1
 *
 * @return  BOOL           0-TRUE 1-FALSE
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
BOOL IDP_ReadFlag(UINT8 *puData, UINT8 Index)
{
    Index &= 0x7F;

    if (puData[Index/8] & (0x01 << (Index%8)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



/**
 * @brief  上行NAS 预载波处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
int IDP_NasPre(NAS_AI_PAYLOAD * pvNasData)
{
    if ((OP_CODE_GET == pvNasData->op_code) && (g_DllGlobalCfg.auNodeId == pvNasData->dst_id))
    {
        if ((tDllPrint->PrintLv == PRINT_DEBUG) || (tDllPrint->PrintLv == PRINT_ALL))
        {
            if (GetTimerCnt(NAS_PRE_IDP) == 0)
            {
                SetTimerOn(NAS_PRE_IDP, 5, SetTimerOff);  // 300ms后清零关闭该定时器
                LOG_DEBUG(s_LogMsgId, "[DLL][%s] src_%d->dst_%d, ", _F_, pvNasData->src_id, pvNasData->dst_id);
            }
        }
    }
    return NO_ERR;
}



/**
 * @brief  上行NAS GPS上拉数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasGpsReportFun(NAS_AI_PAYLOAD * pvNasData)
{
    NAS_AI_PAYLOAD NasAiData;
    GPS_NAS_DATA_T GpsNasData;
    GPS_DEV_DATA_T *GpsDevData;

    memset(ptInfData,  0, sizeof(NAS_INF_DL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if ((OP_CODE_GET == pvNasData->op_code) && (g_DllGlobalCfg.auNodeId == pvNasData->dst_id))//别的NAS查询本机NAS GPS数据
    {
        GpsDevData = (GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData);

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

        IDP_GpsDevToNas(GpsDevData, &GpsNasData);//压缩

        NasAiData.cmd_code = CMD_CODE_GPS_REQUEST;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_GET_ACK;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvNasData->src_id;
        memcpy(NasAiData.data, &GpsNasData, GPS_LEN);
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
        WaitFollowEnable(3000);//等待FPGA未锁定,低电平状态
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);  // by zhoudayuan
        // by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_GPS_ACK);
        ODP_SendInfData(ptInfData, S_NAS_GPS_ACK);
        ODP_SendInfData(ptInfData, S_NAS_GPS_ACK);
        ODP_SendInfData(ptInfData, S_NAS_GPS_ACK);

    }
    else if ((OP_CODE_GET_ACK == pvNasData->op_code) && (g_DllGlobalCfg.auNodeId == pvNasData->dst_id))
    {
        GenNasCclData(ptCclData, CT_GPS_REPORT_ACK_NAS, pvNasData->dst_id, pvNasData->src_id, GPS_LEN, pvNasData->data);
        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS 摇晕数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasStunFun(NAS_AI_PAYLOAD * pvNasData)
{
    NAS_AI_PAYLOAD NasAiData;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (OP_CODE_SET == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        g_DllGlobalCfg.auStunFlag = NAS_KILL_FLAG;
        ptCFGShm->stun_flag.val   = NAS_KILL_FLAG;
        p_DllFpgaShm->StunFlag[0] = NAS_KILL_FLAG      &0xff;
        p_DllFpgaShm->StunFlag[1] = (NAS_KILL_FLAG>>8) &0xff;
        p_DllFpgaShm->StunFlag[2] = (NAS_KILL_FLAG>>16)&0xff;
        p_DllFpgaShm->StunFlag[3] = (NAS_KILL_FLAG>>24)&0xff;

        GenNasCclData(ptCclData, CT_STUN_RPT_NAS, g_DllGlobalCfg.auNodeId, pvNasData->src_id, 0, ptCclData->PayLoad);//遥晕的主动上报，其他节点查询本节点会触发此流程
        IDP_SendCclData(ptCclData);

        NasAiData.cmd_code = CMD_CODE_STUN;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_SET_ACK;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvNasData->src_id;
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
        WaitFollowEnable(3000);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU,FT_VOICE_NO,DI_MSG_WLU,(NM_DATA_LEN+1),(UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);  // by zhoudayuan
        // 返回4份 by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_STUN_ACK);  // 4
        ODP_SendInfData(ptInfData, S_NAS_STUN_ACK);  // 3
        ODP_SendInfData(ptInfData, S_NAS_STUN_ACK);  // 2
        ODP_SendInfData(ptInfData, S_NAS_STUN_ACK);  // 1



    }
    else if (OP_CODE_SET_ACK == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)//本节点查询其他节点给的ACK回复，上报到本地中心
    {
        GenNasCclData(ptCclData, CT_STUN_ACK_NAS, pvNasData->dst_id, pvNasData->src_id, 0, ptCclData->PayLoad);
        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS 摇毙数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasKillFun(NAS_AI_PAYLOAD * pvNasData)
{
    NAS_AI_PAYLOAD NasAiData;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (OP_CODE_SET == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        g_DllGlobalCfg.auKillFlag = NAS_KILL_FLAG;
        ptCFGShm->kill_flag.val   = NAS_KILL_FLAG;
        p_DllFpgaShm->StunFlag[0] = NAS_KILL_FLAG      &0xff;
        p_DllFpgaShm->StunFlag[1] = (NAS_KILL_FLAG>>8) &0xff;
        p_DllFpgaShm->StunFlag[2] = (NAS_KILL_FLAG>>16)&0xff;
        p_DllFpgaShm->StunFlag[3] = (NAS_KILL_FLAG>>24)&0xff;

        GenNasCclData(ptCclData, CT_KILL_RPT_NAS, g_DllGlobalCfg.auNodeId, pvNasData->src_id, 0, ptCclData->PayLoad);
        IDP_SendCclData(ptCclData);

        NasAiData.cmd_code = CMD_CODE_KILL;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_SET_ACK;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvNasData->src_id;
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
        WaitFollowEnable(3000);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU,FT_VOICE_NO,DI_MSG_WLU,(NM_DATA_LEN+1),(UINT8 *)&NasAiData);
//        ODP_PrintNasCmdOpLog(&NasAiData);  // by zhoudayuan
        // by zhoudayuan
        ODP_SendInfData(ptInfData, S_KILL_ACK);
        ODP_SendInfData(ptInfData, S_KILL_ACK);
        ODP_SendInfData(ptInfData, S_KILL_ACK);
        ODP_SendInfData(ptInfData, S_KILL_ACK);

    }
    else if (OP_CODE_SET_ACK == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        GenNasCclData(ptCclData, CT_KILL_ACK_NAS, pvNasData->dst_id, pvNasData->src_id, 0, ptCclData->PayLoad);
        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS 激活数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasEnableFun(NAS_AI_PAYLOAD * pvNasData)
{
    NAS_AI_PAYLOAD NasAiData;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (OP_CODE_SET == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        g_DllGlobalCfg.auStunFlag = 0;
        g_DllGlobalCfg.auKillFlag = 0;
        ptCFGShm->kill_flag.val = 0;
        ptCFGShm->stun_flag.val = 0;
        p_DllFpgaShm->StunFlag[0] = 0;
        p_DllFpgaShm->StunFlag[1] = 0;
        p_DllFpgaShm->StunFlag[2] = 0;
        p_DllFpgaShm->StunFlag[3] = 0;

        GenNasCclData(ptCclData, CT_ENABLE_RPT_NAS, g_DllGlobalCfg.auNodeId, pvNasData->src_id, 0, ptCclData->PayLoad);
        IDP_SendCclData(ptCclData);

        NasAiData.cmd_code = CMD_CODE_ENABLE;
        NasAiData.nm_type = NM_TYPE_CENTER;
        NasAiData.op_code = OP_CODE_SET_ACK;
        NasAiData.src_id = g_DllGlobalCfg.auNodeId;
        NasAiData.dst_id = pvNasData->src_id;
        NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);
        WaitFollowEnable(3000);
        memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
        CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU,FT_VOICE_NO,DI_MSG_WLU,(NM_DATA_LEN+1),(UINT8 *)&NasAiData);
        // by zhoudayuan
        ODP_SendInfData(ptInfData, S_NAS_ENABLE_ACK);
        ODP_SendInfData(ptInfData, S_NAS_ENABLE_ACK);
        ODP_SendInfData(ptInfData, S_NAS_ENABLE_ACK);
        ODP_SendInfData(ptInfData, S_NAS_ENABLE_ACK);


    }
    else if (OP_CODE_SET_ACK == pvNasData->op_code && g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        GenNasCclData(ptCclData, CT_ENABLE_ACK_NAS, pvNasData->dst_id, pvNasData->src_id, 0, ptCclData->PayLoad);
        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS邻点信息查询处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasNeighborQueryFun(NAS_AI_PAYLOAD * pvNasData)
{
    NAS_AI_PAYLOAD NasAiData;
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));

    if (g_DllGlobalCfg.auNodeId == pvNasData->dst_id)
    {
        if ((OP_CODE_GET == pvNasData->op_code) && (g_DllGlobalCfg.auNodeId == pvNasData->dst_id))  // 获取
        {
            NasAiData.cmd_code = CMD_CODE_NER_QUERY;
            NasAiData.nm_type = NM_TYPE_CENTER;
            NasAiData.op_code = OP_CODE_GET_ACK;
            NasAiData.src_id = pvNasData->dst_id;
            NasAiData.dst_id = pvNasData->src_id;
            NasAiData.data[0] = g_DllGlobalCfg.auNodeId;
            memcpy(&NasAiData.data[1], (UINT8 *)&g_DllGlobalCfg.auNegrId2, NER_LEN);
            NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

            if (tDllPrint->PrintLv == PRINT_DEBUG || tDllPrint->PrintLv == PRINT_ALL)
            {
                CheckNerArray();
                CheckNerInfo(&g_DllGlobalCfg.auNodeId, &g_DllGlobalCfg.auNegrId2, "Local->Other");
            }
            WaitFollowEnable(3000);
            memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
            CallingShootData(0xffffffff, tDllPrint->FrqSlt, DI_MSG_WLU, FT_VOICE_NO, DI_MSG_WLU, (NM_DATA_LEN+1), (UINT8 *)&NasAiData);
            ODP_SendInfData(ptInfData, S_NAS_NEGR_ACK);
            ODP_SendInfData(ptInfData, S_NAS_NEGR_ACK);
            ODP_SendInfData(ptInfData, S_NAS_NEGR_ACK);
            ODP_SendInfData(ptInfData, S_NAS_NEGR_ACK);
        }
        else if ((OP_CODE_GET_ACK == pvNasData->op_code) && (g_DllGlobalCfg.auNodeId == pvNasData->dst_id))
        {

            if ((tDllPrint->PrintLv == PRINT_DEBUG) || (tDllPrint->PrintLv == PRINT_ALL))
            {
                if (GetTimerCnt(TMR_NER_ACK) == 0)
                {
                    SetTimerOn(TMR_NER_ACK, 5, SetTimerOff);  // 300ms
                    CheckNerInfo(&pvNasData->data[0], (unsigned long *)&pvNasData->data[1], "Other->Local");
                }
            }
            GenNasCclData(ptCclData, CT_NEGHR_QUERY_ACK, pvNasData->dst_id, pvNasData->src_id, (NER_LEN+1), pvNasData->data);
            IDP_SendCclData(ptCclData);
        }
    }

    return NO_ERR;
}

/**
 * @brief  上行NAS邻点信息上报处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasNegrReportFun(NAS_AI_PAYLOAD * pvNasData)
{
    if (OP_CODE_GET_ACK == pvNasData->op_code)
    {
        memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
        GenNasCclData(ptCclData, CT_NEGHR_REPORT, pvNasData->dst_id, pvNasData->src_id, (NER_LEN+1), pvNasData->data);
        IDP_SendCclData(ptCclData);
    }
    return NO_ERR;
}


/**
 * @brief  上行NAS 响应MS Gps数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasMsGpsFun(NAS_AI_PAYLOAD * pvNasData)
{
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));

    if (OP_CODE_GET_ACK == pvNasData->op_code)
    {
        ptCclData->MsgType = DI_MSG_DATA;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_GPS_REPORT_ACK_MS;
        ptCclData->SrcId[0] = pvNasData->src_id;
        ptCclData->DstId[0] = pvNasData->dst_id;
        ptCclData->DataLen = GPS_LEN;
        memcpy(ptCclData->PayLoad, pvNasData->data, GPS_LEN);

        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS 响应MS 遥毙数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasMsDisableFun(NAS_AI_PAYLOAD * pvNasData)
{
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));

    if (OP_CODE_SET_ACK == pvNasData->op_code)
    {
        ptCclData->MsgType = DI_MSG_DATA;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_DISABLE_ACK_MS;
        memcpy(ptCclData->DstId, &pvNasData->data[3], 3);
        memcpy(ptCclData->SrcId, pvNasData->data, 3);
        ptCclData->DataLen = 0;

        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行NAS 响应MS 复活数据处理函数
 *
 * @param [in]  pvNasData       上行链路机数据
 *
 * @return  int         0:成功1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int IDP_NasMsEnableFun(NAS_AI_PAYLOAD * pvNasData)
{
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));

    if (OP_CODE_SET_ACK == pvNasData->op_code)
    {
        ptCclData->MsgType = DI_MSG_DATA;
        ptCclData->FrmType = FT_VOICE_NO;
        ptCclData->DataType = CT_ENABLE_ACK_MS;
        memcpy(ptCclData->DstId, &pvNasData->data[3], 3);
        memcpy(ptCclData->SrcId, pvNasData->data, 3);

        IDP_SendCclData(ptCclData);
    }

    return NO_ERR;
}


/**
 * @brief  上行中转链路数据处理函数
 *
 * @param [in] pvInfData       上行接口层数据
 * @param [in] RevFrqNo       接收频率号
 * @param [in] SapType         业务数据类型
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void IDP_RelayLinkData(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo, UINT8 SapType)
{
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    UINT8 SndFrq = uSndFrq[RevFrqNo];

    if (DI_MSG_DATA == pDataLink->MsgType)
    {
        ptInfData->TimeStamp = pvInfData->TimeStamp%2880000 +2;
        ptInfData->SlotNum = uSndSlot[RevFrqNo];

        if (F_1 == SndFrq)
        {
            ptInfData->TxFreq1 = 1;
        }
        else
        {
            ptInfData->TxFreq2 = 1;
        }

        memcpy(&(ptInfData->tDataLink[SndFrq]), &(pvInfData->tDataLink[RevFrqNo]), sizeof(DATA_LINK_T));
        ODP_SendInfData(ptInfData, SapType);
        return;
    }

    if (DI_MSG_WLU == pDataLink->MsgType)
    {
        ptInfData->TimeStamp = pvInfData->TimeStamp%2880000 +2;
        ptInfData->SlotNum = uSndSlot[RevFrqNo];

        if (F_1 == SndFrq)
        {
            ptInfData->TxFreq1 = 1;
        }
        else
        {
            ptInfData->TxFreq2 = 1;
        }

        memcpy(&(ptInfData->tDataLink[SndFrq]), &(pvInfData->tDataLink[RevFrqNo]), sizeof(DATA_LINK_T));

        ODP_SendInfData(ptInfData, SapType);
        return;
    }

}


/**
 * @brief  生成GPS内嵌-精度
 *
 * @param [in]
 * @param [in]
 * @author  周大元
 * @since
 * @bug
 */
void GPS_ParseEmbLon(GPS_NAS_DATA_T *ptGpsData, UINT32 ulGpsEmb)
{
    char acbuf[30];
    int iGpsEmb;
    int iDegree, iMinLarge, iMinLittle;
    double dTmp;
    ptGpsData->EW = (((ulGpsEmb & (1<<LON_BIT)) >> LON_BIT) == EAST) ? EAST: WEST;
    ulGpsEmb &= (1<<24) -1;
    iGpsEmb = (int)ulGpsEmb;
    dTmp = (double)(iGpsEmb/LON_RATIO);  // 得出GPS:DD.DD
//    printf("[%s:%d] Lon: dTmp=%f\n", _F_, __LINE__, dTmp);
    iDegree = (int)dTmp;
    dTmp = (dTmp - iDegree)*60.0;
    iMinLarge = (int)dTmp;
    dTmp = (dTmp - iMinLarge)*10000.0;
    iMinLittle = (int)dTmp;
    ptGpsData->EDEG = iDegree;        //<8bit 经度     0~179
    ptGpsData->EMINmm = iMinLarge;    //<6bit 经分的整数部分 0~59
    ptGpsData->EMINF = iMinLittle;    //<14bit 经分的小数部分 0~9999
    // 暂时留着这个打印
    snprintf(acbuf, sizeof(acbuf), "%u.%u,%u", ptGpsData->EDEG, ptGpsData->EMINmm, ptGpsData->EMINF);
    printf("[DLL:%s:%d] Lon:%s, EW=[%u-%s], EDEG=%d, EMINmm=%d, EMINF=%d\n", _F_, __LINE__, acbuf, ptGpsData->EW, g_apcLonStr[ptGpsData->EW%2], ptGpsData->EDEG, ptGpsData->EMINmm, ptGpsData->EMINF);
}




/**
 * @brief  生成GPS内嵌-纬度
 *
 * @param [in]
 * @param [in]
 * @author  周大元
 * @since
 * @bug
 */
void GPS_ParseEmbLat(GPS_NAS_DATA_T *ptGpsData, UINT32 ulGpsEmb)
{
    char acbuf[30];
    int iGpsEmb;
    int iDegree, iMinLarge, iMinLittle;
    double dTmp;
    ptGpsData->NS = (((ulGpsEmb & (1<<LAT_BIT)) >> LAT_BIT) == NORTH) ? NORTH: SOUTH;
    ulGpsEmb &= (1<<LAT_BIT) -1;
    iGpsEmb = (int)ulGpsEmb;
    dTmp = (double)(iGpsEmb/LAT_RATIO);  // 得出GPS:DD.DD
//    printf("[%s:%d] Lat: dTmp=%f\n", _F_, __LINE__, dTmp);
    iDegree = (int)dTmp;
    dTmp = (dTmp - iDegree)*60.0;
    iMinLarge = (int)dTmp;
    dTmp = (dTmp - iMinLarge)*10000.0;
    iMinLittle = (int)dTmp;
    ptGpsData->NDEG = iDegree;      //<7bit 纬度    0~89
    ptGpsData->NMINmm = iMinLarge;  //<6bit 纬分的整数部分 0~59
    ptGpsData->NMINF = iMinLittle;  //<14bit 纬分的小数部分 0~9999
    snprintf(acbuf, sizeof(acbuf), "%u.%u,%u", ptGpsData->NDEG, ptGpsData->NMINmm, ptGpsData->NMINF);
    printf("[DLL:%s:%d] Lat:%s, NS=[%u-%s], NDEG=%d, NMINmm=%d, NMINF=%d\n", _F_, __LINE__, acbuf, ptGpsData->NS, g_apcLatStr[ptGpsData->NS%2], ptGpsData->NDEG, ptGpsData->NMINmm, ptGpsData->NMINF);
}



/**
 * @brief  生成GPS内嵌
 *
 * @param [in]
 * @param [in]
 * @author  周大元
 * @since
 * @bug
 */
void GPS_ParseEmb_1(unsigned long ulEmbLon, unsigned long ulEmbLat, GPS_NAS_DATA_T *ptGpsNas)
{
    GPS_ParseEmbLon(ptGpsNas, ulEmbLon); // 解内嵌经度
    GPS_ParseEmbLat(ptGpsNas, ulEmbLat); // 解内嵌纬度
}

/**
 * @brief  生成GPS内嵌
 *
 * @param [OUT] ptGpsNas - GPS信息
 * @param [in]  ptLcEmb  - 语音内嵌，通过该值计算出GPS信息
 *                       - ptLcEmb ==> ptGpsNas
 * @author  周大元
 * @since
 * @bug
 */
void GPS_ParseEmb(EMBBED_GPS_PDU *ptLcEmb, GPS_NAS_DATA_T *ptGpsNas)
{
    UINT32 ulEmbLon, ulEmbLat;
    ulEmbLon = ptLcEmb->uLongitude[0] << 16 | ptLcEmb->uLongitude[1] << 8 | ptLcEmb->uLongitude[2] | ptLcEmb->uEW << 24;  // 获取经度 bit
    ulEmbLat = ptLcEmb->uLatitude[0]  << 16 | ptLcEmb->uLatitude[1]  << 8 | ptLcEmb->uLatitude[2];  // 获取纬度 bit
    GPS_ParseEmbLon(ptGpsNas, ulEmbLon);  // 解析内嵌经度
    GPS_ParseEmbLat(ptGpsNas, ulEmbLat);  // 解析内嵌纬度

}


/**
 * @brief  接口适配层上行语音处理
 *
 * @param [in] pvInfData        INF上行数据
 * @param [in] RevFrqNo        接收频率号
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void INFVoiceProcess(NAS_INF_UL_T *pvInfData, UINT8 RevFrqNo)
{
    UINT16 ReStatus;
    UINT16 KeyResult;
    UINT8  uAlg_ID;
    UINT8  uKey_ID;
    UINT8  Lc_Tmp[12]={0};
    DATA_LINK_T *ptDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    EMBBED_GPS_PDU *ptEmbData = NULL;
    NAS_PRE_LC_PDU *ptLcData = NULL;
    UINT8  uRet=NO_ERR;
    UINT8  CheckId[27] = {0};
    BOOL   ret=TRUE;
    /*
    ** 包长度判断
    */ 
    if (ptDataLink->DataLen != 27)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s][%d] voice datalen:%d error", _F_, __LINE__, ptDataLink->DataLen);
        return;
    }
    /*
    ** 色码判断
    */ 
    if (g_DllGlobalCfg.auMSCC != ptDataLink->CC)
    {
        LOG_WARNING(s_LogMsgId, "[DLL][%s][%d] voice CC: %d error", _F_,  __LINE__, ptDataLink->CC);
        /* 
        ** g_bCcRightFrmRcvFlg - 当收到色码不一样的语音帧时，
        **                       当 g_bCcRightFrmRcvFlg=0 是首次， 则丢弃该帧，
        **                       当 g_bCcRightFrmRcvFlg=1 说明是中间帧， 则需要记录错误的次数， 再进行判断
        ** g_iCcErrSuperFrmCnt - 错误了多少个超帧, 预设值 [tDllPrint->CcErrSuperFrmCnt] 与其对比。
        ** 
        ** 如果是非首次，即收到干扰帧，如果小于预设定帧数，认为普通干扰不做处理，如果连续出现次数大于预设定数，则丢弃，
        */
        if (!g_bCcRightFrmRcvFlg || (tDllPrint->CcErrSuperFrmCnt <= g_iCcErrSuperFrmCnt))
        {
            LOG_WARNING(s_LogMsgId, "[DLL][%s][%d] voice CC: %d error abandon: ERR_COUNT:%d, SUPERFRM_COUNT:%d, g_bCcRightFrmRcvFlg=%d", 
                    _F_, __LINE__, ptDataLink->CC, g_iCcErrFrmCnt, g_iCcErrSuperFrmCnt, g_bCcRightFrmRcvFlg);
            return;
        }
        else
        {
            g_iCcErrFrmCnt ++;
            if (6 == g_iCcErrFrmCnt)
            {
                g_iCcErrSuperFrmCnt++;
                g_iCcErrFrmCnt = 0;
            }
            LOG_WARNING(s_LogMsgId,"[DLL][%s][%d] voice CC: %d error  ERR_COUNT:%d SUPERFRM_COUNT:%d", _F_, __LINE__, ptDataLink->CC, g_iCcErrFrmCnt, g_iCcErrSuperFrmCnt);
        }
    }
    else
    {
        g_iCcErrFrmCnt = 0;
        g_iCcErrSuperFrmCnt = 0;
    }
    g_bCcRightFrmRcvFlg = 1;

    // 语音误码率测试-调试用-正常情况不执行
    if (ptErrPrint->Voice == 1)
    {
        ErrRateVoiceTest(ptDataLink, SET_ERR_RCV);  // 设置接收语音帧(FRM-A...F)
    }

    if (g_DllGlobalCfg.auPI == 1)  // 有语音上行重新定时计数清0
    {
        uWaitVoiceTimes=0;
        if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA)
        {
            memcpy(dmra_voice_tmp+27*(ptDataLink->FrmType-1), ptDataLink->PayLoad, 27);
        }
    }

    switch (ptDataLink->FrmType)
    {
        case FT_VOICE_A:
        {
            memset(&Later_PI_temp,0x00,sizeof(DMRA_PI_PDU));
            uRet=GenVoiceCclData(ptCclData, ptDataLink);
            if(!uRet)
            {
                IDP_SendCclData(ptCclData);
            }
            break;
        }
        case FT_VOICE_B:
        case FT_VOICE_C:
        case FT_VOICE_D:
        case FT_VOICE_E:
        {
            if (1 == ptDataLink->LCSS)
            {
                //B
                memcpy(&g_DllGlobalCfg.auEmbInfo[0], ptDataLink->EmbInfo, 4);

            }
            else if (2 == ptDataLink->LCSS)
            {
                //E
                memcpy(&g_DllGlobalCfg.auEmbInfo[12], ptDataLink->EmbInfo, 4);
            }
            else if (3 == ptDataLink->LCSS)
            {
                //CD
                if (3 == auLastLcss)
                {
                    // 当第二次进入时，auLastLcss=3,  所以该位置为 D
                    memcpy(&g_DllGlobalCfg.auEmbInfo[8], ptDataLink->EmbInfo, 4);
                    auLastPiFlag=auPiFlag;
                }
                else if (1 == auLastLcss)
                {
                    // 当第一次进入时，auLastLcss=1,  所以该位置为 C
                    memcpy(&g_DllGlobalCfg.auEmbInfo[4], ptDataLink->EmbInfo, 4);
                }
                else
                {
                    ; // do nothing
                }
            }
            else
            {
                ; // do nothing
            }

            auLastLcss = ptDataLink->LCSS;

            if (FT_VOICE_E == ptDataLink->FrmType)
            {
                EmbeddedDec(g_DllGlobalCfg.auEmbInfo, Lc_Tmp, &ReStatus);  // auEmbInfo ==> Lc_Tmp
                if (NO_ERR != ReStatus)
                {
                    LOG_ERROR(s_LogMsgId,"[DLL][%s] Voice EmbeddedDec error,PI=%d",_F_,ptDataLink->PI);//不处理
                }
                else
                {
                    if(ptDataLink->PI==1)//基本加密PI值当成0处理,高级加密进入此分支
                    {
                        if(g_DllGlobalCfg.auPI==1)
                        {
                            if(auPiFlag==0)//未初始化加密向量
                            {
                                if(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_ENHA)//Hytera高级加密
                                {
                                    //初始化加密向量函数
                                    DLL_ENCLIB_VOICE Encryp_str;
                                    HYTERA_PI_PDU_PDT *Later_PI=(HYTERA_PI_PDU_PDT*)Encryp_str.PIInfo;
                                    memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
                                    memcpy(Encryp_str.PIInfo,Lc_Tmp,FLC_LEN);
                                    Later_PI->uChechSum=_PI_CheckSum(&Encryp_str);
                                    Encryp_str.FrameNum = VOICE_PI_FRAME;
                                    Encryp_str.ValidFlag = VALID;
                                    DecryptFunc_Voice(&Encryp_str);//PI调用解码初始化向量函数
                                    auPiFlag=1;
                                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][Hytera Hight Encryption] Not Rcv PI_Head,Enter Later",_F_);
                                }
                                else//基本加密
                                {
                                    LOG_ERROR(s_LogMsgId,"[DLL][%s] Rcv Voice_Emb_PI, But NAS is not Hight Encryption",_F_);
                                }
                            }
                            else
                            {
                                //已初始化过加密向量，不做处理
                            }
                        }
                        else
                        {
                            LOG_ERROR(s_LogMsgId,"[DLL][%s] Rcv Voice_Emb_PI, But NAS is not Encryption",_F_);
                        }
                    }
                    else//处理LC
                    {
                        ptEmbData = (EMBBED_GPS_PDU *)Lc_Tmp;
                        if (FLCO_HDR == ptEmbData->uFLCO)     // lc 数据
                        {
                            ptLcData = (NAS_PRE_LC_PDU *)Lc_Tmp;
                            //  不相等的情景:  迟入或没有收到 LC_HEADER
                            if (0 != memcmp((char *)g_DllGlobalCfg.auUploadLC, (char *)Lc_Tmp, FLC_LEN))
                            {
                                if (ptErrPrint->Voice == 1)
                                {
                                    ErrRateVoiceTest(NULL, SET_ERR_HDR);
                                }
                                memcpy(g_DllGlobalCfg.auUploadLC, Lc_Tmp, FLC_LEN);
                                LOG_WARNING(s_LogMsgId,"[DLL][%s] Lc change!!! Update Lc Info: ",_F_);
                                LOG_PrintM(s_LogMsgId, g_DllGlobalCfg.auUploadLC, FLC_LEN);
                                memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                                ptCclData->MsgType = DI_MSG_DATA;
                                ptCclData->FrmType = FT_VOICE_NO;
                                ptCclData->DataType = CT_LC_HEADER;
                                ptCclData->DataLen = FLC_LEN;
                                ptCclData->Vari[0] = ptDataLink->Vari[0];
                                ptCclData->Vari[1] = ptDataLink->Vari[1];
                                ptCclData->Rssi[0] = ptDataLink->Rssi[0];
                                ptCclData->Rssi[1] = ptDataLink->Rssi[1];
                                ptCclData->Prohibit = ptDataLink->Prohibit;
                                memcpy(ptCclData->SrcId, ptLcData->auSADDR, 3);
                                memcpy(ptCclData->DstId, ptLcData->auTADDR, 3);
                                memcpy(ptCclData->PayLoad, Lc_Tmp, FLC_LEN);
                                EmbeddedEnc(g_DllGlobalCfg.auUploadLC, g_DllGlobalCfg.auEmbInfo);  // auUploadLC => auEmbInfo
                                memcpy(p_DllFpgaShm->EmbInfo, g_DllGlobalCfg.auEmbInfo, 16);
                                g_iCcErrFrmCnt = 0;
                                g_iCcErrSuperFrmCnt = 0;
                                IDP_SendCclData(ptCclData);
                            }
                        }
                        else if (FLCO_GPS == ptEmbData->uFLCO)  // GPS数据
                        {
                            /*
                            ** 1-GPS 内嵌中没有ID
                            ** 2-暂时不判定GPS 是否锁定成功， 即 p_DllFpgaShm->GpsFlag
                            */
                            GPS_NAS_DATA_T tGpsSnd = {0};  // GPS信息-内嵌-DD,FF,FFFF
                            GPS_NAS_DATA_T tGpsRcv = {0};   // GPS信息-自己-DD,FF,FFFF
                            GPS_DEV_DATA_T *ptGpsShm = (GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData);  // 共享内存中本地的GPS信息
                            GPS_ParseEmb(ptEmbData, &tGpsSnd);   // 获取语音内嵌中的GPS, ptEmbData  ==> tGpsSnd
                            IDP_GpsDevToNas(ptGpsShm, &tGpsRcv);  // 获取本地GPS信息, ptGpsShm ==> tGpsRcv

                            //---组包---
                            memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));
                            ptCclData->MsgType = DI_MSG_DATA;
                            ptCclData->FrmType = FT_VOICE_NO;
                            ptCclData->DataType = CT_VOICE_EMB_GPS;
                            ptCclData->DataLen = GPS_LEN + GPS_LEN;  // 两段GPS信息(1-语音内嵌GPS, 2-本地GPS)
                            ptCclData->Vari[0] = ptDataLink->Vari[0];
                            ptCclData->Vari[1] = ptDataLink->Vari[1];
                            ptCclData->Rssi[0] = ptDataLink->Rssi[0];
                            ptCclData->Rssi[1] = ptDataLink->Rssi[1];
                            ptCclData->Prohibit = ptDataLink->Prohibit;

                            if (ptHdrID->ucEnableFlg == 1)  // 是否在 Header 接收到相关ID
                            {
                                memcpy(ptCclData->DstId, ptHdrID->aucCalledID, sizeof(ptHdrID->aucCalledID));    // 被叫ID, 组号
                                memcpy(ptCclData->SrcId, ptHdrID->aucCallingID, sizeof(ptHdrID->aucCallingID));  // 主叫ID，Nas/Ms
                            }

                            memcpy(ptCclData->PayLoad, &tGpsSnd, GPS_LEN);  //  拷贝发射端GPS, 即从语音中解出的GPS信息
                            memcpy(ptCclData->PayLoad + sizeof(GPS_NAS_DATA_T), &tGpsRcv, GPS_LEN);  // 拷贝发接收端GPS, 即自己本地的GPS信息
                            IDP_SendCclData(ptCclData);
                        }
                        else
                        {
                            if (1 == tDllPrint->AIUp)
                            {
                                LOG_DEBUG(s_LogMsgId,"[DLL][%s] Lc uFLCO unknow: %d",_F_, ptEmbData->uFLCO);
                            }
                        }
                    }
                }
            }

            uRet=GenVoiceCclData(ptCclData, ptDataLink);
            if(!uRet)
            {
                IDP_SendCclData(ptCclData);
            }
            break;
        }
        case FT_VOICE_F:
        {
            if((auPiFlag==0)&&(g_DllGlobalCfg.auEncryption_Type==ENCRYPT_DMRA)&&(0 != memcmp((char *)dmra_voice_tmp, (char *)CheckId, VOICE_LEN)))
            {
                //初始化加密向量函数
                rc_dec((UINT16 *)ptDataLink->EmbInfo,&KeyResult);

                uAlg_ID=(UINT8)(KeyResult & 0x0003);
                uKey_ID=(UINT8)((KeyResult>>3) & 0x00ff);
                LOG_WARNING(s_LogMsgId,"[DLL][%s] uAlg_ID=%d,uKey_ID=%d,KeyResult=%d ",_F_,uAlg_ID,uKey_ID,KeyResult);
                Later_PI_temp.uAlgId=uAlg_ID;
                Later_PI_temp.uKeyID=uKey_ID;
                Later_PI_temp.uGI=1;
                Later_PI_temp.uFID=MFID;

                ret = _DMRA_GetParam(Later_PI_temp.auIV,Later_PI_temp.uAlgId);
                if(ret)//语音提取偷比特成功
                {
                    DLL_ENCLIB_VOICE Encryp_str;
                    memset(&Encryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
                    memcpy(Encryp_str.PIInfo,&Later_PI_temp,PI_LEN);
                    Encryp_str.FrameNum = VOICE_PI_FRAME;
                    Encryp_str.ValidFlag = VALID;
                    DecryptFunc_Voice(&Encryp_str);//PI调用解码初始化向量函数
                    auPiFlag=1;
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][DMRA Hight Encryption] Not Rcv PI_Head,Enter Later",_F_);
                }
                else
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] [DMRA] Steal Bit 72bit to 32bit error ", _F_);
                }
            }
            memset(dmra_voice_tmp,0x00,6*VOICE_LEN);

            uRet=GenVoiceCclData(ptCclData, ptDataLink);
            if(!uRet)
            {
                IDP_SendCclData(ptCclData);
            }
            if(g_DllGlobalCfg.auPI==1)
            {
                DLL_SetTimer(CALL_VOICE_U,360);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}


/**
 * @brief   接口适配层上行数据处理
 *
 * @param [in] pvInfData       INF上行数据
 * @param [in] RevFrqNo       接收频率号
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void INFDataProcess(NAS_INF_UL_T *pvInfData, UINT8 RevFrqNo)
{
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);

    if (pDataLink->DataLen > 33)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] data datalen: %d error",_F_, pDataLink->DataLen);
        return;
    }
    if (g_DllGlobalCfg.auMSCC != pDataLink->CC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] data CC: %d error",_F_, pDataLink->CC);
        return;
    }
//    IDP_PrintInfDataLog(pDataLink);  // by zhoudayuan
    switch (pDataLink->DataType)
    {
        case DT_PI_HEADER:
        {

            if ((g_DllGlobalCfg.auPI == 0) || (g_DllGlobalCfg.auEncryption_Type==ENCRYPT_HYT_BASE))
            {
                LOG_ERROR(s_LogMsgId,"[DLL][%s] Rcv PI_Head.But Epack is not Encryption Mode or not Higt Encryption",_F_);
                break;
            }
            else
            {
                IDP_PiHeaderFun(pvInfData, RevFrqNo);
                auLastPiFlag=1;
            }
            break;
        }
        case DT_LC_HEADER:
        {
            if (ptErrPrint->Voice == 1)
            {
                ErrRateVoiceTest(NULL, SET_ERR_HDR);
            }
            auPiFlag=0;
            auLastPiFlag=0;
            g_BurstCnt = 0;
            g_bCcRightFrmRcvFlg = 0;
            g_iCcErrFrmCnt = 0;
            g_iCcErrSuperFrmCnt = 0;
            IDP_LcHeaderFun(pvInfData, RevFrqNo);
            DLL_ClearTimer();
            break;
        }
        case DT_LC_TERMINATOR:
        {
            auPiFlag=0;
            auLastPiFlag=0;
            IDP_LcTerminatorFun(pvInfData, RevFrqNo);
            break;
        }
        case DT_CSBK:
        {
            IDP_MsCSBKFun(pvInfData, RevFrqNo);
            break;
        }
        case DT_MBC_HEADER:
        case DT_MBC_CONTINUE:
        {
            break;
        }
        case DT_DATA_HEADER:
        {
            IDP_DataHeaderFun(pvInfData, RevFrqNo);
            break;
        }
        case DT_R_1_2_DATA:
        {
            IDP_R12PackDataFun(pvInfData, RevFrqNo);
            break;
        }
        case DT_R_3_4_DATA:
        {
            IDP_R34PackDataFun(pvInfData, RevFrqNo);
            break;
        }
        case DT_R_1_1_DATA:
        case DT_IDLE:
        {
            break;
        }
        default:
        {
            LOG_WARNING(s_LogMsgId,"[DLL][%s] Data Type error: %d",_F_, pDataLink->DataType);
            break;
        }
    }

}


/**
 * @brief  接口适配层上行邻点处理
 *
 * @param [in] pvInfData       INF上行数据
 * @param [in] RevFrqNo       接收频率号
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void INFNearDataProcess(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{
    UINT16 u2CRC;
    UINT16 AirCRC = 0;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);

    NAS_NEGR_BURST_T *pNegrBurst = (NAS_NEGR_BURST_T *)(pDataLink->PayLoad);

    if (pDataLink->DataLen > 27)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr_data datalen: %d error", _F_, pDataLink->DataLen);
        return;
    }
    else if (g_DllGlobalCfg.auWLUCC != pDataLink->CC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr_data CC: %d error", _F_, pDataLink->CC);
        return;
    }
    else if (DI_MSG_NEAR != pDataLink->DataType)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr_data type: %d error", _F_, pDataLink->DataType);
        return;
    }

    u2CRC = ALG_Crc16(pDataLink->PayLoad, CSBK_LEN);

        u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;

    AirCRC = ((UINT16)(pNegrBurst->Crc[0]) << 8) | pNegrBurst->Crc[1];

    if (u2CRC != AirCRC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr_data CRC error", _F_);
        return;
    }

    if (pNegrBurst->NodeId > DEV_ID_32)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr_data NodeId error", _F_);
        return;
    }

    if (pNegrBurst->NodeId == g_DllGlobalCfg.auNodeId)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Negr NodeId equal local Err", _F_);
        return;
    }

    g_DllGlobalCfg.auNegrId1 = g_DllGlobalCfg.auNegrId1 | (0x0001 << pNegrBurst->NodeId);  // auNegrId1每个bit代表相应邻点是否存在标志
    g_DllGlobalCfg.auNerInfo1[pNegrBurst->NodeId] = pNegrBurst->NegrId;  // 数据索引代表相应索引, 索应对饮的32bit数组元素即为该借点的邻点信息

    if ((ptCFGShm->start_neighbor.val == 1) && (g_DllGlobalCfg.auNegrId1 != 0))
    {
        g_DisconCnt = 0;
        if (g_DisconRecoverPrintFlg < 3)
        {
            g_DisconRecoverPrintFlg++;
            LOG_DEBUG(s_LogMsgId, "[DLL][%s] Discon Recover",  _F_);
        }
        SetDisconAlarm(TURN_OFF);
    }
    return;
}


/**
 * @brief  接口适配层上行链路数据处理
 *
 * @param [in] pvInfData       INF上行数据
 * @param [in] RevFrqNo       接收频率号
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void INFNasProcess(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo)
{

    UINT8 CheckSum;
    DATA_LINK_T *pDataLink = &(pvInfData->tDataLink[RevFrqNo]);
    NAS_AI_PAYLOAD *pNasAiData = (NAS_AI_PAYLOAD *)(pDataLink->PayLoad);

    if (pDataLink->DataLen > 27)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas datalen: %d err", _F_, pDataLink->DataLen);
        return;
    }
    else if (g_DllGlobalCfg.auWLUCC != pDataLink->CC)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas data CC: %d err", _F_, pDataLink->CC);
        return;
    }
    else if (DI_MSG_WLU != pDataLink->DataType)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas data type: %d err", _F_, pDataLink->DataType);
        return;
    }
    else if (NM_TYPE_CENTER != pNasAiData->nm_type)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas data nm_type err: %d", _F_, pNasAiData->nm_type);
        return;
    }

    CheckSum = ALG_Crc8((UINT8 *)pNasAiData, NM_DATA_LEN);
    if (CheckSum != pNasAiData->crc)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas data crc check fail", _F_);
        return;
    }

    /*
    ** check 处理方式
    ** 广播: dst = src = 31
    ** 转发: dst != src
    */
    //MS业务产生的广播: 邻点上报、MS_GPS_ACK、MS_KILL_ACK   MS_ENABLE_ACK
    if (pNasAiData->dst_id == pNasAiData->src_id && 31 == pNasAiData->dst_id)  // 广播
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] Nas data broadcast", _F_);
        IDP_RelayLinkData(pvInfData, RevFrqNo, S_NAS_RELAY);
    }
    else if (pNasAiData->dst_id != g_DllGlobalCfg.auNodeId)  // 转发
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas data not local(DstId:%d)", _F_, pNasAiData->dst_id);
        IDP_RelayLinkData(pvInfData, RevFrqNo, S_NAS_RELAY);
        return;
    }

    /*
    ** check 异常
    ** 异常: src = Node
    */
    if (pNasAiData->src_id == g_DllGlobalCfg.auNodeId)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] Nas DstId=SrcId=NodeId=%d , abandon !", _F_, g_DllGlobalCfg.auNodeId);
        return;
    }

    // 断链告警恢复
    if ((ptCFGShm->start_neighbor.val == 1) && (g_discon_state == DISCON_HAPPEN))
    {
        SetDisconAlarm(TURN_OFF);
    }


    switch (pNasAiData->cmd_code)
    {
        //本机节点有关的链路机消息
        case CMD_CODE_GPS_REQUEST:
        {
            IDP_NasGpsReportFun(pNasAiData);
            break;
        }
        case CMD_CODE_STUN:
        {
            IDP_NasStunFun(pNasAiData);
            break;
        }
        case CMD_CODE_KILL:
        {
            IDP_NasKillFun(pNasAiData);
            break;
        }
        case CMD_CODE_ENABLE:
        {
            IDP_NasEnableFun(pNasAiData);
            break;
        }
        case CMD_CODE_NER_QUERY: // 邻点查询
        {
            IDP_NasNeighborQueryFun(pNasAiData);
            break;
        }
        //本机节点有关的链路机消息结束

        //广播消息开始
        case CMO_CODE_NER_REPORT:  // 邻点上报-主动
        {
            IDP_NasNegrReportFun(pNasAiData);
            break;
        }
        case CMO_CODE_MS_GPS:
        {
            IDP_NasMsGpsFun(pNasAiData);
            break;
        }
        case CMO_CODE_MS_DISABlE:
        {
            IDP_NasMsDisableFun(pNasAiData);
            break;
        }
        case CMO_CODE_MS_ENABLE:
        {
            IDP_NasMsEnableFun(pNasAiData);
            break;
        }
        //广播消息结束
        //对本节点操作的预载波只做打印，对其他点操作的NAS预载波前面已转发
        case CMO_CODE_NAS_PRE:
        {
            IDP_NasPre(pNasAiData);
            break;
        }
        default:
        {
            break;
        }
    }
}


/**
 * @brief   接口适配层上行线程
 *
 * @param [in] p       传递给线程start_routine的参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void *INFUploadTask(void * p)
{
    int RetSize;
    int aiAiRcvBuf[INF_RECV_MAX];  // 数组-整形-空口-接收-Buf
    int ret;
    UINT8 Frq;
    NAS_INF_UL_T *pvInfData;

    ptCclData = new DLL_CCL_UL_T;
    memset(ptCclData, 0, sizeof(DLL_CCL_UL_T));

    fd_set fdsr;
    struct timeval tv;

    while(1)
    {
        FD_ZERO(&fdsr);//将字符集清空
        FD_SET(INFSocket, &fdsr);//将所要监控的socket 加入到fdsr集合中，将集合对应的比特位置1.

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select((INFSocket+1), &fdsr, NULL, NULL, &tv);//监控集合中置1比特位的socket.  其中范围在 0-socket+1范围内
                                                            //如果fdsr集合为0的话，select会返回-1
                                                            //如果tv时间内超时，则集合清0，返回ret=0
                                                            //如果tv时间内接收到数据，则返回正数，当返回为正数时，表示已经准备好的描述符

        /*select讲解

        10         FD_ZERO(&readfds);
        11         FD_SET(fd,&readfds);
        12         tv.tv_sec=sec;
        13         tv.tv_usec=usec;
        14         select(fd+1,&readfds,NULL,NULL,&tv);
        15         if(FD_ISSET(fd,&readfds))
        16         {
        17             if((n=recvfrom(fd,buf,buf_n,0,addr,len))>=0)

        第10行将集合readfds清零，

        第11行将我们关注的sock加入集合readfds中（置fd对应的bit为1），

        第12和13行设置超时参数，

        第14行以非阻塞的方式调用select，如果tv时间内有数据则返回并设置readfds中fd对应的bit位为1，如果tv时间内没有数据则返回并设置readfds中对应的bit位为0；

        第15行FD_ISSET测试readfds中fd位有没有置1，如果置一则返回成功，否则失败

        这里要强调两点：

        第一：如果tv时间内没有数据到来，你还想继续等待N次，那么一定要注意重新设置readfds，因为它已经被select破坏了，如果不重新设置的话，你的select语句会返回-1，strerr时会打印出参数设置出错，主要是由于readfds中全部为零，select不知道该去监视哪个sock；

        第二：重复等待时不光要注意重新设置readfds，同时还要注意重新设置一下tv的值，因为select同时也破坏了tv的值（select在返回时会改变tv，改变的公式是tv=tv-等待的时间，所以如果tv时间内没有数据到达的话，select返回时tv会变成0）。


        select()调用返回处于就绪状态并且已经包含在fd_set结构中的描述字总数；如果超时则返回0；否则的话，返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError获取相应错误代码。
        当返回为-1时，所有描述符集清0。
        当返回为0时，表示超时。
        当返回为正数时，表示已经准备好的描述符数。
        select（）返回后，在3个描述符集里，依旧是1的位就是准备好的描述符。这也就是为什么，每次用select后都要用FD_ISSET的原因。

        */

        if (ret < 0)
        {
            LOG_ERROR(s_LogMsgId,"[DLL][%s] select error", _F_);
            continue;
        }
        else if (ret == 0)//timeout
        {
            continue;
        }

        if (FD_ISSET(INFSocket, &fdsr))
        {
            RetSize = recvfrom(INFSocket, aiAiRcvBuf, INF_RECV_MAX, 0, (struct sockaddr *)(&RecvAddr), &AddrSize);
            if (RetSize < 0)
            {
                LOG_ERROR(s_LogMsgId,"[DLL][%s] recvfrom <0 fail", _F_);
                continue;
            }

            if(sizeof(NAS_INF_UL_T) != RetSize)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] recvfrom size error: size_%d", _F_, RetSize);
                continue;
            }

            pvInfData = (NAS_INF_UL_T *)aiAiRcvBuf;

            if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)  // 是否摇晕 or 摇毙
            {
                if (0 > EnableNasGrep(pvInfData))  // 遥晕遥毙的状态下只能处理NAS使能遥晕遥毙操作码
                {
                    LOG_WARNING(s_LogMsgId,"[DLL][%s] Nas is Stuned or Killed", _F_);
                    continue;
                }
            }

            if (tDllPrint->AIUp == 1)
            {
                IDP_AiPrintf(pvInfData);  // 看打印
            }

            if (S_2 < pvInfData->SlotNum)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] slot: %x error", _F_, pvInfData->SlotNum);
                continue;
            }

            if (0 == pvInfData->RxFreq1 && 0 == pvInfData->RxFreq2)
            {
                LOG_WARNING(s_LogMsgId,"[DLL][%s] Refreq err no valid", _F_);
                continue;
            }

            if (1 == pvInfData->RxFreq1)
            {
                Frq = F_1;
                IDP_ReptFreqSlot(pvInfData->SlotNum, Frq);

                switch (pvInfData->tDataLink[Frq].MsgType)
                {
                    case DI_MSG_VOICE:  // 语音业务
                    {
                        g_BurstCnt = 0;
                        INFVoiceProcess(pvInfData, Frq);
                        break;
                    }
                    case DI_MSG_DATA:  // 数据业务
                    {
                        INFDataProcess(pvInfData, Frq);
                        break;
                    }
                    case DI_MSG_NEAR:  // 邻点业务 [突发-接收-其他NAS]
                    {
                        INFNearDataProcess(pvInfData, Frq);  // 1-收集邻点信息; 2-断链告警相关
                        break;
                    }
                    case DI_MSG_WLU:  // 链路机业务
                    {
                        INFNasProcess(pvInfData, Frq);
                        break;
                    }
                    default:
                    {
                        LOG_WARNING(s_LogMsgId,"[DLL][%s] MsgType error", _F_);
                        break;
                    }
                }
            }

            if (1 == pvInfData->RxFreq2)
            {
                Frq = F_2;
                IDP_ReptFreqSlot(pvInfData->SlotNum, Frq);

                switch (pvInfData->tDataLink[Frq].MsgType)
                {
                    case DI_MSG_VOICE:
                    {
                        g_BurstCnt = 0;
                        INFVoiceProcess(pvInfData, Frq);
                        break;
                    }
                    case DI_MSG_DATA:
                    {
                        INFDataProcess(pvInfData, Frq);
                        break;
                    }
                    case DI_MSG_NEAR:
                    {
                        INFNearDataProcess(pvInfData, Frq);
                        break;
                    }
                    case DI_MSG_WLU:
                    {
                        INFNasProcess(pvInfData, Frq);
                        break;
                    }
                    default:
                    {
                        LOG_WARNING(s_LogMsgId,"[DLL][%s] MsgType error", _F_);
                        break;
                    }
                }

            }

        }
    }

    delete(ptCclData);
    pthread_exit(NULL);

}


