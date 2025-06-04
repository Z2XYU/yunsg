#ifndef __MQTT_H
#define __MQTT_H

#include "main.h"

typedef struct 
{
    char action[32];
    char option[32];
    int cabinetLocation;
} mqttMsg;

#define MQTT_CLIENT_ID				"123"
#define MQTT_CLIENT_USERNAME		"zhongxu"
#define MQTT_CLIENT_PASSWORD		"310514"

#define MQTT_SERVER_IP				"129.211.26.112"
#define MQTT_SERVER_PORT			"1883"

void mqtt_init(void);
void mqtt_send_heart(void);
void mqtt_reconnect(void);
uint8_t mqtt_publish_data(char *mqtt_topic,char* mqtt_message);
#endif
