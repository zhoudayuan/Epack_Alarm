#ifndef  __CCL_THREAD_H__
#define __CCL_THREAD_H__



/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file   cclthread.h
 * @brief   接收处理中心语音信令数据发送给链路层或处理链路层数据发送给中心
 * @author 牛功喜
 * @version v1.0
 * @date    2016-07-26
 */
/*
 *   函数列表
 *   1. set_ticker                           时间定时器函数
      2. timercallback                       定时器回调函数
 *   3. CCL_init                             CCL初始化函数
 *   4. thread_create                     线程创建函数
 *   5. ODP_HandleCenterVioce       处理中心语音数据线程函数
 *   6. ODP_HandleCenterSig          处理中心数据线程函数
 *   7. IDP_RecfromDll                   接收DLL数据线程函数
 *   8.  isSocketReadble                   检查套接字是否可读
 *   9. Handletimerthread             定时器处理线程
 *   10.
 *   11.
 *   12.
 *   13.
 *      ......

 *   修改历史
 *   2016-8-5           牛功喜                   建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/


/******************************************************************************
 *   宏定义
 *   *************************************************************************/

/**
 * @def  TIMER_INTERVAL
 * @brief  定时器时间间隔(ms)
 */
//#define    TIMER_INTERVAL                 30
/**
 * @def  RECV_DLL_PORT
 * @brief 本地发送DLL 端口
 */
//#define SEND_DLL_PORT                            (6017)
/**
 * @def  SEND_CCL_PORT
 * @brief  本地接收DLL端口
 */
//#define REC_DLL_PORT                                (6018)

/**
 * @def   CENTER_VOICE_PORT
 * @brief 中心业务端口
 */
//#define  CENTER_VOICE_PORT                 (6020)
/**
 * @def  CENTER_SIGNAL_PORT
 * @brief 中心信令端口
 */
//#define  CENTER_SIGNAL_PORT               (6021)




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
  * @brief   定时器按照设定时间发出信号
 */

  int  set_ticker(int n_msecs)  ;

  /**
  * @brief   定时器回调函数
  * @author 牛功喜
  */

  void  timercallback(void)  ;
  /**
  * @brief   定时器处理函数
  * @author 牛功喜
  */

  void timer_handler (int signo);
/**
 * @brief   检查套接字是否可读 无数据可读阻塞时间到则返回
 * @bug
 */

 int  isSocketReadble(int sockfd,int nsec) ;
  /**
* @brief   CCL 初始化函数
*/
int    CCL_init(void);
 /**
 * @brief   处理中心语音线程函数
 */
void  *ODP_HandleCenterVioce(void *arg);

 /**
 * @brief   处理中心信令线程
 */
void *ODP_HandleCenterSig(void *arg);
 /**
 * @brief   CCL 接收DLL数据线程
 */
void* IDP_RecFrmDllthread(void *arg);

/**
* @brief  定时器处理函数线程
*/
void*  Handletimerthread(void);
/**
* @brief   CCL 创建线程函数
*/
int  thread_create(void);

/**
* @brief CCL 处理DLL 链路类型消息
*/

void IDP_MsgWluHandle(unsigned char * pvDllData,unsigned char *CenterData);
/**
* @brief CCL 处理DLL 语音类型消息
*/

void IDP_MsgVoiceHandle(unsigned char * pvDllData,  unsigned char * CenterData);
/**
* @brief CCL 处理DLL 数据类型消息
*/

void IDP_MsgDataHandle(unsigned char * pvDllData,unsigned char * CenterData);





#endif


