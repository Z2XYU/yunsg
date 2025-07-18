#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "user_GUIRefresh.h"
#include "message_receive.h"
#include "message_send.h"
#include "user_MessageSendTask.h"
#include "dwt_delay.h"
#include "sdram.h"
#include "motor_control_tasks.h"
#include "step_motor.h"
#include "tim.h"
#include "kalman_filter.h"
#include "stdio.h"
#include "env_det_task.h"
/*Tasks -------------------------------------*/
/*GUI系统*/
osThreadId_t GUIRefreshTaskHandle;
const osThreadAttr_t GUIRefreshTask_attributes = {
    .name = "GUIRefreshTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal,
};

osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t MessageSendTask_attributes = {
    .name = " MessageSendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh1,
};

/*电控系统*/
/*RFID检测任务*/
osThreadId_t rfidDetectionTaskHandle;
const osThreadAttr_t rfidDetectionTask_attributes = {
    .name = "rfidDetectionTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal2,
};
/*超声波距离测量任务*/
osThreadId_t ultrasonicTaskHandle;
const osThreadAttr_t ultrasonicTask_attributes = {
    .name = "ultrasonicTask",
    .stack_size = 128 * 8,
    .priority = (osPriority_t)osPriorityNormal2,
};
/*控制命令接收任务*/
osThreadId_t CmdReceiveTaskHandle;
const osThreadAttr_t CmdReceiveTask_attributes = {
    .name = "CmdReceiveTaskTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityHigh,
};
/*距离测量完成信号量*/
osSemaphoreId_t distReadySemaphoreHandle;
/*RFID检测信号量*/
osSemaphoreId_t rfidReadySemaphoreHandle;

/*环境检测系统*/
/*SH40温度检测*/
osThreadId_t temperatureMeasurementTaskHandle;
const osThreadAttr_t temperatureMeasurementTask_attributes = {
    .name = "temperatureMeasurementTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityAboveNormal,
};
/*ADC传感器采样*/
osThreadId_t adcSensorsMeasurementTaskHandle;
const osThreadAttr_t adcSensorsMeasurementTask_attributes = {
    .name = "adcSensorsMeasurementTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityAboveNormal,
};
/*onenet信息上传*/
osThreadId_t onenetMsgUploadTaskHandle;
const osThreadAttr_t onenetMsgUploadTask_attributes = {
    .name = "onenetMsgUploadTask",
    .stack_size = 128 * 8,
    .priority = (osPriority_t)osPriorityHigh,
};



/* Message queues ----------------------------*/

// 接收队列
osMessageQueueId_t MQTTMessageReceiveQueueHandle;
const osMessageQueueAttr_t MQTTMessageReceiveQueue_attributes = {
    .name = "MQTTMessageReceiveQueue",
};

// 发送队列
osMessageQueueId_t MQTTMessageSendQueueHandle;
const osMessageQueueAttr_t MQTTMessageSendQueue_attributes = {
    .name = "MQTTMessageSendQueue",
};

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void user_tasks_init(void)
{
    dwt_init();
    sdram_init();

    /* add queues-------------------------------*/
    MQTTMessageReceiveQueueHandle = osMessageQueueNew(10, sizeof(MQTTMessage_t), &MQTTMessageReceiveQueue_attributes);
    MQTTMessageSendQueueHandle = osMessageQueueNew(10, sizeof(VoiceJson_t), &MQTTMessageSendQueue_attributes);
    /* add threads -----------------------------*/

    /*GUI图形化页面*/
    GUIRefreshTaskHandle = osThreadNew(GUIRefreshTask, NULL, &GUIRefreshTask_attributes);
    MessageSendTaskHandle = osThreadNew(MessageSendTask, NULL, &MessageSendTask_attributes); // 备注：后续改为语音发送

    /*电控系统*/
    distReadySemaphoreHandle = osSemaphoreNew(1, 0, NULL);
    rfidReadySemaphoreHandle = osSemaphoreNew(1, 0, NULL);
    ultrasonicTaskHandle = osThreadNew(UltrasonicTask, NULL, &ultrasonicTask_attributes);
    CmdReceiveTaskHandle = osThreadNew(CmdReceiveTask, NULL, &CmdReceiveTask_attributes);
    rfidDetectionTaskHandle = osThreadNew(rfidDetectionTask, NULL, &rfidDetectionTask_attributes);

    
    /*温控系统*/
    temperatureMeasurementTaskHandle = osThreadNew(SH40Task, NULL, &temperatureMeasurementTask_attributes);
    adcSensorsMeasurementTaskHandle = osThreadNew(adcSensorsTask,NULL,&adcSensorsMeasurementTask_attributes);
    onenetMsgUploadTaskHandle = osThreadNew(onenet_upload,NULL,&onenetMsgUploadTask_attributes);

    
}