#pragma once

#include "main.h"

typedef struct
{
    float q; // 过程噪声协方差
    float r; // 测量噪声协方差
    float x; // 上一次的估计值
    float p; // 上一次的估计误差协方差
    float k; // 卡尔曼增益
} KalmanFilter_t;

void kalman_init(KalmanFilter_t *kf, float q, float r, float initial_value);
float kalman_update(KalmanFilter_t *kf, float measurement);
