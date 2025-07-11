#pragma once

#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include "kalman_filter.h"

#define HC_SR04_SENSOR_NUM 2

typedef struct 
{
    GPIO_TypeDef* trig_port;
    uint16_t trig_pin;
}TrigPin_t;

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t tim_channel;
}TIM_t;

typedef struct {
    uint32_t rising_edge_val;     // 上升沿捕获值（Echo 开始）
    uint32_t falling_edge_val;    // 下降沿捕获值（Echo 结束）
    uint8_t is_first_capture;     // 是否是第一次捕获（用于判断上升沿 or 下降沿）
    uint32_t echo_time_us;        // 回波持续时间（us）
    float distance_cm; 
} CaptureData_t;


typedef struct
{
    int id;
    int location;
    TrigPin_t trig;
    TIM_t tim;
    CaptureData_t cap_data;
}HC_SR04_Sensor_t;

extern HC_SR04_Sensor_t hc_sr04_sensor[HC_SR04_SENSOR_NUM];
// extern int hc_sr04_selected_id;
extern KalmanFilter_t ultrasonic_kf[HC_SR04_SENSOR_NUM];


void set_selected_id_safe(int id);
int get_selected_id_safe(void);
int find_hc_sr04_sensor(int location);
void hc_sr04_measurement_start(HC_SR04_Sensor_t* sensor);
void hc_sr04_measurement_stop(HC_SR04_Sensor_t *sensor);
void hc_sr04_timer_start(HC_SR04_Sensor_t *sensor);
void hc_sr04_fliter_init(void);

