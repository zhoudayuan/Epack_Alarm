/*
	˵���� FEC ���루 20ms ����֡����һ�� FEC��	
	���������
	SrcBuf ----- ������������� (49���֣�ÿ���ֵ�һλ��Ч, ÿ���ִ���һ�� bit)
	���������
	StealBitNum ----- ͵������0-6
	���������
	Dstbuf  ----- FEC ��������ݣ� 72 ���֣�ÿ���ֵ�һλ��Ч, ÿ���ִ���һ�� bit��	
*/
extern void AMBE_FECEncFun(UINT16 *SrcBuf, UINT16 *Dstbuf,UINT16 StealBitNum);



/*
	˵���� FEC ���루 20ms ����֡����һ�� FEC��	
	���������
	SrcBuf ----- �� FEC �������ݣ� 72 ���֣�ÿ���ֵ�һλ��Ч, ÿ���ִ���һ�� bit��
	���������
	StealBitNum ----- ͵������0-6
	���������
	Dstbuf  ----- FEC ��������� (49���֣�ÿ���ֵ�һλ��Ч, ÿ���ִ���һ�� bit)	
*/
extern void AMBE_FECDecFun(UINT16 *SrcBuf, UINT16 *Dstbuf,UINT16 StealBitNum);


