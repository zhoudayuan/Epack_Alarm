/**
  * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
  * @file    DLL.c
  * @brief   ʵ����·�����ݴ���ģ��ĳ�ʼ��
  * @author  ������
  * @version v1.0
  * @date    2016-7-20
  */
/*
  *   �޸���ʷ
  *   2016-7-20                      ������                     �����ļ�
  *   *************************************************************************/

/******************************************************************************
  *   ����ͷ�ļ�����
  *   *************************************************************************/
#include "dll_fun.h"
#include "mgr_common.h"


/******************************************************************************
  *   ȫ�ֱ�������
  *   *************************************************************************/
/**
 * @var  s_i4LogMsgId
 * @brief �������׽���
 */
extern INT32 s_LogMsgId;

/**
 * @var  pLogTxtFd
 * @brief �ļ����
 */
extern FILE *pLogFd;

/**
 * @var ptIPCShm
 * @brief �����ڴ�ϵͳ��Ϣ
*/
extern SHM_IPC_STRU *ptIPCShm;

/**
 * @var ptCFGShm
 * @brief �����ڴ�ϵͳ��Ϣ
 */
extern SHM_CFG_STRU *ptCFGShm;

/**
 * @var  g_PdpUBuf
 * @brief ���з������ݻ���
 */
extern PDP_UT g_PdpUBuf;

/**
 * @var  g_PdpDBuf
 * @brief ���з������ݻ���
 */
extern PDP_DT g_PdpDBuf;

/**
 * @var OdpPid
 * @brief  �����߳�PID
 */
pthread_t OdpPid;

/**
 * @var IdpPid
 * @brief  �����߳�PID
 */
pthread_t IdpPid;

/**
 * @var NeighborPid
 * @brief  �ڵ�ͻ���߳�PID
 */
pthread_t NeighborPid;

/**
 * @var TimerPid
 * @brief  ��ʱ���߳�PID
 */
pthread_t TimerPid;

/**
 * @var uTimerSwitch
 * @brief  ��ʱ������
 */
UINT8 uTimerSwitch;

/**
 * @var uLockFreq
 * @brief  ҵ������Ƶ��
 */
UINT8 uLockFreq;

/**
 * @var uLockSlot
 * @brief  ҵ������ʱ϶
 */
UINT8 uLockSlot;

/**
 * @var uGpsFlag
 * @brief  GPSҵ����
 */
UINT8 uGpsFlag;

/**
 * @var uRelayFlag
 * @brief  ת�����
 */
UINT8 uRelayFlag;

/**
 * @var uCallState
 * @brief  ҵ��״̬��ʾ
 */
UINT8 uCallState;

/**
 * @var uCallWait
 * @brief  ҵ��ȴ�ʱ��
 */
UINT16 uCallWait;

/**
 * @var uWaitCallTimes
 * @brief  ���еȴ���ʱ
 */
UINT32 uWaitVoiceTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  ���еȴ���ʱ
 */
UINT32 uWaitDataTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  ���еȴ���ʱ
 */
UINT32 uWaitNearTimes = 0;

/**
 * @var uWaitCallTimes
 * @brief  ���еȴ���ʱ
 */
UINT32 uWaitWluTimes = 0;

/**
 * @var CCLSocket
 * @brief ���п��Ʋ�socket
 */
int CCLSocket;

/**
 * @var INFSocket
 * @brief �ӿ������socket
 */
int INFSocket;

/**
 * @var DllOdpAddr
 * @brief DLLsocket���е�ַ�ṹ
 */
struct sockaddr_in DllOdpAddr;

/**
 * @var DllIdpAddr
 * @brief DLLsocket���е�ַ�ṹ
 */
struct sockaddr_in DllIdpAddr;

/**
 * @var RecvAddr
 * @brief socket���յ�ַ�ṹ
 */
struct sockaddr_in RecvAddr;

/**
 * @var CclAddr
 * @brief CCLsocket��ַ�ṹ
 */
struct sockaddr_in CclAddr;

/**
 * @var InfAddr
 * @brief INFsocket��ַ�ṹ
 */
struct sockaddr_in InfAddr;

/**
 * @var AddrSize
 * @brief socket��ַ����
 */
socklen_t AddrSize = sizeof(struct sockaddr_in);

/**
 * @var ptCclData
 * @brief ���к��п�������ָ��
 */
DLL_CCL_UL_T *ptCclData;

/**
 * @var ptInfData
 * @brief ���нӿ���������ָ��
 */
NAS_INF_DL_T *ptInfData;

/**
 * @var g_DllGlobalCfg
 * @brief ȫ��������
 */
DLL_GLB_CFG_T g_DllGlobalCfg;

/**
 * @var p_DllFpgaShm
 * @brief ���Ӳ��FPGA�����ڴ�
 */
DLL_FPGA_SHM_T *p_DllFpgaShm;

/**
 * @var tDllPrint
 * @brief DLL��ӡ����
 */
DLL_PRINT_T *tDllPrint;


/**
 * @var g_discon_state
 * @brief ����״̬
 */
UINT8 g_discon_state = 0;
    

/**
 * @var g_DisconCnt
 * @brief ����������
 */
UINT8 g_DisconCnt = 0;


/******************************************************************************
  *   �ڲ�����ʵ��
  *   *************************************************************************/
/**
 * @brief  DLL Э��ջȫ�ֳ�ʼ������
 *
 * @param [in] void
 * @author  ������
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
 * @brief  DLL Э��ջFPGA���ó�ʼ������
 *
 * @param [in] void
 *
 * @return  int         0:�ɹ���-1:ʧ��
 * @author  ������
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
    //p_DllFpgaShm->FollowEn = 0;           //��fpga����
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
 * @brief   ������·���ʼ������
 *
 * @return  int         0:�ɹ���-1:ʧ��
 * @author  ������
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
 * @brief   ������·�����ö�ʱ��
 * @param [in] CallState             ҵ������
 * @param [in] CallWait              ��ʱʱ�����
 *
 * @author  ������
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
 * @brief   ������·�������ʱ��
 *
 * @author  ������
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
 * @brief   ������·��ͬ��GPSʱ��
 *
 * @author  ������
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
 * @brief   ������·�����fpga�����ڴ�
 *
 * @author  ������
 * @since   trunk.00001
 * @bug
*/
void DLL_SyncFpgaCfg(void)
{
    msync((void *)FPGA_SHM_ADDR, FPGA_SHM_LEN, MS_SYNC);
}


/**
 * @brief   ������·�㶨ʱ���߳�
 *
 * @param [in] p       ���ݸ��߳�start_routine�Ĳ���
 * @author  ������
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



// ����ڵ�����澯��Ψһ�ж��Ƿ������������������������û�б����ڵ㣬����Ϊ������
static int check_discon_state()
{
    if (ptCFGShm->start_neighbor.val == 0)  // �ж��ڵ㹦���Ƿ��
    {
        return DISCON_DISABLE;
    }

    // �����Ƿ����ڵ㱸���ж϶�������
    if (g_DllGlobalCfg.auNegrId2 == 0)  // auNegrId2=0,��Ϊ����һ��
    {
        g_DisconCnt ++;
        if ((g_DisconCnt % 2) == 0)     // ��������������澯
        {
//            printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA====DISCON HAPPEN==AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
            LOG_DEBUG(s_LogMsgId, "[DLL][%s] DISCON HAPPEN", _F_);
            return (g_discon_state = DISCON_HAPPEN);  //�ж���
        }
    }
    return DISCON_RECOVER;  // û�ж���
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
 * @brief   ������·���ڵ�ͻ���߳�
 *          2��BurstCyc�ϱ�һ�Σ�
 *          2���ϱ�ʱ��ó��Ƿ��������
 * @param [in] p       ���ݸ��߳�start_routine�Ĳ���
 * @author  ������
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
    UINT16 BurstCyc = g_DllGlobalCfg.auNegrCyc*60;      //��
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


        // �ڵ�ͻ��
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

             //��װ����fpga����֡
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

            ODP_SendInfData(ptInfData, S_NEGR_BST);         //�ڵ�ͻ��

        }

        sleep(LeftDelay);
        BurstCnt++;

        // �ڵ��ϱ�
        if (BurstCnt % 2 == 0)
        {
            if (LeftDelay < 5)      //�ڵ�ͻ�����ڵ��ϱ���Ϣ�������
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


                //��װ����fpga����֡
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

                ODP_SendInfData(ptInfData, S_NEGR_RPT);         //�ڵ��ϱ�

                //���ݱ����ڵ���Ϣ
                g_DllGlobalCfg.auNegrId2 = g_DllGlobalCfg.auNegrId1;
                memcpy(g_DllGlobalCfg.auNerInfo2, g_DllGlobalCfg.auNerInfo1, sizeof(g_DllGlobalCfg.auNerInfo1));
                //��������ڵ���Ϣ
                g_DllGlobalCfg.auNegrId1 = 0;
                memset(g_DllGlobalCfg.auNerInfo1, 0, sizeof(g_DllGlobalCfg.auNerInfo1));

                // ����ڵ�����澯
                if (check_discon_state() == DISCON_HAPPEN)
                {
                    set_alarm_discon_switch(TURN_ON);  // �����澯
                }
            }
        }
        continue;
    }

    pthread_exit(NULL);
}

/**
 * @brief   �뼶��ʱ��
 *
 * @param [in] seconds       �����
 * @author  ������
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
 * @brief   ���뼶��ʱ��
 *
 * @param [in] seconds       �������
 * @author  ������
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
 * @brief   ΢�뼶��ʱ��
 *
 * @param [in] seconds       ΢Ц�����
 * @author  ������
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


