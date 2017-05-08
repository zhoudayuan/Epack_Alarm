


#ifndef __CCL_UL_H__
#define __CCL_UL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_ul.h
 * @brief   呼叫控制层上行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   函数列表
 *   1.   IDP_GenPttOnAckpacket          封装PTT-ON 命令回复数据包
 *   2.   IDP_GenPttOffAckpacket          封装PTT-OFF 命令回复数据包
 *   3.   IDP_GenPttOnCmd                  封装PTT-ON命令包
 *   4.   IDP_GenVoiceDataPaket           封装上行语音包
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
/**
 * @def _ENABLE_CLI
 * @brief 使能命令行接口功能开关
 */
//#define _ENABLE_CLI
/**
 * @def _USE_NET_NCOM
 * @brief 使用网口而非串口输入/输出
 */
//#define _USE_NET_NCOM
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
 * @brief   封装PTT-ON 命令回复数据包
 */
void  IDP_GenPttOnAckpacket(unsigned  char *pvCenterData, int *Len,unsigned  char PttCmdResult);

/**
 * @brief   封装PTT-OFF 命令回复数据包
 */
void  IDP_GenPttOffAckpacket(unsigned  char *pvCenterData, int *Len);

/**
 * @brief   封装PTT 命令包
 */


void IDP_GenPttCmd(unsigned char *pvDllData,unsigned char *pvCenterData,unsigned char cmd,int *Len);

/**
 * @brief   封装PTT-ON 命令包
 */
//void IDP_GenPttOnCmd(unsigned char *pvCenterData,unsigned short *Len);
/**
 * @brief   封装上行语音包
 */
void IDP_GenVoiceDataPaket(unsigned char *pvDllData,unsigned char *pvCenterData,int *Len);
/**
 * @brief   封装上行数据包
 */

void IDP_GenSmsPacket(unsigned char *pvDllData,unsigned char *pvCenterData,int *Len);
/**
* @brief    处理中心响应PTTON 命令函数
 */
void  IDP_PttonAckhandle(unsigned char *pvCenterData);
/**
* @brief    保存LC 数据
* @bug
*/

void IDP_Getlcdata(unsigned char *pvDllData);
/**
* @brief    打印PTT 命令响应函数
*/
void    IDP_CclPrintpttack(unsigned char *CenterData);

/**
 * @brief   打印PTT 命令函数
 */
void IDP_Ccl_printpttcmd(unsigned char *CenterData);


/**
* @brief    打印 dll_ccl 数据
*/
void  IDP_CclPrintDllData(unsigned char * ptDllData);

/**
* @brief    打印 上行短消息
*/

 void IDP_CclPrintsms(unsigned char* CenterData);
/**
* @brief    封装上行至中心GPS 数据包
*/
void  IDP_GenGpsData(unsigned char pvAcktype ,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *callidnet);
/**
* @brief    打印GPS数据包
*/
void    IDP_CclPrintGpsData(unsigned char *pvGpsData);
/**
* @brief    封装上行至中心临点数据包
*/
void  IDP_GenNearData(unsigned char neartyppe, unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *callidnet);

/**
* @brief    封装NAS 遥晕、遥毙、激活响应数据包
*/

void  IDP_GenNasSigAckData(unsigned char pvAcktype,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len);

/**
* @brief    封装遥晕响应数据包
*/

void  IDP_GenMsAckData(unsigned char datatype,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len,unsigned char *pvCccvnetId);
/**
* @brief    封装上行至中心场强数据包
*/
void  IDP_GenThrethHoldData( unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len);
/**
 * @brief   封装NAS状态上报数据包
 */
void  IDP_GenNasStatePack(unsigned char pvrelayflg,unsigned char *pvCenterData,int *len);
/**
* @brief    ?????????
*/

void IDP_GenAlarm_ClearData(unsigned char type,unsigned char * pvDlldata,unsigned char * pvCenterData,int *Len);

#endif //CCLUL_H





