/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file     mgr_ack_receive.h
 * @brief  网管响应接收与处理模块头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
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
