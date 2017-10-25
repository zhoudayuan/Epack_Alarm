#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>   
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <time.h>
#include <sys/time.h>

#include"EncDec.h"
#include "typedef.h"
#include "NVOC_FEC_Interface.h"
#include "AMBE_FEC_Interface.h"
//#include "AMBE_FEC.h"


#define SIZE_ONE_FRAME		(UINT8)27
#define	SIZE_SUPER_FRAME	(UINT16)(6*SIZE_ONE_FRAME)		//一个超帧
#define BUFFSIZE 			(UINT16)(10*SIZE_SUPER_FRAME)  	//10个超帧
#define KEYPATH				"/loadapp/enctest/encrypt_table"
#define SRCFILE_AMBE		"/loadapp/enctest/AMBEVoiceSource.dat"
#define DSTFILE_AMBE		"/loadapp/enctest/AMBEVoiceDst.dat"

#define SRCFILE_NVOC		"/loadapp/enctest/NVOCVoiceSource.dat"
#define DSTFILE_NVOC		"/loadapp/enctest/NVOCVoiceDst.dat"

#define TMPFILE				"/loadapp/enctest/Tmp.dat"

#define RUNMODE				BUFFSIZE/27     //   BUFFSIZE/27    1

UINT8 srcbuf[BUFFSIZE];
UINT8 dstbuf[BUFFSIZE];

extern UINT8 dmra_voice_tmp[BUFFSIZE];

UINT8 PIInfo[SIZE_PI_10];
extern UINT16 g_aGolayTable[4096] ;
#if 0
static UINT8 LC_DATA[SIZE_LC_9]={0x01,0x02,0x03,
								 0x04,0x05,0x06,
								 0x07,0x08,0x09,
								 };
#endif
static const UINT32 s_au4Gl24MtxH[12] =
{
	0xA4F800, 0xF68400, 0x7B4200, 0x3DA100, 0x1ED080, 0xAB9040,
	0xF13020, 0xDC6010, 0x6E3008, 0x93E004, 0x49F002, 0xC75001
};

//static UINT8 EncryptType = 0;
//static UINT8 VoiceType   = 0;
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
	//printf("tmpword -- ");
	for(i=0;i<28;i+=2)
	{
		tmpword[i/2] = pInput->Payload[i] << 8 | pInput->Payload[i+1];
		//printf("%04X ",tmpword[i/2]);
	}
	//printf("\n");
	vConvertWord2Bit(tmpbit,tmpword,14);
	memcpy(pDst,tmpbit,size);
	
}

static UINT32 golay_24_12_dec(UINT32 u4Data)
{
	UINT32 u4Res        = 0;
	UINT16 u2Idx        = 0;
	UINT16 u2Cnt        = 0;

	for (u2Cnt = 0; u2Cnt < 12; u2Cnt++)
	{
		u4Res  = u4Data & s_au4Gl24MtxH[u2Cnt];
		u4Res ^= u4Res >> 16;
		u4Res ^= u4Res >> 8;
		u4Res ^= u4Res >> 4;
		u4Res ^= u4Res >> 2;
		u4Res ^= u4Res >> 1;

		u2Idx  = (u2Idx << 1) | (u4Res & 0x01);
	}

	u4Data = (u4Data >> 12)^ g_aGolayTable[u2Idx];


	return (u4Data & 0xfff);
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

static UINT8 bptc_72_32_Dec(UINT16* pu2In, UINT16* pu2Out)
{
	UINT32 u4Golay   = 0;
	UINT16 u2Crc4    = 0;
	UINT16 au2Tmp[2] = {0};

	u4Golay   = pu2In[0];
	u4Golay   <<= 8;
	u4Golay   |= ((pu2In[1] >> 8) & 0xff);
	u4Golay   = golay_24_12_dec(u4Golay);
	au2Tmp[0] = (u4Golay << 4) & 0xfff0;

	u4Golay   = pu2In[1] & 0xffff;
	u4Golay   <<= 16;
	u4Golay   |= pu2In[2];
	u4Golay   = golay_24_12_dec(u4Golay);
	au2Tmp[0] = au2Tmp[0] | ((u4Golay >> 8) & 0x0f);
	au2Tmp[1] = (u4Golay << 8) & 0xff00;

	u4Golay   = pu2In[3];
	u4Golay   <<= 8;
	u4Golay   |= ((pu2In[4] >> 8) & 0xff);
	u4Golay   = golay_24_12_dec(u4Golay);
	au2Tmp[1] = au2Tmp[1] | ((u4Golay >> 4) & 0xff);

	u2Crc4    = u4Golay & 0x0f;

	if (u2Crc4 != ALG_Crc4(au2Tmp, 2))
	{
		return FALSE;
	}

	pu2Out[0] = au2Tmp[0];
	pu2Out[1] = au2Tmp[1];

	return TRUE;
}








BOOL _DMRA_GetParam(UINT8 *NEXTIV,UINT8 AlgID)
{
	UINT8 i=0;
	UINT16 SrcBit[216] = {0};
	UINT16 uCnt20ms = 0;
    UINT16 auStealBit[4] = {0};
	UINT16 stealbitvalue[72] = {0};
    UINT16 auTemp1[5]       = {0};
	UINT16 auTemp2[2]       = {0};
	//UINT8  NEXTIV[4]       = {0};
	BOOL   ret=TRUE;
	DLL_ENCLIB_VOICE Input;
	
    auStealBit[3] = 71;
    auStealBit[2] = 67;
    auStealBit[1] = 63;
    auStealBit[0] = 59;

	//完成一个超帧的解FEC
	for(i=0;i<6;i++)
	{
		//转换成字节流
		memset(SrcBit,0,sizeof(SrcBit));
		memcpy(Input.Payload,dmra_voice_tmp+27*i,sizeof(Input.Payload));
		
		//先取偷比特
		_27Byte_2_216Bit(&Input,SrcBit,sizeof(SrcBit));

		for(uCnt20ms=0;uCnt20ms<3;uCnt20ms++)
    	{
        	stealbitvalue[uCnt20ms*24+i*4 + 0] 	= SrcBit[uCnt20ms*72 + auStealBit[3]];
        	stealbitvalue[uCnt20ms*24+i*4 + 1]	= SrcBit[uCnt20ms*72 + auStealBit[2]];
       		stealbitvalue[uCnt20ms*24+i*4 + 2]	= SrcBit[uCnt20ms*72 + auStealBit[1]];
        	stealbitvalue[uCnt20ms*24+i*4 + 3]	= SrcBit[uCnt20ms*72 + auStealBit[0]];
    	}
	}
	#if 0
		printf("Get StealBit -- \n");
		for(i=0;i<72;i++)
		{
			printf("%d ",stealbitvalue[i]);
			if(7 == i%8)
				printf("\n");
		}
		printf("\n");
	#endif
	memset(auTemp1, 0, sizeof(auTemp1));
	vConvertBit2Word(auTemp1, stealbitvalue, 72);
	ret=bptc_72_32_Dec(auTemp1, auTemp2);
	if(ret)
	{
		NEXTIV[0] = auTemp2[0] >>8;
		NEXTIV[1] = auTemp2[0] >>0;
		NEXTIV[2] = auTemp2[1] >>8;
		NEXTIV[3] = auTemp2[1] >>0;
		//printf("NextIV --- %02X %02X %02X %02X \n",NEXTIV[0],NEXTIV[1],NEXTIV[2],NEXTIV[3]);
	}
	if(AlgID==0)//说明非加密，使之不能呼通加密设备
	{
		ret=0;
	}
	return ret;
}
