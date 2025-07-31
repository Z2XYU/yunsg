#pragma once
#include "main.h"

#define FAN_RELAY_PORT  GPIOG
#define FAN_RELAY_PIN   GPIO_PIN_12

void fan_disable(void);
void fan_enable(void);