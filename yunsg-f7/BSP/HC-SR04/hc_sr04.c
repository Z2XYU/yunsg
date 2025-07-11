#include "hc_sr04.h"
#include "dwt_delay.h"
#include "tim.h"
#include "stdio.h"
#include "kalman_filter.h"

KalmanFilter_t ultrasonic_kf[HC_SR04_SENSOR_NUM];

static int hc_sr04_selected_id = 0;

int get_selected_id_safe(void)
{
    int id;
    osKernelLock();
    id = hc_sr04_selected_id;
    osKernelUnlock();
    return id;
}

void set_selected_id_safe(int id)
{
    osKernelLock();
    hc_sr04_selected_id = id;
    osKernelUnlock();
}

HC_SR04_Sensor_t hc_sr04_sensor[HC_SR04_SENSOR_NUM] = {
    {.id = 0,
     .location = 1,
     .cap_data = {0, 0, 0, 0, 0},
     .tim = {&htim4, TIM_CHANNEL_1},
     .trig = {GPIOD, GPIO_PIN_11}},
    {.id = 1,
     .location = 2,
     .cap_data = {0, 0, 0, 0, 0},
     .tim = {&htim4, TIM_CHANNEL_2},
     .trig = {GPIOG, GPIO_PIN_3}},
};

void hc_sr04_fliter_init(void)
{
    for (int i = 0; i < HC_SR04_SENSOR_NUM; i++)
    {
        kalman_init(&ultrasonic_kf[i], 0.05f, 0.05f, 10.0f);
    }
}

int find_hc_sr04_sensor(int location)
{
    for (uint8_t i = 0; i < HC_SR04_SENSOR_NUM; i++)
    {
        if (hc_sr04_sensor[i].location == location)
        {
            return hc_sr04_sensor[i].id;
        }
    }
    return -1;
}

void hc_sr04_timer_start(HC_SR04_Sensor_t *sensor)
{
    HAL_TIM_IC_Start_IT(sensor->tim.htim, sensor->tim.tim_channel);
}

void hc_sr04_measurement_start(HC_SR04_Sensor_t *sensor)
{
    __HAL_TIM_SET_COUNTER(sensor->tim.htim, 0);

    HAL_GPIO_WritePin(sensor->trig.trig_port, sensor->trig.trig_pin, GPIO_PIN_SET);
    dwt_delay_us(10);
    HAL_TIM_IC_Start_IT(sensor->tim.htim, sensor->tim.tim_channel); // 必须调用
    HAL_GPIO_WritePin(sensor->trig.trig_port, sensor->trig.trig_pin, GPIO_PIN_RESET);
}

void hc_sr04_measurement_stop(HC_SR04_Sensor_t *sensor)
{
    sensor->cap_data.is_first_capture = 0;
    sensor->cap_data.echo_time_us = 0;
    sensor->cap_data.falling_edge_val = 0;
    sensor->cap_data.rising_edge_val = 0;
    sensor->cap_data.distance_cm = 0;

    HAL_TIM_IC_Stop_IT(sensor->tim.htim, sensor->tim.tim_channel);
}
