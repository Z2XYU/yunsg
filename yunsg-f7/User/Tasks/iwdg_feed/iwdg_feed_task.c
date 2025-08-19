#include "iwdg_feed_task.h"
#include "iwdg.h"
#include "cmsis_os.h"
void iwdgFeedTask(void *argument)
{
    while (1)
    {
        HAL_IWDG_Refresh(&hiwdg);

        //内存泄漏BUG修复
        osDelay(1000);
    }
}