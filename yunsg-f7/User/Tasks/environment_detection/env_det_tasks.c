#include "env_det_task.h"
#include "sh40.h"
#include "cmsis_os.h"


SH40_t sh40_snesor;

void SH40Task(void *argument)
{    
    while (1)
    {
        sh40_snesor = sh40_measurement_data();
        osDelay(5000);
    }
}



