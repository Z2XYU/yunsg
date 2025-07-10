#include "user_GUIRefresh.h"
#include "lcd.h"
#include "touch.h"
#include "cmsis_os.h"
#include "dwt_delay.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "gui_app.h"

void GUIRefreshTask(void *argument)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    

    gui_app();
    while (1)
    {
        lv_timer_handler();
        osDelay(5);
    }
}