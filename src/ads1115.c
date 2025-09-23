#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "../inc/ads1115.h"

float ads1115_get_voltage(int fd)
{
	ads1115_trigger_conversion(fd);
	short val = ads1115_read_register(fd, CONVER_REG_ADDR);
	float vol = (val * 4.096) / 32728;
	return vol;
}

void ads1115_trigger_conversion(int fd)
{
	unsigned short val = ads1115_read_register(fd, CONFIG_REG_ADDR);
	val |= 1 << 15;
	ads1115_write_register(fd, CONFIG_REG_ADDR, val);
}

void ads1115_config(int fd, int channel, int mode)
{
	/* select analog channel */
	int val = channel << 12;
	/* set FSR = 4.096V */
	val |= 0b001 << 9;
	/* config mode */ 
	if (mode == CONTINUOUS)
		val &= ~(1 << 8);
	else if (mode == SINGLE_SHOT)
		val |= mode << 8;

	ads1115_write_register(fd, CONFIG_REG_ADDR, val);
}

void ads1115_write_register(int fd, unsigned char reg_addr, unsigned short val)
{
	unsigned char tx_buf[3];
	tx_buf[0] = reg_addr;
	tx_buf[1] = val >> 8;
	tx_buf[2] = val & 0xff;

	int ret = write(fd, tx_buf, 3);
	if (ret < 0)
	{
		perror("write");
		return;
	}
}

short ads1115_read_register(int fd, unsigned char reg_addr)
{
	int ret = write(fd, &reg_addr, 1);
	if (ret < 0)
	{
		perror("write");
		return -1;
	}
		
	unsigned char rx_buf[2] = {0};
	ret = read(fd, rx_buf, 2);
	if (ret < 0)
	{
		perror("read");
		return -1;
	}

	short val = rx_buf[0] << 8 | rx_buf[1];
	return val;
}

int ads1115_init(char* fpath, unsigned int slave_addr)
{
	int fd = open(fpath, O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	int ret = ioctl(fd, I2C_SLAVE, slave_addr);
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	return fd;
}
