#ifndef __CCL_DL_H__
#define __CCL_DL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_dl.h
 * @brief   呼叫控制层下行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   函数列表
 *   1. ODP_GenSilentFrmpacket       封装CCL下行静音帧数据包
 *   2. ODP_GenVoicepacket           封装CCL下行语音包
 *   3. ODP_GenSmsDataPacket         封装CCL层下行短信帧
 *   4. ODP_GenLcHeader              封装语音LC头
 *   5.
 *   6.
 *   7.
 *
 *   修改历史
 *   2016-07-29           建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
//#include "Unit_inf.h"

/******************************************************************************
 *   宏定义
 *   *************************************************************************/
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
 *   可调用函数实现
 *   *************************************************************************/
/**
 * @brief   封装CCL下行静音帧数据包
 */
void  ODP_GenSilentFrmpacket(unsigned  char *pvSilentData,unsigned  char *pvDllData,unsigned  char FrmType , int *Len);

/**
 * @brief   封装CCL下行语音包
 */
void  ODP_GenVoicepacket(unsigned  char *pvCenterData,unsigned  char *pvDllData,unsigned  char FrmType , int *Len);

/**
 * @brief封装CCL层下行短信帧
 */
void ODP_GenSmsDataPacket(unsigned  char * pvCenterData, unsigned  char*pvDllData, int *Len);

/**
* @brief   封装语音LC头
 */
void ODP_GenLcHeader(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len);
/**
* @brief  封装下行信令包
 */
void  ODP_GenSigCtrlDataPacket(unsigned  char *pvCenterData ,unsigned  char* pvDllData,int *Len);
/**
* @brief  封装语音结束帧
 */

void ODP_GenTerminatorPacket(unsigned char * pvCenterData, unsigned  char *pvDllData, int* Len);
/**
* @brief    打印LC_HEADER 、LC_TERMINATOR
*/
void ODP_CclprintLc(unsigned char  * pvDllData);
/**
* @brief  打印中心信令
* @bug
*/
void  ODP_CclPrintCcSig(unsigned char *Pttcmd);
/**
* @brief    打印 ccl_dll 数据
*/
void  ODP_CclPrintDllData(unsigned char *DllData);

/**
* @brief    保存中心呼叫ID、子网段数据
*/
void SavCcIDNetData(unsigned char *pvcenterdata,unsigned char *CCcvNet);
/**
* @brief    封装静音帧数据包
*/
//void LitToBigSmsData(unsigned char *pvdlldata ,unsigned char *pvCenterData,unsigned short datalen);


#endif //CCLDL_H











































