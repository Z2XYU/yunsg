#ifndef __USER_TASKSINIT_H__
#define __USER_TASKSINIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include "cmsis_os.h"

extern osMessageQueueId_t MQTTMessageReceiveQueueHandle;
extern osMessageQueueId_t MQTTMessageSendQueueHandle;

/*看门狗*/
extern osThreadId_t iwdgFeedTaskHandle;

/*GUI*/
extern osThreadId_t GUIRefreshTaskHandle;
extern osMessageQueueId_t PickupCodeMsgSendQueueHandle;
extern osThreadId_t PickupCodeInquiryTaskHandle;

/*电机控制*/
extern osThreadId_t CmdReceiveTaskHandle;
extern osThreadId_t ultrasonicTaskHandle;
extern osThreadId_t rfidDetectionTaskHandle;
extern osSemaphoreId_t distReadySemaphoreHandle;
extern osSemaphoreId_t rfidReadySemaphoreHandle;



/*温度控制*/
extern osThreadId_t temperatureMeasurementTaskHandle;
extern osThreadId_t adcSensorsMeasurementTaskHandle;
extern osThreadId_t onenetMsgUploadTaskHandle;

void user_tasks_init(void);

#ifdef __cplusplus
}
#endif

#endif