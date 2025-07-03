#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "user_MessageReceiveTask.h"
#include "user_GUIRefreshTask.h"
#include "message_receive.h"
#include "message_send.h"
#include "user_MessageSendTask.h"
/*Tasks -------------------------------------*/

//MessageReceiveTask
osThreadId_t MessageReceiveTaskHandle;
const osThreadAttr_t  MessageReceiveTask_attributes = {
    .name = " MessageReceiveTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityHigh,
};

osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t  MessageSendTask_attributes = {
    .name = " MessageSendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh1,
};

osThreadId_t GUIRefreshTaskHandle;
const osThreadAttr_t GUIRefreshTask_attributes={
    .name ="GUIRefreshTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Message queues ----------------------------*/

//接收队列
osMessageQueueId_t MQTTMessageReceiveQueueHandle;
const osMessageQueueAttr_t MQTTMessageReceiveQueue_attributes = {
    .name="MQTTMessageReceiveQueue",
};

//发送队列
osMessageQueueId_t MQTTMessageSendQueueHandle;
const osMessageQueueAttr_t MQTTMessageSendQueue_attributes = {
    .name="MQTTMessageSendQueue",
};

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
 void User_Tasks_Init(void)
 {

    /* add queues-------------------------------*/
    MQTTMessageReceiveQueueHandle = osMessageQueueNew(10,sizeof(MQTTMessage_t),&MQTTMessageReceiveQueue_attributes);
    MQTTMessageSendQueueHandle = osMessageQueueNew(10,sizeof(VoiceJson_t),&MQTTMessageSendQueue_attributes);
    /* add threads -----------------------------*/
    MessageReceiveTaskHandle = osThreadNew(MessageReceiveTask, NULL, &MessageReceiveTask_attributes);
    GUIRefreshTaskHandle = osThreadNew(GUIRefreshTask, NULL, &GUIRefreshTask_attributes);
    MessageSendTaskHandle = osThreadNew(MessageSendTask,NULL,&MessageSendTask_attributes);
    
 }