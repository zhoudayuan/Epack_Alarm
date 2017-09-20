/******************************************************************************
*   ����ͷ�ļ�����
*   *************************************************************************/
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedef.h>
#include "dll.h"

/******************************************************************************
*   ȫ�ֱ�������
*   *************************************************************************/
static INT16 staticX[INSTATIC][INRANGE] =
{
	{ 1,-3,-3,1,3,-1,-1,3 },
	{ -3,1,3,-1,-1,3,1,-3 },
	{ -1,3,3,-1,-3,1,1,-3 },
	{ 3,-1,-3,1,1,-3,-1,3 },
	{ -3,1,1,-3,-1,3,3,-1 },
	{ 1,-3,-1,3,3,-1,-3,1 },
	{ 3,-1,-1,3,1,-3,-3,1 },
	{ -1,3,1,-3,-3,1,3,-1 }

};                                                        
static INT16 staticY[INSTATIC][INRANGE] =
{
	{ -1,3,-1,3,-3,1,-3,1 },
	{ -1,3,-3,1,-3,1,-1,3 },
	{ -1,3,-1,3,-3,1,-3,1 },
	{ -1,3,-3,1,-3,1,-1,3 },
	{ -3,1,-3,1,-1,3,-1,3 },
	{ -3,1,-1,3,-1,3,-3,1 },
	{ -3,1,-3,1,-1,3,-1,3 },
	{ -3,1,-1,3,-1,3,-3,1 }

};

/**
* @brief   0~7�ķ���ӳ��Ϊ3bit��0,1bit��;
*
* @Param   [in] psymbol       �����10��������;
* @Param   [in]  num		   �������ݵĸ���;
* @Param   [out] pbit         ���3bit��Ϣ;
*
*/
void TriBit(UINT16 *psymbol, UINT16 num, UINT16 *pbit)
{
	INT16 temp, i;
	for (i = 0; i < num; i++)
	{
		temp = psymbol[i];
		*pbit++ = (temp >> 2) & 0x01;
		*pbit++ = (temp >> 1) & 0x01;
		*pbit++ = temp & 0x01;
	}
}

/**
* @brief   �������3bit������������ָ�ʽ���
*
* @Param   [in]  ptriBitDecRslt   ����3bit������
* @Param   [out] poutdata	      �����������
*
*/
void TriBitToShort(UINT16 *ptriBitDecRslt, UINT16 *poutdata)
{
	INT16 i, j, k = 0;
	UINT16  bitArray[144] = { 0 };
	memset(poutdata, 0, sizeof(UINT16) * 9);

	TriBit(ptriBitDecRslt, 48, bitArray);
	for (i = 0; i < 9; i++)
	{
		for (j = 15; j >= 0; j--)
		{
			poutdata[i] += bitArray[k++] << j;
		}
	}
}

/**
* @brief   ��8��·����ѡ����С��·����������λ��;
*
* @Param   [in]  pdata       ����8��·��;
* @return  ��С·��λ��
*
*/

UINT16 MinPath(UINT16 *pdata)
{
	UINT16 i, j = 0;
	UINT16 min = pdata[0];				// ����ĵ�һ��ֵ��Ϊ��Сֵ;

	for (i = 0; i < SELECTPATH; i++)
	{
		if (pdata[i] < min)				// ��i������С����Сֵ
		{
			min = pdata[i];				// ������Сֵ
			j = i;
		}
	}
	return j;							// ��Сֵ��λ�ø���ipath
}


/**
* @brief   ��64��·����ѡ����С��8��·�����������Ӧ��״̬������ֵ;
*
* @Param   [in]   parrary        ����64����֧�������׵�ַ;
* @Param   [out]  pipath_ed	     �����С���ź����8��·��;
* @Param   [out]  pstate         �����С��8��·����Ӧ��״ֵ̬;
* @Param   [out]  pdata          �����С��8��·����Ӧ������ֵ;
*
*/

void PathFunc(UINT16 *parrary, UINT16 *pipath_ed, UINT16 *pstate, UINT16 *pdata)
{
	INT16 i, j, locT = 0, ValT = 0;
	UINT16 dataT[PATH] = { 0 };

	memcpy(dataT, parrary, sizeof(UINT16)*PATH);

	for (i = 0; i < INRANGE; i++)
	{
		ValT = dataT[0];
		locT = 0;
		for (j = 1; j < PATH; j++)
		{
			if (dataT[j] < ValT)
			{
				ValT = dataT[j];
				locT = j;
			}
		}
		dataT[locT] = 10000;
		pipath_ed[i] = ValT;
		pstate[i] = locT / 8;	// ȡ��    ״̬
		pdata[i] = locT % 8;	// ȡ��    ����
	}
}




/**
* @brief   ·���������㺯��
*
* @Param   [in] dataAX		����������A��X����;
* @Param   [in] dataBX		����������B��X����;
* @Param   [in] dataAY		����������A��Y����;
* @Param   [in] dataBY		����������B��Y����;
* @return	����·������������
*
*/
UINT16 DistanceCal(INT16 dataAX, INT16 dataBX, INT16 dataAY, INT16 dataBY)
{
	UINT16 DistanceRslt = 0;
	DistanceRslt = abs(dataAX - dataBX) + abs(dataAY - dataBY);
	//DistanceRslt = sqrt((dataAX - dataBX)*(dataAX - dataBX) + (dataAY - dataBY)*(dataAY - dataBY));
	return DistanceRslt;
}

/**
* @brief   ˫bitת��Ϊ����
*
* @Param   [in]  InBit		����˫bitֵ;
* @return  ����ֵ
*
*/
INT16 BitToSymbol(INT16 InBit)
{
	switch (InBit)
	{
	case 0:
		return 1;
	case 1:
		return 3;
	case 2:
		return -1;
	case 3:
		return -3;
	default:
		return 1;
	}
}


/**
* @brief   ������ӳ��Ϊ+3��+1��-1��-3;
*
* @Param   [in]  pinstar	�����������;
* @Param   [in]  num		����������ĸ���;
* @Param   [out] pdataX		�����Ӧ��X����;
* @Param   [out] pdataY		�����Ӧ��Y����;
*
*/
void MapStar(UINT16 *pinstar, UINT16 num, INT16 *pdataX, INT16 *pdataY)
{
	INT16 i, itemp;

	for (i = 0; i < num; i++)
	{
		itemp = (pinstar[i] >> 2) & 0x03;
		*pdataX++ = BitToSymbol(itemp);
		itemp = pinstar[i] & 0x03;
		*pdataY++ = BitToSymbol(itemp);
	}
}


/**
* @brief   viterbi���뺯��������ֵͨ��ά�ر�����ѡ�����ŵ��������;
*
* @Param   [in]  pindata	  ��������������У�0~15��;
* @Param   [out] outdata      ���ά�ر�������;
*
*/
void Virterbi(UINT16 *pindata, UINT16 outdata[])
{
	INT16 i, p, j = 1;
	UINT16 lastDistnc[INRANGE] = { 0 };					// ���һ������ֵ��8����֧����ֵ
	UINT16 crntDistnc[64] = { 0 };						// ��ǰ����Ŀ������з�֧����ֵ
	UINT16 DecodeOut[INDATA][INRANGE] = { {0 }};			// ǰi������Ŀ���8���������
	UINT16 DecodeOutEnd[INDATA][INRANGE] = { {0 }};		// ǰi������Ŀ���8���������
	UINT16 exInDistnc[INRANGE] = { 0 };					// 8����֧����
	UINT16 triBitDecRslt[INDATA] = { 0 };				// ������3bit������
	INT16 starX[INDATA] = { 0 }, starY[INDATA] = { 0 };	// ������XY����ֵ

	UINT16 minDistnc = 0;
	UINT16 ConstelPData[INRANGE] = { 0 };				// ��i�������������Ӧ��״̬������
	UINT16 ConstelPState[INSTATIC] = { 0 }; 			// ��i�������������Ӧ��״̬��״̬
	 
	MapStar(pindata, INDATA, starX, starY);				// ��i��״̬�µ��������ݶ�Ӧ����ͼ����

														// ��һ������ֵ����Ӧ�ķ�֧����
	for (i = 0; i < INRANGE; i++)
	{
		ConstelPData[i] = i;							// ��һ��״ֵ̬;
		DecodeOut[0][i] = i;							// 0״̬�µĿ�������ֵ
		exInDistnc[i] = DistanceCal(starX[0], staticX[0][i], starY[0], staticY[0][i]);		//add by cz
	}

	//��i������ֵ����Ӧ�ķ�֧����
	for (j = 1; j < INDATA - 1; j++)					// �����i������
	{
		for (p = 0; p < INSTATIC; p++)					// ״̬
		{
			for (i = 0; i < INRANGE; i++)				// ״̬��Ӧ�Ŀ�������ֵ
			{
				crntDistnc[p * 8 + i] = DistanceCal(starX[j], staticX[ConstelPData[p]][i], starY[j], staticY[ConstelPData[p]][i]) + exInDistnc[p];   //��i������ֵ����Ӧ��64����֧����
			}
		}

		PathFunc(crntDistnc, exInDistnc, ConstelPState, ConstelPData);	// ���밴С�������У�Instarǰ8����С·����position��С��8��·������λ�ã�

		for (p = 0; p <= j; p++)						// �����q������
		{
			for (i = 0; i < INRANGE; i++)				// ״̬����Ӧ������ֵ
			{
				if (p < j)
				{
					DecodeOutEnd[p][i] = DecodeOut[p][ConstelPState[i]];// ��ǰ������������Ӧ��ǰi-1����������ֵ
				}
				else
				{
					DecodeOut[p][i] = ConstelPData[i];					// ��ǰ��i��������������Ӧ��״̬����������ֵ
					DecodeOutEnd[p][i] = DecodeOut[p][i];
				}
			}
		}
		memcpy(DecodeOut, DecodeOutEnd, j * 8 * sizeof(INT16));
	}

	//���һ������ֵ����Ӧ����״̬
	for (i = 0; i < INRANGE; i++)
	{
		lastDistnc[i] = DistanceCal(starX[INDATA - 1], staticX[DecodeOut[INDATA - 2][i]][0], starY[INDATA - 1], staticY[DecodeOut[INDATA - 2][i]][0]) + exInDistnc[i];//���һ����֧����
	}

	minDistnc = MinPath(lastDistnc);						//m_ipath ��С·��������   m_ipath ��С��֧��������λ��

	for (i = 0; i < INDATA - 1; i++)
	{
		triBitDecRslt[i] = DecodeOutEnd[i][minDistnc];		//min_path_end ά�رȵ����������ǰ48������Ϊ��С·����ǰ48��
	}

	TriBitToShort(triBitDecRslt, outdata);
}

