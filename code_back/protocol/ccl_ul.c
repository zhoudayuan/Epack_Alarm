/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    cclul.c
 * @brief   呼叫控制层上行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-29
 */

/*
 *   函数列表
 *   1. IDP_GenPttOnAckpacket          封装PTT-ON 命令回复数据包
 *   2. IDP_GenPttOffAckpacket         封装PTT-OFF 命令回复数据包
 *   3. IDP_GenPttOnCmd                 封装PTT-ON命令函数
 *   4. IDP_GenVoiceDataPaket          封装上行语音包
 *   5.
 *   6.
 *   7.
 *
 *   修改历史
 *   2016-07-29            建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "ccl_ul.h"
#include "ccl.h"
#include "dll.h"
#include "ccl_fun.h"
#include "print_debug.h"



/******************************************************************************
 *   类型定义
 *   *************************************************************************/
 extern SHM_IPC_STRU *ptIPCShm;
/**
 * @var  pLogTxtFd
 * @brief 文件句柄
 */
extern bool s_tNas_STOP_RELAY;
extern unsigned int  s_LogMsgId;
extern CCL_PRINT_T * tcclPrint;
extern FILE *pLogFd;
extern SMS_CCL_DLL_EN *SMS_CCL_DLL;
extern unsigned char s_tLast_Relay_Flg;
extern unsigned char    INF_CCL_STATE;
/**
 * @var g_DllGlobalCfg
 * @brief 全局配置项
 */
extern DLL_GLB_CFG_T g_DllGlobalCfg;
/**
 * @brief 调试打印信息
 */




 /******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/
 SMS_CCL_DLL_EN *SMS_DLL_CCL=NULL;
 extern  unsigned char s_aLcdata[20];
 extern bool s_tSend_PTTON_FLG;
 LC_HEADER_DATA  LC_DATA;
 unsigned int ThresholdSum=0;


/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
 /**
 * @brief   封装PTT-ON 命令回复数据包
 * @param [in] PttCmdResult   PTT回复状态
 * @param [out] pvCenterData  上行到中心的数据包指针
 * @param [out] Len  上行到中心的数据包长度指针
 * @author 牛功喜
 * @since
 * @bug
 */

void IDP_GenPttOnAckpacket(unsigned  char *pvCenterData, int *Len,unsigned  char PttCmdResult)
{

    PTT_ON_ACK *ptCenterData =(  PTT_ON_ACK *) pvCenterData;
    ptCenterData->SharedHead.SigHead=0xec13;
    ptCenterData->SharedHead.Datalength=sizeof(PTT_ON_ACK)-CENTER_SHARE_HEAD_LEN;
    ptCenterData->Ack=PttCmdResult;
    if(PttCmdResult)
    {
        ptCenterData->Ack=ACK_OK;
    }
    else
    {
        ptCenterData->Ack=ACK_FAIL;
    }
    ptCenterData->SharedHead.SigType=SIG_PTT_ON_ACK;
   // ptCenterData->SharedHead.Datalength=PTT_ACK_VALID_LEN;

    *Len=CENTER_SHARE_HEAD_LEN+PTT_ACK_VALID_LEN;
    if(1==tcclPrint->CclUp)
    {
        IDP_CclPrintpttack((unsigned char *)ptCenterData);
    }
}

/**
 * @brief   封装PTT-OFF 命令回复数据包
 * @param [out] pvCenterData  上行到中心的数据包指针
 * @param [out] Len  上行到中心的数据包长度指针
 * @author 牛功喜
 * @since
 * @bug
 */
void  IDP_GenPttOffAckpacket(unsigned  char *pvCenterData, int *Len)
{
    PTT_OFF_ACK *ptCenterData =(  PTT_OFF_ACK *) pvCenterData;
    ptCenterData->SharedHead.SigHead=0xec13;

    ptCenterData->SharedHead.SigType=SIG_PTT_OFF_ACK;
    //ptCenterData->SharedHead.Datalength=PTT_ACK_VALID_LEN;
    ptCenterData->SharedHead.Datalength=sizeof(PTT_ON_ACK)-CENTER_SHARE_HEAD_LEN;
   // *Len=CENTER_SHARE_HEAD_LEN+PTT_ACK_VALID_LEN;
    *Len=sizeof(PTT_ON_ACK);
    if(1==tcclPrint->CclUp)
    {
      LOG_DEBUG(s_LogMsgId,"[CCL][%s]  SigType=%d" , __FUNCTION__,ptCenterData->SharedHead.SigType);
    }

}


/**
 * @brief   封装NAS状态上报数据包
 * @param [out] pvCenterData  上行到中心的数据包指针
 * @param [out] Len  上行到中心的数据包长度指针
 * @author 牛功喜
 * @since
 * @bug
 */
void IDP_GenNasStatePack(unsigned char pvrelayflg, unsigned char *pvCenterData, int *len)
{
    CCL_STAT_INFO * ptCenterData=(CCL_STAT_INFO *) pvCenterData;
    ptCenterData->StatInfoHead.MsgHead=0xec13;
    ptCenterData->StatInfoHead.MsgType=SIG_STATUS_REPORT;
    ptCenterData->SourceStat=pvrelayflg;
    *len=sizeof( CCL_STAT_INFO);
    ptCenterData->StatInfoHead.Datalength = *len - sizeof(CCL_STAT_INFO_HEAD);
    if(1 == tcclPrint->CclUp)
    {
         LOG_DEBUG(s_LogMsgId,"[CCL][%s] SmsType=%d stat=%d" , __FUNCTION__,ptCenterData->StatInfoHead.MsgType ,ptCenterData->SourceStat);
    }

}

/**
 * @brief 封装PTT 命令数据包
 * @param [in] pvDllData   DLL上行数据
 * @param [in] cmd      PTT命令标识
 * @param [out] pvCenterData  上行到中心的数据包指针
 * @param [out] Len  上行到中心的数据包长度指针
 * @author 牛功喜
 * @since
 * @bug
 */

void IDP_GenPttCmd(unsigned char *pvDllData,unsigned char *pvCenterData,unsigned char cmd,int *Len)
{
    PTT_CMD *ptCenterData =(PTT_CMD *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;          //消息头
    ptCenterData->SharedHead.SigType = 0x000a;          //PTT命令
    ptCenterData->SharedHead.Datalength = 100;          //信令有效长度
    ptCenterData->SharedHead.CheckVal = 0xffff;         //01、02版本全部填写F 标识无校验
    ptCenterData->CalledNum[0]  = LC_DATA.DesId[0];
    ptCenterData->CalledNum[1]  = LC_DATA.DesId[1];
    ptCenterData->CalledNum[2]  = LC_DATA.DesId[2];
    ptCenterData->CallingNum[0] = LC_DATA.SourceId[0];
    ptCenterData->CallingNum[1] = LC_DATA.SourceId[1];
    ptCenterData->CallingNum[2] = LC_DATA.SourceId[2];

    //PPT  命令  标识PTT信令状态（请求CMD_ON=0x06、释放CMD_OFF=0x07�
    if (CMD_PTT_ON == cmd)
    {
        ptCenterData->PttStat = CMD_PTT_ON;
    }
    else
    {
        ptCenterData->PttStat = CMD_PTT_OFF;
    }
    *Len = CENTER_SHARE_HEAD_LEN + PTT_ACK_VALID_LEN;
    if (1 == tcclPrint->CclUp)
    {
        IDP_Ccl_PrintPttCmd((unsigned char *)ptCenterData);
    }
}
/**
* @brief    封装上行至中心短消息
* @param [in] pvDllData   DLL上行数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenSmsPacket(unsigned char *pvDllData,unsigned char *pvCenterData,int *Len)
{
    DLL_CCL_UL_T* ptDllData = ( DLL_CCL_UL_T*)pvDllData;
    SMS_CENTER_CCL_DL* ptCenterData = (SMS_CENTER_CCL_DL*)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.SigType = SIG_SMS_SEND;
    ptCenterData->SmsType = MESSAGE_GROUP_CALL;
    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL)-CENTER_SHARE_HEAD_LEN;;
    SMS_CCL_DLL = (SMS_CCL_DLL_EN*) ptDllData->PayLoad;
    ptCenterData->SenderNum[0] = ptDllData->SrcId[0];
    ptCenterData->SenderNum[1] = ptDllData->SrcId[1];
    ptCenterData->SenderNum[2] = ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0] = ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1] = ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2] = ptDllData->DstId[2];
    ptCenterData->ValidLength = ptDllData->DataLen-5;

    LitToBigSmsData(ptCenterData->SmsData,SMS_CCL_DLL->SmsPayLoad,ptDllData->DataLen-5);

    *Len=sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        LOG_PrintM(s_LogMsgId,SMS_CCL_DLL->SmsPayLoad,30);
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,30);
    }
}

/**
* @brief    保存LC 数据  并判断是否停止转发
* @param [in] pvDllData   DLL上行语音数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_Getlcdata(unsigned char *pvDllData)
{
    DLL_CCL_UL_T* ptDllData=(DLL_CCL_UL_T *)pvDllData;
    LC_DATA.SourceId[0]=ptDllData->SrcId[0];
    LC_DATA.SourceId[1]=ptDllData->SrcId[1];
    LC_DATA.SourceId[2]=ptDllData->SrcId[2];
    LC_DATA.DesId[0]=ptDllData->DstId[0];
    LC_DATA.DesId[1]=ptDllData->DstId[1];
    LC_DATA.DesId[2]=ptDllData->DstId[2];
    if(s_tLast_Relay_Flg != ptDllData->Prohibit )
    {
        s_tLast_Relay_Flg = ptDllData->Prohibit;
        s_tNas_STOP_RELAY = 1;
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]STOPRELAY  FLAG=%d", __FUNCTION__,s_tNas_STOP_RELAY);
    }
}

/**
* @brief    处理中心响应PTTON 命令函数
* @param [out] pvCenterData  中心下行数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_PttonAckhandle(unsigned char *pvCenterData)
{
    PTT_ON_ACK *ptCenterData =(PTT_ON_ACK *)pvCenterData;
    if (ACK_OK == ptCenterData->Ack)
    {
        s_tSend_PTTON_FLG = 1;
    }
    else
    {
        s_tSend_PTTON_FLG = 0;
    }

    if (1 == tcclPrint->CclUp)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]ttonAckhandle PTTON_FLG=%d", __FUNCTION__, s_tSend_PTTON_FLG);
    }
}
/**
* @brief    封装告警、清除信息
* @param [out] pvCenterData  中心上行数据
* @param [in] type  上报信息类型
* @param [in] pvDlldata DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_GenAlarm_ClearData(unsigned char type, unsigned char *pvDlldata, unsigned char *pvCenterData, int *Len)
{
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CENTER_SHARE_HEAD_LEN;
    ptCenterData->SharedHead.SigType = SIG_SMS_SEND;
    ptCenterData->SmsType =type;
    ptCenterData->SenderNum[0] = ptDllData->SrcId[0];
    ptCenterData->SenderNum[1] = ptDllData->SrcId[1];
    ptCenterData->SenderNum[2] = ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0] = ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1] = ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2] = ptDllData->DstId[2];
    ptCenterData->ValidLength = 0;
    *Len = sizeof(SMS_CENTER_CCL_DL);

    if (1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        //LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,20);
    }
}

/**
* @brief    封装MS遥晕、激活响应数据包
* @param [out] pvCenterData  中心上行数据
* @param [in] pvDlldata DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/

void  IDP_GenMsAckData(unsigned char datatype,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *pvCccvnetId)
{
    //DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData =(SMS_CENTER_CCL_DL*)pvCenterData;
    CC_CVID_SUBNET_DATA *ptCccvnetId= (CC_CVID_SUBNET_DATA *)pvCccvnetId;
    ptCenterData->SharedHead.SigHead=0xec13;
    ptCenterData->SharedHead.Datalength=sizeof(SMS_CENTER_CCL_DL)-CENTER_SHARE_HEAD_LEN;
    ptCenterData->SharedHead.SigType=SIG_SMS_SEND;

    ptCenterData->SmsType=datatype;

    ptCenterData->SenderNum[0]=ptCccvnetId->destid[0];
    ptCenterData->SenderNum[1]=ptCccvnetId->destid[1];
    ptCenterData->SenderNum[2]=ptCccvnetId->destid[2];
    ptCenterData->ReceiverNum[0]=ptCccvnetId->srcid[0];
    ptCenterData->ReceiverNum[1]=ptCccvnetId->srcid[1];
    ptCenterData->ReceiverNum[2]=ptCccvnetId->srcid[2];

    ptCenterData->ValidLength=4;
    ptCenterData->SmsData[0]=ptCccvnetId->SubNet;
    ptCenterData->SmsData[1]=ptCccvnetId->destid[0];
    ptCenterData->SmsData[2]=ptCccvnetId->destid[1];
    ptCenterData->SmsData[3]=ptCccvnetId->destid[2];
    *Len=sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,20);
    }
}
/**
* @brief    封装NAS 遥晕、遥毙、激活响应数据包
* @param [out] pvCenterData  中心上行数据
* @param [in] pvDlldata DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenNasSigAckData(unsigned char pvsigtype, unsigned char *pvDlldata, unsigned char *pvCenterData, int *Len)
{
    DLL_CCL_UL_T* ptDllData = ( DLL_CCL_UL_T*)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData =(SMS_CENTER_CCL_DL*)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.SigType = SIG_SMS_SEND;
    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CENTER_SHARE_HEAD_LEN;
    ptCenterData->SenderNum[0] = ptDllData->SrcId[0];
    ptCenterData->SenderNum[1] = ptDllData->SrcId[1];
    ptCenterData->SenderNum[2] = ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0] = ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1] = ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2] = ptDllData->DstId[2];

    if( CT_STUN_ACK_NAS == pvsigtype) //遥晕NAS
    {
        ptCenterData->SmsType = STUN_REQ_NAS_ACK;
    }
    else if(CT_KILL_ACK_NAS == pvsigtype) //遥毙NAS
    {
        ptCenterData->SmsType = KILL_REQ_NAS_ACK;
    }
    else if(CT_ENABLE_ACK_NAS == pvsigtype)  //激活NAS
    {
        ptCenterData->SmsType = REVIVE_REQ_NAS_ACK;
    }
    else if(CT_STUN_RPT_NAS == pvsigtype)
    {
        ptCenterData->SmsType = STUN_REQ_NAS;
    }
    else if(CT_KILL_RPT_NAS == pvsigtype)
    {
        ptCenterData->SmsType = KILL_REQ_NAS;
    }
    else if(CT_ENABLE_RPT_NAS == pvsigtype)
    {
        ptCenterData->SmsType = REVIVE_REQ_NAS;
    }
    else
    {
        LOG_ERROR(s_LogMsgId, "[CCL][%s]GPS DATATYPE ERROR", __FUNCTION__);
    }

    ptCenterData->ValidLength = 4;
    ptCenterData->SmsData[0] = 0;
    ptCenterData->SmsData[1] = 0;
    ptCenterData->SmsData[2] = 0;
    ptCenterData->SmsData[3] = ptDllData->SrcId[0];

    *Len = sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId, ptCenterData->SmsData, 20);
    }
}

/**
* @brief    封装上行至中心GPS 数据包
* @param [in] pvDllData   DLL上行语音数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenGpsData(unsigned char pvsigtype ,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *callidnet)
{
    CC_CVID_SUBNET_DATA *ptCccaldnet= (CC_CVID_SUBNET_DATA *)callidnet;
    DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData =(SMS_CENTER_CCL_DL*)pvCenterData;
    GPS_DATA_CC_T*CenterGpsData=(GPS_DATA_CC_T *)ptCenterData->SmsData;
    ptCenterData->SharedHead.SigHead=0xec13;
    ptCenterData->VoiceId=ptCccaldnet->VoiceId;
    ptCenterData->CallId=ptCccaldnet->CallId;
    ptCenterData->SharedHead.Datalength=sizeof(SMS_CENTER_CCL_DL)-CENTER_SHARE_HEAD_LEN;
    ptCenterData->SharedHead.SigType=SIG_SMS_SEND;
    if(CT_GPS_REPORT_ACK_NAS ==pvsigtype)
    {
        ptCenterData->SmsType=GPS_REPROT_NAS_ACK;
        CenterGpsData->CALLEDID[0]=0;
        CenterGpsData->CALLEDID[1]=0;
        CenterGpsData->CALLEDID[2]=0;
        CenterGpsData->CALLEDID[3]=ptCccaldnet->destid[0];
        ptCenterData->SenderNum[0]=ptDllData->SrcId[0];
        ptCenterData->SenderNum[1]=ptDllData->SrcId[1];
        ptCenterData->SenderNum[2]=ptDllData->SrcId[2];
        ptCenterData->ReceiverNum[0]=ptDllData->DstId[0];
        ptCenterData->ReceiverNum[1]=ptDllData->DstId[1];
        ptCenterData->ReceiverNum[2]=ptDllData->DstId[2];

    }
    else if(CT_GPS_REPORT_ACK_MS ==pvsigtype)
    {
        ptCenterData->SmsType=GPS_REPROT_MS_ACK;
        CenterGpsData->CALLEDID[0]=ptCccaldnet->SubNet;
        CenterGpsData->CALLEDID[1]=ptCccaldnet->destid[0];
        CenterGpsData->CALLEDID[2]=ptCccaldnet->destid[1];
        CenterGpsData->CALLEDID[3]=ptCccaldnet->destid[2];
        ptCenterData->SenderNum[0]=ptCccaldnet->destid[0];
        ptCenterData->SenderNum[1]=ptCccaldnet->destid[1];
        ptCenterData->SenderNum[2]=ptCccaldnet->destid[2];
        ptCenterData->ReceiverNum[0]=ptCccaldnet->srcid[0];
        ptCenterData->ReceiverNum[1]=ptCccaldnet->srcid[1];
        ptCenterData->ReceiverNum[2]=ptCccaldnet->srcid[2];
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]GPS DATATYPE ERROR ", __FUNCTION__);
    }

    GetTime(CenterGpsData->GPSCCLDATA.DATE);   //添加GPS 时间
    ptCenterData->ValidLength=sizeof(GPS_DATA_CC_T);
    memcpy(&CenterGpsData->GPSCCLDATA,ptDllData->PayLoad,ptDllData->DataLen);
    *Len=sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        IDP_CclPrintGpsData(ptCenterData->SmsData);
        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,30);
    }
}

/**
* @brief    封装上行至中心GPS 数据包
* @param [in] pvDllData   DLL上行语音数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
* @author 牛功喜
* @since
* @bug
*/
void  IDP_GenNearData(unsigned char neartyppe, unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *callidnet)
{
    CC_CVID_SUBNET_DATA *ptCccaldnet= (CC_CVID_SUBNET_DATA *)callidnet;
    DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData =(SMS_CENTER_CCL_DL*)pvCenterData;
    ptCenterData->SharedHead.SigHead=0xec13;
    ptCenterData->VoiceId=ptCccaldnet->VoiceId;
    ptCenterData->CallId=ptCccaldnet->CallId;
    ptCenterData->SmsType=NEGHR_QUERY_ACK;

    ptCenterData->SharedHead.Datalength=sizeof(SMS_CENTER_CCL_DL)-CENTER_SHARE_HEAD_LEN;
    ptCenterData->SharedHead.SigType=SIG_SMS_SEND;
    ptCenterData->SenderNum[0]=ptDllData->SrcId[0];
    ptCenterData->SenderNum[1]=ptDllData->SrcId[1];
    ptCenterData->SenderNum[2]=ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0]=ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1]=ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2]=ptDllData->DstId[2];

    if( CC_CCL_SMS_PAYLODDLEN <= ptCenterData->ValidLength)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]near rpt len err LOADLEN=%d ", __FUNCTION__,ptDllData->DataLen);
        ptCenterData->ValidLength=CC_CCL_SMS_PAYLODDLEN;
    }
    if(NEAR_REPORT_PASSIVE ==neartyppe)  //主动上报 与邻点查询PALOAD  数据结构一致
    {
        ptCenterData->ValidLength=ptDllData->DataLen;
        memcpy(&ptCenterData->SmsData,ptDllData->PayLoad,ptDllData->DataLen);
    }
    else
    {
        ptCenterData->ValidLength=ptDllData->DataLen;
        memcpy(&ptCenterData->SmsData,ptDllData->PayLoad,ptDllData->DataLen);

    }
    *Len=sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
//        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,30);
    }
}



/**
* @brief    封装上行至中心场强数据包
* @param [in] pvDllData   DLL上行语音数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
** @param [in] pvthrethhold 场强数据指针
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenThrethHoldData( unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len)
{
    unsigned short ThredholdAvg;
    DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDlldata;
    SMS_CENTER_CCL_DL* ptCenterData =(SMS_CENTER_CCL_DL*)pvCenterData;
    ptCenterData->SharedHead.SigHead=0xec13;
    ptCenterData->SmsType=VARIANCE_HRESHOLD;
    ptCenterData->SharedHead.SigType=SIG_SMS_SEND;
    ptCenterData->SharedHead.Datalength=sizeof(SMS_CENTER_CCL_DL)-CENTER_SHARE_HEAD_LEN;
    ptCenterData->SenderNum[0]=ptDllData->SrcId[0];
    ptCenterData->SenderNum[1]=ptDllData->SrcId[1];
    ptCenterData->SenderNum[2]=ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0]=ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1]=ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2]=ptDllData->DstId[2];
    ThredholdAvg=ThresholdSum/6;
    ptCenterData->ValidLength=2;
    ThredholdAvg=(ThredholdAvg>>8 &0x00ff) | (ThredholdAvg<<8 & 0xff00);
    memcpy(&ptCenterData->SmsData,(unsigned char *)&ThredholdAvg,2);
    *Len=sizeof(SMS_CENTER_CCL_DL);
    if(1 == tcclPrint->CclUp)
    {
        IDP_CclPrintSms((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,30);
    }
}

/**
* @brief    封装上行至中心语音包
* @param [in] pvDllData   DLL上行语音数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
* @author 牛功喜
* @since
* @bug
*/

void IDP_GenVoiceDataPaket(unsigned char *pvDllData, unsigned char *pvCenterData, int *Len)
{
    //unsigned short Threshold=0;
    DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDllData;
    CENTER_VOICE_DATA* ptCenterData =(CENTER_VOICE_DATA*)pvCenterData;
    ptCenterData->Rtp_Head.csrc_count = 0;
    ptCenterData->Rtp_Head.version = 2;
    ptCenterData->Rtp_Head.ex_head_flag = 0;
    ptCenterData->Rtp_Head.padding_flag = 0;
    ptCenterData->Rtp_Head.payload_type = 0;
    ptCenterData->Rtp_Head.marker = 0;
    ptCenterData->Rtp_Head.csrc_count = 0;
    ptCenterData->Rtp_Ex.profile_head = htons(0x0000);
    ptCenterData->Rtp_Ex.exhead_length = htons(4);
    ptCenterData->Rtp_Ex.voice_id = 0xffffffff;
    ptCenterData->Rtp_Ex.gps = 0;
    ptCenterData->Rtp_Ex.codec_flag = 0xff;
    ptCenterData->Rtp_Ex.encode_flag = 0;
    ptCenterData->Rtp_Ex.pack_rate = 60;

    if (DLL_VOICE_UL_PAYLOADLEN != ptDllData->DataLen)
    {
        LOG_WARNING(s_LogMsgId,"[CCL][%s] MsgType=%d DATALEN ERROR DataLen=%d ", __FUNCTION__, ptDllData->MsgType, ptDllData->DataLen);
        ptDllData->DataLen = DLL_VOICE_UL_PAYLOADLEN;
    }
    memcpy(ptCenterData->Payload, ptDllData->PayLoad, ptDllData->DataLen);
    if (s_tLast_Relay_Flg != ptDllData->Prohibit)
    {
        s_tLast_Relay_Flg = ptDllData->Prohibit;
        s_tNas_STOP_RELAY = 1;
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] STOPRELAY  FLAG=%d", __FUNCTION__,s_tNas_STOP_RELAY);
    }
    if (FT_VOICE_A == ptDllData->FrmType)
    {
        ThresholdSum = 0;
    }
    ThresholdSum += (ptDllData->Vari[0] <<8 | ptDllData->Vari[1]);

    *Len=sizeof(CENTER_VOICE_DATA);
    if (1 == tcclPrint->DllUp)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] MsgType=%d FrmType=%d DataLen=%d ThresholdSum=%d ccl_state=%d\n", __FUNCTION__,
        ptDllData->MsgType, ptDllData->FrmType, ptDllData->DataLen,ThresholdSum,INF_CCL_STATE);
    }
}
/**
 * @brief   打印PTT 命令响应函数
 * @param [in] pvCenterData  上行到中心的命令数据包指针
 * @author 牛功喜
 * @since
 * @bug
 */
void IDP_CclPrintpttack(unsigned char *CenterData)
{
    char buf[100];
    PTT_ON_ACK *ptCenterData = (PTT_ON_ACK *)CenterData;
    SetCCSig2Str(&ptCenterData->SharedHead, buf, sizeof(buf));
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%#x %s Ack=%#x(OK=0x04,Fail=0x05)", __FUNCTION__, ptCenterData->SharedHead.SigType, buf, ptCenterData->Ack);
}
/**
 * @brief   打印PTT 命令函数
 * @param [in] pvCenterData  上行到中心的命令数据包指针
 * @author 牛功喜
 * @since
 * @bug
 */
void IDP_Ccl_PrintPttCmd(unsigned char *CenterData)
{
    PTT_CMD *ptCenterData=(PTT_CMD *)CenterData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] ccl2cc SigType=%d PTTCmd=%d call=[%d:%d:%d] called=[%d:%d:%d]",
    __FUNCTION__,
    ptCenterData->SharedHead.SigType,
    ptCenterData->PttStat,
    ptCenterData->CallingNum[0], ptCenterData->CallingNum[1], ptCenterData->CallingNum[2],
    ptCenterData->CalledNum[0], ptCenterData->CalledNum[1], ptCenterData->CalledNum[2]);
}


/**
* @brief    打印ccl_cc 短消息数据
* @param [in] pvCenterData  上行到中心的短消息数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_CclPrintSms(unsigned char* CenterData)
{
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL* )CenterData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%#x SmsType=%#x Length=%d Cid=%d vid=%#x snd[%#x:%#x:%#x] rcv[%#x:%#x:%#x]",
        __FUNCTION__,
        ptCenterData->SharedHead.SigType, ptCenterData->SmsType, ptCenterData->ValidLength,
        ptCenterData->CallId, ptCenterData->VoiceId,
        ptCenterData->SenderNum[0], ptCenterData->SenderNum[1], ptCenterData->SenderNum[2],
        ptCenterData->ReceiverNum[0], ptCenterData->ReceiverNum[1], ptCenterData->ReceiverNum[2]);
    IDP_PrintSmsLog(ptCenterData);  // by zhoudayuan
    PrintIDbyDec(ptCenterData->SenderNum, ptCenterData->ReceiverNum);
}


/**
* @brief    打印 dll_ccl 数据
* @param [in] ptDllData  DLL上行数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_CclPrintDllData(unsigned char *DllData)
{
    DLL_CCL_UL_T *ptDllData=(DLL_CCL_UL_T *)DllData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] MsgType=%d FrmType=%d DataType=%#x ValidLen=%d Src[%#x:%#x:%#x] Dst[%#x:%#x:%#x]", 
        __FUNCTION__,
        ptDllData->MsgType, ptDllData->FrmType, ptDllData->DataType,
        ptDllData->DataLen,
        ptDllData->SrcId[0], ptDllData->SrcId[1], ptDllData->SrcId[2],
        ptDllData->DstId[0], ptDllData->DstId[1], ptDllData->DstId[2]);
}


void IDP_CclPrintGpsData(unsigned char *pvGpsData)
{
    GPS_DATA_CC_T*ptGpsData=(GPS_DATA_CC_T *)pvGpsData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s]NS=%d EW=%d NDEG=%d NMINmm=%d NMINF=%d EDEG=%d EMINmm=%d EMINF=%d", 
        __FUNCTION__,       //EMINmm=%d  EMINF=%d"  , __FUNCTION__,
        ptGpsData->GPSCCLDATA.NS,
        ptGpsData->GPSCCLDATA.EW,
        ptGpsData->GPSCCLDATA.NDEG,
        ptGpsData->GPSCCLDATA.NMINmm,
        ptGpsData->GPSCCLDATA.NMINF,
        ptGpsData->GPSCCLDATA.EDEG,
        ptGpsData->GPSCCLDATA.EMINmm,
        ptGpsData->GPSCCLDATA.EMINF);
}


