





#define  WEB_STRING_MAX_LENGTH  30
#define  NM_CENTER_CONFIG_FILE   "/loadapp/ConfigFile"

/*
///////网络参数默认值///////
#define  DEF_DEV_IP   "192.168.72.31"
#define  DEF_SUBNET_MASK  "255.255.255.0"
#define  DEF_ROUTE     "192.168.72.255"
#define  DEF_CENTER_IP "192.168.4.210"
#define  DEF_CENTER_ID  4051


///////基础设置默认值///////
#define DEF_DEV_ID                          0
#define DEF_POWER                           5
#define DEF_DEV_CC                          11
#define DEF_TERMINAL_CC                     9
#define DEF_WORK_MODE                       0
#define DEF_VOL_CODE                        207
#define DEF_LOCKING_TIME                    500 //MS
#define DEF_DEV_BUSI_TIMEOUT                5000 //MS
#define DEF_GPS_TIMEOUT                     11000//MS
#define DEF_STUN_START_TIMEOUT              6000//MS
#define DEF_HALF_VARIANCE_THRESHOLD         1500
#define DEF_MIC_VOLUME                      2
#define DEF_GSM_VOLUME                      3
#define DEF_HORN_VOLUME                     3
#define DEF_DEV_CALL_TIMEOUT                60 //S
#define DEF_CLOSE_TRANSMIT_THRESHOLD        600 
#define DEF_RESUME_TRANSMIT_THRESHOLD       75 
#define DEF_TEM_ALARM_START_THRESHOLD       85 
#define DEF_TEM_ALARM_CLOSE_THRESHOLD       80 
#define DEF_ALARM_SWITCH_STATUS             0
#define DEF_PER_ALARM_SWITCH_STATUS         0
#define DEF_ALARM_COUNT                     0
#define DEF_ALARM_SHOW_SWITCH               0
#define DEF_GSM_ALARM_SHOW_SWITCH           0
#define DEF_RADIO_ALARM_SHOW_SWITCH         0
///////频率设置默认值///////
#define DEF_CURRENT_FREQ                    358500000
#define DEF_HORN_VOLUME                     3
///////黑名单设置默认值///////
#define DEF_BLACKLIST_SWITCH                0
///////白名单设置默认值///////
#define DEF_WHITELIST_SWITCH                0
///////中转台设置默认值///////
#define DEF_RADIO_IP                        "192.168.72.32"
#define DEF_BUSI_TIMESLOT1                  30007
#define DEF_BUSI_TIMESLOT2                  30008
#define DEF_CTR_TIMESLOT1                   30009
#define DEF_CTR_TIMESLOT2                   30010
#define DEF_VOL_BUSI_TIMESLOT1              30012
#define DEF_VOL_BUSI_TIMESLOT2              30014
#define DEF_CTR_PORT                        30015
#define DEF_VOL_PORT                        30016
#define DEF_RADIO_ID                        100
#define DEF_CONNEXT_GROUP_ID                1000
#define DEF_MANAGER_ID                      101
#define DEF_FILTER_SWITCH                   0
///////版本信息默认值///////
#define DEF_DEV_VERSION                     "V1.0.03.002"

*/

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define RL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define PP(x) (x<<24)|((x<<8)&0xff0000)|((x>>8)&0xff00)|(x>>24)
#define FF(a, b, c, d, x, s, ac) a = b + (RL((a + F(b,c,d) + x + ac),s))
#define GG(a, b, c, d, x, s, ac) a = b + (RL((a + G(b,c,d) + x + ac),s))
#define HH(a, b, c, d, x, s, ac) a = b + (RL((a + H(b,c,d) + x + ac),s))
#define II(a, b, c, d, x, s, ac) a = b + (RL((a + I(b,c,d) + x + ac),s))
#define BUFFERSIZE 4096
#define FILE_INFO_LEN 128
#define MD5_LEN 32
typedef  struct _WEB_NM_DATA_ITEM {
	 char web_user[WEB_STRING_MAX_LENGTH];
	 char web_code[WEB_STRING_MAX_LENGTH];
	
	 char old_user[WEB_STRING_MAX_LENGTH];
	 char new_user[WEB_STRING_MAX_LENGTH];
	 char old_code[WEB_STRING_MAX_LENGTH];
	 char new_code[WEB_STRING_MAX_LENGTH];
	 char confirm_code[WEB_STRING_MAX_LENGTH];
	///////频段标志///////
	 unsigned char freq_channel;//1:350~400 ;2:410~470
	///////网络参数///////
	 char dev_ip[WEB_STRING_MAX_LENGTH];
	 char subnet_mask[WEB_STRING_MAX_LENGTH];
	 char def_route[WEB_STRING_MAX_LENGTH];
	 char center_ip[WEB_STRING_MAX_LENGTH];
	 unsigned int  center_id;
	///////基础设置///////
	 unsigned short dev_id;
	 char  dev_ai_id[WEB_STRING_MAX_LENGTH];
	 unsigned int   dev_power;
	 unsigned int   dev_cc;
	 unsigned int   terminal_cc;
	 char   work_mode[10];
	 char   vol_code[10];
	 unsigned char   work_mode_2;
	 unsigned char   vol_code_2;
	 unsigned short locking_time;
	 unsigned short dev_busi_time_out;
	 unsigned short gps_time_out;
//    unsigned short stun_start_time_out;
     unsigned int   half_variance_threshold;
     unsigned short mic_volume;
	 unsigned short gsm_volume;
	 unsigned short horn_volume;
	 unsigned int   dev_call_timeout;
	 unsigned char  neighbor_switch;
	 unsigned short neighbor_period;
	 unsigned char  neighbor_ai_switch;
			int   close_transmit_rssi_threshold;
			int   resume_transmit_rssi_threshold;
	 unsigned char  alarm_ai_switch;
	 unsigned int   tempratue_alarm_start_threshold;
	 unsigned int   tempratue_alarm_close_threshold;
	 unsigned char  alarm_count;
	 unsigned char  alarm_show_switch;
	 unsigned char  gsm_alarm_show_switch;
	 unsigned char  radio_alarm_show_switch;
	///////组呼设置///////
	 char current_group_id[WEB_STRING_MAX_LENGTH];
	 char group1_id[WEB_STRING_MAX_LENGTH];
	 char group2_id[WEB_STRING_MAX_LENGTH];
	 char group3_id[WEB_STRING_MAX_LENGTH];
	 char group4_id[WEB_STRING_MAX_LENGTH];
	 char group5_id[WEB_STRING_MAX_LENGTH];
	 char group6_id[WEB_STRING_MAX_LENGTH];
	 char group7_id[WEB_STRING_MAX_LENGTH];
	 char group8_id[WEB_STRING_MAX_LENGTH];
	 char group9_id[WEB_STRING_MAX_LENGTH];
	 char group10_id[WEB_STRING_MAX_LENGTH];
	///////频率设置///////
	 unsigned int     current_freq;
	 char freq1[11];
	 char freq2[11];
	 char freq3[11];
	 char freq4[11];
	 char freq5[11];
	 char freq6[11];
	 char freq7[11];
	 char freq8[11];
	 char freq9[11];
	 char freq10[11];
	///////黑名单设置///////
	 unsigned char  blacklist_switch;
	 char black_list1[WEB_STRING_MAX_LENGTH+1];
	 char black_list2[WEB_STRING_MAX_LENGTH+1];
	 char black_list3[WEB_STRING_MAX_LENGTH+1];
	 char black_list4[WEB_STRING_MAX_LENGTH+1];
	 char black_list5[WEB_STRING_MAX_LENGTH+1];
	 char black_list6[WEB_STRING_MAX_LENGTH+1];
	 char black_list7[WEB_STRING_MAX_LENGTH+1];
	 char black_list8[WEB_STRING_MAX_LENGTH+1];
	 char black_list9[WEB_STRING_MAX_LENGTH+1];
	 char black_list10[WEB_STRING_MAX_LENGTH+1];
	///////白名单设置///////
	 unsigned char  whitelist_switch;
	 char white_list1[WEB_STRING_MAX_LENGTH+1];
	 char white_list2[WEB_STRING_MAX_LENGTH+1];
	 char white_list3[WEB_STRING_MAX_LENGTH+1];
	 char white_list4[WEB_STRING_MAX_LENGTH+1];
	 char white_list5[WEB_STRING_MAX_LENGTH+1];
	 char white_list6[WEB_STRING_MAX_LENGTH+1];
	 char white_list7[WEB_STRING_MAX_LENGTH+1];
	 char white_list8[WEB_STRING_MAX_LENGTH+1];
	 char white_list9[WEB_STRING_MAX_LENGTH+1];
	 char white_list10[WEB_STRING_MAX_LENGTH+1];
	///////GSM设置///////
	 char gsm_list1[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list2[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list3[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list4[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list5[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list6[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list7[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list8[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list9[WEB_STRING_MAX_LENGTH+1];
	 char gsm_list10[WEB_STRING_MAX_LENGTH+1];
	///////中转台设置///////
     char radio_ip[WEB_STRING_MAX_LENGTH];
	 unsigned short busi_timeslot1;
	 unsigned short busi_timeslot2;
	 unsigned short ctr_timeslot1;
	 unsigned short ctr_timeslot2;
	 unsigned short vol_timeslot1;
	 unsigned short vol_timeslot2;
	 unsigned short simulate_call_port;
	 unsigned short simulate_vol_port;
	 unsigned int    radio_id;
	 unsigned int    connect_group_number;
	 unsigned int    manager_id;
	 unsigned char   filter_switch;
	///////版本信息///////
     char epack_version[WEB_STRING_MAX_LENGTH];
     char mac[WEB_STRING_MAX_LENGTH];
	 unsigned short    gsm_call_interval;
	 unsigned short    gsm_reboot_time;
	 unsigned char   vol_start_threshold;
	 unsigned char   vol_delay;
	 unsigned short  vol_energy_value;
	 unsigned char   vol_packet_time;
	 unsigned char   terminal_compatible;
}__attribute__((packed,aligned(1)))WEB_NM_DATA_ITEM;



typedef struct _CENTER_DATA_STRUCT {
	 unsigned int      center_ip;
	 unsigned short    center_vol_port;
	 unsigned short    center_command_port;
	 unsigned int      center_id;
	 unsigned short    epack_vol_port;
     unsigned short    epack_command_port;
     unsigned char     center_reserve[32];

	 unsigned short    gsm_call_interval;
	 unsigned short    gsm_reboot_time;
	 unsigned short    gsm_volume;
	 unsigned char  blacklist_switch;
	 char black_list1[WEB_STRING_MAX_LENGTH];
	 char black_list2[WEB_STRING_MAX_LENGTH];
	 char black_list3[WEB_STRING_MAX_LENGTH];
	 char black_list4[WEB_STRING_MAX_LENGTH];
	 char black_list5[WEB_STRING_MAX_LENGTH];
	 char black_list6[WEB_STRING_MAX_LENGTH];
	 char black_list7[WEB_STRING_MAX_LENGTH];
	 char black_list8[WEB_STRING_MAX_LENGTH];
	 char black_list9[WEB_STRING_MAX_LENGTH];
	 char black_list10[WEB_STRING_MAX_LENGTH];
	
	 unsigned char  whitelist_switch;
	 char white_list1[WEB_STRING_MAX_LENGTH];
     char white_list2[WEB_STRING_MAX_LENGTH];
     char white_list3[WEB_STRING_MAX_LENGTH];
     char white_list4[WEB_STRING_MAX_LENGTH];
	 char white_list5[WEB_STRING_MAX_LENGTH];
     char white_list6[WEB_STRING_MAX_LENGTH];
	 char white_list7[WEB_STRING_MAX_LENGTH];
	 char white_list8[WEB_STRING_MAX_LENGTH];
	 char white_list9[WEB_STRING_MAX_LENGTH];
	 char white_list10[WEB_STRING_MAX_LENGTH];
	 char gsm_reserve[32];
	
	 unsigned int  radio_ip;
	 unsigned short busi_timeslot1;
	 unsigned short busi_timeslot2;
	 unsigned short ctr_timeslot1;
	 unsigned short ctr_timeslot2;
	 unsigned short vol_timeslot1;
	 unsigned short vol_timeslot2;
	 unsigned short simulate_call_port;
	 unsigned short simulate_vol_port;
	 unsigned int   radio_id;
	 unsigned int   alarm_group;
	 unsigned int   connect_group_number;
	 unsigned int   manager_id;
	 unsigned char  filter_switch;
	 unsigned char  radio_reserve[32];
	
	 unsigned short gps_time;
	 unsigned short neighbor_period;
	 unsigned short locking_time;
	 unsigned short  half_variance_threshold;
	 unsigned int    dev_power;
	 unsigned int    dev_freq;
	 unsigned char   neighbor_switch;
	 unsigned char   wireless_reserve[32];
	
	 unsigned char   vol_start_threshold;
	 unsigned char   vol_delay;
	 unsigned short  vol_energy_value;
	 unsigned char   vol_packet_time;
	 unsigned char   vol_code;
	 unsigned char   vol_reserve[32];
	
	 unsigned short mic_volume;
	 unsigned short horn_volume; 
	 char group1_id[WEB_STRING_MAX_LENGTH];
	 char group2_id[WEB_STRING_MAX_LENGTH];
	 char group3_id[WEB_STRING_MAX_LENGTH];
	 char group4_id[WEB_STRING_MAX_LENGTH];
	 char group5_id[WEB_STRING_MAX_LENGTH];
	 char group6_id[WEB_STRING_MAX_LENGTH];
	 char group7_id[WEB_STRING_MAX_LENGTH];
	 char group8_id[WEB_STRING_MAX_LENGTH];
	 char group9_id[WEB_STRING_MAX_LENGTH];
	 char group10_id[WEB_STRING_MAX_LENGTH];
	
	 char freq1[10];
	 char freq2[10];
	 char freq3[10];
	 char freq4[10];
     char freq5[10];
	 char freq6[10];
	 char freq7[10];
	 char freq8[10];
	 char freq9[10];
	 char freq10[10];
	 char gsm_list1[WEB_STRING_MAX_LENGTH];
	 char gsm_list2[WEB_STRING_MAX_LENGTH];
	 char gsm_list3[WEB_STRING_MAX_LENGTH];
	 char gsm_list4[WEB_STRING_MAX_LENGTH];
	 char gsm_list5[WEB_STRING_MAX_LENGTH];
	 char gsm_list6[WEB_STRING_MAX_LENGTH];
	 char gsm_list7[WEB_STRING_MAX_LENGTH];
	 char gsm_list8[WEB_STRING_MAX_LENGTH];
	 char gsm_list9[WEB_STRING_MAX_LENGTH];
	 char gsm_list10[WEB_STRING_MAX_LENGTH];
	 unsigned int  dev_call_timeout;
	 unsigned char  alarm_count;
	 unsigned char  alarm_display;
	 unsigned char  dev_alarm_icon_show_switch[8];
	 unsigned char  dev_alarm_word_show_switch[8];
	 unsigned char  mic_reserve[10];
	
	 char dev_ai_id[WEB_STRING_MAX_LENGTH];
	 char current_group_id[WEB_STRING_MAX_LENGTH];
	 unsigned int dev_id;
}__attribute__((packed,aligned(1)))CENTER_DATA_STRUCT;

///////////////操作标志--网页提示///////////////////
typedef  enum {
	REBOOT_SUCCESS_FLAG = 1,
	REBOOT_FAIL_FLAG    = 2,
	GET_SUCCESS_FLAG    = 3,
	GET_FAIL_FLAG       = 4,
	SET_SUCCESS_FLAG    = 5,
	SET_FAIL_FLAG       = 6,
	UPDATE_SUCCESS_FALG = 7,
	UPDATE_FAIL_FLAG    = 8,
	SET_CODE_SUCCESS    = 9,
	SET_CODE_FAIL       = 10,
	GET_VALIDATE_SUCCESS   = 11,
	VALIDATE_FAIL   = 12,
	SET_VALIDATE_SUCCESS   = 13,
	REBOOT_VALIDATE_SUCCESS=14,
	SET_CODE_VALIDATE_SUCCESS=15
}OPERATE_FLAG;

///////////////工作模式（正常/工程）///////////////////
typedef  enum {
	NORMAL_MODE         = 0,
	ENGINEER_MODE       = 1
}WORK_MODE;
///////////////升级文件类型///////////////////
#define     UPDATE_LOADAPP     "loadapp.bin"
#define     UPDATE_DTB         "soc_system_dtb.bin" 
#define     UPDATA_IMAGE       "ramdisk_image.bin"
#define     UPDATA_UBOOT       "bootloader.bin"
