#include "plasma.h"
#include "tim.h"
void plasma_enable(void)
{
    __HAL_TIM_SET_COUNTER(&htim2,0);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_GPIO_WritePin(PLASMA1_RELAY_PORT,PLASMA1_RELAY_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PLASMA2_RELAY_PORT,PLASMA2_RELAY_PIN,GPIO_PIN_RESET);
}

void plasma_disable(void)
{
    HAL_GPIO_WritePin(PLASMA1_RELAY_PORT,PLASMA1_RELAY_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(PLASMA2_RELAY_PORT,PLASMA2_RELAY_PIN,GPIO_PIN_SET);
    HAL_TIM_Base_Stop_IT(&htim2);
}