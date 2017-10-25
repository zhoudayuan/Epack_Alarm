#ifndef __ENCDEC_H_H_H
#define __ENCDEC_H_H_H

#include"typedef.h"


/******************************** �궨�� ****************************/

#define SIZE_LC_9			9	//LC��Ϣ����
#define SIZE_PI_10			10	//PI��Ϣ����
#define SIZE_EH_10			10	//Ehead��Ϣ����
#define SIZE_EMB_6			6	//��Ƕ��Ϣ����
#define TABLENUM_30			30  //��Կ������
#define PAYLOADSIZE			27  //���س���

/*
 * ͵����λ��
 */
#define APCO_HR_BITSTEAL_I2    59
#define APCO_HR_BITSTEAL_I3    63
#define APCO_HR_BITSTEAL_I4    67
#define APCO_HR_BITSTEAL_I5    71




/********************************* ö�ٶ��� ***************************/

/**
 * @def  ENCRYPT_TYPE
 * @brief ��������
 */
enum ENCRYPT_TYPE{
	ENCRYPT_NONE		= 0,	//�Ǽ���
	ENCRYPT_HYT_BASE    = 1,	//Hytera��������
	ENCRYPT_HYT_ENHA    = 2,	//Hytera�߼�����
	ENCRYPT_DMRA		= 3,	//DMRA����
};

/**
 * @def   CODER_TYPE
 * @brief ������������
 */
enum CODER_TYPE{
	AMBE   = 206,
	NVOC   = 207
}; 

/**
 * @def   SLOT_TYPE
 * @brief ʱ϶����
 */
enum SLOT_TYPE{
	VOICE_SLOT		 = 1,	//����ʹ�� 1 ʱ϶
	DATA_SLOT		 = 2	//����ʹ�� 2 ʱ϶
};


/**
 * @def   VOICE_FRAME_TYPE
 * @brief ����֡���
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
 * @brief ������Ч��ö��
 */
enum  VOICE_EFFECTIVE{
	INVALID = 0,			//������֡��Ч����������Ϊ��֡
	VALID = 1				//������֡��Ч�����������ӽ���
};

/**
 * @def   ENCDEC_DATA_TYPE
 * @brief ���ݼӽ���ʱ�����ͱ�ʶ
 */
enum ENCDEC_DATA_TYPE{
	TYPE_EHEAD			=0, //���ݼӽ���ʱ����������ΪEHEAD
	TYPE_DATA			=1	//���ݼӽ���ʱ����������ΪDATA
};

/******************************* ���ڽṹ *******************************/

/**
 * @def   ENCRYPT_TABLE
 * @brief ��̬��Կ���ļ��洢�ṹ
 */
 typedef struct _ST_EncryptTable{
    unsigned char alias_key[32];
    unsigned char keyID;
    unsigned char key_length;    //0x00--40λ  0x01--128λ  0x02--256λ 
    unsigned char pass_sec:1;
    unsigned char reserve:7;
    unsigned char reserve1;
    unsigned char key_value[32];
}ENCRYPT_TABLE;

/**
 * @def   KEY_TABLE
 * @brief ��̬��Կ����Կ�ṹ�����ļ��������Ľṹ
 */
typedef struct _ST_KeyTable{
    unsigned short KeyID;		//��ԿID
    unsigned short valid_flag;	//��Կ��Ч��
    unsigned short key_length;	//��Կ����  40 128 256
    UINT16 KeyValue[16];		//��Կֵ
} __attribute__((packed,aligned(1)))KEY_TABLE; 

/**
 * @def   ENCDEC_PARAM
 * @brief �ӽ��ܲ�����ģ���ڲ�ʹ��
 */
typedef struct _ST_ENCDEC_PARAM{
	UINT16 KeyValue[16];		//A֡��ʹ�ã���ʼ��������ȡNEXTIV
	UINT16 KeyBitLen;			//A֡��ʹ�ã���ʼ��������ȡNEXTIV
	UINT8  KeyID;				//A֡��ʹ�ã����PI��Ϣ
	UINT8  AlgID;				//DMRA F֡��ʹ�ã������Ƕ��Ϣ
	INT8   InitFlag;			//��ʼ���ɹ���ǣ��ɹ�1��ʧ��0
	UINT16 DMRA_GolayEnc[80];	
}ENCDEC_PARAM;

/******************************* ����ṹ *******************************/

/**
 * @def   DLL_ENCLIB_VOICE
 * @brief ���ܿ�����ҵ����νṹ
 */
typedef struct _ST_DLL_ENCLIB_VOICE{
	UINT8 LCInfo[SIZE_LC_9];	//LC��Ϣ
	UINT8 PIInfo[SIZE_PI_10];	//���PI��Ϣ     LC֡���ص�10�ֽ�����PI��Ϣ;
											//HYTERA�߼�����ʱ,A֡ʱ����9�ֽ�PI��Ϣ,��Ҫ��Ƕ����
											//DMRA����ʱ,F֡����2�ֽ� PIInfo[0]:KeyID ,PIInfo[1]:AlgID,��Ҫ��Ƕ����
	UINT8 FrameNum;				//����֡�� 	 �� VOICE_FRAME_TYPE ö��
	UINT8 ValidFlag;			//������Ч��ʶ	 �� VOICE_EFFECTIVE  ö��
	UINT8 Payload[PAYLOADSIZE];	//��������
}DLL_ENCLIB_VOICE;

/**
 * @def   DLL_ENCLIB_DATA
 * @brief ���ܿ�����ҵ����νṹ
 */
typedef struct _ST_DLL_ENCLIB_DATA{
	UINT8   EHead[SIZE_EH_10];	//Ehead��Ϣ 10�ֽ�
	UINT8   Type;				//���ν�������   �� ENCDEC_DATA_TYPEö��
	UINT16  DataLen;			//���ӽ������ݳ���
	UINT8 * Data;				//���ӽ�������
}DLL_ENCLIB_DATA;



/****************************** �ⲿ���� *****************************************/

/**
 * @def   GetLibInfo
 * @brief ��ӡ���ܿ���Ϣ���汾��ʱ϶��
 */
extern void GetLibInfo(void);

/**
 * @def      : InitEncLib
 * @brief    : ��ʼ�����ܿ⣬����ʱ���ã��Լ��ܿ��ȫ��ʱ϶���г�ʼ��
 * @param[1] : In -- �������� 
 * @param[2] : In -- ��������
 * @param[3] : In -- ��Կ��·��
 * @Out		 : �ɹ�����:0 ʧ�ܷ���:-1
 */
extern INT8 InitEncLib(UINT8 VoiceType,UINT8 EncryptType,const CHAR *keypath);

/**
 * @def      : 	EncryptFunc_Voice
 
 * @brief    : 	�������ܺ�������������ļ���ҵ��
 				�������ݴ�pInput->Payload������pInput->Payload��
 
 ��ϸ˵��: 
 Hytera�߼�����ʱ,���ô˺������:
 	pInput->FrameNum ��LC֡����pInput->PIInfo ����10���ֽ�PI��Ϣ����PI֡��Ҫʹ�õ���Ϣ������˵��˼����ĵ�
 	pInput->FrameNum ��A֡�� ��pInput->PIInfo ����9���ֽ�PI��Ϣ��MTU��ʹ��embedded_enc�������������Ƕ����
 							 ��Ҫʹ��B~E֡����Ƕ��Ϣ���ִ�����һ����֡ʹ�õļ��ܲ�����Ϊ�˳ٺ�
 	pInput->FrameNum ��B~F֡,�����������ܲ���
 	
 Hytera��������ʱ,���ô˺������:
  	pInput->FrameNum ��A~F֡��������������
  	
 DMRA����ʱ�����ô˺������:
 	pInput->FrameNum ��LC֡����pInput->PIInfo ����10���ֽ�PI��Ϣ����PI֡��Ҫʹ�õ���Ϣ������˵��˼����ĵ�
 	pInput->FrameNum ��A~F֡,A֡��ȡ��һ����֡ʹ�õ�IV,��������֡ʹ��͵���ش����IV��Ϊ�˳ٺ�
 	pInput->FrameNum ��F֡,pInput->PIInfo�������ֽ�KeyID��AlgID��Ϣ����Ҫʹ��F֡����Ƕ���ִ���ID��Ϣ

 	
 * @param[1] : In -- pInput 
 * @Out		 : �ɹ�����:0 ʧ�ܷ���:-1
 */
extern INT8 EncryptFunc_Voice(DLL_ENCLIB_VOICE* pInput);

/**
 * @def      : DecryptFunc_Voice
 
 * @brief    : �������ܺ�������������Ľ���ҵ��
 			   ˵��1:�յ�PI֡����֮��ɹ���ȫPI��Ϣ����Ҫ���ô˺���
 			   ˵��2:�������ݴ�pInput->Payload������pInput->Payload��	
 ��ϸ˵��: 
 	�������:
 		Hytera�߼�����
 			LC֡����Ҫ���ô˺���
 			�յ�PI֡�����ն˵��˼����ĵ��е�HYTERA��PI�ṹ���pInput->PIInfo��Ϣ��pInput->FrameNum����ΪPI֡�����ô˺���
 			A~F֡��������������
 		Hytera��������
 			LC֡����Ҫ���ô˺���
 			�������ܲ����յ�PI֡������Ҫ���ô˺���
 			A~F֡��������������
 		DMRA����
 			ͬHytera�߼�����
 	�ٺ����:
 		Hytera�߼�����
 			��B~E֡����Ƕ��Ϣ���֣���ȫPI��Ϣ�����ն˵��˼����ĵ���PI��ʽ���PI��Ϣ���pInput->FrameNum����ΪPI֡�����ô˺���
			��ʼ���ɹ�������ͬ�������
		Hytera��������
			PI֡��Ҫ���ô˺������������ͬ�ٺ������ͬ����A֡�������ܳ�ʼ��
		DMRA����
			��A~F֡�е��������ݵ�͵�����н������IV��Ϣ����F֡��Ƕ���ֽ��KeyID��AlgID��Ϣ��ƴ������PI��Ϣ�����ú���
			��ʼ���ɹ�������ͬ�������
			
 * @param[1] : In -- pInput 
 * @Out		 : �ɹ�����:0 ʧ�ܷ���:-1
 */
extern INT8 DecryptFunc_Voice(DLL_ENCLIB_VOICE* pInput);

/**
 * @def      : EncryptFunc_Data
 
 * @brief    : ���ݼ��ܺ�����������ݵļ���ҵ��
 			   pInput->Type ��	 TYPE_EHEAD����ʱ��������ݼ��ܳ�ʼ������EHead���ؼ�����Ϣ
 			   pInput->Type ��	 TYPE_DATA����ʱ����Ҫ������ݳ��Ⱥ��������ָ���ʼ��
 ��ϸ˵��:
 	�˺�����Ϊ�����������
 		1��ͷ����	-- TYPE_EHEAD
 			���ݼ���ҵ��ʱ����pInput->Type ����TYPE_EHEAD��pInput->EHead�ֶη���EHead��Ϣ
 			�߼�����ʱ��EHead��Ϣ��ҵ������Ч����������ʱ��ҵ���ϲ���ҪEHead��Ϣ
 		2���������� -- TYPE_DATA
 			���ݼ���ҵ��ʱ����pInput->Type 		����TYPE_DATA
 							  pInput->DataLen 	���ɴ��������ݵ��ֽ���
 							  pInput->Data		ָ������ܵ����ݻ����ַ
 			���ܺ��������pInput->Data����ԭ���ȷ���
 			
 * @param[1] : In -- pInput 
 * @Out		 : �ɹ�����:0 ʧ�ܷ���:-1
 */

extern INT8 EncryptFunc_Data(DLL_ENCLIB_DATA* pInput);

/**
 * @def      : DecryptFunc_Data
 
 * @brief    : ���ݽ��ܺ�����������ݵĽ���ҵ��
 			   pInput->Type ��	 TYPE_EHEAD����ʱ����EHead��ȡ������Ϣ��������ݽ��ܳ�ʼ����
 			   pInput->Type ��	 TYPE_DATA����ʱ����Ҫ������ݳ��Ⱥ��������ָ���ʼ��
��ϸ˵��:
 	�˺�����Ϊ�����������
 		1��ͷ����	-- TYPE_EHEAD
 			���ݽ���ҵ��ʱ����pInput->Type ����TYPE_EHEAD��pInput->EHead�ֶ����EHead��Ϣ�����ô˺�������ʼ����
 			�߼�����ʱ�����÷������յ�EHead֡���������յ�EHeadʱ���ô˺����ӿ�
 			��������ʱ�����÷��޷��յ�EHead֡���������յ�CHeadʱ���ô˺����ӿ�,���TYPE_EHEAD���ͼ���
 		2���������� -- TYPE_DATA
 			���ݽ���ҵ��ʱ����pInput->Type 		����TYPE_DATA
 							  pInput->DataLen 	���ɴ��������ݵ��ֽ���
 							  pInput->Data		ָ������ܵ����ݻ����ַ
 			���ܺ��������pInput->Data����ԭ���ȷ���
 * @param[1] : In -- pInput 
 * @Out		 : �ɹ�����:0 ʧ�ܷ���:-1

 */
extern INT8 DecryptFunc_Data(DLL_ENCLIB_DATA* pInput);



#endif
