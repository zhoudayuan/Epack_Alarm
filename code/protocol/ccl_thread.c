
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file   cclthread.c
 * @brief   接收处理中心语音信令数据发送给链路层或处理链路层数据发送给中心
 * @author 牛功喜
 * @version v1.0
 * @date    2016-07-26
 */
/*
 *   函数列表
 *   1.  set_ticker              时间定时器函数
 *   2.  timercallback           定时器回调函数
 *   3.  CCL_init                CCL初始化函数
 *   4.  thread_create           线程创建函数
 *   5.  ODP_HandleCenterVioce   处理中心语音数据线程函数
 *   6.  ODP_HandleCenterSig     处理中心数据线程函数
 *   7.  IDP_RecfromDll          接收DLL数据线程函数
 *   8.  isSocketReadble         检查套接字是否可读
 *   9.  timer_handler           时间定时器处理函数
 *   10. IDP_GenVoiceDataPaket   生成上行语音包
 *   11.
 *   12.
 *   13.
 *   ......

 *   修改历史
 *   2016-7-26           牛功喜                   建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
//#include "ccl.h"
#include "ccl_dl.h"
#include "ccl_ul.h"
//#include"ccl.h"
#include "ccl_fun.h"
#include "dll.h"
#include "print_debug.h"

//#include "DLL.h"

/*#include<signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include<math.h>
#include<errno.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
*/





//debug

int  packetnumber=0;


//enddebug

/******************************************************************************
 *  类型定义
 *   *************************************************************************/
 typedef void (*sighandler_t)(int);


extern unsigned int  s_LogMsgId;
CCL_PRINT_T * tcclPrint=NULL;
extern SHM_CFG_STRU *ptCFGShm ;
extern DLL_GLB_CFG_T g_DllGlobalCfg;
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
* @var p_DllFpgaShm
* @brief FPGA配置共享内存
*/
extern DLL_FPGA_SHM_T *p_DllFpgaShm;

/******************************************************************************
 *   宏定义
 *   *************************************************************************/
/**
 * @def  s_tLast_Relay_Flg
 * @brief  上一次停止转发标志
 */

unsigned char s_tLast_Relay_Flg=0;
/**
 * @def  TIMER_INTERVAL
 * @brief  定时器时间间隔(ms)
 */
#define    TIMER_INTERVAL                 30
/**
 * @def  RECV_DLL_PORT
 * @brief 本地发送DLL 端口
 */
#define SEND_DLL_PORT                       (10020)
/**
 * @def  SEND_CCL_PORT
 * @brief  本地接收DLL端口
 */
#define REC_DLL_PORT                       (10021)

/**
 * @def   CENTER_VOICE_PORT
 * @brief 中心业务端口
 */
#define  CENTER_VOICE_PORT               (6017)//(2222)
/**
 * @def  CENTER_SIGNAL_PORT
 * @brief 中心信令端口
 */
#define  CENTER_SIGNAL_PORT              (6018)//(1111)




/******************************************************************************
*   类型定义
*   *************************************************************************/


/******************************************************************************
*   全局变量定义
*   *************************************************************************/


/******************************************************************************
*   局部变量定义
*   *************************************************************************/

/**
* @var  s_tRec_PttOFF_FLG
* @brief 接收中心PTT OFF 信令标识
*/
static  bool  s_tRec_PttOFF_FLG;
/**
* @var  s_tNas_STOP_RELAY
* @brief NAS 停止转发标志
*/

 bool  s_tNas_STOP_RELAY=0;

/**
* @var  s_tRec_LC_FLG
* @brief 接收DLL LC 标志位
*/
static bool s_tRec_LC_FLG=0;
/**
* @var  s_tSend_PTTON_FLG
* @brief 发送 到中心 PTTON 命令标识
*/
 bool s_tSend_PTTON_FLG=0;

/**
* @var  s_tFramType
* @brief CCL 层下行数据帧类型
*/
 static  unsigned  int  s_tFramType;

/**
* @var s_tCenterVoiceSockfd
* @brief 中心语音套接字
*/
 static int s_tCenterVoiceSockfd;
//s_tCclSigUlSockfd
/**
* @var s_tCclSigUlSockfd
* @brief 呼叫控制层信令上行套接字
*/
 static int  s_tCclSigUlSockfd ;

/**
* @var s_tCclVioceUlSockfd
* @brief 呼叫控制层语音上行套接字
*/
 static int  s_tCclVioceUlSockfd ;

/**
* @var  s_tCenterSigSockfd
* @brief 中心信令套接字
*/
static int s_tCenterSigSockfd  ;
 /**
  * @var  s_tRecFrmDllSocket
  * @brief /链路层上行套接字
  */
static int s_tRecFrmDllSocket;
 /**
  * @var  s_tSendToDllSocket
  * @brief /链路层下行套接字
  */
static int s_tSendToDllSocket;
 /**
  * @struct   s_tSendToDllAddr
  * @brief 发送数据到链路层地址
  */
 static  struct sockaddr_in s_tSendToDllAddr ;

  /**
   * @struct   s_tRecFrmDllAddr
   * @brief 接收链路层数据地址
   */
  static  struct sockaddr_in     s_tRecFrmDllAddr ;
 /**
  * @struct  s_tCenterSigAddr
  * @brief 中心信令地址
  */
static  struct sockaddr_in    s_tCenterSigAddr;
/**
* @struct  s_tCenterVoiceAddr
* @brief 中心语音地址
*/
static  struct sockaddr_in  s_tCenterVoiceAddr;

/**
* @struct  s_tCclVioceUlAddr
* @brief 呼叫控制层语音上行地址
*/
static  struct sockaddr_in  s_tCclVioceUlAddr;

/**
* @struct  s_tCclSigUlAddr
* @brief 呼叫控制层信令上行地址
*/
static  struct sockaddr_in  s_tCclSigUlAddr;

/**
 * @var  s_tHandleCenterViocethread
 * @brief 处理中心语音线程标识
 */
static  pthread_t        s_tHandleCenterViocethread;

/**
 * @var  s_tHandleCenterSigthread
 * @brief 处理中心信令线程标识
 */
static  pthread_t        s_tHandleCenterSigthread;
/**
 * @var  s_tRecFrmDllthread
 * @brief 接收数据链路层线程标识
 */
static  pthread_t        s_tRecFrmDllthread;

/**
 * @var  s_tHandletimerthread
 * @brief 定时器处理线程
 */
static  pthread_t        s_tHandletimerthread;

/**
* @var CCL_DATA_STATE
* @brief  呼叫控制层数据处理状态
*/
 unsigned char    INF_CCL_STATE;
/**
 * @var WLU_WORK_STATE
 * @brief  信道机工作状态
 */
static  unsigned char      WLU_WORK_STATE;
/**
 * @var   WaitRevivFlg
 * @brief  等待激活响应标志  包括激活终端、激活WLU
 */
 static unsigned char      WaitRevivFlg;

 /**
  * @var WaitNearNodeReportFlg
  * @brief  等待邻点查询结果标志
  */
 static unsigned char  WaitNearNodeReportFlg;
/**
 * @var WaitGpsReportFlg
 * @brief  等待GPS上报标志
 */
static unsigned char  WaitGpsReportFlg;
/**
 * @var WaitGpsReportFlg
 * @brief  等待遥晕MS 响应标志
 */
static unsigned char WaitStunMsFlg;

/**
 * @var WaitNearNodeReportFlg
 * @brief  等待邻点上报标志
 */
//static unsigned char WaitNearNodeReportFlg;


/**
 * @var  WaitGpsReportNasFlg
 * @brief  等待Gps 查询命令应答标志
 */
//static unsigned short WaitGpsReportNasFlg ;


/**
 * @var  WaitGpsReportMsFlg
 * @brief  等待Gps 查询命令应答标志
 */
//static unsigned short  WaitGpsReportMsFlg;


/**
 * @var  WaitStunNasFlg
 * @brief  等待摇晕WLU 命令应答标志
 */
//static unsigned short  WaitStunNasFlg;

/**
 * @var  WaitKillNasAckFlg
 * @brief  等待摇毙WLU 命令应答标志
 */
//static unsigned short  WaitKillNasAckFlg;
/**
 * @var  WaitRevivNasAckFlg
 * @brief  等待激活WLU 命令应答标志
 */
//static unsigned short  WaitRevivNasAckFlg;
/**
 * @var  WaitRevivMSAckFlg
 * @brief  等待激活MS 命令应答标志
 */
//static unsigned short  WaitRevivMSAckFlg;

/**
 * @var  WaitRevivAckTime
 * @brief激活命令响应  等待时间
 */
static unsigned short   WaitRevivAckTime;
/**
 * @var  WaitRemoteCtrlAckTime
 * @brief失能命令响应等待时间 包或遥晕终端、遥晕WLU、遥毙WLU
 */
static unsigned short   WaitDisableSigAckTime;

/**
 * @var  WaitGpsReportTimer
 * @brief 上拉GPS命令 等待时间
 */
static unsigned short   WaitGpsReportTimer;
/**
 * @var  WaitSigAckTimer
 * @brief   等待信令响应超时时间
 */
//static unsigned short WaitSigAckTimer;

/**
 * @var  WluKilldeTimer
 * @brief遥毙计时器 ，遥毙后半小时关闭系统
 */
static  unsigned int WluKilldeTimer;
/**
 * @var  NearNodeQueyPeriod
 * @brief 临近节点查询 时间周期
 */
static unsigned short   NearNodeQueyPeriod;
/**
 * @var  NearNodeQueyTimeOut
 * @brief 邻点查询超时时间
 */
//static unsigned short NearNodeQueyTimeOut;

/**
 * @var  WaitNearNodeQueyAckTime
 * @brief 邻点查询相应等待时间
 */
static unsigned short   WaitNearNodeQueyAckTime;



/**
 * @var CCL_DL_PRINT
 * @brief 信令消息有效数据长度
 */

 unsigned  char   CCL_DL_PRINT =true;
/**
 * @var CCL_DL_PRINT
 * @brief 保存LC 数据
 */
 unsigned char s_aLcdata[20]={0};
/**
 * @brief 保存CC层ID 数据
 */
 unsigned char s_aCCcvNetId[20]={0};
/**
 * @brief 时间计数
 */
unsigned int TimeCount = 0;

/**
* @var s_au2AmbeEraData[];
* @brief AMBE静音帧数据
*/
static unsigned  short  s_au2AmbeEraData[14] =
{
    0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261,
    0x7300, 0x2a6b, 0xb9e8, 0x8152, 0x6173, 0x002a, 0x6b00
};



  /******************************************************************************
   *   内部函数实现
   *   *************************************************************************/

    /**
  * @brief  定时器按照设定时间发出信号
  * @param [in] n_msecs   定时时间
  * @author 牛功喜
  * @since
  * @bug
  */

int  set_ticker(int n_msecs)                                         //定时器
{
  struct  itimerval new_timeset;
  long n_sec, n_usecs;
  n_sec = n_msecs / 1000;
  n_usecs = (n_msecs % 1000) * 1000L;
  new_timeset.it_interval.tv_sec = n_sec;  // set reload   设置初始间隔
  new_timeset.it_interval.tv_usec = n_usecs; //new ticker value
  new_timeset.it_value.tv_sec = n_sec;   //store this   设置重复间隔
  new_timeset.it_value.tv_usec = n_usecs; //and this
  return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

/**
* @brief 定时器回调函数
* @author 牛功喜
* @since
* @bug
*/
void timercallback()
{
  /* WaitDisableSigAckTime++;
   if(WaitDisableSigAckTime%30==0)
    printf("1s  \n");
    if(WaitDisableSigAckTime/30==20)
    exit(0);*/
    NearNodeQueyPeriod++;
    if(2000==NearNodeQueyPeriod) //
   {
      //发送邻点查询
        WaitNearNodeReportFlg=true;
        NearNodeQueyPeriod=0;
   }
   if(WaitNearNodeReportFlg)
   {
        WaitNearNodeQueyAckTime++;
        if( 30 <WaitNearNodeQueyAckTime)
        {
          //邻点查询失败
        }
   }

   if( NAS_WAIT_STUN_ACK_STATE == WLU_WORK_STATE
               || NAS_WAIT_KILL_ACK_STATE  == WLU_WORK_STATE || true == WaitStunMsFlg)
   {
        WaitDisableSigAckTime++;
        if(WaitDisableSigAckTime >30)                      //失能等待 包括失能WLU、MS  或遥毙WLU命令等待
        {
            WaitDisableSigAckTime=0;
            WLU_WORK_STATE=NAS_NORMAL_STATE;
            WaitStunMsFlg=false;
            //回应中心 命令执行失败
        }

    }
   if(NAS_KILLED_STATE==WLU_WORK_STATE)                   //WLU被遥毙  半小时后 线程退出
   {
        WluKilldeTimer++;
        if(WluKilldeTimer  >60000 )
        {
             WluKilldeTimer=0;
             exit(0);                                    //进程正常退出
        }
   }
   if(WaitGpsReportFlg)                                  //上拉GPS等待  包括上拉WLU、MS
   {
        WaitGpsReportTimer++;
        if(WaitGpsReportTimer >500)
        {
           WaitGpsReportTimer=0;
           WaitGpsReportFlg=false;
            //GPS 上拉超时
            //回复中心

        }
   }
   if(WaitRevivFlg)                                        //等待激活  包或激活WLU  、MS
   {
        WaitRevivAckTime++;
        if(WaitRevivAckTime> 30)
        {
             WaitRevivFlg=false;
        //执行激活命令超时
        //回复中心执行失败
        }
   }

}

/**
* @brief 定时器处理函数
* @param [in] signo 信号值
* @author 牛功喜
* @since
* @bug
*/

void timer_handler (int signo)
{
    switch(signo)
    {
    case SIGALRM:
        timercallback();
        break;
    default:
        break;
    }
}

/**
* @brief  检查套接字是否可读 无数据可读阻塞时间到则返回
* @param [in] sockfd    套接字描述符
* @param [in] nsec阻塞等待时间
* @return     0:套接字可读    -1:套接字不可读
* @author 牛功喜
* @since
* @bug
*/

int  isSocketReadble(int sockfd,int nsec)
{
   struct timeval  timeval;
   int n,len,error,getoptreturn;
   fd_set  rset;
   FD_ZERO(& rset);
   FD_SET(sockfd,&rset);
   timeval.tv_sec=nsec;
   timeval.tv_usec=0;
   error=0;
    if((n=select(sockfd+1,&rset,NULL,NULL,nsec? &timeval:NULL))<=0)   //timeout
    {
        if(0== n)
        {
            LOG_DEBUG(s_LogMsgId,"[CCL][%s] readbletime out return=%d", __FUNCTION__,n);
        }
        else
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]select err =%d", __FUNCTION__,n);
            len=sizeof(error);
            if((getoptreturn=getsockopt(sockfd, SOL_SOCKET, SO_ERROR,&error, (socklen_t *)&len))<0)     //solaris  return -1, berkeley return 0  if erro appears
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] readble getoptretur =%d", __FUNCTION__,getoptreturn);
                LOG_ERROR(s_LogMsgId,"[CCL][%s]readble getoptretur  errno =%d", __FUNCTION__,errno);
            }
            if(error)
            {
                 LOG_ERROR(s_LogMsgId,"[CCL][%s]readble getoptretur  error=%d", __FUNCTION__,error);
                 errno=error;
            }
        }

        return -1;
    }
    else
    {
        if(FD_ISSET(sockfd,&rset))//   readsocket  is ready
        {
             return 0;
        }
        else
        {
             return -1;
        }
    }
}
/**
* @brief  CCL 套接字初始化函数
* @author 牛功喜
* @since
* @bug
*/
int CCL_init()
{
    //set_ticker(TIMER_INTERVAL);        // 设置定时时间
    //signal(SIGALRM, timer_handler);    // 定时回调函数
    INF_CCL_STATE =INF_CCL_IDLE;         // s_tCclVioceUlAddr

    tcclPrint=(CCL_PRINT_T*)ptIPCShm->ccl_printf;
    memset((UINT8 *)tcclPrint, 0x00, sizeof(CCL_PRINT_T));
    s_tCclSigUlSockfd = socket(AF_INET, SOCK_DGRAM, 0);      // 处理中心信令套接字 端口初始化


    //呼叫控制信令上行
    if(0 > s_tCclSigUlSockfd )
    {
        printf("creat  CclSigUlSockfd error\n");
        LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  CclSigUlSockfd error", __FUNCTION__);
        return -1;
    }

    bzero(&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));
    s_tCclSigUlAddr.sin_family = AF_INET;
    s_tCclSigUlAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tCclSigUlAddr.sin_port = htons(SOCK_PORT_CCL_CC_S);

    //呼叫控制层语音上行
    s_tCclVioceUlSockfd = socket(AF_INET, SOCK_DGRAM, 0);        //呼叫控制层语音上行套接字 端口初始化

    if(0 > s_tCclVioceUlSockfd )
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  CclVioceUlSockfd  error", __FUNCTION__);
        printf("creat  CclVioceUlSockfd  error\n");

        return -1;
    }

    bzero(&s_tCclVioceUlAddr, sizeof(s_tCclVioceUlAddr));
    s_tCclVioceUlAddr.sin_family = AF_INET;
    s_tCclVioceUlAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tCclVioceUlAddr.sin_port = htons(SOCK_PORT_CCL_CC_V);


    //中心信令下行
    s_tCenterSigSockfd = socket(AF_INET, SOCK_DGRAM, 0);         //处理中心信令套接字 端口初始化

    if(0 > s_tCenterSigSockfd )
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  centersigfd err", __FUNCTION__);
        printf("creat  centersigfd err\n");

        return -1;
    }

    bzero(&s_tCenterSigAddr, sizeof(s_tCenterSigAddr));
    s_tCenterSigAddr.sin_family = AF_INET;
    s_tCenterSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tCenterSigAddr.sin_port = htons(SOCK_PORT_CC_CCL_S);
    //bind(s_tCenterSigSockfd,(struct sockaddr *)&s_tCenterSigAddr,sizeof(s_tCenterSigAddr));              //??????????
    if (0 > bind(s_tCenterSigSockfd,(struct sockaddr *)&s_tCenterSigAddr,sizeof(s_tCenterSigAddr)))
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s] Bind CenterVoiceSockfd : %s", __FUNCTION__, strerror(errno));
        printf("Bind CenterVoiceSockfd err\n");
        return -1;
    }
    //中心语音下行
     s_tCenterVoiceSockfd = socket(AF_INET, SOCK_DGRAM, 0);  //处理中心语音套接字初始化
     if( 0> s_tCenterVoiceSockfd )
     {
           LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  centervoicefd err !! ", __FUNCTION__);
           printf("creat CenterVoiceSockfd err\n");
           return -1;
     }
     bzero(&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr));                                          //??????????????????????
     s_tCenterVoiceAddr.sin_family = AF_INET;
     s_tCenterVoiceAddr.sin_addr.s_addr = htonl(INADDR_ANY);
     s_tCenterVoiceAddr.sin_port = htons(SOCK_PORT_CC_CCL_V);
     //bind( s_tCenterVoiceSockfd,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterVoiceAddr));
     if (0 > bind( s_tCenterVoiceSockfd, (struct sockaddr *)&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr)))
     {
         LOG_ERROR(s_LogMsgId,"[CCL][%s] Bind CenterVoiceSockfd : %s", __FUNCTION__, strerror(errno));
         printf("Bind CenterVoiceSockfd err\n");
         return -1;
     }
    //发送数据链路层
      s_tSendToDllSocket = socket(AF_INET, SOCK_DGRAM, 0);     //发送到数据链路层套接字初始化
     if(0 > s_tSendToDllSocket )
     {
           LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  send   dll err  !! ", __FUNCTION__);
            printf("creat  send dll err\n");
          return -1;
     }
     bzero(&s_tSendToDllAddr, sizeof(s_tSendToDllAddr));
     s_tSendToDllAddr.sin_family = AF_INET;
     s_tSendToDllAddr.sin_addr.s_addr =  inet_addr("127.0.0.1");
     s_tSendToDllAddr.sin_port = htons(SEND_DLL_PORT);
    //接收数据链路层
     s_tRecFrmDllSocket = socket(AF_INET, SOCK_DGRAM, 0);      //接收数据链路层数据套接字初始化
     if(0 > s_tRecFrmDllSocket )
     {
          LOG_ERROR(s_LogMsgId,"[CCL][%s]creat  REC dll err !! ", __FUNCTION__);
          printf("creat  REC dll err    \n");
          return -1;
     }
     bzero(&s_tRecFrmDllAddr, sizeof(s_tRecFrmDllAddr));
     s_tRecFrmDllAddr.sin_family = AF_INET;
     s_tRecFrmDllAddr.sin_addr.s_addr = htonl(INADDR_ANY);
     s_tRecFrmDllAddr.sin_port = htons(REC_DLL_PORT);
    // bind(s_tRecFrmDllSocket,(struct sockaddr *)&s_tRecFrmDllAddr,sizeof(s_tRecFrmDllAddr));      //检查返回值并打印???????

     if (0 > bind(s_tRecFrmDllSocket,(struct sockaddr *)&s_tRecFrmDllAddr,sizeof(s_tRecFrmDllAddr)))
     {
         LOG_ERROR(s_LogMsgId,"[CCL][%s] Bind RecFrmDllSocket : %s", __FUNCTION__, strerror(errno));
          printf("Bind RecFrmDllSocket err  \n");
         return -1;
     }
      thread_create();
      LOG_ERROR(s_LogMsgId,"[CCL][%s] TIME OUT SET :NAS service =%d :MS stun=%d , revive=%d ,gps=%d", __FUNCTION__, \
                                                               ptCFGShm->work_time_out.val,ptCFGShm->stun_time_out.val,\
                                                               ptCFGShm->start_time_out.val,ptCFGShm->gps_time_out.val);

     // pthread_join(s_tHandleCenterViocethread,NULL);
     // pthread_join(s_tHandleCenterSigthread,NULL);
     // pthread_join(s_tHandletimerthread,NULL);
     return 0;
}

/**
* @brief  处理中心语音线程函数
* @author 牛功喜
* @since
* @bug
*/
void *ODP_HandleCenterVioce(void *arg)
{
    int  u4Datalen;
    int  u4sendlen;
    fd_set fdset;
    struct timeval tv;

    unsigned  char  uSendBuf[768]={0};
    unsigned  char  uRecSigBuf[768]={0};
    s_tFramType=0;
    int ret;
    for(;;)
    {
        FD_ZERO(&fdset);
        FD_SET(s_tCenterVoiceSockfd, &fdset);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select((s_tCenterVoiceSockfd+1), &fdset, NULL, NULL, &tv);

        if (ret < 0)
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s] select error", __FUNCTION__);
            continue;
        }
        else if (ret == 0)
        {
            continue;
        }


        u4Datalen= recvfrom(s_tCenterVoiceSockfd, uRecSigBuf, sizeof(uRecSigBuf), 0, NULL, NULL);

        if (u4Datalen  < (CENTER_VOICE_DL_HEADLEN + CENTER_VOICE_DL_PAYLOADLEN))   //接收数据长度小于公共头长度
        {
            printf("REC  VoiceSockfd  len erro=%d\n",errno);
            LOG_WARNING(s_LogMsgId,"[CCL][%s]REC  VoiceSockfd  len=%d  erro=%d", __FUNCTION__, u4Datalen, errno);
            continue;
        }

        if( s_tRec_PttOFF_FLG)
        {
            LOG_WARNING(s_LogMsgId,"[CCL][%s]Rec_PttOFF_FLG !!", __FUNCTION__);
            continue;
        }

        if (CENTER_VOICE_DL == INF_CCL_STATE)
        {
            s_tFramType++;
            TimeCount = 0;
            if (0 == (s_tFramType % 6))
            {
                if (0 == s_tFramType)
                {
                    s_tFramType = FT_VOICE_A;
                }
                else
                {
                    s_tFramType = FT_VOICE_F;
                }
            }
            else
            {
                s_tFramType = s_tFramType % 6;
            }

            packetnumber++;
            ODP_GenVoicepacket(uRecSigBuf, uSendBuf, s_tFramType, &u4Datalen);

            u4sendlen = sendto(s_tSendToDllSocket, uSendBuf, u4Datalen, 0, (struct sockaddr *)&s_tSendToDllAddr, sizeof(s_tSendToDllAddr));
            if (u4sendlen != u4Datalen)
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s]SEND DLL voice failed  u4Datalen =%d : u4sendlen =%d   FramTyp= %d  ", __FUNCTION__,u4Datalen,u4sendlen,s_tFramType);
                continue;
            }
        }
        else
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]SEND DLL voice failed  INF_CCL_STATE= %d  ", __FUNCTION__,INF_CCL_STATE);
        }
    }
   return 0;
}

/**
* @brief    封装中心信令下行数据包
* @param [in] pvCenterData  接收中心数据
* @param [in] pvDllData 下行到链路层数据包指针
* @author 牛功喜
* @since
* @bug
*/

void ODP_CcSmsSigHandle(unsigned char *pvCenterData, unsigned char *pvDllData)
{
    int u4Datalen;
    CCL_DLL_DL_T *ptDllData = (CCL_DLL_DL_T *)pvDllData;
    if ((INF_CCL_IDLE == INF_CCL_STATE) || ((ptDllData->DstId[0] == g_DllGlobalCfg.auNodeId) && (ptDllData->DstId[1] == 0) && (ptDllData->DstId[2] == 0))) //本地节点 则命令直接下发
    {
        SavCcIDNetData(pvCenterData, s_aCCcvNetId);
        ODP_GenSigCtrlDataPacket(pvCenterData, pvDllData, &u4Datalen);
        sendto(s_tSendToDllSocket, pvDllData, u4Datalen, 0, (struct sockaddr *)&s_tSendToDllAddr, sizeof(s_tSendToDllAddr));
    }
    else
    {
        LOG_WARNING(s_LogMsgId,"[CCL][%s]CCL BUSY cclstate=%d", __FUNCTION__, INF_CCL_STATE);
    }
}
/**
* @brief  处理中心信令线程
* @author 牛功喜
* @since
* @bug
*/
void *ODP_HandleCenterSig(void *arg)
{
    int u4Datalen;
    SHARE_CC_DATA_D *CcCmd;
    unsigned char PttCmdResult = 0;
    unsigned char SendBuf[768] = {0};           //最大长度
    unsigned char RecSigBuf[768] = {0};
    unsigned char i;
    //unsigned char SIGTYPE;
    fd_set fdset;
    struct timeval tv;
    int ret;

    for(;;)
    {
        FD_ZERO(&fdset);
        FD_SET(s_tCenterSigSockfd, &fdset);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select((s_tCenterSigSockfd+1), &fdset, NULL, NULL, &tv);

        if (ret < 0)
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s] select error", __FUNCTION__);
            continue;
        }
        else if (ret == 0)
        {
            continue;
        }

        u4Datalen = recvfrom(s_tCenterSigSockfd, RecSigBuf, sizeof(RecSigBuf), 0, NULL, NULL);
        if (u4Datalen < 0)     //接收错误
        {
            continue;
        }
        else if (u4Datalen < CENTER_SHARE_HEAD_LEN)     //接收数据长度小于公共头长度
        {
            LOG_ERROR(s_LogMsgId, "[CCL][%s]REC CC Sig len=%d", __FUNCTION__, u4Datalen);
            continue;
        }
        else if (0x13 != RecSigBuf[0] || 0xec != RecSigBuf[1])    //消息头接收错误
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]REC CC header err buf[0]=%2.2x buf[1]=%2.2x reclen=%d ", __FUNCTION__,RecSigBuf[0], RecSigBuf[1], u4Datalen);
            continue;
        }
        else
        {
        }

        CcCmd = (SHARE_CC_DATA_D *)RecSigBuf;
        if (1 == tcclPrint->CcDown)
        {
            ODP_CclPrintCcSig((unsigned char *)CcCmd);
        }

        /*if(Wait_Stun_Nas_Ack == INF_CCL_STATE || Wait_Kill_Nas_Ack == INF_CCL_STATE || INF_CCL_DISABLE== INF_CCL_STATE
            || ((NAS_STAT_STUNED == INF_CCL_STATE)&& (REVIVE_REQ_NAS !=CcCmd->CcSmsSig.SmsType)))
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s]REC CC Sig NAS busy   cclworkstate=%d ", __FUNCTION__,INF_CCL_STATE);
                continue;
            }*/
        //if (g_DllGlobalCfg.auStunFlag == NAS_KILL_FLAG || g_DllGlobalCfg.auKillFlag == NAS_KILL_FLAG)// 返回状态标志

        switch (CcCmd->CcSmsSig.SharedHead.SigType)
        {
            case SIG_PTT_CMD:   // 中心下达PTT命令
            {
                if ((INF_CCL_IDLE != INF_CCL_STATE ) && (CENTER_VOICE_DL != INF_CCL_STATE))
                {
                    LOG_ERROR(s_LogMsgId,"[CCL][%s]REC CC SIG CCL busy  state=%d ", __FUNCTION__,INF_CCL_STATE);
                    PttCmdResult=false;
                    IDP_GenPttOnAckpacket(SendBuf, &u4Datalen, PttCmdResult);
                    break;
                }
                if (CMD_PTT_ON == CcCmd->CC_PTT_CMD.PttStat)    //压手台命令
                {
                    //if(NAS_VOICE_UL == INF_CCL_STATE || p_DllFpgaShm->FollowEn)
                    if(p_DllFpgaShm->FollowEn)
                    {
                        PttCmdResult = false;
                        LOG_WARNING(s_LogMsgId,"[CCL][%s]REC CC SIG CCL  state=%d  FollowEn=%d", __FUNCTION__,INF_CCL_STATE,p_DllFpgaShm->FollowEn);
                    }
                    else
                    {
                        PttCmdResult = true;
                        ODP_GenLcHeader(RecSigBuf, SendBuf, &u4Datalen);    // 封LC语音头
                        sendto(s_tSendToDllSocket, SendBuf, u4Datalen, 0,(struct sockaddr *)&s_tSendToDllAddr,sizeof(s_tSendToDllAddr));
                        INF_CCL_STATE   = CENTER_VOICE_DL;
                    }
                    IDP_GenPttOnAckpacket(SendBuf,&u4Datalen,PttCmdResult);
                    sendto(s_tCclSigUlSockfd, SendBuf, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));   //(struct sockaddr *)&address
                }
                else if (CMD_PTT_OFF == CcCmd->CC_PTT_CMD.PttStat)    //释放手台命令
                {
                    packetnumber = 0;
                    if (CENTER_VOICE_DL == INF_CCL_STATE )
                    {
                        s_tRec_PttOFF_FLG = 1;
                        if(0 != s_tFramType % 6)
                        {
                            s_tFramType++;
                            for(i = s_tFramType; i <= FT_VOICE_F; i++)
                            {
                                //delay(45);                                        //45ms 延时
                                TimeCount = 0;
                                ODP_GenSilentFrmpacket((unsigned  char*)s_au2AmbeEraData, SendBuf, i, &u4Datalen);
                                sendto(s_tSendToDllSocket, SendBuf, u4Datalen, 0, (struct sockaddr *)&s_tSendToDllAddr, sizeof(s_tSendToDllAddr));
                            }
                        }
                        s_tFramType = 0;
                        s_tRec_PttOFF_FLG = 0;
                        INF_CCL_STATE = INF_CCL_IDLE;

                        ODP_GenTerminatorPacket(RecSigBuf, SendBuf, &u4Datalen);
                        sendto(s_tSendToDllSocket, SendBuf, u4Datalen, 0, (struct sockaddr *)&s_tSendToDllAddr, sizeof(s_tSendToDllAddr));
                        IDP_GenPttOffAckpacket(SendBuf, &u4Datalen);
                        sendto(s_tCclSigUlSockfd, SendBuf, u4Datalen, 0, (struct sockaddr *)&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));
                    }
                    else
                    {
                        IDP_GenPttOffAckpacket(SendBuf, &u4Datalen);   //PTT-ON 失败 也回 PTT-OFF  ack
                        sendto(s_tCclSigUlSockfd, SendBuf, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));
                        LOG_DEBUG(s_LogMsgId,"[CCL][%s] rec PTT OFF  INF_CCL_STATE= %d", __FUNCTION__,INF_CCL_STATE);
                    }
                }
                else
                {
                    LOG_DEBUG(s_LogMsgId,"[CCL][%s]PTT OFF   != DL", __FUNCTION__);
                }
                break;
            }
            case SIG_PTT_ON_ACK:
            {
                INF_CCL_STATE = NAS_VOICE_UL;
                s_tLast_Relay_Flg = 0;          // 接收PTT-ON ack 清除转发标志
                IDP_PttonAckhandle(RecSigBuf);
                break;
            }
            case SIG_PTT_OFF_ACK:
            {
                break;
            }
            case SIG_SMS_SEND:
            {
                ODP_CcSmsSigHandle(RecSigBuf, SendBuf);
                break;
            }
            default:
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s] CC_SIG _SMS default  systype=%d", __FUNCTION__, CcCmd->CcSmsSig.SharedHead.SigType);
                break;
            }
        }
     }
     return 0;
}
/**
* @brief CCL处理DLL 数据类型消息
* @param [in] pvDllData  接收DLL数据
* @author 牛功喜
* @since
* @bug
*/

void IDP_MsgDataHandle(unsigned char * pvDllData, unsigned char * CenterData)
{
    int u4Datalen;
    int u4sendlen;
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;
    IDP_PrintMsgDatalog(ptDllData); // by zhoudayuan
    
    switch(ptDllData->DataType)
    {
        case CT_LC_HEADER:                                  //  LC 头
        {
            s_tRec_LC_FLG=1;
            IDP_Getlcdata(pvDllData);
            //INF_CCL_STATE=NAS_VOICE_UL;
            break;
        }
        case CT_LC_TERMINATOR:                              //LC 终结帧
        {
            TimeCount=0;
            s_tRec_LC_FLG=0;
            s_tLast_Relay_Flg=0;
            s_tNas_STOP_RELAY=0;

            if (s_tSend_PTTON_FLG)
            {
                IDP_GenPttCmd(pvDllData, CenterData, CMD_PTT_OFF, &u4Datalen);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));
                s_tSend_PTTON_FLG=0;
            }
            INF_CCL_STATE=INF_CCL_IDLE;    // 无法收到 LC 终结帧  要设置超时自恢复状态
            break;
        }
        case CT_PACKET_DATA:    // 短消息
        {
            IDP_GenSmsPacket(pvDllData,CenterData,&u4sendlen);
            sendto(s_tCclSigUlSockfd, CenterData, u4sendlen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        case CT_GPS_REPORT_ACK_MS:    // MS GPS上拉
        {
            if(Wait_Gps_Ms_Ack == INF_CCL_STATE)
            {
                IDP_GenGpsData(CT_GPS_REPORT_ACK_MS,pvDllData,CenterData,&u4Datalen,s_aCCcvNetId);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
                INF_CCL_STATE=INF_CCL_IDLE;
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s]Ms_Ack CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;

        }
        case CT_DISABLE_ACK_MS:    // 遥晕MS
        {
            if (Wait_Stun_Ms_Ack == INF_CCL_STATE)
            {
                IDP_GenMsAckData(STUN_REQ_MS_ACK, pvDllData, CenterData, &u4Datalen, s_aCCcvNetId);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
                INF_CCL_STATE=INF_CCL_IDLE;
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s]Ms_Ack CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;

        }
        case CT_ENABLE_ACK_MS:                                //激活MS
        {
            if(Wait_Reviv_MS_Ack == INF_CCL_STATE)
            {
                IDP_GenMsAckData(REVIVE_REQ_MS_ACK,pvDllData,CenterData,&u4Datalen,s_aCCcvNetId);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
                INF_CCL_STATE=INF_CCL_IDLE;
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s]Ms_Ack CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_DISCON_ALARM:
        {
            //IDP_GenAlarm_ClearData(unsigned char type,char * pvDlldata,unsigned char * pvCenterData,int *Len);
            IDP_GenAlarm_ClearData(DISCON_ALARM, pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        case CT_ALARM_REQ_MS:   //终端告警
        {
            //IDP_GenAlarm_ClearData(unsigned char type,char * pvDlldata,unsigned char * pvCenterData,int *Len);
            IDP_GenAlarm_ClearData(MS_ALARM, pvDllData,CenterData, &u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        case CT_DISCON_ALARM_CLEAR:   //告警清除
        {
            IDP_GenAlarm_ClearData(MS_ALARM_CLEAR, pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        default:
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s] CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            break;
        }
    }
}



/**
* @brief CCL 处理DLL 语音类型消息
* @param [in] pvDllData  接收DLL数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_MsgVoiceHandle(unsigned char *pvDllData, unsigned char *CenterData)
{
    int u4Datalen;
    int u4sendlen;
    DLL_CCL_UL_T * ptDllData=(DLL_CCL_UL_T *)pvDllData;

    if ((FT_VOICE_A > ptDllData->FrmType) || (FT_VOICE_F < ptDllData->FrmType))
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] FT_VOICE ERROR ", __FUNCTION__);
        return ;
    }
    if ((INF_CCL_IDLE != INF_CCL_STATE) && (NAS_VOICE_UL != INF_CCL_STATE))
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s] ccl is busy state=%d", __FUNCTION__, INF_CCL_STATE);
        return;
    }


    if (1 == s_tRec_LC_FLG)
    {
        if(0 == s_tSend_PTTON_FLG) 
        {
            memset(CenterData, 0, sizeof(CenterData));
            IDP_GenPttCmd(pvDllData, CenterData, CMD_PTT_ON, &u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0, (struct sockaddr *)&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address

            //s_tSend_PTTON_FLG=1;
            if (1 == tcclPrint->CclUp)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s] SEND CC PTTON, Rec_LC_FLG=%d Send_PTTON_FLG=%d", __FUNCTION__, s_tRec_LC_FLG, s_tSend_PTTON_FLG);
            }

            if (1 == s_tNas_STOP_RELAY)
            {
                memset(CenterData,0,sizeof(CenterData));
                IDP_GenNasStatePack(s_tLast_Relay_Flg, CenterData, &u4Datalen);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0, (struct sockaddr *)&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));
                if(1 == tcclPrint->CclUp)
                {
                    LOG_DEBUG(s_LogMsgId,"[CCL][%s] SEND CC STOP RELAY!! " , __FUNCTION__ );
                }
                s_tNas_STOP_RELAY=0;
            }
        }
        else 
        {
            CENTER_VOICE_DATA * pCenterData;
//            printf("[%s:%d]@@@@ \n", __FUNCTION__, __LINE__);
            memset(CenterData,0,sizeof(CenterData));
            IDP_GenVoiceDataPaket(pvDllData,CenterData,&u4Datalen);   //全局计算方差平均值
            pCenterData = (CENTER_VOICE_DATA *)CenterData;
//            print_voice_153(pCenterData->Payload, "CCL-idp");


            u4sendlen=sendto(s_tCclVioceUlSockfd, CenterData, u4Datalen, 0, (struct sockaddr *)&s_tCclVioceUlAddr, sizeof(s_tCclVioceUlAddr));  //(struct sockaddr *)&address
            if (u4sendlen != u4Datalen)
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] SEND UP VOICE ERROR  LEN=%d", __FUNCTION__,u4sendlen);
            }
            if (FT_VOICE_F == ptDllData->FrmType)       // 发送超帧方差平均值
            {
                IDP_GenThrethHoldData(pvDllData, CenterData, &u4Datalen);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            }
            if (1 == s_tNas_STOP_RELAY)                  // 发送转发标志
            {
                memset(CenterData,0,sizeof(CenterData));
                IDP_GenNasStatePack(s_tLast_Relay_Flg,CenterData,&u4Datalen);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));
                if(1 == tcclPrint->CclUp)
                {
                    LOG_DEBUG(s_LogMsgId,"[CCL][%s]SEND CC STOP RELAY!! " , __FUNCTION__ );
                }
                s_tNas_STOP_RELAY=0;
            }

        }
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]call error s_tRec_LC_FLG = %d!! " , __FUNCTION__, s_tRec_LC_FLG);
    }
}




static int IDP_IsLocalNasAck(DLL_CCL_UL_T *ptDllData)
{
    // 是否为本地Nas-ACK, 即自己使能自己
    int status = (((ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId) && (ptDllData->SrcId[1] == 0) && (ptDllData->SrcId[2] == 0))) 
        && (memcmp(ptDllData->SrcId, ptDllData->DstId, sizeof(ptDllData->SrcId)) == 0);
    LOG_DEBUG(s_LogMsgId,"[CCL][%s] %s" , __FUNCTION__, (status==1? "Local Nas ACK": "Err: NOT local, check Nas ID"));
    return status;
}



/**
* @brief CCL 处理DLL  链路数据类型消息
* @param [in] pvDllData  接收DLL数据
* @author 牛功喜
* @since
* @bug
*/
void IDP_MsgWluHandle(unsigned char *pvDllData, unsigned char *CenterData)
{
    int u4Datalen;
    DLL_CCL_UL_T *ptDllData = (DLL_CCL_UL_T *)pvDllData;

    IDP_PrintMsgDatalog(ptDllData); // by zhoudayuan
    
    switch(ptDllData->DataType)
    {
        case CT_GPS_REPORT_ACK_NAS:     // 链路机GPS
        {
//            if( (Wait_Gps_Nas_Ack == INF_CCL_STATE) || (ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId && ptDllData->SrcId[1] == 0 && ptDllData->SrcId[2] == 0))
            if( (Wait_Gps_Nas_Ack == INF_CCL_STATE) || IDP_IsLocalNasAck(ptDllData))
            {
                IDP_GenGpsData(CT_GPS_REPORT_ACK_NAS,pvDllData,CenterData,&u4Datalen,s_aCCcvNetId);
                INF_CCL_STATE=INF_CCL_IDLE;
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_STUN_ACK_NAS:
        {
//            if( (Wait_Stun_Nas_Ack == INF_CCL_STATE) || (ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId && ptDllData->SrcId[1] == 0 && ptDllData->SrcId[2] == 0 ))
            if( (Wait_Stun_Nas_Ack == INF_CCL_STATE) || IDP_IsLocalNasAck(ptDllData))
            {
                IDP_GenNasSigAckData(CT_STUN_ACK_NAS, pvDllData, CenterData, &u4Datalen);
                LOG_DEBUG(s_LogMsgId, "[CCL][%s] Stun_Nas_Ack INF_CCL_STATE=%d", __FUNCTION__, INF_CCL_STATE);
                INF_CCL_STATE = INF_CCL_IDLE;
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0, (struct sockaddr *)&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_KILL_ACK_NAS:
        {
//            if( (Wait_Kill_Nas_Ack == INF_CCL_STATE) || (ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId && ptDllData->SrcId[1] == 0 && ptDllData->SrcId[2] == 0))
            if( (Wait_Kill_Nas_Ack == INF_CCL_STATE) || IDP_IsLocalNasAck(ptDllData))
            {
                IDP_GenNasSigAckData(CT_KILL_ACK_NAS,pvDllData,CenterData,&u4Datalen);
                INF_CCL_STATE=INF_CCL_IDLE;
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_ENABLE_ACK_NAS:
        {
//            if( (Wait_Reviv_NAS_Ack == INF_CCL_STATE)  || (ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId && ptDllData->SrcId[1] == 0 && ptDllData->SrcId[2] == 0))
            if( (Wait_Reviv_NAS_Ack == INF_CCL_STATE)  || IDP_IsLocalNasAck(ptDllData))
            {
                IDP_GenNasSigAckData(CT_ENABLE_ACK_NAS,pvDllData,CenterData,&u4Datalen);
                LOG_DEBUG(s_LogMsgId,"[CCL][%s]  Reviv_NAS_Ack INF_CCL_STATE =%d ", __FUNCTION__,INF_CCL_STATE);
                INF_CCL_STATE=INF_CCL_IDLE;
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] NAS CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_NEGHR_QUERY_ACK:
        {
//            if(( Wait_NEGHR_Nas_Ack == INF_CCL_STATE)  || (ptDllData->SrcId[0] == g_DllGlobalCfg.auNodeId && ptDllData->SrcId[1] == 0 && ptDllData->SrcId[2] == 0))
            if(( Wait_NEGHR_Nas_Ack == INF_CCL_STATE)  || IDP_IsLocalNasAck(ptDllData))
            {
                IDP_GenNearData(NEAR_REPORT_PASSIVE,pvDllData,CenterData,&u4Datalen,s_aCCcvNetId);
                sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
                INF_CCL_STATE=INF_CCL_IDLE;
            }
            else
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] CCL STAT ERROR  stata=%d", __FUNCTION__,INF_CCL_STATE);
            }
            break;
        }
        case CT_NEGHR_REPORT:
        {
            IDP_GenNearData(NEAR_REPORT_ACTIVE,pvDllData,CenterData,&u4Datalen,s_aCCcvNetId);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        /*case CT_STUN_REQ_NAS:
            IDP_GenNasSigAckData(CT_STUN_REQ_NAS,pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        case CT_ENABLE_REQ_NAS:
            IDP_GenNasSigAckData(CT_ENABLE_REQ_NAS,pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        case CT_KILL_REQ_NAS:
            IDP_GenNasSigAckData(CT_KILL_REQ_NAS,pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;*/
            //回复忙状态 ????
        case CT_STUN_RPT_NAS:
        {
            IDP_GenNasSigAckData(CT_STUN_RPT_NAS, pvDllData, CenterData, &u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0, (struct sockaddr *)&s_tCclSigUlAddr, sizeof(s_tCclSigUlAddr));
            break;
        }
        case CT_KILL_RPT_NAS:
        {
            IDP_GenNasSigAckData(CT_KILL_RPT_NAS,pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        case CT_ENABLE_RPT_NAS:
        {
            IDP_GenNasSigAckData(CT_ENABLE_RPT_NAS,pvDllData,CenterData,&u4Datalen);
            sendto(s_tCclSigUlSockfd, CenterData, u4Datalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));  //(struct sockaddr *)&address
            break;
        }
        default:
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s] DI_MGS_WLU parameter error ", __FUNCTION__);
            break;
        }
    }
}

/**
* @brief CCL 接收DLL数据线程
* @author 牛功喜
* @since
* @bug
*/
void *IDP_RecFrmDllthread(void *arg)
{
    int  u4Datalen;
    //int  u4sendlen;
    unsigned char uSendBuf[512] = {0};
    unsigned char uRecSigBuf[1024] = {0};
    DLL_CCL_UL_T *ptDllData = NULL;
    fd_set fdset;
    struct timeval tv;
    int ret;

    for(;;)
    {
        FD_ZERO(&fdset);
        FD_SET(s_tRecFrmDllSocket, &fdset);
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select((s_tRecFrmDllSocket+1), &fdset, NULL, NULL, &tv);
        if (ret < 0)
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s] select error", __FUNCTION__);
            continue;
        }
        else if (ret == 0)
        {
            continue;
        }

        u4Datalen= recvfrom(s_tRecFrmDllSocket, uRecSigBuf, sizeof(uRecSigBuf), 0, NULL, NULL);
        if( u4Datalen < DLL_CCL_MSG_HEADLEN)
        {
            LOG_ERROR(s_LogMsgId,"[CCL][%s]REC DLL lSockfd len =%d", __FUNCTION__, u4Datalen);
            continue;
        }

        ptDllData = (DLL_CCL_UL_T *)uRecSigBuf;
        if (1 == tcclPrint->DllUp)
        {
            IDP_CclPrintDllData((unsigned char*)ptDllData);
        }

        switch (ptDllData->MsgType)
        {
            case DI_MSG_DATA:   // 数据
            {
                if (CT_JUNK_DATA == ptDllData->DataType)
                {
                    LOG_DEBUG(s_LogMsgId,"[CCL][%s] REC JUNK data len=%d : ", __FUNCTION__, u4Datalen);
                    continue;
                }
                IDP_MsgDataHandle(uRecSigBuf, uSendBuf);
                break;
            }
            case DI_MSG_VOICE:  // 语音
            {
                TimeCount=0;
                IDP_MsgVoiceHandle( uRecSigBuf,uSendBuf);
                break;
            }
            case DI_MSG_NM:
            {
                LOG_ERROR(s_LogMsgId,"[CCL][%s] DI_MSG_NM  stata=%d", __FUNCTION__,INF_CCL_STATE);
                break;
            }
            case DI_MSG_WLU:   // WLU
            {
                IDP_MsgWluHandle(uRecSigBuf, uSendBuf);
                break;
            }
            default:
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s] DI_MGS_WLU MGStype error =%d ", __FUNCTION__,ptDllData->MsgType);
                break;
            }
        }
   }
    return 0;
}

/**
* @brief  定时器处理函数线程
* @author 牛功喜
* @since
* @bug
*/
void * Handletimerthread(void * arg)
{
    unsigned char  udata[256]={0};
    unsigned char  dlldata[30]={0};
    int  udatalen;
    while(1)
    {
        delay(30);
        switch (INF_CCL_STATE)
        {
        case Wait_Stun_Ms_Ack:
            TimeCount++;
            if((ptCFGShm->stun_time_out.val/30) <= TimeCount)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s]Stun_Ms timeout timecount=%d", __FUNCTION__,TimeCount);
                INF_CCL_STATE=INF_CCL_IDLE;
                TimeCount=0;
            }
            break;
        case Wait_Gps_Ms_Ack:
            TimeCount++;
            if((ptCFGShm->gps_time_out.val /30) <= TimeCount)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s]Gps_Ms timeout  timecount=%d", __FUNCTION__,TimeCount);
                INF_CCL_STATE=INF_CCL_IDLE;
                TimeCount=0;
            }
            break;
        case Wait_Reviv_MS_Ack:
            TimeCount++;
            if((ptCFGShm->gps_time_out.val/30) <= TimeCount)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s]Reviv_MS timeout  timecount=%d", __FUNCTION__,TimeCount);
                INF_CCL_STATE=INF_CCL_IDLE;
                TimeCount=0;
            }
            break;
        case Wait_Gps_Nas_Ack:
        case Wait_Stun_Nas_Ack:
        case Wait_Kill_Nas_Ack:
        case Wait_NEGHR_Nas_Ack:
        case Wait_Reviv_NAS_Ack:
            TimeCount++;
            if((ptCFGShm->work_time_out.val/30 )<= TimeCount)
            {
                LOG_DEBUG(s_LogMsgId,"[CCL][%s][%d] NAS SERVICE timeout ccl_work_state=%d timecount=%d", __FUNCTION__,__LINE__, INF_CCL_STATE,TimeCount);
                INF_CCL_STATE=INF_CCL_IDLE;
                TimeCount=0;
            }
            break;
        case NAS_VOICE_UL:
            TimeCount++;
            if(16 <= TimeCount)
            {
                IDP_GenPttCmd(dlldata,udata,CMD_PTT_OFF,&udatalen);  // 超时没收到TERMINATOR  则发PTT-OFF 命令
                sendto(s_tCclSigUlSockfd, udata, udatalen, 0,(struct sockaddr *)&s_tCclSigUlAddr,sizeof(s_tCclSigUlAddr));
                LOG_ERROR(s_LogMsgId,"[CCL][%s]  CCL VOICE TIME OUT STATE=%d ", __FUNCTION__,INF_CCL_STATE);
                INF_CCL_STATE=INF_CCL_IDLE;
                s_tRec_LC_FLG=0;
                s_tSend_PTTON_FLG=0;
                s_tLast_Relay_Flg=0;
                s_tNas_STOP_RELAY=0;
                TimeCount=0;
            }
            break;
        case CENTER_VOICE_DL:
            TimeCount++;
            if (15 <= TimeCount)
            {
                TimeCount=0;
                LOG_DEBUG(s_LogMsgId,"[CCL][%s][%d] NAS SERVICE timeout ccl_work_state=%d timecount=%d", __FUNCTION__, __LINE__, INF_CCL_STATE,TimeCount);
                INF_CCL_STATE=INF_CCL_IDLE;
            }
            break;
        case INF_CCL_IDLE:
        case NAS_STAT_STUNED:
        case INF_CCL_DISABLE:
            TimeCount=0;
            break;
        default:
            TimeCount=0;
            LOG_ERROR(s_LogMsgId,"[CCL][%s]  default INF_CCL_STATE =%d ", __FUNCTION__,INF_CCL_STATE);
            break;
        }
    }
    return 0;
}


/**
* @brief CCL 创建线程函数
* @author 牛功喜
* @since
* @bug
*/
int thread_create(void)
{
    int res=0;
    /*创建线程*/
    if((res = pthread_create(&s_tHandleCenterViocethread, NULL,ODP_HandleCenterVioce,NULL) != 0) )
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]Thread  HandleCenterVioce create failed", __FUNCTION__);
        return res ;
    }
    else
    {
         LOG_DEBUG(s_LogMsgId,"[CCL][%s]Thread HandleCenterVioce create", __FUNCTION__);
    }
    if((res = pthread_create(&s_tHandleCenterSigthread, NULL,ODP_HandleCenterSig,NULL) != 0) )
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]Thread HandleCenterSig create failed", __FUNCTION__);
        return res ;
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]Thread HandleCenterSig create", __FUNCTION__);
    }
    if((res = pthread_create(&s_tRecFrmDllthread, NULL, IDP_RecFrmDllthread,NULL) != 0) )
    {
        LOG_ERROR(s_LogMsgId,"[CCL][%s]Thread RecFrmDllthread  create failed", __FUNCTION__);
        return res ;
    }
    else
    {
        LOG_DEBUG(s_LogMsgId,"[CCL][%s]Thread  RecFrmDllthread create  ", __FUNCTION__);
    }
    if((res = pthread_create(&s_tHandletimerthread, NULL,Handletimerthread,NULL) != 0) )
    {
        printf("Thread    Handletimerthread  create failed \n ");
        return res ;
    }
    else
    {
        printf("Thread    Handletimerthread    create\n");
    }
    return res ;
}

/*void  main()
{
int i;
     CCL_init();
    i= set_ticker(TIMER_INTERVAL);                                  //设置定时时间
    printf("set_ticker=%d  \n", i);
     signal(SIGALRM, timercallback);    //定时回调函数
     thread_create();
    pthread_join(s_tHandleCenterViocethread,NULL);
    pthread_join(s_tHandleCenterSigthread,NULL);
    pthread_join(s_tHandletimerthread,NULL);
   // while(1)  s_tHandletimerthread
   // ;
}  */



/*void CCL_sendtodll(void *arg)
{


}*/


/*void  TIMER_ProcesTask(void *arg)
{


}

*/

