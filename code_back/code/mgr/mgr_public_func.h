/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_public_func.h
 * @brief   公共函数头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#ifndef __MGR_PUBLIC_FUNC_H__
#define __MGR_PUBLIC_FUNC_H__

#include "mgr.h"


int shm_cfg_create(void);
int shm_ipc_create(void);
int shm_nm_create(void);
int shm_cfg_attch(void);
int shm_ipc_attch(void);
int shm_nm_attch(void);
void shm_cfg_detach(void);
void shm_ipc_detach(void);
void shm_nm_detach(void);
void sem_cfg_p(void);
void sem_cfg_v(void);
void sem_ipc_p(void);
void sem_ipc_v(void);
void sem_nm_p(void);
void sem_nm_v(void);
void write_ini_file(void);
void save_ini_file(void);
unsigned char crc8(unsigned char *ptr, unsigned char len);


#endif
