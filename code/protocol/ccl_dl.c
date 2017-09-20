/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_dl.c
 * @brief   呼叫控制层下行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-29
 */

/*
 *   函数列表
 *   1. ODP_GenSilentFrm         封装CCL下行静音帧数据包
 *   2. ODP_GenVoicePacket             封装CCL下行语音包
 *   3. ODP_GenSmsDataPacket           封装CCL层下行短信帧
 *   4. ODP_GenLcHeader                封装语音LC头
 *   5. ODP_GenRemoteCtrlDataPacket    封装下行控制命令数据包
 *   6. ODP_GenTerminatorPacket        封装语音结束帧terminator数据包
 *   7.
 *
 *   修改历史
 *   2016-07-29            建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "ccl_dl.h"
#include "ccl.h"
#include "dll.h"
#include "ccl_fun.h"
#include "print_debug.h"










/******************************************************************************
 *   类型定义
 *   *************************************************************************/
/**
 * @var g_DllGlobalCfg
 * @brief 全局配置项
 */
extern DLL_GLB_CFG_T g_DllGlobalCfg;
extern unsigned int  s_LogMsgId;
extern CCL_PRINT_T * tcclPrint;
/**
 * @var  pLogTxtFd
 * @brief 文件句柄
 */
extern FILE *pLogFd;

/**
 * @var ptIPCShm
 * @brief 共享内存系统信息
 */
SMS_CCL_DLL_EN *SMS_CCL_DLL=NULL;
extern SHM_IPC_STRU *ptIPCShm;
extern  unsigned  char   CCL_DL_PRINT;
extern unsigned char g_CclState;
//extern unsigned char s_aCCcvNet[5];
unsigned  int CallId;




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
* @brief   将从中心收到的短信数据封装成CCL层下行短信帧
* @param [in] pvCenterData  中心短信数据包句柄
* @param [out] pvDllData  下行到链路层 短信数据包句柄
* @param [out] Len  下行到链路层 短信数据长度指针
* @author 牛功喜
* @since
* @bug
*/

void ODP_GenSmsDataPacket(unsigned  char * pvCenterData, unsigned  char*pvDllData, int *Len)
{
    SMS_CENTER_CCL_DL *   ptCenterData =(SMS_CENTER_CCL_DL *) ( pvCenterData);
    DLL_CCL_UL_T *  ptDllData =( DLL_CCL_UL_T *)( pvDllData);
    ptDllData->DataLen=ptCenterData->ValidLength+CCL_SMS_DL_HEADLEN;               //   负载 长度 =短消息压缩头+短消息有效数据
    SMS_CCL_DLL_EN  *  SmsToDll  =(SMS_CCL_DLL_EN*) (ptDllData->PayLoad);
    SmsToDll->Identification++;
    SmsToDll->SrcPortId=0x01;
    SmsToDll->DestPortId=0x01;
    memcpy(SmsToDll->SmsPayLoad,ptCenterData->SmsData,ptCenterData->ValidLength);
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] sms  valide len=%d", __FUNCTION__,ptCenterData->ValidLength);
    *Len= ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
}
/**
* @brief   封装语音LC头
* @param [in] pvDllData 下行到链路层数据指针
* @param [out] Len  下行到链路层数据长度指针
* @author 牛功喜
* @since
* @bug
*/
void ODP_GenLcHeader(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len)
{
    DLL_CCL_UL_T * ptDllData = (DLL_CCL_UL_T *)( pvDllData);
    DT_VOICE_LC_HEADER * ptDllPayLoad = (DT_VOICE_LC_HEADER *)ptDllData->PayLoad;
    PTT_CMD *ptCenterData=(PTT_CMD *)pvCenterData;


    ptDllData->MsgType  = DI_MSG_DATA;
    ptDllData->DataType = CT_LC_HEADER;
    ptDllData->FrmType  = FT_VOICE_NO;
    ptDllData->DataLen  = LC_HEADER_LEN;  // 负载长度=语音头LC 长度
    *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
    // PayLoad-Full Link Control
    ptDllPayLoad->Reserved = 0;
    ptDllPayLoad->ProtectFlg = 0;
    ptDllPayLoad->CtrOpcode = 0x00;  // FLCO: 0X00代表起始帧OR结束帧
    ptDllPayLoad->FeatureId = 0x00;
    ptDllPayLoad->ServiceOpt = 0x00;  
 


     





    ptDllPayLoad->GroupAddr[0] = ptCenterData->CalledNum[0];
    ptDllPayLoad->GroupAddr[1] = ptCenterData->CalledNum[1];
    ptDllPayLoad->GroupAddr[2] = ptCenterData->CalledNum[2];
    ptDllPayLoad->SourceAddr[0] = ptCenterData->CallingNum[0];
    ptDllPayLoad->SourceAddr[1] = ptCenterData->CallingNum[1];
    ptDllPayLoad->SourceAddr[2] = ptCenterData->CallingNum[2];
    ptDllData->SrcId[0] = ptCenterData->CallingNum[0];  //ptCenterData->SenderNum[0];// 添加源
    ptDllData->SrcId[1] = ptCenterData->CallingNum[1];  //ptCenterData->SenderNum[1];// 添加源
    ptDllData->SrcId[2] = ptCenterData->CallingNum[2];  //ptCenterData->SenderNum[2];// 添加源
    ptDllData->DstId[0] = ptCenterData->CalledNum[0];   //添加目的
    ptDllData->DstId[1] = ptCenterData->CalledNum[1];
    ptDllData->DstId[2] = ptCenterData->CalledNum[2];

    if (1 == tcclPrint->CclDown)
    {
        ODP_PrintLc((unsigned char *)ptDllData);
    }
}

/**
* @brief  封装下行至链路层语音包
* @param [in] pvCenterData  中心语音数据包指针
* @param [in] FrmType 语音帧类型
* @param [in] pvDllData 下行到链路层语音数据包指针
* @param [out] Len  下行到链路层语音包长度指针
* @author 牛功喜
* @since
* @bug
*/
void ODP_GenVoicePacket(unsigned char *pvCenterData, unsigned char *pvDllData, unsigned char FrmType, int *Len)
{
    CENTER_VOICE_DATA *ptCenterData = (CENTER_VOICE_DATA *)pvCenterData;
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)pvDllData;
    ptDllData->MsgType = DI_MSG_VOICE;
    ptDllData->FrmType = FrmType;
    ptDllData->DataType = CT_JUNK_DATA;

    if (1 == tcclPrint->CclDown)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] Frm=%d", __FUNCTION__, FrmType);
    }
    ptDllData->DataLen = CENTER_VOICE_DL_PAYLOADLEN;
    memcpy(ptDllData->PayLoad, ptCenterData->Payload, CENTER_VOICE_DL_PAYLOADLEN);
    *Len = CENTER_VOICE_DL_PAYLOADLEN + DLL_CCL_MSG_HEADLEN;
}


/**
* @brief  大小端转换
* @param [in] pvdlldata  下行DLL数据
* @param [in] pvCenterData中心下行短消息数据
* @param [out] pvDllData 下行到链路层语音数据包指针
* @param [out] datalen  下行到链路层语音包长度指针
* @author 牛功喜
* @since
* @bug
*/
/*void LitToBigSmsData(unsigned char *pvdlldata ,unsigned char *pvCenterData,unsigned short datalen)
{
    unsigned short i;
    for(i=0;i<datalen;i++)
    {
        if(0 ==i%2)
        {
            pvdlldata[i+1]=pvCenterData[i];
        }
        else
        {
            pvdlldata[i-1]=pvCenterData[i];
        }
    }
}*/

/**
* @brief  封装静音帧数据包
* @param [in] pvSilentData  静音帧数据指针
* @param [in] FrmType静音帧帧类型
* @param [out] pvDllData 下行到链路层语音数据包指针
* @param [out] Len  下行到链路层语音包长度指针
* @author 牛功喜
* @since
* @bug
*/
void ODP_GenSilentFrm(unsigned char *pvSilentData, unsigned char *pvDllData, unsigned char FrmType ,int *Len)
{
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
    ptDllData->MsgType = DI_MSG_VOICE;
    ptDllData->FrmType = FrmType;
    ptDllData->DataType = CT_JUNK_DATA;
    ptDllData->DataLen = CENTER_VOICE_DL_PAYLOADLEN;
    *Len = SILENT_FRAME_DATA_LEN + DLL_CCL_MSG_HEADLEN;
    memcpy(ptDllData->PayLoad, pvSilentData, CENTER_VOICE_DL_PAYLOADLEN);
}


/**
* @brief  封装遥控命令数据包
* @param [in] pvCenterData  接收中心数据
* @param [out] pvDllData 下行到链路层数据包指针
* @param [out] Len  下行到链路层语音包长度指针
* @author 牛功喜
* @since
* @bug
*/
void ODP_GenSigCtrlDataPacket(unsigned char *pvCenterData, unsigned char *pvDllData, int *Len)
{
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)pvDllData;
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;
    memset(ptDllData, 0, sizeof(CCL_DLL_DL_T));
    ptDllData->SrcId[0] = ptCenterData->SenderNum[0];
    ptDllData->SrcId[1] = ptCenterData->SenderNum[1];
    ptDllData->SrcId[2] = ptCenterData->SenderNum[2];
    ptDllData->DstId[0] = ptCenterData->ReceiverNum[0];
    ptDllData->DstId[1] = ptCenterData->ReceiverNum[1];
    ptDllData->DstId[2] = ptCenterData->ReceiverNum[2];

    switch (ptCenterData->SmsType)
    {
        case STUN_REQ_NAS:  // 遥晕信道机
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_STUN_REQ_NAS;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            if ((ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId) && (ptDllData->DstId[1] == 0) && (ptDllData->DstId[2] == 0))
            {
                g_CclState = Wait_Stun_Nas_Ack;  // 如是本地节点则不改变呼叫控制层状态
            }
            break;
        }
        case KILL_REQ_NAS:  // 遥毙信道机
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_KILL_REQ_NAS ;
            *Len=ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            if ((ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId) && (ptDllData->DstId[1] == 0) && (ptDllData->DstId[2] == 0))
            {
                g_CclState = Wait_Kill_Nas_Ack;
            }
            break;
        }
        case REVIVE_REQ_NAS:  // 激活链路机
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_ENABLE_REQ_NAS;
            *Len=ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            if ((ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId) && (ptDllData->DstId[1] == 0) && (ptDllData->DstId[2] == 0))
            {
                g_CclState = Wait_Reviv_NAS_Ack;
            }
            break;
        }
        case REVIVE_REQ_MS:  // 激活MS
        {
            ptDllData->MsgType = DI_MSG_DATA;
            ptDllData->DataType = CT_ENABLE_REQ_MS;
            *Len=ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            g_CclState=Wait_Reviv_MS_Ack;
            break;
        }
        case STUN_REQ_MS:  // 遥晕MS
        {
            ptDllData->MsgType = DI_MSG_DATA;
            ptDllData->DataType = CT_DISABLE_REQ_MS;    // 遥晕终端  信道机业务正常
            *Len = ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            g_CclState = Wait_Stun_Ms_Ack;
            break;
        }
        case GPS_REPOTR_MS:   // 终端GPS上拉
        {
            CallId++;
            ptDllData->MsgType = DI_MSG_DATA;
            ptDllData->DataType = CT_GPS_REPORT_REQ_MS;
            memcpy(ptDllData->PayLoad, (unsigned char *)&CallId, 4);
            if (CC_CCL_MSGPSRPT_PAYLODDLEN < ptCenterData->ValidLength)
            {
                LOG_WARNING(s_LogMsgId, "[CCL][%s]CC_GPS_RPT_MS LOAD ERR LEN=%d", __FUNCTION__, ptCenterData->ValidLength);
                ptCenterData->ValidLength = CC_CCL_MSGPSRPT_PAYLODDLEN;
            }
            ptDllData->DataLen = ptCenterData->ValidLength + 4;
            memcpy(&ptDllData->PayLoad[4], ptCenterData->SmsData, ptCenterData->ValidLength);
            g_CclState = Wait_Gps_Ms_Ack;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            break;
        }
        case NEGHR_QUERY:
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_NEGHR_QUERY;
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                g_CclState = Wait_NEGHR_Nas_Ack;
            }
            //g_CclState=Wait_NEGHR_Nas_Ack;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        case GPS_REPORT_NAS:   // 链路机GPS
        {
            ptDllData->MsgType=DI_MSG_WLU;
            ptDllData->DataType=CT_GPS_REPORT_REQ_NAS;
            ptDllData->DataLen=8;
            memset(ptDllData->PayLoad,0,sizeof(ptDllData->PayLoad));
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                g_CclState=Wait_Gps_Nas_Ack;
            }
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        case MESSAGE_GROUP_CALL:
        {
            ptDllData->MsgType=DI_MSG_DATA;
            ptDllData->DataType=CT_PACKET_DATA;
            SMS_CCL_DLL=(SMS_CCL_DLL_EN *)ptDllData->PayLoad;
            SMS_CCL_DLL->Identification=0x0000;
            SMS_CCL_DLL->SrcIpId=0x0;
            SMS_CCL_DLL->DestIpId=0x2;
            SMS_CCL_DLL->CmpresOpcode1=0;
            SMS_CCL_DLL->SrcPortId=1;
            SMS_CCL_DLL->CmpresOpcode2=0;
            SMS_CCL_DLL->DestPortId=1;
            if(DLL_CCL_SMS_PAYLOADLEN <=ptCenterData->ValidLength)
            {
                ptCenterData->ValidLength=DLL_CCL_SMS_PAYLOADLEN;
            }
            LitToBigSmsData(SMS_CCL_DLL->SmsPayLoad,ptCenterData->SmsData,ptCenterData->ValidLength);
            ptDllData->DataLen=ptCenterData->ValidLength +5;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        case MS_ALARM:   // 现行告警
        {
            ptDllData->MsgType  = DI_MSG_DATA;
            ptDllData->DataType = CT_ALARM_REQ_MS;
            ptDllData->DataLen  = 0;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            break;
        }
        case MS_ALARM_CLEAR:  // 告警清除
        {
            ptDllData->MsgType  = DI_MSG_DATA;
            ptDllData->DataType = CT_ALARM_ACK_MS;
            ptDllData->DataLen  = 0;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            break;
        }
        default:
        {
            *Len=0;
            LOG_DEBUG(s_LogMsgId,"[CCL][%s] ERROR SigCtrlsmsType=%d!", __FUNCTION__, ptCenterData->SmsType);
            break;
        }
    }

    if (1 == tcclPrint->CclDown)
    {
        ODP_CclPrintDllData((unsigned char *)ptDllData);
    }
}





void ODP_GenTerminatorPacket(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len)
{
    DLL_CCL_UL_T * ptDllData = (DLL_CCL_UL_T *)( pvDllData);
    DT_VOICE_LC_HEADER * ptDllPayLoad =(DT_VOICE_LC_HEADER *)ptDllData->PayLoad;
    PTT_CMD *ptCenterData=(PTT_CMD *)pvCenterData;
    ptDllData->MsgType = DI_MSG_DATA;
    ptDllData->DataType = CT_LC_TERMINATOR  ;
    ptDllData->FrmType = FT_VOICE_NO;
    ptDllData->DataLen = LC_HEADER_LEN;    // 负载长度=语音头LC 长度
    * Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
    ptDllPayLoad->Reserved = 0;
    ptDllPayLoad->ProtectFlg = 0;
    ptDllPayLoad->CtrOpcode = 0x00;
    ptDllPayLoad->FeatureId = 0x00;
    ptDllPayLoad->ServiceOpt = 0x00;
    ptDllPayLoad->GroupAddr[0] = ptCenterData->CalledNum[0];
    ptDllPayLoad->GroupAddr[1] = ptCenterData->CalledNum[1];
    ptDllPayLoad->GroupAddr[2] = ptCenterData->CalledNum[2];
    ptDllPayLoad->SourceAddr[0] = ptCenterData->CallingNum[0];
    ptDllPayLoad->SourceAddr[1] = ptCenterData->CallingNum[1];
    ptDllPayLoad->SourceAddr[2] = ptCenterData->CallingNum[2];

    ptDllData->SrcId[0] = ptCenterData->CallingNum[0];
    ptDllData->SrcId[1] = ptCenterData->CallingNum[1];
    ptDllData->SrcId[2] = ptCenterData->CallingNum[2];
    ptDllData->DstId[0] = ptCenterData->CalledNum[0];
    ptDllData->DstId[1] = ptCenterData->CalledNum[1];
    ptDllData->DstId[2] = ptCenterData->CalledNum[2];
    if(1 == tcclPrint->CclDown)
    {
        ODP_PrintLc((unsigned char *)ptDllData);
    }
}


/**
* @brief    保存中心呼叫ID、子网段数据
* @param [in] pvCenterData  接收中心数据
* @author 牛功喜
* @since
* @bug
*/
void SavCcIDNetData(unsigned char *pvCenterData, unsigned char *CCcvNetid)
{
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;
    CC_CVID_SUBNET_DATA *ptCVNetid = (CC_CVID_SUBNET_DATA *)CCcvNetid;
    ptCVNetid->CallId  = ptCenterData->CallId;
    ptCVNetid->VoiceId = ptCenterData->VoiceId;
    ptCVNetid->SubNet  = ptCenterData->SmsData[0];
    ptCVNetid->srcid[0]  = ptCenterData->SenderNum[0];
    ptCVNetid->srcid[1]  = ptCenterData->SenderNum[1];
    ptCVNetid->srcid[2]  = ptCenterData->SenderNum[2];
    ptCVNetid->destid[0] = ptCenterData->ReceiverNum[0];
    ptCVNetid->destid[1] = ptCenterData->ReceiverNum[1];
    ptCVNetid->destid[2] = ptCenterData->ReceiverNum[2];

    if (1 == tcclPrint->CclDown)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] CallId=%d VoiceId=%#x SubNet=%#x", __FUNCTION__, ptCVNetid->CallId, ptCVNetid->VoiceId, ptCVNetid->SubNet);
    }
}


/**
* @brief    打印LC_HEADER 、LC_TERMINATOR
* @param [in] ptDllData  发送到DLL 数据
* @author 牛功喜
* @since
* @bug
*/
void ODP_PrintLc(unsigned char * pvDllData)
{
    char IdBuf[100];
    const char *pcDataType;
    DLL_CCL_UL_T * ptDllData = (DLL_CCL_UL_T *)pvDllData;
    SetIdDec2buf(ptDllData->SrcId, ptDllData->DstId, IdBuf, sizeof(IdBuf));

    if (ptDllData->DataType == CT_LC_HEADER)
    {
        pcDataType = "Hdr";
    }
    else if (ptDllData->DataType == CT_LC_TERMINATOR)
    {
        pcDataType = "Ter";
    }
    else
    {
        pcDataType = "Err";

    }
        
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] LC:%s MsgType=%d DataType=%d FrmType=%d DataLen=%d %s", __FUNCTION__, pcDataType, ptDllData->MsgType, ptDllData->DataType, ptDllData->FrmType, ptDllData->DataLen, IdBuf);
}


/**
* @brief  打印中心信令
* @param [in] pvPttcmd 中心信令数据
* @author 牛功喜
* @since
* @bug
*/
void ODP_PrintCcSig(unsigned char *CcCmdBuf)
{
    char SigTypeBuf[50];
    char SmsTypeBuf[100];
    CENTER_CMD_SHARE_HEAD *ptHead = (CENTER_CMD_SHARE_HEAD *)CcCmdBuf;  // 获取接入单元与中心命令交互共用头
    SHARE_CC_DATA_D *ptCcData = (SHARE_CC_DATA_D *)CcCmdBuf;
    PTT_CMD *ptCcPttCmd = (PTT_CMD *)CcCmdBuf;
    PTT_OFF_ACK *ptCcPttAck = (PTT_OFF_ACK *)CcCmdBuf;
    SMS_CENTER_CCL_DL *ptCcSms = (SMS_CENTER_CCL_DL *)CcCmdBuf;  // 获取中心发送短消息到CCL 接口
    SetCCSig2Str(ptHead, SigTypeBuf, sizeof(SigTypeBuf));  // 将Sig转化字符串，方便打印查看

    if (SIG_PTT_CMD == ptHead->SigType) // PPT_ON or PPT_OFF 命令
    {
        LOG_DEBUG(s_LogMsgId, "[CCL][%s] SigType=%s SigLen=%d PttST=%s Calling[0-2]=[%#x:%#x:%#x] Called[0-2]=[%#x:%#x:%#x]", 
            __FUNCTION__, SigTypeBuf, ptHead->Datalength,
            (ptCcPttCmd->PttStat==0x06?"PPT_ON":"PPT_OFF"),
            ptCcPttCmd->CallingNum[0], ptCcPttCmd->CallingNum[1], ptCcPttCmd->CallingNum[2], 
            ptCcPttCmd->CalledNum[0],  ptCcPttCmd->CalledNum[1],  ptCcPttCmd->CalledNum[2]);
    }
    else if (SIG_PTT_ON_ACK == ptHead->SigType)
    {
        LOG_DEBUG(s_LogMsgId, "[CCL][%s] SigType=%s SigLen=%d Calling[0-2]=[%#x:%#x:%#x] Called[0-2]=[%#x:%#x:%#x]", 
            __FUNCTION__, SigTypeBuf, ptHead->Datalength,
            ptCcPttAck->CallingNum[0], ptCcPttAck->CallingNum[1], ptCcPttAck->CallingNum[2], 
            ptCcPttAck->CalledNum[0],  ptCcPttAck->CalledNum[1],  ptCcPttAck->CalledNum[2]);
    }
    else if (SIG_SMS_SEND == ptHead->SigType)
    {
        ODP_PrintSms(&ptCcData->CcSmsSig, SmsTypeBuf, sizeof(SmsTypeBuf));
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%s SigLen=%d SmsType=%s SmsValidLen=%d Snd[0-2]=[%#x:%#x:%#x] Rcv[0-2]=[%#x:%#x:%#x]", 
            __FUNCTION__, SigTypeBuf, ptHead->Datalength,
            SmsTypeBuf, ptCcData->CcSmsSig.ValidLength,
            ptCcSms->SenderNum[0], ptCcSms->SenderNum[1],ptCcSms->SenderNum[2], 
            ptCcSms->ReceiverNum[0], ptCcSms->ReceiverNum[1], ptCcSms->ReceiverNum[2]);
    }
    else if (SIG_PTT_OFF_ACK == ptHead->SigType)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] Rcv sig type SIG_PTT_OFF_ACK", __FUNCTION__);
    }
    else
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] Rcv sig type error", __FUNCTION__);
    }
}



/**
* @brief    打印 ccl_dll 数据
* @param [in] ptDllData  CCL下行数据
* @author 牛功喜
* @since
* @bug
*/
void ODP_CclPrintDllData(unsigned char *DllData)
{
    char MsgDatabuf[100];
    char IDbuf[100];
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)DllData;
    ODP_PrintMsgDatalog(ptDllData, MsgDatabuf, sizeof(MsgDatabuf));
    SetIdDec2buf(ptDllData->SrcId, ptDllData->DstId, IDbuf, sizeof(IDbuf));
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] %s,DataLen=%d,%s", __FUNCTION__, MsgDatabuf, ptDllData->DataLen, IDbuf);
}


