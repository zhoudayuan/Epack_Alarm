#ifndef __DLL_ALGRTHM_H__
#define __DLL_ALGRTHM_H__


#include "typedef.h"

UINT16 crc8(UINT16* pu2Data, UINT16 u2Len);
UINT16 crc9(UINT16 *pu2Data,UINT16 u2Len);
UINT16 crc_ccitt(UINT16* pu2Data, UINT16 u2Len);
UINT32 crc32(UINT16* pu2Data, UINT16 u2Len);
UINT16 csc5(UINT16* pu2Data, UINT16 u2Len);
UINT32 golay_20_8_enc(UINT32 u2Data);
UINT32 golay_20_8_dec(UINT32 u4Data);
UINT16 hamming_7_4_3_enc(UINT16 u2Data);
UINT16 hamming_7_4_3_dec(UINT16 u2Data);
UINT16 hamming_13_9_3_enc(UINT16 u2Data);
UINT16 hamming_13_9_3_dec(UINT16 u2Data);
UINT16 hamming_15_11_3_enc(UINT16 u2Data);
UINT16 hamming_15_11_3_dec(UINT16 u2Data);
UINT16 hamming_16_11_4_enc(UINT16 u2Data);
UINT16 hamming_16_11_4_dec(UINT16 u2Data);
UINT32 hamming_17_12_3_enc(UINT32 u4Data);
UINT32 hamming_17_12_3_dec(UINT32 u4Data);
UINT16 quadratic_residue_16_7_6_enc(UINT16 u2Data);
UINT16 quadratic_residue_16_7_6_dec(UINT16 u2Data);
void   reed_solomon_12_9_4_enc(UINT16* pu2Data, UINT16* u2Mask);
void   reed_solomon_12_9_4_dec(UINT16* p2uData, UINT16* u2Mask, UINT16* pu2Result);
UINT16 ones16(UINT16 u2Data);

#endif /* __ALGRTHM_H__ */

