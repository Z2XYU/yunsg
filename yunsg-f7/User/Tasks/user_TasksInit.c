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
#include "http_send.h"
#include "pickup_code_inquiry.h"
#include "iwdg_feed_task.h"
#include "myiic.h"
#include "plasma.h"
#include "temperature_control_tasks.h"


/*Tasks -------------------------------------*/
/*看门狗喂狗任务*/
osThreadId_t iwdgFeedTaskHandle;
const osThreadAttr_t iwdgFeedTask_attributes = {
    .name = "iwdgFeedTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};

/*GUI页面刷新*/
osThreadId_t GUIRefreshTaskHandle;
const osThreadAttr_t GUIRefreshTask_attributes = {
    .name = "GUIRefreshTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal,
};
/*语音消息发送任务*/
osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t MessageSendTask_attributes = {
    .name = " MessageSendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh2,
};
/*取件码查询任务*/
osThreadId_t PickupCodeInquiryTaskHandle;
const osThreadAttr_t PickupCodeInquiryTask_attributes = {
    .name = " PickupCodeInquiryTask",
    .stack_size = 128 * 6,
    .priority = (osPriority_t)osPriorityHigh3,
};
/*取件码查询消息队列*/
osMessageQueueId_t PickupCodeMsgSendQueueHandle;
const osMessageQueueAttr_t PickupCodeMsgSendQueue_attributes = {
    .name = "PickupCodeMsgSendQueue",
};


/*电控系统*/
/*RFID检测任务*/
osThreadId_t rfidDetectionTaskHandle;
const osThreadAttr_t rfidDetectionTask_attributes = {
    .name = "rfidDetectionTask",
    .stack_size = 128 * 8,
    .priority = (osPriority_t)osPriorityNormal2,
};
/*超声波距离测量任务*/
osThreadId_t ultrasonicTaskHandle;
const osThreadAttr_t ultrasonicTask_attributes = {
    .name = "ultrasonicTask",
    .stack_size = 128 * 8,
    .priority = (osPriority_t)osPriorityNormal4,
};
/*控制命令接收任务*/
osThreadId_t CmdReceiveTaskHandle;
const osThreadAttr_t CmdReceiveTask_attributes = {
    .name = "CmdReceiveTaskTask",
    .stack_size = 128 * 16,
    .priority = (osPriority_t)osPriorityHigh,
};
/*距离测量完成信号量*/
osSemaphoreId_t distReadySemaphoreHandle;
/*RFID检测信号量*/
osSemaphoreId_t rfidReadySemaphoreHandle;

/*温度控制系统*/
osThreadId_t temperatureControlTaskHandle;
const osThreadAttr_t temperatureControlTask_attributes = {
    .name = "temperatureControlTask",
    .stack_size = 128 * 12,
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
    iic_init();
    plasma_disable();

    /* add queues-------------------------------*/
    MQTTMessageReceiveQueueHandle = osMessageQueueNew(10, sizeof(MQTTMessage_t), &MQTTMessageReceiveQueue_attributes);
    MQTTMessageSendQueueHandle = osMessageQueueNew(10, sizeof(VoiceJson_t), &MQTTMessageSendQueue_attributes);
    /* add threads -----------------------------*/

    /*喂狗任务*/
    iwdgFeedTaskHandle = osThreadNew(iwdgFeedTask,NULL,&iwdgFeedTask_attributes);

    /*GUI图形化页面*/
    GUIRefreshTaskHandle = osThreadNew(GUIRefreshTask, NULL, &GUIRefreshTask_attributes);
    /*优先验证结果，想法在实践中不断产生*/
    PickupCodeInquiryTaskHandle = osThreadNew(pickupCodeInquiryTask,NULL,&PickupCodeInquiryTask_attributes);
    MessageSendTaskHandle = osThreadNew(MessageSendTask, NULL, &MessageSendTask_attributes); // 备注：后续改为语音发送
    PickupCodeMsgSendQueueHandle = osMessageQueueNew(10, sizeof(PickupCode_t),&PickupCodeMsgSendQueue_attributes);  

    /*电控系统*/
    distReadySemaphoreHandle = osSemaphoreNew(1, 0, NULL);
    rfidReadySemaphoreHandle = osSemaphoreNew(1, 0, NULL);
    ultrasonicTaskHandle = osThreadNew(UltrasonicTask, NULL, &ultrasonicTask_attributes);
    CmdReceiveTaskHandle = osThreadNew(CmdReceiveTask, NULL, &CmdReceiveTask_attributes);
    rfidDetectionTaskHandle = osThreadNew(rfidDetectionTask, NULL, &rfidDetectionTask_attributes);

    
    /*温控系统*/
    temperatureControlTaskHandle = osThreadNew(temperatureControlTask,NULL,&temperatureControlTask_attributes);

}