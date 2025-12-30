/**
 * @file    dust_sensor.h
 * @brief   PMS7003 driver header file
 */
#ifndef _PMS7003_H_
#define _PMS7003_H_
#include <stdint.h>

#define DUST_DATA_FRAME     32
#define AQI_LEVEL_COUNT     6

enum aqiLevel{
    AQI_GOOD,
    AQI_MODERATE,
    AQI_SENSITIVE,
    AQI_UNHEALTHY,
    AQI_VERY_UNHEALTHY,
    AQI_HAZARDOUS
};

struct pm25_aqi_ctx{
    int iHigh;
    int iLow;
    float cHigh;
    float cLow;
    float aqi;
    uint16_t pm25;
};

typedef enum aqiLevel eAqiLevel;
typedef struct pm25_aqi_ctx pm25_aqi_ctx_t;

/**
 * @brief   Convert PM2.5 concentration to AQI value.
 * @param   ctx pointer to AQI data structure for input and output.
 * @return  none
 */
void pm25ToAqi(pm25_aqi_ctx_t* ctx);

/**
 * @brief   get PM2.5 value 
 * @param   pm25 is variable address to store PM2.5 value
 * @return  none
 *  */
void getPm25(uint16_t* pm25);

/**
 * @brief   read dust data 
 * @param   buf buffer address to store data
 * @param   len length of buffer - must be atlease 32 bytes 
 * @return  none
 *  */
void readDustData(uint8_t* buf, int len);

/**
 * @brief   check dust data 
 * @param   buf buffer address
 * @return  none
 */
void checkDustData(uint8_t* buf);

/**
 * @brief   Initialize the UART interface for dust sensor communication
 * @param   uart_file_path is file path of UART
 * @return  0 if success; -1 otherwise
 */
int dustSensor_uart_init(char* uart_file_path);

#endif