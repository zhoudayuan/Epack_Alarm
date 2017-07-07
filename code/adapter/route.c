/**
 * Copyright (C), 1993-2014, Hytera Comm. Co., Ltd.
 * @file    route.c
 * @brief   适配模块
 * @author  ztt
 * @version v1.0
 * @date    2016-7-26
 */
 #undef TASK_NAME
#define TASK_NAME	"ROUTE"
 #include "route.h"
 
 /**
 * @var s_i4LogMsgId
 * @brief 命令行套接字
 */
static INT32 s_i4LogMsgId;
 /**
 * @var pLogTxtFd
 * @brief 文件句柄
 */
static FILE *pLogTxtFd = NULL;

 #include "log.h"
 /**
 * @var ptIPCShm
 * @brief 共享内存系统信息
 */
 SHM_IPC_STRU *ptIPCShm = NULL;
 /**
 * @var ptRouteNmShm
 * @brief 共享内存配置信息
 */
 SHM_CFG_STRU *ptRouteCfgShm = NULL;
 /**
 * @var tRoutePrintf
 * @brief打印开关
 */
static  ROUTE_PRINTF *ptRoutePrintf;
 /**
 * @var s_tDBCListMutex
 * @brief 发送链表访问互斥信号
 */
static pthread_mutex_t s_tDBCListMutex= PTHREAD_MUTEX_INITIALIZER;
 /**
 * @var ROUTE_AI_SEND_PORT
 * @brief空口数据分发端口号
 */
static UINT32 ROUTE_AI_SEND_PORT[ROUTE_AI_RECV_TYPE_MAX];
/**
 * @var s_tTcpRecvask
 * @brief TCP接收线程线程id
 */
static pthread_t        s_tRouteAIRecvTask;
/**
 * @var s_tRouteSendTask
 * @brief 下行线程任务
 */
static pthread_t        s_tRouteSendTask;
/**
 * @var s_tRouteNMRecvTask
 * @brief 网管接受线程
 */
static pthread_t        s_tRouteNMRecvTask;
/**
 * @var s_tRouteServerRecvTask
 * @brief 业务接收线程
 */
static pthread_t        s_tRouteServerRecvTask;
/**
 * @var s_atNode
 * @brief 节点存储区
 */
static ROUTE_DBC_NODE_T s_atNode[DB_NODE_MAX];
/**
 * @var s_tListUnused
 * @brief 空闲节点
 */
static ROUTE_DBC_NODE_T s_tListUnused;
/**
 * @var s_i4Sem
 * @brief 要下行数据信号量	
 */
static INT32 s_i4Sem;//
/**
 * @var s_u4AIFd
 * @brief 空口设备句柄
 */
static UINT32 s_u4AIFd;//
/**
 * @var s_RouteLocalId
 * @brief 本机ID号
 */
static UINT32    s_RouteLocalId = 1;
 /**
 * @var Route_AI
 * @brief空口数据发送socket
 */
 struct sockaddr_in Route_AI;
/**
 * @var INF_DLL_ODP_DATA_T
 * @brief 下行数据链表
 */
INF_DLL_ODP_DATA_T s_tListODP;
/**
 * @var s_tErrorData
 * @brief 错误日志
 */
ROUTE_DEBUG_DATA_T s_tErrorData[ROUTE_ERROR_NODE_MAX];
/**
 * @var g_RouteDebugIndex
 * @brief 错误日志节点号
 */
UINT32 g_RouteDebugIndex = 0;
/**
 * @var u4MsgId
 * @brief 回环使用
 */
#ifdef TEST
static signed int u4MsgId;
#endif
//////////////////////////DEBUG///////////////////////////////
/**
 * @brief   Route_Debug错误日志模块
 *
 * @param [in] u4Type 错误类型      
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
void Route_Debug(ROUTE_ERROR_DEBUG_TYPE_E u4Type,UINT8 *pInData)
{
	s_tErrorData[g_RouteDebugIndex%ROUTE_ERROR_NODE_MAX].ErrorType = u4Type;
	s_tErrorData[g_RouteDebugIndex%ROUTE_ERROR_NODE_MAX].Index = g_RouteDebugIndex;
	memcpy(s_tErrorData[g_RouteDebugIndex%ROUTE_ERROR_NODE_MAX].Data,pInData,ROUTE_DATA_LEN_MAX);
	g_RouteDebugIndex++;
	
	return;
}
//////////////////////////SHM MEM////////////////////////////
/**
 * @brief   共享内存
 *
 * @param [in] id 共享内存标识号
 * @param [in] size 指定大小
 *
 * @return  void *共享内存首地址
 * @author  ztt
 * @since
 * @bug
 */
void * Route_Get_Shm(int id, size_t size)
{
 int i4ShmId;
 void *shmaddr = NULL;
 key_t key;

 key = ftok(FTOK_F_NAME, id);
 if (key < 0)
 {
  LOG_WFile(pLogTxtFd,"[Route_Get_Shm key error!!!]");
  return NULL;
 }

  i4ShmId = shmget(key, 0, 0);
  if (-1 == i4ShmId)  
  {
  	i4ShmId = shmget(key, size, 0666 | IPC_CREAT); //创建共享内存 
  	if(i4ShmId < 0)
  	{
		LOG_ERROR(s_i4LogMsgId,"[Route_Get_Shm creat error!!!]");
    		return NULL;
	}
#if 0
    LOG_WFile(pLogTxtFd,"[Route_Get_Shm GET error!!!]");
    return NULL;
#endif
  }
   
 shmaddr = shmat(i4ShmId, NULL, 0);
  if (-1 == (int)(shmaddr))
  {
    LOG_WFile(pLogTxtFd,"[Route_Get_Shm MAT error!!!]");
    return NULL;
  }
 return shmaddr;
  
}
//////////////////////////Semaphore////////////////////////////
/**
 * @brief   Route_Get_Semaphore
 *
 * @param [in] id 信号量标示
 *
 * @return  UINT32信号量值
 * @author  ztt
 * @since
 * @bug
 */
UINT32  Route_Get_Semaphore(UINT32 id)
{
	int u4SemId;
	key_t key;
	union semun arg;

	key = ftok(FTOK_F_NAME, id);
	if (key < 0)
	{
		LOG_WFile(pLogTxtFd,"[Route_Get_Semaphore key error!!!]");
		exit(1);
	}

	u4SemId = semget(key, 1, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);
	if (u4SemId >= 0)
	{
		arg.val = 1;
		if (semctl(u4SemId, 0, SETVAL, arg) < 0)
			exit(1);
	}
	else if (errno == EEXIST)
	{
		u4SemId = semget(key, 0, 0);
		if (-1 == u4SemId)
			exit(1);
	} else
		exit(1);

	return u4SemId;
}
/**
 * @brief   Route_Set_Sem_Value
 *
 * @param [in] semid 信号量值
 * @param [in] val     信号量初始值
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
UINT32 Route_Set_Sem_Value(int semid,int val)
{
	union semun arg;

	if (semid < 0)
		return -1;
	arg.val = val;
	return semctl(semid, 0, SETVAL, arg);
}
/**
 * @brief   Route_Lock_Semaphore
 *
 * @param [in] u4SemId 信号量值
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
void Route_Lock_Semaphore(UINT32 u4SemId)
{
	int ret;
	struct sembuf tSemLock;

	tSemLock.sem_num = 0;
	tSemLock.sem_op = -1;
	tSemLock.sem_flg = SEM_UNDO;
	ret = semop(u4SemId, &tSemLock, 1);
	if (-1 == ret)
	{
		LOG_WFile(pLogTxtFd,"[Route_Lock_Semaphore error!!!]");
		exit(1);
	}

	return;
}
/**
 * @brief   Route_Unlock_Semaphore
 *
 * @param [in] u4SemId 信号量值
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
void Route_Unlock_Semaphore(int u4SemId)
{
	int ret;
	struct sembuf tSemUnlock;

	tSemUnlock.sem_num = 0;
	tSemUnlock.sem_op = 1;
	tSemUnlock.sem_flg = (IPC_NOWAIT|SEM_UNDO);
	ret = semop(u4SemId, &tSemUnlock, 1);
	if (-1 == ret)
	{
		LOG_WFile(pLogTxtFd,"[Route_Unlock_Semaphore error!!!]");
		exit(1);
	}

	return ;
}
//////////////////////////SLEEP/////////////////////////////////
/**
 * @brief   被中断打断的休眠函数
 *
 * @param [in] second 休眠秒数      
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
void Route_Sleep(unsigned second)
{
    unsigned  left;

    left = second;
    while (left > 0)
    {
        left = sleep(left);//sleep()被中断之后会返回剩余的秒数
    }
    return;
}
//////////////////////////DBC/////////////////////////////////
/**
 * @brief   DBC_Init业务数据内存池初始化
 *
 * @param [in] void      
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug
 */
void DBC_Init(void)
{
	UINT8 i;
	memset(s_atNode,0x00,sizeof(s_atNode));

	for(i = 0;i<DB_NODE_MAX-1;i++)
	{
		s_atNode[i].ptNext = &s_atNode[i+1];
	}

	s_atNode[DB_NODE_MAX-1].ptNext = NULL;
	s_tListUnused.ptNext = &s_atNode[0];
	
	return;
}
/**
 * @brief   DBC_Alloc业务数据内存池申请节点
 *
 * @param [in] void      
 *
 * @return  DBC_NODE_T *
 * @author  ztt
 * @since
 * @bug外层加锁，申请头节点
 */
ROUTE_DBC_NODE_T * DBC_Alloc(void)
{
	ROUTE_DBC_NODE_T *ptTemp = NULL;
	
	if(NULL != s_tListUnused.ptNext)
	{
		ptTemp = s_tListUnused.ptNext;
		s_tListUnused.ptNext = ptTemp->ptNext;
		ptTemp->ptNext = NULL;
	}
	return ptTemp;
}
/**
 * @brief   DBC_Free业务数据内存池释放节点
 *
 * @param [in] ptNode 内存节点地址      
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug 外层加锁，放在头位置
 */
void DBC_Free(ROUTE_DBC_NODE_T *ptNode)
{
	ROUTE_DBC_NODE_T *ptTemp = NULL;

	memset(ptNode->auData,0x00,ROUTE_DATA_LEN_MAX);
	
	ptTemp = s_tListUnused.ptNext;
	s_tListUnused.ptNext = ptNode;
	ptNode->ptNext = ptTemp;
	
	return ;
}
/**
 * @brief   DBC_Insert业务数据插入发送链表
 *
 * @param [in] ptNode 内存节点地址      
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug 外层加锁，数据插到链表尾
 */
void DBC_Insert(ROUTE_DBC_NODE_T **pHead,ROUTE_DBC_NODE_T *ptTemp,ROUTE_DBC_NODE_T **pTail)
{
	if(NULL == *pHead)
	{
		(*pHead) = ptTemp;
		(*pTail) = ptTemp;
	}
	else
	{
		(*pTail)->ptNext = ptTemp;
		(*pTail) = ptTemp;
	}
	return;
}
/**
 * @brief   DBC_GetData从发送链表中获取数据
 *
 * @param [in] ptNode 内存节点地址      
 *
 * @return  u4SendType 数据类型
 * @author  ztt
 * @since
 * @bug 外层加锁，数据插到链表尾
 */
ROUTE_DBC_NODE_T * DBC_GetData(ROUTE_SEND_TYPE_E u4SendType)
{
	ROUTE_DBC_NODE_T *ptTemp = NULL;
	
	switch(u4SendType)
	{
		case ROUTE_SEND_TYPE_SERVER:
			ptTemp = (ROUTE_DBC_NODE_T *)s_tListODP.pServer;
			(s_tListODP.pServer) = ptTemp->ptNext;
			break;
		case ROUTE_SEND_TYPE_ALARM:
			ptTemp = (ROUTE_DBC_NODE_T *)s_tListODP.pAlarmEmergecy;
			(s_tListODP.pAlarmEmergecy) = ptTemp->ptNext;
			break;
		case ROUTE_SEND_TYPE_NM:
			ptTemp = (ROUTE_DBC_NODE_T *)s_tListODP.pNm;
			(s_tListODP.pNm) = ptTemp->ptNext;
			break;
		case ROUTE_SEND_TYPE_INVALID:
		default:		
			break;
	}
	return ptTemp;
}
/**
 * @brief   Route_Get_F1F2_Status
 *
 * @param [in] ptInData 数据
 * @param [out] pValidNum f1f2有效个数
 * @param [in] index 有效数据节点
 *
 * @return   见ROUTE_RECV_F1F2_TYPE_E
 * @author  ztt
 * @since
 * @bug 
 */
ROUTE_RECV_F1F2_TYPE_E Route_Get_F1F2_Status(UINT8 *ptInData,int *pValidNum,INT8 *index)
{
	NAS_INF_UL_T *ptData  = (NAS_INF_UL_T *)ptInData;
	
	*pValidNum = 0;
	if(NULL == ptData)
		return ROUTE_RECV_F1F2_NO_TYPE;
	if((0 ==ptData->RxFreq1)
		&&(0 ==ptData->RxFreq2))
		return ROUTE_RECV_F1F2_NO_TYPE;

	*pValidNum = 1;
	if((1 ==ptData->RxFreq1)
		&&(0 ==ptData->RxFreq2))
	{
		*index = 0;
		return ROUTE_RECV_F1F2_F1_TYPE;
	}
		
	if((0 ==ptData->RxFreq1)
		&&(1 ==ptData->RxFreq2))
	{
		*index = 1;
		return ROUTE_RECV_F1F2_F2_TYPE;
	}
		
	*pValidNum = 2;
	if((1 ==ptData->RxFreq1)
		&&(1 ==ptData->RxFreq2))
	{
		*index = 0;
		return ROUTE_RECV_F1F2_ALL_TYPE;
	}
	
	*pValidNum = 0;	
	return ROUTE_RECV_F1F2_NO_TYPE;
}
/**
 * @brief   Route_Is_Alarm_Emergecy紧急报警
 *
 * @param [in] pInData 原始包负载数据   
 *
 * @return  void
 * @author  ztt
 * @since
 * @bug 外层加锁，数据插到链表尾;需要使用mgr模块头文件
 */
UINT32 Route_Is_Alarm_Emergecy(UINT8 *pInData)
{
       ROUTE_RECV_F1F2_TYPE_E type = ROUTE_RECV_F1F2_NO_TYPE;
	NAS_INF_DL_T *ptTemp = (NAS_INF_DL_T *)pInData;
	int num  = 0;
	INT8 iIndex = -1;

	type = Route_Get_F1F2_Status(pInData,&num,&iIndex);

	switch(type)
	{
		case ROUTE_RECV_F1F2_F1_TYPE:
			if(OP_CODE_E_ALARM == ((NAS_AI_PAYLOAD *)(ptTemp->tDataLink[0].PayLoad))->op_code)
				return 1;
			break;
		case ROUTE_RECV_F1F2_F2_TYPE:
			if(OP_CODE_E_ALARM == ((NAS_AI_PAYLOAD *)(ptTemp->tDataLink[1].PayLoad))->op_code)
				return 1;
			break;
		case ROUTE_RECV_F1F2_ALL_TYPE:
			LOG_WARNING(s_i4LogMsgId,"[Route_Is_Alarm_Emergecy][f1f2]");
			if(OP_CODE_E_ALARM == ((NAS_AI_PAYLOAD *)(ptTemp->tDataLink[0].PayLoad))->op_code)
				return 1;
			if(OP_CODE_E_ALARM == ((NAS_AI_PAYLOAD *)(ptTemp->tDataLink[1].PayLoad))->op_code)
				return 1;
			break;
		case ROUTE_RECV_F1F2_NO_TYPE:
		default:	
			break;
	}
	return 0;
}
/////////////////////////空口相关//////////////////////////////////
/**
 * @brief   Route_AI_NM_Parse空口网管数据解析
 *
 * @param [in] pAIData       空口数据
 * @param [in] Route_Len    空口数据长度
 *
 * @return  见ROUTE_AI_RECV_TYPE_E
 * @author  ztt
 * @since
 * @bug
 */
ROUTE_SEND_PORT_TYPE_E Route_AI_NM_Parse(NAS_AI_PAYLOAD *ptPayload)
{
#ifdef TEST
	UINT8 uTemp;
#endif

	if(NULL == ptPayload)
		return ROUTE_SEND_TYPE_PORT_INVALID;

	if(OP_CODE_ALARM == ptPayload->op_code)//中心
	{
		return ROUTE_SEND_TYPE_PORT_REPLY;
	}
	
	if(((OP_CODE_GET_ACK == ptPayload->op_code)||(OP_CODE_SET_ACK == ptPayload->op_code))//中心
		&&(1))
	{
		return ROUTE_SEND_TYPE_PORT_REPLY;
	}
#ifdef TEST
	if(OP_CODE_SET == ptPayload->op_code)
	{
		ptPayload->op_code = OP_CODE_SET_ACK;
		uTemp = ptPayload->src_id;
		ptPayload->src_id = ptPayload->dst_id;
		ptPayload->dst_id = uTemp;
		ptPayload->nm_type = 0;	
		LOG_DEBUG(s_i4LogMsgId,"[LOOP BACK][Route_AI_NM_Parse][SID=%d,DID=%d]\n",ptPayload->src_id,ptPayload->dst_id);
		return ROUTE_SEND_TYPE_PORT_REPLY;
	}
#else
	if((1)//本地网管
		&&((OP_CODE_SET == ptPayload->op_code)||(OP_CODE_GET == ptPayload->op_code)))
	{
		return ROUTE_SEND_TYPE_PORT_CONFIG;
	}
#endif
	return ROUTE_SEND_TYPE_PORT_INVALID;
	
}
/**
 * @brief   Route_Get_AI_F1F2_Type获取要发送的端口类型
 *
 * @param [in] f1f2type       见ROUTE_RECV_F1F2_TYPE_E
 * @param [in] ptData    f1f2数据
 * @param [in] i4ValidNum    f1f2有效个数
 *
 * @return  见ROUTE_AI_RECV_TYPE_E
 * @author  ztt
 * @since
 * @bug
 */
ROUTE_AI_RECV_TYPE_E Route_Get_AI_F1F2_Type(DATA_LINK_T *ptData,int i4ValidNum)
{
	int i,i4PortType[2] = {0x00,0x00};
	int i4Result = 0;
	
	if((i4ValidNum > 2)
		||(i4ValidNum <= 0))
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_Get_AI_F1F2_Type]i4ValidNum=%d error",i4ValidNum);
		return ROUTE_AI_RECV_TYPE_MAX;
	}
		
	for(i = 0;i<i4ValidNum;i++)
	{
		if(NULL == ptData)
			return ROUTE_AI_RECV_TYPE_MAX;

		switch(ptData->MsgType)
		{
			case DI_MSG_VOICE:
			case DI_MSG_DATA:
			case DI_MSG_NEAR:
			case DI_MSG_WLU:
				i4PortType[i] = ROUTE_SEND_TYPE_PORT_SERVER;
				break;
			case DI_MSG_NM:
				i4PortType[i] = (int)Route_AI_NM_Parse((NAS_AI_PAYLOAD *)(ptData->PayLoad));
				break;
			default:
				break;				
		} 
		ptData++;//ptData = ptData + sizeof(DATA_LINK_T);
	}
	
	i4Result = i4PortType[0] | i4PortType[1];
	
	if(ptRoutePrintf->uAIUp)
	{
		 LOG_DEBUG(s_i4LogMsgId,"[AI_U][Route_Get_AI_F1F2_Type][0=%x,1=%x(0001LocalNM,0010Center,0100Server,1000Invalid),result=%x]",i4PortType[0],i4PortType[1],i4Result);
	}
	   
	if((ROUTE_SEND_TYPE_PORT_CONFIG | ROUTE_SEND_TYPE_PORT_REPLY) == i4Result)
		return ROUTE_AI_RECV_TYPE_CONFIG_REPLY;

	if((ROUTE_SEND_TYPE_PORT_CONFIG | ROUTE_SEND_TYPE_PORT_SERVER) == i4Result)
		return ROUTE_AI_RECV_TYPE_CONFIG_SERVER;

	if((ROUTE_SEND_TYPE_PORT_SERVER| ROUTE_SEND_TYPE_PORT_REPLY) == i4Result)
		return ROUTE_AI_RECV_TYPE_REPLY_SERVER;

	if(ROUTE_SEND_TYPE_PORT_CONFIG  == i4Result)
		return ROUTE_AI_RECV_TYPE_CONFIG;
	
       if(ROUTE_SEND_TYPE_PORT_REPLY== i4Result)
		return ROUTE_AI_RECV_TYPE_REPLY;

	if(ROUTE_SEND_TYPE_PORT_SERVER== i4Result)
		return ROUTE_AI_RECV_TYPE_SERVER;
	
       return ROUTE_AI_RECV_TYPE_MAX;
}
/**
 * @brief   Route_F1F2_Check
 *
 * @param [in] f1f2type       见ROUTE_RECV_F1F2_TYPE_E
 * @param [in] i4ValidNum   有效个数
 *
 * @return  见0:检查失败1:检查成功
 * @author  ztt
 * @since
 * @bug
 */
int Route_F1F2_Check(ROUTE_RECV_F1F2_TYPE_E f1f2type,int i4ValidNum,INT8 iIndex)
{
	if((iIndex < 0)||(iIndex > 1)
		||(i4ValidNum > 2)||(0 == i4ValidNum))
		return 0;
	
	switch(f1f2type)
	{
		case ROUTE_RECV_F1F2_F1_TYPE:
		case ROUTE_RECV_F1F2_F2_TYPE:
			if(1 != i4ValidNum)
				return 0;
			break;
		case ROUTE_RECV_F1F2_ALL_TYPE:
			if(2 != i4ValidNum)
				return 0;
			break;
		case ROUTE_RECV_F1F2_NO_TYPE:
		default:
			LOG_ERROR(s_i4LogMsgId,"[Route_F1F2_Check]f1f2type=%d",f1f2type);
			return 0;
	}
	return 1;
}
/**
 * @brief   Route_F1F2_Debug
 *
 * @param [in] f1f2type       见ROUTE_AI_RECV_TYPE_E
 *
 * @author  ztt
 * @since
 * @bug
 */
void Route_F1F2_Debug(ROUTE_RECV_F1F2_TYPE_E f1f2type,NAS_INF_UL_T *pData)
{
	switch(f1f2type)
	{
		case ROUTE_RECV_F1F2_NO_TYPE:
			LOG_DEBUG(s_i4LogMsgId,"[AI_U]ROUTE_RECV_F1F2_NO_TYPE");
			break;
		case ROUTE_RECV_F1F2_F1_TYPE:
			LOG_DEBUG(s_i4LogMsgId,"[AI_U]ROUTE_RECV_F1F2_F1_TYPE,MsgType=%d",pData->tDataLink[0].MsgType);
			break;
		case ROUTE_RECV_F1F2_F2_TYPE:
			LOG_DEBUG(s_i4LogMsgId,"[AI_U]ROUTE_RECV_F1F2_F2_TYPE,MsgType=%d",pData->tDataLink[1].MsgType);
			break;
		case ROUTE_RECV_F1F2_ALL_TYPE:
			LOG_DEBUG(s_i4LogMsgId,"[AI_U]ROUTE_RECV_F1F2_ALL_TYPE,MsgType=%d,%d",pData->tDataLink[0].MsgType,pData->tDataLink[1].MsgType);
			break;
		default:
			LOG_DEBUG(s_i4LogMsgId,"[AI_U][Route_F1F2_Debug error]");
	}
	return;
}
/**
 * @brief   Route_AI_Recv_Parse空口解析
 *
 * @param [in] pAIData       空口数据
 * @param [in] Route_Len    空口数据长度
 *
 * @return  见ROUTE_AI_RECV_TYPE_E
 * @author  ztt
 * @since
 * @bug
 */
ROUTE_AI_RECV_TYPE_E Route_AI_Recv_Parse(UINT8 *pInData,UINT32 u4RouteLen)
{
	ROUTE_AI_RECV_TYPE_E type = ROUTE_AI_RECV_TYPE_MAX;
	ROUTE_RECV_F1F2_TYPE_E f1f2type = ROUTE_RECV_F1F2_NO_TYPE;
	NAS_INF_UL_T *pAIData = (NAS_INF_UL_T *)pInData;
	INT8 iIndex = -1;//f1f2有效性索引
	int i4ValidNum = 0;
	
	if((NULL == pAIData) ||(ROUTE_DATA_LEN_MAX < u4RouteLen))
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_AI_Recv_Parse]error RevLen=%d",(int)u4RouteLen);
		return ROUTE_AI_RECV_TYPE_MAX;
	}
		
	f1f2type = Route_Get_F1F2_Status(pInData,&i4ValidNum,&iIndex);//f1f2有效性

	if(ptRoutePrintf->uAIUp)
	{
		LOG_DEBUG(s_i4LogMsgId,"[AI_U]TimeStamp=%x,slotnum=%d,RxFreq1=%d,RxFreq2=%d,recvlen=%d",pAIData->TimeStamp,pAIData->SlotNum,pAIData->RxFreq1,pAIData->RxFreq2,u4RouteLen);
		LOG_DEBUG(s_i4LogMsgId,"[AI_U]ValidNum=%d,Index=%d",i4ValidNum,iIndex);
		Route_F1F2_Debug(f1f2type,pAIData);
	}
	
	if(!Route_F1F2_Check(f1f2type,i4ValidNum,iIndex))
	{
		LOG_ERROR(s_i4LogMsgId,"[AI_U][F1F2_Check][f1f2type=%d,i4ValidNum=%d,iIndex=%d]",f1f2type,i4ValidNum,iIndex);
		return ROUTE_AI_RECV_TYPE_MAX;
	}
		
	type = Route_Get_AI_F1F2_Type((DATA_LINK_T *)&(pAIData->tDataLink[iIndex]),i4ValidNum);//通过f1f2有效性确定数据去处
	
	if(ptRoutePrintf->uAIUp)
	{
		LOG_DEBUG(s_i4LogMsgId,"[AI_U][F1F2_Type=%d][0:LocalNM;1:Center;2:Server;3:0+2;4:1+2;5:0+1;6:Invalid]",type);
	}
	return type;
}
/**
 * @brief   Route_AI_Send空口数据发送
 *
 * @param [in] Route_Type    类型
 *
 * @author  ztt
 * @since 只有0 1 2，代表三个不同去向
 * @bug
 */
void Route_AI_do_Send(UINT32 socketfd,UINT8 *pInData,UINT32 u4RouteLen,ROUTE_AI_RECV_TYPE_E u4RouteType)
{
	if(ROUTE_AI_RECV_TYPE_CONFIG_SERVER <= u4RouteType)
		return;
	
	Route_AI.sin_port = htons(ROUTE_AI_SEND_PORT[u4RouteType]);
	if(-1 == sendto(socketfd, pInData, u4RouteLen, 0, (struct sockaddr *)&Route_AI, sizeof(Route_AI)))
	{
		Route_Debug(ROUTE_ERROR_DEBUG_TYPE_SEND_LEN,pInData);
		LOG_ERROR(s_i4LogMsgId,"[Route_AI_do_Send][RouteType=%d]",u4RouteType);
	}
	return;
}
/**
 * @brief   Route_AI_Send空口数据发送
 *
 * @param [in] Route_Type    类型
 *
 * @author  ztt
 * @since
 * @bug
 */
void Route_AI_Send(UINT32 socketfd,UINT8 *pInData,UINT32 u4RouteLen,ROUTE_AI_RECV_TYPE_E u4RouteType)
{
	if(ROUTE_AI_RECV_TYPE_MAX <= u4RouteType)
		return;

	if(u4RouteType <= ROUTE_AI_RECV_TYPE_SERVER)
	{
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,u4RouteType);
	}
	else if(ROUTE_AI_RECV_TYPE_CONFIG_SERVER== u4RouteType)
	{
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_CONFIG);
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_SERVER);
	}
	else if(ROUTE_AI_RECV_TYPE_REPLY_SERVER== u4RouteType)
       {
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_REPLY);
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_SERVER);

	}
	else if(ROUTE_AI_RECV_TYPE_CONFIG_REPLY== u4RouteType)
	{
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_CONFIG);
		Route_AI_do_Send(socketfd,pInData,u4RouteLen,ROUTE_AI_RECV_TYPE_REPLY);
	}
	return;
}
/////////////////////////////空口接受//////////////////////////////
/**
 * @brief   Route_AI_Recv_Process空口接受任务
 *
 * @param [in] pvArg    unuse
 *
 * @author  ztt
 * @since
 * @bug
 */
static void *Route_AI_Recv_Process(void *pvArg)
{
	INT32 i4SocketFd;
	UINT8 auRouteBuff[ROUTE_DATA_LEN_MAX];
	UINT32 u4RouteLen; 
	ROUTE_AI_RECV_TYPE_E u4RouteType;
#ifdef TEST 
	ROUTE_MSGBUF tMsgBuf;
#endif

	i4SocketFd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > i4SocketFd)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_AI_Recv_Process][socketfd error,exit!!!]");
		exit(1);
	}
	
	LOG_DEBUG(s_i4LogMsgId,"Route_AI_Recv_Process start ...");
	while(1)
	{
#ifdef TEST //业务数据直接发从空口接收
		ptRoutePrintf->uUpBusy = 0;
		u4RouteLen = msgrcv(u4MsgId, &tMsgBuf ,ROUTE_DATA_LEN_MAX + 4, ROUTE_TEST_MSG_SERVER, 0);
		ptRoutePrintf->uUpBusy = 1;
		LOG_DEBUG(s_i4LogMsgId,"[Route_AI_Recv_Process][LOOP BACK]");
		u4RouteType = Route_AI_Recv_Parse(tMsgBuf.mtext,u4RouteLen - 4);
		Route_AI_Send(i4SocketFd,tMsgBuf.mtext,u4RouteLen - 4,u4RouteType);
#else
		ptRoutePrintf->uUpBusy = 0;
		u4RouteLen = read(s_u4AIFd,auRouteBuff,ROUTE_DATA_LEN_MAX);//阻塞方式
		ptRoutePrintf->uUpBusy = 1;
		
		u4RouteType = Route_AI_Recv_Parse(auRouteBuff,u4RouteLen);
		Route_AI_Send(i4SocketFd,auRouteBuff,u4RouteLen,u4RouteType);
#endif	
	}
	close(s_u4AIFd);
	pthread_exit(NULL);
	
}
/////////////////////////////网管接受//////////////////////////////
/**
 * @brief   Route_NM_Recv_Process网管接受任务
 *
 * @param [in] pvArg    unuse
 *
 * @author  ztt
 * @since
 * @bug
 */
static void *Route_NM_Recv_Process(void *pvArg)
{
	INT32 i4SocketFd;
	ssize_t u4RecvLen;
	socklen_t size;
	struct sockaddr_in tServAddr;
	UINT8 auRouteBuff[ROUTE_DATA_LEN_MAX];
	ROUTE_DBC_NODE_T *ptTemp = NULL;
	UINT32 u4IsAlarmEmergecy;
	
	i4SocketFd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > i4SocketFd)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_NM_Recv_Process][socketfd error!!!]");
		exit(1);
	}

	bzero(&tServAddr, sizeof(tServAddr));
	tServAddr.sin_family = AF_INET;
	tServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServAddr.sin_port = htons(SOCK_PORT_MGR_ADP);
	size = sizeof(tServAddr);

	if (bind(i4SocketFd, (struct sockaddr *)&tServAddr, sizeof(tServAddr)) < 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_NM_Recv_Process][bind error,exit!!!]");
		exit(1);
	}
	LOG_DEBUG(s_i4LogMsgId,"Route_NM_Recv_Process start ...");
	while(1)
	{
		u4RecvLen = recvfrom(i4SocketFd, auRouteBuff, ROUTE_DATA_LEN_MAX, 0, (struct sockaddr *)&tServAddr, &size);
		if(ptRoutePrintf->uDown)
		{
			LOG_DEBUG(s_i4LogMsgId,"[Route_NM_Recv_Process][u4RecvLen=%d]",u4RecvLen);
		}
		
		if (ROUTE_DATA_LEN_MAX != u4RecvLen)
		{
			LOG_ERROR(s_i4LogMsgId,"[Route_NM_Recv_Process][recvlen=%d error!!!]",u4RecvLen);
			Route_Debug(ROUTE_ERROR_DEBUG_TYPE_RECV_LEN, auRouteBuff);
			continue;
		}
		
		u4IsAlarmEmergecy = Route_Is_Alarm_Emergecy(auRouteBuff);
		
		if(ptIPCShm->is_connect_cc && u4IsAlarmEmergecy)
		{
			LOG_ERROR(s_i4LogMsgId,"[NM RECV AlarmEmergecy AND Connet CC]");
			Route_Debug(ROUTE_ERROR_DEBUG_TYPE_ALARM, auRouteBuff);
			continue;
		}
				
		pthread_mutex_lock(&s_tDBCListMutex);

		if((ROUTE_BUSY == s_tListODP.Status)&&(!u4IsAlarmEmergecy))//有业务 ，不是紧急告警
		{
			LOG_WARNING(s_i4LogMsgId,"[Route_NM_Recv_Process][BUSY,drop it]");
			pthread_mutex_unlock(&s_tDBCListMutex);
			continue;
		}
					
		ptTemp = DBC_Alloc();

		if(NULL == ptTemp)
		{
			//错误日志
			Route_Debug(ROUTE_ERROR_DEBUG_TYPE_NO_BUFF,auRouteBuff);//可以从内存中打印
			LOG_WFile(pLogTxtFd,"[Route_NM_Recv_Process][DBC_Alloc NO_BUFF]");
			LOG_ERROR(s_i4LogMsgId,"[Route_NM_Recv_Process][DBC_Alloc NO_BUFF]");
			pthread_mutex_unlock(&s_tDBCListMutex);
			continue;
		}
		memcpy(ptTemp->auData,auRouteBuff,ROUTE_DATA_LEN_MAX);	
		
		if(u4IsAlarmEmergecy)
		{
			 DBC_Insert((ROUTE_DBC_NODE_T **)(&s_tListODP.pAlarmEmergecy),ptTemp,(ROUTE_DBC_NODE_T **)(&s_tListODP.pAlarmEmergecyTail));	
		}
		else
		{	
			DBC_Insert((ROUTE_DBC_NODE_T **)(&s_tListODP.pNm),ptTemp,(ROUTE_DBC_NODE_T **)(&s_tListODP.pNmTail));
		}
	
		pthread_mutex_unlock(&s_tDBCListMutex);
	}
	pthread_exit(NULL);
}
/////////////////////////////业务接受//////////////////////////////
/**
 * @brief   Route_Server_Recv_Process业务接受任务
 *
 * @param [in] pvArg    unuse
 *
 * @author  ztt
 * @since
 * @bug
 */
static void *Route_Server_Recv_Process(void *pvArg)
{
	UINT32 u4RecvLen;
	INT32 i4SocketFd;
	socklen_t size;
	struct sockaddr_in tServAddr;
	
	UINT8 auRouteServerData[ROUTE_DATA_LEN_MAX];
	ROUTE_DBC_NODE_T *ptTemp = NULL;
	
	i4SocketFd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > i4SocketFd)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_Server_Recv_Process][socket error,exit!!!]");
		exit(1);
	}

	bzero(&tServAddr, sizeof(tServAddr));
	tServAddr.sin_family = AF_INET;
	tServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServAddr.sin_port = htons(SOCK_PORT_DLL_ADP);
	size = sizeof(tServAddr);

	if (bind(i4SocketFd, (struct sockaddr *)&tServAddr, sizeof(tServAddr)) < 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_Server_Recv_Process][bind error,exit!!!]");
		exit(1);
	}
	LOG_DEBUG(s_i4LogMsgId,"[Route_Server_Recv_Process start...]");
	while(1)
	{
		u4RecvLen = recvfrom(i4SocketFd, auRouteServerData, ROUTE_DATA_LEN_MAX, 0, (struct sockaddr *)&tServAddr, &size);
		
		if(ptRoutePrintf->uDown)
		{
			LOG_DEBUG(s_i4LogMsgId,"[Route_Server_Recv_Process][recvlen=%d]",(int)u4RecvLen);
		}
		
		if (ROUTE_DATA_LEN_MAX != u4RecvLen)
		{
			LOG_ERROR(s_i4LogMsgId,"[Route_Server_Recv_Process][len=%d]",(int)u4RecvLen);
			Route_Debug(ROUTE_ERROR_DEBUG_TYPE_RECV_LEN,auRouteServerData);
			continue;
		}
		
		pthread_mutex_lock(&s_tDBCListMutex);
		ptTemp = DBC_Alloc();
		if(NULL == ptTemp)
		{
			LOG_ERROR(s_i4LogMsgId,"[Route_Server_Recv_Process][DBC_Alloc no buff]");
			Route_Debug(ROUTE_ERROR_DEBUG_TYPE_NO_BUFF,auRouteServerData);
			pthread_mutex_unlock(&s_tDBCListMutex);
			continue;
		}
		
		memcpy(ptTemp->auData,auRouteServerData,u4RecvLen);		
		DBC_Insert((ROUTE_DBC_NODE_T **)(&s_tListODP.pServer),ptTemp,(ROUTE_DBC_NODE_T **)(&s_tListODP.pServerTail));
		s_tListODP.Status = ROUTE_BUSY;
		pthread_mutex_unlock(&s_tDBCListMutex);
	}
	pthread_exit(NULL);
	
}
/////////////////////////////空口下行//////////////////////////////
/**
 * @brief   Route_Get_AI_Send_Type下发数据类型
 *
 * @param [in] void
 *
 * @return  见ROUTE_SEND_TYPE_E
 * @author  ztt
 * @since   有紧急告警 立刻下发
 * @bug
 */
ROUTE_SEND_TYPE_E Route_Get_AI_Send_Type()
{
	if(NULL != s_tListODP.pAlarmEmergecy)
	{
		return ROUTE_SEND_TYPE_ALARM;
#if 0//语音帧问题
		if(NULL != s_tListODP.pServer)
		{
			ptTemp = (NAS_INF_DL_T *)(((ROUTE_DBC_NODE_T *)s_tListODP.pServer)->auData);
			if((ptTemp->MsgType > FT_VOICE_B) 
				&&(ptTemp->MsgType < FT_VOICE_F))
			{
				type = ROUTE_SEND_TYPE_SERVER;
			}
			
		}
#endif
	}

	if(NULL != s_tListODP.pServer)//有业务数据
	{
		return ROUTE_SEND_TYPE_SERVER;
	}
	if(NULL != s_tListODP.pNm)
	{
		return ROUTE_SEND_TYPE_NM;
	}
	
	return ROUTE_SEND_TYPE_INVALID;
}
/**
 * @brief   Route_Send_Process空口下行任务
 *
 * @param [in] pvArg    unuse
 *
 * @author  ztt
 * @since
 * @bug测试
 */
static void *Route_Send_Process(void *pvArg)
{
	ROUTE_DBC_NODE_T *ptTemp = NULL;	
#ifdef TEST	
	ROUTE_MSGBUF tMsgBuf;
#endif

	while(1)
	{
#ifndef TEST
		Route_Lock_Semaphore(s_i4Sem);
#endif			
		pthread_mutex_lock(&s_tDBCListMutex);
		
		ptTemp = DBC_GetData(Route_Get_AI_Send_Type());

#ifdef TEST	
		memset(&tMsgBuf,0x00,sizeof(ROUTE_MSGBUF));
		tMsgBuf.mtype = ROUTE_TEST_MSG_SERVER;
	
		//发送到空口接受
		if(ptTemp)
		{
			LOG_DEBUG(s_i4LogMsgId,"[Route_Send_Process][LOOP BACK]");
			LOG_PrintM(s_i4LogMsgId,ptTemp->auData,ROUTE_DATA_LEN_MAX);
			
			memcpy(tMsgBuf.mtext,ptTemp->auData,ROUTE_DATA_LEN_MAX);
			msgsnd(u4MsgId, &tMsgBuf, ROUTE_DATA_LEN_MAX + 4, 0);
			DBC_Free(ptTemp);	
		}
		usleep(20000);//20ms
#else	
		if(ptTemp)
		{
			if(ptRoutePrintf->uDown)
			{
				LOG_DEBUG(s_i4LogMsgId,"[Route_Send_Process]");
				LOG_PrintM(s_i4LogMsgId,ptTemp->auData,ROUTE_DATA_LEN_MAX);
			}
			write(s_u4AIFd,ptTemp->auData,ROUTE_DATA_LEN_MAX);
			DBC_Free(ptTemp);	
		}
		
		if((NULL == s_tListODP.pServer)
			&&(NULL == s_tListODP.pAlarmEmergecy))
		{
			s_tListODP.Status = ROUTE_FREE;
		}
#endif	
		pthread_mutex_unlock(&s_tDBCListMutex);	
	}
	close(s_u4AIFd);
	pthread_exit(NULL);
}
///////////////////////////////////////////////////////////////////////
/**
 * @brief   Route_Init
 *
 * @param [in] void
 *
 * @author  ztt
 * @since 保证LOG模块和CLI模块先启动
 * @bug
 */
void Route_Init()
{
	key_t msqKey;
	ROUTE_AI_SEND_PORT[0] = SOCK_PORT_ADP_MGRH;//本地网管
	ROUTE_AI_SEND_PORT[1] = SOCK_PORT_ADP_MGRA;//发给中心
	ROUTE_AI_SEND_PORT[2] = SOCK_PORT_ADP_DLL;//业务发送端口
	
	memset(&Route_AI,0x00, sizeof(struct sockaddr_in));
	Route_AI.sin_family = AF_INET;
	Route_AI.sin_addr.s_addr = inet_addr("127.0.0.1");  

	memset(&s_tListODP,0x00,sizeof(s_tListODP));
	memset(&s_tListUnused,0x00,sizeof(s_tListUnused));
	memset(&s_tErrorData,0x00,sizeof(s_tErrorData));
	//log写文件
	pLogTxtFd = fopen("./LOG.txt","r+");
	if(NULL == pLogTxtFd)
	{
		printf("[ROUTE]fopen Log.txt fail!!!\n");
		exit(1);
	}
	fseek(pLogTxtFd,0,SEEK_END);
	//log日志输出
	if((msqKey = ftok("/", LOG_MSG_ID)) == -1)
	{
		LOG_WFile(pLogTxtFd,"[Route_Init]Creat msqKey Error\n");
		exit(1);
	}
	s_i4LogMsgId = msgget(msqKey, IPC_EXCL);  /*检查消息队列是否存在*/
	if(s_i4LogMsgId < 0)
	{
		LOG_WFile(pLogTxtFd,"[Route_Init]failed to get msq\n");
	       exit(1);
	}
	//回环测试
#ifdef TEST
       if((msqKey = ftok("/", ROUTE_MSG_SERVER_ID)) == -1)
	{
		LOG_ERROR(s_i4LogMsgId,"[Route_Init]Creat LOOPBACK TEST msqKey Error");
		exit(1);
	}
	u4MsgId = msgget(msqKey, IPC_EXCL);  /*检查消息队列是否存在*/
	if(u4MsgId < 0)
	{
		u4MsgId = msgget(msqKey, IPC_CREAT|0666);/*创建消息队列*/
		if(u4MsgId < 0)
		{
			LOG_ERROR(s_i4LogMsgId,"[Route_Init]failed to create LOOPBACK TEST  msq");
		       exit(1);
		}
	}   
#endif

	DBC_Init();

	//进程间通信获取资源
	s_i4Sem = Route_Get_Semaphore(ROUTE_SEM_ID);
	if(0 > s_i4Sem)
	{
		LOG_ERROR(s_i4LogMsgId,"Route_Get_Semaphore error,exit!!!");
		exit(1);
	}
	Route_Set_Sem_Value(s_i4Sem,0);

	ptIPCShm = (SHM_IPC_STRU *)Route_Get_Shm(FTOK_ID_SHM_IPC,sizeof(SHM_IPC_STRU));
	if(NULL == ptIPCShm)
	{
		LOG_ERROR(s_i4LogMsgId,"Route_Get_Shm  FTOK_ID_SHM_IPC error,exit!!!");
		exit(1);
	}

	LOG_DEBUG(s_i4LogMsgId,"ptIPCShm->is_connect_cc=%d",ptIPCShm->is_connect_cc);

	ptRoutePrintf = (ROUTE_PRINTF *)ptIPCShm->route_printf;

	memset((UINT8 *)ptRoutePrintf,0x00,sizeof(ROUTE_PRINTF));
	
	ptRouteCfgShm = (SHM_CFG_STRU *)Route_Get_Shm(FTOK_ID_SHM_CFG,sizeof(SHM_CFG_STRU));
	if(NULL == ptRouteCfgShm)
	{
		LOG_ERROR(s_i4LogMsgId,"Route_Get_Shm  FTOK_ID_SHM_CFG error,exit!!!");
		LOG_WFile(pLogTxtFd,"Route_Get_Shm  FTOK_ID_SHM_CFG error,exit!!!");
		exit(1);
	}

	s_RouteLocalId = ptRouteCfgShm->dev_id.val;
	LOG_DEBUG(s_i4LogMsgId,"Local ID is %d",(int)s_RouteLocalId);
	return;
}
/**
 * @brief   Route_Interrupt_Handle
 *
 * @param [in] void
 *
 * @author  ztt
 * @since
 * @bug
 */
void Route_Interrupt_Handle(int signum)
{
	if(ptRoutePrintf->uDownIr)
	{
		LOG_DEBUG(s_i4LogMsgId,"Drive Interupt come");
	}
	Route_Unlock_Semaphore(s_i4Sem);
}
/**
 * @brief   Route_Hang_Interrupt
 *
 * @param [in] void
 *
 * @author  ztt
 * @since
 * @bug
 */
void Route_Hang_Interrupt()
{	
	struct sigaction stSigPps;

	if((s_u4AIFd = open("/dev/axiram-service",O_RDWR)) < 0)
	{
		LOG_ERROR(s_i4LogMsgId,"open axiram-service error,exit!!!");
		exit(1);
	}
     
       LOG_DEBUG(s_i4LogMsgId,"Open axiram-service  device OK!");

	fcntl(s_u4AIFd, F_SETOWN, getpid());
	fcntl(s_u4AIFd, F_SETFL, fcntl(s_u4AIFd, F_GETFL) | FASYNC);
	
	memset(&stSigPps, 0, sizeof(stSigPps));
	stSigPps.sa_handler = Route_Interrupt_Handle;
	stSigPps.sa_flags = 0;
	sigaction(SIGIO, &stSigPps, NULL);   
	return;
}
/**
 * @brief   main
 *
 * @param [in] pvArg    unuse
 *
 * @author  ztt
 * @since
 * @bug
 */
int main()
{	
	Route_Init();	
	Route_Hang_Interrupt();
	
	if (pthread_create(&s_tRouteAIRecvTask, NULL, Route_AI_Recv_Process, NULL) != 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[ROUTE][Route_AI_Recv_Process error");
		exit(1);
	}
	pthread_detach(s_tRouteAIRecvTask);

	if (pthread_create(&s_tRouteNMRecvTask, NULL, Route_NM_Recv_Process, NULL) != 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[ROUTE][Route_NM_Recv_Process error");
		exit(1);
	}
	pthread_detach(s_tRouteNMRecvTask);

	if (pthread_create(&s_tRouteServerRecvTask, NULL, Route_Server_Recv_Process, NULL) != 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[ROUTE][Route_Server_Recv_Process error");
		exit(1);
	}
	pthread_detach(s_tRouteServerRecvTask);

	if (pthread_create(&s_tRouteSendTask, NULL, Route_Send_Process, NULL) != 0)
	{
		LOG_ERROR(s_i4LogMsgId,"[ROUTE][Route_Send_Process error");
		exit(1);
	}
	pthread_detach(s_tRouteSendTask);
	
	while(1)
	{
		Route_Sleep(10);
	}
	return 1;
}
