/******************************************************************************
 * Copyright(C), 1993-2009, HYT
 * File name: interleave.c
 * Author: zhiqi.liu    Version:  0.1           Date: 2009-05-04
 * Description:
 *     BPTC编/解码和(解)交织相关函数实现
 * Others:
 * Function List:
 *
 * History:
 * 1. Date:
 * Author:
 * Modification:
******************************************************************************/
/**
 * @file interleave.c
 * @brief BPTC编/解码和(解)交织相关函数实现
 * @author zhiqi.liu
 * @version 0.1
 * @date 2009-05-04
 */
/******************************************************************************
 *  Include
 * ***************************************************************************/
#include "dll_interleave.h"
#include "string.h"
#include "sap.h"
#include "dll.h"

/******************************************************************************
 * Statical Variable Declaration
 *****************************************************************************/
static const UINT16 s_au2BptcHm13MtxB[16] =
{
    0x0000, 0x0001, 0x0002, 0x0010, 0x0004, 0x0100, 0x0020, 0x0400,
    0x0008, 0x0000, 0x0200, 0x0080, 0x0040, 0x0000, 0x0800, 0x1000
};

static const UINT16 s_au2BptcHm15MtxB[16] =
{
    0x0000, 0x0001, 0x0002, 0x0010, 0x0004, 0x0100, 0x0020, 0x0400,
    0x0008, 0x4000, 0x0200, 0x0080, 0x0040, 0x2000, 0x0800, 0x1000
};

static const UINT16 s_au2FsmTable[8][8] =
{
    { 0x02, 0x0d,  0x0e,  0x01,  0x07,  0x08,  0x0b,  0x04},
    { 0x0e, 0x01,  0x07,  0x08,  0x0b,  0x04,  0x02,  0x0d},
    { 0x0a, 0x05,  0x06,  0x09,  0x0f,  0x00,  0x03,  0x0c},
    { 0x06, 0x09,  0x0f,  0x00,  0x03,  0x0c,  0x0a,  0x05},
    { 0x0f, 0x00,  0x03,  0x0c,  0x0a,  0x05,  0x06,  0x09},
    { 0x03, 0x0c,  0x0a,  0x05,  0x06,  0x09,  0x0f,  0x00},
    { 0x07, 0x08,  0x0b,  0x04,  0x02,  0x0d,  0x0e,  0x01},
    { 0x0b, 0x04,  0x02,  0x0d,  0x0e,  0x01,  0x07,  0x08}
};

/**
 * @var s_au2DmrHeaderRsMask
 * @brief DMR LC_herder Rs mask code
 */
static UINT16 s_au2DmrHeaderRsMask[3] =
{
    0x0096, 0x0096, 0x0096
};
/**
 * @var s_au2DmrHeaderRsMask
 * @brief DMR LC_terminator Rs mask code
 */
static UINT16 s_au2DmrTermtRsMask[3] =
{
    0x0099, 0x0099, 0x0099
};
/**
 * @var s_au2DmrHeaderRsMask
 * @brief PDT LC_herder Rs mask code
 */
static UINT16 s_au2PdtHeaderRsMask[3] =
{
    0x00c6, 0x008a, 0x00a1
};
/**
 * @var s_au2DmrHeaderRsMask
 * @brief PDT LC_herder Rs mask code
 */
static UINT16 s_au2PdtTermtRsMask[3] =
{
    0x00fa, 0x0023, 0x0054
};

static void transpose16(UINT16* pu2Data)
{
    UINT32 u4TemData[8] = {0};
    UINT32 u4Temp       = 0;

    u4TemData[0] = ((UINT32)*(pu2Data + 0) << 16) | (*(pu2Data + 1));
    u4TemData[1] = ((UINT32)*(pu2Data + 2) << 16) | (*(pu2Data + 3));
    u4TemData[2] = ((UINT32)*(pu2Data + 4) << 16) | (*(pu2Data + 5));
    u4TemData[3] = ((UINT32)*(pu2Data + 6) << 16) | (*(pu2Data + 7));
    u4TemData[4] = ((UINT32)*(pu2Data + 8) << 16) | (*(pu2Data + 9));
    u4TemData[5] = ((UINT32)*(pu2Data + 10) << 16) | (*(pu2Data + 11));
    u4TemData[6] = ((UINT32)*(pu2Data + 12) << 16) | (*(pu2Data + 13));
    u4TemData[7] = ((UINT32)*(pu2Data + 14) << 16) | (*(pu2Data + 15));

    u4Temp = (u4TemData[0] ^ (u4TemData[0] >> 15)) & 0xaaaa;
    u4TemData[0] = u4TemData[0] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[1] ^ (u4TemData[1] >> 15)) & 0xaaaa;
    u4TemData[1] = u4TemData[1] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[2] ^ (u4TemData[2] >> 15)) & 0xaaaa;
    u4TemData[2] = u4TemData[2] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[3] ^ (u4TemData[3] >> 15)) & 0xaaaa;
    u4TemData[3] = u4TemData[3] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[4] ^ (u4TemData[4] >> 15)) & 0xaaaa;
    u4TemData[4] = u4TemData[4] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[5] ^ (u4TemData[5] >> 15)) & 0xaaaa;
    u4TemData[5] = u4TemData[5] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[6] ^ (u4TemData[6] >> 15)) & 0xaaaa;
    u4TemData[6] = u4TemData[6] ^ u4Temp ^ (u4Temp << 15);
    u4Temp = (u4TemData[7] ^ (u4TemData[7] >> 15)) & 0xaaaa;
    u4TemData[7] = u4TemData[7] ^ u4Temp ^ (u4Temp << 15);

    u4Temp = (u4TemData[0] ^ (u4TemData[1] >> 2)) & 0x33333333;
    u4TemData[0] ^= u4Temp; u4TemData[1] ^= u4Temp << 2;
    u4Temp = (u4TemData[2] ^ (u4TemData[3] >> 2)) & 0x33333333;
    u4TemData[2] ^= u4Temp; u4TemData[3] ^= u4Temp << 2;
    u4Temp = (u4TemData[4] ^ (u4TemData[5] >> 2)) & 0x33333333;
    u4TemData[4] ^= u4Temp; u4TemData[5] ^= u4Temp << 2;
    u4Temp = (u4TemData[6] ^ (u4TemData[7] >> 2)) & 0x33333333;
    u4TemData[6] ^= u4Temp; u4TemData[7] ^= u4Temp << 2;

    u4Temp = (u4TemData[0] ^ (u4TemData[2] >> 4)) & 0x0f0f0f0f;
    u4TemData[0] ^= u4Temp; u4TemData[2] ^= u4Temp << 4;
    u4Temp = (u4TemData[1] ^ (u4TemData[3] >> 4)) & 0x0f0f0f0f;
    u4TemData[1] ^= u4Temp; u4TemData[3] ^= u4Temp << 4;
    u4Temp = (u4TemData[4] ^ (u4TemData[6] >> 4)) & 0x0f0f0f0f;
    u4TemData[4] ^= u4Temp; u4TemData[6] ^= u4Temp << 4;
    u4Temp = (u4TemData[5] ^ (u4TemData[7] >> 4)) & 0x0f0f0f0f;
    u4TemData[5] ^= u4Temp; u4TemData[7] ^= u4Temp << 4;

    u4Temp = (u4TemData[0] ^ (u4TemData[4] >> 8)) & 0x00ff00ff;
    u4TemData[0] ^= u4Temp;
    u4TemData[4] ^= u4Temp << 8;
    u4Temp = (u4TemData[1] ^ (u4TemData[5] >> 8)) & 0x00ff00ff;
    u4TemData[1] ^= u4Temp;
    u4TemData[5] ^= u4Temp << 8;
    u4Temp = (u4TemData[2] ^ (u4TemData[6] >> 8)) & 0x00ff00ff;
    u4TemData[2] ^= u4Temp;
    u4TemData[6] ^= u4Temp << 8;
    u4Temp = (u4TemData[3] ^ (u4TemData[7] >> 8)) & 0x00ff00ff;
    u4TemData[3] ^= u4Temp;
    u4TemData[7] ^= u4Temp << 8;

    *(pu2Data + 0)  = (UINT16)(u4TemData[0] >> 16);
    *(pu2Data + 1)  = (UINT16) u4TemData[0];
    *(pu2Data + 2)  = (UINT16)(u4TemData[1] >> 16);
    *(pu2Data + 3)  = (UINT16) u4TemData[1];
    *(pu2Data + 4)  = (UINT16)(u4TemData[2] >> 16);
    *(pu2Data + 5)  = (UINT16) u4TemData[2];
    *(pu2Data + 6)  = (UINT16)(u4TemData[3] >> 16);
    *(pu2Data + 7)  = (UINT16) u4TemData[3];
    *(pu2Data + 8)  = (UINT16)(u4TemData[4] >> 16);
    *(pu2Data + 9)  = (UINT16) u4TemData[4];
    *(pu2Data + 10) = (UINT16)(u4TemData[5] >> 16);
    *(pu2Data + 11) = (UINT16) u4TemData[5];
    *(pu2Data + 12) = (UINT16)(u4TemData[6] >> 16);
    *(pu2Data + 13) = (UINT16) u4TemData[6];
    *(pu2Data + 14) = (UINT16)(u4TemData[7] >> 16);
    *(pu2Data + 15) = (UINT16) u4TemData[7];

    return;
}

static void bptc_96_196_enc(UINT16* pu2DataIn, UINT16* pu2DataOut)
{
    UINT16 au2Matrix16[16] = {0};

    au2Matrix16[3]  = (pu2DataIn[0] >> 4);
    au2Matrix16[4]  = (pu2DataIn[0] << 7)  | (pu2DataIn[1] >> 9);
    au2Matrix16[5]  = (pu2DataIn[1] << 2);
    au2Matrix16[6]  = (pu2DataIn[1] << 13) | (pu2DataIn[2] >> 3);
    au2Matrix16[7]  = (pu2DataIn[2] << 8)  | (pu2DataIn[3] >> 8);
    au2Matrix16[8]  = (pu2DataIn[3] << 3);
    au2Matrix16[9]  = (pu2DataIn[3] << 14) | (pu2DataIn[4] >> 2);
    au2Matrix16[10] = (pu2DataIn[4] << 9)  | (pu2DataIn[5] >> 7);
    au2Matrix16[11] = (pu2DataIn[5] << 4);

    transpose16(au2Matrix16);

    au2Matrix16[0]  = 0;
    au2Matrix16[12] = au2Matrix16[1] ^ au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[9];
    au2Matrix16[13] = au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[9] ^ au2Matrix16[10];
    au2Matrix16[14] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[10] ^ au2Matrix16[11];
    au2Matrix16[15] = au2Matrix16[1] ^ au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[8] ^ au2Matrix16[11];

    transpose16(au2Matrix16);

    au2Matrix16[0]  = 0;
    au2Matrix16[1]  = 0;
    au2Matrix16[2]  = 0;
    au2Matrix16[12] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[9];
    au2Matrix16[13] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[9] ^ au2Matrix16[10];
    au2Matrix16[14] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[10] ^ au2Matrix16[11];
    au2Matrix16[15] = au2Matrix16[3] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[8] ^ au2Matrix16[11];

    au2Matrix16[0]  = au2Matrix16[3] >> 3;
    au2Matrix16[1]  = (au2Matrix16[3] << 10) | (au2Matrix16[4] >> 5);
    au2Matrix16[2]  = (au2Matrix16[4] << 8) | (au2Matrix16[5] >> 7);
    au2Matrix16[3]  = (au2Matrix16[5] << 6) | (au2Matrix16[6] >> 9);
    au2Matrix16[4]  = (au2Matrix16[6] << 4) | (au2Matrix16[7] >> 11);
    au2Matrix16[5]  = (au2Matrix16[7] << 2) | (au2Matrix16[8] >> 13);
    au2Matrix16[6]  = au2Matrix16[8];
    au2Matrix16[7]  = au2Matrix16[9] >> 2;
    au2Matrix16[8]  = (au2Matrix16[9] << 11) | (au2Matrix16[10] >> 4);
    au2Matrix16[9]  = (au2Matrix16[10] << 9) | (au2Matrix16[11] >> 6);
    au2Matrix16[10] = (au2Matrix16[11] << 7) | (au2Matrix16[12] >> 8);
    au2Matrix16[11] = (au2Matrix16[12] << 5) | (au2Matrix16[13] >> 10);
    au2Matrix16[12] = (au2Matrix16[13] << 3) | (au2Matrix16[14] >> 12);
    au2Matrix16[13] = (au2Matrix16[14] << 1) | (au2Matrix16[15] >> 14);
    au2Matrix16[14] = au2Matrix16[15] >> 1;
    au2Matrix16[15] = au2Matrix16[15] << 12;

    transpose16(au2Matrix16);

    pu2DataOut[1]  = (pu2DataOut[1] & 0xff00) | (au2Matrix16[3] >> 8);
    pu2DataOut[2]  = (au2Matrix16[3]  << 8)  | (au2Matrix16[15] >> 8);
    pu2DataOut[3]  = (au2Matrix16[15] << 8)  | (au2Matrix16[14] >> 7);
    pu2DataOut[4]  = (au2Matrix16[14] << 9)  | (au2Matrix16[13] >> 6);
    pu2DataOut[5]  = (au2Matrix16[13] << 10) | (au2Matrix16[12] >> 5);
    pu2DataOut[6]  = (au2Matrix16[12] << 11) | (au2Matrix16[11] >> 4);
    pu2DataOut[7]  = (pu2DataOut[7]  & 0x003f) | (au2Matrix16[11] << 12) | ((au2Matrix16[10] >> 3) & 0x1fc0);
    pu2DataOut[11] = (pu2DataOut[11] & 0xfffc) | ((au2Matrix16[10] >> 7) & 0x0003);
    pu2DataOut[12] = (au2Matrix16[10] << 9) | (au2Matrix16[9] >> 6);
    pu2DataOut[13] = (au2Matrix16[9] << 10) | (au2Matrix16[8] >> 5);
    pu2DataOut[14] = (au2Matrix16[8] << 11) | (au2Matrix16[7] >> 4);
    pu2DataOut[15] = (au2Matrix16[7] << 12) | (au2Matrix16[6] >> 3);
    pu2DataOut[16] = (au2Matrix16[6] << 13) | (au2Matrix16[5] >> 2);
    pu2DataOut[17] = (au2Matrix16[5] << 14) | (au2Matrix16[4] >> 1);

    return;
}

static void bptc_96_196_dec(UINT16* pu2DataIn, UINT16* pu2DataOut)
{
    UINT16 u2Cnt           = 0;
    UINT16 au2Matrix16[16] = {0};

    au2Matrix16[3]  = pu2DataIn[0];
    au2Matrix16[4]  = (pu2DataIn[15] << 9)  | ((pu2DataIn[16] >> 7) & 0x01fe);
    au2Matrix16[5]  = (pu2DataIn[14] << 10) | ((pu2DataIn[15] >> 6) & 0x03fe);
    au2Matrix16[6]  = (pu2DataIn[13] << 11) | ((pu2DataIn[14] >> 5) & 0x07fe);
    au2Matrix16[7]  = (pu2DataIn[12] << 12) | ((pu2DataIn[13] >> 4) & 0x0ffe);
    au2Matrix16[8]  = (pu2DataIn[11] << 13) | ((pu2DataIn[12] >> 3) & 0x1ffe);
    au2Matrix16[9]  = (pu2DataIn[10] << 14) | ((pu2DataIn[11] >> 2) & 0x3ffe);
    au2Matrix16[10] = (pu2DataIn[5] << 11) | ((pu2DataIn[6] >> 5) & 0x0600) |
                      ((pu2DataIn[10] >> 1) & 0x01fe);
    au2Matrix16[11] = (pu2DataIn[4] << 12) | ((pu2DataIn[5] >> 4) & 0x0ffe);
    au2Matrix16[12] = (pu2DataIn[3] << 13) | ((pu2DataIn[4] >> 3) & 0x1ffe);
    au2Matrix16[13] = (pu2DataIn[2] << 14) | ((pu2DataIn[3] >> 2) & 0x3ffe);
    au2Matrix16[14] = (pu2DataIn[1] << 15) | ((pu2DataIn[2] >> 1) & 0x7ffe);
    au2Matrix16[15] = pu2DataIn[1] & 0xfffe;

    transpose16(au2Matrix16);

    au2Matrix16[15] = ((au2Matrix16[15] & 0x1000) >> 12) | ((au2Matrix16[14] & 0x1fff) << 1) | (au2Matrix16[13] << 14);
    au2Matrix16[14] = ((au2Matrix16[13] & 0x1fff) >> 1) | (au2Matrix16[12] << 12);
    au2Matrix16[13] = ((au2Matrix16[12] & 0x1fff) >> 3) | (au2Matrix16[11] << 10);
    au2Matrix16[12] = ((au2Matrix16[11] & 0x1fff) >> 5) | (au2Matrix16[10] << 8);
    au2Matrix16[11] = ((au2Matrix16[10] & 0x1fff) >> 7) | (au2Matrix16[9] << 6);
    au2Matrix16[10] = ((au2Matrix16[9] & 0x1fff) >> 9) | (au2Matrix16[8] << 4);
    au2Matrix16[9]  = ((au2Matrix16[8] & 0x1fff) >> 11) | (au2Matrix16[7] << 2);
    au2Matrix16[8]  = (au2Matrix16[6] & 0x1fff) | (au2Matrix16[5] << 13);
    au2Matrix16[7]  = ((au2Matrix16[5] & 0x1fff) >> 2) | (au2Matrix16[4] << 11);
    au2Matrix16[6]  = ((au2Matrix16[4] & 0x1fff) >> 4) | (au2Matrix16[3] << 9);
    au2Matrix16[5]  = ((au2Matrix16[3] & 0x1fff) >> 6) | (au2Matrix16[2] << 7);
    au2Matrix16[4]  = ((au2Matrix16[2] & 0x1fff) >> 8) | (au2Matrix16[1] << 5);
    au2Matrix16[3]  = ((au2Matrix16[1] & 0x1fff) >> 10) | (au2Matrix16[0] << 3);

    //R(0) ~ R(2) 置零
    au2Matrix16[3] &= 0x0fff;

    au2Matrix16[12] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[9]  ^ au2Matrix16[12];
    au2Matrix16[13] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[9]  ^ au2Matrix16[10] ^ au2Matrix16[13];
    au2Matrix16[14] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[6] ^ au2Matrix16[8]  ^ au2Matrix16[10] ^ au2Matrix16[11] ^ au2Matrix16[14];
    au2Matrix16[15] = au2Matrix16[3] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[8] ^ au2Matrix16[11] ^ au2Matrix16[15];

    transpose16(au2Matrix16);

    for (u2Cnt = 1; u2Cnt < 16; u2Cnt++)
    {
        if(0 != s_au2BptcHm13MtxB[au2Matrix16[u2Cnt] & 0x0f])
        {
            au2Matrix16[u2Cnt] ^= s_au2BptcHm13MtxB[au2Matrix16[u2Cnt] & 0x0f];
        }
    }

    //R(0) ~ R(2) 置零
    au2Matrix16[1] &= 0x0ff0;
    au2Matrix16[2] &= 0x0ff0;
    au2Matrix16[3] &= 0x0ff0;

    au2Matrix16[12] = au2Matrix16[1] ^ au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[6] ^ au2Matrix16[8]  ^ au2Matrix16[9]  ^ au2Matrix16[12];
    au2Matrix16[13] = au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[9]  ^ au2Matrix16[10] ^ au2Matrix16[13];
    au2Matrix16[14] = au2Matrix16[3] ^ au2Matrix16[4] ^ au2Matrix16[5] ^ au2Matrix16[6] ^ au2Matrix16[8] ^ au2Matrix16[10] ^ au2Matrix16[11] ^ au2Matrix16[14];
    au2Matrix16[15] = au2Matrix16[1] ^ au2Matrix16[2] ^ au2Matrix16[3] ^ au2Matrix16[5] ^ au2Matrix16[7] ^ au2Matrix16[8]  ^ au2Matrix16[11] ^ au2Matrix16[15];

    transpose16(au2Matrix16);

    for (u2Cnt = 3; u2Cnt < 12; u2Cnt++)
    {
        if(0 != s_au2BptcHm15MtxB[au2Matrix16[u2Cnt] & 0x0f])
        {
            au2Matrix16[u2Cnt] ^= s_au2BptcHm15MtxB[au2Matrix16[u2Cnt] & 0x0f];
        }
    }

    pu2DataOut[0] = ((au2Matrix16[3] << 4) & 0xff00) | ((au2Matrix16[4] >> 7) & 0x00ff);
    pu2DataOut[1] = ((au2Matrix16[4] << 9) & 0xe000) | ((au2Matrix16[5] >> 2) & 0x1ffc) |
                     ((au2Matrix16[6] >> 13) & 0x0003);
    pu2DataOut[2] = ((au2Matrix16[6] << 3) & 0xff80) | ((au2Matrix16[7] >> 8) & 0x007f);
    pu2DataOut[3] = ((au2Matrix16[7] << 8) & 0xf000) | ((au2Matrix16[8] >> 3) & 0x0ffe) |
                     ((au2Matrix16[9] >> 14) & 0x0001);
    pu2DataOut[4] = ((au2Matrix16[9] << 2) & 0xffc0) | ((au2Matrix16[10] >> 9) & 0x003f);
    pu2DataOut[5] = ((au2Matrix16[10] << 7) & 0xf800) | ((au2Matrix16[11] >> 4) & 0x07ff);

    return;
}
/******************************************************************************
 * External Function Implementation
 *****************************************************************************/
void voice_enc(UINT16* pu2Voice, UINT16* pu2AiData)
{
    pu2AiData[1]  = (pu2AiData[1] & 0xff00) | (pu2Voice[0] >> 8);
    pu2AiData[2]  = (pu2Voice[0] << 8) | (pu2Voice[1] >> 8);
    pu2AiData[3]  = (pu2Voice[1] << 8) | (pu2Voice[2] >> 8);
    pu2AiData[4]  = (pu2Voice[2] << 8) | (pu2Voice[3] >> 8);
    pu2AiData[5]  = (pu2Voice[3] << 8) | (pu2Voice[4] >> 8);
    pu2AiData[6]  = (pu2Voice[4] << 8) | (pu2Voice[5] >> 8);
    pu2AiData[7]  = (pu2Voice[5] << 8) | (pu2Voice[6] >> 8);
    pu2AiData[8]  = (pu2AiData[8] & 0x0fff) | ((pu2Voice[6] << 8) & 0xf000);
    pu2AiData[11] = (pu2AiData[11] & 0xf000) | ((pu2Voice[6] << 8) & 0x0f00) | (pu2Voice[7] >> 8);
    pu2AiData[12] = (pu2Voice[7] << 8) | (pu2Voice[8] >> 8);
    pu2AiData[13] = (pu2Voice[8] << 8) | (pu2Voice[9] >> 8);
    pu2AiData[14] = (pu2Voice[9] << 8) | (pu2Voice[10] >> 8);
    pu2AiData[15] = (pu2Voice[10] << 8) | (pu2Voice[11] >> 8);
    pu2AiData[16] = (pu2Voice[11] << 8) | (pu2Voice[12] >> 8);
    pu2AiData[17] = (pu2Voice[12] << 8) | (pu2Voice[13] >> 8);

    return;
}

void voice_dec(UINT16* pu2AiData, UINT16* pu2Voice)
{
    pu2Voice[0]  = pu2AiData[0];
    pu2Voice[1]  = pu2AiData[1];
    pu2Voice[2]  = pu2AiData[2];
    pu2Voice[3]  = pu2AiData[3];
    pu2Voice[4]  = pu2AiData[4];
    pu2Voice[5]  = pu2AiData[5];
    pu2Voice[6]  = (pu2AiData[6] & 0xfff0) | (pu2AiData[9] & 0x000f);
    pu2Voice[7]  = pu2AiData[10];
    pu2Voice[8]  = pu2AiData[11];
    pu2Voice[9]  = pu2AiData[12];
    pu2Voice[10] = pu2AiData[13];
    pu2Voice[11] = pu2AiData[14];
    pu2Voice[12] = pu2AiData[15];
    pu2Voice[13] = (pu2AiData[16] & 0xff00);

    return;
}

void sync_enc(UINT16* pu2Sync, UINT16* pu2AiData)
{
    pu2AiData[8]  = (pu2AiData[8] & 0xf000) | (pu2Sync[0] >> 4);
    pu2AiData[9]  = (pu2Sync[0] << 12) | (pu2Sync[1] >> 4);
    pu2AiData[10] = (pu2Sync[1] << 12) | (pu2Sync[2] >> 4);
    pu2AiData[11] = (pu2Sync[2] << 12) | (pu2AiData[11] & 0x0fff);

    return;
}

void sync_dec(UINT16* pu2AiData, UINT16* pu2Sync)
{
    pu2Sync[0] = (pu2AiData[6] << 12) | (pu2AiData[7] >> 4);
    pu2Sync[1] = (pu2AiData[7] << 12) | (pu2AiData[8] >> 4);
    pu2Sync[2] = (pu2AiData[8] << 12) | (pu2AiData[9] >> 4);

    return;
}

void slottype_enc(UINT16 u2CC, UINT16 u2DataType, UINT16* pu2AiData)
{
    UINT32 u4SlotType = 0;

    u4SlotType = (UINT32)((u2CC << 4) | (u2DataType & 0x0f));
    u4SlotType = golay_20_8_enc(u4SlotType);

    pu2AiData[7]  = (pu2AiData[7] & 0xffc0) | ((u4SlotType >> 14) & 0x003f);
    pu2AiData[8]  = (pu2AiData[8] & 0x0fff) | ((u4SlotType << 2) & 0xf000);
    pu2AiData[11] = (pu2AiData[11] & 0xf003) | ((u4SlotType << 2) & 0x0ffc);

    return;
}

void slottype_dec(UINT16* pu2AiData, UINT16* pu2CC, UINT16* pu2DataType)
{
    UINT32 u4SlotType = 0;

    u4SlotType = (pu2AiData[6] << 2) & 0xffc0;
    u4SlotType = (u4SlotType << 4) | ((pu2AiData[9] << 6) & 0x03c0) | (pu2AiData[10] >> 10);
    u4SlotType = golay_20_8_dec(u4SlotType);

    *pu2CC       = (UINT16)(u4SlotType >> 4);
    *pu2DataType = (UINT16)(u4SlotType & 0x0f);

    return;
}

//小端模式
void pdt_head_fulllc_enc(UINT16* pu2FullLC)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_enc(pu2FullLC, s_au2PdtHeaderRsMask);       //对大端数据

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void pdt_term_fulllc_enc(UINT16* pu2FullLC)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_enc(pu2FullLC, s_au2PdtTermtRsMask);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void dmr_head_fulllc_enc(UINT16* pu2FullLC)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_enc(pu2FullLC, s_au2DmrHeaderRsMask);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void dmr_term_fulllc_enc(UINT16* pu2FullLC)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_enc(pu2FullLC, s_au2DmrTermtRsMask);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void pdt_head_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_dec(pu2FullLC, s_au2PdtHeaderRsMask, pu2Result);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void pdt_term_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_dec(pu2FullLC, s_au2PdtTermtRsMask, pu2Result);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void dmr_head_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_dec(pu2FullLC, s_au2DmrHeaderRsMask, pu2Result);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

//小端模式
void dmr_term_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }

    reed_solomon_12_9_4_dec(pu2FullLC, s_au2DmrTermtRsMask, pu2Result);

    for (i = 0; i < 6; i++)
    {
        *(pu2FullLC+i) = (*(pu2FullLC+i) << 8) | (*(pu2FullLC+i) >> 8);
    }
    return;
}

void common_enc(UINT16* pu2Data, UINT16* pu2AiData)
{
    bptc_96_196_enc(pu2Data, pu2AiData);

    return;
}

void common_dec(UINT16* pu2AiData, UINT16* pu2Data)
{
    bptc_96_196_dec(pu2AiData, pu2Data);

    return;
}

void emb_enc(UINT16 u2EMB, UINT16* pu2AiData)
{
    u2EMB = quadratic_residue_16_7_6_enc(u2EMB);

    pu2AiData[8]  = (pu2AiData[8] & 0xf00f) | ((u2EMB >> 4) & 0x0ff0);
    pu2AiData[10] = (pu2AiData[10] & 0xfff0) | ((u2EMB >> 4) & 0x000f);
    pu2AiData[11] = (pu2AiData[11] & 0x0fff) | ((u2EMB << 12) & 0xf000);

    return;
}

void emb_dec(UINT16* pu2AiData, UINT16* pu2EMB)
{
    UINT16 u2EMB = 0;

    u2EMB = (pu2AiData[6] << 12) | ((pu2AiData[7] >> 4) & 0x0f00) | ((pu2AiData[9] >> 4) & 0x00ff);

    *pu2EMB = (quadratic_residue_16_7_6_dec(u2EMB) >> 9);

    return;
}

void emb_48_enc(UINT16 u2CC, UINT16* u2EMB48, UINT16* pu2AiData)
{
    UINT16 u2EMB = 0;

    u2EMB = ((u2EMB48[0] >> 9) & 0x0007) | ((u2CC << 3) & 0x0078);
    u2EMB = quadratic_residue_16_7_6_enc(u2EMB);
    u2EMB48[0] = (u2EMB48[0] & 0x00ff) | (u2EMB & 0xff00);
    u2EMB48[2] = (u2EMB48[2] & 0xff00) | (u2EMB & 0x00ff);

    sync_enc(u2EMB48, pu2AiData);
}

void emb_48_dec(UINT16* pu2AiData, UINT16* u2EMB48)
{
    sync_dec(pu2AiData, u2EMB48);
}

//小端模式
void EmbeddedEnc(UINT8* puLC, UINT8* puEncodedLC)
{
    UINT16 u2Chksum        = 0;
    UINT16 au2Matrix16[16] = {0};
    UINT16 pu2LC[5] = {0};

    pu2LC[0] = (puLC[0] << 8) | puLC[1];
    pu2LC[1] = (puLC[2] << 8) | puLC[3];
    pu2LC[2] = (puLC[4] << 8) | puLC[5];
    pu2LC[3] = (puLC[6] << 8) | puLC[7];
    pu2LC[4] = puLC[8] << 8;

    u2Chksum += (pu2LC[0] >> 8) + (pu2LC[0] & 0x00ff);
    u2Chksum += (pu2LC[1] >> 8) + (pu2LC[1] & 0x00ff);
    u2Chksum += (pu2LC[2] >> 8) + (pu2LC[2] & 0x00ff);
    u2Chksum += (pu2LC[3] >> 8) + (pu2LC[3] & 0x00ff);
    u2Chksum += pu2LC[4] >> 8;
    u2Chksum  = u2Chksum % 0x1f;

    au2Matrix16[8]  = pu2LC[0] >> 5;
    au2Matrix16[9]  = (pu2LC[0] << 6) | (pu2LC[1] >> 10);
    au2Matrix16[10] = (pu2LC[1] << 1) | ((u2Chksum >> 4) & 0x01);
    au2Matrix16[11] = ((pu2LC[2] >> 5) & 0x07fe) | ((u2Chksum >> 3) & 0x01);
    au2Matrix16[12] = (pu2LC[2] << 5) | ((pu2LC[3] >> 11) & 0x001e) |((u2Chksum >> 2) & 0x01);
    au2Matrix16[13] = ((pu2LC[3] >> 1) & 0x07fe) | ((u2Chksum >> 1) & 0x01);
    au2Matrix16[14] = (pu2LC[3] << 9) | ((pu2LC[4] >> 7) & 0x01fe) |(u2Chksum & 0x01);

    au2Matrix16[8]  = hamming_16_11_4_enc(au2Matrix16[8]);
    au2Matrix16[9]  = hamming_16_11_4_enc(au2Matrix16[9]);
    au2Matrix16[10] = hamming_16_11_4_enc(au2Matrix16[10]);
    au2Matrix16[11] = hamming_16_11_4_enc(au2Matrix16[11]);
    au2Matrix16[12] = hamming_16_11_4_enc(au2Matrix16[12]);
    au2Matrix16[13] = hamming_16_11_4_enc(au2Matrix16[13]);
    au2Matrix16[14] = hamming_16_11_4_enc(au2Matrix16[14]);
    au2Matrix16[15] =
        au2Matrix16[8] ^
        au2Matrix16[9] ^
        au2Matrix16[10]^
        au2Matrix16[11]^
        au2Matrix16[12]^
        au2Matrix16[13]^
        au2Matrix16[14];

    transpose16(au2Matrix16);

    puEncodedLC[0]  = au2Matrix16[0] & 0xff;
    puEncodedLC[1]  = au2Matrix16[1] & 0xff;
    puEncodedLC[2]  = au2Matrix16[2] & 0xff;
    puEncodedLC[3]  = au2Matrix16[3] & 0xff;

    puEncodedLC[4]  = au2Matrix16[4] & 0xff;
    puEncodedLC[5]  = au2Matrix16[5] & 0xff;
    puEncodedLC[6]  = au2Matrix16[6] & 0xff;
    puEncodedLC[7] = au2Matrix16[7] & 0xff;

    puEncodedLC[8] = au2Matrix16[8] & 0xff;
    puEncodedLC[9] = au2Matrix16[9] & 0xff;
    puEncodedLC[10] = au2Matrix16[10] & 0xff;
    puEncodedLC[11] = au2Matrix16[11] & 0xff;

    puEncodedLC[12] = au2Matrix16[12] & 0xff;
    puEncodedLC[13] = au2Matrix16[13] & 0xff;
    puEncodedLC[14] = au2Matrix16[14] & 0xff;
    puEncodedLC[15] = au2Matrix16[15] & 0xff;
    return;
}

//小端模式
void EmbeddedDec(UINT8* puEncodedLC, UINT8* puLC, UINT16 * pu2Result)
{
    UINT16 u2Cnt           = 0;
    UINT16 u2Chksum        = 0;
    UINT16 au2Matrix16[16] = {0};

    *pu2Result = NO_ERR;

    for (UINT16 i = 0; i < 16; i++)
    {
        au2Matrix16[i] = puEncodedLC[i];
    }

    transpose16(au2Matrix16);

    u2Chksum = 0;
    for (u2Cnt = 8; u2Cnt < 15; u2Cnt++)
    {
        au2Matrix16[u2Cnt] = hamming_16_11_4_dec(au2Matrix16[u2Cnt]);
        u2Chksum ^= au2Matrix16[u2Cnt];
    }
    au2Matrix16[15] = hamming_16_11_4_dec(au2Matrix16[15]);

    if (u2Chksum != au2Matrix16[15])
    {
        // 内嵌校验和错误
        *pu2Result |= 0x01;
        return;
    }

    au2Matrix16[0] = au2Matrix16[8] >> 8;
    au2Matrix16[1] = (au2Matrix16[8] & 0xe0) | (au2Matrix16[9] >> 11);
    au2Matrix16[2] = ((au2Matrix16[9] >> 3) & 0xfc) | (au2Matrix16[10] >> 14);
    au2Matrix16[3] = au2Matrix16[10] >> 6;
    au2Matrix16[4] = au2Matrix16[11] >> 8;
    au2Matrix16[5] = (au2Matrix16[11] & 0xc0) | (au2Matrix16[12] >> 10);
    au2Matrix16[6] = ((au2Matrix16[12] >> 2) & 0xf0) | (au2Matrix16[13] >> 12);
    au2Matrix16[7] = ((au2Matrix16[13] >> 4) & 0xfc) | (au2Matrix16[14] >> 14);
    au2Matrix16[8] = au2Matrix16[14] >> 6;

    u2Chksum = 0;

    for (u2Cnt = 0; u2Cnt < 9; u2Cnt++)
    {
        u2Chksum += au2Matrix16[u2Cnt] & 0xff;
    }
    u2Chksum = u2Chksum % 0x1f;

    if (u2Chksum != (((au2Matrix16[10] >> 1) & 0x10) | ((au2Matrix16[11] >> 2) & 0x8) | ((au2Matrix16[12] >> 3) & 0x4) |
        ((au2Matrix16[13] >> 4) & 0x2) | ((au2Matrix16[14] >> 5) & 0x1)))
    {
        // 内嵌校验和错误
        *pu2Result |= 0x02;
        return;
    }

    for (UINT16 i = 0; i < 9; i++)
    {
        puLC[i] = au2Matrix16[i]&0xff;
    }

    return;
}


void zero_lcfrag_enc(UINT16* pu2AiData)
{
    pu2AiData[8]  = (pu2AiData[8] & 0xfff0);
    pu2AiData[9]  = 0;
    pu2AiData[10] = (pu2AiData[10] & 0x000f);

    return;
}

void embedded_lcfrag_enc(UINT16* pu2EncodedLC, UINT16* pu2AiData)
{
    pu2AiData[8]  = (pu2AiData[8] & 0xfff0) | (pu2EncodedLC[0] >> 12);
    pu2AiData[9]  = (pu2EncodedLC[0] << 4) | (pu2EncodedLC[1] >> 12);
    pu2AiData[10] = (pu2EncodedLC[1] << 4) | (pu2AiData[10] & 0x000f);

    return;
}

void embedded_lcfrag_dec(UINT16* pu2AiData, UINT16* pu2EncodedLC)
{
    pu2EncodedLC[0] = (pu2AiData[7] << 4) | (pu2AiData[8] >> 12);
    pu2EncodedLC[1] = (pu2AiData[8] << 4) | (pu2AiData[9] >> 12);

    return;
}

void cach_enc(UINT16* pu2ShortLC, UINT32* pu4CACH)
{
    UINT32 u4TempData[4] = {0};
    UINT32 u4TempMask    = 0;

    // 大小端转换
    u4TempData[0] = (pu2ShortLC[0] << 8) | (pu2ShortLC[0] >> 8);
    u4TempData[1] = (pu2ShortLC[1] << 4) | (pu2ShortLC[1] >> 12);
    u4TempData[2] = (pu2ShortLC[1] & 0x0f00) | (pu2ShortLC[2] >> 8);

    u4TempData[0]  = hamming_17_12_3_enc((UINT16)u4TempData[0]);
    u4TempData[1]  = hamming_17_12_3_enc((UINT16)u4TempData[1]);
    u4TempData[2]  = hamming_17_12_3_enc((UINT16)u4TempData[2]);
    u4TempData[3]  = u4TempData[0] ^ u4TempData[1] ^ u4TempData[2];

    u4TempData[0]  = u4TempData[0] << 7;
    u4TempData[1]  = u4TempData[1] << 7;
    u4TempData[2]  = u4TempData[2] << 7;
    u4TempData[3]  = u4TempData[3] << 7;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[1] >> 1)) & 0x555555;
    u4TempData[0] ^= u4TempMask;
    u4TempData[1] ^= u4TempMask << 1;
    u4TempMask     = (u4TempData[2] ^ (u4TempData[3] >> 1)) & 0x555555;
    u4TempData[2] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 1;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[2] >> 2)) & 0x333333;
    u4TempData[0] ^= u4TempMask;
    u4TempData[2] ^= u4TempMask << 2;
    u4TempMask     = (u4TempData[1] ^ (u4TempData[3] >> 2)) & 0x333333;
    u4TempData[1] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 2;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[1] >> 4)) & 0x0F0F0F;
    u4TempData[0] ^= u4TempMask;
    u4TempData[1] ^= u4TempMask << 4;
    u4TempMask     = (u4TempData[2] ^ (u4TempData[3] >> 4)) & 0x0F0F0F;
    u4TempData[2] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 4;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[2] >> 8)) & 0x00FF00;
    u4TempData[0] ^= u4TempMask;
    u4TempData[2] ^= u4TempMask << 8;
    u4TempMask     = (u4TempData[1] ^ (u4TempData[3] >> 8)) & 0x00FF00;
    u4TempData[1] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 8;

    pu4CACH[0]   = ((u4TempData[0] >> 7) & 0x1FFFE) | ((u4TempData[1] >> 23) & 0x01);
    pu4CACH[1]   = ((u4TempData[1] >> 6) & 0x1FFFC) | ((u4TempData[2] >> 22) & 0x03);
    pu4CACH[2]   = ((u4TempData[2] >> 5) & 0x1FFF8) | ((u4TempData[3] >> 21) & 0x07);
    pu4CACH[3]   = ((u4TempData[3] >> 4) & 0x1FFF0) | ((u4TempData[0] >> 4)  & 0x0F);

    return;
}

void cach_dec(UINT32* pu4CACH, UINT16* pu2ShortLC, UINT16* pu2Result)
{
    UINT32 u4TempData[4] = {0};
    UINT32 u4TempMask    = 0;

    u4TempData[0]  = ((pu4CACH[0] <<  7) & 0x00ffff00) | ((pu4CACH[3] << 4) & 0x000000f0);
    u4TempData[1]  = ((pu4CACH[0] << 23) & 0x00800000) | ((pu4CACH[1] << 6) & 0x007fff00);
    u4TempData[2]  = ((pu4CACH[1] << 22) & 0x00C00000) | ((pu4CACH[2] << 5) & 0x003fff00);
    u4TempData[3]  = ((pu4CACH[2] << 21) & 0x00E00000) | ((pu4CACH[3] << 4) & 0x001fff00);

    u4TempMask     = (u4TempData[0] ^ (u4TempData[2] >> 8)) & 0x00FF00;
    u4TempData[0] ^= u4TempMask;
    u4TempData[2] ^= u4TempMask << 8;
    u4TempMask     = (u4TempData[1] ^ (u4TempData[3] >> 8)) & 0x00FF00;
    u4TempData[1] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 8;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[1] >> 4)) & 0x0F0F0F;
    u4TempData[0] ^= u4TempMask;
    u4TempData[1] ^= u4TempMask << 4;
    u4TempMask     = (u4TempData[2] ^ (u4TempData[3] >> 4)) & 0x0F0F0F;
    u4TempData[2] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 4;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[2] >> 2)) & 0x333333;
    u4TempData[0] ^= u4TempMask;
    u4TempData[2] ^= u4TempMask << 2;
    u4TempMask     = (u4TempData[1] ^ (u4TempData[3] >> 2)) & 0x333333;
    u4TempData[1] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 2;

    u4TempMask     = (u4TempData[0] ^ (u4TempData[1] >> 1)) & 0x555555;
    u4TempData[0] ^= u4TempMask;
    u4TempData[1] ^= u4TempMask << 1;
    u4TempMask     = (u4TempData[2] ^ (u4TempData[3] >> 1)) & 0x555555;
    u4TempData[2] ^= u4TempMask;
    u4TempData[3] ^= u4TempMask << 1;

    u4TempData[0]  = u4TempData[0] >> 7;
    u4TempData[1]  = u4TempData[1] >> 7;
    u4TempData[2]  = u4TempData[2] >> 7;
    u4TempData[3]  = u4TempData[3] >> 7;

    u4TempMask     = u4TempData[0] ^ u4TempData[1] ^ u4TempData[2];

    if (u4TempMask != u4TempData[3])
    {
        *pu2Result |= ERR;
        return;
    }

    u4TempData[0] = hamming_17_12_3_dec(u4TempData[0]);
    u4TempData[1] = hamming_17_12_3_dec(u4TempData[1]);
    u4TempData[2] = hamming_17_12_3_dec(u4TempData[2]);

    u4TempData[0]  = ((u4TempData[0] << 11) & 0x0FFF0000) | ((u4TempData[1] >> 1) & 0x0000FFF0) | ((u4TempData[2] >> 13) & 0x0000000F);
    u4TempData[2]  = (u4TempData[2] >> 5) & 0xff;
    u4TempMask     = (UINT32)crc8((UINT16*)u4TempData, 2);

    if (u4TempData[2] != u4TempMask)
    {
        *pu2Result |= ERR;
        return;
    }

    pu2ShortLC[0] = u4TempData[0] >> 16;
    pu2ShortLC[1] = u4TempData[1] & 0xffff;

    return;
}

void cach_frag_enc(UINT16 u2TACT, UINT32* pu4CACH, UINT16* pu2AiData)
{
    UINT32 u4TactFec;
    UINT32 u4Temp;

    u4TactFec = (UINT32)hamming_7_4_3_enc(u2TACT);

    u4Temp = ((u4TactFec << 17) & 0x00800000) |
             ((pu4CACH[0] << 6) & 0x00400000) |
             ((pu4CACH[0] << 6) & 0x00200000) |
             ((pu4CACH[0] << 6) & 0x00100000) |
             ((u4TactFec << 14) & 0x00080000) |
             ((pu4CACH[0] << 5) & 0x00040000) |
             ((pu4CACH[0] << 5) & 0x00020000) |
             ((pu4CACH[0] << 5) & 0x00010000) |
             ((u4TactFec << 11) & 0x00008000) |
             ((pu4CACH[0] << 4) & 0x00004000) |
             ((pu4CACH[0] << 4) & 0x00002000) |
             ((pu4CACH[0] << 4) & 0x00001000) |
             ((u4TactFec <<  8) & 0x00000800) |
             ((pu4CACH[0] << 3) & 0x00000400) |
             ((u4TactFec <<  7) & 0x00000200) |
             ((pu4CACH[0] << 2) & 0x00000100) |
             ((pu4CACH[0] << 2) & 0x00000080) |
             ((pu4CACH[0] << 2) & 0x00000040) |
             ((u4TactFec <<  4) & 0x00000020) |
             ((pu4CACH[0] << 1) & 0x00000010) |
             ((pu4CACH[0] << 1) & 0x00000008) |
             ((pu4CACH[0] << 1) & 0x00000004) |
             ((u4TactFec <<  1) & 0x00000002) |
             (pu4CACH[0] & 0x00000001);

    pu2AiData[0] = (u4Temp >> 8) & 0xffff;
    pu2AiData[1] = (pu2AiData[1] & 0x00ff) | ((u4Temp << 8) & 0xff00);

    return;
}

void cach_frag_dec(UINT16* pu2AiData, UINT16* pu2TACT, UINT32* pu4CACH)
{
    UINT16 u2TactFec;
    UINT32 u4Temp;

    u4Temp = (pu2AiData[0] << 8) | ((pu2AiData[1] >> 8) & 0x00ff);

    u2TactFec = ((u4Temp >> 17) & 0x00000040) |
                ((u4Temp >> 14) & 0x00000020) |
                ((u4Temp >> 11) & 0x00000010) |
                ((u4Temp >> 8) & 0x00000008) |
                ((u4Temp >> 7) & 0x00000004) |
                ((u4Temp >> 4) & 0x00000002) |
                ((u4Temp >> 1) & 0x00000001);

    *pu4CACH = ((u4Temp >> 6) & 0x00010000) |
               ((u4Temp >> 6) & 0x00008000) |
               ((u4Temp >> 6) & 0x00004000) |
               ((u4Temp >> 5) & 0x00002000) |
               ((u4Temp >> 5) & 0x00001000) |
               ((u4Temp >> 5) & 0x00000800) |
               ((u4Temp >> 4) & 0x00000400) |
               ((u4Temp >> 4) & 0x00000200) |
               ((u4Temp >> 4) & 0x00000100) |
               ((u4Temp >> 3) & 0x00000080) |
               ((u4Temp >> 2) & 0x00000040) |
               ((u4Temp >> 2) & 0x00000020) |
               ((u4Temp >> 2) & 0x00000010) |
               ((u4Temp >> 1) & 0x00000008) |
               ((u4Temp >> 1) & 0x00000004) |
               ((u4Temp >> 1) & 0x00000002) |
               (u4Temp & 0x00000001);

    u2TactFec = hamming_7_4_3_dec(u2TactFec);

    *pu2TACT  = (u2TactFec >> 3) & 0x000f;

    return;
}

void rc_enc(UINT16* pu2RC, UINT16* pu2AiData)
{
    UINT16 u2Temp;
    UINT16 u2Mask;

    u2Temp = hamming_16_11_4_enc(*pu2RC);

    u2Mask = ((u2Temp >> 4) ^ u2Temp) & 0x00f0;
    u2Temp = u2Temp ^ u2Mask ^ (u2Mask << 4);

    u2Mask = ((u2Temp >> 2) ^ u2Temp) & 0x0c0c;
    u2Temp = u2Temp ^ u2Mask ^ (u2Mask << 2);

    u2Mask = ((u2Temp >> 1) ^ u2Temp) & 0x2222;
    u2Temp = u2Temp ^ u2Mask ^ (u2Mask << 1);

    pu2AiData[8]  = (pu2AiData[8] & 0xfff0) | (u2Temp >> 12);
    pu2AiData[9]  = (u2Temp << 4);

    u2Mask = ((u2Temp >> 1) ^ u2Temp) & 0x5555;
    u2Temp = u2Temp ^ u2Mask ^ (u2Mask << 1);


    pu2AiData[9]  = pu2AiData[9] | (u2Temp >> 12);
    pu2AiData[10] = (pu2AiData[10] & 0x000f) | (u2Temp << 4);

    return;
}

void rc_dec(UINT16* pu2AiData, UINT16* pu2RC, UINT16* pu2Result)
{
    UINT32 u4Temp;
    UINT32 u4Mask;
    UINT16 au2RcData[2];

    u4Temp = (pu2AiData[7] << 4);
    u4Temp = (u4Temp << 12) | pu2AiData[8];
    u4Temp = (u4Temp << 4) | (pu2AiData[9] >> 12);

    u4Mask = ((u4Temp >> 1) ^ u4Temp) & 0x22222222;
    u4Temp = u4Temp ^ u4Mask ^ (u4Mask << 1);

    u4Mask = ((u4Temp >> 2) ^ u4Temp) & 0x0c0c0c0c;
    u4Temp = u4Temp ^ u4Mask ^ (u4Mask << 2);

    u4Mask = ((u4Temp >> 4) ^ u4Temp) & 0x00f000f0;
    u4Temp = u4Temp ^ u4Mask ^ (u4Mask << 4);

    u4Mask = ((u4Temp >> 8) ^ u4Temp) & 0x0000ff00;
    u4Temp = u4Temp ^ u4Mask ^ (u4Mask << 8);

    u4Mask = ((u4Temp >> 8) ^ u4Temp) & 0x000000ff;
    u4Temp = u4Temp ^ u4Mask ^ (u4Mask << 8);

    au2RcData[0] = hamming_16_11_4_dec((UINT16)(u4Temp >> 16));
    au2RcData[1] = hamming_16_11_4_dec((UINT16)(u4Temp & 0xffff));

    if (au2RcData[0] != au2RcData[1])
    {
        *pu2Result |= ERR;
        return;
    }

    *pu2RC = ((au2RcData[0] >> 5) & 0x07ff);

    return;
}


//大端模式
void bptc_144_196_enc(UINT16* pu2Data, UINT16* pu2AiData)
{
    UINT16 u2Cnt       = 0;
    UINT16 u2FsmState  = 0;
    UINT16 u2Tribit    = 0;
    UINT16 u2Dibit[49] = {0};

    u2Dibit[0]  = pu2Data[0] >> 13;
    u2Dibit[1]  = pu2Data[0] >> 10;
    u2Dibit[2]  = pu2Data[0] >> 7;
    u2Dibit[3]  = pu2Data[0] >> 4;
    u2Dibit[4]  = pu2Data[0] >> 1;
    u2Dibit[5]  = (pu2Data[0] << 2) | (pu2Data[1] >> 14);
    u2Dibit[6]  = pu2Data[1] >> 11;
    u2Dibit[7]  = pu2Data[1] >> 8;
    u2Dibit[8]  = pu2Data[1] >> 5;
    u2Dibit[9]  = pu2Data[1] >> 2;
    u2Dibit[10] = (pu2Data[1] << 1) | (pu2Data[2] >> 15);
    u2Dibit[11] = pu2Data[2] >> 12;
    u2Dibit[12] = pu2Data[2] >> 9;
    u2Dibit[13] = pu2Data[2] >> 6;
    u2Dibit[14] = pu2Data[2] >> 3;
    u2Dibit[15] = pu2Data[2];

    u2Dibit[16] = pu2Data[3] >> 13;
    u2Dibit[17] = pu2Data[3] >> 10;
    u2Dibit[18] = pu2Data[3] >> 7;
    u2Dibit[19] = pu2Data[3] >> 4;
    u2Dibit[20] = pu2Data[3] >> 1;
    u2Dibit[21] = (pu2Data[3] << 2) | (pu2Data[4] >> 14);
    u2Dibit[22] = pu2Data[4] >> 11;
    u2Dibit[23] = pu2Data[4] >> 8;
    u2Dibit[24] = pu2Data[4] >> 5;
    u2Dibit[25] = pu2Data[4] >> 2;
    u2Dibit[26] = (pu2Data[4] << 1) | (pu2Data[5] >> 15);
    u2Dibit[27] = pu2Data[5] >> 12;
    u2Dibit[28] = pu2Data[5] >> 9;
    u2Dibit[29] = pu2Data[5] >> 6;
    u2Dibit[30] = pu2Data[5] >> 3;
    u2Dibit[31] = pu2Data[5];

    u2Dibit[32] = pu2Data[6] >> 13;
    u2Dibit[33] = pu2Data[6] >> 10;
    u2Dibit[34] = pu2Data[6] >> 7;
    u2Dibit[35] = pu2Data[6] >> 4;
    u2Dibit[36] = pu2Data[6] >> 1;
    u2Dibit[37] = (pu2Data[6] << 2) | (pu2Data[7] >> 14);
    u2Dibit[38] = pu2Data[7] >> 11;
    u2Dibit[39] = pu2Data[7] >> 8;
    u2Dibit[40] = pu2Data[7] >> 5;
    u2Dibit[41] = pu2Data[7] >> 2;
    u2Dibit[42] = (pu2Data[7] << 1) | (pu2Data[8] >> 15);
    u2Dibit[43] = pu2Data[8] >> 12;
    u2Dibit[44] = pu2Data[8] >> 9;
    u2Dibit[45] = pu2Data[8] >> 6;
    u2Dibit[46] = pu2Data[8] >> 3;
    u2Dibit[47] = pu2Data[8];
    u2Dibit[48] = 0x0;

    u2FsmState = 0;

    for (u2Cnt=0; u2Cnt<49; u2Cnt++)
    {
        u2Tribit       = u2Dibit[u2Cnt] & 0x07;
        u2Dibit[u2Cnt] = s_au2FsmTable[u2FsmState][u2Tribit];
        u2FsmState     = u2Tribit;
    }

    pu2AiData[1]  = (pu2AiData[1] & 0xff00) | (u2Dibit[0] << 4) | u2Dibit[4];
    pu2AiData[2]  = (u2Dibit[8] << 12) | (u2Dibit[12] << 8)| (u2Dibit[16] << 4) | u2Dibit[20];
    pu2AiData[3]  = (u2Dibit[24] << 12) | (u2Dibit[28] << 8) | (u2Dibit[32] << 4) | u2Dibit[36];
    pu2AiData[4]  = (u2Dibit[40] << 12) | (u2Dibit[44] << 8) | (u2Dibit[48] << 4) | u2Dibit[1];
    pu2AiData[5]  = (u2Dibit[5] << 12) | (u2Dibit[9] << 8) | (u2Dibit[13] << 4) | u2Dibit[17];
    pu2AiData[6]  = (u2Dibit[21] << 12) | (u2Dibit[25] << 8) | (u2Dibit[29] << 4) | u2Dibit[33];
    pu2AiData[7]  = (pu2AiData[7] & 0x003f) | (u2Dibit[37] << 12) | (u2Dibit[41] << 8) | ((u2Dibit[45] << 4) & 0xc0);
    pu2AiData[11] = (pu2AiData[11] & 0xfffc) | (u2Dibit[45] & 0x03);
    pu2AiData[12] = (u2Dibit[2] << 12) | (u2Dibit[6] << 8) | (u2Dibit[10] << 4) | u2Dibit[14];
    pu2AiData[13] = (u2Dibit[18] << 12) | (u2Dibit[22] << 8) | (u2Dibit[26] << 4) | u2Dibit[30];
    pu2AiData[14] = (u2Dibit[34] << 12) | (u2Dibit[38] << 8) | (u2Dibit[42] << 4) | u2Dibit[46];
    pu2AiData[15] = (u2Dibit[3] << 12) | (u2Dibit[7] << 8) | (u2Dibit[11] << 4) | u2Dibit[15];
    pu2AiData[16] = (u2Dibit[19] << 12) | (u2Dibit[23] << 8) | (u2Dibit[27] << 4) | u2Dibit[31];
    pu2AiData[17] = (u2Dibit[35] << 12) | (u2Dibit[39] << 8) | (u2Dibit[43] << 4) | u2Dibit[47];
}

//大端模式
void bptc_144_196_dec(UINT16* pu2AiData, UINT16* pu2Data)
{
    //UINT16 u2CntA       = 0;
    //UINT16 u2CntB       = 0;
    //UINT16 u2FsmState   = 0;
    UINT16 u2Tribit[49] = {0};

    u2Tribit[0]  = pu2AiData[0] >> 12;
    u2Tribit[4]  = pu2AiData[0] >> 8;
    u2Tribit[8]  = pu2AiData[0] >> 4;
    u2Tribit[12] = pu2AiData[0];
    u2Tribit[16] = pu2AiData[1] >> 12;
    u2Tribit[20] = pu2AiData[1] >> 8;
    u2Tribit[24] = pu2AiData[1] >> 4;
    u2Tribit[28] = pu2AiData[1];
    u2Tribit[32] = pu2AiData[2] >> 12;
    u2Tribit[36] = pu2AiData[2] >> 8;
    u2Tribit[40] = pu2AiData[2] >> 4;
    u2Tribit[44] = pu2AiData[2];
    u2Tribit[48] = pu2AiData[3] >> 12;
    u2Tribit[1]  = pu2AiData[3] >> 8;
    u2Tribit[5]  = pu2AiData[3] >> 4;
    u2Tribit[9]  = pu2AiData[3];
    u2Tribit[13] = pu2AiData[4] >> 12;
    u2Tribit[17] = pu2AiData[4] >> 8;
    u2Tribit[21] = pu2AiData[4] >> 4;
    u2Tribit[25] = pu2AiData[4];
    u2Tribit[29] = pu2AiData[5] >> 12;
    u2Tribit[33] = pu2AiData[5] >> 8;
    u2Tribit[37] = pu2AiData[5] >> 4;
    u2Tribit[41] = pu2AiData[5];
    u2Tribit[45] = ((pu2AiData[6] >> 12) & 0x0c) | ((pu2AiData[10] >> 8) & 0x03);
    u2Tribit[2]  = pu2AiData[10] >> 4;
    u2Tribit[6]  = pu2AiData[10];
    u2Tribit[10] = pu2AiData[11] >> 12;
    u2Tribit[14] = pu2AiData[11] >> 8;
    u2Tribit[18] = pu2AiData[11] >> 4;
    u2Tribit[22] = pu2AiData[11];
    u2Tribit[26] = pu2AiData[12] >> 12;
    u2Tribit[30] = pu2AiData[12] >> 8;
    u2Tribit[34] = pu2AiData[12] >> 4;
    u2Tribit[38] = pu2AiData[12];
    u2Tribit[42] = pu2AiData[13] >> 12;
    u2Tribit[46] = pu2AiData[13] >> 8;
    u2Tribit[3]  = pu2AiData[13] >> 4;
    u2Tribit[7]  = pu2AiData[13];
    u2Tribit[11] = pu2AiData[14] >> 12;
    u2Tribit[15] = pu2AiData[14] >> 8;
    u2Tribit[19] = pu2AiData[14] >> 4;
    u2Tribit[23] = pu2AiData[14];
    u2Tribit[27] = pu2AiData[15] >> 12;
    u2Tribit[31] = pu2AiData[15] >> 8;
    u2Tribit[35] = pu2AiData[15] >> 4;
    u2Tribit[39] = pu2AiData[15];
    u2Tribit[43] = pu2AiData[16] >> 12;
    u2Tribit[47] = pu2AiData[16] >> 8;

#if 0
    for (u2CntA=48; u2CntA>0; u2CntA--)
    {
        u2Tribit[u2CntA] = u2Tribit[u2CntA] & 0x0f;

        for (u2CntB=0; u2CntB<8; u2CntB++)
        {
            if (u2Tribit[u2CntA] == s_au2FsmTable[u2CntB][u2FsmState])
            {
                u2FsmState = u2CntB;
                u2Tribit[u2CntA] = u2CntB;
                break;
            }
        }
    }

    pu2Data[0]  = (u2Tribit[1] << 13) | (u2Tribit[2] << 10) | (u2Tribit[3] << 7) |
                  (u2Tribit[4] << 4) | (u2Tribit[5] << 1) | (u2Tribit[6] >> 2);
    pu2Data[1]  = (u2Tribit[6] << 14) | (u2Tribit[7] << 11) | (u2Tribit[8] << 8) |
                  (u2Tribit[9] << 5) | (u2Tribit[10] << 2) | (u2Tribit[11] >> 1);
    pu2Data[2]  = (u2Tribit[11] << 15) | (u2Tribit[12] << 12) | (u2Tribit[13] << 9) |
                  (u2Tribit[14] << 6) | (u2Tribit[15] << 3) | u2Tribit[16];
    pu2Data[3]  = (u2Tribit[17] << 13) | (u2Tribit[18] << 10) | (u2Tribit[19] << 7) |
                  (u2Tribit[20] << 4) | (u2Tribit[21] << 1) | (u2Tribit[22] >> 2);
    pu2Data[4]  = (u2Tribit[22] << 14) | (u2Tribit[23] << 11) | (u2Tribit[24] << 8) |
                  (u2Tribit[25] << 5) | (u2Tribit[26] << 2) | (u2Tribit[27] >> 1);
    pu2Data[5]  = (u2Tribit[27] << 15) | (u2Tribit[28] << 12) | (u2Tribit[29] << 9) |
                  (u2Tribit[30] << 6) | (u2Tribit[31] << 3) | u2Tribit[32];
    pu2Data[6]  = (u2Tribit[33] << 13) | (u2Tribit[34] << 10) | (u2Tribit[35] << 7) |
                  (u2Tribit[36] << 4) | (u2Tribit[37] << 1) | (u2Tribit[38] >> 2);
    pu2Data[7]  = (u2Tribit[38] << 14) | (u2Tribit[39] << 11) | (u2Tribit[40] << 8) |
                  (u2Tribit[41] << 5) | (u2Tribit[42] << 2) | (u2Tribit[43] >> 1);
    pu2Data[8]  = (u2Tribit[43] << 15) | (u2Tribit[44] << 12) | (u2Tribit[45] << 9) |
                  (u2Tribit[46] << 6) | (u2Tribit[47] << 3) | u2Tribit[48];
#endif
	memset(pu2Data,0,9*sizeof(UINT16));	
    Virterbi(u2Tribit, pu2Data);
}

void bptc_192_196_enc(UINT16* pu2Data, UINT16* pu2AiData)
{
    pu2AiData[1]  = (pu2AiData[1] & 0xff00) | (pu2Data[0] >> 8);
    pu2AiData[2]  = (pu2Data[0] << 8) | (pu2Data[1] >> 8);
    pu2AiData[3]  = (pu2Data[1] << 8) | (pu2Data[2] >> 8);
    pu2AiData[4]  = (pu2Data[2] << 8) | (pu2Data[3] >> 8);
    pu2AiData[5]  = (pu2Data[3] << 8) | (pu2Data[4] >> 8);
    pu2AiData[6]  = (pu2Data[4] << 8) | (pu2Data[5] >> 8);
    pu2AiData[7]  = (pu2Data[5] & 0x003f) | (pu2Data[5] << 8);
    pu2AiData[11] = pu2Data[11] & 0xfffc;
    pu2AiData[12] = pu2Data[6];
    pu2AiData[13] = pu2Data[7];
    pu2AiData[14] = pu2Data[8];
    pu2AiData[15] = pu2Data[9];
    pu2AiData[16] = pu2Data[10];
    pu2AiData[17] = pu2Data[11];
}

void bptc_192_196_dec(UINT16* pu2AiData, UINT16* pu2Data)
{
    pu2Data[0]  = pu2AiData[0];
    pu2Data[1]  = pu2AiData[1];
    pu2Data[2]  = pu2AiData[2];
    pu2Data[3]  = pu2AiData[3];
    pu2Data[4]  = pu2AiData[4];
    pu2Data[5]  = pu2AiData[5];
    pu2Data[6]  = (pu2AiData[10] << 8) | (pu2AiData[11] >> 8);
    pu2Data[7]  = (pu2AiData[11] << 8) | (pu2AiData[12] >> 8);
    pu2Data[8]  = (pu2AiData[12] << 8) | (pu2AiData[13] >> 8);
    pu2Data[9]  = (pu2AiData[13] << 8) | (pu2AiData[14] >> 8);
    pu2Data[10] = (pu2AiData[14] << 8) | (pu2AiData[15] >> 8);
    pu2Data[11] = (pu2AiData[15] << 8) | (pu2AiData[16] >> 8);
}

