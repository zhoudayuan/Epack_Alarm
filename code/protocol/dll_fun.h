#ifndef __DLL_FUN_H__
#define __DLL_FUN_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    DLL.h
 * @brief    ����ȫ��Э��ջ�ڲ�������
 * @author  ������
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   �����б�
 *   1. ALG_AdjustBackoff               �ն��˱����޲�������
 *   2. ALG_Crc8                        crc8����뺯��
 *   3. ALG_Crc16                       ������8λ��Ϣ���ֵ�CRC16ֵ
 *   4. ALG_Crc32                       ������16λ��Ϣ���ֵ�CRC32ֵ
 *   5. DBC_GenIdle                     ����ʱ϶����β�ڵ�
 *   6. DBC_Alloc                       Ԥ�������ڳ�������PDU�Ľڵ���
 *   7. DBC_Free                        �������ڵ�
 *
 *   �޸���ʷ
 *   2016-7-20                 ������                     �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "dll.h"
#include "dll_interleave.h"
#include "dll_algrthm.h"


/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/

/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/

/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/

/******************************************************************************
 *   �ڲ�����ʵ��
 *   *************************************************************************/

/******************************************************************************
 *   ���ú�������
 *   *************************************************************************/
/**
 * @brief   ������·���ʼ������
 */
int DLL_Init(void);

/**
 * @brief   ������·�����fpga�����ڴ�
 */
void DLL_SyncFpgaCfg(void);

/**
 * @brief  ����ת��ʱ϶Ƶ��
 */
void IDP_ReptFreqSlot(UINT8 RevSlot, UINT8 RevFrq);

/**
 * @brief  ����GPS�տ�����ת��NAS����
 */
void IDP_GpsAiToNas(GPS_AI_DATA_T *pvAiGps, GPS_NAS_DATA_T *pvNasGps);

/**
 * @brief  ����GPS �豸����ת��NAS����
 */
void IDP_GpsDevToNas(GPS_DEV_DATA_T *pvDevGps, GPS_NAS_DATA_T *pvNasGps);

/**
 * @brief  ��������ʱ϶Ƶ��
 */
int IDP_LockFreqSlot(UINT8 RevSlot, UINT8 RevFrq1, UINT8 RevFrq2);

/**
 * @brief  ������ת��·���ݴ�����
 */
void IDP_RelayLinkData(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo, UINT8 SapType);

/**
 * @brief ���п��Ʋ������߳�
 */
void * CCLDownloadTask(void * p);

/**
 * @brief �ӿ�����������߳�
 */
void * INFUploadTask(void * p);

/**
 * @brief   ������·�㶨ʱ���߳�
*/
void * DLL_TimerTask(void * p);

/**
 * @brief   ������·���ڵ�ͻ���߳�
*/
void * DLL_NerBurstTask(void * p);

/**
 * @brief   ������·�����ö�ʱ��
*/
void DLL_SetTimer(UINT8 CallState, UINT16 CallWait);

/**
 * @brief   ������·�������ʱ��
*/
void DLL_ClearTimer(void);

/**
 * @brief ���з������ݱ�ǳ�ʼ��
 */
void   IDP_InitFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB, UINT8 uValue);

/**
 * @brief   ����������ͼ��������bit�Ƿ�ȫΪ1
 */
BOOL   IDP_CheckFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB);

/**
 * @brief   Ϊͼ���е�ĳһbitλ��ֵ
 */
void   IDP_WriteFlag(UINT8 *puData, UINT8 Index, UINT8 uValue);

/**
 * @brief   ��ȡͼ����ĳһbitλ��ֵ
 */
BOOL   IDP_ReadFlag(UINT8 *puData, UINT8 Index);

/**
 * @brief crc8����뺯��
 */
UINT8 ALG_Crc8(UINT8* puData, UINT8 uDataLen);

/**
 * @brief crc9����뺯��
 */
UINT16 ALG_Crc9(UINT8 *puData,UINT8 uDataLen);

/**
 * @brief   ������8λ��Ϣ���ֵ�CRC16ֵ
 */
UINT16 ALG_Crc16(const UINT8* puData, UINT16 u2DataCnt);

/**
 * @brief   ������8λ��Ϣ���ֵ�CRC32ֵ
 */
UINT32 ALG_Crc32(const UINT8* puData, UINT16 u2DataCnt);

/**
 * @brief  ��������п��Ʋ㷢�����ݴ���
 */
void IDP_SendCclData(DLL_CCL_UL_T * ptCclData);

/**
 * @brief  ������ӿ�����㷢�����ݴ���
 */
void ODP_SendInfData(NAS_INF_DL_T * pdInfData, UINT8 SapType);

/**
 * @brief  �������ݴ�ӡ
 */
void IDP_AiPrintf(NAS_INF_UL_T * pvInfData);

/**
 * @brief  �������ݴ�ӡ
 */
void IDP_CclPrintf(DLL_CCL_UL_T * ptCclData);

/**
 * @brief  ������ӿ�����㷢�����ݴ���
 */
void ODP_AiPrintf(NAS_INF_DL_T * pdInfData, UINT8 SapType);

/**
 * @brief ���з���������·�ս�֡����
 */
void ODP_TerDataLC(UINT8 *DstId, UINT8 *SrcId, UINT8 GI, UINT8 ResFrq, UINT8 ResSlt);

/**
 * @brief ���з���Ԥ�ز����ݴ�����
 */
int ODP_GenPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI);

/**
 * @brief NAS���з���Ԥ�ز����ݴ�����
 */
int ODP_GenNasPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI);

/**
 * @brief ���з���MSӦ������ͷ����
 */
void ODP_GenReponHeader(UINT8 *DstId, UINT8 *SrcId, PDT_RE_HEAD_PDU_DT *p_ReHdrPdu);

/**
 * @brief ����(196,144)BPTC ���뺯��
 */
void ODP_144to196bptc(UINT8 *pData, UINT8 *pAiData);

/**
 * @brief  ����������ӿڲ㷢�����ݴ���
 */
void CallingShootData(UINT32 TimeS, UINT8 FrqSlt, UINT8 MsgT, UINT8 FrmT, UINT8 DataT, UINT8 Len, UINT8 *PayLoad);

void seconds_sleep(unsigned long seconds);

void milliseconds_sleep(unsigned long mSec);

void microseconds_sleep(unsigned long uSec);


#endif

