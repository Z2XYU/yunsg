#include "env_det_task.h"
#include "sh40.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "user_TaskInit.h"

SH40_t sh40_snesor;

void SH40Task(void *argument)
{
    while (1)
    {
        sh40_snesor = sh40_measurement_data();
        printf("tmp:%d℃ hum:%d%%\n",(int)sh40_snesor.temperature,(int)sh40_snesor.humidity);
        osDelay(10000);
    }
}

