/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_center_agent.h
 * @brief   ���Ĵ���ģ��ͷ�ļ�
 * @author  �����
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   �޸���ʷ
 *   2016-8-1                �����                     �����ļ�
****************************************************************************/


#ifndef __MGR_CENTER_AGENT_H__
#define __MGR_CENTER_AGENT_H__

#include "mgr.h"


int init_ipc_socket(void);
int mgra_cc_pthread_create(void);
void * pthread_cc_mgra_recv(void *arg);
void init_local_val(void);



#endif
