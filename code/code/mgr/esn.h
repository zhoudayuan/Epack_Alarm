#ifndef __NM_H__
#define __NM_H__

/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file    nm.h
 * @brief  ESN头文件
 * @author  王存刚
 * @version v1.0
 * @date    2015-10-28
 */
/*
 *   修改历史
 *   2017-6-20
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "mgr.h"
#include "gpio_interface.h"
#include "mgr_i2c_eeprom.h"


/******************************************************************************
 *   宏定义
 *   *************************************************************************/
#define MSG_HEADER 0x13EC      
#define ESN_LISTEN_PORT 7777    
#define MAX_DATA_SIZE 512
#define EEPROM_ESN_ADDR 0x4000
#define EEPROM_MAC_ADDR 0x4100

/******************************************************************************
 *   枚举声明
 *   *************************************************************************/

enum CMD_CODE {
	CMD_ACTIVATE_BOOT = 0x0100,               
    CMD_SWITCH_STATE = 0x0101,
    CMD_QUERY_STATE = 0x0102,
    CMD_SET_ESN = 0x010A,
    CMD_QUERY_ESN = 0x010B,
    CMD_SET_MAC = 0x010E,
    CMD_QUERY_MAC = 0x010F,
    CMD_RESET = 0x0110
};


enum ERR_CODE {
	EC_OK = 0x00,               
    EC_CMD_CODE = 0x01,
    EC_PROTOCOL_VER = 0x02,
    EC_DEV_TYPE = 0x03,
    EC_DATA = 0x04,
    EC_LEN = 0x05,
    EC_WR_EEPROM = 0x06
};


enum DEV_STATUS {
	S_BOOTLOADER = 0x00,               
    S_APP = 0x01
};


typedef struct{
    unsigned short head;
	unsigned short cmd_code;
	unsigned short len;
}__attribute__((packed,aligned(1)))ESN_MSG_HEAD;


typedef struct{
	unsigned short serial;
	unsigned short protocol_ver;
	unsigned short dev_type;
	unsigned char data[MAX_DATA_SIZE];
}__attribute__((packed,aligned(1)))APP_DATA;


typedef struct{
    unsigned short head;
	unsigned short cmd_code;
	unsigned short len;
	APP_DATA app_data;
}__attribute__((packed,aligned(1)))ESN_MSG;


typedef struct{
	unsigned char sn[32];
	unsigned short manufacturer_num;
	unsigned short dev_type_num;
	unsigned int serial_num;
	unsigned char res[24];
}__attribute__((packed,aligned(1)))ESN;


unsigned short crc16(unsigned char *buf, unsigned short length);
int init_eeprom(void);
int write_eeprom(unsigned short addr, unsigned char * data, unsigned int len);
int read_eeprom(unsigned short addr, unsigned char * data, unsigned int len);



#endif    
