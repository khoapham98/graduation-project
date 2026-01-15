/**
 * @file    uart.h
 * @brief   uart driver header file
 */
#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>
#include <stdbool.h>
#include <termios.h>
#include "device_setup.h"

/* file path of UART in BBB/RPi */
#if BBB
#define 	UART1_FILE_PATH			"/dev/ttyS1"
#define 	UART2_FILE_PATH			"/dev/ttyS2"
#define 	UART3_FILE_PATH			"/dev/ttyS3"
#define 	UART4_FILE_PATH			"/dev/ttyS4"
#define 	UART5_FILE_PATH			"/dev/ttyS5"

#elif RPI
#define 	UART0_FILE_PATH			"/dev/ttyAMA0"
#define     USB0_FILE_PATH          "/dev/ttyUSB0"
#define     USB1_FILE_PATH          "/dev/ttyUSB1"

#endif

/**
 * @brief   Read data from UART file
 * @param   fd is uart file descriptor
 * @param   buf is buffer address to store data
 * @param   len is length of buffer
 * @return  none
 */
void readUART(int fd, uint8_t* buf, int len);

/**
 * @brief   Initialize UART peripheral 
 * @param   UART_PATH is file path of UART
 * @param   BR is baudrate
 * @param   nonBlock enable non-blocking mode if true
 * @return  uart fd if success; -1 otherwise
 */
int uart_init(char* UART_PATH, speed_t BR, bool nonBlock);

#endif