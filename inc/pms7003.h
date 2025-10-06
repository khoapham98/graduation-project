#pragma once
#include <stdint.h>
#include <stdio.h>

void pms7003_read(int fd, uint8_t* rx_buf, int len);
void pms7003_get_start_char(uint8_t* buf);
void pms7003_get_PM(uint8_t* buf);
void pms7003_data_to_json(char* dest, size_t size, uint8_t* src, float lat, float lon);
