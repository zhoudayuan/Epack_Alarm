
#ifndef __CCL_FUN_H__
#define __CCL_FUN_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_fun.h
 * @brief   呼叫控制层应用函数
 * @author  牛功喜
 * @version v1.0
 * @date    2016-08-20
 */
/*
 *   函数列表
 *   1.   QUE_Alloc                 申请空闲节点
 *   2.   QUE_Free                  释放相应队列链表内所用节点数据
 *   3.   QUE_IsEmpty               查询队列链表是否为空
 *   4.   QUE_FlushData             删除相应队列链表内所用节点数据
 *   5.   QUE_AddData               添加节点到相应队列链表头
 *   6.   QUE_DelExpiredNode        删除队列中超时的节点
 *   7.   QUE_GetNum                获取队列链表内所用节点数量
 *   8.   QUE_GetData               获取相应队列链表内首个节点数据
 *   9.   QUE_GetTscn               获取相应队列链表头节点存储时间戳值
 *   10.  QUE_Init                  初始化模块并分配资源
 *   11.  delay                     阻塞延时函数
 *   12.  semaphore_p               信号量P操作
 *   13.  semaphore_v               信号量V操作
 *   14.  set_semvalue              初始化信号量值函数
 *   15.  del_semvalue              删除指定信号量
 *   16.  parsingRTPPacket          解析RTP包
 *   修改历史
 *   2016-08-20                                牛功喜       建立文件                    
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "typedef.h"
#include "ccl.h"
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
 *	 宏定义
*	*************************************************************************/

/**
 * @def DB_SIZE  
 * @brief 节点数据包最大长度定义
 */
#define DB_DATA_SIZE                       (36)
	
 
/******************************************************************************
 *	 结构体定义
 *	 *************************************************************************/

/**
 * @brief  链表节点数据结构
 */
typedef struct _QUE_UNIT_T
{
	struct _QUE_UNIT_T* ptNext; 		 ///< 后一节点
	UINT8				auData[DB_DATA_SIZE]; ///< 帧数据
	UINT32				u4STCN; 		 ///< 节点句柄
	

} QUE_UNIT_T;

/**
 * @brief  semctl()联合体参数
 */
union semun
{
	int val;                            // value for SETVAL 
	struct semid_ds *buf;               // buffer for IPC_STAT & IPC_SET 
	unsigned short *array;              // array for GETALL & SETALL 
};

 
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
 * @brief	申请空闲节点，如果没有空闲节点，返回失败
 */
BOOL QUE_Alloc(QUE_UNIT_T** ptHead);

/**
 * @brief	释放相应队列链表内所用节点数据
 */
void QUE_Free(QUE_UNIT_T* ptData);

/**
 * @brief	查询队列链表是否为空
 */
BOOL QUE_IsEmpty();
/**
* @brief   删除相应队列链表内所用节点数据
*/
void QUE_FlushData();
/**
* @brief   添加节点到相应队列链表头
*/

BOOL QUE_AddData(QUE_UNIT_T* ptData);
/**
 * @brief	删除队列中超时的节点
 */

void  QUE_DelExpiredNode();
/**
 * @brief	获取队列链表内所用节点数量
 */
unsigned short  QUE_GetNum();
/**
 * @brief	获取相应队列链表内首个节点数据
 */

BOOL QUE_GetData(QUE_UNIT_T** ptData);

/**
 * @brief	获取相应队列链表头节点存储时间戳值
 */
unsigned int QUE_GetTscn();

/**
 * @brief	初始化模块并分配资源
 */
unsigned int QUE_Init(void);
/**
 * @brief  阻塞延时函数
 */

void delay(unsigned long msec);
/**
 * @brief  定时器按照设定时间发出信号
 */

int  set_ticker(int n_msecs)	;								   //定时器 
/**
 * @brief  定时处理函数
 */
void timer_handler (int signo);

 /**
  * @brief	初始化信号量值函数
  */
 int set_semvalue(int sem_id, int value);
 
 /**
  * @brief	删除指定信号量
  */
 int  del_semvalue(int sem_id);

 /**
  * @brief	信号量P操作
  */
 int semaphore_p(int sem_id);
 
 /**
  * @brief	信号量V操作
  */
int semaphore_v(int sem_id) ;

 /**
 * @brief  RTP包解析函数
 */
  int	parsingRTPPacket(unsigned char	*data, unsigned int size) ;

  /**
* @brief	大小端转换
*/
void LitToBigSmsData(unsigned char *ConvertedData,unsigned char *RawData,unsigned short datalen);
/**
* @brief	获取系统时间 按字符串输出
*/
int GetTime( char *pvtime);

#endif //CCLUL_H



