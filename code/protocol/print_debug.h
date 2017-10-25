#ifndef __PRINT_DEBUG__
#define __PRINT_DEBUG__
#include  "mgr_common.h"
#include  "ccl.h"
#include  "typedef.h"
#include  "log.h"
#include  "dll.h"
#include  "sap.h"



/*------ ���Կ���------*/

#define EMB_GPS_DEBUG                   0  // GPS-����

//  DEBUG SWITCH 0-off , 1-on
//------------�ڵ����----------------------
#define BURST_CYC_VALUE                 0   // ��ʽ�汾Ϊ0, �ڵ�ͻ�����ڿ��ء��ϴ�SVN����0�� ������ֵΪ0ʱ���ڵ�ͻ������Ϊ������ֵ��������ֵΪ��0ʱ��Ϊ����ģʽ����ֵ�ڵ�ͻ�����ڣ�ȡֵ��Χ > 10�����ڵ�ͻ������10���ӡ�
#define NER_CHECK_DEBUG                 0


//------�����ʵ��Կ���------
#define ERR_VOICE_DEBUG                 0  // ��ʼ��Ĭ��д�ļ��رգ���ʼ����������
#define ERR_D_21_DEBUG                  0
#define ERR_D_43_DEBUG                  0

//--------------------------
#define  VOICE_SRC_153                  2

#define  WAIT_ERR_VOICE_HDR             0  // ��ǰ״̬-�ȴ�����ͷ
#define  WAIT_ERR_VOICE_TER             1  // ��ǰ״̬-�ȴ���������
#define  SET_ERR_HDR                    0
#define  SET_ERR_TER                    1  
#define  SET_ERR_RCV                    2




typedef enum _FRAME_CNT_E
{
    FRM_A    = 0,               ///< ����A֡��ʶ
    FRM_B    = 1,               ///< ����B֡��ʶ
    FRM_C    = 2,               ///< ����C֡��ʶ
    FRM_D    = 3,               ///< ����D֡��ʶ
    FRM_E    = 4,               ///< ����E֡��ʶ
    FRM_F    = 5,               ///< ����F֡��ʶ
} FRAME_CNT_E;


//------------��ʱ��----------------
#define TIMER_BASE  60  // 60MS


// ˳��������������������������, �����������β��
typedef enum _E_TIME_ITEM
{
    TMR_NER_ACK    = 0,   
    NAS_PRE_IDP    = 1,
    NAS_PRE_ODP    = 2,
    FLC_GPS_ODP    = 3,
    TMR_MAX        = 4,
} E_TIME_ITEM;

typedef void (*F_Timer)(unsigned short index);



// ���Դ�ӡ��Ϣ
typedef struct _T_TypePrint
{
    int Type;
    const char *TypeStr;
} T_TypePrint;

typedef struct _ERR_FRM_T
{
    unsigned char FrmTypeID;
    const char *FrmNameStr;
    unsigned int FrmErrBit;
} ERR_FRM_T;

typedef struct _ERR_VOICE_T
{
    unsigned char FrmIndx;   // ����֡����
    unsigned int FrmCnt;  // ��ǰ��������֡����
    unsigned int ErrTotal;//���г���ı�����
    ERR_FRM_T *pErrFrm;
} ERR_VOICE_T;


extern ERR_VOICE_T *ptErrVoice;
extern ERR_FRM_T *ptErrFrm;
extern FILE *g_fp_err_test;

// sig
extern void IDP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead);
extern void ODP_PrintSigLog(CENTER_CMD_SHARE_HEAD *pSharedHead);
// sms
extern void IDP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData);
extern void ODP_PrintSmsLog(SMS_CENTER_CCL_DL *ptCenterData);
extern void PrintSmsLog(unsigned char SmsType, char *buf, int len);

extern void ODP_PrintSms(SMS_CENTER_CCL_DL *ptCenterData, char *buf, int len);

// Msg & Data
extern void IDP_PrintMsgDatalog(DLL_CCL_UL_T  *ptDllData, char *pucMsgBuf, char *pucDataBuf);
extern void ODP_PrintMsgDatalog(CCL_DLL_DL_T *ptDllData, char *buf, int len);
// Cmd & Op
extern void IDP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData);
extern void ODP_PrintNasCmdOpLog(NAS_AI_PAYLOAD *pNasAiData);
// InfData
extern void IDP_PrintInfDataLog(DATA_LINK_T *pDataLink);
extern void ODP_PrintInfDataLog(DATA_LINK_T *pDataLink);
// CSBK & SSO
extern void IDP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk);
//extern void ODP_PrintCsbkoSSOLog(SUPS_CSBK_PDU *pSupsCsbk);
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
extern void InitDebug();
extern void ErrRateVoiceTest(DATA_LINK_T *pDataLink, int state);
extern char *GetCclStateByStr(unsigned char CclState, char *buf);
extern int SetTimerOn(unsigned short item, unsigned int cnt, F_Timer TimerHandle);
extern void SetTimerOff(unsigned short index);
extern int GetTimerCnt(unsigned int item);
extern void TimerTask();
extern void GetAiItemStr(DATA_LINK_T *ptDataLink, char *Buf, unsigned short usBufLen);
extern int RegisterTimerFun(unsigned short item, F_Timer TimerHandle);
extern void DLL_SetIdDec2buf(unsigned char *pucSrc, unsigned char *pucDst, char *OutBuf, unsigned int OutBufLen);

extern FILE *g_fpNerCheckLog;
#endif
