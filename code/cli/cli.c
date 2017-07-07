/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file    cli.c
 * @brief   为其它模块提供命令行接口，使各模块可以通过串口人机交互
 * @author  柳智奇
 * @version v1.0
 * @date    2010-10-13
 */
/*
 *   函数列表
 *   1. CLI_HexToStr                    将16进制无符号数转换成字符串
 *   2. CLI_StrToHex                    将字符串转换成16进制无符号数
 *   3. CLI_StrToDec                    将字符串转换成10进制无符号数
 *   4. CLI_RegisterCommand             注册一条命令行接口命令
 *   5. CLI_Printf                      以队列缓冲方式输出命令行接口打印信息
 *   6. CLI_DirPrintf                   直接输出命令行接口打印信息
 *   7. CLI_Init                        初始化命令行接口模块，分配资源占用
 *
 *   修改历史
 *   2016-08-27            建立文件                    张婷婷
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "typedef.h"

#include "route.h"
#include "cli.h"


#ifdef _ENABLE_CLI
/******************************************************************************
 *   类型定义
 *   *************************************************************************/
 #undef TASK_NAME
#define TASK_NAME	"CLI"
 /**
 * @def CLI_MAX
 * @brief 
 */
#define CLI_MAX    2   // 目前只支持两个命令字get 和set
 /**
 * @def CLI_PRINT_TIMEOUT
 * @brief CLI输出使能默认超时时间
 */
#define CLI_REG_TOTAL    4    // 4个模块
/**
 * @def CLI_PRINT_TIMEOUT
 * @brief CLI输出使能默认超时时间
 */
#define CLI_PRINT_TIMEOUT  4000    // 默认 2 min
/**
 * @def CLI_START_KEY
 * @brief 打开命令行输入/输出
 */
#define CLI_START_KEY "cli on"
/**
 * @def CLI_END_KEY
 * @brief 关闭命令行输入/输出
 */
#define CLI_END_KEY   "cli off"
/**
 * @def CLI_LS
 * @brief 输出当前用户路径信息
 */
#define CLI_LS        "ls"
/**
 * @def CLI_CD
 * @brief 切换用户路径
 */
#define CLI_CD        "cd"
/**
 * @def CLI_HELP
 * @brief 输出帮助信息
 */
#define CLI_HELP      "help"
/**
 * @def CLI_CD_UP
 * @brief CD参数,切换路径到本节点上级路径
 */
#define CLI_CD_UP     ".."
/**
 * @def CLI_CD_UP_DOWN
 * @brief CD参数,切换路径到本节点上级路径
 */
#define CLI_CD_UP_DOWN "../"
/**
 * @def CLI_CD_HERE
 * @brief CD参数,切换路径到本节点路径
 */
#define CLI_CD_HERE   "."
/**
 * @def CLI_CD_ROOT
 * @brief CD参数,切换路径到根节点路径
 */
#define CLI_CD_ROOT   "\\"
/**
 * @def CLI_PWD
 * @brief 查询当前用户路径
 */
#define CLI_PWD       "pwd"

/**
 * @def CLI_MAX_SIZE
 * @brief 命令行接口最大可用存储字节数
 */
#define CLI_MAX_SIZE (CLI_ARC_MAX * CLI_ARG_MAX)
#endif

#define ERR_ANABLE   1  // 误码率测试使能
#define ERR_DISABLE  0  // 误码率测试不使能

typedef enum _ERR_RATE_TEST_E
{
    ERR_INDX_VOICE,  // 误码率-语音测试
    ERR_INDX_21,     // 误码率-1/2数据业务
    ERR_INDX_43,     // 误码率-3/4数据业务
    ERR_INDX_ALL     // 所有开关都打开
} ERR_RATE_TEST_E;



/**
 * @struct CLI_CMD_NODE_T
 * @brief 命令行接口节点内部结构
 */
typedef struct _CLI_CMD_NODE_T
{
    CHAR acClass[16];                                        ///< 父节点名称
    CHAR acName[16];                                         ///< 本节点名称
    CHAR acDescription[128];                                 ///< 本节点描述
    STATUS  (*pfcallback)(UINT32 u4Arc, CHAR* pcArg[]);      ///< 本节点回调函数
    struct _CLI_CMD_NODE_T*   ptBrother;                     ///< 相邻节点指针地址
    struct _CLI_CMD_NODE_T*   ptChild;                       ///< 下级节点指针地址
    struct _CLI_CMD_NODE_T*   ptFather;                      ///< 上级节点指针地址
} CLI_CMD_NODE_T;


/**
 * @struct ERR_MARK_T
 * @brief 误码率结构标识
 */
typedef struct _ERR_MARK_T{
    const char *Name;
    unsigned char *Flag;
    const char *str[2];
} ERR_MARK_T;

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
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
 * @var s_hsocket
 * @brief 命令行套接字
 */
 int s_hsocket;
/**
 * @var i4FifoInFd
 * @brief 命令行输入接口描述符
 */
int i4FifoInFd;
/**
 * @var i4FifoOutFd
 * @brief 命令行输出接口描述符
 */
int i4FifoOutFd;

  /**
 * @var s_i4CLIMsgId
 * @brief 命令行套接字
 */
INT32 s_i4CLIMsgId;
/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/
static SHM_IPC_STRU *ptIPCShm = NULL;
static ROUTE_PRINTF *ptIPCRoutePrintf = NULL;
static DLL_PRINT_T *ptDllPrint = NULL;
static SHM_CFG_STRU *ptCFGShm = 	NULL;
static CCL_PRINT_T *ptCCLPrint = NULL;
static ERR_PRINT_T *ptERRPrint = NULL;
static ERR_MARK_T *pERR_MARK = NULL;  // 误码率测试项结构标识
static unsigned short s_len_ERR_MARK;  // 误码率测试项结构的长度
 /**
 * @var s_auCliRoute
 * @brief Route模块打印
 */
static UINT8  s_auCliRoute[1000];
 /**
 * @var s_auCliCCL
 * @brief Protocol模块打印
 */
static UINT8  s_auCliCCL[1000];
  /**
 * @var s_auCliDLL
 * @brief Protocol模块打印
 */
static UINT8  s_auCliDLL[1000];
  /**
 * @var s_auCliMgr
 * @brief Mgr模块打印
 */
static UINT8  s_auCliMgr[1000];
/**
 * @var s_tCliCmdList
 * @brief 命令行接口命令列表数组
 */
static CLI_CMD_NODE_T s_tCliCmdList;
/**
 * @var s_ptCurCliCmd
 * @brief 命令行接口当前用户命令节点指针
 */
static CLI_CMD_NODE_T* s_ptCurCliCmd;
/**
 * @var s_tCliNodePool
 * @brief 命令行接口节点分配缓冲区
 */
static CLI_CMD_NODE_T s_tCliNodePool[CLI_NODE_NUM] = {0};

/**
 * @var s_tCliInputTask
 * @brief 命令行接口数据输入任务主函数
 */
static pthread_t s_tCliInputTask;
/**
 * @var s_tCliRegTask
 * @brief 命令行注册
 */
static pthread_t s_tCliRegTask;
/**
 * @var s_bEnableCli
 * @brief 命令行接口使能标识
 */
volatile BOOL       s_bEnableCli    = FALSE;
/**
 * @var tPreTime
 * @brief CLI输入时间点记录值
 */
struct timeval tPreTime;
   /**
 * @var tCurTime
 * @brief 下行DMR帧计数
 */                
struct timeval tCurTime;
                    
/**
 * @def CLI_PRINT_TIMEOUT
 * @brief CLI输出使能超时时间
 */
UINT32  s_u4CLI_TIMEOUT  = CLI_PRINT_TIMEOUT;

static int  pfRouteGet(UINT32 u4Arc, CHAR* pcArg[])
{
     if(0 ==u4Arc)
    {
		CLI_Printf("get select    - 0,LogView 1,local.\n");
		CLI_Printf("get upstatus    - 0,up free  1,up busy.\n");
    }
    else if(1 ==u4Arc)
    {
   
		if(0 == strncmp(pcArg[0], "select", sizeof("select")))
		{
		
				if(ptIPCShm->printf_select == 0)
				{
					CLI_Printf("Select NASLogView as Output\n");
				}		
				else if(ptIPCShm->printf_select == 1)
				{
					CLI_Printf("Select Local as Output\n");
				}
				else
				{
					CLI_Printf("Output  error!!!\n");
				}
		}
		else if(0 == strncmp(pcArg[0], "upstatus", sizeof("upstatus")))
		{
			
			if(ptIPCRoutePrintf->uUpBusy== 0)
			{
				CLI_Printf("AI up Free\n");
			}		
			else if(ptIPCRoutePrintf->uUpBusy== 1)
			{
				CLI_Printf("AI up Busy\n");
			}
			else
			{
				CLI_Printf("Output  error!!!\n");
			}
		}	
		
    }
    else
    {
	      CLI_Printf("Bad command input! type \"set\" for help!\n");
     }
   
    return 0;
}
static int  pfRouteSet(UINT32 u4Arc, CHAR* pcArg[])
{
    if(0 ==u4Arc)
    {
		CLI_Printf("set print [u+/d+/u-/d-/ir+/ir-]    - Set route data open(+) or close(-).\n");
		CLI_Printf("set print [local/logview]    - Set Output Type 0(default:logview),1(local).\n");
		CLI_Printf("set up [free/busy]            - Set Up Status (free or busy).\n");
    }
    else if (0 == strncmp(pcArg[0], "up", sizeof("up")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "free"))
			{
				ptIPCRoutePrintf->uUpBusy= 0;
				CLI_Printf("Route UP free\n");
			}
			else if(0 == strcmp(pcArg[1], "busy"))
			{
				ptIPCRoutePrintf->uUpBusy= 1;
				CLI_Printf("Route UP busy\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
    }
    else if (0 == strncmp(pcArg[0], "print", sizeof("print")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "u+"))
			{
				ptIPCRoutePrintf->uAIUp = 1;
				CLI_Printf("Route AI UP Data:OPEN\n");
			}
			else if(0 == strcmp(pcArg[1], "u-"))
			{
				ptIPCRoutePrintf->uAIUp = 0;
				CLI_Printf("Route AI UP Data:CLOSE\n");
			}
			else if(0 == strcmp(pcArg[1], "d+"))
			{
				ptIPCRoutePrintf->uDown = 1;
				CLI_Printf("Route DOWN DATA PRINT:OPEN\n");
			}
			else if(0 == strcmp(pcArg[1], "d-"))
			{
				ptIPCRoutePrintf->uDown = 0;
				CLI_Printf("Route DOWN DATA PRINT:CLOSE\n");
			}
			else if(0 == strcmp(pcArg[1], "local"))
			{
				ptIPCShm->printf_select = 1;
				CLI_Printf("Select local as Output\n");
			}
			else if(0 == strcmp(pcArg[1], "logview"))
			{
				ptIPCShm->printf_select = 0;
				CLI_Printf("Select NASLogView as Output\n");
			}
			else if(0 == strcmp(pcArg[1], "ir+"))
			{
				ptIPCRoutePrintf->uDownIr = 1;
				CLI_Printf("Route Driver Interupt:OPEN\n");
			}
			else if(0 == strcmp(pcArg[1], "ir-"))
			{
				ptIPCRoutePrintf->uDownIr = 0;
				CLI_Printf("Route Driver Interupt:CLOSE\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
		else
    		{
			CLI_Printf("Bad command input! type \"set\" for help!\n");
    		}
    }
    else
    {
		CLI_Printf("Bad command input! type \"set\" for help!\n");
    }
    return 0;
}

static int  pfDllGet(UINT32 u4Arc, CHAR* pcArg[])
{
    if(0 ==u4Arc)
    {
		CLI_Printf("get negrbsw    - NegrBurst Switch: 0,close 1,open.\n");
		CLI_Printf("get negrbcyc   - NegrBurst Cycle: 60-1200 seconds.\n");
    }
    if(0 == strncmp(pcArg[0], "negrbsw", sizeof("negrbsw")))
    {    
        if(ptDllPrint->NegrSw == 0)
        {
            CLI_Printf("no para\n");
        }       
        else if(ptDllPrint->NegrCyc== 1)
        {
            CLI_Printf("no para\n");
        }
        else
        {
            CLI_Printf("Output  error!!!\n");
        }
    }
   
    return 0;
}

static int  pfDllSet(UINT32 u4Arc, CHAR* pcArg[])
{
    if(0 ==u4Arc)
    {
		CLI_Printf("set  ai       [u+/d+/u-/d-/a+/a-]    - Set dll data open(+) or close(-).\n");
		CLI_Printf("set  ccl      [u+/d+/u-/d-/a+/a-]    - Set dll data open(+) or close(-).\n");
		CLI_Printf("set  nbsw     [+/-]                  - Set dll neighber brust switch open(+) close(-).\n");
		CLI_Printf("set  nbcyc    [1-20]                 - Set dll neighber brust cycle (minutes).\n");
		CLI_Printf("set  printlv  [0-3]                  - Set dll print level 0-close 1-debug 2-printm 3-all.\n");
		CLI_Printf("set  frqslt   [0-1] [0-1]            - Set dll shoot frquence slot [0.1]-freq [0.1]-slot.\n");
		CLI_Printf("set  wm       [0-1]                  - Set dll work mode 0-service 1-testing.\n");
    }
    else if (0 == strncmp(pcArg[0], "ai", sizeof("ai")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "u+"))
			{
				ptDllPrint->AIUp = 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf AI Up data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "u-"))
			{
				ptDllPrint->AIUp = 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf AI Up data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "d+"))
			{
				ptDllPrint->AIDown= 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf AI Down data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "d-"))
			{
				ptDllPrint->AIDown = 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf AI Down data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "a+"))
			{
				ptDllPrint->AIUp = 1;
				ptDllPrint->AIDown= 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf AI data print all: open\n");
			}
			else if(0 == strcmp(pcArg[1], "a-"))
			{
				ptDllPrint->AIUp = 0;
				ptDllPrint->AIDown= 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf AI data print all: close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
		else
		{
		    CLI_Printf("Bad command input! type \"set\" for help!\n");
		}
    }    
    else if (0 == strncmp(pcArg[0], "ccl", sizeof("ccl")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "u+"))
			{
				ptDllPrint->CCUp = 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf CCL Up data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "u-"))
			{
				ptDllPrint->CCUp = 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf CCL Up data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "d+"))
			{
				ptDllPrint->CCDown= 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf CCL Down data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "d-"))
			{
				ptDllPrint->CCDown = 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf CCL Down data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "a+"))
			{
				ptDllPrint->CCUp = 1;
				ptDllPrint->CCDown= 1;
                ptDllPrint->PrintLv = 1;
				CLI_Printf("Inf CCL data print all: open\n");
			}
			else if(0 == strcmp(pcArg[1], "a-"))
			{
				ptDllPrint->CCUp = 0;
				ptDllPrint->CCDown= 0;
                ptDllPrint->PrintLv = 0;
				CLI_Printf("Inf CCL data print all: close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
		else
		{
		    CLI_Printf("Bad command input! type \"set\" for help!\n");
		}
    }
    else if (0 == strncmp(pcArg[0], "nbsw", sizeof("nbsw")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "+"))
			{
				ptCFGShm->start_neighbor.val = 1;
				CLI_Printf("neighber brust switch: open\n");
			}
			else if(0 == strcmp(pcArg[1], "-"))
			{
				ptCFGShm->start_neighbor.val = 0;
				CLI_Printf("neighber brust switch: close\n");
			}
            else
            {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
            }
        }
        else
        {
            CLI_Printf("Bad command input! type \"set\" for help!\n");
        }
    }
    else if (0 == strncmp(pcArg[0], "nbcyc", sizeof("nbcyc")))
    {
		if(2 == u4Arc)
		{
		    UINT8 time = atoi(pcArg[1]);
			if(0 < time && 21 > time)
			{
				ptCFGShm->neighbor_period.val = time;
				CLI_Printf("neighber brust cycle (%d minutes)\n", time);
			}
            else
            {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
            }
        }
        else
        {
            CLI_Printf("Bad command input! type \"set\" for help!\n");
        }
    }
    else if (0 == strncmp(pcArg[0], "printlv", sizeof("printlv")))
    {
		if(2 == u4Arc)
		{
		    UINT8 lv = atoi(pcArg[1]);
			if(4 > lv)
			{
				ptDllPrint->PrintLv = lv;
				CLI_Printf("dll print level open (%d level)\n", lv);
			}
            else
            {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
            }
        }
        else
        {
            CLI_Printf("Bad command input! type \"set\" for help!\n");
        }
    }
    else if (0 == strncmp(pcArg[0], "frqslt", sizeof("frqslt")))
    {    
        if(3 == u4Arc)
        {            
		    UINT8 frq = atoi(pcArg[1]) & 0x0f;
		    UINT8 slt = atoi(pcArg[2]) & 0x0f;
            if(frq < 2 && slt < 2)
            {
				ptDllPrint->FrqSlt = (frq << 4) | slt;
				CLI_Printf("dll shoot freq:%d slot:%d \n", frq, slt);
            }
            else
            {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
            }
        }
    }    
    else if (0 == strncmp(pcArg[0], "wm", sizeof("wm")))
    {
        if(3 == u4Arc)
        {
		    UINT8 wm = atoi(pcArg[1]);
		    if(wm < 2)
		    {
		        ptDllPrint->WorkMode = wm;
		        CLI_Printf("set work mode:%d\n", wm);
		    }
		    else
		    {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
		    }
        }
    }
    else
    {
		CLI_Printf("Bad command input! type \"set\" for help!\n");
    }
    return 0;
}

/**
 * @var s_CliCmd
 * @brief CLI命令表
 */
static CLI_CMD_T s_CliRouteCmd[2] =
{
    {"route", "get",    "get  [Type \"get\"  for help!]       - Get current values", pfRouteGet},
    {"route", "set",    "set  [Type \"set\"  for help!]       - Set current values", pfRouteSet},
};

/**
 * @var s_CliCmd
 * @brief CLI命令表
 */
static CLI_CMD_T s_CliDllCmd[2] =
{
    {"dll", "get",    "get  [Type \"get\"  for help!]       - Get current values", pfDllGet},
    {"dll", "set",    "set  [Type \"set\"  for help!]       - Set current values", pfDllSet},
};
static int  pfCCLGet(UINT32 u4Arc, CHAR* pcArg[])
{
	if(0 ==u4Arc)
	{
		CLI_Printf("no get parameter    - 0,... 1,... .\n");
	}
	else if(1 ==u4Arc)
	{
		CLI_Printf("no get parameter    - 0,... 1,... .\n");
	}
	else
	{
		CLI_Printf("Bad command input! type \"set\" for help!\n");
	}

	return 0;

}
static int  pfCCLSet(UINT32 u4Arc, CHAR* pcArg[])
{
	
    if(0 ==u4Arc)
    {
		CLI_Printf("set cc  [u+/d+/u-/d- /a+/a-]    - Set ccl data open(+) or close(-).\n");
		CLI_Printf("set dll [u+/d+/u-/d- /a+/a-]    - Set ccl data open(+) or close(-).\n");
    }
	 else if (0 == strncmp(pcArg[0], "cc", sizeof("cc")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "u+"))
			{
				//ptCCLPrint->CclUp= 1;
				CLI_Printf("Inf ccl-cc Up data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "u-"))
			{
				ptCCLPrint->CclUp = 0;
				CLI_Printf("Inf ccl-cc data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "d+"))
			{
				ptCCLPrint->CcDown= 1;
				CLI_Printf("Inf CC Down data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "d-"))
			{
				ptCCLPrint->CcDown= 0;
				CLI_Printf("Inf CC Down data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "a+"))
			{
				ptCCLPrint->CcDown= 1;
				ptCCLPrint->CclUp = 1;
				CLI_Printf("Inf ccl-cc data print all: open\n");
			}
			else if(0 == strcmp(pcArg[1], "a-"))
			{
				ptCCLPrint->CcDown= 0;
				ptCCLPrint->CclUp = 0;
				CLI_Printf("Inf ccl-cc data print all: close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
		else
		{
		    CLI_Printf("Bad command input! type \"set\" for help!\n");
		}
    }    
    else if (0 == strncmp(pcArg[0], "dll", sizeof("dll")))
    {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "u+"))
			{
				ptCCLPrint->DllUp= 1;
				CLI_Printf("Inf dll Up data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "u-"))
			{
				ptCCLPrint->DllUp = 0;
				CLI_Printf("Inf dll Up data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "d+"))
			{
				ptCCLPrint->CclDown=1;
				CLI_Printf("Inf dll Down data print: open\n");
			}
			else if(0 == strcmp(pcArg[1], "d-"))
			{
				ptCCLPrint->CclDown= 0;
				CLI_Printf("Inf dll Down data print: close\n");
			}
			else if(0 == strcmp(pcArg[1], "a+"))
			{
				ptCCLPrint->DllUp= 1;
				ptCCLPrint->CclDown=1;
				CLI_Printf("Inf dll data print all: open\n");
			}
			else if(0 == strcmp(pcArg[1], "a-"))
			{
				ptCCLPrint->DllUp= 0;
				ptCCLPrint->CclDown=0;
				CLI_Printf("Inf dll data print all: close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			}
		}
		else
		{
		    CLI_Printf("Bad command input! type \"set\" for help!\n");
		}
    }
    else
    {
		CLI_Printf("Bad command input! type \"set\" for help!\n");
    }
    return 0;	
}
/**
 * @var s_CliCCLCmd
 * @brief CLI命令表
 */
static CLI_CMD_T s_CliCCLCmd[2] =
{
    {"ccl", "get",    "get  [Type \"get\"  for help!]       - Get current values", pfCCLGet},
    {"ccl", "set",    "set  [Type \"set\"  for help!]       - Set current values", pfCCLSet},
};

static int  pfMGRGet(UINT32 u4Arc, CHAR* pcArg[])
{
	int i = 0;
     if(0 ==u4Arc)
    {
		CLI_Printf("get alarmtable    - Get alarm info.\n");
		CLI_Printf("get epackver    - Get E-PACK Version.\n");
		CLI_Printf("get fpgaver    - Get FPGA Version.\n");
    }
     else if(1 ==u4Arc) 
	{
		if(0 == strncmp(pcArg[0], "alarmtable", sizeof("alarmtable")))
		{
			CLI_Printf("Alarm table:\n");
			for(i=0;i<(MGR_ALARM_CENTER_NUM+MGR_ALARM_FPGA_NUM+MGR_ALARM_SERVER_NUM);i++)
			{	
				CLI_Printf("code=%d,exist=%d,clear=%d,value=%d\n",ptIPCShm->alarm_struct[i].alm_code,ptIPCShm->alarm_struct[i].alm_exist,ptIPCShm->alarm_struct[i].alm_clear,ptIPCShm->alarm_struct[i].value);

			}
		}
		else if(0 == strncmp(pcArg[0], "epackver", sizeof("epackver")))
		{
			CLI_Printf("E-PACK Version: %s\n", ptCFGShm->s_version.buf);
		}
		else if(0 == strncmp(pcArg[0], "fpgaver", sizeof("fpgaver")))
		{
			CLI_Printf("FPGA Version: %s\n", ptCFGShm->fpga_version.buf);
		}
		else
		{
			CLI_Printf("Bad command input! type \"set\" for help!\n");
		}
		
	}
    return 0;
}
static int  pfMGRSet(UINT32 u4Arc, CHAR* pcArg[])
{
     if(0 ==u4Arc)
     {
		CLI_Printf("set alarm_debug +/-    - Get alarm info.\n");
		CLI_Printf("set mgr_debug +/-    - mgr print switch\n");
		CLI_Printf("set fpga_debug +/++/-    - fpga print switch\n");
		CLI_Printf("set fpga_sleep [time]    - fpga print time\n");
     }
	 
     else if (0 == strncmp(pcArg[0], "fpga_sleep", sizeof("fpga_sleep")))
	 {
		if(2 == u4Arc)
		{ 
			ptIPCShm->fpga_debug_sleep = atoi(pcArg[1]);
			CLI_Printf("set fpga_debug_sleep:%u\n", ptIPCShm->fpga_debug_sleep);
		}
     }
     else if (0 == strncmp(pcArg[0], "fpga_debug", sizeof("fpga_debug")))
	 {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "+"))
			{
				ptIPCShm->mgr_printf[2] = 1;
				CLI_Printf("fpga print all Open\n");
			}
			else if(0 == strcmp(pcArg[1], "++"))
			{
				ptIPCShm->mgr_printf[2] = 2;
				CLI_Printf("fpga print important Open\n");
			}
			else if(0 == strcmp(pcArg[1], "-"))
			{
				ptIPCShm->mgr_printf[2] = 0;
				CLI_Printf("fpga print Close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			} 
			
		}
     }
     else if (0 == strncmp(pcArg[0], "mgr_debug", sizeof("mgr_debug")))
	 {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "+"))
			{
				ptIPCShm->mgr_printf[1]= 1;
				CLI_Printf("MGR print Open\n");
			}
			else if(0 == strcmp(pcArg[1], "-"))
			{
				ptIPCShm->mgr_printf[1]= 0;
				CLI_Printf("MGR print Close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			} 
			
		}
     }
	 else if (0 == strncmp(pcArg[0], "alarm_debug", sizeof("alarm_debug")))
     {
		if(2 == u4Arc)
		{
			if(0 == strcmp(pcArg[1], "+"))
			{
				ptIPCShm->mgr_printf[0]= 1;
				CLI_Printf("MGR Alarm info  Open\n");
			}
			else if(0 == strcmp(pcArg[1], "-"))
			{
				ptIPCShm->mgr_printf[0]= 0;
				CLI_Printf("MGR Alarm info  Close\n");
			}
			else
			{
				CLI_Printf("Bad command input! type \"set\" for help!\n");
			} 
			
		}
     }
     else
     {
	    CLI_Printf("Bad command input! type \"set\" for help!\n");
      } 
	 	
    return 0;
}

/**
 * @var s_CliCCLCmd
 * @brief CLI命令表
 */
static CLI_CMD_T s_CliMGRCmd[2] =
{
    {"mgr", "get",    "get  [Type \"get\"  for help!]       - Get current values", pfMGRGet},
    {"mgr", "set",    "set  [Type \"set\"  for help!]       - Set current values", pfMGRSet},
};


static int pfErrGet(UINT32 u4Arc, CHAR* pcArg[])
{
    int i;
    unsigned short TableErrLen = s_len_ERR_MARK;
    unsigned short IndxALL = TableErrLen - 1;

    for (i = 0; i < TableErrLen; i++)
    {
        CLI_Printf("%s\t- %s", pERR_MARK[i].Name, pERR_MARK[i].str[*pERR_MARK[i].Flag]);
    }
    return 0;
}


static int pfErrSet(UINT32 u4Arc, CHAR* pcArg[])
{
    int i;
    unsigned short TableErrLen = s_len_ERR_MARK;
    unsigned short IndxALL = TableErrLen - 1;

    if (0 == u4Arc)  // 不带参数
    {
        CLI_Printf("set  %s     [a+/a-]    - voice err test,        enable(a+) or disable(a-).\n", pERR_MARK[ERR_INDX_VOICE].Name);
        CLI_Printf("set  %s  [a+/a-]    - data 1/2 err test,     enable(a+) or disable(a-).\n", pERR_MARK[ERR_INDX_21].Name   );
        CLI_Printf("set  %s  [a+/a-]    - data 3/4 err test,     enable(a+) or disable(a-).\n", pERR_MARK[ERR_INDX_43].Name   );
        CLI_Printf("set  %s   [a+/a-]    - voice & data all test, enable(a+) or disable(a-).\n", pERR_MARK[ERR_INDX_ALL].Name  );
    }
    else if (2 == u4Arc)  // 带两个参数
    {
        if (0 == strcmp(pcArg[0], pERR_MARK[IndxALL].Name)) // 如果等于ALL，所有调试使能
        {
            if (0 == strcmp(pcArg[1], "a+"))
            {
                for (i = 0; i < TableErrLen; i++)
                {
                    *pERR_MARK[i].Flag = ERR_ANABLE;
                }
                CLI_Printf(pERR_MARK[IndxALL].str[*pERR_MARK[IndxALL].Flag]);
            }
            else if (0 == strcmp(pcArg[1], "a-"))
            {
                for (i = 0; i < TableErrLen; i++)
                {
                    *pERR_MARK[i].Flag = ERR_DISABLE;
                }
                CLI_Printf(pERR_MARK[IndxALL].str[*pERR_MARK[IndxALL].Flag]);
            }
            else
            {
                CLI_Printf("Bad command input! type \"set\" for help!\n");
            }
        }
        else  // 传进来的参数不是 ALL, 则独立设置打印调试
        {
            for (i = 0; i < TableErrLen; i++)
            {
                if (0 == strcmp(pcArg[0], pERR_MARK[i].Name))
                {
                    if (0 == strcmp(pcArg[1], "a+"))
                    {
                        *pERR_MARK[i].Flag = ERR_ANABLE;
                        CLI_Printf(pERR_MARK[i].str[*pERR_MARK[i].Flag]);
                        break;
                    }
                    else if (0 == strcmp(pcArg[1], "a-"))
                    {
                        *pERR_MARK[i].Flag = ERR_DISABLE;
                        CLI_Printf(pERR_MARK[i].str[*pERR_MARK[i].Flag]);
                        break;
                    }
                    else
                    {
                        CLI_Printf("Bad command input! type \"set\" for help!\n");
                    }
                }
            }
        }
    }   
    else  // 操作错误
    {
        CLI_Printf("Bad command input! type \"set\" for help!\n");
    }

    return 0;
}

/**
 * @var s_CliCCLCmd
 * @brief CLI命令表
 */
static CLI_CMD_T s_CliErrCmd[2] =
{
    {"err", "get",    "get  [Type \"get\"  for help!]       - Get current values", pfErrGet},
    {"err", "set",    "set  [Type \"set\"  for help!]       - Set current values", pfErrSet},
};


/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
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
void * CLI_Get_Shm(int id, size_t size)
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
/**
 * @brief   在内存中分配一个空节点
 *
 * @return  CLI_CMD_NODE_T* 成功分配到的节点结构首地址或空
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static CLI_CMD_NODE_T* CLI_AllocBuf(void)
{
    INT32 i4Cnt;

    for (i4Cnt = 0; i4Cnt < CLI_NODE_NUM; i4Cnt++)
    {
        if (0x0 == s_tCliNodePool[i4Cnt].acClass[0])
        {
            return &s_tCliNodePool[i4Cnt];
        }
    }

    return NULL;
}


/**
 * @brief   更新用户当前节点所在位置到参数节点上
 *
 * @param [in]  ptNode     节点结构首地址
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static void CLI_UpdateCurCmd(CLI_CMD_NODE_T* ptNode)
{
    s_ptCurCliCmd = ptNode;
}


/**
 * @brief   查询用户当前节点所在位置
 *
 * @return  CLI_CMD_NODE_T* 节点结构首地址
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static CLI_CMD_NODE_T* CLI_GetCurCmd(void)
{
    return s_ptCurCliCmd;
}
/**
 * @brief   对节点树进行前向遍历，将参数节点加到相应路径下
 *
 * @param [in]  ptNode      节点树首节点
 * @param [in]  ptFocusNode 查询的目标节点
 * @param [in]  ptCmd       待添加节点
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static void CLI_PreOrder(CLI_CMD_NODE_T* ptNode, CLI_CMD_NODE_T* ptFocusNode, CLI_CMD_T* ptCmd)
{
    CLI_CMD_NODE_T* ptTempNode   = ptNode;

    if (ptTempNode)
    {
        if (0 == strncmp(ptTempNode->acName, ptCmd->pcClass, sizeof(ptTempNode->acName)))
        {
            // 找到父节点，添加到树父节点的ptChild的最后ptBrother
            ptFocusNode->ptFather = ptTempNode;

            if (ptTempNode->ptChild)
            {
                ptTempNode = ptTempNode->ptChild;

                while (ptTempNode->ptBrother)
                {
                    ptTempNode = ptTempNode->ptBrother;
                }
                ptTempNode->ptBrother = ptFocusNode;
            }
            else
            {
                ptTempNode->ptChild = ptFocusNode;
            }

            return;
        }

        CLI_PreOrder(ptNode->ptBrother, ptFocusNode, ptCmd);
        CLI_PreOrder(ptNode->ptChild, ptFocusNode, ptCmd);
    }

    return;
}

/**
 * @brief   执行分析到的命令字符串对应的回调函数
 *
 * @param [in]  u4Arc      命令参数个数
 * @param [in]  pcArg[]    命令参数内容
 *
 * @return  STATUS         见返回值定义
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static STATUS CLI_DoCmd(UINT32 u4Arc, CHAR* pcArg[])
{
    CHAR*           pcCmd  = pcArg[0];
    CLI_CMD_NODE_T* ptNode = NULL;
    STATUS          i4Ret  = 0;

    // ls command
    if (0 == strncmp(pcCmd, CLI_LS, sizeof(CLI_LS)))
    {
        ptNode = CLI_GetCurCmd()->ptChild;

        while (ptNode)
        {
            if (6 < strlen(ptNode->acName))
            {
                CLI_Printf("%s\t%s\n", ptNode->acName, ptNode->acDescription);
            }
            else
            {
                CLI_Printf("%s\t\t%s\n", ptNode->acName, ptNode->acDescription);
            }
            ptNode = ptNode->ptBrother;
        }
        CLI_Printf("%%>");
    }
    // cd command
    else if (0 == strncmp(pcCmd, CLI_CD, sizeof(CLI_CD)))
    {
        ptNode = CLI_GetCurCmd();

        pcCmd = pcArg[1];

        // cd .. command
        if (0 == strncmp(pcCmd, CLI_CD_UP, sizeof(CLI_CD_UP)))
        {
            if (NULL == ptNode->ptFather)
            {
                CLI_Printf("Root directory has been reached!\n%%>");
            }
            else
            {
                CLI_UpdateCurCmd(ptNode->ptFather);

                pcCmd = CLI_LS;
                CLI_DoCmd(1, &pcCmd);
            }
        }
        // cd . command
        else if (0 == strncmp(pcCmd, CLI_CD_HERE, sizeof(CLI_CD_HERE)))
        {
            pcCmd = CLI_LS;
            CLI_DoCmd(1, &pcCmd);
        }
        // cd \ command
        else if (0 == strncmp(pcCmd, CLI_CD_ROOT, sizeof(CLI_CD_ROOT)))
        {
            CLI_UpdateCurCmd(&s_tCliCmdList);

            pcCmd = CLI_LS;
            CLI_DoCmd(1, &pcCmd);
        }
        // cd help command
        else if (0 == strncmp(pcCmd, CLI_HELP, sizeof(CLI_HELP)))
        {
            CLI_Printf("cd ..\t\tgo to upper level directory\ncd .\t\tgo to current directory\ncd \\\t\tgo to root directory\n%%>");
        }
        else if (0 == strncmp(pcCmd, CLI_CD_UP_DOWN, sizeof(CLI_CD_UP_DOWN)))
        {
            if (ptNode->ptFather)
            {
                pcCmd  = pcArg[2];
                ptNode = ptNode->ptFather;
                ptNode = ptNode->ptChild;

                while (ptNode)
                {
                    if (0 == strncmp(pcCmd, ptNode->acName, sizeof(ptNode->acName)))
                    {
                        if (ptNode->ptChild)
                        {
                            CLI_UpdateCurCmd(ptNode);
                
                            pcCmd = CLI_LS;
                            CLI_DoCmd(1, &pcCmd);
                        }
                        else
                        {
                            CLI_Printf("Bottom directory has been reached!\n%%>");
                        }
                
                        return 0;
                    }
                
                    ptNode = ptNode->ptBrother;
                }
                
                CLI_Printf("Bad command input, type \"cd help\" for help!\n%%>");
            }
            else
            {
                CLI_Printf("Bad command input, type \"cd help\" for help!\n%%>");
            }
        }
        else
        {
            // cd dir command
            ptNode = ptNode->ptChild;

            while (ptNode)
            {
                if (0 == strncmp(pcCmd, ptNode->acName, sizeof(ptNode->acName)))
                {
                    if (ptNode->ptChild)
                    {
                        CLI_UpdateCurCmd(ptNode);

                        pcCmd = CLI_LS;
                        CLI_DoCmd(1, &pcCmd);
                    }
                    else
                    {
                        CLI_Printf("Bottom directory has been reached!\n%%>");
                    }

                    return 0;
                }

                ptNode = ptNode->ptBrother;
            }

            CLI_Printf("Bad command input, type \"cd help\" for help!\n%%>");
        }
    }
    // help command
    else if (0 == strncmp(pcCmd, CLI_HELP, sizeof(CLI_HELP)))
    {
        CLI_Printf("ls\t\tlist all commands can be used in current directory\ncd\t\tgo to specific directory\npwd\t\tlist current position\n%%>");
    }
    // pwd command
    else if (0 == strncmp(pcCmd, CLI_PWD, sizeof(CLI_PWD)))
    {
        char  acDir[256]   = {0};
        char* pcPtr        = &acDir[246];
        unsigned int u4Len = 0;

        ptNode = CLI_GetCurCmd();

        while (NULL != ptNode)
        {
            u4Len = strlen(ptNode->acName);
            pcPtr -= u4Len;
            strncpy(pcPtr, ptNode->acName, u4Len);
            pcPtr -= 1;
            *pcPtr = 0x2f; // '/'

            ptNode = ptNode->ptFather;
        }

        CLI_Printf("%s\n%%>", pcPtr);
    }
    else if (0x03 == pcCmd[0])
    {
        CLI_Printf("\nStop all data dumping!\n%%>");
    }
    else
    {
        // custom defined commands
        ptNode = CLI_GetCurCmd()->ptChild;

        while (ptNode)
        {
            if (0 == strncmp(pcCmd, ptNode->acName, sizeof(ptNode->acName)))
            {
                if (ptNode->pfcallback)
                {
                    i4Ret = ptNode->pfcallback(u4Arc - 1, &pcArg[1]);
                    if (OSR_OK == i4Ret)
                    {
                        CLI_Printf("%%>");
                        return OSR_OK;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            ptNode = ptNode->ptBrother;
        }

        // no command
        CLI_Printf("Bad command input! type \"help\" for help!\n%%>");
        return OSR_FAIL;
    }

    return OSR_OK;
}


/**
 * @brief   从输入字符串中解析出命令和参数
 *
 * @param [in]  pcBuff     输入字符串
 * @param [out] argv[]     参数字符串数组
 *
 * @return  UINT32         命令和参数总个数
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static UINT32 CLI_GetCmd(CHAR* pcBuff, CHAR* argv[])
{
    UINT32 u4Argc  = 0;
    CHAR*  pch     = pcBuff;
    CHAR*  pcTmp   = argv[0];

    while (('\n' != *pch) && ('\r' != *pch) && (0x0 != *pch) && (u4Argc < CLI_ARC_MAX))
    {
        if (' ' != *pch)
        {
            if ((UINT32)pcTmp > (UINT32)(argv[0]))
            {
                if ((' '  == *(pch - 1)) ||
                    (0x2f == *(pch - 1)))    // '/'
                {
                    u4Argc++;
                    pcTmp = argv[u4Argc];
                }
            }
            *pcTmp = *pch;
            pcTmp++;
        }

        pch++;
    }

    if ((UINT32)pcTmp > (UINT32)(argv[0]))
    {
        u4Argc++;
    }

    return u4Argc;
}


/**
 * @brief   初始化误码率结构标识
 *
 * @param [in]  
 * @param [in]  
 * @author  周大元
 * @since   trunk.00001
 * @bug
 */
static void ErrRate_Global_Init(void)
{
    static ERR_MARK_T tERR_MARK[] = {
        {"v"    ,  &ptERRPrint->Voice ,   {"voice err test [disable]\n"        ,  "voice err test [enable]\n"        } },
        {"d_21" ,  &ptERRPrint->D_1_2 ,   {"data 1/2 err test [disable]\n"     ,  "data 1/2 err test [enable]\n"     } },
        {"d_43" ,  &ptERRPrint->D_3_4 ,   {"data 3/4 err test [disable]\n"     ,  "data 3/4 err test [enable]\n"     } },
        {"all"  ,  &ptERRPrint->All   ,   {"voice & data all test [disable]\n" ,  "voice & data all test [enable]\n" } },
    };
    pERR_MARK = tERR_MARK;
    s_len_ERR_MARK = sizeof(tERR_MARK)/sizeof(tERR_MARK[0]);
}



/**
 * @brief   命令行接口输入处理任务主函数
 *
 * @param [in]  u4Arc      线程传递参数个数
 * @param [in]  pvArg      线程传递参数内容
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
static void *CliInputTask(void* pvArg)
{
	CHAR   auBuff[CLI_MAX_SIZE];
	UINT32 u4TmpArc;
	CHAR   acCmd[CLI_ARC_MAX][CLI_ARG_MAX];
	CHAR*  pcArg[CLI_ARC_MAX];
	const char auVersion[] = "NAS_CLI_V1.0";

	INT32  i4ChIdx = 0;
	CHAR   auTmpBuff[64] = {0};
	UINT32 u4RevLen = 0;
	UINT32 u4Cnt = 0;

	BOOL bRcvTimeOut = FALSE;

	int i4SelectRet;
	fd_set fdsRecv,fdsEx;

	int opval;
	struct sockaddr_in tLocalAddr;

	struct timeval tTimeItval;

	for (u4TmpArc = 0; u4TmpArc < CLI_ARC_MAX; u4TmpArc++)
	{
		pcArg[u4TmpArc] = &acCmd[u4TmpArc][0];
	}
	u4TmpArc = 0;

    ErrRate_Global_Init();  // 误码率测试全局变量初始化

open_fifo:

	s_hsocket = socket(PF_INET, SOCK_STREAM, 0);
	if( 0 > s_hsocket)
	{
		printf("Failed to create Cli TCP socket");
		exit(1);
	}

	tLocalAddr.sin_family   = AF_INET;
	tLocalAddr.sin_port     = htons(SOCK_PORT_CLI_INPUT);
	tLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	opval = 1;
	setsockopt(s_hsocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opval, sizeof(int));

	if(0 != bind(s_hsocket,(struct sockaddr *)&tLocalAddr, sizeof(tLocalAddr)))
	{
		printf("Bind local addr and port error");
		close(s_hsocket);
		exit(1);
	}

	if(0 != listen(s_hsocket,1))
	{
		printf("listen error\n");//输出到LOGTOOL上
		close(s_hsocket);
		exit(1);
	}

cli_accept_client:

	i4FifoInFd = accept(s_hsocket,NULL,NULL);
	if(i4FifoInFd < 0)
	{
		printf("accept connection error!");
		close(s_hsocket);
		goto open_fifo;
	}
	i4FifoOutFd = i4FifoInFd;
	s_bEnableCli = TRUE;
	
	gettimeofday(&tPreTime, NULL);
	CLI_Printf("Hytera VCHU Software CLI starts  (Version %s)\n%%>", &auVersion[1]);//握手成功就可输入
	while(1)
	{
		FD_ZERO(&fdsRecv);
		FD_SET(i4FifoInFd,&fdsRecv);
		FD_ZERO(&fdsEx);
		FD_SET(i4FifoInFd,&fdsEx);

		tTimeItval.tv_sec = 10;
		tTimeItval.tv_usec = 0;
		bRcvTimeOut = FALSE;

		i4SelectRet = select(i4FifoInFd+1,&fdsRecv,0,&fdsEx,&tTimeItval);
		if(0 > i4SelectRet)//errno
		{
			printf("[CLI INPUT]select err");
			close(i4FifoInFd);
			goto cli_accept_client;
		}
		else if(0 == i4SelectRet)//time out
		{
			bRcvTimeOut = TRUE;
		}
		else if(0 < i4SelectRet)//data for read
		{
			if(FD_ISSET(i4FifoInFd,&fdsRecv))
			{
				u4RevLen = read(i4FifoInFd, (unsigned char *)auTmpBuff, 64);
				if (0 == u4RevLen)
				{
					printf("client disconnected");
					s_bEnableCli = FALSE;
					close(i4FifoInFd);
					goto cli_accept_client;
				}
				else if (s_bEnableCli)
				{
					if ((0 == i4ChIdx) &&
					        (0 != u4TmpArc) &&
					        (30 == auTmpBuff[0])) // 光标上键 重复上次命令
					{
						CLI_Printf("\n");
						if (OSR_OK != CLI_DoCmd(u4TmpArc, pcArg))
						{
							memset(auBuff, 0x0, CLI_MAX_SIZE);
							memset(acCmd,  0x0, CLI_MAX_SIZE);
							i4ChIdx  = 0;
							u4TmpArc = 0;
						}
						continue;
					}
				}

				for (u4Cnt = 0; u4Cnt < u4RevLen; u4Cnt++)
				{
					if (0x08 != auTmpBuff[u4Cnt])
					{
						auBuff[i4ChIdx++] = auTmpBuff[u4Cnt];

						if('\n' == auBuff[i4ChIdx - 1])
						{
							break;
						}
						else if (CLI_ARG_MAX <= i4ChIdx)
						{
							i4ChIdx = 0;
							memset(auBuff, 0x0, CLI_MAX_SIZE);
							memset(acCmd,  0x0, CLI_MAX_SIZE);
							break;
						}
						else if(0x03 == auBuff[i4ChIdx - 1])
						{
							auBuff[i4ChIdx] = '\n';
							break;
						}
					}
					else
					{
						if (i4ChIdx > 0)
						{
							i4ChIdx--;
						}
						else if (0x08 != auTmpBuff[u4Cnt + 1])
						{
							CLI_Printf("\nPlease input a command! type \"help\" for help!\n%%>");
						}
						auBuff[i4ChIdx] = 0;
					}
				}

				if ((u4RevLen == u4Cnt) || (0 == i4ChIdx))
				{
					continue;
				}

				i4ChIdx = 0;
			}
			else if(FD_ISSET(i4FifoInFd,&fdsEx))
			{
				printf("select sock[%d] exception occurs",i4FifoInFd);
				s_bEnableCli = FALSE;
				close(i4FifoInFd);
				goto cli_accept_client;
			}
		}
		if (s_bEnableCli)
		{
			gettimeofday(&tCurTime, NULL);
			if ((tCurTime.tv_sec - tPreTime.tv_sec) >= s_u4CLI_TIMEOUT)
			{
				memset(acCmd,  0x0, CLI_MAX_SIZE);
				CLI_Printf("Hytera VCHU Software CLI stops, timeout.\n");
				s_bEnableCli = FALSE;
				CLI_UpdateCurCmd(&s_tCliCmdList);
				usleep(100000);//休眠100ms
				close(i4FifoInFd);
				goto cli_accept_client;
			}
			else if (0 == strncmp(auBuff, (const CHAR*)CLI_END_KEY, (sizeof(CLI_END_KEY) - 1)))
			{
				u4TmpArc = 0;
				memset(acCmd,  0x0, CLI_MAX_SIZE);
				s_u4CLI_TIMEOUT = CLI_PRINT_TIMEOUT;
				CLI_Printf("Hytera VCHU Software CLI stops\n");
				s_bEnableCli = FALSE;
				CLI_UpdateCurCmd(&s_tCliCmdList);
				usleep(100000);//休眠100ms
				close(i4FifoInFd);
				goto cli_accept_client;
			}
			else if(bRcvTimeOut != TRUE)
			{
				memset(acCmd,  0x0, CLI_MAX_SIZE);
				u4TmpArc = CLI_GetCmd(auBuff, pcArg);
				if (0 != u4TmpArc)
				{
					if (OSR_OK != CLI_DoCmd(u4TmpArc, pcArg))
					{
						memset(acCmd,  0x0, CLI_MAX_SIZE);
						u4TmpArc = 0;
					}
				}
				else
				{
					CLI_Printf("Please input a command! type \"help\" for help!\n%%>");
				}
			}
			else
			{
				continue;
			}
		}
		else if (0 == strncmp(auBuff, (const CHAR*)CLI_START_KEY, (sizeof(CLI_START_KEY) - 1)))
		{
			
			u4TmpArc = 0;
			memset(acCmd,  0x0, CLI_MAX_SIZE);
			gettimeofday(&tCurTime, NULL);
			tPreTime.tv_sec = tCurTime.tv_sec;
			s_bEnableCli = TRUE;
			CLI_Printf("Hytera VCHU Software CLI starts  (Version %s)\n%%>", &auVersion[1]);
		}

		memset(auBuff, 0x0, CLI_MAX_SIZE);

		gettimeofday(&tCurTime, NULL);
		tPreTime.tv_sec = tCurTime.tv_sec;

	}
	pthread_exit(NULL); //退出线程
}



/******************************************************************************
 *   可调用函数实现
 *   *************************************************************************/
/**
 * @brief   将16进制无符号数转换成字符串
 *
 * @param [out] puData     转换后的字符串
 * @param [in]  u4Data     转换前的16进制无符号数
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void CLI_HexToStr(UINT8* puData, UINT32 u4Data)
{
#ifdef _ENABLE_CLI

    INT32 i4Temp;

    if (NULL == puData)
    {
        return;
    }

    puData[8] = 0;

    for (i4Temp = 7; i4Temp >= 0; i4Temp--, u4Data >>= 4)
    {
        if ((u4Data & 0x000f) <= 9)
        {
            puData[i4Temp] = (u4Data & 0x000f) + '0';
        }
        else
        {
            puData[i4Temp] = (u4Data & 0x000f) + 'a' - 0x0a;
        }
    }
#endif
    return;
}


/**
 * @brief   将字符串转换成16进制无符号数
 *
 * @param [in]  puData     转换前的字符串
 *
 * @return  UINT32         转换后的16进制无符号数
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
UINT32 CLI_StrToHex(UINT8* puData)
{
    UINT32 u4Ret = 0;

#ifdef _ENABLE_CLI

    UINT8* puTemp;
    INT32  i4Temp;

    u4Ret  = 0;
    puTemp = puData;

    while (('\0' != *puTemp) && (7 >= (puTemp - puData)))
    {
        if ((*puTemp >= '0') && (*puTemp <= '9'))
        {
            i4Temp = *puTemp - '0';
        }
        else if ((*puTemp >= 'a') && (*puTemp <= 'f'))
        {
            i4Temp = *puTemp - 'a' + 0x0a;
        }
        else if ((*puTemp >= 'A') && (*puTemp <= 'F'))
        {
            i4Temp = *puTemp - 'A' + 0x0a;
        }
        else
        {
            return 0;
        }

        u4Ret <<= 4;
        u4Ret += i4Temp;

        puTemp++;
    }
#endif
    return u4Ret;
}


/**
 * @brief   将字符串转换成10进制无符号数
 *
 * @param [in]  puData     转换前的字符串
 *
 * @return  UINT32         转换后的10进制无符号数
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
UINT32 CLI_StrToDec(UINT8* puData)
{
    UINT32 u4Ret = 0;

#ifdef _ENABLE_CLI

    UINT8* puTemp;

    u4Ret  = 0;
    puTemp = puData;

    while (('\0' != *puTemp) && (9 >= (puTemp - puData)))
    {
        u4Ret *= 10;

        if (('0' > *puTemp) || ('9' < *puTemp))
        {
            return 0;
        }

        u4Ret += (*puTemp - '0');

        puTemp++;
    }
#endif

    return u4Ret;
}


/**
 * @brief   注册一条命令行接口命令
 *
 * @param [in]  ptCmd      待添加命令指针
 *
 * @return  STATUS         见返回值定义
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
STATUS CLI_RegisterCommand(CLI_CMD_T* ptCmd)
{
#ifdef _ENABLE_CLI

    CLI_CMD_NODE_T* ptFocusNode  = NULL;
    CLI_CMD_NODE_T* ptTempNode   = NULL;

    // 遍历, 找到Class
    ptTempNode = &s_tCliCmdList;

    ptFocusNode = CLI_AllocBuf();

    strncpy(ptFocusNode->acClass, ptCmd->pcClass, sizeof(ptFocusNode->acClass));
    strncpy(ptFocusNode->acName, ptCmd->pcName, sizeof(ptFocusNode->acName));
    strncpy(ptFocusNode->acDescription, ptCmd->pcDescription, sizeof(ptFocusNode->acDescription));
    ptFocusNode->pfcallback = ptCmd->pfcallback;
    ptFocusNode->ptChild    = NULL;
    ptFocusNode->ptBrother  = NULL;

    CLI_PreOrder(ptTempNode, ptFocusNode, ptCmd);
#endif

    return 0;
}

/**
 * @brief   初始化命令行接口模块，分配资源占用
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void CLI_Init(void)
{
	int i;
#ifdef _ENABLE_CLI

    CLI_CMD_T tCliCmd = {0};

    CHAR* pcName0 = "ccl";
    CHAR* pcDesc0 = "here is the top level of CCL";

    CHAR* pcName1 = "dll";
    CHAR* pcDesc1 = "here is the top level of DLL";

    CHAR* pcName2 = "route";
    CHAR* pcDesc2 = "here is the top level of Route";
	
    CHAR* pcName3 = "mgr";
    CHAR* pcDesc3 = "here is the top level of Mgr";


    CHAR* pcName4 = "err";
    CHAR* pcDesc4 = "here is the top level of err";
    

    memset(s_tCliCmdList.acClass, 0x0, sizeof(s_tCliCmdList.acClass));
    memcpy(s_tCliCmdList.acName, CLI_ROOT_NAME, sizeof(CLI_ROOT_NAME));
    memcpy(s_tCliCmdList.acDescription, CLI_ROOT_DESC, sizeof(CLI_ROOT_DESC));
    s_tCliCmdList.pfcallback = NULL;
    s_tCliCmdList.ptBrother = NULL;
    s_tCliCmdList.ptChild = NULL;
    s_tCliCmdList.ptFather = NULL;


    strcpy(tCliCmd.pcClass, CLI_ROOT_NAME);
    strcpy(tCliCmd.pcName, pcName0);
    strcpy(tCliCmd.pcDescription, pcDesc0);
    tCliCmd.pfcallback = NULL;
    CLI_RegisterCommand(&tCliCmd);


    strcpy(tCliCmd.pcClass, CLI_ROOT_NAME);
    strcpy(tCliCmd.pcName, pcName1);
    strcpy(tCliCmd.pcDescription, pcDesc1);	
    tCliCmd.pfcallback = NULL;
    CLI_RegisterCommand(&tCliCmd);

    strcpy(tCliCmd.pcClass,CLI_ROOT_NAME);
    strcpy(tCliCmd.pcName,pcName2);
    strcpy(tCliCmd.pcDescription,pcDesc2);
    tCliCmd.pfcallback = NULL;
    CLI_RegisterCommand(&tCliCmd);

    memcpy(tCliCmd.pcClass, CLI_ROOT_NAME,sizeof(CLI_ROOT_NAME));
    memcpy(tCliCmd.pcName, pcName3, sizeof(pcName3));
    memcpy(tCliCmd.pcDescription, pcDesc3, sizeof(pcDesc3));
    tCliCmd.pfcallback = NULL;
    CLI_RegisterCommand(&tCliCmd);

    strcpy(tCliCmd.pcClass, CLI_ROOT_NAME);
    strcpy(tCliCmd.pcName, pcName4);
    strcpy(tCliCmd.pcDescription, pcDesc4);
    tCliCmd.pfcallback = NULL;
    CLI_RegisterCommand(&tCliCmd);
		
    for(i = 0;i<2;i++)
    {
		CLI_RegisterCommand(&s_CliRouteCmd[i]);
		CLI_RegisterCommand(&s_CliDllCmd[i]);
		CLI_RegisterCommand(&s_CliCCLCmd[i]);
		CLI_RegisterCommand(&s_CliMGRCmd[i]);
        CLI_RegisterCommand(&s_CliErrCmd[i]);
    }
	
    CLI_UpdateCurCmd(&s_tCliCmdList);

    s_u4CLI_TIMEOUT = CLI_PRINT_TIMEOUT;
#endif
    return;
}
/**
 * @brief   各模块注册命令行
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void CLI_MSG_Init()
{
	key_t msqKey;
	if((msqKey = ftok("/", CLI_MSG_ID)) == -1)
	{
		printf("[CLI_MSG_Init]Creat msqKey Error\n");
		exit(1);
	}

	s_i4CLIMsgId = msgget(msqKey, IPC_EXCL);  /*检查消息队列是否存在*/
	if(s_i4CLIMsgId > 0)
	{
		printf("[CLI_MSG_Init]MSG Exist,Remove!!!\n");
		msgctl(s_i4CLIMsgId, IPC_RMID, NULL);//删除
	}
	
	s_i4CLIMsgId = msgget(msqKey, IPC_CREAT|0666);/*创建消息队列*/
	if(s_i4CLIMsgId < 0)
	{
		printf("[CLI_MSG_Init]failed to create msq\n");
	       exit(1);
	}

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

	return;

}
/**
 * @brief   各模块打印开关
 * @author  张婷婷
 * @since   trunk.00001
 * @bug
 */
void CLI_IPC_Init()
{
	ptIPCShm = (SHM_IPC_STRU *)CLI_Get_Shm(FTOK_ID_SHM_IPC,sizeof(SHM_IPC_STRU));
	if(NULL == ptIPCShm)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_IPC_Init]Route_Get_Shm  FTOK_ID_SHM_IPC error!!!\n");
		exit(1);
	}

	ptIPCShm->printf_select = 0;//默认是	logview	工具输出
	
	ptIPCRoutePrintf = (ROUTE_PRINTF *)ptIPCShm->route_printf;
	if(NULL == ptIPCRoutePrintf)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_IPC_Init]ptIPCRoutePrintf error!!!\n");
		exit(1);
	}

	ptDllPrint = (DLL_PRINT_T *)ptIPCShm->dll_printf;
	if(NULL == ptDllPrint)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_IPC_Init]ptDllPrintf error!!!\n");
		exit(1);
	}
	ptCCLPrint = (CCL_PRINT_T *)ptIPCShm->ccl_printf;
	if(NULL == ptCCLPrint)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_IPC_Init]ptCCLPrintf error!!!\n");
		exit(1);
	}

	ptERRPrint = (ERR_PRINT_T *)ptIPCShm->Err_printf;
	if(NULL == ptERRPrint)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_IPC_Init]ptERRPrint error!!!\n");
		exit(1);
	}


}
/**
 * @brief   配置区共享内存
 * @author  张婷婷
 * @since   trunk.00001
 * @bug
 */
void CLI_CFG_Init()
{
	ptCFGShm = (SHM_CFG_STRU *)CLI_Get_Shm(FTOK_ID_SHM_CFG,sizeof(SHM_CFG_STRU));
	if(NULL == ptCFGShm)
	{
		LOG_ERROR(s_i4LogMsgId,"[CLI_CFG_Init]Route_Get_Shm  FTOK_ID_SHM_CFG error!!!\n");
		exit(1);
	}
	return;
}
//////////////////////////////////////////////
int main()
{
	CLI_Init();

	CLI_IPC_Init();

	CLI_CFG_Init();

	CLI_MSG_Init();

	if (pthread_create(&s_tCliInputTask, NULL, CliInputTask, NULL) != 0)
	{
		printf("[CLI][s_tCliInputTask error \n");
		exit(1);
	}
	pthread_detach(s_tCliInputTask);

	while(1)
	{
		sleep(10);
	}
	return 1;

}
