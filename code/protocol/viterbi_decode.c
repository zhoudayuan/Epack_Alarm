/******************************************************************************
*   引用头文件声明
*   *************************************************************************/
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedef.h>
#include "dll.h"

/******************************************************************************
*   全局变量定义
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
* @brief   0~7的符号映射为3bit的0,1bit流;
*
* @Param   [in] psymbol       输入的10进制数据;
* @Param   [in]  num		   输入数据的个数;
* @Param   [out] pbit         输出3bit信息;
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
* @brief   将解码后3bit数据流整理成字格式输出
*
* @Param   [in]  ptriBitDecRslt   输入3bit数据流
* @Param   [out] poutdata	      输出整理后的字
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
* @brief   在8条路径中选择最小的路径，返回其位置;
*
* @Param   [in]  pdata       输入8条路径;
* @return  最小路径位置
*
*/

UINT16 MinPath(UINT16 *pdata)
{
	UINT16 i, j = 0;
	UINT16 min = pdata[0];				// 输入的第一个值记为最小值;

	for (i = 0; i < SELECTPATH; i++)
	{
		if (pdata[i] < min)				// 第i个输入小于最小值
		{
			min = pdata[i];				// 更新最小值
			j = i;
		}
	}
	return j;							// 最小值得位置赋与ipath
}


/**
* @brief   在64条路径中选择最小的8条路径，并输出对应的状态及输入值;
*
* @Param   [in]   parrary        输入64条分支度量的首地址;
* @Param   [out]  pipath_ed	     输出最小的排好序的8条路径;
* @Param   [out]  pstate         输出最小的8条路径对应的状态值;
* @Param   [out]  pdata          输出最小的8条路径对应的输入值;
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
		pstate[i] = locT / 8;	// 取商    状态
		pdata[i] = locT % 8;	// 取余    输入
	}
}




/**
* @brief   路径度量计算函数
*
* @Param   [in] dataAX		输入星座点A的X坐标;
* @Param   [in] dataBX		输入星座点B的X坐标;
* @Param   [in] dataAY		输入星座点A的Y坐标;
* @Param   [in] dataBY		输入星座点B的Y坐标;
* @return	返回路径度量计算结果
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
* @brief   双bit转换为符号
*
* @Param   [in]  InBit		输入双bit值;
* @return  符号值
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
* @brief   星座点映射为+3，+1，-1，-3;
*
* @Param   [in]  pinstar	输入的星座点;
* @Param   [in]  num		输入星座点的个数;
* @Param   [out] pdataX		输出对应的X坐标;
* @Param   [out] pdataY		输出对应的Y坐标;
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
* @brief   viterbi译码函数，输入值通过维特比译码选择最优的译码输出;
*
* @Param   [in]  pindata	  输入的星座点序列（0~15）;
* @Param   [out] outdata      输出维特比译码结果;
*
*/
void Virterbi(UINT16 *pindata, UINT16 outdata[])
{
	INT16 i, p, j = 1;
	UINT16 lastDistnc[INRANGE] = { 0 };					// 最后一个输入值的8条分支度量值
	UINT16 crntDistnc[64] = { 0 };						// 当前输入的可能所有分支度量值
	UINT16 DecodeOut[INDATA][INRANGE] = { {0 }};			// 前i个输入的可能8组译码输出
	UINT16 DecodeOutEnd[INDATA][INRANGE] = { {0 }};		// 前i个输入的可能8组译码输出
	UINT16 exInDistnc[INRANGE] = { 0 };					// 8条分支度量
	UINT16 triBitDecRslt[INDATA] = { 0 };				// 解码结果3bit数据流
	INT16 starX[INDATA] = { 0 }, starY[INDATA] = { 0 };	// 星座点XY坐标值

	UINT16 minDistnc = 0;
	UINT16 ConstelPData[INRANGE] = { 0 };				// 第i个输入星座点对应的状态机输入
	UINT16 ConstelPState[INSTATIC] = { 0 }; 			// 第i个输入星座点对应的状态机状态
	 
	MapStar(pindata, INDATA, starX, starY);				// 第i个状态下的输入数据对应的星图坐标

														// 第一个输入值所对应的分支度量
	for (i = 0; i < INRANGE; i++)
	{
		ConstelPData[i] = i;							// 下一个状态值;
		DecodeOut[0][i] = i;							// 0状态下的可能输入值
		exInDistnc[i] = DistanceCal(starX[0], staticX[0][i], starY[0], staticY[0][i]);		//add by cz
	}

	//第i个输入值所对应的分支度量
	for (j = 1; j < INDATA - 1; j++)					// 输入第i个数据
	{
		for (p = 0; p < INSTATIC; p++)					// 状态
		{
			for (i = 0; i < INRANGE; i++)				// 状态对应的可能输入值
			{
				crntDistnc[p * 8 + i] = DistanceCal(starX[j], staticX[ConstelPData[p]][i], starY[j], staticY[ConstelPData[p]][i]) + exInDistnc[p];   //第i个输入值所对应的64条分支度量
			}
		}

		PathFunc(crntDistnc, exInDistnc, ConstelPState, ConstelPData);	// 距离按小到大排列，Instar前8条最小路径，position最小的8条路径所在位置；

		for (p = 0; p <= j; p++)						// 输入第q个数据
		{
			for (i = 0; i < INRANGE; i++)				// 状态机对应的输入值
			{
				if (p < j)
				{
					DecodeOutEnd[p][i] = DecodeOut[p][ConstelPState[i]];// 当前输入的星座点对应的前i-1个可能输入值
				}
				else
				{
					DecodeOut[p][i] = ConstelPData[i];					// 当前第i个输入的星座点对应的状态机可能输入值
					DecodeOutEnd[p][i] = DecodeOut[p][i];
				}
			}
		}
		memcpy(DecodeOut, DecodeOutEnd, j * 8 * sizeof(INT16));
	}

	//最后一个输入值，对应的零状态
	for (i = 0; i < INRANGE; i++)
	{
		lastDistnc[i] = DistanceCal(starX[INDATA - 1], staticX[DecodeOut[INDATA - 2][i]][0], starY[INDATA - 1], staticY[DecodeOut[INDATA - 2][i]][0]) + exInDistnc[i];//最后一个分支度量
	}

	minDistnc = MinPath(lastDistnc);						//m_ipath 最小路径的输入   m_ipath 最小分支度量所在位置

	for (i = 0; i < INDATA - 1; i++)
	{
		triBitDecRslt[i] = DecodeOutEnd[i][minDistnc];		//min_path_end 维特比的译码输出，前48个译码为最小路径的前48个
	}

	TriBitToShort(triBitDecRslt, outdata);
}

