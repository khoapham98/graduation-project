/**
 * @file    device_setup.c
 * @brief   setup device source file
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "sys/log.h"
#include "sys/ringbuffer.h"
#include "device_setup.h"
#include "src/drivers/uart.h"
#include "src/dust_sensor/dust_sensor.h"
#include "src/gps/gps.h"

/* thread array for manage */
pthread_t thread[MAX_THREADS];

/* semaphore for dust data and GPS data */
sem_t dustDataDoneSem;
sem_t dustDataReadySem;
sem_t gpsDataDoneSem;
sem_t gpsDataReadySem;

/* dust data buffer */
uint8_t dust_buf[DUST_DATA_FRAME];

/* GPS data */
double lat;
double lon;

/* json ring buffer */
ring_buffer_t json_ring_buf;
char json_ring_buf_data[RING_BUFFER_SIZE];

void* updateDustDataTask(void* arg)
{
	while (1) {
        /* wait for dust data to finish processing */
        sem_wait(&dustDataDoneSem);
        /* read dust sensor data */
		readDustData(dust_buf, sizeof(dust_buf));
        checkDustData(dust_buf);
        /* signal that dust data is ready */
        sem_post(&dustDataReadySem);
	}

	return arg;
}

void* updateGPSTask(void* arg)
{
	while (1) {
        /* wait for gps data to finish processing */
        sem_wait(&gpsDataDoneSem);
        /* read gps data */
        char gps_buf[NMEA_FRAME] = {0};
        readGpsData((uint8_t*) gps_buf, NMEA_FRAME);
        printf("%s\n", gps_buf);
        /* signal that dust data is ready */
        sem_post(&gpsDataReadySem);
	}

	return arg;
}

void* send2WebTask(void* arg)
{
	while (1) {
#if GPS_ENABLE
        /* wait for gps data is ready */
        sem_wait(&gpsDataReadySem);        

        /* signal that gps data has been processed */
        sem_post(&gpsDataDoneSem);
#endif

#if DUST_SENSOR_ENABLE
        /* wait for dust data is ready */
        sem_wait(&dustDataReadySem);        
        /* parse dust data to json format */
        char dust2json_buf[64] = {0}; 
        parseDustDataToJson(dust2json_buf, dust_buf, sizeof(dust2json_buf));
        /* put dust data json format to ring buffer */
        ring_buffer_queue_arr(&json_ring_buf, dust2json_buf, strlen(dust2json_buf));
        /* signal that dust data has been processed */
        sem_post(&dustDataDoneSem);
#endif
        
        /* send to web */
        char web_buf[256] = {0};
        ring_buffer_size_t ring_buf_size = ring_buffer_num_items(&json_ring_buf);
        ring_buffer_dequeue_arr(&json_ring_buf, web_buf, ring_buf_size);
        printf("%s\n", web_buf);
	}

	return arg;
}

int setupDustSensor(void) 
{
    /* Initialize Dust Sensor */
    int err = dustSensor_init(UART1_FILE_PATH);    
    if (err != 0)
        return err;

    sem_init(&dustDataReadySem, 0, 0);
    sem_init(&dustDataDoneSem, 0, 1);

    /* create thread for update dust sensor data */
    err = pthread_create(&thread[0], NULL, updateDustDataTask, NULL);
    if (err != 0) 
        LOG_ERR("pthread_create: %d\n", err);

    return err;
}

int setupGPS(void) 
{
    /* Initialize GPS */
    int err = GPS_init(UART4_FILE_PATH);    
    if (err != 0)
        return err;

    sem_init(&gpsDataReadySem, 0, 0);
    sem_init(&gpsDataDoneSem, 0, 1);

    /* create thread for update gps data */
    err = pthread_create(&thread[1], NULL, updateGPSTask, NULL);
    if (err != 0) 
        LOG_ERR("pthread_create: %d\n", err);

    return err;
}

int setup4G(void) 
{
    /* Initialize ring buffer to store JSON data */
    ring_buffer_init(&json_ring_buf, json_ring_buf_data, sizeof(json_ring_buf_data));

    /* create thread for push data to web */
    int err = pthread_create(&thread[2], NULL, send2WebTask, NULL);
    if (err != 0) 
        LOG_ERR("pthread_create: %d\n", err);

    return err;
}

int deviceSetup(void)
{
    int err = 0;

#if DUST_SENSOR_ENABLE
    err = setupDustSensor();
    if (err != 0)
        LOG_ERR("Failed to setup dust sensor\n");
#endif

#if GPS_ENABLE
    err = setupGPS();
    if (err != 0)
        LOG_ERR("Failed to setup GPS\n");
#endif

#if ENABLE_4G
    err = setup4G();
    if (err != 0)
        LOG_ERR("Failed to setup 4G module\n");
#endif

    return err;
}
