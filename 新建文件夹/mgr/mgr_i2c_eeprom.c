#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <memory.h>
#include <getopt.h>
#include <sys/types.h>  
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <sys/mman.h>  
#include <fcntl.h>  
#include <memory.h>  
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "gpio_interface.h"


#define I2C_WP_ON  1
#define I2C_WP_OFF 0
#define MAX_READ_COUNT 3000
#define PIOB_Base 0xFF70A000   
#define PIOB_Base 0xFF70A000 

#define GPIO_EEPROM_WP  172   //æ…∞Â «132

typedef unsigned int u32;

char *buf;
unsigned char buf1[MAX_READ_COUNT];
unsigned char buf_read[2];
unsigned short eeprom_write_offAddr = 0x100;
unsigned short eeprom_read_offAddr = 0x100;
unsigned short mac_write_offAddr = 0x500;
unsigned short mac_read_offAddr = 0x500;
unsigned short  add=0x50;  // eeprom ADDR 



int iic_init()
{
	int file;

	if((file = open("/dev/i2c-0",O_RDWR))<0)
	{  
		printf("---eeprom driver open error!!!---\n");

		return  -1; 
	}


	return file;

}
	   
	    
int  eeprom_write(int writefd,unsigned short eepadd,unsigned char *buffer,unsigned int length)

{   
	struct i2c_msg msg_write[1];
	struct i2c_rdwr_ioctl_data rdwr_arg;
	int page,writetimes,k,fk; 

	page=length/256;

	if(length%256 != 0)
	{
	
		page++;

	}    


	fk=gpio_init(GPIO_EEPROM_WP,1);
	gpio_write(fk, 0);
	//printf("eeprom write !\n\n");

	for(writetimes=0;writetimes<page;writetimes++)    

	{  
		msg_write[0].addr = add;
		msg_write[0].flags = 0;

		msg_write[0].len =256+2;


		buf1[0]=(unsigned char)(((eepadd & 0xFF00)>>8)+writetimes*1);
		buf1[1]=(unsigned char)(eepadd & 0x00FF);//addr_l

		memcpy(&buf1[2],&buffer[writetimes*256],256);

		msg_write[0].buf = buf1;

		rdwr_arg.msgs = msg_write;
		rdwr_arg.nmsgs = 1;
		if((k=ioctl(writefd,I2C_RDWR,(u32)&rdwr_arg))<0)
		{
			printf("no=%d",k);
			printf("fail to write to e2prom device at address 0x%x\\n",add);

			return -1;
		}                               
		usleep(20000);
	}
	gpio_write(fk,1);
   
    return 0;
}





int  eeprom_read(int readfd,unsigned short eepadd, unsigned char *data_read,unsigned int length)
 
{ 
	struct i2c_msg msg_read[2];
	struct i2c_rdwr_ioctl_data rdwr_arg; 
	
	//printf("\neeprom read!\n\n"); 
	msg_read[0].addr = 0x50;
	msg_read[0].flags = 0;
	
	buf_read[0] = (unsigned char)((eepadd & 0xFF00)>>8);//addr_h
	buf_read[1] = (unsigned char)(eepadd & 0x00FF);//addr_l
	msg_read[0].buf = buf_read;
	msg_read[0].len = 2;
	
	msg_read[1].addr = 0x50;
	msg_read[1].flags = I2C_M_RD;
	msg_read[1].buf = data_read;
	msg_read[1].len = length;
	
	rdwr_arg.msgs = msg_read;
	rdwr_arg.nmsgs = 2;
	if(ioctl(readfd,I2C_RDWR,(u32)&rdwr_arg) < 0)
	{	
		perror("reason\n");
		printf("fail to read to e2prom device at address 0x%x\\n",add);
		return -1;
	}
	
	return 0;
}





/*



main()

{
	int i,j,fd;
	unsigned char a[1000];
	int len=700;
	unsigned char b[1000];
	int len1=500;
	unsigned short  c=0x100;
	unsigned short  d=0x100;

	for(i=0;i<256;i++)
	{ 

		a[i]=i;

	} 
	for(i=256;i<500;i++)
	{

	a[i]=i-250;

	}
	for(i=500;i<700;i++)
	{

	a[i]=i-450;

	}


	for(j=0;j<256;j++)
	{ b[j]=j;} 
	for(j=256;j<500;j++)
	{b[j]=j-250;}


	fd=iic_init();
	eeprom_write(fd,c,a,len);


	eeprom_read(fd,d,b,len);

}	
*/
