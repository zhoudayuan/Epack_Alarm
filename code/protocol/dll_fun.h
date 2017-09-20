#ifndef __DLL_FUN_H__
#define __DLL_FUN_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    DLL.h
 * @brief    声明全部协议栈内部处理函数
 * @author  陈禹良
 * @version v1.0
 * @date    2016-7-20
 */
/*
 *   函数列表
 *   1. ALG_AdjustBackoff               终端退避门限参数调整
 *   2. ALG_Crc8                        crc8码编码函数
 *   3. ALG_Crc16                       计算多个8位信息码字的CRC16值
 *   4. ALG_Crc32                       计算多个16位信息码字的CRC32值
 *   5. DBC_GenIdle                     生成时隙链表尾节点
 *   6. DBC_Alloc                       预分配用于承载数据PDU的节点组
 *   7. DBC_Free                        清除链表节点
 *
 *   修改历史
 *   2016-7-20                 陈禹良                     建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "dll.h"
#include "dll_interleave.h"
#include "dll_algrthm.h"


/******************************************************************************
 *   类型定义
 *   *************************************************************************/

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/

/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/

/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/

/******************************************************************************
 *   调用函数声明
 *   *************************************************************************/
/**
 * @brief   数据链路层初始化函数
 */
int DLL_Init(void);

/**
 * @brief   数据链路层更新fpga共享内存
 */
void DLL_SyncFpgaCfg(void);

/**
 * @brief  上行转发时隙频率
 */
void IDP_ReptFreqSlot(UINT8 RevSlot, UINT8 RevFrq);

/**
 * @brief  上行GPS空口数据转换NAS数据
 */
void IDP_GpsAiToNas(GPS_AI_DATA_T *pvAiGps, GPS_NAS_DATA_T *pvNasGps);

/**
 * @brief  上行GPS 设备数据转换NAS数据
 */
void IDP_GpsDevToNas(GPS_DEV_DATA_T *pvDevGps, GPS_NAS_DATA_T *pvNasGps);

/**
 * @brief  上行锁定时隙频率
 */
int IDP_LockFreqSlot(UINT8 RevSlot, UINT8 RevFrq1, UINT8 RevFrq2);

/**
 * @brief  上行中转链路数据处理函数
 */
void IDP_RelayLinkData(NAS_INF_UL_T * pvInfData, UINT8 RevFrqNo, UINT8 SapType);

/**
 * @brief 呼叫控制层下行线程
 */
void * CCLDownloadTask(void * p);

/**
 * @brief 接口适配层上行线程
 */
void * INFUploadTask(void * p);

/**
 * @brief   数据链路层定时器线程
*/
void * DLL_TimerTask(void * p);

/**
 * @brief   数据链路层邻点突发线程
*/
void * DLL_NerBurstTask(void * p);

/**
 * @brief   数据链路层设置定时器
*/
void DLL_SetTimer(UINT8 CallState, UINT16 CallWait);

/**
 * @brief   数据链路层清除定时器
*/
void DLL_ClearTimer(void);

/**
 * @brief 上行分组数据标记初始化
 */
void   IDP_InitFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB, UINT8 uValue);

/**
 * @brief   检查分组数据图样中所有bit是否全为1
 */
BOOL   IDP_CheckFlag(UINT8 *puData, UINT8 IndexA, UINT8 IndexB);

/**
 * @brief   为图样中的某一bit位赋值
 */
void   IDP_WriteFlag(UINT8 *puData, UINT8 Index, UINT8 uValue);

/**
 * @brief   读取图样中某一bit位的值
 */
BOOL   IDP_ReadFlag(UINT8 *puData, UINT8 Index);

/**
 * @brief crc8码编码函数
 */
UINT8 ALG_Crc8(UINT8* puData, UINT8 uDataLen);

/**
 * @brief crc9码编码函数
 */
UINT16 ALG_Crc9(UINT8 *puData,UINT8 uDataLen);

/**
 * @brief   计算多个8位信息码字的CRC16值
 */
UINT16 ALG_Crc16(const UINT8* puData, UINT16 u2DataCnt);

/**
 * @brief   计算多个8位信息码字的CRC32值
 */
UINT32 ALG_Crc32(const UINT8* puData, UINT16 u2DataCnt);

/**
 * @brief  上行向呼叫控制层发送数据处理
 */
void IDP_SendCclData(DLL_CCL_UL_T * ptCclData);

/**
 * @brief  下行向接口适配层发送数据处理
 */
void ODP_SendInfData(NAS_INF_DL_T * pdInfData, UINT8 SapType);

/**
 * @brief  上行数据打印
 */
void IDP_AiPrintf(NAS_INF_UL_T * pvInfData);

/**
 * @brief  上行数据打印
 */
void IDP_CclPrintf(DLL_CCL_UL_T * ptCclData);

/**
 * @brief  下行向接口适配层发送数据处理
 */
void ODP_AiPrintf(NAS_INF_DL_T * pdInfData, UINT8 SapType);

/**
 * @brief 下行发送数据链路终结帧函数
 */
void ODP_TerDataLC(UINT8 *DstId, UINT8 *SrcId, UINT8 GI, UINT8 ResFrq, UINT8 ResSlt);

/**
 * @brief 下行发送预载波数据处理函数
 */
int ODP_GenPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI);

/**
 * @brief NAS下行发送预载波数据处理函数
 */
int ODP_GenNasPreCSBKFun(UINT8 cbf, UINT8* Dst, UINT8* Src, UINT8 GI);

/**
 * @brief 下行发送MS应答数据头函数
 */
void ODP_GenReponHeader(UINT8 *DstId, UINT8 *SrcId, PDT_RE_HEAD_PDU_DT *p_ReHdrPdu);

/**
 * @brief 下行(196,144)BPTC 编码函数
 */
void ODP_144to196bptc(UINT8 *pData, UINT8 *pAiData);

/**
 * @brief  主呼下行向接口层发送数据处理
 */
void CallingShootData(UINT32 TimeS, UINT8 FrqSlt, UINT8 MsgT, UINT8 FrmT, UINT8 DataT, UINT8 Len, UINT8 *PayLoad);

void seconds_sleep(unsigned long seconds);

void milliseconds_sleep(unsigned long mSec);

void microseconds_sleep(unsigned long uSec);


#endif

