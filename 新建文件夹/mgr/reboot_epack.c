/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    reboot_epack.c
 * @brief   复位
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   函数列表
 *   1. main       主函数
 *
 *   修改历史
 *   2017-1-16                王存刚                     建立文件
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

