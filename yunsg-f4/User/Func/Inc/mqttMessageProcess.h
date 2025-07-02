#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MQTT_MESSAGE_BUFFER_SIZE 128

typedef struct 
{
    uint8_t data[MQTT_MESSAGE_BUFFER_SIZE];
    uint16_t length; 
} MQTTMessage_t;

typedef struct
{
    char action[32];
    char option[32];
    int cabinetLocation;
}ControlMessage_t;

typedef struct 
{   
    char topic[32];
    ControlMessage_t msg;
}MQTTJson_t;

typedef enum
{
  OPTION_UNKNOWN=0,
  OPTION_CABINET_OPEN,
  OPTION_CABINET_CLOSE
} Option_e;

MQTTJson_t MQTT_Message_Parse(MQTTMessage_t msg);
void MQTT_Cabinet_Control(MQTTJson_t cmd);
#ifdef __cplusplus
}
#endif
