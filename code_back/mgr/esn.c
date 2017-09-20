/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file    esn.c
 * @brief  ESN进程
 * @author  王存刚
 * @version v1.0
 * @date    2017-6-21
 */
/*
 *   函数列表
 *   1. main                            主函数
 *
 *   修改历史
 *   2017-6-21
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <pthread.h>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>

#include "esn.h"




/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
struct sockaddr_in local_addr;
/**
 * @var nm_addr
 * @brief 命令发送端 ip地址
 */
struct sockaddr_in nm_addr;
/**
 * @var sin_size
 * @brief 命令长度
 */
socklen_t sin_size;
/**
 * @var nm_sock
 * @brief 命令socket
 */
int nm_sock;

int eeprom_fd;



/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
/**
 * @brief   进程主函数
 * @return  int   执行结果
 * @author  王存刚
 * @since   trunk.00001
 * @bug
 */
int main(void)
{
	/* 进程间通信 */
	int rd_cnt;
	int ret;	    
    fd_set fdsr;
    struct timeval tv;    
    ESN_MSG recv_msg;
	ESN_MSG send_msg;
	unsigned short send_crc;
	unsigned short recv_crc;
	unsigned short tmp_crc;
	unsigned int payload_len = 0;
	int msg_len;
	int send_len;
	unsigned char esn_buf[64];
	unsigned char mac_buf[6];
	int i;

	nm_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == nm_sock) 
	{
		printf("Socket error : %s\n\a", strerror(errno));
		return -1;
	}

	sin_size = sizeof(struct sockaddr_in);

	bzero(&local_addr, sin_size);
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(ESN_LISTEN_PORT);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(nm_sock, (struct sockaddr *)(&local_addr), sin_size)) 
	{
		printf("esn udp bind error : %s\n\a", strerror(errno));
		return -1;
	}	
   
	while(1) 
	{
		FD_ZERO(&fdsr);
		FD_SET(nm_sock, &fdsr);
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		
		ret = select(nm_sock + 1, &fdsr, NULL, NULL, &tv);
		if (ret < 0) 
		{
			printf("Select error!\n");
			continue;
		} 
		else if (0 == ret) 
		{
			continue;		
		}
		
		if (FD_ISSET(nm_sock, &fdsr)) 
		{
			memset((unsigned char *)&recv_msg, 0, sizeof(recv_msg));
			memset((unsigned char *)&send_msg, 0, sizeof(send_msg));
			rd_cnt = recvfrom(nm_sock, &recv_msg, sizeof(ESN_MSG), 0, (struct sockaddr *)(&nm_addr), &sin_size);
			if (rd_cnt < 0) 
			{	
				printf("recvfrom err\n");
				continue;
			}			
			
			if (MSG_HEADER != recv_msg.head) 
			{
				printf("head err\n");
				continue;
			}		

			msg_len = sizeof(ESN_MSG_HEAD) + recv_msg.len + 2;
			if (msg_len != rd_cnt) 
			{
				printf("len err\n");
				continue;
			}
						
			switch (recv_msg.cmd_code) 
			{
				case CMD_ACTIVATE_BOOT:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						send_msg.cmd_code = recv_msg.cmd_code;
						send_msg.app_data.data[0] = EC_OK;
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_SWITCH_STATE:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						send_msg.cmd_code = recv_msg.cmd_code;
						send_msg.app_data.data[0] = EC_OK;
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_QUERY_STATE:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						send_msg.cmd_code = recv_msg.cmd_code;
						send_msg.app_data.data[0] = S_BOOTLOADER;
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_SET_ESN:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						memcpy(esn_buf, recv_msg.app_data.data, sizeof(esn_buf));
						if (0 == write_eeprom(EEPROM_ESN_ADDR, esn_buf, sizeof(esn_buf)))
						{
							send_msg.cmd_code = recv_msg.cmd_code;
							send_msg.app_data.data[0] = EC_OK;
						}
						else
						{
							send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
							send_msg.app_data.data[0] = EC_WR_EEPROM;
						}
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_QUERY_ESN:
				{
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						if (0 == read_eeprom(EEPROM_ESN_ADDR, esn_buf, sizeof(esn_buf)))
						{
							payload_len = sizeof(esn_buf);
							send_msg.cmd_code = recv_msg.cmd_code;
							memcpy(send_msg.app_data.data, esn_buf, sizeof(esn_buf));
						}
						else
						{
							payload_len = 1;
							send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
							send_msg.app_data.data[0] = EC_WR_EEPROM;
						}
					}
					else
					{
						payload_len = 1;
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_SET_MAC:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						memcpy(mac_buf, recv_msg.app_data.data, sizeof(mac_buf));
						if (0 == write_eeprom(EEPROM_MAC_ADDR, mac_buf, sizeof(mac_buf)))
						{
							send_msg.cmd_code = recv_msg.cmd_code;
							send_msg.app_data.data[0] = EC_OK;
						}
						else
						{
							send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
							send_msg.app_data.data[0] = EC_WR_EEPROM;
						}
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_QUERY_MAC:
				{
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						if (0 == read_eeprom(EEPROM_MAC_ADDR, mac_buf, sizeof(mac_buf)))
						{
							payload_len = sizeof(mac_buf);
							send_msg.cmd_code = recv_msg.cmd_code;
							memcpy(send_msg.app_data.data, mac_buf, sizeof(mac_buf));
						}
						else
						{
							payload_len = 1;
							send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
							send_msg.app_data.data[0] = EC_WR_EEPROM;
						}
					}
					else
					{
						payload_len = 1;
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
					}
					
					send_msg.head = recv_msg.head;
					send_msg.len = 6 + payload_len;
					send_msg.app_data.serial = recv_msg.app_data.serial;
					send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
					send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
					send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
					memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
					send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
					sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					break;
				}
				case CMD_RESET:
				{
					payload_len = 1;
					tmp_crc = crc16((unsigned char *)&recv_msg.cmd_code, sizeof(ESN_MSG_HEAD) + recv_msg.len - 2);
					memcpy((unsigned char *)&recv_crc, (unsigned char *)&recv_msg + sizeof(ESN_MSG_HEAD) + recv_msg.len, 2);
					if (tmp_crc == recv_crc)
					{
						send_msg.cmd_code = recv_msg.cmd_code;
						send_msg.app_data.data[0] = EC_OK;
						send_msg.head = recv_msg.head;
						send_msg.len = 6 + payload_len;
						send_msg.app_data.serial = recv_msg.app_data.serial;
						send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
						send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
						send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
						memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
						send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
						sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);

						sleep(2);
						printf("Start reboot\n");
						chdir("/loadapp");
                        system("./reboot_epack");
					}
					else
					{
						send_msg.cmd_code = recv_msg.cmd_code | 0x0080;
						send_msg.app_data.data[0] = EC_DATA;
						send_msg.head = recv_msg.head;
						send_msg.len = 6 + payload_len;
						send_msg.app_data.serial = recv_msg.app_data.serial;
						send_msg.app_data.protocol_ver = recv_msg.app_data.protocol_ver;
						send_msg.app_data.dev_type = recv_msg.app_data.dev_type;
						send_crc = crc16((unsigned char *)&send_msg.cmd_code, sizeof(ESN_MSG_HEAD) + send_msg.len - 2);
						memcpy(&send_msg.app_data.data[payload_len], (unsigned char *)&send_crc, 2);
						send_len = sizeof(ESN_MSG_HEAD) + send_msg.len + 2;
						sendto(nm_sock, &send_msg, send_len, 0, (struct sockaddr *)(&nm_addr), sin_size);
					}
					break;
				}
				default:
				{
					break;
				}
			}			 
		}
	}

	close(nm_sock);
	return 0;
}


unsigned short crc16(unsigned char *buf, unsigned short length)
{
   unsigned short i,j,c;
   unsigned short crc=0xFFFF;
   for (i=0; i<length; i++)
    {
        c = *(buf+i) & 0x00FF;
        crc^=c;
        for (j=0; j<8; j++)
        {
             if (crc & 0x0001)
             {
                crc >>= 1;
                crc ^= 0xA001;
             }
             else
             { 
                crc >>= 1;
             }
        }
   }
   return(crc);
}


int init_eeprom(void)
{
 	eeprom_fd = open("/dev/i2c-0",O_RDWR);
	if (eeprom_fd < 0)
	{
		printf("Open axiram-nm error!\n");
        return -1;
	}
	return 0;
}


int write_eeprom(unsigned short addr, unsigned char * data, unsigned int len)
{
	if (init_eeprom())
	{
		printf("init_eeprom err\n");
		return -1;
	}
	if (eeprom_write(eeprom_fd, addr, data, len) == -1)
	{
		printf("eeprom_write err\n");
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	printf("eeprom_write ok\n");
	return 0;
}


int read_eeprom(unsigned short addr, unsigned char * data, unsigned int len)
{
	if (init_eeprom())
	{
		printf("init_eeprom err\n");
		return -1;
	}
	if (eeprom_read(eeprom_fd, addr, data, len) == -1)
	{
		printf("eeprom_read err\n");
		close(eeprom_fd);
		return -1;
	}
	close(eeprom_fd);
	printf("eeprom_read ok\n");
	return 0;
}



