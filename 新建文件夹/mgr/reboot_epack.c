/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    reboot_epack.c
 * @brief   ��λ
 * @author  �����
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   �����б�
 *   1. main       ������
 *
 *   �޸���ʷ
 *   2017-1-16                �����                     �����ļ�
****************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 	
#include <unistd.h> 	  //sleep
#include "gpio_interface.h"

#define GPIO_QSPI_RST   233

int fd_qspi_rst;

int main()
{
	fd_qspi_rst = gpio_init(GPIO_QSPI_RST, 1);
	gpio_write(fd_qspi_rst, 0);
	usleep(500000);
	gpio_write(fd_qspi_rst, 1);
	return 0;
}

