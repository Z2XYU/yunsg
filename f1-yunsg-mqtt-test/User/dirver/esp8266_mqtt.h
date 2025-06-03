#ifndef __ESP8266_MQTT_H__
#define __ESP8266_MQTT_H__

#include "main.h"

void ESP_Init(void);
void ESP_MQTT_Connect(void);
void ESP_MQTT_Publish(const char* topic, const char* message);
void ESP_MQTT_Subscribe(const char* topic);
#endif
