/******************************************************************************
 * Copyright(C), 1993-2009, HYT
 * File name: interleave.h
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
 * @file interleave.h
 * @brief BPTC编/解码和(解)交织相关函数实现
 * @author zhiqi.liu
 * @version 0.1
 * @date 2009-05-04
 */
#ifndef __DLL_INTERLEAVE_H__
#define __DLL_INTERLEAVE_H__
/******************************************************************************
 *  Include
 * ***************************************************************************/
#include "dll_algrthm.h"


void voice_enc(UINT16* pu2Voice, UINT16* pu2AiData);

void voice_dec(UINT16* pu2AiData, UINT16* pu2Voice);

void voice_aie(UINT16* pu2Voice, UINT16 u2Mask);

void sync_enc(UINT16* pu2Sync, UINT16* pu2AiData);

void sync_dec(UINT16* pu2AiData, UINT16* pu2Sync);

void slottype_enc(UINT16 u2CC, UINT16 u2DataType, UINT16* pu2AiData);

void slottype_dec(UINT16* pu2AiData, UINT16* pu2CC, UINT16* pu2DataType);

void pdt_head_fulllc_enc(UINT16* pu2FullLC);

void pdt_term_fulllc_enc(UINT16* pu2FullLC);

void dmr_head_fulllc_enc(UINT16* pu2FullLC);

void dmr_term_fulllc_enc(UINT16* pu2FullLC);

void pdt_head_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result);

void pdt_term_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result);

void dmr_head_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result);

void dmr_term_fulllc_dec(UINT16* pu2FullLC, UINT16* pu2Result);

void common_enc(UINT16* pu2Data, UINT16* pu2AiData);

void common_dec(UINT16* pu2AiData, UINT16* pu2Data);

void bptc_144_196_enc(UINT16* pu2Data, UINT16* pu2AiData);

void bptc_144_196_dec(UINT16* pu2AiData, UINT16* pu2Data);

void bptc_192_196_enc(UINT16* pu2Data, UINT16* pu2AiData);

void bptc_192_196_dec(UINT16* pu2Data, UINT16* pu2AiData);

void emb_enc(UINT16 u2EMB, UINT16* pu2AiData);

void emb_dec(UINT16* pu2AiData, UINT16* pu2EMB);

void emb_48_enc(UINT16 u2CC, UINT16* u2EMB48, UINT16* pu2AiData);

void emb_48_dec(UINT16* pu2AiData, UINT16* u2EMB48);

void EmbeddedEnc(UINT8* puLC, UINT8* puEncodedLC);

void EmbeddedDec(UINT8* puEncodedLC, UINT8* puLC, UINT16 * pu2Result);

void zero_lcfrag_enc(UINT16* pu2AiData);

void embedded_lcfrag_enc(UINT16* pu2EncodedLC, UINT16* pu2AiData);

void embedded_lcfrag_dec(UINT16* pu2AiData, UINT16* pu2EncodedLC);

void cach_enc(UINT16* pu2ShortLC, UINT32* pu4CACH);

void cach_dec(UINT32* pu4CACH, UINT16* pu2ShortLC, UINT16* pu2Result);

void cach_frag_enc(UINT16 u2TACT, UINT32* pu4CACH, UINT16* pu2AiData);

void cach_frag_dec(UINT16* pu2AiData, UINT16* pu2TACT, UINT32* pu4CACH);

void rc_enc(UINT16* pu2RC, UINT16* pu2AiData);

void rc_dec(UINT16* pu2AiData, UINT16* pu2RC, UINT16* pu2Result);

#endif /* __INTERLEAVE_H__ */

