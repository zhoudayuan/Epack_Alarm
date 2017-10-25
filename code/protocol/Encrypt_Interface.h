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


/*--------供加密库使用---------------*/

//typedef char                 INT8 ; 
//typedef unsigned char        UINT8 ;
//typedef short                INT16; 
//typedef unsigned short       UINT16;
//typedef long                 INT32; 
//typedef unsigned long        UINT32;
/*
关于以上6种公共数据类型的说明
在不同的编译环境下必须满足以下需求: 
1. INT8, UINT8必须至少为8位，即8位，16位均可;
2. INT16, UINT16必须保证为16位，否则运算结果可能会出错;
3. INT32, UINT32必须至少为32位，即32位，64位均可，
如果修改了以上的定义，则一定要重新编译加密库
*/
/*--------------------------------------------------------*/



/*--------加密库提供的供应用层调用的接口---------*/

#define EPT_OK         0 

//在调用加密库函数时，如果传递的slot超过本库能支持的最大slot时，返回EPT_FAIL
#define EPT_FAIL       -1  


//不能更改加密类型枚举的值
enum _EncryptType
{
    D_ENCRYPT_NONE     = 0  ,  //None
    D_ENCRYPT_BASIC    = 1  ,  //Basic Encrypt
    D_ENCRYPT_ENHANCED = 2     //Enhanced Encrypt, only support 40,128,256bit.  
};


enum _EptStandard
{
	DMRStandard	= 0, //目前仅用于DMR的AMBE版本, 即DMRA加密
	HyteraStandard
};



#ifdef __cplusplus
extern "C" {
#endif


/* 用户在使用加密库时，可以将时隙理解成一个逻辑信道 */


/*
  获取当前加密库的版本号
  版本信息占19个字节，调用该函数获取加密库版本时，
  要给pVerNumBuf分配19字节的空间
  版本信息如下:"\nEptLibVerNum=V1.9"
*/
extern void QueryEptLibVersionNum(UINT8* pVerNumBuf);


/*
    获取加密库能同时支持的最大时隙数  
    返回值为能支持的最大时隙数
*/
extern UINT8 GetEptLibMaxSlotNum(void);


/*
   统一配置所有时隙的加密参数。
   当所有的时隙的加密参数都一样时，通过该函数配置加密参数。
   一般在开机或切换信道时调用。
   
   EncryptType: 参考enum _EncryptType，
                使用Hytera基本加密时，          填D_ENCRYPT_BASIC
                使用DMRA加密或Hytera高级加密时，填D_ENCRYPT_ENHANCED
                
   IVByteLen:   使用DMRA加密时填       4
                使用Hytera高级加密时填 5
                使用Hytera基本加密时填 0
                
   PerVoiceFrameEncryptBitLen: AMBE声码器填       49*3=147
                               Selp声码器(2400)填 48*3=144
                               Selp声码器(2150)填 43*3=129
                               
   EncryptStandard: 参考enum _EptStandard，
                    使用DMRA加密时填 DMRStandard
                    使用Hytera加密填 HyteraStandard
   
   KeyEncryptEnFlag: 是否需要对密钥进行解密后再使用，此处填0

   pRandomData[4]  : 用于解密钥的随机数，此处填NULL，即0  

   返回值: EPT_OK:   配置成功
           EPT_FAIL: 配置失败
*/
extern INT8 EncryptConfigure(UINT8  EncryptType, 
                               UINT8  IVByteLen, 
                               UINT16 PerVoiceFrameEncryptBitLen, 
                               UINT8  EncryptStandard,
                               UINT8  KeyEncryptEnFlag,
                               UINT8  pRandomData[4]);


/*
   配置单个时隙的加密参数。
   如果各个时隙的加密参数不一样，通过该函数配置各个时隙的加密参数
   一般在开机或切换信道时调用
   
   Slot:        1 ～ N 
   
   EncryptType: 参考enum _EncryptType，
                使用Hytera基本加密时，          填D_ENCRYPT_BASIC
                使用DMRA加密或Hytera高级加密时，填D_ENCRYPT_ENHANCED
                
   IVByteLen:   使用DMRA加密时填       4
                使用Hytera高级加密时填 5
                使用Hytera基本加密时填 0
                
   PerVoiceFrameEncryptBitLen: AMBE声码器填       49*3=147
                               Selp声码器(2400)填 48*3=144
                               Selp声码器(2150)填 43*3=129
                               
   EncryptStandard: 参考enum _EptStandard，
                    使用DMRA加密时填 DMRStandard
                    使用Hytera加密填 HyteraStandard
   
   KeyEncryptEnFlag: 是否需要对密钥进行解密后再使用，此处填0

   pRandomData[4]  : 用于解密钥的随机数，此处填NULL，即0  

   返回值: EPT_OK:   配置成功
           EPT_FAIL: 配置失败
*/
extern INT8 EachSlotEncryptConfigure(UINT8 Slot, 
                                         UINT8  EncryptType, 
                                         UINT8  IVByteLen, 
                                         UINT16 PerVoiceFrameEncryptBitLen, 
                                         UINT8  EncryptStandard,
                                         UINT8  KeyEncryptEnFlag,
                                         UINT8  pRandomData[4]);


/*
   该函数只在Hytera高级加密时，填充或解析语音PI头和嵌入语音PI时使用
   
   pBuff: 指向语音PI或嵌入语音PI的有效数据
   ByteLen:要计算CheckSum的有效字节数

   返回值: CheckSum   
*/
extern UINT8 HyteraEptPICheckSum(UINT8* pBuff, UINT8 ByteLen);


/* 
   当使用D_ENCRYPT_ENHANCED类型加密时，
   该函数供应用层调用产生第一个IV数据 
   
   Slot:      1 ～ N
   
   Seed:      产生随机数的种子，建议用户每次调用该函数时，该值都不一样
   
   pIVOutput: 存放产生的IV数据,DMRA加密: 4个字节；Hytera高级加密: 5个字节
   
   一般只在准备发送语音/数据PI头时调用产生第一个IV数据 
   pIVOutput的数据按字节填充到语音/数据PI的相应位置   

   返回值: EPT_OK:   获取第一个IV成功
           EPT_FAIL: 获取第一个IV失败
*/
extern INT8 EncryptGetFirstIV(UINT8 Slot, UINT16 Seed, UINT8* pIVOutput);


/*
   该函数供加密时调用，用于初始化密钥，并获取下一个IV数据。
   
   Slot:      1 ～ N  
   
   pKey:      密钥指针，如果最后不足16bit的数据，有效比特放在高位
              如40比特密钥: 123456789A,
              则pKey[0]=0x1234;
                pKey[1]=0x5678;
                pKey[2]=0x9A00;
   
   KeyBitLen: 40/128/256
   
   pIVOutput: 存放产生的下一个IV数据, DMRA加密: 4个字节；Hytera高级加密: 5个字节
   当采用D_ENCRYPT_ENHANCED类型时，pIVOutput才会输出有效数据
   
   对于语音加密:在以下两种情况下调用:
   1、在调用EncryptGetFirstIV(UINT8 Slot, UINT8* pIVOutput)之后，
      开始加密语音之前需调用一次该函数
   2、在上一语音超帧加密结束之后，下一语音超帧加密之前需调用
      一次该函数
   该函数产生的IV数据，如果需要发送出去，则经过相应处理后嵌入
   到将要发送的超帧的声码器数据或嵌入信令中
      
   对于数据加密:在以下情况下调用:
      在调用EncryptGetFirstIV(UINT8 Slot, UINT8* pIVOutput)之后，
      开始加密数据之前需调用一次该函数

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 EncryptKeyInitandGetNextIV(UINT8          Slot, 
                                              UINT16* pKey, 
                                              UINT16  KeyBitLen, 
                                              UINT8*  pIVOutput);


/*
   在使用DMRA加密进行语音发射时，该函数才被使用。
   该函数的作用是获取4比特的加密IV信息，该信息将通过AMBE偷比特的方式发射出去

   Slot:          1 ～ N
   
   VoiceFrame:    A帧~F帧 ( MS:1-6; BS:9-E)
   
   VF20msCounter: 0 ～ 2 (每个语音帧第一次20ms语音编码为0，第二次为1，第三次为2)
   
   pIVInput[4]:   函数EncryptKeyInitandGetNextIV输出的IV

   返回值: 返回将要通过AMBE偷比特发送的IV数据，低4比特有效。

   每20毫秒AMBE编码时调用一次。
   
*/
extern UINT8 GetDMRA4BitsEptIVInfo(UINT8 Slot, 
                                      UINT8 VoiceFrame, 
                                      UINT8 VF20msCounter, 
                                      UINT8 pIVInput[4]);


/*
   在使用DMRA加密进行语音发射时，该函数才被使用。
   该函数的作用是将4比特的IV信息放到AMBE的偷比特中去

   StealBitNum:   在DMRA加密中填 4
   
   StealBitData:  函数GetDMRA4BitsEptIVInfo获得的返回值
   
   pVocoderBuff:  存放AMBE每20毫秒语音编码及FEC后的72比特数据(AMBE接口是通过72word存储的)
   
   WithFEC:       如果有使用AMBE的FEC，则WithFEC置1，且在AMBE的FEC后调用该函数
                  如果没有使用AMBE的FEC,则WithFEC置0,且在语音加密后调用该函数

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败

   每20毫秒AMBE编码时调用一次。   
*/
extern INT8 FillStealBitsInAmbeBuff(UINT8   StealBitNum, 
                                      UINT8   StealBitData, 
                                      INT16* pVocoderBuff, 
                                      UINT8   WithFEC);


/*
   语音加密
   每20毫秒调用一次，在声码器语音编码之后，声码器FEC之前调用
   
   Slot:       1 ～ N   
   
   VoiceFrame: A帧~F帧 ( MS:1-6; BS:9-E)
   
   pInput:     本次要加密的语音数据；如果最后不足16bit的数据，放在高位
   pOutput:    本次加密后的语音数据；如果最后不足16bit的数据，放在高位
               如:AMBE每20毫秒要加密的数据是49比特，
               则pInput[0]的最高位存放AMBE的第1比特，
                 pInput[0]的最低位存放AMBE的第16比特，
                 pInput[3]的最高位存放AMBE的第49比特，                 
   
   BitLen:     每20毫秒语音要加密的bit个数:
               AMBE声码器       49
               Selp声码器(2400) 48
               Selp声码器(2150) 43
               
   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 VoiceEncrypt(UINT8       Slot,
                             UINT8   VoiceFrame, 
                             UINT16* pInput, 
                             UINT16* pOutput,
                             UINT16  BitLen);


/*
   数据加密
   本函数每次调用最多只能处理20个字节，
   如果要加密的数据超过20个字节，可以分段多次调用。
   
   Slot:       1 ～ N    
   
   pInput:     本次要加密的数据，如果最后不足16bit的数据，放在高位
   pOutput:    本次加密后的数据，如果最后不足16bit的数据，放在高位
               如果存放在PDU的数据顺序是
               Byte0 = 0x12; Byte1 = 0x34,
               则pInput[0]= 0x1234.
   
   BitLen:     本次要加密的数据的bit个数，目前最大可为160比特
   
   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 DataEncrypt(UINT8 Slot,
                         UINT16* pInput,
                         UINT16* pOutput,
                         UINT16 BitLen);


/* 
   当采用D_ENCRYPT_ENHANCED类型解密时，
   在开始语音解密前，调用一次该函数，初始化语音解密相关参数
   
   Slot:      1 ～ N
  

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 VoiceDecryptInit(UINT8 Slot);


/* 
   当采用D_ENCRYPT_ENHANCED类型，语音解密时，
   当获取正确的IV数据后，调用该函数通知加密库
   
   Slot:      1 ～ N
   
   pIVInput:  接收到的IV数据   

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 VoiceDecryptIV(UINT8 Slot, UINT8* pIVInput);


/* 
   当采用D_ENCRYPT_ENHANCED类型，数据解密时，
   在获取正确的IV数据后，调用该函数通知加密库
   
   Slot:      1 ～ N
   
   pIVInput:  接收到的IV数据 

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 DataDecryptIV(UINT8 Slot, UINT8* pIVInput);


/*
   该函数供解密时调用，用于初始化密钥
   
   Slot:      1 ～ N   

   pKey:      密钥指针，如果最后不足16bit的数据，有效比特放在高位
              如40比特密钥: 123456789A,
              则pKey[0]=0x1234;
                pKey[1]=0x5678;
                pKey[2]=0x9A00;
   
   KeyBitLen: 40/128/256      
      
   对于语音解密:
      每次开始对A帧进行解密前调用该函数
      
   对于数据解密:
      在收到数据PI头，执行函数DataDecryptIV之后，对数据解密之前调用一次该函数

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
*/
extern INT8 DecryptKeyInit(UINT8 Slot, UINT16* pKey, UINT16 KeyBitLen);


/*
   在使用DMRA加密，进行加密语音接收时，该函数才被使用。
   该函数的作用是从AMBE的偷比特中获取加密IV信息

   StealBitNum:    在DMRA加密中填 4
   
   pStealBitData:  从AMBE的偷比特中获取加密IV信息放在该指针
   
   pVocoderBuff:  存放接收到的AMBE的72比特数据(AMBE接口是通过72word存储的)
   
   WithFEC:       如果有使用AMBE的FEC，则WithFEC置1，且在AMBE的FEC解码前调用该函数
                  如果没有使用AMBE的FEC,则WithFEC置0,且在语音解密前调用该函数

   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败

   每20毫秒AMBE解码时调用一次。   
*/
extern INT8 GetStealBitsFromAmbeBuff(UINT8   StealBitNum, 
                                           UINT8*  pStealBitData, 
                                           INT16* pVocoderBuff, 
                                           UINT8   WithFEC);

/*
   在使用DMRA加密进行语音接收时，该函数才被使用。
   该函数的作用是从语音超帧的偷比特中获取解密IV

   Slot:          1 ～ N
   
   VoiceFrame:    A帧~F帧 ( MS:1-6; BS:9-E)
   
   VF20msCounter: 0 ～ 2 (每个语音帧第一次20ms为0，第二次为1，第三次为2)
   
   StealBitData:  函数GetStealBitsFromAmbeBuff所获取的偷比特数据
   
   pIVOutput[4]:  输出解密IV

   返回值:返回值为EPT_OK时，pIVOutput的数据才有效，
          此时用户要调用函数VoiceDecryptIV，将IV传递给加密库。
   
   每20毫秒AMBE解码时调用一次。   
*/
extern INT8 GetDMRA_DptIVFromAmbeStealBits(UINT8 Slot, 
                                                     UINT8 VoiceFrame, 
                                                     UINT8 VF20msCounter, 
                                                     UINT8 StealBitData, 
                                                     UINT8 pIVOutput[4]);


/*
   语音解密
   每20毫秒调用一次，在声码器的FEC之后，声码器语音解码之前
   
   Slot:       1 ～ N   
   
   VoiceFrame: A帧~F帧 ( MS:1-6; BS:9-E)
   
   pInput:     本次要解密的语音数据；如果最后不足16bit的数据，放在高位
   pOutput:    本次解密后的语音数据；如果最后不足16bit的数据，放在高位
               如:AMBE每20毫秒要解密的数据是49比特，
               则pInput[0]的最高位存放AMBE的第1比特，
                 pInput[0]的最低位存放AMBE的第16比特，
                 pInput[3]的最高位存放AMBE的第49比特，                 
   
   BitLen:     每20毫秒语音要解密的bit个数:
               AMBE声码器       49
               Selp声码器(2400) 48
               Selp声码器(2150) 43
               
   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败;

   说明:   当返回值为EPT_FAIL，为了避免播放出来的声音是杂音，
           建议调用该函数的应用层在返回值为EPT_FAIL时，
           将pOutput替换为声码器的静音帧，这样经过声码器解码后，
           播放出来的就是静音。
*/
extern INT8 VoiceDecrypt(UINT8       Slot, 
                             UINT8   VoiceFrame, 
                             UINT16* pInput, 
                             UINT16* pOutput,
                             UINT16  BitLen);


/*
   数据解密
   本函数每次调用最多只能处理20个字节，
   如果要解密的数据超过20个字节，可以分段多次调用。
   
   Slot:       1 ～ N    
   
   pInput:     本次要解密的数据，如果最后不足16bit的数据，放在高位
   pOutput:    本次解密后的数据，如果最后不足16bit的数据，放在高位
               如果存放在PDU的数据顺序是
               Byte0 = 0x12; Byte1 = 0x34,
               则pInput[0]= 0x1234.
   
   BitLen:     本次要解密的数据的bit个数，目前最大可为160比特
   
   返回值: EPT_OK:   函数执行成功
           EPT_FAIL: 函数执行失败
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

