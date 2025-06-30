#ifndef __TASKS_H__
#define __TASKS_H__

#include "main.h"

#define WIFI_RX_BUFFER_SIZE 128

typedef struct 
{
  uint8_t data[WIFI_RX_BUFFER_SIZE];
  uint16_t length;
} wifiMessage_t;

typedef struct
{
    char action[32];
    char option[32];
    int cabinetLocation;
}controlMsg_t;

typedef struct 
{   
    char topic[32];
    controlMsg_t msg;
}wifiJson_t;

typedef enum
{
  OPTION_UNKNOWN=0,
  OPTION_CABINET_OPEN,
  OPTION_CABINET_CLOSE
} option_e;


void wifiControlTask(void *argument);
#endif
