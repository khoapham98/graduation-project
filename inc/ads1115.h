#pragma once

#define 	SINGLE_SHOT				1
#define 	CONTINUOUS				0
#define 	CONVER_REG_ADDR			0x00
#define 	CONFIG_REG_ADDR			0x01
#define 	LO_THRESH_REG_ADDR		0x02
#define 	HI_THRESH_REG_ADDR		0x03
#define 	AIN0_AIN1				0b000
#define 	AIN0_AIN3				0b001
#define 	AIN1_AIN3				0b010
#define 	AIN2_AIN3				0b011
#define 	AIN0_GND				0b100 
#define 	AIN1_GND				0b101
#define 	AIN2_GND				0b110 
#define 	AIN3_GND				0b111
#define 	ADS1115_BASE_ADDR		0x48

float ads1115_get_voltage(int fd);
void ads1115_trigger_conversion(int fd);
void ads1115_config(int fd, int channel, int mode);
void ads1115_write_register(int fd, unsigned char reg_addr, unsigned short val);
short ads1115_read_register(int fd, unsigned char reg_addr);
int ads1115_init(char* fpath, unsigned int slave_addr);
