#include <stdio.h>
#include <unistd.h>
#include "inc/ads1115.h"

#define 	FILE_PATH			"/dev/i2c-2"
#define 	ADS1115_BASE_ADDR	0x48

int main()
{
	int fd = ads1115_init(FILE_PATH, ADS1115_BASE_ADDR);
	ads1115_config(fd, AIN0_GND, SINGLE_SHOT);

	while (1)
	{
		float val = ads1115_get_voltage(fd);
		printf("%.2fV\n", val);
		sleep(3);
	}

	return 0;
}
