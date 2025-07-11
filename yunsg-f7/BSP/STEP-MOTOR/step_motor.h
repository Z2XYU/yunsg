#pragma once

#include "main.h"

#define STEP_MOTOR_NUM 1

#define START_FREQUENCY 500

typedef struct
{
    int id;
    int location;

    GPIO_TypeDef* enable_port;
    uint16_t enable_pin;

    TIM_HandleTypeDef* step_htim;
    uint32_t  step_channel;

    GPIO_TypeDef* dir_port;
    uint16_t dir_pin;

    uint32_t delay_us_min;     // 最快速度时的脉冲间隔（us）
    uint32_t delay_us_max;     // 最慢速度时的脉冲间隔（us）

    GPIO_PinState dir_positive; // 正方向电平
}StepMotor_t;

typedef enum
{
    MOTOR_ENABLE=0,
    MOTOR_DISABLE,
}MotorEn_e;

typedef enum
{
    FORWARD=0,
    REVERSE,
}MotorDir_e;

extern StepMotor_t motors[STEP_MOTOR_NUM];

void set_selected_motor_id_safe(int id);
int get_selected_motor_id_safe(void);
void motor_start(StepMotor_t *motor);
void motor_stop(StepMotor_t *motor);
void motor_set_dir(StepMotor_t *motor,MotorDir_e dir);
void motor_set_enable(StepMotor_t *motor,MotorEn_e sta);