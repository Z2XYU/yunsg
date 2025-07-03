#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "mqttMessageProcess.h"
#include "user_MessageReceiveTask.h"

#include "user_LCDTestTask.h"

/*Tasks -------------------------------------*/

//MessageReceiveTask
osThreadId_t MessageReceiveTaskHandle;
const osThreadAttr_t  MessageReceiveTask_attributes = {
    .name = " MessageReceiveTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal1,
};

osThreadId_t LCDTestTaskHandle;
const osThreadAttr_t LCDTestTask_attributes={
    .name ="LCDTestTask",
    .stack_size = 128 * 12,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Message queues ----------------------------*/
osMessageQueueId_t MQTTMessageReceiveQueueHandle;
const osMessageQueueAttr_t MQTTMessageReceiveQueue_attributes = {
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
    MQTTMessageReceiveQueueHandle = osMessageQueueNew(10,sizeof(MQTTMessage_t),&MQTTMessageReceiveQueue_attributes);

    /* add threads -----------------------------*/
    MessageReceiveTaskHandle = osThreadNew(MessageReceiveTask, NULL, &MessageReceiveTask_attributes);
    LCDTestTaskHandle = osThreadNew(LCDTestTask, NULL, &LCDTestTask_attributes);
    
 }