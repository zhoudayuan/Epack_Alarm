
#ifndef __CCL_FUN_H__
#define __CCL_FUN_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    ccl_fun.h
 * @brief   ���п��Ʋ�Ӧ�ú���
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-08-20
 */
/*
 *   �����б�
 *   1.   QUE_Alloc                 ������нڵ�
 *   2.   QUE_Free                  �ͷ���Ӧ�������������ýڵ�����
 *   3.   QUE_IsEmpty               ��ѯ���������Ƿ�Ϊ��
 *   4.   QUE_FlushData             ɾ����Ӧ�������������ýڵ�����
 *   5.   QUE_AddData               ��ӽڵ㵽��Ӧ��������ͷ
 *   6.   QUE_DelExpiredNode        ɾ�������г�ʱ�Ľڵ�
 *   7.   QUE_GetNum                ��ȡ�������������ýڵ�����
 *   8.   QUE_GetData               ��ȡ��Ӧ�����������׸��ڵ�����
 *   9.   QUE_GetTscn               ��ȡ��Ӧ��������ͷ�ڵ�洢ʱ���ֵ
 *   10.  QUE_Init                  ��ʼ��ģ�鲢������Դ
 *   11.  delay                     ������ʱ����
 *   12.  semaphore_p               �ź���P����
 *   13.  semaphore_v               �ź���V����
 *   14.  set_semvalue              ��ʼ���ź���ֵ����
 *   15.  del_semvalue              ɾ��ָ���ź���
 *   16.  parsingRTPPacket          ����RTP��
 *   �޸���ʷ
 *   2016-08-20                                ţ��ϲ       �����ļ�                    
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "typedef.h"
#include "ccl.h"
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
 *	 �궨��
*	*************************************************************************/

/**
 * @def DB_SIZE  
 * @brief �ڵ����ݰ���󳤶ȶ���
 */
#define DB_DATA_SIZE                       (36)
	
 
/******************************************************************************
 *	 �ṹ�嶨��
 *	 *************************************************************************/

/**
 * @brief  ����ڵ����ݽṹ
 */
typedef struct _QUE_UNIT_T
{
	struct _QUE_UNIT_T* ptNext; 		 ///< ��һ�ڵ�
	UINT8				auData[DB_DATA_SIZE]; ///< ֡����
	UINT32				u4STCN; 		 ///< �ڵ���
	

} QUE_UNIT_T;

/**
 * @brief  semctl()���������
 */
union semun
{
	int val;                            // value for SETVAL 
	struct semid_ds *buf;               // buffer for IPC_STAT & IPC_SET 
	unsigned short *array;              // array for GETALL & SETALL 
};

 
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
 * @brief	������нڵ㣬���û�п��нڵ㣬����ʧ��
 */
BOOL QUE_Alloc(QUE_UNIT_T** ptHead);

/**
 * @brief	�ͷ���Ӧ�������������ýڵ�����
 */
void QUE_Free(QUE_UNIT_T* ptData);

/**
 * @brief	��ѯ���������Ƿ�Ϊ��
 */
BOOL QUE_IsEmpty();
/**
* @brief   ɾ����Ӧ�������������ýڵ�����
*/
void QUE_FlushData();
/**
* @brief   ��ӽڵ㵽��Ӧ��������ͷ
*/

BOOL QUE_AddData(QUE_UNIT_T* ptData);
/**
 * @brief	ɾ�������г�ʱ�Ľڵ�
 */

void  QUE_DelExpiredNode();
/**
 * @brief	��ȡ�������������ýڵ�����
 */
unsigned short  QUE_GetNum();
/**
 * @brief	��ȡ��Ӧ�����������׸��ڵ�����
 */

BOOL QUE_GetData(QUE_UNIT_T** ptData);

/**
 * @brief	��ȡ��Ӧ��������ͷ�ڵ�洢ʱ���ֵ
 */
unsigned int QUE_GetTscn();

/**
 * @brief	��ʼ��ģ�鲢������Դ
 */
unsigned int QUE_Init(void);
/**
 * @brief  ������ʱ����
 */

void delay(unsigned long msec);
/**
 * @brief  ��ʱ�������趨ʱ�䷢���ź�
 */

int  set_ticker(int n_msecs)	;								   //��ʱ�� 
/**
 * @brief  ��ʱ������
 */
void timer_handler (int signo);

 /**
  * @brief	��ʼ���ź���ֵ����
  */
 int set_semvalue(int sem_id, int value);
 
 /**
  * @brief	ɾ��ָ���ź���
  */
 int  del_semvalue(int sem_id);

 /**
  * @brief	�ź���P����
  */
 int semaphore_p(int sem_id);
 
 /**
  * @brief	�ź���V����
  */
int semaphore_v(int sem_id) ;

 /**
 * @brief  RTP����������
 */
  int	parsingRTPPacket(unsigned char	*data, unsigned int size) ;

  /**
* @brief	��С��ת��
*/
void LitToBigSmsData(unsigned char *ConvertedData,unsigned char *RawData,unsigned short datalen);
/**
* @brief	��ȡϵͳʱ�� ���ַ������
*/
int GetTime( char *pvtime);

#endif //CCLUL_H



