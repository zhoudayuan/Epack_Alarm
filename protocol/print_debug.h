#ifndef __PRINT_DEBUG__
#define __PRINT_DEBUG__
#include  "mgr_common.h"
#include  "ccl.h"
#include  "typedef.h"
#include  "log.h"
#include  "dll.h"



//#include "print_debug.h"
// 调试打印信息
typedef struct _TYPE_PRINT_T
{
    int Type;
    const char *TypeStr;
} TYPE_PRINT_T;

// sig
extern void IDP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead);
extern void ODP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead);
// sms
extern void IDP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData);
extern void ODP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData);
// Msg & Data
extern void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData);
extern void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData);
// Cmd & Op
extern void IDP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData);
extern void ODP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData);
// InfData
extern void IDP_PrintInfDataLog(DATA_LINK_T *pDataLink);
extern void ODP_PrintInfDataLog(DATA_LINK_T *pDataLink);
// CSBK & SSO
extern void IDP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk);
extern void ODP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk);

extern void PrintIDbyDec(unsigned char *src, unsigned char *dst);
#endif 