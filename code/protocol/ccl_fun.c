/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file   ccl_fun.c
 * @brief
 * @author
 * @version v1.0
 * @date    2016-08-17
 */
/*
 *
 *   修改历史
 *   2016-08-17          牛功喜                建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "ccl_fun.h"
#include"ccl.h"
#include "dll.h"
#include <unistd.h>
/******************************************************************************
 *   类型定义
 *   *************************************************************************/

extern CCL_PRINT_T * tcclPrint;
extern SHM_CFG_STRU *ptCFGShm ;
extern DLL_FPGA_SHM_T *p_DllFpgaShm;


extern unsigned int  s_LogMsgId;

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
/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/
/**
 * @def DB_NODE_MAX
 * @brief 链表最大节点数
 */

#define       DB_QUE_NODE_MAX              (3)
/**
 * @defDB_QUE_DATA_MAX
  * @brief 节点数据包最大长度定义
  */
#define       DB_QUE_DATA_MAX               (36)
/**
 * @var s_tQueSignal
 * @brie链表信令节点
 */
static QUE_UNIT_T  s_tQueSignal[1] ={{NULL,{0},0}};
/**
 * @var s_tListUnused
 * @brief 链表空闲节点
 */
static QUE_UNIT_T  s_tQueUnused[1] ={{NULL,{0},0}};
/**
 * @var s_atQueNode
 * @brief 节点存储区
 */
static QUE_UNIT_T s_atQueNode[DB_QUE_NODE_MAX] = {{NULL,{0},0}};

/******************************************************************************
 *   可调用函数实现
 *   *************************************************************************/
/**
 * @brief   生成队列链表
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
static void QUE_Generate()
{

    QUE_UNIT_T*    ptTemp = s_tQueSignal;
    ptTemp->ptNext        = ptTemp;  /* 信令队列链表自环 */

}
/**
 * @brief   获取队列链表头指针
 * @return  QUE_UNIT_T*      见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
QUE_UNIT_T* QUE_GetHead()
{
        return &s_tQueSignal[0];
}
/**
 * @brief   删除队列中超时的消息
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
void  QUE_DelExpiredNode()
{
    unsigned char DiffTime;
    time_t     CurrentTime;
    QUE_UNIT_T* ptData=NULL;

    QUE_UNIT_T* ptHead    = QUE_GetHead();
    QUE_UNIT_T* ptTemp    = ptHead;
    CurrentTime=time( &CurrentTime);

    while(ptTemp->ptNext != ptHead)
    {
        ptTemp = ptTemp->ptNext;
        //Num++;
        DiffTime=difftime(CurrentTime,ptTemp->u4STCN);
        if(DiffTime>2)               //设定信令超时时间
        {
            ptData=ptTemp;
            QUE_GetData(& ptData);
            QUE_Free(ptData);
        }
    }
}


/**
 * @brief   申请空闲节点，如果没有空闲节点，返回失败
 *
 * @param [in]  uSlotIdx      时隙值
 * @param [out]  ptHead       申请节点输出地址
 *
 * @return  BOOL         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
BOOL QUE_Alloc(QUE_UNIT_T** ptHead)
{
    QUE_UNIT_T* ptTemp = s_tQueUnused[0].ptNext;

    if (ptTemp == &s_tQueUnused[0])
    {
            // printf("QUE_Alloc failed!\n");
        LOG_ERROR(s_LogMsgId,"[CCL][%s]QUE_Alloc failed!!", __FUNCTION__);
        return FALSE;
    }

    *ptHead = ptTemp;

    s_tQueUnused[0].ptNext = ptTemp->ptNext;

    (*ptHead)->ptNext = NULL;

    return TRUE;
}
/**
 * @brief   释放节点，将其放入空闲队列链表
 *
 * @param [in]  ptData        待释放节点地址
 *
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
void QUE_Free(QUE_UNIT_T* ptData)
{
    QUE_UNIT_T* ptTemp = s_tQueUnused[0].ptNext;
    s_tQueUnused[0].ptNext = ptData;
    ptData->ptNext = ptTemp;
    memset(ptData->auData,0,DB_QUE_DATA_MAX );
}
/**
 * @brief   查询队列链表是否为空
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
BOOL QUE_IsEmpty()
{
    QUE_UNIT_T* ptHead  = QUE_GetHead();

    if(ptHead->ptNext == ptHead)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief   删除相应队列链表内所用节点数据
 *
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
void QUE_FlushData()
{
    QUE_UNIT_T* ptTemp=NULL;

    while(!QUE_IsEmpty())
    {
        QUE_GetData(&ptTemp);
        QUE_Free(ptTemp);
        printf("f\n");
    }
}
/**
 * @brief   添加节点到相应队列链表头
 *
 * @param [in]  ptData     节点指针
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
BOOL QUE_AddData(QUE_UNIT_T* ptData)
{
    QUE_UNIT_T* ptHead    = QUE_GetHead();
    QUE_UNIT_T* ptTemp    = ptHead;

    while(ptTemp->ptNext != NULL)
    {
        if(ptTemp->ptNext == ptHead)
        {
            ptTemp->ptNext = ptData;
            ptData->ptNext = ptHead;
            return TRUE;
        }
        ptTemp = ptTemp->ptNext;
    }
    return FALSE;
}

/**
 * @brief   获取队列链表内所用节点数量
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
unsigned short  QUE_GetNum()
{
    UINT16 Num = 0;
    QUE_UNIT_T* ptHead    = QUE_GetHead();
    QUE_UNIT_T* ptTemp    = ptHead;

    while(ptTemp->ptNext != ptHead)
    {
        ptTemp = ptTemp->ptNext;
        Num++;
    }

    return Num;
}
/**
 * @brief   获取相应队列链表内首个节点数据
 * @param [out] ptData       待获取输出节点地址
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
BOOL QUE_GetData(QUE_UNIT_T** ptData)
{
    QUE_UNIT_T* ptHead    = QUE_GetHead();

    if(!QUE_IsEmpty())
    {
        *ptData = ptHead->ptNext;
        ptHead->ptNext = (*ptData)->ptNext;
        (*ptData)->ptNext = NULL;
        return TRUE;
    }

    return FALSE;
}
/**
 * @brief   获取相应队列链表头节点存储时间戳值
 * @return  void         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
unsigned int QUE_GetTscn()
{
     QUE_UNIT_T* ptHead   = QUE_GetHead();
     QUE_UNIT_T* ptTemp   = ptHead->ptNext;
     return ptTemp->u4STCN;
}
/**
 * @brief   初始化模块并分配资源
 * @return  STATUS         见@link returncodes 返回值定义@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
unsigned int QUE_Init(void)
{
    UINT32 u4Cnt  = 0;
    UINT32 u4Temp = 0;

    /* 格式化数据存储区 */
    memset(s_atQueNode, 0x0 , DB_QUE_NODE_MAX  * sizeof(QUE_UNIT_T));
    /* 生成链表基本结构 */
    QUE_Generate();
    u4Temp = DB_QUE_NODE_MAX - 1;

    s_tQueUnused[0].ptNext = &s_atQueNode[0];

    for (u4Cnt = 0; u4Cnt < u4Temp; u4Cnt++)
    {
        s_atQueNode[u4Cnt].ptNext = &s_atQueNode[u4Cnt + 1];
    }

    s_atQueNode[u4Temp].ptNext = &s_tQueUnused[0];

    return 0;
}
/**
* @brief  阻塞延时函数
* @param [in] msec   ms 延时时间
* @author 牛功喜
* @since
* @bug
*/
void delay(unsigned long msec)
{
    struct timeval tv;
    tv.tv_sec = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;
    switch (select(0, NULL, NULL, NULL, &tv))
    {
        case -1:  // 错误
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]d Error!", __FUNCTION__);
            break;
        }
        case 0:  // 超时
        {
            break;
        }
        default:
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]delay default!", __FUNCTION__);
            break;
        }
     }
}

 /**
 * @brief  初始化信号量值函数
 * @param [in] sem_id  信号量ID
 * @param [in] value  信号量初始化值
 * @author 牛功喜
 * @since
 * @bug
 */
int set_semvalue(int sem_id, int value)
{
    union semun sem_union;

    sem_union.val = value;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]Failed to  set_semvalue", __FUNCTION__);
        return -1;
    }
return 0;
}
 /**
 * @brief  删除指定信号量
 * @param [in] sem_id  信号量ID
 * @author 牛功喜
 * @since
 * @bug
 */
int  del_semvalue(int sem_id)
{
    union semun sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]Failed to delete semaphore", __FUNCTION__);
        return -1;
    }
    return 0;
}
/**
* @brief  信号量P操作
* @param [in] sem_id   信号量ID
* @author 牛功喜
* @since
* @bug
*/
int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;         /* P(v) */
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]semaphore_p failed", __FUNCTION__);
        return -1;
    }
    return 0;
}

/**
* @brief  信号量V操作
* @param [in] sem_id   信号量ID
* @author 牛功喜
* @since
* @bug
*/
int semaphore_v(int sem_id)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1;         /* V(v)*/
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1)
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] semaphore_v failed", __FUNCTION__);
        return  -1;
    }
    return 0;
}

/**
* @brief  解析RTP包
* @param [in] data   RTP数据包
* @param [in] size   RTP数据包长度
* @param [out]解析后有效数据
* @author 牛功喜
* @since
* @bug
*/
 int parsingRTPPacket(unsigned char  *rtpdata, int  size,unsigned char *data )
 {
    unsigned char   *extensionData=NULL;
    unsigned short   PayloadOffset;
    unsigned short   PaddingLength;
    //unsigned int       RtpTime;
    //unsigned int  SrcId ,SeqNum;
    unsigned short  NumCSRCs,ExtensionLength;

    if (size < 12)//Too short to be a valid RTP header.
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] short to be a valid RTP header", __FUNCTION__);
        return -1;
    }
    if ((rtpdata[0] >> 6) != 2)  //Currently, the version is 2, if is not 2, unsupported.
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] the version is not 2", __FUNCTION__);
        return -1;
    }
    if (rtpdata[0] & 0x20)// Padding present.
    {
        PaddingLength = rtpdata[size - 1];
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] PaddingLength len=%d ", __FUNCTION__,PaddingLength);
        if (PaddingLength + 12 > size)
        {

            LOG_ERROR(s_LogMsgId,"[CCL][%s] Padding present lenth   error", __FUNCTION__);
            return -1;
        }
        size -= PaddingLength;
    }

    NumCSRCs = rtpdata[0] & 0x0f;
    PayloadOffset = 12 + 4 * NumCSRCs;

    if (size < PayloadOffset)// Not enough data to fit the basic header and all the CSRC entries.
    {

        LOG_ERROR(s_LogMsgId,"[CCL][%s]Not enough data to fit the basic header", __FUNCTION__);
        return -1;
    }

    if (rtpdata[0] & 0x10)   // Header extension present.
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]Header extension presen ", __FUNCTION__);
        if (size < PayloadOffset + 4) // Not enough data to fit the basic header, all CSRC entries and the first 4 bytes of the extension header
        {

            LOG_ERROR(s_LogMsgId,"[CCL][%s]Not enough data to fit the basic header", __FUNCTION__);
            return -1;
        }
        extensionData = &rtpdata[PayloadOffset];
        ExtensionLength = 4 * (extensionData[2] << 8 | extensionData[3]);
        if (size < PayloadOffset + 4 + ExtensionLength)
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]lenth error", __FUNCTION__);
            return -1;
        }
        PayloadOffset += (4 + ExtensionLength);
    }
    memcpy(data,&rtpdata[PayloadOffset],size-PayloadOffset+1);
    //RtpTime = data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7];
    //SrcId = data[8] << 24 | data[9] << 16 | data[10] << 8 | data[11];
    //SeqNum = data[2] << 8 | data[3];
    return (size-PayloadOffset+1);
}


/**
* @brief    大小端转换
* @param [in] RawData 原始数据
* @param [in] datalen 数据长度
* @param [out] ConvertedData 转换后数据
* @author 牛功喜
* @since
* @bug
*/
void LitToBigSmsData(unsigned char *ConvertedData,unsigned char *RawData,unsigned short datalen)
{
    unsigned short i;
    for(i=0;i<datalen;i++)
    {
        if(0 ==i%2)
        {
            ConvertedData[i+1]=RawData[i];
        }
        else
        {
            ConvertedData[i-1]=RawData[i];
        }
    }
}


/**
 * @brief   数据链路层同步GPS时间
 *
*/
void CLL_SyncGpsTime(void)
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
 * @brief   数据链路层同步GPS时间
 *
*/
void CLL_GetGpsTime(struct tm* pvTm)
{
    GPS_DEV_DATA_T *GpsDevData;
    GpsDevData = (GPS_DEV_DATA_T *)(p_DllFpgaShm->GpsData);

    if(NULL != pvTm)
    {
        pvTm->tm_sec = GpsDevData->SEC;
        pvTm->tm_min = GpsDevData->MIN;
        pvTm->tm_hour = (GpsDevData->HOUR)%24;
        pvTm->tm_mday = GpsDevData->DAY;
        pvTm->tm_mon = GpsDevData->MONTH - 1;
        pvTm->tm_year = GpsDevData->YEAR+ 2000- 1900;
        return;
    }
    else
    {
        printf("CLL_SyncGpsTime error!/n");
    }
    return;
}

/**
* @brief    获取系统时间 按字符串输出
* @param [out] pvtime  字符格式时间数据指针
* @author 牛功喜
* @since
* @bug
*/
int GetTime( char *pvtime)
{
    char pttime[20]={0};
    struct tm timeinfo;
    //同步共享内存
    msync((void *)FPGA_SHM_ADDR, FPGA_SHM_LEN, MS_SYNC);

   // GpsExsit = 0x01 & (p_DllFpgaShm->GpsFlag >> 1);

    if((p_DllFpgaShm->GpsFlag & 0x01) != 1)//GPS 没有锁定
    {   
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] GPS time err :gpsexsit:%d lockflg:%d  ", __FUNCTION__,(0x01 & (p_DllFpgaShm->GpsFlag >> 1)),p_DllFpgaShm->GpsFlag & 0x01);
        memset(pttime,48,14);
    }
    else
    {
        CLL_SyncGpsTime();
        memset(&timeinfo,0,sizeof(timeinfo));
        CLL_GetGpsTime(&timeinfo);
        usleep(30000);
        strftime(pttime,20,"%Y%m%d%H%M%S",&timeinfo);
    }
    strcpy(pvtime,pttime);
    if (1 == tcclPrint->DllUp)
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]  GPSTIME=%s   len=%d  ", __FUNCTION__,pvtime,strlen(pvtime));
    }
    return 0;
}


