#include "user_LCDTestTask.h"
#include "lcd.h"
#include "touch.h"
#include "cmsis_os.h"
#include "dwt_delay.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "sram.h"

void LCDTestTask(void *argument)
{
    sram_init();

    dwt_delay_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    lv_obj_t *switch_obj = lv_switch_create(lv_scr_act());
    lv_obj_set_size(switch_obj, 120, 60);
    lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
    while (1)
    {
        lv_timer_handler();
        osDelay(5);
    }
}