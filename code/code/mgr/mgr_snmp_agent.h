/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    mgr_snmp_agent.h
 * @brief   snmp代理模块头文件
 * @author  王存刚
 * @version v1.0
 * @date    2016-8-1
 */
/*
 *   修改历史
 *   2016-8-1                王存刚                     建立文件
****************************************************************************/


#ifndef __MGR_SNMP_AGENT_H__
#define __MGR_SNMP_AGENT_H__


#include "mgr.h"


void init_mgr_snmp_agent(void);
Netsnmp_Node_Handler handle_id1Freq;
Netsnmp_Node_Handler handle_id2Freq;
Netsnmp_Node_Handler handle_id3Freq;
Netsnmp_Node_Handler handle_id4Freq;
Netsnmp_Node_Handler handle_id5Freq;
Netsnmp_Node_Handler handle_id6Freq;
Netsnmp_Node_Handler handle_id7Freq;
Netsnmp_Node_Handler handle_id8Freq;
Netsnmp_Node_Handler handle_id9Freq;
Netsnmp_Node_Handler handle_id10Freq;
Netsnmp_Node_Handler handle_id11Freq;
Netsnmp_Node_Handler handle_id12Freq;
Netsnmp_Node_Handler handle_id13Freq;
Netsnmp_Node_Handler handle_id14Freq;
Netsnmp_Node_Handler handle_id15Freq;
Netsnmp_Node_Handler handle_id16Freq;
Netsnmp_Node_Handler handle_id17Freq;
Netsnmp_Node_Handler handle_id18Freq;
Netsnmp_Node_Handler handle_id19Freq;
Netsnmp_Node_Handler handle_id20Freq;
Netsnmp_Node_Handler handle_id21Freq;
Netsnmp_Node_Handler handle_id22Freq;
Netsnmp_Node_Handler handle_id23Freq;
Netsnmp_Node_Handler handle_id24Freq;
Netsnmp_Node_Handler handle_id25Freq;
Netsnmp_Node_Handler handle_id26Freq;
Netsnmp_Node_Handler handle_id27Freq;
Netsnmp_Node_Handler handle_id28Freq;
Netsnmp_Node_Handler handle_id29Freq;
Netsnmp_Node_Handler handle_id30Freq;
Netsnmp_Node_Handler handle_id31Freq;
Netsnmp_Node_Handler handle_id32Freq;

Netsnmp_Node_Handler handle_id1Power;
Netsnmp_Node_Handler handle_id2Power;
Netsnmp_Node_Handler handle_id3Power;
Netsnmp_Node_Handler handle_id4Power;
Netsnmp_Node_Handler handle_id5Power;
Netsnmp_Node_Handler handle_id6Power;
Netsnmp_Node_Handler handle_id7Power;
Netsnmp_Node_Handler handle_id8Power;
Netsnmp_Node_Handler handle_id9Power;
Netsnmp_Node_Handler handle_id10Power;
Netsnmp_Node_Handler handle_id11Power;
Netsnmp_Node_Handler handle_id12Power;
Netsnmp_Node_Handler handle_id13Power;
Netsnmp_Node_Handler handle_id14Power;
Netsnmp_Node_Handler handle_id15Power;
Netsnmp_Node_Handler handle_id16Power;
Netsnmp_Node_Handler handle_id17Power;
Netsnmp_Node_Handler handle_id18Power;
Netsnmp_Node_Handler handle_id19Power;
Netsnmp_Node_Handler handle_id20Power;
Netsnmp_Node_Handler handle_id21Power;
Netsnmp_Node_Handler handle_id22Power;
Netsnmp_Node_Handler handle_id23Power;
Netsnmp_Node_Handler handle_id24Power;
Netsnmp_Node_Handler handle_id25Power;
Netsnmp_Node_Handler handle_id26Power;
Netsnmp_Node_Handler handle_id27Power;
Netsnmp_Node_Handler handle_id28Power;
Netsnmp_Node_Handler handle_id29Power;
Netsnmp_Node_Handler handle_id30Power;
Netsnmp_Node_Handler handle_id31Power;
Netsnmp_Node_Handler handle_id32Power;
/*
Netsnmp_Node_Handler handle_id1QueryAlarm;
Netsnmp_Node_Handler handle_id2QueryAlarm;
Netsnmp_Node_Handler handle_id3QueryAlarm;
Netsnmp_Node_Handler handle_id4QueryAlarm;
Netsnmp_Node_Handler handle_id5QueryAlarm;
Netsnmp_Node_Handler handle_id6QueryAlarm;
Netsnmp_Node_Handler handle_id7QueryAlarm;
Netsnmp_Node_Handler handle_id8QueryAlarm;
Netsnmp_Node_Handler handle_id9QueryAlarm;
Netsnmp_Node_Handler handle_id10QueryAlarm;
Netsnmp_Node_Handler handle_id11QueryAlarm;
Netsnmp_Node_Handler handle_id12QueryAlarm;
Netsnmp_Node_Handler handle_id13QueryAlarm;
Netsnmp_Node_Handler handle_id14QueryAlarm;
Netsnmp_Node_Handler handle_id15QueryAlarm;
Netsnmp_Node_Handler handle_id16QueryAlarm;
Netsnmp_Node_Handler handle_id17QueryAlarm;
Netsnmp_Node_Handler handle_id18QueryAlarm;
Netsnmp_Node_Handler handle_id19QueryAlarm;
Netsnmp_Node_Handler handle_id20QueryAlarm;
Netsnmp_Node_Handler handle_id21QueryAlarm;
Netsnmp_Node_Handler handle_id22QueryAlarm;
Netsnmp_Node_Handler handle_id23QueryAlarm;
Netsnmp_Node_Handler handle_id24QueryAlarm;
Netsnmp_Node_Handler handle_id25QueryAlarm;
Netsnmp_Node_Handler handle_id26QueryAlarm;
Netsnmp_Node_Handler handle_id27QueryAlarm;
Netsnmp_Node_Handler handle_id28QueryAlarm;
Netsnmp_Node_Handler handle_id29QueryAlarm;
Netsnmp_Node_Handler handle_id30QueryAlarm;
Netsnmp_Node_Handler handle_id31QueryAlarm;
Netsnmp_Node_Handler handle_id32QueryAlarm;
*/


Netsnmp_Node_Handler handle_localFreq;
Netsnmp_Node_Handler handle_localPower;
Netsnmp_Node_Handler handle_emissiveVCOFreq;
Netsnmp_Node_Handler handle_receivingVCOFreq;
Netsnmp_Node_Handler handle_secondLocalOscillatorFreq;
Netsnmp_Node_Handler handle_powerAmplifierSwitch;
Netsnmp_Node_Handler handle_fpgaParam;
Netsnmp_Node_Handler handle_power;
Netsnmp_Node_Handler handle_debugEmissiveFreq;
Netsnmp_Node_Handler handle_afc;
Netsnmp_Node_Handler handle_saveAFC;
Netsnmp_Node_Handler handle_iqs;
Netsnmp_Node_Handler handle_saveIQS;
Netsnmp_Node_Handler handle_savePower;
Netsnmp_Node_Handler handle_offset;
Netsnmp_Node_Handler handle_saveOffset;
Netsnmp_Node_Handler handle_calibrateOffset;
Netsnmp_Node_Handler handle_phase;
Netsnmp_Node_Handler handle_savePhase;
Netsnmp_Node_Handler handle_calibratePhase;
Netsnmp_Node_Handler handle_errorRate;
Netsnmp_Node_Handler handle_printMark;
Netsnmp_Node_Handler handle_connectCCFlag;
Netsnmp_Node_Handler handle_startDebug;
Netsnmp_Node_Handler handle_startErrorRate;
Netsnmp_Node_Handler handle_selectSignalSource;
Netsnmp_Node_Handler handle_localOpenCloseLoop;
Netsnmp_Node_Handler handle_debugOpenCloseLoop;
Netsnmp_Node_Handler handle_debugPower;
Netsnmp_Node_Handler handle_slotControl;
Netsnmp_Node_Handler handle_vgs;
Netsnmp_Node_Handler handle_saveEeprom;

Netsnmp_Node_Handler handle_lockingTime;
Netsnmp_Node_Handler handle_halfVarianceThreshold;
Netsnmp_Node_Handler handle_devId;
Netsnmp_Node_Handler handle_localCc;
Netsnmp_Node_Handler handle_version;
Netsnmp_Node_Handler handle_fpgaVer;
Netsnmp_Node_Handler handle_terminalCc;
Netsnmp_Node_Handler handle_protocolMode;
Netsnmp_Node_Handler handle_airSubNetid;
Netsnmp_Node_Handler handle_localIp;
Netsnmp_Node_Handler handle_localMac;
Netsnmp_Node_Handler handle_workTimeout;
Netsnmp_Node_Handler handle_gpsTimeout;
Netsnmp_Node_Handler handle_stunTimeout;
Netsnmp_Node_Handler handle_startTimeout;
Netsnmp_Node_Handler handle_devCallTimeout;
Netsnmp_Node_Handler handle_scanMode;
Netsnmp_Node_Handler handle_errorRateFreq;
Netsnmp_Node_Handler handle_saveIqData;
Netsnmp_Node_Handler handle_alarmSwitchStatus;
Netsnmp_Node_Handler handle_closeTranThreshold;
Netsnmp_Node_Handler handle_tempAlarmStartThreshold;
Netsnmp_Node_Handler handle_tempAlarmCloseThreshold;
Netsnmp_Node_Handler handle_resumeTranThreshold;
Netsnmp_Node_Handler handle_motoModeSwitch;
Netsnmp_Node_Handler handle_thresholdOptSwitch;
Netsnmp_Node_Handler handle_localNeighborSwitch;
Netsnmp_Node_Handler handle_localNeighborPeriod;
Netsnmp_Node_Handler handle_NeighborReportAi;
Netsnmp_Node_Handler handle_setFreqChannel;








Netsnmp_Node_Handler handle_set981;
Netsnmp_Node_Handler handle_query981;
Netsnmp_Node_Handler handle_set998;
Netsnmp_Node_Handler handle_query998;








Netsnmp_Node_Handler handle_localCombinedData;
Netsnmp_Node_Handler handle_updateData;
Netsnmp_Node_Handler handle_reboot;




Netsnmp_Node_Handler handle_id1StartNeighbor;
Netsnmp_Node_Handler handle_id2StartNeighbor;
Netsnmp_Node_Handler handle_id3StartNeighbor;
Netsnmp_Node_Handler handle_id4StartNeighbor;
Netsnmp_Node_Handler handle_id5StartNeighbor;
Netsnmp_Node_Handler handle_id6StartNeighbor;
Netsnmp_Node_Handler handle_id7StartNeighbor;
Netsnmp_Node_Handler handle_id8StartNeighbor;
Netsnmp_Node_Handler handle_id9StartNeighbor;
Netsnmp_Node_Handler handle_id10StartNeighbor;
Netsnmp_Node_Handler handle_id11StartNeighbor;
Netsnmp_Node_Handler handle_id12StartNeighbor;
Netsnmp_Node_Handler handle_id13StartNeighbor;
Netsnmp_Node_Handler handle_id14StartNeighbor;
Netsnmp_Node_Handler handle_id15StartNeighbor;
Netsnmp_Node_Handler handle_id16StartNeighbor;
Netsnmp_Node_Handler handle_id17StartNeighbor;
Netsnmp_Node_Handler handle_id18StartNeighbor;
Netsnmp_Node_Handler handle_id19StartNeighbor;
Netsnmp_Node_Handler handle_id20StartNeighbor;
Netsnmp_Node_Handler handle_id21StartNeighbor;
Netsnmp_Node_Handler handle_id22StartNeighbor;
Netsnmp_Node_Handler handle_id23StartNeighbor;
Netsnmp_Node_Handler handle_id24StartNeighbor;
Netsnmp_Node_Handler handle_id25StartNeighbor;
Netsnmp_Node_Handler handle_id26StartNeighbor;
Netsnmp_Node_Handler handle_id27StartNeighbor;
Netsnmp_Node_Handler handle_id28StartNeighbor;
Netsnmp_Node_Handler handle_id29StartNeighbor;
Netsnmp_Node_Handler handle_id30StartNeighbor;
Netsnmp_Node_Handler handle_id31StartNeighbor;
Netsnmp_Node_Handler handle_id32StartNeighbor;
Netsnmp_Node_Handler handle_id1NeighborPeriod;
Netsnmp_Node_Handler handle_id2NeighborPeriod;
Netsnmp_Node_Handler handle_id3NeighborPeriod;
Netsnmp_Node_Handler handle_id4NeighborPeriod;
Netsnmp_Node_Handler handle_id5NeighborPeriod;
Netsnmp_Node_Handler handle_id6NeighborPeriod;
Netsnmp_Node_Handler handle_id7NeighborPeriod;
Netsnmp_Node_Handler handle_id8NeighborPeriod;
Netsnmp_Node_Handler handle_id9NeighborPeriod;
Netsnmp_Node_Handler handle_id10NeighborPeriod;
Netsnmp_Node_Handler handle_id11NeighborPeriod;
Netsnmp_Node_Handler handle_id12NeighborPeriod;
Netsnmp_Node_Handler handle_id13NeighborPeriod;
Netsnmp_Node_Handler handle_id14NeighborPeriod;
Netsnmp_Node_Handler handle_id15NeighborPeriod;
Netsnmp_Node_Handler handle_id16NeighborPeriod;
Netsnmp_Node_Handler handle_id17NeighborPeriod;
Netsnmp_Node_Handler handle_id18NeighborPeriod;
Netsnmp_Node_Handler handle_id19NeighborPeriod;
Netsnmp_Node_Handler handle_id20NeighborPeriod;
Netsnmp_Node_Handler handle_id21NeighborPeriod;
Netsnmp_Node_Handler handle_id22NeighborPeriod;
Netsnmp_Node_Handler handle_id23NeighborPeriod;
Netsnmp_Node_Handler handle_id24NeighborPeriod;
Netsnmp_Node_Handler handle_id25NeighborPeriod;
Netsnmp_Node_Handler handle_id26NeighborPeriod;
Netsnmp_Node_Handler handle_id27NeighborPeriod;
Netsnmp_Node_Handler handle_id28NeighborPeriod;
Netsnmp_Node_Handler handle_id29NeighborPeriod;
Netsnmp_Node_Handler handle_id30NeighborPeriod;
Netsnmp_Node_Handler handle_id31NeighborPeriod;
Netsnmp_Node_Handler handle_id32NeighborPeriod;

Netsnmp_Node_Handler handle_id1CombinedDataPacket;
Netsnmp_Node_Handler handle_id2CombinedDataPacket;
Netsnmp_Node_Handler handle_id3CombinedDataPacket;
Netsnmp_Node_Handler handle_id4CombinedDataPacket;
Netsnmp_Node_Handler handle_id5CombinedDataPacket;
Netsnmp_Node_Handler handle_id6CombinedDataPacket;
Netsnmp_Node_Handler handle_id7CombinedDataPacket;
Netsnmp_Node_Handler handle_id8CombinedDataPacket;
Netsnmp_Node_Handler handle_id9CombinedDataPacket;
Netsnmp_Node_Handler handle_id10CombinedDataPacket;
Netsnmp_Node_Handler handle_id11CombinedDataPacket;
Netsnmp_Node_Handler handle_id12CombinedDataPacket;
Netsnmp_Node_Handler handle_id13CombinedDataPacket;
Netsnmp_Node_Handler handle_id14CombinedDataPacket;
Netsnmp_Node_Handler handle_id15CombinedDataPacket;
Netsnmp_Node_Handler handle_id16CombinedDataPacket;
Netsnmp_Node_Handler handle_id17CombinedDataPacket;
Netsnmp_Node_Handler handle_id18CombinedDataPacket;
Netsnmp_Node_Handler handle_id19CombinedDataPacket;
Netsnmp_Node_Handler handle_id20CombinedDataPacket;
Netsnmp_Node_Handler handle_id21CombinedDataPacket;
Netsnmp_Node_Handler handle_id22CombinedDataPacket;
Netsnmp_Node_Handler handle_id23CombinedDataPacket;
Netsnmp_Node_Handler handle_id24CombinedDataPacket;
Netsnmp_Node_Handler handle_id25CombinedDataPacket;
Netsnmp_Node_Handler handle_id26CombinedDataPacket;
Netsnmp_Node_Handler handle_id27CombinedDataPacket;
Netsnmp_Node_Handler handle_id28CombinedDataPacket;
Netsnmp_Node_Handler handle_id29CombinedDataPacket;
Netsnmp_Node_Handler handle_id30CombinedDataPacket;
Netsnmp_Node_Handler handle_id31CombinedDataPacket;
Netsnmp_Node_Handler handle_id32CombinedDataPacket;



	



/*控制中心相关*/
Netsnmp_Node_Handler  handle_centerModule;
Netsnmp_Node_Handler  handle_gsmModule1;
Netsnmp_Node_Handler  handle_gsmModule2;
Netsnmp_Node_Handler  handle_radioModule;
Netsnmp_Node_Handler  handle_dspModule;
Netsnmp_Node_Handler  handle_microphoneModule1;
Netsnmp_Node_Handler  handle_microphoneModule2;
Netsnmp_Node_Handler  handle_controlModule;




/*
Netsnmp_Node_Handler handle_nodeINTrw;
Netsnmp_Node_Handler handle_nodeUINTrw;
Netsnmp_Node_Handler handle_nodeSTRrw;
Netsnmp_Node_Handler handle_nodeIPrw;
Netsnmp_Node_Handler handle_nodeINTr;
Netsnmp_Node_Handler handle_nodeUINTr;
Netsnmp_Node_Handler handle_nodeSTRr;
Netsnmp_Node_Handler handle_nodeIPr;
Netsnmp_Node_Handler handle_nodeOPAQUErw;
Netsnmp_Node_Handler handle_nodeOPAQUEr;
*/

int shm_cfg_attch(void);
int shm_ipc_attch(void);
int shm_nm_attch(void);
void init_local_val(void);
int init_ipc_socket(void);
int is_local_dev(unsigned int id);
void sem_nm_p(void);
void sem_nm_v(void);
void sem_ipc_p(void);
void sem_ipc_v(void);
void sem_cfg_p(void);
void sem_cfg_v(void);
unsigned char crc8(unsigned char *ptr, unsigned char len);
void send_remote_nm_get_cmd(unsigned short cmd_code, unsigned char dst_id);
void send_remote_nm_set_cmd(unsigned short cmd_code, unsigned char dst_id, unsigned char * data);
void send_local_nm_get_cmd(unsigned short cmd_code);
void send_local_nm_set_cmd(unsigned short cmd_code, unsigned char * payload);
/*控制中心相关*/
void send_cc_get_cmd(unsigned short cmd_code);
void send_cc_set_cmd(unsigned short cmd_code, unsigned char * payload);
int bin_to_bz2(char * bin_name, char * bz2_name);

int CalcFileMD5(char *filename, char *md5_sum);
int  confirm_mount_point();
int test_code(int status);

/**
 * @enum  UPDATE_FILE_TYPE
 * @brief 升级文件类型
 */
typedef enum {
	UPDATE_LOADAPP    = 0x01,  ///应用程序
	UPDATE_DTB	      = 0x02,  ///设备树
	UPDATE_ZIMAGE     = 0x03,  ///内核镜像
	UPDATE_IMAGE      = 0x04,  ///文件系统
	UPDATE_RBF        = 0x05,   ///FPGA
	UPDATE_UBOOT      = 0x06,   ///uboot
	FPGA_PARAM        = 0x07    ///fpga_param
}UPDATE_FILE_TYPE;  









#endif 
