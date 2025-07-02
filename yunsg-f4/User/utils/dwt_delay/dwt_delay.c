#include "stm32f4xx.h" 
#include "dwt_delay.h"
#include "core_cm4.h"

void dwt_delay_init(void)
{
    // 使能DWT外设
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_delay_us(uint32_t us)
{
    extern uint32_t SystemCoreClock;
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles);
}

void dwt_delay_ms(uint32_t ms)
{
    while (ms--)
    {
        dwt_delay_us(1000);
    }
}