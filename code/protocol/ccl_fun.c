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
 *   �޸���ʷ
 *   2016-08-17          ţ��ϲ                �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "ccl_fun.h"
#include"ccl.h"
#include "dll.h"
#include <unistd.h>
/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/

extern CCL_PRINT_T * tcclPrint;
extern SHM_CFG_STRU *ptCFGShm ;
extern DLL_FPGA_SHM_T *p_DllFpgaShm;


extern unsigned int  s_LogMsgId;

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
/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/
/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/
/**
 * @def DB_NODE_MAX
 * @brief �������ڵ���
 */

#define       DB_QUE_NODE_MAX              (3)
/**
 * @defDB_QUE_DATA_MAX
  * @brief �ڵ����ݰ���󳤶ȶ���
  */
#define       DB_QUE_DATA_MAX               (36)
/**
 * @var s_tQueSignal
 * @brie��������ڵ�
 */
static QUE_UNIT_T  s_tQueSignal[1] ={{NULL,{0},0}};
/**
 * @var s_tListUnused
 * @brief ������нڵ�
 */
static QUE_UNIT_T  s_tQueUnused[1] ={{NULL,{0},0}};
/**
 * @var s_atQueNode
 * @brief �ڵ�洢��
 */
static QUE_UNIT_T s_atQueNode[DB_QUE_NODE_MAX] = {{NULL,{0},0}};

/******************************************************************************
 *   �ɵ��ú���ʵ��
 *   *************************************************************************/
/**
 * @brief   ���ɶ�������
 * @return  void         ��@link returncodes ����ֵ����@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
static void QUE_Generate()
{

    QUE_UNIT_T*    ptTemp = s_tQueSignal;
    ptTemp->ptNext        = ptTemp;  /* ������������Ի� */

}
/**
 * @brief   ��ȡ��������ͷָ��
 * @return  QUE_UNIT_T*      ��@link returncodes ����ֵ����@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
QUE_UNIT_T* QUE_GetHead()
{
        return &s_tQueSignal[0];
}
/**
 * @brief   ɾ�������г�ʱ����Ϣ
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
        if(DiffTime>2)               //�趨���ʱʱ��
        {
            ptData=ptTemp;
            QUE_GetData(& ptData);
            QUE_Free(ptData);
        }
    }
}


/**
 * @brief   ������нڵ㣬���û�п��нڵ㣬����ʧ��
 *
 * @param [in]  uSlotIdx      ʱ϶ֵ
 * @param [out]  ptHead       ����ڵ������ַ
 *
 * @return  BOOL         ��@link returncodes ����ֵ����@endlink
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
 * @brief   �ͷŽڵ㣬���������ж�������
 *
 * @param [in]  ptData        ���ͷŽڵ��ַ
 *
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ѯ���������Ƿ�Ϊ��
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ɾ����Ӧ�������������ýڵ�����
 *
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ӽڵ㵽��Ӧ��������ͷ
 *
 * @param [in]  ptData     �ڵ�ָ��
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ȡ�������������ýڵ�����
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ȡ��Ӧ�����������׸��ڵ�����
 * @param [out] ptData       ����ȡ����ڵ��ַ
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ȡ��Ӧ��������ͷ�ڵ�洢ʱ���ֵ
 * @return  void         ��@link returncodes ����ֵ����@endlink
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
 * @brief   ��ʼ��ģ�鲢������Դ
 * @return  STATUS         ��@link returncodes ����ֵ����@endlink
 * @author
 * @since   trunk.0001
 * @bug
 */
unsigned int QUE_Init(void)
{
    UINT32 u4Cnt  = 0;
    UINT32 u4Temp = 0;

    /* ��ʽ�����ݴ洢�� */
    memset(s_atQueNode, 0x0 , DB_QUE_NODE_MAX  * sizeof(QUE_UNIT_T));
    /* ������������ṹ */
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
* @brief  ������ʱ����
* @param [in] msec   ms ��ʱʱ��
* @author ţ��ϲ
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
        case -1:  // ����
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]d Error!", __FUNCTION__);
            break;
        }
        case 0:  // ��ʱ
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
 * @brief  ��ʼ���ź���ֵ����
 * @param [in] sem_id  �ź���ID
 * @param [in] value  �ź�����ʼ��ֵ
 * @author ţ��ϲ
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
 * @brief  ɾ��ָ���ź���
 * @param [in] sem_id  �ź���ID
 * @author ţ��ϲ
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
* @brief  �ź���P����
* @param [in] sem_id   �ź���ID
* @author ţ��ϲ
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
* @brief  �ź���V����
* @param [in] sem_id   �ź���ID
* @author ţ��ϲ
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
* @brief  ����RTP��
* @param [in] data   RTP���ݰ�
* @param [in] size   RTP���ݰ�����
* @param [out]��������Ч����
* @author ţ��ϲ
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
* @brief    ��С��ת��
* @param [in] RawData ԭʼ����
* @param [in] datalen ���ݳ���
* @param [out] ConvertedData ת��������
* @author ţ��ϲ
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
 * @brief   ������·��ͬ��GPSʱ��
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
 * @brief   ������·��ͬ��GPSʱ��
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
* @brief    ��ȡϵͳʱ�� ���ַ������
* @param [out] pvtime  �ַ���ʽʱ������ָ��
* @author ţ��ϲ
* @since
* @bug
*/
int GetTime( char *pvtime)
{
    char pttime[20]={0};
    struct tm timeinfo;
    //ͬ�������ڴ�
    msync((void *)FPGA_SHM_ADDR, FPGA_SHM_LEN, MS_SYNC);

   // GpsExsit = 0x01 & (p_DllFpgaShm->GpsFlag >> 1);

    if((p_DllFpgaShm->GpsFlag & 0x01) != 1)//GPS û������
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


