/*
	˵���� FEC ���루 20ms ����֡����һ�� FEC��	
	���������
	SrcWordBuf ----- ������������� (3 ����, �� 48bits)
	���������
	DstBitBuf  ----- FEC �����ݣ� 72 ���֣�ÿ���ֵ�һλ��Ч, ÿ���ִ���һ�� bit��
*/
extern void NVOC_FecEncFun(unsigned short *SrcWordBuf, unsigned short *DstBitBuf);

/*
	˵���� FEC ���루 20ms ����֡����һ�ν� FEC��	
	���������
	SrcBitBuf  ----- �� FEC �������� (72 ����, ��һλ��Ч, ÿ���ִ���һ�� bit)
	���������
	DstWordBuf ----- FEC �����ݣ� 3 ����, �� 48bits��
*/
extern void NVOC_FecDecFun(unsigned short *SrcBitBuf, unsigned short *DstWordBuf);

