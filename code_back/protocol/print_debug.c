#include <arpa/inet.h>
#include <stdio.h>
#include "print_debug.h"
/**
 * @var  s_i4LogMsgId
 * @brief �������׽���
 */
extern INT32 s_LogMsgId;
/**
 * @var ptIPCShm
 * @brief �����ڴ�ϵͳ��Ϣ
 */
extern SHM_IPC_STRU *ptIPCShm;
/**
 * @var tcclPrint
 * @brief CCL��ӡ����
 */
extern CCL_PRINT_T * tcclPrint;
/**
 * @var tDllPrint
 * @brief DLL��ӡ����
 */
extern DLL_PRINT_T *tDllPrint;
/**
 * @var
 * @brief ������Դ����
 */
extern UINT8 s_au2VoiceSource[3][6][27];
/**
 * @var
 * @brief ����������
 */
extern UINT16 ones16(UINT16 u2Data);
/**
 * @var
 * @brief ����������
 */
FILE *g_fp_err_test;



UINT8  g_ErrVoiceFlag = ERR_VOICE_START;




#if 0
extern TYPE_PRINT_T *pTable_sms;
extern unsigned short pTable_sms_len;
extern TYPE_PRINT_T *pTable_msg;
extern unsigned short pTable_msg_len;
extern TYPE_PRINT_T *pTable_data;
extern unsigned short pTable_data_len;
if (tDllPrint->CCUp == 1)
if (tDllPrint->CCDown == 1)
if (tDllPrint->AIUp == 1)
if (tDllPrint->AIDown == 1)

#endif




//------------------------����------------------------
// table-0 sms-���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_sig_type_print[] = {
    {SIG_STATUS_REPORT   ,  "SIG_STATUS_REPORT"},       // NAS״̬�ϱ�
    {SIG_PTT_CMD         ,  "SIG_PTT_CMD"},             // PTT��������
    {SIG_PTT_ON_ACK      ,  "SIG_PTT_ON_ACK"},          // PTTON�ظ�����
    {SIG_SMS_SEND        ,  "SIG_SMS_SEND"},            // ����Ϣ����
    {SIG_SMS_ACK         ,  "SIG_SMS_ACK"},             // ����Ϣ�ظ�����
    {SIG_PTT_OFF_ACK     ,  "SIG_PTT_OFF_ACK"},         // PTTOFF�ظ�����
};

TYPE_PRINT_T *pTable_sig = table_sig_type_print;
unsigned short pTable_sig_len = sizeof(table_sig_type_print)/sizeof(table_sig_type_print[0]);



// table-1 sms-���͵��Ա�--by zhoudayuan
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
TYPE_PRINT_T *pTable_sms = table_sms_type_print;
unsigned short pTable_sms_len = sizeof(table_sms_type_print)/sizeof(table_sms_type_print[0]);




// table-2 msg-���͵��Ա�--by zhoudayuan
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


// table-3 ccl data-���͵��Ա�--by zhoudayuan
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


// table-4 cmd ���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_cmd_type_print[] = {
#if 0
    //���ܿտ�����
    {CMD_CODE_DEV_FREQ                ,    "CMD_CODE_DEV_FREQ"},
    {CMD_CODE_DEV_POWER               ,    "CMD_CODE_DEV_POWER"},
    {CMD_CODE_START_NEIGHBOR          ,    "CMD_CODE_START_NEIGHBOR"},
    {CMD_CODE_NEIGHBOR_PERIOD         ,    "CMD_CODE_NEIGHBOR_PERIOD"},
    {CMD_CODE_COMBINED_DATA           ,    "CMD_CODE_COMBINED_DATA"},
    {CMD_CODE_SCAN_MODE               ,    "CMD_CODE_SCAN_MODE"},          //��ǰ��������0x0006����ʱû�ú��ڿ���ע���޸�

    //�澯ģ��������

    {CMD_CODE_ALARM                  ,    "CMD_CODE_ALARM"},
#endif
    //ҵ��ģ��������C0~FF
    {CMD_CODE_GPS_REPORT              ,    "CMD_CODE_GPS_REPORT"},
    {CMD_CODE_STUN                    ,    "CMD_CODE_STUN"},
    {CMD_CODE_KILL                    ,    "CMD_CODE_KILL"},
    {CMD_CODE_ENABLE                  ,    "CMD_CODE_ENABLE"},
    {CMD_CODE_NER_QUERY               ,    "CMD_CODE_NER_QUERY"},
    {CMO_CODE_NER_REPORT              ,    "CMO_CODE_NER_REPORT"},
    {CMO_CODE_MS_GPS                  ,    "CMO_CODE_MS_GPS"},
    {CMO_CODE_MS_DISABlE              ,    "CMO_CODE_MS_DISABlE"},
    {CMO_CODE_MS_ENABLE               ,    "CMO_CODE_MS_ENABLE"},
    {CMO_CODE_NAS_PRE                 ,    "CMO_CODE_NAS_PRE"},
#if 0
    //���ܱ�������
    {CMD_CODE_OPEN_CLOSE_LOOP         ,    "CMD_CODE_OPEN_CLOSE_LOOP"},
    {CMD_CODE_LOCKING_TIME            ,    "CMD_CODE_LOCKING_TIME"},
    {CMD_CODE_HALF_VARIANCE_THRESHOLD ,    "CMD_CODE_HALF_VARIANCE_THRESHOLD"},
    {CMD_CODE_DEV_ID                  ,    "CMD_CODE_DEV_ID"},
    {CMD_CODE_LOCAL_CC                ,    "CMD_CODE_LOCAL_CC"},
    {CMD_CODE_VERSION                 ,    "CMD_CODE_VERSION"},
    {CMD_CODE_FPGA_VERSION            ,    "CMD_CODE_FPGA_VERSION"},
    {CMD_CODE_TERMINAL_CC             ,    "CMD_CODE_TERMINAL_CC"},
    {CMD_CODE_PROTOCOL_MODE           ,    "CMD_CODE_PROTOCOL_MODE"},
    {CMD_CODE_IS_CONNET_CC            ,    "CMD_CODE_IS_CONNET_CC"},
    {CMD_CODE_AIR_SUBNET_ID           ,    "CMD_CODE_AIR_SUBNET_ID"},
    {CMD_CODE_LOCAL_IP                ,    "CMD_CODE_LOCAL_IP"},
    {CMD_CODE_LOCAL_MAC               ,    "CMD_CODE_LOCAL_MAC"},
    {CMD_CODE_WORK_TIME_OUT           ,    "CMD_CODE_WORK_TIME_OUT"},
    {CMD_CODE_GPS_TIME_OUT            ,    "CMD_CODE_GPS_TIME_OUT"},
    {CMD_CODE_STUN_TIME_OUT           ,    "CMD_CODE_STUN_TIME_OUT"},
    {CMD_CODE_START_TIME_OUT          ,    "CMD_CODE_START_TIME_OUT"},
    {CMD_CODE_FPGA_EEPROM             ,    "CMD_CODE_FPGA_EEPROM"},
    {CMD_CODE_REBOOT                  ,    "CMD_CODE_REBOOT"},
    {CMD_CODE_ALARM_SWITCH            ,    "CMD_CODE_ALARM_SWITCH"},
    {CMD_CODE_UPDATE_DTB              ,    "CMD_CODE_UPDATE_DTB"},
    {CMD_CODE_UPDATE_LOADAPP          ,    "CMD_CODE_UPDATE_LOADAPP"},
    {CMD_CODE_UPDATE_FILE_SYSTEM      ,    "CMD_CODE_UPDATE_FILE_SYSTEM"},
    {CMD_CODE_UPDATE_UBOOT            ,    "CMD_CODE_UPDATE_UBOOT"},
    {CMD_CODE_UPDATE_RBF              ,    "CMD_CODE_UPDATE_RBF"},
    {CMD_CODE_UPDATE_ZIMAGE           ,    "CMD_CODE_UPDATE_ZIMAGE"},

    //FPGA��������
    {CMD_CODE_EMISSIVE_VCO_FREQ       ,    "CMD_CODE_EMISSIVE_VCO_FREQ"},
    {CMD_CODE_RECEIVING_VCO_FREQ      ,    "CMD_CODE_RECEIVING_VCO_FREQ"},
    {CMD_CODE_SECOND_LOCAL_OSCILLATOR ,    "CMD_CODE_SECOND_LOCAL_OSCILLATOR"},
    {CMD_CODE_POWER_AMPLIFIER_SWITCH  ,    "CMD_CODE_POWER_AMPLIFIER_SWITCH"},
    {CMD_CODE_FPGA_PARAM              ,    "CMD_CODE_FPGA_PARAM"},
    {CMD_CODE_POWER                   ,    "CMD_CODE_POWER"},
    {CMD_CODE_AFC                     ,    "CMD_CODE_AFC"},
    {CMD_CODE_SAVE_AFC                ,    "CMD_CODE_SAVE_AFC"},
    {CMD_CODE_IQS                     ,    "CMD_CODE_IQS"},
    {CMD_CODE_SAVE_IQS                ,    "CMD_CODE_SAVE_IQS"},
    {CMD_CODE_OFFSET                  ,    "CMD_CODE_OFFSET"},
    {CMD_CODE_SAVE_OFFSET             ,    "CMD_CODE_SAVE_OFFSET"},
    {CMD_CODE_PHASE                   ,    "CMD_CODE_PHASE"},
    {CMD_CODE_SAVE_PHASE              ,    "CMD_CODE_SAVE_PHASE"},
    {CMD_CODE_CALIBRATE_OFFSET        ,    "CMD_CODE_CALIBRATE_OFFSET"},
    {CMD_CODE_CALIBRATE_PHASE         ,    "CMD_CODE_CALIBRATE_PHASE"},
    {CMD_CODE_DEBUG_EMISSIVE_FREQ     ,    "CMD_CODE_DEBUG_EMISSIVE_FREQ"},
    {CMD_CODE_SAVE_POWER              ,    "CMD_CODE_SAVE_POWER"},
    {CMD_CODE_ERROR_RATE              ,    "CMD_CODE_ERROR_RATE"},
    {CMD_CODE_START_DEBUG             ,    "CMD_CODE_START_DEBUG"},
    {CMD_CODE_START_ERR_RATE          ,    "CMD_CODE_START_ERR_RATE"},
    {CMD_CODE_SELECT_SIGNAL_SOURCE    ,    "CMD_CODE_SELECT_SIGNAL_SOURCE"},
    {CMD_CODE_DEBUG_OPEN_CLOSE_LOOP   ,    "CMD_CODE_DEBUG_OPEN_CLOSE_LOOP"},
    {CMD_CODE_DEBUG_POWER             ,    "CMD_CODE_DEBUG_POWER"},
    {CMD_CODE_SLOT_CONTROL            ,    "CMD_CODE_SLOT_CONTROL"},
    {CMD_CODE_VGS                     ,    "CMD_CODE_VGS"},
    {CMD_CODE_SAVE_EEPROM             ,    "CMD_CODE_SAVE_EEPROM"},
    {CMD_CODE_SET_981                 ,    "CMD_CODE_SET_981"},
    {CMD_CODE_QUERY_981               ,    "CMD_CODE_QUERY_981"},
    {CMD_CODE_SET_998                 ,    "CMD_CODE_SET_998"},
    {CMD_CODE_QUERY_998               ,    "CMD_CODE_QUERY_998"},
    {CMD_CODE_DEV_CALL_TIMEOUT        ,    "CMD_CODE_DEV_CALL_TIMEOUT"},
    {CMD_CODE_ERROR_RATE_FREQ         ,    "CMD_CODE_ERROR_RATE_FREQ"},
    {CMD_CODE_SAVE_IQ_DATA            ,    "CMD_CODE_SAVE_IQ_DATA"},
    {CMD_CODE_CLOSE_TRAN_THRESHOLD    ,    "CMD_CODE_CLOSE_TRAN_THRESHOLD"},
    {CMD_CODE_START_TEMP_ALARM        ,    "CMD_CODE_START_TEMP_ALARM"},
    {CMD_CODE_CLOSE_TEMP_ALARM        ,    "CMD_CODE_CLOSE_TEMP_ALARM"},
    {CMD_CODE_RESUME_TRAN_THRESHOLD   ,    "CMD_CODE_RESUME_TRAN_THRESHOLD"},

    //��������
    {CMD_CODE_CENTER_MODULE           ,    "CMD_CODE_CENTER_MODULE"},
    {CMD_CODE_GSM_MODULE1             ,    "CMD_CODE_GSM_MODULE1"},
    {CMD_CODE_GSM_MODULE2             ,    "CMD_CODE_GSM_MODULE2"},
    {CMD_CODE_RADIO_MODULE            ,    "CMD_CODE_RADIO_MODULE"},
    {CMD_CODE_WIRELESS_MODULE         ,    "CMD_CODE_WIRELESS_MODULE"},
    {CMD_CODE_DSP_MODULE              ,    "CMD_CODE_DSP_MODULE"},
    {CMD_CODE_MICROPHONE_MODULE1      ,    "CMD_CODE_MICROPHONE_MODULE1"},
    {CMD_CODE_MICROPHONE_MODULE2      ,    "CMD_CODE_MICROPHONE_MODULE2"},
    {CMD_CODE_CONTROL_MODULE          ,    "CMD_CODE_CONTROL_MODULE"},
    {CMD_CODE_CENTER_PRINT            ,    "CMD_CODE_CENTER_PRINT"},
    {CMD_CODE_CENTER_INFO             ,    "CMD_CODE_CENTER_INFO"},
    {CMD_CODE_CENTER_QUERY_ALARM      ,    "CMD_CODE_CENTER_QUERY_ALARM"}
#endif
};
TYPE_PRINT_T *pTable_cmd = table_cmd_type_print;
unsigned short pTable_cmd_len = sizeof(table_cmd_type_print)/sizeof(table_cmd_type_print[0]);

// table-5 cmd ���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_op_type_print[] = {
    {OP_CODE_GET        ,  "OP_CODE_GET"},
    {OP_CODE_GET_ACK    ,  "OP_CODE_GET_ACK"},
    {OP_CODE_SET        ,  "OP_CODE_SET"},
    {OP_CODE_SET_ACK    ,  "OP_CODE_SET_ACK"},
    {OP_CODE_ALARM      ,  "OP_CODE_ALARM"},
    {OP_CODE_E_ALARM    ,  "OP_CODE_E_ALARM"}
};
TYPE_PRINT_T *pTable_op = table_op_type_print;
unsigned short pTable_op_len = sizeof(table_op_type_print)/sizeof(table_op_type_print[0]);


// table-6 cmd ���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_AI_type_print[] = {
    {DT_PI_HEADER        ,  "DT_PI_HEADER"},        ///<PIͷ֡��ʶ
    {DT_LC_HEADER        ,  "DT_LC_HEADER"},        ///<LCͷ֡��ʶ
    {DT_LC_TERMINATOR    ,  "DT_LC_TERMINATOR"},    ///<LC�ս�֡��ʶ
    {DT_CSBK             ,  "DT_CSBK"},
    {DT_MBC_HEADER       ,  "DT_MBC_HEADER"},
    {DT_MBC_CONTINUE     ,  "DT_MBC_CONTINUE"},
    {DT_DATA_HEADER      ,  "DT_DATA_HEADER"},
    {DT_R_1_2_DATA       ,  "DT_R_1_2_DATA"},
    {DT_R_3_4_DATA       ,  "DT_R_3_4_DATA"},
    {DT_IDLE             ,  "DT_IDLE"},
    {DT_R_1_1_DATA       ,  "DT_R_1_1_DATA"}
};

TYPE_PRINT_T *pTable_AI = table_AI_type_print;
unsigned short pTable_AI_len = sizeof(table_AI_type_print)/sizeof(table_AI_type_print[0]);

// table-7 CSBKO ���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_CSBKO_type_print[] = {
    {PRE_CSBKO      ,   "PRE_CSBKO"},               //  Ԥ�ز�
    {REQ_CSBKO      ,   "REQ_CSBKO"},               //  Common Signalling Request CSBKO
    {ACK_CSBKO      ,   "ACK_CSBKO"},               //  Common Signalling Answer Response CSBKO
    {ALARM_CSBKO    ,   "ALARM_CSBKO"},             //  Digital Alarm Service Request CSBKO
};
TYPE_PRINT_T *pTable_CSBKO = table_CSBKO_type_print;
unsigned short pTable_CSBKO_len = sizeof(table_CSBKO_type_print)/sizeof(table_CSBKO_type_print[0]);

// table-8 SSO ���͵��Ա�--by zhoudayuan
TYPE_PRINT_T table_SSO_type_print[] = {
    {CALL_ALERT_SSO       ,   "CALL_ALERT_SSO"},      // CallAlertService
    {EN_RADIO_SSO         ,   "EN_RADIO_SSO"},        // RadioEnableService
    {DIS_RADIO_SSO        ,   "DIS_RADIO_SSO"},       // RadioDisableService
    {DIGITAL_ALARM_SSO    ,   "DIGITAL_ALARM_SSO"},   // ��ֵ�ݶ�-DigitalAlarmService
};
TYPE_PRINT_T *pTable_SSO = table_SSO_type_print;
unsigned short pTable_SSO_len = sizeof(table_SSO_type_print)/sizeof(table_SSO_type_print[0]);


// table-9  GPS
TYPE_PRINT_T Table_Gps_Status[] = {
    {GPS_REQ  ,  "GPS_REQ"},
    {GPS_ACK  ,  "GPS_ACK"},
    {GPS_OK   ,  "GPS_OK "},
    {GPS_PRE  ,  "GPS_PRE"},
    {GPS_RLY  ,  "GPS_RLY"},
    {0xff     ,  "0xFF"   }
};

TYPE_PRINT_T *pTable_Gps_Status = Table_Gps_Status;
unsigned short Table_Gps_Status_Len = sizeof(Table_Gps_Status)/sizeof(Table_Gps_Status[0]);



// ��������������
ERR_FRM_T tErrFrm[FRM_F+1] = {
    {FRM_A, "FRM_A", 0}, //0
    {FRM_B, "FRM_B", 0}, //1
    {FRM_C, "FRM_C", 0}, //2
    {FRM_D, "FRM_D", 0}, //3
    {FRM_E, "FRM_E", 0}, //4
    {FRM_F, "FRM_F", 0}  //5
};

ERR_FRM_T *ptErrFrm = tErrFrm;


ERR_VOICE_T tErrVoice = {
    0, 0, 0, tErrFrm
};

ERR_VOICE_T *ptErrVoice = &tErrVoice;



#if 1
char *PrintGPSFlag(int uGpsFlag)
{
    int i;
    static char buff[100]={0};
    for (i = 0; i < Table_Gps_Status_Len; i++)
    {
        if (uGpsFlag == pTable_Gps_Status[i].Type)
        {
//            LOG_DEBUG(s_LogMsgId, "uGpsFlag=[%#x-%s]", pTable_Gps_Status->Type, pTable_Gps_Status->TypeStr);
//            LOG_DEBUG(s_LogMsgId, "uGpsFlag=[%#x-%s]", pTable_Gps_Status->Type, pTable_Gps_Status->TypeStr);
            snprintf(buff, sizeof(buff), "uGpsFlag=[%#x-%s]", pTable_Gps_Status->Type, pTable_Gps_Status->TypeStr);
            return buff;
        }
    }
    return NULL;
}
#endif







//------------------------����------------------------

// SIG-�����ַ�����ӡ
// �����ӡ
void PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead, const char *prompt)
{
    int i;
    for (i = 0; i < pTable_sig_len; i++)
    {
        if (pSharedHead->SigType == pTable_sig[i].Type)
        {
            if (pTable_sig[i].TypeStr != NULL)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s][%s]@SIG:[%#x-%s]", _F_, prompt, pTable_sig[i].Type, pTable_sig[i].TypeStr);
                break;
            }
        }
    }
}

char *SetCCSig2Str(CENTER_CMD_SHARE_HEAD *pSharedHead, char *BufGet, unsigned short BufLen)
{
    int i;
    for (i = 0; i < pTable_sig_len; i++)
    {
        if (pSharedHead->SigType == pTable_sig[i].Type)
        {
            if (pTable_sig[i].TypeStr != NULL)
            {
                snprintf(BufGet, BufLen, "[%#x-%s]", pTable_sig[i].Type, pTable_sig[i].TypeStr);
                return BufGet;
            }
        }
    }
    return strncpy(BufGet, "No The Sig", strlen("No The Sig")+1);
}






// ����
void IDP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead)
{
    const char *prompt = "CclUp";
    if (1 == tcclPrint->CclUp)
    {
        PrintSigLog(pSharedHead, prompt);
    }
}

// ����
void ODP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead)
{
    const char *prompt = "CcDown";
    if (1 == tcclPrint->CcDown)
    {
        PrintSigLog(pSharedHead, prompt);
    }
}



// sms-�����ַ�����ӡ
// ��ӡSms
void PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData, const char *prompt)
{
    int i;
    for (i = 0; i < pTable_sms_len; i++)
    {
        if (ptCenterData->SmsType == pTable_sms[i].Type)
        {
            if (pTable_sms[i].TypeStr != NULL)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s][%s]@SMS:[%#x-%s]", __FUNCTION__, prompt, pTable_sms[i].Type, pTable_sms[i].TypeStr);
                break;
            }
        }
    }
}
// ����
void IDP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData)
{
    const char *prompt = "CclUp";
    if (1 == tcclPrint->CclUp)
    {
        PrintSmsLog(ptCenterData, prompt);
    }
}



// ����
void ODP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData)
{
    const char *prompt = "CcDown";
    if (1 == tcclPrint->CcDown)
    {
        PrintSmsLog(ptCenterData, prompt);
    }
}

void ODP_PrintSms(SMS_CENTER_CCL_DL *ptCenterData, char *buf, int len)
{
    int i;
    for (i = 0; i < pTable_sms_len; i++)
    {
        if (ptCenterData->SmsType == pTable_sms[i].Type)
        {
            if (pTable_sms[i].TypeStr != NULL)
            {
                snprintf(buf , len, "[%#x-%s]", pTable_sms[i].Type, pTable_sms[i].TypeStr);
                return;
            }
        }
    }
    snprintf(buf, len, "no this Smstype");
}




// Msg & Data  ccl�����ַ�����ӡ
void PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, const char *prompt)
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
    for (j = 0; j < pTable_data_len; j++)
    {
        if (ptDllData->DataType == pTable_data[j].Type)
        {
            if (pTable_data[j].TypeStr != NULL)
            {
                j_index = j;
                break;
            }
        }
    }

    if ((i_index != -1) && (j_index != -1))
    {
        snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#06x-%s], DATA:[%#04x-%s]", pTable_msg[i_index].Type, pTable_msg[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
        LOG_DEBUG(s_LogMsgId, "[CCL][%s][%s]@%s", _F_, prompt, LogBuff);
    }
}
// ����  DLL TO CCL
void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData)
{
    const char *prompt = "DllUp";
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    if (1 == tcclPrint->DllUp)
    {
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
        for (j = 0; j < pTable_data_len; j++)
        {
            if (ptDllData->DataType == pTable_data[j].Type)
            {
                if (pTable_data[j].TypeStr != NULL)
                {
                    j_index = j;
                    break;
                }
            }
        }

        if ((i_index != -1) && (j_index != -1))
        {
            snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#x-%s], DATA:[%#x-%s]", pTable_msg[i_index].Type, pTable_msg[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
            LOG_DEBUG(s_LogMsgId, "[CCL][%s][%s]@%s", _F_, prompt, LogBuff);
        }
    }
}




// ����  CCL TO DLL
void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, char *buf, int len)
{

    unsigned short i, j;
    int i_index = -1, j_index = -1;

    if (1 == tcclPrint->CclDown)
    {

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
        for (j = 0; j < pTable_data_len; j++)
        {
            if (ptDllData->DataType == pTable_data[j].Type)
            {
                if (pTable_data[j].TypeStr != NULL)
                {
                    j_index = j;
                    break;
                }
            }
        }

        if ((i_index != -1) && (j_index != -1))
        {
            snprintf(buf, len, "MsgType=[%#x-%s], DataType=[%#x-%s]", pTable_msg[i_index].Type, pTable_msg[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
            return;
        }
        else
        {
            snprintf(buf, len, "MsgType=[no msg], DataType=[no data]");
            return;
        }
    }
}




#if 0
void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData)
{
    const char *prompt = "CclDown";
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    if (1 == tcclPrint->CclDown)
    {

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
        for (j = 0; j < pTable_data_len; j++)
        {
            if (ptDllData->DataType == pTable_data[j].Type)
            {
                if (pTable_data[j].TypeStr != NULL)
                {
                    j_index = j;
                    break;
                }
            }
        }

        if ((i_index != -1) && (j_index != -1))
        {
            snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#x-%s], DATA:[%#x-%s]", pTable_msg[i_index].Type, pTable_msg[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
            LOG_DEBUG(s_LogMsgId, "[CCL][%s][%s]@%s", _F_, prompt, LogBuff);
        }
    }
}
#endif



/**
 * @brief  ��ӡNAS cmd_code, op_code
 * @param [in] NAS_AI_PAYLOAD
 * @author  �ܴ�Ԫ
 * @since   trunk.00001
 * @bug
 */
void PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData, const char *prompt)
{
    int i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    for (i = 0; i < pTable_cmd_len; i++)
    {
        if (pNasAiData->cmd_code == pTable_cmd[i].Type)
        {
            if (pTable_cmd[i].TypeStr != NULL)
            {
                i_index = i;
                break;
            }
        }
    }

    for (j = 0; j < pTable_op_len; j++)
    {

        if (pNasAiData->op_code == pTable_op[j].Type)
        {
            if (pTable_op[j].TypeStr != NULL)
            {
                j_index = j;
                break;
            }
        }
    }

    if ((i_index != -1) && (j_index != -1))
    {
        snprintf(LogBuff, sizeof(LogBuff), "CMD:[%#06x-%s], OP:[%#x-%s]", pTable_cmd[i_index].Type, pTable_cmd[i_index].TypeStr, pTable_op[j_index].Type, pTable_op[j_index].TypeStr);
        LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s]@%s", _F_, prompt, LogBuff);
    }
}

void IDP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData)
{
    const char *prompt = "AIUp";
    if (tDllPrint->AIUp == 1)
    {
        PrintNasCmdOpLog(pNasAiData, prompt);
    }
}

void ODP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData)
{
    const char *prompt = "AIDown";
    if (tDllPrint->AIDown == 1)
    {
        PrintNasCmdOpLog(pNasAiData, prompt);
    }
}


/**
 * @brief  ��ӡ�ӿڲ�data��Ϣ
 * @param [in] NAS_AI_PAYLOAD
 * @author  �ܴ�Ԫ
 * @since   trunk.00001
 * @bug
 */
void PrintInfDataLog(DATA_LINK_T *pDataLink, const char *prompt)
{
    int i;
    for (i = 0; i < pTable_AI_len; i++)
    {
        if (pDataLink->DataType == pTable_AI[i].Type)
        {
            if (pTable_AI[i].TypeStr != NULL)
            {
                LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s]@AI [%#x-%s]", _F_, prompt, pTable_AI[i].Type, pTable_AI[i].TypeStr);
                break;
            }
        }
    }
}

void IDP_PrintInfDataLog(DATA_LINK_T *pDataLink)
{
    const char *prompt = "AIUp";
    if (tDllPrint->AIUp == 1)
    {
        PrintInfDataLog(pDataLink, prompt);
    }
}

void ODP_PrintInfDataLog(DATA_LINK_T *pDataLink)
{
    const char *prompt = "AIDown";
    if (tDllPrint->AIDown == 1)
    {
        PrintInfDataLog(pDataLink, prompt);
    }
}


 /**
  * @brief
  * @param [in]
  * @author  �ܴ�Ԫ
  * @since   trunk.00001
  * @bug
  */
 void PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk, const char *prompt)
 {
     int i, j;
     int i_index = -1, j_index = -1;
     char LogBuff[200] = {0};

     for (i = 0; i < pTable_CSBKO_len; i++)
     {
         if (pSupsCsbk->uCSBKO == pTable_CSBKO[i].Type)
         {
             if (pTable_CSBKO[i].TypeStr != NULL)
             {
                 i_index = i;
                 break;
             }
         }
     }

     for (j = 0; j < pTable_SSO_len; j++)
     {

         if (pSupsCsbk->uSSO == pTable_SSO[j].Type)
         {
            if (pTable_SSO[j].TypeStr != NULL)
            {
                j_index = j;
                break;
            }
         }
     }

     if ((i_index != -1) && (j_index != -1))
     {
         snprintf(LogBuff, sizeof(LogBuff), "CSBKO:[%#x-%s], SSO:[%#x-%s]", pTable_CSBKO[i_index].Type, pTable_CSBKO[i_index].TypeStr, pTable_SSO[j_index].Type, pTable_SSO[j_index].TypeStr);
         LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s]@%s", _F_, prompt, LogBuff);
     }
}


void IDP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk)
{
    const char *prompt = "AIUp";
    if (tDllPrint->AIUp == 1)
    {
        PrintCsbkoSSOLog(pSupsCsbk, prompt);
    }
}



void ODP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk)
{
    const char *prompt = "AIDown";
    if (tDllPrint->AIDown == 1)
    {
        PrintCsbkoSSOLog(pSupsCsbk, prompt);
    }
}


 /**
 * @brief  ���תС�ˣ���ԴID��Ŀ��ID��ʮ���ƴ�ӡ������������Կ�
 * @author �ܴ�Ԫ
 * @since
 * @bug
 */
void PrintIDbyDec(unsigned char *src, unsigned char *dst)
{
    unsigned char src_tmp[4];
    unsigned char dst_tmp[4];
    unsigned int  src_val;
    unsigned int  dst_val;

    if ((src[1]==0 && src[2]==0) || (dst[1]== 0 && dst[2]== 0))
    {
        LOG_DEBUG(s_LogMsgId,"[CCL]@snd=%d-->rcv=%d", src[0], dst[0]);
    }
    else
    {
        // ת��С��ģʽ
        src_tmp[0] = src[2];
        src_tmp[1] = src[1];
        src_tmp[2] = src[0];
        src_tmp[3] = '\0';
        dst_tmp[0] = dst[2];
        dst_tmp[1] = dst[1];
        dst_tmp[2] = dst[0];
        dst_tmp[3] = '\0';
        memcpy(&src_val, src_tmp, sizeof(src_tmp));
        memcpy(&dst_val, dst_tmp, sizeof(dst_tmp));
        LOG_DEBUG(s_LogMsgId,"[CCL]@snd=%d-->rcv=%d", src_val, dst_val);
    }
}


/**
 * @brief  ���תС�ˣ���ԴID��Ŀ��ID��ʮ���ƴ�ӡ������������Կ�
 * @author �ܴ�Ԫ
 * @since
 * @bug
 */
void SetIdDec2buf(unsigned char *src, unsigned char *dst, char *BufGet, unsigned int BufLen)
{
    unsigned char src_tmp[4];
    unsigned char dst_tmp[4];
    unsigned int  src_val;
    unsigned int  dst_val;

    if ((src[1]==0 && src[2]==0) || (dst[1]== 0 && dst[2]== 0))
    {
        snprintf(BufGet, BufLen, "snd=%d->rcv=%d", src[0], dst[0]);
    }
    else
    {
        // ת��С��ģʽ
        src_tmp[0] = src[2];
        src_tmp[1] = src[1];
        src_tmp[2] = src[0];
        src_tmp[3] = '\0';
        dst_tmp[0] = dst[2];
        dst_tmp[1] = dst[1];
        dst_tmp[2] = dst[0];
        dst_tmp[3] = '\0';
        memcpy(&src_val, src_tmp, sizeof(src_tmp));
        memcpy(&dst_val, dst_tmp, sizeof(dst_tmp));
        snprintf(BufGet, BufLen, "snd=%d->rcv=%d", src_val, dst_val);
    }
    return ;
}


void print_voice_153(UINT8 *pPayLoad, const char *prompt)
{
    int i;
    printf("[%s]\n", prompt);
    for (i = 0; i < 27; i++)
    {
        if (i%14 == 0)
        {
            printf("\n");
        }

        printf("%#6x", pPayLoad[i]);
    }
    printf("\n");
}



void test_err_voice_init()
{
    int i;
    ptErrVoice->FrmCnt = 0;
    ptErrVoice->ErrTotal = 0;
    for (i = 0; i < FRM_F + 1;i++)
    {
        ptErrVoice->pErrFrm[i].FrmErrBit = 0;
    }
}

void test_err_voice_result()
{
    int i;
    float  r;
    float  a = (float)(ptErrVoice->ErrTotal);  // ����ȵ� bits
    float  b = (float)(ptErrVoice->FrmCnt*27*8);  // һ�����յ���bits
    r = a/b * 100;

    // �������
    LOG_DEBUG(s_LogMsgId, "[ERR][V][%s] Result:Frm=%d,SuperFrm=%d,bytes=%d,bits=%d", _F_,
        ptErrVoice->FrmCnt, ptErrVoice->FrmCnt/6, ptErrVoice->FrmCnt*27, ptErrVoice->FrmCnt*27*8);

    // ÿ֡A-F �ж�������
    for (i = 0; i < FRM_F + 1;i++)
    {
        LOG_DEBUG(s_LogMsgId, "[ERR][V] %s ErrBits:%d",  ptErrVoice->pErrFrm[i].FrmNameStr, ptErrVoice->pErrFrm[i].FrmErrBit);
    }
    // ����
    LOG_DEBUG(s_LogMsgId, "[ERR][V][%s] Total ErrBits:%d, ErrRate:%.3f%%\n", _F_, ptErrVoice->ErrTotal, r);

#if ERR_VOICE_DEBUG
    if (g_fp_err_test != NULL)
    {
        fclose(g_fp_err_test);
        g_fp_err_test = NULL;
    }
#endif

}



void test_err_voice_dll(DATA_LINK_T *pDataLink)
{
    int i;

#if ERR_VOICE_DEBUG
    int len;
    char buf[200];
#endif
    unsigned short result = 0;
    ptErrVoice->FrmIndx = pDataLink->FrmType - 1; // ʵ������֡ - 1, ƥ��������������
    if (ptErrVoice->FrmIndx > FRM_F)  // ����Frm:A~F ���ڼ�ⷶΧ
    {
        LOG_DEBUG(s_LogMsgId,"[ERR][V][%s] Err: Frm=%d, Frm:A~F(0~5)", _F_, ptErrVoice->FrmIndx);
        return;
    }

    for (i = 0; i < 27; i++) // һ֡����27bytes
    {
        result = ones16(pDataLink->PayLoad[i]^s_au2VoiceSource[VOICE_SRC_153][ptErrVoice->FrmIndx][i]);  // ��������
        if (result != 0)
        {
            ptErrVoice->pErrFrm[ptErrVoice->FrmIndx].FrmErrBit += result;  // ��¼ĳһ֡�ж��ٴ���
            ptErrVoice->ErrTotal += result;  // ��¼���еĴ���
#if ERR_VOICE_DEBUG
            // �ڶ���֡, ��֡(A~F), �������ʵ���ݣ���ǰ���ٸ�
            len = snprintf(buf, sizeof(buf), "voice: RcvCnt=%d,Frm=%s,byte=%dth[%#x-%#x],ErrNum=%d",
                ptErrVoice->FrmCnt,
                ptErrVoice->pErrFrm[ptErrVoice->FrmIndx].FrmNameStr,
                i,
                pDataLink->PayLoad[i],
                s_au2VoiceSource[VOICE_SRC_153][ptErrVoice->FrmIndx][i],
                result);

            LOG_DEBUG(s_LogMsgId,"[ERR][%s] %s", _F_, buf);
            len = fwrite(buf, len, 1, g_fp_err_test);
#endif
        }
    }
    ptErrVoice->FrmCnt ++;  // ��ǰ�ǵڼ��β���(��0��ʼ)��һ�β�����һ������֡(FRM: A~F) ���27bytes
}






void setDebugInit()
{
    ptErrPrint = (ERR_PRINT_T *)ptIPCShm->Err_printf;
    memset(ptErrPrint, 0x00, sizeof(ERR_PRINT_T));

#if ERR_VOICE_DEBUG
    // ��д���ļ�
    g_fp_err_test = fopen("err_test.log","w+");
    if (g_fp_err_test == NULL)
    {
        printf("fopen err_test.log error\n");
        fclose(g_fp_err_test);
        return;
    }
#endif

}

