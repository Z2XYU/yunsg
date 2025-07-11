#ifndef __USER_TASKSINIT_H__
#define __USER_TASKSINIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include "cmsis_os.h"

extern osMessageQueueId_t MQTTMessageReceiveQueueHandle;
extern osMessageQueueId_t MQTTMessageSendQueueHandle;

/*电机控制*/
extern osThreadId_t CmdReceiveTaskHandle;
extern osThreadId_t ultrasonicTaskHandle;
extern osSemaphoreId_t distReadySemaphoreHandle;
extern osEventFlagsId_t ultrasonicTaskWaitEventFlag;


void user_tasks_init(void);

#ifdef __cplusplus
}
#endif

#endif