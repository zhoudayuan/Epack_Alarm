#include <arpa/inet.h>
#include <stdio.h>
#include "print_debug.h"
/**
 * @var  s_i4LogMsgId
 * @brief 命令行套接字
 */
extern INT32 s_LogMsgId;
/**
 * @var ptIPCShm
 * @brief 共享内存系统信息
 */
extern SHM_IPC_STRU *ptIPCShm;
/**
 * @var tcclPrint
 * @brief CCL打印开关
 */
extern CCL_PRINT_T * tcclPrint;
/**
 * @var tDllPrint
 * @brief DLL打印开关
 */
extern DLL_PRINT_T *tDllPrint;
/**
 * @var
 * @brief 语音信源数据
 */
extern UINT8 s_au2VoiceSource[3][6][27];
/**
 * @var
 * @brief 计算误码率
 */
extern UINT16 ones16(UINT16 u2Data);
/**
 * @var
 * @brief 计算误码率
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




//------------------------类型------------------------
// table-0 sms-类型调试表--by zhoudayuan
TYPE_PRINT_T table_sig_type_print[] = {
    {SIG_STATUS_REPORT   ,  "SIG_STATUS_REPORT"},       // NAS状态上报
    {SIG_PTT_CMD         ,  "SIG_PTT_CMD"},             // PTT命令信令
    {SIG_PTT_ON_ACK      ,  "SIG_PTT_ON_ACK"},          // PTTON回复信令
    {SIG_SMS_SEND        ,  "SIG_SMS_SEND"},            // 短消息命令
    {SIG_SMS_ACK         ,  "SIG_SMS_ACK"},             // 短消息回复信令
    {SIG_PTT_OFF_ACK     ,  "SIG_PTT_OFF_ACK"},         // PTTOFF回复信令
};

TYPE_PRINT_T *pTable_sig = table_sig_type_print;
unsigned short pTable_sig_len = sizeof(table_sig_type_print)/sizeof(table_sig_type_print[0]);



// table-1 sms-类型调试表--by zhoudayuan
TYPE_PRINT_T table_sms_type_print[] = {
    //短消息
    {MESSAGE_PRIVATE_CALL    ,   "MESSAGE_PRIVATE_CALL"},
    {MESSAGE_GROUP_CALL      ,   "MESSAGE_GROUP_CALL"},
    {STATUS_PRIVATE_CALL     ,   "STATUS_PRIVATE_CALL"},
    {STATUS_GROUP_CALL       ,   "STATUS_GROUP_CALL"},
    //命令                   ,   //命令
    {STUN_REQ_MS             ,   "STUN_REQ_MS"},
    {STUN_REQ_NAS            ,   "STUN_REQ_NAS"},
    {KILL_REQ_NAS            ,   "KILL_REQ_NAS"},
    {GPS_REPOTR_MS           ,   "GPS_REPOTR_MS"},
    {GPS_REPORT_NAS          ,   "GPS_REPORT_NAS"},
    {REVIVE_REQ_NAS          ,   "REVIVE_REQ_NAS"},
    {REVIVE_REQ_MS           ,   "REVIVE_REQ_MS"},
    {NEGHR_QUERY             ,   "NEGHR_QUERY"},
    //响应                   ,   //响应
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
    //报告                   ,   //报告
    {STUN_NAS_REPORT         ,   "STUN_NAS_REPORT"},
    {KILL_NAS_REPORT         ,   "KILL_NAS_REPORT"},
    {REVIVE_NAS_REPORT       ,   "REVIVE_NAS_REPORT"},
    //告警                   ,   //告警
    {DISCON_ALARM            ,   "DISCON_ALARM"},
    {MS_ALARM                ,   "MS_ALARM"},
    {DISCON_NAS_ALARM_CLEAR  ,   "DISCON_NAS_ALARM_CLEAR"},
    {MS_ALARM_CLEAR          ,   "MS_ALARM_CLEAR"}
};
TYPE_PRINT_T *pTable_sms = table_sms_type_print;
unsigned short pTable_sms_len = sizeof(table_sms_type_print)/sizeof(table_sms_type_print[0]);




// table-2 msg-类型调试表--by zhoudayuan
TYPE_PRINT_T table_msg_type_print[] = {
    {DI_MSG_IDLE             ,   "DI_MSG_IDLE"},               ///< 空闲消息
    {DI_MSG_VOICE            ,   "DI_MSG_VOICE"},              ///< 语音消息
    {DI_MSG_DATA             ,   "DI_MSG_DATA"},               ///< 数据消息
    {DI_MSG_NEAR             ,   "DI_MSG_NEAR"},               ///< 邻点消息
    {DI_MSG_NM               ,   "DI_MSG_NM"},                 ///< 网管消息
    {DI_MSG_WLU              ,   "DI_MSG_WLU"}                 ///< WLU数据消息
};
TYPE_PRINT_T *pTable_msg = table_msg_type_print;
unsigned short pTable_msg_len = sizeof(table_msg_type_print)/sizeof(table_msg_type_print[0]);


// table-3 ccl data-类型调试表--by zhoudayuan
TYPE_PRINT_T table_ccl_data_print[] = {
    // 空口数据类型DC_MSG_DATA
    {CT_JUNK_DATA            ,   "CT_JUNK_DATA"},              ///<非数据消息
    {CT_PI_HEADER            ,   "CT_PI_HEADER"},              ///<PI头帧标识
    {CT_LC_HEADER            ,   "CT_LC_HEADER"},              ///<LC头帧标识
    {CT_LC_TERMINATOR        ,   "CT_LC_TERMINATOR"},          ///<LC终结帧标识
    {CT_PACKET_DATA          ,   "CT_PACKET_DATA"},            ///<短消息
    {CT_GPS_REPORT_REQ_MS    ,   "CT_GPS_REPORT_REQ_MS"},      ///<手台GPS上拉
    {CT_GPS_REPORT_ACK_MS    ,   "CT_GPS_REPORT_ACK_MS"},      ///<手台GPS上拉
    {CT_DISABLE_REQ_MS       ,   "CT_DISABLE_REQ_MS"},         ///<摇晕
    {CT_DISABLE_ACK_MS       ,   "CT_DISABLE_ACK_MS"},         ///<摇晕
    {CT_ENABLE_REQ_MS        ,   "CT_ENABLE_REQ_MS"},          ///<终端激活
    {CT_ENABLE_ACK_MS        ,   "CT_ENABLE_ACK_MS"},          ///<终端激活
    {CT_ALARM_REQ_MS         ,   "CT_ALARM_REQ_MS"},           ///<终端紧急告警
    {CT_ALARM_ACK_MS         ,   "CT_ALARM_ACK_MS"},           ///<终端紧急告警
    // 链路机数据类型DC_MSG_WLU /链路机数据类型DC_MSG_WLU
    {CT_GPS_REPORT_REQ_NAS   ,   "CT_GPS_REPORT_REQ_NAS"},     ///< GPS上拉
    {CT_GPS_REPORT_ACK_NAS   ,   "CT_GPS_REPORT_ACK_NAS"},     ///< GPS上拉
    {CT_STUN_REQ_NAS         ,   "CT_STUN_REQ_NAS"},           ///<摇晕
    {CT_STUN_ACK_NAS         ,   "CT_STUN_ACK_NAS"},           ///<摇晕
    {CT_KILL_REQ_NAS         ,   "CT_KILL_REQ_NAS"},           ///<摇毙
    {CT_KILL_ACK_NAS         ,   "CT_KILL_ACK_NAS"},           ///<摇毙
    {CT_ENABLE_REQ_NAS       ,   "CT_ENABLE_REQ_NAS"},         ///<WLU激活
    {CT_ENABLE_ACK_NAS       ,   "CT_ENABLE_ACK_NAS"},         ///<WLU激活
    {CT_STUN_RPT_NAS         ,   "CT_STUN_RPT_NAS"},           ///<WLU遥晕?
    {CT_KILL_RPT_NAS         ,   "CT_KILL_RPT_NAS"},           ///<WLU遥毙上报
    {CT_ENABLE_RPT_NAS       ,   "CT_ENABLE_RPT_NAS"},         ///< WLU激活上报
    {CT_NEGHR_QUERY          ,   "CT_NEGHR_QUERY"},            ///<邻点信息查询
    {CT_NEGHR_QUERY_ACK      ,   "CT_NEGHR_QUERY_ACK"},        ///< 邻点信息响应
    {CT_NEGHR_REPORT         ,   "CT_NEGHR_REPORT"},           ///<邻点信息上报
    {CT_DISCON_ALARM         ,   "CT_DISCON_ALARM"},           ///<断链告警
    {CT_DISCON_ALARM_CLEAR   ,   "CT_DISCON_ALARM_CLEAR"}      ///< 断链告警清除
};

TYPE_PRINT_T *pTable_data = table_ccl_data_print;
unsigned short pTable_data_len = sizeof(table_ccl_data_print)/sizeof(table_ccl_data_print[0]);


// table-4 cmd 类型调试表--by zhoudayuan
TYPE_PRINT_T table_cmd_type_print[] = {
#if 0
    //网管空口命令
    {CMD_CODE_DEV_FREQ                ,    "CMD_CODE_DEV_FREQ"},
    {CMD_CODE_DEV_POWER               ,    "CMD_CODE_DEV_POWER"},
    {CMD_CODE_START_NEIGHBOR          ,    "CMD_CODE_START_NEIGHBOR"},
    {CMD_CODE_NEIGHBOR_PERIOD         ,    "CMD_CODE_NEIGHBOR_PERIOD"},
    {CMD_CODE_COMBINED_DATA           ,    "CMD_CODE_COMBINED_DATA"},
    {CMD_CODE_SCAN_MODE               ,    "CMD_CODE_SCAN_MODE"},          //以前命令字是0x0006，暂时没用后期开发注意修改

    //告警模块命令字

    {CMD_CODE_ALARM                  ,    "CMD_CODE_ALARM"},
#endif
    //业务模块命令字C0~FF
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
    //网管本地命令
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

    //FPGA调测命令
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

    //手咪命令
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

// table-5 cmd 类型调试表--by zhoudayuan
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


// table-6 cmd 类型调试表--by zhoudayuan
TYPE_PRINT_T table_AI_type_print[] = {
    {DT_PI_HEADER        ,  "DT_PI_HEADER"},        ///<PI头帧标识
    {DT_LC_HEADER        ,  "DT_LC_HEADER"},        ///<LC头帧标识
    {DT_LC_TERMINATOR    ,  "DT_LC_TERMINATOR"},    ///<LC终结帧标识
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

// table-7 CSBKO 类型调试表--by zhoudayuan
TYPE_PRINT_T table_CSBKO_type_print[] = {
    {PRE_CSBKO      ,   "PRE_CSBKO"},               //  预载波
    {REQ_CSBKO      ,   "REQ_CSBKO"},               //  Common Signalling Request CSBKO
    {ACK_CSBKO      ,   "ACK_CSBKO"},               //  Common Signalling Answer Response CSBKO
    {ALARM_CSBKO    ,   "ALARM_CSBKO"},             //  Digital Alarm Service Request CSBKO
};
TYPE_PRINT_T *pTable_CSBKO = table_CSBKO_type_print;
unsigned short pTable_CSBKO_len = sizeof(table_CSBKO_type_print)/sizeof(table_CSBKO_type_print[0]);

// table-8 SSO 类型调试表--by zhoudayuan
TYPE_PRINT_T table_SSO_type_print[] = {
    {CALL_ALERT_SSO       ,   "CALL_ALERT_SSO"},      // CallAlertService
    {EN_RADIO_SSO         ,   "EN_RADIO_SSO"},        // RadioEnableService
    {DIS_RADIO_SSO        ,   "DIS_RADIO_SSO"},       // RadioDisableService
    {DIGITAL_ALARM_SSO    ,   "DIGITAL_ALARM_SSO"},   // 该值暂定-DigitalAlarmService
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



// 误码率语音测试
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







//------------------------函数------------------------

// SIG-类型字符串打印
// 信令打印
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






// 上行
void IDP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead)
{
    const char *prompt = "CclUp";
    if (1 == tcclPrint->CclUp)
    {
        PrintSigLog(pSharedHead, prompt);
    }
}

// 下行
void ODP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead)
{
    const char *prompt = "CcDown";
    if (1 == tcclPrint->CcDown)
    {
        PrintSigLog(pSharedHead, prompt);
    }
}



// sms-类型字符串打印
// 打印Sms
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
// 上行
void IDP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData)
{
    const char *prompt = "CclUp";
    if (1 == tcclPrint->CclUp)
    {
        PrintSmsLog(ptCenterData, prompt);
    }
}



// 下行
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




// Msg & Data  ccl类型字符串打印
void PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, const char *prompt)
{
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    // 打印Msg
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

    // 打印data
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
// 上行  DLL TO CCL
void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData)
{
    const char *prompt = "DllUp";
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    if (1 == tcclPrint->DllUp)
    {
        // 打印Msg
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

        // 打印data
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




// 下行  CCL TO DLL
void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, char *buf, int len)
{

    unsigned short i, j;
    int i_index = -1, j_index = -1;

    if (1 == tcclPrint->CclDown)
    {

        // 打印Msg
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

        // 打印data
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

        // 打印Msg
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

        // 打印data
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
 * @brief  打印NAS cmd_code, op_code
 * @param [in] NAS_AI_PAYLOAD
 * @author  周大元
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
 * @brief  打印接口层data信息
 * @param [in] NAS_AI_PAYLOAD
 * @author  周大元
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
  * @author  周大元
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
 * @brief  大端转小端，将源ID和目的ID以十进制打印出来，方便调试看
 * @author 周大元
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
        // 转回小端模式
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
 * @brief  大端转小端，将源ID和目的ID以十进制打印出来，方便调试看
 * @author 周大元
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
        // 转回小端模式
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
    float  a = (float)(ptErrVoice->ErrTotal);  // 不相等的 bits
    float  b = (float)(ptErrVoice->FrmCnt*27*8);  // 一共接收到的bits
    r = a/b * 100;

    // 接收情况
    LOG_DEBUG(s_LogMsgId, "[ERR][V][%s] Result:Frm=%d,SuperFrm=%d,bytes=%d,bits=%d", _F_,
        ptErrVoice->FrmCnt, ptErrVoice->FrmCnt/6, ptErrVoice->FrmCnt*27, ptErrVoice->FrmCnt*27*8);

    // 每帧A-F 有多少误码
    for (i = 0; i < FRM_F + 1;i++)
    {
        LOG_DEBUG(s_LogMsgId, "[ERR][V] %s ErrBits:%d",  ptErrVoice->pErrFrm[i].FrmNameStr, ptErrVoice->pErrFrm[i].FrmErrBit);
    }
    // 所有
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
    ptErrVoice->FrmIndx = pDataLink->FrmType - 1; // 实际语音帧 - 1, 匹配语音数组索引
    if (ptErrVoice->FrmIndx > FRM_F)  // 超过Frm:A~F 不在检测范围
    {
        LOG_DEBUG(s_LogMsgId,"[ERR][V][%s] Err: Frm=%d, Frm:A~F(0~5)", _F_, ptErrVoice->FrmIndx);
        return;
    }

    for (i = 0; i < 27; i++) // 一帧数据27bytes
    {
        result = ones16(pDataLink->PayLoad[i]^s_au2VoiceSource[VOICE_SRC_153][ptErrVoice->FrmIndx][i]);  // 计算误码
        if (result != 0)
        {
            ptErrVoice->pErrFrm[ptErrVoice->FrmIndx].FrmErrBit += result;  // 记录某一帧有多少错误
            ptErrVoice->ErrTotal += result;  // 记录所有的错误
#if ERR_VOICE_DEBUG
            // 第多少帧, 哪帧(A~F), 错误的真实数据，当前多少个
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
    ptErrVoice->FrmCnt ++;  // 当前是第几次测试(从0开始)，一次测试是一个语音帧(FRM: A~F) 检测27bytes
}






void setDebugInit()
{
    ptErrPrint = (ERR_PRINT_T *)ptIPCShm->Err_printf;
    memset(ptErrPrint, 0x00, sizeof(ERR_PRINT_T));

#if ERR_VOICE_DEBUG
    // 打开写入文件
    g_fp_err_test = fopen("err_test.log","w+");
    if (g_fp_err_test == NULL)
    {
        printf("fopen err_test.log error\n");
        fclose(g_fp_err_test);
        return;
    }
#endif

}

