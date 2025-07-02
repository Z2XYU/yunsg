#include "user_LCDTestTask.h"
#include "lcd.h"
#include "touch.h"
#include "cmsis_os.h"
#include "dwt_delay.h"

void LCDTestTask(void *argument)
{

    dwt_delay_init();

    lcd_init();
    tp_dev.init();

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "TOUCH TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    while (1)
    {
        osDelay(5);
    }
    
}