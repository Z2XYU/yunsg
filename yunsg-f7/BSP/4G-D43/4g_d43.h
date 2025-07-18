#pragma once

#include "main.h"

void d43_send_at(const char* cmd);
uint8_t d43_wait(uint32_t timeout);
uint8_t d43_init(void);