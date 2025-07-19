#include "iwdg_feed_task.h"
#include "iwdg.h"
#include "cmsis_os.h"
void iwdgFeedTask(void *argument)
{
    while (1)
    {
        HAL_IWDG_Refresh(&hiwdg);

        osDelay(6000);
    }
}