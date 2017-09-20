#ifndef __MGR_I2C_EEPROM_H__
#define __MGR_I2C_EEPROM_H__


int  iic_init();
int  eeprom_write(int writefd,unsigned short eepadd,unsigned char *buffer,unsigned int length);
int  eeprom_read(int readfd,unsigned short eepadd, unsigned char *data_read,unsigned int length);


#endif
