/**
 * @file    gps.h
 * @brief   ATGM336H driver header file
 */
#ifndef _GPS_H_
#define _GPS_H_
#include <stdint.h>

/* Default latitude and longitude values.
 * Here set to the coordinates of Ton Duc Thang University (TDTU), Ho Chi Minh City. 
 */
#define     DEFAULT_LATITUDE        10.7318f 
#define     DEFAULT_LONGITUDE       106.6981f
#define     DEFAULT_ALTITUDE        10

// Debug flag - set to 1 to log all message IDs
#define     GPS_DEBUG_MSG_IDS       0

#define     HOVER_SPEED_THRESHOLD_CM_S      20.0
#define     HOVER_TIME_REQUIRED_SEC         4       

typedef struct {
    double lat;
    double lon;
    double alt;
} gps_ctx_t;

/**
 * @brief   Check if drone is hovering based on horizontal velocity
 * @return  none
 */
bool isDroneHovering(void);

/**
 * @brief   Read and parse MAVLink messages from GPS
 * @return  none
 */
void gpsReadMavlink(void);

/**
 * @brief   Initialize the UART interface for GPS communication
 * @param   uart_file_path is file path of UART
 * @return  0 if success; -1 otherwise
 */
int GPS_uart_init(char* uart_file_path);

#endif