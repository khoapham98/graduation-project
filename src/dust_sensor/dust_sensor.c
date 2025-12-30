/**
 * @file    dust_sensor.c
 * @brief   PMS7003 driver source file
 */
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "sys/log.h"
#include "src/dust_sensor/dust_sensor.h"
#include "src/drivers/uart.h"

static int uart_fd = 0;
static const int aqiRanges[AQI_LEVEL_COUNT][2] = {
    {0, 50}, 
    {51, 100},
    {101, 150},
    {151, 200},
    {201, 300},
    {301, 500}
};

static const float pm25Ranges[AQI_LEVEL_COUNT][2] = {
    {0.0f, 12.0f},
    {12.1f, 35.4f},
    {35.5f, 55.4f},
    {55.5f, 150.4f},
    {150.5f, 250.4f},
    {250.5f, 500.0f}
};

static eAqiLevel getPm25AqiLevel(uint16_t pm25)
{
    for (int i = 0; i < AQI_LEVEL_COUNT - 1; i++) {
        if ((float) pm25 <= pm25Ranges[i][1])
            return (eAqiLevel) i;
    }

    return AQI_HAZARDOUS;
}

void pm25ToAqi(pm25_aqi_ctx_t* ctx)
{
    eAqiLevel level = getPm25AqiLevel(ctx->pm25);

    ctx->iLow  = aqiRanges[level][0];
    ctx->iHigh = aqiRanges[level][1];
    ctx->cLow  = pm25Ranges[level][0];
    ctx->cHigh = pm25Ranges[level][1];

    if (ctx->cHigh == ctx->cLow) {
        LOG_ERR("Invalid PM2.5 breakpoint - keep previous data");
        return;
    }

    float rangeAqi = (float) (ctx->iHigh - ctx->iLow);
    float rangeConcentration = (float) (ctx->cHigh - ctx->cLow);
    float concentrationDiff  = (float) (ctx->pm25 - ctx->cLow);

    ctx->aqi = (rangeAqi / rangeConcentration) * concentrationDiff + (float) ctx->iLow;
}

void getPm25(uint16_t* pm25)
{
    uint8_t dust_buf[DUST_DATA_FRAME] = {0};
    readDustData(dust_buf, sizeof(dust_buf));

	*pm25 = dust_buf[12] << 8 | dust_buf[13]; 
}

void checkDustData(uint8_t* buf)
{
	const char* str[2] = {"FAILED", "OK"};
	LOG_INF("Start character 1: 0x%02X [%s]\n", buf[0], str[buf[0] == 0x42]);
	LOG_INF("Start character 2: 0x%02X [%s]\n", buf[1], str[buf[1] == 0x4d]);
	int frame_len = buf[2] << 8 | buf[3];
	LOG_INF("Frame length: %d bytes [%s]\n", frame_len, str[frame_len == 28]);
}

void readDustData(uint8_t* buf, int len)
{
    if (len < DUST_DATA_FRAME) 
        LOG_WRN("dust_sensor: May not receive data fully");

    int total = 0;
    while (total < len) {
        int ret = read(uart_fd, buf + total, len - total);
        if (ret > 0) {
            total += ret;
        } else if (ret < 0) {
            LOG_ERR("Read failed: %s", strerror(errno));
            break;
        }
    }
}

int dustSensor_uart_init(char* uart_file_path)
{
	uart_fd = uart_init(uart_file_path, B9600, false);
    if (uart_fd < 0) {
        return -1;
	}
    
	LOG_INF("Dust Sensor Initialization successful");
	return 0;
}