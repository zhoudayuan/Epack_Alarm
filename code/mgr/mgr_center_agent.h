/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_center_agent.h
 * @brief   中心代理模块头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#ifndef __MGR_CENTER_AGENT_H__
#define __MGR_CENTER_AGENT_H__

#include "mgr.h"


int init_ipc_socket(void);
int mgra_cc_pthread_create(void);
void * pthread_cc_mgra_recv(void *arg);
void init_local_val(void);



#endif
