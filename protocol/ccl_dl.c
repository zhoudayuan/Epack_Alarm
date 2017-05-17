/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_dl.c
 * @brief   ���п��Ʋ����к����ӿ�
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-29
 */

/*
 *   �����б�
 *   1. ODP_GenSilentFrmpacket         ��װCCL���о���֡���ݰ�
 *   2. ODP_GenVoicepacket             ��װCCL����������
 *   3. ODP_GenSmsDataPacket           ��װCCL�����ж���֡
 *   4. ODP_GenLcHeader                ��װ����LCͷ
 *   5. ODP_GenRemoteCtrlDataPacket    ��װ���п����������ݰ�
 *   6. ODP_GenTerminatorPacket        ��װ��������֡terminator���ݰ�
 *   7.
 *
 *   �޸���ʷ
 *   2016-07-29            �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "ccl_dl.h"
#include "ccl.h"
#include "dll.h"
#include "ccl_fun.h"


// sms���ʹ�ӡ--by zhoudayuan
TYPE_PRINT_T table_sms_type_print[] = {
    //����Ϣ
    {MESSAGE_PRIVATE_CALL    ,   "MESSAGE_PRIVATE_CALL"},
    {MESSAGE_GROUP_CALL      ,   "MESSAGE_GROUP_CALL"},
    {STATUS_PRIVATE_CALL     ,   "STATUS_PRIVATE_CALL"},
    {STATUS_GROUP_CALL       ,   "STATUS_GROUP_CALL"},
    //����                   ,   //����
    {STUN_REQ_MS             ,   "STUN_REQ_MS"},
    {STUN_REQ_NAS            ,   "STUN_REQ_NAS"},
    {KILL_REQ_NAS            ,   "KILL_REQ_NAS"},
    {GPS_REPOTR_MS           ,   "GPS_REPOTR_MS"},
    {GPS_REPORT_NAS          ,   "GPS_REPORT_NAS"},
    {REVIVE_REQ_NAS          ,   "REVIVE_REQ_NAS"},
    {REVIVE_REQ_MS           ,   "REVIVE_REQ_MS"},
    {NEGHR_QUERY             ,   "NEGHR_QUERY"},
    //��Ӧ                   ,   //��Ӧ
    {NEGHR_QUERY_ACK         ,   "NEGHR_QUERY_ACK"},
    {GPS_REPROT_MS_ACK       ,   "GPS_REPROT_MS_ACK"},
    {GPS_REPROT_NAS_ACK      ,   "GPS_REPROT_NAS_ACK"},
    {STUN_REQ_MS_ACK         ,   "STUN_REQ_MS_ACK"},
    {STUN_REQ_NAS_ACK        ,   "STUN_REQ_NAS_ACK"},
    {KILL_REQ_NAS_ACK        ,   "KILL_REQ_NAS_ACK"},
    {REVIVE_REQ_NAS_ACK      ,   "REVIVE_REQ_NAS_ACK"},
    {REVIVE_REQ_MS_ACK       ,   "REVIVE_REQ_MS_ACK"},
    {NAS_NEAR_REPORT         ,   "NAS_NEAR_REPORT"},
    {VARIANCE_HRESHOLD       ,   "VARIANCE_HRESHOLD"},
    //����                   ,   //����
    {STUN_NAS_REPORT         ,   "STUN_NAS_REPORT"},
    {KILL_NAS_REPORT         ,   "KILL_NAS_REPORT"},
    {REVIVE_NAS_REPORT       ,   "REVIVE_NAS_REPORT"},
    //�澯                   ,   //�澯
    {DISCON_ALARM            ,   "DISCON_ALARM"},
    {MS_ALARM                ,   "MS_ALARM"},
    {DISCON_NAS_ALARM_CLEAR  ,   "DISCON_NAS_ALARM_CLEAR"},
    {MS_ALARM_CLEAR          ,   "MS_ALARM_CLEAR"}
};

TYPE_PRINT_T *pTable_sms = table_sms_type_print;  // ָ�뷽�������ļ�����
unsigned short pTable_sms_len = sizeof(table_sms_type_print)/sizeof(table_sms_type_print[0]); // sms�������鳤��


TYPE_PRINT_T table_msg_type_print[] = {
    {DI_MSG_IDLE             ,   "DI_MSG_IDLE"},               ///< ������Ϣ
    {DI_MSG_VOICE            ,   "DI_MSG_VOICE"},              ///< ������Ϣ
    {DI_MSG_DATA             ,   "DI_MSG_DATA"},               ///< ������Ϣ
    {DI_MSG_NEAR             ,   "DI_MSG_NEAR"},               ///< �ڵ���Ϣ
    {DI_MSG_NM               ,   "DI_MSG_NM"},                 ///< ������Ϣ
    {DI_MSG_WLU              ,   "DI_MSG_WLU"}                 ///< WLU������Ϣ
};

TYPE_PRINT_T *pTable_msg = table_msg_type_print;
unsigned short pTable_msg_len = sizeof(table_msg_type_print)/sizeof(table_msg_type_print[0]);


TYPE_PRINT_T table_ccl_data_print[] = {
    // �տ���������DC_MSG_DATA
    {CT_JUNK_DATA            ,   "CT_JUNK_DATA"},              ///<��������Ϣ
    {CT_PI_HEADER            ,   "CT_PI_HEADER"},              ///<PIͷ֡��ʶ
    {CT_LC_HEADER            ,   "CT_LC_HEADER"},              ///<LCͷ֡��ʶ
    {CT_LC_TERMINATOR        ,   "CT_LC_TERMINATOR"},          ///<LC�ս�֡��ʶ
    {CT_PACKET_DATA          ,   "CT_PACKET_DATA"},            ///<����Ϣ
    {CT_GPS_REPORT_REQ_MS    ,   "CT_GPS_REPORT_REQ_MS"},      ///<��̨GPS����
    {CT_GPS_REPORT_ACK_MS    ,   "CT_GPS_REPORT_ACK_MS"},      ///<��̨GPS����
    {CT_DISABLE_REQ_MS       ,   "CT_DISABLE_REQ_MS"},         ///<ҡ��
    {CT_DISABLE_ACK_MS       ,   "CT_DISABLE_ACK_MS"},         ///<ҡ��
    {CT_ENABLE_REQ_MS        ,   "CT_ENABLE_REQ_MS"},          ///<�ն˼���
    {CT_ENABLE_ACK_MS        ,   "CT_ENABLE_ACK_MS"},          ///<�ն˼���
    {CT_ALARM_REQ_MS         ,   "CT_ALARM_REQ_MS"},           ///<�ն˽����澯
    {CT_ALARM_ACK_MS         ,   "CT_ALARM_ACK_MS"},           ///<�ն˽����澯
    // ��·����������DC_MSG_WLU /��·����������DC_MSG_WLU
    {CT_GPS_REPORT_REQ_NAS   ,   "CT_GPS_REPORT_REQ_NAS"},     ///< GPS����
    {CT_GPS_REPORT_ACK_NAS   ,   "CT_GPS_REPORT_ACK_NAS"},     ///< GPS����
    {CT_STUN_REQ_NAS         ,   "CT_STUN_REQ_NAS"},           ///<ҡ��
    {CT_STUN_ACK_NAS         ,   "CT_STUN_ACK_NAS"},           ///<ҡ��
    {CT_KILL_REQ_NAS         ,   "CT_KILL_REQ_NAS"},           ///<ҡ��
    {CT_KILL_ACK_NAS         ,   "CT_KILL_ACK_NAS"},           ///<ҡ��
    {CT_ENABLE_REQ_NAS       ,   "CT_ENABLE_REQ_NAS"},         ///<WLU����
    {CT_ENABLE_ACK_NAS       ,   "CT_ENABLE_ACK_NAS"},         ///<WLU����
    {CT_STUN_RPT_NAS         ,   "CT_STUN_RPT_NAS"},           ///<WLUң��?
    {CT_KILL_RPT_NAS         ,   "CT_KILL_RPT_NAS"},           ///<WLUң���ϱ�
    {CT_ENABLE_RPT_NAS       ,   "CT_ENABLE_RPT_NAS"},         ///< WLU�����ϱ�
    {CT_NEGHR_QUERY          ,   "CT_NEGHR_QUERY"},            ///<�ڵ���Ϣ��ѯ
    {CT_NEGHR_QUERY_ACK      ,   "CT_NEGHR_QUERY_ACK"},        ///< �ڵ���Ϣ��Ӧ
    {CT_NEGHR_REPORT         ,   "CT_NEGHR_REPORT"},           ///<�ڵ���Ϣ�ϱ�
    {CT_DISCON_ALARM         ,   "CT_DISCON_ALARM"},           ///<�����澯
    {CT_DISCON_ALARM_CLEAR   ,   "CT_DISCON_ALARM_CLEAR"}      ///< �����澯���
};

TYPE_PRINT_T *pTable_data = table_ccl_data_print;
unsigned short pTable_data_len = sizeof(table_ccl_data_print)/sizeof(table_ccl_data_print[0]);



/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/
/**
 * @var g_DllGlobalCfg
 * @brief ȫ��������
 */
extern DLL_GLB_CFG_T g_DllGlobalCfg;
extern unsigned int  s_LogMsgId;
extern CCL_PRINT_T * tcclPrint;
/**
 * @var  pLogTxtFd
 * @brief �ļ����
 */
extern FILE *pLogFd;

/**
 * @var ptIPCShm
 * @brief �����ڴ�ϵͳ��Ϣ
 */
SMS_CCL_DLL_EN *SMS_CCL_DLL=NULL;
extern SHM_IPC_STRU *ptIPCShm;
extern  unsigned  char   CCL_DL_PRINT;
extern unsigned  char INF_CCL_STATE;
//extern unsigned char s_aCCcvNet[5];
unsigned  int CallId;


extern void PrintIDbyDec(unsigned char *src, unsigned char *dst);

/******************************************************************************
*   ȫ�ֱ�������
*   *************************************************************************/
/******************************************************************************
*   �ֲ���������
*   *************************************************************************/

/******************************************************************************
*   �ڲ�����ʵ��
*   *************************************************************************/

/**
* @brief   ���������յ��Ķ������ݷ�װ��CCL�����ж���֡
* @param [in] pvCenterData  ���Ķ������ݰ����
* @param [out] pvDllData  ���е���·�� �������ݰ����
* @param [out] Len  ���е���·�� �������ݳ���ָ��
* @author ţ��ϲ
* @since
* @bug
*/

void ODP_GenSmsDataPacket(unsigned  char * pvCenterData, unsigned  char*pvDllData, int *Len)
{
    SMS_CENTER_CCL_DL *   ptCenterData =(SMS_CENTER_CCL_DL *) ( pvCenterData);
    DLL_CCL_UL_T *  ptDllData =( DLL_CCL_UL_T *)( pvDllData);
    ptDllData->DataLen=ptCenterData->ValidLength+CCL_SMS_DL_HEADLEN;               //   ���� ���� =����Ϣѹ��ͷ+����Ϣ��Ч����
    SMS_CCL_DLL_EN  *  SmsToDll  =(SMS_CCL_DLL_EN*) (ptDllData->PayLoad);
    SmsToDll->Identification++;
    SmsToDll->SrcPortId=0x01;
    SmsToDll->DestPortId=0x01;
    memcpy(SmsToDll->SmsPayLoad,ptCenterData->SmsData,ptCenterData->ValidLength);
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] sms  valide len=%d", __FUNCTION__,ptCenterData->ValidLength);
    *Len= ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
}
/**
* @brief   ��װ����LCͷ
* @param [in] pvDllData ���е���·������ָ��
* @param [out] Len  ���е���·�����ݳ���ָ��
* @author ţ��ϲ
* @since
* @bug
*/

void ODP_GenLcHeader(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len)
{
    DLL_CCL_UL_T * ptDllData = (DLL_CCL_UL_T *)( pvDllData);
    DT_VOICE_LC_HEADER * ptDllPayLoad =(DT_VOICE_LC_HEADER *)ptDllData->PayLoad;
    PTT_CMD *ptCenterData=(PTT_CMD *)pvCenterData;

    ptDllData->MsgType  = DI_MSG_DATA;
    ptDllData->DataType = CT_LC_HEADER  ;
    ptDllData->FrmType  = FT_VOICE_NO;
    ptDllData->DataLen  = LC_HEADER_LEN;            //���س���=����ͷLC ����
    *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
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
    ptDllData->SrcId[0] = ptCenterData->CallingNum[0];  //ptCenterData->SenderNum[0];// ���Դ
    ptDllData->SrcId[1] = ptCenterData->CallingNum[1];  //ptCenterData->SenderNum[1];// ���Դ
    ptDllData->SrcId[2] = ptCenterData->CallingNum[2];  //ptCenterData->SenderNum[2];// ���Դ
    ptDllData->DstId[0] = ptCenterData->CalledNum[0];   //���Ŀ��
    ptDllData->DstId[1] = ptCenterData->CalledNum[1];
    ptDllData->DstId[2] = ptCenterData->CalledNum[2];

    if (1 == tcclPrint->CclDown)
    {
        ODP_CclprintLc((unsigned char *)ptDllData);
    }
}

/**
* @brief  ��װ��������·��������
* @param [in] pvCenterData  �����������ݰ�ָ��
* @param [in] FrmType ����֡����
* @param [in] pvDllData ���е���·���������ݰ�ָ��
* @param [out] Len  ���е���·������������ָ��
* @author ţ��ϲ
* @since
* @bug
*/

void ODP_GenVoicepacket(unsigned char *pvCenterData, unsigned char *pvDllData, unsigned char FrmType, int *Len)
{
    CENTER_VOICE_DATA*ptCenterData=(CENTER_VOICE_DATA*)pvCenterData;
  //CENTER_VOICE_DATA_DL*ptCenterData=( CENTER_VOICE_DATA_DL*)pvCenterData;
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)pvDllData;
    ptDllData->MsgType = DI_MSG_VOICE;
    ptDllData->FrmType = FrmType;
    ptDllData->DataType = CT_JUNK_DATA;

    if (1 == tcclPrint->CclDown)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]dl voice FrmType=%d", __FUNCTION__,FrmType);
    }
    ptDllData->DataLen = CENTER_VOICE_DL_PAYLOADLEN;
    memcpy(ptDllData->PayLoad, ptCenterData->Payload, CENTER_VOICE_DL_PAYLOADLEN);
    *Len = CENTER_VOICE_DL_PAYLOADLEN+DLL_CCL_MSG_HEADLEN;
}


/**
* @brief  ��С��ת��
* @param [in] pvdlldata  ����DLL����
* @param [in] pvCenterData�������ж���Ϣ����
* @param [out] pvDllData ���е���·���������ݰ�ָ��
* @param [out] datalen  ���е���·������������ָ��
* @author ţ��ϲ
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
* @brief  ��װ����֡���ݰ�
* @param [in] pvSilentData  ����֡����ָ��
* @param [in] FrmType����֡֡����
* @param [out] pvDllData ���е���·���������ݰ�ָ��
* @param [out] Len  ���е���·������������ָ��
* @author ţ��ϲ
* @since
* @bug
*/

void  ODP_GenSilentFrmpacket(unsigned  char *pvSilentData,unsigned  char *pvDllData,unsigned  char FrmType , int *Len)
{
    DLL_CCL_UL_T *ptDllData=(DLL_CCL_UL_T *)pvDllData;
    ptDllData->MsgType=DI_MSG_VOICE;
    ptDllData->FrmType =FrmType;
    ptDllData->DataLen=CENTER_VOICE_DL_PAYLOADLEN;
    *Len=SILENT_FRAME_DATA_LEN+DLL_CCL_MSG_HEADLEN;
    memcpy(ptDllData->PayLoad,pvSilentData,CENTER_VOICE_DL_PAYLOADLEN);
}
/**
* @brief  ��װң���������ݰ�
* @param [in] pvCenterData  ������������
* @param [out] pvDllData ���е���·�����ݰ�ָ��
* @param [out] Len  ���е���·������������ָ��
* @author ţ��ϲ
* @since
* @bug
*/

void ODP_GenSigCtrlDataPacket(unsigned char *pvCenterData, unsigned char *pvDllData, int *Len)
{
    unsigned char SigCtrlType;
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)pvDllData;
    SMS_CENTER_CCL_DL *ptCenterData = (SMS_CENTER_CCL_DL *)pvCenterData;
    SigCtrlType = ptCenterData->SmsType;
    memset(ptDllData, 0, sizeof(CCL_DLL_DL_T));
    ptDllData->SrcId[0] = ptCenterData->SenderNum[0];
    ptDllData->SrcId[1] = ptCenterData->SenderNum[1];
    ptDllData->SrcId[2] = ptCenterData->SenderNum[2];
    ptDllData->DstId[0] = ptCenterData->ReceiverNum[0];
    ptDllData->DstId[1] = ptCenterData->ReceiverNum[1];
    ptDllData->DstId[2] = ptCenterData->ReceiverNum[2];
    //SavCcIDNetData(ptCenterData,);

    switch (SigCtrlType)
    {
        case STUN_REQ_NAS:  // ң���ŵ���
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_STUN_REQ_NAS;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            if ((ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId) && (ptDllData->DstId[1] == 0) && (ptDllData->DstId[2] == 0))
            {
                INF_CCL_STATE = Wait_Stun_Nas_Ack;  // �鿴���ؽڵ� ���Ǳ��ؽڵ��򲻸ұ���п��Ʋ�״̬
            }
            break;
        }

        case KILL_REQ_NAS:  // ң���ŵ���
        {
            ptDllData->MsgType=DI_MSG_WLU;
            ptDllData->DataType=CT_KILL_REQ_NAS ;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                INF_CCL_STATE=Wait_Kill_Nas_Ack;
            }
            break;
        }

        case REVIVE_REQ_NAS:
        {
            ptDllData->MsgType=DI_MSG_WLU;
            ptDllData->DataType=CT_ENABLE_REQ_NAS;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                INF_CCL_STATE=Wait_Reviv_NAS_Ack;
            }
            break;
        }

        case REVIVE_REQ_MS:
        {
            ptDllData->MsgType=DI_MSG_DATA;
            ptDllData->DataType=CT_ENABLE_REQ_MS;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            // if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            // {
                INF_CCL_STATE=Wait_Reviv_MS_Ack;
            //}
            break;
        }

        case STUN_REQ_MS:
        {
            ptDllData->MsgType=DI_MSG_DATA;
            ptDllData->DataType= CT_DISABLE_REQ_MS;                 //ң���ն�  �ŵ���ҵ������
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            // if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            // {
            INF_CCL_STATE=Wait_Stun_Ms_Ack;
            // }
            //INF_CCL_STATE=Wait_Stun_Ms_Ack;
            break;
        }

        case GPS_REPOTR_MS:   // �ն�GPS����
        {
            CallId++;
            ptDllData->MsgType=DI_MSG_DATA;
            ptDllData->DataType=CT_GPS_REPORT_REQ_MS;
            ptDllData->DataLen=ptCenterData->ValidLength+4;
            memcpy(ptDllData->PayLoad,(unsigned char *)&CallId,4);
            if(CC_CCL_MSGPSRPT_PAYLODDLEN <= ptCenterData->ValidLength)
            {
                LOG_WARNING(s_LogMsgId,"[CCL][%s]CC_GPS_RPT_MS LOAD ERR   LEN=%d", __FUNCTION__,ptCenterData->ValidLength);
                ptCenterData->ValidLength=CC_CCL_MSGPSRPT_PAYLODDLEN;
            }
            memcpy(&ptDllData->PayLoad[4],ptCenterData->SmsData,ptCenterData->ValidLength);
            //if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            //{
                INF_CCL_STATE=Wait_Gps_Ms_Ack;
      //}
            //INF_CCL_STATE=Wait_Gps_Ms_Ack;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        case NEGHR_QUERY:
        {
            ptDllData->MsgType = DI_MSG_WLU;
            ptDllData->DataType = CT_NEGHR_QUERY;
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                INF_CCL_STATE = Wait_NEGHR_Nas_Ack;
            }
            //INF_CCL_STATE=Wait_NEGHR_Nas_Ack;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }

        case GPS_REPORT_NAS:
        {
            ptDllData->MsgType=DI_MSG_WLU;
            ptDllData->DataType=CT_GPS_REPORT_REQ_NAS;
            ptDllData->SrcId[0]=g_DllGlobalCfg.auNodeId;//??????????????????????
            ptDllData->SrcId[1]=0;
            ptDllData->SrcId[2]=0;

            ptDllData->DataLen=8;
            memset(ptDllData->PayLoad,0,sizeof(ptDllData->PayLoad));
            if(ptDllData->DstId[0] != g_DllGlobalCfg.auNodeId && ptDllData->DstId[1] == 0 && ptDllData->DstId[2] == 0)
            {
                INF_CCL_STATE=Wait_Gps_Nas_Ack;
            }

            //INF_CCL_STATE=Wait_Gps_Nas_Ack;

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
            //memcpy(SMS_CCL_DLL->SmsPayLoad,ptCenterData->SmsData,ptCenterData->ValidLength);
            ptDllData->DataLen=ptCenterData->ValidLength +5;
            *Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        case MS_ALARM:   //���и澯
        {
            ptDllData->MsgType  = DI_MSG_DATA;
            ptDllData->DataType = CT_ALARM_REQ_MS;
            ptDllData->DataLen  = 0;
            *Len = ptDllData->DataLen + DLL_CCL_MSG_HEADLEN;
            break;
        }

        case MS_ALARM_CLEAR:   //�澯���
        {
            ptDllData->MsgType  = DI_MSG_DATA;
            ptDllData->DataType = CT_ALARM_ACK_MS;
            ptDllData->DataLen  = 0;
            *Len = ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
            break;
        }
        default:
        {
            *Len=0;
            LOG_DEBUG(s_LogMsgId,"[CCL][%s] ERROR SigCtrlsmsType=%d!", __FUNCTION__,SigCtrlType);
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
    ptDllData->MsgType=DI_MSG_DATA;
    ptDllData->DataType=CT_LC_TERMINATOR  ;
    ptDllData->FrmType =FT_VOICE_NO;
    ptDllData->DataLen=LC_HEADER_LEN;            //���س���=����ͷLC ����
    * Len=ptDllData->DataLen+DLL_CCL_MSG_HEADLEN;
    ptDllPayLoad->Reserved=0;
    ptDllPayLoad->ProtectFlg=0;
    ptDllPayLoad->CtrOpcode=0x00;
    ptDllPayLoad->FeatureId=0x00;
    ptDllPayLoad->ServiceOpt=0x00;
    ptDllPayLoad->GroupAddr[0]=ptCenterData->CalledNum[0];
    ptDllPayLoad->GroupAddr[1]=ptCenterData->CalledNum[1];
    ptDllPayLoad->GroupAddr[2]=ptCenterData->CalledNum[2];
    ptDllPayLoad->SourceAddr[0]=ptCenterData->CallingNum[0];
    ptDllPayLoad->SourceAddr[1]=ptCenterData->CallingNum[1];
    ptDllPayLoad->SourceAddr[2]=ptCenterData->CallingNum[2];

    ptDllData->SrcId[0]=ptCenterData->CallingNum[0];
    ptDllData->SrcId[1]=ptCenterData->CallingNum[1];
    ptDllData->SrcId[2]=ptCenterData->CallingNum[2];
    ptDllData->DstId[0]=ptCenterData->CalledNum[0];
    ptDllData->DstId[1]=ptCenterData->CalledNum[1];
    ptDllData->DstId[2]=ptCenterData->CalledNum[2];
     if(1 == tcclPrint->CclDown)
    {
      ODP_CclprintLc((unsigned char *)ptDllData);
    }

}


/**
* @brief    �������ĺ���ID������������
* @param [in] pvCenterData  ������������
* @author ţ��ϲ
* @since
* @bug
*/


void SavCcIDNetData(unsigned char *pvcenterdata, unsigned char *CCcvNetid)
{

    SMS_CENTER_CCL_DL *ptcenterdata = (SMS_CENTER_CCL_DL *)pvcenterdata;
    //SMS_CENTER_CCL_DL *ptcenterdata=(SMS_CENTER_CCL_DL *)centerdata;
    CC_CVID_SUBNET_DATA * ptCVNetid = (CC_CVID_SUBNET_DATA *)CCcvNetid;
    ptCVNetid->CallId  = ptcenterdata->CallId;
    ptCVNetid->VoiceId = ptcenterdata->VoiceId;
    ptCVNetid->SubNet  = ptcenterdata->SmsData[0];
    ptCVNetid->srcid[0]  = ptcenterdata->SenderNum[0];
    ptCVNetid->srcid[1]  = ptcenterdata->SenderNum[1];
    ptCVNetid->srcid[2]  = ptcenterdata->SenderNum[2];
    ptCVNetid->destid[0] = ptcenterdata->ReceiverNum[0];
    ptCVNetid->destid[1] = ptcenterdata->ReceiverNum[1];
    ptCVNetid->destid[2] = ptcenterdata->ReceiverNum[2];

    if (1 == tcclPrint->CclDown)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] CallId=%#x, VoiceId=%#x, SubNet=%#x, snd=[%#x:%#x:%#x], rcv=[%#x:%#x:%#x]",
            __FUNCTION__,
            ptCVNetid->CallId,
            ptCVNetid->VoiceId,
            ptCVNetid->SubNet,
            ptCVNetid->srcid[0],  ptCVNetid->srcid[1],  ptCVNetid->srcid[2],
            ptCVNetid->destid[0], ptCVNetid->destid[1], ptCVNetid->destid[2]);
    }
}


/**
* @brief    ��ӡLC_HEADER ��LC_TERMINATOR
* @param [in] ptDllData  ���͵�DLL ����
* @author ţ��ϲ
* @since
* @bug
*/
void ODP_CclprintLc(unsigned char * pvDllData)
{
    DLL_CCL_UL_T * ptDllData=(DLL_CCL_UL_T * )pvDllData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] LC MsgType=%d,DataType=%d,FrmType=%d", 
        __FUNCTION__,
        ptDllData->MsgType,
        ptDllData->DataType,
        ptDllData->FrmType);
     
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] LC Src[0-2]=[%d:%d:%d] Dst[0-2]=[%d:%d:%d] dlen=%d",
        __FUNCTION__,
        ptDllData->SrcId[0], ptDllData->SrcId[1],ptDllData->SrcId[2],
        ptDllData->DstId[0], ptDllData->DstId[1],ptDllData->DstId[2],
        ptDllData->DataLen);
        PrintIDbyDec(ptDllData->SrcId, ptDllData->DstId);
}


/**
* @brief  ��ӡ��������
* @param [in] pvPttcmd ������������
* @author ţ��ϲ
* @since
* @bug
*/
void ODP_CclPrintCcSig(unsigned char *Pttcmd)
{
    unsigned short i;
    SHARE_CC_DATA_D *CcDatadl = (SHARE_CC_DATA_D *)Pttcmd;
    PTT_CMD *pvPttcmd=(PTT_CMD *)Pttcmd;
    unsigned char sigtype = pvPttcmd->SharedHead.SigType;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SIGTYPE:%#x, DLEN:%d", __FUNCTION__, pvPttcmd->SharedHead.SigType, pvPttcmd->SharedHead.Datalength);

    if( SIG_PTT_CMD == sigtype)
    {
        LOG_DEBUG(s_LogMsgId, "[CCL][%s] pttState:%#x caling[0-2]=[%#x:%#x:%#x] Called[0-2]=[%#x:%#x:%#x]",
            __FUNCTION__,
            CcDatadl->CC_PTT_CMD.PttStat,
            CcDatadl->CC_PTT_CMD.CallingNum[0], CcDatadl->CC_PTT_CMD.CallingNum[1], CcDatadl->CC_PTT_CMD.CallingNum[2],
            CcDatadl->CC_PTT_CMD.CalledNum[0],  CcDatadl->CC_PTT_CMD.CalledNum[1],  CcDatadl->CC_PTT_CMD.CalledNum[2]);
    }
    else if (SIG_PTT_ON_ACK ==sigtype)
    {
        LOG_DEBUG(s_LogMsgId, "[CCL][%s] caling[0-2]=[%#x:%#x:%#x], Called[0-2]=[%#x:%#x:%#x]",
            __FUNCTION__,
            CcDatadl->CC_Ptt_Ack.CallingNum[0], CcDatadl->CC_Ptt_Ack.CallingNum[1], CcDatadl->CC_Ptt_Ack.CallingNum[2],
            CcDatadl->CC_Ptt_Ack.CalledNum[0],  CcDatadl->CC_Ptt_Ack.CalledNum[1],  CcDatadl->CC_Ptt_Ack.CalledNum[2]);
    }
    else if (SIG_SMS_SEND == sigtype)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] SmsType=%#x, validlen=%d, SND[0-2]=[%d:%d:%d], RCV[0-2]=[%d:%d:%d]",
            __FUNCTION__,
            CcDatadl->CcSmsSig.SmsType, CcDatadl->CcSmsSig.ValidLength,
            CcDatadl->CcSmsSig.SenderNum[0], CcDatadl->CcSmsSig.SenderNum[1],CcDatadl->CcSmsSig.SenderNum[2],
            CcDatadl->CcSmsSig.ReceiverNum[0],CcDatadl->CcSmsSig.ReceiverNum[1],CcDatadl->CcSmsSig.ReceiverNum[2]);

        for (i = 0; i< sizeof(table_sms_type_print)/sizeof(table_sms_type_print[0]); i++)
        {
            if (CcDatadl->CcSmsSig.SmsType == table_sms_type_print[i].Type)
            {
                if (table_sms_type_print[i].TypeStr != NULL)
                {
                    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SMS:[%#x-%s]", __FUNCTION__, table_sms_type_print[i].Type, table_sms_type_print[i].TypeStr);
                    break;
                }
            }
        }
    }
    else if (SIG_PTT_OFF_ACK == sigtype)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] REC sig type SIG_PTT_OFF_ACK ",__FUNCTION__);
    }
    else
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] REC sig type error ",__FUNCTION__);

    }

}

// by zhoudayuan
void PrintMsgDatalog(DLL_CCL_UL_T *ptDllData)
{
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    // ��ӡMsg
    for (i = 0; i < pTable_msg_len; i++)
    {
        if (ptDllData->MsgType == pTable_msg[i].Type)
        {
            if (pTable_msg[i].TypeStr != NULL)
            {
                i_index = i; 
                break;
            }
        }
    }

    // ��ӡdata
    for (i = 0; i < pTable_data_len; i++)
    {
        if (ptDllData->DataType == pTable_data[i].Type)
        {
            if (pTable_data[i].TypeStr != NULL)
            {
                j_index = j; 
                break;
            }
        }
    }
    
    if ((i_index != -1) && (j_index != -1))
    {
        snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#06x-%s], DATA:[%#04x-%s]", pTable_msg[i_index].Type, pTable_msg[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
        LOG_DEBUG(s_LogMsgId, "[DLL][%s] %s", _F_, LogBuff);
    }
}



/**
* @brief    ��ӡ ccl_dll ����
* @param [in] ptDllData  CCL��������
* @author ţ��ϲ
* @since
* @bug
*/
void ODP_CclPrintDllData(unsigned char *DllData)
{
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)DllData;
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] MSGTYPE=%#x, DataType=%#x, DataLen=%#x", __FUNCTION__, ptDllData->MsgType, ptDllData->DataType, ptDllData->DataLen);
    PrintMsgDatalog(ptDllData);
    PrintIDbyDec(ptDllData->SrcId, ptDllData->DstId);
}



