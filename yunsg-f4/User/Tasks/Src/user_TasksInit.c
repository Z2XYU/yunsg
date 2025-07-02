#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "mqttMessageProcess.h"
#include "user_MessageReceiveTask.h"

/*Tasks -------------------------------------*/

//MessageReceiveTask
osThreadId_t MessageReceiveTaskHandle;
const osThreadAttr_t  MessageReceiveTask_attributes = {
    .name = " MessageReceiveTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal1,
};

/* Message queues ----------------------------*/
osMessageQueueId_t MQTTMessageQueueHandle;
const osMessageQueueAttr_t MQTTMessageQueue_attributes = {
    .name="MQTTMessageQueue",
};

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
 void User_Tasks_Init(void)
 {

    /* add queues-------------------------------*/
    MQTTMessageQueueHandle = osMessageQueueNew(10,sizeof(MQTTMessage_t),&MQTTMessageQueue_attributes);

    /* add threads -----------------------------*/
    MessageReceiveTaskHandle = osThreadNew(MessageReceiveTask, NULL, &MessageReceiveTask_attributes);


    
 }