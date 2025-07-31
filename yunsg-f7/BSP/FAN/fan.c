#include "fan.h"

void fan_enable(void)
{
    HAL_GPIO_WritePin(FAN_RELAY_PORT,FAN_RELAY_PIN,GPIO_PIN_RESET);
}

void fan_disable(void)
{
    HAL_GPIO_WritePin(FAN_RELAY_PORT,FAN_RELAY_PIN,GPIO_PIN_SET);
}