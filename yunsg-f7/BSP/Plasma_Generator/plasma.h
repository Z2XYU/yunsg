#pragma once

#include "main.h"

#define PLASMA1_RELAY_PORT GPIOE
#define PLASMA1_RELAY_PIN GPIO_PIN_2

#define PLASMA2_RELAY_PORT GPIOI
#define PLASMA2_RELAY_PIN GPIO_PIN_3

void plasma_enable(void);
void plasma_disable(void);