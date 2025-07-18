#include "4g_d43.h"
#include "string.h"
#include "usart.h"

void d43_send_at(const char *cmd)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), 1000);
    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, 100);
}

uint8_t d43_wait(uint32_t timeout)
{
    char buffer[128] = {0};
    HAL_UART_Receive(&huart3, (uint8_t *)buffer, sizeof(buffer), timeout);
    return strstr(buffer, "OK") == NULL ? 1 : 0;
}

uint8_t d43_init(void)
{
    d43_send_at("AT");
    return d43_wait(1000);
}
