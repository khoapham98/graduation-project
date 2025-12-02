/**
 * @file    gps.c
 * @brief   ATGM336H driver source file
 */

#include <stdio.h>
#include <sys/log.h>
#include <src/gps/gps.h>
#include <src/drivers/uart.h>

static int uart_fd = 0;

void readGpsData(uint8_t* buf, int len)
{
	if (len < NMEA_FRAME) 
		LOG_WRN("gps: May not receive data fully");

	readUART(uart_fd, buf, len);	
}

int GPS_init(char* uart_file_path)
{
	uart_fd = uart_init(uart_file_path);
    if (uart_fd < 0) {
        return -1;
	}
    
	LOG_INF("GPS Initialization successful");
	return 0;
}