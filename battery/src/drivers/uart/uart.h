#ifndef __UART__H
#define __UART__H

#include "drivers/register/stc8g.h"
#include <stdint.h>

void uart_init(void);
void uart_send_byte(uint8_t byte);
void uart_send_string(const char *str);
void uart_send_uint16_t(uint16_t num);
void uart_send_float(float num);
#endif
