#pragma once

#include "main.h"

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float prev_error;
    float integral;
    float max_output;
    float min_output;
    float prev_time;
    float sample_time;
} PID_Controller;

void pid_clear(PID_Controller *pid);
void pid_init(PID_Controller *pid, float Kp, float Ki, float Kd, float setpoint, float sample_time);
float pid_computer(PID_Controller *pid, float current_value, float current_time);



