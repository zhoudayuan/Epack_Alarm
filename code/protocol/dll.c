/**
  * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
  * @file    DLL.c
  * @brief   实现链路层数据处理模块的初始化
  * @author  陈禹良
  * @version v1.0
  * @date    2016-7-20
  */
/*
  *   修改历史
  *   2016-7-20                      陈禹良                     建立文件
  *   *************************************************************************/

/******************************************************************************
  *   引用头文件声明
  *   *************************************************************************/
#include "dll_fun.h"
#include "mgr_common.h"
#include "print_debug.h"
#include "EncDec.h"

/******************************************************************************
  *   全局变量定义
  *   *************************************************************************/
extern int g_iCcErrFrmCnt;
extern int g_bCcRightFrmRcvFlg;
extern int g_iCcErrSuperFrmCnt;
#define FPGA_FOLLOW     1
#define FPGA_IDLE       0
#define ARRAY_NEGR_LEN  3  // 邻点信息暂存数组长度
#define Encrypt_key_path "encrypt_table"

/**
 * @var  s_i4LogMsgId
 * @brief 命令行套接字
 */
extern INT32 s_LogMsgId;

/**
 * @var  pLogTxtFd
 * @brief 文件句柄
 */
extern FILE *pLogFd;

/**
 * @var ptIPCShm
 * @brief 共享内存系统信息
*/
extern SHM_IPC_STRU *ptIPCShm;

/**
 * @var ptCFGShm
 * @brief 共享内存系统信息
 */
extern SHM_CFG_STRU *ptCFGShm;

/**
 * @var  g_PdpUBuf
 * @brief 上行分组数据缓存
 */
extern PDP_UT g_PdpUBuf;

/**
 * @var  g_PdpDBuf
 * @brief 下行分组数据缓存
 */
extern PDP_DT g_PdpDBuf;

extern UINT16  s_au2AmbeEraData[14];

/**
 * @var OdpPid
 * @brief  下行线程PID
 */
pthread_t OdpPid;

/**
 * @var IdpPid
 * @brief  上行线程PID
 */
pthread_t IdpPid;

/**
 * @var NeighborPid
 * @brief  邻点突发线程PID
 */
pthread_t NeighborPid;

/**
 * @var TimerPid
 * @brief  定时器线程PID
 */
pthread_t TimerPid;

/**
 * @var uTimerSwitch
 * @brief  定时器开关
 */
UINT8 uTimerSwitch;

/**
 * @var uLockFreq
 * @brief  业务锁定频率
 */
UINT8 uLockFreq;

/**
 * @var uLockSlot
 * @brief  业务锁定时隙
 */
UINT8 uLockSlot;

/**
 * @var uGpsFlag
 * @brief  GPS业务标记
 */
UINT8 uGpsFlag;

/**
 * @var uRelayFlag
 * @brief  转发标记
 */
UINT8 uRelayFlag;

/**
 * @var uCallState
 * @brief  业务状态标示
 */
UINT8 uCallState;

/**
 * @var uCallWait
 * @brief  业务等待时间
 */
UINT16 uCallWait;

/**
 * @var uWaitCallTimes
 * @brief  呼叫等待计时
 */
UINT32 uWaitVoiceTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  呼叫等待计时
 */
UINT32 uWaitDataTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  呼叫等待计时
 */
UINT32 uWaitNearTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  呼叫等待计时
 */
UINT32 uWaitWluTimes = 0;

/**
 * @var uWaitErrTimes
 * @brief  误码率测试计时器
 */
UINT32 uWaitErrTimes = 0;


/**
 * @var CCLSocket
 * @brief 呼叫控制层socket
 */
int CCLSocket;

/**
 * @var INFSocket
 * @brief 接口适配层socket
 */
int INFSocket;

/**
 * @var DllOdpAddr
 * @brief DLLsocket下行地址结构
 */
struct sockaddr_in DllOdpAddr;

/**
 * @var DllIdpAddr
 * @brief DLLsocket上行地址结构
 */
struct sockaddr_in DllIdpAddr;

/**
 * @var RecvAddr
 * @brief socket接收地址结构
 */
struct sockaddr_in RecvAddr;

/**
 * @var CclAddr
 * @brief CCLsocket地址结构
 */
struct sockaddr_in CclAddr;

/**
 * @var InfAddr
 * @brief INFsocket地址结构
 */
struct sockaddr_in InfAddr;

/**
 * @var AddrSize
 * @brief socket地址长度
 */
socklen_t AddrSize = sizeof(struct sockaddr_in);

/**
 * @var ptCclData
 * @brief 上行呼叫控制数据指针
 */
DLL_CCL_UL_T *ptCclData;

/**
 * @var ptInfData
 * @brief 下行接口适配数据指针
 */
NAS_INF_DL_T *ptInfData;

/**
 * @var g_DllGlobalCfg
 * @brief 全局配置项
 */
DLL_GLB_CFG_T g_DllGlobalCfg;

/**
 * @var p_DllFpgaShm
 * @brief 链接层和FPGA共享内存
 */
DLL_FPGA_SHM_T *p_DllFpgaShm;

/**
 * @var tDllPrint
 * @brief DLL打印开关
 */
DLL_PRINT_T *tDllPrint;


/**
 * @var g_discon_state
 * @brief 断链状态
 */
UINT8 g_discon_state = 0;


/**
 * @var g_DisconCnt
 * @brief 断链计数器
 */
UINT8 g_DisconCnt = 0;

/**
 * @var g_BurstCnt
 * @brief 邻点突发周期计数器，
 *        当有语音时，该计时器清零，当其小于2时, 邻点功能无效。
 */
UINT32 g_BurstCnt = 0;
/**
 * @var 
 * @brief 关闭断链打印标志
 */
UINT8 g_DisconRecoverPrintFlg = 0;
/**
 * @var s_NerCycCnt
 * @brief 邻点突发周期计数器。记录邻点突发的个数,主要功能是鉴别奇数or偶数周期
 *        s_NerCycCnt 和 g_BurstCnt 区别是 s_NerCycCnt 不被清零，g_BurstCnt 反之
 */
UINT32 s_NerCycCnt = 0;

/**
 * @var g_NerbValidFlg
 * @brief 在通话过程中，邻点可查询标志, 0为不可查，1为可查(非通话状态下该标志为1)
 */
INT8 g_NerbValidFlg = 0;

/**
 * @var s_auiNegrId
 * @brief 邻点信息暂存数组，数组中的每一个元素代表邻点信息的一次获取，
 * 获取三次邻点信息将数组内元素或计算，得出最终邻点信息数值，
 */
static unsigned int s_auiNegrId[ARRAY_NEGR_LEN] = {0};
static int s_uiNegrIdArrayLen = sizeof(s_auiNegrId)/sizeof(s_auiNegrId[0]);
void delay(unsigned long msec);

/******************************************************************************
  *   内部函数实现
  *   *************************************************************************/
/**
 * @brief  DLL 协议栈全局初始化函数
 *
 * @param [in] void
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
void DLL_GlobalConfig(void)
{
    memset(&g_DllGlobalCfg, 0, sizeof(g_DllGlobalCfg));

    g_DllGlobalCfg.auMSCC = ptCFGShm->terminal_cc.val;
    g_DllGlobalCfg.auWLUCC = ptCFGShm->cc.val;
    g_DllGlobalCfg.auPI = 0;
    g_DllGlobalCfg.auWorkMode = ptCFGShm->protocol_mode.val;
    g_DllGlobalCfg.auCCflag = 1;    //debug set 0
    g_DllGlobalCfg.auNegrBuSw = ptCFGShm->start_neighbor.val;
    g_DllGlobalCfg.auNegrCyc = ptCFGShm->neighbor_period.val;
    g_DllGlobalCfg.auNodeId = ptCFGShm->dev_id.val;
    g_DllGlobalCfg.auSubNet = ptCFGShm->air_subnet_id.val;
    g_DllGlobalCfg.auStunFlag = ptCFGShm->stun_flag.val;
    g_DllGlobalCfg.auKillFlag = ptCFGShm->kill_flag.val;
	#if 0
		g_DllGlobalCfg.auVoiceType = ptCFGShm->voice_type.val;
		g_DllGlobalCfg.auEncryption_Type = ptCFGShm->encryption_type.val;
		g_DllGlobalCfg.ucFlcGpsSwitch= ptCFGShm->nas_gps_switch.val;
	#endif
	
	//test
	
	g_DllGlobalCfg.auVoiceType = AMBE;
	g_DllGlobalCfg.auEncryption_Type = ENCRYPT_HYT_ENHA;
	
	g_DllGlobalCfg.ucFlcGpsSwitch= FLC_GPS_ON;
	g_DllGlobalCfg.ucFlcGpsSndFlg = SND_FLC_MULTI;
	g_DllGlobalCfg.ucFlcGpsSec=3;
	//网管增加   加解密密钥选择(基本加密、高级加密都需要选择)  及随机密钥加密，多密钥解密选项
	//test over
	
	if(g_DllGlobalCfg.auPI==0)
	{
		g_DllGlobalCfg.auEncryption_Type=ENCRYPT_NONE;
	}
	else
	{
		if(g_DllGlobalCfg.auWorkMode==DMR_WORK_MODE)
		{
			g_DllGlobalCfg.auEncryption_Type=ENCRYPT_DMRA;
			if(g_DllGlobalCfg.auVoiceType == NVOC)
			{
				LOG_ERROR(s_LogMsgId,"[DLL][%s][ENCRYPT_DMRA][NVOC][ERROR] ", _F_);
			}
		}
		else
		{
			if((g_DllGlobalCfg.auEncryption_Type!=ENCRYPT_HYT_BASE)&&(g_DllGlobalCfg.auEncryption_Type!=ENCRYPT_HYT_ENHA))
			LOG_ERROR(s_LogMsgId,"[DLL][%s] PI=1,But Encryption_Type=%d Error", _F_,g_DllGlobalCfg.auEncryption_Type);
		}
	}
    uLockFreq = 0xff;
    uLockSlot = 0xff;

    tDllPrint = (DLL_PRINT_T *)ptIPCShm->dll_printf;
    memset((UINT8 *)tDllPrint, 0x00, sizeof(DLL_PRINT_T));
    tDllPrint->FrqSlt = 0;
    tDllPrint->WorkMode = 0;
    tDllPrint->CcErrSuperFrmCnt = 3;

	LOG_DEBUG(s_LogMsgId,"*************[%s]Dll Encryption and Gps Pramater***************", _F_);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][auVoiceType      =%d,[206:AMBE,207:NVOC] ", _F_,g_DllGlobalCfg.auVoiceType);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][auPI             =%d,[0:UnEryption,1:Encry] ", _F_,g_DllGlobalCfg.auPI);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][auEncryption_Type=%d,[0:UnEryption,1:PDT BASE,2:PDT High,3:DMRA] ", _F_,g_DllGlobalCfg.auEncryption_Type);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][ucFlcGpsSwitch   =%d,[0:OFF,1:ON] ", _F_,g_DllGlobalCfg.ucFlcGpsSwitch);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][ucFlcGpsSndFlg   =%d,[0:ONCE,1:CYC] ", _F_,g_DllGlobalCfg.ucFlcGpsSndFlg);
	LOG_DEBUG(s_LogMsgId,"[DLL][%s][ucFlcGpsSec      =%d s,[Cyc]", _F_,g_DllGlobalCfg.ucFlcGpsSec);
	LOG_DEBUG(s_LogMsgId,"*************[%s]Dll Encryption and Gps Pramater Over***************", _F_);

}


/**
 * @brief  DLL 协议栈FPGA配置初始化函数
 *
 * @param [in] void
 *
 * @return  int         0:成功，-1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int DLL_FpgaConfig(void)
{
    int fd_mem = 0;

    fd_mem = open(FPGA_SHM_DEV, O_RDWR|O_SYNC);
    if(fd_mem < 0)
    {
        printf("[DLL] FpgaConfig  open axiram-config error!!\n");
        return -1;
    }

    p_DllFpgaShm = (DLL_FPGA_SHM_T *)mmap(NULL, FPGA_SHM_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, FPGA_SHM_ADDR);
    if (p_DllFpgaShm == MAP_FAILED)
    {
        printf("[DLL] FpgaConfig  map fpga shm addr error!");
        close(fd_mem);
        return -1;
    }
    close(fd_mem);

    memset(p_DllFpgaShm, 0, sizeof(DLL_FPGA_SHM_T));
    //p_DllFpgaShm->FollowEn = 0;           //由fpga设置
    p_DllFpgaShm->MSCC = g_DllGlobalCfg.auMSCC;
    p_DllFpgaShm->WLUCC = g_DllGlobalCfg.auWLUCC;

    if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)
    {
        p_DllFpgaShm->StunFlag[0] = NAS_KILL_FLAG      &0xff;
        p_DllFpgaShm->StunFlag[1] = (NAS_KILL_FLAG>>8) &0xff;
        p_DllFpgaShm->StunFlag[2] = (NAS_KILL_FLAG>>16)&0xff;
        p_DllFpgaShm->StunFlag[3] = (NAS_KILL_FLAG>>24)&0xff;
    }

    DLL_SyncFpgaCfg();//更新

    return 0;

}


/**
 * @brief   数据链路层初始化函数
 *
 * @return  int         0:成功，-1:失败
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
int DLL_Init(void)
{
    int status,ret;
    DLL_GlobalConfig();//初始化本地全局，cfgshm全局配置更新到本地全局
	//初始化加密参数
	ret=InitEncLib(g_DllGlobalCfg.auVoiceType,g_DllGlobalCfg.auEncryption_Type,Encrypt_key_path);
	if(ret == -1)
	{
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] Error Or UnEncryption", _F_);
	}
    status = DLL_FpgaConfig();//通过g_DllGlobalCfg 全局  来配置更新DLL与FPGA的共享内存
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] DLL_FpgaConfig error", _F_);
        return -1;
    }
    DLL_ClearTimer();//置位初始化状态

    CCLSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > CCLSocket)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] Failed to create CCLSocket", _F_);
        return -1;
    }
    INFSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > INFSocket)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] Failed to create INFSocket", _F_);
        return -1;
    }
    bzero(&DllOdpAddr, sizeof(struct sockaddr_in));
    DllOdpAddr.sin_family = AF_INET;
    DllOdpAddr.sin_port = htons(SOCK_PORT_CCL_DLL);
    DllOdpAddr.sin_addr.s_addr = INADDR_ANY;

    bzero(&CclAddr, sizeof(struct sockaddr_in));
    CclAddr.sin_family = AF_INET;
    CclAddr.sin_port = htons(SOCK_PORT_DLL_CCL);
    CclAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bzero(&DllIdpAddr, sizeof(struct sockaddr_in));
    DllIdpAddr.sin_family = AF_INET;
    DllIdpAddr.sin_port = htons(SOCK_PORT_ADP_DLL);
    DllIdpAddr.sin_addr.s_addr = INADDR_ANY;

    bzero(&InfAddr, sizeof(struct sockaddr_in));
    InfAddr.sin_family = AF_INET;
    InfAddr.sin_port = htons(SOCK_PORT_DLL_ADP);
    InfAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (0 > bind(CCLSocket, (struct sockaddr *)(&DllOdpAddr), sizeof(struct sockaddr_in)))
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] Bind DllOdpAddr Error : %s", _F_, strerror(errno));
        return -1;
    }
    if (0 > bind(INFSocket, (struct sockaddr *)(&DllIdpAddr), sizeof(struct sockaddr_in)))
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] Bind DllIdpAddr Error : %s", _F_, strerror(errno));
        return -1;
    }
    status = pthread_create(&IdpPid, NULL, INFUploadTask, NULL);
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] creat IDP thread error", _F_);
        return -1;
    }
    pthread_detach(IdpPid);
    status = pthread_create(&OdpPid, NULL, CCLDownloadTask, NULL);
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] creat ODP thread error", _F_);
        return -1;
    }
    pthread_detach(OdpPid);
    status = pthread_create(&NeighborPid, NULL, DLL_NerBurstTask, NULL);
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] creat Neighbor thread error", _F_);
        return -1;
    }
    pthread_detach(NeighborPid);
    status = pthread_create(&TimerPid, NULL, DLL_TimerTask, NULL);
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] creat Timer thread error", _F_);
        return -1;
    }
    pthread_detach(TimerPid);
    return 0;
}


/**
 * @brief   数据链路层设置定时器
 * @param [in] CallState             业务类型
 * @param [in] CallWait              定时时间毫秒
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void DLL_SetTimer(UINT8 CallState, UINT16 CallWait)
{
    if (CallWait > 60000)
    {
        LOG_WARNING(s_LogMsgId,"[DLL][%s] Sap timer set error %d", _F_, CallWait);
    }

    uTimerSwitch = 1;
    uCallState = CallState;
    uCallWait = CallWait/60;

    uWaitVoiceTimes = 0;
    uWaitDataTimes = 0;
    uWaitNearTimes = 0;
    uWaitWluTimes = 0;

    // 误码率测试计时器, 仅在测语音误码率时有效
    uWaitErrTimes = 0;
}


/**
 * @brief   数据链路层清除定时器
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void DLL_ClearTimer(void)
{
    uTimerSwitch = 0;//定时器关闭
    uCallState = CALL_IDLE;
    uCallWait = 0xffff;

    uWaitVoiceTimes = 0;
    uWaitDataTimes = 0;
    uWaitNearTimes = 0;
    uWaitWluTimes = 0;

    if (uGpsFlag != 0xff)
    {
        uGpsFlag = 0xff;
    }

    // 误码率测试计时器, 仅在测语音误码率时有效
    uWaitErrTimes = 0;
}


/**
 * @brief   数据链路层同步GPS时间
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void DLL_SyncGpsTime(void)
{
    struct tm _tm;
    struct timeval tv;
    time_t timep;

    GPS_DEV_DATA_T *GpsDevData;
    GpsDevData = (GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData);

    if ((p_DllFpgaShm->GpsFlag & 0x01) == 1)
    {
        _tm.tm_sec = GpsDevData->SEC;
        _tm.tm_min = GpsDevData->MIN;
        _tm.tm_hour = (GpsDevData->HOUR+8)%24;
        _tm.tm_mday = GpsDevData->DAY;
        _tm.tm_mon = GpsDevData->MONTH - 1;
        _tm.tm_year = GpsDevData->YEAR+ 2000- 1900;

        timep = mktime(&_tm);
        tv.tv_sec = timep;
        tv.tv_usec = 0;
        if(settimeofday (&tv, (struct timezone *) 0) < 0)
        {
            printf("Set system datatime error!/n");
            return;
        }
    }
    return;
}


/**
 * @brief   数据链路层更新fpga共享内存
 *
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void DLL_SyncFpgaCfg(void)
{
    msync((void *)FPGA_SHM_ADDR, FPGA_SHM_LEN, MS_SYNC);
}


/**
 * @brief   数据链路层定时器线程
 *
 * @param [in] p       传递给线程start_routine的参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void *DLL_TimerTask(void * p)
{
    short GpsTimSync = SYNC_CYC;
    int ret;
    struct timeval tv;

    sleep(10);

    while(1)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 60000;

        ret = select(0, NULL, NULL, NULL, &tv);

        if (ret < 0)
        {
            printf("[DLL] TIM  select error\n");
            continue;
        }

        TimerTask();  // 定时器
        DLL_SyncFpgaCfg();//每60ms都更新一下共享内存
		
        if (0 == p_DllFpgaShm->FollowEn)//FPGA空闲，未锁定，维持低电平
        {
            p_DllFpgaShm->WorkOver = WORK_START;//置FPGA为低电平 ，任何频点时隙都可以上数据
            uLockFreq = 0xff;
            uLockSlot = 0xff;//锁定为ff  表明2个频点2个时隙都可以上数据
            memset(g_DllGlobalCfg.auUploadLC, 0xff, (FLC_LEN+3));
        }

        GpsTimSync--;
        if (GpsTimSync < 0)
        {
            DLL_SyncGpsTime();//更新系统时间250  * 60ms
            GpsTimSync = SYNC_CYC;
        }

        //if (tDllPrint->AIDown == 1)
        //{
        //    if (1 == p_DllFpgaShm->FollowEn)
        //    {
        //        LOG_DEBUG(s_LogMsgId,"[DLL][%s] follow en_%d CallingST: %d", _F_, p_DllFpgaShm->FollowEn, p_DllFpgaShm->CallingST);
        //    }
        //}

        //LOG_DEBUG(s_LogMsgId,"[DLL][%s] follow en_%d ", __FUNCTION__, p_DllFpgaShm->FollowEn);

        if (uTimerSwitch == 0)
        {
            continue;
        }

        if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)
        {
            continue;
        }

        switch(uCallState)
        {
            case CALL_IDLE:
            {
                break;
            }
            case CALL_VOICE_D://无此状态，不处理定时器
            case CALL_VOICE_U://只有加密才置此状态，正常语音DLL层不做定时
            {
                uWaitVoiceTimes++;
                if (uWaitVoiceTimes >= uCallWait)
                {
                	if(uWaitVoiceTimes>=(UINT32)(uCallWait+2))// 2个超帧未上语音且未接收到LC_TER，清除状态 421ms-480ms
					{
	                    g_bCcRightFrmRcvFlg = 0;
	                    g_iCcErrFrmCnt = 0;
	                    g_iCcErrSuperFrmCnt = 0;

						DLL_ClearTimer();
						
						DLL_ENCLIB_VOICE Dencryp_str;
						memset(&Dencryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
						Dencryp_str.FrameNum = VOICE_TERM_FRAME;
						Dencryp_str.ValidFlag = INVALID;
						DecryptFunc_Voice(&Dencryp_str);//LC_Ter清除相关加密信息
	                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][CALL_VOICE_U][Dencryption] TIM  ai data over time, mission complete!", _F_);
					}
					
					if (uWaitVoiceTimes == uCallWait)//301ms-360ms
					{
						DLL_ENCLIB_VOICE Dencryp_str;
						memset(&Dencryp_str, 0x00, sizeof(DLL_ENCLIB_VOICE));
						memcpy(Dencryp_str.Payload,s_au2AmbeEraData,VOICE_LEN);
						Dencryp_str.FrameNum = VOICE_A_FRAME;
						Dencryp_str.ValidFlag = INVALID;
						DecryptFunc_Voice(&Dencryp_str);//A帧密钥流同步函数,且不发送语音上行
	                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][CALL_VOICE_U] Add Frame_A for Dencryption Key_Flow", _F_);
					}
                }
                break;
            }
            case CALL_DATA_U:
            case CALL_DATA_D://只有GPS下行需要启动定时器
            {
                uWaitDataTimes++;

                if (uWaitDataTimes > uCallWait)
                {
                    DLL_ClearTimer();
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s][CALL_DATA_D] TIM  ai data over time, mission complete!", _F_);
                }
                break;
            }
            case CALL_WLU_U:
            case CALL_WLU_D://链路机无需定时器
            {
                uWaitWluTimes++;

                if (uWaitWluTimes > uCallWait)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] TIM  ai data over time, mission complete!", _F_);
                }
                break;
            }
            case CALL_ERR_VOICE_U:
            {
                uWaitErrTimes++;
                if (uWaitErrTimes > uCallWait)
                {
                    if (ptErrPrint->Voice == 1) 
                    {
                        ErrRateVoiceTest(NULL, SET_ERR_TER);
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }

        continue;
    }

    pthread_exit(NULL);

}

/**
 * @brief  通知MGR 当前告警状态
 *
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
static UINT8 MGR_Alarm_Update_Status(UINT32 type, UINT8 uStatus, UINT32 value)
{
    UINT32 i;

    ALARM_ITEM *ptAlarm = ptIPCShm->alarm_struct;


    if (NULL == ptAlarm)
    {
        return 0;
    }

    sem_ipc_p();
    for (i = 0; i < MGR_ALARM_MAX; (i++,ptAlarm++))
    {
        if(type == ptAlarm->alm_code)
        {
            ptAlarm->uStatus = uStatus;
            ptAlarm->value = value;
            sem_ipc_v();
            return 1;
        }
    }
    sem_ipc_v();
    return 0;
}


// 检测邻点断链告警，唯一判断是否产生断链，连续两个周期内没有备份邻点，则认为断链。
static int CheckDisconState()
{
    // 根据是否有邻点备份判断断链产生
    if (g_DllGlobalCfg.auNegrId2 == 0)  // auNegrId2=0,认为断链一次
    {
        g_DisconCnt ++;
        if ((g_DisconCnt % 2) == 0)     // 连续断链两次则告警
        {
            LOG_DEBUG(s_LogMsgId, "[DLL][%s] DISCON HAPPEN", _F_);
            return (g_discon_state = DISCON_HAPPEN);  //有断链
        }
    }
    return DISCON_RECOVER;  // 没有断链
}

/**
 * @brief  设置是否断链
 *
 * @param [in] AlarmSwitch - TURN_OFF 断链恢复
 *                         - TURN_ON  断链产生
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
void SetDisconAlarm(int AlarmSwitch)
{
    if ((AlarmSwitch == TURN_OFF) && (g_discon_state == DISCON_HAPPEN))
    {
        g_discon_state = DISCON_RECOVER;
        LOG_DEBUG(s_LogMsgId, "[DLL][%s] Tell mgr [TURN_OFF] Alarm ", _F_);
        MGR_Alarm_Update_Status(MGR_ALARM_SERVER_1, TURN_OFF, 0);//之前状态是发生，且此次消除告警，则上报关闭
    }
    else if (AlarmSwitch == TURN_ON)
    {
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] Tell mgr [TURN_ON] Alarm ", _F_);
        MGR_Alarm_Update_Status(MGR_ALARM_SERVER_1, TURN_ON, 0);
    }
}


/**
 * @brief   打印本地印邻点信息
 * @author  周大元
*/
void CheckLocalNerInfoPrint()
{
    int i;
    char acNerIdBuf[200] = {0};
    const char *aStrNer[]={
        "[0] ",  "[1] ",  "[2] ",  "[3] ",  "[4] ",
        "[5] ",  "[6] ",  "[7] ",  "[8] ",  "[9] ",
        "[10] ", "[11] ", "[12] ", "[13] ", "[14] ",
        "[15] ", "[16] ", "[17] ", "[18] ", "[19] ",
        "[20] ", "[21] ", "[22] ", "[23] ", "[24] ",
        "[25] ", "[26] ", "[27] ", "[28] ", "[29] ",
        "[30] ", "[31] "
    };
    int StrNerLen = sizeof(aStrNer)/sizeof(aStrNer[0]);
    s_uiNegrIdArrayLen = sizeof(s_auiNegrId)/sizeof(s_auiNegrId[0]); 

    for (i = 0; i < StrNerLen; i++)
    {
        if (g_DllGlobalCfg.auNegrId2 & (1<<i))
        {
            strcat(acNerIdBuf, aStrNer[i]);
        }
    }

    /* 查看邻点数组内容 */
    LOG_DEBUG(s_LogMsgId, "[DLL][%s] s_auiNegrId[0-%d]=[%#x:%#x:%#x], auNegrId2=%#lx",  _F_, (s_uiNegrIdArrayLen-1), s_auiNegrId[0], s_auiNegrId[1], s_auiNegrId[2], g_DllGlobalCfg.auNegrId2);
    LOG_DEBUG(s_LogMsgId, "[DLL][%s] local Neighbor: NodeId=%d, NerId=[ %s]",  _F_, g_DllGlobalCfg.auNodeId, acNerIdBuf);
}

/**
 * @brief   打印其他点邻点信息
 * @author  周大元
*/
void CheckOtherNerInfoPrint(unsigned char *pucData)
{
    int i;
    unsigned char  ucOtherNodeId = pucData[0]; 
    unsigned long  ulOtherNegrId2;
    char acNerIdBuf[200] = {0};
    const char *aStrNer[]={
        "[0] ",  "[1] ",  "[2] ",  "[3] ",  "[4] ",
        "[5] ",  "[6] ",  "[7] ",  "[8] ",  "[9] ",
        "[10] ", "[11] ", "[12] ", "[13] ", "[14] ",
        "[15] ", "[16] ", "[17] ", "[18] ", "[19] ",
        "[20] ", "[21] ", "[22] ", "[23] ", "[24] ",
        "[25] ", "[26] ", "[27] ", "[28] ", "[29] ",
        "[30] ", "[31] "
    };
    int StrNerLen = sizeof(aStrNer)/sizeof(aStrNer[0]);
    memcpy(&ulOtherNegrId2, &pucData[1], NER_LEN);
    for (i = 0; i < StrNerLen; i++)
    {
        if (ulOtherNegrId2 & (1<<i))
        {
            strcat(acNerIdBuf, aStrNer[i]);
        }
    }
    LOG_DEBUG(s_LogMsgId, "[DLL][%s] Other Neighbor: NodeId=%d, NerId=[ %s]",  _F_, ucOtherNodeId, acNerIdBuf);
}

/**
 * @brief   打印邻点信息
 * @author  周大元
*/
void CheckNerInfo(unsigned char *pucNodeID, unsigned long *pulNerID, const char *Prompt)
{
    const char *aStrNer[] = {
        "0:" ,  "1:" , "2:",  "3:",  "4:",
        "5:" ,  "6:" , "7:",  "8:",  "9:",
        "10:",  "11:", "12:", "13:", "14:",
        "15:",  "16:", "17:", "18:", "19:",
        "20:",  "21:", "22:", "23:", "24:",
        "25:",  "26:", "27:", "28:", "29:",
        "30:",  "31"
    };

    int i;
    char acNerIdBuf[200] = {0};
    int StrNerLen = sizeof(aStrNer)/sizeof(aStrNer[0]);
    unsigned char ucNodeID = pucNodeID[0];  // NodeID
    unsigned long ulNerID;  // NerID
    memcpy(&ulNerID, pulNerID, NER_LEN);

    for (i = 0; i < StrNerLen; i++)
    {
        if (ulNerID & (1<<i))
        {
            strcat(acNerIdBuf, aStrNer[i]);
        }
    }
    LOG_DEBUG(s_LogMsgId, "[DLL][%s][%s] NodeId=%d NerId=[%s] NerFlg=%#.8lx",  _F_, Prompt, ucNodeID, acNerIdBuf, ulNerID);
}

/**
 * @brief   打印邻点矩阵信息
 * @author  周大元
*/
void CheckNerMatrixPrint()
{
    UINT8  i;
    UINT8  NodeId;
    UINT32 NerIdFlg;
    
    for (i = 0; i < 32; i++)
    {
        if (g_DllGlobalCfg.auNerInfo2[i] != 0)
        {
            NodeId = i;
            NerIdFlg = g_DllGlobalCfg.auNerInfo2[i];
            CheckNerInfo(&NodeId, &NerIdFlg, "Local Matrix");
        }
    }
}

/**
 * @brief   打印邻点信息
 * @author  周大元
*/
void CheckNerArray()
{
    int flag = 0; // 数组元素相等
    if ((s_auiNegrId[0] != s_auiNegrId[1]) || (s_auiNegrId[0] != s_auiNegrId[2]) || (s_auiNegrId[1] != s_auiNegrId[2]))
    {
        flag = 1; // 数组元素不相等
    }
    LOG_DEBUG(s_LogMsgId, "[DLL][%s] NegrId[0-2]=[%#x:%#x:%#x], differ=%d, NegrId2=%#lx",  _F_,  s_auiNegrId[0], s_auiNegrId[1], s_auiNegrId[2], flag, g_DllGlobalCfg.auNegrId2);
}



/**
 * @brief 获取邻点突发周期随机值
 * @param [in]
 * @author  周大元
 * 
*/
void GetBurstCyc(UINT32 *pCycFirst, UINT32 *pCycLast)
{
    UINT32 BurstCyc;
#if BURST_CYC_VALUE
    BurstCyc = BURST_CYC_VALUE*1000;    // 毫秒级
#else
    BurstCyc = ptCFGShm->neighbor_period.val*60*1000;  // 突发周期赋值，最小值 1*60*1000ms
#endif
    *pCycFirst = rand() % BurstCyc;  // 取值单位周期内的某个时间点
    *pCycLast = BurstCyc - *pCycFirst;
#if BURST_CYC_VALUE
    printf("[%s:%d] BurstCyc=%lu, *pCycFirst=%lu, *pCycLast=%lu\n", _F_, __LINE__, BurstCyc, *pCycFirst, *pCycLast);
#endif
}


/**
 * @brief 当前是否摇晕
 * @param [in]
 * @author  周大元
 * 
*/
int IsNasStun()
{
    return g_DllGlobalCfg.auStunFlag;
}

/**
 * @brief 当前是否摇避
 * @param [in]
 * @author  周大元
 * 
*/
int IsNasKill()
{
    return g_DllGlobalCfg.auKillFlag;
}


/**
 * @brief 邻点突发开关是否打开
 * @param [in]
 * @author  周大元
 * 
*/
int IsNegrBurstSwitch()
{
    return ptCFGShm->start_neighbor.val;
}


/**
 * @brief  获取当前DLL状态
 * @param [in]
 * @author  周大元
 * 
*/
int GetCallState()
{
    return uCallState;
}

/**
 * @brief  获取当前FPGA状态
 * @param [in]
 * @author  周大元
 * 
*/
int GetFpgaFollowState()
{
    return p_DllFpgaShm->FollowEn;
}



/**
 * @brief  邻点突发
 * @param [in]
 * @author  周大元
 * 
*/
void NegrBurstStart()
{
    UINT16 u2CRC;
    NAS_NEGR_BURST_T NegrBurst;
    memset(&NegrBurst, 0, sizeof(NAS_NEGR_BURST_T));
    NegrBurst.NodeId = g_DllGlobalCfg.auNodeId;
    NegrBurst.NegrId = g_DllGlobalCfg.auNegrId2;
    u2CRC = ALG_Crc16((UINT8 *)&NegrBurst, CSBK_LEN);

    u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
    NegrBurst.Crc[0] = (UINT8)(u2CRC >> 8);
    NegrBurst.Crc[1] = (UINT8)(u2CRC & 0xff);
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));  //封装下行fpga数据帧
    ptInfData->TimeStamp = 0xffffffff;
    ptInfData->SlotNum = S_1;
    ptInfData->TxFreq1 = 1;
    ptInfData->TxFreq2 = 0;
    ptInfData->tDataLink[F_1].MsgType = DI_MSG_NEAR;
    ptInfData->tDataLink[F_1].FrmType = FT_VOICE_NO;
    ptInfData->tDataLink[F_1].CC = g_DllGlobalCfg.auWLUCC;
    ptInfData->tDataLink[F_1].PI = g_DllGlobalCfg.auPI;
    ptInfData->tDataLink[F_1].DataType = DI_MSG_NEAR;
    ptInfData->tDataLink[F_1].DataLen = CSBK_LEN+2;
    memcpy(ptInfData->tDataLink[F_1].PayLoad, &NegrBurst, (CSBK_LEN+2));
    ODP_SendInfData(ptInfData, S_NEGR_BST); // 突发
}


/**
 * @brief  邻点周期自加
 * @param [in]
 * @author  周大元
 * 
*/
void IncNgbrCycCnt()
{
    s_NerCycCnt++;   // 不会被清零的计数器，只记录周期数
    g_BurstCnt++;    // 会被其他业务清零的计数器
}



/**
 * @brief  邻点信息是否可以读取
 * @param [in]
 * @author  周大元
 * 每两个周期认为接收到一次邻点突发
*/
int IsGetNgbrInfoReady()
{
    return (s_NerCycCnt % 2 == 0) && (s_NerCycCnt != 0);  // 偶数周期查询一次，首次除外
}

/**
 * @brief   备份当前邻点信息
 * @param [in]
 * @author  周大元
*/
void SaveNgbrInfo()
{
    unsigned char i;
    unsigned int InValidFlag = 0;
    unsigned char MoveNum = 1; // 取值范围 MoveNumq < ARRAY_NEGR_LEN
    unsigned int sum = 0;
    s_uiNegrIdArrayLen = sizeof(s_auiNegrId)/sizeof(s_auiNegrId[0]);

    /*
    ** 如果在正常运行时， g_BurstCnt=0 or 1, 说明有语音介入。
    ** 则认为当前邻点信息受到干扰
    */
    if (g_BurstCnt < 2)
    {
        if ((tDllPrint->PrintLv == PRINT_DEBUG) || (tDllPrint->PrintLv == PRINT_ALL))
        {
            LOG_DEBUG(s_LogMsgId,"[DLL][%s] NerCycCnt=%lu, BurstCnt=%lu", _F_, s_NerCycCnt, g_BurstCnt);
        }
        g_DllGlobalCfg.auNegrId1 = ~0;  // 如果g_BurstCnt=0 或者  g_BurstCnt=1, 则赋值全 (~0 == 0xffffffff), 本次记录为无效
    }

    /* 存入新的邻点信息，数组整体右移 更新一次 */
    for (i = 0; i < s_uiNegrIdArrayLen-MoveNum; i++)
    {
        s_auiNegrId[i] = s_auiNegrId[i + MoveNum];
    }
    s_auiNegrId[s_uiNegrIdArrayLen-1] = g_DllGlobalCfg.auNegrId1; // 存储新值

    /* 过滤无效邻点信息*/
    for (i = 0; i < s_uiNegrIdArrayLen; i++)
    {
        if (s_auiNegrId[i] != (unsigned int )~0)
        {
            sum |= s_auiNegrId[i];
        }
        else
        {
            InValidFlag |= (1<<i); // 设置bit标志位，邻点无效标志。
        }
    }

    /* 如果连续三次，收到的邻点信息均为无效值，则认为当前邻点信息确实为无效 */
    if (InValidFlag == 0x7)  // 数组s_auiNegrId[]内部为 [0xffffffff: 0xffffffff: 0xffffffff ] 
    {
        sum = ~0;
    }

    // 备份本地邻点信息
    g_DllGlobalCfg.auNegrId2 = sum;
    memcpy(g_DllGlobalCfg.auNerInfo2, g_DllGlobalCfg.auNerInfo1, sizeof(g_DllGlobalCfg.auNerInfo1));
    // 清除本地邻点信息
    g_DllGlobalCfg.auNegrId1 = 0;
    memset(g_DllGlobalCfg.auNerInfo1, 0, sizeof(g_DllGlobalCfg.auNerInfo1));

    if ((tDllPrint->PrintLv == PRINT_DEBUG) || (tDllPrint->PrintLv == PRINT_ALL))
    {
        CheckNerArray();   // 查看邻点数组
        CheckNerInfo(&g_DllGlobalCfg.auNodeId, &g_DllGlobalCfg.auNegrId2, "Local Bak");
    }
}



/**
 * @brief   邻点主动上报开关是否开启
 * @param [in]
 * @author  周大元
*/
int IsNgbrReportSwitch()
{
    return ptCFGShm->neighbor_report_ai.val;
}

/**
 * @brief   邻点主动上报
 * @param [in]
 * @author  周大元
*/
void NgbrReportStart()
{
    NAS_AI_PAYLOAD NasAiData;
    memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));
    NasAiData.cmd_code = CMO_CODE_NER_REPORT;
    NasAiData.nm_type = NM_TYPE_CENTER;
    NasAiData.op_code = OP_CODE_GET_ACK;
    NasAiData.src_id = 31;  // 11111
    NasAiData.dst_id = 31;  // 11111
    NasAiData.data[0] = g_DllGlobalCfg.auNodeId;
    memcpy(&NasAiData.data[1], (UINT8 *)&g_DllGlobalCfg.auNegrId2, NER_LEN);
    NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

    //封装下行fpga数据帧
    memset(ptInfData, 0, sizeof(NAS_INF_DL_T));
    ptInfData->TimeStamp = 0xffffffff;
    ptInfData->SlotNum = S_1;
    ptInfData->TxFreq1 = 1;
    ptInfData->TxFreq2 = 0;
    ptInfData->tDataLink[F_1].MsgType = DI_MSG_WLU;
    ptInfData->tDataLink[F_1].FrmType = FT_VOICE_NO;
    ptInfData->tDataLink[F_1].CC = g_DllGlobalCfg.auWLUCC;
    ptInfData->tDataLink[F_1].PI = g_DllGlobalCfg.auPI;
    ptInfData->tDataLink[F_1].DataType = DI_MSG_WLU;
    ptInfData->tDataLink[F_1].DataLen  = CSBK_LEN+2;
    memcpy(ptInfData->tDataLink[F_1].PayLoad, &NasAiData, (CSBK_LEN+2));
    // 主动上报4份 by zhoudayuan
    ODP_SendInfData(ptInfData, S_NEGR_RPT);         // 4-邻点上报
    ODP_SendInfData(ptInfData, S_NEGR_RPT);         // 3-邻点上报
    ODP_SendInfData(ptInfData, S_NEGR_RPT);         // 2-邻点上报
    ODP_SendInfData(ptInfData, S_NEGR_RPT);         // 1-邻点上报
}






/**
 * @brief   数据链路层邻点突发线程
 * @param [in]
 * @author  周大元
*/
void *DLL_NerBurstTask(void * p)
{
    UINT32 BurstCycFirst;  // 毫秒级
    UINT32 BurstCycLast;   // 毫秒级
    sleep(30);  // 目的是等待其他模块准备就绪
    while (1)
    {
        GetBurstCyc(&BurstCycFirst, &BurstCycLast); // 获取前周期 后周期
        delay(BurstCycFirst);  // 前周期延时
        if ((IsNasKill() == 1) || (IsNasStun() == 1)) // 遥晕遥毙是否使能
        {
            continue;
        }

        if (IsNegrBurstSwitch() == 0)  // 邻点开关是否打开
        {
            continue;
        }

        /* 邻点突发 */
        if ((GetCallState() == CALL_IDLE) && (GetFpgaFollowState() == FPGA_IDLE))
        {
            NegrBurstStart();  // 启动邻点突发
        }
        delay(BurstCycLast);   // 后半周期延时


        /* 邻点信息保存 */
        if (IsGetNgbrInfoReady() == 1)
        {
            SaveNgbrInfo();  // 保存邻点信息
        }

        /* 邻点上报 */ 
        if ((IsGetNgbrInfoReady() == 1)  && (IsNgbrReportSwitch() == 1))
        {
            if (BurstCycLast < 5)  // 上报和突发之间 间隔5S
            {
                sleep(5);
            }

            if ((GetCallState() == CALL_IDLE) && (GetFpgaFollowState() == FPGA_IDLE) && (IsNegrBurstSwitch() == 1))
            {
                NgbrReportStart();
            }
        }

        /* 断链告警 */ 
        if (IsGetNgbrInfoReady() == 1)  
        {
            if ((GetCallState() == CALL_IDLE) && (GetFpgaFollowState() == FPGA_IDLE) && (IsNegrBurstSwitch() == 1))
            {
                if (CheckDisconState() == DISCON_HAPPEN)
                {
                    SetDisconAlarm(TURN_ON);  // 开启告警
                    g_DisconRecoverPrintFlg = 0;  // 开打印
                }
            }
        }

        IncNgbrCycCnt(); // 记录邻点突发周期
    }
    pthread_exit(NULL);
}


/**
 * @brief   秒级定时器
 *
 * @param [in] seconds       秒参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void seconds_sleep(unsigned long seconds)
{
    struct timeval tv;
    tv.tv_sec=seconds;
    tv.tv_usec=0;
    int err;
    do
    {
       err=select(0,NULL,NULL,NULL,&tv);

    }while(err<0 && errno==EINTR);
}

/**
 * @brief   毫秒级定时器
 *
 * @param [in] seconds       毫秒参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void milliseconds_sleep(unsigned long mSec)
{
    struct timeval tv;
    tv.tv_sec=mSec/1000;
    tv.tv_usec=(mSec%1000)*1000;
    int err;
    do
    {
       err=select(0,NULL,NULL,NULL,&tv);

    }while(err<0 && errno==EINTR);
}

/**
 * @brief   微秒级定时器
 *
 * @param [in] seconds       微笑秒参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void microseconds_sleep(unsigned long uSec)
{
    struct timeval tv;
    tv.tv_sec=uSec/1000000;
    tv.tv_usec=uSec%1000000;
    int err;
    do
    {
        err=select(0,NULL,NULL,NULL,&tv);

    }while(err<0 && errno==EINTR);
}


