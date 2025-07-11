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
/*Tasks -------------------------------------*/

osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t MessageSendTask_attributes = {
    .name = " MessageSendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh1,
};

osThreadId_t GUIRefreshTaskHandle;
const osThreadAttr_t GUIRefreshTask_attributes = {
    .name = "GUIRefreshTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal,
};

/*电机控制*/
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
    GUIRefreshTaskHandle = osThreadNew(GUIRefreshTask, NULL, &GUIRefreshTask_attributes);
    // MessageSendTaskHandle = osThreadNew(MessageSendTask,NULL,&MessageSendTask_attributes);

    distReadySemaphoreHandle = osSemaphoreNew(1, 0, NULL);
    ultrasonicTaskHandle = osThreadNew(UltrasonicTask, NULL, &ultrasonicTask_attributes);
    CmdReceiveTaskHandle = osThreadNew(CmdReceiveTask, NULL, &CmdReceiveTask_attributes);

}