#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void dwt_delay_init(void);
void dwt_delay_us(uint32_t us);
void dwt_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

