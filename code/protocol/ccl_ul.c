/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    cclul.c
 * @brief   ���п��Ʋ����к����ӿ�
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-29
 */

/*
 *   �����б�
 *   1. IDP_GenPttOnAck          ��װPTT-ON ����ظ����ݰ�
 *   2. IDP_GenPttOffAck         ��װPTT-OFF ����ظ����ݰ�
 *   3. IDP_GenPttOnCmd          ��װPTT-ON�����
 *   4. IDP_GenVoiceDataPaket    ��װ����������
 *   5.
 *   6.
 *   7.
 *
 *   �޸���ʷ
 *   2016-07-29            �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "ccl_ul.h"
#include "ccl.h"
#include "dll.h"
#include "ccl_fun.h"
#include "print_debug.h"



/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/
 extern SHM_IPC_STRU *ptIPCShm;
/**
 * @var  pLogTxtFd
 * @brief �ļ����
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
 * @brief ȫ��������
 */
extern DLL_GLB_CFG_T g_DllGlobalCfg;
/**
 * @brief ���Դ�ӡ��Ϣ
 */




 /******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/
/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/
 SMS_CCL_DLL_EN *SMS_DLL_CCL=NULL;
 extern  unsigned char s_aLcdata[20];
 extern bool s_SndPttOnFlg;
 LC_HEADER_DATA  LC_DATA;
 unsigned int ThresholdSum=0;


/******************************************************************************
 *   �ڲ�����ʵ��
 *   *************************************************************************/
 /**
 * @brief   ��װPTT-ON ����ظ����ݰ�
 * @param [in] PttCmdResult   PTT�ظ�״̬
 * @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
 * @param [out] Len  ���е����ĵ����ݰ�����ָ��
 * @author ţ��ϲ
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
 * @brief   ��װPTT-OFF ����ظ����ݰ�
 * @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
 * @param [out] Len  ���е����ĵ����ݰ�����ָ��
 * @author ţ��ϲ
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
 * @brief   ��װNAS״̬�ϱ����ݰ�
 * @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
 * @param [out] Len  ���е����ĵ����ݰ�����ָ��
 * @author ţ��ϲ
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
 * @brief ��װPTT �������ݰ�
 * @param [in] pvDllData   DLL��������
 * @param [in] cmd      PTT�����ʶ
 * @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
 * @param [out] Len  ���е����ĵ����ݰ�����ָ��
 * @author ţ��ϲ
 * @since
 * @bug
 */
void IDP_GenPttCmd(unsigned char *pvDllData,unsigned char *pvCenterData,unsigned char cmd,int *Len)
{
    PTT_CMD *ptCenterData = (PTT_CMD *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;          //��Ϣͷ
    ptCenterData->SharedHead.SigType = 0x000a;          //PTT����
    ptCenterData->SharedHead.Datalength = 100;          //������Ч����
    ptCenterData->SharedHead.CheckVal = 0xffff;         //01��02�汾ȫ����дF ��ʶ��У��
    ptCenterData->CalledNum[0]  = LC_DATA.DesId[0];
    ptCenterData->CalledNum[1]  = LC_DATA.DesId[1];
    ptCenterData->CalledNum[2]  = LC_DATA.DesId[2];
    ptCenterData->CallingNum[0] = LC_DATA.SourceId[0];
    ptCenterData->CallingNum[1] = LC_DATA.SourceId[1];
    ptCenterData->CallingNum[2] = LC_DATA.SourceId[2];

    //PPT  ����  ��ʶPTT����״̬������CMD_ON=0x06���ͷ�CMD_OFF=0x07
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
* @brief    ��װ���������Ķ���Ϣ
* @param [in] pvDllData DLL��������
* @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
* @param [out] Len  ���е����ĵ����ݰ�����ָ��
* @author ţ��ϲ
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
* @brief    ����LC ����  ���ж��Ƿ�ֹͣת��
* @param [in] pvDllData   DLL������������
* @author ţ��ϲ
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
* @brief    ����������ӦPTTON �����
* @param [out] pvCenterData  ������������
* @author ţ��ϲ
* @since
* @bug
*/

void IDP_PttOnAckHandle(unsigned char *pvCenterData)
{
    PTT_ON_ACK *ptCenterData =(PTT_ON_ACK *)pvCenterData;
    if (PPT_ACK_OK == ptCenterData->Ack)
    {
        s_SndPttOnFlg = 1;//�������ĳɹ���������PTT_ON
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
* @brief    ��װ�澯�������Ϣ
* @param [out] pvCenterData  ������������
* @param [in] type  �ϱ���Ϣ����
* @param [in] pvDllData DLL����������
* @author ţ��ϲ
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
* @brief    ��װMSң�Ρ�������Ӧ���ݰ�
* @param [out] pvCenterData  ������������
* @param [in] pvDllData DLL����������
* @author ţ��ϲ
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
* @brief    ��װNAS ң�Ρ�ң�С�������Ӧ���ݰ�
* @param [out] pvCenterData  ������������
* @param [in] pvDllData DLL����������
* @author ţ��ϲ
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

    if( CT_STUN_ACK_NAS == pvSigType) //ң��NAS
    {
        ptCenterData->SmsType = STUN_REQ_NAS_ACK;
    }
    else if(CT_KILL_ACK_NAS == pvSigType) //ң��NAS
    {
        ptCenterData->SmsType = KILL_REQ_NAS_ACK;
    }
    else if(CT_ENABLE_ACK_NAS == pvSigType)  //����NAS
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
* @brief    ��װ����������GPS ���ݰ�
* @param [in] pvDllData   DLL������������
* @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
* @param [out] Len  ���е����ĵ����ݰ�����ָ��
* @author ţ��ϲ
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

    pttCcData->SharedHead.SigHead = 0xec13;     // AC�ӿ�����ͷ��־
    pttCcData->VoiceId = ptCcCaldNet->VoiceId;  // ҵ����ID
    pttCcData->CallId = ptCcCaldNet->CallId;    // ������ID
    pttCcData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_DL) - CENTER_SHARE_HEAD_LEN;  // ��Ч���ݳ���
    pttCcData->SharedHead.SigType = SIG_SMS_SEND;  // ����Ϣ����

    if (CT_GPS_REPORT_ACK_NAS == pvSigType)  // NAS
    {
        pttCcData->SmsType = GPS_REPROT_NAS_ACK;  // ����Ϣ����
        ptSmsGps->CALLEDID[0] = 0;
        ptSmsGps->CALLEDID[1] = 0;
        ptSmsGps->CALLEDID[2] = 0;
        ptSmsGps->CALLEDID[3] = ptCcCaldNet->destid[0];
        // [����][����]ID ��ֵ
        pttCcData->SenderNum[0] = ptDllData->SrcId[0];
        pttCcData->SenderNum[1] = ptDllData->SrcId[1];
        pttCcData->SenderNum[2] = ptDllData->SrcId[2];
        pttCcData->ReceiverNum[0] = ptDllData->DstId[0];
        pttCcData->ReceiverNum[1] = ptDllData->DstId[1];
        pttCcData->ReceiverNum[2] = ptDllData->DstId[2];
        // GPSʱ��
        GetTime(ptSmsGps->GPSCCLDATA.DATE);   //���GPS ʱ�䣬ͬʱ����ϵͳʱ�䣬����ӵ��ṹ����
        // ��Ч���ݳ���
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T);
        // ��Ч����
        memcpy(&ptSmsGps->GPSCCLDATA, ptDllData->PayLoad, ptDllData->DataLen);    // ʵ�ʵ�GPS����

        *Len = sizeof(SMS_CENTER_CCL_DL);  // ��Ч���Ȼ������峤���д���ͨ

    }
    else if (CT_GPS_REPORT_ACK_MS == pvSigType) // MS
    {
        pttCcData->SmsType = GPS_REPROT_MS_ACK;  // ����Ϣ����
        ptSmsGps->CALLEDID[0] = ptCcCaldNet->SubNet;
        ptSmsGps->CALLEDID[1] = ptCcCaldNet->destid[0];
        ptSmsGps->CALLEDID[2] = ptCcCaldNet->destid[1];
        ptSmsGps->CALLEDID[3] = ptCcCaldNet->destid[2];
        /*
        ** ����\����ID ��ֵ
        */ 
        pttCcData->SenderNum[0] = ptCcCaldNet->destid[0];   // ���, ���� 9002
        pttCcData->SenderNum[1] = ptCcCaldNet->destid[1];   // ���
        pttCcData->SenderNum[2] = ptCcCaldNet->destid[2];   // ���
        pttCcData->ReceiverNum[0] = ptCcCaldNet->srcid[0];  // ��̨���豸
        pttCcData->ReceiverNum[1] = ptCcCaldNet->srcid[1];  // ��̨���豸
        pttCcData->ReceiverNum[2] = ptCcCaldNet->srcid[2];  // ��̨���豸
        // GPSʱ��, ��ӵ�����ID��
        GetTime(ptSmsGps->GPSCCLDATA.DATE);   //���GPS ʱ�䣬ͬʱ����ϵͳʱ�䣬����ӵ��ṹ����
        // ��Ч���ݳ���
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T);
        // ��Ч����
        memcpy(&ptSmsGps->GPSCCLDATA, ptDllData->PayLoad, ptDllData->DataLen);    // ʵ�ʵ�GPS����

        *Len = sizeof(SMS_CENTER_CCL_DL);  // ��Ч���Ȼ������峤���д���ͨ
    }
    else if (CT_VOICE_EMB_GPS == pvSigType)  // ������ǶGPS(MS or Nas)
    {
        
        GPS_DATA_CC_T *ptCcGpsSnd = ptSmsGps;   // ����-���͵�CC
        GPS_DATA_CC_T *ptCcGpsRcv = ptSmsGps+1; // ����-���͵�CC
        GPS_DATA_CCL_T *ptCclGpsCalling = &ptCcGpsSnd->GPSCCLDATA;  // ָ������GPS��Ϣ - ��γ��+ʱ��
        GPS_DATA_CCL_T *ptCclGpsCalled  = &ptCcGpsRcv->GPSCCLDATA;  // ָ�򱻽�GPS��Ϣ - ��γ��+ʱ��
        GPS_NAS_DATA_T *ptDllGpsSrc = (GPS_NAS_DATA_T *)ptDllData->PayLoad;  // ����-����DLL
        GPS_NAS_DATA_T *ptDllGpsDst = (GPS_NAS_DATA_T *)(ptDllData->PayLoad + sizeof(GPS_NAS_DATA_T)); // ����-����DLL

        pttCcData->SmsType = GPS_EMB_VOICE_REPORT;  // ����Ϣ���ͣ������������
        /*
        ** ID ��ֵ
        */ 
        pttCcData->SenderNum[0] = ptDllData->SrcId[0];      // ���, ���� 9002
        pttCcData->SenderNum[1] = ptDllData->SrcId[1];      // ���
        pttCcData->SenderNum[2] = ptDllData->SrcId[2];      // ���
        pttCcData->ReceiverNum[0] = ptDllData->DstId[0];    // ��̨���豸
        pttCcData->ReceiverNum[1] = ptDllData->DstId[1];    // ��̨���豸
        pttCcData->ReceiverNum[2] = ptDllData->DstId[2];    // ��̨���豸
        /* 
        ** ��Ч���ݳ��� , ����GPS��Ϣ + ����GPS��Ϣ
        */ 
        pttCcData->ValidLength = sizeof(GPS_DATA_CC_T) + sizeof(GPS_DATA_CC_T);  // Src + Dst
        /* 
        ** Src-����GPS��Ϣ, [����]GPSʱ��, IDΪ����ID����MS��NAS
        */ 
        ptCcGpsSnd->CALLEDID[0] = 0;   
        ptCcGpsSnd->CALLEDID[1] = ptDllData->SrcId[0];
        ptCcGpsSnd->CALLEDID[2] = ptDllData->SrcId[1];
        ptCcGpsSnd->CALLEDID[3] = ptDllData->SrcId[2];
        memcpy(ptCclGpsCalling, ptDllGpsSrc, ptDllData->DataLen);  // ��������GPS��Ϣ
        memset(ptCclGpsCalling->DATE, 0, sizeof(ptCcGpsSnd->GPSCCLDATA.DATE));  // ������ʱ��, �����������кͱ���
        /* 
        ** Dst-����GPS��Ϣ, [��]GPSʱ��, IDΪ���У����Լ���ID
        */ 
        ptCcGpsRcv->CALLEDID[0] = 0;  
        ptCcGpsRcv->CALLEDID[1] = g_DllGlobalCfg.auNodeId;
        ptCcGpsRcv->CALLEDID[2] = 0;
        ptCcGpsRcv->CALLEDID[3] = 0;
        memcpy(ptCclGpsCalled, ptDllGpsDst, ptDllData->DataLen); // ��������GPS��Ϣ
        /*
        ** ʱ��һ��Ҫ���������Ϊ�ڴ��غ�
        */
        GetTime(ptCcGpsRcv->GPSCCLDATA.DATE);  // ���GPSʱ��(�ַ�����ʽ)��ͬʱ����ϵͳʱ��
        *Len = sizeof(SMS_CENTER_CCL_DL);  
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]GPS DATATYPE ERROR ", __FUNCTION__);
    }

    // ����
    if (1 == tcclPrint->CclUp)
    {
        IDP_CclSmsPrint((unsigned char *)pttCcData);
        IDP_CclPrintGpsData(pttCcData->SmsData);
        LOG_PrintM(s_LogMsgId,pttCcData->SmsData,30);
    }
}

/**
* @brief    ��װ����������GPS ���ݰ�
* @param [in] pvDllData   DLL������������
* @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
* @param [out] Len  ���е����ĵ����ݰ�����ָ��
* @author ţ��ϲ
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
    if(NEAR_REPORT_PASSIVE ==neartyppe)  //�����ϱ� ���ڵ��ѯPALOAD  ���ݽṹһ��
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
* @brief    ��װ���������ĳ�ǿ���ݰ�
* @param [in] pvDllData   DLL������������
* @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
* @param [out] Len  ���е����ĵ����ݰ�����ָ��
** @param [in] pvthrethhold ��ǿ����ָ��
* @author ţ��ϲ
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
* @brief    ��װ����������������
* @param [in] pvDllData   DLL������������
* @param [out] pvCenterData  ���е����ĵ����ݰ�ָ��
* @param [out] Len  ���е����ĵ����ݰ�����ָ��
* @author ţ��ϲ
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
 * @brief   ��ӡPTT ������Ӧ����
 * @param [in] pvCenterData  ���е����ĵ��������ݰ�ָ��
 * @author ţ��ϲ
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
 * @brief   ��ӡPTT �����
 * @param [in] pvCenterData  ���е����ĵ��������ݰ�ָ��
 * @author ţ��ϲ
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
* @brief    ��ӡccl_cc ����Ϣ����
* @param [in] pvCenterData  ���е����ĵĶ���Ϣ����
* @author ţ��ϲ
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
* @brief    ��ӡ dll_ccl ����
* @param [in] ptDllData  DLL��������
* @author ţ��ϲ
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


