#ifndef __USER_TASKSINIT_H__
#define __USER_TASKSINIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include "cmsis_os.h"

extern osMessageQueueId_t MQTTMessageQueueHandle;

void User_Tasks_Init(void);

#ifdef __cplusplus
}
#endif

#endif