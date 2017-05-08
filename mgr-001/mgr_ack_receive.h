/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file     mgr_ack_receive.h
 * @brief  ������Ӧ�����봦��ģ��ͷ�ļ�
 * @author  �����
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   �޸���ʷ
 *   2016-8-1                �����                     �����ļ�
 *   *************************************************************************/


#ifndef __MGR_ACK_RECEIVE_H__
#define __MGR_ACK_RECEIVE_H__

#include "mgr.h"

void init_local_val(void);
int mgra_ack_msg_queue_create(void);
int init_ipc_socket(void);
int mgra_ack_pthread_create(void);
void * pthread_mgrh_mgra_recv(void *arg);
void * pthread_adp_mgra_recv(void *arg);
void * pthread_ack_handle(void *arg);

#endif
