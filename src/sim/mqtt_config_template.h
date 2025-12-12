/**
 * @file    mqtt_config_template.h
 * @brief   MQTT configuration template file for credentials and topics
 * @attention Rename this file to mqtt_config.h before adding real credentials
 */
#ifndef _MQTT_CONFIG_H_
#define _MQTT_CONFIG_H_

#define     MQTT_CLIENT_ID      "ENTER_CLIENT_ID"
#define     MQTT_USERNAME       "ENTER_USERNAME"
#define     MQTT_PASSWORD       "ENTER_PASSWORD"

#define     MQTT_SERVER_ADDR    "tcp://your_server:1883"

#define     MQTT_PUB_TOPIC      "your/pub/topic"
#define     MQTT_SUB_TOPIC      "your/sub/topic"

#endif