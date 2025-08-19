#pragma once

#include "main.h"

#define PTC1_CHANNEL 3
#define PTC2_CHANNEL 4

void ptc_init(void);
void ptc_set_power(uint16_t power);
