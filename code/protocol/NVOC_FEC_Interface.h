/*
	说明： FEC 编码（ 20ms 语音帧进行一次 FEC）	
	输入参数：
	SrcWordBuf ----- 语音编码后数据 (3 个字, 共 48bits)
	输出参数：
	DstBitBuf  ----- FEC 后数据（ 72 个字，每个字低一位有效, 每个字代表一个 bit）
*/
extern void NVOC_FecEncFun(unsigned short *SrcWordBuf, unsigned short *DstBitBuf);

/*
	说明： FEC 解码（ 20ms 语音帧进行一次解 FEC）	
	输入参数：
	SrcBitBuf  ----- 解 FEC 输入数据 (72 个字, 低一位有效, 每个字代表一个 bit)
	输出参数：
	DstWordBuf ----- FEC 后数据（ 3 个字, 共 48bits）
*/
extern void NVOC_FecDecFun(unsigned short *SrcBitBuf, unsigned short *DstWordBuf);

