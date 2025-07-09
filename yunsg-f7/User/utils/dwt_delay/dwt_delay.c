#include "dwt_delay.h"
#include "stm32f7xx.h"

// 需要开启访问 DWT 和 ITM 的权限寄存器
#define DWT_LAR_KEY     0xC5ACCE55
#define DEMCR_TRCENA    (1 << 24)

void dwt_init(void)
{
    // 1. 使能 DWT 寄存器
    CoreDebug->DEMCR |= DEMCR_TRCENA;

    // 2. 解锁 DWT 寄存器（部分芯片需要）
    DWT->LAR = DWT_LAR_KEY;

    // 3. 清零 CYCCNT 并使能
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_delay_us(uint32_t us)
{
    volatile  uint32_t start = DWT->CYCCNT;
    volatile  int32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000U);  // 计算所需周期数

    while ((uint32_t)(DWT->CYCCNT - start) < ticks);  // 防止溢出卡死
}

void dwt_delay_ms(uint32_t ms)
{
    while (ms--)
    {
        dwt_delay_us(1000);
    }
}
