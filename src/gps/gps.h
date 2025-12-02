/**
 * @file    gps.h
 * @brief   ATGM336H driver header file
 */
#ifndef _GPS_H_
#define _GPS_H_
#include <stdint.h>

#define NMEA_FRAME      1024 

void readGpsData(uint8_t* buf, int len);

int GPS_init(char* uart_file_path);

#endif