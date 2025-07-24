#include "motor_control_tasks.h"
#include "hc_sr04.h"
#include "stdio.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"
#include "message_receive.h"
#include "user_TaskInit.h"
#include "cmsis_os.h"
#include "kalman_filter.h"
#include "cmsis_os2.h"
#include "step_motor.h"
#include "rc522.h"
#include "rfid_compare.h"
#include "plasma.h"

#define MOTOR_DEBUG 0

Option_e option_g = OPTION_UNKNOWN; 

extern uint8_t mqtt_rx_buffer[MQTT_MESSAGE_BUFFER_SIZE];
extern DMA_HandleTypeDef hdma_usart2_rx;

HC_SR04_Sensor_t sensor;

/*测试*/
//需要在结构体中加入服装id 
static int clothes_id = -1;

/*RFID检测任务*/
void rfidDetectionTask(void *argument)
{
    RC522_Init();
    osThreadSuspend(rfidDetectionTaskHandle);
    while (1)
    {
        Cardcompare(clothes_id);
        /*检测逻辑*/

        osDelay(500);
    }
}

/*超声波测距任务*/
void UltrasonicTask(void *argument)
{
    hc_sr04_fliter_init();
    osThreadSuspend(ultrasonicTaskHandle);
    while (1)
    {
        int id = get_selected_id_safe();
        hc_sr04_measurement_start(&hc_sr04_sensor[id]);
        osSemaphoreAcquire(distReadySemaphoreHandle, osWaitForever);
#if MOTOR_DEBUG
        printf("dist: %d\n", (int)hc_sr04_sensor[id].cap_data.distance_cm);
#endif

        float raw_dist = hc_sr04_sensor[id].cap_data.distance_cm;

        // 滤波处理
        float filtered_dist = kalman_update(&ultrasonic_kf[id], raw_dist);

        printf("dist_filter: %d\n", (int)filtered_dist);

        /*门磁传感器，暂未使用*/
        if (option_g == OPTION_CABINET_OPEN)
        {
            if (filtered_dist >= 40)
            {
                /*电机停止*/
                motor_stop(&motors[0]);
                motor_set_enable(&motors[0], MOTOR_DISABLE);
#if MOTOR_DEBUG
                printf("电机停止\n");
#endif
                /*超声波测距停止*/
                hc_sr04_measurement_stop(&hc_sr04_sensor[id]);
                osThreadSuspend(ultrasonicTaskHandle);
            }
        }
        else if (option_g == OPTION_CABINET_CLOSE)
        {
            if (filtered_dist <= 10)
            {
                /*电机停止*/
                motor_stop(&motors[0]);
                motor_set_enable(&motors[0], MOTOR_DISABLE);
#if MOTOR_DEBUG
                printf("电机停止\n");
#endif

                /*超声波测距停止*/
                hc_sr04_measurement_stop(&hc_sr04_sensor[id]);
                osThreadSuspend(ultrasonicTaskHandle);
            }
        }

        osDelay(500);
    }
}

/*控制命令接收任务*/
void CmdReceiveTask(void *argument)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, mqtt_rx_buffer, sizeof(mqtt_rx_buffer));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    while (1)
    {
        MQTTMessage_t msg;
        if (osMessageQueueGet(MQTTMessageReceiveQueueHandle, &msg, NULL, osWaitForever) == osOK)
        {
#if MOTOR_DEBUG
            // printf("已接收消息\n");
#endif
            /*处理解析的数据*/
            ControlJson_t control_cmd = mqtt_message_parse(msg);
            int location = control_cmd.msg.cabinetLocation;
            char action[16];
            strcpy(action, control_cmd.msg.action);

            clothes_id = control_cmd.msg.clothesID;

            /*判断是否存在这个传感器*/
            int res = find_hc_sr04_sensor(location);
            if (res == -1)
            {
                printf("不存在该传感器\n");
                continue;
            }
            else
                set_selected_id_safe(res);

            int motor_id =  find_step_motor(location);
            if(motor_id ==-1)
            {
                printf("不存在该位置\n");
                continue;
            }
            else
            {
                set_selected_motor_id_safe(motor_id);
            }

            option_g = control_cmd_prase(control_cmd.msg.option);

            if (option_g == OPTION_CABINET_OPEN)
            {
                if (strcmp(action, "return") == 0)
                {
                    /*rfid溯源逻辑*/
                    osThreadResume(rfidDetectionTaskHandle);

                    if (osSemaphoreAcquire(rfidReadySemaphoreHandle, 10000) == osOK)
                    {
                        printf("检测成功,柜门已打开\n");
                        osThreadResume(ultrasonicTaskHandle); // 在这里打开超声波测距
                        /*启动电机*/
                        motor_set_enable(&motors[motor_id], MOTOR_ENABLE);
                        motor_set_dir(&motors[motor_id], FORWARD);
                        motor_start(&motors[motor_id]);
                        osThreadSuspend(rfidDetectionTaskHandle);
                    }
                    else // 等待失败
                    {
                        printf("检测失败\n");
                        osThreadSuspend(rfidDetectionTaskHandle);
                    }
                }
                else if (strcmp(action, "rent"))
                {
                    osThreadResume(ultrasonicTaskHandle); // 在这里打开超声波测距
                    /*启动电机*/
                    motor_set_enable(&motors[motor_id], MOTOR_ENABLE);
                    motor_set_dir(&motors[motor_id], FORWARD);
                    motor_start(&motors[motor_id]);
                }
            }
            else if (option_g == OPTION_CABINET_CLOSE)
            {
                osThreadResume(ultrasonicTaskHandle); // 在这里打开超声波测距

                motor_set_enable(&motors[motor_id], MOTOR_ENABLE);
                motor_set_dir(&motors[motor_id], REVERSE);
                motor_start(&motors[motor_id]);
                /*启动电机*/
            }

            /*等离子环境净化*/
            plasma_enable();
        }
    }
}