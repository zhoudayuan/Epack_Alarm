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


/******************************************************************************
  *   全局变量定义
  *   *************************************************************************/
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

    uLockFreq = 0xff;
    uLockSlot = 0xff;

    tDllPrint = (DLL_PRINT_T *)ptIPCShm->dll_printf;
    memset((UINT8 *)tDllPrint, 0x00, sizeof(DLL_PRINT_T));
    tDllPrint->FrqSlt = 0;
    tDllPrint->WorkMode = 0;

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
    if (p_DllFpgaShm < 0)
    {
        printf("[DLL] FpgaConfig  map fpga shm addr error!");
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

    DLL_SyncFpgaCfg();

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
    int status;

    DLL_GlobalConfig();
    status = DLL_FpgaConfig();
    if (status != 0)
    {
        LOG_ERROR(s_LogMsgId,"[DLL][%s] DLL_FpgaConfig error", _F_);
        return -1;
    }
    DLL_ClearTimer();

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
    uTimerSwitch = 0;
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

    if (p_DllFpgaShm->LockFlag == 1)
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
void * DLL_TimerTask(void * p)
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

        DLL_SyncFpgaCfg();
        if (0 == p_DllFpgaShm->FollowEn)
        {
            p_DllFpgaShm->WorkOver = WORK_START;
            uLockFreq = 0xff;
            uLockSlot = 0xff;
            memset(g_DllGlobalCfg.auUploadLC, 0xff, (FLC_LEN+3));
        }

        GpsTimSync--;
        if (GpsTimSync < 0)
        {
            DLL_SyncGpsTime();
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
            case CALL_VOICE_U:
            case CALL_VOICE_D:
            {
                uWaitVoiceTimes++;

                if (uWaitVoiceTimes > uCallWait)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] TIM  ai data over time, mission complete!", _F_);
                }
                break;
            }
            case CALL_DATA_U:
            case CALL_DATA_D:
            {
                uWaitDataTimes++;

                if (uWaitDataTimes > uCallWait)
                {
                    DLL_ClearTimer();
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] TIM  ai data over time, mission complete!", _F_);
                }
                break;
            }
            case CALL_WLU_U:
            case CALL_WLU_D:
            {
                uWaitWluTimes++;

                if (uWaitWluTimes > uCallWait)
                {
                    LOG_DEBUG(s_LogMsgId,"[DLL][%s] TIM  ai data over time, mission complete!", _F_);
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

#if 1
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
#endif



// 检测邻点断链告警，唯一判断是否产生断链，连续两个周期内没有备份邻点，则认为断链。
static int check_discon_state()
{
    if (ptCFGShm->start_neighbor.val == 0)  // 判断邻点功能是否打开
    {
        return DISCON_DISABLE;
    }

    // 根据是否有邻点备份判断断链产生
    if (g_DllGlobalCfg.auNegrId2 == 0)  // auNegrId2=0,认为断链一次
    {
        g_DisconCnt ++;
        if ((g_DisconCnt % 2) == 0)     // 连续断链两次则告警
        {
//            printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA====DISCON HAPPEN==AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
            LOG_DEBUG(s_LogMsgId, "[DLL][%s] DISCON HAPPEN", _F_);
            return (g_discon_state = DISCON_HAPPEN);  //有断链
        }
    }
    return DISCON_RECOVER;  // 没有断链
}


void set_alarm_discon_switch(int AlarmSwitch)
{
    if ((AlarmSwitch == TURN_OFF) && (g_discon_state == DISCON_HAPPEN))
    {
        g_discon_state = DISCON_RECOVER;
//        printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC====Tell mgr [TURN_OFF] Alarm==CCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
        LOG_DEBUG(s_LogMsgId, "[DLL][%s] Tell mgr [TURN_OFF] Alarm ", _F_);
        MGR_Alarm_Update_Status(MGR_ALARM_SERVER_1, TURN_OFF, 0);
    }
    else if (AlarmSwitch == TURN_ON)
    {
//      printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB====Tell mgr [TURN_ON] Alarm==BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
        LOG_DEBUG(s_LogMsgId,"[DLL][%s] Tell mgr [TURN_ON] Alarm ", _F_);
        MGR_Alarm_Update_Status(MGR_ALARM_SERVER_1, TURN_ON, 0);
    }
}


/**
 * @brief   数据链路层邻点突发线程
 *          2个BurstCyc上报一次，
 *          2个上报时间得出是否产生断链
 * @param [in] p       传递给线程start_routine的参数
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
*/
void * DLL_NerBurstTask(void * p)
{
    int ret;
    UINT8 DstId;
    UINT8 SrcId;
    UINT16 u2CRC;
    UINT16 LeftDelay = 0;
    UINT16 BurstCyc = g_DllGlobalCfg.auNegrCyc*60;      //秒
    UINT32 BurstCnt = 0;

    struct timeval tv;
    srand((unsigned int)time( NULL ));
    NAS_NEGR_BURST_T NegrBurst;
    NAS_AI_PAYLOAD NasAiData;

    if (BurstCyc < 60)
    {
        BurstCyc = 60;
    }

    while(1)
    {
        BurstCyc = ptCFGShm->neighbor_period.val*60;
        tv.tv_sec = rand() % BurstCyc;
        tv.tv_usec = 0;
        LeftDelay = BurstCyc - tv.tv_sec;

        if (BurstCnt == 0)
        {
            sleep(BurstCyc);
        }

        ret = select(0, NULL, NULL, NULL, &tv);

        if (ret < 0)
        {
            printf("[DLL] TIM  select error\n");
            continue;
        }

        if (g_DllGlobalCfg.auStunFlag == 1 || g_DllGlobalCfg.auKillFlag == 1)
        {
            continue;
        }

        if (ptCFGShm->start_neighbor.val == 0)
        {
            continue;
        }


        // 邻点突发
        if (uCallState == CALL_IDLE && 0 == p_DllFpgaShm->FollowEn)
        {
            memset(&NegrBurst, 0, sizeof(NAS_NEGR_BURST_T));
            NegrBurst.NodeId = g_DllGlobalCfg.auNodeId;
            NegrBurst.NegrId = g_DllGlobalCfg.auNegrId2;

            u2CRC = ALG_Crc16((UINT8 *)&NegrBurst, CSBK_LEN);

            if(PDT_WORK_MODE == g_DllGlobalCfg.auWorkMode)
            {
                u2CRC = u2CRC ^ PDT_CRC_MASK_CSBK;
            }
            else
            {
                u2CRC = u2CRC ^ DMR_CRC_MASK_CSBK;
            }
            NegrBurst.Crc[0] = (UINT8)(u2CRC >> 8);
            NegrBurst.Crc[1] = (UINT8)(u2CRC & 0xff);

             //封装下行fpga数据帧
            memset(ptInfData, 0, sizeof(NAS_INF_DL_T));

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

            ODP_SendInfData(ptInfData, S_NEGR_BST);         //邻点突发

        }

        sleep(LeftDelay);
        BurstCnt++;

        // 邻点上报
        if (BurstCnt % 2 == 0)
        {
            if (LeftDelay < 5)      //邻点突发和邻点上报消息保护间隔
            {
                sleep(5);
            }

            if (uCallState == CALL_IDLE && 0 == p_DllFpgaShm->FollowEn && ptCFGShm->start_neighbor.val == 1)
            {
                memset(&NasAiData, 0, sizeof(NAS_AI_PAYLOAD));
                NasAiData.cmd_code = CMO_CODE_NER_REPORT;
                NasAiData.nm_type = NM_TYPE_CENTER;
                NasAiData.op_code = OP_CODE_GET_ACK;
                NasAiData.src_id = 31;      // 11111
                NasAiData.dst_id = 31;      // 11111

                NasAiData.data[0] = g_DllGlobalCfg.auNodeId;
                memcpy(&NasAiData.data[1], (UINT8 *)&g_DllGlobalCfg.auNegrId2, NER_LEN);
                NasAiData.crc = ALG_Crc8((UINT8 *)&NasAiData, NM_DATA_LEN);

                // 
                DstId = NasAiData.dst_id;
                SrcId = NasAiData.src_id;
                ODP_GenNasPreCSBKFun(4, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(3, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(2, &DstId, &SrcId, 1);
                ODP_GenNasPreCSBKFun(1, &DstId, &SrcId, 1);


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
                ptInfData->tDataLink[F_1].DataLen = CSBK_LEN+2;
                memcpy(ptInfData->tDataLink[F_1].PayLoad, &NasAiData, (CSBK_LEN+2));

                ODP_SendInfData(ptInfData, S_NEGR_RPT);         //邻点上报

                //备份本地邻点信息
                g_DllGlobalCfg.auNegrId2 = g_DllGlobalCfg.auNegrId1;
                memcpy(g_DllGlobalCfg.auNerInfo2, g_DllGlobalCfg.auNerInfo1, sizeof(g_DllGlobalCfg.auNerInfo1));
                //清除本地邻点信息
                g_DllGlobalCfg.auNegrId1 = 0;
                memset(g_DllGlobalCfg.auNerInfo1, 0, sizeof(g_DllGlobalCfg.auNerInfo1));

                // 检测邻点断链告警
                if (check_discon_state() == DISCON_HAPPEN)
                {
                    set_alarm_discon_switch(TURN_ON);  // 开启告警
                }
            }
        }
        continue;
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


