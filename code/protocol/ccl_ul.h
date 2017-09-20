


#ifndef __CCL_UL_H__
#define __CCL_UL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_ul.h
 * @brief   ���п��Ʋ����к����ӿ�
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   �����б�
 *   1.   IDP_GenPttOnAck          ��װPTT-ON ����ظ����ݰ�
 *   2.   IDP_GenPttOffAck          ��װPTT-OFF ����ظ����ݰ�
 *   3.   IDP_GenPttOnCmd                  ��װPTT-ON�����
 *   4.   IDP_GenVoiceDataPaket           ��װ����������
 *   5.
 *   6.
 *   7.
 *
 *   �޸���ʷ
 *   2016-07-29           �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
//#include "Unit_inf.h"

/******************************************************************************
 *   �궨��
 *   *************************************************************************/
/**
 * @def _ENABLE_CLI
 * @brief ʹ�������нӿڹ��ܿ���
 */
//#define _ENABLE_CLI
/**
 * @def _USE_NET_NCOM
 * @brief ʹ�����ڶ��Ǵ�������/���
 */
//#define _USE_NET_NCOM
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
 *   �ɵ��ú���ʵ��
 *   *************************************************************************/
/**
 * @brief   ��װPTT-ON ����ظ����ݰ�
 */
void  IDP_GenPttOnAck(unsigned  char *pvCenterData, int *Len,unsigned  char PttCmdResult);

/**
 * @brief   ��װPTT-OFF ����ظ����ݰ�
 */
void  IDP_GenPttOffAck(unsigned  char *pvCenterData, int *Len);

/**
 * @brief   ��װPTT �����
 */


void IDP_GenPttCmd(unsigned char *pvDllData,unsigned char *pvCenterData,unsigned char cmd,int *Len);

/**
 * @brief   ��װPTT-ON �����
 */
//void IDP_GenPttOnCmd(unsigned char *pvCenterData,unsigned short *Len);
/**
 * @brief   ��װ����������
 */
void IDP_GenVoiceDataPaket(unsigned char *pvDllData,unsigned char *pvCenterData,int *Len);
/**
 * @brief   ��װ�������ݰ�
 */

void IDP_GenSmsPacket(unsigned char *pvDllData,unsigned char *pvCenterData,int *Len);
/**
* @brief    ����������ӦPTTON �����
 */
void  IDP_PttOnAckHandle(unsigned char *pvCenterData);
/**
* @brief    ����LC ����
* @bug
*/

void IDP_GetLcData(unsigned char *pvDllData);
/**
* @brief    ��ӡPTT ������Ӧ����
*/
void IDP_PrintPttAck(unsigned char *CenterData);

/**
 * @brief   ��ӡPTT �����
 */
void IDP_CcPttCmdPrint(unsigned char *CenterData);


/**
* @brief    ��ӡ dll_ccl ����
*/
void IDP_DllDataPrint(unsigned char * ptDllData);

/**
* @brief    ��ӡ ���ж���Ϣ
*/

void IDP_CclSmsPrint(unsigned char* CenterData);
/**
* @brief    ��װ����������GPS ���ݰ�
*/
void  IDP_GenGpsData(unsigned char pvAcktype ,unsigned char * pvDllData,unsigned char * pvCenterData,int *Len,unsigned char *CallIdNet);
/**
* @brief    ��ӡGPS���ݰ�
*/
void    IDP_CclPrintGpsData(unsigned char *pvGpsData);
/**
* @brief    ��װ�����������ڵ����ݰ�
*/
void  IDP_GenNearData(unsigned char neartyppe, unsigned char * pvDllData,unsigned char * pvCenterData,int *Len,unsigned char *CallIdNet);

/**
* @brief    ��װNAS ң�Ρ�ң�С�������Ӧ���ݰ�
*/

void  IDP_GenNasSigAckData(unsigned char pvAcktype,unsigned char * pvDllData,unsigned char * pvCenterData,int *Len);

/**
* @brief    ��װң����Ӧ���ݰ�
*/

void  IDP_GenMsAckData(unsigned char datatype,unsigned char * pvDllData,unsigned char * pvCenterData,int *Len,unsigned char *pvCccvnetId);
/**
* @brief    ��װ���������ĳ�ǿ���ݰ�
*/
void  IDP_GenThrethHoldData( unsigned char * pvDllData,unsigned char * pvCenterData,int *Len);
/**
 * @brief   ��װNAS״̬�ϱ����ݰ�
 */
void  IDP_GenNasStatePack(unsigned char pvrelayflg,unsigned char *pvCenterData,int *len);
/**
* @brief    ?????????
*/

void IDP_GenAlarm_ClearData(unsigned char type,unsigned char * pvDllData,unsigned char * pvCenterData,int *Len);

#endif //CCLUL_H





