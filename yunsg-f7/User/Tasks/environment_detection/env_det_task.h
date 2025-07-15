#pragma once

#include "main.h"

void SH40Task(void *argument);

typedef enum
{
    LIGHT_SENSOR=0,
    TEMPERATURE_SENSAOR,
    Ultraviolet_SNESOR,
    RAINFALL_SENSOR
} SENSOR;