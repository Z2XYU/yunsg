#include "motor_control_tasks.h"
#include "hc_sr04.h"
#include "stdio.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"
#include "message_receive.h"
#include "user_TaskInit.h"
#include "cmsis_os.h"

#define MOTOR_DEBUG 1

// HC_SR04_Sensor_t *sensor = NULL;
Option_e option_g = OPTION_UNKNOWN; // 全局变量 判断现在的操作是什么

extern uint8_t mqtt_rx_buffer[MQTT_MESSAGE_BUFFER_SIZE];
extern DMA_HandleTypeDef hdma_usart2_rx;


HC_SR04_Sensor_t sensor;

void UltrasonicTask(void *argument)
{
    while (1)
    {
        hc_sr04_measurement_start(&hc_sr04_sensor[hc_sr04_selected_id]);
        osSemaphoreAcquire(distReadySemaphoreHandle, osWaitForever);
#if MOTOR_DEBUG
        printf("dist: %d\n", (int)hc_sr04_sensor[hc_sr04_selected_id].cap_data.distance_cm);
#endif
        /*门磁传感器，暂未使用*/
        if (option_g == OPTION_CABINET_OPEN)
        {
            if (hc_sr04_sensor[hc_sr04_selected_id].cap_data.distance_cm >= 40)
            {
                /*电机停止*/
                printf("电机停止\n");

                /*超声波测距停止*/
                hc_sr04_measurement_stop(&hc_sr04_sensor[hc_sr04_selected_id]);
                osThreadSuspend(ultrasonicTaskHandle);
            }
        }
        else if (option_g == OPTION_CABINET_CLOSE)
        {
            if (hc_sr04_sensor[hc_sr04_selected_id].cap_data.distance_cm <= 10)
            {
                /*电机停止*/
                printf("电机停止\n");

                /*超声波测距停止*/
                hc_sr04_measurement_stop(&hc_sr04_sensor[hc_sr04_selected_id]);
                osThreadSuspend(ultrasonicTaskHandle);
            }
        }

        osDelay(500);
    }
}

void CmdReceiveTask(void *argument)
{
    // 先挂起超声波任务
    osThreadSuspend(ultrasonicTaskHandle);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, mqtt_rx_buffer, sizeof(mqtt_rx_buffer));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    while (1)
    {
        MQTTMessage_t msg;
        if (osMessageQueueGet(MQTTMessageReceiveQueueHandle, &msg, NULL, osWaitForever) == osOK)
        {
#if MOTOR_DEBUG
            printf("已接收消息\n");
#endif
            /*处理解析的数据*/
            ControlJson_t control_cmd = mqtt_message_parse(msg);
            int location = control_cmd.msg.cabinetLocation;


            /*判断是否存在这个传感器*/
            uint8_t res = find_hc_sr04_sensor(location);
            if (res == -1)
            {
                printf("不存在该传感器\n");
                continue;
            }
            else
                hc_sr04_selected_id = res;

            option_g = control_cmd_prase(control_cmd.msg.option);

            if (option_g == OPTION_CABINET_OPEN)
            {
                osThreadResume(ultrasonicTaskHandle); // 在这里打开超声波测距
                hc_sr04_measurement_start(&hc_sr04_sensor[hc_sr04_selected_id]);

                printf("开门逻辑\n");
                /*启动电机*/
            }
            else if (option_g == OPTION_CABINET_CLOSE)
            {
                osThreadResume(ultrasonicTaskHandle); // 在这里打开超声波测距
                hc_sr04_measurement_start(&hc_sr04_sensor[hc_sr04_selected_id]);
                printf("关门逻辑\n");
                /*启动电机*/
            }
            //control_cabinet(control_cmd);

            // HAL_UART_Transmit_DMA(&huart2, msg.data, msg.length);
        }
    }
}