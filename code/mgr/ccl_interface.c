/**
 * Copyright (C), 1993-2010, Hytera Comm. Co., Ltd.
 * @file   ccl_interface.c
 * @brief
 * @author
 * @version v1.0
 * @date    2017-07-27
 */
/*
 *
 *   �޸���ʷ
 *   2017-07-27          ţ��ϲ                �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
 *   *************************************************************************/
#include "ccl_interface.h"
#include <sys/socket.h>
#include <arpa/inet.h>	  //inet_addr(); inet_ntoa()
#include <stdio.h> 
#include <string.h>
/******************************************************************************
 *   �궨��
 *   *************************************************************************/

#define     SOCK_PORT_CCL_CC_S   10012

/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/



/**
* @var s_tCclToCcSigfd
* @brief ���п��Ʋ����������׽���
*/
 static int  s_tCclToCcSigfd ;
/**
* @struct  s_tCclToCcSigAddr
* @brief ���п��Ʋ��������е�ַ
*/
static  struct sockaddr_in  s_tCclToCcSigAddr;




/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/
/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/


/******************************************************************************
 *   �ɵ��ú���ʵ��
 *   *************************************************************************/


/**
* @brief   �豸�µ�֪ͨ������Ϣ��
* @param [out] pvCenterData  ������������
* @param [out] Len     ������
* @param [in] SigType  ��������
* @author ţ��ϲ
* @since
* @bug
*/

void IDP_sysopt_Info(unsigned char *pvCenterData, int *Len,unsigned char  SigType)
{
    memset(pvCenterData,0,sizeof(pvCenterData));
    SMS_CENTER_CCL_UL* ptCenterData = (SMS_CENTER_CCL_UL *)pvCenterData;
    ptCenterData->SharedHead.SigHead = 0xec13;
    ptCenterData->SharedHead.Datalength = sizeof(SMS_CENTER_CCL_UL) - CC_SIG_HRD_LEN;
    ptCenterData->SharedHead.SigType = SIG_SMS_SEND;
    ptCenterData->SmsType =SigType;
    ptCenterData->SenderNum[0] = 0xff;
    ptCenterData->SenderNum[1] = 0;
    ptCenterData->SenderNum[2] = 0;
    ptCenterData->ReceiverNum[0] = 0xff;
    ptCenterData->ReceiverNum[1] = 0;
    ptCenterData->ReceiverNum[2] = 0;
    ptCenterData->ValidLength = 0;
    *Len = sizeof(SMS_CENTER_CCL_UL);
}

void  interfaceInit()
{
    s_tCclToCcSigfd = socket(AF_INET, SOCK_DGRAM, 0);     

		    //���п�����������
    if(0 > s_tCclToCcSigfd )
    {
        printf("[interfaceInit]creat  CclToCcSigfd error\n");
        //return -1;
    }

    bzero(&s_tCclToCcSigAddr, sizeof(s_tCclToCcSigAddr));
    s_tCclToCcSigAddr.sin_family = AF_INET;
    s_tCclToCcSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tCclToCcSigAddr.sin_port = htons(SOCK_PORT_CCL_CC_S);

}


/**
* @brief CCL ��CC ����ϵͳ֪ͨ
* @author ţ��ϲ
* @since
* @bug
*/
 void SentSysOptSig(unsigned char Type)
{
    int i4Len;
    unsigned char  centerdata[800];
    int u4Datalen;
    
    interfaceInit();
    
    IDP_sysopt_Info(centerdata,&u4Datalen,Type);
    
    i4Len=sendto(s_tCclToCcSigfd, centerdata, u4Datalen, 0,(struct sockaddr *)&s_tCclToCcSigAddr,sizeof(s_tCclToCcSigAddr));
    if(u4Datalen!=i4Len)
    {
        printf("[SentSysOptSig] len err :%d\n",i4Len);
        perror("send");
    }
    /*else
    {
        for(i4Len=0;i4Len<u4Datalen;i4Len++)
        {
            printf("%02x",centerdata[i4Len]);
            if(i4Len+1/30==0)
            {
                printf("\n");

            }
        }
    }*/
    printf("[SentSysOptSig]sendto cc\n");
}







