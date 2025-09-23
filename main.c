#include <stdio.h>
#include "inc/ads1115.h"

#define 	FILE_PATH			"/dev/i2c-2"
#define 	ADS1115_BASE_ADDR	0x48

int main()
{
	int fd = ads1115_init(FILE_PATH, ADS1115_BASE_ADDR);
	unsigned char reg = 0x01;
	while (1) 
	{
		if (reg > 0x04)
			reg = 0x01;
		int tmp = ads1115_get_reg_val(fd, &reg);
		printf("%d\n", tmp);
		reg++;
	}

	return 0;
}
