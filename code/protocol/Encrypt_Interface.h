#include "typedef.h"
/*******************************************************************************
Copyright (C), Hytera Tech. Co., Ltd.
FileName        : Encrypt_Interface.h
Author          : luozhenghua
Version         : 1.0
Date            : 
Description     :
Function List   :
Support:
1. Voice and Data Encryption
2. Basic, Full Encryption(Hytera, DMRA)
3. Key bitlen: 40bit, 128bit, 256bit
********************************************************************************
History: Scroll to the end of this file.
********************************************************************************/
#ifndef _ENCRYPT_INTERFACE_H_
#define _ENCRYPT_INTERFACE_H_


/*--------�����ܿ�ʹ��---------------*/

//typedef char                 INT8 ; 
//typedef unsigned char        UINT8 ;
//typedef short                INT16; 
//typedef unsigned short       UINT16;
//typedef long                 INT32; 
//typedef unsigned long        UINT32;
/*
��������6�ֹ����������͵�˵��
�ڲ�ͬ�ı��뻷���±���������������: 
1. INT8, UINT8��������Ϊ8λ����8λ��16λ����;
2. INT16, UINT16���뱣֤Ϊ16λ���������������ܻ����;
3. INT32, UINT32��������Ϊ32λ����32λ��64λ���ɣ�
����޸������ϵĶ��壬��һ��Ҫ���±�����ܿ�
*/
/*--------------------------------------------------------*/



/*--------���ܿ��ṩ�Ĺ�Ӧ�ò���õĽӿ�---------*/

#define EPT_OK         0 

//�ڵ��ü��ܿ⺯��ʱ��������ݵ�slot����������֧�ֵ����slotʱ������EPT_FAIL
#define EPT_FAIL       -1  


//���ܸ��ļ�������ö�ٵ�ֵ
enum _EncryptType
{
    D_ENCRYPT_NONE     = 0  ,  //None
    D_ENCRYPT_BASIC    = 1  ,  //Basic Encrypt
    D_ENCRYPT_ENHANCED = 2     //Enhanced Encrypt, only support 40,128,256bit.  
};


enum _EptStandard
{
	DMRStandard	= 0, //Ŀǰ������DMR��AMBE�汾, ��DMRA����
	HyteraStandard
};



#ifdef __cplusplus
extern "C" {
#endif


/* �û���ʹ�ü��ܿ�ʱ�����Խ�ʱ϶����һ���߼��ŵ� */


/*
  ��ȡ��ǰ���ܿ�İ汾��
  �汾��Ϣռ19���ֽڣ����øú�����ȡ���ܿ�汾ʱ��
  Ҫ��pVerNumBuf����19�ֽڵĿռ�
  �汾��Ϣ����:"\nEptLibVerNum=V1.9"
*/
extern void QueryEptLibVersionNum(UINT8* pVerNumBuf);


/*
    ��ȡ���ܿ���ͬʱ֧�ֵ����ʱ϶��  
    ����ֵΪ��֧�ֵ����ʱ϶��
*/
extern UINT8 GetEptLibMaxSlotNum(void);


/*
   ͳһ��������ʱ϶�ļ��ܲ�����
   �����е�ʱ϶�ļ��ܲ�����һ��ʱ��ͨ���ú������ü��ܲ�����
   һ���ڿ������л��ŵ�ʱ���á�
   
   EncryptType: �ο�enum _EncryptType��
                ʹ��Hytera��������ʱ��          ��D_ENCRYPT_BASIC
                ʹ��DMRA���ܻ�Hytera�߼�����ʱ����D_ENCRYPT_ENHANCED
                
   IVByteLen:   ʹ��DMRA����ʱ��       4
                ʹ��Hytera�߼�����ʱ�� 5
                ʹ��Hytera��������ʱ�� 0
                
   PerVoiceFrameEncryptBitLen: AMBE��������       49*3=147
                               Selp������(2400)�� 48*3=144
                               Selp������(2150)�� 43*3=129
                               
   EncryptStandard: �ο�enum _EptStandard��
                    ʹ��DMRA����ʱ�� DMRStandard
                    ʹ��Hytera������ HyteraStandard
   
   KeyEncryptEnFlag: �Ƿ���Ҫ����Կ���н��ܺ���ʹ�ã��˴���0

   pRandomData[4]  : ���ڽ���Կ����������˴���NULL����0  

   ����ֵ: EPT_OK:   ���óɹ�
           EPT_FAIL: ����ʧ��
*/
extern INT8 EncryptConfigure(UINT8  EncryptType, 
                               UINT8  IVByteLen, 
                               UINT16 PerVoiceFrameEncryptBitLen, 
                               UINT8  EncryptStandard,
                               UINT8  KeyEncryptEnFlag,
                               UINT8  pRandomData[4]);


/*
   ���õ���ʱ϶�ļ��ܲ�����
   �������ʱ϶�ļ��ܲ�����һ����ͨ���ú������ø���ʱ϶�ļ��ܲ���
   һ���ڿ������л��ŵ�ʱ����
   
   Slot:        1 �� N 
   
   EncryptType: �ο�enum _EncryptType��
                ʹ��Hytera��������ʱ��          ��D_ENCRYPT_BASIC
                ʹ��DMRA���ܻ�Hytera�߼�����ʱ����D_ENCRYPT_ENHANCED
                
   IVByteLen:   ʹ��DMRA����ʱ��       4
                ʹ��Hytera�߼�����ʱ�� 5
                ʹ��Hytera��������ʱ�� 0
                
   PerVoiceFrameEncryptBitLen: AMBE��������       49*3=147
                               Selp������(2400)�� 48*3=144
                               Selp������(2150)�� 43*3=129
                               
   EncryptStandard: �ο�enum _EptStandard��
                    ʹ��DMRA����ʱ�� DMRStandard
                    ʹ��Hytera������ HyteraStandard
   
   KeyEncryptEnFlag: �Ƿ���Ҫ����Կ���н��ܺ���ʹ�ã��˴���0

   pRandomData[4]  : ���ڽ���Կ����������˴���NULL����0  

   ����ֵ: EPT_OK:   ���óɹ�
           EPT_FAIL: ����ʧ��
*/
extern INT8 EachSlotEncryptConfigure(UINT8 Slot, 
                                         UINT8  EncryptType, 
                                         UINT8  IVByteLen, 
                                         UINT16 PerVoiceFrameEncryptBitLen, 
                                         UINT8  EncryptStandard,
                                         UINT8  KeyEncryptEnFlag,
                                         UINT8  pRandomData[4]);


/*
   �ú���ֻ��Hytera�߼�����ʱ�������������PIͷ��Ƕ������PIʱʹ��
   
   pBuff: ָ������PI��Ƕ������PI����Ч����
   ByteLen:Ҫ����CheckSum����Ч�ֽ���

   ����ֵ: CheckSum   
*/
extern UINT8 HyteraEptPICheckSum(UINT8* pBuff, UINT8 ByteLen);


/* 
   ��ʹ��D_ENCRYPT_ENHANCED���ͼ���ʱ��
   �ú�����Ӧ�ò���ò�����һ��IV���� 
   
   Slot:      1 �� N
   
   Seed:      ��������������ӣ������û�ÿ�ε��øú���ʱ����ֵ����һ��
   
   pIVOutput: ��Ų�����IV����,DMRA����: 4���ֽڣ�Hytera�߼�����: 5���ֽ�
   
   һ��ֻ��׼����������/����PIͷʱ���ò�����һ��IV���� 
   pIVOutput�����ݰ��ֽ���䵽����/����PI����Ӧλ��   

   ����ֵ: EPT_OK:   ��ȡ��һ��IV�ɹ�
           EPT_FAIL: ��ȡ��һ��IVʧ��
*/
extern INT8 EncryptGetFirstIV(UINT8 Slot, UINT16 Seed, UINT8* pIVOutput);


/*
   �ú���������ʱ���ã����ڳ�ʼ����Կ������ȡ��һ��IV���ݡ�
   
   Slot:      1 �� N  
   
   pKey:      ��Կָ�룬��������16bit�����ݣ���Ч���ط��ڸ�λ
              ��40������Կ: 123456789A,
              ��pKey[0]=0x1234;
                pKey[1]=0x5678;
                pKey[2]=0x9A00;
   
   KeyBitLen: 40/128/256
   
   pIVOutput: ��Ų�������һ��IV����, DMRA����: 4���ֽڣ�Hytera�߼�����: 5���ֽ�
   ������D_ENCRYPT_ENHANCED����ʱ��pIVOutput�Ż������Ч����
   
   ������������:��������������µ���:
   1���ڵ���EncryptGetFirstIV(UINT8 Slot, UINT8* pIVOutput)֮��
      ��ʼ��������֮ǰ�����һ�θú���
   2������һ������֡���ܽ���֮����һ������֡����֮ǰ�����
      һ�θú���
   �ú���������IV���ݣ������Ҫ���ͳ�ȥ���򾭹���Ӧ�����Ƕ��
   ����Ҫ���͵ĳ�֡�����������ݻ�Ƕ��������
      
   �������ݼ���:����������µ���:
      �ڵ���EncryptGetFirstIV(UINT8 Slot, UINT8* pIVOutput)֮��
      ��ʼ��������֮ǰ�����һ�θú���

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 EncryptKeyInitandGetNextIV(UINT8          Slot, 
                                              UINT16* pKey, 
                                              UINT16  KeyBitLen, 
                                              UINT8*  pIVOutput);


/*
   ��ʹ��DMRA���ܽ�����������ʱ���ú����ű�ʹ�á�
   �ú����������ǻ�ȡ4���صļ���IV��Ϣ������Ϣ��ͨ��AMBE͵���صķ�ʽ�����ȥ

   Slot:          1 �� N
   
   VoiceFrame:    A֡~F֡ ( MS:1-6; BS:9-E)
   
   VF20msCounter: 0 �� 2 (ÿ������֡��һ��20ms��������Ϊ0���ڶ���Ϊ1��������Ϊ2)
   
   pIVInput[4]:   ����EncryptKeyInitandGetNextIV�����IV

   ����ֵ: ���ؽ�Ҫͨ��AMBE͵���ط��͵�IV���ݣ���4������Ч��

   ÿ20����AMBE����ʱ����һ�Ρ�
   
*/
extern UINT8 GetDMRA4BitsEptIVInfo(UINT8 Slot, 
                                      UINT8 VoiceFrame, 
                                      UINT8 VF20msCounter, 
                                      UINT8 pIVInput[4]);


/*
   ��ʹ��DMRA���ܽ�����������ʱ���ú����ű�ʹ�á�
   �ú����������ǽ�4���ص�IV��Ϣ�ŵ�AMBE��͵������ȥ

   StealBitNum:   ��DMRA�������� 4
   
   StealBitData:  ����GetDMRA4BitsEptIVInfo��õķ���ֵ
   
   pVocoderBuff:  ���AMBEÿ20�����������뼰FEC���72��������(AMBE�ӿ���ͨ��72word�洢��)
   
   WithFEC:       �����ʹ��AMBE��FEC����WithFEC��1������AMBE��FEC����øú���
                  ���û��ʹ��AMBE��FEC,��WithFEC��0,�����������ܺ���øú���

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��

   ÿ20����AMBE����ʱ����һ�Ρ�   
*/
extern INT8 FillStealBitsInAmbeBuff(UINT8   StealBitNum, 
                                      UINT8   StealBitData, 
                                      INT16* pVocoderBuff, 
                                      UINT8   WithFEC);


/*
   ��������
   ÿ20�������һ�Σ�����������������֮��������FEC֮ǰ����
   
   Slot:       1 �� N   
   
   VoiceFrame: A֡~F֡ ( MS:1-6; BS:9-E)
   
   pInput:     ����Ҫ���ܵ��������ݣ���������16bit�����ݣ����ڸ�λ
   pOutput:    ���μ��ܺ���������ݣ���������16bit�����ݣ����ڸ�λ
               ��:AMBEÿ20����Ҫ���ܵ�������49���أ�
               ��pInput[0]�����λ���AMBE�ĵ�1���أ�
                 pInput[0]�����λ���AMBE�ĵ�16���أ�
                 pInput[3]�����λ���AMBE�ĵ�49���أ�                 
   
   BitLen:     ÿ20��������Ҫ���ܵ�bit����:
               AMBE������       49
               Selp������(2400) 48
               Selp������(2150) 43
               
   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 VoiceEncrypt(UINT8       Slot,
                             UINT8   VoiceFrame, 
                             UINT16* pInput, 
                             UINT16* pOutput,
                             UINT16  BitLen);


/*
   ���ݼ���
   ������ÿ�ε������ֻ�ܴ���20���ֽڣ�
   ���Ҫ���ܵ����ݳ���20���ֽڣ����Էֶζ�ε��á�
   
   Slot:       1 �� N    
   
   pInput:     ����Ҫ���ܵ����ݣ���������16bit�����ݣ����ڸ�λ
   pOutput:    ���μ��ܺ�����ݣ���������16bit�����ݣ����ڸ�λ
               ��������PDU������˳����
               Byte0 = 0x12; Byte1 = 0x34,
               ��pInput[0]= 0x1234.
   
   BitLen:     ����Ҫ���ܵ����ݵ�bit������Ŀǰ����Ϊ160����
   
   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 DataEncrypt(UINT8 Slot,
                         UINT16* pInput,
                         UINT16* pOutput,
                         UINT16 BitLen);


/* 
   ������D_ENCRYPT_ENHANCED���ͽ���ʱ��
   �ڿ�ʼ��������ǰ������һ�θú�������ʼ������������ز���
   
   Slot:      1 �� N
  

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 VoiceDecryptInit(UINT8 Slot);


/* 
   ������D_ENCRYPT_ENHANCED���ͣ���������ʱ��
   ����ȡ��ȷ��IV���ݺ󣬵��øú���֪ͨ���ܿ�
   
   Slot:      1 �� N
   
   pIVInput:  ���յ���IV����   

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 VoiceDecryptIV(UINT8 Slot, UINT8* pIVInput);


/* 
   ������D_ENCRYPT_ENHANCED���ͣ����ݽ���ʱ��
   �ڻ�ȡ��ȷ��IV���ݺ󣬵��øú���֪ͨ���ܿ�
   
   Slot:      1 �� N
   
   pIVInput:  ���յ���IV���� 

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 DataDecryptIV(UINT8 Slot, UINT8* pIVInput);


/*
   �ú���������ʱ���ã����ڳ�ʼ����Կ
   
   Slot:      1 �� N   

   pKey:      ��Կָ�룬��������16bit�����ݣ���Ч���ط��ڸ�λ
              ��40������Կ: 123456789A,
              ��pKey[0]=0x1234;
                pKey[1]=0x5678;
                pKey[2]=0x9A00;
   
   KeyBitLen: 40/128/256      
      
   ������������:
      ÿ�ο�ʼ��A֡���н���ǰ���øú���
      
   �������ݽ���:
      ���յ�����PIͷ��ִ�к���DataDecryptIV֮�󣬶����ݽ���֮ǰ����һ�θú���

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 DecryptKeyInit(UINT8 Slot, UINT16* pKey, UINT16 KeyBitLen);


/*
   ��ʹ��DMRA���ܣ����м�����������ʱ���ú����ű�ʹ�á�
   �ú����������Ǵ�AMBE��͵�����л�ȡ����IV��Ϣ

   StealBitNum:    ��DMRA�������� 4
   
   pStealBitData:  ��AMBE��͵�����л�ȡ����IV��Ϣ���ڸ�ָ��
   
   pVocoderBuff:  ��Ž��յ���AMBE��72��������(AMBE�ӿ���ͨ��72word�洢��)
   
   WithFEC:       �����ʹ��AMBE��FEC����WithFEC��1������AMBE��FEC����ǰ���øú���
                  ���û��ʹ��AMBE��FEC,��WithFEC��0,������������ǰ���øú���

   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��

   ÿ20����AMBE����ʱ����һ�Ρ�   
*/
extern INT8 GetStealBitsFromAmbeBuff(UINT8   StealBitNum, 
                                           UINT8*  pStealBitData, 
                                           INT16* pVocoderBuff, 
                                           UINT8   WithFEC);

/*
   ��ʹ��DMRA���ܽ�����������ʱ���ú����ű�ʹ�á�
   �ú����������Ǵ�������֡��͵�����л�ȡ����IV

   Slot:          1 �� N
   
   VoiceFrame:    A֡~F֡ ( MS:1-6; BS:9-E)
   
   VF20msCounter: 0 �� 2 (ÿ������֡��һ��20msΪ0���ڶ���Ϊ1��������Ϊ2)
   
   StealBitData:  ����GetStealBitsFromAmbeBuff����ȡ��͵��������
   
   pIVOutput[4]:  �������IV

   ����ֵ:����ֵΪEPT_OKʱ��pIVOutput�����ݲ���Ч��
          ��ʱ�û�Ҫ���ú���VoiceDecryptIV����IV���ݸ����ܿ⡣
   
   ÿ20����AMBE����ʱ����һ�Ρ�   
*/
extern INT8 GetDMRA_DptIVFromAmbeStealBits(UINT8 Slot, 
                                                     UINT8 VoiceFrame, 
                                                     UINT8 VF20msCounter, 
                                                     UINT8 StealBitData, 
                                                     UINT8 pIVOutput[4]);


/*
   ��������
   ÿ20�������һ�Σ�����������FEC֮����������������֮ǰ
   
   Slot:       1 �� N   
   
   VoiceFrame: A֡~F֡ ( MS:1-6; BS:9-E)
   
   pInput:     ����Ҫ���ܵ��������ݣ���������16bit�����ݣ����ڸ�λ
   pOutput:    ���ν��ܺ���������ݣ���������16bit�����ݣ����ڸ�λ
               ��:AMBEÿ20����Ҫ���ܵ�������49���أ�
               ��pInput[0]�����λ���AMBE�ĵ�1���أ�
                 pInput[0]�����λ���AMBE�ĵ�16���أ�
                 pInput[3]�����λ���AMBE�ĵ�49���أ�                 
   
   BitLen:     ÿ20��������Ҫ���ܵ�bit����:
               AMBE������       49
               Selp������(2400) 48
               Selp������(2150) 43
               
   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��;

   ˵��:   ������ֵΪEPT_FAIL��Ϊ�˱��ⲥ�ų�����������������
           ������øú�����Ӧ�ò��ڷ���ֵΪEPT_FAILʱ��
           ��pOutput�滻Ϊ�������ľ���֡���������������������
           ���ų����ľ��Ǿ�����
*/
extern INT8 VoiceDecrypt(UINT8       Slot, 
                             UINT8   VoiceFrame, 
                             UINT16* pInput, 
                             UINT16* pOutput,
                             UINT16  BitLen);


/*
   ���ݽ���
   ������ÿ�ε������ֻ�ܴ���20���ֽڣ�
   ���Ҫ���ܵ����ݳ���20���ֽڣ����Էֶζ�ε��á�
   
   Slot:       1 �� N    
   
   pInput:     ����Ҫ���ܵ����ݣ���������16bit�����ݣ����ڸ�λ
   pOutput:    ���ν��ܺ�����ݣ���������16bit�����ݣ����ڸ�λ
               ��������PDU������˳����
               Byte0 = 0x12; Byte1 = 0x34,
               ��pInput[0]= 0x1234.
   
   BitLen:     ����Ҫ���ܵ����ݵ�bit������Ŀǰ����Ϊ160����
   
   ����ֵ: EPT_OK:   ����ִ�гɹ�
           EPT_FAIL: ����ִ��ʧ��
*/
extern INT8 DataDecrypt(UINT8 Slot,
                         UINT16* pInput,
                         UINT16* pOutput,
                         UINT16 BitLen);



#ifdef __cplusplus
     }
#endif


#endif

/*******************************************************************************
Revision History
********************************************************************************
  Author                   Date               Changes
 ----------------------------------------------------------------------------
  Luozhenghua                            Initial File Creation

********************************************************************************/

