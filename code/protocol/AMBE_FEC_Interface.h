/*
	说明： FEC 编码（ 20ms 语音帧进行一次 FEC）	
	输入参数：
	SrcBuf ----- 语音编码后数据 (49个字，每个字低一位有效, 每个字代表一个 bit)
	输入参数：
	StealBitNum ----- 偷比特数0-6
	输出参数：
	Dstbuf  ----- FEC 编码后数据（ 72 个字，每个字低一位有效, 每个字代表一个 bit）	
*/
extern void AMBE_FECEncFun(UINT16 *SrcBuf, UINT16 *Dstbuf,UINT16 StealBitNum);



/*
	说明： FEC 解码（ 20ms 语音帧进行一次 FEC）	
	输入参数：
	SrcBuf ----- 解 FEC 输入数据（ 72 个字，每个字低一位有效, 每个字代表一个 bit）
	输入参数：
	StealBitNum ----- 偷比特数0-6
	输出参数：
	Dstbuf  ----- FEC 解码后数据 (49个字，每个字低一位有效, 每个字代表一个 bit)	
*/
extern void AMBE_FECDecFun(UINT16 *SrcBuf, UINT16 *Dstbuf,UINT16 StealBitNum);


