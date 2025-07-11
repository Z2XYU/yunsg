#include "step_motor.h"

void set_motor_sta(uint8_t sta)
{
    if (sta)
    {
        HAL_GPIO_WritePin(MOTOR_EN_1_GPIO_Port, MOTOR_EN_1_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_EN_1_GPIO_Port, MOTOR_EN_1_Pin,GPIO_PIN_RESET);
    }
}

void set_motor_dir(uint8_t dir)
{
    if(dir)
    {
        HAL_GPIO_WritePin(MOTOR_DIR_1_GPIO_Port,MOTOR_DIR_1_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_DIR_1_GPIO_Port,MOTOR_DIR_1_Pin,GPIO_PIN_RESET);
    }
}