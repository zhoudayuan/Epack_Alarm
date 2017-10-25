#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>

#include <pthread.h>


int s_hsocket;

pthread_t g_uCliInputTid;
pthread_t g_uCliOutputTid;

static void * CliInputTaskHandle(void* arg)
{
	char inputBuf[128];
	int len;
	while(1)
	{
		//gets(inputBuf);
		fgets(inputBuf,128,stdin);
		len = strlen(inputBuf); 
		inputBuf[len++] = '\n';
		write(s_hsocket,inputBuf,len);
	}
	pthread_exit(NULL); //退出线程
}
static void * CliOutputTaskHandle(void* arg)

{
	int i4SelectRet;
	fd_set fdsRecv,fdsEx;
	struct timeval tTimeItval;
	int u4RevLen = 0;
	char auTmpBuff[65535]={0};

	while(1)
	{
        FD_ZERO(&fdsRecv);
        FD_SET(s_hsocket,&fdsRecv);
        FD_ZERO(&fdsEx);
        FD_SET(s_hsocket,&fdsEx);

        tTimeItval.tv_sec = 10;
        tTimeItval.tv_usec = 0;


        i4SelectRet = select(s_hsocket+1,&fdsRecv,0,&fdsEx,&tTimeItval);
        if(0 > i4SelectRet)//errno
        {
            printf("\nselect err:%s\n\n",strerror(errno));
            close(s_hsocket);
            exit(0);
        }
        else if(0 == i4SelectRet)//time out
        {

        }
        else if(0 < i4SelectRet)//data for read
        {
            if(FD_ISSET(s_hsocket,&fdsRecv))
            {
                u4RevLen = read(s_hsocket, (unsigned char *)auTmpBuff, 65535-1);
                if (0 == u4RevLen)
                {
                	printf("\nINFO:Disconnected form Cli server.\n\n");
                    close(s_hsocket);
                    exit(0);
                }
                else if(u4RevLen < 0)
                {
                	printf("\nINFO: Read opt error.\n\n");
					close(s_hsocket);
					exit(0);
                }else
                {
                	write(1,auTmpBuff,u4RevLen);
                }
            }
            else if(FD_ISSET(s_hsocket,&fdsEx))
            {
            	printf("\nselect sock[%d] exception occurs\n\n",s_hsocket);
                close(s_hsocket);
                exit(0);
            }
        }
	}
	pthread_exit(NULL); //退出线程
}

int main(int argc, char* argv[])
{
	struct sockaddr_in vchu_addr;
	if(argc>1)
	{
		printf("\n[NAS_CLI_Tool]Input Error!!!\n");
	}

	inet_aton("127.0.0.1",&vchu_addr.sin_addr);
	vchu_addr.sin_port = htons(10050);

	s_hsocket = socket(AF_INET, SOCK_STREAM, 0);
	if( 0 > s_hsocket)
	{
		printf("\nFailed to create new tcp connection\n\n");
		return 0;
	}
	vchu_addr.sin_family   = AF_INET;

	printf("Connecting to NAS Cli server@%s:%d ...\n",inet_ntoa(vchu_addr.sin_addr), ntohs(vchu_addr.sin_port));
	if(0 != connect(s_hsocket, (struct sockaddr*)&vchu_addr, sizeof(vchu_addr)))
	{
		close(s_hsocket);
		printf("\nFailed to connect\n\n");
		return 0;
	}

	printf("OK!\n");

    if (pthread_create(&g_uCliOutputTid,NULL,CliOutputTaskHandle,NULL)!=0)
    {
    	printf("\nCreate thread error!\n\n");
        return 0;
    }
    pthread_detach (g_uCliOutputTid);

    CliInputTaskHandle(0);

	return 0;
}

