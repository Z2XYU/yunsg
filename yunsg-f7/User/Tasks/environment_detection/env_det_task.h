#pragma once

#include "main.h"



#define SNESOR_NUM 5

typedef enum
{
    LIGHT_SENSOR=0,         //光敏传感器
    TEMPERATURE_SENSAOR,    //热敏传感器
    ULTRAVIOLET_SNESOR,     //紫外线传感器
    RAINFALL_SENSOR,        //雨水传感器
    VIBRATION_SENSOR,       //震荡传感器
} SENSOR;



void SH40Task(void *argument);
void adcSensorsTask(void *argument);
void onenet_upload(void *argument);