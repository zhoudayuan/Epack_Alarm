#ifndef __CCL_DL_H__
#define __CCL_DL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_dl.h
 * @brief   ���п��Ʋ����к����ӿ�
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   �����б�
 *   1. ODP_GenSilentFrmpacket       ��װCCL���о���֡���ݰ�
 *   2. ODP_GenVoicepacket           ��װCCL����������
 *   3. ODP_GenSmsDataPacket         ��װCCL�����ж���֡
 *   4. ODP_GenLcHeader              ��װ����LCͷ
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
 * @brief   ��װCCL���о���֡���ݰ�
 */
void  ODP_GenSilentFrmpacket(unsigned  char *pvSilentData,unsigned  char *pvDllData,unsigned  char FrmType , int *Len);

/**
 * @brief   ��װCCL����������
 */
void  ODP_GenVoicepacket(unsigned  char *pvCenterData,unsigned  char *pvDllData,unsigned  char FrmType , int *Len);

/**
 * @brief��װCCL�����ж���֡
 */
void ODP_GenSmsDataPacket(unsigned  char * pvCenterData, unsigned  char*pvDllData, int *Len);

/**
* @brief   ��װ����LCͷ
 */
void ODP_GenLcHeader(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len);
/**
* @brief  ��װ���������
 */
void  ODP_GenSigCtrlDataPacket(unsigned  char *pvCenterData ,unsigned  char* pvDllData,int *Len);
/**
* @brief  ��װ��������֡
 */

void ODP_GenTerminatorPacket(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len);
/**
* @brief    ��ӡLC_HEADER ��LC_TERMINATOR
*/
void ODP_CclprintLc(unsigned char  * pvDllData);
/**
* @brief  ��ӡ��������
* @bug
*/
void  ODP_CclPrintCcSig(unsigned char *Pttcmd);
/**
* @brief    ��ӡ ccl_dll ����
*/
void  ODP_CclPrintDllData(unsigned char *DllData);

/**
* @brief    �������ĺ���ID������������
*/
void SavCcIDNetData(unsigned char *pvcenterdata,unsigned char *CCcvNet);
/**
* @brief    ��װ����֡���ݰ�
*/
//void LitToBigSmsData(unsigned char *pvdlldata ,unsigned char *pvCenterData,unsigned short datalen);


#endif //CCLDL_H











































