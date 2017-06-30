#ifndef __PRINT_DEBUG__
#define __PRINT_DEBUG__
#include  "mgr_common.h"
#include  "ccl.h"
#include  "typedef.h"
#include  "log.h"
#include  "dll.h"


#define  VOICE_SRC_1031        0 
#define  VOICE_SRC_Silence     1
#define  VOICE_SRC_153         2


//#include "print_debug.h"
// 调试打印信息
typedef struct _TYPE_PRINT_T
{
    int Type;
    const char *TypeStr;
} TYPE_PRINT_T;

typedef struct _TABLE_ERR_FRM_T
{
    unsigned char FrmType;
    const char * FrmTypeStr;
    unsigned int FrmErrBit;
} TABLE_ERR_FRM_T;

typedef struct _ERR_VOICE_T
{
    unsigned char FrmIndx;          // 语音帧索引
    unsigned int FrmRcvCnt;  // 当前检测的语音帧数量
    unsigned int FrmErrBitTotal;  // 
    TABLE_ERR_FRM_T *pRcvErrFrm;
} ERR_VOICE_T;


extern ERR_VOICE_T *pErrVoice;
extern TABLE_ERR_FRM_T *pRcvErrFrm;
extern FILE *g_fp_err_test;



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
extern void SetIdDec2buf(unsigned char *src, unsigned char *dst, char *BufGet, unsigned int BufLen);
char *SetCCSig2Str(CENTER_CMD_SHARE_HEAD *pSharedHead, char *BufGet, unsigned short BufLen);
extern char *PrintGPSFlag(int uGpsFlag);
extern void print_voice_153(UINT8 *pPayLoad, const char *prompt);
//extern UINT16 ones16(UINT16 u2Data);
extern void test_err_voice_ccl(DLL_CCL_UL_T *pvDllData);
extern void test_err_voice_dll(DATA_LINK_T *pDataLink);
extern void test_err_voice_init();
extern void test_err_voice_result();
extern void setDebugInit();



#endif 
