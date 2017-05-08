#ifndef  __CCL_THREAD_H__
#define __CCL_THREAD_H__



/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file   cclthread.h
 * @brief   ���մ������������������ݷ��͸���·�������·�����ݷ��͸�����
 * @author ţ��ϲ
 * @version v1.0
 * @date    2016-07-26
 */
/*
 *   �����б�
 *   1. set_ticker                           ʱ�䶨ʱ������
      2. timercallback                       ��ʱ���ص�����
 *   3. CCL_init                             CCL��ʼ������
 *   4. thread_create                     �̴߳�������
 *   5. ODP_HandleCenterVioce       �����������������̺߳���
 *   6. ODP_HandleCenterSig          �������������̺߳���
 *   7. IDP_RecfromDll                   ����DLL�����̺߳���
 *   8.  isSocketReadble                   ����׽����Ƿ�ɶ�
 *   9. Handletimerthread             ��ʱ�������߳�
 *   10.
 *   11.
 *   12.
 *   13.
 *      ......

 *   �޸���ʷ
 *   2016-8-5           ţ��ϲ                   �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/


/******************************************************************************
 *   �궨��
 *   *************************************************************************/

/**
 * @def  TIMER_INTERVAL
 * @brief  ��ʱ��ʱ����(ms)
 */
//#define    TIMER_INTERVAL                 30
/**
 * @def  RECV_DLL_PORT
 * @brief ���ط���DLL �˿�
 */
//#define SEND_DLL_PORT                            (6017)
/**
 * @def  SEND_CCL_PORT
 * @brief  ���ؽ���DLL�˿�
 */
//#define REC_DLL_PORT                                (6018)

/**
 * @def   CENTER_VOICE_PORT
 * @brief ����ҵ��˿�
 */
//#define  CENTER_VOICE_PORT                 (6020)
/**
 * @def  CENTER_SIGNAL_PORT
 * @brief ��������˿�
 */
//#define  CENTER_SIGNAL_PORT               (6021)




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
  * @brief   ��ʱ�������趨ʱ�䷢���ź�
 */

  int  set_ticker(int n_msecs)  ;

  /**
  * @brief   ��ʱ���ص�����
  * @author ţ��ϲ
  */

  void  timercallback(void)  ;
  /**
  * @brief   ��ʱ��������
  * @author ţ��ϲ
  */

  void timer_handler (int signo);
/**
 * @brief   ����׽����Ƿ�ɶ� �����ݿɶ�����ʱ�䵽�򷵻�
 * @bug
 */

 int  isSocketReadble(int sockfd,int nsec) ;
  /**
* @brief   CCL ��ʼ������
*/
int    CCL_init(void);
 /**
 * @brief   �������������̺߳���
 */
void  *ODP_HandleCenterVioce(void *arg);

 /**
 * @brief   �������������߳�
 */
void *ODP_HandleCenterSig(void *arg);
 /**
 * @brief   CCL ����DLL�����߳�
 */
void* IDP_RecFrmDllthread(void *arg);

/**
* @brief  ��ʱ���������߳�
*/
void*  Handletimerthread(void);
/**
* @brief   CCL �����̺߳���
*/
int  thread_create(void);

/**
* @brief CCL ����DLL ��·������Ϣ
*/

void IDP_MsgWluHandle(unsigned char * pvDllData,unsigned char *CenterData);
/**
* @brief CCL ����DLL ����������Ϣ
*/

void IDP_MsgVoiceHandle(unsigned char * pvDllData,  unsigned char * CenterData);
/**
* @brief CCL ����DLL ����������Ϣ
*/

void IDP_MsgDataHandle(unsigned char * pvDllData,unsigned char * CenterData);





#endif


