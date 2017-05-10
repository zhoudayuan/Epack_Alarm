#ifndef __LOG_H__
#define __LOG_H__
#include <sys/time.h>
#include <string.h>


#define LOG_FILE_1G_SIZE 1000*1024*1024 - 10*1024*1024
//#define LOG_FILE_1G_SIZE 100 -20
#define MAX_LOG_LEN     1000


/**
 * @struct LOG_MSG_T
 * @brief ��Ϣ���д�����Ϣ�ṹ
 */
typedef struct _LOG_MSG_T
{
    int msgType;
	char tmsgArray[MAX_LOG_LEN];
	
}__attribute__((packed,aligned(1)))LOG_MSG_T;

/**
 * @struct LOG_PRINT_T
 * @brief ��־��ӡ�ṹ
 */
typedef struct _LOG_PRINT_HEAD_T
{
    unsigned short head;
    unsigned short type;
    unsigned short datalen;
    unsigned short src_id;
    unsigned short dst_id;
    unsigned char src_type;
    unsigned char sig_ver;
    unsigned char path_num;
    unsigned char ex_head[11];
    unsigned int check_val;
}__attribute__((packed,aligned(1)))LOG_PRINT_HEAD_T;

/**
 * @struct LOG_PRINT_T
 * @brief ��־��ӡ�ṹ
 */
typedef struct _LOG_PRINT_T
{
    LOG_PRINT_HEAD_T log_head;
    unsigned char log_level;
    unsigned char log_data[MAX_LOG_LEN];
    
}__attribute__((packed,aligned(1)))LOG_PRINT_T;

/**
 * @struct WLU_LOG_MSG_T
 * @brief ������·��־��Ϣ�ṹ
 */
typedef struct _WLU_LOG_MSG_T
{
	unsigned short Head;
    unsigned char Type;
    unsigned short DataLen;
    LOG_PRINT_T Data;
    unsigned char Revs[2];
    unsigned short Crc;
    unsigned short MsgEnd;    

}__attribute__((packed,aligned(1)))WLU_LOG_MSG_T;

#define LOG_Debug(x,...) do{\
    LOG_MSG_T *tLogMsg;\
    int *len;\
    time_t *timep;\
    struct timeval *tSndTime;\
    tLogMsg = (LOG_MSG_T *)malloc(sizeof(LOG_MSG_T));\
    timep = (time_t *)malloc(sizeof(time_t));\
    len = (int *)malloc(sizeof(int));\
    tSndTime = (struct timeval *)malloc(sizeof(struct timeval));\
    *len = 0;\
    gettimeofday(tSndTime,NULL);\
    time(timep);\
    memset(tLogMsg,0x00,sizeof(LOG_MSG_T));\
    (*len) += sprintf(tLogMsg->tmsgArray + (*len),"%.2d:%.2d:%.2d:%.3d>",(int)localtime(timep)->tm_hour, (int)localtime(timep)->tm_min, (int)localtime(timep)->tm_sec,(int)tSndTime->tv_usec/1000);\
    (*len) += sprintf(tLogMsg->tmsgArray + (*len),__VA_ARGS__);\
    (*len) += sprintf(tLogMsg->tmsgArray + (*len),"\n");\
    tLogMsg->msgType = 1;\
    if(0 == ptIPCShm->printf_select)\
        msgsnd(x, tLogMsg, (*len)+4, 0);\
    else\
        printf("%s",tLogMsg->tmsgArray);\
    free(tLogMsg);\
    free(len);\
    free(timep);\
    free(tSndTime);\
}while(0)

#define LOG_DEBUG(x,...) LOG_Debug(x,"[""DEBUG""]""["TASK_NAME"]"__VA_ARGS__)

#define LOG_ERROR(x,...) LOG_Debug(x,"[""ERROR""]""["TASK_NAME"]"__VA_ARGS__)

#define LOG_WARNING(x,...) LOG_Debug(x,"[""WARNING""]""["TASK_NAME"]"__VA_ARGS__)

#define LOG_Wfile(x,...) do{\
	 time_t timep;\
        struct tm *p;\
        time(&timep);\
        p = localtime(&timep);\
        if(ftell(x)>LOG_FILE_1G_SIZE)\
        	fseek(x,0,SEEK_SET);\
	fprintf(x,"<%4d-%.2d-%.2d %.2d:%.2d:%.2d>",(int)(1900+p->tm_year), (int)(1+p->tm_mon), (int)p->tm_mday,(int)p->tm_hour, (int)p->tm_min, (int)p->tm_sec);\
	fprintf(x,__VA_ARGS__);\
	fprintf(x,"\n");\
	fflush(x);\
}while(0)

#define LOG_WFile(x,...) LOG_Wfile(x,"["TASK_NAME"]"__VA_ARGS__)


#define LOG_printmlocal(p,datalen) do{\
	int *i,*j;\
	 i = (int*)malloc(sizeof(int));\
	  j= (int*)malloc(sizeof(int));\
	  *i=*j=0;\
	printf("MEM PRINT:len=%d  ",datalen);\
	for((*i)= 0;(*i)<datalen;(*i)++){\
		(*j)++;\
		printf("%.2x ",p[*i]);\
		if((*j)%16 == 0)\
			printf("\n                   ");}\
		printf("\n");\
	 free(i);\
	  free(j);\
}while(0)	

#define LOG_printmlogview(x,p,datalen) do{\
	LOG_MSG_T *tLogMsg;\
	int *len;\
	int *i;\
    if(MAX_LOG_LEN > datalen)\
	{\
        time_t *timep;\
        struct timeval *tSndTime;\
        tSndTime = (struct timeval *)malloc(sizeof(struct timeval));\
        timep = (time_t *)malloc(sizeof(time_t));\
        len = (int*)malloc(sizeof(int));\
        i = (int*)malloc(sizeof(int));\
        *len = 0;\
        gettimeofday(tSndTime,NULL);\
        time(timep);\
        tLogMsg = (LOG_MSG_T *)malloc(sizeof(LOG_MSG_T));\
        memset(tLogMsg,0x00,sizeof(LOG_MSG_T));\
        (*len) += sprintf(tLogMsg->tmsgArray + (*len),"%.2d:%.2d:%.2d:%.3d>[MEM]",(int)localtime(timep)->tm_hour, (int)localtime(timep)->tm_min, (int)localtime(timep)->tm_sec,(int)tSndTime->tv_usec/1000);\
	    (*len )+= sprintf(tLogMsg->tmsgArray+(*len),"len=%d:\n",datalen);\
	    for((*i)=0;(*i)<datalen;(*i)++)\
	 	{\
	 	(*len) +=sprintf(tLogMsg->tmsgArray+(*len),"%.2x",*(p+(*i)));\
	 	if(((*i)+1)%8==0)\
			(*len) += sprintf(tLogMsg->tmsgArray+(*len),"   ");\
		if(((*i)+1)%64==0)\
			(*len) +=sprintf(tLogMsg->tmsgArray+(*len),"\n");}\
	    (*len) += sprintf(tLogMsg->tmsgArray + (*len),"\n");\
        tLogMsg->msgType = 2;\
        msgsnd(x, tLogMsg, (*len)+4, 0);\
        free(tLogMsg);\
        free(tSndTime);\
        free(len);\
        free(i);\
        free(timep);\
    }\
    else\
    {\
        printf("LOG print memory over range!!!\n");\
    }\
}while(0)	

#define LOG_printm(x,p,datalen) do{\
        if(0 == ptIPCShm->printf_select)\
	 LOG_printmlogview(x, p,datalen);\
	 else\
	 LOG_printmlocal(p,datalen);\
}while(0)

#define LOG_PrintM(x,p,len) LOG_printm(x,p,len)

#endif

