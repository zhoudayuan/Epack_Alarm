#ifndef MPU_GPIO_H
#define MPU_GPIO_H

#define IN      0
#define OUT     1

int
gpio_init(int portnum, int direction);

int
gpio_write(int fd, int value);

int
gpio_read(int fd);

#endif