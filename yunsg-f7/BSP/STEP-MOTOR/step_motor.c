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
    {
        .id=1,
        .location=2,
        .enable_pin= MOTOR_EN_2_Pin,
        .enable_port = MOTOR_EN_2_GPIO_Port,
        .dir_pin=MOTOR_DIR_2_Pin,
        .dir_port=MOTOR_DIR_2_GPIO_Port,
        .dir_positive=0,
        .step_htim=&htim5,
        .step_channel=TIM_CHANNEL_2,
    }
    
};

int find_step_motor(int location)
{
    for(int i=0;i<STEP_MOTOR_NUM;i++)
    {
        if(location==motors[i].location)
        {
            return motors[i].id;
        }
    }
    return -1;
}

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

//低速匀速运行即可
void motor_start(StepMotor_t *motor)
{
    HAL_TIM_PWM_Start(motor->step_htim,motor->step_channel);
}

void motor_stop(StepMotor_t *motor)
{
    HAL_TIM_PWM_Stop(motor->step_htim,motor->step_channel);
}




