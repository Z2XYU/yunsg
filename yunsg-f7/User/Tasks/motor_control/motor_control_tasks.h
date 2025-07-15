#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


void CmdReceiveTask(void *argument);
void UltrasonicTask(void *argument);
void rfidDetectionTask(void *argument);

#ifdef __cplusplus
}
#endif