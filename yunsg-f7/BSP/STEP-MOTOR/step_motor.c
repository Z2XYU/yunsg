#include "step_motor.h"
#include "tim.h"
#include "cmsis_os.h"

static int step_motor_selected_id=0;

StepMotor_t motors[STEP_MOTOR_NUM]=
{
    {
        .id=0,
        .location=1,
        .enable_pin=MOTOR_EN_1_Pin,
        .enable_port=MOTOR_EN_1_GPIO_Port,
        .dir_pin=MOTOR_DIR_1_Pin,
        .dir_port=MOTOR_DIR_1_GPIO_Port,
        .dir_positive=0,
        .step_htim=&htim5,
        .step_channel=TIM_CHANNEL_1,
    },
    
};

int get_selected_motor_id_safe(void)
{
    int id;
    osKernelLock();
    id = step_motor_selected_id;
    osKernelUnlock();
    return id;
}

void set_selected_motor_id_safe(int id)
{
    osKernelLock();
    step_motor_selected_id = id;
    osKernelUnlock();
}


void motor_set_enable(StepMotor_t *motor,MotorEn_e sta)
{
    if(sta)
    {
        HAL_GPIO_WritePin(motor->enable_port,motor->enable_pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(motor->enable_port,motor->enable_pin,GPIO_PIN_RESET);
    }
}

void motor_set_dir(StepMotor_t *motor,MotorDir_e dir)
{
    if(dir)
    {
        HAL_GPIO_WritePin(motor->dir_port,motor->dir_pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(motor->dir_port,motor->dir_pin,GPIO_PIN_RESET);
    }
}

void motor_start(StepMotor_t *motor)
{
    uint32_t psc=108000000/1000/START_FREQUENCY;
    __HAL_TIM_SET_PRESCALER(motor->step_htim,psc);
    HAL_TIM_PWM_Start(motor->step_htim,motor->step_channel);
}

void motor_stop(StepMotor_t *motor)
{
    HAL_TIM_PWM_Stop(motor->step_htim,motor->step_channel);
}




