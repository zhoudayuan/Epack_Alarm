/*
*	Describle : 基于终端提供的加密库，为E-pack语音和数据加密封装的的一套接口函数
*	Version   : V1.0
*	Author	  : XYM
*	Dependency: 终端部门提供的加密库
*	
*	History	  : 
*
*    Date		  Author         Describle
*	2017-08-29	   XYM         完成V1.0版本
*	2017-09-12	   XYM		   完善部分注释，修改测试版本
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include"EncDec.h"
#include"Encrypt_Interface.h"
#include "typedef.h"
#include "NVOC_FEC_Interface.h"
#include "AMBE_FEC_Interface.h"
#include "time.h"

//#define DEBUG


static UINT8 s_Key_Load_Flag	= 0;  			//秘钥加载状态   0未加载秘钥 1已加载秘钥
static UINT8 s_Valid_Key_Count	= 0;			//有效的秘钥个数
static KEY_TABLE s_Keytable[TABLENUM_30];  		//秘钥表
static ENCDEC_PARAM s_Param_Enc;				//加密参数 -- 语音
static ENCDEC_PARAM s_Param_Dec;				//解密参数 -- 语音
static ENCDEC_PARAM s_Param_Enc_Data;			//加密参数 -- 数据
static ENCDEC_PARAM s_Param_Dec_Data;			//解密参数 -- 数据
static UINT8 s_Voicetype = 0;					//语音类型
static UINT8 s_EncryptType = 0;					//加密类型

#ifdef DEBUG
static UINT16 s_DefaultKey[3] = {0x1234,0x5678,0x9000}; 
#endif

static const UINT16  s_au2Qr16MtxG[9] =
{
    0x57, 0x79, 0x72, 0x64, 0x31, 0x1b, 0x4f, 0x1e, 0x3c
};

static const UINT16 s_au2Hm16MtxG[5] =
{
    0x537, 0x759, 0x1eb, 0x3d6, 0x7ac
};

static const UINT16 s_au2Gl24MtxG[12] =
{
	0x0a4f, 0x0f68, 0x07b4, 0x03da, 0x01ed, 0x0ab9,
	0x0f13, 0x0dc6, 0x06e3, 0x093e, 0x049f, 0x0c75
}; 
static const UINT16 s_au2Hm16MtxH[5] =
{
    0xa6e1, 0xeb22, 0x3d64, 0x7ac8, 0xf590
};
static const UINT16 s_au2Hm16MtxB[32] =
{
    0x0000, 0x0001, 0x0002, 0x0000, 0x0004, 0x0000, 0x0000, 0x0020,
    0x0008, 0x0000, 0x0000, 0x0200, 0x0000, 0x0040, 0x0800, 0x0000,
    0x0010, 0x0000, 0x0000, 0x8000, 0x0000, 0x0400, 0x0100, 0x0000,
    0x0000, 0x0080, 0x4000, 0x0000, 0x1000, 0x0000, 0x0000, 0x2000
};

/*********************************** 功能函数  ***************************/

static UINT32 golay_24_12_enc(UINT32 u4Data)
{
	UINT16  u2Res       = 0;
	UINT16  u2Cnt       = 0;
	UINT32  u4Cal       = u4Data;

	for (u2Cnt = 0; u2Cnt < 12; u2Cnt++)
	{
		u2Res  = u4Data & s_au2Gl24MtxG[u2Cnt];
		u2Res ^= u2Res >> 8;
		u2Res ^= u2Res >> 4;
		u2Res ^= u2Res >> 2;
		u2Res ^= u2Res >> 1;

		u4Cal = (u4Cal << 1) | (u2Res & 0x01);
	}

	return (u4Cal & 0x0ffffff);
}
static UINT16 ALG_Crc4(const UINT16* pu2Data, UINT16 u2DataCnt)
{
	UINT16 u2Cnt = 0;
	UINT16 u2Ret = 0;

	while (0 != (u2DataCnt--))
	{
		for (u2Cnt = 0x8000; u2Cnt != 0; u2Cnt >>= 1)
		{
			if (0 != (u2Ret & 0x8))
			{
				u2Ret *= 2;
				u2Ret ^= 0x3;
			}
			else
			{
				u2Ret *= 2;
			}
			if (0 != (*pu2Data & u2Cnt))
			{
				u2Ret ^= 0x3;
			}
		}
		pu2Data++;
	}

	return ((~u2Ret) & 0x0f);
} 

static void bptc_72_32_enc(UINT16* pu2In, UINT16* pu2Out)
{
	UINT32 u4Golay = 0;
	UINT16 u2Crc4  = 0;

	u2Crc4  = ALG_Crc4(pu2In,2);

	u4Golay = (pu2In[0] >> 4) & 0x0fff;
	u4Golay = golay_24_12_enc(u4Golay);

	pu2Out[0] = (u4Golay >> 8) & 0xffff;
	pu2Out[1] = (u4Golay << 8) & 0xff00;

	u4Golay = ((pu2In[0] << 8) & 0x0f00) | ((pu2In[1] >> 8) & 0x00ff);
	u4Golay = golay_24_12_enc(u4Golay);

	pu2Out[1] = pu2Out[1] | ((u4Golay >> 16) & 0x0ff);
	pu2Out[2] = u4Golay & 0xffff;

	u4Golay = ((pu2In[1] << 4) & 0x0ff0) | u2Crc4;
	u4Golay = golay_24_12_enc(u4Golay);

	pu2Out[3] = (u4Golay >> 8) & 0xffff;
	pu2Out[4] = (u4Golay << 8) & 0xff00;

} 


/**
 * @brief   将short型数组展开成bit流，高bit开始
 * @param [in]   puWords    
 * @param [in]   uWordNum
 * @param [out]  puBits  
 * @author  张超
 * @since   fec1.0
 * @bug
 */

static void vConvertWord2Bit(UINT16 *puBits, UINT16 *puWords, UINT16 uWordNum)
{
    UINT16 i = 0;
    UINT16 j = 0;
    UINT16 uDataTemp = 0;

    for(i=0;i<uWordNum;i++)
    {
        uDataTemp = 0x8000;
        for(j=0;j<16;j++)
        {
            if(0 != (uDataTemp & (*(puWords+i))))
            {
                *(puBits+i*16+j) = 0x0001;
            }
            else
            {
                *(puBits+i*16+j) = 0x0000;
            }
            uDataTemp = uDataTemp >> 1;
        }
    }
}
/**
 * @brief   将bit流合并成short型数组，从高bit开始
 * @param [in]   puBits    
 * @param [in]   uBitNum
 * @param [out]  puWords  
 * @author  张超
 * @since   fec1.0
 * @bug
 */
static void vConvertBit2Word(UINT16 *puWords, UINT16 *puBits, UINT16 uBitNum)
{
	UINT16 i = 0;
	UINT16 j = 0;
	UINT16 uDataTemp = 0x8000; 
    UINT16 aa = 0;
    UINT16 bb = 0;

    aa = uBitNum/16;
    bb = uBitNum%16;

	for(i=0;i<aa;i++)
	{
		for(j=0;j<16;j++)
		{
			if(0x0000 != *(puBits + 16*i+j))
			{	
				*(puWords + i) = *(puWords + i) | uDataTemp;
			}
			else
			{
				*(puWords + i) = *(puWords + i) & (~uDataTemp);
			
			}
			uDataTemp = uDataTemp >> 1;
		}	
		uDataTemp = 0x8000;
	}

    uDataTemp = 0x8000;
    
    for(i=0;i<bb;i++)
    {
        if(0x0000 != *(puBits + 16*aa+i))
        {
            *(puWords + aa) =  *(puWords + aa) | uDataTemp;
        }
        else
        {
            *(puWords + aa) = *(puWords + aa) & (~uDataTemp);
        }
    	uDataTemp = uDataTemp >> 1;
    }
}

//累加和校验
static UINT8 _CaculCheckSum(DLL_ENCLIB_VOICE* pInput)
{
	UINT16 uCheckSum = 0;
	
	uCheckSum = pInput->PIInfo[0] + pInput->PIInfo[1] +  
				pInput->PIInfo[2] + pInput->PIInfo[3] +
				pInput->PIInfo[4] + pInput->PIInfo[5] +
				pInput->PIInfo[6] + pInput->PIInfo[7] +
				pInput->PIInfo[8];
	uCheckSum &= 0xFF;
	uCheckSum = ~uCheckSum + 1;
	return (UINT8)(uCheckSum & 0x00FF);
}

static void _27Byte_2_216Bit(DLL_ENCLIB_VOICE* pInput,UINT16 * pDst,UINT16 size)
{
	UINT16 tmpword[14] = {0};
	UINT16 tmpbit[224] = {0};
	UINT8 i = 0;
	if(NULL == pInput || NULL == pDst)
	{
		printf("[_27Byte_2_216Bit] NULL pointer!\n" );
		return;
	}
	//27字节原始数据转换成word
	for(i=0;i<28;i+=2)
	{
		tmpword[i/2] = pInput->Payload[i] << 8 | pInput->Payload[i+1];
	}
	vConvertWord2Bit(tmpbit,tmpword,14);
	memcpy(pDst,tmpbit,size);
	
}
static void _216Bit_2_27Byte(DLL_ENCLIB_VOICE* pInput,UINT16 * pSrc)
{
	UINT16 tmpword[14] = {0};
	UINT8 i = 0;
	if(NULL == pInput || NULL == pSrc)
	{
		printf("[_216Bit_2_27Byte] NULL pointer!\n" );
		return;
	}
	
	vConvertBit2Word(tmpword,pSrc,216);
	
	//大小端
	for(i=0;i<14;i++)
	{
		tmpword[i] = htons(tmpword[i]);
	}
	memcpy(pInput->Payload,tmpword,27);	
}

/************************************* 内部函数 *********************************/

/**
 * @brief   	 : 初始化秘钥和获取下一IV
 * @param [in ]  : pInput    
 * @param [out]  : NextIV  返回NextIV
 * @author  	 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static INT8 _InitKeyAndGetNextIV(DLL_ENCLIB_VOICE* pInput,UINT8 NextIV[5])
{
	//初始化秘钥失败
	if(EncryptKeyInitandGetNextIV(VOICE_SLOT,												//Slot
								  (UINT16 *)(s_Param_Enc.KeyValue) ,		//KEYEncpKey 
								  (UINT16	)(s_Param_Enc.KeyBitLen),	//KeyBitLen
								  (UINT8  *)NextIV
								  )  < 0)
	{
		printf("[_InitKeyAndGetNextIV] Get NextIV failed!\n");
		return -1;
		#if 0
		//尝试重新初始化
		if(_EncryptConfigure(pInput->VoiceType,pInput->EncryptType,VOICE_SLOT) < 0)
		{
			printf("[_EncryptInit] _EncryptConfigure failed!\n");
			return -1;
		}
		if(EncryptKeyInitandGetNextIV(VOICE_SLOT,												//Slot
									  (UINT16 *)(s_Param_Enc.KeyValue) ,		//KEYEncpKey 
									  (UINT16	)(s_Param_Enc.KeyBitLen),	//KeyBitLen
									  (UINT8  *)NextIV
									  )  < 0)
		{
			printf("[_InitKeyAndGetNextIV] Get NextIV failed!\n");
			return -1;
		}
		#endif
	}
	return 0;
}


/**
 * @brief   	 : A帧调用，同步一次IV
 * @param [in ]  : pInput    
 * @author  	 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _GetNextIV(DLL_ENCLIB_VOICE* pInput)
{
    //UINT16 auPI_Temp[9] = {0};
	UINT16 auTemp1[2]   = {0};
	UINT16 auTemp2[5]   = {0};    
    //memset(auPI_Temp, 0, 9);
	UINT8 NextIV[5];
	INT8 ret = 0;
    switch(s_EncryptType)
    {
    	case ENCRYPT_HYT_BASE:
			ret = _InitKeyAndGetNextIV(pInput,NextIV);
			break;
        case ENCRYPT_HYT_ENHA:
			ret = _InitKeyAndGetNextIV(pInput,NextIV); 
			
			//生成原始的待变成内嵌信息的PI信息,返回给调用方，由调用者进行内嵌编码
		   pInput->PIInfo[0] = 0x0068;
            pInput->PIInfo[1] = 0x0002;
            pInput->PIInfo[2] = s_Param_Enc.KeyID    ;
            pInput->PIInfo[3] = NextIV[0] ;
            pInput->PIInfo[4] = NextIV[1] ;
            pInput->PIInfo[5] = NextIV[2] ;
            pInput->PIInfo[6] = NextIV[3] ;
            pInput->PIInfo[7] = NextIV[4] ;
            
            pInput->PIInfo[8] = pInput->PIInfo[0] + pInput->PIInfo[1] + 
								pInput->PIInfo[2] + pInput->PIInfo[3] +
                           		pInput->PIInfo[4] + pInput->PIInfo[5] + 
                           		pInput->PIInfo[6] + pInput->PIInfo[7];
            pInput->PIInfo[8] = ~(pInput->PIInfo[8]) + 1;
	
			
            break;
        case ENCRYPT_DMRA:
			ret = _InitKeyAndGetNextIV(pInput,NextIV);
			
			//printf("[Frame A]NextIV(32) --- %02X %02X %02X %02X \n",NextIV[0],NextIV[1],NextIV[2],NextIV[3]);
            auTemp1[0] = (NextIV[0] <<8) | NextIV[1];
            auTemp1[1] = (NextIV[2] <<8) | NextIV[3];
            bptc_72_32_enc((UINT16 *)auTemp1, auTemp2);
			
            memset((void *)&(s_Param_Enc.DMRA_GolayEnc), 0, 80);
			
			//printf("[Frame A]NextIV(72) --- %04X %04X %04X %04X %04X \n",auTemp2[0],auTemp2[1],auTemp2[2],auTemp2[3],auTemp2[4]);
			//前72个数据是需要放在偷比特的数据
            vConvertWord2Bit((UINT16*)&s_Param_Enc.DMRA_GolayEnc, (UINT16*)&auTemp2, 5);
            break;         
        default:
        	ret = -1;
            break;

    }
	return ret;
}

/**
 * @brief   	 : 语音加密
 * @param [in ]  : pInput    
 * @author  	 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static INT8 _SoftEncryptVoice(DLL_ENCLIB_VOICE* pInput)
{
    UINT8 Cnt20ms 			= 0;
    UINT16 uEncryptBitLen  	= 0;
	UINT16 DataIn[5]		= {0};
	UINT16 DataOut[5]		= {0};
	UINT8 i=0;

	//初始化失败
	if(s_Param_Enc.InitFlag == 0)
	{
		printf("EncryptInit failed! -- InitFlag 0\n");
		return -1 ;
	}

    switch(s_Voicetype)
    {
        case AMBE: 
            uEncryptBitLen  = 49;
            break;
        default  : 
            uEncryptBitLen  = 48; 
            break;
    }
    for(Cnt20ms=0;Cnt20ms<3;Cnt20ms++)
    {	
    	memset(DataIn,0,sizeof(DataIn));
		memset(DataOut,0,sizeof(DataOut));
		memcpy((UINT8*)DataIn,pInput->Payload+9*Cnt20ms,9);
		for(i=0;i<sizeof(DataIn)/sizeof(DataIn[0]);i++)
		{
			DataIn[i] = htons(DataIn[i]);
		}			
        if(VoiceEncrypt(VOICE_SLOT,pInput->FrameNum,(UINT16*)DataIn,(UINT16*)DataOut,uEncryptBitLen) < 0)
        {
         	printf("[VoiceEncrypt] VoiceEncrypt failed!\n");
			return -1;
        } 
		for(i=0;i<sizeof(DataOut)/sizeof(DataOut[0]);i++)
		{
			DataOut[i] = htons(DataOut[i]);
		}
		memcpy(pInput->Payload+9*Cnt20ms,DataOut,9);
    }
	return 0;
}


/**
 * @brief   	 : DMRA F帧内嵌信息添加
 * @param [in ]  : pInput    
 * @author  	 : XYM
 * @return		 : void
 */
static void _FrmF_Enc_DMRA(DLL_ENCLIB_VOICE* pInput)
{
//	UINT16 uAlgID_KeyID   = 0;
//  UINT16 auRC_EncRes[3] = {0};
	UINT8   GI = 0;

    switch(s_EncryptType)
    {
        case ENCRYPT_DMRA:
#if 0
            uAlgID_KeyID = (  (s_Param_Enc.KeyID) << 3   |
                              (s_Param_Enc.AlgID & 0x07)
                            ) & 0x07FF; 
            
            rc_enc(&uAlgID_KeyID, 1, auRC_EncRes);
            
            pInput->EMB_INFO[0] = auRC_EncRes[0] >> 8; 
            pInput->EMB_INFO[1] = auRC_EncRes[0] >> 0;
            pInput->EMB_INFO[2] = auRC_EncRes[1] >> 8; 
            pInput->EMB_INFO[3] = auRC_EncRes[1] >> 0;
            pInput->EMB_INFO[4] = auRC_EncRes[2] >> 8; 
            pInput->EMB_INFO[5] = auRC_EncRes[2] >> 0;
#endif
            pInput->PIInfo[0]     = (GI<<5) | s_Param_Enc.AlgID;
            pInput->PIInfo[1]     =  0x10;        
            pInput->PIInfo[2]     =  s_Param_Enc.KeyID;

            break;
        default:
            break;
    }
}

/**
 * @brief   	 : 初始化加密参数，获取秘钥，秘钥ID、FirstIV等
 * @param [in ]  : Slot 时隙,语音使用时隙1，数据使用时隙2  
 * @param [out]  : pEncParam	接收加密参数
 * @param [out]  : pFirstIV		接收FirstIV
 * @author  	 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static INT8 _InitEncryptParam(UINT8 Slot,ENCDEC_PARAM* pEncParam,UINT8 pFirstIV[5])
{
	UINT8  uKeywordLen = 0;
	UINT8  uKeySN      = 0;
	INT8   ReturnVal   = 0;

	if(ENCRYPT_HYT_BASE != s_EncryptType)
	{
		ReturnVal = EncryptGetFirstIV(Slot,1,pFirstIV);
	}
	
	if(1 == s_Key_Load_Flag)
	{
		
		if(ENCRYPT_HYT_BASE == s_EncryptType)
		{	
			uKeySN = 0;
		}
		else
		{	
			srand((unsigned)time(NULL));          //初始化随机数
			UINT8 RAD=rand();
			//printf("RAD=%d\n",RAD);
			uKeySN			 = RAD%s_Valid_Key_Count;
			//uKeySN			 = 2;	
		}
		pEncParam->KeyID	 = s_Keytable[uKeySN].KeyID;
		pEncParam->KeyBitLen = s_Keytable[uKeySN].key_length;
		printf("s_Valid_Key_Count=%d,uKeySN=%d,pEncParam->KeyID=%d,pEncParam->KeyBitLen=%d\n",s_Valid_Key_Count,uKeySN,pEncParam->KeyID,pEncParam->KeyBitLen);

		
#ifdef DEBUG
		//测试使用
		pEncParam->KeyID = 1;
		pEncParam->KeyBitLen = 40;
#endif
	}
	else
	{
		return -1;
	}
	switch(pEncParam->KeyBitLen)
	{
		case 40:
			pEncParam->AlgID = 1;
			uKeywordLen 	 = 6;			 
			break;
		case 128:
			pEncParam->AlgID = 4;
			uKeywordLen 	 = 16;					 
			break;		
		case 256:
			pEncParam->AlgID = 5;
			uKeywordLen 	 = 32;					 
			break;
		default:
			ReturnVal 		 = -1;
	}  
	memcpy((UINT8*)pEncParam->KeyValue, (UINT8*)s_Keytable[uKeySN].KeyValue, uKeywordLen); 


	//printf("pEncParam->KeyBitLen=%d\n",pEncParam->KeyBitLen);
	//for(UINT8 i=0;i<16;i++)
		//printf("s_Keytable[%d].KeyValue[%d]=%#04x\n",uKeySN,i,s_Keytable[uKeySN].KeyValue[i]);


	
#ifdef DEBUG
	//测试，采用默认秘钥
	memcpy((UINT8*)pEncParam->KeyValue, (UINT8*)s_DefaultKey, uKeywordLen);
#endif
 
	
	return ReturnVal;
	
}

/**
 * @brief		 : 加密初始化
 * @param [in ]  : Slot 时隙,语音使用时隙1，数据使用时隙2  
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static INT8 _EncryptInit(DLL_ENCLIB_VOICE* pInput)
{
	UINT8   GI = 0;
	UINT8   FirstIV[5] = {0};
	INT8	Ret = 0;
	switch(s_EncryptType)
	{
		case ENCRYPT_HYT_BASE:
			Ret = _InitEncryptParam(VOICE_SLOT,&s_Param_Enc,FirstIV);		
			break;
			
		case ENCRYPT_HYT_ENHA:
			Ret = _InitEncryptParam(VOICE_SLOT,&s_Param_Enc,FirstIV);
			pInput->PIInfo[0] = 0x68;	// should be set "0x68" (hytera)
			pInput->PIInfo[1] = 0x02;	// low 4 bits:should be set "2" high 4 bits:should be set "0" 
			pInput->PIInfo[2] = s_Param_Enc.KeyID; 
			pInput->PIInfo[3] = FirstIV[0];
			pInput->PIInfo[4] = FirstIV[1];
			pInput->PIInfo[5] = FirstIV[2];
			pInput->PIInfo[6] = FirstIV[3];
			pInput->PIInfo[7] = FirstIV[4];
			pInput->PIInfo[8] = 0x00;
			pInput->PIInfo[9] = _CaculCheckSum(pInput);
			break;
			
		case ENCRYPT_DMRA:
			Ret = _InitEncryptParam(VOICE_SLOT,&s_Param_Enc,FirstIV);
			#if 0
			//个呼和组呼
            if((pInput->PIInfo[0] & 0x3F) == 0x0000)
                pParam->muGI = 0x0001;
            
            else
                pParam->muGI = 0x0000; 
			#endif
            pInput->PIInfo[0]     = (GI<<5) | s_Param_Enc.AlgID;
            pInput->PIInfo[1]     =  0x10;        
            pInput->PIInfo[2]     =  s_Param_Enc.KeyID;
            pInput->PIInfo[3]     =  FirstIV[0];
            pInput->PIInfo[4]     =  FirstIV[1];
            pInput->PIInfo[5]     =  FirstIV[2];
            pInput->PIInfo[6]     =  FirstIV[3];
            pInput->PIInfo[7]     =  pInput->LCInfo[3];
            pInput->PIInfo[8]     =  pInput->LCInfo[4];
            pInput->PIInfo[9]     =  pInput->LCInfo[5];

            break;
		default:
			Ret = -1;
			break;
	}
	return Ret;
}



/**
 * @brief   	 : FEC解码
 * @param [in ]  : pInput 
 * @param [in ]  : sTealBit	偷比特个数
 * @author  	 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static void _FecDec(DLL_ENCLIB_VOICE* pInput,UINT8 sTealBit)
{
	UINT8  i = 0;
	UINT8  j = 0;
	UINT16 DstBit[49] = {0};
	UINT16 SrcBit[216] = {0};
	UINT16  Tmpword[4] = {0};
	switch(s_Voicetype)
	{
		case AMBE:
			_27Byte_2_216Bit(pInput,SrcBit,sizeof(SrcBit));
			memset(pInput->Payload,0,sizeof(pInput->Payload));
			for(i=0;i<3;i++)
			{
				memset(DstBit,0,sizeof(DstBit));
				memset(Tmpword,0,sizeof(Tmpword));
				
				//解FEC  得到49bit解码数据
				AMBE_FECDecFun(SrcBit+72*i,DstBit,sTealBit);
				
				//49bit数据转成short 7个
				vConvertBit2Word(Tmpword,DstBit,49);
				for(j=0;j<4;j++)
				{
					Tmpword[j] = htons(Tmpword[j]);
				}
				memcpy(pInput->Payload+9*i,Tmpword,sizeof(Tmpword));				
			}
			break;
			
		case NVOC:
			_27Byte_2_216Bit(pInput,SrcBit,sizeof(SrcBit));

			memset(pInput->Payload,0,sizeof(pInput->Payload));
			for(i=0;i<3;i++)
			{
				memset(DstBit,0,sizeof(DstBit));
				memset(Tmpword,0,sizeof(Tmpword));
				
				//解FEC  进72bit流，出3个word 得到48bit解码数据 
				NVOC_FecDecFun(SrcBit+72*i,Tmpword);

				//大小端转换，完成数据拷贝
				for(j=0;j<3;j++)
				{
					Tmpword[j] = htons(Tmpword[j]);
				}
				memcpy(pInput->Payload+9*i,Tmpword,sizeof(UINT16)*3);				
			}
			break;
		default:break;
	}
}

/**
 * @brief   	 : FEC编码
 * @param [in ]  : pInput    
 * @param [in ]  : sTealBit 偷比特位数    
 * @author  	 : XYM
 */
static void _FecEnc(DLL_ENCLIB_VOICE* pInput,UINT8 sTealBit)
{
	UINT8  i = 0;
	UINT8  j = 0;
	UINT16 DstBit[72] = {0};
	UINT16 Tmpbit[49] = {0};
	UINT16 SrcBit[216] = {0};
	UINT16  Tmpword[5] = {0};
	switch(s_Voicetype)
	{
		case AMBE:
			_27Byte_2_216Bit(pInput,SrcBit,sizeof(SrcBit));
			
			memset(pInput->Payload,0,sizeof(pInput->Payload));
			for(i=0;i<3;i++)
			{
				//拷49位数据
				memcpy(Tmpbit,SrcBit+72*i,sizeof(Tmpbit));

				//FEC编码
				AMBE_FECEncFun(Tmpbit,DstBit,sTealBit);

				//72bit转成9字节
				vConvertBit2Word(Tmpword,DstBit,72);
				for(j=0;j<5;j++)
				{
					Tmpword[j] = htons(Tmpword[j]);
				}
				//存储
				memcpy(pInput->Payload+9*i,Tmpword,9);
			}

			break;
		case NVOC:
			for(i=0;i<3;i++)
			{
				memset(DstBit,0,sizeof(DstBit));
				memset(Tmpword,0,sizeof(Tmpword));
				
				//拷3个字数据
				memcpy(Tmpword,pInput->Payload+9*i,sizeof(UINT16)*3);
				for(j=0;j<3;j++)
				{
					Tmpword[j] = htons(Tmpword[j]);
				}					
				
				//FEC编码 进3个word 出72bit流
				NVOC_FecEncFun(Tmpword,DstBit);

				//72bit转成9字节
				memset(Tmpword,0,sizeof(Tmpword));
				vConvertBit2Word(Tmpword,DstBit,72);
				for(j=0;j<5;j++)
				{
					Tmpword[j] = htons(Tmpword[j]);
				}
				//存储
				memcpy(pInput->Payload+9*i,Tmpword,9);
			}
			break;
		default:break;
	}
}

/**
 * @brief   	 : 添加偷比特
 * @param [in ]  : pInput    
 * @author  	 : XYM
 */
static void _AddStealbit(DLL_ENCLIB_VOICE* pInput)
{
	UINT8 	Cnt20ms = 0;
	UINT16  SrcBit[216] = {0};  
	UINT8 	StealBit[4] = {0};
	switch(s_Voicetype)
	{
		case AMBE:
			StealBit[0] = APCO_HR_BITSTEAL_I2;
			StealBit[1] = APCO_HR_BITSTEAL_I3;
			StealBit[2] = APCO_HR_BITSTEAL_I4;
			StealBit[3] = APCO_HR_BITSTEAL_I5;
			break;
		case NVOC:
			StealBit[3] = 47;
			StealBit[2] = 46;
			StealBit[1] = 45;
			StealBit[0] = 44;
			break;
		default:
			break;
	}
	//将加完FEC的数据转换成bit流
	_27Byte_2_216Bit(pInput,SrcBit,sizeof(SrcBit));	

	for(Cnt20ms=0;Cnt20ms<3;Cnt20ms++)
	{
		SrcBit[Cnt20ms*72 + StealBit[3]] 
				 = s_Param_Enc.DMRA_GolayEnc[Cnt20ms*24 + (pInput->FrameNum - 1)*4 + 0];
		SrcBit[Cnt20ms*72 + StealBit[2]] 
				 = s_Param_Enc.DMRA_GolayEnc[Cnt20ms*24 + (pInput->FrameNum - 1)*4 + 1];
		SrcBit[Cnt20ms*72 + StealBit[1]] 
				 = s_Param_Enc.DMRA_GolayEnc[Cnt20ms*24 + (pInput->FrameNum - 1)*4 + 2];
		SrcBit[Cnt20ms*72 + StealBit[0]] 
				 = s_Param_Enc.DMRA_GolayEnc[Cnt20ms*24 + (pInput->FrameNum - 1)*4 + 3];
	}	

	memset(pInput->Payload,0,sizeof(pInput->Payload));
	_216Bit_2_27Byte(pInput,SrcBit);

}

/**
 * @brief		 : 语音加密
 * @param [in ]  : pInput	 
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */

static INT8 _Encrypt_Voice(DLL_ENCLIB_VOICE* pInput)
{
	INT8 ret = 0;

	
	switch(pInput->FrameNum)
	{
		case VOICE_LC_FRAME:
			
			//LC --- 初始化加密库
			ret = _EncryptInit(pInput);
			if(ret == 0)
			{
				s_Param_Enc.InitFlag = 1;
			}
			else
			{
				s_Param_Enc.InitFlag = 0;
			}
			break;			 
		case VOICE_A_FRAME:	
		
			//初始化秘钥和获取NEXTIV HYTERA加密时，PIInfo存放的是NEXTIV等信息
			if(_GetNextIV(pInput) < 0)
			{
				printf("_GetNextIV failed!\n");
				s_Param_Enc.InitFlag = 0;
				return -1 ;	
			}
			
			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)		
				return 0;
		
			//数据下行，解FEC偷比特数都是0
			_FecDec(pInput,0);

			//语音加密
			ret = _SoftEncryptVoice(pInput);

			//数据下行，加FEC时DMRA偷比特数是4，HYTERA偷比特数是0
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecEnc(pInput,4);

				//添加偷比特
				_AddStealbit(pInput);
			}
			else
			{
				_FecEnc(pInput,0);
			}	
			break;
			
		case VOICE_B_FRAME:  
  		case VOICE_C_FRAME:
  		case VOICE_D_FRAME:
  		case VOICE_E_FRAME:
  		
  			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)		
				return 0;
				
			_FecDec(pInput,0);
			ret = _SoftEncryptVoice(pInput);
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecEnc(pInput,4);
				_AddStealbit(pInput);
			}
			else
			{
				_FecEnc(pInput,0);
			}
			break;
  		case VOICE_F_FRAME:

			//添加DMRA类型的内嵌信息
			_FrmF_Enc_DMRA(pInput);
			
			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)		
				return 0;
				
			_FecDec(pInput,0);
			ret =_SoftEncryptVoice(pInput);
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecEnc(pInput,4);
				_AddStealbit(pInput);
			}
			else
			{
				_FecEnc(pInput,0);
			}
			break;
		case  VOICE_TERM_FRAME:
			memset(&s_Param_Enc,0,sizeof(s_Param_Enc));
			break;
		default:
			break;
	}
	return ret;
}




/************************************** 解密 **************************************************/

/**
 * @brief		 : 获取秘钥信息
 * @param [in ]  : pParam	解密信息
 * @author		 : XYM
 * @return		 : void
 */

static void _InitDecryptParam(ENCDEC_PARAM* pParam)
{
    UINT16 uKeywordLen = 0;
	UINT16 i = 0;
	if(ENCRYPT_HYT_BASE == s_EncryptType)
	{
		pParam->KeyID = s_Keytable[0].KeyID;
		pParam->KeyBitLen = s_Keytable[0].key_length;
		switch(pParam->KeyBitLen)
	    {
	        case 40:
	            pParam->AlgID  = 1;
	            uKeywordLen    = 6;               
	            break;
	        case 128:
	            pParam->AlgID  = 4;
	            uKeywordLen    = 16;                  
	            break;      
	        case 256:
	            pParam->AlgID  = 5;
	            uKeywordLen    = 32;                  
	            break;
	        default:
	            break;
	    }
		memcpy((UINT8*)pParam->KeyValue,(UINT8*)s_Keytable[0].KeyValue,uKeywordLen);
	}
	else
	{
		for(i=0;i<s_Valid_Key_Count;i++)
		{	
			//找到对应的秘钥
			if(pParam->KeyID == s_Keytable[i].KeyID)
			{
				pParam->KeyBitLen = s_Keytable[i].key_length;
				switch(pParam->KeyBitLen)
			    {
			        case 40:
			            pParam->AlgID  = 1;
			            uKeywordLen    = 6;               
			            break;
			        case 128:
			            pParam->AlgID  = 4;
			            uKeywordLen    = 16;                  
			            break;      
			        case 256:
			            pParam->AlgID  = 5;
			            uKeywordLen    = 32;                  
			            break;
			        default:
			            break;
			    }
				
				printf("[_InitDecryptParam] pParam->KeyBitLen=%d \n",pParam->KeyBitLen);
				memcpy((UINT8*)pParam->KeyValue,(UINT8*)s_Keytable[i].KeyValue,uKeywordLen);
				break;
			}
				
		}
	}

#ifdef DEBUG
	pParam->KeyBitLen = 40;
	memcpy((UINT8*)pParam->KeyValue,s_DefaultKey,6);
	pParam->AlgID  = 1;
#endif

}

/**
 * @brief		 : 解密初始化
 * @param [in ]  : pInput	
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _DecryptInit(DLL_ENCLIB_VOICE* pInput)
{
	UINT8 FirstIV[5];
    switch(s_EncryptType)
    {
    	case ENCRYPT_HYT_BASE:
			
			//基本加密 不需要IV信息，只需要秘钥信息即可 BASIC的秘钥ID的获取 需要考虑

            return 0;

			break;
			
        case ENCRYPT_DMRA:
			s_Param_Dec.KeyID  	= pInput->PIInfo[2];
			
            		FirstIV[0] 	= pInput->PIInfo[3];
            		FirstIV[1]	= pInput->PIInfo[4];
            		FirstIV[2]  = pInput->PIInfo[5];
            		FirstIV[3]  = pInput->PIInfo[6];
            if (
                ( 0 != VoiceDecryptInit(VOICE_SLOT) )              || 
                ( s_Param_Dec.KeyID > TABLENUM_30)					||
                ( 0 != VoiceDecryptIV(VOICE_SLOT, (UINT8 *)FirstIV))
               )   
            {
                return -1;
            } 
            _InitDecryptParam(&s_Param_Dec);

            break;

        case ENCRYPT_HYT_ENHA:
            if(_CaculCheckSum(pInput) != pInput->PIInfo[9])
            {
				printf("[_DecryptInit]Enter Later Error! Sum=%d,PIInfo[9]=%d \n",_CaculCheckSum(pInput),pInput->PIInfo[9]);
                return -1;
            }
            else
            {
                s_Param_Dec.KeyID      = pInput->PIInfo[2];

                FirstIV[0] = pInput->PIInfo[3];
                FirstIV[1] = pInput->PIInfo[4];
                FirstIV[2] = pInput->PIInfo[5];
                FirstIV[3] = pInput->PIInfo[6];
                FirstIV[4] = pInput->PIInfo[7];

                if (
                    ( 0 != VoiceDecryptInit(VOICE_SLOT) )              || 
                    ( s_Param_Dec.KeyID > TABLENUM_30)                 ||
                    ( 0 != VoiceDecryptIV(VOICE_SLOT, (UINT8 *)FirstIV)) 
                   )   
                {
                    return -1;
                }
                _InitDecryptParam(&s_Param_Dec);
            }
            break;
			
        default:
            break;
    } 
	return 0;
}

/**
 * @brief		 : 语音解密函数
 * @param [in ]  : pInput	
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _SoftDecryptVoice(DLL_ENCLIB_VOICE* pInput)
{

    UINT16 Cnt20ms = 0;
    UINT16 uEncryptBitLen  = 0;
	UINT16 DataIn[5]		= {0};
	UINT16 DataOut[5]		= {0};
	UINT8 i =0;
	
	//解密初始化失败，不进行解密操作
	if(s_Param_Dec.InitFlag == 0)
	{
		printf("DecryptInit failed! -- InitFlag 0\n");
		return -1 ;
	}
    switch(s_Voicetype)
    {
        case AMBE: 
            uEncryptBitLen  = 49;
            break;
        default  : 
            uEncryptBitLen  = 48; 
            break;
    }

    for(Cnt20ms=0;Cnt20ms<3;Cnt20ms++)
    {
		memcpy((UINT8*)DataIn,pInput->Payload+9*Cnt20ms,9);
		
		for(i=0;i<sizeof(DataIn)/sizeof(DataIn[0]);i++)
		{
			DataIn[i] = htons(DataIn[i]);
		}	
		
        if( VoiceDecrypt(  VOICE_SLOT, 
                           pInput->FrameNum+8, 
                           (UINT16*)DataIn,       //pInput
                           (UINT16*)DataOut,      //pOutput
                           uEncryptBitLen
                          )< 0
          )
        {
        	printf("[VoiceDecrypt] VoiceDecrypt failed!\n");
			return -1;
        }

		for(i=0;i<sizeof(DataOut)/sizeof(DataOut[0]);i++)
		{
			DataOut[i] = htons(DataOut[i]);
		}
		memcpy(pInput->Payload+9*Cnt20ms,DataOut,9);
    }
	return 0;
}

static INT8 _Decrypt_Voice(DLL_ENCLIB_VOICE* pInput)
{
	INT8 ret =0;
	
	switch(pInput->FrameNum)
	{
		case VOICE_LC_FRAME:
			break;
		case VOICE_PI_FRAME:			
			//解密初始化
			ret = _DecryptInit(pInput);
			if(ret == 0)
			{
				s_Param_Dec.InitFlag = 1;
			}
			else
			{
				s_Param_Dec.InitFlag = 0;
			}		
			break;
		case VOICE_A_FRAME:
		
			if(ENCRYPT_HYT_BASE == s_EncryptType)
			{
				if(0 == s_Param_Dec.InitFlag)
				{
					_InitDecryptParam(&s_Param_Dec);
					s_Param_Dec.InitFlag = 1;
				}
			}
			//初始化秘钥
			if(DecryptKeyInit(VOICE_SLOT, (UINT16*)(s_Param_Dec.KeyValue),(UINT16)(s_Param_Dec.KeyBitLen)) < 0)
			{
				printf("DecryptKeyInit failed!\n");
				s_Param_Dec.InitFlag = 0;
				return -1;
			}
			
			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)
				return 0;
		
			//数据上行，解FEC DMRA时 偷比特数是4 HYTERA是0
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecDec(pInput,4);
			}
			else
			{
				_FecDec(pInput,0);//4
			}

			ret = _SoftDecryptVoice(pInput);

			//数据上行，加FEC DMRA HYTERA偷比特数都是0
			_FecEnc(pInput,0);

			break;
		case VOICE_B_FRAME:  
		case VOICE_C_FRAME:
		case VOICE_D_FRAME:
		case VOICE_E_FRAME:	

			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)
				return 0;
				
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecDec(pInput,4);
			}
			else
			{
				_FecDec(pInput,0);
			}

			ret = _SoftDecryptVoice(pInput);
			_FecEnc(pInput,0);
			break;
		case VOICE_F_FRAME:
		
			//如果是补帧数据，退出，不进行语音处理
			if(INVALID == pInput->ValidFlag)
				return 0;
				
			if(ENCRYPT_DMRA == s_EncryptType)
			{
				_FecDec(pInput,4);
			}
			else
			{
				_FecDec(pInput,0);
			}
			ret = _SoftDecryptVoice(pInput);
			_FecEnc(pInput,0);
			break;
		case VOICE_TERM_FRAME:
			memset(&s_Param_Dec,0,sizeof(s_Param_Dec));
			break;
		default:
			break;
	}
	return ret;
}

/**
 * @brief		 : 加载秘钥表
 * @param [in ]  : keypath  秘钥表路径	
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _LoadKeyTable(const CHAR *keypath)
{
	FILE * keyTable_fd = NULL;
	INT16 ret = 0;
	UINT8 i   = 0;
	UINT8 j   = 0;
	ENCRYPT_TABLE f_keytable[TABLENUM_30];//存成文件格式的秘钥
	memset(&f_keytable,0,sizeof(ENCRYPT_TABLE)*TABLENUM_30);

#ifdef DEBUG
	//测试使用
	s_Key_Load_Flag = 1;
	s_Valid_Key_Count = 1;
	return 0;
#endif
	
	if(0 != access(keypath,F_OK) || NULL == keypath)
	{	
		printf("[LoadKeyTable] KeyTable file does not exist or path wrong!\n");
		s_Key_Load_Flag = 0;
		return -1;
	}
	
	keyTable_fd = fopen(keypath,"rb");
	if (NULL == keyTable_fd)
	{
		printf("[LoadKeyTable] Open Key table failed!\n");
		s_Key_Load_Flag = 0;
		return -1;
	}
	
	ret = fread(f_keytable,1,sizeof(ENCRYPT_TABLE)*TABLENUM_30,keyTable_fd);
	if((sizeof(ENCRYPT_TABLE)*TABLENUM_30) != ret)
	{
		printf("[LoadKeyTable] Read table err!\n");
		s_Key_Load_Flag = 0;
		fclose(keyTable_fd);
		return -1;
	}
	fclose(keyTable_fd);
	
	//检索表，统计有效秘钥数量，将有效的秘钥，拷贝到本地
	for(i=0;i<TABLENUM_30;i++)
	{	
		printf("f_keytable[%d].pass_sec=%d\n",i,f_keytable[i].pass_sec);
		if(1 == f_keytable[i].pass_sec)
		{		
			s_Keytable[s_Valid_Key_Count].KeyID = f_keytable[i].keyID;
			if (0x00 == f_keytable[i].key_length) 
			{
				s_Keytable[s_Valid_Key_Count].key_length = 40;
				printf("s_Keytable[s_Valid_Key_Count].key_length=40!\n");
			}	
			else if (0x01 == f_keytable[i].key_length) 
			{
            	s_Keytable[s_Valid_Key_Count].key_length = 128;	
				printf("s_Keytable[s_Valid_Key_Count].key_length=128!\n");
			}
        	else if (0x02 == f_keytable[i].key_length)
        	{
            	s_Keytable[s_Valid_Key_Count].key_length = 256;  
				printf("s_Keytable[s_Valid_Key_Count].key_length=256!\n");
        	}
			else
			{
				printf("LoadKeyTable] There is a wrong key length!\n");
				s_Key_Load_Flag = 0;
				return -1;
			}	
			//大小端
	        for (j = 0; j < 32; j+=2) 
			{
				s_Keytable[s_Valid_Key_Count].KeyValue[j/2] = f_keytable[i].key_value[j] << 8 | f_keytable[i].key_value[j+1];
				//printf("s_Keytable[%d].KeyValue[%d]=%x,f_keytable[i=%d].key_value[j=%d]=%x,f_keytable[i=%d].key_value[j+1=%d]=%x\n",s_Valid_Key_Count,j/2,s_Keytable[s_Valid_Key_Count].KeyValue[j/2],
					//i,j,f_keytable[i].key_value[j],i,j+1,f_keytable[i].key_value[j+1]);
	        }
			s_Valid_Key_Count++;
		}
	}
	printf("s_Valid_Key_Count=%d",s_Valid_Key_Count);
	if(s_Valid_Key_Count > 0)
	{
		s_Key_Load_Flag = 1;
		return 0;
	}
	else
	{
		return -1;
	}
}

/*************************************** 数据加解密 ****************************/

/**
 * @brief		 : 数据加密初始化函数
 * @param [in ]  : pInput  
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _EncryptInit_Data(DLL_ENCLIB_DATA* pInput)
{
	UINT8 FirstIV[5] = {0};
	UINT8 NextIV[5] = {0};			
	memset(&s_Param_Enc_Data,0,sizeof(s_Param_Enc_Data));

	//初始化FirstIV 获取秘钥
	if(_InitEncryptParam(DATA_SLOT,&s_Param_Enc_Data,FirstIV) < 0)
	{
		printf("_InitEncryptParam failed!\n");
		s_Param_Enc_Data.InitFlag = 0;
		return -1;
	}
	else
	{
		s_Param_Enc_Data.InitFlag = 1;
	}
	
	//初始化秘钥信息
	if(EncryptKeyInitandGetNextIV(DATA_SLOT,s_Param_Enc_Data.KeyValue,s_Param_Enc_Data.KeyBitLen,NextIV) < 0)
	{
		printf("EncryptKeyInitandGetNextIV failed!\n ");
		s_Param_Enc_Data.InitFlag = 0;
		return -1;
	}	
	else
	{
		s_Param_Enc_Data.InitFlag = 1;
	}
	
	//返回EHead信息
	if(ENCRYPT_DMRA == s_EncryptType)
	{
		//pInput->EHead[0] = pInput->EHead[0] | 0x0F;//GI,A,HC,POCH转SAP? 含义不同
		pInput->EHead[0] = 0x3F;//GI,A,HC,POCH转SAP? 含义不同
		pInput->EHead[1] = 0x10;
		pInput->EHead[2] = (s_Param_Enc_Data.AlgID << 4) | 0x01;
		pInput->EHead[3] = s_Param_Enc_Data.KeyID;
		pInput->EHead[4] = 0;
		pInput->EHead[5] = 0;
		pInput->EHead[6] = FirstIV[0];
		pInput->EHead[7] = FirstIV[1];
		pInput->EHead[8] = FirstIV[2];
		pInput->EHead[9] = FirstIV[3];
	}
	else if(ENCRYPT_HYT_ENHA == s_EncryptType)
	{
	 	//NVOC语音类型，肯定是PDT产品
	 	
		pInput->EHead[0] = 0x34;
#if 0
		if(NVOC == s_Voicetype)
		{
			//pInput->EHead[0] = pInput->EHead[0] | 0x04;
		}	
		//否则，DMR产品
		else
		{
			//pInput->EHead[0] = pInput->EHead[0] | 0x0F;
		}
#endif
		pInput->EHead[1] = 0x68;
		pInput->EHead[2] = 0x02;//高级加密
		pInput->EHead[3] = s_Param_Enc_Data.KeyID;
		pInput->EHead[4] = FirstIV[0];
		pInput->EHead[5] = FirstIV[1];
		pInput->EHead[6] = FirstIV[2];
		pInput->EHead[7] = FirstIV[3];
		pInput->EHead[8] = FirstIV[4];
		#if 0
			pInput->EHead[4] = 0x17;
			pInput->EHead[5] = 0x29;
			pInput->EHead[6] = 0xc2;
			pInput->EHead[7] = 0x77;
			pInput->EHead[8] = 0xb4;
		#endif
		pInput->EHead[9] = 0;
	}
	else
	{
		pInput->EHead[0] = 0x34;
#if 0
		//NVOC语音类型，肯定是PDT产品
		if(NVOC == s_Voicetype)
		{
			//pInput->EHead[0] = pInput->EHead[0] | 0x04;
			pInput->EHead[0] = 0x34;
		}	
		//否则，DMR产品
		else
		{
			//pInput->EHead[0] = pInput->EHead[0] | 0x0F;
			pInput->EHead[0] = 0x3F;
		}
#endif
		pInput->EHead[1] = 0x68;
		pInput->EHead[2] = 0x01;//基本加密
		pInput->EHead[3] = s_Param_Enc_Data.KeyID;
		pInput->EHead[4] = FirstIV[0];
		pInput->EHead[5] = FirstIV[1];
		pInput->EHead[6] = FirstIV[2];
		pInput->EHead[7] = FirstIV[3];
		pInput->EHead[8] = FirstIV[4];
		pInput->EHead[9] = 0;
	}
	return 0;
}

/**
 * @brief		 : 数据加密函数
 * @param [in ]  : pSrcBuf 源  
 * @param [out]  : pDstBuf 目的
 * @param [In ]  : size	   字节数
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _DataEncryptFun(UINT8* pSrcBuf, UINT8* pDstBuf, UINT16 size)
{
	UINT16 i;
	UINT16 SrcData = 0;
	UINT16 DstData = 0;
	if(	s_Param_Enc_Data.InitFlag == 0)
	{
		printf("Enc Data Init failed!\n");
		return -1;
	}

	for(i=0;i<size;i++)
	{
		SrcData = pSrcBuf[i] << 8;
		if(DataEncrypt(DATA_SLOT,&SrcData,&DstData,8) < 0)
		{
			printf("DataEncrypt failed!\n");
			return -1;
		}
		pSrcBuf[i] = DstData >> 8;	
	}
	
	return 0;

}

/**
 * @brief		 : 数据解密初始化函数
 * @param [in ]  : pInput  
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _DecryptInit_Data(DLL_ENCLIB_DATA* pInput)
{
	UINT8 FirstIV[5] = {0};	
	INT8 ret = 0;
	memset(&s_Param_Dec_Data,0,sizeof(s_Param_Dec_Data));
	
	//EHead信息 获取Key_ID和FirstIV
	if(ENCRYPT_DMRA == s_EncryptType)
	{
		s_Param_Dec_Data.AlgID 	= pInput->EHead[2] >> 4 ;
		s_Param_Dec_Data.KeyID 	= pInput->EHead[3];
		FirstIV[0] 		= pInput->EHead[6];
		FirstIV[1] 		= pInput->EHead[7];
		FirstIV[2] 		= pInput->EHead[8];
		FirstIV[3] 		= pInput->EHead[9];
	}
	else if(ENCRYPT_HYT_ENHA == s_EncryptType)
	{
		 s_Param_Dec_Data.KeyID = pInput->EHead[3];
		 FirstIV[0] 	= pInput->EHead[4];
		 FirstIV[1] 	= pInput->EHead[5];
		 FirstIV[2] 	= pInput->EHead[6];
		 FirstIV[3] 	= pInput->EHead[7];
		 FirstIV[4] 	= pInput->EHead[8];
	}

	
	//初始化FirstIV 获取秘钥
	_InitDecryptParam(&s_Param_Dec_Data);
	
	if(ENCRYPT_HYT_BASE != s_EncryptType)
	{
		//初始化IV
		ret = DataDecryptIV(DATA_SLOT,FirstIV);
		if(ret == 0)
		{
			s_Param_Dec_Data.InitFlag = 1;
		}
		else
		{
			s_Param_Dec_Data.InitFlag = 0;
		}	
	}

	//初始化秘钥信息
	ret = DecryptKeyInit(DATA_SLOT,s_Param_Dec_Data.KeyValue,s_Param_Dec_Data.KeyBitLen);
	if(ret == 0)
	{
		s_Param_Dec_Data.InitFlag = 1;
	}
	else
	{
		s_Param_Dec_Data.InitFlag = 0;
	} 

	return ret;
}

/**
 * @brief		 : 数据解密函数
 * @param [in ]  : pSrcBuf 源  
 * @param [out]  : pDstBuf 目的
 * @param [In ]  : size	   字节数
 * @author		 : XYM
 * @return		 : 成功返回0 失败返回-1
 */
static INT8 _DataDecryptFun(UINT8 *pSrcBuf, UINT8 *pDstBuf, UINT16 size)
{
	UINT16 i;
	UINT16 SrcData = 0;
	UINT16 DstData = 0;
	if(s_Param_Dec_Data.InitFlag == 0)
	{
		printf("_DataDecryptFun failed!\n");
		return -1;
	}
	for(i=0;i<size;i++)
	{
		SrcData = pSrcBuf[i] << 8;
		if(DataDecrypt(DATA_SLOT,&SrcData,&DstData,8) < 0)
		{
			printf("DataEncrypt failed!\n");
			return -1;
		}
		pSrcBuf[i] = DstData >> 8;	
	}
	return 0;
}

/********************************** 外部函数 **************************************/

/**
 * @def   GetLibInfo
 * @brief 打印加密库信息，版本，时隙等
 */
void GetLibInfo(void)
{
	UINT8 version[20]={0};
	memset(version,0,sizeof(version));
	QueryEptLibVersionNum(version);
	printf("**************************************");
	printf("%s\n",version);
	printf("Max SlotNum =%d\n",GetEptLibMaxSlotNum());
	printf("**************************************\n");
}

/**
 * @def      : InitEncLib
 * @brief    : 初始化加密库，开机时调用，对加密库的全部时隙进行初始化
 * @param[1] : In -- 语音类型 
 * @param[2] : In -- 加密类型
 * @param[3] : In -- 秘钥表路径
 * @Out		 : 成功返回:0 失败返回:-1
 */

INT8 InitEncLib(UINT8 VoiceType,UINT8 EncryptType,const CHAR *keypath)
{
	INT8  ReturnVal 	 = 0;
	UINT8 uEncryptBitLen = 0;

	//获取加密类型和语音类型
	s_Voicetype = VoiceType;
	s_EncryptType = EncryptType;
	
	if(AMBE == VoiceType)
		uEncryptBitLen = 147;
	else if(NVOC == VoiceType)
		uEncryptBitLen = 144;
	else
	{
		printf("[%s]VoiceType=%d Wrong!\n",__FUNCTION__,VoiceType);
		return -1;
	}

	//加密类型错误
	if( (ENCRYPT_NONE     != s_EncryptType) &&
		(ENCRYPT_HYT_BASE != s_EncryptType) &&
		(ENCRYPT_HYT_ENHA != s_EncryptType) &&
		(ENCRYPT_DMRA     != s_EncryptType) 
	  )
	{
	  	printf("[%s]Wrong EncryptType=%d!\n",__FUNCTION__,s_EncryptType);
		return -1;
	}

	switch(EncryptType)
	{
		case ENCRYPT_HYT_BASE:
			ReturnVal = EncryptConfigure(D_ENCRYPT_BASIC, 0, uEncryptBitLen, HyteraStandard, 0, NULL);
			break;
		case ENCRYPT_HYT_ENHA:
			ReturnVal = EncryptConfigure(D_ENCRYPT_ENHANCED, 5, uEncryptBitLen, HyteraStandard, 0, NULL);
			break;
		case ENCRYPT_DMRA:
			ReturnVal = EncryptConfigure(D_ENCRYPT_ENHANCED, 4, uEncryptBitLen, DMRStandard, 0, NULL);
			break;
		default:
			ReturnVal = -1;
			break;
	}
	if(ReturnVal == -1)
	{
		return -1;
	}
	
	ReturnVal = _LoadKeyTable(keypath);

	memset(&s_Param_Enc,0,sizeof(s_Param_Enc));
	memset(&s_Param_Dec,0,sizeof(s_Param_Dec));
	memset(&s_Param_Enc_Data,0,sizeof(s_Param_Enc_Data));
	memset(&s_Param_Dec_Data,0,sizeof(s_Param_Dec_Data));
	
	return ReturnVal;		
	
}

/**
 * @def      : EncryptFunc_Data
 
 * @brief    : 数据加密函数，完成数据的加密业务
 			   pInput->Type 是	 TYPE_EHEAD类型时，完成数据加密初始化，由EHead返回加密信息
 			   pInput->Type 是	 TYPE_DATA类型时，需要填充数据长度和完成数据指针初始化
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */

INT8 EncryptFunc_Data(DLL_ENCLIB_DATA* pInput)
{
	INT8 ret = 0;
	switch(pInput->Type)
	{		
		case TYPE_EHEAD:
			
			//初始化加密库
			ret = _EncryptInit_Data(pInput);
			break;
			
		case TYPE_DATA:
		
			//数据加密
			ret = _DataEncryptFun(pInput->Data,pInput->Data,pInput->DataLen);
			break;
			
		default:
			ret = -1;
			break;
		
	}
	return ret;
}

/**
 * @def      : DecryptFunc_Data
 
 * @brief    : 数据解密函数，完成数据的解密业务
 			   pInput->Type 是	 TYPE_EHEAD类型时，由EHead获取加密信息，完成数据解密初始化，
 			   pInput->Type 是	 TYPE_DATA类型时，需要填充数据长度和完成数据指针初始化
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1

 */

INT8 DecryptFunc_Data(DLL_ENCLIB_DATA* pInput)
{
	INT8 ret = 0;
	switch(pInput->Type)
	{
		
		case TYPE_EHEAD:		//商定好 基本加密发什么头
			
			//初始化加密库
			ret = _DecryptInit_Data(pInput);
			break;
			
		case TYPE_DATA:

			//数据加密
			ret = _DataDecryptFun(pInput->Data,pInput->Data,pInput->DataLen);
			break;
			
		default:
			ret = -1;
			break;
		
	}
	return ret;

}
/**
 * @def      : EncryptFunc_Voice
 
 * @brief    : 语音加密函数，完成语音的加密业务
 			   说明1:语音帧类型为LC，pInput->PIInfo返回10字节PI信息
 			   说明2:语音帧类型为A帧，Hytera高级加密时，pInput->PIInfo返回9字节PI信息
 			   说明3:语音数据从pInput->Payload进，从pInput->Payload出
 			   
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */

INT8 EncryptFunc_Voice(DLL_ENCLIB_VOICE* pInput)
{
	//空指针
	if(NULL == pInput)
	{
		printf("NULL Pointer -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	//帧类型错误
	if(pInput->FrameNum > VOICE_TERM_FRAME)
	{
		printf("Wrong FrameNum -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	if(s_Key_Load_Flag == 0)
	{
		printf("EncryptLib UnInit -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	//无错误
	return _Encrypt_Voice(pInput);
}

/**
 * @def      : DecryptFunc_Voice
 
 * @brief    : 语音解密函数，完成语音的解密业务
 			   说明1:收到PI帧或者之后成功收全PI信息后，需要调用此函数
 			   说明2:语音数据从pInput->Payload进，从pInput->Payload出		   
 * @param[1] : In -- pInput 
 * @Out		 : 成功返回:0 失败返回:-1
 */

INT8 DecryptFunc_Voice(DLL_ENCLIB_VOICE* pInput)
{
	//空指针
	if(NULL == pInput)
	{
		printf("NULL Pointer -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	//帧类型错误
	if(!(pInput->FrameNum >= 0 && pInput->FrameNum <= 8))
	{
		printf("Wrong FrameNum -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	if(s_Key_Load_Flag == 0)
	{
		printf("EncryptLib UnInit -- %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;
	}

	//无错误
	return _Decrypt_Voice(pInput);
}



