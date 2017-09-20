#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "gpio_interface.h"


#define oops(x, n)  {perror("x"); exit(n);}

int 
replace_str(char *source, char *src, char *dest)
{
     int stringlen;
     char temp[64];
     char * findpos=NULL;
     if(!src)
     {
        return -1;
     }
     findpos = strstr(source, src);

     if(!findpos)
         return -1; 

     while( findpos )
     {
        memset(temp, 0, sizeof(temp));
        stringlen = findpos - source;
        strncpy(temp, source, stringlen);
        strcat(temp, dest);
        strcat(temp, findpos + strlen(src));
        strcpy(source, temp);    
        findpos = strstr(source,src );
     }

     return 0;
}


int
gpio_num_request(int portnum,char *direc)
{
	int fd_export;	
	char str[8];
	
	fd_export = open("/sys/class/gpio/export", O_WRONLY);
	if (fd_export < 0)
		oops("open /sys/class/gpio/export error", -1);

	sprintf(str, "%d", portnum); //25
	write(fd_export, str, strlen(str));//Éú³Égpio25
	close(fd_export);

	return open(direc,O_RDWR);
}

void gpio_set_direction(int fd_dir,int direction)
{
	if(fd_dir < 0)
		oops("dir error in gpio_set_direction!",-1);
	
	switch(direction)
	{
		case IN:
		if (write(fd_dir, "in", 2) == -1)
			perror("write gpio error");
		break;

		case OUT:
		if (write(fd_dir, "out", 3) == -1)
			perror("write gpio error");
		break;

		default:
			break;
	}
       close(fd_dir);
	return;
} 


 

int gpio_get_value_fd(int portnum,char *value)
{
	char str[8];
	int fd_value;
	
	sprintf(str, "%d", portnum);
	if (replace_str(value, (char *)"x", str) == -1)
		oops("gpio value error in gpio_get_value_fd!",-1);
	
	fd_value = open(value, O_RDWR);
	if (fd_value < 0)
		oops("gpio open value error in gpio_get_value_fd!",-1);

	return fd_value;
}


int 
gpio_exist(int portnum,char *direc,int *fd_dir)	
{
	char str[8];
	
	sprintf(str, "%d", portnum); //25
	
	if (replace_str(direc, (char *)"x", str) == -1)//gpio25
		oops("replace_str direc err!",-1);
	
	*fd_dir = open(direc, O_RDWR);

	if(*fd_dir > 0)
		return 1;
	
	return 0;
}



int
gpio_init(int portnum, int direction)
{
	int fd_dir,fd_value;
	char direc[50] = "/sys/class/gpio/gpiox/direction";
    char value[50] = "/sys/class/gpio/gpiox/value";

	if(!gpio_exist(portnum,direc,&fd_dir))
	{
		//printf("portnum=%d, start gpio_num_request\n", portnum);
		fd_dir = gpio_num_request(portnum,direc);
	}
	  		
	gpio_set_direction(fd_dir,direction);	
    close(fd_dir);
	fd_value =gpio_get_value_fd(portnum,value);
	return fd_value;  
}
 

void gpio_exit(int portnum)
{
    int fd;
    char str[8];

    fd = open("/sys/class/gpio/unexport", O_RDWR);
    if (fd < 0)
        oops("open /sys/class/gpio/unexport error", -1);
   
    sprintf(str, "%d", portnum);
    write(fd, str, strlen(str));
    close(fd);
}


int
gpio_write(int fd, int value)
{
    char str[8];

    sprintf(str, "%d", value);

	lseek(fd, 0, SEEK_SET);
    if(write(fd, str, strlen(str)) == -1)
    {
        perror("write gpio error");
        return -1;
    }

    return 0;
}


 

int
gpio_read(int fd)
{
    char val[1];
    int rtn;

	lseek(fd, 0, SEEK_SET);
    if (read(fd, val, 1) == -1)
    {
        perror("read gpio error");
        return -1;
    }
	rtn = atoi(val);
	
    return rtn;
}



#if 0
int 
main(int argc, char const *argv[])
{
    char direc[] = "/sys/class/gpio/gpiox/direction";
    char str[8] = "10";

    if(-1 == replace_str(direc, "x", str))
    printf("replace_str error\n");

    printf("%s\n", direc);

    return 0;
}
#endif

/*
main()


{  int file,a,b,i;




//file=gpio_init(181,0);



//a=gpio_write(file, 1);
//printf("l=%d",a);

//for(i=0;i<20;i++)

// a=gpio_read(file);
// printf("read_value=%d\n\n",a);

for(i=0;i<50;i++)
{

 file=gpio_init(181,0);
 sleep(5);
 b=gpio_read(file);
 printf("read_value=%d\n\n",b);

}


//gpio_write(file,0);
//printf("t=%d\n",a);
//printf("second_time=%d",a);



}
*/
