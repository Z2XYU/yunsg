#include "pid.h"
#include "cmsis_os.h"

void pid_init(PID_Controller *pid, float Kp, float Ki, float Kd, float setpoint, float sample_time)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->setpoint = setpoint;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->sample_time = sample_time;
    pid->max_output = 4095;
    pid->min_output = 0;
    pid->prev_time = 0;
}

void pid_clear(PID_Controller *pid)
{
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->prev_time = osKernelGetTickCount() / (float)osKernelGetTickFreq();
}

// PID控制器计算
float pid_computer(PID_Controller *pid, float current_value, float current_time)
{
    // 计算时间差
    float delta_time = current_time - pid->prev_time;
    if (delta_time < pid->sample_time)
    {
        return 0.0f;
    }

    float error = pid->setpoint - current_value; // 计算当前误差
    pid->integral += error * delta_time;         // 积分项
    if (pid->integral > pid->max_output)
    {
        pid->integral = pid->max_output;
    }
    else if (pid->integral < pid->min_output)
    {
        pid->integral = pid->min_output;
    }

    float derivate = (error - pid->prev_error) / delta_time; // 微分项

    // PID输出公式
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivate);

    // 输出限幅
    if (output > pid->max_output)
    {
        output = pid->max_output;
    }
    else
    {
        output = pid->min_output;
    }

    // 更新上次误差
    pid->prev_error = error;
    pid->prev_time = current_time;

    return output;
}