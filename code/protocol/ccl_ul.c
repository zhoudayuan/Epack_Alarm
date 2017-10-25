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
 *   1. IDP_GenPttOnAck          封装PTT-ON 命令回复数据包
 *   2. IDP_GenPttOffAck         封装PTT-OFF 命令回复数据包
 *   3. IDP_GenPttOnCmd          封装PTT-ON命令函数
 *   4. IDP_GenVoiceDataPaket    封装上行语音包
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
extern bool s_NasStopRelayFlg;
extern unsigned int  s_LogMsgId;
extern CCL_PRINT_T * tcclPrint;
extern FILE *pLogFd;
extern SMS_CCL_DLL_EN *SMS_CCL_DLL;
extern unsigned char s_LastRelayFlg;
extern unsigned char g_CclState;
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
 extern bool s_SndPttOnFlg;
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

void IDP_GenPttOnAck(unsigned char *pvCenterData, int *Len, unsigned char PttCmdResult)
{
    PTT_ON_ACK *ptCenterData = (PTT_ON_ACK *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.Datalength = sizeof(PTT_ON_ACK) - CENTER_SHARE_HEAD_LEN;
    ptCenterData->Ack = PttCmdResult;

    if (PttCmdResult)
    {
        ptCenterData->Ack = PPT_ACK_OK;
    }
    else
    {
        ptCenterData->Ack = PPT_ACK_FAIL;
    }
    ptCenterData->SharedHead.SigType = SIG_PTT_ON_ACK;
    *Len = CENTER_SHARE_HEAD_LEN + PTT_ACK_VALID_LEN;
    if (1 == tcclPrint->CclUp)
    {
        IDP_PrintPttAck((unsigned char *)ptCenterData);
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
void IDP_GenPttOffAck(unsigned char *pvCenterData, int *Len)
{
    char buf[100];
    PTT_OFF_ACK *ptCenterData = (PTT_OFF_ACK *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.SigType = SIG_PTT_OFF_ACK;
    ptCenterData->SharedHead.Datalength = sizeof(PTT_ON_ACK)-CENTER_SHARE_HEAD_LEN;
    *Len=sizeof(PTT_ON_ACK);

    if (1 == tcclPrint->CclUp)
    {
        SetCCSig2Str(&ptCenterData->SharedHead, buf, sizeof(buf));
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%s", __FUNCTION__, buf);
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
    ptCenterData->StatInfoHead.MsgHead = 0xec13;
    ptCenterData->StatInfoHead.MsgType = SIG_STATUS_REPORT;
    ptCenterData->SourceStat = pvrelayflg;
    *len = sizeof(CCL_STAT_INFO);
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
    PTT_CMD *ptCenterData = (PTT_CMD *)pvCenterData;
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

    //PPT  命令  标识PTT信令状态（请求CMD_ON=0x06、释放CMD_OFF=0x07
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
        IDP_CcPttCmdPrint((unsigned char *)ptCenterData);
    }
}
/**
* @brief    封装上行至中心短消息
* @param [in] pvDllData DLL上行数据
* @param [out] pvCenterData  上行到中心的数据包指针
* @param [out] Len  上行到中心的数据包长度指针
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenSmsPacket(unsigned char *pvDllData,unsigned char *pvCenterData, int *Len)
{
    DLL_CCL_UL_T* ptDllData = (DLL_CCL_UL_T *)pvDllData;
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId, ptCenterData->SmsData, 30);
    }
}

/**
* @brief    保存LC 数据  并判断是否停止转发
* @param [in] pvDllData   DLL上行语音数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_GetLcData(unsigned char *pvDllData)
{
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
    LC_DATA.SourceId[0] = ptDllData->SrcId[0];
    LC_DATA.SourceId[1] = ptDllData->SrcId[1];
    LC_DATA.SourceId[2] = ptDllData->SrcId[2];
    LC_DATA.DesId[0] = ptDllData->DstId[0];
    LC_DATA.DesId[1] = ptDllData->DstId[1];
    LC_DATA.DesId[2] = ptDllData->DstId[2];

    if(s_LastRelayFlg != ptDllData->Prohibit)
    {
        s_LastRelayFlg = ptDllData->Prohibit;
        s_NasStopRelayFlg = 1;
        if (tcclPrint->DllUp == 1)
        {
            LOG_DEBUG(s_LogMsgId, "[CCL][%s] s_NasStopRelayFlg=%d", __FUNCTION__, s_NasStopRelayFlg);
        }
    }
}

/**
* @brief    处理中心响应PTTON 命令函数
* @param [out] pvCenterData  中心下行数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_PttOnAckHandle(unsigned char *pvCenterData)
{
    PTT_ON_ACK *ptCenterData =(PTT_ON_ACK *)pvCenterData;
    if (PPT_ACK_OK == ptCenterData->Ack)
    {
        s_SndPttOnFlg = 1;//本地中心成功处理上行PTT_ON
    }
    else
    {
        s_SndPttOnFlg = 0;
    }

    if (1 == tcclPrint->CclUp)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] s_SndPttOnFlg=%d", __FUNCTION__, s_SndPttOnFlg);
    }
}
/**
* @brief    封装告警、清除信息
* @param [out] pvCenterData  中心上行数据
* @param [in] type  上报信息类型
* @param [in] pvDllData DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_GenAlarm_ClearData(unsigned char type, unsigned char *pvDllData, unsigned char *pvCenterData, int *Len)
{
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
        //LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,20);
    }
}

/**
* @brief    封装MS遥晕、激活响应数据包
* @param [out] pvCenterData  中心上行数据
* @param [in] pvDllData DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/

void  IDP_GenMsAckData(unsigned char datatype,unsigned char * pvDllData,unsigned char * pvCenterData,int *Len,unsigned char *pvCccvnetId)
{
    //DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDllData;
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
        LOG_PrintM(s_LogMsgId,ptCenterData->SmsData,20);
    }
}
/**
* @brief    封装NAS 遥晕、遥毙、激活响应数据包
* @param [out] pvCenterData  中心上行数据
* @param [in] pvDllData DLL层上行数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_GenNasSigAckData(unsigned char pvSigType, unsigned char *pvDllData, unsigned char *pvCenterData, int *Len)
{
    DLL_CCL_UL_T* ptDllData = ( DLL_CCL_UL_T*)pvDllData;
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

    if( CT_STUN_ACK_NAS == pvSigType) //遥晕NAS
    {
        ptCenterData->SmsType = STUN_REQ_NAS_ACK;
    }
    else if(CT_KILL_ACK_NAS == pvSigType) //遥毙NAS
    {
        ptCenterData->SmsType = KILL_REQ_NAS_ACK;
    }
    else if(CT_ENABLE_ACK_NAS == pvSigType)  //激活NAS
    {
        ptCenterData->SmsType = REVIVE_REQ_NAS_ACK;
    }
    else if(CT_STUN_RPT_NAS == pvSigType)
    {
        ptCenterData->SmsType = STUN_REQ_NAS;
    }
    else if(CT_KILL_RPT_NAS == pvSigType)
    {
        ptCenterData->SmsType = KILL_REQ_NAS;
    }
    else if(CT_ENABLE_RPT_NAS == pvSigType)
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
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
void IDP_GenGpsData(unsigned char pvSigType, unsigned char *pvDllData, unsigned char *ptvCcData, int *Len, unsigned char *pucCallIdNet)
{
    /* pvDllData ==> ptvCcData */
    CC_CVID_SUBNET_DATA *ptCcCaldNet = (CC_CVID_SUBNET_DATA *)pucCallIdNet;
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
    SMS_CENTER_CCL_DL *pttCcData = (SMS_CENTER_CCL_DL *)ptvCcData;
    GPS_DATA_CC_T *ptSmsGps = (GPS_DATA_CC_T *)pttCcData->SmsData;

    pttCcData->SharedHead.SigHead = 0xec13;     // AC接口信令头标志
    pttCcData->VoiceId = ptCcCaldNet->VoiceId;  // 业务面ID
    pttCcData->CallId = ptCcCaldNet->CallId;    // 控制面ID
    pttCcData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CENTER_SHARE_HEAD_LEN;  // 有效数据长度
    pttCcData->SharedHead.SigType = SIG_SMS_SEND;  // 短消息命令

    if (CT_GPS_REPORT_ACK_NAS == pvSigType)  // NAS
    {
        pttCcData->SmsType = GPS_REPROT_NAS_ACK;  // 短消息类型
        ptSmsGps->CALLEDID[0] = 0;
        ptSmsGps->CALLEDID[1] = 0;
        ptSmsGps->CALLEDID[2] = 0;
        ptSmsGps->CALLEDID[3] = ptCcCaldNet->destid[0];
        // [发射][接收]ID 赋值
        pttCcData->SenderNum[0] = ptDllData->SrcId[0];
        pttCcData->SenderNum[1] = ptDllData->SrcId[1];
        pttCcData->SenderNum[2] = ptDllData->SrcId[2];
        pttCcData->ReceiverNum[0] = ptDllData->DstId[0];
        pttCcData->ReceiverNum[1] = ptDllData->DstId[1];
        pttCcData->ReceiverNum[2] = ptDllData->DstId[2];
        // GPS时间
        GetTime(ptSmsGps->GPSCCLDATA.DATE);   //添加GPS 时间，同时更新系统时间，并添加到结构当中
        // 有效数据长度
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T);
        // 有效数据
        memcpy(&ptSmsGps->GPSCCLDATA, ptDllData->PayLoad, ptDllData->DataLen);    // 实际的GPS数据

        *Len = sizeof(SMS_CENTER_CCL_DL);  // 有效长度还是整体长度有待沟通

    }
    else if (CT_GPS_REPORT_ACK_MS == pvSigType) // MS
    {
        pttCcData->SmsType = GPS_REPROT_MS_ACK;  // 短消息类型
        ptSmsGps->CALLEDID[0] = ptCcCaldNet->SubNet;
        ptSmsGps->CALLEDID[1] = ptCcCaldNet->destid[0];
        ptSmsGps->CALLEDID[2] = ptCcCaldNet->destid[1];
        ptSmsGps->CALLEDID[3] = ptCcCaldNet->destid[2];
        /*
        ** 发射\接收ID 赋值
        */ 
        pttCcData->SenderNum[0] = ptCcCaldNet->destid[0];   // 组号, 例如 9002
        pttCcData->SenderNum[1] = ptCcCaldNet->destid[1];   // 组号
        pttCcData->SenderNum[2] = ptCcCaldNet->destid[2];   // 组号
        pttCcData->ReceiverNum[0] = ptCcCaldNet->srcid[0];  // 手台或设备
        pttCcData->ReceiverNum[1] = ptCcCaldNet->srcid[1];  // 手台或设备
        pttCcData->ReceiverNum[2] = ptCcCaldNet->srcid[2];  // 手台或设备
        // GPS时间, 添加到主叫ID中
        GetTime(ptSmsGps->GPSCCLDATA.DATE);   //添加GPS 时间，同时更新系统时间，并添加到结构当中
        // 有效数据长度
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T);
        // 有效数据
        memcpy(&ptSmsGps->GPSCCLDATA, ptDllData->PayLoad, ptDllData->DataLen);    // 实际的GPS数据

        *Len = sizeof(SMS_CENTER_CCL_DL);  // 有效长度还是整体长度有待沟通
    }
    else if (CT_VOICE_EMB_GPS == pvSigType)  // 语音内嵌GPS(MS or Nas)
    {
        
        GPS_DATA_CC_T *ptCcGpsSnd = ptSmsGps;   // 主叫-发送到CC
        GPS_DATA_CC_T *ptCcGpsRcv = ptSmsGps+1; // 被叫-发送到CC
        GPS_DATA_CCL_T *ptCclGpsCalling = &ptCcGpsSnd->GPSCCLDATA;  // 指向主叫GPS信息 - 经纬度+时间
        GPS_DATA_CCL_T *ptCclGpsCalled  = &ptCcGpsRcv->GPSCCLDATA;  // 指向被叫GPS信息 - 经纬度+时间
        GPS_NAS_DATA_T *ptDllGpsSrc = (GPS_NAS_DATA_T *)ptDllData->PayLoad;  // 主叫-来自DLL
        GPS_NAS_DATA_T *ptDllGpsDst = (GPS_NAS_DATA_T *)(ptDllData->PayLoad + sizeof(GPS_NAS_DATA_T)); // 被叫-来自DLL

        pttCcData->SmsType = GPS_EMB_VOICE_REPORT;  // 短消息类型（组呼、个呼）
        /*
        ** ID 赋值
        */ 
        pttCcData->SenderNum[0] = ptDllData->SrcId[0];      // 组号, 例如 9002
        pttCcData->SenderNum[1] = ptDllData->SrcId[1];      // 组号
        pttCcData->SenderNum[2] = ptDllData->SrcId[2];      // 组号
        pttCcData->ReceiverNum[0] = ptDllData->DstId[0];    // 手台或设备
        pttCcData->ReceiverNum[1] = ptDllData->DstId[1];    // 手台或设备
        pttCcData->ReceiverNum[2] = ptDllData->DstId[2];    // 手台或设备
        /* 
        ** 有效数据长度 , 主叫GPS信息 + 被叫GPS信息
        */ 
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T) + sizeof(GPS_DATA_CC_T);  // Src + Dst
        /* 
        ** Src-主叫GPS信息, [不带]GPS时间, ID为主叫ID，或MS或NAS
        */ 
        ptCcGpsSnd->CALLEDID[0] = 0;   
        ptCcGpsSnd->CALLEDID[1] = ptDllData->SrcId[0];
        ptCcGpsSnd->CALLEDID[2] = ptDllData->SrcId[1];
        ptCcGpsSnd->CALLEDID[3] = ptDllData->SrcId[2];
        memcpy(ptCclGpsCalling, ptDllGpsSrc, ptDllData->DataLen);  // 拷贝主叫GPS信息
        memset(ptCclGpsCalling->DATE, 0, sizeof(ptCcGpsSnd->GPSCCLDATA.DATE));  // 清主叫时间, 便于区分主叫和被叫
        /* 
        ** Dst-被叫GPS信息, [带]GPS时间, ID为被叫，即自己的ID
        */ 
        ptCcGpsRcv->CALLEDID[0] = 0;  
        ptCcGpsRcv->CALLEDID[1] = g_DllGlobalCfg.auNodeId;
        ptCcGpsRcv->CALLEDID[2] = 0;
        ptCcGpsRcv->CALLEDID[3] = 0;
        memcpy(ptCclGpsCalled, ptDllGpsDst, ptDllData->DataLen); // 拷贝被叫GPS信息
        /*
        ** 时间一定要放在最后，因为内存重合
        */
        GetTime(ptCcGpsRcv->GPSCCLDATA.DATE);  // 添加GPS时间(字符串形式)，同时更新系统时间
        *Len = sizeof(SMS_CENTER_CCL_DL);  
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]GPS DATATYPE ERROR ", __FUNCTION__);
    }

    // 调试
    if (1 == tcclPrint->CclUp)
    {
        IDP_CclSmsPrint((unsigned char *)pttCcData);
        IDP_CclPrintGpsData(pttCcData->SmsData);
        LOG_PrintM(s_LogMsgId,pttCcData->SmsData,30);
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
void IDP_GenNearData(unsigned char neartyppe, unsigned char * pvDllData,unsigned char * pvCenterData,int *Len,unsigned char *CallIdNet)
{
    CC_CVID_SUBNET_DATA *ptCcCaldNet = (CC_CVID_SUBNET_DATA *)CallIdNet;
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;

    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->VoiceId = ptCcCaldNet->VoiceId;
    ptCenterData->CallId = ptCcCaldNet->CallId;
    ptCenterData->SmsType = NEGHR_QUERY_ACK;

    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CENTER_SHARE_HEAD_LEN;
    ptCenterData->SharedHead.SigType=SIG_SMS_SEND;
    ptCenterData->SenderNum[0]=ptDllData->SrcId[0];
    ptCenterData->SenderNum[1]=ptDllData->SrcId[1];
    ptCenterData->SenderNum[2]=ptDllData->SrcId[2];
    ptCenterData->ReceiverNum[0]=ptDllData->DstId[0];
    ptCenterData->ReceiverNum[1]=ptDllData->DstId[1];
    ptCenterData->ReceiverNum[2]=ptDllData->DstId[2];

    if( CC_CCL_SMS_PAYLODD_MAX_LEN <= ptDllData->DataLen)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]near rpt len err LOADLEN=%d ", __FUNCTION__,ptDllData->DataLen);
        ptDllData->DataLen=CC_CCL_SMS_PAYLODD_MAX_LEN;
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
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
void IDP_GenThrethHoldData(unsigned char * pvDllData,unsigned char * pvCenterData,int *Len)
{
    unsigned short ThredholdAvg;
    DLL_CCL_UL_T* ptDllData=( DLL_CCL_UL_T*)pvDllData;
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
        IDP_CclSmsPrint((unsigned char *)ptCenterData);
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
    char CclStateBuf[50];
    DLL_CCL_UL_T* ptDllData = (DLL_CCL_UL_T *)pvDllData;
    CENTER_VOICE_DATA* ptCenterData = (CENTER_VOICE_DATA *)pvCenterData;
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
        LOG_WARNING(s_LogMsgId,"[CCL][%s] Datalen Error, MsgType=%d Datalen=%d", __FUNCTION__, ptDllData->MsgType, ptDllData->DataLen);
        ptDllData->DataLen = DLL_VOICE_UL_PAYLOADLEN;
    }

    memcpy(ptCenterData->Payload, ptDllData->PayLoad, ptDllData->DataLen);

    if (s_LastRelayFlg != ptDllData->Prohibit)
    {
        s_LastRelayFlg = ptDllData->Prohibit;
        s_NasStopRelayFlg = 1;

        if (tcclPrint->DllUp == 1)
        {
            LOG_DEBUG(s_LogMsgId,"[CCL][%s] s_NasStopRelayFlg=%d", __FUNCTION__, s_NasStopRelayFlg);
        }
    }

    if (FT_VOICE_A == ptDllData->FrmType)
    {
        ThresholdSum = 0;
    }

    ThresholdSum += (ptDllData->Vari[0] <<8 | ptDllData->Vari[1]);

    *Len = sizeof(CENTER_VOICE_DATA);

    if (1 == tcclPrint->DllUp)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] MsgType=%d,FrmType=%d,DataLen=%d,ThresholdSum=%d,g_CclState=%s", __FUNCTION__, ptDllData->MsgType, ptDllData->FrmType, ptDllData->DataLen, ThresholdSum, GetCclStateByStr(g_CclState, CclStateBuf));
    }
}
/**
 * @brief   打印PTT 命令响应函数
 * @param [in] pvCenterData  上行到中心的命令数据包指针
 * @author 牛功喜
 * @since
 * @bug
 */
void IDP_PrintPttAck(unsigned char *CenterData)
{
    char buf[100];
    PTT_ON_ACK *ptCenterData = (PTT_ON_ACK *)CenterData;
    SetCCSig2Str(&ptCenterData->SharedHead, buf, sizeof(buf));
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%s, Ack=%s", __FUNCTION__, buf, (ptCenterData->Ack==0x04? "OK":"Fail"));
}
/**
 * @brief   打印PTT 命令函数
 * @param [in] pvCenterData  上行到中心的命令数据包指针
 * @author 牛功喜
 * @since
 * @bug
 */
void IDP_CcPttCmdPrint(unsigned char *CenterData)
{
    PTT_CMD *ptCenterData=(PTT_CMD *)CenterData;
    LOG_DEBUG(s_LogMsgId, "[CCL][%s] SigType=%d PttCmd=%s Calling=[%#x:%#x:%#x] Called=[%#x:%#x:%#x]",
    __FUNCTION__,
    ptCenterData->SharedHead.SigType,
    (ptCenterData->PttStat==CMD_PTT_ON?"PPT_ON":"PPT_OFF"),
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

void IDP_CclSmsPrint(unsigned char* CenterData)
{
    char SmsBuf[50];
    char IdBuf[50];
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL* )CenterData;
    PrintSmsLog(ptCenterData->SmsType, SmsBuf, sizeof(SmsBuf));
    SetIdDec2buf(ptCenterData->SenderNum, ptCenterData->ReceiverNum, IdBuf, sizeof(IdBuf));

    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%#x,SmsType=%s,Length=%d,Cid=%d,vid=%#x,%s",
        __FUNCTION__,
        ptCenterData->SharedHead.SigType, SmsBuf, ptCenterData->ValidLength,
        ptCenterData->CallId, ptCenterData->VoiceId, IdBuf);
#if 0
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SigType=%#x,SmsType=%s,Length=%d,Cid=%d,vid=%#x, snd[%#x:%#x:%#x] rcv[%#x:%#x:%#x]",
        __FUNCTION__,
        ptCenterData->SharedHead.SigType, SmsBuf, ptCenterData->ValidLength,
        ptCenterData->CallId, ptCenterData->VoiceId,
        ptCenterData->SenderNum[0], ptCenterData->SenderNum[1], ptCenterData->SenderNum[2],
        ptCenterData->ReceiverNum[0], ptCenterData->ReceiverNum[1], ptCenterData->ReceiverNum[2]);
#endif

}


/**
* @brief    打印 dll_ccl 数据
* @param [in] ptDllData  DLL上行数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_DllDataPrint(unsigned char *DllData)
{
    char ucMsgGetBuf[50];
    char ucDataGetBuf[50];
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)DllData;
    IDP_PrintMsgDatalog(ptDllData, ucMsgGetBuf, ucDataGetBuf);
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] Msg=%s,Frm=%d,Data=%s,ValidLen=%d,Src=[%#x:%#x:%#x],Dst=[%#x:%#x:%#x]",
        __FUNCTION__,
        ucMsgGetBuf, ptDllData->FrmType, ucDataGetBuf, ptDllData->DataLen,
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


