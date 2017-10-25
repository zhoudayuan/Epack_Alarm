#ifndef __ENCDEC_H_H_H
#define __ENCDEC_H_H_H

#include"typedef.h"


/******************************** 宏定义 ****************************/

#define SIZE_LC_9			9	//LC信息长度
#define SIZE_PI_10			10	//PI信息长度
#define SIZE_EH_10			10	//Ehead信息长度
#define SIZE_EMB_6			6	//内嵌信息长度
#define TABLENUM_30			30  //秘钥表数量
#define PAYLOADSIZE			27  //负载长度

/*
 * 偷比特位置
 */
#define APCO_HR_BITSTEAL_I2    59
#define APCO_HR_BITSTEAL_I3    63
#define APCO_HR_BITSTEAL_I4    67
#define APCO_HR_BITSTEAL_I5    71




/********************************* 枚举定义 ***************************/

/**
 * @def  ENCRYPT_TYPE
 * @brief 加密类型
 */
enum ENCRYPT_TYPE{
	ENCRYPT_NONE		= 0,	//非加密
	ENCRYPT_HYT_BASE    = 1,	//Hytera基本加密
	ENCRYPT_HYT_ENHA    = 2,	//Hytera高级加密
	ENCRYPT_DMRA		= 3,	//DMRA加密
};

/**
 * @def   CODER_TYPE
 * @brief 语音编码类型
 */
enum CODER_TYPE{
	AMBE   = 206,
	NVOC   = 207
}; 

/**
 * @def   SLOT_TYPE
 * @brief 时隙分配
 */
enum SLOT_TYPE{
	VOICE_SLOT		 = 1,	//语音使用 1 时隙
	DATA_SLOT		 = 2	//数据使用 2 时隙
};


/**
 * @def   VOICE_FRAME_TYPE
 * @brief 语音帧编号
 */
enum VOICE_FRAME_TYPE{
    VOICE_A_FRAME    = 1,
    VOICE_B_FRAME    = 2,
    VOICE_C_FRAME    = 3,
    VOICE_D_FRAME    = 4,
    VOICE_E_FRAME    = 5,
    VOICE_F_FRAME    = 6,
    
    VOICE_LC_FRAME   = 0,
    VOICE_TERM_FRAME = 7,
    VOICE_PI_FRAME   = 8
};

/**
 * @def   VOICE_EFFECTIVE
 * @brief 语音有效性枚举
 */
enum  VOICE_EFFECTIVE{
	INVALID = 0,			//该语音帧无效，语音净荷为补帧
	VALID = 1				//该语音帧有效，可以用作加解密
};

/**
 * @def   ENCDEC_DATA_TYPE
 * @brief 数据加解密时的类型标识
 */
enum ENCDEC_DATA_TYPE{
	TYPE_EHEAD			=0, //数据加解密时，交互类型为EHEAD
	TYPE_DATA			=1	//数据加解密时，交互类型为DATA
};

/******************************* 对内结构 *******************************/

/**
 * @def   ENCRYPT_TABLE
 * @brief 静态秘钥表，文件存储结构
 */
 typedef struct _ST_EncryptTable{
    unsigned char alias_key[32];
    unsigned char keyID;
    unsigned char key_length;    //0x00--40位  0x01--128位  0x02--256位 
    unsigned char pass_sec:1;
    unsigned char reserve:7;
    unsigned char reserve1;
    unsigned char key_value[32];
}ENCRYPT_TABLE;

/**
 * @def   KEY_TABLE
 * @brief 静态秘钥表，秘钥结构，从文件中整理后的结构
 */
typedef struct _ST_KeyTable{
    unsigned short KeyID;		//秘钥ID
    unsigned short valid_flag;	//秘钥有效性
    unsigned short key_length;	//秘钥长度  40 128 256
    UINT16 KeyValue[16];		//秘钥值
} __attribute__((packed,aligned(1)))KEY_TABLE; 

/**
 * @def   ENCDEC_PARAM
 * @brief 加解密参数，模块内部使用
 */
typedef struct _ST_ENCDEC_PARAM{
	UINT16 KeyValue[16];		//A帧中使用，初始化向量获取NEXTIV
	UINT16 KeyBitLen;			//A帧中使用，初始化向量获取NEXTIV
	UINT8  KeyID;				//A帧中使用，添加PI信息
	UINT8  AlgID;				//DMRA F帧中使用，添加内嵌信息
	INT8   InitFlag;			//初始化成功标记，成功1，失败0
	UINT16 DMRA_GolayEnc[80];	
}ENCDEC_PARAM;

/******************************* 对外结构 *******************************/

/**
 * @def   DLL_ENCLIB_VOICE
 * @brief 加密库语音业务入参结构
 */
typedef struct _ST_DLL_ENCLIB_VOICE{
	UINT8 LCInfo[SIZE_LC_9];	//LC信息
	UINT8 PIInfo[SIZE_PI_10];	//存放PI信息     LC帧返回的10字节完整PI信息;
											//HYTERA高级加密时,A帧时返回9字节PI信息,需要内嵌编码
											//DMRA加密时,F帧返回2字节 PIInfo[0]:KeyID ,PIInfo[1]:AlgID,需要内嵌编码
	UINT8 FrameNum;				//语音帧号 	 见 VOICE_FRAME_TYPE 枚举
	UINT8 ValidFlag;			//语音有效标识	 见 VOICE_EFFECTIVE  枚举
	UINT8 Payload[PAYLOADSIZE];	//语音净荷
}DLL_ENCLIB_VOICE;

/**
 * @def   DLL_ENCLIB_DATA
 * @brief 加密库数据业务入参结构
 */
typedef struct _ST_DLL_ENCLIB_DATA{
	UINT8   EHead[SIZE_EH_10];	//Ehead信息 10字节
	UINT8   Type;				//本次交互类型   见 ENCDEC_DATA_TYPE枚举
	UINT16  DataLen;			//待加解密数据长度
	UINT8 * Data;				//待加解密数据
}DLL_ENCLIB_DATA;



/****************************** 外部函数 *****************************************/

/**
 * @def   GetLibInfo
 * @brief 打印加密库信息，版本，时隙等
 */
extern void GetLibInfo(void);

/**
 * @def      : InitEncLib
 * @brief    : 初始化加密库，开机时调用，对加密库的全部时隙进行初始化
 * @param[1] : In -- 语音类型 
 * @param[2] : In -- 加密类型
 * @param[3] : In -- 秘钥表路径
 * @Out		 : 成功返回:0 失败返回:-1
 */
extern INT8 InitEncLib(UINT8 VoiceType,UINT8 EncryptType,const CHAR *keypath);

/**
 * @def      : 	EncryptFunc_Voice
 
 * @brief    : 	语音加密函数，完成语音的加密业务
 				语音数据从pInput->Payload进，从pInput->Payload出
 
 详细说明: 
 Hytera高级加密时,调用此函数情况:
 	pInput->FrameNum 是LC帧，则pInput->PIInfo 返回10自字节PI信息，是PI帧需要使用的信息，详见端到端加密文档
 	pInput->FrameNum 是A帧， 则pInput->PIInfo 返回9自字节PI信息，MTU中使用embedded_enc函数对其进行内嵌编码
 							 需要使用B~E帧的内嵌信息部分传输下一个超帧使用的加密参数，为了迟后
 	pInput->FrameNum 是B~F帧,常规语音加密操作
 	
 Hytera基本加密时,调用此函数情况:
  	pInput->FrameNum 是A~F帧，常规语音加密
  	
 DMRA加密时，调用此函数情况:
 	pInput->FrameNum 是LC帧，则pInput->PIInfo 返回10自字节PI信息，是PI帧需要使用的信息，详见端到端加密文档
 	pInput->FrameNum 是A~F帧,A帧获取下一个超帧使用的IV,在整个超帧使用偷比特传输该IV，为了迟后
 	pInput->FrameNum 是F帧,pInput->PIInfo返回两字节KeyID和AlgID信息，需要使用F帧的内嵌部分传输ID信息

 	
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */
extern INT8 EncryptFunc_Voice(DLL_ENCLIB_VOICE* pInput);

/**
 * @def      : DecryptFunc_Voice
 
 * @brief    : 语音解密函数，完成语音的解密业务
 			   说明1:收到PI帧或者之后成功收全PI信息后，需要调用此函数
 			   说明2:语音数据从pInput->Payload进，从pInput->Payload出	
 详细说明: 
 	正常情况:
 		Hytera高级加密
 			LC帧不需要调用此函数
 			收到PI帧，按照端到端加密文档中的HYTERA的PI结构填充pInput->PIInfo信息，pInput->FrameNum设置为PI帧，调用此函数
 			A~F帧，常规语音解密
 		Hytera基本加密
 			LC帧不需要调用此函数
 			基本加密不会收到PI帧，不需要调用此函数
 			A~F帧，常规语音解密
 		DMRA加密
 			同Hytera高级加密
 	迟后情况:
 		Hytera高级加密
 			从B~E帧的内嵌信息部分，收全PI信息，按照端到端加密文档中PI格式填充PI信息，琾Input->FrameNum设置为PI帧，调用此函数
			初始化成功后，流程同正常情况
		Hytera基本加密
			PI帧不要调用此函数，正常情况同迟后情况相同，在A帧中做加密初始化
		DMRA加密
			在A~F帧中的语音数据的偷比特中解出完整IV信息，在F帧内嵌部分解出KeyID和AlgID信息，拼成完整PI信息，调用函数
			初始化成功后，流程同正常情况
			
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */
extern INT8 DecryptFunc_Voice(DLL_ENCLIB_VOICE* pInput);

/**
 * @def      : EncryptFunc_Data
 
 * @brief    : 数据加密函数，完成数据的加密业务
 			   pInput->Type 是	 TYPE_EHEAD类型时，完成数据加密初始化，由EHead返回加密信息
 			   pInput->Type 是	 TYPE_DATA类型时，需要填充数据长度和完成数据指针初始化
 详细说明:
 	此函数分为两种情况调用
 		1、头类型	-- TYPE_EHEAD
 			数据加密业务时，将pInput->Type 填充成TYPE_EHEAD，pInput->EHead字段返回EHead信息
 			高级加密时，EHead信息对业务是有效，基本加密时，业务上不需要EHead信息
 		2、数据类型 -- TYPE_DATA
 			数据加密业务时，将pInput->Type 		填充成TYPE_DATA
 							  pInput->DataLen 	填充成待加密数据的字节数
 							  pInput->Data		指向待加密的数据缓存地址
 			加密后的数据由pInput->Data保持原长度返回
 			
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */

extern INT8 EncryptFunc_Data(DLL_ENCLIB_DATA* pInput);

/**
 * @def      : DecryptFunc_Data
 
 * @brief    : 数据解密函数，完成数据的解密业务
 			   pInput->Type 是	 TYPE_EHEAD类型时，由EHead获取加密信息，完成数据解密初始化，
 			   pInput->Type 是	 TYPE_DATA类型时，需要填充数据长度和完成数据指针初始化
详细说明:
 	此函数分为两种情况调用
 		1、头类型	-- TYPE_EHEAD
 			数据解密业务时，将pInput->Type 填充成TYPE_EHEAD，pInput->EHead字段填充EHead信息，调用此函数，初始化库
 			高级加密时，调用方可以收到EHead帧，可以在收到EHead时调用此函数接口
 			基本加密时，掉用方无法收到EHead帧，可以在收到CHead时调用此函数接口,填成TYPE_EHEAD类型即可
 		2、数据类型 -- TYPE_DATA
 			数据解密业务时，将pInput->Type 		填充成TYPE_DATA
 							  pInput->DataLen 	填充成待解密数据的字节数
 							  pInput->Data		指向待解密的数据缓存地址
 			解密后的数据由pInput->Data保持原长度返回
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1

 */
extern INT8 DecryptFunc_Data(DLL_ENCLIB_DATA* pInput);



#endif
