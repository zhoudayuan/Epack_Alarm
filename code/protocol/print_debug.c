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


//------------------邻点功能调试------------------
#define NER_CHECK_LOG_PATH   "NerCheckLog.log"
FILE *g_fpNerCheckLog = NULL;

UINT8  g_ErrVoiceState = WAIT_ERR_VOICE_HDR;

//------------------------类型------------------------
// table-0 sms-类型调试表--by zhoudayuan
T_TypePrint atSigTable[] = {
    {SIG_STATUS_REPORT   ,  "StatusRpt"},       // NAS状态上报
    {SIG_PTT_CMD         ,  "PttCmd"},          // PTT命令信令
    {SIG_PTT_ON_ACK      ,  "PttOnAck"},        // PTTON回复信令
    {SIG_SMS_SEND        ,  "SmsSnd"},          // 短消息命令
    {SIG_SMS_ACK         ,  "SmsAck"},          // 短消息回复信令
    {SIG_PTT_OFF_ACK     ,  "PttOffAck"},       // PTTOFF回复信令
};

T_TypePrint *ptSigTable = atSigTable;
unsigned short usSigTableLen = sizeof(atSigTable)/sizeof(atSigTable[0]);



// table-1 sms-类型调试表--by zhoudayuan
T_TypePrint table_sms_type_print[] = {
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
T_TypePrint *pTable_sms = table_sms_type_print;
unsigned short pTable_sms_len = sizeof(table_sms_type_print)/sizeof(table_sms_type_print[0]);




// table-2 msg-类型调试表--by zhoudayuan
T_TypePrint atMsgTable[] = {
    {DI_MSG_IDLE             ,   "Idle"},               ///< 空闲消息
    {DI_MSG_VOICE            ,   "Voice"},              ///< 语音消息
    {DI_MSG_DATA             ,   "Data"},               ///< 数据消息
    {DI_MSG_NEAR             ,   "Ner"},               ///< 邻点消息
    {DI_MSG_NM               ,   "Nm"},                 ///< 网管消息
    {DI_MSG_WLU              ,   "Wlu"}                 ///< WLU数据消息
};
T_TypePrint *ptMsgTable = atMsgTable;
unsigned short usMsgTableLen = sizeof(atMsgTable)/sizeof(atMsgTable[0]);


// table-3 ccl data-类型调试表--by zhoudayuan
T_TypePrint table_ccl_data_print[] = {
    // 空口数据类型DC_MSG_DATA
    {CT_JUNK_DATA            ,   "Junk_Data"},            ///<非数据消息
    {CT_PI_HEADER            ,   "PiHdr"},                ///<PI头帧标识
    {CT_LC_HEADER            ,   "LcHdr"},                ///<LC头帧标识
    {CT_LC_TERMINATOR        ,   "LcTer"},                ///<LC终结帧标识
    {CT_PACKET_DATA          ,   "PacketData"},           ///<短消息
    {CT_GPS_REPORT_REQ_MS    ,   "GpsRptReqMs"},          ///<手台GPS上拉
    {CT_GPS_REPORT_ACK_MS    ,   "GpsRptAckMs"},          ///<手台GPS上拉
    {CT_DISABLE_REQ_MS       ,   "DisableReqMs"},         ///<摇晕
    {CT_DISABLE_ACK_MS       ,   "DisableAckMs"},         ///<摇晕
    {CT_ENABLE_REQ_MS        ,   "EnableReqMs"},          ///<终端激活
    {CT_ENABLE_ACK_MS        ,   "EnableAckMs"},          ///<终端激活
    {CT_ALARM_REQ_MS         ,   "AlarmReqMs"},           ///<终端紧急告警
    {CT_ALARM_ACK_MS         ,   "AlarmAckMs"},           ///<终端紧急告警
    // 链路机数据类型DC_MSG_WLU /链路机数据类型DC_MSG_WLU
    {CT_GPS_REPORT_REQ_NAS   ,   "GpsRptReqNas"},         ///< GPS上拉
    {CT_GPS_REPORT_ACK_NAS   ,   "GpsRptAckNas"},         ///< GPS上拉
    {CT_STUN_REQ_NAS         ,   "StunReqNas"},           ///<摇晕
    {CT_STUN_ACK_NAS         ,   "StunAckNas"},           ///<摇晕
    {CT_KILL_REQ_NAS         ,   "KillReqNas"},           ///<摇毙
    {CT_KILL_ACK_NAS         ,   "KillAckNas"},           ///<摇毙
    {CT_ENABLE_REQ_NAS       ,   "EnableReqNas"},         ///<WLU激活
    {CT_ENABLE_ACK_NAS       ,   "EnableAckNas"},         ///<WLU激活
    {CT_STUN_RPT_NAS         ,   "StunRptNas"},           ///<WLU遥晕?
    {CT_KILL_RPT_NAS         ,   "KillRptNas"},           ///<WLU遥毙上报
    {CT_ENABLE_RPT_NAS       ,   "EnableRptNas"},         ///< WLU激活上报
    {CT_NEGHR_QUERY          ,   "NeghrQuery"},           ///<邻点信息查询
    {CT_NEGHR_QUERY_ACK      ,   "NeghrQueryAck"},        ///< 邻点信息响应
    {CT_NEGHR_REPORT         ,   "NeghrReport"},          ///<邻点信息上报
    {CT_DISCON_ALARM         ,   "DisconAlarm"},          ///<断链告警
    {CT_DISCON_ALARM_CLEAR   ,   "DisconAlarmClear"}      ///< 断链告警清除
};

T_TypePrint *pTable_data = table_ccl_data_print;
unsigned short pTable_data_len = sizeof(table_ccl_data_print)/sizeof(table_ccl_data_print[0]);


// table-4 cmd 类型调试表--by zhoudayuan
T_TypePrint atCmdTable[] = {
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
    {CMD_CODE_GPS_REQUEST             ,    "GpsReq"},
    {CMD_CODE_STUN                    ,    "Stun"},
    {CMD_CODE_KILL                    ,    "Kill"},
    {CMD_CODE_ENABLE                  ,    "Enable"},
    {CMD_CODE_NER_QUERY               ,    "NerQuery"},
    {CMO_CODE_NER_REPORT              ,    "NerRpt"},
    {CMO_CODE_MS_GPS                  ,    "MsGps"},
    {CMO_CODE_MS_DISABlE              ,    "MsDisable"},
    {CMO_CODE_MS_ENABLE               ,    "MsEnable"},
    {CMO_CODE_NAS_PRE                 ,    "Pre"},
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
T_TypePrint *ptCmdTable = atCmdTable;
unsigned short usCmdTableLen = sizeof(atCmdTable)/sizeof(atCmdTable[0]);

// table-5 cmd 类型调试表--by zhoudayuan
T_TypePrint atOpTable[] = {
    {OP_CODE_GET        ,  "Get"},
    {OP_CODE_GET_ACK    ,  "GetAck"},
    {OP_CODE_SET        ,  "Set"},
    {OP_CODE_SET_ACK    ,  "SetAck"},
    {OP_CODE_ALARM      ,  "Alarm"},
    {OP_CODE_E_ALARM    ,  "E_Alarm"}
};
T_TypePrint *ptOpTable = atOpTable;
unsigned short usOpTableLen = sizeof(atOpTable)/sizeof(atOpTable[0]);


// table-6 cmd 类型调试表--by zhoudayuan
T_TypePrint atDllDataTable[] = {
    {DT_PI_HEADER        ,  "PiHdr"},   ///<PI头帧标识
    {DT_LC_HEADER        ,  "LcHdr"},   ///<LC头帧标识
    {DT_LC_TERMINATOR    ,  "LcTer"},   ///<LC终结帧标识
    {DT_CSBK             ,  "CSBK"},
    {DT_MBC_HEADER       ,  "MbcHdr"},
    {DT_MBC_CONTINUE     ,  "MbcCont"},
    {DT_DATA_HEADER      ,  "DtHdr"},
    {DT_R_1_2_DATA       ,  "R12"},
    {DT_R_3_4_DATA       ,  "R34"},
    {DT_IDLE             ,  "Idle"},
    {DT_R_1_1_DATA       ,  "R11"}
};
T_TypePrint *ptDllDataTable = atDllDataTable;
unsigned short usDllDataTableLen = sizeof(atDllDataTable)/sizeof(atDllDataTable[0]);

// table-7 CSBKO 类型调试表
T_TypePrint atCsbkoTable[] = {
    {PRE_CSBKO               ,    "Pre"      },    // 预载波
    {REQ_CSBKO               ,    "Req"      },    // Common Signalling Request CSBKO
    {ACK_CSBKO               ,    "Ack"      },    // Common Signalling Answer Response CSBKO
    {ALARM_CSBKO             ,    "Alarm"    },    // Digital Alarm Service Request CSBKO
};
T_TypePrint *ptCsbkoTable = atCsbkoTable;
unsigned short usCsbkoTableLen = sizeof(atCsbkoTable)/sizeof(atCsbkoTable[0]);

// table-8 SSO 类型调试表--by zhoudayuan
T_TypePrint atSsoTable[] = {
    {CALL_ALERT_SSO          ,   "CallAlert" },    // Call Alert Service
    {EN_RADIO_SSO            ,   "En"        },    // Radio Enable Service
    {DIS_RADIO_SSO           ,   "Dis"       },    // Radio Disable Service
//    {DIGITAL_ALARM_SSO       ,   "Alarm"     },    // 该值暂定-Digital Alarm Service
};
T_TypePrint *ptSsoTable = atSsoTable;
unsigned short usSsoTableLen = sizeof(atSsoTable)/sizeof(atSsoTable[0]);


// table-9  GPS
T_TypePrint Table_Gps_Status[] = {
    {GPS_REQ  ,  "GPS_REQ"},
    {GPS_ACK  ,  "GPS_ACK"},
    {GPS_OK   ,  "GPS_OK "},
    {GPS_PRE  ,  "GPS_PRE"},
    {GPS_RLY  ,  "GPS_RLY"},
    {0xff     ,  "0xFF"   }
};

T_TypePrint *pTable_Gps_Status = Table_Gps_Status;
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
    for (i = 0; i < usSigTableLen; i++)
    {
        if (pSharedHead->SigType == ptSigTable[i].Type)
        {
            if (ptSigTable[i].TypeStr != NULL)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s][%s]@SIG:[%#x-%s]", _F_, prompt, ptSigTable[i].Type, ptSigTable[i].TypeStr);
                break;
            }
        }
    }
}


// 将短消息命令转换为字符串
char *SetCCSig2Str(CENTER_CMD_SHARE_HEAD *pSharedHead, char *BufGet, unsigned short BufLen)
{
    int i;
    for (i = 0; i < usSigTableLen; i++)
    {
        if (pSharedHead->SigType == ptSigTable[i].Type)
        {
            if (ptSigTable[i].TypeStr != NULL)
            {
                snprintf(BufGet, BufLen, "[%#x-%s]", ptSigTable[i].Type, ptSigTable[i].TypeStr);
                return BufGet;
            }
            break;
        }
    }
    return strncpy(BufGet, "No The Sig", strlen("No The Sig")+1);
}


// 将短消息命令转换为字符串-未来替换 SetCCSig2Str
char *SetType2Str(unsigned short Type, unsigned short TableLen, char *BufGet, unsigned short BufLen)
{
    unsigned short i;
    for (i = 0; i < TableLen; i++)
    {
        if (Type == ptSigTable[i].Type)
        {
            if (ptSigTable[i].TypeStr != NULL)
            {
                snprintf(BufGet, BufLen, "[%#x-%s]", ptSigTable[i].Type, ptSigTable[i].TypeStr);
                return BufGet;
            }
            break;
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
void PrintSmsLog(unsigned char SmsType, char *buf, int len)
{
    int i;
    for (i = 0; i < pTable_sms_len; i++)
    {
        if (SmsType == pTable_sms[i].Type)
        {
            if (pTable_sms[i].TypeStr != NULL)
            {
                snprintf(buf , len, "[%#x-%s]", pTable_sms[i].Type, pTable_sms[i].TypeStr);
                return;
            }
        }
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
    for (i = 0; i < usMsgTableLen; i++)
    {
        if (ptDllData->MsgType == ptMsgTable[i].Type)
        {
            if (ptMsgTable[i].TypeStr != NULL)
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
        snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#06x-%s], DATA:[%#04x-%s]", ptMsgTable[i_index].Type, ptMsgTable[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
        LOG_DEBUG(s_LogMsgId, "[CCL][%s][%s]@%s", _F_, prompt, LogBuff);
    }
}


// 上行  DLL TO CCL
void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData, char *MsgBuf, char *DataBuf)
{
    unsigned short i, j;

    // 打印Msg
    for (i = 0; i < usMsgTableLen; i++)
    {
        if (ptDllData->MsgType == ptMsgTable[i].Type)
        {
            if (ptMsgTable[i].TypeStr != NULL)
            {
                snprintf(MsgBuf, 50, "[%d-%s]", ptMsgTable[i].Type, ptMsgTable[i].TypeStr);
                break;
            }
        }
    }

    // 打印data
    if ((ptDllData->MsgType == DI_MSG_DATA) || (ptDllData->MsgType == DI_MSG_WLU))
    {
        // 打印data
        for (j = 0; j < pTable_data_len; j++)
        {
            if (ptDllData->DataType == pTable_data[j].Type)
            {
                if (pTable_data[j].TypeStr != NULL)
                {
                    snprintf(DataBuf, 50, "[%#x-%s]", pTable_data[j].Type, pTable_data[j].TypeStr);
                    break;
                }
            }
        }
    }
    else if (ptDllData->MsgType == DI_MSG_IDLE)
    {
        snprintf(DataBuf, 50, "[%d-%s]", 123, "DI_MSG_IDLE");
        return;
    }
    else if (ptDllData->MsgType == DI_MSG_VOICE)
    {
        snprintf(DataBuf, 50, "[%d]", 0);
        return;
    }
    else if (ptDllData->MsgType == DI_MSG_NEAR)
    {
        snprintf(DataBuf, 50, "[%d-%s]", 5, "DI_MSG_NEAR");
        return;
    }
    else if (ptDllData->MsgType == DI_MSG_NM)
    {
        snprintf(DataBuf, 50, "[%d-%s]", 456, "DI_MSG_NM");
        return;
    }
    else
    {
        snprintf(DataBuf, 50, "[%d-%s]", 789, "Data Err");
        return;
    }
}

#if 0
void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData, char *MsgBuf, char *DataBuf)
{
    unsigned short i, j;
    int i_index = -1, j_index = -1;
    char LogBuff[200] = {0};

    if (1 != tcclPrint->DllUp)
    {
        return ;
    }



    // 打印Msg
    for (i = 0; i < usMsgTableLen; i++)
    {
        if (ptDllData->MsgType == ptMsgTable[i].Type)
        {
            if (ptMsgTable[i].TypeStr != NULL)
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


        if ((i_index != -1) && (j_index != -1))
        {
            snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#x-%s], DATA:[%#x-%s]", ptMsgTable[i_index].Type, ptMsgTable[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
            LOG_DEBUG(s_LogMsgId, "[CCL][%s][%s]@%s", _F_, prompt, LogBuff);
        }
    }
}
#endif




// 下行  CCL TO DLL
void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, char *buf, int len)
{

    unsigned short i, j;
    int i_index = -1, j_index = -1;

    if (1 == tcclPrint->CclDown)
    {

        // 打印Msg
        for (i = 0; i < usMsgTableLen; i++)
        {
            if (ptDllData->MsgType == ptMsgTable[i].Type)
            {
                if (ptMsgTable[i].TypeStr != NULL)
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
            snprintf(buf, len, "MsgType=[%#x-%s],DataType=[%#x-%s]", ptMsgTable[i_index].Type, ptMsgTable[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
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
        for (i = 0; i < usMsgTableLen; i++)
        {
            if (ptDllData->MsgType == ptMsgTable[i].Type)
            {
                if (ptMsgTable[i].TypeStr != NULL)
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
            snprintf(LogBuff, sizeof(LogBuff), "MSG:[%#x-%s], DATA:[%#x-%s]", ptMsgTable[i_index].Type, ptMsgTable[i_index].TypeStr, pTable_data[j_index].Type, pTable_data[j_index].TypeStr);
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

    for (i = 0; i < usCmdTableLen; i++)
    {
        if (pNasAiData->cmd_code == ptCmdTable[i].Type)
        {
            if (ptCmdTable[i].TypeStr != NULL)
            {
                i_index = i;
                break;
            }
        }
    }

    for (j = 0; j < usOpTableLen; j++)
    {

        if (pNasAiData->op_code == ptOpTable[j].Type)
        {
            if (ptOpTable[j].TypeStr != NULL)
            {
                j_index = j;
                break;
            }
        }
    }

    if ((i_index != -1) && (j_index != -1))
    {
        snprintf(LogBuff, sizeof(LogBuff), "CMD=[%#x-%s], OP=[%#x-%s]", ptCmdTable[i_index].Type, ptCmdTable[i_index].TypeStr, ptOpTable[j_index].Type, ptOpTable[j_index].TypeStr);
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
    for (i = 0; i < usDllDataTableLen; i++)
    {
        if (pDataLink->DataType == ptDllDataTable[i].Type)
        {
            if (ptDllDataTable[i].TypeStr != NULL)
            {
                LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s]@AI [%#x-%s]", _F_, prompt, ptDllDataTable[i].Type, ptDllDataTable[i].TypeStr);
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

     for (i = 0; i < usCsbkoTableLen; i++)
     {
         if (pSupsCsbk->uCSBKO == ptCsbkoTable[i].Type)
         {
             if (ptCsbkoTable[i].TypeStr != NULL)
             {
                 i_index = i;
                 break;
             }
         }
     }

    for (j = 0; j < usSsoTableLen; j++)
    {

         if (pSupsCsbk->uSSO == ptSsoTable[j].Type)
         {
            if (ptSsoTable[j].TypeStr != NULL)
            {
                j_index = j;
                break;
            }
         }
    }

     if ((i_index != -1) && (j_index != -1))
     {
         snprintf(LogBuff, sizeof(LogBuff), "CSBKO:[%#x-%s], SSO:[%#x-%s]", ptCsbkoTable[i_index].Type, ptCsbkoTable[i_index].TypeStr, ptSsoTable[j_index].Type, ptSsoTable[j_index].TypeStr);
         LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s]@%s", _F_, prompt, LogBuff);
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
void DLL_SetIdDec2buf(unsigned char *pucSrc, unsigned char *pucDst, char *OutBuf, unsigned int OutBufLen)
{
    unsigned char ucSrcTmp[4];
    unsigned char ucDstTmp[4];
    unsigned int  uiSrcVal;
    unsigned int  uiDstVal;

    if ((pucSrc[1]==0 && pucSrc[2]==0) || (pucDst[1]== 0 && pucDst[2]== 0))
    {
        snprintf(OutBuf, OutBufLen, "snd=%d->rcv=%d", pucSrc[0], pucDst[0]);
    }
    else
    {
        // 转回小端模式
        ucSrcTmp[0] = pucSrc[2];
        ucSrcTmp[1] = pucSrc[1];
        ucSrcTmp[2] = pucSrc[0];
        ucSrcTmp[3] = '\0';
        ucDstTmp[0] = pucDst[2];
        ucDstTmp[1] = pucDst[1];
        ucDstTmp[2] = pucDst[0];
        ucDstTmp[3] = '\0';
        memcpy(&uiSrcVal, ucSrcTmp, sizeof(ucSrcTmp));
        memcpy(&uiDstVal, ucDstTmp, sizeof(ucDstTmp));
        snprintf(OutBuf, OutBufLen, "S:%d->R:%d", uiSrcVal, uiDstVal);
    }
    return ;
}


/**
 * @brief  大端转小端，将源ID和目的ID以十进制打印出来，方便调试看
 * @author 周大元
 * @since
 * @bug
 */
void SetIdDec2buf(unsigned char *pucSrc, unsigned char *pucDst, char *OutBuf, unsigned int OutBufLen)
{
    unsigned char ucSrcTmp[4];
    unsigned char ucDstTmp[4];
    unsigned int  uiSrcVal;
    unsigned int  uiDstVal;

    if ((pucSrc[1]==0 && pucSrc[2]==0) || (pucDst[1]== 0 && pucDst[2]== 0))
    {
        snprintf(OutBuf, OutBufLen, "S:%d->R:%d", pucSrc[0], pucDst[0]);
    }
    else
    {
        // 转回小端模式
        ucSrcTmp[0] = pucSrc[2];
        ucSrcTmp[1] = pucSrc[1];
        ucSrcTmp[2] = pucSrc[0];
        ucSrcTmp[3] = '\0';
        ucDstTmp[0] = pucDst[2];
        ucDstTmp[1] = pucDst[1];
        ucDstTmp[2] = pucDst[0];
        ucDstTmp[3] = '\0';
        memcpy(&uiSrcVal, ucSrcTmp, sizeof(ucSrcTmp));
        memcpy(&uiDstVal, ucDstTmp, sizeof(ucDstTmp));
        snprintf(OutBuf, OutBufLen, "snd=%d->rcv=%d", uiSrcVal, uiDstVal);
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

    // 每帧A-F 有多少误码,最后一个超帧
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




void ErrRateVoiceTest(DATA_LINK_T *pDataLink, int state)
{
    if (ptErrPrint->Voice == 0) // 语音误码率开关关闭
    {
        return;
    }

    switch (state)
    {
        case SET_ERR_HDR:  // 设置为接收语音头
        {
            if (g_ErrVoiceState == WAIT_ERR_VOICE_HDR)  // 当前状态是否为接收语音头
            {
                test_err_voice_init();
                g_ErrVoiceState = WAIT_ERR_VOICE_TER;  // 当前状态切换为接收语音结束
                DLL_ClearTimer();  // 清除超时处理
            }
            break;
        }
        case SET_ERR_TER: // 设置为接收语音结束
        {
            if (g_ErrVoiceState == WAIT_ERR_VOICE_TER)  // 当前状态是否为接收语音结束
            {
                test_err_voice_result();
                g_ErrVoiceState = WAIT_ERR_VOICE_HDR;  // 当前状态切换为接收语音头
                DLL_ClearTimer();  // 清除超时处理
            }
            break;
        }
        case SET_ERR_RCV:
        {
            if (pDataLink != NULL)
            {
                LOG_DEBUG(s_LogMsgId,"Rcv:%d-%s", ptErrVoice->FrmCnt, ptErrVoice->pErrFrm[(pDataLink->FrmType-1)%(FRM_F+1)].FrmNameStr);
                test_err_voice_dll(pDataLink); //  语音接收时间
                if (pDataLink->FrmType == FT_VOICE_A)
                {
                    // 设置超时处理，如果在指定时间(1.5s)内没有清定时器，则说明没有收到 Terminator,
                    DLL_SetTimer(CALL_ERR_VOICE_U, 1500);  // 设置超时时间
                }
            }
            break;
        }
        default:
        {
            LOG_DEBUG(s_LogMsgId,"[%s:%d] Err: default\n", __FUNCTION__, __LINE__);
            break;
        }
    }
}




void InitDebug()
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
#if NER_CHECK_DEBUG
//    g_fpNerCheckLog = fopen(NER_CHECK_LOG_PATH, "w+");
    g_fpNerCheckLog = fopen(NER_CHECK_LOG_PATH, "w+");
    if (g_fpNerCheckLog == NULL)
    {
        printf("Err: open=%s\n", NER_CHECK_LOG_PATH);
        fclose(g_fpNerCheckLog);
        return;
    }
#endif
}





T_TypePrint atCclState[] = {
    {INF_CCL_IDLE        ,   "CCL_IDLE"       },
    {CENTER_VOICE_DL     ,   "CenterVoiceDl"  },
    {Wait_Stun_Ms_Ack    ,   "WaitStunMsAck"  },
    {Wait_Stun_Nas_Ack   ,   "WaitStunNasAck" },
    {Wait_Kill_Nas_Ack   ,   "WaitKillNasAck" },
    {Wait_Gps_Nas_Ack    ,   "WaitGpsNasAck"  },
    {Wait_Gps_Ms_Ack     ,   "WaitGpsMsAck"   },
    {Wait_NEGHR_Nas_Ack  ,   "WaitNerNasAck"  },
    {Wait_Reviv_NAS_Ack  ,   "WaitRcvNasAck"  },
    {Wait_Reviv_MS_Ack   ,   "WaitRcvMsAck"   },
    {NAS_VOICE_UL        ,   "NasVoiceUl"     },
    {NAS_STAT_STUNED     ,   "NasStatStuned"  },
    {INF_CCL_DISABLE     ,   "InfCclDisable"  },
    {SMS_SEND            ,   "SmsSend"        }
};

T_TypePrint *ptCclState = atCclState;

char *GetCclStateByStr(unsigned char CclState, char *buf)
{
    unsigned char i;
    for (i = 0; i < sizeof(atCclState)/sizeof(atCclState[0]); i++)
    {
        if (ptCclState[i].Type ==  CclState)
        {
            strcpy(buf, ptCclState[i].TypeStr);
            return buf;
        }
    }
    strcpy(buf, "Err: No This State");
    return buf;
}



/*********************************************************************************
**********************************************************************************
***************************定时器*************************************************
**********************************************************************************
**********************************************************************************/


typedef struct _T_Timer {
    unsigned short item;
    const char *name;
    unsigned int Switch;
    unsigned int Cnt;
    F_Timer TimerHandle;
} T_Timer;




// 定时初始化
T_Timer TimerTable[] = {

    {TMR_NER_ACK,     "TMR_NER_ACK",     0/*switch*/,    0/*cnt=17, 1000ms*/,  SetTimerOff/*fun*/},
    {NAS_PRE_IDP,     "NAS_PRE_IDP",     0/*switch*/,    0/**/,                SetTimerOff/*fun*/},
    {NAS_PRE_ODP,     "NAS_PRE_ODP",     0/*switch*/,    0/**/,                SetTimerOff/*fun*/},
    {FLC_GPS_ODP,     "FLC_GPS_ODP",     0/*switch*/,    0,                    SetTimerOff/*fun*/},
//    {TMR_MAX,         "TMR_MAX",         0/*switch*/,    0/**/,                NULL/*fun*/},
};

unsigned short usTimerTableLen = sizeof(TimerTable)/sizeof(TimerTable[0]);




int RegisterTimerFun(unsigned short item, F_Timer TimerHandle)
{  
    int i;
    for (i = 0; i < usTimerTableLen; i++)
    {
        if (TimerTable[i].item == item)
        {
            TimerTable[i].TimerHandle = TimerHandle;
            return 0;
        }
    }
    return -1;
}



/**
 * @brief  定时器计时
 * @author  周大元
 */
void TimerTask()
{
    unsigned short i;
    for (i = 0; i < usTimerTableLen; i++)
    {
        if (TimerTable[i].Switch == 1)
        {
            if (TimerTable[i].Cnt != 0)
            {
                TimerTable[i].Cnt --;
            }
            else
            {
                if (TimerTable[i].TimerHandle != NULL)
                {
                    TimerTable[i].TimerHandle(i);
                }
            }
        }
    }
}


/**
 * @brief  获取定时器当前时间
 * @author  周大元
 */
int GetTimerCnt(unsigned int item)
{
    if (item >= usTimerTableLen)
    {
        return -1;
    }
    return TimerTable[item].Cnt;
}

/**
 * @brief  启动定时器
 * @author  周大元
 */
int SetTimerOn(unsigned short item, unsigned int cnt, F_Timer TimerHandle)
{
    if (item >= usTimerTableLen)
    {
        return -1;
    }

    TimerTable[item].Cnt = cnt;
    TimerTable[item].TimerHandle = TimerHandle;
    TimerTable[item].Switch = 1;    // 必须放在最后赋值
    return 0;
}

/**
 * @brief  关闭定时器
 * @author  周大元
 */
void SetTimerOff(unsigned short index)
{
    TimerTable[index].Cnt = 0;
    TimerTable[index].Switch = 0;
}



/*********************************************************************************
**********************************************************************************
***************************IDP-打印************************************************
**********************************************************************************
**********************************************************************************/
void SearchAiItem(T_TypePrint *ptAiTable, unsigned short usLen, unsigned char ucItem, const char **ppItemStr)
{
    unsigned short i;
    for (i = 0; i < usLen; i++)
    {
        if (ucItem == ptAiTable[i].Type)
        {
            if (ptAiTable[i].TypeStr != NULL)
            {
                *ppItemStr = ptAiTable[i].TypeStr;
                return;
            }
        }
    }
    *ppItemStr = "";
}



/**
 * @brief   将消息转化为字符串
 * @author  周大元
 */
void GetAiItemStr(DATA_LINK_T *ptDataLink, char *Buf, unsigned short usBufLen)
{
    const char *pStrData = NULL;
    const char *pStrCmd = NULL;
    const char *pStrOp = NULL;
    const char *pStrCsbk = NULL;
    const char *pStrsSso = NULL;

    if (ptDataLink->MsgType == DI_MSG_WLU)
    {
        NAS_AI_PAYLOAD *pNasAiData = (NAS_AI_PAYLOAD *)(ptDataLink->PayLoad);
        SearchAiItem(ptCmdTable, usCmdTableLen, pNasAiData->cmd_code, &pStrCmd);
        if (pNasAiData->cmd_code != CMO_CODE_NAS_PRE)
        {
            SearchAiItem(ptOpTable, usOpTableLen, pNasAiData->op_code, &pStrOp);
            snprintf(Buf, usBufLen, "W:%s:%s", pStrCmd, pStrOp);        
            return;
        }
        snprintf(Buf, usBufLen, "W:%s", pStrCmd);
        return;
    }
    else if (ptDataLink->MsgType == DI_MSG_DATA)
    {
        SUPS_CSBK_PDU *ptSupsCsbkPdu = (SUPS_CSBK_PDU *)(ptDataLink->PayLoad);
        SearchAiItem(ptDllDataTable, usDllDataTableLen, ptDataLink->DataType, &pStrData);
        if (ptDataLink->DataType == DT_CSBK)
        {
            SearchAiItem(ptCsbkoTable, usCsbkoTableLen, ptSupsCsbkPdu->uCSBKO, &pStrCsbk);
            if ((ptSupsCsbkPdu->uCSBKO != PRE_CSBKO) && (ptSupsCsbkPdu->uCSBKO != ALARM_CSBKO))
            {
                SearchAiItem(ptSsoTable, usSsoTableLen, ptSupsCsbkPdu->uCSBKO, &pStrsSso);
                snprintf(Buf, usBufLen, "D:%s:%s:%s", pStrData, pStrCsbk, pStrsSso);
                return;
            }
            snprintf(Buf, usBufLen, "D:%s:%s", pStrData, pStrCsbk);
            return;
        }
        snprintf(Buf, usBufLen, "D:%s", pStrData);
        return;

    }
    else if (ptDataLink->MsgType == DI_MSG_NEAR)
    {
        snprintf(Buf, usBufLen, "B");
        return;
    }
    else if (ptDataLink->MsgType == DI_MSG_VOICE)
    {
        Buf[0] = 'V';
        Buf[1] = '\0';
        return;
    }
    Buf[0] = '\0';
    return;
}

